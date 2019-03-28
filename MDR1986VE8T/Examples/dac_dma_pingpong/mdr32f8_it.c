/**
  ******************************************************************************
  * @file    MDR32F8_it.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   Main Interrupt Service Routines.
  *
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "mdr32f8_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_ChannelInitTypeDef DMA_InitStr;
extern DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
extern DMA_CtrlDataInitTypeDef DMA_AltCtrlStr;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void INT_UART0_Handler(void)
{
	uint32_t temp_1;

  if (UART_GetITStatusMasked(MDR_UART0, UART_IT_RX) == SET)
  {
		temp_1 = UART_ReceiveData(MDR_UART0);	
		
		UART_ClearITPendingBit(MDR_UART0, UART_IT_RX);

				while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_TXFE)!= SET)
				{
				}
				UART_SendData (MDR_UART0,temp_1);
  }
}

void DMA_DONE0_Handler(void)
{
  if((MDR_DMA->CHNL_PRI_ALT_SET &(1<<DMA_Channel_TIM1)) == (0<<DMA_Channel_TIM1))
  {
   DMA_CtrlInit(DMA_Channel_TIM1, DMA_CTRL_DATA_ALTERNATE,  &DMA_AltCtrlStr);
  }
  else if((MDR_DMA->CHNL_PRI_ALT_SET & (1<<DMA_Channel_TIM1)) == (1<<DMA_Channel_TIM1))
  {
   DMA_CtrlInit(DMA_Channel_TIM1, DMA_CTRL_DATA_PRIMARY, &DMA_PriCtrlStr);
  }
}
/******************* (C) COPYRIGHT 2015 Milandr *********/

/* END OF FILE MDR32F8_it.c */
