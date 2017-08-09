#include <sys/types.h>

#include <kernel/syscalls.h>

#include "syscalls.h"

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    return (void *) do_syscall6((void *) addr, (void *) length, (void *) prot,
                                (void *) flags, (void *) fd, (void *) offset,
                                SYS_MMAP);
}

int munmap(__unused void *addr, __unused size_t length)
{
    return do_syscall2((void *) addr, (void *) length, SYS_MUNMAP);
}
