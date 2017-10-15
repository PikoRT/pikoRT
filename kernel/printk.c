#include <stdarg.h>
#include <stdio.h>
#include <ucontext.h>

#define VSNPRINTF_BUF_SIZE 256

static ucontext_t printk_context;
static ucontext_t vsnprintf_context = {.uc_link = &printk_context};
static unsigned int ctx_stack[128];
static char vsnprintf_buf[VSNPRINTF_BUF_SIZE];
static int retval;

/* not thread-safe, not reentrant */
static void co_vsnprintf(const char *format, va_list ap)
{
    retval = vsnprintf(vsnprintf_buf, VSNPRINTF_BUF_SIZE, format, ap);
}

#include <serial.h>
#include "platform.h"

void __printk_init(void)
{
    uart_init();
}

void __printk_putchar(char c)
{
    if (c == '\n')
        __printk_putchar('\r');

    while (!(USARTx->SR & USART_SR_TXE))
        ;
    USARTx->DR = (0xff) & c;
}

int printk(const char *format, ...)
{
    /*FIXME: should be interruptable*/
    __disable_irq();
    va_list ap;

    va_start(ap, format);
    vsnprintf_context.uc_stack.ss_sp = &ctx_stack[128];
    makecontext(&vsnprintf_context, co_vsnprintf, 2, format, ap);
    swapcontext(&printk_context, &vsnprintf_context);
    for (char *c = vsnprintf_buf; *c != '\0'; c++)
        __printk_putchar(*c);
    va_end(ap);

    /*FIXME: should be interruptable*/
    __enable_irq();
    return retval;
}
