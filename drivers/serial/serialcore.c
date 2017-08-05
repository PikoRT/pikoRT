#include <sys/types.h>

#include <kernel/serial.h>

void serial_activity_callback(struct serial_info *serial)
{
	if (serial->callback)
		serial->callback(serial);
}

int serial_getc(struct serial_info *serial, char *c)
{
	return serial->serial_getc(serial, c);
}

int serial_gets(struct serial_info *serial,
                size_t len,
                size_t *retlen,
                char *buf)
{
	return serial->serial_gets(serial, len, retlen, buf);
}

int serial_putc(struct serial_info *serial, char c)
{
	return serial->serial_putc(serial, c);
}

int serial_puts(struct serial_info *serial,
                size_t len,
                size_t *retlen,
                const char *buf)
{
	return serial->serial_puts(serial, len, retlen, buf);
}
