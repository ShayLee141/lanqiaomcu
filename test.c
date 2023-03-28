#include "test.h"

void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void Delay100ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 195;
	j = 138;
	do
	{
		while (--j);
	} while (--i);
}

#if LED_PWM_TEST == 1
/* led pwm���ԣ�ǰ�ĸ��ƻ��ۼӼ��������ĸ��ƻ���������ƣ����еƾ�����ֺ����Ƶ���Ч */
void test(void)
{
	/* ������鱣������ˮ�Ƶ�ÿһ֡��LED״̬�������ĸ��� */
	uint8_t code led_state_all[6] = 
	{ 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, };
	
	int16_t add = 1; //���ڿ����ۼӵķ���
	uint8_t cnt = 0; //����������ʾ��ǰ�ĸ�����
	uint8_t led_state_cnt = 0; //������ˮ��
	
	dig = dig_all[1]; //����ܳ�ʼ��������Ϊ����1
	
	while(1)
	{
		if (Timer500ms_cnt >= 500)
		{
			Timer500ms_cnt = 0;
			
			cnt++;
			
			if (++led_state_cnt >= 6) //������ˮ��
				led_state_cnt = 0;
		}
		
		if (Timer100ms_cnt >= 100) //10ms����һ��pwmռ�ձȺ�led���״̬
		{
			Timer100ms_cnt = 0;
			
			/* ledpwmռ�ձȵĿ��� */
			led_pwm_duty += add;
			if (led_pwm_duty >= 10)
				add = -1;
			if (led_pwm_duty <= 1)
				add = 1;
		
			led_state = (cnt & 0x0F) | led_state_all[led_state_cnt];
		}
		
		dig_all[1][0] = cnt / 0x10;
		dig_all[1][1] = cnt % 0x10;
	}
}

#elif KEY_TEST == 1
/* �������� */
void test(void)
{
	uint8_t cnt = 127;
	
	dig = dig_all[2];
	
	while(1)
	{
		/* 10msִ��һ�ΰ���ɨ�� */
		if (Timer10ms_cnt >= 10) //��������޸�����ʱ�䣬����뿴�������ܺ�LEDһ�����������ݣ����������ֵ����50���߸��󣬵���Ҫ�ǵõ�����Ŷ
		{
			Timer10ms_cnt = 0;
			
			/* ����ɨ�� */
			key_pad_scan();

			/* �жϰ��� */
			{ /* �޷Ǿ�������key_value��key_state�����������ж� */
				if (key_value == 5 && key_state == 2) //s5�̰�
					led_state |= 0x01;
				
				else if (key_value == 5 && key_state == 100) //s5����1��
					led_state |= 0x02;
				
				else if (key_value == 5 && key_state == 200) //s5����2��
					led_state |= 0x04;
				
				else if (key_value == 5 && key_state > 250) //s5����
					led_state |= 0x08;
				
				else if (key_value == 9 && key_state >= 2) //s9��סʱ
				{
					led_state |= 0x10;
					
					if (key_value == 9 && key_state == 255) //s9���²���1���ɿ�
						led_state |= 0x20;
					
					else if (key_value == 9 && key_state == 254) //s9����1����2�����ɿ�
						led_state |= 0x40;
					
					else if (key_value == 9 && key_state == 253) //s9����2���������ɿ�
						led_state |= 0x80;
				}
				
				else if (key_value == 13 && key_state == 2) //s13�̰������Ʊ�������
					cnt++;
				
				else if (key_value == 12 && key_state == 2) //s12�̰������Ʊ�������
					cnt--;
				
				else //����֮�⣬Ϩ��LED
					led_state = 0x00;
			}

			/* �������ʾ */
			{
				/* ��ֵ��ʾ */
				dig_all[2][0] = key_value / 10;
				dig_all[2][1] = key_value % 10;
				
				/* ״̬��ʾ */
				dig_all[2][3] = key_state / 100;
				dig_all[2][4] = key_state / 10 % 10;
				dig_all[2][5] = key_state % 10;
				
				/* ������ʾ */
				dig_all[2][7] = cnt % 10;
			}
		}
	}
}

#elif DISPLAY_TEST == 1
/* ��ʾ���� */
void test(void)
{
	uint8_t screen = 1; //��ǰ����
	uint8_t screen_bak = screen; //���ݵ�ǰ���棬�Ա����·���
	bit screen_hold = 0; //��ʾ����5�ı�־λ
	bit screen_switch = 1; //��ʾ����
	
	dig = dig_all[screen]; //Ĭ��Ϊ����1
	
	while(1)
	{
		/* 10msִ��һ�ΰ���ɨ�� */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			/* ����ɨ�� */
			key_pad_scan();
			
			/* �жϰ��� */
			if (key_value == 9 && key_state >= 2) //��סs9ʱ
			{
				if (!screen_hold && screen_switch) //��ϸ���һ�����if�������Ǿ�if
				{
					screen_hold = 1;
					screen_bak = screen;
					screen = 5;
				}
			}
			else
			{
				if (screen_hold) //��ϸ���һ�����if�������Ǿ�if
				{
					screen_hold = 0;
					screen = screen_bak;
				}
				
				if (key_value == 13 && key_state == 255) //��s13����ʱ
				{
					if (screen < 4 && screen_switch) //�����ڽ���1������4֮��
						screen++;
				}
				else if (key_value == 12 && key_state == 255) //��s12����ʱ
				{
					if (screen > 1 && screen_switch)
						screen--;
				}
				else if (key_value == 13 && key_state == 100) //����1��s13ʱ
				{
					if (screen_switch) //�����ر���ʾ
					{
						screen_bak = screen;
						screen = 0;
						screen_switch = 0;
					}
					else //��������ʾ
					{
						screen = screen_bak;
						screen_switch = 1;
					}
				}
			}
			
			/* ��ʾ��� */
			dig = dig_all[screen]; //��ʾ����
		}
	}
}

#elif ULTRASONIC_TEST == 1
/* ���������� */
void test(void)
{
	uint16_t distance_buf[3] = { 2, 2, 2 }; //������������3�β��������ֵ��Ҫ��0
	uint16_t distance_out = 0; //��ž�����ֵ�˲���Ľ��
	uint8_t p = 0;
	
	while(1)
	{
		if (operate_timer_cnt >= 50)
		{
			operate_timer_cnt = 0;
			
			/* ��� */
			read_distance();
			distance_buf[p] = distance;
			if (++p >= 3)
				P = 0;
			distance_out = median_filter(distance_buf); //ʹ���˲��ķ�������ʹ��Ҳ�е�
			
			/* ���Ƽ̵��� */;
			if (distance < 30)
				high_power_state |= 0x10;
			else
				high_power_state &= ~0x10;
			
			/* ������ʾ */
			dig[0] = distance_out / 10000;
			dig[1] = distance_out / 1000 % 10;
			dig[2] = distance_out / 100 % 10;
			dig[3] = distance_out / 10 % 10;
			dig[4] = distance_out % 10;
		}
	}
}

#elif RTC_TEST == 1
/* ʵʱʱ�Ӳ��� */
void test(void)
{
	uint8_t write_ds1302_flag = 0;
	write_ds1302();
	
	dig = dig_all[2]; //�ĸ����涼һ��
	
	dig_all[2][2] = 17; //�ָ�����-��
	dig_all[2][5] = 17; //�ָ�����-��
	
	loop: //����չʾ����һ��ѭ����д������while(1)��һ����

	if (Timer10ms_cnt)
	{
		Timer10ms_cnt = 0;
		
		key_pad_scan();
		
		if (key_value == 9 && key_state == 2) //s9��ͣ������ʱ��
		{
			write_ds1302_flag++;
		}
	}
	
	if (operate_timer_cnt > 50)
	{
		operate_timer_cnt = 0;
		
		if (write_ds1302_flag >= 2)
		{
			write_ds1302();
			write_ds1302_flag = 0;
		}
		else if (write_ds1302_flag == 0)
			read_ds1302();
		else //���write_ds1302_flag == 1����ʾʱ����ͣ
			;
	}
	
	dig_all[2][0] = time_10[0] / 10;
	dig_all[2][1] = time_10[0] % 10;
	
	dig_all[2][3] = time_10[1] / 10;
	dig_all[2][4] = time_10[1] % 10;
	
	dig_all[2][6] = time_10[2] / 10;
	dig_all[2][7] = time_10[2] % 10;
	
	if (write_ds1302_flag) //ʱͣ��ʱ�����led
		led_state |= 0x80;
	else
		led_state &= ~0x80;
	
	goto loop; //�����ܵ��������ת�������loop��־���¿�ʼ����
}

#elif E2PROM_TEST == 1
/* e2prom���� */
void test(void) δ�깤
{
	uint8_t Data_e[16] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, };
	uint8_t Data0[16] = { 2, 0, 2, 3, 0, 3, 2, 6, 1, 2, 3, 4, 5, 6, 7, 8, };
	uint8_t Data1[18] = { 0, };
	uint8_t operate = 0;
	uint8_t addr = 15;
	uint8_t addr_flag = 0;
	
	while(1);
	{
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			key_pad_scan();
			
			if (operate_timer_cnt >= 100)
			{
				operate_timer_cnt = 0;
				
				switch (operate++)
				{
					default:
						operate = 1;
					case 0:
						e2prom_read(addr, Data1, 8);
						break;
					
					case 1:
						break;
					
					case 10:
						e2prom_write(addr, Data_e, 8);
						break;
					
					case 11:
						e2prom_write(addr + 8, Data_e + 8, 8);
						break;
				}
			}
			
			if (key_value == 9 && key_state == 2)
			{
				operate = 10;
			}
			else if (key_value == 13 && key_state == 2)
			{
				if (addr_flag == 0)
					addr += 8;
				else
					addr -= 8;
				addr_flag = !addr_flag;
			}
			
			dig[0] = Data1[0];
			dig[1] = Data1[1];
			dig[2] = Data1[2];
			dig[3] = Data1[3];
			dig[4] = Data1[4];
			dig[5] = Data1[5];
			dig[6] = Data1[6];
			dig[7] = Data1[7];
		}
	}
}

#elif ADC_DAC_TEST == 1
/* adc_dac���� */
void test(void)
{
	uint8_t operate_flag = 0;
	uint8_t channel = 0;
	
	dig[0] = 0x0C;
	
	dac_level = 127;
	dac_out();
	Delay5ms();
	
	loop:
	if (Timer10ms_cnt >= 10)
	{
		Timer10ms_cnt = 0;
		
		key_pad_scan();
		
		if (operate_timer_cnt >= 20)
		{
			operate_timer_cnt = 0;
			
			switch (operate_flag++)
			{
				default:
					operate_flag = 1;
				case 0:
					adc_read();
					break;
				
				case 10:
					dac_out();
					break;
				
				case 12:
					dac_stop();
					break;
			}
		}
		
		if (key_value == 9 && key_state == 2)
		{
			if (control_byte & 0x40)
				operate_flag = 12;
			else
				operate_flag = 10;
		}
		else if (key_value == 13 && key_state == 2)
		{
			if (++channel >= 4)
				channel = 0;
		}
		
		dig[1] = channel;
		dig[3] = adc_level[channel] / 0x10;
		dig[4] = adc_level[channel] % 0x10;
		if (control_byte & 0x40)
		{
			dig[6] = dac_level / 0x10;
			dig[7] = dac_level % 0x10;
			led_state |= 0x80;
		}
		else
		{
			dig[6] = 17;
			dig[7] = 17;
			led_state &= ~0x80;
		}
	}
	goto loop;
}

#elif TEMP_TEST == 1
/* �¶Ȳ��� */
void test(void)
{
	read_temp();
	Delay100ms();
	
	while (1)
	{
		if (operate_timer_cnt >= 20)
		{
			operate_timer_cnt = 0;
			
			read_temp();
		}
		
		dig[0] = temperature / 1000;
		dig[1] = temperature / 100 % 10 + 32;
		dig[2] = temperature / 10 % 10;
		dig[3] = temperature % 10;
	}
}

#elif CHIPS_TEST == 1
/* ����оƬ�ۺϲ��� */
void test(void)
{
	uint8_t mode = 1;
	uint8_t operate_flag = 0;
	
	read_temp();
	write_ds1302();
	dac_out();
	Delay5ms();
	adc_read();
	
	dig_all[1][2] = dig_all[1][5] = 17; //�ָ�����-��
	
	loop:
	
	if (Timer10ms_cnt >= 10)
	{
		Timer10ms_cnt = 0;
		
		/* ����ɨ�� */
		key_pad_scan();
		
		/* ����ʱ����� */
		if (operate_timer_cnt >= 20)
		{
			operate_timer_cnt = 0;
			
			switch (operate_flag++)
			{
				default:
					operate_flag = 1;
				case 0:
					read_ds1302();
					break;
				case 1:
					adc_read();
					break;
				case 2:
					read_temp();
					break;
				case 3:
					read_distance();
					break;
			}
		}
		
		/* �������� */
		if (key_value == 13 && key_state == 2)
		{
			if (++mode >= SCREEN_NUM)
				mode = 1;
		}
		else if (key_value == 12 && key_state == 2)
		{
			if (--mode <= 1)
				mode = SCREEN_NUM - 1;
		}
		
		/* led��ʾ��ǰģʽ */
		led_state = 0x01 << (mode - 1);
		
		/* ����ܽ���ѡ�� */
		dig = dig_all[mode];
		
		/* ����1 ʱ����ʾ */
		dig_all[1][0] = time_10[0] / 10;
		dig_all[1][1] = time_10[0] % 10;
		
		dig_all[1][3] = time_10[1] / 10;
		dig_all[1][4] = time_10[1] % 10;
		
		dig_all[1][6] = time_10[2] / 10;
		dig_all[1][7] = time_10[2] % 10;
		
		/* ����2 ADCͨ��0��1 */
		dig_all[2][0] = adc_level[0] / 100;
		dig_all[2][1] = adc_level[0] / 10 % 10;
		dig_all[2][2] = adc_level[0] % 10;
		
		dig_all[2][5] = adc_level[1] / 100;
		dig_all[2][6] = adc_level[1] / 10 % 10;
		dig_all[2][7] = adc_level[1] % 10;
		
		/* ����3 ADCͨ��2��3 */
		dig_all[3][0] = adc_level[2] / 100;
		dig_all[3][1] = adc_level[2] / 10 % 10;
		dig_all[3][2] = adc_level[2] % 10;
		
		dig_all[3][5] = adc_level[3] / 100;
		dig_all[3][6] = adc_level[3] / 10 % 10;
		dig_all[3][7] = adc_level[3] % 10;
		
		/* ����4 DAC����������� */
		dig_all[4][0] = dac_level / 100;
		dig_all[4][1] = dac_level / 10 % 10;
		dig_all[4][2] = dac_level % 10;
		
		dig_all[4][5] = distance / 100;
		dig_all[4][6] = distance / 10 % 10;
		dig_all[4][7] = distance % 10;
		
		/* ����5 ne555Ƶ�� */
		dig_all[5][3] = ne555_out / 1000;
		dig_all[5][4] = ne555_out / 100 % 10;
		dig_all[5][5] = ne555_out / 10 % 10;
		dig_all[5][6] = ne555_out % 10;
		dig_all[5][7] = 0;
		
		/* ����6 �¶���ʾ */
		dig_all[6][4] = temperature / 10000;
		dig_all[6][5] = temperature / 1000 % 10;
		dig_all[6][6] = temperature / 100 % 10 + 32;
		dig_all[6][7] = temperature % 10;
	}
	
	goto loop;
}

#else
void test(void) //���ֻ��Ϊ�˷�ֹ����
{
	;
}
#endif