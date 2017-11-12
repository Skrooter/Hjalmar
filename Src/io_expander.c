/*
 * io_expander.c
 *
 *  Created on: Nov 12, 2017
 *      Author: jaxc
 */

#include "stm32f4xx_hal.h"
#include "i2c.h"

const uint16_t device_addr = 0b0100000;

void init_io_expander(void){
    I2C_HandleTypeDef *i2c_handle = get_i2c_handle();
    uint8_t config_data = 0xFF;
    HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x00, 0x1, &config_data, 1, 1000);
    config_data = 0xFF;
    HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x02, 0x1, &config_data, 1, 1000);
    config_data = 0xFF;
    HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x06, 0x1, &config_data, 1, 1000);
    config_data = 0xFF;
    HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x00, 0x1, &config_data, 1, 1000);
}


