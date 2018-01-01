/*
 * midi_cmd.c
 *
 *  Created on: Oct 8, 2017
 *      Author: jaxc
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "audio_gen.h"
#include "midi_constants.h"
#include "debug_uart.h"
#include "usart.h"
#include "midi_cmd.h"


uint8_t midi_rx_msg[3]; // Standard midi messenges are not larger than 3 bytes.
uint16_t *midi_rx_size;
uint8_t midi_rx_state;
uint8_t midi_tx_rdy;
midi_message_type_t current_midi_message;
uint8_t midi_rx_rdy;

enum{
    MIDI_RX_IDLE    = 0,
    MIDI_RX_BYTE_1  = 1,
    MIDI_RX_PAYLOAD = 2
};

float midi_to_frequency(midi_note_number_t midi_note)
{
    /* Frequency is calculated from 440 Hz (A6)*/
    float freq = (440.0 / 32) * powf(2, (((float)midi_note - 9) / 12));
    return freq;
}

HAL_StatusTypeDef transmit_midi_message(uint8_t *message, uint16_t size) {
    midi_tx_rdy = 0;
    return HAL_UART_Transmit_DMA(&huart2, message, size);
}

HAL_StatusTypeDef start_midi_receive(void) {
    return receive_midi_byte_1();
}

HAL_StatusTypeDef receive_midi_byte_1(void) {
    midi_rx_rdy = 0;
    midi_rx_state = MIDI_RX_BYTE_1;
    return HAL_UART_Receive_DMA(&huart2, midi_rx_msg, 1);
}

void handle_midi(void) {
    uint8_t debug_msg[64] = {0};

    switch (midi_rx_state){
    case MIDI_RX_BYTE_1:
        switch ((midi_rx_msg[0] & 0xF0) >> 4) {
        case MIDI_NOTE_ON:
            current_midi_message = MIDI_NOTE_ON;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Note on received on channel %d. "
                    "(0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 2);
            break;

        case MIDI_NOTE_OFF:
            current_midi_message = MIDI_NOTE_OFF;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Note off received on channel %d. "
                    "(0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 2);
            break;

        case MIDI_POLYPHONIC_PRESSURE:
            current_midi_message = MIDI_POLYPHONIC_PRESSURE;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Polyphonic pressure received on channel "
                    "%d. (0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 2);
            break;

        case MIDI_CONTROL_CHANGE:
            current_midi_message = MIDI_CONTROL_CHANGE;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Control change received on channel %d. "
                    "(0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 2);
            break;

        case MIDI_PROGRAM_CHANGE:
            current_midi_message = MIDI_PROGRAM_CHANGE;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Program change received on channel %d. "
                    "(0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 1);
            break;

        case MIDI_CHANNEL_PRESSURE:
            current_midi_message = MIDI_CHANNEL_PRESSURE;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Channel pressure received on channel %d."
                    " (0x%02x).", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 1);
            break;

        case MIDI_PITCH_BEND:
            current_midi_message = MIDI_PITCH_BEND;
            midi_rx_state = MIDI_RX_PAYLOAD;
            sprintf((char *)debug_msg,"Pitch bend received on channel %d. "
                    "(0x%02x)", (midi_rx_msg[0] & 0xF) + 1, midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            HAL_UART_Receive_DMA(&huart2, midi_rx_msg + 1, 2);
            break;

        default:
            sprintf((char *)debug_msg,"Unknown command received (0x%02x)",
                    midi_rx_msg[0]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);

            if (receive_midi_byte_1() != HAL_OK)
            {
                _Error_Handler(__FILE__, __LINE__);
            }
        }
        break;
    case MIDI_RX_PAYLOAD:
        midi_rx_rdy = 1;
        midi_rx_state = MIDI_RX_IDLE;
        float note_freq;
        switch (current_midi_message) {
        case MIDI_NOTE_ON:
            note_freq = midi_to_frequency(midi_rx_msg[1]);
            audio_gen_wave_start(note_freq, midi_rx_msg[2]);
            sprintf((char *)debug_msg,"Note: 0x%02x, velocity 0x%02x", midi_rx_msg[1], midi_rx_msg[2]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;

        case MIDI_NOTE_OFF:
            note_freq = midi_to_frequency(midi_rx_msg[1]);
            audio_gen_wave_stop();
            sprintf((char *)debug_msg,"Note: 0x%02x, velocity 0x%02x", midi_rx_msg[1], midi_rx_msg[2]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        case MIDI_POLYPHONIC_PRESSURE:
            sprintf((char *)debug_msg,"Note: 0x%02x, pressure 0x%02x", midi_rx_msg[1], midi_rx_msg[2]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        case MIDI_CONTROL_CHANGE:
            sprintf((char *)debug_msg,"Control number: 0x%02x, Value 0x%02x", midi_rx_msg[1], midi_rx_msg[2]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        case MIDI_PROGRAM_CHANGE:
            sprintf((char *)debug_msg,"Program number: 0x%02x", midi_rx_msg[1]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        case MIDI_CHANNEL_PRESSURE:
            sprintf((char *)debug_msg,"Pressure: 0x%02x", midi_rx_msg[1]);
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        case MIDI_PITCH_BEND:
            sprintf((char *)debug_msg,"Pitch: 0x%04x", midi_rx_msg[1] + (midi_rx_msg[2] << 8));
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        default:
            sprintf((char *)debug_msg,"error incorrect payload");
            debug_log_add(debug_msg, strlen((char *)debug_msg), LOG_LEVEL_INFO);
            break;
        }

        if (receive_midi_byte_1() != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

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
