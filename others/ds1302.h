#ifndef __DS1302_H_
#define __DS1302_H_

//#include <reg52.h> 注释掉这句
#include <intrins.h>
#include <STC15F2K60S2.H> //加上这句

#include "myint.h" //加上这句

/* 这几句没有改，但是可以注释掉 */
void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

/* 自己写的函数和全局变量 */
extern uint8_t time_dec[]; //读写都可以通过十进制
extern uint8_t time_hex[]; //写不能通过十六进制
void write_time(void);
void read_time(void);

#endif
