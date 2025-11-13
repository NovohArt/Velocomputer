#include "ADC1_Speed.h"

volatile uint32_t number = 0;
volatile uint32_t ticks = 0;

void TIM2_IRQHandler(void){		//Timer conter for Get_speed_stupid
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;
		ticks++;
	
	}
}

void Init_ADC1(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	// Clock enable for GPIOA (pins)
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		// Clock enable ADC1
	
	for(volatile int i = 0; i < 1000; i++);	// Stabilisation delay
	
	GPIOA->MODER |= GPIO_MODER_MODER1;		// Pin PA1 initiation
		
	ADC1->SQR1 = 0;		// 1st conversion
	ADC1->SQR3 = 1;		// ADC works on channel 1
	
	//ADC1->SMPR2 |= ADC_SMPR2_SMP1;	// Sample time for channel 1 (239.5)
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP1;		// Sample time for channel 1 (3)
	
	ADC1->CR2 = ADC_CR2_ADON;								// ADC ON
	for(volatile int i = 0; i < 1000; i++);	// Stabilisation delay
	
}

uint16_t Get_value_adc1(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;			// Start Conversion
    while(!(ADC1->SR & ADC_SR_EOC));  // Wait until end of conversion
    return ADC1->DR;  								// Get value
}


uint16_t Get_speed_stupid(void) {
	uint32_t data = 0;
	uint32_t target_ticks = 0x0AAA;
  uint32_t front = 0;
	
		while(ticks <= target_ticks){
			
			data = Get_value_adc1();
							
				if (data >= 2280 && (front == 0)){
						front = 1;
						number++;
				}
				
				if (data <= 2220 && (front == 1)){
					front = 0;
				}
			
		  }
		ticks=0;
		return number;
}
	
