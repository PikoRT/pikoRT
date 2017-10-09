#include <kernel/compiler.h>
#include <serial.h>

#include "platform.h"

#define CPU_FREQ_IN_HZ 72000000
#define SYSTICK_FREQ_IN_HZ 1000
#define SYSTICK_PERIOD_IN_MSECS (SYSTICK_FREQ_IN_HZ / 1000)

struct timer_operations;

void config_timer_operations(struct timer_operations *tops);

extern struct timer_operations systick_tops;

void rcc_clock_init(void);
void __uart_enable(void);

__weak void __platform_init(void)
{
    config_timer_operations(&systick_tops);

    /* SysTick running at 1kHz */
    SysTick_Config(CPU_FREQ_IN_HZ / SYSTICK_FREQ_IN_HZ);
}

__weak void __platform_halt(void)
{
    for (;;)
        ;
}

void __printk_init(void)
{
    usart_init();
}

void __printk_putchar(char c)
{
    if (c == '\n')
        __printk_putchar('\r');

    while (!(USART2->SR & USART_SR_TXE))
        ;
    USART2->DR = (0xff) & c;
}
