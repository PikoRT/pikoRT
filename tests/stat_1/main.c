/* simple test for stat() */

#include <sys/stat.h>

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

    struct stat st;
    if (stat("/dev/flash/id_rsa.pub", &st)) {
        printk("error: Wrong pathname\n");
        TEST_EXIT(1);
    }
    printk("Opened file with inode number %d\n", st.st_ino);
    if (!S_ISREG(st.st_mode)) {
        printk("error: File is not a regular file\n");
        TEST_EXIT(1);
    }
    if (st.st_size != 182) {
        printk("error: Wrong file size (%d != 182)\n", st.st_size);
        TEST_EXIT(1);
    }

    TEST_EXIT(0);
}
