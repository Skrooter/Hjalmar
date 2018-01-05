/*
 * polyphony_control.c
 *
 *  Created on: Jan 5, 2018
 *      Author: jaxc
 */

#include <math.h>

#include "i2s.h"

#include "audio_constants.h"
#include "audio_functions.h"
#include "audio_gen.h"
#include "envelope.h"
#include "polyphony_control.h"

uint8_t n_active_voices;
uint32_t sample_per_sec;
struct wave_information voice_info[POLYPHONY_VOICES];

void init_polyphony_control(void)
{
    n_active_voices = 0;
    I2S_HandleTypeDef *i2s_handle = get_i2s_handle();
    sample_per_sec = i2s_handle->Init.AudioFreq;
    for(uint8_t i = 0; i < POLYPHONY_VOICES; i++) {
        voice_info[i].active_note = 0xFF;
        voice_info[i].mute_output = 1;
        voice_info[i].note_freq = 0;
        voice_info[i].pulse_duty_cycle = 0.05;
        voice_info[i].rads = 0;
        voice_info[i].sample_position = 0;
        voice_info[i].slope = 0;
        voice_info[i].wave_amplitude = 0;
        voice_info[i].wave_period = 0;
        voice_info[i].wave_state = 0;
    }
}

void get_voice_parameters(midi_note_number_t requested_note, float level, uint8_t voice)
{
    voice_info[voice].active_note = requested_note;
    voice_info[voice].note_freq = midi_to_frequency (requested_note);
    voice_info[voice].wave_period = sample_per_sec / voice_info[voice].note_freq;
    voice_info[voice].wave_amplitude = level;
    voice_info[voice].wave_state = 0;
    voice_info[voice].pulse_duty_cycle = 0.05;
    start_envelope(voice);

    voice_info[voice].mute_output = 0;
    return;
}

hjalmar_error_code_t request_voice(midi_note_number_t requested_note, uint8_t velocity)
{
    if ((requested_note > 127) || (velocity > 127)) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    if(n_active_voices < POLYPHONY_VOICES) {

        get_voice_parameters(requested_note, (float)velocity * 0.007874016, n_active_voices); // 1 / 127


        n_active_voices++;
    }
    return HJALMAR_OK;
}

hjalmar_error_code_t start_release_voice(midi_note_number_t release_note)
{
    if (release_note > 127) {
        return HJALMAR_INVALID_ARGUMENT;
    }

    for(uint8_t i = 0; i < n_active_voices; i++) {
        if (voice_info[i].active_note == release_note) {
            start_release(i);
            break;
        }
    }
    return HJALMAR_OK;
}

void complete_release_voice(uint8_t voice)
{
    n_active_voices--;
    for(uint8_t j = voice; j < n_active_voices; j++) {
        voice_info[j] = voice_info[j + 1];
    }
}

uint8_t get_n_active_voices(void)
{
    return n_active_voices;
}

void get_voice_information(struct wave_information **voice_data, uint8_t voice)
{
    *voice_data = &voice_info[voice];
    return;
}
