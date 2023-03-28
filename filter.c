#include "filter.h"

void swap(uint16_t *a, uint16_t *b)
{
	uint16_t c = *a;
	*a = *b;
	*b = c;
}

/* ֻ��һ���ܼ�ª����ֵ�˲����������ݼ�����λ����
	 ������ע�⣬���������ֻ����3�������ܶ�Ҳ�����٣�
	 ��Ȼ��ª�����������¶ȡ���������adc��Ƶ�ʵĶ�ȡ��
	 Ч�����Ǻ��ȶ��ģ����Դ����������
	 �������ż����ȡ��������� */
uint16_t median_filter(uint16_t *Data)
{
	uint16_t temp1;
	uint16_t temp2;
	
	temp1 = MAX(*(Data + 0), *(Data + 1));
	if (temp1 <= *(Data + 2))
		return temp1;
	else
	{
		temp2 = MIN(*(Data + 0), *(Data + 1));
		return MAX(temp2, *(Data + 2));
	}
}

///* �򵥵�ð������ */
//void sort(uint16_t *Data, uint16_t len)
//{
//	int i, j;
//	
//	for (i = 0; i < len; i++)
//	{
//		for (j = i + 1; j < len; j++)
//		{
//			if (*(Data + i) > *(Data + j))
//				swap((Data + i), (Data + j));
//		}
//	}
//}

///* ֻ��һ���򵥵���ֵ�˲���������Щ���ݵ���λ�� */
//uint16_t median_filter(uint16_t *Data, uint16_t len)
//{
//	uint16_t Data_bak[20];
//	uint16_t Data_out;
//	uint32_t sum = 0;
//	
//	memcpy(Data_bak, Data, sizeof(uint16_t) * len);
//	
//	sort(Data_bak, len);
//	
//	Data_out = Data_bak[len - (len / 2)];
//	
//	return Data_out;
//}

/* ֻ��һ���򵥵ľ�ֵ�˲�������ƽ��ֵ */
uint16_t average_filter(uint16_t *Data, uint16_t len)
{
	uint32_t sum = 0;
	uint16_t i;
	
	for (i = 0; i < len; i++)
		sum += *(Data + i);
	
	return sum / len;
}
