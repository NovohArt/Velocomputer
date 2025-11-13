#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

/* Don't need yet*/
void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char* str);

#endif
