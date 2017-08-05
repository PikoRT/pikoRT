#include <unistd.h>

enum sc_varname {
	PAGESIZE,
};

struct sys_param {
	const char *name;
	long value;
};

struct sys_param sys_params[] = {
	{"pagesize", 2048},
	{"clock_tick", 0},  // FIXME: set by systick_init()
};

long sys_sysconf(int name)
{
	switch (name) {
		case PAGESIZE:
			return sys_params[0].value;
		case _SC_CLK_TCK:
			return sys_params[1].value;
	}

	return -1;
}
