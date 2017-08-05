#include "sh.h"
#include "platform.h"

static int reboot(__unused int argc, __unused char *argv[])
{
	//printk("Requesting system reboot\n");
    NVIC_SystemReset();

    return -1;
}

SHELLCMD_REGIST(reboot, reboot);
