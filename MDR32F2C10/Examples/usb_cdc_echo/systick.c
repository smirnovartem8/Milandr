#include "systick.h"

volatile uint32_t ulSysTick;

void SysTick_Handler(void)
{
    ulSysTick++;
}
