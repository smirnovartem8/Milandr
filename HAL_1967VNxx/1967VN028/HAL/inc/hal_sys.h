/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#ifndef __HAL_SYS_H__
#define __HAL_SYS_H__

#include <defts201.h>
#include <sysreg.h>

#define __NOP			asm("nop;;")
#define __IDLE			asm("idle;;")
#define __RDS			asm("rds;;")
#define __BTBEN			asm("btben;;")
#define __WDT_REINIT	0x04072013
#define SysregWrite		__builtin_sysreg_write
#define SysregRead		__builtin_sysreg_read

#ifdef __cplusplus
extern "C"
{
#endif

	void HAL_SYS_SDRAMEnable(int sdrconValue);
	void HAL_SYS_SDRAMDisable(void);
	void HAL_SYS_SRAMEnable(int sysconValue);
	void HAL_SYS_SRAMDisable(void);

	__inline	__attribute__((always_inline)) void HAL_SYS_WaitClk(unsigned int clk){
	    unsigned long long int stop = __read_ccnt();
	    stop += clk;
	    while ((((unsigned long long int)(__read_ccnt())) - stop) & ((unsigned long long int)1<<63));
	}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
