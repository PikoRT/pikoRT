#include "unit.h"
#include "kernel.h"

unsigned long get_sp(void);
int syscall_register(unsigned ix, void *(*fn)());

int sysarg_ack[7];

#define SYSARG_A0 0xbabe
#define SYSARG_A1 0xbeef
#define SYSARG_A2 0x123
#define SYSARG_A3 0x456
#define SYSARG_A4 0xcafe
#define SYSARG_A5 0xfeed

int sysarg0_handler(void)
{
	printk("    no arg\n");
	sysarg_ack[0] = 1;

	return 0;
}

int sysarg1_handler(int a0)
{
	printk("    arg0: %x\n", a0);
	if (a0 != SYSARG_A0)
		return -1;
	sysarg_ack[1] = 1;

	return 0;
}

int sysarg2_handler(int a0, int a1)
{
	printk("    arg0: %x\n", a0);
	printk("    arg1: %x\n", a1);
	if (a0 != SYSARG_A0)
		return -1;
	if (a1 != SYSARG_A1)
		return -1;
	sysarg_ack[2] = 1;

	return 0;
}

int sysarg3_handler(int a0, int a1, int a2)
{
	printk("    arg0: %x\n", a0);
	printk("    arg1: %x\n", a1);
	printk("    arg2: %x\n", a2);
	if (a0 != SYSARG_A0)
		return -1;
	if (a1 != SYSARG_A1)
		return -1;
	if (a2 != SYSARG_A2)
		return -1;
	sysarg_ack[3] = 1;

	return 0;
}

int sysarg4_handler(int a0, int a1, int a2, int a3)
{
	printk("    arg0: %x\n", a0);
	printk("    arg1: %x\n", a1);
	printk("    arg2: %x\n", a2);
	printk("    arg3: %x\n", a3);
	if (a0 != SYSARG_A0)
		return -1;
	if (a1 != SYSARG_A1)
		return -1;
	if (a2 != SYSARG_A2)
		return -1;
	if (a3 != SYSARG_A3)
		return -1;
	sysarg_ack[4] = 1;

	return 0;
}

int sysarg5_handler(int a0, int a1, int a2, int a3, int a4)
{
	printk("    arg0: %x\n", a0);
	printk("    arg1: %x\n", a1);
	printk("    arg2: %x\n", a2);
	printk("    arg3: %x\n", a3);
	printk("    arg4: %x\n", a4);
	if (a0 != SYSARG_A0)
		return -1;
	if (a1 != SYSARG_A1)
		return -1;
	if (a2 != SYSARG_A2)
		return -1;
	if (a3 != SYSARG_A3)
		return -1;
	if (a4 != SYSARG_A4)
		return -1;
	sysarg_ack[5] = 1;

	return 0;
}

int sysarg6_handler(int a0, int a1, int a2, int a3, int a4, int a5)
{
	printk("    arg0: %x\n", a0);
	printk("    arg1: %x\n", a1);
	printk("    arg2: %x\n", a2);
	printk("    arg3: %x\n", a3);
	printk("    arg4: %x\n", a4);
	printk("    arg5: %x\n", a5);
	if (a0 != SYSARG_A0)
		return -1;
	if (a1 != SYSARG_A1)
		return -1;
	if (a2 != SYSARG_A2)
		return -1;
	if (a3 != SYSARG_A3)
		return -1;
	if (a4 != SYSARG_A4)
		return -1;
	if (a5 != SYSARG_A5)
		return -1;
	sysarg_ack[6] = 1;

	return 0;
}

void test_svcall0();
void test_svcall1();
void test_svcall2();
void test_svcall3();
void test_svcall4();
void test_svcall5();
void test_svcall6();

int main(void *arg)
{
	(void) arg;

	void (*test_svcall[])() = {
		test_svcall0, test_svcall1, test_svcall2, test_svcall3,
		test_svcall4, test_svcall5, test_svcall6,
	};

	syscall_register(41, (void *(*) ()) sysarg0_handler);
	syscall_register(42, (void *(*) ()) sysarg1_handler);
	syscall_register(43, (void *(*) ()) sysarg2_handler);
	syscall_register(44, (void *(*) ()) sysarg3_handler);
	syscall_register(45, (void *(*) ()) sysarg4_handler);
	syscall_register(46, (void *(*) ()) sysarg5_handler);
	syscall_register(47, (void *(*) ()) sysarg6_handler);

	for (int i = 0; i <= 6; i++) {
		printk("Test syscall with %d arg(s).\n", i);
		// XXX: check SP before and after. also in syscall handler, in
		// trampoline.
		unsigned long sp = get_sp();
		test_svcall[i]();
		if (get_sp() != sp) {
			printk("error: Incorrect r13/sp\n");
			TEST_EXIT(1);
		}
		if (!sysarg_ack[i]) {
			printk("error: syscall was not acknowledged\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
