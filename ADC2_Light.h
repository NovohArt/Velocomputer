#ifndef ADC2_LIGHT_H_
#define ADC2_LIGHT_H_

#include "stm32f4xx.h"                  // Device header
#include "ILI9341.h"

void Init_ADC2(void);	// Int ADC1
uint16_t Get_value_adc2(void);// Get Value
void Light(void);// Transform value

#define LIGHT()   GPIOE->BSRR = GPIO_BSRR_BS_9;
#define DARK()   GPIOE->BSRR = GPIO_BSRR_BR_9;

#endif
