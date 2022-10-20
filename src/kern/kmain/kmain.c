#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include "../arch/stm32f446re/include/dev/timer.h"
#include "../arch/stm32f446re/include/dev/usart.h"
#include <stdint.h>

void displayCore()
{
	kprintf((uint8_t *)"%s", (uint8_t *)"Core register values\n");
	uint32_t fault = get_faultMask();
	uint32_t basep = get_basePri();
	uint32_t prim = get_priMask();
	uint32_t isActive = __NVIC_GetActive(USART2_IRQn);

	kprintf((uint8_t *)"%s", (uint8_t *)"Fault: ");
	kprintf((uint8_t *)"%d", (uint8_t *)&fault);
	newline();

	kprintf((uint8_t *)"%s", (uint8_t *)"Base Priority: ");
	kprintf((uint8_t *)"%d", (uint8_t *)&basep);
	newline();

	kprintf((uint8_t *)"%s", (uint8_t *)"Primask: ");
	kprintf((uint8_t *)"%d", (uint8_t *)&prim);
	newline();

	kprintf((uint8_t *)"%s", (uint8_t *)"Is Active: ");
	kprintf((uint8_t *)"%d", (uint8_t *)&isActive);
	newline();
}

void kmain(void)
{
	__sys_init();

	sysTick_init(10);

	while (1)
	{
		Delay_ms(5000);
		// __NVIC_EnableIRQn(USART2_IRQn);
		// __enable_irq();
		// __enable_fault_irq();
		set_basePri(12);
		kprintf((uint8_t *)"%s", (uint8_t *)"\nUSART2 IRQ enabled\n");
		displayCore();

		Delay_ms(5000);
		// __NVIC_DisableIRQn(USART2_IRQn);
		// __disable_irq();
		// __disable_fault_irq();
		set_basePri(9);
		kprintf((uint8_t *)"%s", (uint8_t *)"\nUSART2 IRQ disabled\n");
		displayCore();
	}
}

void USART2_Handler()
{
	// simply return what was sent
	uint8_t c = UART_GetChar(USART2);
	uint32_t isActive = __NVIC_GetActive(USART2_IRQn);
	kprintf((uint8_t *)"%s", (uint8_t *)"\nIs Active: ");
	kprintf((uint8_t *)"%d", (uint8_t *)&isActive);
	newline();
	UART_SendChar(USART2, c);
}