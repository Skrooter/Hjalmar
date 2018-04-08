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

#ifndef APPLICATION_INC_UART_ABSTRACTION_H_
#define APPLICATION_INC_UART_ABSTRACTION_H_

#include "error_codes.h"

typedef enum uart_ref {
    HJALMAR_UART_MIDI,
    HJALMAR_UART_DEBUG
} uart_ref_t;

hjalmar_error_code_t uart_transmit(uart_ref_t sel_uart, uint8_t *message,
        uint16_t size);

hjalmar_error_code_t uart_receive(uart_ref_t sel_uart, uint8_t *message,
        uint16_t size);

hjalmar_error_code_t uart_ready(uart_ref_t sel_uart);

#endif /* APPLICATION_INC_UART_ABSTRACTION_H_ */
