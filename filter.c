#include "filter.h"

void swap(uint16_t *a, uint16_t *b)
{
	uint16_t c = *a;
	*a = *b;
	*b = c;
}

/* 只是一个很简陋的中值滤波，返回数据集的中位数，
	 不过请注意，传入的数据只能是3个，不能多也不能少，
	 虽然简陋，但是用在温度、超声波、adc、频率的读取，
	 效果都是很稳定的，可以从软件层面上
	 解决数据偶尔读取错误的问题 */
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

///* 简单的冒泡排序 */
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

///* 只是一个简单的中值滤波，返回这些数据的中位数 */
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

/* 只是一个简单的均值滤波，计算平均值 */
uint16_t average_filter(uint16_t *Data, uint16_t len)
{
	uint32_t sum = 0;
	uint16_t i;
	
	for (i = 0; i < len; i++)
		sum += *(Data + i);
	
	return sum / len;
}
