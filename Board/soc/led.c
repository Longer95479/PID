#include "led.h"

/**
  * @brief  LED��ʼ��
  * @param  Led��LED�ƣ�LED1 - LED6��
  * @retval ��
  * @example Led_Init(LED1);
  */
void Led_Init(LED Led)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�����������ڸ���JTAGռ�õ�����Ϊ��ͨIO�������Ȳ����˽�
	if(Led==LED1)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	//����һ������GPIO_InitStructure�Ľṹ�壬������������Ա
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//ʹ��APB2���ߵ�GPIOD���֣���������ֻ��Ҫ����GPIODΪ��������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ѡ��GPIOD�ϵ�pin4���ţ�����ֻ�����Pin_4Ϊ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��������Ϊ�������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//��������Ƶ�ʣ�������ʱ�������壩
		GPIO_Init(GPIOD ,&GPIO_InitStructure);//�ú�����GPIO_InitStructure�ṹ���Ա�������д��Ĵ���
	}
	else if(Led==LED2)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB ,&GPIO_InitStructure);
	}
	else if(Led==LED3)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB ,&GPIO_InitStructure);
	}
	else if(Led==LED4)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED5)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED6)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED_ALL)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD ,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;
		GPIO_Init(GPIOB ,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_6;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
}

/**
  * @brief  ����LED״̬
  * @param  Led��LED�ƣ�LED1 - LED3��
  * @param  State�� ���õ�״̬��0��1��
  * @retval ��
  * @example Led_Init(LED1,1);
  */
void Led_Set(LED Led, uint8_t State)
{
	if(Led==LED1)
	{
		if(State==1)
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);//����GPIOD PIN4����͵�ƽ
		else if(State==0)
			GPIO_SetBits(GPIOD,GPIO_Pin_4);//����GPIOD PIN4����ߵ�ƽ
	}
	else if(Led==LED2)
	{
		if(State==1)
			GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		else if(State==0)
			GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}
	else if(Led==LED3)
	{
		if(State==1)
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		else if(State==0)
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
	else if(Led==LED4)
	{
		if(State==1)
			GPIO_ResetBits(GPIOE,GPIO_Pin_0);
		else if(State==0)
			GPIO_SetBits(GPIOE,GPIO_Pin_0);
	}
	else if(Led==LED5)
	{
		if(State==1)
			GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		else if(State==0)
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}
	else if(Led==LED6)
	{
		if(State==1)
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);
		else if(State==0)
			GPIO_SetBits(GPIOE,GPIO_Pin_6);
	}
	else if(Led==LED_ALL)
	{
		if(State==1)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_6);
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);
			GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5);
		}
		else if(State==0)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_6);
			GPIO_SetBits(GPIOD,GPIO_Pin_4);
			GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5);
		}
	}
}
