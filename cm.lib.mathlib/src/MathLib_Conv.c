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

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define cMAX_RESOLUTION         ((sint8)-3)
#define cMIN_RESOLUTION         ((sint8)3)

#define cFAHR_FACTOR            (18)
#define cFAHR_SUMMAND           ((sint32)32)

#define cCONV_CONST_2           ((uint32)2)
#define cCONV_CONST_8           ((uint32)8)
#define cCONV_CONST_10          (10)
#define cCONV_CONST_11          ((uint32)11)
//#define cCONV_CONST_100         ((uint32)100)
#define cCONV_CONST_119         ((uint32)119)
#define cCONV_CONST_125         ((uint32)125)
#define cCONV_CONST_127         ((uint32)127)
#define cCONV_CONST_198         ((uint32)198)
#define cCONV_CONST_1000        ((uint32)1000)
#define cCONV_CONST_5000        ((uint32)5000)
//#define cCONV_CONST_9190        ((uint32)9190)
//#define cCONV_CONST_10000       ((uint32)10000)
#define cCONV_CONST_41666       ((uint32)41666)
#define cCONV_CONST_50000       ((uint32)50000)
#define cCUBIC_METER            ((uint32)1000000)
//#define cCONV_FACT_NUM          ((uint32)1000U)

#define cMETER_IN_YARDS         ((uint32)1093)

#define cPOWERS_NUMBER       ( 4 )
#define cPOWER_OF_TEN_INIT() { 1, 10, 100, 1000 }

// Constants for GetDigitCount
#define c10             ((uint32)10)
#define c100            ((uint32)100)
#define c1000           ((uint32)1000)
#define c10000          ((uint32)10000)
#define c100000         ((uint32)100000)
#define c1000000        ((uint32)1000000)
#define c10000000       ((uint32)10000000)
#define c100000000      ((uint32)100000000)
#define c1000000000     ((uint32)1000000000)

#define cDIGITS_1        ((uint8)1)
#define cDIGITS_2        ((uint8)2)
#define cDIGITS_3        ((uint8)3)
#define cDIGITS_4        ((uint8)4)
#define cDIGITS_5        ((uint8)5)
#define cDIGITS_6        ((uint8)6)
#define cDIGITS_7        ((uint8)7)
#define cDIGITS_8        ((uint8)8)
#define cDIGITS_9        ((uint8)9)
#define cDIGITS_10       ((uint8)10)

// Constants for conversion BCD -> ASCII and ACII -> BCD
#define cSHIFT_4_BITS               ((uint8)(4))
#define cASCII_ZERO_OFFSET          ((uint8)('\x30'))  //((uint8)0x30)
#define cBCD_LOW_BYTE_MASK          ((uint8)('\xF'))  //((uint8)0x0F)
#define cBCD_MAX_ALLOWED_VALUE      ((uint8)9)
#define cREMOVE_ASCII_ZERO_OFFSET   ((uint8)('\xCF'))  //((uint8)0xCF)

// Constants for GetBitCount functions
#define cSHIFT_8_BITS              (8)
#define cSHIFT_16_BITS             (16)
#define cSHIFT_24_BITS             (24)
#define cBITS_IN_BYTE_ARRAY_SIZE   (256)   // One element needed for every value in the range 0 - 255

static const uint32 cPOWER_OF_TEN[ cPOWERS_NUMBER ] = cPOWER_OF_TEN_INIT();

static uint32 ConvertMetersToMiles( const uint32 Dividend, const uint32 Divisor, const ERoundingMethods RoundMethod );

// Distance
// Meters, Miles, Yards
static const ConvFunc cDISTANCE_CONV_FUNCS[ eDistanceUnits_Count ] = { &MathLib_DivAndRound, &ConvertMetersToMiles, &MathLib_DivAndRound };
static const uint32 cDISTANCE_CONV_CONST_NUMERATORS[ eDistanceUnits_Count ] = { cCONV_CONST_1000, 1, cMETER_IN_YARDS };
static const uint32 cDISTANCE_CONV_CONST_DENOMINATORS[ eDistanceUnits_Count ] = { cCONV_CONST_1000, cCONV_CONST_1000, cCONV_CONST_1000 };

// Volume
// Liters, Gallons UK, Gallons US, Cubic Meters
static const uint32 cVOLUME_CONV_CONST_NUMERATORS[ eVolumeUnits_Count ] = { 1, cCONV_CONST_11, cCONV_CONST_11, 1 };
static const uint32 cVOLUME_CONV_CONST_DENOMINATORS[ eVolumeUnits_Count ] = { cCONV_CONST_1000, cCONV_CONST_50000, cCONV_CONST_41666, cCUBIC_METER };

// Weight
// Grams, Pounds
static const uint32 cWEIGHT_CONV_CONST_NUMERATORS[ eWeightUnits_Count ] = { cCONV_CONST_1000, cCONV_CONST_11 };
static const uint32 cWEIGHT_CONV_CONST_DENOMINATORS[ eWeightUnits_Count ] = { cCONV_CONST_1000, cCONV_CONST_5000 };

/// @brief Constant array with bit counts for every value in the range 0 - 255
static const uint8 cBITS_IN_BYTE[cBITS_IN_BYTE_ARRAY_SIZE] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3,
    4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,
    4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4,
    5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5,
    4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3,
    4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5,
    5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//----------------------------------------------------------------------------------------------------------------------
/// @brief    Converts meters to miles with chosen resolution.
///<br>
///<br>       miles = meters / 1609,344 = (meters / 1609344) / 1000 = ( meters * 1000 ) / 1609344
///<br>       Max resolution is -3 => miles * 1000 = ( meters * 1000000 ) / 1609344
///<br>       miles * 1000 = ( meters * 2^6 * 5^6 ) / ( 2^7 * 3^2 * 11 * 127 )
///<br>       miles * 1000 = ( meters * 5^6 ) / ( 2 * 3^2 * 11 * 127 )
///<br>       miles * 1000 = ( meters * 125 * 125 ) / ( 198 * 127 )
///<br>       To prevent overflow: miles * 1000 = ( ( ( meters / 127 ) * 125 ) / 198 ) * 125
///
/// @param    Dividend:      Distance in meters.
/// @param    Divisor:       Denominator of conversion constant.
/// @param    RoundMethod:   Rounding method.
///
/// @return   uint32:   Converted value in miles
//----------------------------------------------------------------------------------------------------------------------
static uint32 ConvertMetersToMiles( const uint32 Dividend, const uint32 Divisor, const ERoundingMethods RoundMethod )
{
    uint32 Output;

    Output = MathLib_DivAndRound( Dividend, cCONV_CONST_127, RoundMethod );
    Output = Output * cCONV_CONST_125;

    Output = MathLib_DivAndRound( Output, cCONV_CONST_198, RoundMethod );
    Output = Output * cCONV_CONST_125;

    Output = MathLib_DivAndRound( Output, Divisor, RoundMethod );

    return Output;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert temperatures from Celsius to Fahrenheit.
///
/// @param    Celsius:      Input temperature in degrees Celsius.
/// @param    Resolution:   Resolution for the conversion. Can be from [-3 to 0].
///
/// @return   sint32:       Fahrenheit degrees.
//----------------------------------------------------------------------------------------------------------------------
sint32 MathLib_CelsToFahrRes( const sint32 Celsius, const sint8 Resolution )
{
    uint16 ResolutionOfTen;
    sint32 Fahrenh = 0;
    uint32 Index = 0UL;

    if ( (cMAX_RESOLUTION <= Resolution) && (1 > Resolution) )
    {
        Index = MathLib_Abs( (sint32)Resolution );
        ResolutionOfTen = (uint16)cPOWER_OF_TEN[Index];

        Fahrenh = (sint32)((Celsius * cFAHR_FACTOR) / cCONV_CONST_10);

        Fahrenh += (sint32)( cFAHR_SUMMAND * (sint32)ResolutionOfTen );
    }

    return Fahrenh;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert between different distance units.
///
/// @param    Meters:       Value in meters.
/// @param    OutputUnit:   Selected output unit.
/// @param    OutputRes:    Selected output resolution.
/// @param    RoundMethod:  Selected rounding method.
/// @param    pOutput:      Output value of the conversion.
///
/// @return   Std_ReturnType:   E_OK    - success
/// <br>                        E_INVALID_ARG - invalid argument
///
/// <br> Note: If using to convert meters to meters or meters to yards
/// <br> the max value that shall be passed is 4500km = 4500000m
/// <br> otherwise overflow is observed and the result is not correct.
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_ConvertDistance( const uint32 Meters, const EDistanceUnits OutputUnit, const sint8 OutputRes,
                                        const ERoundingMethods RoundMethod, uint32 * const pOutput )
{
    uint32 Resolution;
    uint32 Index = 0UL;
    Std_ReturnType Ret = E_INVALID_ARG;

    if( (NULL != pOutput) && (OutputUnit < eDistanceUnits_Count) && (cMAX_RESOLUTION <= OutputRes)
                    && (cMIN_RESOLUTION >= OutputRes) && (RoundMethod < eRoundingMethods_Count) )
    {
        Index = MathLib_Abs( (sint32)OutputRes );
        Resolution = cPOWER_OF_TEN[Index];

        if ( OutputRes < 0 )
        {
            *pOutput = cDISTANCE_CONV_FUNCS[OutputUnit](
                Meters * cDISTANCE_CONV_CONST_NUMERATORS[OutputUnit],
                cDISTANCE_CONV_CONST_DENOMINATORS[OutputUnit] / Resolution,
                RoundMethod);
        }
        else
        {
            *pOutput = cDISTANCE_CONV_FUNCS[OutputUnit](
                Meters * cDISTANCE_CONV_CONST_NUMERATORS[OutputUnit],
                cDISTANCE_CONV_CONST_DENOMINATORS[OutputUnit] * Resolution,
                RoundMethod);
        }

        Ret = E_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert between different volume units.
///
/// @param    Mililiters:   Input value in mililiters.
/// @param    OutputUnit:   Selected output unit.
/// @param    OutputRes:    Selected output resolution.
/// @param    RoundMethod:  Selected rounding method.
/// @param    pOutput:      Output value of the conversion.
///
/// @return   Std_ReturnType:   E_OK    - success
//  <br>                        E_INVALID_ARG - invalid argument
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_ConvertVolume( const uint32 Mililiters, const EVolumeUnits OutputUnit, const sint8 OutputRes,
                                      const ERoundingMethods RoundMethod, uint32 * pOutput )
{
    uint32 Resolution;
    uint32 Divisor;
    uint32 Index = 0UL;
    Std_ReturnType Ret = E_INVALID_ARG;

    if( (NULL != pOutput) && (OutputUnit < eVolumeUnits_Count) && (cMAX_RESOLUTION <= OutputRes)
                    && (cMIN_RESOLUTION >= OutputRes) && (RoundMethod < eRoundingMethods_Count) )
    {
        Index = MathLib_Abs( (sint32)OutputRes );
        Resolution = cPOWER_OF_TEN[Index];

        if ( OutputRes < 0 )
        {
            Divisor = cVOLUME_CONV_CONST_DENOMINATORS[OutputUnit] / Resolution;
        }
        else
        {
            Divisor = cVOLUME_CONV_CONST_DENOMINATORS[OutputUnit] * Resolution;
        }

        *pOutput = MathLib_DivAndRound(Mililiters * cVOLUME_CONV_CONST_NUMERATORS[OutputUnit], Divisor, RoundMethod);

        Ret = E_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert between different weight units.
///
/// @param    Grams:        Input value in grams.
/// @param    OutputUnit:   Selected output unit.
/// @param    OutputRes:    Selected output resolution.
/// @param    RoundMethod:  Selected rounding method.
/// @param    pOutput:      Output value of the conversion.
///
/// @return   Std_ReturnType:   E_OK    - success
//  <br>                        E_INVALID_ARG - invalid argument
//----------------------------------------------------------------------------------------------------------------------
Std_ReturnType MathLib_ConvertWeight( const uint32 Grams, const EWeightUnits OutputUnit, const sint8 OutputRes,
                                      const ERoundingMethods RoundMethod, uint32 * const pOutput )
{
    uint32 Resolution;
    uint32 Divisor;
    uint32 Index = 0UL;
    Std_ReturnType Ret = E_INVALID_ARG;

    if( (NULL != pOutput) && (OutputUnit < eWeightUnits_Count) && (cMAX_RESOLUTION <= OutputRes)
                    && (cMIN_RESOLUTION >= OutputRes) && (RoundMethod < eRoundingMethods_Count) )
    {
        Index = MathLib_Abs( (sint32)OutputRes );
        Resolution = cPOWER_OF_TEN[Index];

        if ( OutputRes < 0 )
        {
            Divisor = cWEIGHT_CONV_CONST_DENOMINATORS[OutputUnit] / Resolution;
        }
        else
        {
            Divisor = cWEIGHT_CONV_CONST_DENOMINATORS[OutputUnit] * Resolution;
        }

        *pOutput = MathLib_DivAndRound(Grams * cWEIGHT_CONV_CONST_NUMERATORS[OutputUnit], Divisor, RoundMethod);

        Ret = E_OK;
    }

    return Ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert a 2-byte ASCII value to BCD.
///
/// @param    pAsciiArray:      Input ASCII array.
/// @param    pBcdValue:        Returned value in BCD.
///
/// @return   sint8:    Invalid input parameters (-1) or operation successful (0)
//----------------------------------------------------------------------------------------------------------------------
sint8 MathLib_AsciiToBcd( const uint8 * const pAsciiArray, uint8 * const pBcdValue )
{
    sint8 Result = -1;

    if( (NULL != pAsciiArray) && (NULL != pBcdValue) )
    {
        if(((pAsciiArray[0] & cREMOVE_ASCII_ZERO_OFFSET) <= cBCD_MAX_ALLOWED_VALUE)
            && ((pAsciiArray[1] & cREMOVE_ASCII_ZERO_OFFSET) <= cBCD_MAX_ALLOWED_VALUE) )
        {
            *pBcdValue = (uint8) ((pAsciiArray[0] & cREMOVE_ASCII_ZERO_OFFSET) << cSHIFT_4_BITS);
            *pBcdValue |= (uint8) ((pAsciiArray[1] & cREMOVE_ASCII_ZERO_OFFSET));
            Result = 0;
        }
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Convert a BCD value to a 2-byte ASCII.
///
/// @param    BcdValue:      Value in BCD.
/// @param    pAsciiArray:   Returned array - 2 bytes ASCII.
///
/// @return   sint8:    Invalid input parameters (-1) or operation successful (0)
//----------------------------------------------------------------------------------------------------------------------
sint8 MathLib_BcdToAscii( const uint8 BcdValue, uint8 * const pAsciiArray )
{
    sint8 Result = -1;

    if(    ((BcdValue >> cSHIFT_4_BITS) <= cBCD_MAX_ALLOWED_VALUE)
        && ((BcdValue & cBCD_LOW_BYTE_MASK) <= cBCD_MAX_ALLOWED_VALUE)
        && (NULL != pAsciiArray) )
    {
        pAsciiArray[0] = ((BcdValue >> cSHIFT_4_BITS) | cASCII_ZERO_OFFSET);
        pAsciiArray[1] = ((BcdValue & cBCD_LOW_BYTE_MASK) | cASCII_ZERO_OFFSET);
        Result = 0;
    }

    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Count the digits in a given 32-bit integer.
///
/// @param    Number:   The number whose digits are counted.
///
/// @return   uint8:    The number of digits of the input value
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_GetDigitCount( const uint32 Number )
{
    uint8 Digits;

    if ( Number < c100000 )
    {
        if ( Number >= c1000 )
        {
            if ( Number < c10000 )
            {
                Digits = cDIGITS_4;
            }
            else
            {
                Digits = cDIGITS_5;
            }
        }
        else
        {
            if ( Number < c10 )
            {
                Digits = cDIGITS_1;
            }
            else
            {
                if (Number < c100)
                {
                    Digits = cDIGITS_2;
                }
                else
                {
                    Digits = cDIGITS_3;
                }
            }
        }
    }
    else
    {
        if ( Number < c10000000 )
        {
            if ( Number < c1000000 )
            {
                Digits = cDIGITS_6;
            }
            else
            {
                Digits = cDIGITS_7;
            }
        }
        else
        {
            if (Number < c100000000)
            {
                Digits = cDIGITS_8;
            }
            else
            {
                if (Number < c1000000000)
                {
                    Digits = cDIGITS_9;
                }
                else
                {
                    Digits = cDIGITS_10;
                }
            }
        }
    }

    return Digits;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Returns the number of set bits in the given 8-bit number.
///
/// @param    Number:   The input number.
///
/// @return   uint8:    The number of bits which are set in the input number.
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_GetBitCount_U8( const uint8 Number )
{
    return cBITS_IN_BYTE[ Number ];
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Returns the number of set bits in the given 16-bit number.
///
/// @param    Number:   The input number.
///
/// @return   uint8:    The number of bits which are set in the input number.
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_GetBitCount_U16( const uint16 Number )
{
    const uint32 Temp = Number;

    return (uint8)(cBITS_IN_BYTE[(uint32)((uint8)(Temp >> cSHIFT_8_BITS))] +
                   cBITS_IN_BYTE[(uint32)((uint8)(Temp))]);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief    Returns the number of set bits in the given 32-bit number.
///
/// @param    Number:   The input number.
///
/// @return   uint8:    The number of bits which are set in the input number.
//----------------------------------------------------------------------------------------------------------------------
uint8 MathLib_GetBitCount_U32( const uint32 Number )
{
    return (uint8)( cBITS_IN_BYTE[(uint32)((uint8)(Number >> cSHIFT_24_BITS))] +
                    cBITS_IN_BYTE[(uint32)((uint8)(Number >> cSHIFT_16_BITS))] +
                    cBITS_IN_BYTE[(uint32)((uint8)(Number >> cSHIFT_8_BITS))]  +
                    cBITS_IN_BYTE[(uint32)((uint8)(Number))] );
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

