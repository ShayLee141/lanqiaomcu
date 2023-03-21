#ifndef __IIC_H_
#define __IIC_H_

//#include <reg52.h> 注释掉这句
#include "intrins.h"
#include <STC15F2K60S2.H> //加上这句

#include "myint.h" //加上这句

/* 这几句函数声明没有改，但是可以注释掉 */
void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

/********** ↓↓↓ 自己写的函数 ↓↓↓ **********/
/*** ↓↓↓ PCF8591 ↓↓↓ ***/
extern uint8_t dac_level;
extern uint8_t adc_level[];
void write_dac(void);
void stop_dac(void);
void read_adc(void);

/*** ↓↓↓ AT24C02 ↓↓↓ ***/
/* 单字节 */
void write_e2prom_single_byte(uint8_t addr, uint8_t *Data);
void read_e2prom_single_byte(uint8_t addr, uint8_t *Data);
/* 多字节 */
void write_e2prom(uint8_t addr, uint8_t *Data, uint8_t len);
void read_e2prom(uint8_t addr, uint8_t *Data, uint8_t len);
	
#endif