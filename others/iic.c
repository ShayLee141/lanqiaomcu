#include "iic.h"

sbit SDA = P2^1; //�������Ǵ�h�ļ��ƹ�����
sbit SCL = P2^0;

#define DELAY_TIME 5

//I2C�����ڲ���ʱ����
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C���������ź�
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C����ֹͣ�ź�
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//����Ӧ����Ӧ���ź�
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

//�ȴ�Ӧ��
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

//I2C���߷���һ���ֽ�����
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

//I2C���߽���һ���ֽ�����
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

/**** ������ ���ϴ����δ�޸ģ��������Լ��ӵ� ������ ********/

/****************** ������ PCF8591 ������ ******************/
uint8_t dac_level = 0; //��ǰdac����ȼ�
uint8_t control_byte = 0x04;
//�й�������������ã����PCF0591�������ֲᣬCONTROL BYTE

/* dac��� */
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

/* ֹͣdac��� */
void stop_dac(void)
{
	control_byte &= ~0x40;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(control_byte);
	IIC_Stop();
}

uint8_t adc_level[4] = { 0 }; //��Ӧ4��adcͨ��
/* adc�ɼ� */
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
	
	IIC_RecByte(); //Ԥ��
	IIC_SendAck(0);
	//ADCÿһ�ζ���������һ�β�����ֵ��
	//������Ϊ���ǿ�����ͨ��ͨ���������Ķ�����
	//�ᵼ�·��ص�ֵ����������Ҫ��ͨ����
	//���������Ԥ����Ϊ����оƬ������������������
	
	adc_level[0] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[1] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[2] = IIC_RecByte();
	IIC_SendAck(0);
	adc_level[3] = IIC_RecByte();
	IIC_Stop(); //���һ�ζ�����ֱ�ӽ��������跢Ӧ���ź�
}
/****************** ������ PCF8591 ������ ******************/

/****************** ������ AT24C02 ������ ******************/
/* ���ֽ�д�루��������������ɣ�ʡ���������ĵ��ֽڲ�����
   addr��	Ŀ���ַ
   Data��	д�����ݵ�ָ��
   ����ʵDataֱ���ñ���������ָ��Ҳ�У�ֻ��Ϊ�˺ͺ����ͳһ�� */
void write_e2prom_single_byte(uint8_t addr, uint8_t *Data)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //ȷ��Ŀ���ַ
	IIC_WaitAck();
	
	IIC_SendByte(*Data); //д������
	IIC_WaitAck();
	
	IIC_Stop();
}

/* ���ֽڶ�ȡ����������������ɣ�ʡ���������ĵ��ֽڲ�����
   addr��	Ŀ���ַ
   Data����ȡ���ݵ�ָ��
   �����Dataֻ����ָ�룬���ܸĳɱ����� */
void read_e2prom_single_byte(uint8_t addr, uint8_t *Data)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //ȷ��Ŀ���ַ
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	
	*Data = IIC_RecByte(); //��ȡ����
	
	IIC_Stop();
}

/* ���ֽ�д�루ѧ��������ֱ���������������ȡ�����ֽ�д�룬��ʵҲ��ûʲô�Ѷȵģ�����ѭ������Ҫ���һ�£�
   addr��	Ŀ���ַ
   Data��	д�����ݵ�ָ��
   len��	д���ֽ��� */
void write_e2prom(uint8_t addr, uint8_t *Data, uint8_t len)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //ȷ��Ŀ���ַ
	IIC_WaitAck();
	
	while(len--)
	{
		IIC_SendByte(*Data);
		IIC_WaitAck();
		Data++; //ָ���Լӣ���ָ��ĵ�ַ���������һ���ֽ�
	}
	
	IIC_Stop();
}

/* ���ֽڶ�ȡ��ѧ��������ֱ���������������ȡ�����ֽڶ�ȡ����ʵҲ��ûʲô�Ѷȵģ�����ѭ������Ҫ���һ�£�
   addr��	Ŀ���ַ
   Data��	��ȡ���ݵ�ָ��
   len��	��ȡ�ֽ��� */
void read_e2prom(uint8_t addr, uint8_t *Data, uint8_t len)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr); //ȷ��Ŀ���ַ
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	
	loop: //�������goto�������ѭ����˼����ȥ����goto�����ѭ�����
	*Data = IIC_RecByte(); //��ȡ����
	if (--len) //���һ�ν��ղ���Ҫ��Ӧ�����ֱ��stop
	{
		IIC_SendAck(0);
		Data++;
		goto loop;
	}
	
	IIC_Stop();
}
