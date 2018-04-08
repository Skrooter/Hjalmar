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

#ifndef INC_ENVELOPE_H_
#define INC_ENVELOPE_H_

#include "error_codes.h"
#include <stdint.h>

typedef enum {
    ENVELOPE_IDLE_STATE,
    ENVELOPE_ATTACK_STATE,
    ENVELOPE_DECAY_STATE,
    ENVELOPE_SUSTAIN_STATE,
    ENVELOPE_RELEASE_STATE,
    ENVELOPE_RELEASE_NEW_ATTACK_STATE
} envelope_state_t;

struct envelope_variables {
    uint32_t envelope_sample_counter;
    uint8_t release_done;
    float release_start_level;
    envelope_state_t envelope_state;
} ;

void init_envelope(void);
hjalmar_error_code_t set_attack(uint8_t midi_attack);
hjalmar_error_code_t set_decay(uint8_t midi_decay);
hjalmar_error_code_t set_sustain(uint8_t midi_sustain);
hjalmar_error_code_t set_release(uint8_t midi_release);
void start_envelope(struct envelope_variables *envelope_parameters);
void start_release(struct envelope_variables *envelope_parameters);
void get_sample_envelope(struct envelope_variables *envelope_parameters, float *sample_level, uint16_t n_samples);
uint8_t get_release_done(struct envelope_variables *envelope_parameters);

#endif /* INC_ENVELOPE_H_ */
