#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "xPower_inc.h"
   
typedef enum
{
    BUTTON_USER = 0U,
    BUTTONn
}Button_TypeDef;

typedef enum
{
    BUTTON_MODE_GPIO = 0U,
    BUTTON_MODE_EXTI = 1U
}ButtonMode_TypeDef;  
   
   
/* Button state */
#define BUTTON_RELEASED                       0U
#define BUTTON_PRESSED                        1U

/**
  * @brief User push-button
  */
#define USER_BUTTON_PIN                       GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                 GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn                 EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE                 EXTI_LINE_13
   
int32_t  BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t  BSP_PB_DeInit(Button_TypeDef Button);
int32_t  BSP_PB_GetState(Button_TypeDef Button);
void     BSP_PB_Callback(Button_TypeDef Button);
void     BSP_PB_IRQHandler(Button_TypeDef Button);  
   
   
#ifdef __cplusplus
}
#endif

#endif /* STM32G4XX_NUCLEO_H */