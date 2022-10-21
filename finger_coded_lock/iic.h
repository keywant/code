#ifndef _IIC_H
#define _IIC_H

sbit SDA = P2^0;
sbit SCL = P2^1;

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

unsigned char EEPROM_read(unsigned char add); //读取add地址数据
void EEPROM_write(unsigned char add,unsigned char dat); //向add地址写入数据

#endif