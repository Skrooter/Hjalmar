/*
 * audio_gen.c
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#include <stdint.h>
#include <stdlib.h>

#include "math.h"

#include "audio_gen.h"
#include "audio_interface.h"
#include "envelope.h"
#include "polyphony_control.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

audio_wave_type_t active_wave_type;

void audio_gen_wave_form(audio_wave_type_t wave_type)
{
    if ( wave_type < N_WAVES) {
        active_wave_type = wave_type;
    }
    return;
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

            switch (active_wave_type) {
            case WAVE_SQUARE:
                for(int j = 0; j < n_sample; j++){
                    if (voice_data->sample_position >= (voice_data->wave_period / 2)){
                        voice_samples[i][j] = voice_data->wave_amplitude;
                        if (voice_data->sample_position >= voice_data->wave_period) {
                            voice_data->sample_position -= voice_data->wave_period;
                        }
                        else {
                            voice_data->sample_position += 1.0f;
                        }
                    }
                    else{
                        voice_data->sample_position += 1.0f;
                        voice_samples[i][j] = - voice_data->wave_amplitude;
                    }

                }
                break;
            case WAVE_SAW:
                voice_data->slope = 2 * voice_data->wave_amplitude / voice_data->wave_period;
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->wave_state == 0) {

                        if (voice_data->sample_position >= voice_data->wave_period / 2){
                            voice_data->wave_state = 1;
                            voice_samples[i][j] = (voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude);
                        }
                        else {
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else {
                        voice_samples[i][j] = (voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2);

                        if (voice_data->sample_position >= voice_data->wave_period) {
                            voice_data->sample_position -= voice_data->wave_period;
                            voice_data->wave_state = 0;
                        }
                        else {
                            voice_data->sample_position += 1.0;
                        }
                    }
                }
                break;

            case WAVE_PULSE:
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->sample_position >= voice_data->wave_period) {
                        voice_data->sample_position = 0;
                    }
                    else {
                        voice_data->sample_position += 1.0;
                    }

                    if (voice_data->sample_position >= voice_data->pulse_duty_samples) {
                        voice_samples[i][j] = voice_data->wave_amplitude;
                    }
                    else {
                        voice_samples[i][j] = -voice_data->wave_amplitude;
                    }
                }
                break;

            case WAVE_SINE:
                voice_data->rads = 2 * M_PI / voice_data->wave_period;
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->sample_position >= voice_data->wave_period){
                        voice_data->sample_position -= voice_data->wave_period;
                    }
                    else{
                        voice_data->sample_position += 1.0;
                    }
                    voice_samples[i][j] = voice_data->wave_amplitude * sinf(voice_data->rads * voice_data->sample_position );
                }
                break;

            case WAVE_TRIANGLE:
                voice_data->slope = 4 * voice_data->wave_amplitude / voice_data->wave_period;
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->wave_state == 0) {

                        if (voice_data->sample_position >= voice_data->wave_period / 4){
                            voice_data->wave_state = 1;
                            voice_samples[i][j] = 2 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        else {
                            voice_samples[i][j] = voice_data->sample_position * voice_data->slope;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else if (voice_data->wave_state == 1) {

                        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
                            voice_data->wave_state = 2;
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 4;
                        }
                        else {
                            voice_samples[i][j] = 2 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else {
                        voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 4;

                        if (voice_data->sample_position >= voice_data->wave_period) {
                            voice_data->sample_position -= voice_data->wave_period;
                            voice_data->wave_state = 0;
                        }
                        else {
                            voice_data->sample_position += 1.0;
                        }
                    }
                }
                break;

            case WAVE_INVERTED_TRIANGLE:
                voice_data->slope = 4 * voice_data->wave_amplitude / voice_data->wave_period;
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->wave_state == 0) {

                        if (voice_data->sample_position >= voice_data->wave_period / 4){
                            voice_data->wave_state = 1;
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude;
                        }
                        else {
                            voice_samples[i][j] = voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else if (voice_data->wave_state == 1) {

                        if (voice_data->sample_position >= (voice_data->wave_period / 2)) {
                            voice_data->wave_state = 2;
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 3;
                        }
                        else {
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else if (voice_data->wave_state == 2) {

                        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
                            voice_data->wave_state = 3;
                            voice_samples[i][j] = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        else {
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 3;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else {
                        voice_samples[i][j] = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;

                        if (voice_data->sample_position >= voice_data->wave_period) {
                            voice_data->sample_position -= voice_data->wave_period;
                            voice_data->wave_state = 0;
                        }
                        else {
                            voice_data->sample_position += 1.0;
                        }
                    }
                }
                break;
            case WAVE_DOUBLE_SAW:
                voice_data->slope = 4 * voice_data->wave_amplitude / voice_data->wave_period;
                for(int j = 0; j < n_sample; j++) {
                    if (voice_data->wave_state == 0) {

                        if (voice_data->sample_position >= voice_data->wave_period / 4){
                            voice_data->wave_state = 1;
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude;
                        }
                        else {
                            voice_samples[i][j] = voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else if (voice_data->wave_state == 1) {

                        if (voice_data->sample_position >= (voice_data->wave_period * 2 / 4)) {
                            voice_data->wave_state = 2;
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
                        }
                        else {
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else if (voice_data->wave_state == 2) {

                        if (voice_data->sample_position >= (voice_data->wave_period * 3 / 4)) {
                            voice_data->wave_state = 3;
                            voice_samples[i][j] = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;
                        }
                        else {
                            voice_samples[i][j] = voice_data->slope * voice_data->sample_position - voice_data->wave_amplitude * 2;
                        }
                        voice_data->sample_position += 1.0;
                    }
                    else {
                        voice_samples[i][j] = 3 * voice_data->wave_amplitude - voice_data->slope * voice_data->sample_position;

                        if (voice_data->sample_position >= voice_data->wave_period) {
                            voice_data->sample_position -= voice_data->wave_period;
                            voice_data->wave_state = 0;
                        }
                        else {
                            voice_data->sample_position += 1.0;
                        }
                    }
                }
                break;
            default:
                for(int j = 0; j < n_sample; j++) {
                    voice_samples[i][j] = 0;
                }
                voice_data->sample_position = 0;
                break;
            }
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
            audio_samples[j] = audio_samples[j] * 0.125;
        }
    }

    return;
}
