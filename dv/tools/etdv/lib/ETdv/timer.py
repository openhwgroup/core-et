# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import time

"""
Collection of classes useful for script creation.
"""


class Timer:
    def __init__(self, start=time.time()):
        """
        Create timer object we can use to get elapsed time.
        """
        self._start = start

    def minutes(self):
        return self.elapsed(as_str=False, as_min=True)

    def elapsed(self, as_str=True, force_dd=False, as_min=False):
        """
        Get elapsed time.
        :param as_str: True to return string, else dict.
        :param force_dd: True to force DD (days) to show, other iff. day > 0.
        :return: string or dict.

        >>> t = Timer()
        >>> t.elapsed()
        '00:00:16'
        >>> t.elapsed(force_dd=True)
        '00:00:00:30'
        >>> t.elapsed(as_str=False)
        {'DD': 0, 'HH': 0, 'MM': 0, 'SS': 45}
        """
        ss = int(time.time() - self._start)
        if as_min:
            return ss // Timer.SEC_PER_MIN
        dd = ss // Timer.SEC_PER_DAY
        ss -= dd * Timer.SEC_PER_DAY
        hh = ss // Timer.SEC_PER_HOUR
        ss -= hh * Timer.SEC_PER_HOUR
        mm = ss // Timer.SEC_PER_MIN
        ss -= mm * Timer.SEC_PER_MIN
        if as_str:
            fmt = [Timer.FMT] * 3
            vals = [hh, mm, ss]
            if force_dd or dd:
                fmt += [Timer.FMT]
                vals = [dd] + vals
            return ':'.join(fmt).format(*vals)
        else:
            return {'DD': dd, 'HH': hh, 'MM': mm, 'SS': ss}

    SEC_PER_MIN = 60
    SEC_PER_HOUR = 60 * SEC_PER_MIN
    SEC_PER_DAY = 24 * SEC_PER_HOUR
    FMT = '{:02d}'
