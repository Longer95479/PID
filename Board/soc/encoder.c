#include "stm32f10x.h"
#include "encoder.h"
#include "ANO_SpeedSend.h"


void Encoder_Config(void)
{
	TIMx_GPIO_Config();
	TIMx_IC_Config();
	TIM_ICUserValue_Init();
	
}

void TIMx_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = TIMx_CH1_PIN | TIMx_CH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIMx_CH_PORT, &GPIO_InitStructure);
	
}

void TIMx_IC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Period = TIM_PERIOD;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = 1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);
	
	//TIM_ClearFlag(TIMx, TIM_IT_CC1 | TIM_IT_Update);
	
	//TIMx_NVIC_Config();
	//TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIMx,ENABLE);
	
}

void TIMx_NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

TIM_ICUserValueTypeDef TIM_ICUserValueStructure;
void TIM_ICUserValue_Init(void)
{
	
	TIM_ICUserValueStructure.Capture_StartFlag = 0;
	TIM_ICUserValueStructure.Capture_FinishFlag = 0;
	TIM_ICUserValueStructure.Capture_CcrValue = 0;
	TIM_ICUserValueStructure.Capture_Period = 0;
}

int32_t read_cnt(void)
{
	uint32_t encoder_cnt;
	encoder_cnt = TIM4->CNT;		//读取计数器CNT的值，CNT系uint32_t型的变量
	TIM_SetCounter(TIM4, 0);		//每一次读取完计数值后将计数值清零，重新开始累加脉冲，方便下一次计数
	return encoder_cnt;				//返回的值就是本次读到的计数值
}
