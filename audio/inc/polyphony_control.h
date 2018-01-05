/*
 * polyphony_control.h
 *
 *  Created on: Jan 5, 2018
 *      Author: jaxc
 */

#ifndef AUDIO_INC_POLYPHONY_CONTROL_H_
#define AUDIO_INC_POLYPHONY_CONTROL_H_

#include "error_codes.h"
#include "midi_constants.h"

struct wave_information{
    midi_note_number_t active_note;
    float note_freq;
    float wave_period;
    float sample_position;
    float wave_amplitude;
    uint8_t mute_output;
    float slope;
    float pulse_duty_cycle;
    float rads;
    uint8_t wave_state;
} wave_information_t;

void init_polyphony_control(void);
hjalmar_error_code_t request_voice(midi_note_number_t requested_note, uint8_t velocity);
hjalmar_error_code_t start_release_voice(midi_note_number_t release_note);
void complete_release_voice(uint8_t voice);
uint8_t get_n_active_voices(void);
void get_voice_information(struct wave_information **voice_data, uint8_t voice);

#endif /* AUDIO_INC_POLYPHONY_CONTROL_H_ */
