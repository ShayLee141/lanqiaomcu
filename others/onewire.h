#ifndef __ONEWIRE_H
#define __ONEWIRE_H

//#include "reg52.h" 注释掉这句
#include <STC15F2K60S2.H> //加上这句

#include "base.h"

#include "myint.h" //加上这句

sbit DQ = P1^4;  

/*    因为我希望这个函数将返回温度的一百倍，
   以便显示小数温度而不使用float类型，
	 所以类型是int16_t */
extern int16_t temperature;

/* 这里他给了一个未定义的函数，
   那我们就将计就计，完善这个函数，
   但把原先的unsigned char改成void
   我的温度使用上面的全局变量来输出 */
void rd_temperature(void);  

#endif
