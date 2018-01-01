/*
 * audio_gen.h
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#ifndef INC_AUDIO_GEN_H_
#define INC_AUDIO_GEN_H_

#define GEN_LEVEL_VALUES 127
#define GEN_MAX_AMPLITUDE ((int16_t)DAC_MAX_VALUE / GEN_LEVEL_VALUES)
#define GEN_MIN_AMPLITUDE ((int16_t)DAC_MIN_VALUE / GEN_LEVEL_VALUES)

typedef enum {
    WAVE_NONE,
    WAVE_SQUARE,
    WAVE_SAW,
    WAVE_PULSE,
    WAVE_TRIANGLE,
    WAVE_SINE
} audio_wave_type_t;

void audio_gen_wave_start(float freq, uint8_t level);
void audio_gen_wave_form(audio_wave_type_t wave_type);
void audio_gen_wave_stop(void);
void fetch_next_audio_buffer(uint16_t *audio_samples, uint16_t n_sample);

#endif /* INC_AUDIO_GEN_H_ */
