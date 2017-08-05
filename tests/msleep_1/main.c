#include "unit.h"
#include "kernel.h"

extern void msleep(unsigned int);

int main()
{
	for (int i = 1; i < 6; i++) {
		printk("%d...", i);
		msleep(1000);
	}
	printk("\n");

	TEST_EXIT(0);
}
