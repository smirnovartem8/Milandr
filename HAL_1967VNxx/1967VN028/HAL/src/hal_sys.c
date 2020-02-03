/**********************************************************************************************************************
  Copyright (c) 2016 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
**********************************************************************************************************************/
#include "hal_1967VN028.h"
#include <sysreg.h>
#include <builtins.h>
#include <stdint.h>


/*************************************************************************************************/
void HAL_SYS_SDRAMEnable(int sdrconValue){
	__builtin_sysreg_write(__SDRCON, sdrconValue);
	while( !(__builtin_sysreg_read(__SYSTAT)&(1<<13)) ); //Wait SDRAM Enable
}
/*************************************************************************************************/
void HAL_SYS_SDRAMDisable(void){
	__builtin_sysreg_write(__SDRCON, 0);
}
/*************************************************************************************************/
void HAL_SYS_SRAMEnable(int sysconValue){
	__builtin_sysreg_write(__SYSCON, sysconValue);
}
/*************************************************************************************************/
void HAL_SYS_SRAMDisable(void){
	__builtin_sysreg_write(__SYSCON, 0);
}







