#ifndef LED_H
#define LED_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "xPower_inc.h"
   
   
typedef enum
{
    LED_NORMAL = 0U,
    LED_ERROR = 1U ,
    LEDn
}Led_TypeDef;


/** @defgroup STM32G4XX_NUCLEO_LOW_LEVEL_LED LOW LEVEL LED
  * @{
  */
#define LED_NORMAL_PIN                        GPIO_PIN_5
#define LED_NORMAL_GPIO_PORT                  GPIOB
#define LED_ERROR_PIN                         GPIO_PIN_6
#define LED_ERROR_GPIO_PORT                   GPIOB
#define LED_NORMAL_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_NORMAL_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOB_CLK_DISABLE()
#define LED_ERROR_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_ERROR_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOB_CLK_DISABLE()
/** @defgroup STM32G4XX_NUCLEO_LOW_LEVEL_Private_Variables LOW LEVEL Private Variables
  * @{
  */

int32_t  BSP_LED_Init(Led_TypeDef Led);
int32_t  BSP_LED_DeInit(Led_TypeDef Led);
int32_t  BSP_LED_On(Led_TypeDef Led);
int32_t  BSP_LED_Off(Led_TypeDef Led);
int32_t  BSP_LED_Toggle(Led_TypeDef Led);
int32_t  BSP_LED_GetState (Led_TypeDef Led);
void ToggleLEDsThread(void const * argument);


#ifdef __cplusplus
}
#endif

#endif /* STM32G4XX_NUCLEO_H */