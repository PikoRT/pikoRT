#include <string.h>
#include <sys/param.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

// https://linux.die.net/lkmpg/x861.html
// https://lwn.net/Articles/22355/

extern const char _version_ptr;
extern const int _version_len;

static int open_version(__unused struct inode *inode,
                        __unused struct file *file)
{
    return 0;
}

static ssize_t read_version(__unused struct file *file,
                            char *buf,
                            size_t count,
                            off_t offset)
{
    size_t n = MIN(count, (int) &_version_len - offset);

    strncpy(buf, &_version_ptr + offset, n);

    return n;
}

static int open_meminfo(__unused struct inode *inode,
                        __unused struct file *file)
{
    return 0;
}

static ssize_t read_meminfo(__unused struct file *file,
                            char *buf,
                            size_t count,
                            off_t offset)
{
    memcpy(buf, (void *) offset, count);

    return count;
}

static const struct file_operations version_fops = {
    .open = open_version,
    .read = read_version,
};

static const struct file_operations meminfo_fops = {
    .open = open_meminfo,
    .read = read_meminfo,
};

extern const struct inode_operations tmpfs_iops;

static struct inode proc_inodes[] = {
    {
        /* /proc/version */
        .i_ino = 1001,
        .i_op = &tmpfs_iops,
        .i_fop = &version_fops,
    },
    {
        /* /proc/meminfo */
        .i_ino = 1002,
        .i_op = &tmpfs_iops,
        .i_fop = &meminfo_fops,
    },
};

void proc_init(void)
{
    struct dentry dentry;
    const char *names[] = {
        "version", "meminfo",
    };

    for (int i = 0; i < 2; i++) {
        printk("Creating /proc/%s\n", names[i]);
        dentry.d_inode = &proc_inodes[i], strcpy(dentry.d_name, names[i]);
        vfs_link(0, proc_inode(), &dentry);
    }
}
