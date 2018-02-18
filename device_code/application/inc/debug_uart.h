/*
 * debug_uart.h
 *
 *  Created on: Oct 25, 2017
 *      Author: jaxc
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

typedef enum{
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} log_level_t;

void debug_log_add(uint8_t *message, uint8_t size, log_level_t log_level);
uint8_t inc_position(uint8_t head);
void init_debug(uint8_t message_size, uint8_t buffer_size, log_level_t log_level);
void send_debug_from_buffer();
uint8_t debug_buffer_empty(void);


#endif /* INC_DEBUG_UART_H_ */
