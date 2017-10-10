#include <kernel/syscalls.h>
#include <kernel/linkage.h>

	.syntax unified
	.thumb

	// -1: unlocked, 0: locked, positive: locked, possible waiters

/**
 * FIXME: In order to reduce the code base and we tend to keep just one
 *			'bx lr'. But it would make it not intuitional. Probably, we
 *			could discard this optimization.
 */
	@ int pthread_mutex_lock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_lock)
	movs	r2, #0
0:	ldrex	r1, [r0]
    teq r1, #-1			@ check locked?
	bne	1f
	strex	r1, r2, [r0]
    teq r1, #0			@ 'strex' success?
	bne	0b
	dmb			@ ARMv7-M ARM, A3.4.6
	movs	r0, #0		@ it also update EQ flag
1:	beq 2f
	mov	r1, #SYS_PTHREAD_MUTEX_LOCK
	svc	#1
2:	bx	lr
ENDPROC(pthread_mutex_lock)

	@ int pthread_mutex_trylock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_trylock)
	movs	r2, #0
0:	ldrex	r1, [r0]
    teq r1, #-1			@ check locked?
    bne 1f
	strex	r1, r2, [r0]
	teq	r1, #0			@ 'strex' success?
    bne 1f
	movs	r0, #0		@ it also update EQ flag
1:  beq 2f
    mov	r0, #-1
2:	bx	lr
ENDPROC(pthread_mutex_trylock)

	@ int pthread_mutex_unlock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_unlock)
	movs	r2, #-1
0:	ldrex	r1, [r0]
	teq	r1, #0			@ Just one hold lock?
    bne 1f
	strex	r1, r2, [r0]
	teq	r1, #0			@ 'strex' success?
	bne	0b
	dmb			@ ARMv7-M ARM, A3.4.6
	movs	r0, #0		@ it also update EQ flag
1:  beq 2f
	mov	r1, #SYS_PTHREAD_MUTEX_UNLOCK
	svc	#1
2:	bx	lr
ENDPROC(pthread_mutex_lock)
