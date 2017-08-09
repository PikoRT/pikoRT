#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H

#include <sys/types.h>

#include <kernel/fs.h>

#include "linux/list.h"

#define PID_BASE 7000
#define PID_MAX 32768

struct task_info {
    pid_t pid;
    unsigned long filemap;
    struct file filetable[FILE_MAX];
    struct list_head list;
    struct list_head thread_head;
    struct list_head signal_head; /* list of installed handlers */
};

struct task_info *task_init(struct task_info *task);
struct task_info *task_create(void *(*start_routine)(void *), void *arg);
void task_exit(struct task_info *task);
struct task_info *current_task_info(void);

#define CURRENT_TASK_INFO(var) struct task_info *var = current_task_info();

#endif /* !_KERNEL_TASK_H */
