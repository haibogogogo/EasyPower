/**
*************************************************************************************************
 * \fn uint32 UTIL_Helper_Conv4xU8ToU32(uint8 u8HighestByte, uint8 u8SecondHighestByte, 
 * 											uint8 u8SecondLowestByte, uint8 u8LowestByte)
 * \brief This function converts 4 Byte of data into a data of one DWord
 *
 * \param[in] uint8 u8HighestByte 			- Highest byte of 32bit data
 * \param[in] uint8 u8SecondHighestByte  	- 2nd Highest byte of 32bit data
 * \param[in] uint8 u8SecondLowestByte 		- 2nd Lowest byte of 32bit data
 * \param[in] uint8 u8LowestByte  			- Lowest byte of 32bit data
 * \return uint32
 *
 ***************************************************************************************************
*/
uint32 UTIL_Helper_Conv4xU8ToU32(uint8 u8HighestByte, uint8 u8SecondHighestByte, 
									uint8 u8SecondLowestByte, uint8 u8LowestByte)
{
  return( ((uint32)u8HighestByte << 24) +
          ((uint32)u8SecondHighestByte << 16) +
          ((uint32)u8SecondLowestByte << 8) +
          ((uint32)u8LowestByte) );
}

/**
*************************************************************************************************
 * \fn void UTIL_Helper_Conv1xU16To2xU8(uint16  u16Word, uint8 * u8HighByte, uint8 * u8LowByte)
 * \brief This function converts a Word of data into 2 bytes of data
 *
 * \param[in] 	uint16 u16Word 		- 16bit data be seperated into 2 bytes
 * \param[out]  uint8* u8HighByte  	- Higher byte of data
 * \param[out]  uint8* u8LowByte  	- lower byte of data
 * \return void
 *
 ***************************************************************************************************
*/
void UTIL_Helper_Conv1xU16To2xU8(uint16  u16Word, uint8 * u8HighByte, uint8 * u8LowByte)
{
  *u8HighByte = (uint8)((u16Word & 0xFF00u) >> 8);
  *u8LowByte  = (uint8)((u16Word & 0x00FFu) );
}

/**
*************************************************************************************************
 * \fn void UTIL_Helper_Conv1xU32To4xU8(uint32  u32Word, uint8 * u8HighestByte, 
 *					uint8 * u8SecondHighestByte, uint8 * u8SecondLowestByte, uint8 * u8LowestByte)
 * \brief This function converts a DWord of data into 4 bytes of data
 *
 * \param[in] 	uint32 u16Word 				- 32bit data be seperated into 4 bytes
 * \param[out]  uint8* u8HighestByte  		- Highest byte of 32bit data
 * \param[out]  uint8* u8SecondHighestByte  - 2nd Highest byte of 32bit data
 * \param[out]  uint8* u8SecondLowestByte  	- 2nd Lowest byte of 32bit data
 * \param[out]  uint8* u8LowestByte  		- Lowest byte of 32bit data
 * \return void
 *
 ***************************************************************************************************
*/
void UTIL_Helper_Conv1xU32To4xU8(uint32  u32Word, uint8 * u8HighestByte, 
						uint8 * u8SecondHighestByte, uint8 * u8SecondLowestByte, uint8 * u8LowestByte)
{
  *u8HighestByte       = (uint8)((u32Word & 0xFF000000u) >> 24);
  *u8SecondHighestByte = (uint8)((u32Word & 0x00FF0000u) >> 16);
  *u8SecondLowestByte  = (uint8)((u32Word & 0x0000FF00u) >> 8);
  *u8LowestByte        = (uint8)((u32Word & 0x000000FFu) );
}

 /**
 *************************************************************************************************
 * \fn void Appl_DltUtils_GetRegisterValueRaw(uint8 *PduDataPtr, uint16 PduLength)
 * \brief Fetch the register value at the specified address.
 * \par Message should consist of the following bytes:
 *  -  byte 0:        REGISTER VALUE LENGTH (1, 2 or 4 bytes)
 *  -  bytes 1,2,3,4: REGISTER ADDRESS (written in big endian)
 *
 * @param[in]       PduDataPtr,PduLength
 *
**************************************************************************************************/
void Appl_DltUtils_GetRegisterValueRaw(uint8 *PduDataPtr,
                                       uint16 PduLength)
{

   if (PduLength == APPL_UTILS_MAX_RAW_LENGTH)
   {
      /* left intentionally uninitialised because of LINT :-( !!! */
      /* --> Info 838: Previously assigned value to variable 'RegisterValue' has not been used */
      uint32  RegisterValue /*= 0x00000000u*/;

      /* Get the byte holding the register value length */
      Appl_DltUtils_RegLen  ValueLength = (Appl_DltUtils_RegLen)
                                    PduDataPtr[APPL_UTILS_VALUE_LENGTH_IDX];

      /* The assembled 32 bit address */
      uint32  AddressValue = UTIL_Helper_Conv4xU8ToU32(
                                  PduDataPtr[APPL_UTILS_VALUE_START_IDX + 0],
                                  PduDataPtr[APPL_UTILS_VALUE_START_IDX + 1],
                                  PduDataPtr[APPL_UTILS_VALUE_START_IDX + 2],
                                  PduDataPtr[APPL_UTILS_VALUE_START_IDX + 3]);

      /* Read out the register value */
      RegisterValue = Appl_DltUtils_GetRegisterValue(AddressValue,
                                                     ValueLength);

      /* Send the DLT message with the read out register value */
      Appl_Dlt_TraceVariable(Appl_DltApplId_4chars,
                             Appl_DltContextId_4chars,
                             APPL_DLT_TRACE_HEX,
                             (uint8)(strlen(Appl_DltUtils_DltVarName) + 1),
                             (const uint8 *)Appl_DltUtils_DltVarName,
                             RegisterValue);
   }

}

/**
 *************************************************************************************************
 * \fn uint32 Appl_DltUtils_GetRegisterValue(uint32 RegisterAddressAsUint32, 
 *                                           Appl_DltUtils_RegLen ValueLength)
 * \brief  Fetch the register value at the specified address.
 *
 * @param[in]       RegisterAddressAsUint32, ValueLength
 *
**************************************************************************************************/
uint32 Appl_DltUtils_GetRegisterValue(uint32  RegisterAddressAsUint32,
                                      Appl_DltUtils_RegLen  ValueLength)
{
  volatile uint32 RegisterValue = 0x00000000u;

  /* get the value from the specified address depending on the value length */
  switch (ValueLength)
  {
    case REGISTER_LENGTH_IN_BYTES_IS_1:
    {
      RegisterValue = *((volatile uint8 *)RegisterAddressAsUint32);
      break;
    }
    case REGISTER_LENGTH_IN_BYTES_IS_2:
    {
      RegisterValue = *((volatile uint16 *)RegisterAddressAsUint32);
      break;
    }
    case REGISTER_LENGTH_IN_BYTES_IS_4:
    {
      RegisterValue = *((volatile uint32 *)RegisterAddressAsUint32);
      break;
    }
    default:
    {
      RegisterValue = *((volatile uint32 *)RegisterAddressAsUint32);
      break;
    }
  }

#if 0  /* kept for testing */
  /* Send the DLT message with the read out register value */
  Appl_Dlt_TraceVariable(Appl_DltApplId_4chars,
                         Appl_DltContextId_4chars,
                         APPL_DLT_TRACE_HEX,
                         (uint8)(strlen(Appl_DltUtils_DltVarName) + 1),
                         (const uint8 *)Appl_DltUtils_DltVarName,
                         RegisterValue);
#endif

  return RegisterValue;
}

/**
 *************************************************************************************************
 * \fn uint32 Appl_Dlt_InjectionDispatcher(const uint8 * EcuId, const uint8 * SessionId,
 *                                         const uint8 * AppId, const uint8 * ContextId,
 *                                         uint32        ServiceId,   uint32  InjDataLength,
 *                                         const uint8 * InjData)
 * \brief  Injection Dispatcher.
 *
 * @param[in] EcuId, SessionId, AppId, ContextId, ServiceId, InjDataLength, InjData
 *
**************************************************************************************************/
Dlt_ResponseType Appl_Dlt_InjectionDispatcher(const uint8 * EcuId,
                                              const uint8 * SessionId,
                                              const uint8 * AppId,
                                              const uint8 * ContextId,
                                              uint32        ServiceId,
                                              uint32        InjDataLength,
                                              const uint8 * InjData)
{

  Dlt_ResponseType  ResponseStatus = Dlt_NOT_SUPPORTED;

  /* satisfy LINT and "use" the passed parameters */
  PARAM_UNUSED(EcuId);
  PARAM_UNUSED(SessionId);

  if ((uint32)Dlt_SessionId_Appl_Dlt_Core == ServiceId)
  {
    /* TO DO: add the real handling */
    uint32  AppId_32bit =
                  UTIL_Helper_Conv4xU8ToU32(AppId[0],
                                              AppId[1],
                                              AppId[2],
                                              AppId[3]);

    uint32  ContextId_32bit =
                      UTIL_Helper_Conv4xU8ToU32(ContextId[0],
                                                  ContextId[1],
                                                  ContextId[2],
                                                  ContextId[3]);

    volatile uint32  ByteCntr = 0x00u;

    volatile uint8   InjCmd[APPL_DLT_IC_CMD_MAX_LENGTH] = { 0 };

    /* Our DLT Light is mentioned */
    if ( (AppId_32bit == Appl_DltApplId_32bit)
         &&
         (ContextId_32bit == Appl_DltApplId_32bit)
       )
    {
      /* extract the sent 'command' */
      while ( (InjData[ByteCntr] != (uint8)',')
              &&
              (ByteCntr < APPL_DLT_IC_CMD_MAX_LENGTH)
              &&
              (ByteCntr < InjDataLength)
            )
      {
        InjCmd[ByteCntr] = InjData[ByteCntr];
        ByteCntr++;
      }

    }

  }

  return ResponseStatus;
}