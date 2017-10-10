#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include_next <sched.h>

struct thread_info;

/* 0 <= PRI_MAX <= PRI_MIN */
#define PRI_MAX 0
#define PRI_MIN 31

#define SCHED_CLASS_RR 0
#define SCHED_CLASS_BITMAP 1

#define SCHED_OPT_NONE 0
#define SCHED_OPT_RESTORE_ONLY 1
#define SCHED_OPT_RESET 2
#define SCHED_OPT_TICK 3

struct sched {
    int (*init)(struct thread_info *thread);
    int (*enqueue)(struct thread_info *thread);
    int (*dequeue)(struct thread_info *thread);
    int (*elect)(int switch_type);
};

int sched_select(int sched_type, struct thread_info *thread);
int sched_enqueue(struct thread_info *thread);
int sched_dequeue(struct thread_info *thread);
int sched_elect(int flags);

#endif /* !KERNEL_SCHEDULER_H */
