#include <kernel/sched.h>
#include <kernel/thread.h>

#include "linux/list.h"
#include "kernel.h"

static LIST_HEAD(rr_runq);
extern struct thread_info *thread_idle;

int sched_rr_init(void)
{
    return 0;
}

static struct thread_info *find_next_thread(struct thread_info *thread)
{
    if (list_is_last(&thread->ti_q, &rr_runq))
        return list_first_entry(&rr_runq, struct thread_info, ti_q);

    return list_next_entry(thread, ti_q);
}

int sched_rr_enqueue(struct thread_info *thread)
{
    list_add(&thread->ti_q, &rr_runq);

    return 0;
}

int sched_rr_dequeue(struct thread_info *thread)
{
    CURRENT_THREAD_INFO(current);

    if (current == thread) {
        struct thread_info *next = thread_idle;
        if (!list_is_singular(&rr_runq)) {
            next = find_next_thread(current);
        }
        list_del(&thread->ti_q);
        thread_restore(next);  // FIXME: rename to switch_to_no_save
    } else {
        list_del(&thread->ti_q);
    }

    return 0;
}

/* This function is used when the runqueue has been modified externally, and it
   is not possible to fetch the next thread.    */
static int sched_rr_elect_reset(void)
{
    CURRENT_THREAD_INFO(current);
    struct thread_info *next = thread_idle;

    if (!list_empty(&rr_runq))
        next = list_first_entry(&rr_runq, struct thread_info, ti_q);
    switch_to(next, current);

    return 0;
}

int sched_rr_elect(int switch_type)
{
    CURRENT_THREAD_INFO(current);
    struct thread_info *next;

    if (switch_type & SCHED_OPT_RESET)
        return sched_rr_elect_reset();

    if (list_empty(&rr_runq)) {
        // go to thread idle.
        next = thread_idle;
    } else {
        next = find_next_thread(current);
    }

    /* keep running the previous thread */
    if (next == current)
        return -1;

    /* Leave _current_ thread for now. The _current_ thread will be elected
       again after _next_ thread has run. Inform the caller function (in
       _current_ context) that the thread gently gave way.    */
    switch_to(next, current);

    return 0;
}

// clang-format off
static struct sched sched_rr = {
    .class_type = SCHED_CLASS_RR,
    .init = sched_rr_init,
    .enqueue = sched_rr_enqueue,
    .dequeue = sched_rr_dequeue,
    .elect = sched_rr_elect,
};
// clang-format on

HOOK_SCHED_CLASS(RR, &sched_rr)
