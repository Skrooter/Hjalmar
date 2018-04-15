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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "audio_filter.h"
#include "audio_gen.h"
#include "debug_uart.h"
#include "envelope.h"
#include "main.h"
#include "midi_cmd.h"
#include "midi_constants.h"
#include "midi_conv_tables.h"
#include "polyphony_control.h"
#include "uart_abstraction.h"
#include "work_queue.h"



uint8_t midi_rx_msg[3]; // Standard midi messenges are not larger than 3 bytes.
uint8_t midi_rx_dma_byte; // Incomming byte.
uint16_t *midi_rx_size;
uint8_t midi_tx_rdy;
midi_message_type_t current_midi_message;
uint8_t midi_rx_rdy;

typedef enum MIDI_state{
    MIDI_RX_IDLE,
    MIDI_RX_CMD_BYTE,
    MIDI_RX_DATA_BYTE_1,
    MIDI_RX_DATA_BYTE_2,
    MIDI_RX_SINGLE_DATA,
    MIDI_RX_SYSTEM
} midi_state_t;

midi_state_t midi_rx_state;

hjalmar_error_code_t transmit_midi_message(uint8_t *message, uint16_t size) {
    midi_tx_rdy = 0;
    return uart_transmit(HJALMAR_UART_MIDI, message, size);
}

hjalmar_error_code_t start_midi_receive(void) {
    return receive_first_midi_byte();
}

hjalmar_error_code_t receive_first_midi_byte(void) {
    midi_rx_rdy = 0;
    midi_rx_state = MIDI_RX_CMD_BYTE;
    return uart_receive(HJALMAR_UART_MIDI, &midi_rx_dma_byte, 1);
}

void handle_midi_int(void)
{
    uint8_t *midi_rx_byte = malloc(sizeof(uint8_t));
    *midi_rx_byte = midi_rx_dma_byte;
    if(uart_receive(HJALMAR_UART_MIDI, &midi_rx_dma_byte, 1) != HJALMAR_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    work_queue_add(handle_midi, (void *)midi_rx_byte);

}

void handle_midi(void *midi_rx_in) {

    uint8_t midi_rx_byte = *(uint8_t *)midi_rx_in;

    if((midi_rx_byte & 0x80)) {
        midi_rx_state = MIDI_RX_CMD_BYTE;
    }
    switch(midi_rx_state) {
    case MIDI_RX_IDLE:
        break;

    case MIDI_RX_CMD_BYTE:
        midi_rx_msg[0] = midi_rx_byte;
        current_midi_message = (midi_rx_msg[0] & 0xF0) >> 4;
        switch (current_midi_message) {
        case MIDI_NOTE_OFF:
        case MIDI_NOTE_ON:
        case MIDI_POLYPHONIC_PRESSURE:
        case MIDI_CONTROL_CHANGE:
        case MIDI_PITCH_BEND:
            midi_rx_state = MIDI_RX_DATA_BYTE_1;
            break;

        case MIDI_PROGRAM_CHANGE:
        case MIDI_CHANNEL_PRESSURE:
            midi_rx_state = MIDI_RX_SINGLE_DATA;
            break;

        case MIDI_SYSTEM:
            switch (midi_rx_msg[0]) {
            case MIDI_SYSTEM_PANIC:
                reset_polyphony_voices();
                break;

            default:
                break;
            }
            if (midi_rx_msg[0] == 0xF0) {
                midi_rx_state = MIDI_RX_SYSTEM;
            }
            else {
                midi_rx_state = MIDI_RX_CMD_BYTE;
            }
            break;

        default:
            midi_rx_state = MIDI_RX_CMD_BYTE;
            return;
        }

        break;

    case MIDI_RX_DATA_BYTE_1:
        midi_rx_msg[1] = midi_rx_byte;
        midi_rx_state = MIDI_RX_DATA_BYTE_2;
        break;

    case MIDI_RX_DATA_BYTE_2:
        midi_rx_msg[2] = midi_rx_byte;
        midi_rx_state = MIDI_RX_DATA_BYTE_1;
        switch(current_midi_message) {
        case MIDI_NOTE_OFF:
            start_release_voice(midi_rx_msg[1]);
            break;

        case MIDI_NOTE_ON:
            request_voice(midi_rx_msg[1], midi_rx_msg[2]);
            break;
        case MIDI_POLYPHONIC_PRESSURE:
        case MIDI_CONTROL_CHANGE:
            switch (midi_rx_msg[1]) {
            case 0x15:
                audio_gen_wave_form(midi_rx_msg[2] / 8);
                break;

            case 0x16:
                change_cutoff(frequency_lut[midi_rx_msg[2]]);
                break;

            case 0x17:
                change_q(q_lut[midi_rx_msg[2]]);
                break;

            case 0x19:
                set_attack(adr_time[midi_rx_msg[2]]);
                break;

            case 0x1a:
                set_decay(adr_time[midi_rx_msg[2]]);
                break;

            case 0x1b:
                set_sustain(midi_percent_lin[midi_rx_msg[2]]);
                break;

            case 0x1c:
                set_release(adr_time[midi_rx_msg[2]]);
                break;

            case 0x68:

            case 0x69:
            case 0x6a:
            case 0x6b:
            case 0x6c:
                enable_slow_slew();
            case 0x6d:
                disable_slow_slew();
            default:
                break;
            }
            break;
        default:
            midi_rx_state = MIDI_RX_CMD_BYTE;
            break;


        }
        break;

    case MIDI_RX_SINGLE_DATA:
        break;

    case MIDI_RX_SYSTEM:
        break;

    default:
        midi_rx_state = MIDI_RX_CMD_BYTE;
        break;
    }

    if(midi_rx_byte) {
        free(midi_rx_in);
        midi_rx_in = NULL;
    }
}


uint8_t get_midi_tx_state (void) {
    return midi_rx_rdy;
}

void set_midi_tx_state (uint8_t state) {
    midi_rx_rdy = state;
}

uint8_t get_midi_rx_state (void) {
    return midi_rx_rdy;
}

void set_midi_rx_state (uint8_t state) {
    midi_rx_rdy = state;
}
