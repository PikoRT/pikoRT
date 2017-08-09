/* Newlib stubs */

#include <stdint.h>
#include <sys/cdefs.h>
#include <unistd.h>

#include <kernel/kernel.h>
#include <kernel/syscalls.h>

#include "syscalls.h"

#define HANGS_ON()                                  \
    ({                                              \
        printk("error: Newlib needs %s", __func__); \
        for (;;)                                    \
            ;                                       \
    })

int _isatty(__unused int file)
{
    HANGS_ON();

    return 1;
}

int _fstat()
{
    HANGS_ON();

    return -1;
}

void *_sbrk(__unused int incr)
{
    HANGS_ON();

    return NULL;
}

void _exit(__unused int status)
{
    HANGS_ON();
}

clock_t _times()
{
    HANGS_ON();

    return -1;
}

void _fini(void)
{
    HANGS_ON();
}

int _getpid(void)
{
    return do_syscall0(SYS_GETPID);
}
