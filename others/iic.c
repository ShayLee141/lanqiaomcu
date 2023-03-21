#include "iic.h"

sbit SDA = P2^1; //这两句是从h文件移过来的
sbit SCL = P2^0;

#define DELAY_TIME 5

//I2C总线内部延时函数
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C总线启动信号
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C总线停止信号
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答或非应答信号
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//I2C总线发送一个字节数据
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//I2C总线接收一个字节数据
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}

/**** ↑↑↑ 以上代码均未修改，下面是自己加的 ↓↓↓ ********/

/****************** ↓↓↓ PCF8591 ↓↓↓ ******************/
uint8_t dac_level = 0; //当前dac输出等级
uint8_t control_byte = 0x04;
//有关这个变量的设置，请见PCF0591的数据手册，CONTROL BYTE

/* dac输出 */
void write_dac(void)
{
	control_byte |= 0x40;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(control_byte);
	IIC_WaitAck();
	IIC_SendByte(dac_level);
	IIC_Stop();
}

/* 停止dac输出 */
void stop_dac(void)
{
	control_byte &= ~0x40;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(control_byte);
	IIC_Stop();
}

uint8_t adc_level[4] = { 0 }; //对应4个adc通道
/* adc采集 */
void read_adc(void)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(control_byte);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	
	IIC_RecByte(); //预读
	IIC_SendAck(0);
	//ADC每一次读数都是上一次采样的值，
	//并且因为我们开启了通道通道数自增的读法，
	//会导致返回的值不是我们想要的通道，
	//所以这里的预读仅为了让芯片采样，而不保留读数
	
	adc_level[0] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[1] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[2] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[3] = IIC_RecByte();
	IIC_Stop(); //最后一次读可以直接结束，无需发应答信号
}
/****************** ↑↑↑ PCF8591 ↑↑↑ ******************/

/****************** ↓↓↓ AT24C02 ↓↓↓ ******************/
/* 单字节写入（新手掌握这个即可，省赛多数考的单字节操作）
   addr：	目标地址
   Data：	写入数据的指针
   （其实Data直接用变量，不用指针也行，只是为了和后面的统一） */
void write_e2prom_single_byte(uint8_t addr, uint8_t *Data)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //确定目标地址
	IIC_WaitAck();
	
	IIC_SendByte(*Data); //写入数据
	IIC_WaitAck();
	
	IIC_Stop();
}

/* 单字节读取（新手掌握这个即可，省赛多数考的单字节操作）
   addr：	目标地址
   Data：读取数据的指针
   （这个Data只能用指针，不能改成变量） */
void read_e2prom_single_byte(uint8_t addr, uint8_t *Data)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //确定目标地址
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	
	*Data = IIC_RecByte(); //读取数据
	
	IIC_Stop();
}

/* 多字节写入（学有余力的直接掌握这个，可以取代单字节写入，其实也是没什么难度的，就是循环那里要理解一下）
   addr：	目标地址
   Data：	写入数据的指针
   len：	写入字节数 */
void write_e2prom(uint8_t addr, uint8_t *Data, uint8_t len)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //确定目标地址
	IIC_WaitAck();
	
	while(len--)
	{
		IIC_SendByte(*Data);
		IIC_WaitAck();
		Data++; //指针自加，所指向的地址则会跳到下一个字节
	}
	
	IIC_Stop();
}

/* 多字节读取（学有余力的直接掌握这个，可以取代单字节读取，其实也是没什么难度的，就是循环那里要理解一下）
   addr：	目标地址
   Data：	读取数据的指针
   len：	读取字节数 */
void read_e2prom(uint8_t addr, uint8_t *Data, uint8_t len)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //确定目标地址
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	
	loop: //和下面的goto结合做成循环，思来想去还是goto做这个循环最简单
	*Data = IIC_RecByte(); //读取数据
	if (--len) //最后一次接收不需要发应答可以直接stop
	{
		IIC_SendAck(0);
		Data++;
		goto loop;
	}
	
	IIC_Stop();
}
