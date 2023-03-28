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

/* led pwm����
   ǰ�ĸ��ƻ��ۼӼ���
	 ���ĸ��ƻ����������
	 ���еƾ���ͬ�����ֺ����Ƶ���Ч */
#define LED_PWM_TEST 0
/* ��������
   �����������ʵʱ��ʾ������ֵ��״ֵ̬��
	 s13��s12�������������ĩλ���֣�
	 led��s5��s9���ƣ�չʾ��
	 ��ס���̰�������1�롢����2�롢
	 �̰����֣�С��1�룩����������1����2�����ɿ���
	 ��������2����ɿ������ֿ��Ʒ�ʽ */
#define KEY_TEST 0
/* ��ʾ����
   ��ϰ�����ʵ�ְ����л��������ʾ���棬
	 s13��s12�����л�����1��4
	 ��סs9������ʾ����5���ɿ�����ʾ����֮ǰ�Ľ��棬
	 s13����1�����ҿ��Կ�����ʾ���������棩 */
#define DISPLAY_TEST 0
/* ����������
   ��ʾ��ǰ��࣬����С��30��򿪼̵��� */
#define ULTRASONIC_TEST 0
/* ʵʱʱ�Ӳ���
	 ����s9������������ͣʱ��	 */
#define RTC_TEST 0
/* e2prom���� */
#define E2PROM_TEST 0
/* adc_dac���� */
#define ADC_DAC_TEST 0
/* �¶Ȳ��� */
#define TEMP_TEST 0
/* ����оƬ�ۺϲ��� */
#define CHIPS_TEST 1

#if LED_PWM_TEST + KEY_TEST + DISPLAY_TEST + \
		ULTRASONIC_TEST + RTC_TEST + E2PROM_TEST + \
		ADC_DAC_TEST + TEMP_TEST + CHIPS_TEST > 1
#error ÿ�����ֻ����һ���궨��Ϊ 1
/* ÿ�����ֻ����һ���궨��Ϊ1 */
/* ÿ�����ֻ����һ���궨��Ϊ1 */
/* ÿ�����ֻ����һ���궨��Ϊ1 */
/* ������ȫΪ0 */
#endif

void test(void);

#endif
