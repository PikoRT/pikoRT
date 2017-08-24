#include "platform.h"
#include "py/mphal.h"


int mp_hal_stdin_rx_chr() {
    char c;
    int fd = open("/dev/ttyS0", 0);

    read(fd, &c, 1);
    close(fd);
    return c;
}

void mp_hal_stdout_tx_strn(const char *s, mp_uint_t n) {
    while (n--) {
        __printk_putchar(*s++);
    }
}
