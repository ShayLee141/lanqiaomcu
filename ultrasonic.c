#include "ultrasonic.h"

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
	CCON = 0;	//初始化PCA控制寄存器
						//PCA定时器停止
						//清除CF标志
						//清除模块中断标志
	CMOD = 0x01;		//设置PCA时钟源 1MHz
									//开启PCA定时器溢出中断
	CCAPM0 = 0x11;	//PCA模块0为下降沿触发
									//利用脉冲捕获来模拟P11下降沿外部中断
}

uint16_t distance = 0;
/* ultrasonic_flag 超声波状态标志
	 0：空闲阶段
	 1：发送阶段
	 2：确认接收到返回的超声波
	 3：接收不到超声波，或超出量程 */
uint8_t ultrasonic_flag = 0;
static const uint8_t CH_DEF = 0;
static const uint8_t CL_DEF = 0;

/* P11，即超声波RX，一旦出现下降沿，
	 则说明已经收到返回的超声波，进入此中断 */
void TimerPCAIsr() interrupt 7
{
	CR = 0; //优先结束计时
	
	/* 清除标志位，必须把标志位清除，不然会卡死 */
	if (CCF0)
	{
		CCF0 = 0;
		ultrasonic_flag = 2;
	}
	
	if (CF)
	{
		CF = 0;
		ultrasonic_flag = 3;
	}
	
	CCF1 = 0; //这两个标志位虽然没有用到
	CCF2 = 0; //但是以防万一，导致中断卡住
}

/* 超声波发送 */
void sand_ultrasonic()
{ //计时在前还是发送在前都可以，在我的板子上我发现计时在前的数据比较准确
	/* 启动计时 */
	CL = 0x1C;		//设置定时初始值
	CH = 0x8D;		//设置定时初始值
	/* 经过测试，板子能测的最长距离是4米（至少我这块板子是这样），
		 而定时器的理论量程远超4米，所以设置初值的意义在于缩短量程
		 （大概缩减到5米），以此来减少不必要的测量时间，
		 所以超声波传播的时间就是终值减初值 */
	CF = 0;		//清除CF标志
	EA = 0; //我们有个定时器是100微秒的，对这个一定会有影响，所以要先关闭中断
	
	/* 启动超声波发送 */
	CR = 1;		//定时器开始计时
	P10 = 1; Delay13us(); P10 = 0; Delay13us(); //高电平在前低电平在后
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0;
	
	EA = 1; //重新打开中断
	
	ultrasonic_flag = 1; //进入发送阶段
}

/* 计算距离 */
void calculate_distance()
{
	if (ultrasonic_flag < 2) //没有完成发送之前不能计算
		return ;
	
	if (ultrasonic_flag == 2) //确认接收到返回的超声波
	{
		distance = CCAP0H;
		distance <<= 8;
		distance |= CCAP0L;
		distance -= 0x8D1C; //减掉定时器初值，减去初值原因在设置初值的地方有注解
		distance = (float)distance * 0.017;
	}
	else //接收不到超声波，或超出量程
	{
		distance = DISTANCE_OUTRANG;
	}
	
	ultrasonic_flag = 0;
}