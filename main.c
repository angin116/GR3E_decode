/*
  硬件和协议相关介绍请看本工程的README。
 */

#include "12C5A60S2.h"

sbit test = P2^0;

unsigned char mk[3];	// 三个通道的脉宽信息

void sendB(unsigned char dat)
{
    test = 1;
    SBUF=dat;
    while(!TI);
    TI=0;
    test = 0;
}

void t0() interrupt 1
{
    static unsigned long num=0;
    if(num++ == 100000)
    {
	num = 0;
	sendB(0x55);
    }
    test = 0;
}

void main()
{
    // 初始化中断
    ET0 = 1;
    ES=0;	//关串口中断，在主循环中用查询方式发送数据
    EA = 1;
    
    // 初始化串口，波特率115200
    AUXR|=0x40;//定时器1工作在1T模式
    TL1 = 0xCC;		//设定定时初值
    TH1 = 0xFF;		//设定定时初值
    TMOD&=0x0f;//清除高4位
    TMOD|=0x20;//t1工作于方式2
    TR1=1;
    SM0=0;
    SM1=1;
    REN=1;
    ET1 = 0;

    // 初始化T0，中断周期20us
    AUXR &= 0x7F;		//定时器0工作在12T模式
    TMOD &= 0xF0; TMOD |= 0x02;	//定时器0工作在8为自动填装模式
    TL0 = 216;
    TH0 = 216;
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时

    while(1)
    {
    }
}
