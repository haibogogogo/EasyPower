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
   @defgroup TmExt_Pulse
   @brief Provides synchronization with different pulse channels along with callback notifications.
   @ingroup TmExt_Pulse
   @file TmExt_Pulse.h
   @brief TmExt_Pulse Interfaces along with structure definitions.
***************************************************************************************************/

#ifndef TMEXT_PULSE_H
#define TMEXT_PULSE_H

/***************************************************************************************************
*    P U B L I C   T Y P E   D E F I N I T I O N S                                                 *
***************************************************************************************************/
/**
* @brief This structure defines the attributes of pattern & start phase of the pulse channel
*/
typedef struct 
{
    VCONST uint32 * VCONST pPattern; /**< Pointer to array in flash with description of generated channel */
    uint8 StartPhase;        /**<  Channel generation starts in this state */

}STmExtChannelConfigType_t;

/**
* @brief defines the STmExtChannelConfigType_t structure type
*/
typedef STmExtChannelConfigType_t STmExtChannelConfigType;

/**
* @brief defines the callback notification function pointer type
*/
typedef void (* p_FClientNotificationFunction)(const SPulseNotification* Notification);

/**
* @brief This structure defines list of attributes of a client for a particular pulse channel
*/

typedef struct 
{
    EPulsePhase StartPhase;
    EPulsePhase EndPhase;
    boolean     RequiresNotification;
    boolean     HasToStart;
    boolean     HasToEnd;
}STmExtClient_t;

/**
* @brief defines the STmExtClient_t structure type
*/
typedef STmExtClient_t STmExtClient; 
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
/**
* @brief     This function is used to get a channel pattern related configuration parameters of a channel.
*
* @param[in] *TPulseChannelId - Channel Id
*
* @return    STmExtChannelConfigType - Return the requested channl's configuration parameters
*/
const STmExtChannelConfigType* TmExt_GetChannelCfgTable(const TPulseChannelId ChannelId);

/**
* @brief     This function is used to get a configured notification function of a client.
*
* @param[in] *TPulseClientId - Client Id
*
* @return    p_FClientNotificationFunction - Return the requested client's configured callback function
*/
p_FClientNotificationFunction TmExt_GetClientFunctionCfgTable(const TPulseClientId ClientId);

/**
* @brief     This method is used to initialize Transitional state in TmExt_Pulse
*/
extern void              TmExt_Pulse_Init   ( void );

/**
* @brief     This method is used to deinitialize Transitional state in TmExt_Pulse
*/
extern void              TmExt_Pulse_DeInit ( void );

/**
* @brief     This method is used to activate Transitional state in TmExt_Pulse
*
* @return    E_OK - deactivated
*/
extern Std_ReturnType    TmExt_Pulse_Sleep  ( void );

/**
* @brief     This method is used to activate Transitional state in TmExt_Pulse
*
* @return    E_NOT_READY-Not Activated, E_OK-Activated
*/
extern Std_ReturnType    TmExt_Pulse_WakeUp ( void );

/**
* @brief     This method is used to request the notification on a particular channel
*
* @param[in] Client - Id of the component making the request.
* @param[in] *Notification - Contains the specifics of the request.
* @param[in] Sync: request for synchronization with another channel
*/
extern void CTmExt_Impl_EnableNotification( TPulseClientId Client, const SPulseNotification *Notification, const SPulseNotification *Sync );

/**
* @brief     This method is used to stop the notification on a particular channel
*
* @param[in] Client - Id of the component making the request.
* @param[in] *Notification - Contains the specifics of the request.
*/
extern void CTmExt_Impl_DisableNotification( TPulseClientId Client, const SPulseNotification *Notification );

#endif  // TMEXT_PULSE_H

/*---------------------------------------------------------------------------
Date              : 11-May-2018
CDSID             : asivalin
Traceability      : RTC-1109220, 1199221, 1116694
Change Description: Timer Elapsed Defectes Fix doxygen documentation.
Version           : 3:turing_bsw_tmext_0.2
-----------------------------------------------------------------------------*/

