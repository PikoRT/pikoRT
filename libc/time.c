/* syscall wrappers */

#include <signal.h>
#include <sys/types.h>
#include <time.h>

#include <kernel/syscalls.h>
#include "piko/syscalls.h"

int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid)
{
	return do_syscall3((void *) clockid, (void *) sevp, (void *) timerid,
	                   SYS_TIMER_CREATE);
}

int timer_settime(timer_t timerid,
                  int flags,
                  const struct itimerspec *new_value,
                  struct itimerspec *old_value)
{
	return do_syscall4((void *) timerid, (void *) flags, (void *) new_value,
	                   (void *) old_value, SYS_TIMER_SETTIME);
}

int timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
	return do_syscall2((void *) timerid, (void *) curr_value,
	                   SYS_TIMER_GETTIME);
}
