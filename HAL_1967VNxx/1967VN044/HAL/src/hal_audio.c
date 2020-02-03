/*
 *
 *	File/Файл: 		hal_audio.с
 *	Description/Описание: 	HAL for audio-codec interface AC97/I2S / HAL для интерфейса к аудио-кодеку AC97/I2S
 *
 *	History/История:
 *				21-Mar-2017 Dmitry Sharavev - created/создан
 *
 */

#include "hal_1967VN044.h"

static __builtin_quad audioTcbTx;
static uint32_t ulChannel0, ulChannel1;

void HAL_AUDIO_I2SInit( AUDIO_type* const AudioX, AUDIO_I2S_type* I2sConf )
{
	uint16_t usReqlp;

	AudioX->SICR0.word = 0x00000000;

	usReqlp = ( PLL_Freq.CoreClk * 1000 / 4 ) / ( I2sConf->AudioFreq * I2sConf->DataLen );
	AudioX->SICR2.b.REQLP = usReqlp;
	AudioX->SICR2.b.ERPL = 1;

	AudioX->I2S_T_CR.b.TEN = 1;
	AudioX->I2S_T_CR.b.MODE = I2sConf->Mode;
	AudioX->I2S_T_CR.b.SONY = I2sConf->Standart;
	AudioX->I2S_T_CR.b.MS = I2sConf->Role;
	AudioX->I2S_T_CR.b.DSS = I2sConf->DataLen - 1;
	AudioX->I2S_T_CR.b.PNOS = I2sConf->TfsPos;
	AudioX->I2S_T_CR.b.SWHW = I2sConf->SwapLR;

	if( I2sConf->DataLen <= 16 )
		AudioX->I2S_T_CR.b.PACKH = 1;
	else
		AudioX->I2S_T_CR.b.PACKH = 0;

	if( AudioX == LX_AUDIO0 )
		HAL_GPIO_Init( LX_GPIO_PA, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_16 | GPIO_PIN_17 | GPIO_PIN_18, GPIO_PinMode_Alt );
	else
		HAL_GPIO_Init( LX_GPIO_PA, GPIO_PIN_19 | GPIO_PIN_20 | GPIO_PIN_21 | GPIO_PIN_22 | GPIO_PIN_23 | GPIO_PIN_24, GPIO_PinMode_Alt );
}

void HAL_AUDIO_StartDMA( uint32_t ulChannel, AUDIO_type* const AudioX, void *pvDataBuf, uint16_t usSize, void* pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &audioTcbTx;

	if( ulChannel < 4 || ulChannel > 7 )
		ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	*( ptr + 0 ) = ( uint32_t ) pvDataBuf;							// Information source/ Источник информации
	*( ptr + 1 ) = ( usSize << 16 ) | 1;							// The number of words + address increment/ Количество слов + инкремент адреса
	*( ptr + 2 ) = 0;												// The number of X-transmission attempts + X address increment/ Количество  попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_NORMAL |									// The length of the transmitted data (operand) in one exchange cycle/ Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_INT ;										// Generation of the interrupt request when the channel completed operation/ Генерация запроса прерывания после окончания 		//работы канала
	HAL_DMA_CreateChannelDest( ulChannel, &audioTcbTx, &audioTcbTx );
	*( ptr + 3 ) |= 	( ( uint32_t ) pvDataBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// The source is in the external or in the internal RAM/ Источник в внешней/внутренней памяти

	if( AudioX == LX_AUDIO0 )
	{
		ulChannel0 = ulChannel;
		HAL_DMA_RqstSet( ulChannel, dmaSSI0 );
	}
	else
	{
		ulChannel1 = ulChannel;
		HAL_DMA_RqstSet( ulChannel, dmaSSI1 );
	}

	if ( pvDmaIsrHandler )																			// Setting of the interrupt if needed / Настройка прерывания, если оно нужно
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

	HAL_DMA_WriteDC( ulChannel, &audioTcbTx );														// DMA start/Запуск DMA
}

void HAL_AUDIO_StopDMA( AUDIO_type* const AudioX )
{
	uint32_t ulCh;

	if( AudioX == LX_AUDIO0 )
		ulCh = ulChannel0;
	else
		ulCh = ulChannel1;

	HAL_DMA_Stop( ulCh );

	switch( ulCh )
	{
		case 4:
			HAL_Interrupt_Disable( intDMA4 );
			break;
		case 5:
			HAL_Interrupt_Disable( intDMA5 );
			break;
		case 6:
			HAL_Interrupt_Disable( intDMA6 );
			break;
		case 7:
			HAL_Interrupt_Disable( intDMA7 );
			break;
		default:
			break;
	}
}
