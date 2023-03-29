#include "uart.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xC7;			//���ö�ʱ��ʼֵ
	T2H = 0xFE;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES = 1;					//ʹ�ܴ���1�ж�
}

bit busy = 0;
uint8_t cmd = 0;

void Uart() interrupt 4
{
	uint8_t buf;

	if (RI)
	{
		RI = 0;                 //���RIλ
		buf = SBUF;
		
		switch (buf)
		{
			case 'A':
				cmd |= 0x01;
			break;
			
			case 'B':
				cmd |= 0x02;
			break;
			
			case 'C':
				cmd |= 0x04;
			break;
		}
	}
	
	if (TI)
	{
		TI = 0;                 //���TIλ
		busy = 0;               //��æ��־
	}
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(uint8_t dat)
{
	while (busy);               //�ȴ�ǰ������ݷ������
	busy = 1;
	SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}
