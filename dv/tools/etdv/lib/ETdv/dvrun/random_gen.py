# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from random import Random
from os import urandom, path, makedirs
from sys import byteorder
from ETdv.dvrun.messaging import info, warn
from ETdv.util import write_file

RANDOM_SEED_FNAME = '.randomseed'


def _get_initial_seed():
    return int.from_bytes(urandom(4), byteorder)


class RandomGen(Random):
    __the_one = None

    @staticmethod
    def the_one():
        return RandomGen.__the_one

    def __init__(self, xseed=None):
        assert RandomGen.__the_one is None
        xseed = xseed or _get_initial_seed()
        super().__init__(xseed)
        self.__seed = xseed
        RandomGen.__the_one = self
        warn('RandomGen-1', f'initialize RandomGen with seed={self.get_seed()}')

    def get_seed(self):
        return self.__seed

    def write_seed(self, xpath=None):
        if xpath is None: return
        fname = path.realpath(path.join(xpath, RANDOM_SEED_FNAME))
        makedirs(path.dirname(fname), exist_ok=True)
        write_file(fname, self.get_seed())


def etdv_random():
    """
    Get random as floating point.
    :return: random as floating point.
    """
    rnd = RandomGen.the_one().random()
    info('RandomGen-1', f'etdv_random={rnd}')
    return rnd


def etdv_random_seed():
    return RandomGen.the_one().get_seed()


def etdv_randint(lo=1, hi=(2 ** 32) - 1):
    rnd = RandomGen.the_one().randint(lo, hi)
    info('RandomGen-1', f'etdv_randint={rnd}')
    return rnd


if __name__ == '__main__':
    from ETdv.dvrun.messaging import Messaging

    Messaging('I')
    RandomGen()  # (1234)
    x = etdv_random_seed()
    etdv_random()
    etdv_random()
    etdv_randint()
    etdv_randint()
    etdv_random()
    etdv_random()
    etdv_randint()
    etdv_randint()
