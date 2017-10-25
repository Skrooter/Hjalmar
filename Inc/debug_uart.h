/*
 * debug_uart.h
 *
 *  Created on: Oct 25, 2017
 *      Author: jaxc
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

void debug_log_add(uint8_t *message, uint8_t size);
uint8_t inc_head(uint8_t head);
void init_debug(uint8_t message_size, uint8_t buffer_size);
void send_debug_from_buffer();




#endif /* INC_DEBUG_UART_H_ */
