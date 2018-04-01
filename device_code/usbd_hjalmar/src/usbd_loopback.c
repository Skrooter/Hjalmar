#include <stdint.h>
#include <stdio.h>
#include "usb_hjalmar.h"
#include "work_queue.h"
//#include "stm32f4xx_hal.h"

static uint8_t msg[256];
static uint16_t rec_len = 0;

static void vcom_echo(void *data)
{
    uint16_t len = *((uint16_t *)data);
    int msg_len = snprintf((char *)msg, sizeof(msg), "Received %u characters\n\r", len);

    usb_cdc_transmit(msg, msg_len);
    //HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
}

void vcom_rx_complete(uint8_t *data, uint16_t len, void *usr)
{
    (void)usr;
    (void)data;
    rec_len = len;
    work_queue_add(vcom_echo, &rec_len);
    //HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
}
