/* test cancellation of timer */

#include <stddef.h>
#include <time.h>

#include <kernel/time.h>
#include <kernel/signal.h>

#include "unit.h"

static volatile int canary;
static volatile int count;

static void event_should_not_happen(union sigval sival)
{
    (void) sival;

    canary++;
}

static void event_check(union sigval sival)
{
    (void) sival;

    count++;
}

int main()
{
    timer_t timerid_a;
    struct sigevent sevp_a = {
        .sigev_notify_function = event_should_not_happen,
    };
    timer_create(0, &sevp_a, &timerid_a);

    timer_t timerid_b;
    struct sigevent sevp_b = {
        .sigev_notify_function = event_check,
    };
    timer_create(0, &sevp_b, &timerid_b);

    struct itimerspec val_a = {
        .it_value = {.tv_sec = 1, .tv_nsec = 0},
        .it_interval = {.tv_sec = 0, .tv_nsec = 0},
    };
    struct itimerspec val_b = {
        .it_value = {.tv_sec = 2, .tv_nsec = 0},
        .it_interval = {.tv_sec = 0, .tv_nsec = 0},
    };
    struct itimerspec val_zero = {
        .it_value = {.tv_sec = 0, .tv_nsec = 0},
        .it_interval = {.tv_sec = 0, .tv_nsec = 0},
    };
    timer_settime(timerid_a, 0, &val_a, NULL);
    timer_settime(timerid_b, 0, &val_b, NULL);
    timer_settime(timerid_a, 0, &val_zero, NULL);

    while (!count)
        ;
    if (canary)
        TEST_EXIT(1);

    TEST_EXIT(0);
}
