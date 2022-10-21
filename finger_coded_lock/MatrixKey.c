#include "head.h"

u8 KeyValue; //按键值
char fla = -1; //按键按下一次信号
extern MODE;
void key_test()//行列扫描
{
	char a = 0;
	gpio_key = 0x0f;
	
	if (gpio_key != 0x0f)
	{
		delay_1ms(100);
		if(gpio_key!=0x0f)
		{
			switch(gpio_key)
			{
				case(0x07):KeyValue = 1; break;
				case(0x0b):KeyValue = 2; break;
				case(0x0d):KeyValue = 3; break;
				case(0x0e):KeyValue = 4; break;
			}	
			gpio_key=0xf0;
			switch(gpio_key)
			{
				case(0x70):break;
				case(0xb0):KeyValue += 4; break;
				case(0xd0):KeyValue += 8; break;
				case(0xe0):KeyValue += 12; break;
			}	
			while((a<50)&&(gpio_key!=0xf0))
			{
				delay_1ms(10);
				a++;
				fla = 0;
			}
		}		
	}
}
void Key_Init(void)	
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = 0x18;		//设置定时初始值
	TH2 = 0xFC;		//设置定时初始值
	RCAP2L = 0x18;		//设置定时重载值
	RCAP2H = 0xFC;		//设置定时重载值
	TR2 = 1;		//定时器2开始计时

	ET2 = 1;	
	EA =1;
}
void tm2_isr() interrupt 5
{
		key_test();
		
				
		TF2 = 0; 
}