#include "USART.h"

/* Don't need yet*/
void USART1_Init(void) {

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 


    GPIOA->MODER   |= (2 << 18) | (2 << 20);  
    GPIOA->AFR[1]  |= (7 << 4) | (7 << 8);   


    USART1->BRR = SystemCoreClock / 9600; 
	
    USART1->CR1 = USART_CR1_TE |    
                  USART_CR1_UE;     
	
		for(volatile int i = 0; i < 1000; i++);
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));    
    USART1->DR = c;                          
}

	
void USART1_SendString(char* str) {
    while (*str) {
        USART1_SendChar(*str++);          
    }
}
