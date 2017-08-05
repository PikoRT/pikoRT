#include <stddef.h>

#include <kernel/mm/page.h>
#include "unit.h"

int main()
{
	void *p, *q;
	unsigned long order = size_to_page_order(256);

	if ((p = alloc_pages(order)) == NULL)
		TEST_EXIT(1);
	free_pages((unsigned long) p, order);

	/* same page should be reallocated */
	if ((q = alloc_pages(order)) != p)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
