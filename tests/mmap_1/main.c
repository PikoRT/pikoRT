/* simple mmap() test, check non-nil memory is allocated */

#include <stddef.h>

#include <piko/sys/mman.h>

#include "unit.h"

int main()
{
    void *p;

    p = mmap(0, 256, 0, MAP_ANONYMOUS, 0, 0);
    if (p == MAP_FAILED)
        TEST_EXIT(1);

    TEST_EXIT(0);
}
