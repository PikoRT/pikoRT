/* strong mutex test, mixing lock/unlock/trylock */
#include <stddef.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "kernel.h"
#include "pthread.h"
#include "unit.h"

pthread_mutex_t lock;

void *fn(__unused void *arg)
{
	printk("thread 2: acquire mutex...\n");
	if (!pthread_mutex_trylock(&lock))
		return NULL;
	printk("thread 2: mutex was locked...\n");
	sched_yield();
	printk("thread 2: re-acquire mutex...\n");
	if (pthread_mutex_trylock(&lock))
		return NULL;
	sched_yield();
	if (pthread_mutex_unlock(&lock))
		return NULL;

	return NULL;
}

int main(void)
{
	pthread_t tid;

	pthread_mutex_init(&lock, NULL);

	if (pthread_create(&tid, NULL, fn, NULL))
		printk("error: Could not create new thread.\n");

	if (pthread_mutex_trylock(&lock))
		TEST_EXIT(1);
	printk("thread 1: mutex locked, yield now...\n");
	sched_yield();
	printk("thread 1: return from yield.\n");
	if (pthread_mutex_unlock(&lock))
		TEST_EXIT(1);
	printk("thread 1: mutex released...\n");
	sched_yield();

	/* re-acquire the mutex to check the thread released the mutex
	   correctly */
	if (!pthread_mutex_trylock(&lock))
		TEST_EXIT(1);
	if (pthread_mutex_lock(&lock))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
