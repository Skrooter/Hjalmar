/*
 * general_functions.c
 *
 *  Created on: Apr 1, 2018
 *      Author: jaxc
 */

#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "tim.h"
#include "usart.h"

void init_periphials(void){

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_I2S3_Init();
    MX_TIM6_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    return;
}
