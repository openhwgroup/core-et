# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'pipe_unit_test'

assert (getenv('ET_PROJ_ROOT') is not None), "env ET_PROJ_ROOT is not set"
setenv('CCACHE_BASEDIR', getenv('ET_PROJ_ROOT'))

env = ['CCACHE_BASEDIR']

cwd = getenv('PWD')
check_build_status = f'{cwd}/todo'
check_test_status = f'{cwd}/todo'

build = new_build(
    name='build1', 
    preamble="""
# Stuff written into script before any run-cmd(s).
# These could be functions and alike we want to reuse.
    """,
    run_cmd=f'cd {cwd}' """
vcs -full64 -sverilog -debug_all  -timescale=1ns/100ps  -f run.vcs.f +vcs+lic+wait 
    """,
    check_cmd=f'{check_build_status}' " $LOG $TARGET"
)


add_build(build)

import random
random.seed(1234)
for i in range(10):
    rand = int(1000 * random.random())
    test = new_test(
        name=f'rand-data-seed{rand}',
        run_cmd=f"cd {cwd}" '\n'
                f'./simv +dump=1 +use_rand_data +ntb_random_seed={rand} +vcs+lic+wait'
        ,
        check_cmd=f'{check_test_status}' " $LOG $TARGET"
    )
    build.add_test(test)

