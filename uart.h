#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <string.h>

#include <STC15F2K60S2.H>
#include "intrins.h"

#include "myint.h"

#define CMD_NUM 5		//CMD_NUM = 指令数量
#define CMD_LEN 10	//CMD_LEN = 最长的指令长度 + 1

bit busy = 0;
uint8_t cmd = 0;

void UartInit(void);
void SendData(uint8_t dat);
void SendString(char *s);

#endif
