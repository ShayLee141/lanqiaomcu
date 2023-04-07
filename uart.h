#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <string.h>

#include <STC15F2K60S2.H>
#include "intrins.h"

#include "main.h"

#include "base.h"

extern bit busy;
extern uint8_t cmd;

void UartInit(void);
void SendData(uint8_t dat);
void SendString(char *s);

#endif
