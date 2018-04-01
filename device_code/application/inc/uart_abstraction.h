/*
 * uart_abstraction.h
 *
 *  Created on: Apr 5, 2018
 *      Author: jaxc
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
