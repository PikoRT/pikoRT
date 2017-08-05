#ifndef _KERNEL_MM_PAGE_H
#define _KERNEL_MM_PAGE_H

#define MAX_PAGE_ORDER 3  /* max page size is 2 KB */
#define MIN_PAGE_SIZE 256 /* min page size is 256 bytes */

void *alloc_pages(unsigned long order);
void free_pages(unsigned long addr, unsigned long order);
void show_page_bitmap(void);
long size_to_page_order(unsigned long size);
unsigned long page_alloc_signature(void);

#endif /* !_KERNEL_MM_PAGE_H */
