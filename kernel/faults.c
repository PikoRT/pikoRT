#include <kernel/faults.h>
#include <kernel/thread.h>

#include "kernel.h"
#include "utils.h"
#include "platform.h"

void fault_enter(const char *s)
{
	printk("\n-------------------------------------------------------------\n");
	printk(" #%s\n\n", s);
}

void fault_exit(void)
{
	printk("-------------------------------------------------------------\n");
	__platform_halt();
}

void hardfault(struct kernel_context_regs *noscratch,
               struct thread_context_regs *scratch,
               u32 exc_return)
{
	fault_enter("HardFault");
	dump_frame(noscratch, scratch, exc_return);
	fault_exit();
}
