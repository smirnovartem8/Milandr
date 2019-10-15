/*
 *
 *	Файл: 		hal_gptimer.c
 *	Описание: 	HAL для таймеров общего назначения
 *
 *	История:
 *	 			07-Mar-2017 Ivan Osadchy 	- 	создан
 */
#include "hal_1967VN044.h"

// Инициализация таймера:
void HAL_GPTimer_Init( GPTimer_X_type Timer, uint64_t Count, GPTimer_State_type State )
{
	// Проверки аргументов:
	if ( Timer > GPTimer_1 ) return;
	if ( State > GPTimer_State_Enable ) return;

	HAL_GPTimer_SetCount( Timer, Count );
	HAL_GPTimer_SetState( Timer, State );
}

// Деинициализация таймера:
void HAL_GPTimer_DeInit( GPTimer_X_type Timer )
{
	HAL_GPTimer_Init( Timer, 0, GPTimer_State_Disable );
}

// Получить текущее значение таймера:
uint64_t HAL_GPTimer_GetCurrent( GPTimer_X_type Timer )
{
	volatile uint32_t *pReg = ( volatile uint32_t* )(& (LX_GPTIMER->Timer_curr[ Timer ] ) );
	uint64_t Curr;
	uint32_t *pCurr =  ( uint32_t* )(& ( Curr ) );

	*pCurr++ = *pReg++;
	*pCurr = *pReg;

	return Curr;
}

// Установка значения для счета таймера в следущем цикле:
void HAL_GPTimer_SetCount( GPTimer_X_type Timer, uint64_t Count )
{
	uint32_t *pCount = ( uint32_t* )(& ( Count ) );
	volatile uint32_t *pTimer = ( volatile uint32_t* )(& ( LX_GPTIMER->Timer_count[ ( uint8_t ) Timer ] ) );

	*pTimer++ = *pCount++;
	*pTimer = *pCount;
}

// Немедленное изменение значения таймера:
void HAL_GPTimer_SetCountNow( GPTimer_X_type Timer, uint64_t Count )
{
	GPTimer_State_type State = HAL_GPTimer_GetState( Timer );
	HAL_GPTimer_SetState( Timer, GPTimer_State_Disable );
	HAL_GPTimer_SetCount( Timer, Count );
	HAL_GPTimer_SetState( Timer, State );
}

// Получение значения счета таймера:
uint64_t HAL_GPTimer_GetCount( GPTimer_X_type Timer )
{
	volatile uint32_t *pReg = ( volatile uint32_t* )(& ( LX_GPTIMER->Timer_count[ Timer ] ) );
	uint64_t Count;
	uint32_t *pCount = ( uint32_t* )(& ( Count ) );

	*pCount++ = *pReg++;
	*pCount = *pReg;

	return Count;
}

// Включение/выключенние таймера:
void HAL_GPTimer_SetState( GPTimer_X_type Timer, GPTimer_State_type State )
{
	if ( Timer == GPTimer_0 ) LX_GPTIMER->CTRL.T0.ena = State;
	if ( Timer == GPTimer_1 ) LX_GPTIMER->CTRL.T1.ena = State;
}

// Текущее состояние таймера (включен/выключен):
GPTimer_State_type HAL_GPTimer_GetState( GPTimer_X_type Timer )
{
	return ( ( GPTimer_State_type ) (
			( Timer == GPTimer_0 ) ? LX_GPTIMER->CTRL.T0.ena : LX_GPTIMER->CTRL.T1.ena ) );
}

// Включение/выключение двух таймеров сразу:
void HAL_GPTimers_SetState( GPTimer_State_type Timer0, GPTimer_State_type Timer1 )
{
	uint32_t State = ( ( ( uint32_t ) Timer1 ) << 1 ) | ( ( uint32_t ) Timer0 );
	LX_GPTIMER->CTRL.All.ena = State;
}
