# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Stuff related to et-dvrun-status
"""

#
# NOTES: from the makefile/
# find . -maxdepth 5 -name status
# ./minion_shire/neigh/build/status
# ./minion_shire/neigh/tests/sting_regress_1c_rv32i_only_arith_lw/status
# very predictable depth
#
# To be consistent with previous: not using the incr for now
#
import sys, os, time, glob, re
from functools import reduce
from ETdv.timer import Timer
from ETdv.colortext import ColorText
from ETdv.util import (eprint, flock, read_file)
from ETdv.ostream import OStream

TYPES = ['builds', 'tests']
SVR = ['fail', 'unknown', 'pass']
LOCK_FNAME = '.status_lock'


def _set_lock(rootdir='.', fname=LOCK_FNAME):
    fqn = os.path.realpath(os.path.join(rootdir, fname))
    flock(fqn)
    return fqn


def _rm_lock(fqn):
    if os.path.exists(fqn):
        try:
            os.remove(fqn)
        except:
            pass


__ISTATUS = '.istatus'


def get_istatus_fname(dir=None):
    if dir is None: return __ISTATUS
    return os.path.join(dir, __ISTATUS)


def write_istatus(dir, status=None, n=None):
    if status is None:
        status = {
            'builds': {'n': 0, 'pass': 0, 'fail': 0, 'unknown': 0},
            'tests': {'n': 0, 'pass': 0, 'fail': 0, 'unknown': 0}
        }
    if n is not None:
        for k, v in n.items(): status[k]['n'] = v
    with OStream(open(get_istatus_fname(dir), 'w+t')) as ofs:
        ofs.println('#type:n,pass,fail,unknown')
        for type in status.keys():
            r = status[type]
            ofs.println(f'{type}:{r["n"]},{r["pass"]},{r["fail"]},{r["unknown"]}')


def read_istatus(dir):
    istatus = {}
    with open(get_istatus_fname(dir), 'r') as ifs:
        for line in ifs:
            line = line.strip()
            if 1 > len(line) or line.startswith('#'): continue
            m = re.match(r'(\w+):(\d+),(\d+),(\d+),(\d+)', line, re.IGNORECASE)
            assert m, "expect match"
            istatus[m.group(1)] = {
                'n': m.group(2), 'pass': int(m.group(3)), 'fail': int(m.group(4)), 'unknown': int(m.group(5))}
    return istatus


def get_status(rootdir='.', get_details=False, do_lock=False):
    if do_lock: lockfn = _set_lock(rootdir)
    details, files, counts = {}, {}, {}
    if get_details:
        for svr in SVR: details[svr] = []
    for type in TYPES:
        if type == 'builds':
            dir = 'build'
            files[type] = glob.glob(os.path.join(rootdir, '*', '*', dir, 'status'), recursive=False)
        else:
            dir = type
            files[type] = glob.glob(os.path.join(rootdir, '*', '*', dir, '*', 'status'), recursive=False)
        counts[type] = {}
        for svr in SVR: counts[type][svr] = 0
    for type, files in files.items():
        for fname in files:
            with open(fname) as ifs:
                svr = ifs.read().strip().lower()
                # we'll allow other failxxx as well
                if re.match(r'fail', svr): svr = 'fail'
            if 0 == len(svr) or svr not in SVR: svr = 'unknown'
            counts[type][svr] += 1
            if get_details:
                details[svr].append(fname)
    if do_lock: _rm_lock(lockfn)
    if get_details:
        return counts, details
    else:
        return counts


def get_builds(rootdir='.'):
    return glob.glob(os.path.join(rootdir, '*', '*', 'build', 'buildme'), recursive=False)


def get_tests(rootdir='.'):
    return glob.glob(os.path.join(rootdir, '*', '*', 'tests', '*', 'testme'), recursive=False)


def get_not_run(rootdir='.'):
    not_run = []
    for fname in get_builds(rootdir) + get_tests(rootdir):
        tryme = os.path.join(os.path.dirname(fname), 'status')
        if not os.path.exists(tryme):
            tryme = f'{fname}.log'
            if not os.path.exists(tryme):
                not_run.append(fname)
    return not_run


def get_running(rootdir='.'):
    running = []
    for fname in get_builds(rootdir) + get_tests(rootdir):
        tryme = os.path.join(os.path.dirname(fname), 'status')
        if not os.path.exists(tryme):
            tryme = f'{fname}.log'
            if os.path.exists(tryme):
                running.append(fname)
    return running


def get_nbuild_ntest(rootdir='.'):
    return len(get_builds(rootdir)), len(get_tests(rootdir))


def print_status(status, args=None, rootdir='.'):
    if rootdir is not None:
        nbuild, ntest = get_nbuild_ntest(rootdir=rootdir)
        total = {'builds': nbuild, 'tests': ntest}
    else:
        total = {'builds': status['builds']['n'], 'tests': status['tests']['n']}
    if args is None: args = [str(time.time())]
    started = args[0]
    elapsed = Timer(float(started)).elapsed()
    text = ''
    for type in TYPES:
        text += f'| {type}({total[type]}) '
        vals = []
        for svr in SVR:
            cnt = status[type][svr]
            if 0 < cnt: vals.append(f'{svr}:{cnt}')
        text += '; '.join(vals) + ' '
    text += ' |'
    if re.match(r'.*(fail|unknown)', text):
        color, svr = ColorText.red, 'E'
    else:
        color, svr = ColorText.green, 'I'
    text = f'{svr}-{elapsed}: {text}  (RunStatus-1)'
    print(color(text))


def print_progress(rootdir, details):
    for svr in SVR:
        for fname in details[svr]:
            if svr == 'fail':
                with open(fname) as ifs:
                    detail = re.sub(r'\n+', '', ifs.read()[5:])
                    detail = f": {detail}"
            else:
                detail = ''
            print(f'{svr}: {os.path.abspath(fname)}{detail}')
    for fname in get_running(rootdir):
        print(f'running: {os.path.abspath(fname)}')
    for fname in get_not_run(rootdir):
        print(f'not_run: {os.path.abspath(fname)}')


USAGE = """
Usage: et-dvrun-status [-h|--help] <subcommand> <options>...

subcommand    description
----------    -----------
progress      Report status of each build and test.
              Status is one of: pass, fail, unknown, running, not_run.
               
              options
              -------
              <makefile_dir> : makefile_dir is path to makefile directory 
                               generated by et-dvrun.
              
Example:

et-dvrun-status progress /path/nightly/makefile

Report current progress on runs started from command:
   
   et-dvrun --runroot /path/nightly/makefile --config ...
"""


def _usage():
    eprint(USAGE)
    sys.exit(1)


def _error(msg, code=1):
    eprint(f'Error: {msg}.')
    sys.exit(code)


def _incr(argv):
    started, root, status = argv
    istatus = read_istatus(root)
    if status[0] == '/':
        # for file, lets update status
        is_build = '/build/status' in status
        if not is_build: assert '/tests/' in status
        type = (is_build and 'builds') or 'tests'
        signature = read_file(status)
        if signature is None:
            key = 'unknown'
        elif signature == 'pass':
            key = 'pass'
        else:
            key = 'fail'
        istatus[type][key] += 1
        write_istatus(root, status=istatus)
    print_status(status=istatus, rootdir=None, args=argv)


def main(argv=sys.argv):
    argc = len(argv)
    if 2 > argc or reduce(lambda a, b: a or b, map(lambda a: a in ['-h', '--help'], sys.argv)):
        _usage()
    subcmd = argv[1]
    if subcmd == 'now':  # not user
        print(time.time())
    elif subcmd == 'status':  # not user
        if 3 < argc:
            rdir = argv[3]
        else:
            rdir = '.'
        status = get_status(rootdir=rdir)
        print_status(status, argv[2:], rootdir=rdir)
    elif subcmd == 'incr':  # not user
        _incr(argv[2:])
    elif subcmd == 'progress':
        if 2 < argc:
            rootdir = os.path.abspath(argv[2])
            if os.path.basename(rootdir) != 'makefile':
                _error(f"'{argv[2]}': <makefile_dir> must end in 'makefile'")
        else:
            rootdir = '.'
        if not os.path.exists(rootdir):
            _error(f"'{rootdir}': <makefile_dir> does not exist")
        status, details = get_status(rootdir, get_details=True)
        print_status(status, rootdir=rootdir)
        print_progress(rootdir, details)
    else:
        _error(f"'{subcmd}': invalid subcommand")
    exit(0)
