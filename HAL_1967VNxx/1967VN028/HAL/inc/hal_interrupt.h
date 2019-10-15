/**********************************************************************************************************************
  Copyright (c) 2016 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

#include "hal_typedef.h"
#include <defts201.h>

typedef enum  {	intKERNEL,  intTMR0LP=2, intTMR1LP, intLINK0=6, intLINK1, intLINK2, intLINK3,
				intDMA0=14, intDMA1, intDMA2, intDMA3, intDMA4 = 22, intDMA5, intDMA6, intDMA7,
				intDMA8 = 29, intDMA9, intDMA10, intDMA11, intDMA12=37, intDMA13,
				intIRQ0 = 41, intIRQ1, intIRQ2, intIRQ3, intVIRPT=48,  intBUSLK=50, intTMR0HP=52,
				intTMR1HP, intHW=57} InterruptRequest_type;

/*******************Include Function*********************/
#ifdef __cplusplus
extern "C"
{
#endif
void HAL_Interrupt_Enable(InterruptRequest_type intRQST, void *ptrHndlr);
void HAL_Interrupt_Disable(InterruptRequest_type intRQST);
void HAL_Interrupt_GlobalEnable(void);
void HAL_Interrupt_GlobalDisable(void);
void HAL_Interrupt_IMASKClear(void);
void HAL_Interrupt_ILATClear(void);
void HAL_Interrupt_RqstReset(InterruptRequest_type intRQST);
void HAL_Interrupt_RqstSet(InterruptRequest_type intRQST);
#ifdef __cplusplus
}
#endif // __cplusplus

/********************************************************/

#endif
