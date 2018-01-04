/*
 * envelope.c
 *
 *  Created on: Jan 4, 2018
 *      Author: jaxc
 */

#include "envelope.h"

uint32_t attack_length;
float attack_slope;
uint32_t decay_length;
float decay_slope;
float sustain_level;
uint32_t release_length;
float release_slope;

float release_start_level;


uint32_t envelope_sample_counter;
uint8_t release_done;

typedef enum {
    ENVELOPE_IDLE_STATE,
    ENVELOPE_ATTACK_STATE,
    ENVELOPE_DECAY_STATE,
    ENVELOPE_SUSTAIN_STATE,
    ENVELOPE_RELEASE_STATE
} envelope_state_t;

envelope_state_t envelope_state;

void init_envelope(void)
{
    envelope_state = ENVELOPE_IDLE_STATE;
    envelope_sample_counter = 0;
    attack_length = 11025;
    attack_slope = 1/(float)attack_length;
    sustain_level = 0.5;
    decay_length = 10000;
    decay_slope = ((1-sustain_level)/(float)decay_length);
    release_length = 15000;
    return;
}

hjalmar_error_code_t set_attack(uint8_t midi_attack)
{
    if (midi_attack > 127) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    attack_length = 500 * (uint16_t)midi_attack;
    attack_slope = 1/(float)attack_length;
    return HJALMAR_OK;
}

hjalmar_error_code_t set_decay(uint8_t midi_decay)
{
    if (midi_decay > 127) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    decay_length = 500 * (uint16_t)midi_decay;
    decay_slope = ((1-sustain_level)/(float)decay_length);
    return HJALMAR_OK;
}

hjalmar_error_code_t set_sustain(uint8_t midi_sustain)
{
    if (midi_sustain > 127) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    decay_slope = ((1-sustain_level)/(float)decay_length);
    sustain_level = (float)midi_sustain / 127;
    return HJALMAR_OK;
}

hjalmar_error_code_t set_release(uint8_t midi_release)
{
    if (midi_release > 127) {
        return HJALMAR_INVALID_ARGUMENT;
    }
    release_length = 500 * (uint16_t)midi_release;
    return HJALMAR_OK;
}

void start_envelope(void)
{
    envelope_sample_counter = 0;
    release_done = 0;
    envelope_state = ENVELOPE_ATTACK_STATE;
    return;
}

void start_release(void)
{
    release_slope = (release_start_level/(float)release_length);
    envelope_state = ENVELOPE_RELEASE_STATE;
    envelope_sample_counter = 0;
}

void get_sample_envelope(float *sample_level, uint16_t n_samples)
{
    for(uint16_t i = 0; i < n_samples; i++) {
        switch (envelope_state) {
        case ENVELOPE_IDLE_STATE:
            sample_level[i] = 0;
            release_done = 1;
            break;

        case ENVELOPE_ATTACK_STATE:
            if(envelope_sample_counter >= attack_length) {
                envelope_sample_counter = 0;
                envelope_state = ENVELOPE_DECAY_STATE;
            }
            else {
                envelope_sample_counter++;
            }

            release_start_level = attack_slope * envelope_sample_counter;
            sample_level[i] = sample_level[i] * release_start_level;
            break;

        case ENVELOPE_DECAY_STATE:
            if(envelope_sample_counter >= decay_length) {
                envelope_sample_counter = 0;
                envelope_state = ENVELOPE_SUSTAIN_STATE;
            }
            else {
                envelope_sample_counter++;
            }
            release_start_level = (1 - (decay_slope * envelope_sample_counter));
            sample_level[i] =  sample_level[i] * release_start_level;
            break;

        case ENVELOPE_SUSTAIN_STATE:
            release_start_level = sustain_level;
            sample_level[i] = sample_level[i] * sustain_level;
            break;

        case ENVELOPE_RELEASE_STATE:
            if(envelope_sample_counter >= release_length) {
                envelope_sample_counter = 0;
                envelope_state = ENVELOPE_IDLE_STATE;
            }
            else {
                envelope_sample_counter++;
            }
            sample_level[i] = sample_level[i] * (release_start_level - (release_slope * envelope_sample_counter));
            break;

        default:
            break;
        }
    }
}

uint8_t get_release_done(void)
{
    return release_done;
}
