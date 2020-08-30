//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2015] Visteon Corporation
// All Rights Reserved.
//
// NOTICE: This is an unpublished work of authorship, which contains trade secrets.
// Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
// its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
// or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
// in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
// under all copyright laws to protect this work as a published work, when appropriate.
// Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
// without the written authorization of Visteon Corporation.
//
//---------------------------------------------------------------------------------------------------------------------

#include "Std_Types.h"
#include "AssertLib.h"

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  This function allows to detect assertion failure.
///
/// @param Condition:   If this is false infinity loop is observed.
//---------------------------------------------------------------------------------------------------------------------
#ifdef DEBUG_ASSERT_ON
void DebugAssert( const boolean Condition )
{
    // You can set a BreakPoint here.
    if ( (boolean)FALSE == Condition )
    {
        // polyspace<RTE:NTL> Only in Debug. Infinite loop in order to detect assertion failure.
        for(;;){} // Infinity loop cannot be tested with Vector Cast
    }
}
#endif //DEBUG_ASSERT_ON
