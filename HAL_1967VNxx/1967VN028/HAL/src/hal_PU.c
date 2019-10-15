#include "hal_PU.h"
#include <stdint.h>

void HAL_PUx_Write(PUx_Write Reg_Name, uint32_t Value){
	switch(Reg_Name){
		case PU0_Wr   : asm("PUx_Write(0);;");
					    break;
		case PU1_Wr   : asm("PUx_Write(2);;");
					    break;
		case PU2_Wr   : asm("PUx_Write(4);;");
					    break;
		case PU3_Wr   : asm("PUx_Write(6);;");
					    break;
		case PU4_Wr   : asm("PUx_Write(8);;");
					    break;
		case PU5_Wr   : asm("PUx_Write(10);;");
					    break;
		case PU6_Wr   : asm("PUx_Write(12);;");
					    break;
		case PU7_Wr   : asm("PUx_Write(14);;");
					    break;
		case PU_CR_Wr : asm("PU_CR_Write();;");
		  	  	  	    break;
	}
}

uint32_t HAL_PUx_Read (PUx_Read Reg_Name){
	uint32_t Value_reg;

	switch(Reg_Name){
		case PU0_Rd   : asm("PUx_Read(0);;");
						break;
		case PU1_Rd   : asm("PUx_Read(1);;");
						break;
		case PU2_Rd   : asm("PUx_Read(2);;");
						break;
		case PU3_Rd   : asm("PUx_Read(3);;");
						break;
		case PU4_Rd   : asm("PUx_Read(4);;");
						break;
		case PU5_Rd   : asm("PUx_Read(5);;");
						break;
		case PU6_Rd   : asm("PUx_Read(6);;");
						break;
		case PU7_Rd   : asm("PUx_Read(7);;");
						break;
		case PU_CR_Rd : asm("PU_CR_Read();;");
						break;
	}
	asm("[%0+0] = j1;;"\
	                 ::"j"(&Value_reg));
	return Value_reg;
}
