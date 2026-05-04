# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Global vars and methods used to manipulate configurations
"""
import os
from ETdv.util import as_list
from ETdv.dvrun.build import Build
from ETdv.dvrun.test import Test

# If we use __ prefix: we cannot set outside module.
# But if we use _ prefix, we can.
# So, let's keep this somewhat hidden (and use _).
_builds = []
_current = None
name = None
env = []
rof_signature_exclude = []
CWD = None

# Methods for use within config files

def add_build(build):
    assert (isinstance(build, Build)), "build must be Build subclass"
    _builds.append(build)


def new_build(**kwargs):
    return Build(**kwargs)


def new_test(**kwargs):
    return Test(**kwargs)


def get_config_filename():
    return os.path.realpath(_current.filename)


def get_user_arg():
    """
    Get --uarg value(s).
    :return: [] or value(s).
    """
    from ETdv.dvrun.main import Main
    user_arg = Main.the_one().options['--uarg'] or []
    return as_list(user_arg)
