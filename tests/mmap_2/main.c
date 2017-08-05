/* test mapping file to memory */

#include <string.h>
#include <fcntl.h>

#include <piko/sys/mman.h>
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
	init_tmpfs_inode(&inode);
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	int fd = open("/dev/flash/id_rsa.pub", 0);
	if (fd < 0) {
		printk("error: failed to open /data/id_rsa.pub\n");
		TEST_EXIT(1);
	}

	void *p = mmap(NULL, 256, 0, 0, fd, 0);
	if (p == MAP_FAILED)
		TEST_EXIT(1);
	if (strncmp((char *) p, "-----BEGIN PUBLIC KEY-----", 26))
		TEST_EXIT(1);

	p = mmap(NULL, 256, 0, 0, fd, 27);
	if (p == MAP_FAILED)
		TEST_EXIT(1);
	if (strncmp(
	        (char *) p,
	        "MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAI/rmTOBknHe2ro+8sUNl1MjNTRopU1/",
	        64))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
