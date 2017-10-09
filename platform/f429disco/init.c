#include <serial.h>
#include <kernel/compiler.h>

#include "platform.h"

#define CPU_FREQ_IN_HZ 168000000
#define SYSTICK_FREQ_IN_HZ 1000
#define SYSTICK_PERIOD_IN_MSECS (SYSTICK_FREQ_IN_HZ / 1000)

struct timer_operations;

void config_timer_operations(struct timer_operations *tops);

extern struct timer_operations systick_tops;

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
