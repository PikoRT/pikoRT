#ifndef KERNEL_COND_H
#define KERNEL_COND_H

#include <sys/types.h>

int sys_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int sys_pthread_cond_signal(pthread_cond_t *cond);

#endif /* !KERNEL_COND_H */
