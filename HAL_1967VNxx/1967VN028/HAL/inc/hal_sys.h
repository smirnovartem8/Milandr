/**********************************************************************************************************************
  Copyright (c) 2016 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#ifndef __HAL_SYS_H__
#define __HAL_SYS_H__

#include <defts201.h>

#define __NOP			asm("nop;;")
#define __IDLE			asm("idle;;")
#define __RDS			asm("rds;;")
#define __BTBEN			asm("btben;;")
#define __WDT_REINIT	0x04072013
#define SysregWrite		__builtin_sysreg_write
#define SysregRead		__builtin_sysreg_read
#define FIX_IT_BUG() \
       asm("JL0 = J27 + 0x100;;"); \
       asm("JB0 = 8;;"); \
       asm("KL0 = JL0;;"); \
       asm("KB0 = JB0;;")

#ifdef __cplusplus
extern "C"
{
#endif
	void HAL_SYS_SDRAMEnable(int sdrconValue);
	void HAL_SYS_SDRAMDisable(void);
	void HAL_SYS_SRAMEnable(int sysconValue);
	void HAL_SYS_SRAMDisable(void);

	/*static inline void HAL_SYS_WaitClk(unsigned int clk){
	    unsigned long long int stop = __read_ccnt();
	    stop += clk;
	    while ((((unsigned long long int)(__read_ccnt())) - stop) & ((unsigned long long int)1<<63));
	}*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
