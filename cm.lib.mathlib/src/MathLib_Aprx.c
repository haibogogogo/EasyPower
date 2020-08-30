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
/// @brief Minimum number of defined points for function values
#define cMIN_NUM_POINTS_2           ((uint16) 2)

#define cMAX_S16_POSITIVE_VALUE     ((uint16)32767)
#define cMAX_S16_NEGATIVE_VALUE     ((sint16)-32768)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static uint32 Interpolate ( const uint32 Xe, const uint32 X1,
                            const uint32 Y1, const uint32 X2,
                            const uint32 Y2 );

static sint32 InterpolateS ( const sint32 Xe, const sint32 X1,
                             const sint32 Y1, const sint32 X2,
                             const sint32 Y2 );

static uint32 InterpolateSU ( const sint32 Xe, const sint32 X1,
                              const uint32 Y1, const sint32 X2,
                              const uint32 Y2 );

static sint32 InterpolateUS ( const uint32 Xe, const uint32 X1,
                              const sint32 Y1, const uint32 X2,
                              const sint32 Y2 );

static sint16 InterpolateS16( const sint16 Xe, const sint16 X1,
                              const sint16 Y1, const sint16 X2,
                              const sint16 Y2,
                              boolean * const pState );

static sint16 ExtrapolateS16( const sint16 * const pXPoint,
                              const sint16 * const pYPoint,
                              const uint16 Count,
                              const sint16 Xe,
                              boolean * const pState );

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
/// @brief    Interpolation function
/// <br>      The result of this function (not the result of the division) is rounded.
/// <br>      So a remainder of 0.5 will always be rounded up. That means if the
/// <br>      characteristic of the interpolation is falling and the remainder of the
/// <br>      division has the value 0.5 the result will be rounded up to the higher
/// <br>      unbroken value.
/// <br>
/// <br>      General Formula (see also function header):
/// <br>             Ya = Y1 + (((Y2 - Y1) * (Xe - X1)) / (X2 - X1))
/// <br>    Steps:         |     +---+---+           |    |       |    1. Step
/// <br>                   |         +-----+---------+    |       |    2. Step
/// <br>                   |               |              +---+---+    3. Step
/// <br>                   |               +------------+-----+        4. Step
/// <br>                   +----------------------------+              5. Step
/// <br>       The result of the 4. Step will be rounded before it is used at Step 5
///
///
/// @param    Xe:   Input value Xe.
/// @param    X1:   Input value X1.
/// @param    Y1:   Input value Y1.
/// @param    X2:   Input value X2.
/// @param    Y2:   Input value Y2.
///
/// @return   uint32:  The calculated Y value from the linear interpolation. On error returned value is Y1.
//----------------------------------------------------------------------------------------------------------------------
static uint32 Interpolate ( const uint32 Xe, const uint32 X1,
                            const uint32 Y1, const uint32 X2,
                            const uint32 Y2 )
{
    uint32 Product;
    uint32 Divisor;
    uint32 Quotient;
    uint32 Result;

    // Check if characteristic is rising or falling
    if ( Y1 > Y2 )
    {
        // Falling characteristic
        // Step 1
        Product = Y1 - Y2;
    }
    else
    {
        // Rising characteristic
        // Step 1
        Product = Y2 - Y1;
    }

    if(((Xe > X1) && ( X2 > Xe)) || ((Xe < X1) && ( X2 < Xe)))
    {
       Product = ( Xe - X1 )  * Product;
       // Step 3
       Divisor = ( X2 - X1 );

     // Step 4
      Quotient = MathLib_DivAndRound( Product, Divisor, eRoundingMethods_Common );


      if ( Y1 > Y2 )
      {
          // Step 5
          // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
          Result = Y1 - Quotient;
      }
      else
      {
          // Step 5
          // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
          Result = Y1 + Quotient;
      }
    }
    else
    {
    	Result =Y1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Interpolation function
/// <br>      The result of this function (not the result of the division) is rounded.
/// <br>      So a remainder of 0.5 will always be rounded up. That means if the
/// <br>      characteristic of the interpolation is falling and the remainder of the
/// <br>      division has the value 0.5 the result will be rounded up to the higher
/// <br>      unbroken value.
/// <br>
/// <br>      General Formula (see also function header):
/// <br>             Ya = Y1 + (((Y2 - Y1) * (Xe - X1)) / (X2 - X1))
/// <br>    Steps:         |     +---+---+           |    |       |    1. Step
/// <br>                   |         +-----+---------+    |       |    2. Step
/// <br>                   |               |              +---+---+    3. Step
/// <br>                   |               +------------+-----+        4. Step
/// <br>                   +----------------------------+              5. Step
/// <br>       The result of the 4. Step will be rounded before it is used at Step 5
///
///
/// @param    Xe:   Input value Xe.
/// @param    X1:   Input value X1.
/// @param    Y1:   Input value Y1.
/// @param    X2:   Input value X2.
/// @param    Y2:   Input value Y2.
///
/// @return   uint32:  The calculated Y value from the linear interpolation. On error returned value is Y1.
//----------------------------------------------------------------------------------------------------------------------
static sint32 InterpolateS ( const sint32 Xe, const sint32 X1,
                             const sint32 Y1, const sint32 X2,
                             const sint32 Y2 )
{
    sint32 Product;
    sint32 Divisor;
    sint32 Quotient;
    sint32 Result ;

    // Check if characteristic is rising or falling
    if ( Y1 > Y2 )
    {
        // Falling characteristic
        // Step 1
        Product =  Y1 - Y2;
    }
    else
    {
        // Rising characteristic
        // Step 1
        Product = Y2 - Y1;
    }
	
    if(((Xe > X1) && ( X2 > Xe)) || ((Xe < X1) && ( X2 < Xe)))
    {
		// Step 2
		Product = ( Xe - X1 )  * Product;

		// Step 3
		Divisor = ( X2 - X1 );

		// Step 4
		Quotient = MathLib_DivAndRoundS32( Product, Divisor, eRoundingMethods_Common );


		if ( Y1 > Y2 )
		{
			// Step 5
			// polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
			Result = Y1 - Quotient;
		}
		else
		{
			// Step 5
			// polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
			Result = Y1 + Quotient;
		}
    }
    else
    {
    	Result=Y1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Interpolation function
/// <br>      The result of this function (not the result of the division) is rounded.
/// <br>      So a remainder of 0.5 will always be rounded up. That means if the
/// <br>      characteristic of the interpolation is falling and the remainder of the
/// <br>      division has the value 0.5 the result will be rounded up to the higher
/// <br>      unbroken value.
/// <br>
/// <br>      General Formula (see also function header):
/// <br>             Ya = Y1 + (((Y2 - Y1) * (Xe - X1)) / (X2 - X1))
/// <br>    Steps:         |     +---+---+           |    |       |    1. Step
/// <br>                   |         +-----+---------+    |       |    2. Step
/// <br>                   |               |              +---+---+    3. Step
/// <br>                   |               +------------+-----+        4. Step
/// <br>                   +----------------------------+              5. Step
/// <br>       The result of the 4. Step will be rounded before it is used at Step 5
///
///
/// @param    Xe:   Input value Xe.
/// @param    X1:   Input value X1.
/// @param    Y1:   Input value Y1.
/// @param    X2:   Input value X2.
/// @param    Y2:   Input value Y2.
///
/// @return   uint32:  The calculated Y value from the linear interpolation. On error returned value is Y1.
//----------------------------------------------------------------------------------------------------------------------
static uint32 InterpolateSU ( const sint32 Xe, const sint32 X1,
                              const uint32 Y1, const sint32 X2,
                              const uint32 Y2 )
{
	sint32 Product;
	sint32 Divisor;
	sint32 Quotient;
    uint32 Result;

    // Check if characteristic is rising or falling
    if ( Y1 > Y2 )
    {
        // Falling characteristic
        // Step 1
        Product = (sint32)(Y1 - Y2);
    }
    else
    {
        // Rising characteristic
        // Step 1
        Product = (sint32)(Y2 - Y1);
    }

    if(((Xe > X1) && ( X2 > Xe)) || ((Xe < X1) && ( X2 < Xe)))
    {
	 // Step 2
		Product = ( Xe - X1 ) * Product;

		// Step 3
		Divisor = ( X2 - X1 );

		// Step 4
		Quotient = MathLib_DivAndRoundS32( Product, Divisor, eRoundingMethods_Common );

		if ( Y1 > Y2 )
		{
			// Step 5
			Result = Y1 - Quotient;
		}
		else
		{
			// Step 5
			Result = Y1 + Quotient;
		}
    }
    else
    {
    	Result =Y1;
    }



    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Interpolation function
/// <br>      The result of this function (not the result of the division) is rounded.
/// <br>      So a remainder of 0.5 will always be rounded up. That means if the
/// <br>      characteristic of the interpolation is falling and the remainder of the
/// <br>      division has the value 0.5 the result will be rounded up to the higher
/// <br>      unbroken value.
/// <br>
/// <br>      General Formula (see also function header):
/// <br>             Ya = Y1 + (((Y2 - Y1) * (Xe - X1)) / (X2 - X1))
/// <br>    Steps:         |     +---+---+           |    |       |    1. Step
/// <br>                   |         +-----+---------+    |       |    2. Step
/// <br>                   |               |              +---+---+    3. Step
/// <br>                   |               +------------+-----+        4. Step
/// <br>                   +----------------------------+              5. Step
/// <br>       The result of the 4. Step will be rounded before it is used at Step 5
///
///
/// @param    Xe:   Input value Xe.
/// @param    X1:   Input value X1.
/// @param    Y1:   Input value Y1.
/// @param    X2:   Input value X2.
/// @param    Y2:   Input value Y2.
///
/// @return   sint32:  The calculated Y value from the linear interpolation. On error returned value is Y1.
//----------------------------------------------------------------------------------------------------------------------

static sint32 InterpolateUS ( const uint32 Xe, const uint32 X1,
                              const sint32 Y1, const uint32 X2,
                              const sint32 Y2 )
{
    uint32 Product;
    uint32 Divisor;
    uint32 Quotient;
    sint32 Result;

    // Check if characteristic is rising or falling
    if ( Y1 > Y2 )
    {
        // Falling characteristic
        // Step 1
        Product =Y1 - Y2;
    }
    else
    {
        // Rising characteristic
        // Step 1
        Product =Y2 - Y1;
    }

    if(((Xe > X1) && ( X2 > Xe)) || ((Xe < X1) && ( X2 < Xe)))
    {
		// Step 2
		Product = ( Xe - X1 ) * Product;

		// Step 3
		Divisor = ( X2 - X1 );

		// Step 4
		Quotient = MathLib_DivAndRound( Product, Divisor, eRoundingMethods_Common );

		if ( Y1 > Y2 )
		{
			// Step 5
			// polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
			Result = Y1 - Quotient;
		}
		else
		{
			// Step 5
			// polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
			Result = Y1 + Quotient;
		}
    }
    else
    {
    	Result =Y1;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Interpolation function
/// <br>      The result of this function (not the result of the division) is rounded.
/// <br>      So a remainder of 0.5 will always be rounded up. That means if the
/// <br>      characteristic of the interpolation is falling and the remainder of the
/// <br>      division has the value 0.5 the result will be rounded up to the higher
/// <br>      unbroken value.
/// <br>
/// <br>      General Formula (see also function header):
/// <br>             Ya = Y1 + (((Y2 - Y1) * (Xe - X1)) / (X2 - X1))
/// <br>    Steps:         |     +---+---+           |    |       |    1. Step
/// <br>                   |         +-----+---------+    |       |    2. Step
/// <br>                   |               |              +---+---+    3. Step
/// <br>                   |               +------------+-----+        4. Step
/// <br>                   +----------------------------+              5. Step
/// <br>       The result of the 4. Step will be rounded before it is used at Step 5
///
///
/// @param    Xe:      Input value Xe.
/// @param    X1:      Input value X1.
/// @param    Y1:      Input value Y1.
/// @param    X2:      Input value X2.
/// @param    Y2:      Input value Y2.
/// @param    pState:  'FALSE' if there's overflow or underflow and TRUE if there isn't.
///
/// @return   sint16:  The calculated Y value from the linear interpolation. On error returned value is Y1.
//----------------------------------------------------------------------------------------------------------------------
static sint16 InterpolateS16( const sint16 Xe, const sint16 X1,
                              const sint16 Y1, const sint16 X2,
                              const sint16 Y2,
                              boolean * const pState )
{
    uint16 Num;
    uint16 Temp;
    sint16 Result = Y1;

    // polyspace<RTE:OVFL> There isn't a problem when the result is bigger than MAX INT16
    const uint16 Divisor = (uint16)MathLib_AbsDeltaS32( (sint32)X2, (sint32)X1 );
    const uint16 HalfDivisor = (uint16)( Divisor >> 1 );

    *pState = (boolean)TRUE;

    if ( Divisor > (uint16)0 )
    {
        // polyspace<RTE:OVFL> There isn't a problem when the result is bigger than MAX INT16
        const uint16 Product = (uint16)MathLib_AbsDeltaS32( (sint32)Xe, (sint32)X1 );
        const uint16 Mult = (uint16)MathLib_AbsDeltaS32( (sint32)Y1, (sint32)Y2 );

        // Check if characteristic is rising or falling
        if ( Y1 > Y2 )
        {
            // Falling characteristic
            // polyspace<RTE:OVFL> Overflowing is checking on the next row
            Num = (uint16)( Product * Mult );

#ifdef cFAST_EXECUTION
            Temp = (uint16)((Num + HalfDivisor) / Divisor);
            Result = (sint16)(Result - (sint16)Temp);
#else
            // Checking for overflowing
            //polyspace<RTE:ZDV> The divisor can't be zero, because it's already checked.
            if ( (Num / Mult) != Product )
            {
                *pState = (boolean)FALSE;
                Result = (sint16)cMAX_S16_POSITIVE_VALUE;
            }
            else
            {
                Temp = (uint16)( (Num + HalfDivisor) / Divisor );
                // polyspace<RTE:OVFL> Underflowing can't be presented, because the sum of the members is always bigger than MIN INT16
                Result = (sint16)( Result - (sint16)Temp );
            }
#endif
        }
        else
        {
            // polyspace<RTE:OVFL> Overflowing is checking on the next row
            Num = (uint16)( Product * Mult );

#ifdef cFAST_EXECUTION
            Temp = (uint16)((Num + HalfDivisor) / Divisor);
            Result = (sint16)(Result + (sint16)Temp);
#else
            // Checking for overflowing
            if ( (Mult > (uint16)0) && ((Num / Mult) != Product) )
            {
                *pState = (boolean)FALSE;
                Result = (sint16)cMAX_S16_POSITIVE_VALUE;
            }
            else
            {
                Temp = (uint16)( ( Num + HalfDivisor ) / Divisor );
                // polyspace<RTE:OVFL> Overflowing can't be presented, because the sum of the members is always smaller than MAX INT16
                Result = (sint16)( Result + (sint16)Temp );
            }
#endif
        }
    }
    else
    {
        *pState = (boolean)FALSE;
        Result = (sint16)cMAX_S16_POSITIVE_VALUE;
    }
    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Linear extrapolation function.
///
/// @param    XPoints:   Table with X-values.
/// @param    pYPoints:  Table with Y-values.
/// @param    Count:     Number of points (2..65535).
/// @param    Xe:        Input value.
/// @param    bState:    FALSE if there's overflow or underflow and TRUE if there isn't
///
/// @return   sint16:    The calculated Y value from the linear extrapolation.
///
/// <br>Note: On error value for *bState is set to "FALSE", on overflow returned value is "cMaxS16PositiveValue"
/// <br>      and on underflow returned value is "cMAX_S16_NEGATIVE_VALUE".
//----------------------------------------------------------------------------------------------------------------------
static sint16 ExtrapolateS16( const sint16 * const pXPoint,
                              const sint16 * const pYPoint,
                              const uint16 Count,
                              const sint16 Xe,
                              boolean * const pState )
{
    // Initializing of the returning value
    uint16 Num;
    uint16 u16Temp = 0U;
    sint16 s16Temp = 0;

    sint16 Result = pYPoint[0];

    const uint16 DivisorBegin = (uint16)MathLib_AbsDeltaS32( (sint32)pXPoint[1], (sint32)pXPoint[0] );
    const uint16 DivisorEnd = (uint16)MathLib_AbsDeltaS32( (sint32)pXPoint[Count - (uint16)1], (sint32)pXPoint[Count - (uint16)2] );

    // Half of Divisor
    const uint16 HalfDivisorBegin = (uint16)( DivisorBegin >> 1 );
    const uint16 HalfDivisorEnd = (uint16)( DivisorEnd >> 1 );

    *pState = (boolean)TRUE;

    if ( Xe > pXPoint[0] )
    {
        Result = pYPoint[Count - (uint16) 1];
    }
    // The point is less than the first point from the array
    if ( (Xe < pXPoint[0]) && (DivisorBegin > (uint16) 0) )
    {
        // Begin of characteristic
        const uint16 ProductBegin = (uint16)MathLib_AbsDeltaS32( (sint32)pYPoint[1], (sint32)pYPoint[0] );
        Num = (uint16)(pXPoint[0] - Xe);

        // Begin of characteristic
        // polyspace<RTE:OVFL> In case of overflow FALSE is returned.
        Num = (uint16)(Num * ProductBegin);

#ifdef cFAST_EXECUTION
        u16Temp = (uint16)((Num + HalfDivisorBegin) / DivisorBegin);
        s16Temp = (sint16)u16Temp;
#else
        if( ( Num / (uint16)( pXPoint[0] - Xe ) ) != ProductBegin )
        {
            *pState = (boolean)FALSE;
            Result = (sint16) cMAX_S16_POSITIVE_VALUE;
        }
        else
        {
            u16Temp = (uint16)( ( Num + HalfDivisorBegin ) / DivisorBegin );

            if ( u16Temp > (uint16)cMAX_S16_POSITIVE_VALUE )
            {
                *pState = (boolean)FALSE;
                Result = (sint16)cMAX_S16_POSITIVE_VALUE;
            }
            else
            {
                s16Temp = (sint16)u16Temp;
            }
        }

#endif
        if ( Result != (sint16)cMAX_S16_POSITIVE_VALUE )
        {
            // Rising characteristic
            if ( pYPoint[1] > pYPoint[0] )
            {
                // polyspace<RTE:OVFL> Underflowing is checking on the next row
                Result = (sint16)( pYPoint[0] - s16Temp );

#ifndef cFAST_EXECUTION
                // Check for underflowing
                if ( Result > pYPoint[0] )
                // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
                {
                    *pState = (boolean)FALSE;
                    Result = cMAX_S16_NEGATIVE_VALUE;
                }
#endif
            }
            // Falling characteristic
            else
            {
                // polyspace<RTE:OVFL> Overflowing is checking on the next row
                Result = (sint16)( pYPoint[0] + s16Temp );

#ifndef cFAST_EXECUTION
                // Check for overflowing
                if ( Result < pYPoint[0] )
                // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
                {
                    *pState = (boolean)FALSE;
                    Result = (sint16) cMAX_S16_POSITIVE_VALUE;
                }
#endif
            }
        }
    }
    // The point is bigger than the last point from the array
    else if ( (Xe > pXPoint[Count - (uint16) 1]) && ( DivisorEnd > (uint16) 0) )
    {
        // End of characteristic
        const uint16 ProductEnd = (uint16)MathLib_AbsDeltaS32( (sint32)pYPoint[Count - (uint16)1], (sint32)pYPoint[Count - cMIN_NUM_POINTS_2] );
        Num = (Xe - pXPoint[Count - (uint16) 1] );

        // End of characteristic
        // polyspace<RTE:OVFL> In case of overflow FALSE is returned.
        Num = (uint16)(Num * ProductEnd);

#ifdef cFAST_EXECUTION
        uint32 u32Temp = (uint16)((Num + HalfDivisorEnd) / DivisorEnd);
        s16Temp = (sint16)u32Temp;
#else
        if ( (Num / (uint16)( Xe - pXPoint[Count - (uint16) 1] )) != ProductEnd )
        {
            *pState = (boolean)FALSE;
            Result = (sint16) cMAX_S16_POSITIVE_VALUE;
        }
        else
        {
            u16Temp = (uint16)( ( Num + HalfDivisorEnd ) / DivisorEnd );

            if ( u16Temp > (uint16)cMAX_S16_POSITIVE_VALUE )
            {
                *pState = (boolean)FALSE;
                Result = (sint16)cMAX_S16_POSITIVE_VALUE;
            }
            else
            {
                s16Temp = (sint16) u16Temp;
            }
        }
#endif
        if ( Result != (sint16)cMAX_S16_POSITIVE_VALUE )
        {
            // Rising characteristic
            if ( pYPoint[Count - (uint16)1] > pYPoint[Count - cMIN_NUM_POINTS_2] )
            {
                // polyspace<RTE:OVFL> Overflowing is checking on the next row
                Result = (sint16)( pYPoint[Count - (uint16)1] + s16Temp );

#ifndef cFAST_EXECUTION
                // Check for overflowing
                if ( Result < pYPoint[Count - (uint16)1] )
                // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
                {
                    *pState = (boolean)FALSE;
                    Result = (sint16)cMAX_S16_POSITIVE_VALUE;
                }
#endif
            }
            // Falling characteristic
            else
            {
                // polyspace<RTE:OVFL> Underflowing is checking on the next row
                Result = (sint16)( pYPoint[Count - (uint16)1] - s16Temp );

#ifndef cFAST_EXECUTION
                // Check for underflowing
                if ( Result > pYPoint[Count - (uint16)1] )
                // polyspace<RTE:UNR> Because this function is called internally this code is unreachable however this is tested with VectorCast.
                {
                    *pState = (boolean)FALSE;
                    Result = cMAX_S16_NEGATIVE_VALUE;
                }
#endif
            }
        }
    }
    else
    {
        *pState = (boolean)FALSE;
    }
    return Result;
}

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for extrapolation of signed 8-bit values.
///
/// @param    XPoints:   Extrapolation table with X-values.
/// @param    pYPoints:  Extrapolation table with Y-values.
/// @param    Count:     Number of extrapolation points.
/// @param    XValue:    Input value.
///
/// @return   sint16:   Output value from the extrapolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or if state is false, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
sint16 MathLib_ExtrapolateS16( const sint16 * const pXPoints,
                                       const sint16 * const pYPoints,
                                       const uint16 Count,
                                       const sint16 XValue )
{
    uint16 Ix;
    const sint16 * pPoint;
    sint16 YValue = 0;
    boolean State = (boolean)TRUE;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || (cMIN_NUM_POINTS_2 > Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( (XValue < pXPoints[0]) || (XValue > pXPoints[Count - (uint16)1]) )
        {
            // The input value is outside of the interpolation table
            YValue = ExtrapolateS16(pXPoints, pYPoints, Count, XValue, &State);
        }
        else if ( XValue == pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else if ( XValue == pXPoints[Count - (uint16)1] )
        {
            YValue = pYPoints[Count - (uint16)1];
        }
        else
        {
            // The input value is inside from the interpolation table
            // Result = Y-Value from the last interpolation point
            Ix = (uint16)( Count - (uint16)1 );

            // divide array in two parts for faster search
            if ( XValue < pXPoints[Count >> 1U] )
            {
                Ix = (uint16)( Count >> 1U );
            }

            pPoint = &pXPoints[Ix];

            do
            {
                Ix--;
                pPoint--;
            }
            // polyspace<RTE:IDP> If the client supplies the correct array address and size there is no danger of failure here.
            while ( (*pPoint) > XValue );


            // polyspace<RTE:IDP> If the client supplies the correct array address and size there is no danger of failure here.
            YValue = InterpolateS16( XValue, pXPoints[Ix], pYPoints[Ix], pXPoints[Ix + (uint16)1], pYPoints[Ix + (uint16)1], &State );
        }

        DEBUG_ASSERT( State );
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Calculates linear interpolation with uint8 points

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points (1..255)
/// @param    XValue:    Input value.
///
/// @return   uint8:   Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_InterpolateLinearU8 ( const uint8 * const pXPoints,
                                             const uint8 * const pYPoints,
                                             const uint8 Count,
                                             const uint8 XValue )
{
    uint8 Ix;
    const uint8 * pPoint;
    uint8 YValue = (uint8)0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint8)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if ( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count;
                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                YValue = (uint8)Interpolate( (uint32)XValue, (uint32)pXPoints[Ix], (uint32)pYPoints[Ix], (uint32)pXPoints[Ix + ((uint8)1)], (uint32)pYPoints[Ix + ((uint8)1)] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for linear interpolation of 16-bit values.

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points.
/// @param    XValue:    Input value.
///
/// @return   uint16:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
uint16 MathLib_InterpolateLinearU16 ( const uint16 * const pXPoints,
                                              const uint16 * const pYPoints,
                                              const uint16 Count,
                                              const uint16 XValue )
{
    uint16 Ix;
    const uint16 * pPoint;
    uint16 YValue = (uint16)0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ( (uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if ( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix =  Count - (uint16)1 ;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                YValue = (uint16)Interpolate( (uint32)XValue, (uint32)pXPoints[Ix], (uint32)pYPoints[Ix], (uint32)pXPoints[Ix + 1UL], (uint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Calculates linear interpolation with uint8 points

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points (1..255)
/// @param    XValue:    Input value.
///
/// @return   sint8:   Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
sint8 MathLib_InterpolateLinearS8 ( const sint8 * const pXPoints,
                                             const uint8 * const pYPoints,
                                             const uint8 Count,
                                             const sint8 XValue )
{
    uint8 Ix;
    const sint8 * pPoint;
    sint8 YValue = 0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = (sint8)pYPoints[0];
        }
        else
        {
            if( XValue >= pXPoints[Count - 1U] )
            {
                YValue = (sint8)pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint8)1;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                YValue = (sint8)InterpolateSU( (sint32)XValue, (sint32)pXPoints[Ix], (uint32)pYPoints[Ix], (sint32)pXPoints[Ix + 1UL], (uint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for linear interpolation of signed 16-bit values.

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points.
/// @param    XValue:    Input value.
///
/// @return   sint16:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
sint16 MathLib_InterpolateLinearS16 ( const sint16 * const pXPoints,
                                              const sint16 * const pYPoints,
                                              const uint16 Count,
                                              const sint16 XValue )
{
    uint16 Ix;
    const sint16 * pPoint;
    sint16 YValue = 0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if ( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint16)1;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
                YValue = (sint16)InterpolateS( (sint32)XValue, (sint32)pXPoints[Ix], (sint32)pYPoints[Ix], (sint32)pXPoints[Ix + 1UL], (sint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    his operation is used for linear interpolation of unsigned 16-bit to signed 8-bit values.

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points.
/// @param    XValue:    Input value.
///
/// @return   sint8:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
sint8 MathLib_InterpolateLinearU16_S8 ( const uint16 * const pXPoints,
                                                 const sint8 * const pYPoints,
                                                 const uint16 Count,
                                                 const uint16 XValue )
{
    uint16 Ix;
    const uint16 * pPoint;
    sint8 YValue = 0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint16)1;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
               YValue = (sint8)InterpolateUS( (uint32)XValue, (uint32)pXPoints[Ix], (sint32)pYPoints[Ix], (uint32)pXPoints[Ix + 1UL], (sint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for linear interpolation of unsigned to signed 16-bit values.

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points.
/// @param    XValue:    Input value.
///
/// @return   sint16:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
sint16 MathLib_InterpolateLinearU16_S16 ( const uint16 * const pXPoints,
                                                  const sint16 * const pYPoints,
                                                  const uint16 Count,
                                                  const uint16 XValue )
{
    uint16 Ix;
    const uint16 * pPoint;
    sint16 YValue = 0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint16)1;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                // polyspace<RTE:OVFL> Overflowing can't be presented, values of numbers are in range
               YValue = (sint16)InterpolateUS( (uint32)XValue, (uint32)pXPoints[Ix], (sint32)pYPoints[Ix], (uint32)pXPoints[Ix + 1UL], (sint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for linear interpolation of unsigned 16-bit to unsigned 8-bit values.

/// @param    XPoints:   Interpolation table with X-values.
/// @param    pYPoints:  Interpolation table with Y-values.
/// @param    Count:     Number of interpolation points.
/// @param    XValue:    Input value.
///
/// @return   uint8:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_InterpolateLinearU16_U8 ( const uint16 * const pXPoints,
                                                 const uint8 * const pYPoints,
                                                 const uint16 Count,
                                                 const uint16 XValue )
{
    uint16 Ix;
    const uint16 * pPoint;
    uint8 YValue = (uint8)0;

    if ( (NULL == pXPoints) || (NULL == pYPoints) || ((uint16)0 == Count) )
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pXPoints[0] )
        {
            YValue = pYPoints[0];
        }
        else
        {
            if ( XValue >= pXPoints[Count - 1U] )
            {
                YValue = pYPoints[Count - 1U];
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint16)1;

                // divide array in two pars for faster search
                if ( XValue < pXPoints[Count >> 1U] )
                {
                    Ix = Count >> 1U;
                }

                pPoint = &pXPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( (*pPoint) > XValue );

                // linear interpolation between the points
                YValue = (uint8)Interpolate( (uint32)XValue, (uint32)pXPoints[Ix], (uint32)pYPoints[Ix], (uint32)pXPoints[Ix + 1UL], (uint32)pYPoints[Ix + 1UL] );
            }
        }
    }

    return YValue;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    This operation is used for linear interpolation of coordinate point values.

/// @param    pCoordPoints:   Interpolation table with point values.
/// @param    Count:          Number of interpolation points.
/// @param    XValue:         Input value.
///
/// @return   uint16:    Output value from the interpolation.
///
/// <br>Note: In case of invalid pointer (function arguments) or number of interpolation points is 0, debug assert fails.
//----------------------------------------------------------------------------------------------------------------------
uint16 MathLib_InterpolateLinearPoint ( const SPoint * const pCoordPoints,
                                                const uint16 Count,
                                                const uint16 XValue )
{
    uint16 Ix;
    const SPoint * pPoint;
    uint16 YValue = (uint16)0;

    if ( (NULL == pCoordPoints) || ((uint16)0 == Count ))
    {
        DEBUG_ASSERT( FALSE );
    }
    else
    {
        if ( XValue <= pCoordPoints[0].X )
        {
            YValue = pCoordPoints[0].Y;
        }
        else
        {
            if ( XValue >= pCoordPoints[Count - 1U].X )
            {
                YValue = pCoordPoints[Count - 1U].Y;
            }
            else
            {
                // The input value is inside from the interpolation table
                // Result = Y-Value from the last interpolation point
                Ix = Count - (uint16)1;

                // divide array in two pars for faster search
                if ( XValue < pCoordPoints[Count >> 1U].X )
                {
                    Ix = Count >> 1;
                }

                pPoint = &pCoordPoints[Ix];

                do
                {
                    Ix--;
                    pPoint--;
                }
                // polyspace<RTE:IDP> If the client supplies the correct array there is no danger of failure here.
                while ( pPoint->X > XValue );

                // linear interpolation between the points
                YValue = (uint16)Interpolate( (uint32)XValue, (uint32)(pCoordPoints[Ix].X), (uint32)(pCoordPoints[Ix].Y), (uint32)(pCoordPoints[Ix + 1UL].X), (uint32)(pCoordPoints[Ix + 1UL].Y) );
            }
        }
    }

    return YValue;
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

