#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>
#include "./stm32_peps.h"
#define STK ((SysTick_Typedef *) 0xE000E010)

void sysTick_init(uint32_t);
void sysTick_enable(void);
void sysTick_disable(void);
uint32_t getSysTickCount(void);
void updateSysTick(uint32_t);
uint32_t getTime(void);

#endif // __SYS_H
