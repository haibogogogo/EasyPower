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

#include <stdlib.h>
#include "Std_Types.h"
#include "MathLib.h"
#include "AssertLib.h"

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
/// @brief    Initialization of PT1 filter.
///
/// @param    pPt1Filter:     PT1 filter data structure.
/// @param    DampingConst:   Damping constant which will be set for the filter.
/// @param    InitValue:      Initial filtering value.
///
/// @return   Std_ReturnType:   E_OK     - PT1 filter is initialized
///                             E_NOT_OK - PT1 filter is NOT initialized, debug assert fails
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_PT1Filter_Init( SPt1FilterData * const pPt1Filter, const uint16 DampingConst, const uint16 InitValue )
{
    Std_ReturnType Ret;

    if ( NULL != pPt1Filter )
    {
        pPt1Filter->OutputValue = InitValue;
        pPt1Filter->Remainder = (uint16)0;
        pPt1Filter->DampingTime = DampingConst;
        pPt1Filter->Status = eFilterStatus_Initialized;

        Ret = E_OK;
    }
    else
    {
        DEBUG_ASSERT( FALSE );
        Ret = E_NOT_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used to apply a new value to the PT1 filter.
///           F(T) : Filter output at the time t = T
///           R(T-1) : Rest of the integer division (modulo ..) that was memorized at the occurrence of the last (t = T-1) calculation
///           N(T) : New current input value of the filter
///           W : Weight of the PT1 filter
///           TAU : Relax time (parameter of the PT1 filter)
///           Algorithm :  F(T) = (  W*F(T-1) + N(T) + R(T-1)  )  /  (W + 1)
///           W must be chosen so that  TAU = W * CallPeriodOfTheAlgorithm
///
/// @param    pPt1Filter:     PT1 filter data structure.
/// @param    ValueToFilter:  The new value which must be filtered.
///
/// @return   uint16:   Output value after filtering.
///
/// Note!   -   In case of invalid pointer, debug assert fails and return value is 0.
///             If pointer is valid but the filter is not initialized returned value is not changed.
//----------------------------------------------------------------------------------------------------------------------
uint16 MathLib_PT1Filter_Apply( SPt1FilterData * const pPt1Filter, const uint16 ValueToFilter )
{
    uint32 Temp;
    uint32 DmpTime;
    uint16 OutValue = (uint16)0;

    if ( NULL != pPt1Filter )
    {
        if ( (pPt1Filter->Status == eFilterStatus_Initialized) || (pPt1Filter->Status == eFilterStatus_Calculated) )
        {
            DmpTime = (uint32)pPt1Filter->DampingTime;

            Temp = (uint32)pPt1Filter->OutputValue * DmpTime;

            Temp += (uint32)ValueToFilter + (uint32)(pPt1Filter->Remainder);

            ++DmpTime;

            pPt1Filter->OutputValue = (uint16)(Temp / DmpTime);

            OutValue = pPt1Filter->OutputValue;

            pPt1Filter->Remainder = (uint16)(Temp % DmpTime);

            pPt1Filter->Status = eFilterStatus_Calculated;
        }
        else
        {
            DEBUG_ASSERT( FALSE );
            OutValue = pPt1Filter->OutputValue;
        }
    }
    else
    {
        DEBUG_ASSERT( FALSE );
    }

    return OutValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get status of PT1 filter.
///
/// @param    pPt1Filter:     PT1 filter data structure.
///
/// @return   Std_ReturnType:   E_OK        - when there is filtered value that can be used
///                             E_NOT_READY - if first value is not yet calculated
///                             E_NOT_OK    - if filter fails or not initialized
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_PT1Filter_GetStatus ( const SPt1FilterData * const pPt1Filter )
{
    Std_ReturnType Ret;

    if ( NULL != pPt1Filter )
    {
        if ( pPt1Filter->Status == eFilterStatus_Initialized )
        {
            Ret = E_NOT_READY;
        }
        else if ( pPt1Filter->Status == eFilterStatus_Calculated )
        {
            Ret = E_OK;
        }
        else
        {
            Ret = E_NOT_OK;
        }
    }
    else
    {
        Ret = E_NOT_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Initialization of a moving average filter.
///
/// @param    pFilterData:    Moving average filter data structure.
/// @param    pMArrayData:    Filtered array.
/// @param    MArrayLen:      Filtered array length.
/// @param    InitValue:      Initial value for the array.
///
/// @return   Std_ReturnType:   E_OK     - MoveAvgFilter is initialized
///                             E_NOT_OK - MoveAvgFilter is NOT initialized, debug assert fails
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_MoveAvgFilter_Init( SMoveAvgFilter * const pFilterData, uint16 * const pMArrayData, const uint16 MArrayLen, const uint16 InitValue )
{
    uint16 Ix;
    uint16 * pArray;
    Std_ReturnType Ret;

    if ( (NULL != pFilterData) && (NULL != pMArrayData) && (0U < MArrayLen) )
    {
        pFilterData->pMArray = pMArrayData;
        pFilterData->MArrayLen = MArrayLen;
        pFilterData->MArrayPos = (uint16)0;

        Ix = 0U;
        pArray = pFilterData->pMArray;

        while ( Ix < MArrayLen )
        {
            (*pArray) = InitValue;
            pArray = &pArray[1];
            ++Ix;
        }

        // polyspace<RTE:OVFL> There isn't a problem here. MArrayLen cannot be bigger than 65535.
        pFilterData->Sum = ((uint32)InitValue * (uint32)MArrayLen);
        pFilterData->Status = eFilterStatus_Initialized;

        Ret = E_OK;
    }
    else
    {
        DEBUG_ASSERT( FALSE );
        Ret = E_NOT_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used to apply a new value to the moving average filter.
///
/// @param    pFilterData:      Moving average filter data structure.
/// @param    ValueToFilter:    The new value to be filtered.
/// @param    pOutValue:        The output value after filtering.
///
/// @return   uint16:   Output value after filtering.
///
/// Note!   -   In case of invalid pointer (function arguments), debug assert fails and return value is 0
///             If pointer is valid but the filter is not initialized returned value is (pFilterData->Sum / pFilterData->MArrayLen)
//----------------------------------------------------------------------------------------------------------------------
uint16 MathLib_MoveAvgFilter_Apply( SMoveAvgFilter * const pFilterData, const uint16 ValueToFilter )
{
    uint16 * pArray;
    uint16 OutValue = 0U;

    if ( (NULL != pFilterData) && (NULL != pFilterData->pMArray) && (0U < pFilterData->MArrayLen) )
    {
        if ( (pFilterData->Status == eFilterStatus_Initialized) || (pFilterData->Status == eFilterStatus_Calculated) )
        {
            // add new value to sum of numbers in filter
            pFilterData->Sum += ValueToFilter;

            pArray = &pFilterData->pMArray[ pFilterData->MArrayPos ];

            // remove from sum oldest value
            // polyspace<RTE:IDP> If the client supplies the correct filter data there is no danger of failure here.
            pFilterData->Sum -= (*pArray);

            // add new value
            (*pArray) = ValueToFilter;

            // move to next position in array
            pFilterData->MArrayPos++;

            // if end of array is reached, start from beginning
            if ( pFilterData->MArrayPos >= pFilterData->MArrayLen )
            {
                pFilterData->MArrayPos = (uint16)0;
            }

            pFilterData->Status = eFilterStatus_Calculated;
        }
        else
        {
            DEBUG_ASSERT( FALSE );
        }

        // calculate average value
        OutValue = (uint16)( pFilterData->Sum / pFilterData->MArrayLen );
    }
    else
    {
        DEBUG_ASSERT( FALSE );
    }

    return OutValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get status of moving average filter.
///
/// @param    pFilterData:      Moving average filter data structure.
///
/// @return   Std_ReturnType:   E_OK        - when there is filtered value that can be used
///                             E_NOT_READY - if first value is not yet calculated
///                             E_NOT_OK    - if filter fails or not initialized
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_MoveAvgFilter_GetStatus ( const SMoveAvgFilter * const pFilterData )
{
    Std_ReturnType Ret;

    if ( NULL != pFilterData )
    {
        if ( pFilterData->Status == eFilterStatus_Initialized )
        {
            Ret = E_NOT_READY;
        }
        else if ( pFilterData->Status == eFilterStatus_Calculated )
        {
            Ret = E_OK;
        }
        else
        {
            Ret = E_NOT_OK;
        }
    }
    else
    {
        Ret = E_NOT_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Initialization of a moving average filter.
///
/// @param    pFilterData:    Moving average filter data structure.
/// @param    pMArrayData:    Filtered array.
/// @param    MArrayLen:      Filtered array length.
/// @param    InitValue:      Initial value for the array.
///
/// @return   Std_ReturnType:   E_OK     - MoveAvgFilter is initialized
///                             E_NOT_OK - MoveAvgFilter is NOT initialized, debug assert fails
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_MoveAvgFilter_32bit_Init( SMoveAvgFilter_32  * const pFilterData, uint32 * const pMArrayData, const uint16 MArrayLen, const uint32 InitValue )
{
    uint16 Ix;
    uint32 * pArray;
    Std_ReturnType Ret;

    if ( (NULL != pFilterData) && (NULL != pMArrayData) && (0U < MArrayLen) )
    {
        pFilterData->pMArray = pMArrayData;
        pFilterData->MArrayLen = MArrayLen;
        pFilterData->MArrayPos = 0U;

        Ix = 0U;
        pArray = pFilterData->pMArray;

        while ( Ix < MArrayLen )
        {
            (*pArray) = InitValue;
            pArray = &pArray[1];
            ++Ix;
        }

        // polyspace<RTE:OVFL> There isn't a problem here. MArrayLen cannot be bigger than 65535.
        pFilterData->Sum = (InitValue * (uint32)MArrayLen);
        pFilterData->Status = eFilterStatus_Initialized;

        Ret = E_OK;
    }
    else
    {
        DEBUG_ASSERT( FALSE );
        Ret = E_NOT_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used to apply a new value to the moving average filter.
///
/// @param    pFilterData:      Moving average filter data structure.
/// @param    ValueToFilter:    The new value to be filtered.
/// @param    pOutValue:        The output value after filtering.
///
/// @return   uint16:   Output value after filtering.
///
/// Note!   -   In case of invalid pointer (function arguments), debug assert fails and return value is 0
///             If pointer is valid but the filter is not initialized returned value is (pFilterData->Sum / pFilterData->MArrayLen)
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_MoveAvgFilter_Apply_32( SMoveAvgFilter_32 * const pFilterData, const uint32 ValueToFilter )
{
    uint32 * pArray;
    uint32 OutValue = 0U;

    if ( (NULL != pFilterData) && (NULL != pFilterData->pMArray) && (0U < pFilterData->MArrayLen) )
    {
        if ( (pFilterData->Status == eFilterStatus_Initialized) || (pFilterData->Status == eFilterStatus_Calculated) )
        {
            // add new value to sum of numbers in filter
            pFilterData->Sum += ValueToFilter;

            pArray = &pFilterData->pMArray[ pFilterData->MArrayPos ];

            // remove from sum oldest value
            // polyspace<RTE:IDP> If the client supplies the correct filter data there is no danger of failure here.
            pFilterData->Sum -= (*pArray);

            // add new value
            (*pArray) = ValueToFilter;

            // move to next position in array
            pFilterData->MArrayPos++;

            // if end of array is reached, start from beginning
            if ( pFilterData->MArrayPos >= pFilterData->MArrayLen )
            {
                pFilterData->MArrayPos = 0U;
            }

            pFilterData->Status = eFilterStatus_Calculated;
        }
        else
        {
            DEBUG_ASSERT( FALSE );
        }

        // calculate average value
        OutValue = (uint32)( pFilterData->Sum / pFilterData->MArrayLen );
    }
    else
    {
        DEBUG_ASSERT( FALSE );
    }

    return OutValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get status of moving average filter.
///
/// @param    pFilterData:      Moving average filter data structure.
///
/// @return   Std_ReturnType:   E_OK        - when there is filtered value that can be used
///                             E_NOT_READY - if first value is not yet calculated
///                             E_NOT_OK    - if filter fails or not initialized
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_MoveAvgFilter_GetStatus_32 ( const SMoveAvgFilter_32 * const pFilterData )
{
    Std_ReturnType Ret;

    if ( NULL != pFilterData )
    {
        if ( pFilterData->Status == eFilterStatus_Initialized )
        {
            Ret = E_NOT_READY;
        }
        else if ( pFilterData->Status == eFilterStatus_Calculated )
        {
            Ret = E_OK;
        }
        else
        {
            Ret = E_NOT_OK;
        }
    }
    else
    {
        Ret = E_NOT_OK;
    }

    return Ret;
}
