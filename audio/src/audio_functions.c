/*
 * audio_functions.c
 *
 *  Created on: Jan 5, 2018
 *      Author: jaxc
 */

#include <math.h>

#include "audio_functions.h"


float midi_to_frequency(midi_note_number_t midi_note)
{
    /* Frequency is calculated from 440 Hz (A6)*/
    float freq = (440.0 / 32) * powf(2, (((float)midi_note - 9) / 12));
    return freq;
}

