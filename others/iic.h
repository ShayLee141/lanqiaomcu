#ifndef __IIC_H__
#define __IIC_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

#include "main.h"

extern uint8_t control_byte;
extern uint8_t dac_level;
extern uint8_t adc_level[4];

void e2prom_write(uint8_t addr, uint8_t *Data, uint8_t len);
void e2prom_read(uint8_t addr, uint8_t *Data, uint8_t len);

void dac_out(void);
void dac_stop(void);
void adc_read(void);

#endif
