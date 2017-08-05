/* test thread_join() */

#include "kernel.h"
#include "pthread.h"
#include "unit.h"
#include "linux/stddef.h"

void *fn(void *arg)
{
	/* printk("Running thread with arg=%d\n", (int) arg); */
	pthread_exit(arg);

	return 0;
}

int main()
{
	void *retval;
	pthread_t tips[7];

	printk("Creating a bunch of threads...  ");
	for (int i = 0; i < 7; i++) {
		if (pthread_create(&tips[i], NULL, fn, (void *) i)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
	}
	printk("OK\n");

	for (int i = 0; i < 7; i++) {
		printk("Joining thread with arg=%d...  ", i);
		pthread_join(tips[i], &retval);
		if ((int) retval != i) {
			printk("pthread_join: wrong return value: got %d, expected %d\n",
			       (int) retval, i);
			TEST_EXIT(1);
		}
		printk("OK\n");
	}

	TEST_EXIT(0);
}
