#include "stm32f10x.h"
#include <stdio.h>
#include "encoder.h"
#include "usart.h"
#include "motor.h"
#include "delay.h"
#include "ANO_SpeedSend.h"
#include "PID.h"
#include "led.h"

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;
extern PIDtypedef PID_L;
double frequency;
u16 rsp, count_num;



int main(void)
{
	Led_Init(LED_ALL);
	delay_init();
	USART_set_bound(9600);
	Encoder_Config();
	
	TIM_PWM_Init();
	
	incPIDLConfig();
	
	MotorRun(0, 0);
	
	while(1);
		
	
}

