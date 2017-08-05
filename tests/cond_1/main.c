/* simple test for condition variable */

#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "kernel.h"
#include "unit.h"

pthread_cond_t cond;
pthread_mutex_t mutex;

void *threadfunc(void *parm)
{
	(void) parm;

	pthread_mutex_lock(&mutex);

	printk("Wait for signal\n");
	pthread_cond_wait(&cond, &mutex);
	printk("Thread awake, finish work!\n");

	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main()
{
	int rc = 0;
	pthread_t threadid;

	printk("Enter Testcase\n");

	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);

	rc = pthread_create(&threadid, NULL, threadfunc, NULL);
	if (rc) {
		printk("failed: can't create new posix thread.\n");
		TEST_EXIT(1);
	}

	pthread_yield();

	printk("Wake up a worker, work to do...\n");

	rc = pthread_mutex_lock(&mutex);

	rc = pthread_cond_signal(&cond);
	// checkResults("pthread_cond_broadcast()\n", rc);

	rc = pthread_mutex_unlock(&mutex);
	// checkResults("pthread_mutex_unlock()\n", rc);
	// sleep(5);  /* Sleep is not a very robust way to serialize threads */

	printk("Main completed\n");

	TEST_EXIT(0);
}
