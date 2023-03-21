#ifndef __ONEWIRE_H
#define __ONEWIRE_H

//#include "reg52.h" ע�͵����
#include <STC15F2K60S2.H> //�������

#include "base.h"

#include "myint.h" //�������

sbit DQ = P1^4;  

/*    ��Ϊ��ϣ����������������¶ȵ�һ�ٱ���
   �Ա���ʾС���¶ȶ���ʹ��float���ͣ�
	 ����������int16_t */
extern int16_t temperature;

/* ����������һ��δ����ĺ�����
   �����Ǿͽ��ƾͼƣ��������������
   ����ԭ�ȵ�unsigned char�ĳ�void
   �ҵ��¶�ʹ�������ȫ�ֱ�������� */
void rd_temperature(void);  

#endif
