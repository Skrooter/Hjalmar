/*
 * audio_gen.c
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#include <stdint.h>
#include <stdlib.h>

#include "audio_gen.h"
#include "audio_interface.h"
#include "i2s.h"

uint32_t sample_per_sec;
audio_wave_type_t wave_type = WAVE_NONE;
uint32_t wave_period;
uint32_t sample_position;
int16_t last_value;

void audio_gen_sqr_start(float freq, uint8_t level)
{
    I2S_HandleTypeDef *i2s_handle = get_i2s_handle();
    sample_per_sec = i2s_handle->Init.AudioFreq;
    wave_type = WAVE_PULSE;
    wave_period = sample_per_sec / freq;
    sample_position = 0;
    last_value = (INT16_MIN / (128 - level))+1;

}

uint16_t *fetch_next_audio_buffer(void)
{
    int16_t *buffer = calloc(sizeof(int16_t), AUDIO_BUFFER_SIZE);
    switch (wave_type){
    case WAVE_SQUARE:
        for(int i = 0; i < AUDIO_BUFFER_SIZE; i++){
            if (sample_position >= (wave_period / 2)){
                sample_position = 0;
                last_value = -last_value;
            }
            else{
                sample_position++;
            }
            buffer[i] = last_value;
        }
        break;
    case WAVE_SAW:
        for(int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
            if (sample_position >= (wave_period / 2)){
                sample_position = 0;
            }
            else{
                sample_position++;
            }
            buffer[i] = sample_position;
        }
        break;

    case WAVE_PULSE:
        for(int i = 0; i < AUDIO_BUFFER_SIZE; i++){
            if (sample_position >= (wave_period / 2)){
                sample_position = 0;
            }
            else{
                sample_position++;
            }
            if (i == 0) {
                buffer[i] = 0xF;
            }
            else {
                buffer[i] = 0;
            }
        }
        break;

    default:
        break;
    }
    return (uint16_t *)buffer;
}
