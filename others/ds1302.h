#ifndef __DS1302_H__
#define __DS1302_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

#include "myint.h"

extern uint8_t time_10[];

void write_ds1302(void);
void read_ds1302(void);

#endif
