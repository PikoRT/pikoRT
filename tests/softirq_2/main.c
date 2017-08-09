/* softirq task create/rasie 1000 times */

#include <kernel/kernel.h>

#include <kernel/softirq.h>

#include "unit.h"

static volatile int val = 1111;

static void fn(void *arg)
{
    val += (int) arg;
}

int main()
{
    int i;
    static struct tasklet_struct *tsk = NULL;

    /**
     *  FIXME: modifing to 1000 times would fail to create.
     *          Concludion with this problem is without freeing
     *          softirq task. That's make sense, so we could avoid
     *          it by testing tsk ptr. No more creation of same task.
     *  FIXME: After patch above, we would get another problem about
     *          same tsk with same addr space can concatenate two same tsk?
     *
     *          ANS: Probably NOT
     */
    for (i = 0; i < 100; i++) {
        tsk = tasklet_init(fn, (void *) 1, PRIO_TASKLET_MAXPRIO);

        if (!tsk) {
            printk("failed: can't create new softirq task.\n");
            TEST_EXIT(1);
        }

        if (tasklet_schedule(tsk) == -1) {
            printk("failed: can't rasie softirq task.\n");
            TEST_EXIT(1);
        }
    }

    while (val != 1211)
        ;

    TEST_EXIT(0);
}
