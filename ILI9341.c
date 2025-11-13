#include "ILI9341.h"

void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

void ILI9341_WriteCommand(uint8_t cmd) {
    DC_LOW();    				  // ILI9341 get command 
    CS_LOW();     				// Start SPI transmit
    SPI1_Transmit(cmd);		// Transmit command
    CS_HIGH();    				// End SPI transmit
}

void ILI9341_WriteData(uint8_t data) {
    DC_HIGH();    				// ILI9341 get data 
    CS_LOW();     				// Start SPI transmit
    SPI1_Transmit(data);	// Transmit command
    CS_HIGH();    				// End SPI transmit
}

void ILI9341_Reset(void) {
    RST_HIGH();						// Start display 
    delay(10000);					// Stabilization delay
    RST_LOW();						// Reset display 
    delay(10000);					// Stabilization delay
    RST_HIGH();						// Start display 
    delay(10000);					// Stabilization delay
}

void ILI9341_Init(void) {
    ILI9341_Reset();												// Reset display to original setings
    ILI9341_WriteCommand(Sleep_OUT); 				// Get out of sleep mode 
    delay(1200000);             							// Stabilization delay (won't work without it)
    ILI9341_WriteCommand(Pixel_Format_Set);	// Set color
    ILI9341_WriteData(RGB_16_Bit);    			// 16-bit RGB
    ILI9341_WriteCommand(Display_ON);				// Turn on display
}

void ILI9341_FillScreen(uint16_t color) {
		// Split 16 bit color data in two 8 bit color data
    uint8_t hi = color >> 8;    // Get rid of last 8 bits and move first 8 bits to ther place
    uint8_t lo = color & 0xFF;  // Replace first 8 bits with zeroes
    
    ILI9341_WriteCommand(Column_Address_Set); 
    ILI9341_WriteData(0x00);		// Start of the column adress (SC)
    ILI9341_WriteData(0x00);		// 0 = 0x00
    ILI9341_WriteData(0x00);		// End of the column adress (EC)
    ILI9341_WriteData(0xEF);    // 239 = 0xEF
    
    ILI9341_WriteCommand(Page_Address_Set);
    ILI9341_WriteData(0x00);		// Start of the page adress (SP)
    ILI9341_WriteData(0x00);		// 0 = 0x00
    ILI9341_WriteData(0x01);		// End of the page adress (EP)
		ILI9341_WriteData(0x3F);    // 319 = 0x13F, so we split this data in two: 0x01 and 0x3F
	
    // Send color data to pixels
    ILI9341_WriteCommand(Memory_Write);
    DC_HIGH();								  // ILI9341 get data 
    CS_LOW();	 									// Start SPI transmit	
		
    // To optimize speed we will call SPI transmit right here
    for(uint32_t i = 0; i < 76800; i++) {		//(240 * 320 = 76800)
			
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = hi;											// Send high bite
        
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = lo;											// Send low bite
    }
    CS_HIGH(); 															// End SPI transmit
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    // If we accidentialy send wrong data
    if (x >= 240 || y >= 320) return;
	
 		// Like in the previous function we split our data in half to get pixel coordinates   
    ILI9341_WriteCommand(Column_Address_Set);
    ILI9341_WriteData(x >> 8);		// Start of the column adress (SC)
    ILI9341_WriteData(x & 0xFF); 
    ILI9341_WriteData(x >> 8);		// End of the column adress (EC)
    ILI9341_WriteData(x & 0xFF);
    
    ILI9341_WriteCommand(Page_Address_Set);
    ILI9341_WriteData(y >> 8);		// Start of the page adress (SP)
    ILI9341_WriteData(y & 0xFF);
    ILI9341_WriteData(y >> 8);		// End of the page adress (EP)
    ILI9341_WriteData(y & 0xFF);
    
    // Send color data to pixel
    ILI9341_WriteCommand(Memory_Write);
    ILI9341_WriteData(color >> 8);
    ILI9341_WriteData(color & 0xFF);
}

void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
		// Split 16 bit color data in two 8 bit color data
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
	
		// Define size of rectangle
    uint32_t pixels = width * height;
    
    ILI9341_WriteCommand(Column_Address_Set);
    ILI9341_WriteData(x >> 8);									// Start of the column adress (SC)
    ILI9341_WriteData(x & 0xFF);
    ILI9341_WriteData((x + width - 1) >> 8);		// End of the column adress (EC)
    ILI9341_WriteData((x + width - 1) & 0xFF);
    
    ILI9341_WriteCommand(Page_Address_Set);
    ILI9341_WriteData(y >> 8);									// Start of the page adress (SP)
    ILI9341_WriteData(y & 0xFF);
    ILI9341_WriteData((y + height - 1) >> 8);		// End of the page adress (EP)
    ILI9341_WriteData((y + height - 1) & 0xFF);
 
		// Send color data to pixels
    ILI9341_WriteCommand(Memory_Write);
    DC_HIGH();								  						// ILI9341 get data 
    CS_LOW();	 															// Start SPI transmit	
		
		// To optimize speed we will call SPI transmit right here
    for(uint32_t i = 0; i < pixels + 1; i++) {
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = hi;											// Send high bite
        
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = lo;											// Send low bite
    }
    CS_HIGH(); 															// End SPI transmit
		delay(1000);
}

void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        ILI9341_DrawPixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ILI9341_FillCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color) {
    uint16_t x = radius;
    uint16_t y = 0;
    uint16_t decision = 1 - x;
    
    while (x >= y) {
        ILI9341_FillRect(x0 - x, y0 + y, 2 * x, 1, color); 
        ILI9341_FillRect(x0 - x, y0 - y, 2 * x, 1, color);
        ILI9341_FillRect(x0 - y, y0 + x, 2 * y, 1, color);
        ILI9341_FillRect(x0 - y, y0 - x, 2 * y, 1, color);
        
        y++;
        if (decision <= 0) {
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}

void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	  // This function does 
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);
    ILI9341_WriteData(x2 >> 8);
    ILI9341_WriteData(x2 & 0xFF);
    
    
    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);
    ILI9341_WriteData(y2 >> 8);
    ILI9341_WriteData(y2 & 0xFF);
    
    
    ILI9341_WriteCommand(0x2C);
}

void ILI9341_DrawNumber(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor) {
    const uint16_t* char_data = GetFontData(ch, font);
    
    for (uint8_t row = 0; row < font->height; row++) {
        uint16_t line = char_data[row];

        for (uint8_t col = 0; col < font->width; col++) {
            if (line & (1 <<  col)) {
                ILI9341_DrawPixel(x + col, y + row, color);
            } else if (bgcolor != color) { 
                ILI9341_DrawPixel(x + col, y + row, bgcolor);
            }
        }
    }
}

void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, FontDef* font, uint16_t color, uint16_t bgcolor) {

    const char* end = str;
    while (*end) end++;
    
    uint16_t current_x = x;
    while (end > str) {
        end--;
        ILI9341_DrawNumber(current_x, y, *end, font, color, bgcolor);
        current_x += font->width + 1;
    }
}

void ILI9341_DrawNumber_Scaled(uint16_t x, uint16_t y, char ch, FontDef* font, 
                               uint16_t color, uint16_t bgcolor, uint8_t scale) {
    const uint16_t* char_data = GetFontData(ch, font);
    
    for (uint8_t row = 0; row < font->height; row++) {
        uint16_t line = char_data[row];

        for (uint8_t col = 0; col < font->width; col++) {
            uint16_t block_x = x + col * scale;
            uint16_t block_y = y + row * scale;
            
            if (line & (1 << col)) {
                ILI9341_FillRect(block_x, block_y, scale, scale, color);
            } else if (bgcolor != color) {
                ILI9341_FillRect(block_x, block_y, scale, scale, bgcolor);
            }
        }
    }
}
															 
void ILI9341_DrawString_Scaled(uint16_t x, uint16_t y, const char* str, FontDef* font, uint16_t color, uint16_t bgcolor, uint8_t scale) {
		const char* end = str;
    while (*end) end++;
	
    uint16_t current_x = x;
    
    while (end > str) {
				end--;
        ILI9341_DrawNumber_Scaled(current_x, y, *end, font, color, bgcolor, scale);
        current_x += (font->width + 1) * scale;
    }
}
