#include <ucontext.h>

#include "unit.h"

static int test_status;
static ucontext_t main_context, other_context;
static unsigned int ctx_stack[128];

void __printk_putchar(char c);

static void pputs(const char *s)
{
    for (; *s != '\0'; s++)
        __printk_putchar(*s);
}

void test(int a1, int a2, int a3, int a4)
{
    pputs("Hello from a new context!\n");

    if ((a1 != 9) || (a2 != 0xcafe) || (a3 != 13) || (a4 != 14)) {
        pputs("failed: incorrect correct arg.\n");
        test_status = 1;
    }
}

int main()
{
    other_context.uc_link = &main_context;
    other_context.uc_stack.ss_sp = &ctx_stack[128];

    /* pass 4 arguments to the new context, and swap */
    makecontext(&other_context, test, 4, 9, 0xcafe, 13, 14);
    swapcontext(&main_context, &other_context);

    pputs("And back to the main context.\n");

    TEST_EXIT(test_status);
}
