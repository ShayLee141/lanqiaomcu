#include "base.h"

/* NE555频率计数器 */
void Timer0Init(void)
{
	TMOD &= 0xF0;	//定时器0模式复位
	TMOD |= 0x04;	//设置为计数模式，计数引脚为P34
	TL0 = 0x00;		//复位初始值
	TH0 = 0x00;		//复位初始值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
}

/* ledpwm、数码管、继电器、蜂鸣器，以及各种周期延时均使用这个定时器 */
void Timer1Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x50;		//设置定时初始值
	TH1 = 0xFB;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}

/*************  本地常量声明    **************/
uint8_t code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
//	 0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};

uint8_t code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

uint8_t xdata dig_com = 0; //当前显示的数码管位
uint8_t xdata dig_all[SCREEN_NUM][8] = 
{
	{ 16, 16, 16, 16, 16, 16, 16, 16, },	//黑屏界面
	{ 25, 1, 16, 16, 16, 16, 16, 16, },	//界面1
	{ 25, 2, 16, 16, 16, 16, 16, 16, },	//界面2
	{ 25, 3, 16, 16, 16, 16, 16, 16, },	//界面3
	{ 25, 4, 16, 16, 16, 16, 16, 16, },	//界面4
	{ 25, 5, 16, 16, 16, 16, 16, 16, },	//界面5
	{ 25, 6, 16, 16, 16, 16, 16, 16, },	//界面6
};
uint8_t xdata *dig = dig_all[0]; //当前显示界面
uint8_t xdata dig_switch = 0xFF; //数码管每一位的开关

uint8_t xdata led_state = 0; //每个led的状态
uint8_t xdata led_pwm_cnt = 0;	//led的pwm计数器
uint8_t xdata led_pwm_duty = 10; //led的pwm占空比，默认满占空比，占空比不能超过周期
uint8_t xdata led_pwm_period = 10; //led的pwm周期（单位就是定时器的周期，这里是100us）

/* 大功率外设状态 */
uint8_t xdata high_power_state = 0;

/* 虚拟定时器 */
uint8_t xdata Timer10ms_cnt = 0;
uint8_t xdata Timer100ms_cnt = 0;
uint16_t xdata Timer500ms_cnt = 0;
uint16_t xdata Timer1s_cnt = 0;
uint16_t xdata operate_timer_cnt = 0; //操作周期定时器，用来错开每个时序操作防止时序混乱，需要根据操作的芯片数量来决定定时器周期

/* NE555频率 */
uint16_t xdata ne555_out = 0;

void Timer1Isr(void) interrupt 3 //100us一周期
{
	static uint8_t Timer1ms_cnt = 10;
	static uint16_t ne555_cnt = 100; //ne555专用的定时器，可以调整采样周期

	{ //LED_PWM
		if (led_pwm_cnt <= led_pwm_duty) //有效电平
		{
			CHANNLE(4, ~led_state);
		}
		else //无效电平
		{
			CHANNLE(4, 0xFF); //关闭LED
		}
		if (++led_pwm_cnt >= led_pwm_period)
			led_pwm_cnt = 0;
	}
	
	if (++Timer1ms_cnt >= 10) //1ms
	{
		Timer1ms_cnt = 0;
		
		Timer10ms_cnt++;
		operate_timer_cnt++;
		Timer100ms_cnt++;
		Timer500ms_cnt++;
		Timer1s_cnt++;
		
		{ //数码管
			CHANNLE(6, 0x00);
			CHANNLE(7, ~t_display[dig[dig_com]]); //关键步骤
			CHANNLE(6, T_COM[dig_com] & dig_switch); //关键步骤 根据这一位数码管开关来决定是否显示
			if (++dig_com >= 8)
				dig_com = 0;
		}

		if (++ne555_cnt >= 100)
		{
			ne555_cnt = 0;
			
			TR0 = 0;		//定时器0暂停计数
			ne555_out = TH0 << 8 | TL0;
			TL0 = 0x00;	//复位初始值
			TH0 = 0x00;	//复位初始值
			TR0 = 1;		//定时器0开始计数
//			ne555_out *= 10;
		}
		
		//大功率外设
		CHANNLE(5, high_power_state);
	}
}
