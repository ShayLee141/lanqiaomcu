#ifndef __MAIN_H__
#define __MAIN_H__

typedef unsigned char uint8_t; //8位无符号整形
typedef unsigned int uint16_t; //16位无符号整形
typedef unsigned long uint32_t; //32位无符号整形

typedef signed char int8_t; //8位整形
typedef int int16_t; //16位整形
typedef long int32_t; //16位整形

/* 按键配置 */
/* 只要在这里启用或者注释宏定义，就可以实现相应的功能就开关 */
//#define MM_MODE //使用MM模式时启用，此时删除对P42的输出操作，矩阵按键第2列失效
//#define USE_UART //使用串口时启用，此时删除对P30和P31的输出操作，矩阵按键1、2行失效
#define USE_NE555 //需要ne555测频时启用，此时删除对P34的输出操作，矩阵按键第4列失效
#define KEY_RELEASE //需要检测松手时启用，按键松手瞬间可以使按键状态返回特定值
/* 三种条件可以同时启用，互不干扰，
	 此时按键只有3、4行和1、2、3列有效，
	 并且能返回松手按键状态，
	 但是真的会考那么复杂的按键吗？（笑哭）
	 多读几次，把代码理解透彻，
	 如果比赛用不到相应功能是可以之间不写那一段的 */

#endif
