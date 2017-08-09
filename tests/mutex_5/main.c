/* test mutexs' waitqueue */
#include <stddef.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "kernel.h"
#include "pthread.h"
#include "unit.h"

static pthread_mutex_t m;

void *fn(__unused void *arg)
{
    printk("thread %d locking...\n", (int) arg);
    pthread_mutex_lock(&m);
    printk("OK, thread %d got the mutex!\n", (int) arg);
    pthread_mutex_unlock(&m);

    return NULL;
}

int main()
{
    pthread_t tips[4];

    pthread_mutex_init(&m, NULL);
    pthread_mutex_lock(&m);

    for (int i = 0; i < 4; i++) {
        if (pthread_create(&tips[i], NULL, fn, (void *) i)) {
            printk("failed: can't create new posix thread.\n");
            TEST_EXIT(1);
        }
    }

    printk("locked the mutex, now yielding...\n");
    sched_yield();

    printk("unlocking the mutex...\n");
    pthread_mutex_unlock(&m);

    printk("relocking the mutex...\n");
    pthread_mutex_lock(&m);

    TEST_EXIT(0);
}
