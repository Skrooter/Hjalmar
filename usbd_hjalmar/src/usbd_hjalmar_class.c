
#include "usbd_hjalmar_desc.h"
#include "usbd_internal.h"
#include "usb_hjalmar.h"
#include "error_codes.h"

static struct {
    uint32_t dwBaudrate;
    uint8_t bStopBits;
    uint8_t bParityType;
    uint8_t bDataBits;
} __attribute__((packed)) cdc_line_coding = {
    .dwBaudrate = 921600,
    .bStopBits = 1,
    .bParityType = 1,
    .bDataBits = 8,
};

static uint8_t tx_busy = 0;
static usb_hjalmar_t *hjalmar = NULL;

static int usb_transmit(uint8_t epnum, uint8_t *data, uint16_t length)
{
    int ret = HJALMAR_BUSY;

    if (tx_busy)
    {
        goto error;
    }

    ret = usbd_transmit(epnum, data, length);
    if (ret)
    {
        goto error;
    }

    tx_busy = 1;

error:

    return ret;
}

int usbd_hjalmar_class_init(usbd_context_t *ctx, uint8_t cfgidx)
{
	(void)cfgidx;
    int ret = 0;

    /* Open Endpoints */
    ret = usbd_ep_open(ctx, USBD_EP_MIDI_TX,
                       USBD_EP_BULK_TYPE, USBD_BULK_PACKET_SIZE);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_open(ctx, USBD_EP_MIDI_RX,
                       USBD_EP_BULK_TYPE, USBD_BULK_PACKET_SIZE);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_open(ctx, USBD_EP_CDC_TX,
                       USBD_EP_BULK_TYPE, USBD_BULK_PACKET_SIZE);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_open(ctx, USBD_EP_CDC_RX,
                       USBD_EP_BULK_TYPE, USBD_BULK_PACKET_SIZE);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_open(ctx, USBD_EP_CDC_CMD,
                       USBD_EP_INTR_TYPE, USBD_INT_PACKET_SIZE);
    if (ret)
    {
        goto error;
    }

    if ((ctx != NULL) && (hjalmar != NULL))
    {
        ctx->class_data = (void *)hjalmar;

        usbd_ep_receive(ctx, USBD_EP_MIDI_RX, hjalmar->midi_rx_buffer, hjalmar->midi_rx_size);
        usbd_ep_receive(ctx, USBD_EP_CDC_RX, hjalmar->cdc_rx_buffer, hjalmar->cdc_rx_size);
    }
    else
    {
        ret = HJALMAR_FAILED;
    }

error:

    return ret;
}

int usbd_hjalmar_class_deinit(usbd_context_t *ctx, uint8_t cfgidx)
{
	(void)cfgidx;
    int ret = 0;

    ret = usbd_ep_close(ctx, USBD_EP_MIDI_TX);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_close(ctx, USBD_EP_MIDI_RX);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_close(ctx, USBD_EP_CDC_TX);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_close(ctx, USBD_EP_CDC_RX);
    if (ret)
    {
        goto error;
    }

    ret = usbd_ep_close(ctx, USBD_EP_CDC_CMD);
    if (ret)
    {
        goto error;
    }

    ctx->class_data = NULL;

error:

    return ret;
}

int usbd_hjalmar_setup(usbd_context_t *ctx,
                       usb_setup_packet_t *setup)
{
    uint8_t ifalt = 0;

    switch (setup->bmRequestType.type)
    {
    case USB_REQUEST_TYPE_CLASS:
        if (setup->wLength)
        {
            if ((setup->bmRequestType.direction) &&
                (setup->bRequest == CDC_GET_LINE_CODING))
            {
                usbd_ctrl_transmit(ctx, (uint8_t *)&cdc_line_coding,
                                   sizeof(cdc_line_coding));
            }
            else if (setup->bRequest == CDC_SET_LINE_CODING)
            {
                usbd_ctrl_receive(ctx, (uint8_t *)&cdc_line_coding,
                                  sizeof(cdc_line_coding));
            }
        }
        break;
    case USB_REQUEST_TYPE_STANDARD:
        if (setup->bRequest == USB_REQ_GET_INTERFACE)
        {
            usbd_ctrl_transmit(ctx, &ifalt, 1);
        }
        break;
    default:
        break;
    }

    return HJALMAR_OK;
}

void usbd_midi_tx(usbd_context_t *ctx)
{
    usb_hjalmar_t *hj = (usb_hjalmar_t *)ctx->class_data;
    tx_busy = 0;

    if (hj)
    {
        if (hj->midi_tx_complete)
        {
            hj->midi_tx_complete(hj->midi_user);
        }
    }
}

void usbd_midi_rx(usbd_context_t *ctx, uint16_t length)
{
    usb_hjalmar_t *hj = (usb_hjalmar_t *)ctx->class_data;
    if (hj)
    {
        if (hj->midi_rx_complete)
        {
            hj->midi_rx_complete(hj->midi_rx_buffer, length, hj->midi_user);
        }

        usbd_ep_receive(ctx, USBD_EP_MIDI_RX, hj->midi_rx_buffer, hj->midi_rx_size);
    }
}

void usbd_cdc_tx(usbd_context_t *ctx)
{
    usb_hjalmar_t *hj = (usb_hjalmar_t *)ctx->class_data;
    tx_busy = 0;

    if (hj)
    {
        if (hj->cdc_tx_complete)
        {
            hj->cdc_tx_complete(hj->cdc_user);
        }
    }
}

void usbd_cdc_rx(usbd_context_t *ctx, uint16_t length)
{
    usb_hjalmar_t *hj = (usb_hjalmar_t *)ctx->class_data;
    if (hj)
    {
        if (hj->cdc_rx_complete)
        {
            hj->cdc_rx_complete(hj->cdc_rx_buffer, length, hj->cdc_user);
        }

        usbd_ep_receive(ctx, USBD_EP_CDC_RX, hj->cdc_rx_buffer, hj->cdc_rx_size);
    }
}

int usb_midi_transmit(uint8_t *data, uint16_t length)
{
    return usb_transmit(USBD_EP_MIDI_TX, data, length);
}

int usb_cdc_transmit(uint8_t *data, uint16_t length)
{
    return usb_transmit(USBD_EP_CDC_TX, data, length);
}

int usb_hjalmar_init(usb_hjalmar_t *usb_hjalmar)
{
    int ret = HJALMAR_OK;

    if (usb_hjalmar == NULL)
    {
        ret = HJALMAR_FAILED;
        goto error;
    }

    /* Check if we have valid buffers */
    if ((usb_hjalmar->midi_rx_buffer == NULL) ||
        (usb_hjalmar->midi_rx_size == 0) ||
        (usb_hjalmar->cdc_rx_buffer == NULL) ||
        (usb_hjalmar->cdc_rx_size == 0))
    {
        ret = HJALMAR_INVALID_ARGUMENT;
        goto error;
    }

    ret = usbd_init();
    if (ret)
    {
        goto error;
    }

    hjalmar = usb_hjalmar;

error:

    return ret;
}

int usb_hjalmar_deinit(void)
{
    hjalmar = NULL;

    return usbd_deinit();
}
