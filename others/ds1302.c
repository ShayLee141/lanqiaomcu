#include "ds1302.h"  									

sbit SCK = P1^7; //�������Ǵ�h�ļ��ƹ�����
sbit SDA = P2^3;		
sbit RST = P1^3; 

//д�ֽ�
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

//��DS1302�Ĵ���д������
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//��DS1302�Ĵ�����������
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

/* ���ϵ��ļ����ݾ�δ�޸ģ���������Ҽӵ� */
//                     ʱ  ��  ��  ��  ��  �� ����
uint8_t time_dec[] = { 23, 59, 55, 03, 16, 23, 04, }; //ʮ����
uint8_t time_hex[] = { 0x23, 0x59, 0x55, 0x03, 0x16, 0x23, 0x04, }; //ʮ������

/* ʮ����תʮ������ */
void time_dec_to_hex()
{
	uint8_t i = 6;
	while(i--)
	{
		time_hex[i] = (time_dec[i] / 10) * 0x10 + time_dec[i] % 10;
	}
}

/* ʮ������תʮ���� */
void time_hex_to_dec()
{
	uint8_t i = 6;
	while(i--)
	{
		time_dec[i] = (time_hex[i] / 0x10) * 10 + time_hex[i] % 0x10;
	}
}

void write_time(void)
{
	time_dec_to_hex();
	
	Write_Ds1302_Byte(0x8E, 0x00); //����д����
	
	Write_Ds1302_Byte(0x80, time_hex[2]); //��
	Write_Ds1302_Byte(0x82, time_hex[1]); //��
	Write_Ds1302_Byte(0x84, time_hex[0]); //ʱ
	Write_Ds1302_Byte(0x86, time_hex[4]); //��
	Write_Ds1302_Byte(0x88, time_hex[3]); //��
	Write_Ds1302_Byte(0x8A, time_hex[6]); //����
	Write_Ds1302_Byte(0x8C, time_hex[5]); //��
	
	Write_Ds1302_Byte(0x8E, 0x80); //�ָ�д����
}

void read_time(void)
{
	time_hex[2] = Read_Ds1302_Byte(0x81); //��
	time_hex[1] = Read_Ds1302_Byte(0x83); //��
	time_hex[0] = Read_Ds1302_Byte(0x85); //ʱ
	time_hex[4] = Read_Ds1302_Byte(0x87); //��
	time_hex[3] = Read_Ds1302_Byte(0x89); //��
	time_hex[6] = Read_Ds1302_Byte(0x8B); //����
	time_hex[5] = Read_Ds1302_Byte(0x8D); //���� //��
	
	time_hex_to_dec();
}
