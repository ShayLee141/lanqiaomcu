#ifndef __FILTER_H__
#define __FILTER_H__

#include <stdlib.h>
#include <string.h>

#include "myint.h"

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

void sort(uint16_t *Data, uint16_t len);

uint16_t median_filter(uint16_t *Data);
uint16_t average_filter(uint16_t *Data, uint16_t len);

#endif
