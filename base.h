#ifndef __BASE_H__
#define __BASE_H__

/* 
	听着，所谓基础（BASE），
	就是ledpwm、数码管、继电器、蜂鸣器、ne555测频率、
	以及一些主函数里使用到的软件定时器
 */

#include <STC15F2K60S2.H>

#include "myint.h"

#define CHANNLE(n, x) { P0 = x; P2 |= n << 5; P2 &= 0x1F; }

#define SCREEN_NUM 7 //界面的数量

extern uint8_t xdata *dig; //当前显示界面
extern uint8_t xdata dig_all[][8];
extern uint8_t xdata dig_switch; //数码管每一位的开关

extern uint8_t xdata led_state; //每个led的状态
extern uint8_t xdata led_pwm_duty; //led的pwm占空比，默认满占空比

/* 大功率外设状态
   bit 0 - 3为步进电机驱动输出
   bit 4为继电器输出
   bit 5为电机驱动输出
   bit 6为蜂鸣器输出 */
extern uint8_t xdata high_power_state;

/* 虚拟定时器 */
extern uint8_t xdata Timer10ms_cnt;
extern uint8_t xdata Timer100ms_cnt;
extern uint16_t xdata Timer500ms_cnt;
extern uint16_t xdata Timer1s_cnt;
extern uint16_t xdata Timern_cnt;
extern uint16_t xdata operate_timer_cnt;

extern uint16_t xdata ne555_out;

void Timer0Init(void);
void Timer1Init(void);

#endif
