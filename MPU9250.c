#include "MPU9250.h"

// in process

uint8_t MPU9250_ReadReg(uint8_t reg) {
    CS2_LOW();       
    
    SPI2_ReadWrite(reg | 0x80);         
    uint8_t value = SPI2_ReadWrite(0);
    
    CS2_HIGH();       // CS high
    return value;
}

void MPU9250_WriteReg(uint8_t reg, uint8_t value) {
    CS2_LOW();       // CS low
    
    SPI2_ReadWrite(reg & 0x7F);        
    SPI2_ReadWrite(value);             
    
    CS2_HIGH();       // CS high
}

void MPU9250_Init(void) {
    CS2_HIGH();          // CS high initially
   
    
    MPU9250_WriteReg(0x6B, 0x80);  // PWR_MGMT_1 - Device Reset
    while(MPU9250_ReadReg(0x6B) & 0x80);
    
    MPU9250_WriteReg(0x6B, 0x00);  // PWR_MGMT_1 - Normal operation
    MPU9250_WriteReg(0x6A, 0x10);  // USER_CTRL - Enable SPI
}
