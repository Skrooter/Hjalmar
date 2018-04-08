/*
 * This file is part of Hjalmar.
 *
 * Hjalmar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hjalmar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hjalmar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdint.h>

#include "stm32f7xx_hal.h"

#include "audio_constants.h"
#include "audio_gen.h"
#include "audio_interface.h"
#include "i2c.h"
#include "i2s.h"


I2C_HandleTypeDef *i2c_handle;
I2S_HandleTypeDef *i2s_handle;
uint16_t *audio_buffer_0, *audio_buffer_1 = NULL;
float *gen_buffer = NULL;
uint8_t next_audio_buffer = 0;

typedef enum next_buffer_enum {
    BUFFER_0_NEXT,
    BUFFER_1_NEXT
} next_buffer_t;

next_buffer_t next_buffer = BUFFER_0_NEXT;

/*for(int i = 0; i < AUDIO_BUFFER_SIZE; i+=2){
    audio_buffer[i] = INT16_MAX*i/AUDIO_BUFFER_SIZE;
}*/

void init_audio_output(void)
{
    const uint16_t device_addr = 0b10010100;

    i2c_handle = get_i2c_handle();
    HAL_StatusTypeDef HAL_error;

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
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x08, 0x1, &config_data, 1, 1000)) != HAL_OK){
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

    config_data = 0b01110000;
    if ((HAL_error = HAL_I2C_Mem_Write(i2c_handle,device_addr, 0x0D, 0x1, &config_data, 1, 1000)) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }

    config_data = 0b00000100;
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
    audio_buffer_0 = calloc(sizeof(uint16_t), AUDIO_BUFFER_SIZE);
    audio_buffer_1 = calloc(sizeof(uint16_t), AUDIO_BUFFER_SIZE);
    gen_buffer = calloc(sizeof(float), GEN_BUFFER_SIZE);

    fetch_next_audio_buffer(gen_buffer, AUDIO_BUFFER_SIZE);
    mono_to_stereo(audio_buffer_0, gen_buffer, AUDIO_BUFFER_SIZE);

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

void mono_to_stereo (uint16_t *stereo_samples, float *mono_samples, uint16_t n_sample_mono)
{
    uint16_t current_sample_int;
    for(uint32_t i = 0; i < n_sample_mono; i++) {
        current_sample_int = (int16_t)(mono_samples[i] * INT16_MAX);
        stereo_samples[2 * i] =  current_sample_int;
        stereo_samples[(2 * i) + 1] = current_sample_int;
    }
}

void send_audio(void)
{
    if (next_buffer == BUFFER_0_NEXT) {
        if (HAL_I2S_Transmit_DMA(i2s_handle, audio_buffer_0, AUDIO_BUFFER_SIZE) != HAL_OK){
            _Error_Handler(__FILE__, __LINE__);
        }
        fetch_next_audio_buffer(gen_buffer, GEN_BUFFER_SIZE);
        mono_to_stereo(audio_buffer_1, gen_buffer, GEN_BUFFER_SIZE);
        next_buffer = BUFFER_1_NEXT;
    }
    else {
        if (HAL_I2S_Transmit_DMA(i2s_handle, audio_buffer_1, AUDIO_BUFFER_SIZE) != HAL_OK){
            _Error_Handler(__FILE__, __LINE__);
        }
        fetch_next_audio_buffer(gen_buffer, GEN_BUFFER_SIZE);
        mono_to_stereo(audio_buffer_0, gen_buffer, GEN_BUFFER_SIZE);
        next_buffer = BUFFER_0_NEXT;
    }

}

