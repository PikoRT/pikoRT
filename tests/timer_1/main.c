#include <stddef.h>
#include <time.h>

#include "unit.h"
#include <kernel/time.h>
#include <kernel/signal.h>

#define EXPECTED_VALUE 0xabadcafeul

static volatile int received_signal;

static void event(union sigval sival)
{
	if (sival.sival_int != (int) EXPECTED_VALUE) {
		printk("error: Did not received expected value (%x != %x)\n",
		       sival.sival_int, EXPECTED_VALUE);
		TEST_EXIT(1);
	}
	received_signal = 1;
}

int main()
{
	struct sigevent sevp = {.sigev_notify_function = event,
		       .sigev_value.sival_int = EXPECTED_VALUE
	};
	struct itimerspec val = {.it_value = {.tv_sec = 1, .tv_nsec = 0}};
	timer_t timerid;

	timer_create(1, &sevp, &timerid);
	timer_settime(timerid, 0, &val, NULL);

	while (!received_signal)
		;

	TEST_EXIT(0);
}
