#include <sys/types.h>

#include <kernel/cbuf.h>
#include <kernel/fs.h>
#include <kernel/irq.h>
#include <kernel/serial.h>
#include <kernel/types.h>

#include <cmsis.h>

static struct cbuf_info cbuf;
static char buf[16];

int stm32f429_getc(struct serial_info *serial, char *c)
{
    serial->rx_count--;
    cbuf_getc(&cbuf, c);

    return 0;
}

int stm32f429_putc(struct serial_info *serial, char c)
{
    USART_TypeDef *uart = serial->priv;

    while (!(uart->SR & USART_SR_TXE))
        ;
    uart->DR = c;

    return 0;
}

int stm32f429_puts(struct serial_info *serial,
                   size_t len,
                   size_t *retlen,
                   const char *buf)
{
    *retlen = len;
    for (int i = 0; len > 0; len--, i++)
        stm32f429_putc(serial, buf[i]);

    return 0;
}

struct serial_info stm32f429_uart1 = {
    .serial_getc = stm32f429_getc,
    .serial_putc = stm32f429_putc,
    .serial_puts = stm32f429_puts,

    .priv = USART1,
};

static void stm32f429_uart1_isr(void)
{
    if (USART1->SR & USART_SR_RXNE) {
        char c = (char) USART1->DR;
        cbuf_putc(&cbuf, c);
        stm32f429_uart1.rx_count++;
        serial_activity_callback(&stm32f429_uart1);
    }
}

extern const struct file_operations serialchar_fops;

static struct inode stm32f429_inode = {
    .i_fop = &serialchar_fops,
    .i_private = &stm32f429_uart1,
};

int stm32f429_init(void)
{
    struct dentry dentry = {.d_inode = &stm32f429_inode, .d_name = "ttyS0"};

    cbuf_init(&cbuf, buf, 16);

    init_tmpfs_inode(&stm32f429_inode);
    vfs_link(0, dev_inode(), &dentry);


    /* enable rx interrupt */
    request_irq(USART1_IRQn, stm32f429_uart1_isr);
    NVIC_EnableIRQ(USART1_IRQn);

    return 0;
}
