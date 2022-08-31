#include "../include/sys/sys.h"
#include "../include/sys/stm32_peps.h"
// #include "../include/sys/stm32_startup.h"
#include "../include/dev/nvic.h"
#include "../../../include/kstdio.h"
volatile uint32_t mscount = 0;

void SysTick_Handler()
{
    mscount++;
}

void sysTick_init(uint32_t reloadValue)
{
    // reset value
    STK->CTRL = 0x00000000;
    STK->VAL = 0;
    STK->LOAD = reloadValue;
    STK->CTRL |= (1 << 1); // exception request enabled
    STK->CTRL |= (1 << 0);
}

void sysTick_enable(void)
{
    uint32_t ctrlReg = STK->CTRL;
    if (!(ctrlReg && 1))
    {
        STK->CTRL |= (1 << 0);
        mscount = 0;
    }
}

void sysTick_disable(void)
{
    uint32_t ctrlReg = STK->CTRL;
    if (ctrlReg && 1)
    {
        STK->CTRL |= (0 << 0);
    }
}

uint32_t getSysTickCount(void)
{
    return STK->VAL;
}

void updateSysTick(uint32_t reloadValue)
{
    sysTick_disable();
    STK->LOAD = reloadValue;
    mscount = 0;
}

uint32_t getTime(void)
{

    kprintf((uint8_t *)"%d\n", (uint8_t *)&mscount);
    newline();
    return mscount;
}