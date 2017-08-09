#ifndef _KERNEL_MUTEX_H
#define _KERNEL_MUTEX_H

typedef _Atomic int atomic_s32;

typedef struct {
    atomic_s32 val;
} kernel_mutex_t;

int sys_pthread_mutex_lock(kernel_mutex_t *mutex);
int sys_pthread_mutex_unlock(kernel_mutex_t *mutex);

#endif /* !_KERNEL_MUTEX_H */
