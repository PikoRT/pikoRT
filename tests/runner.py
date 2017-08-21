import glob
import subprocess
from datetime import datetime
from re import search
from time import strftime


# FIXME: this can only detect one digit (e.g. not include fs_77)
def find_all_tests(exclude=[]):
    tests = list(map(lambda p: p.strip('/').split('/')[-1], glob.glob('tests/*_[0-9]')))
    for test in exclude:
        if test in tests:
            tests.remove(test)
    return tests


testsuite_v7m = find_all_tests(exclude=['test_2'])


def print_qemu_version():
    cmd = ["qemu-system-arm", "--version"]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    print("QEMU version:\n" + res.stdout)


def print_gcc_version():
    cmd = ["arm-none-eabi-gcc", "--version"]
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
