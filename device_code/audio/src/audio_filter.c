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

