#include "ADC2_Light.h"

void Init_ADC2(void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;	//clock enable for GPIOA (pins)
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;		//clock enable ADC2
	
	for(volatile int i = 0; i < 1000; i++);	// Stabilisation delay
	
	GPIOA->MODER |= GPIO_MODER_MODER4;		// Pin PA4 initiation
	GPIOE->MODER |= GPIO_MODER_MODER9_0;
	
	
	ADC2->SQR1 = 0;		// 1st conversion
	ADC2->SQR3 = 4;		// ADC works on channel 4
	
	ADC2->SMPR2 |= (ADC_SMPR2_SMP4_0 | ADC_SMPR2_SMP4_1);	// Sample time for channel 4 (56)
	
	ADC2->CR2 = ADC_CR2_ADON;								// ADC ON
	for(volatile int i = 0; i < 1000; i++);	// Stabilisation delay
}

uint16_t Get_value_adc2(void){
	  ADC2->CR2 |= ADC_CR2_SWSTART;  		// Start Conversion
    while(!(ADC2->SR & ADC_SR_EOC));  // Wait until end of conversion
    return ADC2->DR; 									// Get value
}

void Light(void){
	uint16_t light = 0;
	if( (light = Get_value_adc2()) > 2000 ){
		LIGHT()
		ILI9341_FillCircle(20, 20, 10, 0x07E0);
	}
	else{
		DARK()
		ILI9341_FillCircle(20, 20, 10, 0x001F);
	}
	light = 0;
}
