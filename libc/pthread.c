#include <pthread.h>
#include <stddef.h>
#include <string.h>

#include "linux/list.h"

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	if (!attr)
		return -1;
	attr->stacksize = stacksize;

	return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
	if (!attr)
		return -1;
	*stacksize = attr->stacksize;

	return 0;
}

const pthread_attr_t pthread_attr_default = {.stacksize = 1024};

int pthread_attr_init(pthread_attr_t *attr)
{
	if (!attr)
		return -1;
	memcpy(attr, &pthread_attr_default, sizeof(pthread_attr_t));

	return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex, __unused const pthread_mutexattr_t *attr)
{
	*mutex = -1;

	return 0;
}

int pthread_cond_init(__unused pthread_cond_t *cond,
                      __unused const pthread_condattr_t *attr)
{
	return 0;
}

/* syscall wrappers */

#include <kernel/syscalls.h>

#include "piko/syscalls.h"

int sched_yield(void)
{
	return do_syscall0(SYS_PTHREAD_YIELD);
}

int pthread_yield(void) __attribute__((alias("sched_yield")));

pthread_t pthread_self(void)
{
	return (pthread_t) do_syscall0(SYS_PTHREAD_SELF);
}

void pthread_exit(void *retval)
{
	do_syscall1((void *) retval, SYS_PTHREAD_EXIT);

	/* compiler complains about 'noreturn' function does return */
	for (;;)
		;
}

int pthread_detach(pthread_t thread)
{
	return do_syscall1((void *) thread, SYS_PTHREAD_DETACH);
}

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg)
{
	return do_syscall4((void *) thread, (void *) attr, (void *) start_routine,
	                   arg, SYS_PTHREAD_CREATE);
}

int pthread_join(pthread_t thread, void **retval)
{
	return do_syscall2((void *) thread, (void *) retval, SYS_PTHREAD_JOIN);
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	return do_syscall2((void *) cond, (void *) mutex, SYS_PTHREAD_COND_WAIT);
}

int pthread_cond_signal(pthread_cond_t *cond)
{
	return do_syscall1((void *) cond, SYS_PTHREAD_COND_SIGNAL);
}
