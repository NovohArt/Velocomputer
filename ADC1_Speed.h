#ifndef ADC1_SPEED_H_
#define ADC1_SPEED_H_

#include "stm32f4xx.h"                  // Device header
#include "TIM.h"

void Init_ADC1(void); // Int ADC1
uint16_t Get_value_adc1(void);	// Get Value
uint16_t Get_speed_stupid(void); 

extern volatile uint32_t number;

#endif
