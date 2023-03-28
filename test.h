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
#include "filter.h"

/* led pwm测试
   前四个灯会累加计数
	 后四个灯会运行跑马灯
	 所有灯均会同步呈现呼吸灯的特效 */
#define LED_PWM_TEST 0
/* 按键测试
   会在数码管上实时显示按键键值和状态值，
	 s13和s12可以增减数码管末位数字，
	 led由s5和s9控制，展示了
	 按住、短按、长按1秒、长按2秒、
	 短按松手（小于1秒）、长按按下1秒至2秒内松开、
	 长按按下2秒后松开，几种控制方式 */
#define KEY_TEST 0
/* 显示测试
   配合按键，实现按键切换数码管显示界面，
	 s13和s12可以切换界面1至4
	 按住s9可以显示界面5，松开会显示按下之前的界面，
	 s13长按1秒左右可以开关显示（黑屏界面） */
#define DISPLAY_TEST 0
/* 超声波测试
   显示当前测距，距离小于30会打开继电器 */
#define ULTRASONIC_TEST 0
/* 实时时钟测试
	 按键s9可以启动和暂停时钟	 */
#define RTC_TEST 0
/* e2prom测试 */
#define E2PROM_TEST 0
/* adc_dac测试 */
#define ADC_DAC_TEST 0
/* 温度测试 */
#define TEMP_TEST 0
/* 所有芯片综合测试 */
#define CHIPS_TEST 1

#if LED_PWM_TEST + KEY_TEST + DISPLAY_TEST + \
		ULTRASONIC_TEST + RTC_TEST + E2PROM_TEST + \
		ADC_DAC_TEST + TEMP_TEST + CHIPS_TEST > 1
#error 每次最多只能有一个宏定义为 1
/* 每次最多只能有一个宏定义为1 */
/* 每次最多只能有一个宏定义为1 */
/* 每次最多只能有一个宏定义为1 */
/* 但可以全为0 */
#endif

void test(void);

#endif
