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
    WAVE_INVERTED_TRIANGLE,
    WAVE_DOUBLE_SAW,
    WAVE_SINE,
    WAVE_LAST
} audio_wave_type_t;

#define N_WAVES WAVE_LAST

void audio_gen_wave_start(float freq, float level);
void audio_gen_wave_form(audio_wave_type_t wave_type);
void audio_gen_wave_stop(void);
void fetch_next_audio_buffer(float *audio_samples, uint16_t n_sample);

#endif /* INC_AUDIO_GEN_H_ */
