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
/*!*************************************************************************************************
*  @file Led.c
*  @brief Led Initialization, De-initialization, Activtion & Deactivation
***************************************************************************************************/

/**********************************************************************************************************************
*  Include Files                                                                                                      *
*********************************************************************************************************************/
#include "cmsis_os.h"
#include "led.h"

extern osTimerId LEDTimerHandle;;
/** @defgroup STM32G4XX_NUCLEO_LOW_LEVEL_Private_Variables LOW LEVEL Private Variables
  * @{
  */
static GPIO_TypeDef*   LED_PORT[LEDn] = {LED_NORMAL_GPIO_PORT,LED_ERROR_GPIO_PORT};

static const uint16_t  LED_PIN[LEDn] = {LED_NORMAL_PIN,LED_ERROR_PIN};

/**
  * @brief  Configures LED GPIO.
  * @param  Led Specifies the Led to be configured.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval BSP status
  */
int32_t BSP_LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef  gpio_init_structure;

    /* Enable the GPIO LED Clock */
    LED_NORMAL_GPIO_CLK_ENABLE();
    LED_ERROR_GPIO_CLK_ENABLE(); 
    /* Configure the GPIO_LED pin */
    gpio_init_structure.Pin   = LED_PIN[Led];
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(LED_PORT[Led], &gpio_init_structure);
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);

    return BSP_ERROR_NONE;
}

/**
  * @brief  DeInit LEDs.
  * @param  Led Specifies the Led to be de-init.
  *         This parameter can be one of the following values:
  *           @arg  LED2
  * @note Led DeInit does not disable the GPIO clock
  * @retval BSP status
  */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
    /* Turn off LED */
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);

    /* DeInit the GPIO_LED pin */
    HAL_GPIO_DeInit(LED_PORT[Led], LED_PIN[Led]);

    return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval BSP status
  */
int32_t BSP_LED_On(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);

    return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval BSP status
  */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);

    return BSP_ERROR_NONE;
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval BSP status
  */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
    HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);

    return BSP_ERROR_NONE;
}

/**
  * @brief  Get the status of the selected LED.
  * @param  Led Specifies the Led to get its state.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval LED status
  */
int32_t BSP_LED_GetState(Led_TypeDef Led)
{
    return (int32_t)HAL_GPIO_ReadPin(LED_PORT[Led], LED_PIN[Led]);
}

/* USER CODE BEGIN Header_ToggleLEDsThread */
/**
  * @brief  Function implementing the LEDThread thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ToggleLEDsThread */
void ToggleLEDsThread(void const * argument)
{
    (void) argument;
    uint32_t count = 0;
    /* Infinite loop */
    for(;;)
    {
        osTimerStop(LEDTimerHandle);
        count = osKernelSysTick() + 5000;

        BSP_LED_On(LED_NORMAL);
        BSP_LED_On(LED_ERROR);
        while (count > osKernelSysTick())
        {
            /* Toggle LED2 every 400ms*/
            osDelay(400);
            BSP_LED_Toggle(LED_NORMAL);
            BSP_LED_Toggle(LED_ERROR);
        }

        BSP_LED_Off(LED_NORMAL);
        BSP_LED_Off(LED_ERROR);
        osTimerStart(LEDTimerHandle, 200);

        osThreadSuspend(NULL);
    }
}
