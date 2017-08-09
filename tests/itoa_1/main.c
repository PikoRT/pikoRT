#include "unit.h"
#include "utils.h"
#include <string.h>

#include <kernel/kernel.h>

int itoa_base_00(void)
{
    char buf[64];
    if (strcmp("0", itoa_base(0, buf, 1)))
        return -1;
    return 0;
}

int itoa_base_01(void)
{
    char buf[64];
    if (strcmp("0", itoa_base(0, buf, 8)))
        return -1;
    return 0;
}

int itoa_base_02(void)
{
    char buf[64];
    if (strcmp("1234567890", itoa_base(1234567890, buf, 10)))
        return -1;
    return 0;
}

int itoa_base_03(void)
{
    char buf[64];
    if (strcmp("20", itoa_base(32, buf, 16)))
        return -1;
    return 0;
}

int itoa_base_04(void)
{
    char buf[64];
    if (strcmp("21", itoa_base(33, buf, 16)))
        return -1;
    return 0;
}

int itoa_base_05(void)
{
    char buf[64];
    if (itoa_base(458, buf, 2048))
        return -1;
    return 0;
}

int itoa_base_06(void)
{
    char buf[64];
    if (strcmp("ffffffff", itoa_base(0xffffffff, buf, 16)))
        return -1;
    return 0;
}

int itoa_base_07(void)
{
    char buf[64];
    if (strcmp("-1", itoa_base(0xffffffff, buf, 10)))
        return -1;
    return 0;
}

int itoa_base_08(void)
{
    char buf[64];
    if (strcmp("1000", itoa_base(0x1000, buf, 16)))
        return -1;
    return 0;
}

int itoa_base_09(void)
{
    char buf[64];
    if (strcmp("-752", itoa_base(-752, buf, 10)))
        return -1;
    return 0;
}

int itoa_base_10(void)
{
    char buf[64];
    if (strcmp("0", itoa_base(0, buf, 10)))
        return -1;
    return 0;
}

int main()
{
    int status = 0;
    int (*test[])(void) = {itoa_base_00, itoa_base_01, itoa_base_02,
                           itoa_base_03, itoa_base_04, itoa_base_05,
                           itoa_base_06, itoa_base_07, itoa_base_08,
                           itoa_base_09, itoa_base_10};

    for (int i = 0; i < (int) ARRAY_SIZE(test); i++) {
        int r = test[i]();
        /* printk("itoa test #%d %s\n", i, r ? "(failed)" : ""); */
        status += r;
    }
    if (status)
        TEST_EXIT(1);

    TEST_EXIT(0);
}
