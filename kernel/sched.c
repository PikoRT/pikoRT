#include <kernel/sched.h>
#include <kernel/thread.h>

extern const struct sched sched_rr;
extern const struct sched sched_bitmap;

static const struct sched *sched;

int sched_select(int sched_type)
{
	switch (sched_type) {
		case SCHED_CLASS_RR:
			sched = &sched_rr;
			break;
		case SCHED_CLASS_BITMAP:
			sched = &sched_bitmap;
			break;
		default:
			return -1;
	}

	return sched->init();
}

int sched_enqueue(struct thread_info *thread)
{
	thread->ti_state = THREAD_STATE_ACTIVED;

	return sched->enqueue(thread);
}

int sched_dequeue(struct thread_info *thread)
{
	return sched->dequeue(thread);
}

int sched_elect(int flags)
{
	int r;
	CURRENT_THREAD_INFO(cur_thread);

	KERNEL_STACK_CHECKING;

	r = sched->elect(flags);
	cur_thread->ti_state = THREAD_STATE_RUNNING;

	return r;
}
