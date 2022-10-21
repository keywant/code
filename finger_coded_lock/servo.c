#include "head.h"



sbit  Car_Servo = P2^0;      // 舵机pwm控制
u16 Servo0PwmDuty = 600;	//PWM脉冲宽度   1.5ms脉冲宽度  为舵机正中位置

void Timer0Init(void)		//20毫秒@12.000MHz
{
	//AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xE0;		//设置定时初始值
	TH0 = 0xB1;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时

	ET0 = 1;			//开定时器0中断
	EA = 1;       //开总中断
}
void Car_Servo_Init()	//舵机初始化
{
	Timer0Init();
}

void Timer0Value(u16 pwm)
{
	
	u16 value;
	value=0xffff-pwm;	
	TR0 = 0;
	TL0 = value;			//16位数据给8位数据赋值默认将16位数据的低八位直接赋给八位数据
  TH0 = value >> 8;		//将16位数据右移8位，也就是将高8位移到低八位，再赋值给8位数据	
	TR0 = 1;
}
void Timer0_isr(void) interrupt 1 using 1
{
	
	static u8 i = 1;	//静态变量：每次调用函数时保持上一次所赋的值，
							//跟全局变量类似，不同是它只能用于此函数内部
	
	switch(i)
	{
		case 1:
			Car_Servo = 1;	//PWM控制脚高电平
			//给定时器0赋值，计数Pwm0Duty个脉冲后产生中断，下次中断会进入下一个case语句
			Timer0Value(Servo0PwmDuty);	
			break;
		case 2:
			Car_Servo = 0;	//PWM控制脚低电平
			//高脉冲结束后剩下的时间(20000-Pwm0Duty)全是低电平了，Pwm0Duty + (20000-Pwm0Duty) = 20000个脉冲正好为一个周期20毫秒 
			Timer0Value(20000-Servo0PwmDuty);
			i = 0;	
			break;	 
	}
	i++;
}