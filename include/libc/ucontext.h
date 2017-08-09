#ifndef UCONTEXT_H
#define UCONTEXT_H

#include <signal.h>

#include "linux/types.h"

/* machine context on ARM */
typedef struct mcontext {
    u32 sp;  // FIXME: reuse uc_stack.ss_sp
    u32 lr;
    u32 gprs[13]; /* r0-r12 */
    u32 pc;
} __attribute__((packed)) mcontext_t;

typedef struct ucontext {
    struct ucontext *uc_link;
    /* sigset_t uc_sigmask; */
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    /* ... */
} ucontext_t;

/* forward declarations */

void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
int swapcontext(ucontext_t *oucp, ucontext_t *ucp);

#endif /* !UCONTEXT_H */
