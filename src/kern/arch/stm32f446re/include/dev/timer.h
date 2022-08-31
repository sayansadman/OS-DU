#ifndef __TIMER_H
#define __TIMER_H
#include "../sys/stm32_peps.h"
void DRV_TIMER_INIT();
void TIM6Config(void);
void Delay_us(uint16_t);
void Delay_ms(uint16_t);
#endif
