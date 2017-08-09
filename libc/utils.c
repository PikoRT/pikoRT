#include <stdlib.h>
#include <string.h>

static char *strrev(char *s)
{
    char *t = s;
    char *u = s;

    if (!s || (*s == '\0'))
        return s;

    /* u points to the last char in the string */
    while (*(u + 1) != '\0')
        u++;

    /* t moves forward, u moves backward */
    while (t < u) {
        char tmp = *t;
        *t++ = *u;
        *u-- = tmp;
    }

    return s;
}

void strpad(char *buf, char pad_val, int count)
{
    buf[count--] = '\0';
    for (; count >= 0; count--)
        buf[count] = pad_val;
}

char *itoa_base(int value, char *buf, int base)
{
    const char base36[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0;
    int isneg = 0;
    unsigned int val = value;

    if (!buf || (base > 36))
        return NULL;

    if ((value < 0) && (base == 10)) {
        val = abs(value);
        isneg = 1;
    }

    do {
        buf[i++] = base36[val % base];
        val /= base;
    } while (val);

    if (isneg)
        buf[i++] = '-';
    buf[i] = '\0';

    return strrev(buf);
}
