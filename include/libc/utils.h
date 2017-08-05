#ifndef UTILS_H
#define UTILS_H

#ifndef __LINKER__

static inline void infinite_loop(void)
{
	for (;;)
		;
}

void strpad(char *buf, char pad_val, int count);
char *itoa_base(int value, char *buf, int base);

#endif /* !__LINKER__ */

#endif /* !UTILS_H */
