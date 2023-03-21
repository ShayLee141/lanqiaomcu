#ifndef __TEST_H__
#define __TEST_H__

#include <STC15F2K60S2.H>

#include "base.h"
#include "key.h"
#include "ultrasonic.h"

#include "ds1302.h"
#include "iic.h"
#include "onewire.h"

#include "myint.h"

/* 专门用来测试各个功能的代码 */
/* 每一个函数中都有一个while (1) */
/* 因此每次只能调用一个函数 */

/* led pwm测试
   前四个灯会累加计数
	 后四个灯会运行跑马灯
	 所有灯均会同步呈现呼吸灯的特效 */
void led_pwm_test(void);

/* 按键测试
   会在数码管上实时显示按键键值和状态值，
	 s7和s6可以增减数码管末位数字，
	 led由s4控制，展示了
	 按住、短按、长按1秒、长按2秒、
	 短按松手（小于1秒）、长按松手（大于1秒），几种控制方式
   具体功能由调用的按键扫描函数决定 */
void key_test(void);

/* 显示测试
   配合按键，实现按键切换数码管显示界面，
	 这里用的按键扫描函数为“串口三五双双冲突残缺简单版”，
	 第1、2行按键和第4列按键是无效的，
	 s8和s9可以切换界面1至3
	 按住s4可以显示界面4
	 s5长按1秒可以开关显示（黑屏界面）；
	 界面0是黑屏界面，界面1是测试界面，
	 界面2为温度界面，界面3为时钟界面，
	 界面4为频率界面 */
void display_test(void);

/* 超声波测试
   显示当前测距，距离小于30会打开继电器 */
void ultrasonic_test(void);

/* e2prom测试 */
void e2prom_test(void);

/* 温度测试 */
void temperature_test(void);

#endif
