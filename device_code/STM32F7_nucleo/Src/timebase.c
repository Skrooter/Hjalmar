/*
 * timebase.c
 *
 *  Created on: Apr 6, 2018
 *      Author: jaxc
 */

#include <stdint.h>

#include "stm32f7xx.h"

uint32_t get_mcu_tick(void)
{
    return (uint32_t) HAL_GetTick();
}

