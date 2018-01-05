/*
 * midi_cmd.h
 *
 *  Created on: Nov 5, 2017
 *      Author: jaxc
 */

#ifndef INC_MIDI_CMD_H_
#define INC_MIDI_CMD_H_

float midi_to_frequency (midi_note_number_t midi_note);
HAL_StatusTypeDef transmit_midi_message(uint8_t *message, uint16_t size);
HAL_StatusTypeDef start_midi_receive(void);
HAL_StatusTypeDef receive_midi_byte_1(void);
uint8_t get_midi_tx_state (void);
void set_midi_tx_state (uint8_t state);
uint8_t get_midi_rx_state (void);
void set_midi_rx_state (uint8_t state);
void handle_midi(void);

#endif /* INC_MIDI_CMD_H_ */
