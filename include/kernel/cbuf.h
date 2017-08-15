#ifndef _KERNEL_CBUF_H
#define _KERNEL_CBUF_H

#include <sys/types.h>

struct cbuf_info {
    size_t len;
    int pos_begin;
    int pos_end;
    void *buf;
};

static inline void cbuf_init(struct cbuf_info *cbuf, void *buf, size_t len)
{
    cbuf->len = len;
    cbuf->pos_begin = 0;
    cbuf->pos_end = 0;
    cbuf->buf = buf;
}

static inline int cbuf_getc(struct cbuf_info *cbuf, char *c)
{
    if (cbuf->pos_begin == cbuf->pos_end)
        return 0;
    *c = *((char *) cbuf->buf + cbuf->pos_begin);
    cbuf->pos_begin = (cbuf->pos_begin + 1) % cbuf->len;

    return 1;
}

static inline int cbuf_putc(struct cbuf_info *cbuf, char c)
{
    *((char *) cbuf->buf + cbuf->pos_end) = c;
    cbuf->pos_end = (cbuf->pos_end + 1) % cbuf->len;

    return 0;
}

#endif /* !_KERNEL_CBUF_H */
