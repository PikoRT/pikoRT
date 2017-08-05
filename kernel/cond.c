#include <sys/types.h>

#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/thread.h>

#include "linux/list.h"

static LIST_HEAD(cond_head);

static struct thread_info *find_other_thread(pthread_cond_t *cond)
{
	struct thread_info *other;

	list_for_each_entry(other, &cond_head, ti_q) {
		if (other->ti_private == cond)
			return other;
	}

	return NULL;
}

int sys_pthread_cond_wait(pthread_cond_t *cond, kernel_mutex_t *mutex)
{
	CURRENT_THREAD_INFO(curr_thread);
	curr_thread->ti_private = cond;
	curr_thread->ti_state = THREAD_STATE_BLOCKED;
	list_add_tail(&curr_thread->ti_q, &cond_head);
	sys_pthread_mutex_unlock(mutex);

	/* contend for the lock */
	sys_pthread_mutex_lock(mutex);

	return 0;
}

int sys_pthread_cond_signal(pthread_cond_t *cond)
{
	struct thread_info *other;

	other = find_other_thread(cond);
	if (!other)
		return 0;
	list_del(&other->ti_q);
	sched_enqueue(other);
	CURRENT_THREAD_INFO(curr_thread);
	if (other->ti_priority >= curr_thread->ti_priority) {
		sched_enqueue(curr_thread);
		sched_elect(SCHED_OPT_NONE);
	}

	return 0;
}
