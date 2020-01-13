/*-----------------------------------------------------------------------------------------------------
编写：718创新实验室
平台：STM32F103VET6
功能：输出PWM控制电机
-------------------------------------------------------------------------------------------------------*/
#include "motor.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/**
  * @brief	初始化高级定时器TIM8
  *	@param	无
  *	@retval 无
  */
void TIM_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	//输出比较通道1、2、3 GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能外设总线时钟
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//设置GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置输出频率
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//初始化GPIO
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//定时器时基结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;	 //定时器输出比较结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	 //使能定时器时钟
	
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为0.0005s
	TIM_TimeBaseStructure.TIM_Period = 7200;	//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Prescaler= 0;	 //驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		//时钟分频因子 ，输出互补脉冲配置死区时间需要用到
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//设置重复计数器的值为0，不设置重复计数
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  //初始化定时器

	/*--------------------输出比较结构体初始化-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能输出比较
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置初始占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性:TIM输出比较极性高(高电平有效)
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //初始化输出比较通道1
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能通道1的CCR4上的预装载寄存器
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //初始化输出比较通道2
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能通道2的CCR4上的预装载寄存器
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //初始化输出比较通道3
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能通道3的CCR4上的预装载寄存器
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //初始化输出比较通道4
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能通道4的CCR4上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM
	TIM_CtrlPWMOutputs(TIM8, ENABLE);  //主输出使能，当使用的是通用定时器，不需要主输出使能
}

/**
  * @brief	电机控制
  *	@param	无
  *	@retval 无
  */
void MotorRun(int speed1,int speed2)
{                                                
	if(speed1>=0)
	{
		TIM_SetCompare1(TIM8,0);
		TIM_SetCompare2(TIM8,speed1);
	}
	else
	{
		TIM_SetCompare1(TIM8,-speed1);
		TIM_SetCompare2(TIM8,0);
	}
	
	if(speed2>=0)
	{
		TIM_SetCompare4(TIM8,0);
		TIM_SetCompare3(TIM8,speed2);
	}
	else
	{
		TIM_SetCompare4(TIM8,-speed2);
		TIM_SetCompare3(TIM8,0);
	}
}

