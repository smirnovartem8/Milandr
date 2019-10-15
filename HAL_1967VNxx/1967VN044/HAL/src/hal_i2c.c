/*
 *
 *	Файл: 		hal_i2c.c
 *	Описание: 	HAL для интерфейса i2c
 *
 *	История:
 *	 			17-May-2017 Dmitry Sharavev		-	создан
 */

#include "hal_i2c.h"

volatile I2C_Struct_type I2C_Struct;

#pragma interrupt
static void HAL_I2C_IrqHandler( void );

// Инициализация i2c
void HAL_I2C_Init( I2C_Conf_type *pxConf )
{
	uint32_t ulTmp;

	if( pxConf->eMode == I2C_MASTER )																// Ведущий
	{
		ulTmp = ( PLL_Freq.CoreClk * 1000 ) / ( 8 * pxConf->ulClockSpeed );
		LX_I2C->VR.b.DIV = ulTmp & 0x0FFF;
	}
	else																							// Ведомый
	{
		if( pxConf->eAddrZero == I2C_ADDR_0_ENABLE )
			LX_I2C->AR.b.AZE = 1;
		else
			LX_I2C->AR.b.AZE = 0;

		if( pxConf->eAddrMode == I2C_ADDR_7BIT )
		{
			LX_I2C->AR.b.SA = pxConf->usAddr & 0x7F;
		}
		else
		{
			LX_I2C->AR.b.SA = ( ( pxConf->usAddr >> 8 ) & 0x03 ) | 0x78;
			LX_I2C->AXR = pxConf->usAddr & 0xFF;
		}

		LX_I2C->CR_SET = I2C_CR_AA | I2C_CR_SLVE_EN;
	}
}

// Инициализация используемых пинов
void HAL_I2C_SetupGpio( void )
{
	LX_CMU->CFG1.b.I2C_ALT = 0;																		// На пинах PC2, PC3 не работает i2c
	HAL_GPIO_Init( LX_GPIO_PC, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PinMode_In );
}

// Включение i2c
void HAL_I2C_Enable( void )
{
	HAL_Interrupt_GlobalEnable();
	HAL_Interrupt_Enable( intI2C, HAL_I2C_IrqHandler );
	LX_I2C->CR_SET = I2C_CR_I2C_ON;
}

// Запись в устройство массива байт
void HAL_I2C_Write( uint8_t ucDevAddr, uint8_t *pucBuff, uint8_t ucLenght )
{
	uint8_t ucI;

	while( I2C_Struct.eStatus == I2C_BUSY );														// Ждем завершения предыдущей операции

	I2C_Struct.eStatus = I2C_BUSY;
	I2C_Struct.eControl = I2C_MSTR_WR;

	if( ucLenght > I2C_MASTER_BUFF_SIZE )
		ucLenght = I2C_MASTER_BUFF_SIZE;

	I2C_Struct.ucSlaveAddr = ucDevAddr;

	for( ucI = 0; ucI < ucLenght; ucI++ )
		I2C_Struct.ucMasterBuff[ ucI ] = pucBuff[ ucI ];

	I2C_Struct.ucMasterDataLen = ucLenght;

	LX_I2C->CR_SET = I2C_CR_TLOW_EN;
	LX_I2C->CR_SET = I2C_CR_STA;

	while( I2C_Struct.eStatus == I2C_BUSY );														// Ждем окончания операции
}

// Чтение из устройства массива байт
void HAL_I2C_Read( uint8_t ucDevAddr, uint8_t ucLenght )
{
	while( I2C_Struct.eStatus == I2C_BUSY );

	I2C_Struct.eStatus = I2C_BUSY;
	I2C_Struct.eControl = I2C_MSTR_RD;
	I2C_Struct.ucSlaveAddr = ucDevAddr;
	I2C_Struct.ucMasterDataLen = ucLenght;

	LX_I2C->CR_SET = I2C_CR_TLOW_EN;
	LX_I2C->CR_SET = I2C_CR_STA;

	while( I2C_Struct.eStatus == I2C_BUSY );
}

// Чтение из устройства массива байт расположенных по заданному адресу
void HAL_I2C_ReadMemAddr( uint8_t ucDevAddr, uint8_t ucDataAddr, uint8_t ucLenght )
{
	while( I2C_Struct.eStatus == I2C_BUSY );

	I2C_Struct.eStatus = I2C_BUSY;
	I2C_Struct.eControl = I2C_MSTR_RD_ADDR;
	I2C_Struct.ucSlaveAddr = ucDevAddr;
	I2C_Struct.ucMasterDataLen = ucLenght;
	I2C_Struct.ucDataAddr = ucDataAddr;

	LX_I2C->CR_SET = I2C_CR_TLOW_EN;
	LX_I2C->CR_SET = I2C_CR_STA;

	while( I2C_Struct.eStatus == I2C_BUSY );
}

void HAL_I2C_IrqHandler( void )
{
	static uint8_t ucLen;
	static uint8_t usNoAnswer;

	LX_I2C->CR_CLEAR = I2C_CR_IRQ;

	if( LX_I2C->SR.b.ABRT )
	{
		LX_I2C->CR_CLEAR = I2C_CR_ABRT;
		I2C_Struct.eStatus = I2C_BUS_FAIL;
	}
	else
	{
		switch( LX_I2C->SR.b.STAT & 0x1F )
		{
			case STAT_MSTR_STOP:																		// >>Мастер сформировал условие STOP. Интерфейс завершил работу в режиме мастера
				if( usNoAnswer )
					I2C_Struct.eStatus = I2C_NO_ANSWER;
				else
					I2C_Struct.eStatus = I2C_OK;
				break;
			case STAT_MSTR_START:																		// >>Мастер сформировал условие START. Интерфейс начал работу в режиме мастера
				LX_I2C->CR_CLEAR = I2C_CR_STA;
				ucLen = 0;
				usNoAnswer = 0;
				 if( I2C_Struct.eControl == I2C_MSTR_RD )
					LX_I2C->DR = ( I2C_Struct.ucSlaveAddr << 1 ) | 1;
				 else
					LX_I2C->DR = I2C_Struct.ucSlaveAddr << 1;
				break;
			case STAT_MSTR_RESTART:																		// >>Мастер сформировал условие RESTART. Интерфейс продолжает работу в режиме мастера
				LX_I2C->CR_CLEAR = I2C_CR_STA;
				ucLen = 0;
				usNoAnswer = 0;
				if( I2C_Struct.eControl == I2C_MSTR_RD_ADDR )
					LX_I2C->DR = ( I2C_Struct.ucSlaveAddr << 1 ) | 1;
				break;
			case STAT_SLV_WR_NULL_ADR:																	// >>Подчиненный обнаружил со стороны мастера обращение для записи по адресу 0. Режим широковещательной записи во все подчиненные устройства
				break;
			case STAT_SLV_STOP:																			// >>Подчиненный обнаружил условие «stop». Интерфейс закончил работу в режиме подчиненного
				I2C_Struct.ucRxCnt = ucLen;
				I2C_Struct.eStatus = I2C_OK;
				LX_I2C->CR_SET = I2C_CR_AA;
				LX_I2C->CR_CLEAR = I2C_CR_SLVE;
				I2C_Struct.eStatus = I2C_SLAVE_STOP;
				break;
			case STAT_SLV_WRF_ACK:																		// >>Интерфейс, работая в состоянии «подчиненный», обнаружил на шине «свой» адрес, а также запрос на запись, после чего ответил битом подтверждения ACK = 1
				I2C_Struct.eStatus = I2C_BUSY;
				ucLen = 0;
				break;
			case STAT_SLV_RDF_ACK:																		// >>Интерфейс, работая в состоянии «подчиненный», обнаружил на шине «свой» адрес, а также запрос на чтение, после чего ответил битом подтверждения ACK = 1
				I2C_Struct.eStatus = I2C_BUSY;
				ucLen = 0;
				LX_I2C->DR = I2C_Struct.ucSlaveOutBuff[ ucLen++ ];
				break;
			case STAT_MSTR_WR_ADR_SLV_NAK:																// >>Мастер отослал адрес, получил в ответ бит ACK=0 - устройство не ответило
				usNoAnswer = 1;
				LX_I2C->CR_SET = I2C_CR_STO;
				break;
			case STAT_MSTR_WR_ADR_SLV_ACK:																// >>Мастер отослал адрес, получил в ответ бит ACK=1 и ждет записи данных для передачи
				if( I2C_Struct.eControl == I2C_MSTR_RD_ADDR )
					LX_I2C->DR = I2C_Struct.ucDataAddr;
				else
				{
					LX_I2C->DR = I2C_Struct.ucMasterBuff[ ucLen++ ];
					I2C_Struct.ucMasterDataLen--;
				}
				break;
			case STAT_MSTR_RD_ADR_SLV_NAK:																// >>Мастер отослал первый байт, получил в ответ бит ACK=0 и ждет сброса флага прерывания для начала чтения данных из внешнего устройства
				usNoAnswer = 1;
				LX_I2C->CR_SET = I2C_CR_STO;
				break;
			case STAT_MSTR_RD_ADR_SLV_ACK:																// >>Мастер отослал первый байт, получил в ответ бит ACK=1 и ждет сброса флага прерывания для начала чтения данных из внешнего устройства
				if( I2C_Struct.ucMasterDataLen > 1 )
					LX_I2C->CR_SET = I2C_CR_AA;
				else
					LX_I2C->CR_CLEAR = I2C_CR_AA;
				break;
			case STAT_SLV_WR_NAK:																		// >>«Подчиненный» принял байт данных и ответил битом подтверждения ACK = 0
				I2C_Struct.ucSlaveInBuff[ ucLen++ ] = LX_I2C->DR;
				break;
			case STAT_SLV_WR_ACK:																		// >>«Подчиненный» принял байт данных и ответил битом подтверждения ACK = 1
				I2C_Struct.ucSlaveInBuff[ ucLen++ ] = LX_I2C->DR;
				if( ucLen == ( I2C_SLAVE_BUFF_SIZE - 1 ) )
					LX_I2C->CR_CLEAR = I2C_CR_AA;
				break;
			case STAT_SLV_RD_NAK:																		// >>«Подчиненный» отослал байт данных и бит подтверждения ACK = 0
				break;
			case STAT_SLV_RD_ACK:																		// >>«Подчиненный» отослал байт данных и бит подтверждения ACK = 1
				LX_I2C->DR = I2C_Struct.ucSlaveOutBuff[ ucLen++ ];
				if( ucLen == ( I2C_SLAVE_BUFF_SIZE - 1 ) )
					LX_I2C->CR_CLEAR = I2C_CR_AA;
				break;
			case STAT_MSTR_WR_SLV_NAK:																	// >>Мастер отослал байт данных и принял бит АСК=0
				usNoAnswer = 1;
				LX_I2C->CR_SET = I2C_CR_STO;
				break;
			case STAT_MSTR_WR_SLV_ACK:																	// >>Мастер отослал байт данных и принял бит АСК=1
				if( I2C_Struct.eControl == I2C_MSTR_RD_ADDR )
					LX_I2C->CR_SET = I2C_CR_STA;
				else
				{
					if( I2C_Struct.ucMasterDataLen )
					{
						LX_I2C->DR = I2C_Struct.ucMasterBuff[ ucLen++ ];
						I2C_Struct.ucMasterDataLen--;
					}
					else
						LX_I2C->CR_SET = I2C_CR_STO;
				}
				break;
			case STAT_MSTR_RD_SLV_NAK:																	// >>Мастер принял байт данных и отослал бит АСК=0
				I2C_Struct.ucMasterBuff[ ucLen++ ] = LX_I2C->DR;
				LX_I2C->CR_SET = I2C_CR_STO;
				break;
			case STAT_MSTR_RD_SLV_ACK:																	// >>Мастер принял байт данных и отослал бит АСК=1
				I2C_Struct.ucMasterBuff[ ucLen++ ] = LX_I2C->DR;
				if( ucLen == I2C_Struct.ucMasterDataLen - 1 )
					LX_I2C->CR_CLEAR = I2C_CR_AA;
				break;
			case STAT_BUS_FREE:																			// >>Шина свободна
				break;
			case STAT_RESET:																			// >>Значение после сброса
				break;
		}
	}
	LX_I2C->CR_CLEAR = I2C_CR_SI;
}
