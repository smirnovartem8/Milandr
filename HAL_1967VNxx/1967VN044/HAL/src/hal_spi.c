/*
 *
 *	Файл: 		hal_spi.c
 *	Описание: 	HAL для модуля SPI
 *
 *	История:
 *	 			23-May-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *				15-Mar-2017 Ivan Osadchy 		- 	создан
 *
 */
#include "hal_1967VN044.h"

enum
{
	SPI_MODE_RX	= 0,
	SPI_MODE_TX,
	SPI_MODE_RX_TX
};

volatile SPI_TransferStatus_type SPI_TransferStatus;

struct
{
	uint8_t Spi0_Mode;
	uint8_t Spi1_Mode;
	uint8_t Spi2_Mode;
}SPI_CurrentMode;

#pragma interrupt
static void HAL_SPI0_DmaIrqHandler( void );
#pragma interrupt
static void HAL_SPI1_DmaIrqHandler( void );
#pragma interrupt
static void HAL_SPI2_DmaIrqHandler( void );

// Инициализация SPI:
void HAL_SPI_Init( SPI_type *SPI, SPI_Init_type *InitStr )
{
	HAL_SPI_Control( SPI, SPI_State_Disable );

	SPI->SPCR0.word  = InitStr->CLK_Polarity | InitStr->CLK_Phase | InitStr->FirstBit;
	SPI->SPCR0.b.SCR = InitStr->CLK_Prescaler;
	SPI->SPCR0.b.DSS = InitStr->WordSize - 1;

	if( SPI == LX_SPI0 )
		SPI->SPCR0.b.CSN = InitStr->CSNum;
	else
		SPI->SPCR0.b.CSN = 0;

	SPI->SPCR1.word  = InitStr->CS_Hold | InitStr->Mode | InitStr->LoopBack | ( 1 << 7 );
	SPI->SPCR1.b.DLY_T = InitStr->CS_Hold_Delay & 0x0F;

	if( SPI == LX_SPI0 )
		SPI->SPCR1.b.CSAL = ( InitStr->CS_Active << InitStr->CSNum );
	else
		SPI->SPCR1.b.CSAL = InitStr->CS_Active;

	HAL_SPI_Control( SPI, SPI_State_Enable );

	HAL_SPI_ClearFlag( SPI, SPI_Flag_RxFifoOverflow );
}

// Деинициализация SPI:
void HAL_SPI_DeInit( SPI_type *SPI )
{
	// Сбрасываем регистры в состояния по умолчанию:
	SPI->SPCR0.word 	= 0;
	SPI->SPCR1.word 	= 0;
	SPI->RX_CNT 		= 0;
}

// Инициализация структуры настройки по умолчанию:
void HAL_SPI_DefaultInitStruct( SPI_Init_type *InitStr )
{
	InitStr->CLK_Phase 		= SPI_CLK_Phase_Negedge;			// активность по спадающему фронту
	InitStr->CLK_Polarity 	= SPI_CLK_Polarity_High;			// выскокий уровень CLK
	InitStr->CLK_Prescaler 	= 2;								// предделитель частоты на (2 + 1)
	InitStr->CSNum			= 0;								// обмен по сигналу СS0
	InitStr->CS_Active 		= SPI_CS_Active_Low;				// активный уровень CS - низкий
	InitStr->FirstBit 		= SPI_FirstBit_MSB;					// первым выдает старший байт слова
	InitStr->Mode 			= SPI_Mode_Master;					// режим работы - ведущий
	InitStr->CS_Hold 		= SPI_CS_Unhold;					// сигнал CS не удерживается в активном состоянии
	InitStr->CS_Hold_Delay  = 0;								// Нет паузы между отправки данных
	InitStr->WordSize 		= 8;								// размер выдаваемого слова - 8 бит
	InitStr->LoopBack 		= SPI_LoopBack_Off;					// тестовый режим возврата данных выключен
}

// Включение/отключение SPI:
void HAL_SPI_Control( SPI_type *SPI, SPI_State_type State )
{
	SPI->SPCR1.b.SPE = (uint32_t) State;
}

// Установка режима обмена по SPI (общий, только Tх, только Rx):
void HAL_SPI_SetExchangeMode( SPI_type *SPI, SPI_ExchangeMode_type Mode )
{
	SPI->SPCR1.b.TXO = (Mode == SPI_ExchangeMode_TxOnly);
	SPI->SPCR1.b.RXO = (Mode == SPI_ExchangeMode_RxOnly);
}

// Получить текущий режим обмена по SPI:
SPI_ExchangeMode_type HAL_SPI_GetExchangeMode( SPI_type *SPI )
{
	return ( ( SPI_ExchangeMode_type )( ( SPI->SPCR1.b.RXO << 1 ) | SPI->SPCR1.b.TXO ) );
}

// Записать данных в FIFO передатчика:
uint32_t HAL_SPI_WriteToTxFIFO( SPI_type *SPI, uint32_t *src, uint32_t Size )
{
	uint32_t *SrcStart 	= src;
	uint32_t *SrcEnd 	= src + Size;

	while( ( HAL_SPI_GetFlag( SPI, SPI_Flag_TxFifoNotFull ) == SPI_State_Enable ) && ( src != SrcEnd ) )
	{
		SPI->SPDR = *src++;
	}

	return ( src - SrcStart );
}

// Считать данные из FIFO приемника:
uint32_t HAL_SPI_ReadFromRxFIFO( SPI_type *SPI, uint32_t *dst, uint32_t Size )
{
	uint32_t *DstStart 	= dst;
	uint32_t *DstEnd 	= dst + Size;

	while( ( HAL_SPI_GetFlag ( SPI, SPI_Flag_RxFifoNotEmpty ) == SPI_State_Enable ) && ( dst != DstEnd ) )
	{
		*dst++ = SPI->SPDR;
	}

	return ( dst - DstStart );
}

// Отправка данных:
void HAL_SPI_SendOnly( SPI_type *SPI, uint32_t *src,  uint32_t Size )
{
	SPI_ExchangeMode_type EM = HAL_SPI_GetExchangeMode( SPI );
	uint32_t *SrcEnd 	= src + Size;

	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_TxOnly );

	while ( src != SrcEnd )
	{
		if ( SPI->SPSR.b.TNF != 0 )
			SPI->SPDR = *src++;
	}
	
	while ( ( SPI->SPSR.b.TFE == 0 ) || SPI->SPSR.b.BSY );

	HAL_SPI_SetExchangeMode( SPI, EM );
}

// Прием данных:
void HAL_SPI_ReceiveOnly( SPI_type *SPI, uint32_t *dst, uint32_t Size, uint8_t DO_State )
{
	SPI_ExchangeMode_type EM = HAL_SPI_GetExchangeMode( SPI );
	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_RxOnly );

	SPI->SPCR1.b.ROTL = ( DO_State != 0 );

	while ( Size != 0 )
	{
		if ( Size > 8 )
		{
			SPI->RX_CNT = 8;
			Size -= 8;
		}
		else
		{
			SPI->RX_CNT = Size;
			Size = 0;
		}

		while( SPI->RX_CNT != 0 );
		while( SPI->SPSR.b.BSY == 1 );

		while( SPI->SPSR.b.RNE )
			*dst++ = SPI->SPDR;
	}

	HAL_SPI_SetExchangeMode( SPI, EM );
}

// Одновременный прием и отправка:
void HAL_SPI_SendAndReceive( SPI_type *SPI, uint32_t *src, uint32_t *dst, uint16_t Size )
{
	uint32_t *SrcEnd = src + Size;
	uint32_t *DstEnd = dst + Size;
	SPI_ExchangeMode_type EM = HAL_SPI_GetExchangeMode( SPI );
	uint8_t i;

	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_All );

	while( ( src != SrcEnd ) || ( dst != DstEnd ) )
	{
		i = 0;
		while( ( i < 8 ) && ( src != SrcEnd ) )
		{
			SPI->SPDR = *src++;
			i++;
		}
		while( ( SPI->SPSR.b.TFE == 0 ) || SPI->SPSR.b.BSY );

		while( SPI->SPSR.b.RNE && ( dst != DstEnd ) )
			*dst++ = SPI->SPDR;
	}

	HAL_SPI_SetExchangeMode ( SPI, EM );
}

// Получить состояние флага:
SPI_State_type HAL_SPI_GetFlag( SPI_type *SPI, const SPI_Flag_type Flag )
{
	return ( ( SPI_State_type )( ( SPI->SPSR.word & Flag ) != 0 ) );
}

// Сбросить значение флага:
void HAL_SPI_ClearFlag( SPI_type *SPI, const SPI_Flag_type Flag )
{
	SPI->SPSR.word |= ( ( uint32_t ) Flag);
}

// Настройка событий для прерывания:
void HAL_SPI_ITConfig( SPI_type *SPI, const SPI_IT_type IT, const SPI_State_type State )
{
	uint32_t S = ( ( uint32_t ) State );

	if( IT == SPI_IT_RxCntEnd )
		SPI->SPCR1.b.CIM = S;
	else if( ( IT == SPI_IT_RxFifoHalfFull ) || ( IT == SPI_IT_RxFifoNotEmpty ) )
	{
		SPI->SPCR1.b.R_RQM 	= ( IT == SPI_IT_RxFifoNotEmpty );
		SPI->SPCR1.b.RIM 	= S;
	}
	else if( IT == SPI_IT_RxFifoOverflow )
		SPI->SPCR1.b.ROM = S;
	else if( ( IT == SPI_IT_TxFifoHalfEmpty ) || ( IT == SPI_IT_TxFifoNotFull ) )
	{
		SPI->SPCR1.b.T_RQM 	= ( IT == SPI_IT_TxFifoNotFull );
		SPI->SPCR1.b.TIM 	= S;
	}
	else if( IT == SPI_IT_TxReadEmptyFifo )
		SPI->SPCR1.b.TUM = S;
}

// Проверка события прерывания:
SPI_State_type HAL_SPI_ITCheck( SPI_type *SPI, const SPI_IT_type IT )
{
	uint32_t S = SPI_State_Disable;

	switch ( IT )
	{
		case SPI_IT_RxCntEnd:			S = SPI->SPCR1.b.CIM && ( SPI->RX_CNT == 0 );								break;
		case SPI_IT_RxFifoHalfFull: 	S = SPI->SPCR1.b.RIM && HAL_SPI_GetFlag( SPI, SPI_Flag_RxFifoHalfFull );	break;
		case SPI_IT_RxFifoNotEmpty: 	S = SPI->SPCR1.b.RIM && HAL_SPI_GetFlag( SPI, SPI_Flag_RxFifoNotEmpty ); 	break;
		case SPI_IT_RxFifoOverflow: 	S = SPI->SPCR1.b.ROM && HAL_SPI_GetFlag( SPI, SPI_Flag_RxFifoOverflow );	break;
		case SPI_IT_TxFifoHalfEmpty: 	S = SPI->SPCR1.b.TIM && HAL_SPI_GetFlag( SPI, SPI_Flag_TxFifoHalfEmpty );	break;
		case SPI_IT_TxFifoNotFull: 		S = SPI->SPCR1.b.TIM && HAL_SPI_GetFlag( SPI, SPI_Flag_TxFifoNotFull );		break;
		case SPI_IT_TxReadEmptyFifo: 	S = SPI->SPCR1.b.TUM && HAL_SPI_GetFlag( SPI, SPI_Flag_TxFifoReadEmpty );	break;
	}

	return ( SPI_State_type )S;
}

// Выдача данных в SPI с помощью DMA:
void HAL_SPI_DMA_SendOnly( SPI_type *SPI, uint32_t ulChannel, uint32_t *pusBuff, uint16_t usSize )
{
	__builtin_quad tcb;
	uint32_t *ptr = ( uint32_t* ) &tcb;
	void *pvDmaIsrHandler;

	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_TxOnly );

	if( ulChannel < 4 || ulChannel > 7 )
			ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	*( ptr + 0 ) = ( uint32_t ) pusBuff;															// Источник информации
	*( ptr + 1 ) = ( usSize << 16 ) | 1;															// Количество слов + инкремент адреса
	*( ptr + 2 ) = 0;																				// Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_NORMAL;																		// Длина передаваемых данных (операнда) в одном цикле обмена
	*( ptr + 3 ) |= ( ( uint32_t ) pusBuff < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// Источник в внешней/внутренней памяти

	switch( ( uint32_t )SPI )
	{
		case ( uint32_t )LX_SPI0:
		pvDmaIsrHandler = HAL_SPI0_DmaIrqHandler;
		SPI_CurrentMode.Spi0_Mode = SPI_MODE_TX;
		SPI_TransferStatus.Spi0_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI0 );
			break;
		case ( uint32_t )LX_SPI1:
		pvDmaIsrHandler = HAL_SPI1_DmaIrqHandler;
		SPI_CurrentMode.Spi1_Mode = SPI_MODE_TX;
		SPI_TransferStatus.Spi1_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI1 );
			break;
		case ( uint32_t )LX_SPI2:
		pvDmaIsrHandler = HAL_SPI2_DmaIrqHandler;
		SPI_CurrentMode.Spi2_Mode = SPI_MODE_TX;
		SPI_TransferStatus.Spi2_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI2 );
			break;
		default:
			break;
	}

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

	HAL_DMA_WriteDC( ulChannel, &tcb );
}

// Прием данных из SPI с помощью DMA:
void HAL_SPI_DMA_ReceiveOnly( SPI_type *SPI, uint32_t ulChannel, uint32_t *pusBuff, uint16_t usSize )
{
	__builtin_quad tcb;
	uint32_t *ptr = ( uint32_t* ) &tcb;
	void *pvDmaIsrHandler;

	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_RxOnly );

	if( ulChannel < 8 || ulChannel > 11 )
		ulChannel = 8;

	HAL_DMA_Stop( ulChannel );

	*( ptr + 0 ) = ( uint32_t ) pusBuff;															// Приемник информации
	*( ptr + 1 ) = ( usSize << 16 ) | 1;															// Количество слов + инкремент адреса
	*( ptr + 2 ) = 0;																				// Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_NORMAL;																		// Длина передаваемых данных (операнда) в одном цикле обмена
	*( ptr + 3 ) |= ( ( uint32_t ) pusBuff < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// Приемник в внешней/внутренней памяти

	switch( ( uint32_t )SPI )
	{
		case ( uint32_t )LX_SPI0:
		pvDmaIsrHandler = HAL_SPI0_DmaIrqHandler;
		SPI_CurrentMode.Spi0_Mode = SPI_MODE_RX;
		SPI_TransferStatus.Spi0_RxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI0 );
			break;
		case ( uint32_t )LX_SPI1:
		pvDmaIsrHandler = HAL_SPI1_DmaIrqHandler;
		SPI_CurrentMode.Spi1_Mode = SPI_MODE_RX;
		SPI_TransferStatus.Spi1_RxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI1 );
			break;
		case ( uint32_t )LX_SPI2:
		pvDmaIsrHandler = HAL_SPI2_DmaIrqHandler;
		SPI_CurrentMode.Spi2_Mode = SPI_MODE_RX;
		SPI_TransferStatus.Spi2_RxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannel, dmaSPI2 );
			break;
		default:
			break;
	}

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
	*( ptr + 3 ) |= TCB_INT;

	HAL_DMA_WriteDC( ulChannel, &tcb );

	SPI->RX_CNT = usSize;
}

// Прием данных из SPI с помощью DMA:
void HAL_SPI_DMA_SendAndReceive( SPI_type *SPI, uint32_t ulChannelRx, uint32_t *pusBuffRx, uint32_t ulChannelTx, uint32_t *pusBuffTx, uint16_t usSize )
{
	__builtin_quad tcbRx;
	__builtin_quad tcbTx;
	uint32_t *ptrRx = ( uint32_t* ) &tcbRx;
	uint32_t *ptrTx = ( uint32_t* ) &tcbTx;
	void *pvDmaIsrHandler;

	HAL_SPI_SetExchangeMode( SPI, SPI_ExchangeMode_All );

	if( ulChannelRx < 8 || ulChannelRx > 11 )
		ulChannelRx = 8;

	if( ulChannelTx < 4 || ulChannelTx > 7 )
		ulChannelTx = 4;

	HAL_DMA_Stop( ulChannelRx );
	HAL_DMA_Stop( ulChannelTx );

	*( ptrRx + 0 ) = ( uint32_t ) pusBuffRx;														// Приемник информации
	*( ptrRx + 1 ) = ( usSize << 16 ) | 1;															// Количество слов + инкремент адреса
	*( ptrRx + 2 ) = 0;																				// Количество попыток передачи по X + инкремент адреса X
	*( ptrRx + 3 ) = TCB_NORMAL;																	// Длина передаваемых данных (операнда) в одном цикле обмена
	*( ptrRx + 3 ) |= ( ( uint32_t ) pusBuffRx < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;			// Приемник в внешней/внутренней памяти

	*( ptrTx + 0 ) = ( uint32_t ) pusBuffTx;														// Источник информации
	*( ptrTx + 1 ) = ( usSize << 16 ) | 1;															// Количество слов + инкремент адреса
	*( ptrTx + 2 ) = 0;																				// Количество попыток передачи по X + инкремент адреса X
	*( ptrTx + 3 ) = TCB_NORMAL;																	// Длина передаваемых данных (операнда) в одном цикле обмена
	*( ptrTx + 3 ) |= ( ( uint32_t ) pusBuffTx < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;			// Источник в внешней/внутренней памяти

	switch( ( uint32_t )SPI )
	{
		case ( uint32_t )LX_SPI0:
		pvDmaIsrHandler = HAL_SPI0_DmaIrqHandler;
		SPI_CurrentMode.Spi0_Mode = SPI_MODE_RX_TX;
		SPI_TransferStatus.Spi0_RxDmaReady = 0;
		SPI_TransferStatus.Spi0_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannelRx, dmaSPI0 );
		HAL_DMA_RqstSet( ulChannelTx, dmaSPI0 );
			break;
		case ( uint32_t )LX_SPI1:
		pvDmaIsrHandler = HAL_SPI1_DmaIrqHandler;
		SPI_CurrentMode.Spi1_Mode = SPI_MODE_RX_TX;
		SPI_TransferStatus.Spi1_RxDmaReady = 0;
		SPI_TransferStatus.Spi1_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannelRx, dmaSPI1 );
		HAL_DMA_RqstSet( ulChannelTx, dmaSPI1 );
			break;
		case ( uint32_t )LX_SPI2:
		pvDmaIsrHandler = HAL_SPI2_DmaIrqHandler;
		SPI_CurrentMode.Spi2_Mode = SPI_MODE_RX_TX;
		SPI_TransferStatus.Spi2_RxDmaReady = 0;
		SPI_TransferStatus.Spi2_TxDmaReady = 0;
		HAL_DMA_RqstSet( ulChannelRx, dmaSPI2 );
		HAL_DMA_RqstSet( ulChannelTx, dmaSPI2 );
			break;
		default:
			break;
	}

	switch( ulChannelRx )
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
	*( ptrRx + 3 ) |= TCB_INT;

	HAL_DMA_WriteDC( ulChannelRx, &tcbRx );
	HAL_DMA_WriteDC( ulChannelTx, &tcbTx );


}
/* ----------------------------------- Обработчики прерываний от DMA ---------------------------- */
void HAL_SPI0_DmaIrqHandler( void )
{
	switch( SPI_CurrentMode.Spi0_Mode )
	{
	case SPI_MODE_RX:
	SPI_TransferStatus.Spi0_RxDmaReady = 1;
		break;
	case SPI_MODE_TX:
	SPI_TransferStatus.Spi0_TxDmaReady = 1;
		break;
	case SPI_MODE_RX_TX:
	SPI_TransferStatus.Spi0_RxDmaReady = 1;
	SPI_TransferStatus.Spi0_TxDmaReady = 1;
		break;
	}
}

void HAL_SPI1_DmaIrqHandler( void )
{
	switch( SPI_CurrentMode.Spi1_Mode )
	{
	case SPI_MODE_RX:
	SPI_TransferStatus.Spi1_RxDmaReady = 1;
		break;
	case SPI_MODE_TX:
	SPI_TransferStatus.Spi1_TxDmaReady = 1;
		break;
	case SPI_MODE_RX_TX:
	SPI_TransferStatus.Spi1_RxDmaReady = 1;
	SPI_TransferStatus.Spi1_TxDmaReady = 1;
		break;
	}
}

void HAL_SPI2_DmaIrqHandler( void )
{
	switch( SPI_CurrentMode.Spi2_Mode )
	{
	case SPI_MODE_RX:
	SPI_TransferStatus.Spi2_RxDmaReady = 1;
		break;
	case SPI_MODE_TX:
	SPI_TransferStatus.Spi2_TxDmaReady = 1;
		break;
	case SPI_MODE_RX_TX:
	SPI_TransferStatus.Spi2_RxDmaReady = 1;
	SPI_TransferStatus.Spi2_TxDmaReady = 1;
		break;
	}
}

