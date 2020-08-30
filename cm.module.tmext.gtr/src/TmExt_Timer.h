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
   @defgroup TmExt_Timer
   @brief provides Timer Start, Stop, ElapsedTime & Restart related functionalities.
   @ingroup TmExt_Timer
   @file TmExt_Timer.h
   @brief TmExt_Timer Interfaces.
***************************************************************************************************/

#ifndef TMEXT_TIMER_H
#define TMEXT_TIMER_H


typedef uint8 HTimer;
/**
* @brief 	This method Initialize TmExt module.
*/
extern void          TmExt_Timer_Init                 ( void );

/**
* @brief  This method is used to start a timer
*
* @param hTimer - Timer Id
*/
extern void          TmExt_Timer_Start                ( const HTimer hTimer );

/**
* @brief  This method is used to stop a timer
*
* @param hTimer - Timer Id
*/
extern void          TmExt_Timer_Stop                 ( const HTimer hTimer );

/**
* @brief  This method is used to check whether Timer has already started
*
* @param hTimer - Timer Id
* @param *IsStarted - Pointer to get the timer started status
*/
extern void          TmExt_Timer_IsStarted            ( const HTimer hTimer, boolean* IsStarted );

/**
* @brief  This method is used to check whether Timer has elapsed by the specified timeout period
*
* @param hTimer - Timer Id
* @param Timeout - Timeout in ms
* @param IsElapsed - Pointer to get the timer elapsed status
*/
extern void          TmExt_Timer_IsElapsed            ( const HTimer hTimer, const uint32 Timeout, boolean* IsElapsed );

/**
* @brief  This method is used to check whether Timer has restarted after the specified timeout elapsed
*
* @param hTimer - Timer Id
* @param Timeout - Timeout in ms
* @param IsElapsed - Pointer to get the timer elapsed restarted status
*/
extern void          TmExt_Timer_IsElapsedRestart     ( const HTimer hTimer, const uint32 Timeout, boolean* IsElapsed );

/**
* @brief  This method is used to get the elapsed timeout period in milliseconds from the timer starting period
*
* @param hTimer - Timer Id
* @param ElapsedTime - Pointer to get the Elapsed Timeout period in milliseconds
*/
extern void          TmExt_Timer_GetElapsedTime       ( const HTimer hTimer, uint32* ElapsedTime );

/**
* @brief  This method is used to actively wait a predetermined time in nano seconds
* @brief  Minimal waiting time is 1ns
* @brief  Minimal increment of waiting time is 500 ns. Source execution takes approximately 1 us.
*
* @param hTimer - Timer Id
* @param ElapsedTime - Pointer to get the Elapsed Timeout period in milliseconds
*/
extern void          TmExt_BusyWait1us                ( const uint32 WaitingTimeMin );

/**
* @brief 	This method checks and update the timer expired status periodically for all the running timers.
*/
extern void          TmExt_Timer_UpdateExpiredStatus  (void);

#endif  // TMEXT_TIMER_H

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1109220, 1199221, 1116694
Change Description: Timer Elapsed Defectes Fix & doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

