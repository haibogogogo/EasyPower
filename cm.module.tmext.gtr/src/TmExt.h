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
   @file TmExt.h
   @brief Timer Extension Interfaces
***************************************************************************************************/
#ifndef TMEXT_H
#define TMEXT_H

/**
* @brief     This method is used to initialize TmExt module
*/
extern void              TmExt_Init                ( void );

/**
* @brief     This method is used to deinitialize TmExt module
*/
extern void              TmExt_DeInit              ( void );

/**
* @brief     This method is used to deactivate TmExt module
*
* @return    E_OK-deactivated
*/
extern Std_ReturnType    TmExt_Sleep               ( void );

/**
* @brief     This method is used to activate TmExt module
*
* @return    Std_ReturnType E_NOT_READY-Not Activated, E_OK-Activated
*/
extern Std_ReturnType    TmExt_WakeUp              ( void );

#endif  // TMEXT_H

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1116694
Change Description: doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

