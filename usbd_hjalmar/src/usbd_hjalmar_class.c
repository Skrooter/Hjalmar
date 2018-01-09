
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

static uint8_t midi_rx_idx = 0;
static uint8_t cdc_rx_idx = 0;
static uint8_t *midi_rx_buffer = NULL;
static uint8_t *cdc_rx_buffer = NULL;
static uint16_t midi_rx_size = 0;
static uint16_t cdc_rx_size = 0;

static usb_hjalmar_t *hjalmar = NULL;

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
        midi_rx_idx = 0;
        cdc_rx_idx = 0;

        midi_rx_buffer = hjalmar->midi_rx_buffers[midi_rx_idx];
        midi_rx_size = hjalmar->midi_rx_sizes[midi_rx_idx];

        cdc_rx_buffer = hjalmar->cdc_rx_buffers[cdc_rx_idx];
        cdc_rx_size = hjalmar->cdc_rx_sizes[cdc_rx_idx];

        usbd_ep_receive(ctx, USBD_EP_MIDI_RX, midi_rx_buffer, midi_rx_size);
        usbd_ep_receive(ctx, USBD_EP_CDC_RX, cdc_rx_buffer, cdc_rx_size);
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

    hjalmar = NULL;

    midi_rx_idx = 0;
    cdc_rx_idx = 0;
    midi_rx_buffer = NULL;
    cdc_rx_buffer = NULL;
    midi_rx_size = 0;
    cdc_rx_size = 0;

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

    return 0;
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
            hj->midi_rx_complete(midi_rx_buffer, length, hj->midi_user);
            midi_rx_idx = (midi_rx_idx + 1) % hjalmar->midi_num_of_buffers;
        }

        midi_rx_buffer = hjalmar->midi_rx_buffers[midi_rx_idx];
        midi_rx_size = hjalmar->midi_rx_sizes[midi_rx_idx];

        usbd_ep_receive(ctx, USBD_EP_MIDI_RX, midi_rx_buffer, midi_rx_size);
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
            hj->cdc_rx_complete(cdc_rx_buffer, length, hj->cdc_user);
            cdc_rx_idx = (cdc_rx_idx + 1) % hjalmar->cdc_num_of_buffers;
        }

        cdc_rx_buffer = hjalmar->cdc_rx_buffers[cdc_rx_idx];
        cdc_rx_size = hjalmar->cdc_rx_sizes[cdc_rx_idx];

        usbd_ep_receive(ctx, USBD_EP_CDC_RX, cdc_rx_buffer, cdc_rx_size);
    }
}

int usb_midi_transmit(uint8_t *data, uint16_t length)
{
    int ret = HJALMAR_OK;
    usbd_context_t *ctx = usbd_get_context();

    if (ctx == NULL)
    {
        ret = HJALMAR_FAILED;
        goto error;
    }

    if ((ctx->current_state != USB_DEVICE_STATE_CONFIGURED) ||
        (ctx->class_data == NULL))
    {
        ret = HJALMAR_FAILED;
        goto error;
    }

    if (tx_busy)
    {
        ret = HJALMAR_BUSY;
        goto error;
    }

    tx_busy = 1;
    usbd_ep_transmit(ctx, USBD_EP_MIDI_TX, data, length);

error:

    return ret;
}

int usb_cdc_transmit(uint8_t *data, uint16_t length)
{
    int ret = HJALMAR_OK;
    usbd_context_t *ctx = usbd_get_context();

    if (ctx == NULL)
    {
        ret = HJALMAR_FAILED;
        goto error;
    }

    if (ctx->current_state != USB_DEVICE_STATE_CONFIGURED)
    {
        ret = HJALMAR_FAILED;
        goto error;
    }

    if (tx_busy)
    {
        ret = HJALMAR_BUSY;
        goto error;
    }

    tx_busy = 1;
    usbd_ep_transmit(ctx, USBD_EP_CDC_TX, data, length);

error:

    return ret;
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
    if ((usb_hjalmar->midi_num_of_buffers != 0) ||
        (usb_hjalmar->cdc_num_of_buffers != 0))
    {
        for (int i = 0; i < usb_hjalmar->midi_num_of_buffers; i++)
        {
            if (usb_hjalmar->midi_rx_buffers[i] == NULL)
            {
                ret = HJALMAR_FAILED;
                goto error;
            }
        }

        for (int i = 0; i < usb_hjalmar->cdc_num_of_buffers; i++)
        {
            if (usb_hjalmar->cdc_rx_buffers[i] == NULL)
            {
                ret = HJALMAR_FAILED;
                goto error;
            }
        }
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
