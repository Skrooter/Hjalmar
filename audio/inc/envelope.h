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

void init_envelope(void);
hjalmar_error_code_t set_attack(uint8_t midi_attack);
hjalmar_error_code_t set_decay(uint8_t midi_decay);
hjalmar_error_code_t set_sustain(uint8_t midi_sustain);
hjalmar_error_code_t set_release(uint8_t midi_release);
void start_envelope(uint8_t voice);
void start_release(uint8_t voice);
void get_sample_envelope(float *sample_level, uint16_t n_samples, uint8_t voice);
uint8_t get_release_done(uint8_t voice);

#endif /* INC_ENVELOPE_H_ */
