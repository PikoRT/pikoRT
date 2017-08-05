/* open files in romFS multiple times */

#include <fcntl.h>

#include <piko/sys/mount.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include <drivers/mtd/mtd.h>

#include "unit.h"

extern char _binary_sda1_start;
struct mtd_info mtd1;

static struct inode inode = {
	.i_private = &mtd1,
};

void flash_init(void)
{
	struct dentry dentry = {.d_inode = &inode, .d_name = "mtd1"};

	printk("Creating MTD device %s\n", dentry.d_name);
	if (mtdram_init_device(&mtd1, &_binary_sda1_start, 1024, dentry.d_name))
		printk("error: mtdram init device failed\n");
	vfs_link(NULL, dev_inode(), &dentry);
}

int main()
{
	int fd1, fd2;

	init_tmpfs_inode(&inode);
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	for (int i = 0; i < 1000; i++) {
		fd1 = open("/dev/flash/id_rsa", 0);
		fd2 = open("/dev/flash/id_rsa.pub", 0);
		if (fd1 < 0 || fd2 < 0) {
			printk("error: failed to open file in flash\n");
			TEST_EXIT(1);
		}
		close(fd1);
		close(fd2);
	}

	TEST_EXIT(0);
}
