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

uint8_t *log_line = {0};
uint8_t line_length;
uint8_t **log_buffer = {0};
uint8_t buffer_head;
uint8_t buffer_tail;
uint8_t buffer_size;

uint8_t tx_rdy;

uint8_t led_stat = 0;

void debug_log_add(uint8_t *message, uint8_t size)
{
    if (inc_head(buffer_head) == buffer_tail)
    {
        /* Buffer full, drop message */
    }

    else
    {

        memcpy(log_buffer[buffer_head], message, line_length);
        HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
        buffer_head = inc_head(buffer_head);
    }
    HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
    if (tx_rdy == 1)
    {
        send_debug_from_buffer();
    }
    HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
}

uint8_t inc_head(uint8_t head)
{
    if (head + 1 == buffer_size){
        head = 0;
    }
    else
    {
        head++;
    }
    return head;
}



void init_debug(uint8_t message_size, uint8_t buffer_size)
{
    line_length = message_size;
    log_line = malloc(sizeof(uint8_t) * line_length);
    log_buffer = malloc(sizeof(log_line) * buffer_size);
    buffer_head = 0;
    buffer_tail = 0;
    tx_rdy = 1;

    led_stat = !led_stat;
}

void send_debug_from_buffer()
{
    //HAL_GPIO_WritePin(GPIOD, LD3_Pin, led_stat);
    led_stat = !led_stat;
    if(buffer_tail != buffer_head)
    {
        HAL_UART_Transmit_DMA(&huart3, log_buffer[buffer_tail], line_length);
        buffer_tail++;
    }
}

