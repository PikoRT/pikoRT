#ifndef KERNEL_BITOPS_H
#define KERNEL_BITOPS_H

#define BITS_PER_CHAR 8
#define BITS_PER_LONG (BITS_PER_CHAR * sizeof(long))

static inline unsigned long flsl(unsigned long word)
{
    return word ? sizeof(long) * BITS_PER_CHAR - __builtin_clz(word) : 0;
}

static inline void clear_bit(unsigned long bit, unsigned long *word)
{
    *word &= ~(1 << bit);
}

static inline void set_bit(unsigned long bit, unsigned long *word)
{
    *word |= (1 << bit);
}

static inline void bitmap_set_bit(unsigned long *map, unsigned long bit)
{
    set_bit(bit % BITS_PER_LONG, &map[bit / BITS_PER_LONG]);
}

static inline void bitmap_clear_bit(unsigned long *map, unsigned long bit)
{
    clear_bit(bit % BITS_PER_LONG, &map[bit / BITS_PER_LONG]);
}

static inline unsigned long bitmap_get_bit(unsigned long *map,
                                           unsigned long bit)
{
    return (map[bit / BITS_PER_LONG] >> (bit % BITS_PER_LONG)) & 1;
}

unsigned long find_first_bit(const unsigned long *addr, unsigned long size);
unsigned long find_first_zero_bit(const unsigned long *addr,
                                  unsigned long size);

#endif /* !KERNEL_BITOPS_H */
