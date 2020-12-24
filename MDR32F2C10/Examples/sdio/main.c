/**
 ******************************************************************************
 * @file    readme.txt
 * @author  Milandr Application Team
 * @version V1.0.1
 * @date    11/11/2020
 * @brief   Main program body.
 ******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2020 Milandr</center></h2>
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2C10_port.h"
#include "MDR32F2C10_timer.h"
#include "MDR32F2C10_clk.h"
#include "MDR32F2C10_uart.h"
#include "sdio.h"

#include <string.h>
#include <stdlib.h>

/** @addtogroup  __MDR1986VK01_StdPeriph_Examples MDR1986VK01 StdPeriph Examples
 *  @{
 */

/** @addtogroup __MDR1986VK01_EVALBOARD MDR1986VK01 Demo Board
 *  @{
 */

/** @addtogroup sdio sdio
 *  @{
 */

PORT_InitTypeDef p;
uint32_t size;


FATFS Fatfs[1] __attribute__ ((aligned(1024)));        // File system object for each logical drive

TCHAR       *ptr;
int32_t     p1;
FRESULT     res;
TCHAR       temp_Str1[2];
FATFS *fs;


void ClockInit()
{
    MDR_CLK->KEY = 0x8555AAA1;
    MDR_CLK->PER0_CLK =  0xFFFFFFFF;
    MDR_CLK->PER1_CLK =  0xFFFFFFFF;

    MDR_BKP->KEY = 0x8555AAA1;

    CLK_XSEConfig( CLK_HSE0, CLK_XSE_ON );
    if( !CLK_XSEWaitReady( CLK_HSE0 ) )
        while(1){}

    CLK_SetSystemClock( MAX_CLK_HSE0 );                                                 
}

int main()
{
	
	ClockInit();

	MDR_CLK->SDIO_CLK = CLK_SOURCE_MAX_CLK << 28 | CLK_ENABLE | 2; 
	
	SDIO_PORT->KEY = 0x8555AAA1;
	MDR_PORTB->KEY = 0x8555AAA1;
		
		PORT_StructInit( &p );
		p.PORT_SOE     = PORT_SOE_OUT;
		p.PORT_SFUNC   = PORT_SFUNC_1;
		p.PORT_SANALOG = PORT_SANALOG_DIGITAL;
		p.PORT_SPWR    = PORT_SPWR_10;
		p.PORT_Pin     = SDIO_DAT0_PIN | SDIO_DAT1_PIN | SDIO_DAT2_PIN |
											 SDIO_DAT3_PIN | SDIO_CMD_PIN  | SDIO_CLK_PIN;
		PORT_Init( SDIO_PORT, &p );
	
    f_mount(&Fatfs[0], "", (BYTE)0);

    temp_Str1[0] = '0';
    temp_Str1[1] = ':';
    ptr = &temp_Str1[0];

    res = f_getfree(ptr, (DWORD*)&p1, &fs);	
		
		size = (fs->n_fatent - 2) * (fs->csize / 2);
			
		while(1)
		{
		
		}
	
}

/** @} */ /* End of group sdio */

/** @} */ /* End of group __MDR1986VK01_EVALBOARD */

/** @} */ /* End of group __MDR1986VK01_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2020 Milandr *********/

/* END OF FILE main.c */
