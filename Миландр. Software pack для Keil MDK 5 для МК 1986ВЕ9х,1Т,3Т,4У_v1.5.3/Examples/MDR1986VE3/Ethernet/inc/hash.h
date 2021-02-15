#ifndef __HASH_H
#define __HASH_H

#include <stdint.h>
#include "MDR32F9Qx_config.h"

uint32_t Crc( uint8_t* arr, uint32_t len );
uint64_t CalcHash (MDR_ETHERNET_TypeDef* ETHERNETx, uint8_t* mac);

#endif
