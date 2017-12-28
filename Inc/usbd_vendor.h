

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_VENDOR_H
#define __USB_VENDOR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include  "usbd_ioreq.h"

#define USBD_VENDOR_DEVICE_CLASS                        0xFF

#define VENDOR_IN_EP1                                   0x81  /* EP1 for data IN */
#define VENDOR_OUT_EP1                                  0x01  /* EP1 for data OUT */

/* Vendor Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define FS_MAX_PACKET_SIZE                              64  /* Endpoint IN & OUT Packet size */

#define USB_VENDOR_CONFIG_DESC_SIZ                      32 /* Cfg = 9 + Int = 9 + Ep = 7 + Ep = 7 */

typedef struct {
    __IO uint32_t tx_state;

    /* receive(uint8_t epnum, uint8_t *rx_buffer, uint32_t *rx_len) */
    int32_t (* receive)(uint8_t, uint8_t *, uint32_t *); /* Receive callback */

} USBD_vendor_context_t;

typedef struct {

} USBD_vendor_callbacks_t;

USBD_ClassTypeDef *USBD_vendor_get_class_def (void);
void USBD_vendor_register_cb(int32_t (* receive)(uint8_t, uint8_t *, uint32_t *));




#ifdef __cplusplus
}
#endif

#endif  /* __USB_VENDOR_H */
