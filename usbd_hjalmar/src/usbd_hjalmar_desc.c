
#include "usbd_hjalmar_desc.h"
#include "usbd_internal.h"

#define USBD_VID                        0x0483 /*STM32 Full Speed Default VID*/
#define USBD_PID                        0x5740 /*STM32 Full Speed Default PID*/

#define USBD_LANGID_ENG_US              0x0409
#define USBD_MANUFACTURER_STRING        L"STMicroelectronics"
#define USBD_PRODUCT_STRING             L"Hjalmar USB/MIDI Converter"
#define USBD_SERIALNUMBER_STRING        L"00000000001A"
#define USBD_CONFIGURATION_STRING       L"Hjalmar Config"
#define USBD_MIDI_IF_STRING             L"Hjalmar USB/MIDI Interface"
#define USBD_CDC_IF_STRING              L"Hjalmar CDC Interface"

/* USB Standard Device Descriptor */
static uint8_t usbd_hjalmar_dev_desc[] = {
    USB_DEVICE_DESC_TYPE,
    USB_DEVICE_DESC_SIZE,
    0x00,
    0x02,
    0xEF,
    0x02,
    0x01,
    0x40,
    LOBYTE(USBD_VID),
    HIBYTE(USBD_VID),
    LOBYTE(USBD_PID),
    HIBYTE(USBD_PID),
    0x00,
    0x02,
    USBD_IDX_MFC_STR,
    USBD_IDX_PRODUCT_STR,
    USBD_IDX_SERIAL_STR,
    0x01,
};

/* USB CDC device Configuration Descriptor */
static uint8_t usbd_hjalmar_cfg_desc[] = {
    /*Configuration Descriptor*/
    USB_CONFIG_DESC_SIZE,               /* bLength: Configuration Descriptor size */
    USB_CONFIG_DESC_TYPE,               /* bDescriptorType: Configuration */
    LOBYTE(USBD_CFG_SIZE),              /* wTotalLength:no of returned bytes */
    HIBYTE(USBD_CFG_SIZE),
    0x04,                               /* bNumInterfaces: 4 interface */
    0x01,                               /* bConfigurationValue: Configuration value */
    0x00,                               /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,                               /* bmAttributes: self powered */
    0x32,                               /* MaxPower 0 mA */

/*---------------------------------------------------------------------------*/

    /* IAD Class descriptor */
    USB_IAD_DESC_SIZE,                  /* bLength: Interface Descriptor size */
    USB_IAD_DESC_TYPE,                  /* bDescriptorType: IAD */
    0x00,                               /* bInterfaceNumber: Number of Interface */
    0x02,                               /* bNumInterfaces: 2 interfaces */
    0x01,                               /* bFunctionClass: Audio Class */
    0x03,                               /* bFunctionSubClass: MIDI streaming */
    0x00,                               /* bFunctionProtocol */
    0x00,                               /* iFunction */

/*---------------------------------------------------------------------------*/
/*  MIDI Interface                                                           */
/*---------------------------------------------------------------------------*/

    /*Interface Descriptor */
    USB_INTERFACE_DESC_SIZE,            /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESC_TYPE,            /* bDescriptorType: Interface */
    0x00,                               /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: Alternate setting */
    0x00,                               /* bNumEndpoints: One endpoints used */
    0x01,                               /* bInterfaceClass: Audio Class */
    0x01,                               /* bInterfaceSubClass: MIDI streaming */
    0x00,                               /* bInterfaceProtocol */
    USBD_IDX_MIDI_IF_STR,                /* iInterface: */

    /* MIDI Interface Header Descriptor */
    USBD_MIDI_CONTROL_SIZE,             /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    0x01,                               /* bDescriptorSubType */
    0x00,                               /* bcdMSC */
    0x01,                               /* bcdMSC */
    LOBYTE(USBD_MIDI_CONTROL_SIZE),     /* wTotalLenght */
    HIBYTE(USBD_MIDI_CONTROL_SIZE),     /* wTotalLenght */
    0x01,                               /* Number of Streaming interfaces */
    0x01,                               /* Streaming interface 1 belongs here */

    /*Interface Descriptor */
    USB_INTERFACE_DESC_SIZE,            /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESC_TYPE,            /* bDescriptorType: Interface */
    0x01,                               /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: Alternate setting */
    0x02,                               /* bNumEndpoints: One endpoints used */
    0x01,                               /* bInterfaceClass: Audio Class */
    0x03,                               /* bInterfaceSubClass: MIDI streaming */
    0x00,                               /* bInterfaceProtocol */
    USBD_IDX_MIDI_IF_STR,                /* iInterface: */

    /* MIDI Interface Header Descriptor */
    USBD_MIDI_STREAM_SIZE,              /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    0x01,                               /* bDescriptorSubType */
    0x00,                               /* bcdMSC */
    0x01,                               /* bcdMSC */
    LOBYTE(54),                         /* wTotalLenght */
    HIBYTE(54),                         /* wTotalLenght */

    /* MIDI IN Jack Embedded Descriptor */
    USBD_MIDI_IN_JACK,                  /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    USBD_MIDI_IN_JACK,                  /* bDescriptorSubType */
    USBD_MIDI_JACK_EMBEDDED,            /* JackType */
    0x01,                               /* JackId */
    0x00,                               /* Unused */

    /* MIDI IN Jack External Descriptor */
    USBD_MIDI_JACK_IN_SIZE,             /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    USBD_MIDI_IN_JACK,                  /* bDescriptorSubType */
    USBD_MIDI_JACK_EXTERNAL,            /* JackType */
    0x02,                               /* JackId */
    0x00,                               /* Unused */

    /* MIDI OUT Jack Embedded Descriptor */
    USBD_MIDI_JACK_OUT_SIZE,            /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    USBD_MIDI_OUT_JACK,                 /* bDescriptorSubType */
    USBD_MIDI_JACK_EMBEDDED,            /* JackType */
    0x03,                               /* JackId */
    0x01,                               /* Number of Jack pins */
    0x02,                               /* ID of connected entity */
    0x01,                               /* Output pin number of connected entity */
    0x00,                               /* Unused */

    /* MIDI OUT Jack External Descriptor */
    USBD_MIDI_JACK_OUT_SIZE,            /* bLength */
    USBD_MIDI_CS_IF_DESC_TYPE,          /* bDescriptorType */
    USBD_MIDI_OUT_JACK,                 /* bDescriptorSubType */
    USBD_MIDI_JACK_EXTERNAL,            /* JackType */
    0x04,                               /* JackId */
    0x01,                               /* Number of Jack pins */
    0x01,                               /* ID of connected entity */
    0x01,                               /* Output pin number of connected entity */
    0x00,                               /* Unused */

    /* Endpoint OUT Descriptor */
    USB_ENDPOINT_DESC_SIZE,             /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESC_TYPE,             /* bDescriptorType: Endpoint */
    USBD_EP_MIDI_RX,                    /* bEndpointAddress */
    0x02,                               /* bmAttributes: Bulk */
    LOBYTE(USBD_BULK_PACKET_SIZE),      /* wMaxPacketSize: */
    HIBYTE(USBD_BULK_PACKET_SIZE),
    0x00,                               /* bInterval: ignore for Bulk transfer */

    /* Class-specific MIDI bulk endpoint descriptor */
    USBD_MIDI_ENDPOINT_SIZE,            /* bLength: Endpoint Descriptor size */
    USBD_MIDI_CS_EP_DESC_TYPE,          /* bDescriptorType: CS EP */
    0x01,                               /* bDescriptorSubType: MS General */
    0x01,                               /* bNumEmbMidiOutJacks */
    0x01,                               /* bAssociatedMidiJack */

    /* Endpoint IN Descriptor */
    USB_ENDPOINT_DESC_SIZE,             /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESC_TYPE,             /* bDescriptorType: Endpoint */
    USBD_EP_MIDI_TX,                    /* bEndpointAddress */
    0x02,                               /* bmAttributes: Bulk */
    LOBYTE(USBD_BULK_PACKET_SIZE),      /* wMaxPacketSize: */
    HIBYTE(USBD_BULK_PACKET_SIZE),
    0x00,                               /* bInterval: ignore for Bulk transfer */

    /* Class-specific MIDI bulk endpoint descriptor */
    USBD_MIDI_ENDPOINT_SIZE,            /* bLength: Endpoint Descriptor size */
    USBD_MIDI_CS_EP_DESC_TYPE,          /* bDescriptorType: CS EP */
    0x01,                               /* bDescriptorSubType: MS General */
    0x01,                               /* bNumEmbMidiInJacks */
    0x03,                               /* bAssociatedMidiJack */

/*---------------------------------------------------------------------------*/

    /* IAD Class descriptor */
    USB_IAD_DESC_SIZE,                  /* bLength: Interface Descriptor size */
    USB_IAD_DESC_TYPE,                  /* bDescriptorType: IAD */
    0x02,                               /* bInterfaceNumber: Number of Interface */
    0x02,                               /* bNumInterfaces: 2 interfaces */
    0x02,                               /* bFunctionClass: Communication Interface Class */
    0x02,                               /* bFunctionSubClass: Abstract Control Model */
    0x01,                               /* bFunctionProtocol: Common AT commands */
    0x02,                               /* iFunction */

/*---------------------------------------------------------------------------*/
/*  CDC Interface                                                            */
/*---------------------------------------------------------------------------*/

    /*Interface Descriptor */
    USB_INTERFACE_DESC_SIZE,            /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESC_TYPE,            /* bDescriptorType: Interface */
    0x02,                               /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: Alternate setting */
    0x01,                               /* bNumEndpoints: One endpoints used */
    0x02,                               /* bInterfaceClass: Communication Interface Class */
    0x02,                               /* bInterfaceSubClass: Abstract Control Model */
    0x01,                               /* bInterfaceProtocol: Common AT commands */
    USBD_IDX_CDC_IF_STR,                /* iInterface: */

    /*Header Functional Descriptor*/
    0x05,                               /* bLength: Endpoint Descriptor size */
    USBD_CDC_CS_IF_DESC_TYPE,           /* bDescriptorType: CS_INTERFACE */
    0x00,                               /* bDescriptorSubtype: Header Func Desc */
    0x10,                               /* bcdCDC: spec release number */
    0x01,

    /*Call Management Functional Descriptor*/
    0x05,                               /* bFunctionLength */
    USBD_CDC_CS_IF_DESC_TYPE,           /* bDescriptorType: CS_INTERFACE */
    0x01,                               /* bDescriptorSubtype: Call Management Func Desc */
    0x00,                               /* bmCapabilities: D0+D1 */
    0x01,                               /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04,                               /* bFunctionLength */
    USBD_CDC_CS_IF_DESC_TYPE,           /* bDescriptorType: CS_INTERFACE */
    0x02,                               /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,                               /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05,                               /* bFunctionLength */
    USBD_CDC_CS_IF_DESC_TYPE,           /* bDescriptorType: CS_INTERFACE */
    0x06,                               /* bDescriptorSubtype: Union func desc */
    0x00,                               /* bMasterInterface: Communication class interface */
    0x01,                               /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07,                               /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESC_TYPE,             /* bDescriptorType: Endpoint */
    USBD_EP_CDC_CMD,                    /* bEndpointAddress */
    0x03,                               /* bmAttributes: Interrupt */
    LOBYTE(USBD_INT_PACKET_SIZE),        /* wMaxPacketSize: */
    HIBYTE(USBD_INT_PACKET_SIZE),
    0x10,                               /* bInterval: */

/*---------------------------------------------------------------------------*/

    /*Data class interface descriptor*/
    0x09,                               /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESC_TYPE,            /* bDescriptorType: */
    0x03,                               /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: Alternate setting */
    0x02,                               /* bNumEndpoints: Two endpoints used */
    0x0A,                               /* bInterfaceClass: CDC */
    0x00,                               /* bInterfaceSubClass: */
    0x00,                               /* bInterfaceProtocol: */
    USBD_IDX_CDC_IF_STR,                /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07,                               /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESC_TYPE,             /* bDescriptorType: Endpoint */
    USBD_EP_CDC_RX,                     /* bEndpointAddress */
    0x02,                               /* bmAttributes: Bulk */
    LOBYTE(USBD_BULK_PACKET_SIZE),      /* wMaxPacketSize: */
    HIBYTE(USBD_BULK_PACKET_SIZE),
    0x00,                               /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,                               /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESC_TYPE,             /* bDescriptorType: Endpoint */
    USBD_EP_CDC_TX,                     /* bEndpointAddress */
    0x02,                               /* bmAttributes: Bulk */
    LOBYTE(USBD_BULK_PACKET_SIZE),      /* wMaxPacketSize: */
    HIBYTE(USBD_BULK_PACKET_SIZE),
    0x00                                /* bInterval: ignore for Bulk transfer */
};

#define USB_STRING(Name, String) \
struct { \
    uint8_t  bLength; \
    uint8_t  bDescriptorType; \
    uint16_t wString[sizeof(String) / sizeof(uint16_t) - 1]; \
} __attribute__((packed)) Name = { \
    .bLength = sizeof(Name), \
    .bDescriptorType = USB_STRING_DESC_TYPE, \
    .wString = String \
}

USB_STRING(usbd_hjalmar_mfc_str, USBD_MANUFACTURER_STRING);
USB_STRING(usbd_hjalmar_prod_str, USBD_PRODUCT_STRING);
USB_STRING(usbd_hjalmar_serial_str, USBD_SERIALNUMBER_STRING);
USB_STRING(usbd_hjalmar_cfg_str, USBD_CONFIGURATION_STRING);
USB_STRING(usbd_hjalmar_midi_if_str, USBD_MIDI_IF_STRING);
USB_STRING(usbd_hjalmar_cdc_if_str, USBD_CDC_IF_STRING);

uint8_t usbd_hjalmar_langid_str[] = {
    4,
    USB_LANGID_DESC_TYPE,
    LOBYTE(USBD_LANGID_ENG_US),
    HIBYTE(USBD_LANGID_ENG_US),
};

uint8_t *usbd_get_dev_desc(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_dev_desc);
    return usbd_hjalmar_dev_desc;
}

uint8_t *usbd_hjalmar_dev_qualifier_desc(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_dev_qualifier_desc);
    return usbd_hjalmar_dev_qualifier_desc;
}

uint8_t *usbd_get_cfg_desc(uint8_t cfg_no, uint16_t *length)
{
	(void)cfg_no;
    *length = sizeof(usbd_hjalmar_cfg_desc);
    return usbd_hjalmar_cfg_desc;
}

uint8_t *usbd_get_langid_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_langid_str);
    return usbd_hjalmar_langid_str;
}

uint8_t *usbd_get_mfc_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_mfc_str);
    return (uint8_t *)&usbd_hjalmar_mfc_str;
}

uint8_t *usbd_get_prod_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_prod_str);
    return (uint8_t *)&usbd_hjalmar_prod_str;
}

uint8_t *usbd_get_serial_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_serial_str);
    return (uint8_t *)&usbd_hjalmar_serial_str;
}

uint8_t *usbd_get_cfg_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_cfg_str);
    return (uint8_t *)&usbd_hjalmar_cfg_str;
}

uint8_t *usbd_get_midi_if_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_midi_if_str);
    return (uint8_t *)&usbd_hjalmar_midi_if_str;
}

uint8_t *usbd_get_cdc_if_str(uint16_t *length)
{
    *length = sizeof(usbd_hjalmar_cdc_if_str);
    return (uint8_t *)&usbd_hjalmar_cdc_if_str;
}
