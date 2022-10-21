#ifndef __LCD1602_H__
#define __LCD1602_H__
//1602屏地址定义
#define LINE1 0x80				   //第一行地址
#define LINE2 0xc0				   //第二行地址

//用户调用函数：
void LCD_Init();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

extern char local_date,base_date;
extern unsigned char code *Main_Menu[];



//外部函数
//void Delay_us(unsigned int us);
//void Delay_ms(unsigned int ms);
void LCD1602_WriteCMD(unsigned char cmd);
void LCD1602_WriteDAT(unsigned char dat);
void LCD1602_CheckBusy(void);
void LCD1602_Init(void);
void LCD1602_Display(unsigned char addr,unsigned char* pointer,unsigned char index,unsigned char num);

#endif
