#ifndef MPU9250_H_
#define MPU9250_H_
 
#include "SPI.h"
#include "stm32f4xx.h"    

#define CS2_LOW()   GPIOD->BSRR = GPIO_BSRR_BR_9
#define CS2_HIGH()  GPIOD->BSRR = GPIO_BSRR_BS_9
//#define INT_LOW()   GPIOD->BSRR = GPIO_BSRR_BR_10
//#define INT_HIGH()  GPIOD->BSRR = GPIO_BSRR_BS_10


uint8_t MPU9250_ReadReg(uint8_t reg);
void MPU9250_WriteReg(uint8_t reg, uint8_t value);
void MPU9250_Init(void);

#endif
