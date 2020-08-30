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
#ifndef MATHLIB_H_
#define MATHLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
// Always use this macros for binary search functions.
#define MATHLIB_BINARY_SEARCH_S8(pKey, pArray, ArraySize)        bsearch(pKey, pArray, ArraySize, sizeof(sint8), Compare_S8)
#define MATHLIB_BINARY_SEARCH_S16(pKey, pArray, ArraySize)       bsearch(pKey, pArray, ArraySize, sizeof(sint16), Compare_S16)
#define MATHLIB_BINARY_SEARCH_S32(pKey, pArray, ArraySize)       bsearch(pKey, pArray, ArraySize, sizeof(sint32), Compare_S32)
#define MATHLIB_BINARY_SEARCH_U8(pKey, pArray, ArraySize)        bsearch(pKey, pArray, ArraySize, sizeof(uint8), Compare_U8)
#define MATHLIB_BINARY_SEARCH_U16(pKey, pArray, ArraySize)       bsearch(pKey, pArray, ArraySize, sizeof(uint16), Compare_U16)
#define MATHLIB_BINARY_SEARCH_U32(pKey, pArray, ArraySize)       bsearch(pKey, pArray, ArraySize, sizeof(uint32), Compare_U32)

// Always use this macros for linear search functions.
#define MATHLIB_LINEAR_SEARCH_S8(pArray, ArraySize, pSearchValue, pPositionFound)      MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(sint8), Compare_S8)
#define MATHLIB_LINEAR_SEARCH_S16(pArray, ArraySize, pSearchValue, pPositionFound)     MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(sint16), Compare_S16)
#define MATHLIB_LINEAR_SEARCH_S32(pArray, ArraySize, pSearchValue, pPositionFound)     MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(sint32), Compare_S32)
#define MATHLIB_LINEAR_SEARCH_U8(pArray, ArraySize, pSearchValue, pPositionFound)      MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(uint8), Compare_U8)
#define MATHLIB_LINEAR_SEARCH_U16(pArray, ArraySize, pSearchValue, pPositionFound)     MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(uint16), Compare_U16)
#define MATHLIB_LINEAR_SEARCH_U32(pArray, ArraySize, pSearchValue, pPositionFound)     MathLib_LinearSearch(pArray, ArraySize, pSearchValue, pPositionFound, sizeof(uint32), Compare_U32)

/// @brief This type is used to describe different rounding methods.
typedef enum 
{
    eRoundingMethods_Common,
    eRoundingMethods_Floor,
    eRoundingMethods_Ceiling,
    eRoundingMethods_Count
} ERoundingMethods_t;
typedef ERoundingMethods_t ERoundingMethods;
/// @brief This type is used to describe different distance units.
typedef enum 
{
    eDistanceUnits_Meters,
    eDistanceUnits_Miles,
    eDistanceUnits_Yards,
    eDistanceUnits_Count
} EDistanceUnits_t;
typedef EDistanceUnits_t EDistanceUnits;
/// @brief This type is used to describe different volume units.
typedef enum 
{
    eVolumeUnits_Liters,
    eVolumeUnits_GalonsUK,
    eVolumeUnits_GalonsUS,
    eVolumeUnits_CubicMeters,
    eVolumeUnits_Count
} EVolumeUnits_t;
typedef EVolumeUnits_t EVolumeUnits;
/// @brief This type is used to describe different weight units.
typedef enum 
{
    eWeightUnits_Grams,
    eWeightUnits_Pounds,
    eWeightUnits_Count
} EWeightUnits_t;
typedef EWeightUnits_t EWeightUnits;

/// @brief This type contains points X and Y for linear interpolation.
typedef struct 
{
    uint16 X;
    uint16 Y;
} SPoint_t;
typedef SPoint_t SPoint;
typedef enum 
{
    eFilterStatus_NOT_OK,
    eFilterStatus_Initialized,
    eFilterStatus_Calculated
} EFilterStatus_t;

/// @brief This type contains the attributes elements for PT1 filtering.
typedef struct 
{
    /// @brief Damping time constant used for the filtering.
    uint16 DampingTime;

    /// @brief Output value after filtering.
    uint16 OutputValue;

    /// @brief Remainder after the filtering.
    uint16 Remainder;

    /// @brief Current status of the filter.
    EFilterStatus_t Status;
} SPt1FilterData_t;
typedef SPt1FilterData_t SPt1FilterData;

/// @brief This type contains the attributes elements for moving average filtering.
typedef struct 
{
    /// @brief The sum of numbers in the filter.
    uint32 Sum;

    /// @brief Reference to the filtered array.
    uint16 * pMArray;

    /// @brief Length of the filtered array.
    uint16 MArrayLen;

    /// @brief Current position in the filtered array.
    uint16 MArrayPos;

    /// @brief Current status of the filter.
    EFilterStatus_t Status;
} SMoveAvgFilter_t;
typedef SMoveAvgFilter_t SMoveAvgFilter;

/// @brief This type contains the attributes elements for moving average filtering.
typedef struct 
{
    /// @brief The sum of numbers in the filter.
    uint32 Sum;

    /// @brief Reference to the filtered array.
    uint32 * pMArray;

    /// @brief Length of the filtered array.
    uint16 MArrayLen;

    /// @brief Current position in the filtered array.
    uint16 MArrayPos;

    /// @brief Current status of the filter.
    EFilterStatus_t Status;
} SMoveAvgFilter_32_t;
typedef SMoveAvgFilter_32_t SMoveAvgFilter_32;
/// @brief Status of the filter.

/// @brief This type is used as a reference to compare functions for binary search.
typedef sint  (* CompareFunc)( void const * const pElem1, void const * const pElem2);

/// @brief This type is used as a reference to conversion functions.
typedef uint32 (* ConvFunc)( const uint32 Dividend, const uint32 Divisor, const ERoundingMethods RoundMethod );


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
// ---------------------------------------------------------------------------------------------------------------------
// UML::IMathLibApproximation
// ---------------------------------------------------------------------------------------------------------------------
sint16 MathLib_ExtrapolateS16( const sint16 * const pXPoints,
                                       const sint16 * const pYPoints,
                                       const uint16 Count,
                                       const sint16 XValue );

uint8 MathLib_InterpolateLinearU8 ( const uint8 * const pXPoints,
                                             const uint8 * const pYPoints,
                                             const uint8 Count,
                                             const uint8 XValue );

uint16 MathLib_InterpolateLinearU16 ( const uint16 * const pXPoints,
                                              const uint16 * const pYPoints,
                                              const uint16 Count,
                                              const uint16 XValue );

sint8 MathLib_InterpolateLinearS8 ( const sint8 * const pXPoints,
                                             const uint8 * const pYPoints,
                                             const uint8 Count,
                                             const sint8 XValue );

sint16 MathLib_InterpolateLinearS16 ( const sint16 * const pXPoints,
                                              const sint16 * const pYPoints,
                                              const uint16 Count,
                                              const sint16 XValue );

sint8 MathLib_InterpolateLinearU16_S8 ( const uint16 * const pXPoints,
                                                 const sint8 * const pYPoints,
                                                 const uint16 Count,
                                                 const uint16 XValue );

sint16 MathLib_InterpolateLinearU16_S16 ( const uint16 * const pXPoints,
                                                  const sint16 * const pYPoints,
                                                  const uint16 Count,
                                                  const uint16 XValue );

uint8 MathLib_InterpolateLinearU16_U8 ( const uint16 * const pXPoints,
                                                 const uint8 * const pYPoints,
                                                 const uint16 Count,
                                                 const uint16 XValue );

uint16 MathLib_InterpolateLinearPoint ( const SPoint * const pCoordPoints,
                                                const uint16 Count,
                                                const uint16 XValue );

// ---------------------------------------------------------------------------------------------------------------------
// UML::IMathLibCalculation
// ---------------------------------------------------------------------------------------------------------------------
uint32 MathLib_Abs( const sint32 Value );
uint32 MathLib_AbsDelta( const uint32 Value1, const uint32 Value2 );
uint32 MathLib_AbsDeltaS32( const sint32 Value1, const sint32 Value2 );
uint32 MathLib_DivAndRound( const uint32 Divident, const uint32 Divisor,  const ERoundingMethods RoundMethod );
sint32 MathLib_DivAndRoundS32( sint32 const Dividend, sint32 const Divisor, const ERoundingMethods RoundMethod );
sint32 MathLib_DivAndRoundS32ToU32( sint32 const Dividend, uint32 const Divisor, const ERoundingMethods RoundMethod );
uint32 MathLib_Round( const uint32 InputValue, const uint32 RoundingBase, const ERoundingMethods RoundMethod );
sint32 MathLib_RoundS( const sint32 InputValue, const uint32 RoundingBase, const uint32 Offset, const ERoundingMethods RoundMethod );
uint32 MathLib_Sqrt( const uint32 Value );

// ---------------------------------------------------------------------------------------------------------------------
// UML::IMathLibConversion
// ---------------------------------------------------------------------------------------------------------------------
sint32 MathLib_CelsToFahrRes( const sint32 Celsius, const sint8 Resolution );

Std_ReturnType MathLib_ConvertDistance( const uint32 Meters, const EDistanceUnits OutputUnit, const sint8 OutputRes,
                                        const ERoundingMethods RoundMethod, uint32 * const pOutput );

Std_ReturnType MathLib_ConvertVolume( const uint32 Mililiters, const EVolumeUnits OutputUnit, const sint8 OutputRes,
                                      const ERoundingMethods RoundMethod, uint32 * pOutput );

Std_ReturnType MathLib_ConvertWeight( const uint32 Grams, const EWeightUnits OutputUnit, const sint8 OutputRes,
                                      const ERoundingMethods RoundMethod, uint32 * const pOutput );

sint8 MathLib_AsciiToBcd( const uint8 * const pAsciiArray, uint8 * const pBcdValue );
sint8 MathLib_BcdToAscii( const uint8 BcdValue, uint8 * const pAsciiArray );
uint8 MathLib_GetDigitCount( const uint32 Number );
uint8 MathLib_GetBitCount_U8( const uint8 Number );
uint8 MathLib_GetBitCount_U16( const uint16 Number );
uint8 MathLib_GetBitCount_U32( const uint32 Number );

// ---------------------------------------------------------------------------------------------------------------------
// UML::IMathLibFilter
// ---------------------------------------------------------------------------------------------------------------------
// PT1 Filter
Std_ReturnType MathLib_PT1Filter_Init( SPt1FilterData * const pPt1Filter, const uint16 DampingConst, const uint16 InitValue );
uint16         MathLib_PT1Filter_Apply( SPt1FilterData * const pPt1Filter, const uint16 ValueToFilter );
Std_ReturnType MathLib_PT1Filter_GetStatus( const SPt1FilterData * const pPt1Filter );

// MoveAvgFilter
Std_ReturnType MathLib_MoveAvgFilter_Init( SMoveAvgFilter * const pFilterData, uint16 * const pMArrayData, const uint16 MArrayLen, const uint16 InitValue );
uint16         MathLib_MoveAvgFilter_Apply( SMoveAvgFilter * const pFilterData, const uint16 ValueToFilter );
Std_ReturnType MathLib_MoveAvgFilter_GetStatus( const SMoveAvgFilter * const pFilterData );


// MoveAvgFilter 32 bit
Std_ReturnType MathLib_MoveAvgFilter_32bit_Init( SMoveAvgFilter_32 * const pFilterData, uint32 * const pMArrayData, const uint16 MArrayLen, const uint32 InitValue );
uint32         MathLib_MoveAvgFilter_Apply_32( SMoveAvgFilter_32 * const pFilterData, const uint32 ValueToFilter );
Std_ReturnType MathLib_MoveAvgFilter_GetStatus_32( const SMoveAvgFilter_32 * const pFilterData );

// ---------------------------------------------------------------------------------------------------------------------
// UML::IMathLibSearch
// ---------------------------------------------------------------------------------------------------------------------

// Compare functions
sint Compare_S8 ( void const * const pElem1, void const * const pElem2 );
sint Compare_S16( void const * const pElem1, void const * const pElem2 );
sint Compare_S32( void const * const pElem1, void const * const pElem2 );
sint Compare_U8 ( void const * const pElem1, void const * const pElem2 );
sint Compare_U16( void const * const pElem1, void const * const pElem2 );
sint Compare_U32( void const * const pElem1, void const * const pElem2 );

sint8 MathLib_LinearSearch( void * const pArray,
                            const uint32 ArraySize,
                            void const * const pSearchValue,
                            uint32 * const pPositionFound,
                            const uint32 TypeSize,
                            const CompareFunc pCompareFunction );

// uint8 MathLib_Table_RowMatch ( STable * const pTAble ); // TODO after review

#ifdef __cplusplus
}
#endif

#endif // MATHLIB_H_
