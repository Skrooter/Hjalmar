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
#include "math.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

uint32_t sample_per_sec;
audio_wave_type_t wave_type = WAVE_NONE;
uint32_t wave_period;
uint32_t sample_position;
int16_t last_value;

int pulse_pos = 2;

void audio_gen_sqr_start(float freq, uint8_t level)
{
    I2S_HandleTypeDef *i2s_handle = get_i2s_handle();
    sample_per_sec = i2s_handle->Init.AudioFreq;
    wave_type = WAVE_SAW;
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
                buffer[i] = last_value;
                if (sample_position >= wave_period) {
                    sample_position = 0;
                }
                else {
                    sample_position++;
                }
            }
            else{
                sample_position++;
                buffer[i] = -last_value;
            }

        }
        break;
    case WAVE_SAW:
        for(int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
            if (sample_position >= 0xFF){
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
            if (sample_position >= wave_period){
                sample_position = 0;
            }
            else{
                sample_position++;
            }

            if (i == pulse_pos) {
                buffer[i] = 0xF;
            }
            else {
                buffer[i] = 0;
            }
        }
        break;

    case WAVE_SINE:
        for(int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
            if (sample_position >= wave_period){
                sample_position = 0;
            }
            else{
                sample_position++;
            }

            buffer[i] = (int16_t) last_value * sin(2 * M_PI * sample_position / wave_period);
        }
        break;

    default:
        break;
    }
    return (uint16_t *)buffer;
}
