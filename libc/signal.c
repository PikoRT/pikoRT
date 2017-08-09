/* syscall wrappers */

#include <signal.h>

#include <kernel/syscalls.h>
#include "piko/syscalls.h"

int sigaction(int sig,
              const struct sigaction *restrict act,
              struct sigaction *restrict oact)
{
    return do_syscall3((void *) sig, (void *) act, (void *) oact,
                       SYS_SIGACTION);
}

int _kill(pid_t pid, int sig)
{
    return do_syscall2((void *) pid, (void *) sig, SYS_KILL);
}
