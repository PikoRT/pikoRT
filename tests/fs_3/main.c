/* test simple romFS */

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
    char buffer[128];

    init_tmpfs_inode(&inode);
    flash_init();
    mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

    fd = open("/dev/flash/id_rsa.pub", 0);
    if (fd < 0) {
        printk("error: failed to open /data/id_rsa.pub\n");
        TEST_EXIT(1);
    }

    memset(buffer, 0, 128);
    read(fd, buffer, 27);
    printk("read(): %s\n", buffer);
    if (strncmp(buffer, "-----BEGIN PUBLIC KEY-----", 26))
        TEST_EXIT(1);

    memset(buffer, 0, 128);
    read(fd, buffer, 65);
    printk("read(): %s\n", buffer);
    if (strncmp(
            buffer,
            "MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAI/rmTOBknHe2ro+8sUNl1MjNTRopU1/",
            64))
        TEST_EXIT(1);

    memset(buffer, 0, 128);
    read(fd, buffer, 65);
    printk("read(): %s\n", buffer);
    if (strncmp(
            buffer,
            "WJTcYvsrFML10Z2IBrFifc3Q1x92uTvyFU21cn+/ekU8L+8qK+V98fMCAwEAAQ==",
            64))
        TEST_EXIT(1);

    /* rewind the file */
    lseek(fd, 0, SEEK_SET);
    memset(buffer, 0, 128);
    read(fd, buffer, 27);
    printk("read(): %s\n", buffer);
    if (strncmp(buffer, "-----BEGIN PUBLIC KEY-----", 26))
        TEST_EXIT(1);

    TEST_EXIT(0);
}
