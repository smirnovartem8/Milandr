/**********************************************************************************************************************
  Copyright (c) 2016 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
**********************************************************************************************************************/
#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

#include "hal_typedef.h"
#include <defts201.h>

typedef enum {dmaCopyValid, dmaCopyAddrErr,dmaCopyLengthErr, dmaCopyChNumErr, dmaCopyInitActCh, dmaCopyCfgErr} DMAReturnType;
/*******************Include Function*********************/
#ifdef __cplusplus
extern "C"
{
#endif
void HAL_DMA_WriteDC (int ch_number, void *qw_tcb);
void HAL_DMA_WriteDCS(int ch_number, void *qw_tcb);
void HAL_DMA_WriteDCD(int ch_number, void *qw_tcb);
void HAL_DMA_ReadDC (int ch_number, __builtin_quad *qw_tcb);
void HAL_DMA_ReadDCS(int ch_number, __builtin_quad *qw_tcb);
void HAL_DMA_ReadDCD(int ch_number, __builtin_quad *qw_tcb);
void HAL_DMA_Stop(int ch_number);
int HAL_DMA_GetChannelStatus(int channel);
int HAL_DMA_GetChannelStatusClear(int channel);
int HAL_DMA_WaitForChannel(int channel);
DMAReturnType HAL_DMA_MemCopy32(int ch_number, const void *src, const void *dst, int size);
int HAL_DMA_GetTCBChannelDest(int channel);
void HAL_DMA_CreateChannelDest(int channel, void *tcb_current, void *tcb_next );


#ifdef __cplusplus
}
#endif // __cplusplus

/********************************************************/

#endif
