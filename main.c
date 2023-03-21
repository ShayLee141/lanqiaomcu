#include <STC15F2K60S2.H>
#include <intrins.h>

#include "base.h"
#include "key.h"
#include "ultrasonic.h"

#include "ds1302.h"
#include "onewire.h"
#include "iic.h"

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
//	TimerPCAInit();
	
	EA = 1;
	
	/* 测试的时候只能保留一个函数，因为每个函数里都有while(1) */
//	led_pwm_test();
//	key_test();
	display_test();
//	ultrasonic_test();
//	e2prom_test();
//	temperature_test();
	
	return 0;
}
