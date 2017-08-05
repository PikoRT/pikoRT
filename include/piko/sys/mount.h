#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

int mount(const char *source,
          const char *target,
          const char *filesystemtype,
          unsigned long mountflags,
          const void *data);

#endif /* !_SYS_MOUNT_H */
