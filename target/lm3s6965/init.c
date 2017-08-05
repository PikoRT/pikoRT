#include <kernel/compiler.h>

#include "platform.h"

#define SYSTICK_FREQ_IN_HZ 1000
#define SYSTICK_PERIOD_IN_MSECS (SYSTICK_FREQ_IN_HZ / 1000)

struct timer_operations;

void config_timer_operations(struct timer_operations *tops);

extern struct timer_operations systick_tops;

void lm3s6965_init(void);

__weak void __platform_init(void)
{
	config_timer_operations(&systick_tops);

	/* create /dev/ttyS0, serial interface for QEMU UART0 */
	lm3s6965_init();
}

__weak void __platform_halt(void)
{
	for (;;)
		;
}

void __printk_init(void)
{
	UART0->CTL |= 1;         /* UART enabled */
	UART0->LCRH |= (3 << 5); /* 8 bits word length, no parity */

	NVIC_SetPriority(UART1_IRQn, 0xE);
}

void __printk_putchar(char c)
{
	while (UART0->FR & (1 << 3))
		;
	UART0->DR = c;
}
