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

#ifndef INC_MIDI_CMD_H_
#define INC_MIDI_CMD_H_

#include "error_codes.h"

hjalmar_error_code_t transmit_midi_message(uint8_t *message, uint16_t size);
hjalmar_error_code_t start_midi_receive(void);
hjalmar_error_code_t receive_first_midi_byte(void);
uint8_t get_midi_tx_state (void);
void set_midi_tx_state (uint8_t state);
uint8_t get_midi_rx_state (void);
void set_midi_rx_state (uint8_t state);
void handle_midi_int(void);
void handle_midi(void *midi_rx_byte);

#endif /* INC_MIDI_CMD_H_ */
