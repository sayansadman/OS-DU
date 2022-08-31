#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/systick.h"
#include "../arch/stm32f446re/include/dev/timer.h"
#include <stdint.h>


void kmain(void) {
	__sys_init();

	sysTick_init(100);
	while (1) {
		Delay_ms(1000);
		getTime();
	}
}
