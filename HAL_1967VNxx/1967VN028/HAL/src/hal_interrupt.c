/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#include "hal_1967VN028.h"
#include <sysreg.h>
#include <builtins.h>


void HAL_Interrupt_Enable(InterruptRequest_type intRQST, void *ptrHndlr){
	unsigned int tmp32;

	switch(intRQST){
	case intKERNEL:
		__builtin_sysreg_write(__IVKERNEL, (unsigned int)ptrHndlr);
		break;
	case intTMR0LP:
		__builtin_sysreg_write(__IVTIMER0LP, (unsigned int)ptrHndlr);
		break;
	case intTMR1LP:
		__builtin_sysreg_write(__IVTIMER1LP, (unsigned int)ptrHndlr);
		break;
	case intLINK0:
		__builtin_sysreg_write(__IVLINK0, (unsigned int)ptrHndlr);
		break;
	case intLINK1:
		__builtin_sysreg_write(__IVLINK1, (unsigned int)ptrHndlr);
		break;
	case intLINK2:
		__builtin_sysreg_write(__IVLINK2, (unsigned int)ptrHndlr);
		break;
	case intLINK3:
		__builtin_sysreg_write(__IVLINK3, (unsigned int)ptrHndlr);
		break;
	case intDMA0:
		__builtin_sysreg_write(__IVDMA0, (unsigned int)ptrHndlr);
		break;
	case intDMA1:
		__builtin_sysreg_write(__IVDMA1, (unsigned int)ptrHndlr);
		break;
	case intDMA2:
		__builtin_sysreg_write(__IVDMA2, (unsigned int)ptrHndlr);
		break;
	case intDMA3:
		__builtin_sysreg_write(__IVDMA3, (unsigned int)ptrHndlr);
		break;
	case intDMA4:
		__builtin_sysreg_write(__IVDMA4, (unsigned int)ptrHndlr);
		break;
	case intDMA5:
		__builtin_sysreg_write(__IVDMA5, (unsigned int)ptrHndlr);
		break;
	case intDMA6:
		__builtin_sysreg_write(__IVDMA6, (unsigned int)ptrHndlr);
		break;
	case intDMA7:
		__builtin_sysreg_write(__IVDMA7, (unsigned int)ptrHndlr);
		break;
	case intDMA8:
		__builtin_sysreg_write(__IVDMA8, (unsigned int)ptrHndlr);
		break;
	case intDMA9:
		__builtin_sysreg_write(__IVDMA9, (unsigned int)ptrHndlr);
		break;
	case intDMA10:
		__builtin_sysreg_write(__IVDMA10, (unsigned int)ptrHndlr);
		break;
	case intDMA11:
		__builtin_sysreg_write(__IVDMA11, (unsigned int)ptrHndlr);
		break;
	case intDMA12:
		__builtin_sysreg_write(__IVDMA12, (unsigned int)ptrHndlr);
		break;
	case intDMA13:
		__builtin_sysreg_write(__IVDMA13, (unsigned int)ptrHndlr);
		break;
	case intIRQ0:
		__builtin_sysreg_write(__IVIRQ0, (unsigned int)ptrHndlr);
		break;
	case intIRQ1:
		__builtin_sysreg_write(__IVIRQ1, (unsigned int)ptrHndlr);
		break;
	case intIRQ2:
		__builtin_sysreg_write(__IVIRQ2, (unsigned int)ptrHndlr);
		break;
	case intIRQ3:
		__builtin_sysreg_write(__IVIRQ3, (unsigned int)ptrHndlr);
		break;
	case intVIRPT:
		__builtin_sysreg_write(__VIRPT, (unsigned int)ptrHndlr);
		break;
	case intBUSLK:
		__builtin_sysreg_write(__IVBUSLK, (unsigned int)ptrHndlr);
		break;
	case intTMR0HP:
		__builtin_sysreg_write(__IVTIMER0HP, (unsigned int)ptrHndlr);
		break;
	case intTMR1HP:
		__builtin_sysreg_write(__IVTIMER1HP, (unsigned int)ptrHndlr);
		break;
	case intHW:
		__builtin_sysreg_write(__IVHW, (unsigned int)ptrHndlr);
		break;
	case intSW:
		__builtin_sysreg_write(__IVSW, (unsigned int)ptrHndlr);
		return;
		break;
	}

	if (intRQST < 32){
		tmp32 = __builtin_sysreg_read(__IMASKL);
		__builtin_sysreg_write(__IMASKL, ( tmp32 | (1<<(int)intRQST) ));
	}
	else{
		tmp32 = __builtin_sysreg_read(__IMASKH);
		__builtin_sysreg_write(__IMASKH, (tmp32 | (1<<((int)intRQST-32))));
	}
}

void HAL_Interrupt_Disable(InterruptRequest_type intRQST){
	unsigned int tmp32;

	if (intRQST < 32){
		tmp32 = __builtin_sysreg_read(__IMASKL);
		__builtin_sysreg_write(__IMASKL, ( tmp32 & ~(1<<intRQST) ));
	}
	else{
		tmp32 = __builtin_sysreg_read(__IMASKH);
		__builtin_sysreg_write(__IMASKH, (tmp32 & ~(1<<(intRQST-32))));
	}

}


void HAL_Interrupt_GlobalEnable(void){
	__builtin_sysreg_write(__SQCTLST, SQCTL_GIE);

}

void HAL_Interrupt_SWGlobalEnable(void){
	__builtin_sysreg_write(__SQCTLST, SQCTL_SW);

}

void HAL_Interrupt_GlobalDisable(void){
	__builtin_sysreg_write(__SQCTLCL, ~SQCTL_GIE);

}

void HAL_Interrupt_SWGlobalDisable(void){
	__builtin_sysreg_write(__SQCTLCL, ~SQCTL_SW);

}

void HAL_Interrupt_IMASKClear(void){
	__builtin_sysreg_write(__IMASKL, 0);
	__builtin_sysreg_write(__IMASKH, 0);
}


void HAL_Interrupt_ILATClear(void){
	__builtin_sysreg_write(__ILATCLL, 0);
	__builtin_sysreg_write(__ILATCLH, 0);
}


void HAL_Interrupt_RqstReset(InterruptRequest_type intRQST){

	if (intRQST < 32){
		__builtin_sysreg_write( __ILATCLL, ~(1<<intRQST) );
	}
	else{
		__builtin_sysreg_write( __ILATCLH, ~(1<<(intRQST-32)) );
	}

}

void HAL_Interrupt_RqstSet(InterruptRequest_type intRQST){

	if (intRQST < 32){
		__builtin_sysreg_write( __ILATSTL, (1<<intRQST) );
	}
	else{
		__builtin_sysreg_write( __ILATSTH, (1<<(intRQST-32)) );
	}

}


