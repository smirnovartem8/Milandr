/**********************************************************************************************************************
  Copyright (c) 2017 Milandr Corporation. All rights reserved.

  History:
  	  27-Feb-2017 Ivan Osadchy   - Created
**********************************************************************************************************************/
#include "hal_1967VN028.h"
#include <sysreg.h>
#include <builtins.h>


// Timer initialization:
void HAL_GPTimer_Init (GPTimer_X Timer, u64 Count, GPTimer_State State) {

	// Arguments checking:
	if (Timer > GPTimer_1) 				return;
	if (State > GPTimer_State_Enable) 	return;

	HAL_GPTimer_SetCount (Timer, Count);
	HAL_GPTimer_SetState (Timer, State);

}


// Timer deinitialization:
void HAL_GPTimer_DeInit (GPTimer_X Timer) {

	HAL_GPTimer_Init (Timer, 0, GPTimer_State_Disable);

}


// Get current timer value:
u64 HAL_GPTimer_GetCurrent (GPTimer_X Timer) {

	u64 Curr;
	if(Timer == GPTimer_0)
		Curr = (u64)__builtin_sysreg_read(__TIMER0L);
		Curr += (u64)__builtin_sysreg_read(__TIMER0H) << 32;
	if(Timer == GPTimer_1)
		Curr = (u64)__builtin_sysreg_read(__TIMER1L);
		Curr += (u64)__builtin_sysreg_read(__TIMER1H) << 32;

	return Curr;
}



// Set timer period:
void HAL_GPTimer_SetCount (GPTimer_X Timer, u64 Count) {

	u32 CountL = (u32)(Count & 0x0000000FFFFFFFF);
	u32 CountH = (u32)((Count >> 32) & 0x00000000FFFFFFFF);
	if(Timer == GPTimer_0)
		__builtin_sysreg_write(__TMRIN0L, CountL);
		__builtin_sysreg_write(__TMRIN0H, CountH);
	if(Timer == GPTimer_1)
		__builtin_sysreg_write(__TMRIN1L, CountL);
		__builtin_sysreg_write(__TMRIN1H, CountH);

}


// Set timer period now:
void HAL_GPTimer_SetCountNow (GPTimer_X Timer, u64 Count) {

	GPTimer_State State = HAL_GPTimer_GetState (Timer);
	HAL_GPTimer_SetState (Timer, GPTimer_State_Disable);
	HAL_GPTimer_SetCount (Timer, Count);
	HAL_GPTimer_SetState (Timer, State);

}

// Get timer count value:
u64 HAL_GPTimer_GetCount (GPTimer_X Timer) {
	u64 tmp;
	if(Timer == GPTimer_0){
		tmp = (u64)__builtin_sysreg_read(__TMRIN0L);
		tmp += (u64)__builtin_sysreg_read(__TMRIN0H) << 32;
	}
	if(Timer == GPTimer_1){
		tmp = (u64)__builtin_sysreg_read(__TMRIN1L);
		tmp += (u64)__builtin_sysreg_read(__TMRIN1H) << 32;
	}
	return tmp;
}


// Timer enable/disable:
void HAL_GPTimer_SetState (GPTimer_X Timer, GPTimer_State State) {
	u32 value = __builtin_sysreg_read(__INTCTL);
	u32 tmp;
	if ((Timer > GPTimer_1) || (State > GPTimer_State_Enable))	return;
	if (Timer == GPTimer_0){
		if (State == GPTimer_State_Enable){
			tmp = 0x00000010;
			__builtin_sysreg_write(__INTCTL, tmp|value);
		}
		else {
			tmp = 0xFFFFFFEF;
			__builtin_sysreg_write(__INTCTL, tmp&value);
		}
	}

	if(Timer==GPTimer_1){
		if(State == GPTimer_State_Enable){
			tmp = 0x00000020;
			__builtin_sysreg_write(__INTCTL, tmp|value);
		}
		else{
			tmp = 0xFFFFFFDF;
			__builtin_sysreg_write(__INTCTL, tmp&value);
		}
	}
}


// Get timer current state (enable/disable):
GPTimer_State HAL_GPTimer_GetState (GPTimer_X Timer) {
	u32 value = __builtin_sysreg_read(__INTCTL);
	u32 tmp;
	if (Timer > GPTimer_1)	return -1;
	switch(Timer){
		case GPTimer_0:
		{
			tmp = 0x00000010;
			if ((value & tmp) == tmp) return (GPTimer_State)GPTimer_State_Enable;
			else return (GPTimer_State)GPTimer_State_Disable;
			}
		case GPTimer_1:
		{
			tmp = 0x00000020;
			if ((value & tmp) == tmp) return (GPTimer_State)GPTimer_State_Enable ;
			else return (GPTimer_State)GPTimer_State_Disable;
			}
		}
}


// Enable/disable both timers:
void HAL_GPTimers_SetState (GPTimer_State Timer0, GPTimer_State Timer1) {
	HAL_GPTimer_SetState(GPTimer_0, Timer0);
	HAL_GPTimer_SetState(GPTimer_1, Timer1);
}
