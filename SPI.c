#include "SPI.h"
#include "stm32f4xx_ll_spi.h"
//
void Init_SPI1_display(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;    // Clock enable for GPIOA and GPIOE (pins)
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;    												// Clock enable ADC1
	
	for(volatile int i = 0; i < 1000; i++);
	
	//Init GPIO
	GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOA->MODER |= (2 << GPIO_MODER_MODER5_Pos) | (2 << GPIO_MODER_MODER6_Pos) | (2 << GPIO_MODER_MODER7_Pos);
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL6_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos);

	// PE11-CS, PE12-DC, PE13-RESET)
	GPIOE->MODER &= ~(GPIO_MODER_MODER11 | GPIO_MODER_MODER12 | GPIO_MODER_MODER13);
	GPIOE->MODER |= (1 << GPIO_MODER_MODER11_Pos) | (1 << GPIO_MODER_MODER12_Pos) | (1 << GPIO_MODER_MODER13_Pos);

	//Init spi
	SPI1->CR1 = 0; // Reset CR1

	SPI1->CR1 |= SPI_CR1_MSTR;       // Set as Master
	SPI1->CR1 |= SPI_CR1_SSM;        // Enable software slave management
	SPI1->CR1 |= SPI_CR1_SSI;        // Set internal slave select
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;  // MSB
	SPI1->CR1 &= ~SPI_CR1_DFF;       // 8-bit data

	SPI1->CR1 |= SPI_CR1_CPOL;       // CPOL = 0
	SPI1->CR1 |= SPI_CR1_CPHA;       // CPHA = 0

	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // BR=011 (fPCLK/16)
	
	SPI1->CR1 |= SPI_CR1_SPE;  // Enable SPI
}

void Init_SPI2_MPU9250(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN;    // PIOB
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;  	// Clock enable ADC1
	
	for(volatile int i = 0; i < 1000; i++);
	
	
  // PB13-SCK, PB14-MISO, PB15-MOSI
  GPIOB->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15);
	GPIOB->MODER |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1);

	// PD9-CS
  GPIOD->MODER &= ~GPIO_MODER_MODER9 ;
	GPIOD->MODER |=  GPIO_MODER_MODER9_0;
	
  GPIOB->AFR[1] |= (5 << GPIO_AFRH_AFSEL13_Pos) | (5 << GPIO_AFRH_AFSEL14_Pos) | (5 << GPIO_AFRH_AFSEL15_Pos); 
	
	//Init spi
	SPI2->CR1 = 0; // 

	SPI2->CR1 |= SPI_CR1_MSTR;       // 
	SPI2->CR1 |= SPI_CR1_SSM;        // 
	SPI2->CR1 |= SPI_CR1_SSI;        // 
	SPI2->CR1 &= ~SPI_CR1_LSBFIRST;  // 
	SPI2->CR1 &= ~SPI_CR1_DFF;       // 

	SPI2->CR1 |= SPI_CR1_CPOL;       // 
	SPI2->CR1 |= SPI_CR1_CPHA;       // 

	SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 ; // 
	
	SPI2->CR1 |= SPI_CR1_SPE;
}

void SPI1_Transmit(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    
    SPI1->DR = data;
    
    while (SPI1->SR & SPI_SR_BSY);
	
		for(volatile int i = 0; i < 100; i++); // Small delay for display between multiple transmitions
}

uint8_t SPI2_ReadWrite(uint8_t data) {
    while (!(SPI2->SR & SPI_SR_TXE)); //
    SPI2->DR = data;
    while (SPI2->SR & SPI_SR_BSY);   // 
	  return SPI2->DR;
}
