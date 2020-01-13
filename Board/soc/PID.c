#include "stm32f10x.h"
#include "PID.h"
#include "motor.h"
#include "usart.h"
#include "encoder.h"
#include "ANO_SpeedSend.h"

#if 0
#include "led.h"
#include "delay.h"
#endif

float Kp = 0.1  ; //��������
float ki = 0.09 ; //���ֳ���
float kd = 0.75 ;  //΢�ֳ���

/*********************
*��ʼ����������������*
*********************/
void incPIDinit(PIDtypedef *PIDx)
{
	PIDx->setpoint = 3;			//�趨Ŀ��
	PIDx->sum_error = 0;		//����ۼ�
  PIDx->proportion = Kp;		//��������
  PIDx->integral = ki;		//���ֳ���
  PIDx->derivative = kd;		//΢�ֳ���
  PIDx->last_error = 0;		//e[-1]
  PIDx->prev_error = 0;		//e[-2]
	
}


/********
*PID����*
********/
int incPIDcalc(PIDtypedef *PIDx,u16 nowpoint)
{
	int Error,incpid;
	
	Error = PIDx->setpoint - nowpoint;  //��ǰ���
	
	incpid=                                                          //��������
	PIDx->proportion * (Error - PIDx->last_error)
	+ PIDx->integral * Error
	+ PIDx->derivative * (Error - 2 * PIDx->last_error + PIDx->prev_error);
 
	PIDx->prev_error = PIDx->last_error;	//�洢�������´μ���
	
	PIDx->last_error = Error;
	
	return (incpid) ;

}


/*******************
*PID ������ʱ���趨*
*******************/
void PIDperiodinit(u16 arr,u16 psc)
{
	// PERIOD/f s  PERIOD us
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	//TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}

/***************
*�趨 PIDԤ��ֵ*
***************/
void PID_setpoint(PIDtypedef*PIDx,u16 setvalue)
{
	PIDx->setpoint = setvalue;
}

/**************************
*�趨PID  kp ki kd��������*
**************************/
void PID_set(float pp,float ii,float dd)
{
	Kp = pp; //��������
	ki = ii; //����ʱ�䳣��
	kd = dd;  //΢��ʱ�䳣��
}


/************
*PID�жϵ���*
************/
extern u16 rsp;
extern PIDtypedef PID_L;
uint8_t count = 0;
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {	//�����ж�
		
		if(count == 20) {
			int32_t cnt_temp;
			double pulse, round_t;
			cnt_temp = read_cnt();						//�õ������ؼ���ֵ�����������������
			pulse = (1000 / 20)* cnt_temp/2.0f;						//������TIM_EncoderMode_TI12������Ҫ����Ƶ�������ԣ��õ�ʵ�ʵ������ظ���ֵ������ֵ��������
			round_t = pulse / 520.0f;				//������ÿת����260�����壬��ô����520�������أ���ͨ���ù�ʽ��������ת�˼�Ȧ		
			ANO_Send_Data((u16)(round_t * 10000)); 
			
			uint32_t PWM_L;
			round_t	+= incPIDcalc(&PID_L, round_t);
			PWM_L += 960 * round_t;		//���˸��Ӻţ��������������������ʽ����������
			MotorRun(PWM_L, 0);	
		}
		else
			count ++;
	}
	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //����жϱ�־λ      
}


/*********
*PIDL����*
*********/
PIDtypedef PID_L;
void incPIDLConfig(void)
{
	incPIDinit(&PID_L);
	PIDperiodinit(1000, 72 - 1);
}
