import datetime
import glob
import locale
import subprocess
import sys
import time
import textwrap
import platform
import os
from time import strftime

# Test result constants.
PASSED = 1
FAILED = 0
INTERRUPTED = -4
CHILD_ERROR = -5   # error in a child process

_FORMAT_TEST_RESULT = {
    PASSED: '%s passed',
    FAILED: '%s failed',
    INTERRUPTED: '%s interrupted',
    CHILD_ERROR: '%s crashed',
}


def find_all_tests(excludes=[]):
    tests = list(map(lambda p: p.strip('/').split('/')[-1], glob.glob('tests/*_[0-9]*')))
    for exclude in excludes:
        if exclude in tests:
            tests.remove(exclude)
    return tests


testsuite_v7m = find_all_tests(excludes=['test_2'])


class PikoTest:
    """Execute Piko/RT test suite.
    """

    def __init__(self):
        # Namespace of command line options
        self.ns = None

        # tests
        self.tests = []
        self.selected = []

        # test results
        self.good = []
        self.bad = []
        self.interrupted = False

        # used by --slow
        self.test_times = []

        # used to display the progress bar "[ 4/100]"
        self.start_time = time.monotonic()
        self.test_count = ''
        self.test_count_width = 1

    def format_test_result(self, test_name, result):
        fmt = _FORMAT_TEST_RESULT.get(result, "%s")
        return fmt % test_name

    def format_duration(self, seconds):
        if seconds < 1.0:
            return '%.0f ms' % (seconds * 1e3)
        if seconds < 60.0:
            return '%.0f sec' % seconds

        minutes, seconds = divmod(seconds, 60.0)
        return '%.0f min %.0f sec' % (minutes, seconds)

    def display_progress(self, test_index, test):
        if self.ns.quiet:
            return

        # "[ 51/405/1] test_tcl passed"
        if self.bad:
            fmt = "{time} [{test_index:{count_width}}{test_count}/{nbad}] {test_name}"
        else:
            fmt = "{time} [{test_index:{count_width}}{test_count}] {test_name}"
        test_time = time.monotonic() - self.start_time
        test_time = datetime.timedelta(seconds=int(test_time))
        line = fmt.format(count_width=self.test_count_width,
                          test_index=test_index,
                          test_count=self.test_count,
                          nbad=len(self.bad),
                          test_name=test,
                          time=test_time)
        print(line, flush=True)

    def display_result(self):
        if self.interrupted:
            print()
            print("Test suite interrupted by signla SIGINT.")
            executed = set(self.good) | set(self.bad)
            omitted = set(self.selected) - executed
            print(count(len(omitted), "test"), "omitted:")
            printlist(omitted)

        if self.good and not self.ns.quiet:
            if (not self.bad
                and not self.interrupted
                and len(self.good) > 1):
                print("All", end=' ')
            print(count(len(self.good), "test"), "OK.")

        if self.ns.print_slow:
            self.test_times.sort(reverse=True)
            print()
            print("10 slowest tests:")
            for t, test in self.test_times[:10]:
                print("- %s: %s" % (test, self.format_duration(t)))

        if self.bad:
            print()
            print(count(len(self.bad), "test"), "failed:")
            printlist(self.bad)

    def display_header(self):
        print('== Welcome to Piko/RT test suite')
        print("==", platform.python_implementation(), *sys.version.split())
        print("==", platform.platform(aliased=True),
              "%s-endian" % sys.byteorder)
        print("== cwd:", os.getcwd())
        cpu_count = os.cpu_count()
        if cpu_count:
            print("== CPU count:", cpu_count)
        print("== encodings: locale=%s, FS=%s"
              % (locale.getpreferredencoding(False),
                 sys.getfilesystemencoding()))
        print()
        print_qemu_version()
        print_gcc_version()

    def accumulate_result(self, test, result):
        ok, test_time = result
        if ok not in (CHILD_ERROR, INTERRUPTED):
            self.test_times.append((test_time, test))
        if ok == PASSED:
            self.good.append(test)
        elif ok in (FAILED, CHILD_ERROR):
            self.bad.append(test)
        elif ok != INTERRUPTED:
            raise ValueError("invalid test result: %r" % ok)

    def run_tests(self):
        self.display_header()

        self.test_count = '/{}'.format(len(self.selected))
        self.test_count_width = len(self.test_count) - 1
        self.run_tests_sequential()

    def run_tests_sequential(self):
        print('Run tests sequentially')
        previous_test = None
        for test_index, test in enumerate(self.tests, 1):
            start_time = time.monotonic()

            text = test
            if previous_test:
                text = '%s -- %s' % (text, previous_test)
            self.display_progress(test_index, text)

            try:
                result = run_single_test(test, self.ns)
            except KeyboardInterrupt:
                self.interrupted = True
                self.accumulate_result(test, (INTERRUPTED, None))
                break
            else:
                self.accumulate_result(test, result)

            previous_test = self.format_test_result(test, result[0])
            test_time = time.monotonic() - start_time
            if test_time >= 15.0:
                previous_test = "%s in %s" % (previous_test, self.format_duration(test_time))
            elif result[0] == PASSED:
                previous_test = None

        if previous_test:
            print(previous_test)

    def finalize(self):
        print()
        duration = time.monotonic() - self.start_time
        print("Total duration: %s" % (self.format_duration(duration)))

        if self.bad:
            result = "FAILURE"
        elif self.interrupted:
            result = "INTERRUPTED"
        else:
            result = "SUCCESS"
        print("Tests result: %s" % result)

    def main(self, tests, ns):
        self.ns = ns
        self.tests = tests
        self.selected = tests

        self.run_tests()
        self.display_result()

        self.finalize()
        if self.bad:
            sys.exit(2)
        if self.interrupted:
            sys.exit(130)
        sys.exit(0)


def print_qemu_version():
    cmd = ["qemu-system-arm", "--version"]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.DEVNULL)
    print()
    print("QEMU version\n", res.stdout.strip())
    print()


def print_gcc_version():
    cmd = ["arm-none-eabi-gcc", "--version"]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.DEVNULL)
    print()
    print("GCC version:\n" + res.stdout.strip())
    print()


def print_header(testname, arch):
    print("--------------------------------------------")
    print("running test:  \033[1;37m%s\033[0m" % testname)
    print("arch        :  %s" % arch)
    print("time        :  %s\n" % strftime("%c"))


def run_single_test(test, ns):
    """Run a single test.

    ns -- parser namespace of options
    test -- the name of the test

    Returns the tuple (result, test_time), where result is one of the
    constants:

        INTERRUPTED     KeyboardInterrupt
        FAILED          test failed
        PASS            test passed
    """

    cmd = ['make', '--file', 'tests/Makefile', 'clean_test', 'all']
    cmd.append('dbg' if ns.debug else 'run')
    cmd.append('TEST=%s' % (test))

    if ns.platform:
        cmd.append('PLAT=%s' % (ns.platform))

    test_time = 0.0

    if ns.verbose:
        print(' '.join(cmd))
    try:
        start_time = time.time()
        p = subprocess.Popen(cmd, universal_newlines=True,
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output, error = p.communicate(timeout=ns.timeout)
        test_time = time.time() - start_time
        if ns.verbose:
            print(output)
    except KeyboardInterrupt:
        p.kill()
        output, error = p.communicate()
        if ns.verbose:
            print(output)
        raise
    except subprocess.TimeoutExpired:
        p.kill()
        output, error = p.communicate()
        if ns.verbose:
            print(output)
        return FAILED, test_time

    if p.returncode != 0:
        if ns.verbose:
            print(output)
        return FAILED, test_time

    return PASSED, test_time


def main(ns):
    PikoTest().main(testsuite_v7m, ns)


def printlist(x, width=70, indent=4, file=None):
    """Print the elements of iterable x to stdout.

    Optional arg width (default 70) is the maximum line length.
    Optional arg indent (default 4) is the number of blanks with which to
    begin each line.
    """

    blanks = ' ' * indent
    # Print the sorted list: 'x' may be a '--random' list or a set()
    print(textwrap.fill(' '.join(str(elt) for elt in sorted(x)), width,
                        initial_indent=blanks, subsequent_indent=blanks),
          file=file)


def count(n, word):
    if n == 1:
        return "%d %s" % (n, word)
    else:
        return "%d %ss" % (n, word)
