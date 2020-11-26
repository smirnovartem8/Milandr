/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  27-Feb-2017 Ivan Osadchy   - Created
**********************************************************************************************************************/
#include "hal_1967VN028.h"
#include <sysreg.h>
#include <builtins.h>


// Timer initialization:
void HAL_Timer_Init (Timer_X Timer, u64 Count, Timer_State State) {

	// Arguments checking:
	if (Timer > Timer_1) 				return;
	if (State > Timer_State_Enable) 	return;

	HAL_Timer_SetCount (Timer, Count);
	HAL_Timer_SetState (Timer, State);

}


// Timer deinitialization:
void HAL_Timer_DeInit (Timer_X Timer) {

	HAL_Timer_Init (Timer, 0, Timer_State_Disable);

}


// Get current timer value:
u64 HAL_Timer_GetCurrent (Timer_X Timer) {

	u64 Curr;
	if(Timer == Timer_0)
		Curr = (u64)__builtin_sysreg_read(__TIMER0L);
		Curr += (u64)__builtin_sysreg_read(__TIMER0H) << 32;
	if(Timer == Timer_1)
		Curr = (u64)__builtin_sysreg_read(__TIMER1L);
		Curr += (u64)__builtin_sysreg_read(__TIMER1H) << 32;

	return Curr;
}



// Set timer period:
void HAL_Timer_SetCount (Timer_X Timer, u64 Count) {

	u32 CountL = (u32)(Count & 0x0000000FFFFFFFF);
	u32 CountH = (u32)((Count >> 32) & 0x00000000FFFFFFFF);
	if(Timer == Timer_0)
		__builtin_sysreg_write(__TMRIN0L, CountL);
		__builtin_sysreg_write(__TMRIN0H, CountH);
	if(Timer == Timer_1)
		__builtin_sysreg_write(__TMRIN1L, CountL);
		__builtin_sysreg_write(__TMRIN1H, CountH);

}


// Set timer period now:
void HAL_Timer_SetCountNow (Timer_X Timer, u64 Count) {

	Timer_State State = HAL_Timer_GetState (Timer);
	HAL_Timer_SetState (Timer, Timer_State_Disable);
	HAL_Timer_SetCount (Timer, Count);
	HAL_Timer_SetState (Timer, State);

}

// Get timer count value:
u64 HAL_Timer_GetCount (Timer_X Timer) {
	u64 tmp;
	if(Timer == Timer_0){
		tmp = (u64)__builtin_sysreg_read(__TMRIN0L);
		tmp += (u64)__builtin_sysreg_read(__TMRIN0H) << 32;
	}
	if(Timer == Timer_1){
		tmp = (u64)__builtin_sysreg_read(__TMRIN1L);
		tmp += (u64)__builtin_sysreg_read(__TMRIN1H) << 32;
	}
	return tmp;
}


// Timer enable/disable:
void HAL_Timer_SetState (Timer_X Timer, Timer_State State) {
	u32 value = __builtin_sysreg_read(__INTCTL);
	u32 tmp;
	if ((Timer > Timer_1) || (State > Timer_State_Enable))	return;
	if (Timer == Timer_0){
		if (State == Timer_State_Enable){
			tmp = 0x00000010;
			__builtin_sysreg_write(__INTCTL, tmp|value);
		}
		else {
			tmp = 0xFFFFFFEF;
			__builtin_sysreg_write(__INTCTL, tmp&value);
		}
	}

	if(Timer==Timer_1){
		if(State == Timer_State_Enable){
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
Timer_State HAL_Timer_GetState (Timer_X Timer) {
	u32 value = __builtin_sysreg_read(__INTCTL);
	u32 tmp;
	if (Timer > Timer_1)	return -1;
	switch(Timer){
		case Timer_0:
		{
			tmp = 0x00000010;
			if ((value & tmp) == tmp) return (Timer_State)Timer_State_Enable;
			else return (Timer_State)Timer_State_Disable;
			}
		case Timer_1:
		{
			tmp = 0x00000020;
			if ((value & tmp) == tmp) return (Timer_State)Timer_State_Enable ;
			else return (Timer_State)Timer_State_Disable;
			}
		}
}


// Enable/disable both timers:
void HAL_Timers_SetState (Timer_State Timer0, Timer_State Timer1) {
	HAL_Timer_SetState(Timer_0, Timer0);
	HAL_Timer_SetState(Timer_1, Timer1);
}
