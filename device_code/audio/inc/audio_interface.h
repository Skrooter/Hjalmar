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

#ifndef INC_AUDIO_INTERFACE_H_
#define INC_AUDIO_INTERFACE_H_

#define DAC_MAX_VALUE 0x7FFF
#define DAC_MIN_VALUE 0x8000

void init_audio_output(void);
//void add_to_audio_buffer(int16_t *audio_buffer, uint32_t buf_len);
void mono_to_stereo (uint16_t *stereo_samples, float *mono_samples, uint16_t n_sample_mono);
void send_audio(void);


#endif /* INC_AUDIO_INTERFACE_H_ */
