#include <kernel/linkage.h>
#include <kernel/syscalls.h>

	.syntax unified
	.thumb

	@ int do_syscall0(void, int no)
ENTRY(do_syscall0)
	svc	#0
	bx	lr
ENDPROC(do_syscall0)

	@ int do_syscall1(void *a0, int no)
ENTRY(do_syscall1)
	svc	#1
	bx	lr
ENDPROC(do_syscall1)

	@ int do_syscall2(void *a0, void *a1, int no)
ENTRY(do_syscall2)
	svc	#2
	bx	lr
ENDPROC(do_syscall2)

	@ int do_syscall3(void *a0, void *a1, void *a2, int no)
ENTRY(do_syscall3)
	svc	#3
	bx	lr
ENDPROC(do_syscall3)

	@ int do_syscall4(void *a0, void *a1, void *a2, void *a3,
	@                 int no)
ENTRY(do_syscall4)
	svc	#4
	bx	lr
ENDPROC(do_syscall4)

	@ int do_syscall5(void *a0, void *a1, void *a2, void *a3,
	@                 void *a4, int no)
ENTRY(do_syscall5)
	svc	#5
	bx	lr
ENDPROC(do_syscall5)

	@ int do_syscall6(void *a0, void *a1, void *a2, void *a3,
	@                 void *a4, void *a5, int no)
ENTRY(do_syscall6)
	svc	#6
	bx	lr
ENDPROC(do_syscall6)
