/* stress test of page alloc/free */

#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#include <kernel/kernel.h>
#include <kernel/mm/page.h>
#include "unit.h"

struct alloc_info {
    void *addr;
    int order;
};

struct alloc_info alloc_info[128];

int main()
{
    int i;
    int fd;
    unsigned int order;
    unsigned long hash;
    size_t sz;

    fd = open("/dev/random", 0);
    if (fd < 0) {
        printk("error: failed to open /dev/random\n");
        TEST_EXIT(1);
    }

    /* take a snapshot of memory state bgefore the test */
    hash = page_alloc_signature();

    for (int k = 0; k < 1000; k++) {
        sz = 0;
        for (i = 0; i < 40; i++) {
            read(fd, &order, 1);
            order = order % 4;
            alloc_info[i].addr = alloc_pages(order);
            alloc_info[i].order = order;
            if (!alloc_info[i].addr)
                break;
            sz += 1 << (order + 8);
            /* printk("  %p  (order=%d)\n", alloc_info[i].addr, */
            /* 	alloc_info[i].order); */
        }
        /* printk("Allocated %d bytes (%d bytes available)\n", sz, 32 * 1024);
         */

        /* pseudo-randomish freeing of the memory.. */
        for (int j = 0; j < i; j += 3)
            free_pages((unsigned long) alloc_info[j].addr, alloc_info[j].order);
        for (int j = 1; j < i; j += 3)
            free_pages((unsigned long) alloc_info[j].addr, alloc_info[j].order);
        for (int j = 2; j < i; j += 3)
            free_pages((unsigned long) alloc_info[j].addr, alloc_info[j].order);

        if (page_alloc_signature() != hash) {
            printk("error: Memory not correctly restored\n");
            TEST_EXIT(1);
        }
    }

    TEST_EXIT(0);
}
