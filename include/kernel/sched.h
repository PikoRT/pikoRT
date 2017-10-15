#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include_next <sched.h>

struct thread_info;

/* 0 <= PRI_MAX <= PRI_MIN */
#define PRI_MAX 0
#define PRI_MIN 31

#define SCHED_CLASS_RR 0
#define SCHED_CLASS_BITMAP 1
typedef int sched_class_t;

#define SCHED_OPT_NONE 0
#define SCHED_OPT_RESTORE_ONLY 1
#define SCHED_OPT_RESET 2
#define SCHED_OPT_TICK 3

/* scheduler implementation hooks */
#define HOOK_SCHED_CLASS(name, sched_struct)                            \
    static struct sched *sched_class_##name                             \
        __attribute__((section(".sched.class"), aligned(sizeof(long)),  \
                        used)) = sched_struct;

struct sched {
    sched_class_t class_type;
    int (*init)(void);
    int (*enqueue)(struct thread_info *thread);
    int (*dequeue)(struct thread_info *thread);
    int (*elect)(int switch_type);
};

int sched_init();
int sched_select(sched_class_t sched_type);
int sched_enqueue(struct thread_info *thread);
int sched_dequeue(struct thread_info *thread);
int sched_elect(int flags);

#endif /* !KERNEL_SCHEDULER_H */
