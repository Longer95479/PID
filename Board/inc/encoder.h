#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f10x.h"

#define TIMx TIM4
#define TIMx_CH_PORT GPIOB
#define TIMx_CH1_PIN GPIO_Pin_6
#define TIMx_CH2_PIN GPIO_Pin_7

#define TIMx_IRQn TIM4_IRQn
#define TIMx_IRQHandler TIM4_IRQHandler

#define RCC_APB1Periph_TIMx RCC_APB1Periph_TIM4

#define TIM_PERIOD 65535



typedef struct {
	uint8_t Capture_FinishFlag;
	uint8_t Capture_StartFlag;
	uint16_t Capture_CcrValue;
	uint16_t Capture_Period;
} TIM_ICUserValueTypeDef;

void Encoder_Config(void);
void TIMx_GPIO_Config(void);
void TIMx_IC_Config(void);
void TIMx_NVIC_Config(void);
void TIM_ICUserValue_Init(void);
int32_t read_cnt(void);

#endif
