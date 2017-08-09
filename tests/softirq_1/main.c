/* simple softirq task create and rasie */

#include <kernel/kernel.h>

#include <kernel/softirq.h>

#include "unit.h"

static volatile int val = 111;

static void fn(void *arg)
{
    val += (int) arg;
}

int main()
{
    struct tasklet_struct *tsk = NULL;

    tsk = tasklet_init(fn, (void *) 666, PRIO_TASKLET_MAXPRIO);
    if (!tsk) {
        printk("failed: can't create new softirq task.\n");
        TEST_EXIT(1);
    }

    if (tasklet_schedule(tsk) == -1) {
        printk("failed: can't rasie softirq task.\n");
        TEST_EXIT(1);
    }

    while (val != 777)
        ;

    TEST_EXIT(0);
}
