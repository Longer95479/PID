#ifndef USART_H
#define USART_H

#include "stdio.h"	//为了重定向printf()
#include "stm32f10x.h"

void USART_GPIO_init(void);
void UART4_Interrupt(void);
void USART_set_bound(int bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif
