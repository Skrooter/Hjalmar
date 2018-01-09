
#ifndef USB_HJALMAR_H_
#define USB_HJALMAR_H_

#include <stdint.h>

typedef struct {
    /* Up to USB_MAX_NUM_OF_BUFFERS rotating buffers per interface */
    uint8_t *midi_rx_buffer;
    uint8_t *cdc_rx_buffer;
    uint16_t midi_rx_size;
    uint16_t cdc_rx_size;
    /* User data to be passed through the callbacks */
    void *midi_user;
    void *cdc_user;
    /* Transmission Callbacks */
    void (* midi_rx_complete)(uint8_t *, uint16_t, void *);
    void (* midi_tx_complete)(void *);
    void (* cdc_rx_complete)(uint8_t *, uint16_t, void *);
    void (* cdc_tx_complete)(void *);
} usb_hjalmar_t;

int usb_hjalmar_is_configured(void);
int usb_midi_transmit(uint8_t *data, uint16_t length);
int usb_cdc_transmit(uint8_t *data, uint16_t length);

int usb_hjalmar_init(usb_hjalmar_t *usb_hjalmar);
int usb_hjalmar_deinit(void);

#endif /* USB_HJALMAR_H_ */