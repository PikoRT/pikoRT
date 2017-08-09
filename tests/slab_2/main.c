/* test for cache creation, allocation, destruction */

#include <kernel/kernel.h>
#include <kernel/mm/page.h>
#include <kernel/mm/slab.h>

#include "unit.h"

struct foo {
    int a;
    int b;
};

int main(void)
{
    unsigned long hash;

    struct kmem_cache *cache = KMEM_CACHE(struct foo, "cache-foo");

    if (!cache) {
        printk("error: Cannot create cache\n");
        TEST_EXIT(1);
    }

    hash = page_alloc_signature();

    struct foo *fp[40];
    for (int i = 0; i < 40; i++) {
        fp[i] = kmem_cache_alloc(cache, CACHE_OPT_NONE);
        fp[i]->a = i;
    }

    if (page_alloc_signature() == hash) {
        printk("error: No memory allocated\n");
        TEST_EXIT(1);
    }

    for (int i = 0; i < 40; i++) {
        if (fp[i]->a != i) {
            printk("error: Address allocated multiple times\n");
            TEST_EXIT(1);
        }
        kmem_cache_free(cache, fp[i]);
    }

    if (page_alloc_signature() != hash) {
        printk("error: Memory not correctly restored\n");
        TEST_EXIT(1);
    }

    TEST_EXIT(0);
}
