#ifndef KERNEL_FAULTS_H
#define KERNEL_FAULTS_H

#include <sys/types.h>
#include <kernel/thread.h>

void fault_enter(const char *s);
void fault_exit(void);

/* arch-dependent */
void dump_frame(struct kernel_context_regs *noscratch,
                struct thread_context_regs *scratch,
                u32 exc_return);

#endif /* !KERNEL_FAULTS_H */
