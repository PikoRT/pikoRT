#ifndef SYS_RESOURCE_H
#define SYS_RESOURCE_H

/* http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_resource.h.html
 */

/* Unsigned integer type used for limit values. */
typedef unsigned int rlim_t;

struct rlimit {
    rlim_t rlim_cur; /* Soft limit */
    rlim_t rlim_max; /* Hard limit (ceiling for rlim_cur) */
};

#define RLIMIT_STACK 0

#ifdef __KERNEL__

int sys_getrlimit(int resource, struct rlimit *rlim);

int sys_setrlimit(int resource, const struct rlimit *rlim);

#else /* !__KERNEL__ */

int getrlimit(int resource, struct rlimit *rlim);

int setrlimit(int resource, const struct rlimit *rlim);

#endif /* __KERNEL__ */

#endif /* !SYS_RESOURCE_H */
