#include <STC15F2K60S2.H>
#include <intrins.h>

#include "base.h"
#include "key.h"
#include "ultrasonic.h"

#include "ds1302.h"
#include "iic.h"

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
	
	EA = 1;

	test(); //ģ����Կ���ʹ�������������������һ����ѭ��������ִ�к����while(1)������ģ��û�����
	
	while(1)
	{
		;
	}
	
//	return 0;
}
