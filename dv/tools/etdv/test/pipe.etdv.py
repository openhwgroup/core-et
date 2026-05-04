# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


assert getenv('REPOROOT') is not None, 'must setenv REPOROOT'

env = ['REPOROOT']

import os
CWD = os.path.dirname(get_config_filename())

config_name = 'pipe'

# Many builds
BUILDS = [
{'dir': 'config_8m_8b_8sb',              'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8"},
{'dir': 'config_8m_8b_4sb',              'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4"},
{'dir': 'config_8m_4b_8sb',              'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8"},
{'dir': 'config_4m_8b_4sb',              'build_opts': "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4"},
{'dir': 'config_8m_8b_8sb_asic_mem',     'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS"},
{'dir': 'config_8m_8b_4sb_asic_mem',     'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS"},
{'dir': 'config_8m_4b_8sb_asic_mem',     'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS"},
{'dir': 'config_4m_8b_4sb_asic_mem',     'build_opts': "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS"},
{'dir': 'config_8m_8b_4sb_cbuf4',        'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_CBUF_SIZE=4"},
{'dir': 'config_8m_8b_4sb_sptagst',      'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_TAG_STATE_RAM_SINGLE_PORT"},
{'dir': 'config_8m_8b_4sb_cbuf4_rbuf6',  'build_opts': "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_CBUF_SIZE=4 +define+SC_RBUF_SIZE=6"}
]

# map of build object by dir/config
all_builds = {}  
for build in BUILDS:
    name = build['dir']
    dir = os.path.join(CWD, name)
    opts = build['build_opts']
    build_cmd = f'dir={dir}\nbuild_opts="{opts}"' """
    /bin/rm -rf $dir
    mkdir -p $dir
    cd $dir
    vcs -full64 -sverilog -f $REPOROOT/dv/tests/unit_tests/pipe/run.vcs.f "$build_opts" -debug_all -timescale=1ns/100ps  +vcs+lic+wait
    """
    bld = new_build(name=name,run_cmd=build_cmd)
    all_builds[name] = bld
    add_build(bld)

#   $(BUILD_DIR)/simv -l $(SIM_LOG) $(SIM_OPTS) +ntb_random_seed=$(SEED) +vcs+lic+wait   
#simple tests
CHECK = \
        "@if [ `fgrep -c '*** TEST HAS PASSED ***' $LOG` -eq 1 ]; " \
        "then echo pass > $TARGET; " \
        "else echo fail > $TARGET; " \
        "fi"

for name, build in all_builds.items():
    dir = os.path.join(CWD, name)
    cmd = f'dir={dir}' """
    cd $dir
    ./simv +ntb_random_seed=0 +vcs+lic+wait
    """
    test_name = f'simple_{name}'
    test = new_test(name=test_name, run_cmd=cmd, check_cmd=CHECK)
    build.add_test(test)

#cmd based tests
import glob, pathlib, re
for name, build in all_builds.items():
    dir = os.path.join(CWD, name)
    cmd_files = glob.iglob(f'{CWD}/cmd/*.cmd')
    for cmd_file in cmd_files:
        cmdnm = pathlib.PurePosixPath(cmd_file).stem
        cmdnm = re.sub(r'\W','_',cmdnm)
        cmd = f'dir={dir}' """
        cd $dir
        """ \
        './simv +ntb_random_seed=0 +vcs+lic+wait +directed ' \
            f'+cmd_file={cmd_file}'
        test_name = f'directed_{name}_{cmdnm}'
        test = new_test(name=test_name, run_cmd=cmd, check_cmd=CHECK)
        build.add_test(test)


#   mkdir -p $(BUILD_DIR)
#   cd       $(BUILD_DIR); vcs -full64 -sverilog -f $(REPOROOT)/dv/tests/unit_tests/pipe/run.vcs.f -l $(BUILD_LOG) $(BUILD_OPTS) -debug_all -timescale=1ns/100ps  +vcs+lic+wait
    
