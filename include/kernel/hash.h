#ifndef _KERNEL_HASH_H
#define _KERNEL_HASH_H

#include <stddef.h>

static inline unsigned long hash_djb2(unsigned char *str, size_t len)
{
    unsigned long hash = 5381;

    for (int i = 0; i < (int) len; i++) {
        int c = str[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

#endif /* _KERNEL_HASH_H */
