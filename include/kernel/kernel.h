#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

/* round-down to a power of 2 */
#define align(x, a) align_mask(x, (__typeof__(x))((a) -1))
#define align_mask(x, mask) ((x) & ~(mask))

/* round-up to a power of 2 */
#define align_next(x, a) align_next_mask(x, (__typeof__(x))((a) -1))
#define align_next_mask(x, mask) (((x) + (mask)) & ~(mask))

#define ARRAY_SIZE(arr) ((size_t)(sizeof(arr) / sizeof(*(arr))))

#define ARRAY_INDEX(elt, arr)                     \
    ({                                            \
        unsigned int _elt = (unsigned int) (elt); \
        unsigned int _arr = (unsigned int) (arr); \
        (_elt - _arr) / sizeof(*(elt));           \
    })

#define container_of(ptr, type, member)                         \
    ({                                                          \
        const __typeof__(((type *) 0)->member) *__mptr = (ptr); \
        (type *) ((char *) __mptr - offsetof(type, member));    \
    })

#define SWAP(x, y)             \
    {                          \
        __typeof__(x) tmp = x; \
        x = y;                 \
        y = tmp;               \
    }                          \
    while (0)

int printk(const char *fmt, ...);

#endif /* !_KERNEL_KERNEL_H */
