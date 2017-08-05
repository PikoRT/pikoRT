#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <kernel/bitops.h>
#include <kernel/errno-base.h>
#include <kernel/sched.h>
#include <kernel/signal.h>
#include <kernel/thread.h>
#include <kernel/time.h>

/* sleep functions */

// XXX: sleep() is part of <unistd.h>, but nanosleep() is part of <time.h>

extern struct thread_info *thread_idle;

static void msleep_callback(struct timer_info *timer)
{
	sched_enqueue(timer->owner);
	timer->disarmed = 1;
}

// FIXME: POSIX standard is sys_nanosleep()
int sys_msleep(unsigned int msec)
{
	struct timer_info *timer = timer_alloc();

	if (!timer)
		return -1;
	CURRENT_THREAD_INFO(curr_thread);
	timer->owner = curr_thread;
	timer->disarmed = 0;
	timer->type = ONESHOT_TIMER;
	timer->callback = msleep_callback;
	struct timespec value = {.tv_sec = msec / 1000,
		       .tv_nsec = (msec % 1000) * 1000000
	};
	timer_set(timer, &value);
	sched_dequeue(curr_thread);
	sched_elect(SCHED_OPT_NONE);
	timer_free(timer);

	return 0;
}

/* POSIX timers */

static LIST_HEAD(timer_head);

static struct timer_info *find_timer_by_id(timer_t timerid,
        struct list_head *timer_list)
{
	struct timer_info *pos;
	list_for_each_entry(pos, timer_list, list) {
		if (pos->id == timerid)
			return pos;
	}

	return NULL;
}

static int reserve_timer_id(timer_t *timerid)
{
	static unsigned long bitmap = 0;

	unsigned long bit = find_first_zero_bit(&bitmap, BITS_PER_LONG);
	if (bit == BITS_PER_LONG)
		return -1;
	bitmap_set_bit(&bitmap, bit);
	*timerid = bit;

	return 0;
}

static void timer_callback(struct timer_info *timer)
{
	if (timer->type == ONESHOT_TIMER)
		timer->disarmed = 1;
	do_sigevent(&timer->sigev, timer->owner);
}

static void timer_callback_and_link(struct timer_info *timer)
{
	timer->type = INTERVAL_TIMER;
	timer->callback = timer_callback;
	timer_set(timer, &timer->value.it_interval);
	do_sigevent(&timer->sigev, timer->owner);
}

int sys_timer_create(__unused clockid_t clockid,
                     struct sigevent *sevp,
                     timer_t *timerid)
{
	struct timer_info *timer = timer_alloc();
	if (!timer)
		return -1;

	if (reserve_timer_id(&timer->id)) {
		timer_free(timer);
		return EAGAIN;
	}

	*timerid = timer->id;
	timer->disarmed = 1;
	memcpy(&timer->sigev, sevp, sizeof(struct sigevent));
	list_add(&timer->list, &timer_head);

	return 0;
}

int sys_timer_settime(timer_t timerid,
                      __unused int flags,
                      const struct itimerspec *new_value,
                      struct itimerspec *old_value)
{
	struct timer_info *timer = find_timer_by_id(timerid, &timer_head);

	if (!timer)
		return EINVAL;
	if (old_value != NULL)
		memcpy(old_value, &timer->value, sizeof(struct itimerspec));
	memcpy(&timer->value, new_value, sizeof(struct itimerspec));

	/* disarm timer */
	if (!new_value->it_value.tv_sec && !new_value->it_value.tv_nsec) {
		if (!timer->disarmed) {
			timer_set(timer, &new_value->it_value);
			timer->disarmed = 1;
		}
		return 0;
	}

	CURRENT_THREAD_INFO(curr_thread);
	timer->owner = curr_thread;
	timer->disarmed = 0;
	if (new_value->it_interval.tv_sec || new_value->it_interval.tv_nsec) {
		if ((new_value->it_value.tv_sec == new_value->it_interval.tv_sec) &&
		    (new_value->it_value.tv_nsec == new_value->it_interval.tv_nsec)) {
			timer->type = INTERVAL_TIMER;
			timer->callback = timer_callback;
			timer_set(timer, &new_value->it_interval);
		} else {
			timer->type = ONESHOT_TIMER;
			timer->callback = timer_callback_and_link;
			timer_set(timer, &new_value->it_value);
		}
	} else {
		timer->type = ONESHOT_TIMER;
		timer->callback = timer_callback;
		timer_set(timer, &new_value->it_value);
	}

	return 0;
}

int sys_timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
	struct timer_info *timer = find_timer_by_id(timerid, &timer_head);

	if (!timer)
		return EINVAL;
	timer_get(timer, curr_value);

	return 0;
}
