#ifndef __DS1302_H_
#define __DS1302_H_

//#include <reg52.h> ע�͵����
#include <intrins.h>
#include <STC15F2K60S2.H> //�������

#include "myint.h" //�������

/* �⼸��û�иģ����ǿ���ע�͵� */
void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

/* �Լ�д�ĺ�����ȫ�ֱ��� */
extern uint8_t time_dec[]; //��д������ͨ��ʮ����
extern uint8_t time_hex[]; //д����ͨ��ʮ������
void write_time(void);
void read_time(void);

#endif
