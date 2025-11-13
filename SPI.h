#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"                  // Device header

void Init_SPI1_display(void);
void SPI1_Transmit(uint8_t data);
void Init_SPI2_MPU9250(void);
uint8_t SPI2_ReadWrite(uint8_t data);

#endif
