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
/** @defgroup STM32G4XX_NUCLEO_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Variables
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "pushbutton.h"

EXTI_HandleTypeDef hpb_exti[BUTTONn];


typedef void (* BSP_EXTI_LineCallback) (void);

static GPIO_TypeDef*   BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT};

static const uint16_t  BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN};

static const IRQn_Type BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn};

static void BUTTON_USER_EXTI_Callback(void);

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button Button to be configured.
  *         This parameter can be one of the following values:
  *           @arg  BUTTON_USER: User Push Button
  * @param  ButtonMode Button mode.
  *         This parameter can be one of following parameters:
  *           @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *           @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line
  *                                  with interrupt generation capability
  * @retval BSP status
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
    GPIO_InitTypeDef gpio_init_structure;
    static BSP_EXTI_LineCallback ButtonCallback[BUTTONn] = {BUTTON_USER_EXTI_Callback};
    static uint32_t BSP_BUTTON_PRIO [BUTTONn] = {BSP_BUTTON_USER_IT_PRIORITY};
    static const uint32_t BUTTON_EXTI_LINE[BUTTONn] = {USER_BUTTON_EXTI_LINE};

    /* Enable the BUTTON Clock */
    USER_BUTTON_GPIO_CLK_ENABLE();

    gpio_init_structure.Pin = BUTTON_PIN[Button];
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    if(ButtonMode == BUTTON_MODE_GPIO)
    {
        /* Configure Button pin as input */
        gpio_init_structure.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
    }
    else /* (ButtonMode == BUTTON_MODE_EXTI) */
    {
        /* Configure Button pin as input with External interrupt */
        gpio_init_structure.Mode = GPIO_MODE_IT_RISING;

        HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);

        (void)HAL_EXTI_GetHandle(&hpb_exti[Button], BUTTON_EXTI_LINE[Button]);
        (void)HAL_EXTI_RegisterCallback(&hpb_exti[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
        HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
    }

    return BSP_ERROR_NONE;
}

/**
  * @brief  Push Button DeInit.
  * @param  Button Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  * @note PB DeInit does not disable the GPIO clock
  * @retval BSP status
  */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
    HAL_NVIC_DisableIRQ((BUTTON_IRQn[Button]));
    HAL_GPIO_DeInit(BUTTON_PORT[Button], BUTTON_PIN[Button]);

    return BSP_ERROR_NONE;
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button Button to be checked.
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  * @retval The Button GPIO pin value
  */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
    return (int32_t)HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  This function handles Push-Button interrupt requests.
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_IRQHandler(Button_TypeDef Button)
{
    HAL_EXTI_IRQHandler(&hpb_exti[Button]);
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(Button);

    /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */

    
    /**
  * @brief  Key EXTI line detection callbacks.
  * @retval BSP status
  */
}

static void BUTTON_USER_EXTI_Callback(void)
{
    BSP_PB_Callback(BUTTON_USER);
}

