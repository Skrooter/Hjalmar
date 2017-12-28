

/* Includes ------------------------------------------------------------------*/
#include "usbd_desc.h"
#include "usbd_ctlreq.h"
#include "usbd_vendor.h"

static uint8_t  USBD_vendor_init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t  USBD_vendor_deinit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx);

static uint8_t  USBD_vendor_setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t  USBD_vendor_data_in (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_vendor_data_out (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  *USBD_vendor_get_cfg_desc (uint16_t *length);

/* CDC interface class callbacks structure */
static USBD_ClassTypeDef USBD_vendor =
{
    USBD_vendor_init, /* Init */
    USBD_vendor_deinit, /* DeInit */
    USBD_vendor_setup, /* Setup */
    NULL, /* EP0_TxSent */
    NULL, /* EP0_RxReady */
    USBD_vendor_data_in, /* Data in */
    USBD_vendor_data_out, /* Data out */
    NULL, /* Start of Frame (SOF) */
    NULL, /* Iso in */
    NULL, /* Iso out */
    NULL, /* High speed config */
    USBD_vendor_get_cfg_desc, /* Full speed config */
    NULL, /* Other speed config */
    NULL, /* Get user string */
};

/* USB CDC device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_vendor_cfg_desc[] __ALIGN_END =
{
    /*Configuration Descriptor*/
    0x09, /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_VENDOR_CONFIG_DESC_SIZ, /* wTotalLength:no of returned bytes */
    0x00,
    0x01, /* bNumInterfaces: 2 interface */
    0x01, /* bConfigurationValue: Configuration value */
    0x00, /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0, /* bmAttributes: self powered */
    0x32, /* MaxPower 100 mA, 2mA in steps of 2 mA. */

/*---------------------------------------------------------------------------*/

    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: One endpoints used */
    USBD_VENDOR_DEVICE_CLASS,   /* bInterfaceClass: Communication Interface Class */
    USBD_VENDOR_DEVICE_CLASS,   /* bInterfaceSubClass: Abstract Control Model */
    USBD_VENDOR_DEVICE_CLASS,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */

/*---------------------------------------------------------------------------*/

    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
    VENDOR_OUT_EP1,                        /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(FS_MAX_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
    VENDOR_IN_EP1,                         /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(FS_MAX_PACKET_SIZE),
    0x00                               /* bInterval: ignore for Bulk transfer */
};

static uint8_t  USBD_vendor_init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   VENDOR_IN_EP1,
                   USBD_EP_TYPE_BULK,
                   FS_MAX_PACKET_SIZE);

    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   VENDOR_OUT_EP1,
                   USBD_EP_TYPE_BULK,
                   FS_MAX_PACKET_SIZE);

    return USBD_OK;
}

static uint8_t  USBD_vendor_deinit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
  /* Open EP IN */
  USBD_LL_CloseEP(pdev,
              VENDOR_IN_EP1);

  /* Open EP OUT */
  USBD_LL_CloseEP(pdev,
              VENDOR_OUT_EP1);

  return USBD_OK;
}

static uint8_t  USBD_vendor_setup (USBD_HandleTypeDef *pdev,
                                   USBD_SetupReqTypedef *req)
{
    static uint8_t ifalt = 0;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        case USB_REQ_GET_INTERFACE :
            USBD_CtlSendData (pdev, &ifalt, 1);
            break;

        case USB_REQ_SET_INTERFACE :
            break;
        }

    default:
        break;
    }
    return USBD_OK;
}

static uint8_t  USBD_vendor_data_in (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    return USBD_OK;
}

static uint8_t  USBD_vendor_data_out (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    return USBD_OK;
}

static uint8_t  *USBD_vendor_get_cfg_desc (uint16_t *length)
{
    *length = sizeof (USBD_vendor_cfg_desc);
    return USBD_vendor_cfg_desc;
}

/*---------------------------------------------------------------------------*/

USBD_ClassTypeDef *USBD_vendor_get_class_def (void)
{
    return &USBD_vendor;
}

/**
* @brief  USBD_CDC_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: CD  Interface callback
  * @retval status
  */
// uint8_t  USBD_CDC_RegisterInterface  (USBD_HandleTypeDef   *pdev,
//                                       USBD_CDC_ItfTypeDef *fops)
// {
//   uint8_t  ret = USBD_FAIL;

//   if(fops != NULL)
//   {
//     pdev->pUserData= fops;
//     ret = USBD_OK;
//   }

//   return ret;
// }

/**
  * @brief  USBD_CDC_SetTxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
  */
// uint8_t  USBD_CDC_SetTxBuffer  (USBD_HandleTypeDef   *pdev,
//                                 uint8_t  *pbuff,
//                                 uint16_t length)
// {
//   USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) pdev->pClassData;

//   hcdc->TxBuffer = pbuff;
//   hcdc->TxLength = length;

//   return USBD_OK;
// }


/**
  * @brief  USBD_CDC_SetRxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Rx Buffer
  * @retval status
  */
// uint8_t  USBD_CDC_SetRxBuffer  (USBD_HandleTypeDef   *pdev,
//                                    uint8_t  *pbuff)
// {
//   USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) pdev->pClassData;

//   hcdc->RxBuffer = pbuff;

//   return USBD_OK;
// }

/**
  * @brief  USBD_CDC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
// uint8_t  USBD_CDC_TransmitPacket(USBD_HandleTypeDef *pdev)
// {
//   USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) pdev->pClassData;

//   if(pdev->pClassData != NULL)
//   {
//     if(hcdc->TxState == 0)
//     {
//       /* Tx Transfer in progress */
//       hcdc->TxState = 1;

//        Transmit next packet
//       USBD_LL_Transmit(pdev,
//                        CDC_IN_EP,
//                        hcdc->TxBuffer,
//                        hcdc->TxLength);

//       return USBD_OK;
//     }
//     else
//     {
//       return USBD_BUSY;
//     }
//   }
//   else
//   {
//     return USBD_FAIL;
//   }
// }


/**
  * @brief  USBD_CDC_ReceivePacket
  *         prepare OUT Endpoint for reception
  * @param  pdev: device instance
  * @retval status
  */

// uint8_t  USBD_CDC_ReceivePacket(USBD_HandleTypeDef *pdev)
// {
//   USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) pdev->pClassData;

//   /* Suspend or Resume USB Out process */
//   if(pdev->pClassData != NULL)
//   {
//     if(pdev->dev_speed == USBD_SPEED_HIGH  )
//     {
//       /* Prepare Out endpoint to receive next packet */
//       USBD_LL_PrepareReceive(pdev,
//                              CDC_OUT_EP,
//                              hcdc->RxBuffer,
//                              CDC_DATA_HS_OUT_PACKET_SIZE);
//     }
//     else
//     {
//       /* Prepare Out endpoint to receive next packet */
//       USBD_LL_PrepareReceive(pdev,
//                              CDC_OUT_EP,
//                              hcdc->RxBuffer,
//                              CDC_DATA_FS_OUT_PACKET_SIZE);
//     }
//     return USBD_OK;
//   }
//   else
//   {
//     return USBD_FAIL;
//   }
// }

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
