# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'txfma'

REPOROOT = getenv('REPOROOT')
assert REPOROOT is not None, 'env var REPOROOT needs to be set'

import os

TESTROOT = os.path.join(REPOROOT, 'test')
CWD = os.path.dirname(get_config_filename())

# Export these vars so we can have a truly replayable script
# which does not depend on env.sh
env = ['RISCV', 'TESTROOT', 'TESTSHIRE', 'TESTSTUB', 'RTLROOT']


def my_presim_cmd(script, options=None, args=None):
    presim = \
        f"export SEED={options['SEED']}\n" \
        "export testRunDir=${ETDV_RUNDIR}\n" \
        "pushd ${ETDV_RUNDIR}\n" \
        f"{script} {' '.join(args)}\n" \
        "popd\n"
    return presim


tb_exe = f'{CWD}/build/vcs/{config_name}_tb'

build = new_build(
    name=os.path.basename(tb_exe),
    run_cmd=f'make -C {CWD} clean VCS',
    check_cmd=f'@if [ -f {tb_exe} ]; then echo pass > $TARGET ; else echo fail > $TARGET ; fi'
)
add_build(build)

tests = []
tests.append('compare')
tests.append('computational')
tests.append('conversion')
tests.append('uvm')

for test in tests:
    add_tests_from_simfile(build, tb_exe, os.path.join(CWD, f'{test}.sim'), presim=my_presim_cmd)
