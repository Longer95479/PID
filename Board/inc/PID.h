#ifndef PID_H
#define PID_H

#include "stm32f10x.h"

#define T      0.02 //采样周期
#define Ki     Kp*(T/Ti)        // Kp Ki Kd 三个主要参数
#define Kd     Kp*(Td/T)

typedef struct 
{
int setpoint;//设定目标
int sum_error;//误差累计
float proportion ;//比例常数
float integral ;//积分常数
float derivative;//微分常数
int last_error;//e[-1]
int prev_error;//e[-2]
} PIDtypedef;

void PIDperiodinit(u16 arr,u16 psc);        //PID 采样定时器设定
void incPIDinit(PIDtypedef *PIDx);                //初始化，参数清零
float incPIDcalc(PIDtypedef*PIDx,u16 nextpoint);           //PID计算
void PID_setpoint(PIDtypedef*PIDx,u16 setvalue);  //设定 PID预期值
void PID_set(float pp,float ii,float dd);//设定PID  kp ki kd三个参数
void incPIDLConfig(void); //PID_L配置 

#endif
