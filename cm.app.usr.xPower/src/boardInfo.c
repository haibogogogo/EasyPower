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
#include "boardinfo.h"

/**
  * @brief  This method returns the STM32G4xx NUCLEO BSP Driver revision
  * @retval version 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_GetVersion(void)
{
    return (int32_t)STM32G4XX_NUCLEO_BSP_VERSION;
}

/**
  * @brief  This method returns the board name
  * @retval pointer to the board name string
  */
const uint8_t* BSP_GetBoardName(void)
{
    return (const uint8_t*)STM32G4XX_NUCLEO_BSP_BOARD_NAME;
}

/**
  * @brief  This method returns the board ID
  * @retval pointer to the board ID string
  */
const uint8_t* BSP_GetBoardID(void)
{
    return (const uint8_t*)STM32G4XX_NUCLEO_BSP_BOARD_ID;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
