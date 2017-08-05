/* test the trylock interface */

#include <stddef.h>

#include "kernel.h"
#include "pthread.h"
#include "unit.h"

int main(void)
{
	pthread_mutex_t lock;

	pthread_mutex_init(&lock, NULL);

	if (pthread_mutex_lock(&lock))
		TEST_EXIT(1);
	printk("mutex is now locked...\n");
	if (!pthread_mutex_trylock(&lock))
		TEST_EXIT(1);
	printk("tried to lock a locked mutex...\n");
	if (pthread_mutex_unlock(&lock))
		TEST_EXIT(1);
	printk("mutex unlocked, trylock should succeed...\n");
	if (pthread_mutex_trylock(&lock))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
