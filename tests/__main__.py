import sys
if sys.version_info < (3, 5):
    print ("Only support python version >= 3.5")
    exit(1)

import argparse
import tests.runner as runner
from tests.runner import PikoTest


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('test', nargs='*')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')
    parser.add_argument('--quiet', action='store_true', default=False,
                        help='no output unless one or more tests fail')
    parser.add_argument('--debug', action='store_true', help='Run test and wait for gdb connect')
    parser.add_argument('-p', '--platform', default='stm32p103')
    parser.add_argument('--qemu', default='qemu-system-arm')
    parser.add_argument('--cc', default='arm-none-eabi-gcc')
    parser.add_argument('--timeout', type=int, default=60)
    parser.add_argument('--slowest', action='store_true', dest='print_slow',
                        help='print the slowest 10 tests')

    ns = parser.parse_args()
    if ns.test:
        PikoTest().main(ns.test, ns)
    else:
        # This will include all test
        runner.main(ns)
