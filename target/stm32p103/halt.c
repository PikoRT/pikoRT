
#include <arch/semihosting.h>

void __platform_halt(void)
{
	v7m_semihost_exit(0);
}
