/* http://www.ibm.com/support/knowledgecenter/ssw_i5_54/apis/users_76.htm */

#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "kernel.h"
#include "unit.h"

/* For safe condition variable usage, must use a boolean predicate and  */
/* a mutex with the condition.                                          */
int workToDo = 0;
pthread_cond_t cond;
pthread_mutex_t mutex;

#define NTHREADS 2

unsigned int msleep(unsigned int msecs);

int checkResults(const char *s, int rc)
{
	if (rc) {
		printk("error: %s\n", s);
		TEST_EXIT(1);
	}

	return 0;
}

void *threadfunc(void *parm)
{
	int rc;
	(void) parm;

	while (1) {
		/* Usually worker threads will loop on these operations */
		rc = pthread_mutex_lock(&mutex);
		checkResults("pthread_mutex_lock()\n", rc);

		while (!workToDo) {
			printk("Thread blocked\n");
			rc = pthread_cond_wait(&cond, &mutex);
			checkResults("pthread_cond_wait()\n", rc);
		}
		printk("Thread awake, finish work!\n");

		/* Under protection of the lock, complete or remove the work     */
		/* from whatever worker queue we have. Here it is simply a flag  */
		workToDo = 0;

		rc = pthread_mutex_unlock(&mutex);
		checkResults("pthread_mutex_lock()\n", rc);
	}
	return NULL;
}

int main()
{
	int rc = 0;
	int i;
	pthread_t threadid[NTHREADS];

	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);

	printk("Enter Testcase - %s\n", "IBM pthread_cond_*()");

	printk("Create %d threads\n", NTHREADS);
	for (i = 0; i < NTHREADS; ++i) {
		rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	msleep(40); /* Sleep is not a very robust way to serialize threads   */

	for (i = 0; i < 5; ++i) {
		printk("Wake up a worker, work to do...\n");

		rc = pthread_mutex_lock(&mutex);
		checkResults("pthread_mutex_lock()\n", rc);

		/* In the real world, all the threads might be busy, and        */
		/* we would add work to a queue instead of simply using a flag  */
		/* In that case the boolean predicate might be some boolean     */
		/* statement like: if (the-queue-contains-work)                 */
		if (workToDo) {
			printk("Work already present, likely threads are busy\n");
		}
		workToDo = 1;
		rc = pthread_cond_signal(&cond);
		checkResults("pthread_cond_broadcast()\n", rc);

		rc = pthread_mutex_unlock(&mutex);
		checkResults("pthread_mutex_unlock()\n", rc);
		msleep(40); /* Sleep is not a very robust way to serialize threads */
	}

	printk("Main completed\n");
	TEST_EXIT(0);
	return 0;
}
