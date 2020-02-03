
//PUx = j4;;
#define PUx_Write(reg_name)\
__INSTR	 0x89851F00 + (reg_name << 0)

//j1 = PUx;;
#define PUx_Read(reg_name)\
__INSTR	 0x8BE00C02 + (reg_name << 16)

//PU_CR = j4;;
#define PU_CR_Write()\
__INSTR  0x89851F38

//j1 = CR_CR;;
#define PU_CR_Read()\
__INSTR  0x8BFC0C02
