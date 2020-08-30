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

/*!*******************************************************************************************************************
* @file TmExt_Pulse.c
* @brief Implementation of Pulse synchronization related functionalities along with enable/disable callback notifications to client.
* *******************************************************************************************************************/

/**********************************************************************************************************************
*  Include Files                                                                                                      *
*********************************************************************************************************************/
#include "Rte_TmExt.h"
#include "TmExt_Pulse.h"
#include "TmExt_Cfg.h"
#include "MemLib.h"
#include "TmExt_Timer.h"

/**********************************************************************************************************************
*  Private Macros Definitions                                                                                         *                                                                                       *
**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------
// Masks to manage field Restart of SChannelStatus
//----------------------------------------------------------------------------------------------------------------------
/**
* @brief Restart is requested
*/
#define cRESTART_COMMON         0x01u

/**
* @brief Restart should happen synchronously with another channel
*/
#define cRESTART_WITH_SYNC      0x02u
//bit 0: Phase to be notified on:  0:down; 1:up
//bit 1: First notification:       0:off; 1:on
//bit 2: Last notification:        0:off; 1:on
//bit 3: Notification enabled:     0:off; 1:on

#define cMASK_PHASE                         0x01u
#define cMASK_FIRST_NOTIFICATION            0x02u
#define cMASK_LAST_NOTIFICATION             0x04u
#define cMASK_ENABLE_NOTIFICATION           0x08u

#define cSTATE_DEINIT                       0x0u
#define cSTATE_SLEEP                        0x1u
#define cSTATE_OPERATIONAL                  0x2u

//----------------------------------------------------------------------------------------------------------------------
// Channel runtime status
//----------------------------------------------------------------------------------------------------------------------
/**
* @brief This structure defines list of attributes for channel runtime status
*/
typedef struct 
{
    SPulseNotification ChannelSync;
    uint32 Counter;                                 /**< Time measurement counter */
    uint32 Ix;                                      /**< Index in pattern. PatternSize is uint32, so this member is pushed to uint32 */
    uint8 ClientStatusArray[eClientCount];          /**< Keeps information of the clients who want to be notified */
    uint8 CurrentPhase;                             /**< 1 -> UP, 0 -> DOWN */
    uint8 Restart;                                  /**< Management of channel restart */
    uint8 ClientCount;                              /**< Number of active clients. Zero is stopped channel */
} SChannelStatus_t;
typedef SChannelStatus_t SChannelStatus;

/**
* @brief used to invert current channel state
*/
#define cPHASE_SWITCH                0x01u

/**********************************************************************************************************************
*  Private Variable Definitions                                                                                       *
**********************************************************************************************************************/
static SChannelStatus ChannelStatusTable[ePulseChannelCount];

static uint8 ComponentState = cSTATE_DEINIT;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

/**
* @brief     This method is used to remove a client from a channel.
*
* @param[in] Channel - Id of the channel
* @param[in] Client - Id of the client
*/
static void RemoveClient(const TPulseChannelId Channel, const TPulseClientId Client);

/**
* @brief     This method is used to start a channel.
*
* @param[in] Channel - Id of the channel
* @param[in] Sync - Synchronization with another channel
*/
static void StartChannel(const TPulseChannelId Channel, const SPulseNotification * const Sync);

/**
* @brief     This method is used to synchronize the blinking of a channel to another channel.
*
* @param[in] Channel - Index of the channel to synchronize
*/
static void SyncChannels(const TPulseChannelId Channel);

/**
* @brief     This method is used to notify the clients of a specific channel.
*
* @param[in] Channel - Id of the channel which issues the notification
* @param[in] NewPhase - Current phase of the channel
*/
static void NotifyClient(const TPulseChannelId Channel, const uint8 NewPhase);

/*!********************************************************************************************************************

 @fn        TmExt_Pulse_Init

 @brief     This method is used to initialize Transitional state in TmExt_Pulse

 @param[in] None

 @return    None

**********************************************************************************************************************/
void TmExt_Pulse_Init( void )
{
    ComponentState = cSTATE_SLEEP;
}

/*!********************************************************************************************************************

 @fn        TmExt_Pulse_DeInit

 @brief     This method is used to deinitialize Transitional state in TmExt_Pulse

 @param[in] None

 @return    None

**********************************************************************************************************************/
void TmExt_Pulse_DeInit( void )
{
    ComponentState = cSTATE_DEINIT;
}


/*!********************************************************************************************************************

 @fn        TmExt_Pulse_WakeUp

 @brief     This method is used to activate Transitional state in TmExt_Pulse

 @param[in] None

 @return    Std_ReturnType E_NOT_READY-Not Activated, E_OK-Activated

**********************************************************************************************************************/
Std_ReturnType TmExt_Pulse_WakeUp( void )
{
    Std_ReturnType Result = E_NOT_READY;
    SChannelStatus *pChannelStatus;
    TPulseChannelId ChannelId;

    if( cSTATE_SLEEP == ComponentState )
    {
        for( ChannelId = (TPulseChannelId)0; ChannelId < ePulseChannelCount ; ChannelId++)
        {
            pChannelStatus = &ChannelStatusTable[ChannelId];

            (void)MemLib_MemSet( pChannelStatus->ClientStatusArray, 0U, sizeof( pChannelStatus->ClientStatusArray ) );

            pChannelStatus->Restart = 0U;         // No restart request
            pChannelStatus->ChannelSync.Phase = ePulsePhase_Off;
            pChannelStatus->ChannelSync.Channel = ePulseChannelCount;
            pChannelStatus->Counter = 0U;
            pChannelStatus->Ix = 0U;
            pChannelStatus->CurrentPhase = 0U;
            pChannelStatus->ClientCount = 0U;
        }

        Result = E_OK;
        ComponentState = cSTATE_OPERATIONAL;
    }
    return Result;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
/*!********************************************************************************************************************

 @fn        TmExt_Pulse_Sleep

 @brief     This method is used to deactivate Transitional state in TmExt_Pulse

 @param[in] None

 @return    E_OK-deactivated

**********************************************************************************************************************/
Std_ReturnType TmExt_Pulse_Sleep( void )
{
    ComponentState = cSTATE_SLEEP;
    return E_OK;
}

/*!********************************************************************************************************************

 @fn        TmExt_MainFunction

 @brief     Periodic function of TmExt

 @param[in] None

 @return    None

**********************************************************************************************************************/
void TmExt_MainFunction( void )
{
    SChannelStatus * pChnStatus;
    const STmExtChannelConfigType * pChnConfig;
    TPulseChannelId ChannelId;
    uint8 NewPhase;

    if( cSTATE_OPERATIONAL == ComponentState )
    {
        // Pointers avoid too many indexations
        for( ChannelId = (TPulseChannelId)0; ChannelId < ePulseChannelCount ; ChannelId++)
        {
            pChnStatus = &ChannelStatusTable[ChannelId];
            pChnConfig = TmExt_GetChannelCfgTable(ChannelId);

            // Drive channels only in case of active notifications
            if( ( 0U < pChnStatus->ClientCount ) )
            {

                if( 0U != ( cRESTART_COMMON & pChnStatus->Restart ) )
                {
                    if( 0U != (cRESTART_WITH_SYNC & pChnStatus->Restart)  )
                    {
                        SyncChannels(ChannelId);
                    }
                    else
                    {
                        pChnStatus->Restart = 0U;
                    }

                    if( 0U == pChnStatus->Restart )
                    {
                        pChnStatus->Counter = (uint32)1;    // Push phase triggering mechanism to react immediately
                        // Put index at last sample of the pattern, so triggering mechanism will switch to the beginning
                        pChnStatus->Ix = *( pChnConfig->pPattern );
                        // To notify on channel start.
                        pChnStatus->CurrentPhase = ( cPHASE_SWITCH ^ pChnConfig->StartPhase );
                    }
                    else
                    {
                        continue;
                    }

                }

                // Triggering mechanism
                pChnStatus->Counter--;          // Samples counter
                if( (uint32)0x00u == pChnStatus->Counter )
                {
                    pChnStatus->Ix++;        // Go to next phase

                    // Check for roll-over of the pattern sequence
                    if( pChnStatus->Ix > *( pChnConfig->pPattern ) )
                    {
                        // Repeat the pattern
                        pChnStatus->Ix = (uint32)0x01u;           // Patterns start at position 1
                        NewPhase = pChnConfig->StartPhase;    // Start phase of the pattern
                    }
                    else
                    {
                        NewPhase = (uint8)( cPHASE_SWITCH ^ pChnStatus->CurrentPhase );    // Invert current phase
                    }

                    // Check for edges
                    if( NewPhase != pChnStatus->CurrentPhase )
                    {
                        NotifyClient( ChannelId, NewPhase );
                    }

                    pChnStatus->CurrentPhase = NewPhase;

                    // Reload samples counter with new value
                    pChnStatus->Counter = pChnConfig->pPattern[pChnStatus->Ix];

                }
            }
        }
        TmExt_Timer_UpdateExpiredStatus();
    }
}

/*!********************************************************************************************************************

 @fn        CTmExt_Impl_DisableNotification

 @brief     This method is used to stop the notification on a particular channel

 @param[in] Client - Id of the component making the request
 @param[in] *Notification - Contains the specifics of the request.

 @return    None

**********************************************************************************************************************/
void CTmExt_Impl_DisableNotification( TPulseClientId Client, const SPulseNotification *Notification )
{
    SChannelStatus * pChnStatus;
    uint8 ClientNotificationFlag;
    uint8 EndPhase = 0U;

    if( cSTATE_OPERATIONAL == ComponentState )
    {
        if( ePulseChannelCount > Notification->Channel )
        {
            pChnStatus = &ChannelStatusTable[Notification->Channel];
            if( eClientCount > Client )
            {
                ClientNotificationFlag = pChnStatus->ClientStatusArray[Client];

                if( ( 0U != ( ClientNotificationFlag & cMASK_ENABLE_NOTIFICATION ) ) )
                {
                    if( (ePulsePhase_Any == Notification->Phase ) || ( 0U != ( cRESTART_COMMON & pChnStatus->Restart ) ) )
                    {
                        RemoveClient(Notification->Channel, Client);
                    }
                    else
                    {
                        if( ePulsePhase_On == Notification->Phase )
                        {
                            EndPhase = 1U;
                        }

                        pChnStatus->ClientStatusArray[Client] = ( cMASK_ENABLE_NOTIFICATION | cMASK_LAST_NOTIFICATION | EndPhase );
                    }
                }
            }
        }
    }
}

/*!********************************************************************************************************************

 @fn        CTmExt_Impl_EnableNotification

 @brief     This method is used to request the notification on a particular channel.

 @param[in] Client - Id of the component making the request.
 @param[in] *Notification - Contains the specifics of the request.
 @param[in] Sync: request for synchronization with another channel

 @return    None

**********************************************************************************************************************/
void CTmExt_Impl_EnableNotification( TPulseClientId Client, const SPulseNotification *Notification, const SPulseNotification *Sync )
{
    SChannelStatus * pChnStatus;
    uint8 ClientNotificationFlag;

    if( cSTATE_OPERATIONAL == ComponentState )
    {
        if( ePulseChannelCount > Notification->Channel )
        {
            pChnStatus = &ChannelStatusTable[Notification->Channel];
            if( eClientCount > Client )
            {
                ClientNotificationFlag = pChnStatus->ClientStatusArray[Client];

                if( ( 0U == ( ClientNotificationFlag & cMASK_ENABLE_NOTIFICATION ) ) ||
                    ( cMASK_LAST_NOTIFICATION == ( ClientNotificationFlag & cMASK_LAST_NOTIFICATION ) )
                  )
                {
                    if ( cMASK_ENABLE_NOTIFICATION == ( ClientNotificationFlag & cMASK_ENABLE_NOTIFICATION ) )
                    {
                        RemoveClient( Notification->Channel, Client);
                    }
                    
                    if( ePulsePhase_On == Notification->Phase )
                    {
                        ClientNotificationFlag = ( cMASK_ENABLE_NOTIFICATION | cMASK_FIRST_NOTIFICATION | cMASK_PHASE );
                    }
                    else if( ePulsePhase_Off == Notification->Phase )
                    {
                        ClientNotificationFlag = ( cMASK_ENABLE_NOTIFICATION | cMASK_FIRST_NOTIFICATION );
                    }
                    else
                    {
                        ClientNotificationFlag = cMASK_ENABLE_NOTIFICATION;
                    }

                    pChnStatus->ClientStatusArray[Client] = ClientNotificationFlag;
                    StartChannel( Notification->Channel, Sync );
                    pChnStatus->ClientCount++;
                }
            }
        }
    }
}

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
// UML::CTmExt_Pulse::CTmExt_Notification
//----------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

/*!********************************************************************************************************************

 @fn        SyncChannels

 @brief     This method is used to synchronize the blinking of a channel to another channel.

 @param[in] Channel - Index of the channel to synchronize

 @return    None

**********************************************************************************************************************/
static void SyncChannels( const TPulseChannelId Channel )
{
    SChannelStatus *pChnStatusToSync;
    SChannelStatus * pChnStatus;
    const STmExtChannelConfigType * pChnConfigToSync;
    uint8 ChannelSyncPhase = 0U;

    pChnStatus = &ChannelStatusTable[Channel];

    // No synchronization on "any phase"
    if( (ePulseChannelCount > pChnStatus->ChannelSync.Channel ) && (ePulsePhase_Any != pChnStatus->ChannelSync.Phase))
    {
        pChnStatusToSync = &ChannelStatusTable[pChnStatus->ChannelSync.Channel];


        if( ePulsePhase_On == pChnStatus->ChannelSync.Phase )
        {
            ChannelSyncPhase = cPHASE_SWITCH;
        }

        // Channels with smaller id will already be processed - their phase will be changed already
        if( pChnStatus->ChannelSync.Channel < Channel )
        {
            pChnConfigToSync = TmExt_GetChannelCfgTable( pChnStatus->ChannelSync.Channel );
            if( pChnConfigToSync->pPattern[pChnStatusToSync->Ix] == pChnStatusToSync->Counter )
            {
                if( ChannelSyncPhase == pChnStatusToSync->CurrentPhase )
                {
                    pChnStatus->Restart = 0U;
                }
            }
        }
        else if( pChnStatus->ChannelSync.Channel > Channel )
        {
            if( (uint32)1 == pChnStatusToSync->Counter )
            {
                if( ChannelSyncPhase != pChnStatusToSync->CurrentPhase )
                {
                    pChnStatus->Restart = 0U;
                }
            }
        }
        else
        {
            pChnStatus->Restart = 0U;
        }
    }
    else
    {
        pChnStatus->Restart = 0U;
    }
}
/*!********************************************************************************************************************

 @fn        NotifyClient

 @brief     This method is used to notify the clients of a specific channel.

 @param[in] Channel - Id of the channel which issues the notification
 @param[in] NewPhase - Current phase of the channel

 @return    None

**********************************************************************************************************************/
static void NotifyClient( const TPulseChannelId Channel, const uint8 NewPhase )
{
    TPulseClientId ClientId;
    p_FClientNotificationFunction pFunctionConfig;
    uint8 ClientNotificationFlag;
    SPulseNotification Notification;
    SChannelStatus * pChnStatus;

    pChnStatus = &ChannelStatusTable[Channel];
    for( ClientId = (TPulseClientId)0; ClientId < eClientCount ; ClientId++)
    {
        pFunctionConfig = NULL;

        ClientNotificationFlag = pChnStatus->ClientStatusArray[ClientId];

        if( ( 0U != ( ClientNotificationFlag & cMASK_ENABLE_NOTIFICATION ) ) )
        {
            if( 0U != ( ClientNotificationFlag & cMASK_FIRST_NOTIFICATION ) )
            {
                if( NewPhase == ( ClientNotificationFlag & cMASK_PHASE ) )
                {
                    pChnStatus->ClientStatusArray[ClientId] = cMASK_ENABLE_NOTIFICATION;
                    pFunctionConfig = TmExt_GetClientFunctionCfgTable( ClientId );
                }
            }
            else
            {
                if( 0U != ( ClientNotificationFlag & cMASK_LAST_NOTIFICATION ) )
                {
                    if( NewPhase == ( ClientNotificationFlag & cMASK_PHASE ) )
                    {
                        RemoveClient(Channel, ClientId);
                    }
                }

                pFunctionConfig = TmExt_GetClientFunctionCfgTable( ClientId );
            }
        }

        if( NULL != pFunctionConfig )
        {
            Notification.Channel = Channel;
            if( 0U == NewPhase )
            {
                Notification.Phase = ePulsePhase_Off;
            }
            else
            {
                Notification.Phase = ePulsePhase_On;
            }
            // polyspace<RTE:COR> Function pointer point to a valid function.
            pFunctionConfig( &Notification );
        }
    }
}

/*!********************************************************************************************************************

 @fn        RemoveClient

 @brief     This method is used to remove a client from a channel.

 @param[in] Channel - Id of the channel
 @param[in] Client - Id of the client

 @return    None

**********************************************************************************************************************/
static void RemoveClient(const TPulseChannelId Channel, const TPulseClientId Client)
{
    SChannelStatus * pChnStatus = &ChannelStatusTable[Channel];

    pChnStatus->ClientStatusArray[Client] = 0U;
    pChnStatus->ClientCount--;
}

/*!********************************************************************************************************************

 @fn        StartChannel

 @brief     This method is used to start a channel.

 @param[in] Channel - Id of the channel
 @param[in] Sync - Synchronization with another channel

 @return    None

**********************************************************************************************************************/
static void StartChannel(const TPulseChannelId Channel, const SPulseNotification * const Sync)
{
    SChannelStatus * pChnStatus = &ChannelStatusTable[Channel];

    if( 0U == pChnStatus->ClientCount )
    {
        pChnStatus->Restart = cRESTART_COMMON;
        if( NULL != Sync )
        {
            SChannelStatus * pChannelToSync = &ChannelStatusTable[Sync->Channel];

            //Synchronize only with already running channels
            if(0U != pChannelToSync->ClientCount)
            {
                pChnStatus->ChannelSync.Channel = Sync->Channel;
                pChnStatus->ChannelSync.Phase = Sync->Phase;
                pChnStatus->Restart = cRESTART_COMMON | cRESTART_WITH_SYNC;
            }
        }
        else
        {
            pChnStatus->ChannelSync.Channel = ePulseChannelCount;
        }
    }
}

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1109220, 1199221, 1116694
Change Description: Timer Elapsed Defectes Fix & QAC, Coverity Warnings Fix and doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

