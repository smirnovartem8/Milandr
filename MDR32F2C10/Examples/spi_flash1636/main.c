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
#include "spi.h"

/** @addtogroup  __MDR1986VK01_StdPeriph_Examples MDR1986VK01 StdPeriph Examples
 *  @{
 */

/** @addtogroup __MDR1986VK01_EVALBOARD MDR1986VK01 Demo Board
 *  @{
 */

/** @addtogroup spi_flash1636 spi_flash1636
 *  @{
 */

PORT_InitTypeDef uartPort;
UART_InitTypeDef uartInit;


uint8_t spiar[ 4 ] = { 0x11, 0x46, 0x55, 0x83 };
uint8_t spiarback[ 4 ]; 

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
	
		InitSpi(400000);
		
		//SpiExchangeLback( spiar,spiarback, 4 );
	
    SpiTest();
		
		while(1)
		{
		
		}
	
}

/** @} */ /* End of group spi_flash1636 */

/** @} */ /* End of group __MDR1986VK01_EVALBOARD */

/** @} */ /* End of group __MDR1986VK01_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2020 Milandr *********/

/* END OF FILE main.c */
