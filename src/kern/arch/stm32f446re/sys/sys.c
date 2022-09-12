#include<stdint.h>
#include "../include/sys/sys.h"
#include "../include/sys/stm32_peps.h"
#include "../../../include/kstdio.h"
volatile uint32_t mscount = 0;
// since we are using clock source AHB/8
// frequency 180MHz / 8 = 22.5 MHz
// it means the cur val will decrement by 1 every 1/22.5*1000 milisecond or 1/22.5 microsecond

const int clockFreq = 22.5e3; // if mili 22.5e3, if micro 22.5, if second 22.5e6, however second is not possible because load register is only 24 bit (2^24 < 22.5 * 10^6)

// This function will be called every <<tick>> ms
// if tick = 10ms, every 10ms this function will be called.
// then add 10ms to current time count
void SysTick_Handler() {
    // tick = STK->LOAD / clockFreq; It is the tick count provided in init
    mscount += ((STK->LOAD + 1) / clockFreq);
}

void sysTick_init(uint32_t tick) {
    sysTick_disable();
    uint32_t reloadValue = (tick * clockFreq) - 1;
    STK->LOAD = reloadValue;
    STK->VAL = 0;
    STK->CTRL = 0; // AHB / 8 clock source, disable systick
    // STK->CTRL |= (1 << 2); // processor clock source
    STK->CTRL |= (1 << 1); // exception request enabled
    STK->CTRL |= (1 << 0); // enable clock source
}

void sysTick_enable(void) {
    uint32_t ctrlReg = STK->CTRL;
    if (!(ctrlReg & 1)) {
        STK->CTRL |= (1 << 0);
        // STK->CTRL |= (1 << 1);
        mscount = 0;
    }
}

void sysTick_disable(void) {
    uint32_t ctrlReg = STK->CTRL;
    if (ctrlReg & 1) {
        STK->CTRL &= ~(1 << 0);
    }
}

uint32_t getSysTickCount(void) {
    return STK->VAL;
}

void updateSysTick(uint32_t tick) {
    sysTick_disable();
    uint32_t reloadValue = (tick * clockFreq) - 1;
    STK->LOAD = reloadValue;
    mscount = 0;
}

uint32_t getTime(void) {

    return mscount + (STK->LOAD - STK->VAL) / clockFreq;
}

void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority) {

    if (IRQn < 0) {
        // for IRQn < 0 there can be max 15 priority.
        // But this priority is left shifted by 4 when setting up.
        uint8_t prty = (priority << 4) & 0xff00;
        //{ -14, -12, -11, -10,  -5,  -4,  -2,  -1 }
        //{  2,   4,    5,   6,  11,  12,  14,  15 }
        uint32_t pri = (uint32_t)((int32_t)IRQn & 0xf);
        if (pri >= 4 && pri <= 6) {
            SCB->SHPR1 |= (uint8_t)prty << ((pri - 4) * 8);
        }
        else if (pri == 11) {
            SCB->SHPR2 |= (uint8_t)prty << 24;
        }
        else if (pri == 14) {
            SCB->SHPR3 |= (uint8_t)prty << 16;
        }
        else if (pri == 15) {
            SCB->SHPR3 |= (uint8_t)prty << 24;
        }
    }
    else {
        NVIC->IP[IRQn] = (uint8_t)((priority << 4) & 0xff);
    }
}
uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn) {
    if (IRQn < 0) {
        uint32_t pri = (uint32_t)((int32_t)IRQn & 0xf);
        uint32_t ret = 0;
        if (pri >= 4 && pri <= 6) {
            uint32_t r1 = SCB->SHPR1;
            ret = (r1 >> ((pri - 4) * 8)) & 0xff;
        }
        else if (pri == 11) {
            uint32_t r2 = SCB->SHPR2;
            ret = r2 >> 24;
        }
        else if (pri == 14) {
            uint32_t r3 = SCB->SHPR3;
            ret = (r3 >> 16) & 0xff;
        }
        else if (pri == 15) {
            uint32_t r3 = SCB->SHPR3;
            ret = r3 >> 24;
        }
        return ((ret >> 4) & 0x00ff);
    }
    else {
        return NVIC->IP[IRQn] >> 4;
    }
}
void __NVIC_EnableIRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0) {
        uint32_t i = IRQn / 32;
        uint32_t j = IRQn % 32;
        NVIC->ISER[i] |= (uint32_t)(1 << j);
    }
}
void __NVIC_DisableIRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0) {
        uint32_t i = IRQn / 32;
        uint32_t j = IRQn % 32;
        NVIC->ICER[i] |= (uint32_t)(1 << j);
    }
}
static inline void __set_BASEPRI(uint32_t value) {
    register uint32_t regBASEPRI asm("basepri");
    regBASEPRI = (value & 0xff);
}
void __disable_irq() {
    __set_PRIMASK(1);
}
void __enable_irq() {
    __set_PRIMASK(0);
}
static inline void __unset_BASEPRI(uint32_t value) {
    register uint32_t regBASEPRI asm("basepri");
    __set_BASEPRI(regBASEPRI - value);
}

uint32_t __get_PRIMASK(void) {
    register uint32_t regPRIMASK asm("primask");
    return(regPRIMASK);
}


static inline void __set_PRIMASK(uint32_t priMask) {
    register uint32_t regPRIMASK asm("primask");
    regPRIMASK = (priMask);
}

void __enable_fault_irq(void) {
    __set_FAULTMASK(0);
}
static inline void __set_FAULTMASK(uint32_t faultMask) {
    register uint32_t __regFaultMask asm("faultmask");
    __regFaultMask = (faultMask & (uint32_t)1);
}
void __disable_fault_irq(void) {
    __set_FAULTMASK(1);
}
static inline uint32_t __get_FAULTMASK(void) {
    register uint32_t __regFaultMask asm("faultmask");
    return (__regFaultMask);
}

void __clear_pending_IRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0) {
        uint32_t i = IRQn / 32;
        uint32_t j = IRQn % 32;
        NVIC->ICPR[i] = (uint32_t)(1 << j);
    }
}
uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn) {
    uint32_t i = IRQn / 32;
    uint32_t j = IRQn % 32;
    return (uint32_t)((NVIC->ISPR[i] & (uint32_t)(1 << j)) == 0 ? 0 : 1);
}
uint32_t __NVIC_GetActive(IRQn_TypeDef IRQn) {
    uint32_t i = IRQn / 32;
    uint32_t j = IRQn % 32;
    return (uint32_t)((NVIC->IABR[i] & (uint32_t)(1 << j)) == 0 ? 0 : 1);
}