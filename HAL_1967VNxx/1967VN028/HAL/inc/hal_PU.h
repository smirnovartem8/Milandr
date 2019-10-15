#include <stdint.h>


#define PU0_LOC		0x1E03E0
#define PU1_LOC		0x1E03E1
#define PU2_LOC		0x1E03E2
#define PU3_LOC 	0x1E03E3
#define PU4_LOC 	0x1E03E4
#define PU5_LOC		0x1E03E5
#define PU6_LOC		0x1E03E6
#define PU7_LOC		0x1E03E7
#define PU_CR_LOC	0x1E03FC


//PUx = j5;;
asm ("#define PUx_Write(reg_name)\
         __INSTR  0x89851F00 + (reg_name << 0)");

//j1 = PUx;;
asm ("#define PUx_Read(reg_name)\
      __INSTR     0x8BE00C02 + (reg_name << 16)");

//PU_CR = j5;;
asm ("#define PU_CR_Write()\
         __INSTR  0x89851F38");

//j1 = CR_CR;;
asm ("#define PU_CR_Read()\
        __INSTR  0x8BFC0C02");

/**********************************************************/

typedef enum {
	PU0_Wr	 = 0,
	PU1_Wr	 = 2,
	PU2_Wr	 = 4,
	PU3_Wr	 = 6,
	PU4_Wr 	 = 8,
	PU5_Wr 	 = 10,
	PU6_Wr 	 = 12,
	PU7_Wr	 = 14,
	PU_CR_Wr = 16
}PUx_Write;


typedef enum {
	PU0_Rd	 = 0,
	PU1_Rd	 = 1,
	PU2_Rd	 = 2,
	PU3_Rd	 = 3,
	PU4_Rd 	 = 4,
	PU5_Rd 	 = 5,
	PU6_Rd 	 = 6,
	PU7_Rd	 = 7,
	PU_CR_Rd = 8
}PUx_Read;

/*******************Include Function**********************/
#ifdef __cplusplus
extern "C"
{
#endif

	void HAL_PUx_Write(PUx_Write Reg_Name, uint32_t value);
	uint32_t HAL_PUx_Read (PUx_Read Reg_Name);

#ifdef __cplusplus
}
#endif // __cplusplus

/*********************************************************/
