#ifndef PID_H
#define PID_H

#include "stm32f10x.h"

#define T      0.02 //��������
#define Ki     Kp*(T/Ti)        // Kp Ki Kd ������Ҫ����
#define Kd     Kp*(Td/T)

typedef struct 
{
int setpoint;//�趨Ŀ��
int sum_error;//����ۼ�
float proportion ;//��������
float integral ;//���ֳ���
float derivative;//΢�ֳ���
int last_error;//e[-1]
int prev_error;//e[-2]
} PIDtypedef;

void PIDperiodinit(u16 arr,u16 psc);        //PID ������ʱ���趨
void incPIDinit(PIDtypedef *PIDx);                //��ʼ������������
float incPIDcalc(PIDtypedef*PIDx,u16 nextpoint);           //PID����
void PID_setpoint(PIDtypedef*PIDx,u16 setvalue);  //�趨 PIDԤ��ֵ
void PID_set(float pp,float ii,float dd);//�趨PID  kp ki kd��������
void incPIDLConfig(void); //PID_L���� 

#endif
