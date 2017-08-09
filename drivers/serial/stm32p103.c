#include <sys/types.h>

#include <kernel/cbuf.h>
#include <kernel/fs.h>
#include <kernel/irq.h>
#include <kernel/serial.h>
#include <kernel/types.h>

#include <cmsis.h>

static struct cbuf_info cbuf;
static char buf[16];

int stm32p103_getc(struct serial_info *serial, char *c)
{
    serial->rx_count--;
    cbuf_getc(&cbuf, c);

    return 0;
}

/* int serial_gets(struct serial_info *serial, size_t len, */
/* 		size_t *retlen, char *buf); */

int stm32p103_putc(struct serial_info *serial, char c)
{
    USART_TypeDef *uart = serial->priv;

    while (!(uart->SR & USART_SR_TXE))
        ;
    uart->DR = c;

    return 0;
}

int stm32p103_puts(struct serial_info *serial,
                   size_t len,
                   size_t *retlen,
                   const char *buf)
{
    *retlen = len;
    for (int i = 0; len > 0; len--, i++)
        stm32p103_putc(serial, buf[i]);

    return 0;
}

struct serial_info stm32p103_uart2 = {
    .serial_getc = stm32p103_getc,
    .serial_putc = stm32p103_putc,
    .serial_puts = stm32p103_puts,

    .priv = USART2,
};

static void stm32p103_uart2_isr(void)
{
    if (USART2->SR & USART_SR_RXNE) {
        char c = (char) USART2->DR;
        cbuf_putc(&cbuf, c);
        stm32p103_uart2.rx_count++;
        // FIXME:
        // USART2->ICR = 1 << LM3S_UARTICR_RXIC_Pos; /* ack */
        serial_activity_callback(&stm32p103_uart2);
    }
}

extern const struct file_operations serialchar_fops;

static struct inode stm32p103_inode = {
    .i_fop = &serialchar_fops,
    .i_private = &stm32p103_uart2,
};

int stm32p103_init(void)
{
    struct dentry dentry = {.d_inode = &stm32p103_inode, .d_name = "ttyS0"};

    cbuf_init(&cbuf, buf, 16);

    init_tmpfs_inode(&stm32p103_inode);
    vfs_link(0, dev_inode(), &dentry);

    /* configure link */
    // USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);

    /* enable rx interrupt */
    request_irq(USART2_IRQn, stm32p103_uart2_isr);
    NVIC_EnableIRQ(USART2_IRQn);
    // FIXME:
    // UART1->IM = 1 << LM3S_UARTIM_RXIM_Pos;

    return 0;
}
