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
void Timer1Init(void)		//10΢��@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x88;		//���ö�ʱ��ʼֵ
	TH1 = 0xFF;		//���ö�ʱ��ʼֵ
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

uint8_t dig_com = 0; //��ǰ��ʾ�������λ
uint8_t xdata dig_all[SCREEN_NUM][8] = 
{
	{ 16, 16, 16, 16, 16, 16, 16, 16, },	//��������
	{ 0x0C, 16, 16, 16, 16, 16, 2, 6, },	//�¶Ƚ���
	{ 16, 16, 17, 16, 16, 17, 16, 16, },	//ʱ�ӽ���
	{ 16, 16, 16, 16, 16, 16, 16, 16, },	//���ڽ���
	{ 0x0F, 16, 16, 16, 16, 16, 16, 16, },	//Ƶ�ʽ���
	{ 16, 16, 16, 16, 16, 16, 16, 16, },	//AD/DA����
}; //��Щ��ֵ���ǿ��Ը��ĵ�
uint8_t *dig = dig_all[0]; //��ǰ��ʾ����
uint8_t dig_switch = 0xFF; //�����ÿһλ�Ŀ���

uint8_t led_state = 0; //ÿ��led��״̬
uint8_t led_pwm_cnt = 0;	//led��pwm������
uint8_t led_pwm_duty = 100; //led��pwmռ�ձȣ�Ĭ����ռ�ձ�
uint8_t led_pwm_period = 100; //led��pwm���ڣ���λ���Ƕ�ʱ�������ڣ�������10us��

/* ��������״̬ */
uint8_t high_power_state = 0;

/* ���ⶨʱ�� */
uint8_t Timer10ms_cnt = 10;
uint8_t Timer50ms_cnt = 50;
uint8_t Timer100ms_cnt = 100;
uint8_t Timer200ms_cnt = 200;
uint16_t Timer1s_cnt = 1000;
uint16_t ne555_cnt = 100; //ne555ר�õĶ�ʱ�������Ե�����������

/* NE555Ƶ�� */
uint16_t ne555_out = 0;

void Timer1Isr(void) interrupt 3 //10usһ����
{
	static uint8_t Timer1ms_cnt = 100;
	
	if (++Timer1ms_cnt >= 100) //1ms
	{
		Timer1ms_cnt = 0;
		
		Timer10ms_cnt++;
		Timer50ms_cnt++;
		Timer100ms_cnt++;
		Timer200ms_cnt++;
		Timer1s_cnt++;
		ne555_cnt++;
		
		{ //�����
			CHANNLE(6, 0x00);
			if (dig_switch & (1 << dig_com)) //������һλ����ܿ����������Ƿ���ʾ
			{
				CHANNLE(7, ~t_display[dig[dig_com]]); //�ؼ�����
				CHANNLE(6, T_COM[dig_com]); //�ؼ�����
			}
			
			if (++dig_com >= 8)
			{
				dig_com = 0;
			}
		}
		
		//��������
		CHANNLE(5, high_power_state);
	}
	
	{ //LED_PWM
		if (led_pwm_cnt < led_pwm_duty) //��Ч��ƽ
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
	
	if (ne555_cnt >= 100)
	{
		ne555_cnt = 0;
		
		TR0 = 0;		//��ʱ��0��ͣ����
		ne555_out = TH0;
		ne555_out <<= 8;
		ne555_out |= TL0;
		TL0 = 0x00;	//��λ��ʼֵ
		TH0 = 0x00;	//��λ��ʼֵ
		TR0 = 1;		//��ʱ��0��ʼ����
		ne555_out *= 10;
	}
}
