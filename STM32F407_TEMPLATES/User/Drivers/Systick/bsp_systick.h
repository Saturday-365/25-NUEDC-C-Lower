#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f4xx.h"

extern __IO u32 TimingDelay;
void Systick_Delay_ms(__IO u32 nTime);

void SysTick_Init(void);
//void SysTick_Delay_Ms( __IO uint32_t ms);

#endif /* __SYSTICK_H */

