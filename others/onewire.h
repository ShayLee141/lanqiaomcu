#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

#include "myint.h"

extern uint16_t temperature;

void read_temp(void);

#endif
