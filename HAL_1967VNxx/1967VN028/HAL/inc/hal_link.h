/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

  History:
  	  Zatonsky Pavel - Created.
  	  Zatonsky Pavel - Corrected.
**********************************************************************************************************************/
#ifndef __HAL_LINK_H__
#define __HAL_LINK_H__

#include "hal_typedef.h"
#include <defts201.h>

#define __IO 			volatile

typedef enum {Link_CheckSum_Dis = 0,	Link_CheckSum_En = LRCTL_RVERE} 	Link_CheckSum_type;
typedef enum {Link_CheckBCMP_Dis = 0,	Link_CheckBCMP_En = LRCTL_RBCMPE} 	Link_CheckBCMP_type;
typedef enum {Link_TOIT_Dis = 0,		Link_TOIT_En = LRCTL_RTOE} 			Link_TOIT_type;
typedef enum {Link_OvrIT_Dis = 0,		Link_OvrIT_En = LRCTL_ROVRE} 		Link_OvrIT_type;
typedef enum {Link_DataSize_1bit = 0,	Link_DataSize_4bit = LRCTL_RDSIZE} 	Link_DataSize_type;
typedef enum {Link_ClkDiv_1 = LTCTL_TCLKDIV1, Link_ClkDiv_1p5 = LTCTL_TCLKDIV1P5, Link_ClkDiv_2 = LTCTL_TCLKDIV2, Link_ClkDiv_4 = LTCTL_TCLKDIV4} Link_ClkDiv_type;


typedef struct{
	__IO Link_CheckSum_type		CheckSum;
	__IO Link_CheckBCMP_type 	CheckBCMPI;
	__IO Link_DataSize_type  	DataSize;
	__IO Link_TOIT_type 		TOIntEn;
	__IO Link_OvrIT_type 		OVRIntEn;
}LinkRx_Init_type;

typedef struct{
	__IO Link_CheckSum_type 	CheckSum;
	__IO Link_CheckBCMP_type 	CheckBCMPO;
	__IO Link_DataSize_type  	DataSize;
	__IO Link_TOIT_type 		TOIntEn;
	__IO Link_ClkDiv_type  		ClkDiv;
}LinkTx_Init_type;


/*******************Include Function*********************/
#ifdef __cplusplus
extern "C"
{
#endif

int  HAL_LinkRx_GetStat(int link_num);
int  HAL_LinkRxCl_GetStat(int link_num);
int  HAL_LinkTx_GetStat(int link_num);
int  HAL_LinkTxCl_GetStat(int link_num);

int  HAL_LinkRx_Enable(int link_num, LinkRx_Init_type *LRxInit);
void HAL_LinkRx_Disable(int link_num);
int  HAL_LinkCore_Receive(int link_num, void *BuffPtr, int size);
void HAL_LinkDMA_Receive(int link_num, void *tcb);

int  HAL_LinkTx_Enable(int link_num, LinkTx_Init_type *LTxInit);
void HAL_LinkTx_Disable(int link_num);
int  HAL_LinkCore_Transmit(int link_num, void *BuffPtr, int size);
void HAL_LinkDMA_Transmit(int link_num, void *tcb);

#ifdef __cplusplus
}
#endif // __cplusplus

/********************************************************/

#endif
