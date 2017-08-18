#include <kernel/bitops.h>
#include <kernel/softirq.h>
#include <kernel/task.h>
#include <kernel/thread.h>
#include <kernel/sched.h>

#include <stdlib.h>

#include "kernel/bitmap.h"

enum {
    PRIO_TASKLET = 0,
    NR_SOFTIRQS,
};

const char *const softirq_to_name[NR_SOFTIRQS] = {"PRIO_TASKLET"};

static struct softirq_action softirq_vec[NR_SOFTIRQS];

/* Priority-based tasklet */
static struct bitmap_struct prio_tasklet;

/* softirq_daemon */
static struct task_info softirq_daemon;
static struct thread_info *thread_softirqd;

int open_softirq(unsigned int nr, int (*action)(struct softirq_action *))
{
    if (nr >= NR_SOFTIRQS)
        return -1;

    softirq_vec[nr].action = action;

    return 0;
}

/* XXX: represent softirqd whether in runq or not */
static volatile int softirq_run = 0;
int raise_softirq(unsigned int nr)
{
    if (nr >= NR_SOFTIRQS)
        return -1;

    /* XXX: we ignore do_softirq routine as Linux PREEMPT_RT */
    if (!softirq_run) {
        sched_enqueue(thread_softirqd);
        softirq_run = 1;
    }

    return 0;
}

struct tasklet_struct *tasklet_init(void(*func), void *data, unsigned long prio)
{
    struct tasklet_struct *tsk =
        (struct tasklet_struct *) malloc(sizeof(struct tasklet_struct));
    if (!tsk)
        return NULL;

    tsk->prio = prio;
    tsk->func = func;
    tsk->data = data;
    INIT_LIST_HEAD(&tsk->tsk_q);

    return tsk;
}

int tasklet_schedule(struct tasklet_struct *task)
{
    CURRENT_THREAD_INFO(cur);
    if (!task || task->prio > PRIO_TASKLET_MINPRIO || cur != thread_softirqd)
        return -1;

    //	list_add_tail(&task->tsk_q, &prio_tasklet.runq[task->prio]);
    //  bitmap_set_bit(&prio_tasklet.bitmap, task->prio);
    bitmap_enqueue(&task->tsk_q, task->prio, &prio_tasklet);

    return raise_softirq(PRIO_TASKLET);
}

static int tasklet_action(struct softirq_action __unused *a)
{
    struct tasklet_struct *tsk = NULL;

    while (1) {
        if (prio_tasklet.map) {
            int max_prio = find_first_bit(&prio_tasklet.map, 32);

            // tsk = list_first_entry(&prio_tasklet.runq[max_prio], struct
            // tasklet_struct, tsk_q);
            tsk = bitmap_first_entry((&prio_tasklet), max_prio,
                                     struct tasklet_struct, tsk_q);

            // list_del(&tsk->tsk_q);
            // if (list_empty(&prio_tasklet.runq[prio]))
            //    bitmap_clear_bit(&prio_tasklet.bitmap, prio);
            bitmap_queue_del(&tsk->tsk_q, max_prio, &prio_tasklet);

            if (!tsk->func) {
                printk(
                    "[!] prio_taskletd: prio_tasklet function is NULL ptr.\n");
                break;
            }

            tsk->func(tsk->data);
        } else
            return 0;
    }

    return -1;
}

static void init_softirq_entry()
{
    return;
}

extern void sched_yield();

static void *softirqd(__unused void *arg)
{
    int ret = -1;

    while (1) {
        ret = tasklet_action(NULL);

        if (ret == 0)
            sched_yield();
        else
            break;
    }
    printk("[!] softirqd thread should not return.\n");

    softirq_run = 0;
    return NULL;
}

int init_softirq(void)
{
    /* initialize softirq vector */
    open_softirq(PRIO_TASKLET, tasklet_action);

    /* initialize priority tasklet obj */
    INIT_BITMAP(&prio_tasklet);

    // FIXME: no arg for priority in thread_create()
    /* initialize softirq daemon thread */
    task_init(&softirq_daemon);
    thread_softirqd =
        thread_create(softirqd, NULL, THREAD_PRIV_USER, 1024, &softirq_daemon);
    if (!thread_softirqd) {
        printk("[!] Could not create softirqd thread.\n");
        return -1;
    }
    thread_set_priority(thread_softirqd, PRI_MAX);

    /* initialize softirq entries */
    init_softirq_entry();

    return 0;
}
