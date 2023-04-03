/*	#   I2C代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/

#include "iic.h"

#define DELAY_TIME	5

sbit scl = P2^0;
sbit sda = P2^1;

//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//
void I2CStart(void)
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//
void I2CStop(void)
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

// 发送字节
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

// 接收字节
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

// 等待回应
unsigned char I2CWaitAck(void)
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

// 发送回应
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}

/* ------------------at24c02----------------- */
void e2prom_write(uint8_t addr, uint8_t *Data, uint8_t len)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	while(len--)
	{
		I2CSendByte(*Data);
		I2CWaitAck();
		Data++;
	}
	I2CStop();
}

void e2prom_read(uint8_t addr, uint8_t *Data, uint8_t len)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	
	loop:
	*Data = I2CReceiveByte();
	if (--len)
	{
		I2CSendAck(0); //发送应答
		Data++;
		goto loop;
	}
	I2CSendAck(1); //发送非应答
	I2CStop();
}

/* ------------------pcf8591----------------- */
uint8_t control_byte = 0x04;
uint8_t dac_level = 0; //当前dac输出的等级，输出前先修改这个值再调用输出函数
uint8_t adc_level[4] = { 0 };

void dac_out(void)
{
	control_byte |= 0x40; //开启dac输出
	
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	I2CSendByte(dac_level);
	I2CWaitAck();
	I2CStop();
}

void dac_stop(void)
{
	control_byte &= ~0x40; //关闭dac输出
	
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	I2CStop();
}

void adc_read(void)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	
	I2CReceiveByte(); //第一次读取的数据不要使用
	I2CSendAck(0); //发送应答
	adc_level[0] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[1] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[2] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[3] = I2CReceiveByte();
	I2CSendAck(1); //发送非应答
	I2CStop();
}
