#include "led.h"

/**
  * @brief  LED初始化
  * @param  Led：LED灯（LED1 - LED6）
  * @retval 无
  * @example Led_Init(LED1);
  */
void Led_Init(LED Led)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//以上两句用于复用JTAG占用的引脚为普通IO，可以先不做了解
	if(Led==LED1)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	//定义一个叫做GPIO_InitStructure的结构体，它包含三个成员
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能APB2总线的GPIOD部分，更改引脚只需要更改GPIOD为其他即可
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//选择GPIOD上的pin4引脚，更改只需更改Pin_4为其他
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置引脚为推挽输出模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置引脚频率（可以暂时忽略意义）
		GPIO_Init(GPIOD ,&GPIO_InitStructure);//该函数将GPIO_InitStructure结构体成员相关设置写入寄存器
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
  * @brief  设置LED状态
  * @param  Led：LED灯（LED1 - LED3）
  * @param  State： 设置的状态（0、1）
  * @retval 无
  * @example Led_Init(LED1,1);
  */
void Led_Set(LED Led, uint8_t State)
{
	if(Led==LED1)
	{
		if(State==1)
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);//设置GPIOD PIN4输出低电平
		else if(State==0)
			GPIO_SetBits(GPIOD,GPIO_Pin_4);//设置GPIOD PIN4输出高电平
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
