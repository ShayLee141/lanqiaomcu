#include "ultrasonic.h"

uint8_t ultrasonic_busy = 0;

/* 如果你发现测距的量程很短，那你应该看看
	 下载的时候IRC频率有没有调成12.000MHz，
	 而不是来质疑我的代码 (>w<) */
void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);
}

void TimerPCAInit()
{
	CR = 0;		//停止定时器PCA
	CCON = 0;	//初始化PCA控制寄存器
						//PCA定时器停止
						//清除CF标志
						//清除模块中断标志
	CMOD = 0x01;		//设置PCA时钟源 1MHz
									//开启PCA定时器溢出中断
	CCAPM0 = 0x11;	//PCA模块0为下降沿触发
									//利用脉冲捕获来模拟P11下降沿外部中断
}

/* P11，即超声波RX，一旦出现下降沿，
	 则说明已经收到返回的超声波，进入此中断 */
void TimerPCAIsr() interrupt 7
{
	CR = 0; //优先结束计时
	
	/* 清除标志位 */
	if (CCF0)
		CCF0 = 0;
	if (CCF1)
		CCF1 = 0;
	if (CCF2)
		CCF2 = 0;
	//这里没有清除CF标志位是以为要在read_distance函数中判断是否溢出
	
	ultrasonic_busy = 0;
}

/* 超声波测距函数
	 每一次调用会先对上一次测距的数值进行换算，
	 再发送一组新的超声波，随后返回换算的值，
	 若调用的时候上一次测距未完成，则会直接跳过，
	 所以理论上可以连续调用不用间隔时间，
	 出于安全和全局考虑，还是建议读取间隔至少为100ms，
	 可根据需要更改（100ms效果已经很好了，太快没啥用哈哈） */
uint16_t read_distance()
{
	static uint16_t distance = 0;
	/* 实际上distance不可能为0，所以用0作为初值，
		 可以识别是否为开机第一次测距离 */
	
	if (!ultrasonic_busy) //如果正处于发送阶段则直接跳过
	{
		/* 距离换算 */
		if (distance)
		/* 如果是开机第一次测距，则会跳过if里的内容，
			 执行后面else里的distance = 1，确保第一次测距不会
			 进行距离换算，进而不会得到错误的结果 */
		{
			if (CF) //如果计数溢出了表面超出量程了
			{
				CF = 0;
				distance = DISTANCE_OUTRANG;
			}
			else //没有超量程则要进行
			{
				led_state |= 0x80;
				distance = CCAP0H;
				distance <<= 8;
				distance |= CCAP0L;
				distance -= 0x8D1C; //减掉定时器初值，减去初值原因在设置初值的地方有注解
				distance = (float)distance * 0.017;
			}
		}
		else
			distance = 1;
		
		/* 启动超声波发送 */
		EA = 0; //我们有个定时器是10微秒的，对这个一定会有影响，所以要先关闭中断
		P10 = 1; Delay13us(); P10 = 0; Delay13us(); //高电平在前低电平在后
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0; Delay13us();
		P10 = 1; Delay13us(); P10 = 0;
		EA = 1; //不关中断会减少量程
		
		/* 启动定时器 */
		CL = 0x1C;		//设置定时初始值
		CH = 0x8D;		//设置定时初始值
		/* 经过测试，板子能测的最长距离是4米（至少我这块板子是这样），
		   而定时器的理论量程远超4米，所以设置初值的意义在于缩短量程
			 （大概缩减到5米），以此来减少不必要的测量时间，
			 所以超声波传播的时间就是终值减初值 */
		CF = 0;		//清除CF标志
		CR = 1;		//定时器开始计时
		
		ultrasonic_busy = 1; //只有中断7才能重新置零
	}
	
	return distance;
}
