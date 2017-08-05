#include <kernel/linkage.h>

	.syntax unified
	.thumb

	@ do the thread-context switch
	@
	@ r0: struct thread_info *next
	@ r1: struct thread_info *prev (i.e. the current thread)
	@ (r1), r2, r3, r12: scratch registers
ENTRY(switch_to)
	/* save previous thread context */
	push	{r4-r12, lr}
	mov	r2, sp
	mrs	r3, psp
	stm	r1!, {r2, r3}

ENTRY(thread_restore)
	/* restore next task context */
	ldm	r0!, {r1-r3}
	mov	sp, r1
	msr	psp, r2
	msr	control, r3		@ write to SPSEL is ignored in Handler_Mode
	pop	{r4-r12, pc}
ENDPROC(thread_restore)
ENDPROC(switch_to)

	.macro	emulate_iret
	ldrd	r0, r1, [sp, #24]	/* r0 = ret_addr, r1 = xpsr */
	orr	r0, #1
	str	r0, [sp, #24]
	msr	apsr_nzcvq, r1		/* restore flags */
	pop	{r0-r3, r12, lr}
	ldr	pc, [sp], #8		/* return into user thread */
	.endm

ENTRY(return_from_sigaction)
	add	sp, #12			/* reclaim siginfo_t storage */
	emulate_iret
END(return_from_sigaction)

ENTRY(return_from_sighandler)
	emulate_iret
END(return_from_sighandler)

	.set MC_SP, 0
	.set MC_LR, 4
	.set MC_GPRS, 8
	.set MC_PC, 15*4
	.set UC_MCONTEXT, 16
	.set UC_LINK, 0

	/* Swap context to a clean context (created from makecontext()):
	 *   - save the non-scratch registers
	 *   - just restore scratch registers (for arguments to entry function)
	 *
	 * Swap to a dirty context (inited with getcontext, or return to a
	 * swapped context):
	 *   - save the non-scratch registers
	 *   - restore all registers
	 */

ENTRY(swapcontext)
	/* save to oucp */
	add	r0, #UC_MCONTEXT
	str	sp, [r0, #MC_SP]
	str	lr, [r0, #MC_LR]
	add	r0, #MC_GPRS
	stm	r0, {r0-r12, lr}

	/* retore from ucp */
	add	r1, #UC_MCONTEXT
	ldr	sp, [r1, #MC_SP]
	ldr	lr, [r1, #MC_LR]
	add	r1, #MC_GPRS
	ldm	r1, {r0-r12, pc}
ENDPROC(swapcontext)

ENTRY(return_from_makecontext)
	/* retrieve the struct& from the top of the stack */
	ldr	r0, [sp]

	/* get the machine struct for the linked context */
	ldr	r0, [r0, #UC_LINK]
	add	r0, #UC_MCONTEXT

	/* restore the link context */
	ldr	sp, [r0, #MC_SP]
	ldr	lr, [r0, #MC_LR]
	add	r0, #MC_GPRS
	//FIXME: return code - the r0 value that is restored from the stack should be 0
	ldm	r0, {r0-r12, pc}
ENDPROC(return_from_makecontext)

	.macro	fault_handler f
	push	{r4-r11}	@ save non-scratch registers at time of fault
	mov	r0, sp
	mov	r2, lr		@ lr contains EXC_RETURN, fault was taken in thread or interrupt?

	@ We don't handle faults taken in interrupt handler at the moment.
	@ Hence the scratch registers auto pushed to the stack by the CPU
	@ on interrupt-entry have been pushed to PSP (MSP is only used by
	@ interrupts handler).
	mrs	r1, psp

	b	\f
	.endm	/* fault_handler */

ENTRY(hardf)
	fault_handler hardfault
ENDPROC(hardf)

ENTRY(memf)
	fault_handler memmanage
ENDPROC(memf)

ENTRY(busf)
	fault_handler busfault
ENDPROC(busf)

ENTRY(usgf)
	fault_handler usagefault
ENDPROC(usgf)

ENTRY(__do_idle)
	wfi
	bx	lr
ENDPROC(__do_idle)

	/* void v7m_semihost_exit(int status); */
ENTRY(v7m_semihost_exit)
	tst	r0, r0
	ite	eq
	ldreq	r1, =0x20026    /* [a] */
	movne	r1, #0
	mov	r0, #0x18       /* [b] */
	bkpt	#0xab
0:	b	0b
ENDPROC(v7m_semihost_exit)

/* [a] https://lists.nongnu.org/archive/html/qemu-devel/2014-12/msg01575.html
 *     ADP_Stopped_ApplicationExit is used for exit(0), anything else is
 *     implemented as exit(1). */

/* [b] http://git.qemu.org/?p=qemu.git;a=blob_plain;f=target-arm/arm-semi.c;hb=HEAD
 *     List of supported semihosting calls in Qemu. */
