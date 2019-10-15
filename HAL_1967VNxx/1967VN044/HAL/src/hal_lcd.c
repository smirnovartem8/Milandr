/*
 *
 *	Файл: 		hal_lcd.c
 *	Описание: 	HAL для контроллера LCD панели
 *
 *	История:
 *				20-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *				20-Mar-2017 Dmitriy Sharaviev 	- 	создан
 *
 */
#include "hal_1967VN044.h"

#define wa_delay()	asm("nop;; nop;; nop;; nop;;")

static __builtin_quad tcb;

void HAL_LCD_Enable( void )
{
	LX_LCD->CTRL.b.VEN = 1;
}

void HAL_LCD_Disable( void )
{
	LX_LCD->CTRL.b.VEN = 0;
}

void HAL_LCD_GpioInit( void )
{
	LX_GPIO_PB->ALT.SET = 0x7C3FFFF0;
}

void HAL_LCD_GpioDeInit( void )
{
	LX_GPIO_PB->ALT.CLR = 0x7C3FFFF0;
}

void HAL_LCD_ExGpioInit( LCD_ExGpioNum_type eGpio, LCD_ExGpio_type *pxExGpioCfg )
{
	uint32_t ulTemp;

	switch ( eGpio )
	{
		case LCD_Gpio_0:
			LX_GPIO_PB->ALT.SET = 0x00400000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_0.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_0.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_0.b.HPL = 0;
			else
				LX_LCD->GPIO_0.b.HPL = 1;
			break;
		case LCD_Gpio_1:
			LX_GPIO_PB->ALT.SET = 0x00800000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_1.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_1.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_1.b.HPL = 0;
			else
				LX_LCD->GPIO_1.b.HPL = 1;
			break;
		case LCD_Gpio_2:
			LX_GPIO_PB->ALT.SET = 0x01000000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_2.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_2.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_2.b.HPL = 0;
			else
				LX_LCD->GPIO_2.b.HPL = 1;
			break;
		case LCD_Gpio_3:
			LX_GPIO_PB->ALT.SET = 0x02000000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_3.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_3.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_3.b.HPL = 0;
			else
				LX_LCD->GPIO_3.b.HPL = 1;
			break;
	}
}

void HAL_LCD_ExGpioDeInit( LCD_ExGpioNum_type eGpio )
{
	switch( eGpio )
	{
		case LCD_Gpio_0:
			LX_GPIO_PB->ALT.CLR = 0x00400000;
			break;
		case LCD_Gpio_1:
			LX_GPIO_PB->ALT.CLR = 0x00800000;
			break;
		case LCD_Gpio_2:
			LX_GPIO_PB->ALT.CLR = 0x01000000;
			break;
		case LCD_Gpio_3:
			LX_GPIO_PB->ALT.CLR = 0x02000000;
			break;
	}
}

void HAL_LCD_Setup( LCD_Conf_type *pxCfg )
{
	uint32_t ulTemp;

	LX_LCD->CTRL.word = 0;																			// Выключаем контроллер
	LX_LCD->PWM_CR.word = 0;

	LX_LCD->VSIZE.b.VSIZE = pxCfg->usHSize * pxCfg->usVSize / 										// Задаем размер видеобуффера в 32-битных словах
			( ( pxCfg->ulRgbMode >> 4 ) & 0x0F );

	ulTemp = PLL_Freq.CoreClk / ( 2 * pxCfg->ulPixelClock );										// PX_CLK = SOC_CLK / (P_div + 1)
	if ( ulTemp > 0 ) ulTemp -= 1;
	LX_LCD->PXDV.b.P_DIV = ulTemp;
	wa_delay();
	LX_LCD->PXDV.b.EN_DIV = 1;

	// Количество пикселей в линии и общее количество линий
	// Не все линии и пиксели видны на экране
	LX_LCD->HVLEN.b.HT = ( ( pxCfg->usHFrontBlank + pxCfg->usHSize + pxCfg->usHBackBlank ) >> 3 ) - 1;
	wa_delay();
	LX_LCD->HVLEN.b.VT = ( pxCfg->usVFrontBlank + pxCfg->usVSize + pxCfg->usVBackBlank ) - 1;

	// HSYNC (fpline) тайминги
	LX_LCD->HTIM.b.HPS = pxCfg->usHSyncPos - 1;
	wa_delay();
	LX_LCD->HTIM.b.HPL = ~pxCfg->bHSyncActiveLevel;													// Активный уровень: 0 - высокий, 1 - низкий
	wa_delay();
	LX_LCD->HTIM.b.HPW = pxCfg->usHSyncPos + pxCfg->usHSyncLen - 1;

	// VSYNC (fpframe) тайминги
	LX_LCD->VTIM.b.VPS = pxCfg->usVSyncPos - 1;
	wa_delay();
	LX_LCD->VTIM.b.VPL = ~pxCfg->bVSyncActiveLevel;													// Активный уровень: 0 - высокий, 1 - низкий
	wa_delay();
	LX_LCD->VTIM.b.VPW = pxCfg->usVSyncPos + pxCfg->usVSyncLen - 1;

	// Управление горизонтальной активной областью панели
	LX_LCD->HDTIM.b.HDPS = pxCfg->usHFrontBlank - 1;
	wa_delay();
	LX_LCD->HDTIM.b.HDPE = pxCfg->usHFrontBlank + pxCfg->usHSize - 1;

	// Управление вертикальной активной областью панели
	LX_LCD->VDTIM.b.VDPS = pxCfg->usVFrontBlank - 1;
	wa_delay();
	LX_LCD->VDTIM.b.VDPE = pxCfg->usVFrontBlank + pxCfg->usVSize - 1;

	if ( pxCfg->pxWindow )
	{
		// Видимая область по горизонтали
		LX_LCD->HDxTIM.b.HXDPS = pxCfg->usHFrontBlank + pxCfg->pxWindow->sHOffset - 1;
		wa_delay();
		LX_LCD->HDxTIM.b.HXDPE = pxCfg->usHFrontBlank + pxCfg->pxWindow->sHOffset + pxCfg->pxWindow->usHSize - 1;

		// Видимая область по вертикали
		LX_LCD->VDxTIM.b.VXDPS = pxCfg->usVFrontBlank + pxCfg->pxWindow->sVOffset - 1;
		wa_delay();
		LX_LCD->VDxTIM.b.VXDPE = pxCfg->usVFrontBlank + pxCfg->pxWindow->sVOffset + pxCfg->pxWindow->usVSize - 1;

		// Фон (для оконного режима)
		LX_LCD->FON = pxCfg->pxWindow->ulBackgndColor;
	}

	LX_LCD->PANEL_CFG.b.FPSHI = pxCfg->bPClkInverse;												// Инверсия пиксель клока

	// Настройка ШИМ подсветки
	LX_LCD->PWM_CR.b.RELOAD = HAL_LCD_MAX_PWM_DUTY;
	wa_delay();
	LX_LCD->PWM_CR.b.DUTY = 0;
	wa_delay();
	LX_LCD->PWM_CR.b.PWM_Dv = pxCfg->ulPwmDiv; 														// Значение делителя;
	wa_delay();
	LX_LCD->PWM_CR.b.CLKEN = 1;

	LX_LCD->CTRL.b.VBGR = pxCfg->bSwapRgbToBgr;														// Порядок следования цветов BGR или RGB
	wa_delay();
	LX_LCD->CTRL.b.CD = ( pxCfg->ulRgbMode >> 2 ) & 0x3;											// 0 - 8bit, 1 - 16bit, 2 - 24bit, 3 - 32bit в пикселе
	wa_delay();
	LX_LCD->CTRL.b.VBL = ( pxCfg->ulRgbMode >> 0 ) & 0x3;											// Количеств бит в пикселе
	wa_delay();
	LX_LCD->CTRL.b.BL = ~pxCfg->bDReadyActiveLevel;													// Активный уровень линии готовности данных (0 - DRDY активный высокий, 1 - DRDY активный низкий)
	wa_delay();
	LX_LCD->CTRL.b.W2W_EN = ( pxCfg->pxWindow != 0 );												// Если 1, то используются регистры HDxTIM и VDxTIM
	wa_delay();
	LX_LCD->CTRL.b.PXP_EN = pxCfg->bStopPclkWhenNoData;												// Если 1, разрешен останов пиксельклока в случае если в выходном FIFO нет данных
}

void HAL_LCD_PwmSetDuty( uint8_t ucValue )
{
	if ( ucValue >= HAL_LCD_MAX_PWM_DUTY ) 															// Устранение небольших мерцаний при значениях duty == reload
		ucValue = HAL_LCD_MAX_PWM_DUTY + 1;
	LX_LCD->PWM_CR.b.DUTY = ucValue;
}

void HAL_LCD_StartDma( uint32_t ulChannel, void *pvLcdBuf,
					uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode,
					void *pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &tcb;

	if( ulChannel < 4 || ulChannel > 7 )
			ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	switch ( ( ulRgbMode >> 4 ) & 0x0F )
	{
		case 2:
			ulXSize >>= 1;																			// 2 пикселя в 32-bit слове
			break;
		case 4:
			ulXSize >>= 2;																			// 4 пикселя в 32-bit слове
			break;
		default:
			break;
	}

	*( ptr + 0 ) = ( uint32_t ) pvLcdBuf;															// Источник информации
	*( ptr + 1 ) = ( ulXSize << 16 ) | 4;															// Количество слов + инкремент адреса
	*( ptr + 2 ) = ( ulYSize << 16 ) | 4;															// Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_TWODIM |																	// Включение режима 2-х мерной пересылки
					TCB_QUAD |																		// Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) pvLcdBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// Источник в внешней/внутренней памяти

	HAL_DMA_CreateChannelDest( ulChannel, &tcb, &tcb );
	HAL_DMA_RqstSet( ulChannel, dmaVIDEO );

	if ( pvDmaIsrHandler )																			// Настройка прерывания, если оно нужно
	{
		switch( ulChannel )
		{
			case 4:
				HAL_Interrupt_Enable( intDMA4, pvDmaIsrHandler );
				break;
			case 5:
				HAL_Interrupt_Enable( intDMA5, pvDmaIsrHandler );
				break;
			case 6:
				HAL_Interrupt_Enable( intDMA6, pvDmaIsrHandler );
				break;
			case 7:
				HAL_Interrupt_Enable( intDMA7, pvDmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	HAL_DMA_WriteDC( ulChannel, &tcb );																// Запуск DMA
}

void HAL_LCD_StartDma2Q( uint32_t ulChannel, void *pvLcdBuf,
					uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode,
					void *pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &tcb;
	uint64_t DCA;

	DCA = 0x2000000000000180;

	if( ulChannel < 4 || ulChannel > 7 )
			ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	switch ( ( ulRgbMode >> 4 ) & 0x0F )
	{
		case 2:
			ulXSize >>= 1;																			// 2 пикселя в 32-bit слове
			break;
		case 4:
			ulXSize >>= 2;																			// 4 пикселя в 32-bit слове
			break;
		default:
			break;
	}

	*( ptr + 0 ) = ( uint32_t ) pvLcdBuf;															// Источник информации
	*( ptr + 1 ) = ( ulXSize << 16 ) | 4;															// Количество слов + инкремент адреса
	*( ptr + 2 ) = ( ulYSize << 16 ) | 4;															// Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_TWODIM |																	// Включение режима 2-х мерной пересылки
					TCB_QUAD |																		// Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) pvLcdBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// Источник в внешней/внутренней памяти

	HAL_DMA_CreateChannelDest( ulChannel, &tcb, &tcb );
	HAL_DMA_RqstSet( ulChannel, dmaVIDEO );

	if ( pvDmaIsrHandler )																			// Настройка прерывания, если оно нужно
	{
		switch( ulChannel )
		{
			case 4:
				HAL_Interrupt_Enable( intDMA4, pvDmaIsrHandler );
				break;
			case 5:
				HAL_Interrupt_Enable( intDMA5, pvDmaIsrHandler );
				break;
			case 6:
				HAL_Interrupt_Enable( intDMA6, pvDmaIsrHandler );
				break;
			case 7:
				HAL_Interrupt_Enable( intDMA7, pvDmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	LX_LCD->CTRL.b.DMA_2QW = 1;
	HAL_DMA_PrimaryPriority();
	HAL_DMA_WriteDCA( ulChannel, &DCA );
	HAL_DMA_WriteDC( ulChannel, &tcb );																// Запуск DMA
}

// Этот режим пока не работает
//void HAL_LCD_StartIntDma( void *pvLcdBuf )
//{
//	LX_LCD->EXT_MEM_ADDR = ( uint32_t ) pvLcdBuf;
//	LX_LCD->CTRL.b.DMA_EXT_EN = 1;
//}
