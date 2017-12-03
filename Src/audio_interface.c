/*
 * audio_interface.c
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "audio_gen.h"
#include "audio_interface.h"
#include "i2c.h"
#include "i2s.h"


I2C_HandleTypeDef *i2c_handle;
I2S_HandleTypeDef *i2s_handle;
uint16_t *audio_buffer_0, *audio_buffer_1;
uint8_t next_audio_buffer = 0;


/*for(int i = 0; i < AUDIO_BUFFER_SIZE; i+=2){
    audio_buffer[i] = INT16_MAX*i/AUDIO_BUFFER_SIZE;
}*/

void init_audio_output(){
    const uint16_t device_addr = 0b10010100;

    i2c_handle = get_i2c_handle();
    HAL_StatusTypeDef HAL_error;
    HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_SET);

    uint8_t config_data = 0b10011110;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x02, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0b10101111;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x04, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0b00100000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x05, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000100;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x06, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x07, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x09, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0A, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0C, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b11100000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0D, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000111;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0E, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0F, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x1A, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x1B, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }


/*    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x1C, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000110;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x1D, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b11000000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x1E, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }*/

    /*config_data = 0b00011000;
    if ((HAL_error = HAL_I2C_Mem_Read(i2c_handle,device_addr, 0x20, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    config_data = 0b00011000;
    if ((HAL_error = HAL_I2C_Mem_Read(i2c_handle,device_addr, 0x21, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }*/

    if ((HAL_error = HAL_I2C_Mem_Read(i2c_handle,device_addr, 0x2E, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    if (config_data != 0){
        _Error_Handler(__FILE__, __LINE__);
    }
    i2s_handle = get_i2s_handle();
    audio_buffer_0 = calloc(sizeof(int16_t), AUDIO_BUFFER_SIZE);
    audio_buffer_1 = calloc(sizeof(int16_t), AUDIO_BUFFER_SIZE);
    send_audio();
}

/*void add_to_audio_buffer(int16_t *audio_buffer)
{
    if(next_audio_buffer){
        audio_buffer_1 = audio_buffer;
    }
    else {
        audio_buffer_0 = audio_buffer;
    }
}*/

void send_audio(void){
    if(audio_buffer_0){
        free(audio_buffer_0);
        audio_buffer_0 = NULL;
    }
    audio_buffer_0 = audio_buffer_1;
    if (HAL_I2S_Transmit_DMA(i2s_handle, audio_buffer_0, AUDIO_BUFFER_SIZE) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
    audio_buffer_1 = fetch_next_audio_buffer();

}

