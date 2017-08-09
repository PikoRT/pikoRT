#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include <kernel/errno-base.h>
#include <kernel/time.h>

static struct timer_operations *timer_operations;

void config_timer_operations(struct timer_operations *tops)
{
    timer_operations = tops;
}

struct timer_info *timer_alloc(void)
{
    struct timer_info *timer;

    timer = malloc(sizeof(struct timer_info));
    if (!timer) {
        errno = ENOMEM;
        return NULL;
    }
    timer->tops = timer_operations;  // FIXME: get info from vnode or device
    if (timer->tops->timer_alloc(timer)) {
        free(timer);
        return NULL;
    }

    return timer;
}

int timer_set(struct timer_info *timer, const struct timespec *value)
{
    return timer->tops->timer_set(timer, value);
}

int timer_get(struct timer_info *timer, struct itimerspec *value)
{
    return timer->tops->timer_get(timer, value);
}

int timer_free(struct timer_info *timer)
{
    if (!timer->disarmed) {
        const struct timespec zero_val = {0, 0};
        timer_set(timer, &zero_val);
    }
    timer->tops->timer_free(timer);
    free(timer);

    return 0;
}

void timer_expire_callback(struct timer_info *timer)
{
    if (timer->callback)
        timer->callback(timer);
}
