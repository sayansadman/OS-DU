#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include <stdint.h>


void kmain(void)
{
	__sys_init();

	sysTick_init(1000000);
	while (1)
	{
		for (int i = 0; i < 10000000; i++)
			;
		getTime();
	}
}
