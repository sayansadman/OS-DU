#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include "../arch/stm32f446re/include/dev/timer.h"
#include <stdint.h>

void kmain(void)
{
	__sys_init();

	sysTick_init(10);
	int t = 0;
	while (1)
	{
		Delay_ms(1000);
		t += 1000;
		kprintf((uint8_t *)"%s", (uint8_t *)"TIM6: ");
		kprintf((uint8_t *)"%d", (uint8_t *)&t);
		kprintf((uint8_t *)"%s", (uint8_t *)" Systick: ");
		int ms = getTime();
		kprintf((uint8_t *)"%d\n", (uint8_t *)&ms);
		newline();
		if (t == 5000)
		{
			sysTick_disable();
		}
		if (t == 10000)
		{
			sysTick_enable();
		}
	}
}
