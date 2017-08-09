/* simple create  multiple concurrent timers */

#include <signal.h>
#include <stddef.h>
#include <time.h>

#include "kernel.h"
#include "unit.h"

static volatile int vals[] = {0, 0, 0, 0};

static void event(union sigval sival)
{
    printk("In event %d.\n", sival.sival_int);
    vals[sival.sival_int] = 1;
}

int main()
{
    struct sigevent sevp = {.sigev_notify_function = event};
    timer_t timerid[4];

    for (int i = 0; i < 4; i++) {
        printk("Creating timer %d...\n", i);
        sevp.sigev_value.sival_int = i;
        timer_create(0, &sevp, &timerid[i]);

        unsigned long val_in_nsecs = 750 * (i + 1) * 1000000;
        struct itimerspec val = {
            .it_value = {.tv_sec = val_in_nsecs / 1000000000,
                         .tv_nsec = val_in_nsecs % 1000000000}};
        timer_settime(timerid[i], 0, &val, NULL);
    }
    printk("All timers armed.\n");
    for (int i = 0; i < 4; i++) {
        while (vals[i] == 0)
            ;
    }

    TEST_EXIT(0);
}
