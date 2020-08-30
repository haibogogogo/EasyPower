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
//  CONSTANTS & TYPES
//=====================================================================================================================
#define cMAX_POSITIVE_VALUE_S32        ((uint32)2147483647)
#define cMAX_U32                       ((uint32)0xFFFFFFFFu)

#define cSQRT_START_LOOP_MASK          ((uint32) 0x8000)
#define cSQRT_MASK_SHIFT_VALUE         ((uint32) 0x01)

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get the absolute value of a signed integer.
///
/// @param    Value:   The number whose absolute value is returned.
///
/// @return   uint32:   The calculated absolute value.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_Abs( const sint32 Value )
{
    uint32 Result = (uint32)Value;

    if ( (sint32)0 > Value )
    {
        Result = ~Result + (uint32)1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get the absolute difference between two values.
///
/// @param    Value1:   First value for the calculation.
/// @param    Value2:   Second value for the calculation.
///
/// @return   uint32:   Delta between Value1 and Value2.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_AbsDelta( const uint32 Value1, const uint32 Value2 )
{
    uint32 Result;

    if ( Value1 > Value2 )
    {
        Result = Value1 - Value2;
    }
    else
    {
        Result = Value2 - Value1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Get the absolute difference between two values.
///
/// @param    Value1:   First value for the calculation.
/// @param    Value2:   Second value for the calculation.
///
/// @return   uint32:   Delta between Value1 and Value2.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_AbsDeltaS32( const sint32 Value1, const sint32 Value2 )
{
    uint32 Result;

    if( Value1 >= Value2 )
    {
        Result = Value1 - Value2;
    }
    else
    {
        Result = Value2 - Value1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Divide and round a value using a specified rounding method.
///
/// @param    Dividend:     Value which is being devided.
/// @param    Divisor:      Used divisor.
/// @param    RoundMethod:  Specified rounding method.
///
/// @return   uint32:       Quotient rounded by the chosen method. In case of failure uint32 max value is returned.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_DivAndRound( const uint32 Divident, const uint32 Divisor,  const ERoundingMethods RoundMethod )
{
    uint32 Result = cMAX_U32;

    if ( 0UL != Divisor )
    {
        switch( RoundMethod )
        {
            case eRoundingMethods_Common:
            {
                Result = ( Divident + ( Divisor >> 1 ) ) / Divisor;

                break;
            }

            case eRoundingMethods_Floor:
            {
                Result = Divident / Divisor;

                break;
            }

            case eRoundingMethods_Ceiling:
            {
                Result = ( Divident + ( Divisor - 1UL ) ) / Divisor;

                break;
            }

            default:
            {
                Result = cMAX_U32;

                break;
            }
        }
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Used for division of two signed integers. and returns the result.
///
/// @param    Dividend:     Value which is being devided.
/// @param    Divisor:      Used divisor.
/// @param    RoundMethod:  Specified rounding method.
///
/// @return   sint32:       Quotient rounded by the chosen method. In case of failure max sint32 positive value is returned.
//----------------------------------------------------------------------------------------------------------------------
sint32 MathLib_DivAndRoundS32( sint32 const Dividend, sint32 const Divisor, const ERoundingMethods RoundMethod )
{
    sint32 Result = (sint32)cMAX_POSITIVE_VALUE_S32;
    uint32 const AbsDivisor = MathLib_Abs(Divisor);

    if ( 0UL != AbsDivisor )
    {
        uint32 const AbsDividend = MathLib_Abs(Dividend);
        switch( RoundMethod )
        {
            case eRoundingMethods_Common:
            {
                if ((Dividend < (sint32)0) != (Divisor < (sint32)0))
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)(((sint64)AbsDividend + ( (sint64)AbsDivisor >> (sint64)1 ) ) / (sint64)AbsDivisor);
					
                }
                else
                {
                    Result = (sint32)(( (sint64)AbsDividend + ( (sint64)AbsDivisor >> (sint64)1 ) ) /(sint64)AbsDivisor);
                }
                break;
            }

            case eRoundingMethods_Floor:
            {
                if ((Dividend < (sint32)0) != (Divisor < (sint32)0))
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)(( (sint64)AbsDividend + ( (sint64)AbsDivisor - (sint64)1 ) ) / (sint64)AbsDivisor);
                }
                else
                {
                    Result = (sint32)((sint64)AbsDividend / (sint64)AbsDivisor);
                }
                break;
            }

            case eRoundingMethods_Ceiling:
            {
                if ((Dividend < (sint32)0) != (Divisor < (sint32)0))
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)((sint64)AbsDividend / (sint64)AbsDivisor);
                }
                else
                {
                    Result = (sint32)(((sint64)AbsDividend + ( (sint64)AbsDivisor - (uint32)1 ) ) / (sint64)AbsDivisor);
                }
                break;
            }

            default:
            {
                Result = (sint32)cMAX_POSITIVE_VALUE_S32;
                break;
            }
        }
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Divide a signed integer number to unsigend long number and return the result rounded by the chosen type.
///
/// @param    Dividend:     Value which is being devided.
/// @param    Divisor:      Used divisor.
/// @param    RoundMethod:  Specified rounding method.
///
/// @return   sint32:       Quotient rounded by the chosen method. In case of failure max sint32 positive value is returned.
//----------------------------------------------------------------------------------------------------------------------
sint32 MathLib_DivAndRoundS32ToU32( sint32 const Dividend, uint32 const Divisor, const ERoundingMethods RoundMethod )
{
    sint32 Result = (sint32)cMAX_POSITIVE_VALUE_S32;

    if ( 0UL != Divisor )
    {
        uint32 const AbsDividend = MathLib_Abs(Dividend);
        switch( RoundMethod )
        {
            case eRoundingMethods_Common:
            {
                if (Dividend < (sint32)0)
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)(( (sint64)AbsDividend + ( (sint64)Divisor >> (sint64)1 ) ) / (sint64)Divisor);
                }
                else
                {
                    Result = (sint32)(( (sint64)AbsDividend + ( (sint64)Divisor >> (sint64)1 ) ) /(sint64) Divisor);
                }
                break;
            }

            case eRoundingMethods_Floor:
            {
                if (Dividend < (sint32)0)
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)(( (sint64)AbsDividend + ((sint64) Divisor - (sint64)1 ) ) / (sint64)Divisor );
                }
                else
                {
                    Result = (sint32)((sint64)AbsDividend / (sint64)Divisor);
                }

                break;
            }

            case eRoundingMethods_Ceiling:
            {
                if (Dividend < (sint32)0)
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = -(sint32)((sint64)AbsDividend / (sint64)Divisor);
                }
                else
                {
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = (sint32)(( (sint64)AbsDividend + ( (sint64)Divisor - (sint64)1 ) ) /(sint64) Divisor );
                }
                break;
            }

            default:
            {
                Result = (sint32)cMAX_POSITIVE_VALUE_S32;
                break;
            }
        }
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Round a value using a specified rounding method and base.
///
/// @param    InputValue:     Value which must be rounded.
/// @param    RoundingBase:   Base used for rounding the number.
/// @param    RoundMethod:    Specified rounding method.
///
/// @return   uint32:         The rounded value. In case of failure InputValue is returned.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_Round( const uint32 InputValue, const uint32 RoundingBase, const ERoundingMethods RoundMethod )
{
    uint32 Result;

    if ( 0UL != RoundingBase )
    {
        switch( RoundMethod )
        {
            case eRoundingMethods_Common:
            {
                Result = ( ( InputValue + ( RoundingBase >> 1 ) ) / RoundingBase ) * RoundingBase;

                break;
            }

            case eRoundingMethods_Floor:
            {
                Result = ( InputValue / RoundingBase ) * RoundingBase;

                break;
            }

            case eRoundingMethods_Ceiling:
            {
                Result = ( ( InputValue + ( RoundingBase - 1UL ) ) / RoundingBase ) * RoundingBase;

                break;
            }

            // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
            default:
            {
                Result = InputValue;

                break;
            }
        }
    }
    else
    // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
    {
        Result = InputValue;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Round a signed value using a specified rounding method and base.
///
/// @param    InputValue:     Value which must be rounded.
/// @param    RoundingBase:   Base used for rounding the number.
/// @param    Offset:         Offest of a negative value. This value has to be multiple of RoundingBase, otherwise the rounding is not correct.
/// @param    RoundMethod:    Specified rounding method.
///
/// @return   sint32:         The rounded value. In case of failure InputValue is returned.
///
/// <br>Note:   Function has no protections against overflow.
/// <br>        Round common:  Overflow can happened when InputValue + Offset + ( RoundingBase / 2 ) is bigger than sint32 max value
/// <br>        Round floor:   No overflow can occur
/// <br>        Round ceiling: Overflow can happened when InputValue + Offset + ( RoundingBase - 1 ) is bigger than sint32 max value
//----------------------------------------------------------------------------------------------------------------------
sint32 MathLib_RoundS( const sint32 InputValue, const uint32 RoundingBase, const uint32 Offset, const ERoundingMethods RoundMethod )
{
    sint32 Result = InputValue;

    if ( (0UL != RoundingBase) && (RoundMethod < eRoundingMethods_Count) )
    {
        // check if Offset is multiple of RoundingBase
        if ( (Offset % RoundingBase) == 0U )
        {
            if ( InputValue < (sint32)0 )
            {
                // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                if ( (uint32)(-InputValue) <= Offset )
                {
                    // input value is in range [offset; 0]
                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result = (sint32)MathLib_Round( (uint32)( InputValue + (sint32)Offset ), RoundingBase, RoundMethod );

                    // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range.
                    Result -= (sint32)Offset;
                }
            }
            else
            {
                Result = (sint32)MathLib_Round( (uint32)InputValue, RoundingBase, RoundMethod );
            }
        }
        else
        {
            // Offset is not multiple of RoundingBase
            DEBUG_ASSERT( FALSE );
        }
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Calculates the square root of a number.
///
/// @param    Value:   The input value whose square root is returned.
///
/// @return   uint32:   The calculated square root.
//----------------------------------------------------------------------------------------------------------------------
uint32 MathLib_Sqrt( const uint32 Value )
{
    uint32 Square;
    uint32 Root;
    uint32 Mask;
    uint32 Result = 0U;

    for ( Mask = cSQRT_START_LOOP_MASK; Mask > (uint32)0; Mask >>= cSQRT_MASK_SHIFT_VALUE )
    {
        Root = (uint32)(Result | Mask);

        Square = (uint32)(Root * Root);

        if( Square == Value )
        {
            Result = Root;
            break;
        }
        else
        {
            if( Square < Value )
            {
                Result = Root;
            }
        }
    }

    return Result;
}

/*--------------------------------------------------------------------------------------------------------------------
REVISION HISTORY
---------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------
Date              : 29/June/2018
By                : pragesh
Traceability      : RTC #1147208
Change Description: MISRA and Coverity warning analysis and fix
----------------------------------------------------------------------------------------------------------------------*/

