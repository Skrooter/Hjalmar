/*
 * audio_filter.c
 *
 *  Created on: Jan 13, 2018
 *      Author: jaxc
 */

#include <stdint.h>

#define N_FILTER_TAPS 2

struct filter_taps_t{
    float x[N_FILTER_TAPS];
    float y[N_FILTER_TAPS];
};

struct filter_taps_t filter_taps;

float x_buf[N_FILTER_TAPS];
float y_buf[N_FILTER_TAPS];

void init_filter (void)
{
    for(uint8_t i = 0; i < N_FILTER_TAPS; i++) {
        filter_taps.x[i] = 0;
        filter_taps.y[i] = 0;
        x_buf[i] = 0;
        y_buf[i] = 0;
    }
}

void filter_calculate_samples (float *sample_level, uint16_t n_samples)
{
    for(uint16_t i = 0; i < n_samples; i++) {
        x_buf[0] = sample_level[i];
        sample_level[0] = filter_taps.x[0] * x_buf[0];

        for(uint8_t j = 1; j < N_FILTER_TAPS; j++) {
            sample_level[i] = filter_taps.x[j] * x_buf[j] + filter_taps.y[j] * y_buf[j];
        }
        y_buf[0] = sample_level[i];
        for(uint8_t j = N_FILTER_TAPS - 1; j > 0 ; j--) {
            y_buf[j] = y_buf[j - 1];
            x_buf[j] = x_buf[j - 1];
        }


    }
}

