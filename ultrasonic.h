#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

/* 如果你发现测距的量程很短，那你应该看看
	 下载的时候IRC频率有没有调成12.000MHz，
	 而不是来质疑我的代码 (>w<) */
#include <STC15F2K60S2.H>

#include "base.h"

#include "myint.h"
#include "intrins.h"

#define DISTANCE_OUTRANG (0xFFFF) //定义为超出量程时的距离数值
#define RANGE_LIMIT 

void TimerPCAInit();
void sand_ultrasonic();
void calculate_distance();

extern uint16_t distance;
extern uint8_t ultrasonic_flag;

#endif
