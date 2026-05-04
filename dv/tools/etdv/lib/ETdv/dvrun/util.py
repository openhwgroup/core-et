# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re, os
from ETdv.dvrun.check_log import CheckLog
from ETdv.util import getenv, get_timestamped_suffix, exec
from ETdv.dvrun.dvrun import DvRun


def cmdline_as_string(argv):
    def conv(x):
        if re.compile(r' |\t').search(x):
            y = x.replace("\"", "\\\"")
            return f"\"{y}\""
        else:
            return str(x)

    return ' '.join(map(lambda x: conv(x), argv))


def get_branch_and_bist():
    branch = exec('git rev-parse --abbrev-ref HEAD').strip().split()[-1]
    is_bist = re.compile(r'master_bisted|bist_rtl', re.IGNORECASE).search(branch)
    is_bist = (is_bist and True) or False
    return [branch, is_bist]


def is_valid_pid(pid):
    """ Check For the existence of a unix pid. """
    try:
        os.kill(pid, 0)
    except OSError:
        return False
    else:
        return True


def log_checker(log, check):
    checker = CheckLog(test_check=check)
    return checker.process(['test', log, None])


def get_default_runroot():
    """
    Determine default value of runroot.
    :return: value.
    """
    dir, sfx = None, get_timestamped_suffix(DvRun.NAME)

    def join_dir():
        return os.path.join(dir, sfx)

    dir = getenv('TESTRUN')
    if dir is not None: return join_dir()
    user = getenv('USER')
    assert (user)
    for i in ['/data/users', '/data']:
        dir = os.path.join(i, user)
        if os.path.isdir(dir):
            dir = os.path.join(dir, 'test_run_dir')
            if not os.path.isdir(dir):
                os.mkdir(dir)
            return join_dir()
    dir = '.'
    return join_dir()
