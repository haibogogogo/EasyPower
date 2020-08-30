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
*  @file TmExt.c
*  @brief Timer Extension Initialization, De-initialization, Activtion & Deactivation
***************************************************************************************************/

/**********************************************************************************************************************
*  Include Files                                                                                                      *
*********************************************************************************************************************/
#include "Rte_TmExt.h"
#include "TmExt_Pulse.h"
#include "TmExt_Timer.h"

/*!********************************************************************************************************************

 @fn        TmExt_Init

 @brief     This method is used to initialize TmExt module

 @param[in] None

 @return    None

**********************************************************************************************************************/
void TmExt_Init( void )
{
    TmExt_Pulse_Init();
    TmExt_Timer_Init();
}

/*!********************************************************************************************************************

 @fn        TmExt_DeInit

 @brief     This method is used to deinitialize TmExt module

 @param[in] None

 @return    None

**********************************************************************************************************************/
void TmExt_DeInit( void )
{
    TmExt_Pulse_DeInit();
}

/*!********************************************************************************************************************

@fn        TmExt_Sleep

@brief     This method is used to deactivate TmExt module

@param[in] None

@return    E_OK-deactivated

**********************************************************************************************************************/
Std_ReturnType TmExt_Sleep( void )
{
    return TmExt_Pulse_Sleep();
}

/*!********************************************************************************************************************

@fn        TmExt_WakeUp

@brief     This method is used to activate TmExt module

@param[in] None

@return    Std_ReturnType E_NOT_READY-Not Activated, E_OK-Activated

**********************************************************************************************************************/
Std_ReturnType TmExt_WakeUp( void )
{
    return TmExt_Pulse_WakeUp();
}

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1116694
Change Description: doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

