#include <sys/types.h>

extern int _open(const char *pathname, int flag);
extern int _close(int fd);
extern int _read(int fd, void *buf, size_t count);
extern int _write(int fd, void *buf, size_t count);
extern int _lseek(int fd, off_t offset, int whence);

int open(const char *pathname, int flag)
{
    return _open(pathname, flag);
}

int close(int fd)
{
    return _close(fd);
}

int read(int fd, void *buf, size_t count)
{
    return _read(fd, buf, count);
}

int write(int fd, void *buf, size_t count)
{
    return _write(fd, buf, count);
}

int lseek(int fd, off_t offset, int whence)
{
    return _lseek(fd, offset, whence);
}
