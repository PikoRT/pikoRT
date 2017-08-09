#include "sh.h"
#include "platform.h"

static int reboot(__unused int argc, __unused char *argv[])
{
    NVIC_SystemReset();

    return -1;
}

HOOK_BUILTIN_CMD(reboot, reboot);
