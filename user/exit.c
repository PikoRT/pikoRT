#include "sh.h"
#include "platform.h"

static int _exit(__unused int argc, __unused char *argv[])
{
    __platform_halt();
    return 0;
}

SHELLCMD_REGIST(exit, _exit);
