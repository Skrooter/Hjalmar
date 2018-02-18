/*
 * envelope.c
 *
 *  Created on: Jan 4, 2018
 *      Author: jaxc
 */

#include <stdio.h>

#include "audio_constants.h"
#include "envelope.h"
#include "polyphony_control.h"

uint32_t attack_length;
float attack_slope;
uint32_t decay_length;
float decay_slope;
float sustain_level;
uint32_t release_length;
float release_length_f;
float release_slope;

void init_envelope(void)
{
    attack_length = 11025;
    attack_slope = 1/(float)attack_length;
    sustain_level = 0.5;
    decay_length = 10000;
    decay_slope = ((1-sustain_level)/(float)decay_length);
    release_length = 15000;
    release_length_f = 1 / (float) release_length;
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
    release_length_f = 1 / (float) release_length;

    return HJALMAR_OK;
}

void start_envelope(struct envelope_variables *envelope_parameters)
{
    envelope_parameters->envelope_sample_counter = 0;
    envelope_parameters->release_done = 0;
    envelope_parameters->envelope_state = ENVELOPE_ATTACK_STATE;
    return;
}

void start_release(struct envelope_variables *envelope_parameters)
{
    release_slope = (envelope_parameters->release_start_level * release_length_f);
    envelope_parameters->envelope_state = ENVELOPE_RELEASE_STATE;
    envelope_parameters->envelope_sample_counter = 0;
}

void get_sample_envelope(struct envelope_variables *envelope_parameters, float *sample_level, uint16_t n_samples)
{
    for(uint16_t i = 0; i < n_samples; i++) {
        switch (envelope_parameters->envelope_state) {
        case ENVELOPE_IDLE_STATE:
            sample_level[i] = 0;
            envelope_parameters->release_done = 1;
            break;

        case ENVELOPE_ATTACK_STATE:
            envelope_parameters->release_start_level = attack_slope * envelope_parameters->envelope_sample_counter;
            sample_level[i] = sample_level[i] * envelope_parameters->release_start_level;
            if(envelope_parameters->envelope_sample_counter >= attack_length) {
                envelope_parameters->envelope_sample_counter = 0;
                envelope_parameters->envelope_state = ENVELOPE_DECAY_STATE;
            }
            else {
                envelope_parameters->envelope_sample_counter++;
            }

            break;

        case ENVELOPE_DECAY_STATE:
            envelope_parameters->release_start_level = (1 - (decay_slope * envelope_parameters->envelope_sample_counter));
            sample_level[i] =  sample_level[i] * envelope_parameters->release_start_level;
            if(envelope_parameters->envelope_sample_counter >= decay_length) {
                envelope_parameters->envelope_sample_counter = 0;
                envelope_parameters->envelope_state = ENVELOPE_SUSTAIN_STATE;
            }
            else {
                envelope_parameters->envelope_sample_counter++;
            }
            break;

        case ENVELOPE_SUSTAIN_STATE:
            envelope_parameters->release_start_level = sustain_level;
            sample_level[i] = sample_level[i] * sustain_level;
            break;

        case ENVELOPE_RELEASE_STATE:
          sample_level[i] = sample_level[i] * (envelope_parameters->release_start_level - (release_slope * envelope_parameters->envelope_sample_counter));
            if(envelope_parameters->envelope_sample_counter >= release_length) {
                envelope_parameters->envelope_sample_counter = 0;
                envelope_parameters->envelope_state = ENVELOPE_IDLE_STATE;
            }
            else {
                envelope_parameters->envelope_sample_counter++;
            }
            break;

        default:
            break;
        }
    }
}

uint8_t get_release_done(struct envelope_variables *envelope_parameters)
{
    return envelope_parameters->release_done;
}
