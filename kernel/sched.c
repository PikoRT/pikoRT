#include <kernel/sched.h>
#include <kernel/thread.h>

extern unsigned long __sched_classes_start__;
extern unsigned long __sched_classes_end__;

/* static vars to repesent sched classes list */
static struct sched **sched_classes = 
    (struct sched **) &__sched_classes_start__;
static struct sched **sched_classes_end = 
    (struct sched **) &__sched_classes_end__;

static struct sched *sched;

int sched_init()
{
    int ret = 0;

    /* Initialize each scheduler class by traversing hooks */
    for (struct sched **c = sched_classes;
            c < sched_classes_end; c++) {
        struct sched *class = *c;
        ret |= class->init();
    }

    return ret;
}

int sched_select(sched_class_t sched_type)
{
    int ret = -1;
    struct sched *class = NULL;
    
    /* Examine specified sched class in hooks or not */
    for (struct sched **c = sched_classes;
            c < sched_classes_end; c++)
        if (sched_type == (*c)->class_type)
            class = *c;
    
    if (class) {
        sched = class;
        ret = 0;
    }

    return ret;
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
