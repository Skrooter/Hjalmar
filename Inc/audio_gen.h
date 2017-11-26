/*
 * audio_gen.h
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#ifndef INC_AUDIO_GEN_H_
#define INC_AUDIO_GEN_H_

typedef enum {
    WAVE_NONE,
    WAVE_SQUARE,
    WAVE_SAW,
    WAVE_TRIANGLE,
    WAVE_SINE
} audio_wave_type_t;

void audio_gen_sqr_start(float freq, uint8_t level);
uint16_t *fetch_next_audio_buffer(void);

#endif /* INC_AUDIO_GEN_H_ */
