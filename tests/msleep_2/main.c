// simple thread create and thread yield

#include <pthread.h>
#include <stddef.h>
#include "kernel.h"
#include "unit.h"

extern void msleep(unsigned int);

static int val;

static void *fn(void *arg)
{
    // msleep(30 * (1 + (int) arg));
    msleep(30);
    val++;

    return 0;
}

int main()
{
    pthread_t tip;

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&tip, NULL, fn, (void *) i)) {
            printk("failed: can't create new posix thread.\n");
            TEST_EXIT(1);
        }
    }
    while (val != 3)
        pthread_yield();

    TEST_EXIT(0);
}
