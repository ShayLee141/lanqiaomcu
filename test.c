#include "test.h"

/* led pwm���ԣ�ǰ�ĸ��ƻ��ۼӼ��������ĸ��ƻ���������ƣ����еƾ�����ֺ����Ƶ���Ч */
void led_pwm_test(void)
{
	/* ������鱣������ˮ�Ƶ�ÿһ֡��LED״̬�������ĸ��� */
	uint8_t code led_state_all[6] = 
	{ 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, };
	
	int16_t add = 1; //���ڿ����ۼӵķ���
	uint8_t cnt = 0; //����������ʾ��ǰ�ĸ�����
	uint8_t led_state_cnt = 0; //������ˮ��
		
	while(1)
	{
		if (Timer1s_cnt >= 1000)
		{
			Timer1s_cnt = 0;
			
			cnt++;
			
			if (++led_state_cnt >= 6) //������ˮ��
				led_state_cnt = 0;
		}
		
		if (Timer10ms_cnt >= 10) //10ms����һ��pwmռ�ձȺ�led���״̬
		{
			Timer10ms_cnt = 0;
			
			/* ��ˮ�Ŀ��� */
			led_pwm_duty += add;
			if (led_pwm_duty >= 100)
				add = -1;
			if (led_pwm_duty == 0)
				add = 1;
		
			led_state = (cnt & 0x0F) | led_state_all[led_state_cnt];
		}
	}
}

/* �������� */
void key_test(void)
{
	uint8_t cnt = 127;
	
	while(1)
	{
		/* 10msִ��һ�ΰ���ɨ�� */
		if (Timer10ms_cnt >= 10) //��������޸�����ʱ�䣬����뿴�������ܺ�LEDһ�����������ݣ����������ֵ����50���߸��󣬵���Ҫ�ǵõ�����Ŷ
		{
			Timer10ms_cnt = 0;
			
			/* ����ɨ�� */
//			key_scan_simple();							/* ���ּ򵥰�	�������� */
//			key_pad_scan_simple();					/* ���ּ򵥰� ���󰴼� */
//			key_scan_release();							/* ���ּ��� �������� */
			key_pad_scan_release();					/* ���ּ��� ���󰴼� */
//			key_pad_half_scan_simple();			/* ���ڳ�ͻ����򵥰� ���󰴼� */
//			key_pad_remain_scan_simple();		/* ��������˫˫��ͻ��ȱ�򵥰� ���󰴼� */
			
			/* �жϰ��� */
			/* �޷Ǿ�������key_value��key_state�����������ж� */
			if (key_value == 4) //���s4����
			{
				switch (key_state)
				{
					case 2: //�̰�
						led_state |= 0x02; //�̰���־��һ��������
					break;
					
					case 100: //����1s
						led_state |= 0x04; //����1s��־��һ��������
					break;
					
					case 200: //����2s
						led_state |= 0x08; //����2s��־��һ��������
					break;
					
					case 255: //�̰����֣�һ��������
						led_state |= 0x10; //���ֱ�־
					break;
					
					case 254: //����1s�����֣�һ��������
						led_state |= 0x20; //���ֱ�־
					break;
					
					case 253: //����2s�����֣�һ��������
						led_state |= 0x40; //���ֱ�־
					break;
					
					default:
						led_state &= ~0x7E; //����ʱ�̰�ledϨ��
				}
				
				led_state |= 0x01; //���±�־�����³���������
			}
			else //s4�ɿ�
			{
				led_state &= ~0xFF; //������б�־
				
				if (key_value == 7 && key_state == 2) //s7�̰�
					cnt++;
				if (key_value == 6 && key_state == 2) //s6�̰�
					cnt--;
			}
			
			/* �������ʾ */
			{
				/* ��ֵ��ʾ */
				dig[0] = key_value / 10;
				dig[1] = key_value % 10;
				
				/* ״̬��ʾ */
				dig[3] = key_state / 100;
				dig[4] = key_state / 10 % 10;
				dig[5] = key_state % 10;
				
				/* �ɿر�����ʾ */
				dig[7] = cnt % 10;
			}
		}
	}
}

/* ��ʾ���� */
void display_test(void)
{
	uint8_t screen = 2; //��ǰ����
	uint8_t screen_bak = screen; //���ݵ�ǰ���棬�Ա����·���
	uint8_t operate_num = 0; //���ڴ����ֳ�ʱ��д��ʱ��
	
	dig = dig_all[screen]; //Ĭ��Ϊ����2
	
	rd_temperature();
	//������Լ���ʱ�ӳ�ֵ�����ã�Ҳ��������һ����ds1302.c��ֱ�����ó�ֵ
	write_time(); //ʱ�ӳ�ʼ��
	dac_level = 20;
	write_dac();
	
	while(1)
	{
		/* 10msִ��һ�ΰ���ɨ�� */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			/* ����ɨ�� */
			key_pad_remain_scan_simple();
			
			if (Timer50ms_cnt >= 50)
			{
				Timer50ms_cnt = 0;
				
				switch (operate_num++)
				{
					default:
						operate_num = 0;
					case 1:
						read_time(); //��ȡʱ��
						break;
					
					case 2:
						read_adc(); //��ȡADC
						break;
					
					case 3:
						break;
					
					case 4:
						rd_temperature();
						break;
				}
			}
			
			/* �жϰ��� */
			if (key_value == 4 && key_state > 2) //�������s4�������ڼ�
			{
				if (screen != 1) //����Գ���ע�͵�����ж���俴�����ܻᷢ��ʲô�仯
				{
					screen_bak = screen; //���ݵ�ǰ�����
					screen = 1; //��ʾ����1
				}
			}
			else
			{
				if (screen == 1) //����Գ���ע�͵�����ж���俴�����ܻᷢ��ʲô�仯
				{
					screen = screen_bak; //��ԭ��s4�ı�Ľ���
				}
			}
			
			if (key_value == 5 && key_state == 100) //���s5����1��
			{
				if (screen) //�ر�
				{
					screen_bak = screen; //���ݵ�ǰ�����
					screen = 0; //�ر���ʾ���������棩
					stop_dac(); //ֹͣDAC���
				}
				else //����
				{
					screen = screen_bak; //��ԭ��ʾ
					write_dac(); //����DAC���
				}
			}
			else if (key_value == 8 && key_state == 2) //���s8�̰�
			{
				if (--screen < 2)
					screen = SCREEN_NUM - 1;
			}
			else if (key_value == 9 && key_state == 2) //���s9�̰�
			{
				if (++screen > SCREEN_NUM - 1)
					screen = 2;
			}
			
			/* ��ʾ��� */
			dig = dig_all[screen]; //��ʾ����
			
			/* ����1 ��ʾ��ǰ�¶� */
			
			/* ����2 ��ʾ��ǰʱ�� */
			dig_all[2][0] = time_dec[0] / 10;
			dig_all[2][1] = time_dec[0] % 10;
			dig_all[2][3] = time_dec[1] / 10;
			dig_all[2][4] = time_dec[1] % 10;
			dig_all[2][6] = time_dec[2] / 10;
			dig_all[2][7] = time_dec[2] % 10;
						
			/* ����3 ��ʾ��ǰ���� */
			dig_all[3][0] = 2;
			dig_all[3][1] = 0;
			dig_all[3][2] = time_dec[5] / 10;
			dig_all[3][3] = time_dec[5] % 10;
			dig_all[3][4] = time_dec[3] / 10;
			dig_all[3][5] = time_dec[3] % 10;
			dig_all[3][6] = time_dec[4] / 10;
			dig_all[3][7] = time_dec[4] % 10;
			
			/* ����4 ��ʾ��ǰne555���Ƶ�� */
			dig_all[4][3] = ne555_out / 10000 % 10;
			dig_all[4][4] = ne555_out / 1000 % 10;
			dig_all[4][5] = ne555_out / 100 % 10;
			dig_all[4][6] = ne555_out / 10 % 10;
			dig_all[4][7] = ne555_out % 10;
			
			/* ����5 ��ʾADC������DAC����ȼ� */
			dig_all[5][0] = adc_level[1] / 0x10;
			dig_all[5][1] = adc_level[1] % 0x10;
			dig_all[5][3] = adc_level[3] / 0x10;
			dig_all[5][4] = adc_level[3] % 0x10;
			dig_all[5][6] = dac_level / 0x10;
			dig_all[5][7] = dac_level % 0x10;
		}
	}
}

/* ���������� */
void ultrasonic_test(void)
{
	unsigned int distance = 0xFFFF;
	
	while(1)
	{
		if (Timer100ms_cnt >= 100)
		{
			Timer100ms_cnt = 0;
			
			/* ��� */
			distance = read_distance();
			
			/* ���Ƽ̵��� */
			if (distance < 30)
				high_power_state |= 0x10;
			else
				high_power_state &= ~0x10;
			
			/* ������ʾ */
			dig[0] = distance / 10000;
			dig[1] = distance / 1000 % 10;
			dig[2] = distance / 100 % 10;
			dig[3] = distance / 10 % 10;
			dig[4] = distance % 10;
		}
	}
}

/* e2prom���� */
void e2prom_test(void)
{
	uint16_t number;
	
	read_e2prom(0x14, (uint8_t*)&number, 2); //������˸���ַ���ԣ�uint16_t�������ֽ�
	
	while(1)
	{
		/* 10msִ��һ�ΰ���ɨ�� */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			key_pad_remain_scan_simple();
			
			if (key_value == 5 && key_state == 2) //������ֵ
			{
				number++;
			}
			else if (key_value == 4 && key_state == 2) //������ֵ
			{
				number--;
			}
			else if (key_value == 9 && key_state == 2) //������ֵ�������ϵ���ܿ���Ч��
			{
				write_e2prom(0x14, (uint8_t*)&number, 2);
				
				led_state = 0x01;
				Timer100ms_cnt = 0;
			}
			
			if (led_state && Timer100ms_cnt >= 100)
			{
				led_state = 0x00;
			}
			
			dig[0] = number / 10000 % 10;
			dig[1] = number / 1000 % 10;
			dig[2] = number / 100 % 10;
			dig[3] = number / 10 % 10;
			dig[4] = number % 10;
		}
	}
}

/* �¶Ȳ��� */
void temperature_test(void)
{
	while(1)
	{
		if (Timer100ms_cnt >= 100)
		{
			Timer100ms_cnt = 0;
			
			rd_temperature();
			
			if (temperature < 0) //���¶�
			{
				dig[0] = 17;
				dig[1] = (-temperature) / 10000 % 10;
				dig[2] = (-temperature) / 1000 % 10;
				dig[3] = (-temperature) / 100 % 10 + 32;
				dig[4] = (-temperature) / 10 % 10;
				dig[5] = (-temperature) % 10;
			}
			else //�Ǹ��¶�
			{
				dig[0] = 16;
				dig[1] = temperature / 10000 % 10;
				dig[2] = temperature / 1000 % 10;
				dig[3] = temperature / 100 % 10 + 32;
				dig[4] = temperature / 10 % 10;
				dig[5] = temperature % 10;
			}
		}
	}
}
