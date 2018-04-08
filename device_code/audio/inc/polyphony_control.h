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

#ifndef AUDIO_INC_POLYPHONY_CONTROL_H_
#define AUDIO_INC_POLYPHONY_CONTROL_H_

#include "error_codes.h"
#include "midi_constants.h"
#include "envelope.h"

struct wave_information{
    midi_note_number_t active_note;
    float note_freq;
    float wave_period;
    float sample_position;
    float wave_amplitude;
    uint8_t mute_output;
    float slope;
    float pulse_duty_cycle;
    float pulse_duty_samples;
    float rads;
    uint8_t wave_state;
    struct envelope_variables env_var;
} wave_information_t;

void init_polyphony_control(void);
void get_voice_parameters(midi_note_number_t requested_note, float level, uint8_t voice);
void reset_polyphony_voices(void);
hjalmar_error_code_t request_voice(midi_note_number_t requested_note, uint8_t velocity);
hjalmar_error_code_t start_release_voice(midi_note_number_t release_note);
void complete_release_voice(uint8_t voice);
uint8_t get_n_active_voices(void);
void get_voice_information(struct wave_information **voice_data, uint8_t voice);
void get_envelope_information(struct envelope_variables **envelope_data, uint8_t voice);

#endif /* AUDIO_INC_POLYPHONY_CONTROL_H_ */
