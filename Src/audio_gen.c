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
float wave_period;
float sample_position = 0.;
int16_t wave_max_amplitude, wave_min_amplitude;
uint16_t wave_amplitude;
int16_t SQ_MAX = 0;
uint8_t wave_state;

uint8_t mute_output = 0;

float pulse_duty_cycle = 0.3;

void audio_gen_wave_start(float freq, uint8_t level)
{
    I2S_HandleTypeDef *i2s_handle = get_i2s_handle();
    sample_per_sec = i2s_handle->Init.AudioFreq;
    wave_period = sample_per_sec / (float)freq;
    wave_max_amplitude = GEN_MAX_AMPLITUDE * level;
    wave_min_amplitude = GEN_MIN_AMPLITUDE * level;
    wave_amplitude = wave_max_amplitude - wave_min_amplitude;
    mute_output = 0;
    wave_state = 0;
    pulse_duty_cycle = 0.05;
}

void audio_gen_wave_form(audio_wave_type_t wave_type)
{
    active_wave_type = wave_type;
}

void audio_gen_wave_stop(void)
{
    mute_output = 1;
}

void fetch_next_audio_buffer(uint16_t *audio_samples, uint16_t n_sample)
{
    if (mute_output == 1) {
        for(int i = 0; i < n_sample; i++) {
            audio_samples[i] = 0;
        }
        sample_position = 0.;
    }
    else {
        switch (active_wave_type) {
        case WAVE_SQUARE:
            for(int i = 0; i < n_sample; i++){
                if (sample_position >= (wave_period / 2)){
                    audio_samples[i] = wave_max_amplitude;
                    if (sample_position >= wave_period) {
                        sample_position -= wave_period;
                    }
                    else {
                        sample_position++;
                    }
                }
                else{
                    sample_position += 1.0f;
                    audio_samples[i] = wave_min_amplitude;
                }

            }
            break;
        case WAVE_SAW:
            for(int i = 0; i < n_sample; i++) {
                if (wave_state == 0) {

                    if (sample_position >= wave_period / 2){
                        wave_state = 1;
                        audio_samples[i] = (int16_t)((1 - (sample_position / wave_period)) * wave_min_amplitude * 2);
                    }
                    else {
                        audio_samples[i] = (sample_position / wave_period) * wave_max_amplitude * 2;
                    }
                    sample_position += 1.0;
                }
                else {
                    audio_samples[i] = (int16_t)((1 - (sample_position / wave_period)) * wave_min_amplitude * 2);

                    if (sample_position >= wave_period) {
                        sample_position -= wave_period;
                        wave_state = 0;
                    }
                    else {
                        sample_position += 1.0;
                    }
                }
            }
            break;

        case WAVE_PULSE:
            for(int i = 0; i < n_sample; i++) {
                if (sample_position >= wave_period) {
                    sample_position = 0;
                }
                else {
                    sample_position += 1.0;
                }

                if (sample_position >= wave_period * pulse_duty_cycle) {
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
                    sample_position -= wave_period;
                }
                else{
                    sample_position += 1.0;
                }
                double gen_sin = wave_max_amplitude * sinf(2 * M_PI * sample_position / wave_period);
                audio_samples[i] = (int16_t) gen_sin;
            }
            break;

        case WAVE_TRIANGLE:
            for(int i = 0; i < n_sample; i++) {
                if (wave_state == 0) {

                    if (sample_position >= wave_period / 4){
                        wave_state = 1;
                        audio_samples[i] = (int16_t)((0.5 - (sample_position / wave_period)) * wave_max_amplitude * 4);
                    }
                    else {
                        audio_samples[i] = (sample_position / wave_period) * wave_max_amplitude * 4;
                    }
                    sample_position += 1.0;
                }
                else if (wave_state == 1) {

                    if (sample_position >= (wave_period * 3 / 4)) {
                        wave_state = 2;
                        audio_samples[i] =(int16_t)((1 -(sample_position / wave_period)) * wave_min_amplitude * 4);
                    }
                    else {
                        audio_samples[i] = (int16_t)((0.5 - (sample_position / wave_period)) * wave_max_amplitude * 4);
                    }
                    sample_position += 1.0;
                }
                else {
                    audio_samples[i] =(int16_t)((1 -(sample_position / wave_period)) * wave_min_amplitude * 4);

                    if (sample_position >= wave_period) {
                        sample_position -= wave_period;
                        wave_state = 0;
                    }
                    else {
                        sample_position += 1.0;
                    }
                }
            }
            break;

        default:
            for(int i = 0; i < n_sample; i++) {
                audio_samples[i] = 0;
            }
            sample_position = 0;
            break;
        }
    }
    return;
}
