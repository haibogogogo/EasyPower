//---------------------------------------------------------------------------------------------------------------------
//
// CM CORPORATION CONFIDENTIAL
// ________________________________
//
// [2015] CM Corporation
// All Rights Reserved.
//
// NOTICE: This is an unpublished work of authorship, which contains trade secrets.
// CM Corporation owns all rights to this work and intends to maintain it in confidence to preserve
// its trade secret status. CM Corporation reserves the right, under the copyright laws of the United States
// or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
// in the event of an inadvertent or deliberate unauthorized publication. CM Corporation also reserves its rights
// under all copyright laws to protect this work as a published work, when appropriate.
// Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
// without the written authorization of CM Corporation.
//
//---------------------------------------------------------------------------------------------------------------------

#include "debugport.h"

#if (USE_BSP_COM_FEATURE > 0)
UART_HandleTypeDef hcom_uart[COMn];
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE > 0)
static USART_TypeDef*  COM_USART[COMn] = {COM1_UART};

#if (USE_COM_LOG > 0)
static COM_TypeDef COM_ActiveLogPort = COM1;
#endif

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsComMspCbValid[COMn] = {0};
#endif
#endif /* (USE_BSP_COM_FEATURE > 0) */

#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief  Configures COM port.
  * @param  COM COM port to be configured.
  *         This parameter can be COM1
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified USART peripheral.
  * @retval BSP error code
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init)
{
    int32_t ret = BSP_ERROR_NONE;

    if(COM > COMn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* Init the UART Msp */
        COM1_MspInit(&hcom_uart[COM]);
#else
        if(IsComMspCbValid == 0U)
        {
            if(BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
            {
                return BSP_ERROR_MSP_FAILURE;
            }
        }
#endif

        if(MX_LPUART1_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
        {
            return BSP_ERROR_PERIPH_FAILURE;
        }
    }

    return ret;
}

/**
  * @brief  DeInit COM port.
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
    int32_t ret = BSP_ERROR_NONE;

    if(COM >= COMn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        /* USART configuration */
        hcom_uart[COM].Instance = COM_USART[COM];

        #if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
        COM1_MspDeInit(&hcom_uart[COM]);
        #endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */

        if(HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
        {
            return BSP_ERROR_PERIPH_FAILURE;
        }
    }

    return ret;
}

/**
  * @brief  Configures COM port.
  * @param  huart USART handle
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified USART peripheral.
  * @retval HAL error code
  */
__weak HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init)
{
    /* USART configuration */
    huart->Instance          = COM_USART[COM1];
    huart->Init.BaudRate     = COM_Init->BaudRate;
    huart->Init.Mode         = UART_MODE_TX_RX;
    huart->Init.Parity       = (uint32_t)COM_Init->Parity;
    huart->Init.WordLength   = COM_Init->WordLength;
    huart->Init.StopBits     = (uint32_t)COM_Init->StopBits;
    huart->Init.HwFlowCtl    = (uint32_t)COM_Init->HwFlowCtl;
    huart->Init.OverSampling = UART_OVERSAMPLING_8;

    return HAL_UART_Init(huart);
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default COM Msp Callbacks
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
    int32_t ret = BSP_ERROR_NONE;

    if(COM >= COMn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

        /* Register default MspInit/MspDeInit Callback */
        if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, COM1_MspInit) != HAL_OK)
        {
            ret = BSP_ERROR_PERIPH_FAILURE;
        }
        else if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, COM1_MspDeInit) != HAL_OK)
        {
            ret = BSP_ERROR_PERIPH_FAILURE;
        }
        else
        {
            IsComMspCbValid[COM] = 1U;
        }
    }

    /* BSP status */
    return ret;
}

/**
  * @brief Register COM Msp Callback registering
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @param Callbacks     pointer to COM1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM , BSP_COM_Cb_t *Callback)
{
    int32_t ret = BSP_ERROR_NONE;

    if(COM >= COMn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

        /* Register MspInit/MspDeInit Callbacks */
        if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
        {
            ret = BSP_ERROR_PERIPH_FAILURE;
        }
        else if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
        {
            ret = BSP_ERROR_PERIPH_FAILURE;
        }
        else
        {
            IsComMspCbValid[COM] = 1U;
        }
    }

    return ret;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG > 0)
/**
  * @brief  Select the active COM port.
  * @param  COM COM port to be activated.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{
    if(COM_ActiveLogPort != COM)
    {
        COM_ActiveLogPort = COM;
    }
    
    return BSP_ERROR_NONE;
}

/**
  * @brief  Redirect console output to COM
  */
#ifdef __GNUC__
int __io_putchar (int ch)
#else
int fputc (int ch, FILE *f)
#endif /* __GNUC__ */
{
    (void) HAL_UART_Transmit(&hcom_uart [COM_ActiveLogPort], (uint8_t *) &ch, 1, COM_POLL_TIMEOUT);
    return ch;
}
#endif /* USE_COM_LOG */
#endif /* (USE_BSP_COM_FEATURE > 0) */


#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief  Initializes COM1 MSP.
  * @param  huart UART handle
  * @retval BSP status
  */
static void COM1_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_structure;

    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);

    /* Enable GPIO clock */
    COM1_TX_GPIO_CLK_ENABLE();
    COM1_RX_GPIO_CLK_ENABLE();

    /* Enable USART clock */
    COM1_CLK_ENABLE();

    /* Configure USART Tx as alternate function */
    gpio_init_structure.Pin       = COM1_TX_PIN;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    gpio_init_structure.Alternate = COM1_TX_AF;
    HAL_GPIO_Init(COM1_TX_GPIO_PORT, &gpio_init_structure);

    /* Configure USART Rx as alternate function */
    gpio_init_structure.Pin = COM1_RX_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Alternate = COM1_RX_AF;
    HAL_GPIO_Init(COM1_RX_GPIO_PORT, &gpio_init_structure);
}

/**
  * @brief  DeInitialize COM1 MSP part
  * @param  huart UART handle
  * @retval BSP status
  */
static void COM1_MspDeInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_structure;

    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);

    /* COM GPIO pin configuration */
    gpio_init_structure.Pin  = COM1_TX_PIN;
    HAL_GPIO_DeInit(COM1_TX_GPIO_PORT, gpio_init_structure.Pin);

    gpio_init_structure.Pin  = COM1_RX_PIN;
    HAL_GPIO_DeInit(COM1_RX_GPIO_PORT, gpio_init_structure.Pin);

    /* Disable USART clock */
    COM1_CLK_DISABLE();
}
#endif /* (USE_BSP_COM_FEATURE > 0) */