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
	��������USART_GPIO_init
	����  ��UART4 GPIO����
  ����  ����
  ���  ����
******************/
void  USART_GPIO_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK, ENABLE);         // �򿪴��� GPIO ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = UART1_TX;                       //C10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   						// �� USART Tx �� GPIO ����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = UART1_RX;             					//C11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// �� USART Rx �� GPIO ����Ϊ��������ģʽ
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}



/***********************
	��������UART4_Interrupt
	����  ��UART4�ж�����
  ����  ����
  ���  ����
***********************/
void UART4_Interrupt()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 						//�жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;  			//���� UART4 Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	// �������ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 					// �����ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  						//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);															//��ʼ������ NVIC
}


/***************************************
	��������USART_set_bound
	����  ����ʼ��UART�����ò����ʣ�����9600��115200��
  ����  ��������
  ���  ����
****************************************/
void USART_set_bound(int bound)
{
	USART_InitTypeDef USART_InitStructure;
	USART_GPIO_init();                                          // GPIO����
	RCC_APB1PeriphClockCmd(DEBUG_USART_CLK,ENABLE); 						// �򿪴��������ʱ��
	USART_InitStructure.USART_BaudRate = bound;	   							// ���ò����� 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // ���� �����ֳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   	  // ����ֹͣλ1λ 
	USART_InitStructure.USART_Parity = USART_Parity_No;         // ��У��λ 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		 		//����&&����ģʽ
  USART_Init(DEBUG_USARTx, &USART_InitStructure); 
	UART4_Interrupt();                                                   // �ж�����
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE); 					  		 // ʹ�ܴ��ڽ����ж�
	USART_Cmd(DEBUG_USARTx, ENABLE);  													  			 // ʹ�ܴ���
}


/************************
	��������Usart_SendByte
	����  ������һ���ַ�
  ����  �����������ַ�
  ���  ����
*************************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch) 
{
	 USART_SendData(pUSARTx,ch);
	
	/* �ȴ��������ݼĴ���Ϊ��*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);   
	
}

/****************************
	��������Usart_SendString
	����  �������ַ���
  ����  �����������ַ�����ַ
  ���  ����
*****************************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0; 
	do {
		Usart_SendByte( pUSARTx, *(str + k) );   									 //���͵�������
		   k++; 
	   }while (*(str + k)!='\0');                                //�ȴ�������� 
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
	{}
}

/**************************
	��������UART4_IRQHandler
	����  �������жϷ�����
  ����  ����
  ���  ����
***************************/
void UART4_IRQHandler(void)
{
	uint32_t ucTemp;
	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)    //���ռĴ������ж� 
		{
			/*����û�����*/
			/*
			 ucTemp = USART_ReceiveData( DEBUG_USARTx );             //���յ����ַ�
			 USART_SendData(DEBUG_USARTx,ucTemp);  			//�����յ��ַ����ͻ�ȥ
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
  ��������fputc
  ����  ���ض���c�⺯��printf��UART4
  ����  ����
  ���  ����
  ����  ����printf����
 *****************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USARTx, (unsigned char) ch);
	while (!(DEBUG_USARTx->SR & USART_FLAG_TXE));
	return (ch);
}

/*****************************************
  ��������fgetc
  ����  ���ض���c�⺯��scanf��UART4
  ����  ����
  ���  ����
  ����  ����scanf����
 *****************************************/
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
	
}
