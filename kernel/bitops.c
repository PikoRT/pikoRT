#include <sys/param.h>

#include <kernel/bitops.h>
#include <kernel/kernel.h>

unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
{
    for (unsigned long i = 0; i * BITS_PER_LONG < size; i++) {
        if (addr[i])
            return MIN(i * BITS_PER_LONG + __builtin_ffsl(addr[i]) - 1, size);
    }

    return size;
}

unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
    for (unsigned long i = 0; i * BITS_PER_LONG < size; i++) {
        if (addr[i] != ~0ul)
            return MIN(i * BITS_PER_LONG + __builtin_ffsl(~addr[i]) - 1, size);
    }

    return size;
}
