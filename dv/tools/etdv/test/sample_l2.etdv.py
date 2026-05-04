# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


REPOROOT = getenv('REPOROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"
CWD = getenv('PWD')

env = ['REPOROOT']

config_name = 'sample_l2'

build = new_build(name='with_dpi')
run_cmd = \
    f'cd {CWD}' \
    """
    /bin/rm -rf vbuild
    mkdir vbuild
    cd vbuild
    build_dir=`/bin/pwd` 
    ${REPOROOT}/dv/dpi/evl_tcl/src/build_dpi
    vcs -full64 -f ${REPOROOT}/dv/sample_l2/run.vcs.f  \
        -debug_all  \
        -debug_access+all  \
        +memcbk +v2k -ntb_opts uvm-1.2 -sverilog -timescale=1ns/1ps  \
        ${build_dir}/libevl_tcl.so  \
        +define+ET_NEW_SHIRE_CACHE +define+SHIRE_CACHE_AXI
	"""
build['run_cmd'] = run_cmd

add_build(build)

test = new_test(
    name='rand_all_ops',
    run_cmd = \
		f'cd {CWD}' \
		"""
        vbuild/simv -l log +dump=1 +verbose_mesh -licwait 5 +tclPath="$REPOROOT/dv/tests/tcl_tests/shire_cache_tcl_tests" +neighStim="rand_all_ops.tcl"
        """,
    check_cmd= \
		"@if [ `fgrep -c 'TEST HAS PASSED' $LOG` -eq 4 ]; " \
		"then echo pass > $TARGET; " \
		"else echo fail > $TARGET; " \
		"fi"
)
build.add_test(test)

