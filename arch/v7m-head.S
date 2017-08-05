#include <kernel/linkage.h>

	.syntax unified
	.thumb

	.section ".vector", "a"
	.long	__early_stack_start__		@ SP_main value
	.long	reset				@ Reset
	.long	0				@ NMI
	.long	hardf				@ HardFault
	.long	memf				@ MemManage
	.long	busf				@ BusFault
	.long	usgf				@ UsageFault
	.long	0				@ Reserved
	.long	0				@ Reserved
	.long	0				@ Reserved
	.long	0				@ Reserved
	.long	svcall				@ SVCall
	.long	0				@ Debug Monitor
	.long	0				@ Reserved
	.long	pendsv				@ PendSV
	.long	systick				@ SysTick
	.long	irq_entry			@ IRQ 0
	.long	irq_entry			@ IRQ 1
	.long	irq_entry			@ IRQ 2
	.long	irq_entry			@ IRQ 3
	.long	irq_entry			@ IRQ 4
	.long	irq_entry			@ IRQ 5
	.long	irq_entry			@ IRQ 6
	.long	irq_entry			@ IRQ 7
	.long	irq_entry			@ IRQ 8
	.long	irq_entry			@ IRQ 9
	.long	irq_entry			@ IRQ 10
	.long	irq_entry			@ IRQ 11
	.long	irq_entry			@ IRQ 12
	.long	irq_entry			@ IRQ 13
	.long	irq_entry			@ IRQ 14
	.long	irq_entry			@ IRQ 15
	.long	irq_entry			@ IRQ 16
	.long	irq_entry			@ IRQ 17
	.long	irq_entry			@ IRQ 18
	.long	irq_entry			@ IRQ 19
	.long	irq_entry			@ IRQ 20
	.long	irq_entry			@ IRQ 21
	.long	irq_entry			@ IRQ 22
	.long	irq_entry			@ IRQ 23
	.long	irq_entry			@ IRQ 24
	.long	irq_entry			@ IRQ 25
	.long	irq_entry			@ IRQ 26
	.long	irq_entry			@ IRQ 27
	.long	irq_entry			@ IRQ 28
	.long	irq_entry			@ IRQ 29
	.long	irq_entry			@ IRQ 30
	.long	irq_entry			@ IRQ 31
	.long	irq_entry			@ IRQ 32
	.long	irq_entry			@ IRQ 33
	.long	irq_entry			@ IRQ 34
	.long	irq_entry			@ IRQ 35
	.long	irq_entry			@ IRQ 36
	.long	irq_entry			@ IRQ 37
	.long	irq_entry			@ IRQ 38
	.long	irq_entry			@ IRQ 39
	.long	irq_entry			@ IRQ 40
	.long	irq_entry			@ IRQ 41
	.long	irq_entry			@ IRQ 42
	.long	irq_entry			@ IRQ 43
	.long	irq_entry			@ IRQ 44
	.long	irq_entry			@ IRQ 45
	.long	irq_entry			@ IRQ 46
	.long	irq_entry			@ IRQ 47
	.long	irq_entry			@ IRQ 48
	.long	irq_entry			@ IRQ 49
	.long	irq_entry			@ IRQ 50
	.long	irq_entry			@ IRQ 51
	.long	irq_entry			@ IRQ 52
	.long	irq_entry			@ IRQ 53
	.long	irq_entry			@ IRQ 54
	.long	irq_entry			@ IRQ 55
	.long	irq_entry			@ IRQ 56
	.long	irq_entry			@ IRQ 57
	.long	irq_entry			@ IRQ 58
	.long	irq_entry			@ IRQ 59
	.long	irq_entry			@ IRQ 60
	.long	irq_entry			@ IRQ 61
	.long	irq_entry			@ IRQ 62
	.long	irq_entry			@ IRQ 63

	.text

/*
 * mov32 - loads a 32-bit value into a register without a data access
 */
	.macro  mov32 rd, imm32
	movw    \rd, #:lower16:\imm32
	.if     \imm32 & 0xffff0000
	movt    \rd, #:upper16:\imm32
	.endif
	.endm

ENTRY(reset)
	ldr	r0, =SystemInit			/* CMSIS system init */
	blx	r0

	mov32	r0, 0xe000ed00			/* SCB_BASE */

	@ switch to Handler_Mode
	ldr	r1, =__early_stack_end__	/* allocate a temporary vector */
	str	r1, [r0, #8]			/* update VTOR */
	ldr	r2, =0f
	orr	r2, #1				/* set thumb bit */
	str	r2, [r1, #11 * 4]		/* offset to SVC entry */
	dsb					/* [1] */
	svc	#0
0:

	@ restore the early vector
	ldr	r1, =__vector_start__
	str	r1, [r0, #8]			/* restore VTOR */
	dsb					/* [1] */

	@ copy the initialized data sections
	ldr	r0, =__data_start__
	ldr	r1, =__rodata_end__
	ldr	r2, =__data_size__
	bl	memcpy

	@ zero-fill the non-initialized data sections
	ldr	r0, =__bss_start__
	movs    r1, #0
	ldr r2, =__bss_size__
	bl	memset

	@ start_kernel procedure returns the first thread to run on the CPU
	bl	start_kernel
	cmp	r0, #0
	itt	ne
	ldrne	r1, =thread_restore
	bxne	r1
0:	b	0b
ENDPROC(reset)

/* [1] http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHDGBJG.html */

ENTRY(irq_entry)
	push	{lr}
	ldr	r0, =irq_handler
	mrs	r1, ipsr
	sub	r1, #16
	ldr.w	r0, [r0, r1, lsl #2]
	blx	r0
	pop	{pc}
ENDPROC(irq_entry)

	.weak systick
systick:
	bx	lr

	.balign 8
