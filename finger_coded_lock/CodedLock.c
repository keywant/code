#include "head.h"


extern u8 KeyValue;			//按键值
extern char fla;				//按键按下一次信号
extern unsigned int Servo0PwmDuty;//PWM脉冲宽度



typedef struct {
	unsigned int password_insurance; //保险密码
	unsigned int password_open;		//用户密码
	unsigned int password;				//当前输入密码值
	unsigned int OPEN_time;				//打开次数
}codedlock_t;

codedlock_t chen = {815,0,0,0};

unsigned char count = 0; //数字按键按下次数
int mode = 1;

int MODE_Choose()
{
	if(KeyValue == 4 && fla == 0)
	{
		LCD_ShowString(2,1," *key  finger");
		mode = 1;
		fla = -1;
	}
	if(KeyValue == 12 && fla == 0)
	{
		LCD_ShowString(2,1,"  key *finger");
		mode = 2;
		fla = -1;
	}
	if(KeyValue == 8 && fla == 0)
	{
		fla = -1;
		return mode;
	}
	return 0;
}

void PassWord_Input()		//密码初始化
{
//	EEPROM_write(0x05,1256/256);delay_1ms(10); 	//eeprom写入用户初始密码 第一次使用写入一次即可
//	EEPROM_write(0x06,1256%256);delay_1ms(10);
//	EEPROM_write(0x04,0%256);delay_1ms(10); 		//eeprom写入开锁次数
	
		delay_1ms(100); 
		chen.password_open = EEPROM_read(0x05) * 256;delay_1ms(10);
		chen.password_open += EEPROM_read(0x06);delay_1ms(10); //上电读取用户密码
	
		chen.OPEN_time += EEPROM_read(0x04);delay_1ms(20);		//上电读取开锁次数
	
}


void reset()  //输入重置函数
{
	chen.password = 0;	//当前输入密码值清零
	count = 0;		//数字按键按下次数清零
	LCD_ShowString(1,1,"===Coded Lock===");
	LCD_ShowString(2,1,"Input:          ");	//刷新界面
	fla = -1;      //按键可读取信号
}

void Key_Input() //密码按键输入函数
{
	
	if (count < 4 && fla == 0 && KeyValue % 4 != 0 && KeyValue != 13 && KeyValue != 15) 
		{
			
				count += 1;
				chen.password *= 10; 
				switch(KeyValue)
				{
					case 5:chen.password += 4;break;
					case 6:chen.password += 5;break;
					case 7:chen.password += 6;break;	
					case 9:chen.password += 7;break;
					case 10:chen.password += 8;break;
					case 11:chen.password += 9;break;
					case 14:chen.password += 0;break;
					default:chen.password += KeyValue;break;
				}
				LCD_ShowNum(2,7,chen.password,count);
				fla = -1;
		}
}



void Key_Check()			//密码检查函数 KeyValue=16
{
	
	if (count == 4 && KeyValue == 16 && fla == 0)		
	{
			if (chen.password == chen.password_open || (chen.password == chen.password_insurance))		//用户密码正确 或输入保险密码
			{
				open_door();
				chen.OPEN_time++;
				reset();
			}
			else       //密码错误
			{
				LCD_ShowString(2,14,"ERR");	//显示ERR
				delay_1ms(1000);
				reset();
			}
		}
}
void Key_Change() //密码修改函数	KeyValue=13
{
	unsigned int i = 0;
	if(KeyValue == 13 && fla == 0) 
	{
		fla = -1;
		delay_1ms(10);
		i = EEPROM_read(0x04);		//读取eeprom中的开锁次数
		if(i != chen.OPEN_time)		//开锁次数与当前次数对比
		{
			LCD_ShowString(2,1,"PleaseChange");
			delay_1ms(1200);
			reset();
			while(1)
			{
				Key_Input();
				Key_Eliminate();
				if (count == 4 && KeyValue == 16 && fla == 0)
				{
					chen.password_open = chen.password;		
					delay_1ms(100);
					EEPROM_write(0x05,chen.password_open/256);delay_1ms(100); 
					EEPROM_write(0x06,chen.password_open%256);delay_1ms(100);
					if(chen.OPEN_time == 255) {chen.OPEN_time = 0;}
					EEPROM_write(0x04,chen.OPEN_time%256);delay_1ms(100);

					LCD_ShowString(2,11,"Change");
					delay_1ms(1200);
					reset();
					break;
				}
			}
		}
	}
}
void Key_Eliminate() //清除输入 KeyValue=15
{
	if(KeyValue == 15 && fla == 0) 
	{
			reset();
	}
}

void Key_sudo()  //超级用户开锁 KeyValue=8 
{
	static char sudo = 3;
	if(KeyValue == 8 && fla == 0 && sudo != 0)
	{
		sudo--;
		if(sudo == 0)
		{
			open_door();
		}
		reset();
	}
}
void open_door()  //开锁
{
		LCD_ShowString(2,13,"OPEN");	//显示OPEN		
		Servo0PwmDuty = 2350;
		delay_1ms(2000);
		Servo0PwmDuty = 600;
}

void Finger_main()
{							
	LCD_ShowString(1,1,"Finger Test    ");//液晶开机显示界面
	LCD_ShowString(2,1,"                "); 
  Uart_Init();			//初始化串口
	
 	delay_1ms(200);          //延时200MS，等待指纹模块复位
	Device_Check();		   	//校对指纹模块是否接入正确，液晶做出相应的提示
	delay_1ms(1000);			//对接成功界面停留一定时间
	P1 = 0xF0; 
	LCD_ShowString(1,1,"  search finger ");
	LCD_ShowString(2,1,"  Add   Delete  "); 		
	while(1)
	{
		if(KeyValue == 16 && fla == 0)
			{
				fla = -1;
				break;
			}		
	    /**************进入主功能界面****************/
		

		if(local_date==0)
		{
			LCD_ShowString(1,1," *search finger ");
			LCD_ShowString(2,1,"  Add   Delete  "); 
		}
		else if(local_date==1)
		{
			LCD_ShowString(1,1,"  search finger ");
			LCD_ShowString(2,1," *Add   Delete  "); 
		}
		else if(local_date==2)
		{
			LCD_ShowString(1,1,"  search finger ");
			LCD_ShowString(2,1,"  Add  *Delete ");
		}			
		//确认键
		if(KeyValue == 8 && fla == 0)
		{	 
		 	fla = -1;								
			switch(local_date)
			{
					case 0:  //搜索指纹						
					FPM10A_Find_Fingerprint();																								
					break;	
					
					case 1:	 //添加指纹
					FPM10A_Add_Fingerprint();
					break; 					
					
					case 2:	//清空指纹
					FPM10A_Delete_All_Fingerprint();
		  		break;
				}
			}
		  //切换键 KeyValue == 12
			if(KeyValue == 12 && fla == 0)
			{
			 	fla = -1;			
	  	 	if (local_date <= 2)
				{

					if(++local_date == 3)local_date = 0;						
				}		
			}						
			delay_1ms(100); //延时判断100MS检测一次		
	}
}