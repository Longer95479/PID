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

float Kp = 0.1  ; //比例常数
float ki = 0.09 ; //积分常数
float kd = 0.75 ;  //微分常数

/*********************
*初始化，参数清零清零*
*********************/
void incPIDinit(PIDtypedef *PIDx)
{
	PIDx->setpoint = 3;			//设定目标
	PIDx->sum_error = 0;		//误差累计
  PIDx->proportion = Kp;		//比例常数
  PIDx->integral = ki;		//积分常数
  PIDx->derivative = kd;		//微分常数
  PIDx->last_error = 0;		//e[-1]
  PIDx->prev_error = 0;		//e[-2]
	
}


/********
*PID计算*
********/
int incPIDcalc(PIDtypedef *PIDx,u16 nowpoint)
{
	int Error,incpid;
	
	Error = PIDx->setpoint - nowpoint;  //当前误差
	
	incpid=                                                          //增量计算
	PIDx->proportion * (Error - PIDx->last_error)
	+ PIDx->integral * Error
	+ PIDx->derivative * (Error - 2 * PIDx->last_error + PIDx->prev_error);
 
	PIDx->prev_error = PIDx->last_error;	//存储误差，便于下次计算
	
	PIDx->last_error = Error;
	
	return (incpid) ;

}


/*******************
*PID 采样定时器设定*
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
*设定 PID预期值*
***************/
void PID_setpoint(PIDtypedef*PIDx,u16 setvalue)
{
	PIDx->setpoint = setvalue;
}

/**************************
*设定PID  kp ki kd三个参数*
**************************/
void PID_set(float pp,float ii,float dd)
{
	Kp = pp; //比例常数
	ki = ii; //积分时间常数
	kd = dd;  //微分时间常数
}


/************
*PID中断调整*
************/
extern u16 rsp;
extern PIDtypedef PID_L;
uint8_t count = 0;
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {	//更新中断
		
		if(count == 20) {
			int32_t cnt_temp;
			double pulse, round_t;
			cnt_temp = read_cnt();						//得到跳变沿计数值（脉冲个数的两倍）
			pulse = (1000 / 20)* cnt_temp/2.0f;						//由于是TIM_EncoderMode_TI12，所以要二分频，即除以，得到实际的跳变沿个数值（脉冲值的两倍）
			round_t = pulse / 520.0f;				//假设电机每转产生260个脉冲，那么就有520个跳变沿，则通过该公式可求出电机转了几圈		
			ANO_Send_Data((u16)(round_t * 10000)); 
			
			uint32_t PWM_L;
			round_t	+= incPIDcalc(&PID_L, round_t);
			PWM_L += 960 * round_t;		//忘了个加号！！！致命！这可是增量式啊。。。。
			MotorRun(PWM_L, 0);	
		}
		else
			count ++;
	}
	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //清除中断标志位      
}


/*********
*PIDL配置*
*********/
PIDtypedef PID_L;
void incPIDLConfig(void)
{
	incPIDinit(&PID_L);
	PIDperiodinit(1000, 72 - 1);
}
