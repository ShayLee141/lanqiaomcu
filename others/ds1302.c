/*	# 	DS1302����Ƭ��˵��
	1. 	���ļ������ṩ���������빩����ѡ����ɳ�����Ʋο���
	2. 	����ѡ�ֿ������б�д��ش�����Ըô���Ϊ������������ѡ��Ƭ�����͡������ٶȺ�����
		�жԵ�Ƭ��ʱ��Ƶ�ʵ�Ҫ�󣬽��д�����Ժ��޸ġ�
*/								

#include "ds1302.h"

sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;

//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

/* ------------------------------------- */
#define DATA_NUM 3 //���ݸ���

uint8_t time_10[DATA_NUM] = { 23, 59, 55, };
static uint8_t time_16[DATA_NUM] = { 0x23, 0x59, 0x55, };

void time_10_to_16(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_16[num] = time_10[num] / 10 * 0x10 + time_10[num] % 10;
}

void time_16_to_10(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_10[num] = time_16[num] / 0x10 * 10 + time_16[num] % 0x10;
}

void write_ds1302(void)
{
	time_10_to_16();
	
	Write_Ds1302_Byte(0x84, time_16[0]); // ʱ
	Write_Ds1302_Byte(0x82, time_16[1]); // ��
	Write_Ds1302_Byte(0x80, time_16[2]); // ��
}

void read_ds1302(void)
{
	time_16[0] = Read_Ds1302_Byte(0x85); // ʱ
	time_16[1] = Read_Ds1302_Byte(0x83); // ��
	time_16[2] = Read_Ds1302_Byte(0x81); // ��
	
	time_16_to_10();
}
