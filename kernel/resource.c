#include <piko/sys/resource.h>

static struct rlimit rlimits[] = {
	{.rlim_cur = 1024, .rlim_max = 1024} /* RLIMIT_STACK */
};

int sys_getrlimit(int resource, struct rlimit *rlim)
{
	rlim->rlim_cur = rlimits[resource].rlim_cur;
	rlim->rlim_max = rlimits[resource].rlim_max;

	return 0;
}

int sys_setrlimit(int resource, const struct rlimit *rlim)
{
	rlimits[resource].rlim_cur = rlim->rlim_cur;
	rlimits[resource].rlim_max = rlim->rlim_max;

	return 0;
}
