/**********************************************************************************************************************
  Copyright (c) 2017 Milandr Corporation. All rights reserved.

  History:
  	  07-Mar-2017 Ivan Osadchy   - Created
**********************************************************************************************************************/
#ifndef __HAL_GPTIMER_H
#define __HAL_GPTIMER_H


// Variable types:
#define 	__IO					volatile
typedef 	unsigned int 			u32;
typedef 	unsigned long long 		u64;
typedef 	unsigned char 			u8;

// Timer types:
typedef enum {
	GPTimer_0 	= 0,
	GPTimer_1 	= 1,
} GPTimer_X;

// Timer states:
typedef enum {
	GPTimer_State_Disable	= 0,
	GPTimer_State_Enable 	= 1,
} GPTimer_State;

/* Exported functions ------------------------------------------------------- */


/*******************Include Function*********************/
#ifdef __cplusplus
  extern "C"
	{
	#endif

  	 	 void 			HAL_GPTimer_Init 			(GPTimer_X Timer, u64 Count, GPTimer_State State);
  	 	 void 			HAL_GPTimer_DeInit 			(GPTimer_X Timer);

  	 	 u64 			HAL_GPTimer_GetCurrent 		(GPTimer_X Timer);

  	 	 void 			HAL_GPTimer_SetCount		(GPTimer_X Timer, u64 Count);
  	 	 void 			HAL_GPTimer_SetCountNow 	(GPTimer_X Timer, u64 Count);
  	 	 u64 			HAL_GPTimer_GetCount		(GPTimer_X Timer);
  	 	 void 			HAL_GPTimer_SetState		(GPTimer_X Timer, GPTimer_State State);
  	 	 GPTimer_State 	HAL_GPTimer_GetState 		(GPTimer_X Timer);

  	 	 void 			HAL_GPTimers_SetState 		(GPTimer_State Timer0, GPTimer_State Timer1);

	#ifdef __cplusplus
	}
#endif // __cplusplus

#endif
