#include <sys/types.h>

#include <kernel/cbuf.h>
#include <kernel/fs.h>
#include <kernel/irq.h>
#include <kernel/serial.h>
#include <kernel/types.h>

#include <../cmsis/lm3s6965/lm3s_cmsis.h>

#define LM3S_UARTIM_RXIM_Pos 4
#define LM3S_UARTICR_RXIC_Pos 4

static struct cbuf_info cbuf;
static char buf[16];

int lm3s6965_getc(struct serial_info *serial, char *c)
{
	serial->rx_count--;
	cbuf_getc(&cbuf, c);

	return 0;
}

/* int serial_gets(struct serial_info *serial, size_t len, */
/* 		size_t *retlen, char *buf); */

int lm3s6965_putc(struct serial_info *serial, char c)
{
	UART_Type *uart = serial->priv;

	while (uart->FR & (1 << 3))
		;
	uart->DR = c;

	return 0;
}

int lm3s6965_puts(struct serial_info *serial,
                  size_t len,
                  size_t *retlen,
                  const char *buf)
{
	*retlen = len;
	for (int i = 0; len > 0; len--, i++)
		lm3s6965_putc(serial, buf[i]);

	return 0;
}

struct serial_info lm3s6965_uart0 = {
	.serial_getc = lm3s6965_getc,
	.serial_putc = lm3s6965_putc,
	.serial_puts = lm3s6965_puts,

	.priv = UART0,
};

static void lm3s6965_uart0_isr(void)
{
	char c = (char) UART0->DR;
	cbuf_putc(&cbuf, c);
	lm3s6965_uart0.rx_count++;
	UART0->ICR = 1 << LM3S_UARTICR_RXIC_Pos; /* ack */
	serial_activity_callback(&lm3s6965_uart0);
}

extern const struct file_operations serialchar_fops;

static struct inode lm3s6965_inode = {
	.i_fop = &serialchar_fops,
	.i_private = &lm3s6965_uart0,
};

int lm3s6965_init(void)
{
	struct dentry dentry = {.d_inode = &lm3s6965_inode, .d_name = "ttyS0"};

	cbuf_init(&cbuf, buf, 16);

	init_tmpfs_inode(&lm3s6965_inode);
	vfs_link(0, dev_inode(), &dentry);

	/* configure link */
	UART0->CTL |= 1;
	UART0->LCRH |= (3 << 5);

	/* enable rx interrupt */
	request_irq(UART0_IRQn, lm3s6965_uart0_isr);
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->IM = 1 << LM3S_UARTIM_RXIM_Pos;

	return 0;
}
