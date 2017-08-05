/* test raise() return code */

#include <stddef.h>
#include <signal.h>

#include <kernel/errno-base.h>
#include "kernel.h"
#include "unit.h"

int main(void *arg)
{
	(void) arg;

	sigaction(SIGUSR1, NULL, NULL); /* shall return -EINVAL */
	int retval = raise(0);
	printk("Got return value %d (negative)\n", -retval);
	if (retval != -EINVAL)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
