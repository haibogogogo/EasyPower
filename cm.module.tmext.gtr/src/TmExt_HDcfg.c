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
* @file TmExt_HCfg.c
* @brief Timer Extension wait time implementation in nanosecond.
***************************************************************************************************/
/**********************************************************************************************************************
*  Include Files                                                                                                      *
*********************************************************************************************************************/
#include "Rte_TmExt.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"
#include "TmExt_Cfg.h"
#include "Gpt.h"

/*!********************************************************************************************************************

 @fn        TmExt_BusyWait1us

 @param[in] Time to wait in [us]

 @return    None

**********************************************************************************************************************/


void TmExt_BusyWait1us(const uint32 WaitingTimeMin )
{ 
	Gpt_ValueType  StartingTime = 0;
	uint32  TimeElapsed = 0;
	Gpt_ValueType  CurrentTime = 0;

	Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &StartingTime);
	while(TimeElapsed <= WaitingTimeMin)
	{
		Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT,&CurrentTime);
		TimeElapsed = (CurrentTime - StartingTime);
	}
}

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1116694
Change Description: doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

