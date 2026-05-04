# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


REPOROOT         = getenv('REPOROOT')
MINION_DIAGS     = getenv('MINION_DIAGS')
RISCV            = getenv('RISCV')
COMPLIANCE_TESTS = getenv('COMPLIANCE_TESTS')

assert REPOROOT         is not None, "setenv REPOROOT must be defined"
assert MINION_DIAGS     is not None, "setenv MINION_DIAGS must be defined"
assert RISCV            is not None, "setenv RISCV must be defined"
assert COMPLIANCE_TESTS is not None, "setenv COMPLIANCE_TESTS must be defined"
# import utilities
import io
import math
import os
import sys
import random
import string
import re
from collections import namedtuple
from ETdv.dvrun.messaging import (info)
from ETdv.dvrun.messaging import (warn)
from ETdv.dvrun.main import Main
sys.path.append(f'{REPOROOT}/dv/tools')
from dvutil import *
sys.path.append(f'{REPOROOT}/dv/tests/minion_core/regress_lists')

MEM_DELAYS_DICT = {
   "none"  : 10,
   "short" : 20,
   "medium": 10,
   "long"  :  5,
   "bursty":  5,
   "random": 50,
}

ALLOW_BUS_ERROR_OPTS = " +allow_bus_err=1 +disableSCSBChecks +disableAXIChecks +disableETLinkChecks "

config_name = 'minion_shire'
random.seed()

common_sim_opts = ' +disable_random_data=1 +ALLOC_MEM_RUNTIME=1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +maxErrorCount=1 +SEED=0 +vcs+lic+wait fgp=single_socket_mode,dynamictoggle -reportstats'
rand_delays     = ' +axiReadReqRdyDelays=random +axiWriteReqRdyDelays=random +axiWriteDataRdyDelays=random +axiReadRspRdyDelays=random +axiWriteRspRdyDelays=random +etlinkReqRdyDelays=random +etlinkRspRdyDelays=random +memDelays=random'


#  ________  ________     _    _____ _   _ _____      ___ ______ _____ _____
# /  __ |  \/  |  _  \   | |  |_   _| \ | |  ___|    / _ \| ___ |  __ /  ___|
# | /  \| .  . | | | |   | |    | | |  \| | |__     / /_\ | |_/ | |  \\ `--.
# | |   | |\/| | | | |   | |    | | | . ` |  __|    |  _  |    /| | __ `--. \
# | \__/| |  | | |/ /    | |____| |_| |\  | |___    | | | | |\ \| |_\ /\__/ /
#  \____\_|  |_|___/     \_____\___/\_| \_\____/    \_| |_\_| \_|\____\____/
#

sys_emu_log         = '-l'
pre_test_args       = ''
compile_opts        = ''
use_upf             = False
use_xprop           = False
extra_sim_opts      = '' + add_cm_args('test')
extra_build_opts    = ' '
extra_make_opts     = ''
user_seed           = None
debug_mode          = 0
ukernel_mode        = 0
ddr_enable          = 0
allow_rand_seed     = False
forced_build        = ''
skip_vcs_run        = False
is_coverage_enabled = False
build_only          = False
test_only           = False
kernel_count        = 1
fgp_count           = 2
clean_swlib         = False
is_bist             = False
postsi_ust          = False
postsi              = False
power_eval          = False
bringup_tgt_shires  = False
tgt_memshires_mask = False

usr_args = get_user_arg();

# Debug Mode (trace + verbose + sysemu run before test + disassemble elf)
if usr_args.count('debug') != 0:
   info('Msgs-1', 'Debug mode enabled.')
   debug_mode = 1

if usr_args.count('ukernel_mode') != 0:
   info('Msgs-1', 'ukernel mode enabled.')
   ukernel_mode = 1

# Enable Fine Grain Parallism - parallel threads executing SIMV partitions simultaneously
fgp_cnt_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('fgp=')]
if fgp_cnt_opt:
   info('Msgs-1', 'VCS FGP enabled.')
   fgp_count  = fgp_cnt_opt[0]
   compile_opts += 'VCS_FGP_ENABLE=1 '
   common_sim_opts += f' -fgp=num_threads:{fgp_count} '


if usr_args.count('vcs_sw_sanitize') != 0:
   info('Msgs-1', 'Enable Address Sanitizer for VCS runs - to help debugging Memory Leaks.')
   compile_opts += 'VCS_CPP_DEBUG=1 '

if usr_args.count('enable_ddr') != 0:
   info('Msgs-1', 'Enable real DDR model.')
   ddr_enable = 1
   compile_opts += ' +define+USE_REAL_DDRC +define+MEMSHIRE_AUTO_INIT_ENABLE '

if usr_args.count("enable_uevent"):
    info('Msgs-1', 'Enable uevent library + event fifos')
    compile_opts += ' ENABLE_UEVENT=1 '

# Add verbosity to tests
if debug_mode == 1 or usr_args.count('vv') or usr_args.count('babble') != 0 or usr_args.count('verbose') != 0:
   common_sim_opts += ' +verbosity=HIGH +DEFAULT_LOG_LEVEL=0 +COSIM_LOG_LEVEL=0 +EMU_LOG_LEVEL=0'
   info('Msgs-1', 'Test log set to verbosity DEBUG')
elif usr_args.count('v') or usr_args.count('info') != 0:
   common_sim_opts += ' +verbosity=MEDIUM +DEFAULT_LOG_LEVEL=1'
   info('Msgs-1', 'Test log set to verbosity INFO')
elif usr_args.count('cosim_info') != 0:
   # Buffer up to 5 MB of compressed cosim info logs
   common_sim_opts += ' +verbosity=LOW +DEFAULT_LOG_LEVEL=2 +COSIM_LOG_LEVEL=1 +COSIM_LOG_PATH=cosim.log.lz4 +COSIM_LOG_LIMIT=5M'
   info('Msgs-1', 'Test log set to verbosity INFO (for CoSim only)')
else:
   common_sim_opts += ' +verbosity=LOW +DEFAULT_LOG_LEVEL=2'

# Build only - no test
if usr_args.count('build_only') != 0:
   build_only = True
   info('Msgs-1', 'Coverage enabled')

#clean SWlib before compiling it
if usr_args.count('clean_swlib') != 0:
   clean_swlib = True
   info('Msgs-1', 'Clean SWlib prior to compiling it')

# Is this BISTed RTL?
if repo_is_bisted():
   is_bist = True
   extra_sim_opts += ' +init_mem_data=0 '
   warn('Msgs-1', 'RTL has BIST inserted')
else:
   warn('Msgs-1', 'RTL does NOT have BIST inserted')


# Enable coverage
if usr_args.count('coverage') != 0:
   compile_opts   += ' ENABLE_COV=1'
   extra_sim_opts += ' -cm line+tgl+branch+assert+fsm+cond -cm_dir test.vdb'
   is_coverage_enabled = True
   info('Msgs-1', 'Coverage enabled')
elif usr_args.count('func_coverage') != 0:
   compile_opts   += ' ENABLE_FUNC_COV=1'
   extra_sim_opts += ' -cm_dir test.vdb'
   info('Msgs-1', 'Functional coverage enabled')

# Enable UPF
if usr_args.count('upf') != 0:
   compile_opts   += ' ENABLE_UPF=1 +define+MRS_TS_GATE_PW_SIM  +define+ET_LIBS_USE_PG_STD_CELLS +define+DWC_EMMCPHY_PG_PINS +define+DWC_USB2_PG_PINS +define+ET_USE_ASIC_MEMS '
   #+define+ET_RAM_CLK_FIX +define+ET_LIBS_USE_ET_PRIMITIVES '
   extra_sim_opts += ' +init_mem_data=0'
   use_upf = True
   os.environ["ET_PG_RAM_ROM_DIR"] = "/pwr"
   env = ['ET_PG_RAM_ROM_DIR'];
   info('Msgs-1', 'UPF enabled')

# Enable UPF_COV
if usr_args.count('upf_cov') != 0:
   compile_opts   += ' ENABLE_UPF_COV=1'
   info('Msgs-1', 'UPF coverage enabled')

# Display errors
if usr_args.count('max_errors') != 0:
   compile_opts += ' ENABLE_MAX_ERRORS=1'
   info('Msgs-1', '10000 erros enabled')

# Enable Simulation profile
if usr_args.count('profile') != 0:
   compile_opts   += ' ENABLE_PROFILE=1'
   extra_sim_opts += ' -simprofile'
   info('Msgs-1', 'Simulation Profiling enabled')

# Enable trace build opts
if usr_args.count('trace_build') != 0:
   if usr_args.count('trace_ddr') != 0:
      compile_opts   += ' VCS_DEBUG=3 BUILD_VERDI_IMAGE=-kdb +dump=1 -kdb'
   elif usr_args.count('trace_functions') == 0:
      compile_opts   += ' VCS_DEBUG=1 BUILD_VERDI_IMAGE=-kdb +dump=1 -kdb'
   else: # yes, trace functions
      compile_opts   += ' VCS_DEBUG=4 BUILD_VERDI_IMAGE=-kdb +dump=1 -kdb'

# Enable trace
if debug_mode == 1 or usr_args.count('trace') != 0:
   if usr_args.count('trace_ddr') != 0:
      compile_opts   += ' VCS_DEBUG=3 BUILD_VERDI_IMAGE=-kdb'
      extra_sim_opts += ' +fsdb+struct=on +traceEnable=1'
   elif usr_args.count('trace_functions') == 0:
      compile_opts   += ' VCS_DEBUG=1 BUILD_VERDI_IMAGE=-kdb'
      extra_sim_opts += ' +fsdb+struct=on +traceEnable=1'
   else: # yes, trace functions
      compile_opts   += ' VCS_DEBUG=4 BUILD_VERDI_IMAGE=-kdb'
      extra_sim_opts += ' +fsdb+struct=on +traceEnable=1 +fsdb+functions'
      info('Msgs-1', 'Wave dump enabled')

# Enable X propagation
if usr_args.count('xprop') != 0:
   compile_opts   += ' ENABLE_XPROP=1'
   compile_opts   += ' +define+ET_LIBS_USE_STD_CELLS +define+ET_REG_DELAY_ONLY +define+SNPS_SEQ_DELAY=0 '
   use_xprop= True
   info('Msgs-1', 'X propagation enabled')

# Include movellus lock monitor
if usr_args.count('mvls_lock_mon') != 0:
   compile_opts   += ' +define+INCLUDE_MOVELLUS_LOCK_MONITOR '

# Enable DPI event recording
if usr_args.count('record') != 0:
   extra_sim_opts += ' +MONITOR=1'
   info('Msgs-1', 'Event recording enabled')

# Enable gatesim
if get_uarg('gatesim'):
    compile_opts += add_gatesim_args('build');
    compile_opts += add_gatesim_args('flist');
    compile_opts += add_gatesim_args('makedef');
    extra_sim_opts += add_gatesim_args('simvar');
    os.environ["VCS_PRINT_INITREG_INITIALIZATION"] = "1"
    env = ['VCS_PRINT_INITREG_INITIALIZATION'];
    info('Msgs-1','Gatesim enbaled')
    # Enable UPF'
    if use_upf:
       compile_opts   += ' +define+PD_GATE_PW_SIM +define+SNPS_SEQ_DELAY=0'
       os.environ["VCS_NOPART_COMP"] = "1"
       env = ['VCS_NOPART_COMP'];
       info('Msgs-1', 'UPF\' enabled')

# Enable dumping for forced/signals
my_force_list_compile_opts=""
if get_uarg('force_list'):
    my_force_list_compile_opts += get_force_list_build_args();  # FUTURE: This is a hack since rebuild script is fundamentally flawed in that it makes bad assumptions about the formatting of options
    extra_sim_opts += get_force_list_sim_args();
    info('Msgs-1','Dumping sim_force_list.log enbaled')

# Enable zsim
if usr_args.count('zsim') != 0:
    compile_opts += ' ENABLE_BEMU=0 ENABLE_FP=0 ENABLE_ZSIM=1'

# Disable BEMU
if usr_args.count('disable_BEMU') != 0:
    compile_opts += ' ENABLE_BEMU=0'

# Specify target Shires for Bringup tests
bringup_tgt_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('bringup_tgt_shires=')]
if bringup_tgt_opt:
    bringup_tgt_shires = bringup_tgt_opt[0]
    info('Msgs-1', f'UltraSoC NoC test target shire mask is {bringup_tgt_shires}')

# Specify target mem shires for test:
tgt_memshires_mask_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('tgt_memshires_mask=')]
if tgt_memshires_mask_opt:
    tgt_memshires_mask = tgt_memshires_mask_opt[0]
    info('Msgs-1', f'Target memshire mask is {tgt_memshires_mask}')

# ResNet Kernel Count
kernel_cnt_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('kernel_count=')]
if kernel_cnt_opt:
   kernel_count = kernel_cnt_opt[0]
   info('Msgs-1', f'Kernel Count is {kernel_count}')

# sys_emu log option
if (usr_args.count('sys_emu_quiet') != 0):
   sys_emu_log = ''
   info('Msgs-1', f'sys_emu quiet/nolog option was specified')

# Random Instruction Generators Seed
seed_opt = [arg.split('=')[-1] for arg in usr_args if (arg.startswith('rig_seed=') or arg.startswith('seed='))]
if seed_opt:
   user_seed = seed_opt[0]
   info('Msgs-1', f'Using fixed seed {user_seed} for RNGs')

# Allow directed tests to use random seed
if usr_args.count('allow_directed_rand') != 0:
   allow_rand_seed = True
   info('Msgs-1', 'Directed tests will get a random SystemVerilog seed')

# Allow setting of special macro for directed tests
if usr_args.count('postsi_ust') != 0:
   postsi_ust = True
   info('Msgs-1', 'Running tests for Post-Silicon in UltraSOC pre-load environment')

# DIAG FLOW
if get_uarg('postsi'):
   postsi = True
   test_only = True
   skip_vcs_run = True
   info('Msgs-1', 'Running tests for Post-Silicon using DIAG FLOW')
   si_system = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('postsi=')]
   with open (REPOROOT+'/dv/tools/diag_flow/etdv_config', 'r') as file:
      diag_flow_args = file.read().rstrip()

if postsi:
   if usr_args.count('power_eval') !=0:
      power_eval = True
      info('Msgs-1', 'Power evaluation is enabled')

# skip zebu in case we want postsi compile only
skip_zebu = usr_args.count('skip_zebu') != 0

# skip UST PC trace
skip_ust_pc_trace = usr_args.count('skip_ust_pc_trace') != 0
if skip_ust_pc_trace:
  gen_pc = 0
else:
  gen_pc = 1

# skip laoding data into ddr
skip_ddr_postsi = usr_args.count('skip_ddr_postsi') != 0
if skip_ddr_postsi:
  load_ddr = 0
else:
  load_ddr = 1

# laod tg elf into silicon or ZeBu
load_tf_elf = usr_args.count('load_tf_elf') != 0

zebu_path_file = "none"
zebu_path_file_list = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('zebu_path=')]
if zebu_path_file_list:
    zebu_path_file = zebu_path_file_list[0]

# Skip VCS run, only build test
if usr_args.count('skip_vcs') or usr_args.count('test_only') != 0:
   skip_vcs_run = True
   info('Msgs-1', 'Skip VCS generation/execution, only build test')

if usr_args.count('test_only') !=0:
   test_only = True
   warn('Msgs-1', 'Only tests compilation')

#Glow arguments for Firmware
sw_args = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('sw_args=')]
args = ''
if sw_args:
   for s in sw_args:
       info('Msgs-1', 'Adding SW (Glow/Jitter-Lib Executor) Arguments: {s}')
       args += s + ' '
   pre_test_args = 'flags="' + args + '"'

# User plusargs (DEPRECATED but still there to support old habits)
usr_plusargs = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('plusargs=')]
if usr_plusargs:
   for p in usr_plusargs:
      extra_sim_opts += ' ' + p
      info('Msgs-1', f'Adding plusarg string to all tests: {p}')

# User plusargs
for arg in usr_args:
   if arg.startswith('+'):
      info('Msgs-1', f'Adding plusarg to all tests: {arg}')
      extra_sim_opts += ' ' + arg

# Enable replay stimuli
if usr_args.count('replay') != 0:
   info('Msgs-1', 'Replay stimuli dump enabled')
   compile_opts += ' ENABLE_REPLAY_STIMULI=1 ET_NO_TESTEND_MONITOR=1'
   compile_opts += ' +define+ET_PTPX_SIM=1 +define+ET_NO_TESTEND_MONITOR '
   compile_opts += ' +define+VIRL_functiononly +define+ET_LIBS_USE_ET_PRIMITIVES +define+ET_REG_DELAY_ONLY '
   extra_sim_opts += ' +et_disable_init_rams '

# Pass in VCS build make options
make_opts = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('make_options=')]
if make_opts:
   for b in make_opts:
      extra_make_opts += ' ' + b
   info('Msgs-1', f'Passing the following make options to the make command: {extra_make_opts}')

# Pass in VCS build options
build_opts = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('build_options=')]
if build_opts:
   for b in build_opts:
      extra_build_opts += ' ' + b
   info('Msgs-1', f'Passing the following build options to VCS: {extra_build_opts}')

compile_opts += f' {extra_make_opts}' + " VCS_CMPL_OPTS=\" " + add_cm_args('build') + f' {extra_build_opts}' + f' {get_user_sv_build_args()}' + f' {my_force_list_compile_opts} ' +"\""

# Force build
force_build = [arg.split('=')[-1] for arg in usr_args if arg.startswith('force_build=')]
if force_build:
   forced_build = force_build[0]
   assert forced_build in ['shire', 'shirex2', 'fc', 'fclite', 'fc2shire', 'fc3shire', 'fc4shire' ], f'force_build {forced_build} is illegal. Possible options are shire, shirex2, fc, fclite, fc2shire, fc3shire, fc4shire'
   info('Msgs-1', f'Forcing the testbench to {forced_build} when possible')

extra_sim_opts += get_user_sv_sim_args()

# User help
if usr_args.count('help') != 0 or usr_args.count('h') != 0:
   print(f'')
   print(f'Available arguments for this etdv config script:  --uarg <arg_string>   where arg_string is one of the following')
   print(f'')
   print(f' === Options available for all tests ===')
   print(f'   vv                      Change verbosity of test logs to Very Verbose')
   print(f'   v                       Change verbosity of test logs to Verbose')
   print(f'   trace                   Enable wave dump')
   print(f'   trace_build             Build with debug opts, but does not turn on wave dumps in the tests')
   print(f'   trace_ddr               If wave dump enabled, limit dumping in cells')
   print(f'   trace_functions         If wave dump enabled, add support for tracing inside functions')
   print(f'   debug                   very verbose + trace + run against SYSEMU first + disassemble ELF')
   print(f'   sw_args                 Enable an arguments related to SW/FW execution')
   print(f'   +globalDebug=<pathtotcl> Global Debug control file. Used to control time window when FSDB dumping should occur, increase/reduce verbosity within a time window, sample file in: dv/waves/globalDebug.tcl')
   print(f'   +fsdbConfigFile=<path file> FSDB hiearchy control flag - to dump only hierachy specified in file, sample file in: dv/waves/eagle_shire_fsdb_config')
   print(f'   coverage                Enable code and functional coverage')
   print(f'   func_coverage           Enable functional coverage only')
   print(f'   force_list              Dumps reports of forces in PLI/UCLI (new build required)')
   print(f'   upf                     Enable Unified Power Format')
   print(f'   upf_cov                 Enable UPF Coverage')
   print(f'   xprop                   Enable X-propagation')
   print(f'   gatesim=<string>        Enables the gatesim on the modules provided with <string>. (e.g. --uarg gatesim=\'IOSHIRE,PSHIRE,MINSHIRE=34\'h7,MAXSHIRE,<blockname>\'')
   print(f'   zsim                    Disable cosim and enable zsim')
   print(f'   profile                 Enable simulation profiling')
   print(f'   replay                  Enable dumping stimuli vectors for replaying')
   print(f'   +<arg>                  Pass provided plusarg to all tests')
   print(f'   build_only              Do not build nor run any test, only build the VCS binary (preferably used with --build_name_enable)')
   print(f'   test_only               Do not build nor run the VCS binary, but do all other steps of the tests (e.g. generation, compilation, pre-test scripts)')
   print(f'   build_options=<opt>     Pass custom option to the VCS compilation process (repeat this uarg for each new option)')
   print(f'   make_options=<opt>      Pass custom option to the VCS make compilation command (repeat this uarg for each new option)')
   print(f'   force_build=<build>     Change the testbench to the specified model when possible (if it is a superset of the original testbench e.g. shire can replace neigh)')
   print(f'   record                  Enable recording of CoSim events')
   print(f'')
   print(f' === Options specific to some tests/builds ===')
   print(f'   clean_swlib             Select the option to clean SWlib before compiling it for Full-Chip tests with IOSHIRE.')
   print(f'   sys_emu_quiet           Disable full logging (-l) for standalone sysemu.log run. Default is to run with verbose logging (not quiet)')
   print(f'   seed=<N>                Set a fixed seed for Random Instruction Generator and SystemVerilog RNG.')
   print(f'   allow_directed_random   Directed tests can have some randomness in them, but it is fixed by the SystemVerilog seed which is by default 0. This allows randomisation of the SV seed.')
   print(f'   postsi_ust              Directed tests are being run for Post-Silicon UltraSOC preload and have different test pass/fail macros appropriate for that environment.')
   print(f'   postsi                  Directed tests are being run for Post-Silicon using DIAG FLOW. --uarg skip_zebu for the compile only')
   print(f'   lsbuilds=<string>       Lists the build name and the corresponding build command. <string> can be comma separated list of build names. If <string> == \'all\', this will list all the build names. If <string> == \'full\', this will list all the builds along with the run and check commands')
   print(f'   lstests=<string>       Lists the test name and the corresponding test command. <string> can be comma separated list of test names. If <string> == \'all\', this will list all the test names and corresponding builds. If <string> == \'full\', this will list all the tests along with the run and check commands and corresponding builds')
   print(f'')
   exit(0)

def add_build_slurm_zero_license(slurm_licenses = ''):
   return "" # No need to specify licenses

#Specify SLRUM licenses per build
def add_build_slurm_license(slurm_licenses = ''):
   return "" # No need to specify licenses

# ______ _   _ _____ _    ______ _____F
# | ___ | | | |_   _| |   |  _  /  ___|
# | |_/ | | | | | | | |   | | | \ `--.
# | ___ | | | | | | | |   | | | |`--. \
# | |_/ | |_| |_| |_| |___| |/ //\__/ /
# \____/ \___/ \___/\_____|___/ \____/
#
git_sha_cmd = 'cd $REPOROOT && git rev-parse HEAD > $ETDV_RUNDIR/GIT_SHA && git log $(git rev-parse origin/main~1)..HEAD > $ETDV_RUNDIR/LOCAL-CHANGES'
standard_build_check = f'[[ -e $(dirname $TARGET)/vbuild/simv ]] && echo pass > $TARGET || echo fail > $TARGET'

# Neighbourhood
build_neigh_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/neigh/rebuild {compile_opts}'
build_neigh = new_build(name='neigh', run_cmd=build_neigh_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# CPU subsystem
# ANT: temportal hack to have vcs and verilator ...
build_cpu_ss_vl_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/cpu_subsystem/rebuild -verilator {compile_opts}'
build_cpu_ss_vl = new_build(name='cpu_ss_vl', run_cmd=build_cpu_ss_vl_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

build_cpu_ss_vcs_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/cpu_subsystem/rebuild {compile_opts}'
build_cpu_ss_vcs = new_build(name='cpu_ss_vcs', run_cmd=build_cpu_ss_vcs_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Minion Shire
#build_shire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/minion_shire/rebuild {compile_opts} +define+ULTRASOC_USB_USE_BFM '
build_shire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/minion_shire/rebuild {compile_opts} '
build_shire = new_build(name='shire', run_cmd=build_shire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# (Charles temp) Minion Shire_1:
shire_1_cfg = ' +define+DV_MINION_SHIRE_DISABLE=0xffffffffd +define+DV_MINION_SHIRE_CACHE_STUB=0xffffffffd +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffffffffd '
build_shire_1_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild &&  $REPOROOT/dv/standalone/minion_shire/rebuild {shire_1_cfg} {compile_opts} '
build_shire_1 = new_build(name='shire_1', run_cmd=build_shire_1_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# (Charles temp) FC Minion_Shire_1:
fc_shire_1_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffffffffd +define+DV_IO_SHIRE_STUB=1 \
                   +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_fc_shire_1_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_shire_1_cfg} {compile_opts}'
build_fc_shire_1 = new_build(name='fc_shire_1', run_cmd=build_fc_shire_1_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# Sparse Minion Shire (4 threads active in each Neighbourhood)
sparse_1s4n4t_cfg = ' +define+DV_MINION_DISABLE=0xfc +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS '
build_1s4n4t_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/minion_shire/rebuild {compile_opts} {sparse_1s4n4t_cfg}'
build_1s4n4t = new_build(name='sparse_1s4n4t', run_cmd=build_1s4n4t_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())
build_1s4n4t.add_test(new_test(name='dummy_build_1s4n4t_check', run_cmd='echo TEST HAS PASSED', check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}', tags=['b4c', 'b4c_stg1']))

# One full Neighbourhood + Shire Channel
shire_1s1n16t_cfg = ' +define+DV_MINION_SHIRE_NEIGH_STUB=0xfe +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS '
build_1s1n16t_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/minion_shire/rebuild {compile_opts} {shire_1s1n16t_cfg}'
build_1s1n16t = new_build(name='shire_1s1n16t', run_cmd=build_1s1n16t_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Minion Shire with GFX enabled
shiregfx_cfg = ' +define+USE_TBOX_MONITORS +define+GFX_ENABLED '
build_shiregfx_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/minion_shire/rebuild {compile_opts} {shiregfx_cfg}'
build_shiregfx = new_build(name='shiregfx', run_cmd=build_shiregfx_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Full Chip
fc_cfg       = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xe00000000 +define+DV_IO_SHIRE_STUB=1 \
               +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+ULTRASOC_USB_USE_BFM'
build_fc_cmd =     f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_cfg} {compile_opts}'
build_fc = new_build(name='fc', run_cmd=build_fc_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Full Chip PTPX
fc_ptpx_cfg       = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xe00000000 +define+DV_IO_SHIRE_STUB=1 \
               +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+ET_PTPX_SIM=1'
build_fc_ptpx_cmd =     f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ptpx_cfg} {compile_opts}'
build_fc_ptpx = new_build(name='fc_ptpx', run_cmd=build_fc_ptpx_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Full Chip PTPX for DDR gates
fc_ddr_ptpx_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xe00000000 +define+DV_IO_SHIRE_STUB=1 +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+ET_PTPX_SIM=1'
build_fc_ddr_ptpx_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ddr_ptpx_cfg} {compile_opts}'
build_fc_ddr_ptpx = new_build(name='fc_ddr_ptpx', run_cmd=build_fc_ddr_ptpx_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

## Enable all Shire Channels + Real NoC
scnoc_opts = ' +define+NOC_ENABLE=1 +define+DV_MINION_SHIRE_DISABLE=0'
build_scnoc_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/shire_cache/rebuild {scnoc_opts}'
build_scnoc = new_build(name='scnoc', run_cmd=build_scnoc_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Full Chip light (no IOShire/PShire and stub the neighbourhoods in all but 8 Minion Shires)
fclite_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xeffffff00 +define+DV_IO_SHIRE_STUB=1 \
               +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_fclite_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fclite_cfg} {compile_opts}'
build_fclite = new_build(name='fclite', run_cmd=build_fclite_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 2 Minion Shires (1 compute shire + master shire)
fc2shire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=1 \
                 +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_fc2shire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc2shire_cfg} {compile_opts}'
build_fc2shire = new_build(name='fc2shire', run_cmd=build_fc2shire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 2 Minion Shires (2 compute shires)
shirex2_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffffffffc +define+DV_IO_SHIRE_STUB=1 \
                +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_shirex2_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {shirex2_cfg} {compile_opts}'
build_shirex2 = new_build(name='shirex2', run_cmd=build_shirex2_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 3 Minion Shires (2 compute shires + master shire)
fc3shire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffc +define+DV_IO_SHIRE_STUB=1 \
                 +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_fc3shire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc3shire_cfg} {compile_opts} '
build_fc3shire = new_build(name='fc3shire', run_cmd=build_fc3shire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 4 Minion Shires (3 compute shires + master shire)
fc4shire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffff8 +define+DV_IO_SHIRE_STUB=1 \
                 +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_fc4shire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc4shire_cfg} {compile_opts}'
build_fc4shire = new_build(name='fc4shire', run_cmd=build_fc4shire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 4 Minion Shires (4 compute shires)
shirex4_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffffffff0 +define+DV_IO_SHIRE_STUB=1 \
                +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_shirex4_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {shirex4_cfg} {compile_opts}'
build_shirex4 = new_build(name='shirex4', run_cmd=build_shirex4_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 8 Minion Shires (8 compute shires)
shirex8_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xfffffff00 +define+DV_IO_SHIRE_STUB=1 \
                +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 '
build_shirex8_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {shirex4_cfg} {compile_opts}'
build_shirex8 = new_build(name='shirex8', run_cmd=build_shirex8_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 2 Minion Shires with IO Shire
fc_ioshire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+LVDPLL_PROG_MEASUREMENT_TB \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+ULTRASOC_USB_USE_BFM +define+LVDPLL_PROG_MEASUREMENT_TB '
build_fc_ioshire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_cfg} {compile_opts}'
slurm_lic_fc_ioshire = ''
build_fc_ioshire = new_build(name='fc_ioshire', run_cmd=build_fc_ioshire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire with IO forces
fc_ioshire_io_forces_cfg = ' +define+SNPS_SEQ_DELAY=0 +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+USE_IOS_FORCES \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB'
build_fc_ioshire_io_forces_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_io_forces_cfg} {compile_opts}'
build_fc_ioshire_io_forces = new_build(name='fc_ioshire_io_forces', run_cmd=build_fc_ioshire_io_forces_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init
fc_ioshire_io_forces_real_init_cfg = ' +define+SNPS_SEQ_DELAY=0 +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+USE_IOS_FORCES \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT'
build_fc_ioshire_io_forces_real_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_io_forces_real_init_cfg} {compile_opts}'
build_fc_ioshire_io_forces_real_init = new_build(name='fc_ioshire_io_forces_real_init', run_cmd=build_fc_ioshire_io_forces_real_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire
fc_ioshire_spi_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_SPI_VIP +define+USE_LE25S161_MODEL'
build_fc_ioshire_spi_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_spi_cfg} {compile_opts}'
build_fc_ioshire_spi = new_build(name='fc_ioshire_spi', run_cmd=build_fc_ioshire_spi_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire
fc_ioshire_i2c_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_I2C_VIP'
build_fc_ioshire_i2c_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_i2c_cfg} {compile_opts}'
build_fc_ioshire_i2c = new_build(name='fc_ioshire_i2c', run_cmd=build_fc_ioshire_i2c_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire with external loopbacks enabled
fc_ioshire_debug_cfg = ' +define+SNPS_SEQ_DELAY=0 +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+ET_USE_GPIO_LOOPBACK +define+ET_USE_SPI_LOOPBACK +define+USE_IOS_FORCES'
build_fc_ioshire_debug_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_debug_cfg} {compile_opts}'
build_fc_ioshire_debug = new_build(name='fc_ioshire_debug', run_cmd=build_fc_ioshire_debug_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 33 Minion Shires with IO Shire
fc33s_ioshire_cfg = ' +define+SNPS_SEQ_DELAY=0 +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0x0 +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB'
build_fc33s_ioshire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc33s_ioshire_cfg} {compile_opts}'
build_fc33s_ioshire = new_build(name='fc33s_ioshire', run_cmd=build_fc33s_ioshire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 4 Minion Shires with IO Shire
fc4s_ioshire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffff8 +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB'
build_fc4s_ioshire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc4s_ioshire_cfg} {compile_opts}'
build_fc4s_ioshire = new_build(name='fc4s_ioshire', run_cmd=build_fc4s_ioshire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init
fc_ioshire_real_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT '
build_fc_ioshire_real_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_real_init_cfg} {compile_opts}'
build_fc_ioshire_real_init = new_build(name='fc_ioshire_real_init', run_cmd=build_fc_ioshire_real_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, real DDR init
fc_ioshire_ddr_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0xfc\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_REAL_DDRC +define+DV_MEMSHIRE_USE_PLL_BEH=0'
build_fc_ioshire_ddr_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_ddr_init_cfg} {compile_opts}'
build_fc_ioshire_ddr_init = new_build(name='fc_ioshire_ddr_init', run_cmd=build_fc_ioshire_ddr_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, All memshires, real DDR init
fc_ioshire_all_mem_ddr_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0x0\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_E2A_AXI_VIP +define+USE_REAL_DDRC +define+DV_MEMSHIRE_USE_PLL_BEH=0'
build_fc_ioshire_all_mem_ddr_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_all_mem_ddr_init_cfg} {compile_opts}'
build_fc_ioshire_all_mem_ddr_init = new_build(name='fc_ioshire_all_mem_ddr_init', run_cmd=build_fc_ioshire_all_mem_ddr_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, All memshires, DDR_AUTO_INIT
fc_ioshire_all_mem_ddr_auto_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0x0\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_E2A_AXI_VIP +define+USE_REAL_DDRC +define+DV_MEMSHIRE_USE_PLL_BEH=0 \
                   +define+MEMSHIRE_AUTO_INIT_ENABLE '
build_fc_ioshire_all_mem_ddr_auto_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_all_mem_ddr_auto_init_cfg} {compile_opts}'
build_fc_ioshire_all_mem_ddr_auto_init = new_build(name='fc_ioshire_all_mem_ddr_auto_init', run_cmd=build_fc_ioshire_all_mem_ddr_auto_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, All memshires, DDR_AUTO_INIT, ULTRASOC
fc_ioshire_all_mem_ddr_auto_init_ust_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0x0\
                   +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=1 +define+DV_PCIE_SS_DISABLE +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_E2A_AXI_VIP +define+USE_REAL_DDRC +define+DV_MEMSHIRE_USE_PLL_BEH=0 \
                   +define+MEMSHIRE_AUTO_INIT_ENABLE +define+MINION_RESETS_CONNECTED_TO_IOSHIRE=1 +define+DV_PCIE_SS_STUB=0 \
                   +define+ENABLE_ULTRASOC_SIM=1 +define+ULTRASOC_USB_USE_BFM=1 '
build_fc_ioshire_all_mem_ddr_auto_init_ust_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_all_mem_ddr_auto_init_ust_cfg} {compile_opts}'
build_fc_ioshire_all_mem_ddr_auto_init_ust = new_build(name='fc_ioshire_all_mem_ddr_auto_init_ust', run_cmd=build_fc_ioshire_all_mem_ddr_auto_init_ust_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 32 Minion Shires with IO Shire, real ESR init, All memshires, DDR_AUTO_INIT, ULTRASOC
fc32s_ioshire_all_mem_ddr_auto_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffff0000 +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0x0\
                   +define+DV_IO_SHIRE_STIM=1 +define+DV_PCIE_SS_STUB=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+DV_PCIE_SS_DISABLE +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_E2A_AXI_VIP +define+USE_REAL_DDRC +define+DV_MEMSHIRE_USE_PLL_BEH=0 \
                   +define+MEMSHIRE_AUTO_INIT_ENABLE +define+ENABLE_ULTRASOC_SIM=1 +define+ULTRASOC_USB_USE_BFM=1 +define+MINION_RESETS_CONNECTED_TO_IOSHIRE=1 '
build_fc32s_ioshire_all_mem_ddr_auto_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc32s_ioshire_all_mem_ddr_auto_init_cfg} {compile_opts}'
build_fc32s_ioshire_all_mem_ddr_auto_init = new_build(name='fc32s_ioshire_all_mem_ddr_auto_init', run_cmd=build_fc32s_ioshire_all_mem_ddr_auto_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))



# fc_ioshire with real memshire PLL
fc_ioshire_real_memshire_pll_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0x00 +define+DV_MEMSHIRE_USE_PLL_BEH=0\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB'
build_fc_ioshire_real_memshire_pll_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_real_memshire_pll_cfg} {compile_opts}'
build_fc_ioshire_real_memshire_pll = new_build(name='fc_ioshire_real_memshire_pll', run_cmd=build_fc_ioshire_real_memshire_pll_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, real west side DDR init
fc_ioshire_ddr_auto_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_DDR_DISABLE=0xfc\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS +define+USE_IOS_FORCES\
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_REAL_DDRC +define+MEMSHIRE_AUTO_INIT_ENABLE '
build_fc_ioshire_ddr_auto_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_ddr_auto_init_cfg} {compile_opts}'
build_fc_ioshire_ddr_auto_init = new_build(name='fc_ioshire_ddr_auto_init', run_cmd=build_fc_ioshire_ddr_auto_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))

# 2 Minion Shires with IO Shire, real ESR init, real east side DDR init
fc_ioshire_ddr_auto_init_east_side_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_DDR_DISABLE=0xcf\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT +define+USE_REAL_DDRC +define+MEMSHIRE_AUTO_INIT_ENABLE '
build_fc_ioshire_ddr_auto_init_east_side_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_ddr_auto_init_east_side_cfg} {compile_opts}'
build_fc_ioshire_ddr_auto_init_east_side = new_build(name='fc_ioshire_ddr_auto_init_east_side', run_cmd=build_fc_ioshire_ddr_auto_init_east_side_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))


# 2 Minion Shires with IO Shire, MEM Shires and PCIe phy, real init
fc_ioshire_pshire_real_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV +define+SKIP_FAKE_ESR_INIT \
                   +define+PCIE_AXI_SCB_EN +define+DV_PCIE_SS_ENABLE '
if use_upf:
   fc_ioshire_pshire_real_init_cfg = fc_ioshire_pshire_real_init_cfg + ' +define+DV_MEMSHIRE_USE_PLL_BEH=0'
compile_opts_pshire_real_init = compile_opts + ' "-P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o" '
build_fc_ioshire_pshire_real_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pshire_real_init_cfg} {compile_opts_pshire_real_init}'
build_fc_ioshire_pshire_real_init = new_build(name='fc_ioshire_pshire_real_init', run_cmd=build_fc_ioshire_pshire_real_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 2 Minion Shires with IO Shire,2 MEM Shires and PCIe phy, real DDR, no forces
fc_ioshire_pcie_ddr_real_init_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0xfc\
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV +define+SKIP_FAKE_ESR_INIT \
                   +define+PCIE_AXI_SCB_EN +define+USE_REAL_DDRC +define+DV_PCIE_SS_ENABLE '
if use_upf:
   fc_ioshire_pcie_ddr_real_init_cfg = fc_ioshire_pcie_ddr_real_init_cfg + ' +define+DV_MEMSHIRE_USE_PLL_BEH=0'
build_fc_ioshire_pcie_ddr_real_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pcie_ddr_real_init_cfg} {compile_opts_pshire_real_init}'
build_fc_ioshire_pcie_ddr_real_init = new_build(name='fc_ioshire_pcie_ddr_real_init', run_cmd=build_fc_ioshire_pcie_ddr_real_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# FC_ALL mem auto init
#2 Minion Shires with IO Shire,2 MEM Shires and P Shire, real DDR, no forces
fc_all_mem_init_cfg  = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0x0 +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0 \
                         +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 \
                         +define+USE_IOSHIRE_COMPS +define+SVT_APB +define+USE_E2A_AXI_VIP \
                         +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV +define+PCIE_AXI_SCB_EN +define+DV_PCIE_SS_ENABLE \
                         +define+USE_REAL_DDRC +define+ET_USE_ASIC_MEMS \
                         +define+INITIALIZE_MEM \
                         +define+SKIP_FAKE_ESR_INIT'

compile_opts_fc_all_mem_init = compile_opts + ' -vcs -P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o '
build_fc_all_mem_init_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_all_mem_init_cfg} {compile_opts_fc_all_mem_init}'
build_fc_all_mem_init = new_build(name='fc_all_mem_init', run_cmd=build_fc_all_mem_init_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# FC_ALL FAKE INIT UltraSoc
#32 Minion Shires with IO Shire,8 MEM Shires and P Shire, real DDR, no forces
fc_all_fake_init_us_cfg  = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0x0 +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0 \
                             +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 \
                             +define+USE_IOSHIRE_COMPS +define+SVT_APB +define+USE_E2A_AXI_VIP \
                             +define+USE_PCIE_PIPE_INTERFACE +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV +define+PCIE_AXI_SCB_EN \
                             +define+USE_REAL_DDRC +define+ET_USE_ASIC_MEMS \
                             +define+MEMSHIRE_AUTO_INIT_ENABLE \
                             +define+ENABLE_ULTRASOC_SIM=1 \
                             +define+DV_PCIE_SS_ENABLE \
                             +define+ULTRASOC_USB_USE_BFM=1 '

compile_opts_fc_all_fake_init_us = compile_opts + ' -vcs -P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o '
build_fc_all_fake_init_us_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_all_fake_init_us_cfg} {compile_opts_fc_all_fake_init_us}'
build_fc_all_fake_init_us = new_build(name='fc_all_fake_init_us', run_cmd=build_fc_all_fake_init_us_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# FC_ALL
#32 Minion Shires with IO Shire,8 MEM Shires and P Shire, real DDR, no forces
fc_all_cfg  = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0x0 +define+DV_IO_SHIRE_STUB=0 +define+DV_MEMSHIRE_DISABLE=0 \
                +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 \
                +define+USE_IOSHIRE_COMPS +define+SVT_APB +define+USE_E2A_AXI_VIP \
                +define+USE_PCIE_PIPE_INTERFACE +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV +define+PCIE_AXI_SCB_EN +define+DV_PCIE_SS_ENABLE \
                +define+USE_REAL_DDRC +define+ET_USE_ASIC_MEMS \
                +define+MEMSHIRE_AUTO_INIT_ENABLE \
                +define+SKIP_FAKE_ESR_INIT '

compile_opts_fc_all = compile_opts + ' -vcs -P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o '
build_fc_all_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_all_cfg} {compile_opts_fc_all}'
build_fc_all = new_build(name='fc_all', run_cmd=build_fc_all_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# FC_FULL
# 33 Minion Shires with IO Shire,2 MEM Shires and P Shire, real DDR
fc_full_cfg  = ' +define+USE_IOSHIRE_COMPS +define+SVT_APB +define+USE_E2A_AXI_VIP '

compile_opts_fc_full = compile_opts + fc_full_cfg
build_fc_full_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc_full/rebuild {compile_opts_fc_full}'
build_fc_full = new_build(name='fc_full', run_cmd=build_fc_full_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# Full Chip for minion gls ultraSoc testing
fc_gls_cfg       = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xe00000000 +define+DV_IO_SHIRE_STUB=0 \
                     +define+DV_IO_SHIRE_STIM=1 +define+DV_PSHIRE_STUB=0 \
                     +define+DV_PSHIRE_STIM=1 +define+ENABLE_ULTRASOC_SIM=1 \
                     +define+ULTRASOC_USB_USE_BFM=1 +define+USE_IOSHIRE_COMPS'
build_fc_gls_cmd =     f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_gls_cfg} {compile_opts}'
build_fc_gls = new_build(name='fc_gls', run_cmd=build_fc_gls_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# 2 Minion Shires with IO Shire, MEM Shires and P Shire
fc_ioshire_pshire_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV \
                   +define+USE_PCIE_PIPE_INTERFACE +define+PCIE_AXI_SCB_EN +define+DV_PCIE_SS_ENABLE '
if use_upf:
   fc_ioshire_pshire_cfg = fc_ioshire_pshire_cfg  + ' +define+DV_MEMSHIRE_USE_PLL_BEH=0'
compile_opts_iopshire = compile_opts + ' -vcs -P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o '
build_fc_ioshire_pshire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pshire_cfg} {compile_opts_iopshire}'
build_fc_ioshire_pshire = new_build(name='fc_ioshire_pshire', run_cmd=build_fc_ioshire_pshire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# 2 Minion Shires with IO Shire, MEM Shires and P Shire and DDR
fc_ioshire_pshire_ddr_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0  +define+USE_REAL_DDRC +define+MEMSHIRE_AUTO_INIT_ENABLE \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV \
                   +define+USE_PCIE_PIPE_INTERFACE +define+PCIE_AXI_SCB_EN +define+DV_MEMSHIRE_USE_PLL_BEH=0 +define+DV_PCIE_SS_ENABLE '
build_fc_ioshire_pshire_ddr_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pshire_ddr_cfg} {compile_opts_iopshire}'
build_fc_ioshire_pshire_ddr = new_build(name='fc_ioshire_pshire_ddr', run_cmd=build_fc_ioshire_pshire_ddr_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# 2 Minion Shires with IO Shire, MEM Shires and P Shire and PCIEPhy
fc_ioshire_pshire_pciephy_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV \
                   +define+PCIE_AXI_SCB_EN +define+DV_PCIE_SS_ENABLE '
if use_upf:
   fc_ioshire_pshire_pciephy_cfg = fc_ioshire_pshire_pciephy_cfg + ' +define+DV_MEMSHIRE_USE_PLL_BEH=0'
compile_opts_iopshire_pciephy = compile_opts + ' "-P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o" '
build_fc_ioshire_pshire_pciephy_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pshire_pciephy_cfg} {compile_opts_iopshire_pciephy}'
build_fc_ioshire_pshire_pciephy = new_build(name='fc_ioshire_pshire_pciephy', run_cmd=build_fc_ioshire_pshire_pciephy_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# [Debug/Ultrasoc] 2 Minion Shires with IO Shire, MEM Shires and P Shire
fc_ioshire_pshire_ust_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_STIM=0 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+USE_E2A_AXI_VIP +define+SVT_PCIE_ENABLE_GEN4 +define+USE_PSHIRE_ENV \
                   +define+USE_PCIE_PIPE_INTERFACE +define+PCIE_AXI_SCB_EN +define+ENABLE_ULTRASOC_SIM=1 +define+ULTRASOC_USB_USE_BFM=1 +define+DV_PCIE_SS_ENABLE '
if use_upf:
   fc_ioshire_pshire_ust_cfg = fc_ioshire_pshire_ust_cfg + ' +define+DV_MEMSHIRE_USE_PLL_BEH=0'
else:
   fc_ioshire_pshire_ust_cfg = fc_ioshire_pshire_ust_cfg +  ' +define+USE_PLL_BEH'
compile_opts_iopshire_ust = compile_opts + ' -vcs -P $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/pli.tab $DESIGNWARE_HOME/vip/svt/etsoc_vip/pcie/N-2017.12_1/obj/amd64/vcs/msglog.o '
build_fc_ioshire_pshire_ust_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_pshire_ust_cfg} {compile_opts_iopshire_ust}'
build_fc_ioshire_pshire_ust = new_build(name='fc_ioshire_pshire_ust', run_cmd=build_fc_ioshire_pshire_ust_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# 2 Minion Shires with IO Shire, NOC config, S32 and S33
fc_ioshire_noc_remap_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xcffffffff +define+DV_IO_SHIRE_STUB=0 \
                   +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=1 +define+DV_PSHIRE_STIM=1 +define+USE_IOSHIRE_COMPS \
                   +define+SVT_APB +define+SKIP_FAKE_ESR_INIT '
build_fc_ioshire_noc_remap_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild {fc_ioshire_noc_remap_cfg} {compile_opts}'
build_fc_ioshire_noc_remap = new_build(name='fc_ioshire_noc_remap', run_cmd=build_fc_ioshire_noc_remap_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license(slurm_lic_fc_ioshire))


# Real IOShire, PShire NoC, Minion Shires stubbed
fc_pnoc_cfg = ' +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xfffffffff +define+DV_MINION_SHIRE_STUB=0xfffffffff +define+DV_MINION_SHIRE_CACHE_STUB=0xfffffffff +define+DV_MINION_SHIRE_STIM=0xfffffffff \
                +define+DV_IO_SHIRE_STUB=0 +define+DV_IO_SHIRE_STIM=0 +define+DV_PSHIRE_STUB=0 +define+DV_PSHIRE_NOC_ONLY +define+DV_IOSHIRE_NOC_ONLY \
                +define+MINION_RESETS_CONNECTED_TO_IOSHIRE=1 +define+ENABLE_ULTRASOC_SIM=1 +define+ULTRASOC_USB_USE_BFM=1 '
build_fc_pnoc_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && unset GEN_DRAL && $REPOROOT/dv/standalone/fc/rebuild -no_bemu {fc_pnoc_cfg} {compile_opts} '
build_fc_pnoc = new_build(name='fc_pnoc', run_cmd=build_fc_pnoc_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# Neighbourhood standalone (Minion stimulus agents)
build_neigh_sa_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && {compile_opts} make -f $REPOROOT/dv/standalone/neigh/Makefile all USE_MINION_STUBS=1'
build_neigh_sa = new_build(name='neigh_sa', run_cmd=build_neigh_sa_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# SYSEMU
build_sysemu_cmd = f'make -C $REPOROOT/emu/bemu/sys_emu/build'
build_sysemu_check = f'[[ -e $REPOROOT/emu/bemu/sys_emu/build/sys_emu ]] && echo pass > $TARGET || echo fail > $TARGET'
build_sysemu = new_build(name='sys_emu', run_cmd=build_sysemu_cmd, check_cmd=build_sysemu_check, slurmopts=add_build_slurm_license())

# Coverage build check
build_shire_cov_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && make ENABLE_COV=1 {compile_opts} -f $REPOROOT/dv/standalone/minion_shire/Makefile all'
build_shire_cov = new_build(name='cov_build_check', run_cmd=build_shire_cov_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())
build_shire_cov.add_test(new_test(name='dummy_cov_build_check', run_cmd='echo TEST HAS PASSED', check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}', tags=['b4c', 'b4c_stg1']))
add_build(build_shire_cov)


# DFT Builds
#

# shire_cache build
build_shire_cache_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/shire_cache/rebuild'
build_shire_cache = new_build(name='shire_cache', run_cmd=build_shire_cache_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# shire_cache ptpx build
build_shire_cache_ptpx_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/shire_cache/rebuild +define+ET_PTPX_SIM=1 +define+ET_NO_TESTEND_MONITOR'
build_shire_cache_ptpx = new_build(name='shire_cache_ptpx', run_cmd=build_shire_cache_ptpx_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# memshire build
build_memshire_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/memshire/rebuild +define+DV_MEMSHIRE_USE_PLL_BEH=0 '
build_memshire = new_build(name='memshire', run_cmd=build_memshire_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# dft build ioshire, pshire, all minion shires
# It is a full-chip build with no neighborhoods/Minions (shire channel/shire cache only).
# It uses the standard Makefile flow found in dv/standalone/fc with one exception ..
# BEMU is disabled (there is no need for it).  In addition, I added a run-time
# parameter (+assertDisable=1) which will disable all assertions.
# We don't want to include the HDL/Synopsys SV code (there are some problems
# with a lingering objection during the main phase which we have not figured out)
# and because of that there are some Xs within the IO shire that we can tolerate for now.
build_dft_33s_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} +define+STOP_COND=0 +define+PRINTF_COND=0 +define+MAXION_DISABLE_DPI=1 +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+USE_BUILD_DFT_33S +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +define+MEM_CHECK_OFF '
build_dft_33s = new_build(name='dft_33s', run_cmd=build_dft_33s_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# the following build disables all the moortech timing checks with  +notimingchecks
build_dft_33s_scan_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} VCS_DEBUG=1 +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+USE_BUILD_DFT_33S_SCAN +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +notimingchecks MICRON_LPDDR=1 +define+DV_DDR_RAM_DISABLE=0xff +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+MEM_CHECK_OFF '
build_dft_33s_scan = new_build(name='dft_33s_scan', run_cmd=build_dft_33s_scan_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# This is for the minshire gates to test scandump mode
build_dft_33s_mingates_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+USE_BUILD_DFT_33S_SCAN +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +notimingchecks MICRON_LPDDR=1 +define+DV_DDR_RAM_DISABLE=0xff +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xefffffffe +define+MEM_CHECK_OFF +define+DFT_SD_DUMP_FLOPS'
build_dft_33s_mingates = new_build(name='dft_33s_mingates', run_cmd=build_dft_33s_mingates_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# Similar to above (micron_lpddr, disable moortech checks), but also stub out ioshire so we can write ddr registers via apb
build_dft_33s_ddr_noio_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +notimingchecks MICRON_LPDDR=1 +define+DV_DDR_RAM_DISABLE=0xff +define+DV_IO_SHIRE_STUB=1 +define+DV_IO_SHIRE_STIM=1'
build_dft_33s_ddr_noio = new_build(name='dft_33s_ddr_noio', run_cmd=build_dft_33s_ddr_noio_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())


# the following build enables all the minion shires
build_dft_33s_real_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+USE_BUILD_DFT_33S_REAL +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +notimingchecks MICRON_LPDDR=1 +define+DV_DDR_RAM_DISABLE=0xff +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0 +define+MEM_CHECK_OFF '
build_dft_33s_real = new_build(name='dft_33s_real', run_cmd=build_dft_33s_real_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# the following build enables ddr gates for flyover testing
build_dft_33s_gate_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} VCS_DEBUG=1 +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS +define+USE_BUILD_DFT_33S_SCAN +define+UVM_REGEX_NO_DPI +define+DV_MEMSHIRE_USE_PLL_BEH=0 +define+MEM_CHECK_OFF +notimingchecks MICRON_LPDDR=1 +define+DV_DDR_RAM_DISABLE=0xff +define+DV_MINION_SHIRE_ALL_NEIGH_STUB=0xffffffffe  DDR_GATESIM_ENABLE=255'
build_dft_33s_gate = new_build(name='dft_33s_gate', run_cmd=build_dft_33s_gate_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# dft build, 1 minion. Some of the dft tests require debug flags for vpi functions to peek at internal values, so this build has VCS_DEBUG=1
build_dft_1s_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/dft/rebuild {compile_opts} VCS_DEBUG=1 +define+STOP_COND=0 +define+PRINTF_COND=0 +define+MAXION_DISABLE_DPI=1 +define+UVM_REGEX_NO_DPI +define+DV_MINION_SHIRE_DISABLE=0xffffffffe +define+DV_MEMSHIRE_USE_PLL_BEH=0 +define+MEM_CHECK_OFF +define+ET_USE_ASIC_MEMS'
build_dft_1s = new_build(name='dft_1s', run_cmd=build_dft_1s_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# dft build, 1 minion. PTPX mode
build_dft_1s_ptpx_cmd = build_dft_1s_cmd + f'  +define+ET_PTPX_SIM=1 '
build_dft_1s_ptpx = new_build(name='dft_1s_ptpx', run_cmd=build_dft_1s_ptpx_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# BSDL (boundary scan) build
build_dft_bscan_cmd = f'{git_sha_cmd} && cd $ETDV_RUNDIR/ && mkdir -p vbuild && $REPOROOT/dv/standalone/boundary_scan/rebuild {compile_opts} +define+UVM_REGEX_NO_DPI +define+DV_MINION_SHIRE_DISABLE=0xffffffffe +define+DV_MEMSHIRE_USE_PLL_BEH=0'
build_dft_bscan = new_build(name='dft_bscan', run_cmd=build_dft_bscan_cmd, check_cmd=standard_build_check, slurmopts=add_build_slurm_license())

# create a dummy build that doesn't do anything
build_dummy = new_build(name='dummy', run_cmd="# this is a dummy nop build", check_cmd='echo PASS > ${@}', slurmopts=add_build_slurm_license())

# Keep builds in a dict for easy access
builds = {
          'neigh':                build_neigh,
          'cpu_ss_vl':            build_cpu_ss_vl,
          'cpu_ss_vcs':           build_cpu_ss_vcs,
          'shire':                build_shire,
          'shire_1':              build_shire_1,
          'fc_shire_1':           build_fc_shire_1,
          '1s4n4t':               build_1s4n4t,
          '1s1n16t':              build_1s1n16t,
          'fc':                   build_fc,
          'fc_gls':               build_fc_gls,
          'fc_ptpx':              build_fc_ptpx,
          'fc_ddr_ptpx':          build_fc_ddr_ptpx,
          'fclite':               build_fclite,
          'fc2shire':             build_fc2shire,
          'shirex2':              build_shirex2,
          'fc3shire':             build_fc3shire,
          'fc4shire':             build_fc4shire,
          'shirex4':              build_shirex4,
          'shirex8':              build_shirex8,
          'fc_ioshire':           build_fc_ioshire,
          'fc_ioshire_io_forces': build_fc_ioshire_io_forces,
          'fc_ioshire_io_forces_real_init': build_fc_ioshire_io_forces_real_init,
          'fc_ioshire_spi':       build_fc_ioshire_spi,
          'fc_ioshire_i2c':       build_fc_ioshire_i2c,
          'fc_ioshire_debug':     build_fc_ioshire_debug,
          'fc33s_ioshire':        build_fc33s_ioshire,
          'fc4s_ioshire':         build_fc4s_ioshire,
          'fc_ioshire_real_init': build_fc_ioshire_real_init,
          'fc_ioshire_ddr_init': build_fc_ioshire_ddr_init,
          'fc_ioshire_all_mem_ddr_init': build_fc_ioshire_all_mem_ddr_init,
          'fc_ioshire_all_mem_ddr_auto_init': build_fc_ioshire_all_mem_ddr_auto_init,
          'fc_ioshire_all_mem_ddr_auto_init_ust': build_fc_ioshire_all_mem_ddr_auto_init_ust,
          'fc32s_ioshire_all_mem_ddr_auto_init': build_fc32s_ioshire_all_mem_ddr_auto_init,
          'fc_ioshire_ddr_auto_init': build_fc_ioshire_ddr_auto_init,
          'fc_ioshire_ddr_auto_init_east_side': build_fc_ioshire_ddr_auto_init_east_side,
          'fc_ioshire_pshire' :   build_fc_ioshire_pshire,
          'fc_ioshire_pshire_ddr' :   build_fc_ioshire_pshire_ddr,
          'fc_ioshire_pshire_ust':build_fc_ioshire_pshire_ust,
          'fc_ioshire_pshire_real_init' :   build_fc_ioshire_pshire_real_init,
          'fc_ioshire_pcie_ddr_real_init' :   build_fc_ioshire_pcie_ddr_real_init,
          'fc_all'            :   build_fc_all,
          'fc_all_fake_init_us'            :   build_fc_all_fake_init_us,
          'fc_full'           :   build_fc_full,
          'fc_all_mem_init'            :   build_fc_all_mem_init,
          'fc_ioshire_pshire_pciephy' : build_fc_ioshire_pshire_pciephy,
          'fc_pnoc':              build_fc_pnoc,
          'fc_ioshire_noc_remap': build_fc_ioshire_noc_remap,
          'neigh_sa':             build_neigh_sa,
          'sys_emu':              build_sysemu,
          'scnoc':                build_scnoc,
          'shiregfx':             build_shiregfx,
          'shire_cache':          build_shire_cache,
          'shire_cache_ptpx':     build_shire_cache_ptpx,
          'dft_33s':              build_dft_33s,
          'dft_33s_scan':         build_dft_33s_scan,
          'dft_33s_mingates':     build_dft_33s_mingates,
          'dft_33s_ddr_noio':     build_dft_33s_ddr_noio,
          'dft_33s_real':         build_dft_33s_real,
          'dft_33s_gate':         build_dft_33s_gate,
          'dft_1s':               build_dft_1s,
          'dft_bscan':            build_dft_bscan,
          'dft_1s_ptpx':          build_dft_1s_ptpx,
          'memshire':             build_memshire,
          'fc_ioshire_real_memshire_pll':  build_fc_ioshire_real_memshire_pll,
          'nop':  build_dummy
         }

# If the force_build option is given, modify accordingly
# Only a smaller testbench can be modified to use a bigger one
if forced_build in ['shire', 'shirex2', 'fc2shire', 'fc3shire', 'fc4shire' ,'fclite', 'fc']:
   builds['neigh']    = builds[forced_build]
if forced_build in ['shirex2', 'fc2shire', 'fc3shire', 'fc4shire', 'fclite', 'fc']:
   builds['shire']    = builds[forced_build]
if forced_build in ['fc3shire', 'fc4shire', 'fclite', 'fc']:
   builds['fc2shire'] = builds[forced_build]
if forced_build in ['fc4shire', 'fclite', 'fc']:
   builds['fc3shire'] = builds[forced_build]
if forced_build in ['fclite', 'fc']:
   builds['fc4shire'] = builds[forced_build]
if forced_build == 'fc':
   builds['fclite']   = builds[forced_build]

# For each build, replace with a dummy build if we only want to compile the test
# And finally register the build with etdv
for b in builds:
   if skip_vcs_run:
      builds[b] = new_build(name=b, run_cmd="#" + builds[b]['run_cmd'], check_cmd='echo PASS > ${@}', slurmopts=add_build_slurm_zero_license())
   add_build(builds[b])

#  _____ _____ _____ _____ _____
# |_   _|  ___/  ___|_   _/  ___|
#   | | | |__ \ `--.  | | \ `--.
#   | | |  __| `--. \ | |  `--. \
#   | | | |___/\__/ / | | /\__/ /
#   \_/ \____/\____/  \_/ \____/
#

#
# If Build Only option has been selected, just add a dummy test on each build so etdv doesn't complain
#
if build_only:
   for b in builds:
      builds[b].add_test(new_test(name='dummy', run_cmd='echo TEST HAS PASSED', check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'))
else:

   # Utility function
   # When --uarg debug is selected, this adds extra steps to all co-sim jobs.
   # The test is first run against sys_emu, and the ELF is disassembled.
   def add_debug_steps(run_cmd, elfname, args='', max_cycles='100000000', mtg_test=False):
      shires      = re.search("\+SHIRE_MASK=\S+", args)
      minions     = re.search("\+MINION_MASK=\S+", args)
      reset       = re.search("\+START_PC=\S+", args)
      threads     = re.search("\+THREAD_MASK=\S+", args)

      shire_mask  = shires.group().split('=', 1)[-1]  if shires  is not None else '0x1'
      minion_mask = minions.group().split('=', 1)[-1] if minions is not None else '0x1'
      reset_pc    = reset.group().split('=', 1)[-1]   if reset   is not None else '0x8000001000'
      thread_mask = threads.group().split('=', 1)[-1] if threads is not None else '0x3'
      single_thread = '-single_thread' if (int(thread_mask, 0) == 1) else '';
      have_master = (int(shire_mask, base=16) >> 32) & 1;

      run_cmd += 'mkdir -p $REPOROOT/emu/bemu/sys_emu/build &&\n'
      run_cmd += f'[[ -f $REPOROOT/emu/bemu/sys_emu/build/sys_emu ]] || flock $REPOROOT/emu/bemu/sys_emu/build make -C $REPOROOT/emu/bemu/sys_emu &&\n'
      if (ukernel_mode == 1) :
        run_cmd += f'$REPOROOT/emu/bemu/sys_emu/build/sys_emu {sys_emu_log} -mem_desc $ETDV_RUNDIR/mem_desc.txt -shires {shire_mask} -minions {minion_mask} {single_thread} -reset_pc {reset_pc} -max_cycles {max_cycles} > sysemu.log'
      else :
        if (have_master == 1) :
            run_cmd += f'$REPOROOT/emu/bemu/sys_emu/build/sys_emu {sys_emu_log} -elf {elfname} -shires {shire_mask} -minions {minion_mask} {single_thread} -reset_pc {reset_pc} -max_cycles {max_cycles} > sysemu.log'
        else :
            run_cmd += f'$REPOROOT/emu/bemu/sys_emu/build/sys_emu {sys_emu_log} -elf {elfname} -shires {shire_mask} -minions {minion_mask} {single_thread} -reset_pc {reset_pc} -max_cycles {max_cycles} > sysemu.log'

      if mtg_test:
         run_cmd += "\n"
      else:
         run_cmd += " &&\n"

      return run_cmd

   #--------------------------------------------------------------------------------------
   # Directed tests
   #--------------------------------------------------------------------------------------
   def overwriteNetDesc(run_cmd, test_plusargs):
      """modify elf depends on s (number of minions to use) and k (number of layers to launch)
         remove '-s X' from test_plusargs
      """

      # -k|--kernels N => run only N kernels    fixed to 1 by now
      # -s|--shires N => run only N shires
      kernel = 1
      shires     = re.search("\+SHIRE_MASK=\S+", test_plusargs)
      shire_mask = shires.group().split('=', 1)[-1]  if shires  is not None else '0x1'
      binary_compute_shire_mask = bin(int(shire_mask,16) & 0xffffffff)
      shire_count = binary_compute_shire_mask.count("1")

      # check whether we will run in PTPX
      enable_fw_shire_init = '-enable_fw_shire_init' in test_plusargs

      if kernel_count:
        kernel = int(kernel_count)

      if kernel != 0:
        k1 = ((kernel%256)<<8)
        k2 = int((kernel/256))
        k1k2 = (k1|k2)
        k3 = kernel + 1
        test_plusargs += " +CSR_STALL_DONE_COUNT=%s " % k3

      run_cmd += f'cp -f net_desc.mem net_desc.mem.orig\n'
      run_cmd += f'xxd net_desc.mem > net_desc.mem.txt\n'
      if shire_count:
         run_cmd += f'sed -e "s/0000000: ..../0000000: {format(k1k2,"04x")}/" -e "s/\(0000000: .... .... .... ....\) ../\\1 {format(int(shire_count),"02x")}/" net_desc.mem.txt > net_desc.mem.txt.new\n'
      else:
         #Shires doesn't modify it.
         run_cmd += f'sed -e "s/0000000: ..../0000000: {format(k1k2,"04x")}/" net_desc.mem.txt > net_desc.mem.txt.new\n'

      run_cmd += f'sed -i -e "s/\(0000000: .... .... .... ....\) ../\\1 {format(int(enable_fw_shire_init),"02x")}/" net_desc.mem.txt\n'
      run_cmd += f'xxd -r net_desc.mem.txt.new > net_desc.mem \n'
      run_cmd += f'rm -rf net_desc.mem.txt net_desc.mem.txt.new\n'
      #Asure in mem_desc.txt file that net_desc.mem path is pointed to the modified one instead of original where test has been launched.
      run_cmd += f'sed -e "s#\(File.*,\)\(.*\)\(/net_desc.mem\)#\\1 $ETDV_RUNDIR\\3#" mem_desc.txt > mem_desc.txt.new\n'
      run_cmd += f'cat mem_desc.txt.new > mem_desc.txt\n'
      run_cmd += f'rm -f mem_desc.txt.new\n'
      run_cmd += f'memdesc2hex >> memImage.hex\n'
      if ddr_enable:
         run_cmd += f'$REPOROOT/test/scripts/genZebuMem.pl -micron memImage.hex\n'
         test_plusargs += " +ddr_init_zero=1 +ddr_auto_init=1 +memImage=memImage +memImagePath=$ETDV_RUNDIR "
      return run_cmd, test_plusargs


   def dft_t(build_a, tags = [], plusargs = '', pre_test = '', post_test = '', test_name='', test_type='', slurm_timeout=False):
      for target_build in build_a:
         add_dft_diag(builds[target_build], test_name, test_plusargs=plusargs+' +sc_sb_en=0 +l1d_sb_en=0 +disableETLinkChecks +disableAXIChecks ', test_tags=tags, pre_test=pre_test, post_test=post_test, test_type=test_type, slurm_timeout=slurm_timeout)

   def add_dft_diag(build, test_name, test_plusargs='', test_tags=[], pre_test='', post_test='', test_type='', slurm_timeout=False):

      build_tags = ['directed']
      build_tags.extend(test_tags)
      seed = user_seed if user_seed is not None else etdv_randint() if allow_rand_seed is True else 0

      # Craft the run command
      run_cmd  = f'set -o pipefail\n'
      run_cmd += f'cd $ETDV_RUNDIR\n'
      if pre_test != '': run_cmd += f'{pre_test_args} {pre_test}  &&\n'
      run_cmd += f'echo {seed} > SEED\n'

      if not skip_vcs_run: run_cmd += f'$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {""} {common_sim_opts} \n'
      else:                run_cmd += f'#$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {""} {common_sim_opts} \n'

      if post_test != '': run_cmd += f'{post_test}\n'

      # Set the check command and add test to build
      if test_type == 'sms':
         check_cmd='@et-dvrun-check-log-v2 sms ${<}.log ${@} ${SHOW_FAIL}'
      elif test_type == 'boundary_scan':
         check_cmd='@et-dvrun-check-log-v2 boundary_scan ${<}.log ${@} ${SHOW_FAIL}'
      elif skip_vcs_run:
         check_cmd='if [ -f ${script_dir}/memImage.hex ]; then \n echo PASS > ${@} ; \n fi'
      else:
         check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
      if seed != 0:
         test_name = f'{test_name}_{seed}'

      # Slurm tiemout
      if slurm_timeout:
         slowdown_factor = 1
         if (use_xprop == True):
            slowdown_factor *= XPROP_SLOWDOWN_FACTOR

         if (is_coverage_enabled ==True or any(arg for arg in usr_args if arg.startswith('CM='))):
            slowdown_factor *= COVERAGE_SLOWDOWN_FACTOR

         if (is_bist == True):
            slowdown_factor *= BIST_SLOWDOWN_FACTOR

         run_slurm_timeout = ' --time=' + str(int(slurm_timeout) * slowdown_factor) + ' '
      else:
         run_slurm_timeout = ''

      #Add test to a build; Use same licenses for runtime as we used for a build
      run_slurm_lic_opts = str(build['slurmopts']).replace('vcscompiler','vcsruntime')
      build.add_test(new_test(name=test_name, run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags, slurmopts=build['slurmopts']+run_slurm_timeout))


   def t(name, path, build_a, tags = [], plusargs = '', pre_test = '', post_test = '', gen_mem_desc=True, ioshire_test=False, ddr_code_preload=True, ddr_auto_init=False, test_name='', diag_compile_opts='', compile_diag=True, slurm_timeout=False, test_count=1, print_enable=False):
      if 'neigh' in build_a and 'cpu_ss_vl' not in build_a:
         build_a.append('cpu_ss_vl')
      if 'neigh' in build_a and 'cpu_ss_vcs' not in build_a:
         build_a.append('cpu_ss_vcs')
      for target_build in build_a:
         if '+START_PC' not in plusargs and target_build in ['neigh', 'cpu_ss_vl', 'cpu_ss_vcs']:
            plusargs += ' +START_PC=40000000 '
         for iii in range(0, test_count):
            add_diag(builds[target_build], name, '$MINION_DIAGS/'+path, test_plusargs=plusargs, test_tags=tags, pre_test=pre_test, post_test=post_test, gen_mem_desc=gen_mem_desc, ioshire_test=ioshire_test, ddr_code_preload=ddr_code_preload, ddr_auto_init=ddr_auto_init, force_test_name=test_name, diag_compile_opts=diag_compile_opts, compile_diag=compile_diag, slurm_timeout=slurm_timeout, test_count=test_count, print_enable=print_enable)

   def add_maxion_diag(name, path, build_a, tags = [], plusargs = '', pre_test = '', post_test = '', gen_mem_desc=True, ioshire_test=False, ddr_code_preload=True, ddr_auto_init=False, test_name='', diag_compile_opts='', compile_diag=True, slurm_timeout=False, test_count=1, print_enable=False):
      for target_build in build_a:
         for iii in range(0, test_count):
            add_diag(builds[target_build], name, '$REPOROOT/'+path, test_plusargs=plusargs, test_tags=tags, pre_test=pre_test, post_test=post_test, gen_mem_desc=gen_mem_desc, ioshire_test=ioshire_test, ddr_code_preload=ddr_code_preload, ddr_auto_init=ddr_auto_init, force_test_name=test_name, diag_compile_opts=diag_compile_opts, compile_diag=compile_diag, slurm_timeout=slurm_timeout, test_count=test_count, print_enable=print_enable, maxshire_test=True)

   def add_uc_diag(build, test_name, test_src, test_plusargs='', test_tags=[], cflags = '', checker=True, compile_diag=True, pre_test='', post_test=''):
      build_tags = ['ucblock', 'directed']
      build_tags.extend(test_tags)
      seed = user_seed if user_seed is not None else etdv_randint() if allow_rand_seed is True else 0
      no_diag = (test_name == '')
      elf_name = test_src.split('/')[-1].split('.')[0]

      # Craft the run command
      run_cmd = f'set -o pipefail\n'
      run_cmd += f'cd $ETDV_RUNDIR\n'
      run_cmd += f'echo {seed} > SEED\n'
      if not no_diag: run_cmd += f'export MINION_BIN={elf_name}\n'
      run_cmd += f'cp -rf $MINION_DIAGS/{test_src} $MINION_DIAGS/ucblock/Makefile .\n'
      if pre_test != '': run_cmd += f'{pre_test} &&\n'
      if not no_diag:
         if compile_diag: run_cmd += f'make {cflags} &&\n'
         if debug_mode:   run_cmd = add_debug_steps(run_cmd, elf_name, test_plusargs)
         run_cmd += f'elf2hex {elf_name} memImage.hex && echo "ELF Load: {elf_name}" > mem_desc.txt &&\n'
         if ddr_enable:
             run_cmd += f'$REPOROOT/test/scripts/genZebuMem.pl -micron memImage.hex &&\n'
             test_plusargs += " +ddr_init_zero=1 +ddr_auto_init=1 +memImage=memImage +memImagePath=$ETDV_RUNDIR "
      if not skip_vcs_run: run_cmd += f'$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {"+CHECKER=1" if checker else ""} {common_sim_opts}\n'
      else:                run_cmd += 'echo Skip VCS\n'
      if post_test != '': run_cmd += f'{post_test}\n'
      # Set the check command and add test to build
      if skip_vcs_run:
         check_cmd='if [ -f ${script_dir}/memImage.hex ]; then \n echo PASS > ${@} ; \n fi'
      else:
         check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
      build.add_test(new_test(name=test_name, run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags))

   def check_maxshire_build(build_name):
      if any(word in build_name for word in ['maxshire']):
         return True
      else:
         return False

   def check_fc_build(build_name):
      if any(word in build_name for word in ['fc','dft','neigh','shire', 'cpu_ss_vcs', ' cpu_ss_vl']):
         return True
      else:
         return False

   #Generates hex file from ELF
   def generate_minion_hex(command, test_path, test_name, generate_mem_desc):
      #if we want to generate new mem desc, otherwisw use the one provided by the test
      if generate_mem_desc is True:
         command += f'echo "ELF Load: {test_path}/{test_name}" > mem_desc.txt &&\n'
         command += f'elf2hex {test_path}/{test_name} memImage.hex &&\n'
         command += f'ln -sfn {test_path}/{test_name} test.elf &&\n'
      else:
         command += f'elf2hex {test_path}/{test_name} memImage.hex &&\n'
         command += f'ln -sfn {test_path}/{test_name} test.elf &&\n'
         # Generate memImage.hex with mem_desc contents
         command += f'pushd {test_path} && memdesc2hex > memImage.hex && popd && cat {test_path}/memImage.hex >> memImage.hex &&\n'
         # Copy mem_desc for sys_emu
         command += f'cp {test_path}/mem_desc.txt . && cp {test_path}/*.mem . &&\n'
         # Copy the ELF file
         command += f'cp test.elf {test_name} &&\n'
      return command

   def get_load_icache_dcache_cmd(ust_extra_sim_opts: str) -> str:
      add_cmd = f'''

export ETDV_BUILD_RUNDIR=$ETDV_BUILD_RUNDIR/../../shire/build
if [ ! -f "$ETDV_BUILD_RUNDIR/status" ]; then
   # Hack to make sure the shire build finishes before the UST flow is run
   # The load_icache_dcache script depends on having the shire build.
   sleep 900
fi

cd $ETDV_RUNDIR
export MINION_BIN=finite_loop
export DIAG_DIR=$MINION_DIAGS/ultrasoc/finite_loop
rm -rf $ETDV_RUNDIR/finite_loop_minion_code &&
cp -r -L $MINION_DIAGS/ultrasoc/finite_loop finite_loop_minion_code &&
make  -C $ETDV_RUNDIR/finite_loop_minion_code &&
echo "ELF Load: $ETDV_RUNDIR/finite_loop_minion_code/finite_loop" > mem_desc.txt &&
elf2hex $ETDV_RUNDIR/finite_loop_minion_code/finite_loop memImage.hex &&
# ln -sfn $ETDV_RUNDIR/finite_loop_minion_code/finite_loop test.elf &&
export ET_UST_DVRUN=$ETDV_RUNDIR/../../..
export ET_UST_CONFIG_FILE=$ETDV_RUNDIR/config.ini

if [ -f "trace.fsdb" ]; then
   mv -f trace.fsdb orig_trace.fsdb
fi

'''
      if (debug_mode):
         add_cmd += f'''
# Running the test through the UltraSOC flow with logs and debug traces turned on
$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed=0 {ust_extra_sim_opts} +verbosity=HIGH +fsdb+struct=on +traceEnable=1 +EMU_LOG_LEVEL=0 +COSIM_LOG_LEVEL=0 +CHECKER=0 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x00000000 +l1d_sb_en=0 +neigh_sb_en=0 +py_verbose +py_test=$RTLROOT/ip/ultrasoc/scripts/b4c/bring_up/minion_shire/load_icache_dcache.py +py_start_script=$REPOROOT/test/scripts/start_ust_vcs +py_usb +py_sys=minion-shire_tb +CHECKER=1  +disable_random_data=1 +ALLOC_MEM_RUNTIME=1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +maxErrorCount=1 +SEED=0 +vcs+lic+wait fgp=single_socket_mode,dynamictoggle -reportstats +verbosity=LOW +DEFAULT_LOG_LEVEL=2 >& ust_testme.log
'''
      else:
         add_cmd += f'''
$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed=0 {ust_extra_sim_opts} +EMU_LOG_LEVEL=2 +COSIM_LOG_LEVEL=2 +CHECKER=0 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x00000000 +l1d_sb_en=0 +neigh_sb_en=0 +py_test=$RTLROOT/ip/ultrasoc/scripts/b4c/bring_up/minion_shire/load_icache_dcache.py +py_start_script=$REPOROOT/test/scripts/start_ust_vcs +py_usb +py_sys=minion-shire_tb +CHECKER=1  +disable_random_data=1 +ALLOC_MEM_RUNTIME=1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +maxErrorCount=1 +SEED=0 +vcs+lic+wait fgp=single_socket_mode,dynamictoggle -reportstats +verbosity=LOW +DEFAULT_LOG_LEVEL=2 >& ust_testme.log
'''
      return add_cmd

   def add_diag(build, diag, diag_path, test_plusargs='', test_tags=[], checker=True, compile_diag=True, diag_compile_opts='', pre_test='', post_test='', tglw=False, force_test_name='', gen_mem_desc=True, ioshire_test=False, maxshire_test=False, ddr_code_preload=True, ddr_auto_init=False, slurm_timeout=False, test_count=1, print_enable=False, mtg_test=False, mtg_conf='', mtg_args=''):
      preserved_build_name = "na"
      minion_diags = os.getenv("MINION_DIAGS")
      preserved_diag_path = diag_path
      preserved_diag_path = preserved_diag_path.replace('$MINION_DIAGS', minion_diags)
      if postsi:
         diag_flow_cpus = '--master msmin'
      if test_only:
          preserved_build_name = build['name']
          build = builds["nop"]


      build_tags = ['directed']
      build_tags.extend(test_tags)
      if (mtg_test):
         seed = etdv_randint() if user_seed is None else user_seed
      else:
         seed = user_seed if user_seed is not None else etdv_randint() if ((allow_rand_seed is True) or (test_count > 1)) else 0
      no_diag = (diag == '')
      if maxshire_test:
          test_name = diag_path.replace(('$ETDV_RUNDIR/' if tglw else '$REPOROOT/dv/tests/ioshire/'), '').replace('/ioMax', '')
      else:
          test_name = diag_path.replace(('$ETDV_RUNDIR/' if tglw else '$MINION_DIAGS/'), '').replace('/', '_')

      if not no_diag: test_name += ('_' + diag)
      if mtg_test: test_name = diag
      if force_test_name != '': test_name = force_test_name
      # Craft the run command
      run_cmd  = f'set -o pipefail\n'
      run_cmd += f'cd $ETDV_RUNDIR\n'
      run_cmd += f'echo {seed} > SEED\n'

      etdv_shire_mask = 0x1
      if ("+SHIRE_MASK" in f'{extra_sim_opts} {test_plusargs} {common_sim_opts}'):
         etdv_shire_mask = int(re.sub(r'\+SHIRE_MASK=', '', re.search("\+SHIRE_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group()), 16)
      etdv_minion_mask = int(re.sub(r'\+MINION_MASK=', '', re.search("\+MINION_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group()), 16)
      etdv_thread_mask = int(re.sub(r'\+THREAD_MASK=', '', re.search("\+THREAD_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group()), 16)

      diag_compile_opts = diag_compile_opts.replace("EXTRA_CFLAGS=","")
      diag_compile_opts = diag_compile_opts.replace("'","")
      if postsi_ust or postsi:
         diag_compile_opts = diag_compile_opts.replace("-DIOSHIRE_GATESIM", "")
         diag_compile_opts += ' -DPOSTSI_UST'

      mask_cmd = f'export EXTRA_CFLAGS="$EXTRA_CFLAGS -DETDV_SHIRE_MASK={hex(etdv_shire_mask)} -DETDV_MINION_MASK={hex(etdv_minion_mask)} -DETDV_THREAD_MASK={hex(etdv_thread_mask)}'

      etdv_lead_hart = None
      etdv_shire_harts_mask = 0x0
      _idx = 0
      while etdv_minion_mask > 0:
         if etdv_minion_mask & 0x1:
            if etdv_lead_hart == None:
               etdv_lead_hart = _idx
               if etdv_thread_mask == 0x2:
                  etdv_lead_hart += 1
            etdv_shire_harts_mask |= (etdv_thread_mask << _idx)
         _idx += 2
         etdv_minion_mask >>= 1

      mask_cmd += f' -DETDV_SHIRE_HARTS_MASK={hex(etdv_shire_harts_mask)} -DETDV_LEAD_HART={etdv_lead_hart} {diag_compile_opts}'
      run_cmd += f'{mask_cmd}"\n'

      if (mtg_test):
         if compile_diag:
            run_cmd = compile_mtg_test(run_cmd, mtg_conf, mtg_args, seed, f'{extra_sim_opts} {test_plusargs} {common_sim_opts}')
         else:
            run_cmd += "export POSTSI_CFG_RESUME_DRET=1\n"
            run_cmd += f"cp {diag_path}/{diag} base\n"
         run_cmd = postcompile_mtg_test(run_cmd)
      if not no_diag: run_cmd += f'export MINION_BIN={diag}\n'
      if (check_maxshire_build(build['name'])):
         run_cmd += f'export MAXION_SA=1\n'
         run_cmd += f'export PRINT_OPT=-DNO_PRINT\n'
         run_cmd += f'export EVL_MAXSHIRE_CHECKS=1\n'
         run_cmd += f'export SWLIB_ROOT_OBJDIR=$ETDV_BUILD_RUNDIR/swLib\n'
         run_cmd += f'export SWTC_ROOT_OBJDIR=$ETDV_RUNDIR/obj\n'
      run_cmd += f'export DIAG_DIR={diag_path}\n'
      if bringup_tgt_shires:
         run_cmd += f'export UST_TGT_SHIRES={bringup_tgt_shires}\n'
      if pre_test != '': run_cmd += f'{pre_test_args} {pre_test} &&\n'
      if not no_diag:
         if compile_diag:
            if (check_maxshire_build(build['name'])):
               run_cmd += f'rm -rf $ETDV_RUNDIR/obj $ETDV_RUNDIR/{diag} &&\n'
               run_cmd += f'mkdir -p $ETDV_RUNDIR/obj &&\n'
               run_cmd += f'cp -r -L {diag_path}/ioMax {diag_path}/*.h {diag_path}/sim/* $ETDV_RUNDIR/obj &&\n'
               diag_path = f'$ETDV_RUNDIR'
            elif (mtg_test != True):
               minion_test_build_out_of_tree =  (check_fc_build(build['name']) or check_fc_build(preserved_build_name)) and (not tglw)
               if minion_test_build_out_of_tree:
                  #print(f'Out of tree build detected: {build["name"]} test name: {test_name}')
                  #make Minion hex
                  run_cmd += f'rm -rf $ETDV_RUNDIR/{diag_path.split("/")[-1]}_minion_code &&\n'
                  run_cmd += f'cp -r -L {diag_path} {diag_path.split("/")[-1]}_minion_code &&\n'
                              #cp -r -L $MINION_DIAGS/micro_kernels/matmul_max_2s matmul_max_2s_minion_code &&
                  if ukernel_mode:
                     run_cmd += f'cp -r -L {diag_path.split("/")[-1]}_minion_code/{diag_path.split("/")[-1]} . &&\n'
                  diag_path = f'$ETDV_RUNDIR/{diag_path.split("/")[-1]}_minion_code'
            add_postsi_define = os.getenv("POSTSI_MAKE_OPTS", "")
            if postsi_ust or postsi:
               add_postsi_define = 'RAW_DATA_SIZE=64K'
            if (check_maxshire_build(build['name'])):
               run_cmd += f'make -C {diag_path}/obj sw &&\n'
               run_cmd += f'cp {diag_path}/obj/*.elf {diag} &&\n'
            elif (mtg_test != True):
               run_cmd += f'make {add_postsi_define} -C {diag_path} &&\n'
         # TODO: add 'copy part' here just for ukernel mode
         if ukernel_mode:
            run_cmd += f'cp {diag_path}/mem_desc.txt . && cp {diag_path}/*.mem . &&\n'
         if debug_mode:
            if ioshire_test is not True:
                if (mtg_test):
                   run_cmd = add_debug_steps(run_cmd, 'base', f'{extra_sim_opts} {test_plusargs}', '100000000', mtg_test=True)
                   run_cmd += 'set +x && #disable verbose\n'
                   run_cmd += 'err_msg=$(tac sysemu.log | grep -m 1 "FAIL" || true) &&\n'
                   run_cmd += 'if [[ $err_msg ]]; then echo $err_msg; else echo "TEST PASSED in sys_emu"; fi &&\n'
                   run_cmd += 'set -x && #enable  verbose\n'


                else:
                   run_cmd = add_debug_steps(run_cmd, diag_path+'/'+diag, f'{extra_sim_opts} {test_plusargs} {common_sim_opts}')
         if not tglw:
           if ddr_code_preload:
             run_cmd = generate_minion_hex(run_cmd, diag_path, diag, gen_mem_desc)
             #for uarg enable_ddr
             if ddr_enable or ddr_auto_init:
               run_cmd += f'$REPOROOT/test/scripts/genZebuMem.pl -micron memImage.hex &&\n'
               test_plusargs += " +ddr_init_zero=1 +ddr_auto_init=1 +memImage=memImage +memImagePath=$ETDV_RUNDIR "
         else:
           run_cmd += f'elf2hex $ETDV_RUNDIR/kernels memImage_kernel.hex &&\n'
           run_cmd += f'cat $ETDV_RUNDIR/firmware.hex memImage_kernel.hex > memImage.hex &&\n'

           run_cmd, test_plusargs = overwriteNetDesc(run_cmd, test_plusargs)

      if not skip_vcs_run: run_cmd += f'$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {"+CHECKER=1" if checker else ""} {common_sim_opts} \n'
      else:                run_cmd += f'#$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {"+CHECKER=1" if checker else ""} {common_sim_opts} \n'

      if (postsi_ust or postsi):
         shire_mask = '0x1'
         if ("+SHIRE_MASK" in f'{extra_sim_opts} {test_plusargs} {common_sim_opts}'):
            shire_mask = re.sub(r'\+SHIRE_MASK=', '', re.search("\+SHIRE_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group())
         minion_mask = re.sub(r'\+MINION_MASK=', '', re.search("\+MINION_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group())
         thread_mask = re.sub(r'\+THREAD_MASK=', '', re.search("\+THREAD_MASK=\S+", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}').group())
         if re.search("sp_ends_test", f'{extra_sim_opts} {test_plusargs} {common_sim_opts}') is not None:
            sp_ends_test = '--sp_ends_test'
         else:
            sp_ends_test = ''
         run_cmd += f'if [ "$1" != "skip_elf_gen" ] ; then\n'
         if postsi:
            if (shire_mask == '0x1'):
               load_stack = 1
            else:
               load_stack = 0
            run_cmd += f'minboot=`$REPOROOT/dv/tools/gen_config_ini.py --load_stack {load_stack} --gen_pc {gen_pc} --load_ddr {load_ddr} --shire_mask {shire_mask} --minion_mask={minion_mask} --thread_mask={thread_mask} {sp_ends_test}`\n'
         else:
            run_cmd += f'$REPOROOT/dv/tools/gen_config_ini.py --shire_mask {shire_mask} --minion_mask={minion_mask} --thread_mask={thread_mask}\n'
         run_cmd += f'fi\n'
         if postsi:
            #very specific case, fc test with minion side empty, which causes issues.
            #check if c, c++, asm exist in diag_path
            minion_code_exists = False
            try:
               for fname in os.listdir(preserved_diag_path):
                  if (fname.endswith((".c", ".cc", ".s", ".S"))):
                      minion_code_exists = True
                      break
            except:
                  pass

            #if "mtg_" not in diag and not minion_code_exists:
            if not mtg_test and not minion_code_exists:
                diag_flow_cpus = diag_flow_cpus.replace("--master msmin", "")
            run_cmd += f'if [ "$1" != "skip_elf_gen" ] ; then\n'
            run_cmd += f'$REPOROOT/dv/tools/gen_diag_config_ini.py {diag_flow_cpus}\n'
            run_cmd += f'fi\n'
            run_cmd += f'export ET_UST_DVRUN=$ETDV_BUILD_RUNDIR/../../../\n'
            run_cmd += f'export UST_PHASE2_CONFIG_FILE=diagflow_config.ini\n'
            run_cmd += f'export ET_UST_DVRUN=$ETDV_RUNDIR/../../..\n'
            # Ugly but works. This means we need to enable all the shire channels, irrespective of the shire_mask because the test may use the L3/scratchpad
            # for zebu we are running on 2shire model. So only enable those two shires.
            diag_flags = f' {extra_sim_opts} {test_plusargs} {common_sim_opts} '
            # minshire diags tag or noc diag tags or memshire tag
            if ('postsi_min_shire_noc_ust' in test_tags) or ('postsi_noc_ust' in test_tags) or ('memshire_bu' in test_tags):
               diag_flags = re.sub(r'\+SHIRE_MASK=\S+', '', diag_flags)
               if zebu_path_file == "none":
                  diag_flags += ' +SHIRE_MASK=0x3ffffffff '
               else:
                  diag_flags += ' +SHIRE_MASK=0x100000001 '
            # handle memshire tests
            # Always enable ddr for memshire tests
            # enable ddr for noc phase 3
            if ('postsi_noc_ust_ph3' in test_tags) or ('memshire_bu' in test_tags):
               diag_flags += ' +define+USE_REAL_DDRC '

            power_eval_str = "true" if power_eval is True else "false"

            if not skip_zebu:
               run_cmd += f'$REPOROOT/dv/tools/diag_flow/diag_flow.sh -q {power_eval_str} {diag_flow_args} -e {load_tf_elf} -v {gen_pc} -z {zebu_path_file} -f \' \"{diag_flags}\" {diag_flow_cpus} --minboot \'"${{minboot}}" \n'
            else:
               run_cmd += f'#$REPOROOT/dv/tools/diag_flow/diag_flow.sh {diag_flow_args} -e {load_tf_elf} -v {gen_pc} -z {zebu_path_file} -f \' \"{diag_flags}\" {diag_flow_cpus} --minboot \'"${{minboot}}" \n'
         else:
            # Run the load_icache_dcache.py script for non load_icache_dcache test(s)
            if (diag != 'finite_loop'):
               ust_extra_sim_opts = os.getenv("UST_EXTRA_SIM_OPTS", "")
               run_cmd += f'# This test ran with the environment variable UST_EXTRA_SIM_OPTS set to the following\n'
               run_cmd += f'# export UST_EXTRA_SIM_OPTS={ust_extra_sim_opts}\n'
               run_cmd += get_load_icache_dcache_cmd(ust_extra_sim_opts)
      if usr_args.count('do_post_test') != 0 or re.search("\+do_post_test\S+", test_plusargs):
        run_cmd += f'{post_test}\n'

      # Set the check command and add test to build
      if not (postsi and not skip_zebu):
         if skip_vcs_run:
            check_cmd='if [ -f ${script_dir}/memImage.hex ]; then \n echo PASS > ${@}; \n fi'
         else:
            check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
         if (seed != 0) and (mtg_test != True):
            test_name = f'{test_name}_{seed}'
      else:
         check_cmd=''

      #Add test to a build; Create run time licenses
      run_slurm_lic_opts = " " # No parameters needed
      run_slurm_mem_req = ' '# No big build licenses
      if get_uarg('gatesim'):
          run_slurm_mem_req=" --mem=200G"


      #Add SLURM timeout per test
      XPROP_SLOWDOWN_FACTOR = 2
      COVERAGE_SLOWDOWN_FACTOR = 4
      BIST_SLOWDOWN_FACTOR = 2

      if slurm_timeout:
         slowdown_factor = 1
         if (use_xprop == True):
            slowdown_factor *= XPROP_SLOWDOWN_FACTOR

         if (is_coverage_enabled ==True or any(arg for arg in usr_args if arg.startswith('CM='))):
            slowdown_factor *= COVERAGE_SLOWDOWN_FACTOR

         if (is_bist == True):
            slowdown_factor *= BIST_SLOWDOWN_FACTOR

         run_slurm_timeout = ' --time=' + str(int(slurm_timeout) * slowdown_factor) + ' '
      else:
         run_slurm_timeout = ''

      build.add_test(new_test(name=test_name, run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags, slurmopts=run_slurm_lic_opts+run_slurm_timeout+run_slurm_mem_req))

   def uct(name, path, build_a, tags = [], plusargs = '', cflags = '', pre_test = '', post_test = ''):
      for target_build in build_a:
         add_uc_diag(builds[target_build], name, path, test_plusargs=plusargs, test_tags=tags, cflags=cflags, pre_test=pre_test, post_test=post_test)

   def args_to_str(args):
      return args.replace('-','').replace('=','_')

   def tglw(glow_test, test_filter, build_a, tags = [], plusargs = '', pre_test = '', post_test = '', test_name='', glow_plus_args=''):
      name = test_filter.replace('.', '_').replace('/', '_')
      for target_build in build_a:
         if test_name is not None:
            post_test = f'TESTNAME={test_name} {post_test}'
         test_dir = '$ETDV_RUNDIR/'+glow_test+'/'
         if glow_plus_args is not '':
             test_dir += '/' + args_to_str(glow_plus_args)
         add_diag(builds[target_build], name, test_dir, test_plusargs=plusargs, test_tags=tags, compile_diag=False, pre_test=f'TESTNAME={glow_test} TESTFILTER={test_filter} GLOW_PLUS_ARGS={glow_plus_args} {pre_test}', post_test=post_test, tglw=True, force_test_name=test_name)

   def dbgt(test_name, us_test, build, minion_diag=None, minion_diag_path=None, tags = [], plusargs = '', post_test = '', ioshire_test=False, ddr_auto_init=False, shire_mask="", minion_mask="", thread_mask=""):
      assert build in ['shire', 'fc', 'fc_gls', 'fc_ioshire', 'fc_ioshire_pshire_pciephy', 'fc_ioshire_pshire_ust', 'fc_pnoc', 'fc_all','fc_all_fake_init_us', 'fc_all_mem_init', 'fc_ioshire_all_mem_ddr_auto_init_ust'], f'No support to add a debug test on build {build}'
      dbg_build = 'minion-shire' if build == 'shire' else 'soc'
      # If this is a Minion Shire standalone test, and no minion diag code has been specified, choose a random diag to run on a random traget minion
      diag_plusargs = ''
      diag_compile_opts = ''
      if (minion_diag is None) or (minion_diag_path is None):
         if (build is 'shire') :
            debug_diag_test = one_of(list(debug_diag_dict.keys()))
            minion_diag = debug_diag_dict[debug_diag_test]["minion_diag"]
            minion_diag_path = f"$MINION_DIAGS/" + debug_diag_dict[debug_diag_test]["minion_diag_path"]
            # BFLOOD: I am temporarily rolling this back as it introduced a bug.
            #minion_shift = random.randrange(32 - debug_diag_dict[debug_diag_test]["num_minions"])
            minion_shift = 0
            shire_mask = f"+SHIRE_MASK=" + hex(debug_diag_dict[debug_diag_test]["num_shires"])
            minion_mask = f"+MINION_MASK=" + str(hex(debug_diag_dict[debug_diag_test]["num_minions"] << minion_shift))
            thread_mask = f"+THREAD_MASK=" + hex(debug_diag_dict[debug_diag_test]["num_threads"])
            diag_plusargs = debug_diag_dict[debug_diag_test]["diag_plusargs"]
            diag_compile_opts = debug_diag_dict[debug_diag_test]["diag_compile_opts"]
         else :
            minion_diag = 'finite_loop'
            minion_diag_path = '$MINION_DIAGS/ultrasoc/finite_loop'
      build_tags = ['debug']
      build_tags.extend(tags)
      add_diag(builds[build], minion_diag, minion_diag_path, test_plusargs=f'{plusargs} {diag_plusargs} {shire_mask} {minion_mask} {thread_mask} +py_test={us_test} +py_start_script=$REPOROOT/test/scripts/start_ust_vcs +py_usb +py_sys={dbg_build}_tb', test_tags=build_tags, diag_compile_opts=diag_compile_opts, post_test=post_test, force_test_name=test_name, ioshire_test=ioshire_test,ddr_auto_init=ddr_auto_init)

   #
   # Here we read files defining the individual tests
   #
   exec(open(REPOROOT+"/dv/tests/minion_core/regress_lists/minion_diags.py").read())
   exec(open(REPOROOT+"/dv/tests/minion_core/regress_lists/compliance_diags.py").read())
   exec(open(REPOROOT+"/dv/tests/minion_core/regress_lists/mtg.etdv.py").read())

   # Add custom elfs (specified as user arguments)
   # These tests will be added to all available builds (so we need to set --build_name_enable)
   def add_custom_elf(custom_elf, checker=True):
      assert os.path.isfile(custom_elf), f'User ELF file not found: {custom_elf}'
      test_name = os.path.basename(custom_elf).replace('.elf', '')
      build_tags = ['custom_elf']
      run_cmd  = 'set -o pipefail\n'
      run_cmd += 'cd $ETDV_RUNDIR\n'
      run_cmd += f'export CUSTOM_ELF={os.path.abspath(custom_elf)}\n'
      run_cmd += 'ln -s $CUSTOM_ELF custom.elf\n'
      run_cmd += 'elf2hex custom.elf memImage.hex && echo "ELF Load: custom.elf" > mem_desc.txt &&\n'
      if skip_vcs_run:
         run_cmd += 'echo Skip VCS\n'
         check_cmd = '[ -f ${script_dir}/memImage.hex ] && echo PASS > ${@}'
      else:
         run_cmd += f'$ETDV_BUILD_RUNDIR/vbuild/simv $* {extra_sim_opts} {rand_delays} {common_sim_opts} {"+CHECKER=1" if checker else ""}\n'
         check_cmd = '@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
      for build_name, build in builds.items():
         build.add_test(new_test(name=f'custom_{test_name}', run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags))

   custom_elfs = [arg.split('=')[-1] for arg in usr_args if arg.startswith('custom_elf=')]
   for custom_elf in custom_elfs:
      add_custom_elf(custom_elf)

# print the builds and tests if user wants
lsbuildstests(builds)
