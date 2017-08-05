#include <kernel/bitops.h>

#include "unit.h"

int main()
{
	unsigned long j;
	unsigned long k[2] = {0, 1};

	/* test first bit set */
	for (unsigned long i = 0; i < 32; i++) {
		j = 1 << i;
		if (find_first_bit(&j, 32) != i)
			TEST_EXIT(1);
	}

	/* test first zero bit */
	for (unsigned long i = 0; i < 32; i++) {
		j = ~(1 << i);
		if (find_first_zero_bit(&j, 32) != i)
			TEST_EXIT(1);
	}

	/* test boundaries */
	if (find_first_bit(k, 32) != 32)
		TEST_EXIT(1);
	if (find_first_bit(k, 33) != 32)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
