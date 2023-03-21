#include "onewire.h"

//�������ڲ���ʱ����
void Delay_OneWire(unsigned int t)  
{
	t *= 8; //������Ҽӵģ�ԭ��������51��Ƭ���Ļ�����д�ģ�15��Ƭ���ٶ�Ҫ�죬������ʱ��Ҫ����ֵҪ����
	while(t--);
}

//������д����
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		EA = 0; //Ϊ��ȷ�����䲻���жϴ��Ҫ��������ж�
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		EA = 1; //���´��ж�
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//�����߶�����
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		EA = 0; //Ϊ��ȷ�����䲻���жϴ��Ҫ��������ж�
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		Delay_OneWire(1);
		if(DQ)
		{
			dat |= 0x80;
		}
		EA = 1; //���´��ж�
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
		EA = 0; //Ϊ��ȷ�����䲻���жϴ��Ҫ��������ж�
  	Delay_OneWire(10); 
    initflag = DQ;     
		EA = 1; //���´��ж�
  	Delay_OneWire(5);
  
  	return initflag;
}

int16_t temperature = -11451;

void rd_temperature(void)
{
	uint8_t temp[2];
	
	if (init_ds18b20())
		return ; //��ʼ�����ɹ���ֱ�ӷ���
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	if (init_ds18b20())
		return ; //��ʼ�����ɹ���ֱ�ӷ���
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	temp[1] = Read_DS18B20(); //�Ͱ�λ
	temp[0] = Read_DS18B20(); //�߰�λ
	
	temperature = (float)(*(int16_t*)temp) * 6.25F; //�������ɣ��������ͶԿ�
	/* temp�Ǹ�uint8_t���͵ĵ�ַ��
	   ������˵Ҳ���Խ�uint8_tָ�볣�����̶�ָ��һ����ַ��ָ�룩��
	   (uint16_t*)����tempǿ��ת����int16_tָ�볣����
	   *��ȡֵ����˼�ǽ�temp�����������Աֱ�Ӱ�int16_t�ķ�ʽ��ȡ��
	   ֮����ǿ��ת��Ϊfloat���������һ������˷����㣬
	   ��������¶�����ֵҪ��0.0625���ܵõ������¶ȣ�
	   ������ѽ������100���Ͳ�����float���ͱ����ˣ�
	   ���Գ˵���6.25��F��ָ�������ֵ�Ǹ�������
	   ���ֵ��int16_t���ͱ���temperature��
	   ֱ�Ӱ�floatת��Ϊint16_t������ */
}
