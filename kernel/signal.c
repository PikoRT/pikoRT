#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <arch/v7m-helper.h>

#include <kernel/bitops.h>
#include <kernel/errno-base.h>
#include <kernel/signal.h>
#include <kernel/task.h>
#include <kernel/thread.h>

#include "kernel.h"
#include "platform.h"

extern void return_from_sighandler(void);
extern void return_from_sigaction(void);

static void *v7m_alloca_thread_context(struct thread_info *tip, size_t len)
{
    tip->ti_mach.mi_psp -= len;

    return (void *) tip->ti_mach.mi_psp;
}

static void stage_sighandler(struct sigaction *sigaction)
{
    CURRENT_THREAD_INFO(curr_thread);
    struct thread_context_regs *ctx;

    /* update current thread SP_process */
    curr_thread->ti_mach.mi_psp = __get_PSP();

    /* this is the exception stacked-context */
    ctx = (struct thread_context_regs *) curr_thread->ti_mach.mi_psp;

    /* return value of syscall, cannot fail after this point */
    ctx->r0_r3__r12[0] = 0;

    /* the sigaction context will be poped by cpu on exception return */
    v7m_alloca_thread_context(curr_thread, sizeof(struct thread_context_regs));

    /* build the sigaction trampoline */
    ctx = (struct thread_context_regs *) curr_thread->ti_mach.mi_psp;
    /* #ifdef SECURE_KERNEL */
    ctx->r0_r3__r12[1] = 0;
    ctx->r0_r3__r12[2] = 0;
    ctx->r0_r3__r12[3] = 0;
    ctx->r0_r3__r12[4] = 0;
    /* #endif */
    ctx->lr = (u32) v7m_set_thumb_bit(return_from_sighandler);
    ctx->ret_addr = (u32) v7m_clear_thumb_bit(sigaction->sa_handler);
    ctx->xpsr = xPSR_T_Msk;

    /* update current thread SP_process */
    __set_PSP(curr_thread->ti_mach.mi_psp);
}

static void stage_sigaction(const struct sigaction *sigaction,
                            int sig,
                            union sigval value)
{
    CURRENT_THREAD_INFO(curr_thread);
    struct thread_context_regs *ctx;

    /* update current thread SP_process */
    curr_thread->ti_mach.mi_psp = __get_PSP();

    /* this is the exception stacked-context */
    ctx = (struct thread_context_regs *) curr_thread->ti_mach.mi_psp;

    /* return value of syscall, cannot fail after this point */
    ctx->r0_r3__r12[0] = 0;

    /* The siginfo_t struct is allocated on thread's stack; that memory
     * will be reclaimed during return_from_sigaction. */
    siginfo_t *siginfo_ptr =
        v7m_alloca_thread_context(curr_thread, sizeof(siginfo_t));
    siginfo_ptr->si_signo = sig;
    siginfo_ptr->si_value = value;
    siginfo_ptr->si_pid = curr_thread->ti_id;

    /* the sigaction context will be poped by cpu on exception return */
    v7m_alloca_thread_context(curr_thread, sizeof(struct thread_context_regs));

    /* build a sigaction trampoline */
    ctx = (struct thread_context_regs *) curr_thread->ti_mach.mi_psp;
    ctx->r0_r3__r12[1] = (u32) siginfo_ptr;
    ctx->r0_r3__r12[2] = 0; /* ucontext_t *, but commonly unused */
    ctx->r0_r3__r12[3] = 0;
    ctx->r0_r3__r12[4] = 0;
    ctx->lr = (u32) v7m_set_thumb_bit(return_from_sigaction);
    ctx->ret_addr = (u32) v7m_clear_thumb_bit(sigaction->sa_sigaction);
    ctx->xpsr = xPSR_T_Msk;

    /* update current thread SP_process */
    __set_PSP(curr_thread->ti_mach.mi_psp);
}

void do_sigevent(const struct sigevent *sigevent, struct thread_info *thread)
{
    CURRENT_THREAD_INFO(curr_thread);
    struct thread_context_regs *ctx;

    // if (sigevent->sigev_notify == SIGEV_THREAD) {

    /* update current thread SP_process */
    if (thread == curr_thread)
        thread->ti_mach.mi_psp = __get_PSP();

    /* the sigevent context will be poped by cpu on exception return */
    v7m_alloca_thread_context(thread, sizeof(struct thread_context_regs));

    /* build a sigevent trampoline */
    ctx = (struct thread_context_regs *) thread->ti_mach.mi_psp;
    ctx->r0_r3__r12[0] = sigevent->sigev_value.sival_int;
    ctx->r0_r3__r12[1] = 0;
    ctx->r0_r3__r12[2] = 0;
    ctx->r0_r3__r12[3] = 0;
    ctx->r0_r3__r12[4] = 0;
    ctx->lr = (u32) v7m_set_thumb_bit(return_from_sighandler);
    ctx->ret_addr = (u32) v7m_clear_thumb_bit(sigevent->sigev_notify_function);
    ctx->xpsr = xPSR_T_Msk;

    /* update current thread SP_process */
    if (thread == curr_thread)
        __set_PSP(thread->ti_mach.mi_psp);
}

static struct sigaction *find_sigaction_by_sig(__unused pid_t pid, int sig)
{
    /* FIXME: consider multi-tasking environment */
    struct signal_info *signal;
    CURRENT_TASK_INFO(curr_task);

    list_for_each_entry(signal, &curr_task->signal_head, list)
    {
        if (signal->signo == sig)
            return &signal->act_storage;
    }

    return NULL;
}

int sys_sigaction(int signo,
                  const struct sigaction *restrict act,
                  struct sigaction *restrict oldact)
{
    if ((signo == SIGKILL) || (signo == SIGSTOP)) {
        errno = EINVAL;
        return -1;
    }
    if (!act) {
        errno = EFAULT;
        return -1;
    }

    if (oldact) {
        struct sigaction *oact = find_sigaction_by_sig(0, signo);
        if (oact != NULL)
            memcpy(oldact, oact, sizeof(struct sigaction));
    }

    struct signal_info *signal = malloc(sizeof(struct signal_info));
    if (!signal) {
        errno = ENOMEM;
        return -1;
    }

    signal->signo = signo;
    CURRENT_TASK_INFO(curr_task);
    list_add(&signal->list, &curr_task->signal_head);
    memcpy(&signal->act_storage, act, sizeof(struct sigaction));

    return 0;
}

/* enabled signal mask */
static unsigned long supported_signal_mask =
    (1 << SIGKILL) | (1 << SIGUSR1) | (1 << SIGUSR2) | (1 << SIGSTOP);

static int is_signal_supported(int sig)
{
    if (sig > SIGMAX)
        return 0;
    return bitmap_get_bit(&supported_signal_mask, sig);
}

/* How signal works?
 *
 * - A fake exception return context is allocated to the user thread stack.
 * - This context is a trampoline to the signal handler.
 * - When the syscall handler returns, the return value is pushed to the user
 *   stack in r0.  For signal handling, r0 must contain the first parameter to
 *   the signal handler function.  The actual return code of the syscall must
 *   be written into the auto-pushed stack context.  The staging functions
 *   handle the update of the error code in the cpu-pushed stackframe.
 */

int sys_kill(__unused pid_t pid, int sig)
{
    if (!is_signal_supported(sig))
        return -EINVAL;

    struct sigaction *act = find_sigaction_by_sig(0, sig);
    if (!act)
        return -EINVAL;

    if (act->sa_flags & SA_SIGINFO)
        stage_sigaction(act, sig, (union sigval){.sival_int = 0});
    else
        stage_sighandler(act);

    return sig;
}
