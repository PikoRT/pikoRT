#include <kernel/kernel.h>
#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/thread.h>

#include "linux/list.h"

static LIST_HEAD(mutex_head);

/* The thread owns the mutex on return. We also check the case when the lock
 * has been released between the test of the mutex and this syscall. */
int sys_pthread_mutex_lock(kernel_mutex_t *mutex)
{
    mutex->val++;
    if (!mutex->val)
        return 0;
    CURRENT_THREAD_INFO(curr_thread);
    curr_thread->ti_private = mutex;
    curr_thread->ti_state = THREAD_STATE_BLOCKED;
    list_add_tail(&curr_thread->ti_q, &mutex_head);
    sched_elect(SCHED_OPT_NONE);

    return 0;
}

static struct thread_info *find_first_blocking_thread(kernel_mutex_t *mutex)
{
    struct thread_info *thread;

    list_for_each_entry(thread, &mutex_head, ti_q)
    {
        if (thread->ti_private == mutex)
            return thread;
    }

    return NULL;
}

int sys_pthread_mutex_unlock(kernel_mutex_t *mutex)
{
    struct thread_info *waiter = NULL;

    mutex->val--;
    if (mutex->val >= 0) {
        waiter = find_first_blocking_thread(mutex);
        if (!waiter) {
            printk("[mutex_unlock] No blocking threads for mutex=<%p>\n",
                   mutex);
            return -1;
        }
        list_del(&waiter->ti_q);
        sched_enqueue(waiter);
    }
    CURRENT_THREAD_INFO(curr_thread);
    if (curr_thread->ti_state == THREAD_STATE_BLOCKED) {
        sched_elect(SCHED_OPT_NONE);
    } else if (waiter && (curr_thread->ti_priority <= waiter->ti_priority)) {
        sched_enqueue(curr_thread);
        sched_elect(SCHED_OPT_NONE);
    }

    return 0;
}
