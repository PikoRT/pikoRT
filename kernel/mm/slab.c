#include <stdio.h>
#include <string.h>
#include <sys/cdefs.h>

#include <kernel/bitops.h>
//#include <kernel/kernel.h>
#include <kernel/mm/page.h>
#include <kernel/mm/slab.h>

#include "linux/list.h"

#define OBJECTS_PER_SLAB(objsize) \
    ((CACHE_PAGE_SIZE - sizeof(struct slab)) / (objsize))

static LIST_HEAD(caches);

static struct kmem_cache cache_caches = {
    .objsize = sizeof(struct kmem_cache),
    .objnum = OBJECTS_PER_SLAB(sizeof(struct kmem_cache)),
    .name = "cache-cache",
    .slabs_free = LIST_HEAD_INIT(cache_caches.slabs_free),
    .slabs_partial = LIST_HEAD_INIT(cache_caches.slabs_partial),
    .slabs_full = LIST_HEAD_INIT(cache_caches.slabs_full),
    .alloc_succeed = 0,
    .alloc_fail = 0,
    .opts = CACHE_OPT_NONE,
};

static inline struct slab *get_slab_from_obj(void *obj, size_t page_size)
{
    return (struct slab *) align((unsigned long) obj, page_size);
}

static inline int obj_index_in_slab(void *obj, struct kmem_cache *cache)
{
    return (((unsigned long) obj & ((1 << 8) - 1)) - sizeof(struct slab)) /
           cache->objsize;
}

struct kmem_cache *kmem_cache_create(const char *name,
                                     size_t size,
                                     __unused size_t align,
                                     __unused unsigned long flags,
                                     __unused void (*ctor)(void *))
{
    struct kmem_cache *cache;

    if (size < 8) {
        printk("error: Use cache for objects with size >= 8 bytes\n");
        return NULL;
    }
    if (size > (256 - sizeof(struct slab)) / 2) {
        printk("error: Object size is too big\n");
        return NULL;
    }

    cache = kmem_cache_alloc(&cache_caches, 0);
    if (!cache)
        return NULL;
    cache->objsize = size;
    cache->objnum = OBJECTS_PER_SLAB(size);
    cache->opts = CACHE_OPT_NONE;
    strncpy(cache->name, name, CACHE_NAMELEN);
    INIT_LIST_HEAD(&cache->slabs_free);
    INIT_LIST_HEAD(&cache->slabs_partial);
    INIT_LIST_HEAD(&cache->slabs_full);
    list_add(&cache->list, &caches);

    return cache;
}

static struct slab *kmem_cache_grow(struct kmem_cache *cache)
{
    struct slab *slab = alloc_pages(0);

    if (!slab)
        return NULL;
    slab->free_bitmap[0] = 0;
    slab->free_objects = cache->objnum;
    list_add(&slab->list, &cache->slabs_free);

    return slab;
}

void *kmem_cache_alloc(struct kmem_cache *cache, __unused unsigned long flags)
{
    struct slab *slab = NULL;
    void *mem;

    if (list_empty(&cache->slabs_partial)) {
        if (list_empty(&cache->slabs_free)) {
            cache->alloc_fail++;
            slab = kmem_cache_grow(cache);
            if (!slab)
                return NULL;
        } else {
            cache->alloc_succeed++;
            slab = list_first_entry(&cache->slabs_free, struct slab, list);
        }
        list_move(&slab->list, &cache->slabs_partial);
    } else {
        cache->alloc_succeed++;
        slab = list_first_entry(&cache->slabs_partial, struct slab, list);
    }

    int bit = find_first_zero_bit(slab->free_bitmap, cache->objnum);
    bitmap_set_bit(slab->free_bitmap, bit);
    mem = slab->data + bit * cache->objsize;
    slab->free_objects--;
    if (!slab->free_objects)
        list_move(&slab->list, &cache->slabs_full);

    return mem;
}

static int slab_destroy(__unused struct kmem_cache *cache, struct slab *slab)
{
    list_del(&slab->list);
    free_pages((unsigned long) slab, 0);

    return 0;
}

void kmem_cache_free(struct kmem_cache *cache, void *obj)
{
    struct slab *slab;
    int bit;

    slab = get_slab_from_obj(obj, 256);
    bit = obj_index_in_slab(obj, cache);
    bitmap_clear_bit(slab->free_bitmap, bit);
    slab->free_objects++;
    if (slab->free_objects == cache->objnum)
        slab_destroy(cache, slab);
    else if (slab->free_objects == 1)
        list_move(&slab->list, &cache->slabs_partial);
}

void kmem_cache_init(void)
{
    list_add(&cache_caches.list, &caches);
    kmem_cache_grow(&cache_caches);
}
