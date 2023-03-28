/*	# 	单总线代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/

#include "onewire.h"

sbit DQ = P1^4;

//
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		EA = 0; //------------关中断
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
		EA = 1; //------------开中断
	}
	Delay_OneWire(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		EA = 0; //------------关中断
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
		EA = 1; //------------开中断
	}
	return dat;
}

//
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
		EA = 0; //------------关中断
  	Delay_OneWire(10); 
    initflag = DQ;    
		EA = 1; //------------开中断
  	Delay_OneWire(5);
  
  	return initflag;
}

/* -------------------------------------------- */
uint16_t temperature = 0;
void read_temp(void)
{
	uint16_t temp_H, temp_L;
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	temp_L = Read_DS18B20();
	temp_H = Read_DS18B20();
	
	temperature = (float)(temp_H << 8 | temp_L) * 6.25;
}
