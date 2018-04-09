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

#ifndef AUDIO_INC_AUDIO_FILTER_H_
#define AUDIO_INC_AUDIO_FILTER_H_

void init_filter (void);

void filter_calculate_samples (float *sample_level, uint16_t n_samples);
void change_cutoff(float new_cutoff);
void change_q(float new_q);

#endif /* AUDIO_INC_AUDIO_FILTER_H_ */
