/*
 * polyphony_control.c
 *
 *  Created on: Jan 5, 2018
 *      Author: jaxc
 */

#include <math.h>
#include <stdlib.h>

#include "audio_constants.h"
#include "audio_functions.h"
#include "audio_gen.h"
#include "polyphony_control.h"

uint8_t n_active_voices;
uint32_t sample_per_sec;
struct wave_information *voice_info;


void init_polyphony_control(void)
{
    sample_per_sec = HJALMAR_AUDIOFREQ;
    voice_info = malloc(sizeof(struct wave_information) * POLYPHONY_VOICES);
    init_envelope();
    reset_polyphony_voices();
}

void get_voice_parameters(midi_note_number_t requested_note, float level, uint8_t voice)
{
    voice_info[voice].active_note = requested_note;
    voice_info[voice].note_freq = midi_to_frequency (requested_note);
    voice_info[voice].wave_period = sample_per_sec / voice_info[voice].note_freq;
    voice_info[voice].wave_amplitude = level;
    voice_info[voice].wave_state = 0;
    voice_info[voice].pulse_duty_cycle = 0.05;
    voice_info[voice].pulse_duty_samples = voice_info[voice].wave_period * voice_info[voice].pulse_duty_cycle;
    start_envelope(&voice_info[voice].env_var);

    voice_info[voice].mute_output = 0;
    return;
}

void reset_polyphony_voices(void)
{
    n_active_voices = 0;
    for(uint8_t i = 0; i < POLYPHONY_VOICES; i++) {
        voice_info[i].active_note = 0xFF;
        voice_info[i].mute_output = 1;
        voice_info[i].note_freq = 0;
        voice_info[i].pulse_duty_cycle = 0.05;
        voice_info[i].pulse_duty_samples = 0;
        voice_info[i].rads = 0;
        voice_info[i].sample_position = 0;
        voice_info[i].slope = 0;
        voice_info[i].wave_amplitude = 0;
        voice_info[i].wave_period = 0;
        voice_info[i].wave_state = 0;
        voice_info[i].env_var.envelope_state = ENVELOPE_IDLE_STATE;
        voice_info[i].env_var.envelope_sample_counter = 0;
        voice_info[i].env_var.release_done = 1;
        voice_info[i].env_var.release_start_level = 0.;
    }
}

hjalmar_error_code_t request_voice(midi_note_number_t requested_note, uint8_t velocity)
{
    if ((requested_note > 127) || (velocity > 127)) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    for(uint8_t i = 0; i < n_active_voices; i++) {
        if (voice_info[i].active_note == requested_note) {
            get_voice_parameters(requested_note, (float)velocity * 0.007874016, i); // 1 / 127
            return HJALMAR_OK;
        }
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
            start_release(&voice_info[i].env_var);
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

void get_envelope_information(struct envelope_variables **envelope_data, uint8_t voice)
{
    *envelope_data = &voice_info[voice].env_var;
    return;
}
