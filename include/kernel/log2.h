#ifndef KERNEL_LOG2_H
#define KERNEL_LOG2_H

#include <kernel/bitops.h>

// clang-format off

#define ilog2(x)                        \
(                                       \
    __builtin_constant_p(x) ? (         \
        (x) < 1 ? __ilog2_NaN() :       \
        (x) & (1ul << 31) ? 31 :        \
        (x) & (1ul << 30) ? 30 :        \
        (x) & (1ul << 29) ? 29 :        \
        (x) & (1ul << 28) ? 28 :        \
        (x) & (1ul << 27) ? 27 :        \
        (x) & (1ul << 26) ? 26 :        \
        (x) & (1ul << 25) ? 25 :        \
        (x) & (1ul << 24) ? 24 :        \
        (x) & (1ul << 23) ? 23 :        \
        (x) & (1ul << 22) ? 22 :        \
        (x) & (1ul << 21) ? 21 :        \
        (x) & (1ul << 20) ? 20 :        \
        (x) & (1ul << 19) ? 19 :        \
        (x) & (1ul << 18) ? 18 :        \
        (x) & (1ul << 17) ? 17 :        \
        (x) & (1ul << 16) ? 16 :        \
        (x) & (1ul << 15) ? 15 :        \
        (x) & (1ul << 14) ? 14 :        \
        (x) & (1ul << 13) ? 13 :        \
        (x) & (1ul << 12) ? 12 :        \
        (x) & (1ul << 11) ? 11 :        \
        (x) & (1ul << 10) ? 10 :        \
        (x) & (1ul <<  9) ?  9 :        \
        (x) & (1ul <<  8) ?  8 :        \
        (x) & (1ul <<  7) ?  7 :        \
        (x) & (1ul <<  6) ?  6 :        \
        (x) & (1ul <<  5) ?  5 :        \
        (x) & (1ul <<  4) ?  4 :        \
        (x) & (1ul <<  3) ?  3 :        \
        (x) & (1ul <<  2) ?  2 :        \
        (x) & (1ul <<  1) ?  1 :        \
        (x) & (1ul <<  0) ?  0 :        \
        __ilog2_NaN()                   \
        ) :                             \
        __ilog2(x)                      \
)

// clang-format on

static inline unsigned long __ilog2(unsigned long x)
{
    return flsl(x) - 1;
}

static inline __attribute__((noreturn)) unsigned long __ilog2_NaN(void)
{
    for (;;)
        ;
}

#endif /* !KERNEL_LOG2_H */
