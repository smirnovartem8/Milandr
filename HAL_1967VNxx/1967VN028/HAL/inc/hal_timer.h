/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  07-Mar-2017 Ivan Osadchy   - Created
**********************************************************************************************************************/

#ifndef __HAL_TIMER_H
#define __HAL_TIMER_H


// Variable types:
#define 	__IO					volatile
typedef 	unsigned int 			u32;
typedef 	unsigned long long 		u64;
typedef 	unsigned char 			u8;

// Timer types:
typedef enum {
	Timer_0 	= 0,
	Timer_1 	= 1,
} Timer_X;

// Timer states:
typedef enum {
	Timer_State_Disable	= 0,
	Timer_State_Enable 	= 1,
} Timer_State;

/* Exported functions ------------------------------------------------------- */


/*******************Include Function*********************/
#ifdef __cplusplus
  extern "C"
	{
	#endif

  	 	 void 			HAL_Timer_Init 			(Timer_X Timer, u64 Count, Timer_State State);
  	 	 void 			HAL_Timer_DeInit 		(Timer_X Timer);

  	 	 u64 			HAL_Timer_GetCurrent 	(Timer_X Timer);

  	 	 void 			HAL_Timer_SetCount		(Timer_X Timer, u64 Count);
  	 	 void 			HAL_Timer_SetCountNow 	(Timer_X Timer, u64 Count);
  	 	 u64 			HAL_Timer_GetCount		(Timer_X Timer);
  	 	 void 			HAL_Timer_SetState		(Timer_X Timer, Timer_State State);
  	 	 Timer_State 	HAL_Timer_GetState 		(Timer_X Timer);

  	 	 void 			HAL_Timers_SetState 	(Timer_State Timer0, Timer_State Timer1);

	#ifdef __cplusplus
	}
#endif // __cplusplus

#endif
