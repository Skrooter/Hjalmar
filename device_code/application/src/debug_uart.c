/*
 * debug_uart.c
 *
 *  Created on: Oct 25, 2017
 *      Author: jaxc
 */

#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug_uart.h"
#include "timebase.h"
#include "uart_abstraction.h"

uint8_t line_max_length;
uint8_t **log_buffer = {0};
uint8_t buffer_head;
uint8_t buffer_tail;
uint8_t buffer_size;
uint8_t buffer_header_size = 10;

log_level_t buffer_log_level;

uint8_t tx_rdy;

uint8_t led_stat = 0;

void debug_log_add(uint8_t *message, uint8_t size, log_level_t log_level)
{
    if (log_level < buffer_log_level) {
        return;
    }

    if (!message) {
        return;
    }

    if (inc_position(buffer_head) == buffer_tail) {
        /* Buffer full, drop message */
        return;
    }

    uint8_t copy_size;
    copy_size = (size > line_max_length - buffer_header_size) ?
            line_max_length - buffer_header_size : size;

    memset(log_buffer[buffer_head], 0, line_max_length);
    sprintf((char *) log_buffer[buffer_head],"%8"PRIu32": ", get_mcu_tick());
    memcpy(log_buffer[buffer_head] + buffer_header_size, message, copy_size);
    if (strlen((char *) log_buffer[buffer_head]) > (size_t)(line_max_length - 3)) {
        log_buffer[buffer_head][line_max_length-2] = '\r';
        log_buffer[buffer_head][line_max_length-1] = '\n';
        log_buffer[buffer_head][line_max_length] = '\0';
    }
    else {
        log_buffer[buffer_head][strlen((char *)log_buffer[buffer_head])] = '\r';
        log_buffer[buffer_head][strlen((char *)log_buffer[buffer_head])] = '\n';
        log_buffer[buffer_head][strlen((char *)log_buffer[buffer_head])] = '\0';
    }
    buffer_head = inc_position(buffer_head);

    if (tx_rdy == 1)
    {
        tx_rdy = 0;
        send_debug_from_buffer();
    }
}

uint8_t inc_position(uint8_t position)
{
    if (position + 1 == buffer_size){
        position = 0;
    }
    else
    {
        position++;
    }
    return position;
}



void init_debug(uint8_t message_size, uint8_t n_buff_elem, log_level_t log_level)
{
    if ((message_size <= 1) || (n_buff_elem <= 1)){
        return;
    }
    log_buffer = malloc(sizeof( *log_buffer) * n_buff_elem);
    line_max_length = message_size + buffer_header_size + 1;
    for(int8_t i = 0; i < n_buff_elem; i++){
        log_buffer[i] = malloc(sizeof(uint8_t) * line_max_length);
    }
    buffer_size = n_buff_elem;
    buffer_head = 0;
    buffer_tail = 0;
    tx_rdy = 1;

    buffer_log_level = log_level;
}

void send_debug_from_buffer()
{
    /*HAL_GPIO_WritePin(GPIOD, LD3_Pin, led_stat);*/
    led_stat = !led_stat;
    if(buffer_tail != buffer_head)
    {
        uint8_t prev_tail = buffer_tail;
        buffer_tail = inc_position(buffer_tail);
        uart_transmit(HJALMAR_UART_DEBUG, log_buffer[prev_tail],
                strlen((char *)log_buffer[prev_tail]));
    }
    else{
        tx_rdy = 1;
    }
}

uint8_t debug_buffer_empty(void){
    return buffer_tail == buffer_head;
}

void inc_tail(void){
    buffer_tail = inc_position(buffer_tail);
}
