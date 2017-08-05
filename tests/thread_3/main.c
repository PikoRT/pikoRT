#include <pthread.h>

#include <kernel/kernel.h>

#include "unit.h"

static int count;

void *fn(void *arg)
{
	(void) arg;
	printk("counter=%d\n", count);
	count++;

	return 0;
}

int main()
{
	pthread_t tips[25];

	/* reduce the thread stack size to 256 bytes */
	pthread_attr_t attr;
	pthread_attr_setstacksize(&attr, 256);

	/* create 25 threads */
	for (int i = 0; i < 25; i++) {
		if (pthread_create(&tips[i], &attr, fn, NULL)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
	}

	while (count < 24)
		sched_yield();

	TEST_EXIT(0);
}
