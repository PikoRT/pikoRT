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

static struct thread_info *find_next_thread(void)
{
    return list_first_entry(&rr_runq, struct thread_info, ti_q);
}

int sched_rr_enqueue(struct thread_info *thread)
{
    if (thread->ti_queued)
        return -1;

    list_add_tail(&thread->ti_q, &rr_runq);
    thread->ti_queued = 1;

    return 0;
}

int sched_rr_dequeue(struct thread_info *thread)
{
    if (!thread->ti_queued)
        return -1;

    list_del(&thread->ti_q);
    thread->ti_queued = 0;

    return 0;
}

void sched_rr_requeue(struct thread_info *thread)
{
    sched_rr_dequeue(thread);
    sched_rr_enqueue(thread);
}

int sched_rr_elect(__unused int switch_type)
{
    CURRENT_THREAD_INFO(current);
    struct thread_info *next;

    if (list_empty(&rr_runq)) {
        switch_to(thread_idle, current);
        return -1;
    }

    /* XXX: Will RR stuck inside the loop? */
    while ((next = find_next_thread()) == current && !list_is_singular(&rr_runq))
        sched_rr_requeue(next);

    /* Shortcut if next is current */
    if (next == current)
        return -1;

    /* Leave _current_ thread for now. The _current_ thread will be elected
       again after _next_ thread has run. Inform the caller function (in
       _current_ context) that the thread gently gave way.    */
    switch_to(next, current);

    return 0;
}

// clang-format off
const struct sched sched_rr = {
    .init = sched_rr_init,
    .enqueue = sched_rr_enqueue,
    .dequeue = sched_rr_dequeue,
    .elect = sched_rr_elect,
};
// clang-format on
