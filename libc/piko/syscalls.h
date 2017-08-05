/* libc/piko/syscalls.h */

#ifndef LIBC_SYSCALLS_H
#define LIBC_SYSCALLS_H

int do_syscall0(int no);
int do_syscall1(void *a0, int no);
int do_syscall2(void *a0, void *a1, int no);
int do_syscall3(void *a0, void *a1, void *a2, int no);
int do_syscall4(void *a0, void *a1, void *a2, void *a3, int no);
int do_syscall5(void *a0, void *a1, void *a2, void *a3, void *a4, int no);
int do_syscall6(void *a0,
                void *a1,
                void *a2,
                void *a3,
                void *a4,
                void *a5,
                int no);

#endif /* !LIBC_SYSCALLS_H */
