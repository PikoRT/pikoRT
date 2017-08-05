/* syscall wrappers */

#include <sys/stat.h>
#include <sys/types.h>

#include <kernel/fs.h>
#include <kernel/syscalls.h>
#include "piko/syscalls.h"

int _open(const char *pathname, int flags)
{
	return do_syscall2((void *) pathname, (void *) flags, SYS_OPEN);
}

int _close(int fd)
{
	return do_syscall1((void *) fd, SYS_CLOSE);
}

ssize_t _read(int fd, void *buf, size_t count)
{
	return (ssize_t) do_syscall3((void *) fd, buf, (void *) count, SYS_READ);
}

ssize_t _write(int fd, void *buf, size_t count)
{
	return (ssize_t) do_syscall3((void *) fd, buf, (void *) count, SYS_WRITE);
}

off_t _lseek(int fd, off_t offset, int whence)
{
	return (off_t) do_syscall3((void *) fd, (void *) offset, (void *) whence,
	                           SYS_LSEEK);
}

int stat(const char *pathname, struct stat *buf)
{
	return do_syscall2((void *) pathname, (void *) buf, SYS_STAT);
}

int mount(const char *source,
          const char *target,
          const char *filesystemtype,
          unsigned long mountflags,
          const void *data)
{
	return do_syscall5((void *) source, (void *) target,
	                   (void *) filesystemtype, (void *) mountflags,
	                   (void *) data, SYS_MOUNT);
}

DIR *opendir(const char *name)
{
	return (DIR *) do_syscall2((void *) name, (void *) O_DIRECTORY, SYS_OPEN);
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
	return do_syscall3((void *) dirp, (void *) entry, (void *) result,
	                   SYS_READDIR_R);
}

int closedir(DIR *dirp)
{
	return do_syscall1((void *) dirp, SYS_CLOSE);
}
