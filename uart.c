#include "uart.h"

bit busy = 0;

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

uint8_t cmd = 0;
/* 0����ָ��
	 1��ָ��1
	 2��ָ��2
	 ...
	 n��ָ��n */
uint8_t code cmd[CMD_NUM][CMD_LEN] = 
{
	{ "Led" },					//ָ��1
	{ "Dig" },					//ָ��2
	{ "Beep" },					//ָ��3
	{ "Relay" },				//ָ��4
	{ "Return" },				//ָ��5
}

void Uart() interrupt 4
{
	uint8_t i, j, len, buf;
	
	if (RI)
	{
		RI = 0;                 //���RIλ
		buf = SBUF;
		
		for (i = 0; i < CMD_NUM; i++)
		{
			for (j = 0, len = strlen(cmd[i]); j < len; j++)
			{
				if (buf != cmd[i][j])
					break;
			}
			
			if (j == len)
			{
				cmd = i + 1;
				break;
			}
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
