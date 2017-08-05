#include <kernel/kernel.h>

#include "sh.h"

static int echo(int argc, char *argv[])
{
	if (argc == 1)
		return 0;
	for (int i = 1; i < argc; i++) {
		if (i == 1)
			printk("%s", argv[i]);
		else
			printk(" %s", argv[i]);
	}
	printk("\n");

	return 0;
}

SHELLCMD_REGIST(echo, echo);
