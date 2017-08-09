#include <stddef.h>
#include <time.h>

#include "unit.h"
#include <kernel/time.h>
#include <kernel/signal.h>

static void event(union sigval sival)
{
    (void) sival;
}

int main()
{
    struct sigevent sevp = {.sigev_notify_function = event};
    struct itimerspec val = {.it_value = {.tv_sec = 5, .tv_nsec = 0}};
    timer_t timerid;

    timer_create(1, &sevp, &timerid);
    timer_settime(timerid, 0, &val, NULL);
    do {
        timer_gettime(timerid, &val);
    } while (val.it_value.tv_sec != 4);

    TEST_EXIT(0);
}
