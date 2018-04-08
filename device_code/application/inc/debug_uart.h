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
