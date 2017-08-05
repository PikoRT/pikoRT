#!/usr/bin/env python

import string
from optparse import OptionParser

xs = [
    # <pthread.h>
    'pthread_exit',
    'pthread_self',
    'pthread_yield',
    'pthread_create',
    'pthread_join',
    'pthread_detach',
    'pthread_mutex_lock',
    'pthread_mutex_unlock',
    'pthread_cond_signal',
    'pthread_cond_wait',

    # <time.h>
    'timer_create',
    'timer_settime',
    'timer_gettime',

    # <unistd.h>
    'msleep',
    'sysconf',

    # <signal.h>
    'sigaction',
    'kill',

    # <fcntl.h>, <unistd.h>, <sys/mount.h>...
    'open',
    'close',
    'read',
    'write',
    'lseek',
    'stat',
    'mount',
    'readdir_r',

    # <task.h>
    'getpid',

    # <sys/mman.h>
    'mmap',
    'munmap',
]

parser = OptionParser()

parser.add_option("--header", action="store_true", dest="header")
parser.add_option("--source", action="store_true", dest="source")

(opts, args) = parser.parse_args()

import datetime
print('// GENERATED. DO NOT EDIT FROM HERE!')
print('// Change definitions in scripts/gen-syscalls.py')
print('// Created on ' +
      datetime.datetime.now().strftime("%Y-%m-%d %H:%M"))

print('')

#for x in list(enumerate(xs)):
#    name = 'sys_' + x[1]
#    print('[{}] = {},'.format(name.upper(), name))

if (opts.header):
    print('#ifndef KERNEL_SYSCALLS_H')
    print('#define KERNEL_SYSCALLS_H')
    for x in list(enumerate(xs)):
        name = 'sys_' + x[1]
        print('#define {} {}'.format(name.upper(), x[0]))
    print('')
    print('#endif /* !KERNEL_SYSCALLS_H */')


if (opts.source):
    print('#include <kernel/syscalls.h>')
    print('')
    for x in list(enumerate(xs)):
        name = 'sys_' + x[1]
        print('int {}();'.format(name))
    print('#define SYS_MAX 48')
    print('')
    print('void *syscall_vect[SYS_MAX] = {')
    for x in list(enumerate(xs)):
        name = 'sys_' + x[1]
        print('    [{}] = {},'.format(name.upper(), name))
    print('};');
    print('')
    print('int syscall_register(unsigned ix, void *(*fn)()) {')
    print('    if (ix >= SYS_MAX) return -1;')
    print('    syscall_vect[ix] = fn;')
    print('return 0;')
    print('}');
