#include <sys/types.h>

#include <kernel/cbuf.h>
#include <kernel/fs.h>
#include <kernel/irq.h>
#include <kernel/serial.h>
#include <kernel/types.h>

#include <cmsis.h>

static struct cbuf_info cbuf;
static char buf[16];

static int stm32f1_getc(struct serial_info *serial, char *c)
{
    serial->rx_count--;
    cbuf_getc(&cbuf, c);

    return 0;
}

static int stm32f1_putc(struct serial_info *serial, char c)
{
    USART_TypeDef *uart = serial->priv;

    while (!(uart->SR & USART_SR_TXE))
        ;
    uart->DR = c;

    return 0;
}

static int stm32f1_puts(struct serial_info *serial,
                        size_t len,
                        size_t *retlen,
                        const char *buf)
{
    *retlen = len;
    for (int i = 0; len > 0; len--, i++)
        stm32f1_putc(serial, buf[i]);

    return 0;
}

static struct serial_info stm32f1_uart2 = {
    .serial_getc = stm32f1_getc,
    .serial_putc = stm32f1_putc,
    .serial_puts = stm32f1_puts,

    .priv = USART2,
};

static void stm32f1_uart2_isr(void)
{
    if (USART2->SR & USART_SR_RXNE) {
        char c = (char) USART2->DR;
        cbuf_putc(&cbuf, c);
        stm32f1_uart2.rx_count++;
        serial_activity_callback(&stm32f1_uart2);
    }
}

extern const struct file_operations serialchar_fops;

static struct inode stm32f1_inode = {
    .i_fop = &serialchar_fops,
    .i_private = &stm32f1_uart2,
};

int stm32f1_init(void)
{
    struct dentry dentry = {.d_inode = &stm32f1_inode, .d_name = "ttyS0"};

    cbuf_init(&cbuf, buf, 16);

    init_tmpfs_inode(&stm32f1_inode);
    vfs_link(0, dev_inode(), &dentry);

    /* enable rx interrupt */
    request_irq(USART2_IRQn, stm32f1_uart2_isr);
    NVIC_EnableIRQ(USART2_IRQn);

    return 0;
}
