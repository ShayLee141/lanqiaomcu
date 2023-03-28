#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include <STC15F2K60S2.H>

#include "base.h"

#include "myint.h"
#include "intrins.h"

#define DISTANCE_OUTRANG (0xFFFF) //����Ϊ��������ʱ�ľ�����ֵ

void TimerPCAInit();
uint16_t read_distance();

extern uint16_t distance;

#endif
