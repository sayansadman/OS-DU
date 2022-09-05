#include "../include/sys/sys.h"
#include "../include/sys/stm32_peps.h"
#include "../include/dev/nvic.h"
#include "../../../include/kstdio.h"
volatile uint32_t mscount = 0;

// since we are using clock source AHB/8
// frequency 180MHz / 8 = 22.5 MHz
// it means the cur val will decrement by 1 every 1/22.5*1000 milisecond or 1/22.5 microsecond

const int clockFreq = 22.5e3; // if mili 22.5e3, if micro 22.5, if second 22.5e6, however second is not possible because load register is only 24 bit (2^24 < 22.5 * 10^6)

// This function will be called every <<tick>> ms
// if tick = 10ms, every 10ms this function will be called.
// then add 10ms to current time count
void SysTick_Handler()
{
    // tick = STK->LOAD / clockFreq; It is the tick count provided in init
    mscount += ((STK->LOAD + 1) / clockFreq);
}

void sysTick_init(uint32_t tick)
{
    sysTick_disable();
    uint32_t reloadValue = (tick * clockFreq) - 1;
    STK->LOAD = reloadValue;
    STK->VAL = 0;
    STK->CTRL = 0; // AHB / 8 clock source, disable systick
    // STK->CTRL |= (1 << 2); // processor clock source
    STK->CTRL |= (1 << 1); // exception request enabled
    STK->CTRL |= (1 << 0); // enable clock source
}

void sysTick_enable(void)
{
    uint32_t ctrlReg = STK->CTRL;
    if (!(ctrlReg & 1))
    {
        STK->CTRL |= (1 << 0);
        // STK->CTRL |= (1 << 1);
        mscount = 0;
    }
}

void sysTick_disable(void)
{
    uint32_t ctrlReg = STK->CTRL;
    if (ctrlReg & 1)
    {
        STK->CTRL &= ~(1 << 0);
    }
}

uint32_t getSysTickCount(void)
{
    return STK->VAL;
}

void updateSysTick(uint32_t tick)
{
    sysTick_disable();
    uint32_t reloadValue = (tick * clockFreq) - 1;
    STK->LOAD = reloadValue;
    mscount = 0;
}

uint32_t getTime(void)
{
   
    return mscount + (STK->LOAD - STK->VAL) / clockFreq;
}