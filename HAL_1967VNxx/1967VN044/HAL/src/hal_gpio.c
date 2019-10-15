/**********************************************************************************************************************
 Copyright (c) 2017 Milandr Corporation. All rights reserved.

 History:
 14-Mar-2017 Ivan Osadchy 	 - add LX_GPIO_...
 02-Mar-2017 Ivan Osadchy   - Created
 **********************************************************************************************************************/
#include "hal_1967VN044.h"

// Инициализация режима работы пинов для порта PA, PB или PC:
void HAL_GPIO_Init( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const GPIO_PinMode Mode )
{
	volatile uint32_t *ALT = ( Mode & 0x4 ) ? &GPIO_Pi->ALT.SET : &GPIO_Pi->ALT.CLR;
	volatile uint32_t *DDR = ( Mode & 0x2 ) ? &GPIO_Pi->DDR.SET : &GPIO_Pi->DDR.CLR;
	volatile uint32_t *PUR = ( Mode & 0x1 ) ? &GPIO_Pi->PUR.SET : &GPIO_Pi->PUR.CLR;

	*ALT = Pins;
	*DDR = Pins;
	*PUR = Pins;
	GPIO_Pi->DR.CLR = Pins;
	GPIO_Pi->INVR.CLR = Pins;
}

// Деинициализация пинов в начальное состояние:
void HAL_GPIO_DeInit( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins )
{
	HAL_GPIO_InterruptConfig( GPIO_Pi, Pins, GPIO_InterruptMode_Off );

	GPIO_Pi->ALT.CLR = Pins;
	GPIO_Pi->DDR.CLR = Pins;
	GPIO_Pi->PUR.CLR = Pins;
}

void HAL_GPIO_Px_AltInit( const GPIO_PxAlt_type PxAlt, const GPIO_PxAlt_Mode_type PxAltMode )
{
	LX_GPIO_PxALT->b.PDXF |= ( uint32_t ) PxAltMode;
	LX_GPIO_PxALT->word |= ( uint32_t ) PxAlt;
}

void HAL_GPIO_Px_AltDeInit( const GPIO_PxAlt_type PxAlt )
{
	LX_GPIO_PxALT->word &= ~( uint32_t ) PxAlt;
	LX_GPIO_PxALT->b.PDXF &= ~ 0x03;
}

void HAL_GPIO_Px_AltFullDeInit( void )
{
	LX_GPIO_PxALT->b.PDXF = 0;
}

// Инициализация режима работы пинов для порта PxD как GPIO:
void HAL_GPIO_PxD_Init( const uint32_t Pins, const GPIO_PinMode Mode )
{
	HAL_GPIO_Px_AltDeInit( GPIO_PxAlt_All );
	if ( Mode & 0x02 )
		LX_GPIO_PxD->DIR |= Pins;
	else
		LX_GPIO_PxD->DIR &= ( ~Pins );
	if ( Mode & 0x01 )
		LX_GPIO_PxD->PUR |= Pins;
	else
		LX_GPIO_PxD->PUR &= ( ~Pins );
}

// Деинициализация режима работы пинов для порта PxD:
void HAL_GPIO_PxD_DeInit( const uint32_t Pins )
{
	HAL_GPIO_Px_AltDeInit( GPIO_PxAlt_All );
	LX_GPIO_PxD->DIR &= ( ~Pins );
	LX_GPIO_PxD->PUR &= ( ~Pins );
}

// Запись значения в 1 пин порта:
void HAL_GPIO_WritePin( GPIO_PortPi * const GPIO_Pi, const GPIO_PIN Pin, const GPIO_PinState NewState )
{
	if ( NewState == GPIO_PinState_Set )
		GPIO_Pi->DR.SET = Pin;
	else
		GPIO_Pi->DR.CLR = Pin;
}

// Прочитать текущее значение 1 пина порта:
GPIO_PinState HAL_GPIO_ReadPin( GPIO_PortPi * const GPIO_Pi, const GPIO_PIN Pin )
{
	return ( ( GPIO_PinState ) ( ( GPIO_Pi->PXD & Pin ) != 0 ) );
}

// Записать значения в набор пинов порта:
void HAL_GPIO_WritePins( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const uint32_t Data )
{
	GPIO_Pi->DR.CLR = ( Pins & ( ~Data ) );
	GPIO_Pi->DR.SET = ( Pins & Data );
}

// Инвертировать значения в наборе пинов порта:
void HAL_GPIO_TogglePins( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins )
{
	GPIO_Pi->DR.INV = Pins;
}

// Прочитать значения из набора пинов порта:
uint32_t HAL_GPIO_ReadPins( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins )
{
	return ( GPIO_Pi->PXD & Pins );
}

// Записать значения в пины портов PxA или PxD:
void HAL_GPIO_Px_WritePins( const GPIO_PxPort Px, const uint32_t Pins, const uint32_t Data )
{
	volatile uint32_t *CLR = ( Px == GPIO_PxPort_A ) ? &LX_GPIO_PxA->CLR : &LX_GPIO_PxD->CLR;
	volatile uint32_t *SET = ( Px == GPIO_PxPort_A ) ? &LX_GPIO_PxA->SET : &LX_GPIO_PxD->SET;

	*CLR = ( Pins & ( ~Data ) );
	*SET = ( Pins & Data );
}

// Прочитать значения из пинов портов PxA или PxD:
uint32_t HAL_GPIO_Px_ReadPins( const GPIO_PxPort Px, const uint32_t Pins )
{
	volatile uint32_t *DR = ( Px == GPIO_PxPort_A ) ? &LX_GPIO_PxA->DR : &LX_GPIO_PxD->DR;
	return ( *DR & Pins );
}

// Настройка прерываний от пинов порта:
void HAL_GPIO_InterruptConfig( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const GPIO_InterruptMode Mode )
{
	volatile uint32_t *IMR = ( Mode == GPIO_InterruptMode_Off ) ? &GPIO_Pi->IMR.CLR : &GPIO_Pi->IMR.SET;
	volatile uint32_t *PEIE = ( Mode == GPIO_InterruptMode_Posedge ) ? &GPIO_Pi->PEIE.SET : &GPIO_Pi->PEIE.CLR;
	volatile uint32_t *NEIE = ( Mode == GPIO_InterruptMode_Negedge ) ? &GPIO_Pi->NEIE.SET : &GPIO_Pi->NEIE.CLR;
	volatile uint32_t *INVR = ( Mode == GPIO_InterruptMode_LowLvl ) ? &GPIO_Pi->INVR.SET : &GPIO_Pi->INVR.CLR;

	// Сброс запросов прерываний до настройки:
	HAL_GPIO_InterruptEdgeClear( GPIO_Pi, Pins );

	// Настройка прерываний:
	*IMR = Pins;
	*PEIE = Pins;
	*NEIE = Pins;
	*INVR = Pins;
}

// Проверка возникновения прерывания на пине порта:
uint32_t HAL_GPIO_InterruptCheck( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins )
{
	return ( GPIO_Pi->INTREQ & Pins );
}

// Сброс прерывания по фронту:
void HAL_GPIO_InterruptEdgeClear( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins )
{
	GPIO_Pi->ECLR = Pins;
}
