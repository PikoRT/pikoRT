#include "unit.h"
#include "utils.h"
#include <string.h>
#include "kernel.h"
#include <stdio.h>

int sprintf_00(void)
{
    char buf[128];

    sprintf(buf, "%d", 1789);
    if (strcmp(buf, "1789")) {
        printk("%s", buf);
        return -1;
    }

    return 0;
}

int sprintf_01(void)
{
    char buf[128];

    sprintf(buf, "%x", 0xdeadbeef);
    if (strcmp(buf, "deadbeef")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_02(void)
{
    char buf[128];

    sprintf(buf, "Hello %s", "World!");
    if (strcmp(buf, "Hello World!")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_03(void)
{
    char buf[128];

    sprintf(buf, "abcdABCD0123 0x%x, %d, %d, 0x%x", 0xdeadbeef, 1983, 2014,
            4096);
    if (strcmp(buf, "abcdABCD0123 0xdeadbeef, 1983, 2014, 0x1000")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_04(void)
{
    char buf[128];

    sprintf(buf, "%w, %%, %s, %d", "foo", 1986);
    if (strcmp(buf, "%w, %, foo, 1986")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_05(void)
{
    char buf[128];

    sprintf(buf, "%08d %06d %04d", 1986, 1986, 1986);
    if (strcmp(buf, "00001986 001986 1986")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_06(void)
{
    char buf[128];

    sprintf(buf, "%08x %06x %04x %02x", 0xef, 0xef, 0xef, 0xef);
    if (strcmp(buf, "000000ef 0000ef 00ef ef")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int sprintf_07(void)
{
    char buf[128];

    sprintf(buf, "% 7d % 5x", 1986, 0x13);
    if (strcmp(buf, "   1986    13")) {
        printk("%s\n", buf);
        return -1;
    }

    return 0;
}

int main()
{
    int status = 0;
    int (*test[])(void) = {sprintf_00, sprintf_01, sprintf_02, sprintf_03,
                           sprintf_04, sprintf_05, sprintf_06, sprintf_07};

    for (int i = 0; i <= 7; i++) {
        printk("sprintf test #%d\n", i);
        status += test[i]();
    }
    if (status)
        TEST_EXIT(1);

    TEST_EXIT(0);
}
