#include "stm32f4xx.h"                  // Device header
#include "SPI.h"
#include "ADC1_Speed.h"
#include "TIM.h"
#include "ADC2_Light.h"
#include "USART.h"
#include "ILI9341.h"
#include "MPU9250.h"

#include <math.h> // for MPU9250 

void Clock_Init(void)
{
    // Init HSE
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));													// wait until ready

    // Configure PLL. I get those numbers from cube MX.
    RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos)   |							// /M = 8
                   (336 << RCC_PLLCFGR_PLLN_Pos) |							// *N = 336
                   (0 << RCC_PLLCFGR_PLLP_Pos)   |							// /P = 2
                   RCC_PLLCFGR_PLLSRC_HSE;											// Init PLL

    // Flash configure
    FLASH->ACR = FLASH_ACR_LATENCY_5WS |												// Delay 5 cycles
								 FLASH_ACR_ICEN 			 |  											// Enable cached instructions
								 FLASH_ACR_DCEN;																// Enable cached data

    // Init PLL
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));													// wait until ready

    // Prescaler configure
    RCC->CFGR = RCC_CFGR_HPRE_DIV1  |														// AHB  = 168 MHz
                RCC_CFGR_PPRE1_DIV4 |														// APB1 = 42 MHz
                RCC_CFGR_PPRE2_DIV2;														// APB2 = 84 MHz

    // Switching to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);	// wait until ready
		
	  // HSI off
    RCC->CR &= ~RCC_CR_HSION;
    while(RCC->CR & RCC_CR_HSIRDY); 														// wait until ready
}

int main(void) {
	  Clock_Init();
		volatile uint32_t adc_value2= 0;
		volatile uint32_t adc_value1= 0;
	
	  Init_ADC1();
	
		Init_ADC2();	
    Init_SPI1_display();
    ILI9341_Init();
    ILI9341_FillScreen(0xFFFF);
	  delay(100); 
	  ILI9341_FillRect( 5, 5, 230, 310, 0x0000);
	  delay(100); 
	  ILI9341_FillRect( 0, 310, 10, 10, 0x07E0);	//axsis y green
	  delay(100); 
		ILI9341_FillRect( 230, 0, 10, 10, 0x001F);	//axsis x red
	  delay(100); 
		ILI9341_FillRect( 5, 157, 230, 6, 0xFFFF);
		delay(100);
		ILI9341_DrawNumber_Scaled(220, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 1);
		ILI9341_DrawNumber_Scaled(200, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 2);
		ILI9341_DrawNumber_Scaled(175, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 3);
		ILI9341_DrawNumber_Scaled(140, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 4);
		ILI9341_DrawNumber_Scaled(100, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 5);
		//USART1_Init();
	
    while(1) {
		/*('A');
		delay(1000000);*/

			
			
			
			
			
			
			// Animation test 5 diffrent font size string
		/*ILI9341_DrawString_Scaled(10, 10, "0123456789", &Font_Custom, 0xFFFF, 0x0000, 1);
			ILI9341_DrawString_Scaled(10, 20, "0123456789", &Font_Custom, 0xFFFF, 0x0000, 2);
			ILI9341_DrawString_Scaled(10, 50, "0123456789", &Font_Custom, 0xFFFF, 0x0000, 3);
			ILI9341_DrawString_Scaled(10, 100, "0123456789", &Font_Custom, 0xFFFF, 0x0000, 4);
			delay(1000000);*/
			
			// Animation test 4 diffrent font size number
		/*ILI9341_DrawNumber_Scaled(220, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 1);
			ILI9341_DrawNumber_Scaled(200, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 2);
			ILI9341_DrawNumber_Scaled(175, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 3);
			ILI9341_DrawNumber_Scaled(140, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 4);
			ILI9341_DrawNumber_Scaled(100, 100, '6', &Font_Custom, 0xFFFF, 0x0000, 5);
			delay(1000000);*/

			
			
			
			// Train circle
			//ILI9341_FillCircle_Fast(120, 160, 50, 0x001F);
			//delay(1000);
			//ILI9341_FillCircle_Fast(120, 160, 40, 0x07E0);
			//rotations = Get_RPM_Count_3s();  
      //delay(1000);
	
			
			// Train Fillscreen speed. At first there were multiple fillscreen functions.
      /*ILI9341_FillScreen(0xF800);
			ILI9341_FillScreen(0x07E0);
			ILI9341_FillScreen(0x001F);
			ILI9341_FillScreen(0xFFFF);
			delay(100);*/
			
			
			
			//Animation test 1 diffrent square init
		/*uint16_t n = 0;
			uint16_t g = 0;	
			uint16_t v = 0;
			for(volatile uint32_t j = 0; j < 20; j++) {
				for(volatile uint32_t i = 0; i < 20; i++ ){
					ILI9341_FillRect(20+g, 20 + n, 4, 4, 0xF800);
					n=n+4;
					//delay(1000000);
				}
			n=0;
			g=g+4;
			}
			ILI9341_FillRect(120, 20, 80, 80, 0xF800);
			delay(1000000);
			
			// second cube
			g=0;
			for(volatile uint32_t j = 0; j < 20; j++) {
				for(volatile uint32_t i = 0; i < 20; i++ ){
					ILI9341_FillRect20 + g, 20 + v, 4, 4, 0x07E0);
					v=v+4;
					//delay(1000000);
				}
				v=0;
				g=g+4;	
			}
				ILI9341_FillRect(120, 20, 80, 80, 0x07E0);
				delay(1000000);*/





			//Animation test 2 moving square
		/*uint16_t x = 0;
			ILI9341_FillRect(120, 120 + x, 80, 80, 0xF800);
			for(volatile uint32_t j = 0; j < 20; j++){
				ILI9341_FillRect(120, 120 + x, 80, 4, 0x0000);
				ILI9341_FillRect(120, 200 + x, 80, 4, 0xF800);
			delay(60000);
			x=x+4;
			}

			ILI9341_FillRect(120, 120 + x, 80, 80, 0x07E0);
			for(volatile uint32_t j = 0; j < 20; j++){
				ILI9341_FillRect(120, 120 + x, 80, 4, 0x07E0);
				ILI9341_FillRect(120, 200 + x, 80, 4, 0x0000);
			delay(60000);
			x=x-4;
			}
			ILI9341_FillRect(120, 200, 80, 80, 0x000);*/
			
			
			
			
			//Animation test 3 numbers
		/*ILI9341_DrawNumber(100, 100, '0', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '1', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '2', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '3', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '4', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '5', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '6', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '7', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '8', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawNumber(100, 100, '9', &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_DrawString(100, 100, "0123456789", &Font_7x10, 0xFFFF, 0x0000);
			delay(1000000);
			ILI9341_FillRect( 100, 100, 110, 10, 0x0000);
			delay(1000000);	*/
    }
}
