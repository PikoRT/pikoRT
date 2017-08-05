#include <kernel/linkage.h>

	.syntax unified
	.thumb

ENTRY(test_svcall0)
	ldr	r0, =41
	svc	#0
	bx	lr
ENDPROC(test_svcall0)

ENTRY(test_svcall1)
	ldr	r0, =0xbabe
	ldr	r1, =42
	svc	#1
	bx	lr
ENDPROC(test_svcall1)

ENTRY(test_svcall2)
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =43
	svc	#2
	bx	lr
ENDPROC(test_svcall2)

ENTRY(test_svcall3)
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =44
	svc	#3
	bx	lr
ENDPROC(test_svcall3)

ENTRY(test_svcall4)
	ldr	r3, =45
	push	{r3}
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =0x456
	svc	#4
	add	sp, #4
	bx	lr
ENDPROC(test_svcall4)

ENTRY(test_svcall5)
	ldr	r2, =0xcafe
	ldr	r3, =46
	push	{r2, r3}
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =0x456
	svc	#5
	add	sp, #8
	bx	lr
ENDPROC(test_svcall5)

ENTRY(test_svcall6)
	ldr	r1, =0xcafe
	ldr	r2, =0xfeed
	ldr	r3, =47
	push	{r1-r3}
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =0x456
	svc	#6
	add	sp, #12
	bx	lr
ENDPROC(test_svcall6)

ENTRY(get_sp)
	mov	r0, sp
	bx	lr
ENDPROC(get_sp)
