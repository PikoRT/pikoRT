/* test softirq task sequeniality */

#include <kernel/kernel.h>

#include <kernel/softirq.h>

#include "unit.h"

static volatile int bucket[6] = {0};

#define DEFINE_FUNC(_id_) \
    static void fn##_id_(void *arg) { bucket[_id_] = (int) arg; }
DEFINE_FUNC(0)
DEFINE_FUNC(1)
DEFINE_FUNC(2)
DEFINE_FUNC(3)
DEFINE_FUNC(4)
DEFINE_FUNC(5)

int main()
{
#define SOFTIRQ_TASK_IMPL(_id_)                                          \
    do {                                                                 \
        struct tasklet_struct *tsk##_id_ =                               \
            tasklet_init(fn##_id_, (void *) _id_, PRIO_TASKLET_MAXPRIO); \
        if (!tsk##_id_) {                                                \
            printk("failed: can't create new softirq task" #_id_ ".\n"); \
            TEST_EXIT(1);                                                \
        }                                                                \
                                                                         \
        if (tasklet_schedule(tsk##_id_) == -1) {                         \
            printk("failed: can't rasie softirq task" #_id_ ".\n");      \
            TEST_EXIT(1);                                                \
        }                                                                \
    } while (0)

    SOFTIRQ_TASK_IMPL(0);
    SOFTIRQ_TASK_IMPL(1);
    SOFTIRQ_TASK_IMPL(2);
    SOFTIRQ_TASK_IMPL(3);
    SOFTIRQ_TASK_IMPL(4);
    SOFTIRQ_TASK_IMPL(5);

    for (int i = 0; i < 6; i++)  // Test sequenciality
        while (!(bucket[i] == i))
            ;


    TEST_EXIT(0);
}
