# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Lightweight ui for et-dvrun-status.
"""
from os import path
from glob import glob
from ETdv.dvrun.util import get_default_runroot
from ETdv.dvrun.messaging import debug, Messaging, early_termination_error, EarlyTerminationException


class Ets:
    def __init__(self, args):
        self.__makefile = None
        Messaging()

    def __validate(self):
        dirs = [path.dirname(mkf)
                for mkf in glob(path.join('.', '**', 'Makefile'), recursive=True)
                if path.isfile(mkf)]
        dirs = [dir for dir in dirs if path.split(dir)[-1] == 'makefile']
        n = len(dirs)
        if 0 == n:
            #perhaps just try here
            mkf = path.join('.', 'Makefile')
            if path.isfile(mkf):
                dirs = ['.']
                n = 1
        if 0 == n:
            early_termination_error('Ets-2')
        elif 1 < n:
            early_termination_error('Ets-3', dirs)
        self.__makefile = dirs[0]
        return self

    def __transmogrify(self):
        argv = ['progress', self.__makefile]
        return argv

    @staticmethod
    def xmain(args):
        ets = Ets(args)
        argv = ets \
            .__validate() \
            .__transmogrify()
        argv.insert(0, 'ets')
        debug('Ets-1', ' '.join([str(e) for e in argv[1:]]))
        from ETdv.dvrun.status import main
        main(argv)


def main():
    import argparse

    parser = argparse.ArgumentParser(description='Simple UI for et-dvrun-status progress')
    args = parser.parse_args()
    try:
        Ets.xmain(args)
    except EarlyTerminationException:
        exit(1)
