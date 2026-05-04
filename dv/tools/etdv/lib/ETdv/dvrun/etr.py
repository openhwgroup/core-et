# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Lightweight ui for et-dvrun.
"""
from os import path
from glob import glob
from ETdv.dvrun.util import get_default_runroot, getenv
from ETdv.dvrun.messaging import debug, Messaging, early_termination_error, EarlyTerminationException


class Etr:
    CONFIG_SFX = '.etdv.py'
    DEFAULT_J = 2
    DEFAULT_ALL_TESTS = 9999999999

    def __init__(self, args):
        self.__runroot = args.r or get_default_runroot()
        self.__test_names = args.t or []
        self.__build_names = args.b or []
        self.__configs = args.config
        self.__make_j = args.j
        self.__verbose = args.v
        self.__uargs = args.u or []
        self.__slurm = args.s
        self.__max_tests = args.n
        self.__timeout = args.m or 0
        Messaging(self.__verbose)

    @staticmethod
    def __find_config(*args):
        return glob(path.join(*args), recursive=True)

    def __validate(self):
        actual_configs = []
        for config in self.__configs:
            xconfig = config
            if not config.endswith(self.CONFIG_SFX): xconfig = config + self.CONFIG_SFX
            if '/' in xconfig:
                # assume we are a path to file
                actual_configs.append(xconfig)
            else:
                reporoot = getenv('REPOROOT')
                if reporoot:
                    found_configs = self.__find_config(reporoot, '**', xconfig)
                    n = len(found_configs)
                else:
                    n = 0
                if 0 == n:
                    found_configs = self.__find_config('**', xconfig)
                n = len(found_configs)
                if 0 == n:
                    early_termination_error('Etr-2', config)
                elif 1 < n:
                    early_termination_error('Etr-3', config, found_configs)
                [actual_configs.append(c) for c in found_configs]
        for config in actual_configs:
            if not path.isfile(config):  early_termination_error('File-1', config, 'read')
        self.__configs = actual_configs
        return self

    def __transmogrify(self):
        argv = []

        def append(a, b=None):
            argv.append(a)
            if b is not None: argv.append(b)

        append('--runroot', self.__runroot)
        [append('--config', config) for config in self.__configs]
        [append('--test_name_enable', testname) for testname in self.__test_names]
        [append('--build_name_enable', buildname) for buildname in self.__build_names]
        [append('--uarg', arg) for arg in self.__uargs]
        append('--make')
        append('--timeout', self.__timeout)
        append('-j', self.__make_j)
        append('-s', '--noquiet')
        append('--noclean', '--keep_pass')
        if self.__slurm:
            append('--slurm')
        else:
            append('--noslurm')
        if self.__max_tests: append('--max_tests', self.__max_tests)
        return argv

    @staticmethod
    def xmain(args):
        etr = Etr(args)
        argv = etr \
            .__validate() \
            .__transmogrify()
        argv.insert(0, 'etr')
        debug('Etr-1', ' '.join([str(e) for e in argv[1:]]))
        from ETdv.dvrun.main import main
        main(argv, start_msg=False)


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description='Simple UI for et-dvrun.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-v', type=str, default='W', choices=['I', 'W', 'D'], help='message severity')
    parser.add_argument('-s', action='store_true',
                        help='dispatch using Slurm')
    parser.add_argument('-t', type=str, action='append',
                        metavar='test_name,...', help='(partial) test name')
    parser.add_argument('-u', type=str, action='append',
                        metavar='arg', help='user-level argument')
    parser.add_argument('-b', type=str, action='append',
                        metavar='build_name,...', help='(partial) build name')
    parser.add_argument('-r', type=str,
                        metavar='dirname', help='runroot directory')
    parser.add_argument('-j', type=int, default=Etr.DEFAULT_J,
                        metavar='njobs', help='number of parallel jobs')
    parser.add_argument('-n', type=int, metavar='N',
                        help='run no more than <N> tests')
    parser.add_argument('-m', type=int, metavar='N',
                        help='test timeout <N> (min); N=0 for infinite', default='0')
    parser.add_argument('config', type=str, nargs='+',
                        metavar='config',
                        help='et-dvrun config file (.etdv.py suffix optional)')
    args = parser.parse_args()
    try:
        Etr.xmain(args)
    except EarlyTerminationException:
        exit(1)
    exit(0)
