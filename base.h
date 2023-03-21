#ifndef __BASE_H__
#define __BASE_H__

/* 
	���ţ���ν������BASE����
	����ledpwm������ܡ��̵�������������ne555��Ƶ�ʡ�
	�Լ�һЩ��������ʹ�õ��������ʱ��
 */

#include <STC15F2K60S2.H>

#include "myint.h"

#define CHANNLE(n, x) { P0 = x; P2 |= n << 5; P2 &= 0x1F; }

#define SCREEN_NUM 6 //���������

extern uint8_t *dig; //��ǰ��ʾ����
extern uint8_t xdata dig_all[][8];
extern uint8_t dig_switch; //�����ÿһλ�Ŀ���

extern uint8_t led_state; //ÿ��led��״̬
extern uint8_t led_pwm_duty; //led��pwmռ�ձȣ�Ĭ����ռ�ձ�

/* ��������״̬
   bit 0 - 3Ϊ��������������
   bit 4Ϊ�̵������
   bit 5Ϊ����������
   bit 6Ϊ��������� */
extern uint8_t high_power_state;

/* ���ⶨʱ�� */
extern uint8_t Timer10ms_cnt;
extern uint8_t Timer50ms_cnt;
extern uint8_t Timer100ms_cnt;
extern uint8_t Timer200ms_cnt;
extern uint16_t Timer1s_cnt;

extern uint16_t ne555_out;

void Timer0Init(void);
void Timer1Init(void);

#endif
