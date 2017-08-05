#include <stdbool.h>

#include <kernel/kernel.h>
#include <kernel/types.h>

#include "linux/types.h"
#include "linux/list.h"

/* This is a boundary tag, located at the beginning of the block wether it's
 * free or allocated.    */
struct malloc_tag {
	/* composite field:
	 *   -  bit 31       0 = block not free, 1 = block free
	 *   -  bit 30..0    full block length, this tag comprised
	 */
	u32 free__length;
	struct list_head list;
	char data[0];
};

#define BLOCK_FREE_MASK (1 << 30)
#define BLOCK_LENGTH_MASK (~(1 << 30))

static LIST_HEAD(blocks);

/* helper functions */

static inline size_t get_block_length(struct malloc_tag *block)
{
	return block->free__length & BLOCK_LENGTH_MASK;
}

static inline void set_block_length(struct malloc_tag *block, size_t len)
{
	block->free__length = (block->free__length & BLOCK_FREE_MASK) | len;
}

static inline bool get_block_free(struct malloc_tag *block)
{
	return block->free__length & BLOCK_FREE_MASK ? true : false;
}

static inline void set_block_free(struct malloc_tag *block, bool free)
{
	if (free)
		block->free__length |= BLOCK_FREE_MASK;
	else
		block->free__length &= BLOCK_LENGTH_MASK;
}

void kernel_heap_init(void *heap_start, size_t heap_size)
{
	struct malloc_tag *first_block = heap_start;

	first_block->free__length = BLOCK_FREE_MASK | heap_size;
	list_add(&first_block->list, &blocks);
}

void *malloc(size_t size)
{
	struct malloc_tag *free_block, *new_block;

	/* allocation size is a multiple of 4-byte aligned, plus size of tag */
	size = align_next(size, 4) + sizeof(struct malloc_tag);

	/* find a free block wich is large enough to fullfill the memory requirement
	 */
	list_for_each_entry(free_block, &blocks, list) {
		if (get_block_free(free_block) &&
		    (get_block_length(free_block) >= size)) {
			if ((get_block_length(free_block) - size) >
			    sizeof(struct malloc_tag)) {
				set_block_length(free_block,
				                 get_block_length(free_block) - size);
				new_block =
				    (struct malloc_tag *) ((u32) free_block +
				                           get_block_length(free_block));
				set_block_free(new_block, false);
				set_block_length(new_block, size);
				list_add(&new_block->list, &free_block->list);
				return new_block->data;
			} else {
				set_block_free(free_block, false);
				return free_block->data;
			}
		}
	}

	return NULL;
}

void free(void *ptr)
{
	struct malloc_tag *block = container_of(ptr, struct malloc_tag, data);
	struct malloc_tag *prev_block, *next_block;

	set_block_free(block, true);

	/* merge with previous block if free */
	if (block->list.prev != &blocks) {
		prev_block = list_prev_entry(block, list);
		if (get_block_free(prev_block)) {
			set_block_length(prev_block, get_block_length(prev_block) +
			                 get_block_length(block));
			list_del(&block->list);
			block = prev_block;
		}
	}

	/* merge with next block if free */
	if (block->list.next != &blocks) {
		next_block = list_next_entry(block, list);
		if (get_block_free(next_block)) {
			set_block_length(
			    block, get_block_length(block) + get_block_length(next_block));
			list_del(&next_block->list);
		}
	}
}
