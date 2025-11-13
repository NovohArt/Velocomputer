#ifndef ILI9341_H_
#define ILI9341_H_

#include "stm32f4xx.h"  
#include "SPI.h"   
#include "Symbols.h"  

// Commands for display
#define Sleep_OUT 					0x11
#define Pixel_Format_Set 		0x3A
#define Display_ON 					0x29
#define Column_Address_Set 	0x2A
#define Page_Address_Set 		0x2B
#define Memory_Write 				0x2C

// Data shortcuts
#define RGB_16_Bit 0x55 													// 0x55 = 0b01010101


// Control display
#define CS_LOW()   GPIOE->BSRR = GPIO_BSRR_BR_11  // If chip select 0. SPI conversation active
#define CS_HIGH()  GPIOE->BSRR = GPIO_BSRR_BS_11	// If chip select 1. SPI conversation not active
#define DC_LOW()   GPIOE->BSRR = GPIO_BSRR_BR_12	// Command for ILI9341
#define DC_HIGH()  GPIOE->BSRR = GPIO_BSRR_BS_12	// Data for ILI9341
#define RST_LOW()  GPIOE->BSRR = GPIO_BSRR_BR_13	// Reset active
#define RST_HIGH() GPIOE->BSRR = GPIO_BSRR_BS_13	// Reset disabeld. Display work normaly if only RST = 1

void delay(uint32_t count);
void ILI9341_Init(void);
void ILI9341_WriteCommand(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_Reset(void);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ILI9341_FillCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);
void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_DrawNumber(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, FontDef* font, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawNumber_Scaled(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor, uint8_t scale);
void ILI9341_DrawString_Scaled(uint16_t x, uint16_t y, const char* str, FontDef* font, uint16_t color, uint16_t bgcolor, uint8_t scale);


#endif
