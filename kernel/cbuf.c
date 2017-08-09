#include <sys/types.h>

#include <kernel/cbuf.h>

void cbuf_init(struct cbuf_info *cbuf, void *buf, size_t len)
{
    cbuf->len = len;
    cbuf->pos_begin = 0;
    cbuf->pos_end = 0;
    cbuf->buf = buf;
}

int cbuf_getc(struct cbuf_info *cbuf, char *c)
{
    if (cbuf->pos_begin == cbuf->pos_end)
        return 0;
    *c = *((char *) cbuf->buf + cbuf->pos_begin);
    cbuf->pos_begin = (cbuf->pos_begin + 1) % cbuf->len;

    return 1;
}

int cbuf_putc(struct cbuf_info *cbuf, char c)
{
    *((char *) cbuf->buf + cbuf->pos_end) = c;
    cbuf->pos_end = (cbuf->pos_end + 1) % cbuf->len;

    return 0;
}
