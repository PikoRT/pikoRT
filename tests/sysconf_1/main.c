#include "unit.h"
#include "unistd.h"

int main()
{
	printk("system tick per seconds: %d\n", sysconf(_SC_CLK_TCK));

	TEST_EXIT(0);
}
