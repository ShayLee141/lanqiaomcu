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
	 253：当前按键从按下到松开的时间大于2秒
	 254：当前按键从按下到松开的时间在1秒到2秒之间
	 255：当前按键从按下到松开的时间不到1秒 */

/* 键值查询表 */
uint8_t code key_pad_value_all[4][4] = 
{ //常用于矩阵按键
	{7, 11, 15, 19, },
	{6, 10, 14, 18, },
	{5, 9, 13, 17, },
	{4, 8, 12, 16, },
};
/* 一般都是10ms左右调用一次，
	 10ms为消抖时间，可以根据需要更改 */
/* 矩阵按键 */
void key_pad_scan()
{
	uint8_t row = 255, column = 255; //行号与列号，255是默认值，即无效值
	
	/* 操作IO口，这里按每个位来操作的，实际上可以统一赋值 */
#ifndef USE_UART
	P30 = 1; //第1行
	P31 = 1; //第2行
#endif //USE_UART
	P32 = 1; //第3行
	P33 = 1; //第4行
	
	P44 = 0; //第1列
	P42 = 0; //第2列
	P35 = 0; //第3列
#ifndef USE_NE555
	P34 = 0; //第4列
#endif //USE_NE555
	
	if (~P3 & 0x0F) //如果有按键按下；这里可以翻译为P3取反后屏蔽掉高四位，有按键按下则这个结果就不为零，和前面独立按键一致的
	{
		/* 行判断 */
		if (!P33) //第4行
			row = 3;
		else if (!P32) //第3行
			row = 2;
#ifndef USE_UART
		else if (!P31) //第2行
			row = 1;
		else if (!P30) //第1行
			row = 0;
#endif //USE_UART
		else //如果使误读则退出函数
			return ;
		
		/* 操作IO口 */
#ifndef USE_UART
		P30 = 0; //第1行
		P31 = 0; //第2行
#endif //USE_UART
		P32 = 0; //第3行
		P33 = 0; //第4行
		
		P44 = 1; //第1列
		P42 = 1; //第2列
		P35 = 1; //第3列
		P34 = 1; //第4列
		
		/* 列判断 */
		if (!P44) //第1列
			column = 0;
		else if (!P42) //第2列
			column = 1;
		else if (!P35) //第3列
			column = 2;
#ifndef USE_NE555
		else if (!P34) //第4列
			column = 3;
#endif //USE_NE555
		else //如果使误读则退出函数
			return ;
		
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
#ifdef KEY_RELEASE
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
#endif //KEY_RELEASE
		{
			key_value = 0;
			key_state = 0;
		}
	}
}
