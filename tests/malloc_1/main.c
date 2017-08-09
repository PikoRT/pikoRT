#include <stdlib.h>
#include <string.h>

#include "unit.h"
#include "kernel.h"

int main()
{
    char *p[3], *q[3];

    p[0] = malloc(48);
    p[1] = malloc(64);
    p[2] = malloc(96);

    for (int i = 0; i < 3; i++)
        printk("allocated %p\n", p[i]);

    memset(p[0], 'a', 48);
    memset(p[1], 'b', 64);
    memset(p[2], 'c', 96);

    for (int i = 0; i < 48; i++) {
        if (p[0][i] != 'a')
            TEST_EXIT(1);
    }
    for (int i = 0; i < 64; i++) {
        if (p[1][i] != 'b')
            TEST_EXIT(1);
    }
    for (int i = 0; i < 96; i++) {
        if (p[2][i] != 'c')
            TEST_EXIT(1);
    }

    /* free all pointers */
    free(p[0]);
    free(p[1]);
    free(p[2]);

    /* Realloc the same memory mapping, should return the same allocation
     * mapping.    */
    q[0] = malloc(48);
    q[1] = malloc(64);
    q[2] = malloc(96);

    for (int i = 0; i < 3; i++)
        printk("allocated %p\n", p[i]);

    for (int i = 0; i < 3; i++) {
        if (p[i] != q[i])
            TEST_EXIT(1);
    }

    TEST_EXIT(0);
}
