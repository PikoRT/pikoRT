#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <piko/sys/mman.h>

#include <kernel/fs.h>
#include <kernel/mm/page.h>

#define M_ISANON(f) (((f) &MAP_ANONYMOUS) == MAP_ANONYMOUS)
#define M_ISUNINIT(f) (((f) &MAP_UNINITIALIZED) == MAP_UNINITIALIZED)

static void *map_anon(void *addr, size_t length, __unused int prot, int flags)
{
    int order;

    order = size_to_page_order(length);
    addr = alloc_pages(order);
    if (!addr) {
        printk("mmap: ENOMEM\n");
        errno = ENOMEM;
        return MAP_FAILED;
    }
    if (!M_ISUNINIT(flags))
        memset(addr, 0, length);

    return addr;
}

static void *map_file(__unused size_t length,
                      __unused int prot,
                      __unused int flags,
                      int fd,
                      off_t offset)
{
    void *addr;
    struct file *file;

    file = fd_to_file(fd);
    if (!file) {
        printk("mmap: BADF\n");
        errno = EBADF;
        return MAP_FAILED;
    }
    if (!S_ISREG(file->f_dentry->d_inode->i_mode)) {
        printk("mmap: ACCESS\n");
        errno = EACCES;
        return MAP_FAILED;
    }
    if (vfs_mmap(file, offset, &addr)) {
        printk("mmap: failed in romfs_map\n");
        for (;;)
            ;
        return MAP_FAILED;
    }

    return addr;
}

void *sys_mmap(void *addr,
               size_t length,
               int prot,
               int flags,
               int fd,
               off_t offset)
{
    if (!length) {
        errno = EINVAL;
        printk("mmap: length is 0\n");
        return MAP_FAILED;
    }
    if (prot & PROT_NONE) {
        errno = EACCES;
        printk("mmap: PROT_NONE\n");
        return MAP_FAILED;
    }
    if (M_ISANON(flags))
        addr = map_anon(addr, length, prot, flags);
    else
        addr = map_file(length, prot, flags, fd, offset);

    return addr;
}

int sys_munmap(__unused void *addr, __unused size_t length)
{
    /* Closing the file descriptor does not unmap the region. */
    return 0;
}
