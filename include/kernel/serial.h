/*
 * Interface for serial devices (UART, ...)
 */

#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <sys/types.h>

struct serial_ops;
struct device;
struct thread_info;

struct serial_info {
    void *priv;
    unsigned int rx_count;

    struct serial_ops *ops;

    // XXX: owner thread pointer could go to priv, and device pripheral
    // base address should be linked to private data for the device
    struct thread_info *owner;
};

struct serial_ops {
    int (*serial_init)(struct serial_info *serial);

    int (*serial_getc)(struct serial_info *serial, char *c);
    int (*serial_gets)(struct serial_info *serial,
                       size_t len,
                       size_t *retlen,
                       char *buf);
    int (*serial_putc)(struct serial_info *serial, char c);
    int (*serial_puts)(struct serial_info *serial,
                       size_t len,
                       size_t *retlen,
                       const char *buf);

    /* callback on device activity, set by ioctl() */
    void (*callback)(struct serial_info *self);
};


/* Generic usart/uart setup func */
void usart_init(void);
/* Generic serial init func */
int serial_init(void);

/* XXX: All func below is callback function */
int serial_getc(struct serial_info *serial, char *c);
int serial_gets(struct serial_info *serial,
                size_t len,
                size_t *retlen,
                char *buf);
int serial_putc(struct serial_info *serial, char c);
int serial_puts(struct serial_info *serial,
                size_t len,
                size_t *retlen,
                const char *buf);

void serial_activity_callback(struct serial_info *serial);

#endif /* !_KERNEL_SERIAL_H */
