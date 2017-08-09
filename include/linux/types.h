#ifndef LINUX_TYPES_H
#define LINUX_TYPES_H

#define S32_MAX 2147483647

typedef unsigned int u32;
typedef int s32;

typedef unsigned short u16;
typedef short s16;

typedef unsigned char u8;
typedef char s8;

typedef unsigned int size_t;

struct list_head {
    struct list_head *next, *prev;
};

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

typedef struct {
    volatile s32 val;
} atomic_t;

#endif /* !LINUX_TYPES_H */
