/*
 * audio_interface.c
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#include "stm32f4xx_hal.h"
#include "i2c.h"

I2C_HandleTypeDef *i2c_handle;

void init_audio_output(){
    const uint16_t device_addr = 0b10010100;

    i2c_handle = get_i2c_handle();
    uint8_t config_data = 0x99;
    HAL_StatusTypeDef HAL_error;
    HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_SET);

    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x00, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0x80;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x47, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0x01 << 7;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x32, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0x00;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x32, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0x00;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x00, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }


}

