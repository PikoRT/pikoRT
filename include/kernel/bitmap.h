#ifndef _LINUX_BITMAP_aAA
#define _LINUX_BITMAP_H

#include "linux/list.h"
#include "kernel/bitops.h"

struct bitmap_struct {
    unsigned long map;
    struct list_head queue[32];
};

static inline void INIT_BITMAP(struct bitmap_struct *bm)
{
    WRITE_ONCE(bm->map, 0UL);
    for (int i = 0; i < 32; i++)
        INIT_LIST_HEAD(&bm->queue[i]);
}

static inline void bitmap_queue_add(struct list_head *new,
                                    unsigned long bit,
                                    struct bitmap_struct *bm)
{
    list_add_tail(new, &bm->queue[bit]);
    bitmap_set_bit(&bm->map, bit);
}

static inline void bitmap_queue_add_first(struct list_head *new,
                                          unsigned long bit,
                                          struct bitmap_struct *bm)
{
    list_add(new, &bm->queue[bit]);
    bitmap_set_bit(&bm->map, bit);
}

static inline void bitmap_enqueue(struct list_head *new,
                                  unsigned long bit,
                                  struct bitmap_struct *bm)
{
    bitmap_queue_add(new, bit, bm);
}

static inline void bitmap_enqueue_first(struct list_head *new,
                                        unsigned long bit,
                                        struct bitmap_struct *bm)
{
    bitmap_queue_add_first(new, bit, bm);
}

static inline int bitmap_empty(const struct bitmap_struct *bm)
{
    return !(!READ_ONCE(bm->map));
}

static inline int bitmap_queue_empty(struct bitmap_struct *bm,
                                     unsigned long bit)
{
    return !bitmap_get_bit(&bm->map, bit);
}

static inline int bitmap_first_bit(const struct bitmap_struct *bm)
{
    return find_first_bit(&bm->map, 32);
}

static inline void bitmap_queue_del(struct list_head *queue,
                                    unsigned long bit,
                                    struct bitmap_struct *bm)
{
    list_del(queue);
    if (list_empty(&bm->queue[bit]))
        bitmap_clear_bit(&bm->map, bit);
}

static inline struct list_head *bitmap_dequeue(struct bitmap_struct *bm,
                                               unsigned long bit)
{
    struct list_head *first = bm->queue[bit].next;

    bitmap_queue_del(first, bit, bm);
    return first;
}

static inline struct list_head *bitmap_dequeue_tail(struct bitmap_struct *bm,
                                                    unsigned long bit)
{
    struct list_head *last = bm->queue[bit].prev;

    bitmap_queue_del(last, bit, bm);
    return last;
}

#define bitmap_first_entry(bm, bit, type, member) \
    list_entry(bm->queue[bit].next, type, member)

#endif
