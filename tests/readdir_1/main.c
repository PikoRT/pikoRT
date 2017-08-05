#include <stddef.h>
#include <string.h>

#include <piko/dirent.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include "unit.h"

int found_dot;
int found_dotdot;
int found_null;
int found_zero;

int main()
{
	struct dirent dirent;
	struct dirent *result;

	DIR *dir = opendir("/dev");
	do {
		readdir_r(dir, &dirent, &result);
		if (result != NULL)
			printk("% 6d %s\n", dirent.d_ino, dirent.d_name);
		if (!strcmp(result->d_name, "."))
			found_dot++;
		if (!strcmp(result->d_name, ".."))
			found_dotdot++;
		if (!strcmp(result->d_name, "zero"))
			found_zero++;
		if (!strcmp(result->d_name, "null"))
			found_null++;
	} while (result != NULL);
	closedir(dir);

	if (found_dot != 1)
		TEST_EXIT(1);
	if (found_dotdot != 1)
		TEST_EXIT(1);
	if (found_zero != 1)
		TEST_EXIT(1);
	if (found_null != 1)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
