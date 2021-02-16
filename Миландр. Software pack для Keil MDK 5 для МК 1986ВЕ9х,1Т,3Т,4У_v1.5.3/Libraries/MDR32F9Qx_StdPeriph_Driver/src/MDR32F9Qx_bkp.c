/**
  * FILE MDR32F9Qx_bkp.c
  */
/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_bkp.h"
#include "MDR32F9Qx_rst_clk.h"


#define ASSERT_INFO_FILE_ID FILEID__MDR32F9X_BKP_C

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @defgroup BKP BKP
  * @{
  */

/** @defgroup BKP_Private_TypesDefinitions BKP Private Types Definitions
  * @{
  */

/** @} */ /* End of group BKP_Private_TypesDefinitions */

/** @defgroup BKP_Private_Defines BKP Private Defines
  * @{
  */

/* BKP registers bit address in the alias region */
#define PERIPH_BASE                 0x40000000
#define PERIPH_BB_BASE              0x42000000
#define BKP_OFFSET                 (MDR_BKP_BASE - PERIPH_BASE)

#define SFR_OFFSET(TP, MOD, SFR)    ((uint32_t)&((TP*)MOD)->SFR)
#define BB_ADDR(TP, MOD, SFR, BIT)  (PERIPH_BB_BASE + SFR_OFFSET(TP, MOD, SFR)*32 + BIT*4)
#define BKP_BB(SFR, BIT)            BB_ADDR(MDR_BKP_TypeDef, BKP_OFFSET, SFR, BIT)

#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	#define BKP_JTAGA_BB                BKP_BB(REG_0E, BKP_REG_0E_JTAGA_Pos)
	#define BKP_JTAGB_BB                BKP_BB(REG_0E, BKP_REG_0E_JTAGB_Pos)
	#define BKP_FPOR_BB                 BKP_BB(REG_0E, BKP_REG_0E_FPOR_Pos)
	#define BKP_STANDBY_BB              BKP_BB(REG_0F, BKP_REG_0F_STANDBY_Pos)
	#define RTC_ENABLE_BB               BKP_BB(REG_0F, BKP_REG_0F_RTC_EN_Pos)
	#define RTC_RESET_BB                BKP_BB(REG_0F, BKP_REG_0F_RTC_RESET_Pos)
	#define RTC_WEC_BB                  BKP_BB(RTC_CS, BKP_RTC_CS_WEC_Pos)
#endif
/* --------------------- BKP registers bit mask ------------------------ */

/* BKP_REG0E register bit mask */
#define DUcc_Mask         ((uint32_t)(BKP_REG_0E_LOW_Msk | BKP_REG_0E_SELECTRI_Msk))
#define DUccTrim_Mask     ((uint32_t)BKP_REG_0E_TRIM_Msk)
#define BKP_REG_0E_ON     ((uint32_t)(0x01U<<11))
#define BKP_REG_0E_OFF    ((uint32_t)(~((0x01U<<31)|(0x07U<<8)|(0x3FU))))

/* CFR register bit mask */
#define WDGTB_Mask        ((uint32_t)0xFFFFFE7F)
#define WIN_Mask          ((uint32_t)0x7F)
#define CTR_Mask          ((uint32_t)0x7F)

/** @} */ /* End of group BKP_Private_Defines */

/** @defgroup BKP_Private_Macros BKP Private Macros
  * @{
  */

/** @} */ /* End of group BKP_Private_Macros */

/** @defgroup BKP_Private_Variables BKP Private Variables
  * @{
  */

/** @} */ /* End of group BKP_Private_Variables */

/** @defgroup BKP_Private_Functions BKP Private Functions
  * @{
  */

/**
  * @brief  BKP_DeInit - Deinitializes the BKP peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void BKP_DeInit(void)
{
  #define BKP_RTC_CS_NUM ((uint32_t) (SFR_OFFSET(MDR_BKP_TypeDef, BKP_OFFSET, RTC_CS) - BKP_OFFSET)/4)
  #define BKP_REG_0F_NUM ((uint32_t) (SFR_OFFSET(MDR_BKP_TypeDef, BKP_OFFSET, REG_0F) - BKP_OFFSET)/4)
  #define BKP_REG_0E_NUM ((uint32_t) (SFR_OFFSET(MDR_BKP_TypeDef, BKP_OFFSET, REG_0E) - BKP_OFFSET)/4)
  #define BKP_RTC_CNT_NUM ((uint32_t) (SFR_OFFSET(MDR_BKP_TypeDef, BKP_OFFSET, RTC_CNT) - BKP_OFFSET)/4)

  uint32_t * _bkp = (uint32_t *) MDR_BKP_BASE;
  uint32_t i;

  for ( i = 0; i < BKP_REG_0E_NUM; i++)
  {
    _bkp[i] = 0;
  }

  for ( i = BKP_RTC_CNT_NUM; i <= BKP_RTC_CS_NUM; i++)
  {
    _bkp[i] = 0;
  }

  MDR_BKP->REG_0E |= (uint32_t) (BKP_REG_0E_ON);
  MDR_BKP->REG_0E &= (uint32_t) (BKP_REG_0E_OFF);
  MDR_BKP->REG_0F = (uint32_t) (BKP_REG_0F_LSI_ON | BKP_REG_0F_HSI_ON);
}

#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
/**
  * @brief  Enables or disables the JTAG A.
  * @param  NewState: new state of the JTAG A.
  *         @arg ENABLE
  *         @arg DISABLE
  * @retval None
  */
void BKP_JTAGA_CMD(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) BKP_JTAGA_BB = (uint32_t)NewState;
}


/**
  * @brief  Enables or disables the JTAG B.
  * @param  NewState: new state of the JTAG B.
  *         @arg ENABLE
  *         @arg DISABLE
  * @retval None
  */
void BKP_JTAGB_CMD(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) BKP_JTAGB_BB = (uint32_t)NewState;
}
#endif

/**
  * @brief  BKP_RTCclkSource - Configures the RTC clock source.
  * @param  RTC_CLK: specifies the RTC clock source.
  *         @arg BKP_RTC_LSIclk: LSI oscillator clock is selected as RTC clock source.
  *         @arg BKP_RTC_LSEclk: LSE oscillator clock is selected as RTC clock source.
  *         @arg BKP_RTC_HSIclk: HSI oscillator clock is selected as RTC clock source.
  *         @arg BKP_RTC_HSEclk: HSE oscillator clock is selected as RTC clock source.
  * @retval None
  */
void BKP_RTCclkSource(uint32_t RTC_CLK)
{
	uint32_t tmpreg;

	/* Check the parameters */
	assert_param(IS_RTC_CLK_SOURCE(RTC_CLK));
	/* Clear BKP_REG0F[3:2] bits */
	tmpreg = MDR_BKP->REG_0F & (uint32_t) (~BKP_REG_0F_RTC_SEL_Msk );
	/* Set BKP_REG0F[3:2] bits according to RTC clock source*/
	tmpreg |= BKP_REG_0F_RTC_SEL_Msk & RTC_CLK;
	MDR_BKP ->REG_0F = tmpreg;

}


/**
  * @brief  BKP_RTC_Enable - Enables or disables the RTC.
  * @param  NewState: new state of the RTC.
  *         @arg ENABLE
  *         @arg DISABLE
  * @retval None
  */
void BKP_RTC_Enable ( FunctionalState NewState )
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	*(__IO uint32_t *) RTC_ENABLE_BB = (uint32_t) NewState;
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
	if(NewState != DISABLE){
		MDR_BKP->REG_0F |= BKP_REG_0F_RTC_EN;
	}
	else{
		MDR_BKP->REG_0F &= ~BKP_REG_0F_RTC_EN;
	}
#endif
}

/**
  * @brief  BKP_RTC_Calibration - Configures the RTC clock source.
  * @param  RTC_Calibration: specifies the tact drop count.
  *         @arg 0 <= RTC_Calibration <= 255.
  * @retval None
  */
void BKP_RTC_Calibration(uint32_t RTC_Calibration)
{
  uint32_t tmpreg;

  /* Check the parameters */
  assert_param(IS_RTC_CALIBRATION(RTC_Calibration));
  /* Clear BKP_REG0F[12:5] bits */
  tmpreg  = MDR_BKP -> REG_0F & (uint32_t) (~BKP_REG_0F_CAL_Msk);
  /* Set BKP_REG0F[12:5] bits according to RTC clock source*/
  tmpreg |= BKP_REG_0F_CAL_Msk & (RTC_Calibration << BKP_REG_0F_CAL_Pos);
  MDR_BKP -> REG_0F = tmpreg;
}


/**
  * @brief  BKP_RTC_Reset -  Reset the RTC.
  * @param  NewState: new state of the RTC.
  *         @arg ENABLE  : Reset the RTC
  *         @arg DISABLE : Not reset the RTC
  * @retval None
  */
void BKP_RTC_Reset ( FunctionalState NewState )
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	*(__IO uint32_t *) RTC_RESET_BB = (uint32_t) NewState;
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
	if(NewState != DISABLE){
		MDR_BKP->REG_0F |= BKP_REG_0F_RTC_RESET;
	}
	else{
		MDR_BKP->REG_0F &= ~BKP_REG_0F_RTC_RESET;
	}
#endif // #elif defined (USE_MDR1986VE3)
}

/**
  * @brief  BKP_RTC_ITConfig - Enables or disables the specified RTC interrupts.
  * @param  BKP_RTC_IT: specifies the RTC interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg BKP_RTC_IT_ALRF: Alarm interrupt enable
  *     @arg BKP_RTC_IT_SECF: Second interrupt enable
  *     @arg BKP_RTC_IT_OWF : Overflow interrupt enable
  * @param  NewState: new state of the specified RTC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_RTC_ITConfig(uint32_t BKP_RTC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RTC_CLK_IT(BKP_RTC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    MDR_BKP->RTC_CS |= BKP_RTC_IT;
  }
  else
  {
    MDR_BKP->RTC_CS &= (uint32_t)~BKP_RTC_IT;
  }
}

/**
  * @brief  BKP_RTC_GetCounter - Gets the RTC counter value.
  * @param  None
  * @retval RTC counter value.
  */
uint32_t BKP_RTC_GetCounter(void)
{
  return MDR_BKP -> RTC_CNT;
}

/**
  * @brief  BKP_RTC_SetCounter - Sets the RTC counter value.
  * @param  CounterValue: RTC counter new value.
  * @retval None
  */
void BKP_RTC_SetCounter(uint32_t CounterValue)
{
  MDR_BKP -> RTC_CNT = CounterValue;
}

/**
  * @brief  BKP_RTC_SetAlarm - Sets the RTC alarm value.
  * @param  AlarmValue: RTC alarm new value.
  * @retval None
  */
void  BKP_RTC_SetAlarm(uint32_t AlarmValue)
{
  MDR_BKP -> RTC_ALRM = AlarmValue;
}

/**
  * @brief  BKP_RTC_GetDivider - Gets the RTC divider value.
  * @param  None
  * @retval RTC Divider value.
  */
uint32_t BKP_RTC_GetDivider(void)
{
  return MDR_BKP -> RTC_DIV;
}

/**
  * @brief  BKP_RTC_SetPrescaler - Sets the RTC prescaler (RTC_DIV) preloaded value.
  * @param  PrescalerValue: RTC prescaler new value.
  *         This parameter should be less or equal then 0x000FFFFF
  * @retval None
  */
void BKP_RTC_SetPrescaler(uint32_t PrescalerValue)
{
  /* Check the parameter */
  assert_param(IS_BKP_PRESCALER_VALUE(PrescalerValue));

  MDR_BKP -> RTC_PRL = PrescalerValue;
}

/**
  * @brief  BKP_RTC_WaitForUpdate - Waits until last write operation on RTC registers has finished.
  * @note   This function must be called before any write to RTC registers.
  * @param  None
  * @retval None
  */
void BKP_RTC_WaitForUpdate ( void )
{
	/* Loop until WEC flag is set */
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	while (*(__IO uint32_t *) RTC_WEC_BB != 0);
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
	while((MDR_BKP->RTC_CS & BKP_RTC_CS_WEC) == BKP_RTC_CS_WEC);
#endif

}

/**
  * @brief  BKP_RTC_GetFlagStatus - Checks whether the specified RTC flag is set or not.
  * @param  BKP_RTC_FLAG: specifies the flag to check.
  *   This parameter can be one the following values:
  *     @arg BKP_RTC_FLAG_WEC: RTC Write operation ended flag
  *     @arg BKP_RTC_FLAG_OWF: Overflow flag
  *     @arg BKP_RTC_FLAG_ALRF: Alarm flag
  *     @arg BKP_RTC_FLAG_SECF: Second flag
  * @retval The new state of BKP_RTC_FLAG (SET or RESET).
  */
FlagStatus BKP_RTC_GetFlagStatus(uint32_t BKP_RTC_FLAG)
{
  FlagStatus status;

  /* Check the parameters */
  assert_param(IS_BKP_RTC_GET_FLAG(BKP_RTC_FLAG));

  if ((MDR_BKP->RTC_CS & BKP_RTC_FLAG) != 0)
  {
    status = SET;
  }
  else
  {
    status = RESET;
  }
  return status;
}

/**
  * @brief  BKP_RTC_ClearFlagStatus - Clears the specified RTC flag.
  * @param  BKP_RTC_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg BKP_RTC_FLAG_OWF : Overflow flag
  *     @arg BKP_RTC_FLAG_ALRF: Alarm flag
  *     @arg BKP_RTC_FLAG_SECF: Second flag
  * @retval None.
  */
void BKP_RTC_ClearFlagStatus(uint32_t BKP_RTC_FLAG)
{
  uint32_t tmpreg;
	
  /* Check the parameters */
  assert_param(IS_BKP_RTC_CLAER_FLAG(BKP_RTC_FLAG));
	
  tmpreg = MDR_BKP->RTC_CS & (~BKP_RTC_CS_Msk);
  tmpreg |= BKP_RTC_FLAG;

  MDR_BKP->RTC_CS = tmpreg;
}

/**
  * @brief  BKP_SET_DUCC_MODE - Select the internal voltage regulator mode
  * @param  DUccMode: specifies the DUcc mode.
  *   This parameter can be one of the following values:
  *     @arg BKP_DUcc_upto_10MHz:  system clock is up to 10 MHz
  *     @arg BKP_DUcc_upto_200kHz: system clock is up to 200 kHz
  *     @arg BKP_DUcc_upto_500kHz: system clock is up to 500 kHz
  *     @arg BKP_DUcc_clk_off:   all clocks are switnhed off
  *     @arg BKP_DUcc_upto_1MHz:   system clock is up to 1 MHz
  *     @arg BKP_DUcc_upto_40MHz:  system clock is up to 40 MHz
  *     @arg BKP_DUcc_upto_80MHz:  system clock is up to 80 MHz
  *     @arg BKP_DUcc_over_80MHz:  system clock is over 80 MHz
  * @retval None
  */
void BKP_DUccMode(uint32_t DUccMode)
{
  uint32_t tmpreg;
  /* Check the parameters */
  assert_param(IS_BKP_DUCC_MODE(DUccMode));
  /* Clear BKP_REG0E[5:0] bits */
  tmpreg  = MDR_BKP -> REG_0E & (uint32_t) (~DUcc_Mask);
  /* Set BKP_REG0E[5:0] bits according to DUcc mode */
  tmpreg |= DUcc_Mask & DUccMode;
 
  MDR_BKP -> REG_0E = tmpreg;
}

/**
  * @brief  BKP_SET_DUCC_TRIM - Set the internal voltage regulator trim
  * @param  DUccTrim: specifies the DUcc trim.
  *   This parameter can be one of the following values:
  *     @arg BKP_DUcc_plus_100mV:  trim DUcc with + 100mV
  *     @arg BKP_DUcc_plus_060mV:  trim DUcc with + 060mV
  *     @arg BKP_DUcc_plus_040mV:  trim DUcc with + 040mV
  *     @arg BKP_DUcc_plus_010mV:  trim DUcc with + 010mV
  *     @arg BKP_DUcc_minus_010mV: trim DUcc with - 010mV
  *     @arg BKP_DUcc_minus_040mV: trim DUcc with - 040mV
  *     @arg BKP_DUcc_minus_060mV: trim DUcc with - 060mV
  *     @arg BKP_DUcc_minus_100mV: trim DUcc with - 100mV
  * @retval None
  */
void BKP_DUccTrim(uint32_t DUccTrim)
{
  uint32_t tmpreg;
  /* Check the parameters */
  assert_param(IS_BKP_DUCC_TRIM(DUccTrim));
  /* Clear BKP_REG0E[5:0] bits */
  tmpreg  = MDR_BKP -> REG_0E & (uint32_t) (~DUccTrim_Mask);
  /* Set BKP_REG0E[5:0] bits according to DUcc mode */
  tmpreg |= DUccTrim_Mask & DUccTrim;
  
    MDR_BKP -> REG_0E = tmpreg;
}

/**
  * @brief  BKP_DUccStandby - Enter standby mode.
  * @param  None
  * @retval None
  */
void BKP_DUccStandby ( void )
{
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
*(__IO uint32_t *) BKP_STANDBY_BB = (uint32_t) 0x01;
#elif defined (USE_MDR1986VE3)
	MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
#endif

}

/**
  * @brief  BKP_SetFlagPOR - Set power on reset flag (FPOR).
  * @param  None
  * @retval None
  */
void BKP_SetFlagPOR ( void )
{
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	*(__IO uint32_t *) BKP_FPOR_BB = (uint32_t) 0x01;
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
	MDR_BKP->REG_0E |= BKP_REG_0E_FPOR;
#endif
}

/**
  * @brief  BKP_FlagPORstatus - power on reset flag (FPOR) status
  * @param  None
  * @retval enum ErrorStatus - SUCCESS if FPOR is zero, else ERROR
  */
ErrorStatus BKP_FlagPORstatus(void)
{
  ErrorStatus state = ERROR;
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
  if (*(__IO uint32_t *) BKP_FPOR_BB == 0)
  {
    state = SUCCESS;
  }
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
  if (( MDR_BKP->REG_0E & BKP_REG_0E_FPOR ) == BKP_REG_0E_FPOR) {
	  state = SUCCESS;
  }
#endif
  return state;
}

#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
/**
  * @brief  Enters STOP mode.
  * @param  BKP_Regulator_state: specifies the regulator state in STOP mode.
  *   This parameter can be: ENABLE or DISABLE.
  *     @arg ENABLE: STOP mode with regulator ON
  *     @arg DISABLE: STOP mode with regulator in low power mode
  * @param  BKP_STOPentry: specifies if STOP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg BKP_STOPentry_WFI: enter STOP mode with WFI instruction
  *     @arg BKP_STOPentry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void BKP_EnterSTOPMode(FunctionalState BKP_Regulator_state, uint8_t BKP_STOPentry)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(BKP_Regulator_state));
  assert_param(IS_BKP_STOP_ENTRY(BKP_STOPentry));

  if(BKP_Regulator_state == DISABLE)
  {
    MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
  }
  else
  {
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Select STOP mode entry --------------------------------------------------*/
    if(BKP_STOPentry == BKP_STOPentry_WFI)
    {
      /* Request Wait For Interrupt */
      __WFI();
    }
    else
    {
      /* Request Wait For Event */
      __WFE();
    }
  }
}
#elif defined (USE_MDR1986VE1T) || defined (USE_MDR1986VE3)

/**
  * @brief	Shifting core controller into a low power consumption. In this mode,
  * 		the clock frequency is applied only to the selected peripheral
  * 		blocks, which interrupt the supply resumes clock on the core.
  * @param	None
  * @retval None
  */
void BKP_EnterSLEEPMode(void)
{
	/* Enter in SLEEP mode */
	MDR_RST_CLK->ETH_CLOCK |= 1 << RST_CLK_ETH_CLOCK_SLEEP_Pos;
}

#endif

/**
  * @brief  Enters STANDBY mode.
  * @param  None
  * @retval None
  */
void BKP_EnterSTANDBYMode ( void )
{
	/* Select STANDBY mode */
#if defined (USE_MDR1986VE9x) || defined (USE_MDR1901VC1T)
	*(__IO uint32_t *) BKP_STANDBY_BB = (uint32_t) 0x01;
#elif defined (USE_MDR1986VE3) || defined (USE_MDR1986VE1T)
	MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
#endif
}

#if defined (USE_MDR1986VE1T) || defined (USE_MDR1986VE3)

/**
 * @brief	POWER_SetTrim - Adjustment coefficient of the reference voltage
 * 			integrated voltage regulator DUcc roughly.
 * @param 	trim: coefficient of the reference voltage.
 * 			This parameter can be one the following values:
 * 				@arg BKP_TRIM_1_8_V
 * 				@arg BKP_TRIM_1_6_V
 * 				@arg BKP_TRIM_1_4_V
 * @return	None.
 */
void BKP_SetTrim(uint32_t trim)
{
	uint32_t tmpreg;
	/* Check the parameters */
	assert_param(IS_BKP_TRIM(trim));

	tmpreg = MDR_BKP->REG_0E & (~(3 << BKP_REG_0E_TRIM_34_Pos));
	tmpreg = tmpreg | trim;
	MDR_BKP->REG_0E = tmpreg;
}

#endif

/** @} */ /* End of group BKP_Private_Functions */

/** @} */ /* End of group BKP */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

/*
*
* END OF FILE MDR32F9Qx_bkp.c */

