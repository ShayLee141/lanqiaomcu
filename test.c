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
/* led pwm测试，前四个灯会累加计数，后四个灯会运行跑马灯，所有灯均会呈现呼吸灯的特效 */
void test(void)
{
	/* 这个数组保存了流水灯的每一帧的LED状态，仅后四个灯 */
	uint8_t code led_state_all[6] = 
	{ 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, };
	
	int16_t add = 1; //用于控制累加的方向
	uint8_t cnt = 0; //计数，并显示在前四个灯上
	uint8_t led_state_cnt = 0; //用于流水灯
	
	dig = dig_all[1]; //数码管初始界面设置为界面1
	
	while(1)
	{
		if (Timer500ms_cnt >= 500)
		{
			Timer500ms_cnt = 0;
			
			cnt++;
			
			if (++led_state_cnt >= 6) //用于流水灯
				led_state_cnt = 0;
		}
		
		if (Timer100ms_cnt >= 100) //10ms更新一次pwm占空比和led输出状态
		{
			Timer100ms_cnt = 0;
			
			/* ledpwm占空比的控制 */
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
/* 按键测试 */
void test(void)
{
	uint8_t cnt = 127;
	
	dig = dig_all[2];
	
	while(1)
	{
		/* 10ms执行一次按键扫描 */
		if (Timer10ms_cnt >= 10) //这里可以修改消抖时间，如果想看清楚数码管和LED一闪而过的内容，不妨把这个值增大到50或者更大，但是要记得调回来哦
		{
			Timer10ms_cnt = 0;
			
			/* 按键扫描 */
			key_pad_scan();

			/* 判断按键 */
			{ /* 无非就是利用key_value和key_state两个变量来判断 */
				if (key_value == 5 && key_state == 2) //s5短按
					led_state |= 0x01;
				
				else if (key_value == 5 && key_state == 100) //s5长按1秒
					led_state |= 0x02;
				
				else if (key_value == 5 && key_state == 200) //s5长按2秒
					led_state |= 0x04;
				
				else if (key_value == 5 && key_state > 250) //s5松手
					led_state |= 0x08;
				
				else if (key_value == 9 && key_state >= 2) //s9按住时
				{
					led_state |= 0x10;
					
					if (key_value == 9 && key_state == 255) //s9按下不到1秒松开
						led_state |= 0x20;
					
					else if (key_value == 9 && key_state == 254) //s9按下1秒至2秒内松开
						led_state |= 0x40;
					
					else if (key_value == 9 && key_state == 253) //s9按下2秒以上再松开
						led_state |= 0x80;
				}
				
				else if (key_value == 13 && key_state == 2) //s13短按，控制变量增减
					cnt++;
				
				else if (key_value == 12 && key_state == 2) //s12短按，控制变量增减
					cnt--;
				
				else //除此之外，熄灭LED
					led_state = 0x00;
			}

			/* 数码管显示 */
			{
				/* 键值显示 */
				dig_all[2][0] = key_value / 10;
				dig_all[2][1] = key_value % 10;
				
				/* 状态显示 */
				dig_all[2][3] = key_state / 100;
				dig_all[2][4] = key_state / 10 % 10;
				dig_all[2][5] = key_state % 10;
				
				/* 变量显示 */
				dig_all[2][7] = cnt % 10;
			}
		}
	}
}

#elif DISPLAY_TEST == 1
/* 显示测试 */
void test(void)
{
	uint8_t screen = 1; //当前界面
	uint8_t screen_bak = screen; //备份当前界面，以便重新返回
	bit screen_hold = 0; //显示界面5的标志位
	bit screen_switch = 1; //显示开关
	
	dig = dig_all[screen]; //默认为界面1
	
	while(1)
	{
		/* 10ms执行一次按键扫描 */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			/* 按键扫描 */
			key_pad_scan();
			
			/* 判断按键 */
			if (key_value == 9 && key_state >= 2) //按住s9时
			{
				if (!screen_hold && screen_switch) //仔细体会一下这句if和下面那句if
				{
					screen_hold = 1;
					screen_bak = screen;
					screen = 5;
				}
			}
			else
			{
				if (screen_hold) //仔细体会一下这句if和上面那句if
				{
					screen_hold = 0;
					screen = screen_bak;
				}
				
				if (key_value == 13 && key_state == 255) //按s13松手时
				{
					if (screen < 4 && screen_switch) //限制在界面1到界面4之间
						screen++;
				}
				else if (key_value == 12 && key_state == 255) //按s12松手时
				{
					if (screen > 1 && screen_switch)
						screen--;
				}
				else if (key_value == 13 && key_state == 100) //长按1秒s13时
				{
					if (screen_switch) //即将关闭显示
					{
						screen_bak = screen;
						screen = 0;
						screen_switch = 0;
					}
					else //即将打开显示
					{
						screen = screen_bak;
						screen_switch = 1;
					}
				}
			}
			
			/* 显示输出 */
			dig = dig_all[screen]; //显示界面
		}
	}
}

#elif ULTRASONIC_TEST == 1
/* 超声波测试 */
void test(void)
{
	uint16_t distance_buf[3] = { 2, 2, 2 }; //用来存放最近的3次测距结果，初值不要给0
	uint16_t distance_out = 0; //存放经过均值滤波后的结果
	uint8_t p = 0;
	
	while(1)
	{
		if (operate_timer_cnt >= 100)
		{
			operate_timer_cnt = 0;
			
			/* 测距 */
			read_distance();
			
			distance_buf[p] = distance;
			if (++p >= 3)
				p = 0;
			distance_out = median_filter(distance_buf); //使用滤波的范例，不使用滤波也行的
			
			/* 控制继电器 */;
			if (distance < 30)
				high_power_state |= 0x10;
			else
				high_power_state &= ~0x10;
			
			/* 距离显示 */
			dig[0] = distance_out / 10000;
			dig[1] = distance_out / 1000 % 10;
			dig[2] = distance_out / 100 % 10;
			dig[3] = distance_out / 10 % 10;
			dig[4] = distance_out % 10;
			
			/* 用数码管显示ultrasonic_flag */
			led_state &= ~0x03;
			led_state |= ultrasonic_flag;
		}
	}
}

#elif RTC_TEST == 1
/* 实时时钟测试 */
void test(void)
{
	uint8_t write_ds1302_flag = 0;
	write_ds1302();
	
	dig = dig_all[2]; //哪个界面都一样
	
	dig_all[2][2] = 17; //分隔符“-”
	dig_all[2][5] = 17; //分隔符“-”
	
	loop: //这里展示另外一种循环的写法，和while(1)是一样的

	if (Timer10ms_cnt)
	{
		Timer10ms_cnt = 0;
		
		key_pad_scan();
		
		if (key_value == 9 && key_state == 2) //s9暂停或启动时间
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
		else //如果write_ds1302_flag == 1，表示时间暂停
			;
	}
	
	dig_all[2][0] = time_10[0] / 10;
	dig_all[2][1] = time_10[0] % 10;
	
	dig_all[2][3] = time_10[1] / 10;
	dig_all[2][4] = time_10[1] % 10;
	
	dig_all[2][6] = time_10[2] / 10;
	dig_all[2][7] = time_10[2] % 10;
	
	if (write_ds1302_flag) //时停的时候点亮led
		led_state |= 0x80;
	else
		led_state &= ~0x80;
	
	goto loop; //程序跑到这里会跳转到上面的loop标志重新开始运行
}

#elif E2PROM_TEST == 1
/* e2prom测试 */
void test(void) 未完工
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
/* adc_dac测试 */
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
/* 温度测试 */
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

#elif UART_TEST == 1
/* 串口测试 */
void test(void)
{
	dig_switch = 0x00;
	
	dig[1] = 5;
	dig[2] = 25;
	dig[3] = 0xC;
	dig[4] = 0xC;
	dig[5] = 0xE;
	dig[6] = 5;
	dig[7] = 5;
	
	SendString("echo uart\r\n");
	
	while (1)
	{
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			key_pad_scan(); //矩阵键盘并不会影响串口收发
		}
		
		if (cmd)
		{
			Timern_cnt = 0;
		}
		else if (Timern_cnt >= 5000)
		{
			Timern_cnt = 0;
			SendString("input 'A' or 'B' or 'C':\r\n");
		}
		
		if (cmd & 0x01)
		{
			led_state ^= 0x80;
			cmd &= ~0x01;
		}
		if (cmd & 0x02)
		{
			dig_switch = ~dig_switch;
			cmd &= ~0x02;
		}
		if (cmd & 0x04)
		{
			high_power_state ^= 0x10;
			cmd &= ~0x04;
		}
	}
}

#elif CHIPS_TEST == 1
/* 所有芯片综合测试 */
void test(void)
{
	uint8_t mode = 1;
	uint8_t operate_flag = 0;
	
	read_temp();
	write_ds1302();
	dac_out();
	Delay5ms();
	adc_read();
	
	dig_all[1][2] = dig_all[1][5] = 17; //分隔符“-”
	
	loop:
	
	if (Timer10ms_cnt >= 10)
	{
		Timer10ms_cnt = 0;
		
		/* 按键扫描 */
		key_pad_scan();
		
		/* 各种时序操作 */
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
		
		/* 按键功能 */
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
		
		/* led显示当前模式 */
		led_state = 0x01 << (mode - 1);
		
		/* 数码管界面选择 */
		dig = dig_all[mode];
		
		/* 界面1 时钟显示 */
		dig_all[1][0] = time_10[0] / 10;
		dig_all[1][1] = time_10[0] % 10;
		
		dig_all[1][3] = time_10[1] / 10;
		dig_all[1][4] = time_10[1] % 10;
		
		dig_all[1][6] = time_10[2] / 10;
		dig_all[1][7] = time_10[2] % 10;
		
		/* 界面2 ADC通道0、1 */
		dig_all[2][0] = adc_level[0] / 100;
		dig_all[2][1] = adc_level[0] / 10 % 10;
		dig_all[2][2] = adc_level[0] % 10;
		
		dig_all[2][5] = adc_level[1] / 100;
		dig_all[2][6] = adc_level[1] / 10 % 10;
		dig_all[2][7] = adc_level[1] % 10;
		
		/* 界面3 ADC通道2、3 */
		dig_all[3][0] = adc_level[2] / 100;
		dig_all[3][1] = adc_level[2] / 10 % 10;
		dig_all[3][2] = adc_level[2] % 10;
		
		dig_all[3][5] = adc_level[3] / 100;
		dig_all[3][6] = adc_level[3] / 10 % 10;
		dig_all[3][7] = adc_level[3] % 10;
		
		/* 界面4 DAC、超声波测距 */
		dig_all[4][0] = dac_level / 100;
		dig_all[4][1] = dac_level / 10 % 10;
		dig_all[4][2] = dac_level % 10;
		
		dig_all[4][5] = distance / 100;
		dig_all[4][6] = distance / 10 % 10;
		dig_all[4][7] = distance % 10;
		
		/* 界面5 ne555频率 */
		dig_all[5][3] = ne555_out / 1000;
		dig_all[5][4] = ne555_out / 100 % 10;
		dig_all[5][5] = ne555_out / 10 % 10;
		dig_all[5][6] = ne555_out % 10;
		dig_all[5][7] = 0;
		
		/* 界面6 温度显示 */
		dig_all[6][4] = temperature / 10000;
		dig_all[6][5] = temperature / 1000 % 10;
		dig_all[6][6] = temperature / 100 % 10 + 32;
		dig_all[6][7] = temperature % 10;
	}
	
	goto loop;
}

#else
void test(void) //这个只是为了防止警告
{
	;
}
#endif