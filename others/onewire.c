#include "onewire.h"

//单总线内部延时函数
void Delay_OneWire(unsigned int t)  
{
	t *= 8; //这个是我加的，原代码是在51单片机的环境下写的，15单片机速度要快，所以延时需要的数值要增加
	while(t--);
}

//单总线写操作
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		EA = 0; //为了确保传输不被中断打断要在这里关中断
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		EA = 1; //重新打开中断
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//单总线读操作
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		EA = 0; //为了确保传输不被中断打断要在这里关中断
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		Delay_OneWire(1);
		if(DQ)
		{
			dat |= 0x80;
		}
		EA = 1; //重新打开中断
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
		EA = 0; //为了确保传输不被中断打断要在这里关中断
  	Delay_OneWire(10); 
    initflag = DQ;     
		EA = 1; //重新打开中断
  	Delay_OneWire(5);
  
  	return initflag;
}

int16_t temperature = -11451;

void rd_temperature(void)
{
	uint8_t temp[2];
	
	if (init_ds18b20())
		return ; //初始化不成功就直接返回
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	if (init_ds18b20())
		return ; //初始化不成功就直接返回
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	temp[1] = Read_DS18B20(); //低八位
	temp[0] = Read_DS18B20(); //高八位
	
	temperature = (float)(*(int16_t*)temp) * 6.25F; //看不懂吧，看不懂就对咯
	/* temp是个uint8_t类型的地址，
	   广义来说也可以叫uint8_t指针常量（固定指向一个地址的指针），
	   (uint16_t*)可以temp强制转换成int16_t指针常量，
	   *即取值，意思是将temp数组的两个成员直接按int16_t的方式读取，
	   之后在强制转换为float方便进行下一步浮点乘法运算，
	   正常情况下读到的值要乘0.0625才能得到摄氏温度，
	   但我想把结果扩大100倍就不用以float类型保存了，
	   所以乘的是6.25，F是指定这个数值是浮点数，
	   最后赋值给int16_t类型变量temperature，
	   直接把float转化为int16_t类型了 */
}
