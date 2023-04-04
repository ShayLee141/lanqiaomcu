#include <STC15F2K60S2.H>
#include <intrins.h>

#include "base.h"
#include "key.h"
#include "ultrasonic.h"
#include "uart.h"

#include "ds1302.h"
#include "iic.h"
#include "onewire.h"

#include "myint.h"
#include "filter.h"

#include "test.h"

int main(void)
{
	P2 = 0x00;
	CHANNLE(4, 0xFF);
	CHANNLE(5, 0x00);
	CHANNLE(6, 0x00);
	CHANNLE(7, 0x00);
	
	Timer0Init();
	Timer1Init();
	TimerPCAInit();
	UartInit();
	
	EA = 1;

	test(); //模块测试可以使用这个函数，函数里有一个死循环，不会执行后面的while(1)，正常模板没有这句
	
	while(1)
	{
		;
	}
	
//	return 0;
}
