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
#include <stdlib.h>

#include "math.h"

#include "audio_constants.h"
#include "audio_filter.h"
#include "audio_gen.h"
#include "audio_interface.h"
#include "envelope.h"
#include "polyphony_control.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Maximum allowed slew, measured in x/2% change per sample*/
#define MAX_SLEW (1.0/16)

uint8_t slow_slew_enabled = 0;
float last_sample = 0;


audio_wave_type_t active_wave_type;

void audio_gen_wave_form(audio_wave_type_t wave_type)
{
    if ( wave_type < N_WAVES) {
        active_wave_type = wave_type;
    }
    return;
}

void enable_slow_slew(void)
{
    slow_slew_enabled = 1;
    return;
}

void disable_slow_slew(void)
{
    slow_slew_enabled = 0;
    return;
}

void slew_adjust(uint16_t n_sample, float *voice_samples)
{
    if (slow_slew_enabled) {
        for(int i = 0; i < n_sample; i++) {
            float sample_slope = fabsf(voice_samples[i] - last_sample);
            if (sample_slope >= MAX_SLEW) {
                if (sample_slope < last_sample) {
                    voice_samples[i] = last_sample - MAX_SLEW;
                } else {
                    voice_samples[i] = last_sample + MAX_SLEW;
                }
            }
            last_sample = voice_samples[i];
        }
    }
}

float gen_square(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->sample_position >= (voice_data->wave_period / 2)){
        next_sample = voice_data->wave_amplitude;
        if (voice_data->sample_position >= voice_data->wave_period) {
            voice_data->sample_position -= voice_data->wave_period;
        }
        else {
            voice_data->sample_position += 1.0f;
        }
    }
    else{
        voice_data->sample_position += 1.0f;
        next_sample = -voice_data->wave_amplitude;
    }
    return next_sample;
}

float gen_saw(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->wave_state == 0) {

        if (voice_data->sample_position >= voice_data->wave_period / 2){
            voice_data->wave_state = 1;
            next_sample = (voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude);
        }
        else {
            next_sample = voice_data->slope * voice_data->sample_position;
        }
        voice_data->sample_position += 1.0;
    }
    else {
        next_sample = (voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2);

        if (voice_data->sample_position >= voice_data->wave_period) {
            voice_data->sample_position -= voice_data->wave_period;
            voice_data->wave_state = 0;
        }
        else {
            voice_data->sample_position += 1.0;
        }
    }
    return next_sample;
}

float gen_pulse(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->sample_position >= voice_data->wave_period) {
        voice_data->sample_position = 0;
    }
    else {
        voice_data->sample_position += 1.0;
    }

    if (voice_data->sample_position >= voice_data->pulse_duty_samples) {
        next_sample = voice_data->wave_amplitude;
    }
    else {
        next_sample = -voice_data->wave_amplitude;
    }
    return next_sample;
}

float gen_sine(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->sample_position >= voice_data->wave_period){
        voice_data->sample_position -= voice_data->wave_period;
    }
    else{
        voice_data->sample_position += 1.0;
    }
    next_sample = voice_data->wave_amplitude *
                   sinf(voice_data->rads * voice_data->sample_position );
    return next_sample;
}

float gen_triangle(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->wave_state == 0) {
        if (voice_data->sample_position >= voice_data->wave_period / 4){
            voice_data->wave_state = 1;
            next_sample = 2 * voice_data->wave_amplitude - voice_data->slope *
                            voice_data->sample_position;
        }
        else {
            next_sample = voice_data->sample_position * voice_data->slope;
        }
        voice_data->sample_position += 1.0;
    }
    else if (voice_data->wave_state == 1) {

        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
            voice_data->wave_state = 2;
            next_sample = voice_data->slope * voice_data->sample_position -
                            voice_data->wave_amplitude * 4;
        }
        else {
            next_sample = 2 * voice_data->wave_amplitude - voice_data->slope *
                            voice_data->sample_position;
        }
        voice_data->sample_position += 1.0;
    }
    else {
        next_sample = voice_data->slope * voice_data->sample_position -
                        voice_data->wave_amplitude * 4;

        if (voice_data->sample_position >= voice_data->wave_period) {
            voice_data->sample_position -= voice_data->wave_period;
            voice_data->wave_state = 0;
        }
        else {
            voice_data->sample_position += 1.0;
        }
    }
    return next_sample;
}

float gen_inverted_triangle(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->wave_state == 0) {
        if (voice_data->sample_position >= voice_data->wave_period / 4){
            voice_data->wave_state = 1;
            next_sample = voice_data->slope * voice_data->sample_position -
                            voice_data->wave_amplitude;
        }
        else {
            next_sample = voice_data->wave_amplitude - voice_data->slope *
                            voice_data->sample_position;
        }
        voice_data->sample_position += 1.0;
    }
    else if (voice_data->wave_state == 1) {

        if (voice_data->sample_position >= (voice_data->wave_period / 2)) {
            voice_data->wave_state = 2;
            next_sample = voice_data->slope * voice_data->sample_position -
                            voice_data->wave_amplitude * 3;
        }
        else {
            next_sample = voice_data->slope * voice_data->sample_position -
                            voice_data->wave_amplitude;
        }
        voice_data->sample_position += 1.0;
    }
    else if (voice_data->wave_state == 2) {

        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
            voice_data->wave_state = 3;
            next_sample = 3 * voice_data->wave_amplitude - voice_data->slope *
                            voice_data->sample_position;
        }
        else {
            next_sample = voice_data->slope * voice_data->sample_position -
                            voice_data->wave_amplitude * 3;
        }
        voice_data->sample_position += 1.0;
    }
    else {
        next_sample = 3 * voice_data->wave_amplitude - voice_data->slope *
                        voice_data->sample_position;

        if (voice_data->sample_position >= voice_data->wave_period) {
            voice_data->sample_position -= voice_data->wave_period;
            voice_data->wave_state = 0;
        }
        else {
            voice_data->sample_position += 1.0;
        }
    }
    return next_sample;
}

float gen_double_saw(struct wave_information *voice_data)
{
    float next_sample = 0;
    if (voice_data->wave_state == 0) {

        if (voice_data->sample_position >= voice_data->wave_period / 4){
            voice_data->wave_state = 1;
            next_sample = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude;
        }
        else {
            next_sample = voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
        }
        voice_data->sample_position += 1.0;
    }
    else if (voice_data->wave_state == 1) {

        if (voice_data->sample_position >= (voice_data->wave_period * 2 / 4)) {
            voice_data->wave_state = 2;
            next_sample = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
        }
        else {
            next_sample = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
        }
        voice_data->sample_position += 1.0;
    }
    else if (voice_data->wave_state == 2) {

        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
            voice_data->wave_state = 3;
            next_sample = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
        }
        else {
            next_sample = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
        }
        voice_data->sample_position += 1.0;
    }
    else {
        next_sample = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;

        if (voice_data->sample_position >= voice_data->wave_period) {
            voice_data->sample_position -= voice_data->wave_period;
            voice_data->wave_state = 0;
        }
        else {
            voice_data->sample_position += 1.0;
        }
    }
    return next_sample;
}

void generate_wave(struct wave_information *wave_data, uint16_t n_sample,
                    float *voice_samples)
{
    switch (active_wave_type) {
    case WAVE_SQUARE:
        for(int j = 0; j < n_sample; j++){
            voice_samples[j] = gen_square(wave_data);
        }

        break;
    case WAVE_SAW:
        wave_data->slope = 2 * wave_data->wave_amplitude / wave_data->wave_period;
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_saw(wave_data);
        }
        break;

    case WAVE_PULSE:
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_pulse(wave_data);
        }
        break;

    case WAVE_SINE:
        wave_data->rads = 2 * M_PI / wave_data->wave_period;
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_sine(wave_data);
        }
        break;

    case WAVE_TRIANGLE:
        wave_data->slope = 4 * wave_data->wave_amplitude / wave_data->wave_period;
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_triangle(wave_data);
        }
        break;

    case WAVE_INVERTED_TRIANGLE:
        wave_data->slope = 4 * wave_data->wave_amplitude / wave_data->wave_period;
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_inverted_triangle(wave_data);
        }
        break;
    case WAVE_DOUBLE_SAW:
        wave_data->slope = 4 * wave_data->wave_amplitude / wave_data->wave_period;
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = gen_double_saw(wave_data);
        }
        break;
    default:
        for(int j = 0; j < n_sample; j++) {
            voice_samples[j] = 0;
        }
        wave_data->sample_position = 0;
        break;
    }
}

void fetch_next_audio_buffer(float *audio_samples, uint16_t n_sample)
{
    uint8_t active_polyphony = get_n_active_voices();


    float voice_samples[active_polyphony][n_sample];
    for(uint8_t i = 0; i < active_polyphony; i++) {

        struct wave_information *voice_data;
        get_voice_information(&voice_data, i);

        voice_data->mute_output = voice_data->env_var.release_done;
        if (voice_data->mute_output == 1) {
            complete_release_voice(i);
            for(int i = 0; i < n_sample; i++) {
                audio_samples[i] = 0;
            }
            voice_data->sample_position = 0.;
        }
        else {
            generate_wave(voice_data, n_sample, voice_samples[i]);

            get_sample_envelope(&voice_data->env_var, voice_samples[i], n_sample);
        }

    }

    if(active_polyphony == 0) {
        for(int i = 0; i < n_sample; i++) {
            audio_samples[i] = 0;
        }

    }
    else {
        for (uint16_t j = 0; j < n_sample; j++){
            audio_samples[j] = 0;
            for(uint8_t i = 0; i < active_polyphony; i++){
               audio_samples[j] += voice_samples[i][j];
            }
            audio_samples[j] = audio_samples[j] * (1.0 / POLYPHONY_VOICES);
        }
        filter_calculate_samples(audio_samples, n_sample);
    }

    return;
}
