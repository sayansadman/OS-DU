#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include "../arch/stm32f446re/include/dev/timer.h"
#include "../arch/stm32f446re/include/dev/usart.h"
#include <stdint.h>

void kmain(void) {
	__sys_init();

	sysTick_init(10);
	while (1) {
		Delay_ms(5000);
		__NVIC_EnableIRQn(USART2_IRQn);
		kprintf((uint8_t*)"%s", (uint8_t*)"\nUSART2 IRQ enabled\n");
		Delay_ms(5000);
		__NVIC_DisableIRQn(USART2_IRQn);
		kprintf((uint8_t*)"%s", (uint8_t*)"\nUSART2 IRQ disabled\n");
	}
}

void USART2_Handler() {
	// simply return what was sent
	uint8_t c = UART_GetChar(USART2);
	UART_SendChar(USART2, c);
}