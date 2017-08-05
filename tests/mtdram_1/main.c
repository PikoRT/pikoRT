/* test simple romFS */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include "unit.h"

int main()
{
	char str[] = "Hello World!";
	char buf[32];
	ssize_t len = strlen(str);
	int fd;

	printk("Opening /dev/mtd0...\n");
	fd = open("/dev/mtd0", 0);
	if (write(fd, str, len) != len)
		TEST_EXIT(1);
	lseek(fd, 0, SEEK_SET);
	if (read(fd, buf, len) != len)
		TEST_EXIT(1);
	printk("Read /dev/mtd0: %s\n", buf);
	if (strcmp(str, buf))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
