/* simple test for interval timers */

#include <stddef.h>
#include <time.h>

#include <kernel/time.h>
#include <kernel/signal.h>

#include "unit.h"

static volatile int count;

static void event(union sigval sival)
{
    (void) sival;

    printk("Counter=%d\n", ++count);
}

int main()
{
    timer_t timerid;
    struct sigevent sevp = {
        .sigev_notify_function = event,
    };
    struct itimerspec val = {
        .it_value = {.tv_sec = 1, .tv_nsec = 0},
        .it_interval = {.tv_sec = 1, .tv_nsec = 0},
    };

    timer_create(0, &sevp, &timerid);
    timer_settime(timerid, 0, &val, NULL);
    while (count < 4)
        ;

    TEST_EXIT(0);
}
