#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__



typedef unsigned char u8;
typedef unsigned int u16;

#define gpio_key P1 //P1口数据
void Key_Init(void);		//1毫秒@11.0592MHz
void key_test();//行列扫描

#endif
