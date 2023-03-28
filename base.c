#include "base.h"

/* NE555Ƶ�ʼ����� */
void Timer0Init(void)
{
	TMOD &= 0xF0;	//��ʱ��0ģʽ��λ
	TMOD |= 0x04;	//����Ϊ����ģʽ����������ΪP34
	TL0 = 0x00;		//��λ��ʼֵ
	TH0 = 0x00;		//��λ��ʼֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
}

/* ledpwm������ܡ��̵��������������Լ�����������ʱ��ʹ�������ʱ�� */
void Timer1Init(void)		//100΢��@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x50;		//���ö�ʱ��ʼֵ
	TH1 = 0xFB;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

/*************  ���س�������    **************/
uint8_t code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
//	 0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};

uint8_t code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��

uint8_t xdata dig_com = 0; //��ǰ��ʾ�������λ
uint8_t xdata dig_all[SCREEN_NUM][8] = 
{
	{ 16, 16, 16, 16, 16, 16, 16, 16, },	//��������
	{ 25, 1, 16, 16, 16, 16, 16, 16, },	//����1
	{ 25, 2, 16, 16, 16, 16, 16, 16, },	//����2
	{ 25, 3, 16, 16, 16, 16, 16, 16, },	//����3
	{ 25, 4, 16, 16, 16, 16, 16, 16, },	//����4
	{ 25, 5, 16, 16, 16, 16, 16, 16, },	//����5
	{ 25, 6, 16, 16, 16, 16, 16, 16, },	//����6
};
uint8_t xdata *dig = dig_all[0]; //��ǰ��ʾ����
uint8_t xdata dig_switch = 0xFF; //�����ÿһλ�Ŀ���

uint8_t xdata led_state = 0; //ÿ��led��״̬
uint8_t xdata led_pwm_cnt = 0;	//led��pwm������
uint8_t xdata led_pwm_duty = 10; //led��pwmռ�ձȣ�Ĭ����ռ�ձȣ�ռ�ձȲ��ܳ�������
uint8_t xdata led_pwm_period = 10; //led��pwm���ڣ���λ���Ƕ�ʱ�������ڣ�������100us��

/* ��������״̬ */
uint8_t xdata high_power_state = 0;

/* ���ⶨʱ�� */
uint8_t xdata Timer10ms_cnt = 0;
uint8_t xdata Timer100ms_cnt = 0;
uint16_t xdata Timer500ms_cnt = 0;
uint16_t xdata Timer1s_cnt = 0;
uint16_t xdata operate_timer_cnt = 0; //�������ڶ�ʱ����������ÿ��ʱ�������ֹʱ����ң���Ҫ���ݲ�����оƬ������������ʱ������

/* NE555Ƶ�� */
uint16_t xdata ne555_out = 0;

void Timer1Isr(void) interrupt 3 //100usһ����
{
	static uint8_t Timer1ms_cnt = 10;
	static uint16_t ne555_cnt = 100; //ne555ר�õĶ�ʱ�������Ե�����������

	{ //LED_PWM
		if (led_pwm_cnt <= led_pwm_duty) //��Ч��ƽ
		{
			CHANNLE(4, ~led_state);
		}
		else //��Ч��ƽ
		{
			CHANNLE(4, 0xFF); //�ر�LED
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
		
		{ //�����
			CHANNLE(6, 0x00);
			CHANNLE(7, ~t_display[dig[dig_com]]); //�ؼ�����
			CHANNLE(6, T_COM[dig_com] & dig_switch); //�ؼ����� ������һλ����ܿ����������Ƿ���ʾ
			if (++dig_com >= 8)
				dig_com = 0;
		}

		if (++ne555_cnt >= 100)
		{
			ne555_cnt = 0;
			
			TR0 = 0;		//��ʱ��0��ͣ����
			ne555_out = TH0 << 8 | TL0;
			TL0 = 0x00;	//��λ��ʼֵ
			TH0 = 0x00;	//��λ��ʼֵ
			TR0 = 1;		//��ʱ��0��ʼ����
//			ne555_out *= 10;
		}
		
		//��������
		CHANNLE(5, high_power_state);
	}
}
