#include <kernel/linkage.h>

	.syntax unified
	.thumb

	@ offsets in the frame to the registers saved on interrupt-entry
	.set	R0, 0
	.set    RET_ADDRESS, 24
	.set	xPSR, 0x1c

ENTRY(svcall)
	push	{lr}
	mrs	lr, psp
	ldr	r12, [lr, #RET_ADDRESS]
	ldrb	r12, [r12, #-2]		@ address of the SVC call site
	tbb	[pc, r12]

0:	.irpc	argc, 0123456
	.byte	($\argc - 0b) / 2
	.endr
	.balign 2

	.balign	2
$0:	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, r0, lsl #2]
	blx	r12
	b	0f

	.balign	2
$1:	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, r1, lsl #2]
	blx	r12
	b	0f

	.balign	2
$2:	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, r2, lsl #2]
	blx	r12
	b	0f

	.balign	2
$3:	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, r3, lsl #2]
	blx	r12
	b	0f

	.balign	2
$4:	ldr	r12, [lr, #xPSR]	@ test stack alignment
	tst	r12, #1 << 9
	ite	eq
	ldreq	lr, [lr, #0x20]		@ load syscall id
	ldrne	lr, [lr, #0x24]
	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, lr, lsl #2]
	blx	r12
	b	0f

	.balign	2
$5:	ldr	r12, [lr, #xPSR]	@ test stack alignment
	tst	r12, #1 << 9
	ite	eq
	addeq	lr, #0x20
	addne	lr, #0x24
	ldm	lr, {r12, lr}		@ load syscall id, arg4
	push	{r12}			@ copy arg4 to kernel stack
	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, lr, lsl #2]
	blx	r12
	add	sp, #4
	b	0f

	.balign	2
$6:	ldr	r12, [lr, #xPSR]	@ test stack alignment
	tst	r12, #1 << 9
	ite	eq
	addeq	lr, #0x20
	addne	lr, #0x24
	ldm	lr, {r11, r12, lr}	@ load syscall id, arg4, arg5
	push	{r11, r12}		@ copy arg4, arg5 to kernel stack
	ldr	r12, =syscall_vect
	ldr.w	r12, [r12, lr, lsl #2]
	blx	r12
	pop	{r11, r12}		//FIXME: pop {r11}; add	sp, #4;

	.global syscall_return
syscall_return:
0:	mrs	r1, psp
	str	r0, [r1, #R0]           @ update return value in exception frame
	pop	{pc}
ENDPROC(svcall)
