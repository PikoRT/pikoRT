/* simple test for cache creation and allocation */

#include <kernel/kernel.h>
#include <kernel/mm/slab.h>

#include "unit.h"

struct foo {
    int a;
    int b;
};

int main(void)
{
    struct kmem_cache *cache = KMEM_CACHE(struct foo, "cache-foo");
    if (!cache) {
        printk("error: Cannot create cache\n");
        TEST_EXIT(1);
    }

    struct foo *foo = kmem_cache_alloc(cache, CACHE_OPT_NONE);
    if (!foo) {
        printk("error: Cannot allocate from cache\n");
        TEST_EXIT(1);
    }

    foo->a = 1;
    foo->b = 2;

    TEST_EXIT(0);
}
