#ifndef __SERVO_H_
#define __SERVO_H_
#define Corner_init 1500  
/*******舵机转量*******/
#define Servo_Corner1 4
#define Servo_Corner2 5
#define Servo_Corner3 6
/********00-6********/

extern unsigned int Servo0PwmDuty;//PWM脉冲宽度 
void Car_Servo_Init();	//舵机初始化
#endif