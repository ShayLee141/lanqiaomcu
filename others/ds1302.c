/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
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
#define DATA_NUM 3 //数据个数

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
	
	Write_Ds1302_Byte(0x84, time_16[0]); // 时
	Write_Ds1302_Byte(0x82, time_16[1]); // 分
	Write_Ds1302_Byte(0x80, time_16[2]); // 秒
}

void read_ds1302(void)
{
	time_16[0] = Read_Ds1302_Byte(0x85); // 时
	time_16[1] = Read_Ds1302_Byte(0x83); // 分
	time_16[2] = Read_Ds1302_Byte(0x81); // 秒
	
	time_16_to_10();
}
