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

/* Includes ------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G4XX_NUCLEO_H
#define STM32G4XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "xPower_inc.h"

#if !defined (USE_NUCLEO_32) && !defined (USE_NUCLEO_64)
#error "Board Pin number not defined"
#endif

/**
  * @brief STM32G4XX_NUCLEO BSP Driver version number
  */
#define STM32G4XX_NUCLEO_BSP_VERSION_MAIN   0x01U /*!< [31:24] main version */
#define STM32G4XX_NUCLEO_BSP_VERSION_SUB1   0x00U /*!< [23:16] sub1 version */
#define STM32G4XX_NUCLEO_BSP_VERSION_SUB2   0x00U /*!< [15:8]  sub2 version */
#define STM32G4XX_NUCLEO_BSP_VERSION_RC     0x00U /*!< [7:0]  release candidate */
#define STM32G4XX_NUCLEO_BSP_VERSION        ((STM32G4XX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                            |(STM32G4XX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                            |(STM32G4XX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                            |(STM32G4XX_NUCLEO_BSP_VERSION_RC))

/**
  * @brief Define for STM32G4XX_NUCLEO board
  */
#if !defined (USE_STM32G4XX_NUCLEO)
 #define USE_STM32G4XX_NUCLEO
#endif

#define STM32G4XX_NUCLEO_BSP_BOARD_NAME  "STM32G474-XPOWER"
#define STM32G4XX_NUCLEO_BSP_BOARD_ID    "MB1367"


/** @addtogroup STM32G4XX_NUCLEO_LOW_LEVEL_Exported_Variables
  * @{
  */
#if defined (USE_NUCLEO_64)
extern EXTI_HandleTypeDef hpb_exti[];
#endif /* USE_NUCLEO_64 */
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
  * @}
  */

/** @defgroup STM32G4XX_NUCLEO_LOW_LEVEL_Exported_Functions LOW LEVEL Exported Functions
  * @{
  */
int32_t  BSP_GetVersion(void);
const uint8_t* BSP_GetBoardName(void);
const uint8_t* BSP_GetBoardID(void);

/* Legacy APIs aliases */
#define BSP_GetHWBoardVersionName    BSP_GetBoardName
#define BSP_GetPDTypeName            BSP_GetBoardID

#ifdef __cplusplus
}
#endif

#endif /* STM32G4XX_NUCLEO_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
