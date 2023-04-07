#ifndef __KEY_H__
#define __KEY_H__

#include <STC15F2K60S2.H>

#include "main.h"

/* 请注意，这个按键函数不适用多按键同时按下 */

/* 如果你觉得按键不正常，
   你应该优先检查设置矩阵按键和独立按键的跳帽有没有接对，
	 再去检查频率测量那个跳帽是不是影响了
	 （这时候应该把下面的USE_NE555取消注释），
	 以及是不是在使用串口的情况下调用了按键函数
	 （这时候应该把下面的USE_UART取消注释），
	 而不是来质疑我的代码有问题 o((>ω<))o */

extern uint8_t key_value;
extern uint8_t key_state;

/* 独立按键不想写了，
	 矩阵按键函数用在独立按键模式下，
	 也是可以正确返回s4到s7的值的 */
void key_pad_scan(); //矩阵按键

#endif
