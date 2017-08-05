#ifndef KERNEL_SOFTIRQ_H
#define KERNEL_SOFTIRQ_H

#include "linux/list.h"

#define PRIO_TASKLET_VEC     32
#define PRIO_TASKLET_MAXPRIO  0
#define PRIO_TASKLET_MINPRIO  31

/* softirq */
enum
{
	TIMER_SOFTIRQ = 0,
};

/* softirq default priority */
enum
{
	TIMER_SOFTIRQ_PRIO = 0,
};

/* softirq entry */
struct softirq_action
{
	int (*action)(struct softirq_action *);
};

/* FIXME: re-entrance */
/* dynamic task */
struct tasklet_struct
{
	struct list_head tsk_q;
    unsigned long prio;
    //unsigned long state;
	void (* func)(void *);
	void *data;
};

enum
{
	TASKLET_STATE_SCHED,	/* Tasklet is scheduled for execution */
	TASKLET_STATE_RUN,	/* Tasklet is running (SMP only) */
	TASKLET_STATE_PENDING	/* Tasklet is pending */
};

#define TASKLET_STATEF_SCHED	(1 << TASKLET_STATE_SCHED)
#define TASKLET_STATEF_RUN	(1 << TASKLET_STATE_RUN)
#define TASKLET_STATEF_PENDING	(1 << TASKLET_STATE_PENDING)

int open_softirq(unsigned int nr, int (*action)(struct softirq_action *));
int raise_softirq(unsigned int nr);

int tasklet_schedule(struct tasklet_struct *task);
struct tasklet_struct *tasklet_init(void (*func), void *data,
                        unsigned long prio);
int init_softirq(void);

#endif /* KERNEL_SOFTIRQ_H */
