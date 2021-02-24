/**
  * FILE MDR32F9Qx_eeprom.c
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_eeprom.h"


#define ASSERT_INFO_FILE_ID FILEID__MDR32F9X_EEPROM_C

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @defgroup EEPROM EEPROM
  * @{
  */

/** @defgroup EEPROM_Private_Defines EEPROM Private Defines
  * @{
  */

#define EEPROM_REG_ACCESS_KEY           ((uint32_t)0x8AAA5551)

/** @} */ /* End of group EEPROM_Private_Defines */

/** @defgroup EEPROM_Private_Macros EEPROM Private Macros
  * @{
  */

#define IS_TWO_BYTE_ALLIGNED(ADDR)      (((ADDR) & 1) == 0)
#define IS_FOUR_BYTE_ALLIGNED(ADDR)     (((ADDR) & 3) == 0)

#define DELAY_LOOP_CYCLES               (8UL)
#define GET_US_LOOPS(N)                 ((uint32_t)((float)(N) * FLASH_PROG_FREQ_MHZ / DELAY_LOOP_CYCLES))

/** @} */ /* End of group EEPROM_Private_Macros */

/** @defgroup EEPROM_Private_Functions EEPROM Private Functions
  * @{
  */

__RAMFUNC static uint32_t _PointerReadData(uint32_t Address, uint32_t Shift) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC static void ProgramDelay(uint32_t Loops) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));


/**
  * @brief  Reads memory using pointer to address. For internal use only.
  * @param  Address: memory address.
  * @param  Shift: additional shift if reads halfword or byte.
  * @retval Memory value from specific address
  */
__RAMFUNC static uint32_t _PointerReadData(uint32_t Address, uint32_t Shift){
  uint32_t Data;
  Data = *(uint32_t*) (Address & ~0x3); // Read 4-byte aligned word
  Data >>= Shift; // Shift data. Unwanted bytes will be removed when converting types outside of this function
  return Data;
}

/**
  * @brief  Program delay.
  * @param  Loops: Number of the loops.
  * @retval None.
  */
__RAMFUNC static void ProgramDelay(uint32_t Loops)
{
  volatile uint32_t i = Loops;
  for (; i > 0; i--)
  {
  }
}

#ifdef USE_MDR1901VC1T

/**
  * @brief  Sets instructions and/or data cache usage.
  * @param  EEPROM_CacheType: specifies the cache type.
  *          This parameter can be one of the following values:
  *            @arg EEPROM_InstructionsCache: Instructions Cache
  *            @arg EEPROM_DataCache: Data Cache
  *            @arg EEPROM_All_Cache: Instructions and Data Cache
  * @param  NewState: specifies the state of cache.
  *          This parameter can be one of the following values:
  *            @arg ENABLE: Enable cache usage
  *            @arg DISABLE: Disable cache usage
  * @retval None
  */
void EEPROM_SetCacheState(uint32_t EEPROM_CacheType, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_EEPROM_CACHE(EEPROM_CacheType));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(NewState == ENABLE)
  {
    MDR_EEPROM->CTRL |= EEPROM_CacheType;
  }
  else
  {
    MDR_EEPROM->CTRL &= ~EEPROM_CacheType;
  }
  
}

/**
  * @brief  Gets instructions and/or data cache usage.
  * @param  None
  * @retval EEPROM_CacheType: enabled cache type.
  *          This can be one of the following values:
  *            @arg EEPROM_InstructionsCache: Instructions Cache is used
  *            @arg EEPROM_DataCache: Data Cache is used
  *            @arg EEPROM_All_Cache: Instructions and Data Cache are used
  */
uint32_t EEPROM_GetCacheState()
{
  return (uint32_t) (MDR_EEPROM->CTRL & EEPROM_Cache_Msk);
}

#endif

/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *          This parameter can be one of the following values:
  *            @arg EEPROM_Latency_0: EEPROM Zero Latency cycle
  *            @arg EEPROM_Latency_1: EEPROM One Latency cycle
  *            @arg EEPROM_Latency_2: EEPROM Two Latency cycles
  *            @arg EEPROM_Latency_3: EEPROM Three Latency cycles
  *            @arg EEPROM_Latency_4: EEPROM Four Latency cycles
  *            @arg EEPROM_Latency_5: EEPROM Five Latency cycles
  *            @arg EEPROM_Latency_6: EEPROM Six Latency cycles
  *            @arg EEPROM_Latency_7: EEPROM Seven Latency cycles
  * @retval None
  */
void EEPROM_SetLatency(uint32_t EEPROM_Latency)
{
  /* Check the parameters */
  assert_param(IS_EEPROM_LATENCY(EEPROM_Latency));

  /* Set the new latency value */
  MDR_EEPROM->CMD = EEPROM_Latency;
}

/**
  * @brief  Gets the code latency value.
  * @param  None
  * @retval  FLASH Latency value.
  *          This can be one of the following values:
  *            @val EEPROM_Latency_0: EEPROM Zero Latency cycle
  *            @val EEPROM_Latency_1: EEPROM One Latency cycle
  *            @val EEPROM_Latency_2: EEPROM Two Latency cycles
  *            @val EEPROM_Latency_3: EEPROM Three Latency cycles
  *            @val EEPROM_Latency_4: EEPROM Four Latency cycles
  *            @val EEPROM_Latency_5: EEPROM Five Latency cycles
  *            @val EEPROM_Latency_6: EEPROM Six Latency cycles
  *            @val EEPROM_Latency_7: EEPROM Seven Latency cycles
  */
uint32_t EEPROM_GetLatency()
{
  /* Get current latency value */
  return (uint32_t) (MDR_EEPROM->CMD & EEPROM_Latency_MSK) >> EEPROM_CMD_DELAY_Pos;
}


/**
  * @brief  Reads the 8-bit EEPROM memory value.
  *         Uses memory reading by pointer to address if EEPROM Main_Bank_Select is selected.
  * @param  Address: The EEPROM memory byte address.
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint8_t EEPROM_ReadByte(uint32_t Address, uint32_t BankSelector)
{
  uint32_t Data;
  uint32_t Command;
  uint32_t Shift;
  
  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

  /* Use memory reading by pointer to address if EEPROM Main_Bank_Select is selected.*/
  if (BankSelector == EEPROM_Main_Bank_Select){
    if((MDR_EEPROM->CMD & EEPROM_CMD_CON) == 0){
      // EEPROM control from the core, working mode.
      Data = _PointerReadData(Address, (Address & 3) * 8);
    }else{
      /* Control from registers, programming mode.*/
      /* Turn off programming mode. */
      MDR_EEPROM->CMD &= ~EEPROM_CMD_CON;
      Data = _PointerReadData(Address, (Address & 3) * 8);
      /* Turn on programming mode back. */
      MDR_EEPROM->CMD |= EEPROM_CMD_CON;
    }
    return (uint8_t)Data;
  }

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
  Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->ADR = Address;
  MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  Data = MDR_EEPROM->DO;
  Command &= EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->KEY = 0;

  Shift = (Address & 3) * 8;
  Data >>= Shift;
  return (uint8_t)Data;
}

/**
  * @brief  Reads the 16-bit EEPROM memory value.
  *         Uses memory reading by pointer to address if EEPROM Main_Bank_Select is selected.
  * @param  Address: The EEPROM memory half word address (two byte aligned).
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint16_t EEPROM_ReadHalfWord(uint32_t Address, uint32_t BankSelector)
{
  uint32_t Data;
  uint32_t Command;
  uint32_t Shift;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
  assert_param(IS_TWO_BYTE_ALLIGNED(Address));

  /* Use memory reading by pointer to address if EEPROM Main_Bank_Select is selected.*/
  if (BankSelector == EEPROM_Main_Bank_Select){
    if((MDR_EEPROM->CMD & EEPROM_CMD_CON) == 0){
      // EEPROM control from the core, working mode.
      Data = _PointerReadData(Address, (Address & 2) * 8);
    }else{
      /* Control from registers, programming mode.*/
      /* Turn off programming mode. */
      MDR_EEPROM->CMD &= ~EEPROM_CMD_CON;
      Data = _PointerReadData(Address, (Address & 2) * 8);
      /* Turn on programming mode back. */
      MDR_EEPROM->CMD |= EEPROM_CMD_CON;
    }
    return (uint16_t)Data;
  }

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
  Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->ADR = Address;
  MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  Data = MDR_EEPROM->DO;
  Command &= EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->KEY = 0;

  Shift = (Address & 2) * 8;
  Data >>= Shift;
  return (uint16_t)Data;
}

/**
  * @brief  Reads the 32-bit EEPROM memory value.
  *         Uses memory reading by pointer to address if EEPROM Main_Bank_Select is selected.
  * @param  Address: The EEPROM memory word address (four byte aligned).
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint32_t EEPROM_ReadWord(uint32_t Address, uint32_t BankSelector)
{
  uint32_t Command;
  uint32_t Data;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
  assert_param(IS_FOUR_BYTE_ALLIGNED(Address));

  /* Use memory reading by pointer to address if EEPROM Main_Bank_Select is selected.*/
  if (BankSelector == EEPROM_Main_Bank_Select){
    if((MDR_EEPROM->CMD & EEPROM_CMD_CON) == 0){
      // EEPROM control from the core, working mode.
      Data = *(uint32_t*) (Address);
    }else{
      /* Control from registers, programming mode.*/
      /* Turn off programming mode. */
      MDR_EEPROM->CMD &= ~EEPROM_CMD_CON;
      Data = *(uint32_t*) (Address);
      /* Turn on programming mode back. */
      MDR_EEPROM->CMD |= EEPROM_CMD_CON;
    }
    return Data;
  }

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
  Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->ADR = Address;
  MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  MDR_EEPROM->DO;   /* Idle Reading for Delay */
  Data = MDR_EEPROM->DO;
  Command &= EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->KEY = 0;
  return Data;
}

/**
  * @brief  Erases one page of the selected EEPROM memory bank.
  * @param  Address: Page Address in the EEPROM memory.
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @retval None
  */
__RAMFUNC void EEPROM_ErasePage(uint32_t Address, uint32_t BankSelector)
{
  uint32_t Command;
  uint32_t Offset;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
  Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
  MDR_EEPROM->CMD = Command;

  for (Offset = 0; Offset < (4 << 2); Offset += 4)
  {
    MDR_EEPROM->ADR = Address + Offset;             /* Page Address */
    MDR_EEPROM->DI = 0;
    Command |= EEPROM_CMD_XE | EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));              /* Wait for 5 us */
    Command |= EEPROM_CMD_NVSTR;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(40000));          /* Wait for 40 ms */
    Command &= ~EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));              /* Wait for 5 us */
    Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(1));              /* Wait for 1 us */
  }
  Command &= EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->KEY = 0;
  ProgramDelay(GET_US_LOOPS(4000));               /* Wait for 4 ms */
}

/**
  * @brief  Erases all pages of the selected EEPROM memory bank.
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_All_Banks_Select:      The EEPROM All Banks selector.
  * @retval None
  */
__RAMFUNC void EEPROM_EraseAllPages(uint32_t BankSelector)
{
  uint32_t Command;
  uint32_t Offset;

  assert_param(IS_EEPROM_ERASE_SELECTOR(BankSelector));

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
  Command |= (BankSelector == EEPROM_All_Banks_Select) ? EEPROM_CMD_IFREN : 0;

  MDR_EEPROM->CMD = Command;

  for (Offset = 0; Offset < (4 << 2); Offset += 4)
  {
    MDR_EEPROM->ADR = Offset;
    MDR_EEPROM->DI = 0;
    Command |= EEPROM_CMD_XE | EEPROM_CMD_MAS1 | EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));                /* Wait for 5 us */
    Command |= EEPROM_CMD_NVSTR;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(40000));            /* Wait for 40 ms */
    Command &= ~EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(100));              /* Wait for 100 us */
    Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_MAS1 | EEPROM_CMD_NVSTR);
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(1));                /* Wait for 1 us */
  }
  Command &= EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->KEY = 0;
  ProgramDelay(GET_US_LOOPS(4000));               /* Wait for 4 ms */
}

/**
  * @brief  Programs the 8-bit EEPROM memory value.
  * @param  Address: The EEPROM memory byte address.
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramByte(uint32_t Address, uint32_t BankSelector, uint32_t Data)
{
  uint32_t Mask;
  uint32_t Tmp;
  uint32_t Shift;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

  Shift = (Address & 3) * 8;
  Data = Data << Shift;
  Mask = 0xFF << Shift;
  Tmp = EEPROM_ReadWord(Address, BankSelector);
  EEPROM_ProgramWord(Address, BankSelector, (Tmp & ~Mask) | (Data & Mask));
}

/**
  * @brief  Programs the 16-bit EEPROM memory value.
  * @param  Address: The EEPROM memory half word address (two byte aligned).
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramHalfWord(uint32_t Address, uint32_t BankSelector, uint32_t Data)
{
  uint32_t Mask;
  uint32_t Tmp;
  uint32_t Shift;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
  assert_param(IS_TWO_BYTE_ALLIGNED(Address));

  Shift = (Address & 2) * 8;
  Data = Data << Shift;
  Mask = 0xFFFF << Shift;
  Tmp = EEPROM_ReadWord(Address, BankSelector);
  EEPROM_ProgramWord(Address, BankSelector, (Tmp & ~Mask) | (Data & Mask));
}

/**
  * @brief  Programs the 32-bit EEPROM memory value.
  * @param  Address: The EEPROM memory word address (four byte aligned).
  * @param  BankSelector: Selects EEPROM Bank (Main Bank or Information Bank).
  *         This parameter can be one of the following values:
  *           @arg EEPROM_Main_Bank_Select:      The EEPROM Main Bank selector.
  *           @arg EEPROM_Info_Bank_Select:      The EEPROM Information Bank selector.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramWord(uint32_t Address, uint32_t BankSelector, uint32_t Data)
{
  uint32_t Command;

  assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
  assert_param(IS_FOUR_BYTE_ALLIGNED(Address));

  MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
  BankSelector = (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
  Command = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
  Command |= EEPROM_CMD_CON | BankSelector;
  MDR_EEPROM->CMD = Command;
  MDR_EEPROM->ADR = Address;
  MDR_EEPROM->DI  = Data;
  Command |= EEPROM_CMD_XE | EEPROM_CMD_PROG;
  MDR_EEPROM->CMD = Command;
  ProgramDelay(GET_US_LOOPS(5));                /* Wait for 5 us */
  Command |= EEPROM_CMD_NVSTR;
  MDR_EEPROM->CMD = Command;
  ProgramDelay(GET_US_LOOPS(10));               /* Wait for 10 us */
  Command |= EEPROM_CMD_YE;
  MDR_EEPROM->CMD = Command;
  ProgramDelay(GET_US_LOOPS(40));               /* Wait for 40 us */
  Command &= ~EEPROM_CMD_YE;
  MDR_EEPROM->CMD = Command;
  Command &= ~EEPROM_CMD_PROG;
  MDR_EEPROM->CMD = Command;
  ProgramDelay(GET_US_LOOPS(5));                /* Wait for 5 us */
  Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
  MDR_EEPROM->CMD = Command;
  ProgramDelay(GET_US_LOOPS(1));                /* Wait for 1 us */

  MDR_EEPROM->CMD = Command & EEPROM_CMD_DELAY_Msk;
  MDR_EEPROM->KEY = 0;
}

/** @} */ /* End of group EEPROM_Private_Functions */

/** @} */ /* End of group EEPROM */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

/*
*
* END OF FILE MDR32F9Qx_eeprom.c */

