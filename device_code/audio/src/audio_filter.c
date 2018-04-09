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

#include <math.h>
#include <stdint.h>

#include "audio_constants.h"

#define N_FILTER_TAPS 3

struct filter_taps_t{
    float a[N_FILTER_TAPS];
    float b[N_FILTER_TAPS];

};

float q_value;
float cutoff_omega;
float alpha;
float sample_freq_res;

struct filter_taps_t filter_taps, filter_parameters;

float filter_buffer[N_FILTER_TAPS - 1] = {0};

void update_filtertaps(void)
{
    float devisor = 1.0 / filter_parameters.a[0];
    filter_taps.a[0] = filter_parameters.b[0] * devisor;
    filter_taps.a[1] = filter_parameters.b[1] * devisor;
    filter_taps.a[2] = filter_parameters.b[2] * devisor;
    filter_taps.b[1] = filter_parameters.a[1] * devisor;
    filter_taps.b[2] = filter_parameters.a[2] * devisor;
}

void update_alpha(void)
{
    alpha = sinf(cutoff_omega) / (2 * q_value);
}

void change_q(float new_q)
{
    q_value = new_q;
    update_alpha();

    filter_parameters.a[0] = 1 + alpha;
    filter_parameters.a[2] = 1 - alpha;

    update_filtertaps();
}

void change_cutoff(float new_cutoff)
{
    cutoff_omega = 2 * M_PI * new_cutoff * sample_freq_res ;

    update_alpha();

    float cos_cutoff = cosf(cutoff_omega);

    filter_parameters.a[0] = 1 + alpha;
    filter_parameters.a[1] = -2 * cos_cutoff;
    filter_parameters.a[2] = 1 - alpha;
    filter_parameters.b[0] = (1 - cos_cutoff) / 2;
    filter_parameters.b[1] = 1 - cos_cutoff;
    filter_parameters.b[2] = (1 - cos_cutoff) / 2;

    update_filtertaps();
}

void change_cutoff_and_q(float new_cutoff, float new_q)
{
    cutoff_omega = 2 * M_PI * new_cutoff * sample_freq_res ;
    q_value = new_q;
    update_alpha();

    float cos_cutoff = cosf(cutoff_omega);

    filter_parameters.a[0] = 1 + alpha;
    filter_parameters.a[1] = -2 * cos_cutoff;
    filter_parameters.a[2] = 1 - alpha;
    filter_parameters.b[0] = (1 - cos_cutoff) / 2;
    filter_parameters.b[1] = 1 - cos_cutoff;
    filter_parameters.b[2] = (1 - cos_cutoff) / 2;

    update_filtertaps();
}

void init_filter (void)
{
    for(uint8_t i = 0; i < N_FILTER_TAPS; i++) {
        filter_taps.a[i] = 0;
        filter_taps.b[i] = 0;
    }

    sample_freq_res = 1.0 / HJALMAR_AUDIOFREQ;

    change_cutoff_and_q(10000, 1);


}

void filter_calculate_samples (float *sample_level, uint16_t n_samples)
{
    for(uint16_t i = 0; i < n_samples; i++) {
        float filter_output = 0;

        filter_output = filter_taps.a[0] * sample_level[i];

        filter_output += filter_buffer[0];

        for(uint8_t j = 0; j < N_FILTER_TAPS - 2; j++) {
            filter_buffer[j] = filter_buffer[j + 1] +
                    sample_level[i] * filter_taps.a[j + 1] -
                    filter_output * filter_taps.b[j + 1];
        }

        filter_buffer[N_FILTER_TAPS - 2] =
                sample_level[i] * filter_taps.a[N_FILTER_TAPS - 1] -
                filter_output * filter_taps.b[N_FILTER_TAPS - 1];

        sample_level[i] = filter_output;

    }
}
