/*
 *
 *	Файл: 		hal_sys.c
 *	Описание: 	HAL для внешней шины памяти
 *
 *	История:
 *						28-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию, добавлена функция задания периода регенерации
 *									Zatonsky Pavel 		- 	создан
 *
 */
#include <sysreg.h>
#include <builtins.h>
#include <stdint.h>
#include "hal_1967VN044.h"

#define TIC_PERIOD_GCF	16000
uint64_t ccntStart, ccntStop = 0;

// Включение внешнего порта
void HAL_SYS_ExtBusEnable( SYS_BusSize_type size )
{
	uint32_t tmp = 0;
	switch ( size )
	{
		case SYS_BUS_16b:
			tmp = PX_ALT_PDB0 | PX_ALT_PDB1 | PX_ALT_PAB0 | PX_ALT_PAB1 | PX_ALT_PAB2;
			break;
		case SYS_BUS_32b:
			tmp = PX_ALT_PDB0 | PX_ALT_PDB1 | PX_ALT_PDB2 | PX_ALT_PDB3 | PX_ALT_PAB0 | PX_ALT_PAB1	| PX_ALT_PAB2;
			break;
	}
	*( uint32_t * ) PX_ALT_LOC = tmp;
}

// Выключение внешнего порта
void HAL_SYS_ExtBusDisable( void )
{
	*( uint32_t * ) PX_ALT_LOC = 0;
}

// Включение SDRAM
void HAL_SYS_SDRAMEnable( uint32_t sdrconValue )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = ( 0x3FF << 8 );
	__builtin_sysreg_write( __SDRCON, sdrconValue );
	while( !( __builtin_sysreg_read( __SYSTAT ) & ( 1 << 13 ) ) );
}

// Задание периода регенерации SDRAM
void HAL_SYS_SDRAMRegenPer( uint16_t RegenPer )
{
	*( uint16_t* ) SD_REP_LOC = RegenPer;
}

// Выключение SDRAM
void HAL_SYS_SDRAMDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = ( 0x3FF << 8 );
	__builtin_sysreg_write( __SDRCON, 0 );
}

// Включение SRAM
void HAL_SYS_SRAMEnable( uint32_t sysconValue )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = ( 0x1F << 18 );
	__builtin_sysreg_write( __SYSCON, sysconValue );
}

// Выключение SRAM
void HAL_SYS_SRAMDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = ( 0x1F << 18 );
	__builtin_sysreg_write( __SYSCON, 0 );
}

// Включение FLAG
void HAL_SYS_FlagEnable( void )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = 0xF;
}

// Выключение FLAG
void HAL_SYS_FlagDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = 0xF;
}

// Задание наивысшего приоритета процессору перед DMA
void HAL_SYS_CorePrimariPriority( void )
{
	*( uint32_t * ) CMU_CFG1_LOC &= ~( 1 << 12 );
}

// Разрешение многократной записи в регистры SYSCON и SDRCON
void HAL_SYS_MultipleWriteSysconSdrconEn( void )
{
	LX_CMU->CFG1.b.SYS_WE = 1;
}

// Пауза в N мс
void HAL_SYS_WaitMs( uint32_t ms )
{
	uint64_t ullStop = __read_ccnt();
	ullStop += ms * PLL_Freq.CoreClk;

	while( ( ( ( uint64_t ) ( __read_ccnt() ) ) - ullStop ) & ( ( uint64_t ) 1 << 63 ) );
}
