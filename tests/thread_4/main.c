#include <pthread.h>

#include <kernel/kernel.h>
#include <kernel/sched.h>
#include <kernel/task.h>
#include <kernel/thread.h>

#include "unit.h"

static int count;

static void *fn(void *arg)
{
    int i = (int) arg;

    if (i != count)
        TEST_EXIT(1);
    count++;

    return 0;
}

int main()
{
    struct thread_info *t;
    CURRENT_TASK_INFO(curr_task);

    for (int i = 0; i < 15; i++) {
        t = thread_create(fn, (void *) i, THREAD_PRIV_USER, 256, curr_task);
        if (!t) {
            printk("failed: can't create new posix thread.\n");
            TEST_EXIT(1);
        }
        thread_set_priority(t, i);
        sched_enqueue(t);
    }

    sched_yield();

    if (count != 15) {
        printk("count != 15 (%d)\n", count);
        TEST_EXIT(1);
    }

    TEST_EXIT(0);
}
