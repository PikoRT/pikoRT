from time import strftime
import subprocess
from datetime import datetime
from re import search
import os

testsuite_v7m = [
    "test_1",
    "bitops_1",
    "thread_1",
    "thread_2",
    "thread_3",
    "thread_4",
    "thread_5",
    "thread_6",
    "msleep_1",
    "msleep_2",
    "mm_1",
    "mm_2",
    "timer_1",
    "timer_2",
    "timer_3",
    "timer_4",
    "timer_5",
    "mutex_1",
    "mutex_2",
    "mutex_3",
    "mutex_4",
    "mutex_5",
    "syscall_1",
    "raise_1",
    "raise_2",
    "raise_3",
    "sysconf_1",
    "itoa_1",
    "sprintf_1",
    "ucontext_1",
    "malloc_1",
    "cond_1",
    "cond_2",
    "cond_3",
    "fs_1",
    "fs_2",
    "fs_3",
    "fs_4",
    "fs_5",
    "fs_6",
    "fs_7",
    "mtdram_1",
    "readdir_1",
    "stat_1",
    "getpid_1",
    "slab_1",
    "slab_2",
    "mmap_1",
    "mmap_2",
    "page_3",
    "softirq_1",
    "softirq_2",
    "softirq_3",
]

def print_qemu_version():
    cmd = [ "qemu-system-arm", "--version" ]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    print("QEMU version:\n" + res.stdout)

def print_gcc_version():
    cmd = [ "arm-none-eabi-gcc", "--version" ]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    print("GCC version:\n" + res.stdout)

def print_header(testname, arch):
    print("--------------------------------------------")
    print("running test:  \033[1;37m%s\033[0m" % testname)
    print("arch        :  %s" % arch)
    print("time        :  %s\n" % strftime("%c"))


def run_single_test(testname, verbose, platform='stm32p103'):
    # platform = os.getenv('PLATFORM', 'qemu')
    cmd = ["make", "TEST=%s" % testname, "TARGET=%s" % platform,
           "--file", "tests/Makefile", "clean_test", "all", "run"]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    if verbose:
        print(' '.join(cmd))
        print(res.stdout)
    # Ubuntu Trusty run an old QEMU, semihosting does not return a code
    # return res.returncode
    if search("test passed", res.stdout) is None:
        return 1
    return 0


def run_test(tests, verbose):
    print_qemu_version()
    print_gcc_version()
    print('Staging %s test' % tests)

    failed_count = 0
    results = dict()
    t0 = datetime.now()
    for test in tests:
        print_header(test, 'ARMv7M')
        status = run_single_test(test, verbose)
        failed_count += status
        results[test] = 'failed' if status else 'ok'

    t = datetime.now()

    print("Test result:")
    for key in sorted(results.keys()):
        print("% 16s:  %s" % (key, results[key]))

    print("\nRan %d tests in %d.%ds" % (len(testsuite_v7m),
                                        (t - t0).seconds, (t - t0).microseconds / 1000))

    if failed_count:
        exit(1)


def main(verbose=True):
    run_test(testsuite_v7m, verbose)


if __name__ == "__main__":
    main()
