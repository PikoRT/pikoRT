#ifndef LINUX_POISON_H
#define LINUX_POISON_H

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 * Make sure the values raise faults when these addresses are
 * read/written.
 */
#define LIST_POISON1 ((void *) 0x100)
#define LIST_POISON2 ((void *) 0x200)

#endif /* !LINUX_POISON_H */
