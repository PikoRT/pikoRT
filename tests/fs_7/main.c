/* test directory hierarchy in RomFS */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
    int fd;
    const char filename[] = "/dev/flash/.ssh/id_rsa.pub";
    char buffer[32];

    init_tmpfs_inode(&inode);
    flash_init();
    mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

    fd = open(filename, 0);
    if (fd < 0) {
        printk("error: failed to open %s\n", filename);
        TEST_EXIT(1);
    }

    memset(buffer, 0, 32);
    if (read(fd, buffer, 11) != 11)
        TEST_EXIT(1);
    if (strncmp(buffer, "-----BEGIN", 10))
        TEST_EXIT(1);

    memset(buffer, 0, 32);
    if (read(fd, buffer, 10) != 10)
        TEST_EXIT(1);
    if (strncmp(buffer, "PUBLIC KEY", 10))
        TEST_EXIT(1);

    TEST_EXIT(0);
}
