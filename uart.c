#include "uart.h"

bit busy = 0;

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xC7;			//设置定时初始值
	T2H = 0xFE;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES = 1;					//使能串口1中断
}

uint8_t cmd = 0;
/* 0：无指令
	 1：指令1
	 2：指令2
	 ...
	 n：指令n */
uint8_t code cmd[CMD_NUM][CMD_LEN] = 
{
	{ "Led" },					//指令1
	{ "Dig" },					//指令2
	{ "Beep" },					//指令3
	{ "Relay" },				//指令4
	{ "Return" },				//指令5
}

void Uart() interrupt 4
{
	uint8_t i, j, len, buf;
	
	if (RI)
	{
		RI = 0;                 //清除RI位
		buf = SBUF;
		
		for (i = 0; i < CMD_NUM; i++)
		{
			for (j = 0, len = strlen(cmd[i]); j < len; j++)
			{
				if (buf != cmd[i][j])
					break;
			}
			
			if (j == len)
			{
				cmd = i + 1;
				break;
			}
		}
	}
	
	if (TI)
	{
		TI = 0;                 //清除TI位
		busy = 0;               //清忙标志
	}
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(uint8_t dat)
{
	while (busy);               //等待前面的数据发送完成
	busy = 1;
	SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}
