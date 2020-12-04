/**
  ******************************************************************************
  * @file    MDR32F9Qx_config.h
  * @author  Milandr Application Team
  * @version V1.2.1
  * @date    06/08/2020
  * @brief   Library configuration file.
  ******************************************************************************
  * <br><br>
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2020 Milandr</center></h2>
  */

/**
 * @mainpage MDR32F9Qx Standard Peripherals Library.
 * MDR32F9Qx Standard Peripherals Library is a package consisting of
 * all standard peripheral device drivers for 1986BE4 microcontrollers.
 * This library is a firmware package which contains a collection of routines,
 * data structures and macros covering the features of Milandr MDR32F9Qx
 * peripherals. It includes a description of the device drivers plus a set of
 * examples for each peripheral. The firmware library allows any device to be
 * used in the user application without the need for in-depth study of each
 * peripherals specifications. Using the Standard Peripherals Library has two
 * advantages: it saves significant time that would otherwise be spent in
 * coding, while simultaneously reducing application development and
 * integration costs.
 *
 * The MDR32F9Qx Standard Peripherals Library is compatible with Milandr
 * 1986BE4x evaluation boards.
 *
 * The MDR32F9Qx Standard Peripherals Library is full CMSIS compliant.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F9Qx_CONFIG_H
#define __MDR32F9Qx_CONFIG_H

#include "MDR32F9Qx_lib.h"
#include <stdint.h>

#include "MDR1986BE4.h"


#define USE_MDR1986BE4


/* Target system parameters */
/* RST_CLK generators frequencies in HZ */
#define HSI_Value       ((uint32_t)8000000)
#define HSE_Value       ((uint32_t)8000000)
#define HSE2_Value      ((uint32_t)25000000)
#define LSI_Value       ((uint32_t)40000)
#define LSE_Value       ((uint32_t)32768)

/* RST_CLK frequencies startup timeouts settings */
#define HSEonTimeOut    ((uint16_t)0x0600)
#define HSE2onTimeOut	((uint16_t)0x8000)
#define LSEonTimeOut    ((uint16_t)0x0600)
#define HSIonTimeOut    ((uint16_t)0x0600)
#define LSIonTimeOut    ((uint16_t)0x0600)
#define PLLCPUonTimeOut ((uint16_t)0x0600)
#define PLLUSBonTimeOut ((uint16_t)0x0600)
#define PLLDSPonTimeOut ((uint16_t)0x0600)

#define FLASH_PROG_FREQ_MHZ     (8.0)


/* RTC configuration parameters */
#define RTC_CalibratorValue   	0
#define RTC_PRESCALER_VALUE		32768

/* DMA configuration parameters */
/* Number of DMA channels to use */
#define DMA_Channels_Number   32          /* This parameter is in range 1..32 */

/* Alternate Control Data Structure Usage */
/* This parameter can be a value of:
    0 = DMA_ALternateDataDisabled;
    1 = DMA_ALternateDataEnabled; */
/*#define DMA_AlternateData   0 */          /* DMA_AlternateDataDisabled */
#define DMA_AlternateData   1             /* DMA_AlternateDataEnabled */


/* Parameter run-time check support ------------------------------------------*/

/* Select one of the following values of USE_ASSERT_INFO macro to control
   parameter checking in the Standard Peripheral Library drivers:
     0 - no parameter checks ("assert_param" macro is disabled);
     1 - check enabled, source file ID and line number are available;
     2 - check enabled, source file ID, line number and checking expression
         (as string) are available (increased code size).
*/
 #define USE_ASSERT_INFO    0 
/* #define USE_ASSERT_INFO    1 */
/* #define USE_ASSERT_INFO    2 */

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed user's function
  *   which gets the source file ID (see MDR32F9Qx_lib.h), line number and
  *   expression text (if USE_ASSERT_INFO == 2) of the call that failed. That
  *   function should not return. If expr is true, nothing is done.
  * @retval None
  */
#if (USE_ASSERT_INFO == 0)
  #define assert_param(expr) ((void)0)
#elif (USE_ASSERT_INFO == 1)
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed(ASSERT_INFO_FILE_ID, __LINE__))
  void assert_failed(uint32_t file_id, uint32_t line);
#elif (USE_ASSERT_INFO == 2)
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed(ASSERT_INFO_FILE_ID, __LINE__, #expr))
  void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr);
#else
  #error "Unsupported USE_ASSERT_INFO level"
#endif /* USE_ASSERT_INFO */

#if defined (__ICCARM__)
	#define __attribute__(name_section)
#endif
#if defined (__CMCARM__)
	#define __attribute__(name_section)
	#define IAR_SECTION(section)
#endif
#if defined (__CC_ARM)
	#define IAR_SECTION(section)
#endif

#endif /* __MDR32F9Qx_CONFIG_H */

/******************* (C) COPYRIGHT 2020 Milandr *********************************
*
* END OF FILE MDR32F9Qx_config.h */
