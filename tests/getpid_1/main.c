/* simple test for getpid */

#include <unistd.h>
#include "unit.h"

int main()
{
	pid_t pid = getpid();

	if (!pid)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
