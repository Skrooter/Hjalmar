/*
 * envelope.h
 *
 *  Created on: Jan 4, 2018
 *      Author: jaxc
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
    ENVELOPE_RELEASE_STATE
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
