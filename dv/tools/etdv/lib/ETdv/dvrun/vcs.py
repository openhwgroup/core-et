# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.tool import Tool


class Vcs(Tool):
    NAME = 'VCS'

    def __init__(self, arg_parser):
        super().__init__(Vcs.NAME, arg_parser)

    def add_options(self):
        opt_group = self.arg_parser().add_group(
            tool=Vcs.NAME,
            name='common-opts',
            title='Common Simulator Options',
            description='Options common to VCS and Verilator')
        opt_group \
            .add('-f@* filename') \
            .add('-v@* library_file') \
            .add('^+incdir@* +dir', usage='[+incdir[+<dir>]...]...') \
            .add('-y@* library_directory') \
            .add('^+libext@* +suffix', usage='[+libext[+<sfx>]...]...') \
            .add('^+systemverilogext@* +ext', usage='[+systemverilogext[+<sfx>]...]...') \
            .add('^+verilog2001ext@* +ext', usage='[+verilog2001ext[+<sfx>]...]...') \
            .add('^+define@* +var', usage='[+define[+<var>[=<val>]]...]...')

        opt_group = self.arg_parser().add_group(
            tool=Vcs.NAME,
            name='vcs-native',
            title='{}: Native Testbench'.format(Vcs.NAME))
        opt_group \
            .add('-cg_coverage_control 0|1') \
            .add('-ntb_opts uvm|')

        opt_group = self.arg_parser().add_group(
            tool=Vcs.NAME,
            name='vcs-analyze-compile',
            title='{}: Analyze/Compile'.format(Vcs.NAME))
        opt_group \
            .add('-full64') \
            .add('+delay_mode_path') \
            .add('+delay_mode_zero') \
            .add('+delay_mode_unit') \
            .add('+delay_mode_distributed') \
            .add('+notimingchecks') \
            .add('+nospecify') \
            .add('+nowarnTFMPC') \
            .add('-sverilog') \
            .add('-extinclude') \
            .add('+v2k')

        opt_group = self.arg_parser().add_group(
            tool=Vcs.NAME,
            name='vcs-elaborate-compile',
            title='{}: Elaborate/Compile'.format(Vcs.NAME))
        opt_group \
            .add('-cm@* line|cond|fsm|tgl|branch|assert', usage='[-cm [+(line|cond|fsm|tgl|branch|assert)]...]...') \
            .add('-kdb') \
            .add('-debug_pp') \
            .add('-debug_all') \
            .add('^-debug_access@* +access', usage='[-debug_access[+option]...]...') \
            .add('-designunit design_unit', usage='[-designunit [libname.]design_unit]') \
            .add('+liborder') \
            .add('+librescan') \
            .add('+libverbose') \
            .add('+vcs+initreg+random') \
            .add('+vcs+initreg+random+nomem') \
            .add('+vcs+initreg+random+noreg') \
            .add('+rad') \
            .add('-R') \
            .add('-simprofile time|mem') \
            .add('-V') \
            .add('-Vt') \
            .add('-licwait timeout', help='timeout in mins') \
            .add('-race') \
            .add('-boundscheck') \
            .add('+vcs+vcdpluson') \
            .add('+memcbk') \
            .add('^+vcs+loopreport +number=2000000', usage='[+vcs+loopreport+<number>]') \
            .add('^+vcs+loopdetect +number=2000000', usage='[+vcs+loopdetect+<number>]') \
            .add('-diag env') \
            .add('^+vcs+stop +time', usage='[+vcs+stop+<time>]') \
            .add('^+vcs+finish +time', usage='[+vcs+finish+<time>]') \
            .add('^-timescale= value', usage='[-timescale=<value>]', help='Example <value>: 1ns/1ns') \
            .add('^+ntb_exit_on_error= value', usage='[+ntb_exit_on_error=<value>]',
                 help='<value>: one of 0 (continue); 1 (exit on first error (default)); N (exit on N-th error)') \
            .add('^+ntb_random_seed= value', usage='[+ntb_random_seed=<value>]') \
            .add('+ntb_random_seed_automatic')

        opt_group = self.arg_parser().add_group(
            tool=Vcs.NAME,
            name='vcs-incr-compile',
            title='{}: Incremental Compile'.format(Vcs.NAME))
        opt_group \
            .add('-Mdir directory') \
            .add('-Mlib directory') \
            .add('-noIncrComp') \
            .add('-paralle_compile_off')
