#include "stm32f10x.h"
#include "usart.h"
#include "motor.h"
#include "PID.h"

extern PIDtypedef PID_L;

#define UART_num  4  

#if     UART_num ==  4
#define DEBUG_USARTx  UART4 
#define UART1_TX      GPIO_Pin_10          
#define UART1_RX      GPIO_Pin_11
#define  DEBUG_USART_CLK           RCC_APB1Periph_UART4
#define DEBUG_USART_GPIO_CLK       RCC_APB2Periph_GPIOC
#define DEBUG_USART_TX_GPIO_PORT   GPIOC
#define DEBUG_USART_RX_GPIO_PORT   GPIOC
#define DEBUG_USART_IRQ            UART4_IRQn

#elif   UART_num ==  5
#define DEBUG_USARTx  UART5 
#define UART1_TX      GPIO_Pin_12          
#define UART1_RX      GPIO_Pin_0
#define  DEBUG_USART_CLK           RCC_APB1Periph_UART5
#define DEBUG_USART_GPIO_CLK       RCC_APB2Periph_GPIOC
#define DEBUG_USART_TX_GPIO_PORT   GPIOC
#define DEBUG_USART_RX_GPIO_PORT   GPIOD
#define DEBUG_USART_IRQ            UART5_IRQn


#endif 



/*****************
	函数名：USART_GPIO_init
	描述  ：UART4 GPIO配置
  输入  ：无
  输出  ：无
******************/
void  USART_GPIO_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK, ENABLE);         // 打开串口 GPIO 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = UART1_TX;                       //C10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   						// 将 USART Tx 的 GPIO 配置为推挽复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = UART1_RX;             					//C11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// 将 USART Rx 的 GPIO 配置为上拉输入模式
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}



/***********************
	函数名：UART4_Interrupt
	描述  ：UART4中断配置
  输入  ：无
  输出  ：无
***********************/
void UART4_Interrupt()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 						//中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;  			//配置 UART4 为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	// 抢断优先级1 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 					// 子优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  						//使能中断
	NVIC_Init(&NVIC_InitStructure);															//初始化配置 NVIC
}


/***************************************
	函数名：USART_set_bound
	描述  ：初始化UART，设置波特率（常用9600，115200）
  输入  ：波特率
  输出  ：无
****************************************/
void USART_set_bound(int bound)
{
	USART_InitTypeDef USART_InitStructure;
	USART_GPIO_init();                                          // GPIO配置
	RCC_APB1PeriphClockCmd(DEBUG_USART_CLK,ENABLE); 						// 打开串口外设的时钟
	USART_InitStructure.USART_BaudRate = bound;	   							// 配置波特率 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 配置 数据字长8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   	  // 配置停止位1位 
	USART_InitStructure.USART_Parity = USART_Parity_No;         // 无校验位 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		 		//发送&&接受模式
  USART_Init(DEBUG_USARTx, &USART_InitStructure); 
	UART4_Interrupt();                                                   // 中断配置
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE); 					  		 // 使能串口接收中断
	USART_Cmd(DEBUG_USARTx, ENABLE);  													  			 // 使能串口
}


/************************
	函数名：Usart_SendByte
	描述  ：发送一个字符
  输入  ：串口名，字符
  输出  ：无
*************************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch) 
{
	 USART_SendData(pUSARTx,ch);
	
	/* 等待发送数据寄存器为空*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);   
	
}

/****************************
	函数名：Usart_SendString
	描述  ：发送字符串
  输入  ：串口名，字符串地址
  输出  ：无
*****************************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0; 
	do {
		Usart_SendByte( pUSARTx, *(str + k) );   									 //发送单个数据
		   k++; 
	   }while (*(str + k)!='\0');                                //等待发送完成 
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
	{}
}

/**************************
	函数名：UART4_IRQHandler
	描述  ：串口中断服务函数
  输入  ：无
  输出  ：无
***************************/
void UART4_IRQHandler(void)
{
	uint32_t ucTemp;
	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)    //接收寄存器满中断 
		{
			/*添加用户程序*/
			/*
			 ucTemp = USART_ReceiveData( DEBUG_USARTx );             //接收单个字符
			 USART_SendData(DEBUG_USARTx,ucTemp);  			//将接收的字符发送回去
			*/
			/*
		  scanf("%u", &ucTemp);
			MotorRun(ucTemp, 0);
			printf("temp:%u\n", ucTemp);
			*/
			float pp, ii, dd;
			scanf("%f,%f,%f", &pp, &ii, &dd);
			PID_set(pp, ii, dd);
			
			
		}
}



/*****************************************
  函数名：fputc
  描述  ：重定向c库函数printf到UART4
  输入  ：无
  输出  ：无
  调用  ：由printf调用
 *****************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USARTx, (unsigned char) ch);
	while (!(DEBUG_USARTx->SR & USART_FLAG_TXE));
	return (ch);
}

/*****************************************
  函数名：fgetc
  描述  ：重定向c库函数scanf到UART4
  输入  ：无
  输出  ：无
  调用  ：由scanf调用
 *****************************************/
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
	
}
