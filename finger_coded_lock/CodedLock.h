#ifndef __CODEDLOCK_H_
#define __CODEDLOCK_H_



int MODE_Choose(); 		//模式选择

void PassWord_Input();//密码初始化
void reset();					//输入重置函数
void Key_Input();			//密码按键输入函数
void Key_Change(); 		//密码修改函数
void Key_Check();			//密码检查
void Key_Eliminate(); //清除输入
void Key_sudo();  		//超级用户
void open_door();     //开门

void Finger_main();  //指纹main

#endif