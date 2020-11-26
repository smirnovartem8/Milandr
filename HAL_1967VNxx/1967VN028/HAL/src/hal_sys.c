/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#include "hal_1967VN028.h"
#include <sysreg.h>
#include <builtins.h>
#include <stdint.h>


/*************************************************************************************************/
void HAL_SYS_SDRAMEnable(int sdrconValue){
	if( (__builtin_sysreg_read(__SYSTAT)&0x7) == 0)                     //If ID == 000
	{
		*(unsigned int *)(BCST_OFFSET_LOC+SDRCON_LOC) = sdrconValue;
		while( !(__builtin_sysreg_read(__SYSTAT)&(1<<13)) );            //Wait SDRAM Enable
	}
	else
		while( !(*(unsigned int*)(P0_OFFSET_LOC+SYSTAT_LOC)&(1<<13)) ); //Wait SDRAM Enable
}
/*************************************************************************************************/
void HAL_SYS_SDRAMDisable(void){
	if( (__builtin_sysreg_read(__SYSTAT)&0x7) == 0)                     //If ID == 000
		*(unsigned int *)(BCST_OFFSET_LOC+SDRCON_LOC) = 0;
	else
		__builtin_sysreg_write(__SDRCON, 0);
}
/*************************************************************************************************/
void HAL_SYS_SRAMEnable(int sysconValue){
	if( (__builtin_sysreg_read(__SYSTAT)&0x7) == 0)                     //If ID == 000
		*(unsigned int *)(BCST_OFFSET_LOC+SYSCON_LOC) = sysconValue;
	else
		while( __builtin_sysreg_read(__SYSCON) != sysconValue );
}
/*************************************************************************************************/
void HAL_SYS_SRAMDisable(void){
	if( (__builtin_sysreg_read(__SYSTAT)&0x7) == 0)                     //If ID == 000
		*(unsigned int *)(BCST_OFFSET_LOC+SYSCON_LOC) = 0;
	else
		__builtin_sysreg_write(__SYSCON, 0);
}
