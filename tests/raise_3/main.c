/* test signal handler with SA_SIGINFO set */

#include <stddef.h>
#include <signal.h>
#include "kernel.h"
#include "unit.h"

int val;
int signo = SIGUSR1;

void sigact(int sig, siginfo_t *siginfo, void *unused)
{
    (void) unused;

    printk("In a sigaction handler (signo=%d, sival=0x%x)\n", sig,
           siginfo->si_value.sival_int);
    if (sig != signo)
        TEST_EXIT(1);

    val = 1;
}

int main(void *arg)
{
    (void) arg;

    const struct sigaction act = {.sa_sigaction = sigact,
                                  .sa_flags = SA_SIGINFO};

    sigaction(signo, &act, NULL);
    int retval = raise(signo);
    if (retval) {
        printk("Syscall returned %d, expected 0\n", retval);
        TEST_EXIT(1);
    }
    if (!val)
        TEST_EXIT(1);

    TEST_EXIT(0);
}
