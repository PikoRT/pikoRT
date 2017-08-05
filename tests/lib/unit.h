#ifndef TEST_LIB_UNIT_H
#define TEST_LIB_UNIT_H

#include "kernel.h"

#define C_GREEN "\033[1;32m"
#define C_RED "\033[1;31m"
#define C_NORMAL "\033[0m"

#define printk_green(format, ...) \
    printk(C_GREEN format C_NORMAL, ##__VA_ARGS__);

#define printk_red(format, ...) printk(C_RED format C_NORMAL, ##__VA_ARGS__);

#include <arch/semihosting.h>

#define TEST_EXIT(status)                            \
    ({                                               \
        if (status) {                                \
            printk_red("test failed: %d", (status)); \
            printk("\n");                            \
        } else {                                     \
            printk_green("test passed");             \
            printk("\n");                            \
        }                                            \
        v7m_semihost_exit(status);                   \
    })

#endif /* !TEST_LIB_UNIT_H */
