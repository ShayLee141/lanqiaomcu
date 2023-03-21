#include "test.h"

/* led pwm测试，前四个灯会累加计数，后四个灯会运行跑马灯，所有灯均会呈现呼吸灯的特效 */
void led_pwm_test(void)
{
	/* 这个数组保存了流水灯的每一帧的LED状态，仅后四个灯 */
	uint8_t code led_state_all[6] = 
	{ 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, };
	
	int16_t add = 1; //用于控制累加的方向
	uint8_t cnt = 0; //计数，并显示在前四个灯上
	uint8_t led_state_cnt = 0; //用于流水灯
		
	while(1)
	{
		if (Timer1s_cnt >= 1000)
		{
			Timer1s_cnt = 0;
			
			cnt++;
			
			if (++led_state_cnt >= 6) //用于流水灯
				led_state_cnt = 0;
		}
		
		if (Timer10ms_cnt >= 10) //10ms更新一次pwm占空比和led输出状态
		{
			Timer10ms_cnt = 0;
			
			/* 流水的控制 */
			led_pwm_duty += add;
			if (led_pwm_duty >= 100)
				add = -1;
			if (led_pwm_duty == 0)
				add = 1;
		
			led_state = (cnt & 0x0F) | led_state_all[led_state_cnt];
		}
	}
}

/* 按键测试 */
void key_test(void)
{
	uint8_t cnt = 127;
	
	while(1)
	{
		/* 10ms执行一次按键扫描 */
		if (Timer10ms_cnt >= 10) //这里可以修改消抖时间，如果想看清楚数码管和LED一闪而过的内容，不妨把这个值增大到50或者更大，但是要记得调回来哦
		{
			Timer10ms_cnt = 0;
			
			/* 按键扫描 */
//			key_scan_simple();							/* 新手简单版	独立按键 */
//			key_pad_scan_simple();					/* 新手简单版 矩阵按键 */
//			key_scan_release();							/* 松手检测版 独立按键 */
			key_pad_scan_release();					/* 松手检测版 矩阵按键 */
//			key_pad_half_scan_simple();			/* 串口冲突减半简单版 矩阵按键 */
//			key_pad_remain_scan_simple();		/* 串口三五双双冲突残缺简单版 矩阵按键 */
			
			/* 判断按键 */
			/* 无非就是利用key_value和key_state两个变量来判断 */
			if (key_value == 4) //如果s4按下
			{
				switch (key_state)
				{
					case 2: //短按
						led_state |= 0x02; //短按标志（一闪而过）
					break;
					
					case 100: //长按1s
						led_state |= 0x04; //长按1s标志（一闪而过）
					break;
					
					case 200: //长按2s
						led_state |= 0x08; //长按2s标志（一闪而过）
					break;
					
					case 255: //短按松手（一闪而过）
						led_state |= 0x10; //松手标志
					break;
					
					case 254: //长按1s后松手（一闪而过）
						led_state |= 0x20; //松手标志
					break;
					
					case 253: //长按2s后松手（一闪而过）
						led_state |= 0x40; //松手标志
					break;
					
					default:
						led_state &= ~0x7E; //其他时刻把led熄灭
				}
				
				led_state |= 0x01; //按下标志（按下持续点亮）
			}
			else //s4松开
			{
				led_state &= ~0xFF; //清除所有标志
				
				if (key_value == 7 && key_state == 2) //s7短按
					cnt++;
				if (key_value == 6 && key_state == 2) //s6短按
					cnt--;
			}
			
			/* 数码管显示 */
			{
				/* 键值显示 */
				dig[0] = key_value / 10;
				dig[1] = key_value % 10;
				
				/* 状态显示 */
				dig[3] = key_state / 100;
				dig[4] = key_state / 10 % 10;
				dig[5] = key_state % 10;
				
				/* 可控变量显示 */
				dig[7] = cnt % 10;
			}
		}
	}
}

/* 显示测试 */
void display_test(void)
{
	uint8_t screen = 2; //当前界面
	uint8_t screen_bak = screen; //备份当前界面，以便重新返回
	uint8_t operate_num = 0; //用于错开两种长时读写的时机
	
	dig = dig_all[screen]; //默认为界面2
	
	rd_temperature();
	//这里可以加上时钟初值的设置，也可以像我一样在ds1302.c里直接设置初值
	write_time(); //时钟初始化
	dac_level = 20;
	write_dac();
	
	while(1)
	{
		/* 10ms执行一次按键扫描 */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			/* 按键扫描 */
			key_pad_remain_scan_simple();
			
			if (Timer50ms_cnt >= 50)
			{
				Timer50ms_cnt = 0;
				
				switch (operate_num++)
				{
					default:
						operate_num = 0;
					case 1:
						read_time(); //读取时间
						break;
					
					case 2:
						read_adc(); //读取ADC
						break;
					
					case 3:
						break;
					
					case 4:
						rd_temperature();
						break;
				}
			}
			
			/* 判断按键 */
			if (key_value == 4 && key_state > 2) //如果处于s4被按下期间
			{
				if (screen != 1) //你可以尝试注释掉这个判断语句看看功能会发送什么变化
				{
					screen_bak = screen; //备份当前界面号
					screen = 1; //显示界面1
				}
			}
			else
			{
				if (screen == 1) //你可以尝试注释掉这个判断语句看看功能会发送什么变化
				{
					screen = screen_bak; //还原因s4改变的界面
				}
			}
			
			if (key_value == 5 && key_state == 100) //如果s5长按1秒
			{
				if (screen) //关闭
				{
					screen_bak = screen; //备份当前界面号
					screen = 0; //关闭显示（黑屏界面）
					stop_dac(); //停止DAC输出
				}
				else //开启
				{
					screen = screen_bak; //还原显示
					write_dac(); //重启DAC输出
				}
			}
			else if (key_value == 8 && key_state == 2) //如果s8短按
			{
				if (--screen < 2)
					screen = SCREEN_NUM - 1;
			}
			else if (key_value == 9 && key_state == 2) //如果s9短按
			{
				if (++screen > SCREEN_NUM - 1)
					screen = 2;
			}
			
			/* 显示输出 */
			dig = dig_all[screen]; //显示界面
			
			/* 界面1 显示当前温度 */
			
			/* 界面2 显示当前时间 */
			dig_all[2][0] = time_dec[0] / 10;
			dig_all[2][1] = time_dec[0] % 10;
			dig_all[2][3] = time_dec[1] / 10;
			dig_all[2][4] = time_dec[1] % 10;
			dig_all[2][6] = time_dec[2] / 10;
			dig_all[2][7] = time_dec[2] % 10;
						
			/* 界面3 显示当前日期 */
			dig_all[3][0] = 2;
			dig_all[3][1] = 0;
			dig_all[3][2] = time_dec[5] / 10;
			dig_all[3][3] = time_dec[5] % 10;
			dig_all[3][4] = time_dec[3] / 10;
			dig_all[3][5] = time_dec[3] % 10;
			dig_all[3][6] = time_dec[4] / 10;
			dig_all[3][7] = time_dec[4] % 10;
			
			/* 界面4 显示当前ne555输出频率 */
			dig_all[4][3] = ne555_out / 10000 % 10;
			dig_all[4][4] = ne555_out / 1000 % 10;
			dig_all[4][5] = ne555_out / 100 % 10;
			dig_all[4][6] = ne555_out / 10 % 10;
			dig_all[4][7] = ne555_out % 10;
			
			/* 界面5 显示ADC采样和DAC输出等级 */
			dig_all[5][0] = adc_level[1] / 0x10;
			dig_all[5][1] = adc_level[1] % 0x10;
			dig_all[5][3] = adc_level[3] / 0x10;
			dig_all[5][4] = adc_level[3] % 0x10;
			dig_all[5][6] = dac_level / 0x10;
			dig_all[5][7] = dac_level % 0x10;
		}
	}
}

/* 超声波测试 */
void ultrasonic_test(void)
{
	unsigned int distance = 0xFFFF;
	
	while(1)
	{
		if (Timer100ms_cnt >= 100)
		{
			Timer100ms_cnt = 0;
			
			/* 测距 */
			distance = read_distance();
			
			/* 控制继电器 */
			if (distance < 30)
				high_power_state |= 0x10;
			else
				high_power_state &= ~0x10;
			
			/* 距离显示 */
			dig[0] = distance / 10000;
			dig[1] = distance / 1000 % 10;
			dig[2] = distance / 100 % 10;
			dig[3] = distance / 10 % 10;
			dig[4] = distance % 10;
		}
	}
}

/* e2prom测试 */
void e2prom_test(void)
{
	uint16_t number;
	
	read_e2prom(0x14, (uint8_t*)&number, 2); //随便找了个地址测试，uint16_t是两个字节
	
	while(1)
	{
		/* 10ms执行一次按键扫描 */
		if (Timer10ms_cnt >= 10)
		{
			Timer10ms_cnt = 0;
			
			key_pad_remain_scan_simple();
			
			if (key_value == 5 && key_state == 2) //增加数值
			{
				number++;
			}
			else if (key_value == 4 && key_state == 2) //减少数值
			{
				number--;
			}
			else if (key_value == 9 && key_state == 2) //保存数值，重新上电就能看到效果
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

/* 温度测试 */
void temperature_test(void)
{
	while(1)
	{
		if (Timer100ms_cnt >= 100)
		{
			Timer100ms_cnt = 0;
			
			rd_temperature();
			
			if (temperature < 0) //负温度
			{
				dig[0] = 17;
				dig[1] = (-temperature) / 10000 % 10;
				dig[2] = (-temperature) / 1000 % 10;
				dig[3] = (-temperature) / 100 % 10 + 32;
				dig[4] = (-temperature) / 10 % 10;
				dig[5] = (-temperature) % 10;
			}
			else //非负温度
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
