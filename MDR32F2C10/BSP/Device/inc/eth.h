﻿/**
  ******************************************************************************
  * @file    eth.h
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    11/11/2020
  * @brief   ETH header file for demo board.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2020 Milandr</center></h2>
  ******************************************************************************
  * FILE eth.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ETH_H
#define __ETH_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "MDR32F2C10_eth.h"

/** @addtogroup __MDR32F2C10_BoardPeriph_Driver MDR32F2C10 Standard Peripherial Demoboard Driver
  * @{
  */

/** @addtogroup  BSP_ETH BSP_ETH
  * @{
  */

/** @defgroup BSP_ETH_Private_Variables BSP_ETH Private Variables
  * @{
  */

extern uint32_t ulOurIp;

/** @} */ /* End of group BSP_ETH_Private_Variables */

/** @defgroup BSP_ETH_Exported_Functions BSP_ETH Exported Functions
  * @{
  */

void InitEth( uint32_t speed );
void DeinitEth( void );
void EthLoop( void );

/** @} */ /* End of group BSP_ETH_Exported_Functions */

/** @} */ /* End of group BSP_ETH */

/** @} */ /* End of group __MDR32F2C10_BoardPeriph_Driver */

#endif /* __ETH_H */

/******************* (C) COPYRIGHT 2020 Milandr ********************************
*
* END OF FILE eth.h */


