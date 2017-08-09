#ifndef _KERNEL_CBUF_H
#define _KERNEL_CBUF_H

#include <sys/types.h>

struct cbuf_info {
    size_t len;
    int pos_begin;
    int pos_end;
    void *buf;
};

void cbuf_init(struct cbuf_info *cbuf, void *buf, size_t len);
int cbuf_getc(struct cbuf_info *cbuf, char *c);
int cbuf_putc(struct cbuf_info *cbuf, char c);

#endif /* !_KERNEL_CBUF_H */
