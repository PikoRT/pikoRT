#include <kernel/bitops.h>
#include <kernel/sched.h>
#include <kernel/thread.h>

#include "linux/list.h"
#include "kernel.h"
#include "kernel/bitmap.h"

extern struct thread_info *thread_idle;

static struct bitmap_struct _active, _expire;

static struct {
    struct bitmap_struct *active;
    struct bitmap_struct *expire;
} sched_struct = {
    .active = &_active,
    .expire = &_expire,
};

static int sched_bitmap_init(void)
{
    INIT_BITMAP(sched_struct.active);
    INIT_BITMAP(sched_struct.expire);

    return 0;
}

static struct thread_info *find_next_thread(struct bitmap_struct *bm)
{
    int max_prio = find_first_bit(&bm->map, 32);

    /* all runqueues are empty, return the idle_thread */
    if (max_prio == 32)
        return thread_idle;  // idle_thread

    return bitmap_first_entry(bm, max_prio, struct thread_info, ti_q);
}

static int thread_enqueue(struct thread_info *thread, struct bitmap_struct *bm)
{
    bitmap_enqueue(&thread->ti_q, thread->ti_priority, bm);

    return 0;
}

static int sched_bitmap_enqueue(struct thread_info *thread)
{
    return thread_enqueue(thread, sched_struct.active);
}

static int thread_dequeue(struct thread_info *thread, struct bitmap_struct *bm)
{
    CURRENT_THREAD_INFO(current);

    /* active thread is not in the runqueue */
    if (thread == current)
        return -1;

    bitmap_queue_del(&thread->ti_q, thread->ti_priority, bm);

    return 0;
}

static int sched_bitmap_dequeue(struct thread_info *thread)
{
    int state = thread->ti_state;
    if (state == THREAD_STATE_ACTIVED)
        return thread_dequeue(thread, sched_struct.active);
    else if (state == THREAD_STATE_EXPIRED)
        return thread_dequeue(thread, sched_struct.expire);
    else
        return -1;
}

static int sched_bitmap_elect(int flags)
{
    CURRENT_THREAD_INFO(current);
    struct thread_info *next;

    next = find_next_thread(sched_struct.active);

    // check each thrd timeslice in active queue
    // if necessary swap active and expire queue
    if (next == thread_idle &&
        find_next_thread(sched_struct.expire) != thread_idle) {
        SWAP(sched_struct.active, sched_struct.expire);
        swap_sched_state_map();
        next = find_next_thread(sched_struct.active);
    }


    if (next != thread_idle) {  // idle_thread
        list_del(&next->ti_q);
        if (list_empty(&sched_struct.active->queue[next->ti_priority]))
            bitmap_clear_bit(&sched_struct.active->map, next->ti_priority);
    }

    if (flags == SCHED_OPT_RESTORE_ONLY)
        thread_restore(next);  // switch_to_restore_only
    else {
        if (next == thread_idle || next == current)
            return 0;
        if (flags == SCHED_OPT_TICK) {
            thread_enqueue(current, sched_struct.expire);
            current->ti_state = THREAD_STATE_EXPIRED;
        }
        switch_to(next, current);
    }

    return 0;
}

// clang-format off
static struct sched sched_bitmap = {
    .class_type = SCHED_CLASS_BITMAP,
    .init = sched_bitmap_init,
    .enqueue = sched_bitmap_enqueue,
    .dequeue = sched_bitmap_dequeue,
    .elect = sched_bitmap_elect,
};
// clang-format on

HOOK_SCHED_CLASS(bitmap, &sched_bitmap)
