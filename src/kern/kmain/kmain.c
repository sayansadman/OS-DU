#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include "../arch/stm32f446re/include/dev/timer.h"
#include <stdint.h>

void kmain(void) {
	__sys_init();

	sysTick_init(10);
	int t = 0;
	while (1) {
		Delay_ms(5000);
		__NVIC_EnableIRQn(USART2_IRQn);
		kprintf((uint8_t*)"%s", (uint8_t*)"USART2 IRQ enabled");
		Delay_ms(5000);
		__NVIC_DisableIRQn(USART2_IRQn);
		kprintf((uint8_t*)"%s", (uint8_t*)"USART2 IRQ enabled");
	}
}

void USART2_Handler() {
	kprintf((uint8_t*)"%s", (uint8_t*)"USART2 IRQ handler invoked");
}