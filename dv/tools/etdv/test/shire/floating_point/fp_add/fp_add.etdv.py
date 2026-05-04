# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os, sys
from ETdv.dvrun.simfile import Simfile

config_name = 'fp_add'

REPOROOT = getenv('REPOROOT')
assert REPOROOT is not None, 'env var REPOROOT needs to be set'

CWD = os.path.dirname(get_config_filename())

# Export these vars so we can have a truly replayable script
# which does not depend on env.sh
env = ['RISCV', 'TESTROOT', 'TESTSHIRE', 'TESTSTUB', 'RTLROOT']

# Use import technique so we can use IDE/debugger
sys.path.append(os.path.abspath(CWD))
from mysimfile import MySimfile
sys.path.pop()  #we're done with above import and dont want to pollute global

tests = []
tests.append('regression')

for test in tests:
    MySimfile(os.path.join(CWD, f'{test}.sim'), config_name, CWD).process()
