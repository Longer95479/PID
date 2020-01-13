/*-----------------------------------------------------------------------------------------------------
��д��718����ʵ����
ƽ̨��STM32F103VET6
���ܣ����PWM���Ƶ��
-------------------------------------------------------------------------------------------------------*/
#include "motor.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/**
  * @brief	��ʼ���߼���ʱ��TIM8
  *	@param	��
  *	@retval ��
  */
void TIM_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��1��2��3 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�������Ƶ��
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��GPIO
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//��ʱ��ʱ���ṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;	 //��ʱ������ȽϽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	 //ʹ�ܶ�ʱ��ʱ��
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ0.0005s
	TIM_TimeBaseStructure.TIM_Period = 7200;	//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler= 0;	 //����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ���� ���������������������ʱ����Ҫ�õ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//�����ظ���������ֵΪ0���������ظ�����
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  //��ʼ����ʱ��

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 0;  //���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //�������:TIM����Ƚϼ��Ը�(�ߵ�ƽ��Ч)
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��1
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��ͨ��1��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��2
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��ͨ��2��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��3
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��4
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��ͨ��4��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM
	TIM_CtrlPWMOutputs(TIM8, ENABLE);  //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��
}

/**
  * @brief	�������
  *	@param	��
  *	@retval ��
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

