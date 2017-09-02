# Helper for generate POSIX API table
# XXX: used from docs, not here

import glob
import os
import tempfile
import shutil
import subprocess

CSCOPE = shutil.which('cscope') if shutil.which('cscope') else 'posix/cscope'
CSCOPE_DB = tempfile.NamedTemporaryFile()
BACKTO = '../'
LIBC_PATH = BACKTO + 'libc'


def read_file(path):
    # Return list of API
    return open(path).read().replace(' ', '').replace('\n', '').strip().split(',')


def build_cscope():
    CSCOPE_DB.write(subprocess.check_output("find %s -name '*.c'" % LIBC_PATH, shell=True))
    CSCOPE_DB.write(subprocess.check_output("find %s -name '*.h'" % LIBC_PATH, shell=True))
    CSCOPE_DB.write(subprocess.check_output("find %s -name '*.S'" % LIBC_PATH, shell=True))
    CSCOPE_DB.flush()
    return CSCOPE_DB.name


def cscope_search(name):
    return subprocess.check_output("%s -i %s -L1%s" % (CSCOPE, CSCOPE_DB.name, name), shell=True)


def parse_path(s):
    return s.split('\n')[0].strip(BACKTO).split()[0:3:2] if s and s.startswith(LIBC_PATH) else ('', 0)


def generate_table(name):
    header = ['.. table:: %s' % os.path.basename(name), '   :widths: 45 10 35', '']
    longest_api_length = 10

    apis = read_file(name)
    result = []

    for api in apis:
        if not api:
            continue
        longest_api_length = max(len(api), longest_api_length)

        s = cscope_search(api.strip('()')).decode('utf-8')
        result.append([api, s.startswith(LIBC_PATH), *parse_path(s)])

    splitline = '   {:=>{}} ===== {}'.format('', longest_api_length, '=' * 30)
    header.append(splitline)
    header.append('   {:<{}} IMPL  Path'.format('API Name', longest_api_length))
    header.append(splitline)

    for i, r in enumerate(result):
        result[i] = ('   {:<{}} {}'.format(r[0], longest_api_length, 'O' if r[1] else 'X') +
                     '     {}'.format('{}:{}'.format(r[2], r[3]) if r[1] else ''))
    result.append(splitline)
    result = header + result

    return '\n'.join(result)


if __name__ == '__main__':
    build_cscope()

    reqs = glob.glob('posix/posix_req/*')
    with open('posix/posix_req.rst', 'w') as f:
        for req in reqs:
            f.write(generate_table(req))
            f.write('\n\n')

    opts = glob.glob('posix/posix_opt/*')
    with open('posix/posix_opt.rst', 'w') as f:
        for opt in opts:
            f.write(generate_table(opt))
            f.write('\n\n')
