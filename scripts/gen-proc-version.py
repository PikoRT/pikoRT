import subprocess
import string
from datetime import datetime
import argparse

def run_cmd(cmd):
    res = subprocess.run(cmd, universal_newlines=True,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    if res.returncode == 0:
        return res.stdout.strip()
    return 'unknown'

def cc_version(cc):
    cmd = [ cc, '--version' ]
    return run_cmd(cmd).splitlines()[0]

parser = argparse.ArgumentParser()

parser.add_argument("-n", "--name", action="store", dest="name")
parser.add_argument("-a", "--arch", action="store", dest="arch")
parser.add_argument("-c", "--cpu", action="store", dest="cpu")
parser.add_argument("-u", "--user", action="store", dest="user", default="unknown")
parser.add_argument(      "--host", action="store", dest="host", default="unknown")
parser.add_argument(      "--major", action="store", dest="major", default=0)
parser.add_argument(      "--minor", action="store", dest="minor", default=0)
parser.add_argument(      "--micro", action="store", dest="micro", default=0)
parser.add_argument(      "--cc-version", action="store_true", dest="cc_version")

args = parser.parse_args()

version = '.'.join(map(lambda x: str(x), [args.major, args.minor, args.micro]))
hostname = '@'.join([args.user, args.host])
platform = ', '.join(filter(lambda x: x is not None, [args.arch, args.cpu]))

print('{} version {} ({}) ({}) #{}'.format(args.name, version, hostname, platform,
                                           datetime.now().strftime("%c")))
if (args.cc_version):
    print(cc_version('arm-none-eabi-gcc'))
