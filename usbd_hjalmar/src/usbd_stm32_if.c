
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usbd_internal.h"
#include "usbd_hjalmar_desc.h"
#include "error_codes.h"

PCD_HandleTypeDef hpcd_hjalmar;

void _Error_Handler(char * file, int line);
void SystemClock_Config(void);

static int stm32_to_hjalmar_error(HAL_StatusTypeDef status)
{
    switch (status)
    {
    case HAL_OK: return HJALMAR_OK;
    case HAL_ERROR: return HJALMAR_FAILED;
    case HAL_BUSY: return HJALMAR_BUSY;
    case HAL_TIMEOUT: return HJALMAR_TIMEOUT;
    }

    return HJALMAR_INVALID_ARGUMENT;
}

/*------------------------------------------------------------------------------*/
/*  STM32 HAL_PCD callbacks                                                     */
/*------------------------------------------------------------------------------*/

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(pcdHandle->Instance == USB_OTG_FS)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

        HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
    if(pcdHandle->Instance == USB_OTG_FS)
    {
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_setup_stage((usbd_context_t *)hpcd->pData, (usb_setup_packet_t *)hpcd->Setup);
}

/* Rx complete callback */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    uint8_t *xfer_buff = hpcd->OUT_ep[0xF & epnum].xfer_buff;
    uint16_t xfer_count = hpcd->OUT_ep[0xF & epnum].xfer_count;

    usbd_data_rx_stage((usbd_context_t *)hpcd->pData, epnum, xfer_buff, xfer_count);
}

/* Tx complete callback */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    uint8_t *xfer_buff = hpcd->IN_ep[0xF & epnum].xfer_buff;

    usbd_data_tx_stage((usbd_context_t *)hpcd->pData, epnum, xfer_buff);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_start_of_frame((usbd_context_t *)hpcd->pData);
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_reset((usbd_context_t *)hpcd->pData);
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_suspend((usbd_context_t *)hpcd->pData);
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_resume((usbd_context_t *)hpcd->pData);
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_connect((usbd_context_t *)hpcd->pData);
}


void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    usbd_disconnect((usbd_context_t *)hpcd->pData);
}

/*------------------------------------------------------------------------------*/
/*  STM32 HAL_PCD driver interface                                              */
/*------------------------------------------------------------------------------*/

int usbd_hw_init(usbd_context_t *ctx)
{
    /* Init USB_IP */
    hpcd_hjalmar.pData = ctx;
    ctx->dev_data = &hpcd_hjalmar;

    hpcd_hjalmar.Instance = USB_OTG_FS;
    hpcd_hjalmar.Init.dev_endpoints = 7;
    hpcd_hjalmar.Init.speed = PCD_SPEED_FULL;
    hpcd_hjalmar.Init.dma_enable = DISABLE;
    hpcd_hjalmar.Init.ep0_mps = DEP0CTL_MPS_64;
    hpcd_hjalmar.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_hjalmar.Init.Sof_enable = DISABLE;
    hpcd_hjalmar.Init.low_power_enable = DISABLE;
    hpcd_hjalmar.Init.lpm_enable = DISABLE;
    hpcd_hjalmar.Init.vbus_sensing_enable = DISABLE;
    hpcd_hjalmar.Init.use_dedicated_ep1 = DISABLE;
    if (HAL_PCD_Init(&hpcd_hjalmar) != HAL_OK)
    {
        return HJALMAR_FAILED;
    }

    HAL_PCDEx_SetRxFiFo(&hpcd_hjalmar, 0x80);
    HAL_PCDEx_SetTxFiFo(&hpcd_hjalmar, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(&hpcd_hjalmar, 1, 0x80);

    return HJALMAR_OK;
}

int usbd_hw_deinit(usbd_context_t *ctx)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_DeInit(&hpcd_hjalmar);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_start(usbd_context_t *ctx)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_Start(&hpcd_hjalmar);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_stop(usbd_context_t *ctx)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_Stop(&hpcd_hjalmar);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_open(usbd_context_t *ctx, uint8_t ep_addr,
                 uint8_t ep_type, uint16_t ep_mps)
{
    (void)ctx;

    HAL_StatusTypeDef hal_status = HAL_PCD_EP_Open(&hpcd_hjalmar, ep_addr, ep_mps, ep_type);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_close(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_EP_Close(&hpcd_hjalmar, ep_addr);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_flush(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_EP_Flush(&hpcd_hjalmar, ep_addr);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_stall(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_EP_SetStall(&hpcd_hjalmar, ep_addr);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_stall_clear(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_EP_ClrStall(&hpcd_hjalmar, ep_addr);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_is_stalled(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    if((ep_addr & 0x80) == 0x80)
    {
        return (int)hpcd_hjalmar.IN_ep[ep_addr & 0x7F].is_stall;
    }
    else
    {
        return (int)hpcd_hjalmar.OUT_ep[ep_addr & 0x7F].is_stall;
    }
}

int usbd_set_address(usbd_context_t *ctx, uint8_t dev_addr)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_SetAddress(&hpcd_hjalmar, dev_addr);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_transmit(usbd_context_t *ctx, uint8_t ep_addr,
                     const uint8_t *tx_buff, uint16_t size)
{
    (void)ctx;

    HAL_StatusTypeDef hal_status = HAL_PCD_EP_Transmit(&hpcd_hjalmar, ep_addr, tx_buff, size);

    return stm32_to_hjalmar_error(hal_status);
}

int usbd_ep_receive(usbd_context_t *ctx, uint8_t ep_addr,
                    uint8_t *rx_buff, uint16_t size)
{
    (void)ctx;
    HAL_StatusTypeDef hal_status = HAL_PCD_EP_Receive(&hpcd_hjalmar, ep_addr, rx_buff, size);

    return stm32_to_hjalmar_error(hal_status);
}

uint16_t usbd_get_rx_data_len(usbd_context_t *ctx, uint8_t ep_addr)
{
    (void)ctx;
    return HAL_PCD_EP_GetRxCount(&hpcd_hjalmar, ep_addr);
}

void usbd_delay_ms(uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}
