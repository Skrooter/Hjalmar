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
audio_wave_type_t active_wave_type = WAVE_NONE;
uint32_t wave_period;
uint32_t sample_position;
int16_t wave_max_amplitude, wave_min_amplitude;
int16_t SQ_MAX = 0;

int pulse_pos = 15;

void audio_gen_wave_start(float freq, uint8_t level, audio_wave_type_t wave_type )
{
    I2S_HandleTypeDef *i2s_handle = get_i2s_handle();
    sample_per_sec = i2s_handle->Init.AudioFreq;
    active_wave_type = wave_type;
    wave_period = sample_per_sec / freq;
    sample_position = 0;
    wave_max_amplitude = GEN_MAX_AMPLITUDE * level;
    wave_min_amplitude = GEN_MIN_AMPLITUDE * level;

}

void fetch_next_audio_buffer(uint16_t *audio_samples, uint16_t n_sample)
{
    switch (active_wave_type){
    case WAVE_SQUARE:
        for(int i = 0; i < n_sample; i++){
            if (sample_position >= (wave_period / 2)){
                audio_samples[i] = wave_max_amplitude;
                if (sample_position >= wave_period) {
                    sample_position = 0;
                }
                else {
                    sample_position++;
                }
            }
            else{
                sample_position++;
                audio_samples[i] = wave_min_amplitude;
            }

        }
        break;
    case WAVE_SAW:
        for(int i = 0; i < n_sample; i++) {
            if (sample_position >= wave_max_amplitude){
                sample_position = 0x0000;
            }
            else{
                sample_position += wave_max_amplitude / wave_period;
            }
            audio_samples[i] = sample_position - wave_max_amplitude;
        }
        break;

    case WAVE_PULSE:
        for(int i = 0; i < n_sample; i++) {
            if (sample_position >= wave_period) {
                sample_position = 0;
            }
            else {
                sample_position++;
            }

            if (i >= pulse_pos) {
                audio_samples[i] = wave_max_amplitude;
            }
            else {
                audio_samples[i] = wave_min_amplitude;
            }
        }
        break;

    case WAVE_SINE:
        for(int i = 0; i < n_sample; i++) {
            if (sample_position >= wave_period){
                sample_position = 0;
            }
            else{
                sample_position++;
            }
            double gen_sin = wave_max_amplitude * sin(2 * M_PI * sample_position / wave_period);
            audio_samples[i] = (int16_t) gen_sin;
        }
        break;

    case WAVE_TRIANGLE:
        for(int i = 0; i < n_sample; i++) {

            sample_position += 0xFFFF / wave_period;
            if (sample_position >= 2 * 0xFFFF){
                sample_position = 0x0000;
                audio_samples[i] = 0x8000;
            }
            else if (sample_position < 0xFFFF) {
                audio_samples[i] = sample_position - 0x7FFF;
            }
            else {
                audio_samples[i] = (0xFFFF + 0x7FFF) - sample_position;
            }

        }
        break;

    default:
        break;
    }

    return;
}
