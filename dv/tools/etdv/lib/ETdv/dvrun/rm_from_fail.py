# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Remove file(s) on fail
"""
import shutil
from os import path
from glob import glob
from ETdv.util import read_file, delete_file
from ETdv.dvrun.messaging import info, Messaging, EarlyTerminationException


class RmFromFail:
    def __init__(self, args):
        self.__runroot = args.r
        self.__files = args.file
        self.__rm_cnt = 0
        Messaging(args.v)

    def __process(self):
        status = glob(path.join(self.__runroot, '**', 'status'), recursive=True)
        failed_dirs = [path.dirname(s) for s in status if 'pass' != read_file(s)]
        for dir in failed_dirs:
            for f in self.__files:
                to_rm = glob(path.join(dir, f), recursive=False)
                if to_rm: assert 1 == len(to_rm)
                for r in to_rm:
                    if path.isfile(r): delete_file(r)
                    else:
                        assert path.isdir(r)
                        shutil.rmtree(r, ignore_errors=True, onerror=None)
                    self.__rm_cnt += 1
        info('RmOnFail-1', self.__rm_cnt)
        return self

    @staticmethod
    def main(args):
        RmFromFail(args).__process()


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description='Remove file(s) and dir(s) from failed tests.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-v', type=str, default='W', choices=['I', 'W', 'D'], help='message severity')
    parser.add_argument('-r', type=str, default='.',
                        metavar='dirname', help='runroot directory')
    parser.add_argument('file', type=str, nargs='+',
                        metavar='file_or_dir',
                        help='file or dir (glob pattern) to remove')
    args = parser.parse_args()
    try:
        RmFromFail.main(args)
    except EarlyTerminationException:
        exit(1)
    exit(0)
