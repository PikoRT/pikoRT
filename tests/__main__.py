import argparse
import tests.runner as runner


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('test', nargs='*')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')
    parser.add_argument('--debug', action='store_true', help='Run test and wait for gdb connect')

    args = parser.parse_args()
    if args.test:
        runner.run_test(args.test, args.verbose, debug=args.debug)
    else:
        runner.main(args.verbose)
