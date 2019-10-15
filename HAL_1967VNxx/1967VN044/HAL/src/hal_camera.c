/*
 *
 *	Файл: 		hal_camera.c
 *	Описание: 	HAL для интерфейса видеокамеры
 *
 *	История:
 *				19-Jun-2017 Dmitry Sharavev - изменен под вторую ревизию
 *				15-Mar-2017 Dmitry Sharavev - создан
 *
 */

#include "hal_1967VN044.h"

static __builtin_quad camera_tcb;

void HAL_CAMERA_SetupGpio( void )
{
	LX_GPIO_PA->ALT.CLR = 0xFE000000;
	LX_GPIO_PA->DDR.CLR = 0xFE000000;

	LX_GPIO_PB->ALT.CLR = 0x0000000F;
	LX_GPIO_PB->DDR.CLR = 0x0000000F;
}

void HAL_CAMERA_Config( CAMERA_Conf_type *pxConf )
{
	(pxConf->eMode == CAMERA_RECEIVE_MODE_MASTER) ? ( LX_CAMERA->CR.b.SMODE = 1 ) : ( LX_CAMERA->CR.b.SMODE = 0 );
	(pxConf->eVSYNC == CAMERA_VSYNC_HIGH) ? ( LX_CAMERA->CR.b.VPOL = 1 ) : ( LX_CAMERA->CR.b.VPOL = 0 );
}

void HAL_CAMERA_Enable( void )
{
	LX_CAMERA->CR.b.VCON = 1;
}

void HAL_CAMERA_Disable( void )
{
	LX_CAMERA->CR.word = 0x00000000;																// Выключаем интерфейс камеры
	LX_CAMERA->SR.word = 0xFFFFFFFF;																// Сбрасываем все флаги
}

void HAL_CAMERA_StartDma( uint32_t ulChannel, void *pvCameraBuf, uint32_t ulXSize, uint32_t ulYSize,
					uint32_t ulDelta, uint8_t ucPixelsInWord, void *pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &camera_tcb;

	if( ulChannel < 8 || ulChannel > 11 )
		ulChannel = 8;

	HAL_DMA_Stop( ulChannel );

	switch ( ucPixelsInWord )
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

	*( ptr + 0 ) = ( uint32_t ) pvCameraBuf;														// Источник информации
	*( ptr + 1 ) = ( ulXSize << 16 ) | 4;															// Количество слов + инкремент адреса
	*( ptr + 2 ) = ( ulYSize << 16 ) | ulDelta;														// Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_TWODIM |																	// Включение режима 2-х мерной пересылки
//					TCB_HPRIORITY |																	// Приоритет циклов обмена
					TCB_QUAD ;																		// Длина передаваемых данных (операнда) в одном цикле обмена
//					HAL_DMA_GetTCBChannelDest( ulChannel );  											// Канал-приемник следующей цепочки

	*( ptr + 3 ) |= ( ( uint32_t ) pvCameraBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;			// Источник в внешней/внутренней памяти

	HAL_DMA_RqstSet( ulChannel, dmaVIDEO );

	if ( pvDmaIsrHandler )																			// Настройка прерывания, если оно нужно
	{
		switch( ulChannel )
		{
			case 8:
				HAL_Interrupt_Enable( intDMA8, pvDmaIsrHandler );
				break;
			case 9:
				HAL_Interrupt_Enable( intDMA9, pvDmaIsrHandler );
				break;
			case 10:
				HAL_Interrupt_Enable( intDMA10, pvDmaIsrHandler );
				break;
			case 11:
				HAL_Interrupt_Enable( intDMA11, pvDmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;																	// Генерация запроса прерывания после окончания работы канала
	}

	HAL_DMA_WriteDC( ulChannel, &camera_tcb );														// Запуск DMA
}
