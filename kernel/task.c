#include <stdlib.h>

#include <kernel/task.h>
#include <kernel/thread.h>

#include "linux/list.h"

static LIST_HEAD(task_head);

static pid_t alloc_pid()
{
	static pid_t pid = 7000;
	pid_t retpid;

	retpid = pid;
	pid++;

	return retpid;
}

struct task_info *task_init(struct task_info *task)
{
	task->pid = alloc_pid();
	task->filemap = 0;
	INIT_LIST_HEAD(&task->thread_head);
	INIT_LIST_HEAD(&task->signal_head);
	list_add(&task->list, &task_head);

	return task;
}

void task_exit(struct task_info *task)
{
	// this is called after last thread has exited, or when the
	// task is killed
	list_del(&task->list);
	free(task);
}

struct task_info *current_task_info(void)
{
	CURRENT_THREAD_INFO(curr_thread);

	return curr_thread->ti_task;
}

int sys_getpid(void)
{
	CURRENT_TASK_INFO(curr_task);

	return curr_task->pid;
}
