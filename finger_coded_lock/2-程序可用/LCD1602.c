#include "head.h"

//引脚配置：
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0

char local_date=0,base_date=0;//全局变量，当前箭头位置

//函数定义：
/**
  * @brief  LCD1602延时函数，12MHz调用可延时1ms
  * @param  无
  * @retval 无
  */
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

/**
  * @brief  LCD1602写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}

/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0c);//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);//光标复位，清屏
}

/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}

//unsigned char code *Main_Menu[]=
//{
//{"  指纹识别系统"},
//{"  1.添加指纹"},
//{"  2.搜索指纹"},
//{"  3.清空指纹库"}
//};


/***********************************************
函数名称：Delay_us
功    能：STC 1T单片机1us延时程序
入口参数：us:延时的微秒数
返 回 值：无	
备    注：内部时钟11.0592MHz
************************************************/
void Delayus(unsigned int us)
{
  	while( us--)
   	{
    	_nop_(); 
   	}             
}
/***********************************************
函数名称：Delay_ms
功    能：STC 1T单片机1ms延时程序
入口参数：ms:延时的毫秒数
返 回 值：无	
备    注：示波器实测1.05ms 内部时钟11.0592MHz
************************************************/
void Delayms(unsigned int ms)
{
  	unsigned int i;
  	while( (ms--) != 0)
   	{
    	for(i = 0; i < 600; i++); 
   	}             
}
/*************************************
* 函 数 名: LCD1602_WriteCMD
* 函数功能: 向lcd1602写指令
* 入口参数: cmd:指令
* 返    回: 无
* 备    注：无
**************************************/
void LCD1602_WriteCMD(unsigned char cmd)
{
  	LCD_EN=0;
  	LCD_RS=0;
  	LCD_RW=0;
  	Delayus(10);
  	LCD_EN=1; 
  	Delayus(10);
  	LCD_DataPort=cmd;
  	Delayus(10);
  	LCD_EN=0;
}
/***********************************************
函数名称：LCD1602_WriteDAT
功    能：向lcd1602写数据
入口参数：dat：数据
返 回 值：无	
备    注：无
************************************************/
void LCD1602_WriteDAT(unsigned char dat)
{
  	LCD_EN=0;
  	LCD_RS=1;
  	LCD_RW=0;
  	Delayus(10);
  	LCD_EN=1; 
  	Delayus(10);
  	LCD_DataPort=dat;
  	Delayus(10);
  	LCD_EN=0;
}
/***********************************************
函数名称：LCD1602_CheckBusy
功    能：检测lcd1602忙信号
入口参数：无
返 回 值：无	
备    注：无
************************************************/
void LCD1602_CheckBusy(void)
{
  	unsigned char temp;
	LCD_DataPort=0xff;		  //做输入先置高,12c系列单片机需转换端口模式
  	while(1)
  	{    
    	LCD_EN=0;			   
    	LCD_RS=0;
    	LCD_RW=1;
    	Delayus(10);
    	LCD_EN=1;
    	Delayus(10);
    	temp=LCD_DataPort;    //读取忙通道数据
    	Delayus(10);
    	LCD_EN=0;
    	if((temp&0x80)!=0x80)
    	{    
      		break;
    	}
  	}
}
 /***********************************************
函数名称：LCD1602_Init
功    能：LCD1602初始化
入口参数：无
返 回 值：无	
备    注：详见数据手册定义。
************************************************/
void LCD1602_Init(void)  
{
  	Delayms(15);      		//上电延时15ms
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	Delayms(5);
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	Delayms(5);
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x38); //写显示指令
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x08); //显示关闭
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x01); //清屏
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x06); //显示光标移动设置
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x0c); //显示开及光标设置  
}
 /***********************************************
函数名称：LCD1602_Display
功    能：写多字节字符
入口参数：addr起始地址，pointer指针地址，index下标，num个数
返 回 值：无	
备    注：无
************************************************/
void LCD1602_Display(unsigned char addr,unsigned char* pointer,unsigned char index,unsigned char num)
{
  	unsigned char i;
  	LCD1602_CheckBusy();	//判断忙信号
  	LCD1602_WriteCMD(addr);	//写入地址
  	for(i=0;i<num;i++)		//写入数据
  	{
     	LCD1602_CheckBusy();			   //判断忙信号
     	LCD1602_WriteDAT(pointer[index+i]);//写入数据     
  	}
}
