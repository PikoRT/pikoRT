#include <stddef.h>

#include <kernel/bitops.h>
#include <kernel/hash.h>
#include <kernel/kernel.h>
#include <kernel/log2.h>
#include <kernel/mm/page.h>

/* .pgmem section is 32KB:
 *      - 128 pages of 256B is a 16 bytes map
 *      -  64 pages of 512B is a  8 bytes map
 *      -  32 pages of 1KB is a  4 bytes map
 *      -  16 pages of 2KB is a  2 bytes map
 *
 * Last page is reserved by the early stack, and freed after system init.
 */

/* 0 = allocated/undefined, 1 = free */
static unsigned long *const page_bitmap[] = {
    (unsigned long[]){0, 0, 0, 0}, (unsigned long[]){0, 0},
    (unsigned long[]){0}, (unsigned long[]){0x7fff}};

static const unsigned long page_bitmap_sz[] = {128, 64, 32, 16};

long size_to_page_order(unsigned long size)
{
    if (size <= 256)
        return 0;
    if (size <= 512)
        return 1;
    if (size <= 1024)
        return 2;
    if (size <= 2048)
        return 3;

    return -1;
}

static long find_first_free_page(unsigned long order)
{
    unsigned long page_idx =
        find_first_bit(page_bitmap[order], page_bitmap_sz[order]);

    if (page_idx >= page_bitmap_sz[order])
        return -1;

    return page_idx;
}

static void split_first_free_page(unsigned long order)
{
    unsigned long page_idx =
        find_first_bit(page_bitmap[order], page_bitmap_sz[order]);

    bitmap_clear_bit(page_bitmap[order], page_idx);
    bitmap_set_bit(page_bitmap[order - 1], page_idx * 2);
    bitmap_set_bit(page_bitmap[order - 1], page_idx * 2 + 1);
}

extern char __pgmem_start__;

static void *page_idx_to_addr(unsigned long idx, unsigned long order)
{
    return &__pgmem_start__ + idx * (1 << (order + ilog2(MIN_PAGE_SIZE)));
}

void *alloc_pages(unsigned long order)
{
    unsigned long page_idx, o;

    for (o = order; (o <= MAX_PAGE_ORDER) && (find_first_free_page(o) < 0); o++)
        ;
    if (o > MAX_PAGE_ORDER)
        return NULL;
    for (; o > order; o--)
        split_first_free_page(o);
    page_idx = find_first_bit(page_bitmap[order], page_bitmap_sz[order]);
    bitmap_clear_bit(page_bitmap[order], page_idx);

    /* printk("Returning address %p\n", page_idx_to_addr(page_idx, order)); */

    return page_idx_to_addr(page_idx, order);
}

static inline unsigned long addr_to_page_idx(unsigned long addr,
                                             unsigned long order)
{
    // FIXME: multiple zone to allocate from? use the zone's base address
    // instead
    return (addr - (unsigned long) &__pgmem_start__) >>
           (order + ilog2(MIN_PAGE_SIZE));
}

static inline unsigned long get_buddy_index(unsigned long idx)
{
    return idx % 2 ? idx - 1 : idx + 1;
}

/* try to coalesce free buddies */
void free_pages(unsigned long addr, unsigned long order)
{
    unsigned long page_idx, buddy_idx, mask;

    for (; order < MAX_PAGE_ORDER; order++) {
        page_idx = addr_to_page_idx(addr, order);
        buddy_idx = get_buddy_index(page_idx);
        if (bitmap_get_bit(page_bitmap[order], buddy_idx)) {
            bitmap_clear_bit(page_bitmap[order], buddy_idx);
            mask = ~((1 << (order + 1 + ilog2(MIN_PAGE_SIZE))) - 1);
            addr &= mask;
        } else {
            bitmap_set_bit(page_bitmap[order], page_idx);
            return;
        }
    }
    bitmap_set_bit(page_bitmap[order], addr_to_page_idx(addr, order));
}

void show_page_bitmap(void)
{
    printk("Order  Bitmap\n");
    for (int i = 0; i <= 3; i++) {
        printk("    %d  ", i);
        for (unsigned long j = 0; j < page_bitmap_sz[i]; j += BITS_PER_LONG)
            printk("%08x  ", *(page_bitmap[i] + j / BITS_PER_LONG));
        printk("\n");
    }
}

/* Useful function to get a signature of memory fragmentation before and
 * after allocating/freeing memory. */
unsigned long page_alloc_signature(void)
{
    unsigned long hash = hash_djb2((unsigned char *) page_bitmap[0], 4 * 4) +
                         hash_djb2((unsigned char *) page_bitmap[1], 2 * 4) +
                         hash_djb2((unsigned char *) page_bitmap[2], 1 * 4) +
                         hash_djb2((unsigned char *) page_bitmap[3], 1 * 4);

    return hash;
}
