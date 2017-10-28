/*
 * debug_uart.c
 *
 *  Created on: Oct 25, 2017
 *      Author: jaxc
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "usart.h"
#include "debug_uart.h"

//uint8_t *log_line = {0};
uint8_t line_length;
uint8_t **log_buffer = {0};
uint8_t buffer_head;
uint8_t buffer_tail;
uint8_t buffer_size;

uint8_t tx_rdy;

uint8_t led_stat = 0;

void debug_log_add(uint8_t *message, uint8_t size)
{
    if (inc_position(buffer_head) == buffer_tail)
    {
        /* Buffer full, drop message */
    }
    else
    {
        memcpy(log_buffer[buffer_head], message, line_length);
        buffer_head = inc_position(buffer_head);
    }
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



void init_debug(uint8_t message_size, uint8_t n_buff_elem)
{
    log_buffer = malloc(sizeof(*log_buffer) * n_buff_elem);
    line_length = message_size;
    for(int8_t i = 0; i < n_buff_elem; i++){
        log_buffer[i] = malloc(sizeof(uint8_t) * line_length);
    }
    buffer_size = n_buff_elem;
    buffer_head = 0;
    buffer_tail = 0;
    tx_rdy = 1;

    led_stat = !led_stat;
}

void send_debug_from_buffer()
{
    HAL_GPIO_WritePin(GPIOD, LD3_Pin, led_stat);
    led_stat = !led_stat;
    if(buffer_tail != buffer_head)
    {
        uint8_t prev_tail = buffer_tail;
        buffer_tail = inc_position(buffer_tail);
        HAL_UART_Transmit_DMA(&huart3, log_buffer[prev_tail], line_length);
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
