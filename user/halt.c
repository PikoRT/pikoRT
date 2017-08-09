#include "sh.h"
#include "platform.h"

static int halt(__unused int argc, __unused char *argv[])
{
    __platform_halt();
    return 0;
}

HOOK_BUILTIN_CMD(halt, halt);
