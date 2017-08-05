/* test multiple opening/closing /dev/random */

#include <fcntl.h>
#include <unistd.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include "unit.h"

int main()
{
	int fd;

	for (int i = 0; i < 10000; i++) {
		/* printk("% 3d/9999\n", i); */
		fd = open("/dev/random", 0);
		if (fd < 0) {
			printk("error: failed to open /dev/random\n");
			TEST_EXIT(1);
		}
		if (close(fd)) {
			printk("error: failed to close /dev/random\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
