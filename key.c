#include "key.h"

/* 下面列出的所有按键函数
	 均使用这两个变量作为输出，
	 主函数里只需要判断这两个变量，
	 即可获取按键状态信息 */
uint8_t key_value = 0; //取值范围4 - 19
uint8_t key_state = 0;
/* key_state表示按键当前状态
   0：表示未按下
   1：按键正在消抖处理
   2 - 250：表示消抖以后按下的时间，单位是10ms
            本模板只计时到2.5秒，需要的可以延长
						注意要改成uint16_t类型
	 255：当前按键松开
	      前面的时间延长了此处也要修改到合适的值 */

/* 下列函数一般都是10ms左右调用一次，
	 10ms为消抖时间，可以根据需要更改 */

/* 键值查询表 */
uint8_t code key_pad_value_all[4][4] = 
{ //常用于矩阵按键
	{19, 15, 11, 7, }, 
	{18, 14, 10, 6, }, 
	{17, 13, 9, 5, }, 
	{16, 12, 8, 4, }, 
};
uint8_t code key_pad_value_half[2][4] = 
{ //只有一半的按键，常用于和串口同时使用的矩阵按键
	{17, 13, 9, 5, }, 
	{16, 12, 8, 4, }, 
};
uint8_t code key_pad_value_remain[2][3] = 
{ //残缺的矩阵键盘，常用于串口和三五定时器频率测量同时使用时的矩阵按键
	{13, 9, 5, }, 
	{12, 8, 4, }, 
};

/* 新手简单版 独立按键 */
void key_scan_simple()
{
	/* 操作IO口 */
	P3 |= 0x0F; //P30 - P33均输出高电平
	
	if (~P3 & 0x0F)
	/* 如果有按键按下；这里可以翻译为
		 P3取反后屏蔽掉高四位，总之若有按键按下，
		 则这个结果就不为零 */
	{
		/* 键值判断 */
		if (!P33) //P33如果被拉低则按键S4按下
			key_value = 4;
		else if (!P32) //P32如果被拉低则按键S5按下
			key_value = 5;
		else if (!P31) //P31如果被拉低则按键S6按下
			key_value = 6;
		else if (!P30) //P30如果被拉低则按键S7按下
			key_value = 7;
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else //如果没有按键按下
	{
		/* 清零 */
		key_value = 0;
		key_state = 0;
	}
}

/* 新手简单版 矩阵按键 */
void key_pad_scan_simple()
{
	uint8_t row = 255, column = 255; //行号与列号，255是默认值，无效值
	
	/* 操作IO口 */
	P3 = 0x0F; // P30 - P33均输出高电平，P34 - P37均输出低电平，实际上P36和P37是不用管的，只是写起来方便
	P4 = 0x00; // P4输出低电平，实际上只需要P42和P44输出低电平即可
	
	if (~P3 & 0x0F) //如果有按键按下；这里可以翻译为P3取反后屏蔽掉高四位，有按键按下则这个结果就不为零，和前面独立按键一致的
	{
		/* 行判断 */
		if (!P30) //第1行
			row = 0;
		else if (!P31) //第2行
			row = 1;
		else if (!P32) //第3行
			row = 2;
		else if (!P33) //第4行
			row = 3;
		
		/* 操作IO口 */
		P3 = 0xF0; // P30 - P33均输出低电平，P34 - P37均输出高电平，实际上P36和P37是不用管的，只是写起来方便
		P4 = 0xFF; // P4输出高电平，实际上只需要P42和P44输出高电平即可
	
		/* 列判断 */
		if (!P34) //第1列
			column = 0;
		else if (!P35) //第2列
			column = 1;
		else if (!P42) //第3列
			column = 2;
		else if (!P44) //第4列
			column = 3;
		
		/* 键值判断 */
		if (row != 255 && column != 255) //如果按键有效
		{
			key_value = key_pad_value_all[row][column];
		}
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else
	{
		key_value = 0;
		key_state = 0;
	}
}

/* 松手检测版 独立按键 */
void key_scan_release()
{
	/* 操作IO口 */
	P3 |= 0x0F; //P30 - P33均输出高电平
	
	if (~P3 & 0x0F)
	/* 如果有按键按下；这里可以翻译为
		 P3取反后屏蔽掉高四位，总之若有按键按下，
		 则这个结果就不为零 */
	{
		/* 键值判断 */
		if (!P33) //P33如果被拉低则按键S4按下
			key_value = 4;
		else if (!P32) //P32如果被拉低则按键S5按下
			key_value = 5;
		else if (!P31) //P31如果被拉低则按键S6按下
			key_value = 6;
		else if (!P30) //P30如果被拉低则按键S7按下
			key_value = 7;
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else //如果没有按键按下
	{
		/* 和没带松手检测的版本只有下面的区别 */
		if (key_value && (key_state <= 250)) //如果按键刚松开
		{
			if (key_state < 100) //若按下到松手时间没有超过1秒
				key_state = 255;
			else if (key_state < 200) //若按下到松手时间超过1秒而没超过2秒
				key_state = 254;
			else //若按下到松手时间超过2秒
				key_state = 253;
		}
		else //按键完全松开
		{
			key_value = 0;
			key_state = 0;
		}
	}
}

/* 松手检测版 矩阵按键 */
void key_pad_scan_release()
{
	uint8_t row = 255, column = 255; //行号与列号，255是默认值，无效值
	
	/* 操作IO口 */
	P3 = 0x0F; // P30 - P33均输出高电平，P34 - P37均输出低电平，实际上P36和P37是不用管的，只是写起来方便
	P4 = 0x00; // P4输出低电平，实际上只需要P42和P44输出低电平即可
	
	if (~P3 & 0x0F) //如果有按键按下；这里可以翻译为P3取反后屏蔽掉高四位，有按键按下则这个结果就不为零，和前面独立按键一致的
	{
		/* 行判断 */
		if (!P30) //第1行
			row = 0;
		else if (!P31) //第2行
			row = 1;
		else if (!P32) //第3行
			row = 2;
		else if (!P33) //第4行
			row = 3;
		
		/* 操作IO口 */
		P3 = 0xF0; // P30 - P33均输出低电平，P34 - P37均输出高电平，实际上P36和P37是不用管的，只是写起来方便
		P4 = 0xFF; // P4输出高电平，实际上只需要P42和P44输出高电平即可
	
		/* 列判断 */
		if (!P34) //第1列
			column = 0;
		else if (!P35) //第2列
			column = 1;
		else if (!P42) //第3列
			column = 2;
		else if (!P44) //第4列
			column = 3;
		
		/* 键值判断 */
		if (row != 255 && column != 255) //如果按键有效
		{
			key_value = key_pad_value_all[row][column];
		}
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else
	{
		/* 和没带松手检测的版本只有下面的区别 */
		if (key_value && (key_state <= 250)) //如果按键刚松开
		{
			if (key_state < 100) //若按下到松手时间没有超过1秒
				key_state = 255;
			else if (key_state < 200) //若按下到松手时间超过1秒而没超过2秒
				key_state = 254;
			else //若按下到松手时间超过2秒
				key_state = 253;
		}
		else //按键完全松开
		{
			key_value = 0;
			key_state = 0;
		}
	}
}

/* 串口冲突减半简单版 矩阵按键 */
void key_pad_half_scan_simple()
{
	uint8_t row = 255, column = 255; //行号与列号，255是默认值，无效值
	
	/* 操作IO口 */
	P3 &= ~0x30; P4 &= ~0x14;	// P34 P35 P42 P44输出低电平
	P3 |= 0x0C;								// P32和P33输出高电平

	if (~P3 & 0x0C) //如果有按键按下
	{
		/* 行判断 */
		if (!P32) //第1行
			row = 0;
		else if (!P33) //第2行
			row = 1;
		
		/* 操作IO口 */
		P3 |= 0x30; P4 |= 0x14;	// P34 P35 P42 P44输出高电平
		P3 &= ~0x0C;						// P32和P33输出低电平
		
		/* 列判断 */
		if (!P34) //第1列
			column = 0;
		else if (!P35) //第2列
			column = 1;
		else if (!P42) //第3列
			column = 2;
		else if (!P44) //第4列
			column = 3;
		
		/* 键值判断 */
		if (row != 255 && column != 255) //如果按键有效
		{
			key_value = key_pad_value_half[row][column];
		}
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else
	{
		key_value = 0;
		key_state = 0;
	}
}

/* 串口三五双双冲突残缺简单版 矩阵按键 */
void key_pad_remain_scan_simple()
{
	uint8_t row = 255, column = 255; //行号与列号，255是默认值，无效值
	
	/* 操作IO口 */
	P3 &= ~0x20; P4 &= ~0x14;	// P35 P42 P44输出低电平
	P3 |= 0x0C;								// P32和P33输出高电平

	if (~P3 & 0x0C) //如果有按键按下
	{
		/* 行判断 */
		if (!P32) //第1行
			row = 0;
		else if (!P33) //第2行
			row = 1;
		
		/* 操作IO口 */
		P3 |= 0x20; P4 |= 0x14;	// P35 P42 P44输出高电平
		P3 &= ~0x0C;						// P32和P33输出低电平
		
		/* 列判断 */
		if (!P35) //第1列
			column = 0;
		else if (!P42) //第2列
			column = 1;
		else if (!P44) //第3列
			column = 2;
		
		/* 键值判断 */
		if (row != 255 && column != 255) //如果按键有效
		{
			key_value = key_pad_value_remain[row][column];
		}
		
		/* 按键计时 */
		if (key_state < 250)
			key_state++;
	}
	else
	{
		key_value = 0;
		key_state = 0;
	}
}

