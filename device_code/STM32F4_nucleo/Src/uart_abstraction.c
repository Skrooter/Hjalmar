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

#include "uart_abstraction.h"
#include "usart.h"

hjalmar_error_code_t uart_transmit(uart_ref_t sel_uart, uint8_t *message,
        uint16_t size)
{
    hjalmar_error_code_t ret = HJALMAR_OK;
    switch (sel_uart){
    case HJALMAR_UART_MIDI:
        if (HAL_UART_Transmit_DMA(get_midi_handle(), message, size) != HAL_OK) {
            ret = HJALMAR_HAL_ERROR;
        } else {
            ret = HJALMAR_OK;
        }
        break;
    case HJALMAR_UART_DEBUG:
        if (HAL_UART_Transmit_DMA(get_debug_handle(), message, size) != HAL_OK) {
            ret = HJALMAR_HAL_ERROR;
        } else {
            ret = HJALMAR_OK;
        }
        break;
    default:
        ret = HJALMAR_INVALID_ARGUMENT;
        break;
    }

    return ret;
}

hjalmar_error_code_t uart_receive(uart_ref_t sel_uart, uint8_t *message,
        uint16_t size)
{
    hjalmar_error_code_t ret = HJALMAR_OK;
    switch (sel_uart){
    case HJALMAR_UART_MIDI:
        if (HAL_UART_Receive_DMA(get_midi_handle(), message, size) != HAL_OK) {
            ret = HJALMAR_HAL_ERROR;
        } else {
            ret = HJALMAR_OK;
        }
        break;
    case HJALMAR_UART_DEBUG:
        if (HAL_UART_Receive_DMA(get_debug_handle(), message, size) != HAL_OK) {
            ret = HJALMAR_HAL_ERROR;
        } else {
            ret = HJALMAR_OK;
        }
        break;
    default:
        ret = HJALMAR_INVALID_ARGUMENT;
        break;
    }

    return ret;
}

hjalmar_error_code_t uart_ready(uart_ref_t sel_uart)
{
    hjalmar_error_code_t ret = HJALMAR_OK;
    switch(sel_uart) {
    case HJALMAR_UART_MIDI:
        ret = HJALMAR_NOT_IMPLEMENTED;
        break;
    case HJALMAR_UART_DEBUG:
        switch (get_debug_handle()->gState) {
        case HAL_UART_STATE_READY:
            ret = HJALMAR_OK;
            break;
        case HAL_UART_STATE_BUSY:
        case HAL_UART_STATE_BUSY_TX:
        case HAL_UART_STATE_BUSY_RX:
        case HAL_UART_STATE_BUSY_TX_RX:
            ret = HJALMAR_BUSY;
            break;
        default:
            ret = HJALMAR_HAL_ERROR;
            break;
        }
    default:
        ret = HJALMAR_INVALID_ARGUMENT;
        break;
    }
    return ret;
}



