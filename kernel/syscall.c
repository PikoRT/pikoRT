// GENERATED. DO NOT EDIT FROM HERE!
// Change definitions in scripts/gen-syscalls.py
// Created on 2017-04-30 17:58

#include <kernel/syscalls.h>

int sys_pthread_exit();
int sys_pthread_self();
int sys_pthread_yield();
int sys_pthread_create();
int sys_pthread_join();
int sys_pthread_detach();
int sys_pthread_mutex_lock();
int sys_pthread_mutex_unlock();
int sys_pthread_cond_signal();
int sys_pthread_cond_wait();
int sys_timer_create();
int sys_timer_settime();
int sys_timer_gettime();
int sys_msleep();
int sys_sysconf();
int sys_sigaction();
int sys_kill();
int sys_open();
int sys_close();
int sys_read();
int sys_write();
int sys_lseek();
int sys_stat();
int sys_mount();
int sys_readdir_r();
int sys_getpid();
int sys_mmap();
int sys_munmap();
#define SYS_MAX 48

void *syscall_vect[SYS_MAX] = {
	[SYS_PTHREAD_EXIT] = sys_pthread_exit,
	[SYS_PTHREAD_SELF] = sys_pthread_self,
	[SYS_PTHREAD_YIELD] = sys_pthread_yield,
	[SYS_PTHREAD_CREATE] = sys_pthread_create,
	[SYS_PTHREAD_JOIN] = sys_pthread_join,
	[SYS_PTHREAD_DETACH] = sys_pthread_detach,
	[SYS_PTHREAD_MUTEX_LOCK] = sys_pthread_mutex_lock,
	[SYS_PTHREAD_MUTEX_UNLOCK] = sys_pthread_mutex_unlock,
	[SYS_PTHREAD_COND_SIGNAL] = sys_pthread_cond_signal,
	[SYS_PTHREAD_COND_WAIT] = sys_pthread_cond_wait,
	[SYS_TIMER_CREATE] = sys_timer_create,
	[SYS_TIMER_SETTIME] = sys_timer_settime,
	[SYS_TIMER_GETTIME] = sys_timer_gettime,
	[SYS_MSLEEP] = sys_msleep,
	[SYS_SYSCONF] = sys_sysconf,
	[SYS_SIGACTION] = sys_sigaction,
	[SYS_KILL] = sys_kill,
	[SYS_OPEN] = sys_open,
	[SYS_CLOSE] = sys_close,
	[SYS_READ] = sys_read,
	[SYS_WRITE] = sys_write,
	[SYS_LSEEK] = sys_lseek,
	[SYS_STAT] = sys_stat,
	[SYS_MOUNT] = sys_mount,
	[SYS_READDIR_R] = sys_readdir_r,
	[SYS_GETPID] = sys_getpid,
	[SYS_MMAP] = sys_mmap,
	[SYS_MUNMAP] = sys_munmap,
};

int syscall_register(unsigned ix, void *(*fn)())
{
	if (ix >= SYS_MAX) return -1;
	syscall_vect[ix] = fn;
	return 0;
}
