# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


sting_plusargs = ''
use_mkcosim_flow    = 0

#--------------------------------------------------------------------------
# Sting User Arguments
#--------------------------------------------------------------------------

sting_conf_vars=[]

# L2SCP Modes
#        -----------------------------------------------------------------------------------------------------
#        | L2SCP in MEM set | L2SCP with Shire Alias (0xffff)     | Include Tensor Load L2 insns             |
# ------------------------------------------------------------------------------------------------------------
# |Mode 0| YES/NO (Random)  | If L2SCP in memset, YES/NO (Random) | If L2SCP not in memset, YES otherwise NO |
# ------------------------------------------------------------------------------------------------------------
# |Mode 1| NO               | NO                                  | YES                                      |
# ------------------------------------------------------------------------------------------------------------
# |Mode 2| YES              | YES                                 | NO                                       |
# ------------------------------------------------------------------------------------------------------------
sting_l2scp_mode = 0

# Sting Zebu Hints
add_sting_zebu_hints = 0
if usr_args.count('sting_zebu_hints') != 0:
   info('Msgs-1', 'Sting Zebu Hints enabled.')
   add_sting_zebu_hints = 1

# Sting KERNEL SIZE
sting_kernel_size = ""
sting_mac_sup_arg = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_kernel_size=')]
if sting_mac_sup_arg:
   sting_kernel_size = sting_mac_sup_arg[0]
   info('Msgs-1', f'Sting MAC/SUP Kernel size: {sting_kernel_size}')

# Sting USER SIZE
sting_user_size = ""
sting_user_arg = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_user_size=')]
if sting_user_arg:
   sting_user_size = sting_user_arg[0]
   info('Msgs-1', f'Sting User Code size: {sting_user_size}')

# Sting KERNEL SLICESIZE
sting_kernel_slice_size = ""
sting_kernel_slice_arg = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_kernel_slice_size=')]
if sting_kernel_slice_arg:
   sting_kernel_slice_size = sting_kernel_slice_arg[0]
   info('Msgs-1', f'Sting Kernel Slice size: {sting_kernel_slice_size}')

# Sting Rand LOOP SIZE
sting_loop_size = ""
sting_loop_size_arg = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_rand_loop_size=')]
if sting_loop_size_arg:
   sting_loop_size = sting_loop_size_arg[0]
   info('Msgs-1', f'Sting Rand test loop size: {sting_loop_size}')

# Sting Silicone Mode
sting_silicon_mode = False
if usr_args.count('sting_silicon_mode') != 0:
   info('Msgs-1', 'Sting will run in Silicone Mode.')
   sting_silicon_mode = True

# Sting Signature Checkpoint Rate
sting_checkpoint_rate = int(100)
sting_checkpoint_arg = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_checkpoint_rate=')]
if sting_checkpoint_arg:
   assert sting_silicon_mode == True,"Silicon Mode not enabled and it is required for the checkpoint snippet (--uarg sting_silicon_mode)"
   sting_checkpoint_rate = int(sting_checkpoint_arg[0])
   info('Msgs-1', 'Sting checkpoint rate is 1 checkpoint every '+str(sting_checkpoint_rate))

if sting_silicon_mode:
   sting_num_ops_mult = float(sting_num_ops_mult) / float(sting_checkpoint_rate)

# Sting mkcosim flow (Pre-generate randomized values for reduced boot time)
if usr_args.count('sting_mkcosim') != 0:
   info('Msgs-1', 'Sting mkcosim flow enabled.')
   use_mkcosim_flow = 1

# Sting MKCosim Pre-randomization
if usr_args.count('sting_mkcosim_prerand') != 0:
   info('Msgs-1', 'Sting mkcosim pre-randomization flow enabled.')
   use_mkcosim_flow = 1

# Sting MKCosim Checking Flow
sting_mkcosim_checking = False
if usr_args.count('sting_mkcosim_checking') != 0:
   info('Msgs-1', 'Sting will use Cosim Checking flow.')
   sting_mkcosim_checking = True
   assert sting_silicon_mode == True,"Silicon Mode not enabled and it is required for Cosim Checking (--uarg sting_silicon_mode)"
   assert sting_num_tests <= 10, "Sting MKCosim check only supports up to 10 tests"

# Sting Debug Mode for MKCOSIM
sting_debug = False
if usr_args.count('sting_debug') != 0:
   info('Msgs-1', 'Sting will create aditional files and folders for helping Valtrix debug issues')
   sting_debug = True
   assert use_mkcosim_flow == 1 or sting_mkcosim_checking == 1, "Sting Debug only applies to MKCosim flows"

# Sting Alternative Memory Map
sting_start_pc = 0x8100000000
sting_workarounds = 'conf.d/platform/rv64_triton/workarounds.conf'
sting_memory_map = usr_args.count('sting_memory_map') != 0
if sting_memory_map:
   info('Msgs-1', f'Sting Memory Map enabled.')
   sting_start_pc = 0x8000000000
   sting_workarounds = 'conf.d/platform/rv64_triton/workarounds_v2.conf'
   sting_plusargs += '+FESB_FETCH_PROTECTION_DISABLE'
   sting_num_ops_mult = float(sting_num_ops_mult) * 0.25

# Sting N_PASS_CHECK
sting_n_pass_check = False
if usr_args.count('sting_n_pass_check') != 0:
   info('Msgs-1', 'Sting will enable N PASS CHECK')
   sting_n_pass_check = True
   assert sting_silicon_mode == True,"Silicon Mode not enabled and it is required for N PASS Check (--uarg sting_silicon_mode)"
   assert int(sting_num_test_iters) > 1,"Sting Number of iterations is "+str(sting_num_test_iters)+". N PASS Check requires at least 2 iterations (e.g. --uarg sting_num_test_iters=2)"

# Sting AVOW tensor
sting_avow_tensors = False
if usr_args.count('sting_avow_tensors') != 0:
   info('Msgs-1', 'Enable generation of filtered (only tensors) sys_emu log for AVOW')
   assert debug_mode == True, "sys_emu execution is not enabled (--uarg debug)"
   assert usr_args.count('sys_emu_quiet') == 0, "sys_emu log (verbosity) is disabled and it is required for AVOW."
   sting_avow_tensors = True

# Sting MTL
sting_mtl_build = False
if usr_args.count('sting_mtl_build') != 0:
   info('Msgs-1', 'Enable alternative Sting build with MTL support (base_onlg.conf)')
   sting_mtl_build = True

sting_enable_mtl = False
if usr_args.count('sting_enable_mtl') != 0:
   info('Msgs-1', 'The parameter STING_CONF.ENABLE_MTL controls whether the co-simulation is required to be done with MTL APIs or not')
   assert sting_mtl_build == True, "The 'STING_CONF.ENABLE_MTL' requires Sting MTL Build (--uarg sting_mtl_build)"
   sting_enable_mtl = True

sting_mtl_lib_path = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_mtl_lib_path=')]
if sting_mtl_lib_path:
   sting_mtl_lib_path = sting_mtl_lib_path[0]
   info('Msgs-1', f'Setting MTL_LIB_PATH to {sting_mtl_lib_path}')

# Sting version
sting_root_base = '/eng/dvtools/release/sting/minion'
sting_version = os.getenv('STING_VERSION') # Use default global version

# Parse uargs for custom version
version_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('sting_version=')]
if version_opt:
   sting_version = version_opt[0]
   info('Msgs-1', f'Using STING version {sting_version}')


sting_root = os.path.join(sting_root_base, sting_version)
assert os.path.isdir(sting_root), f'STING version {sting_version} does not exist'

#--------------------------------------------------------------------------
# Sting Random tests
#--------------------------------------------------------------------------

sting_plusargs += f' +WFI_ENDS_TEST=0 +ECALL_ENDS_TEST=0 +START_PC=0x{sting_start_pc:x} +clk_gate_random '
sting_common_args = f'STING_CONF.FLEXLM_WAIT_FOR_LIC_CHKOUT=1 BOARD_CONF.DEVICES.RAMS[0].BASE_ADDR=0x8100000000UL override_conf={sting_workarounds}'

if sting_kernel_size != "":
   sting_common_args += f' KERNEL_PARAMS.RV_MAC_KERNEL_SIZE={sting_kernel_size}'
   sting_common_args += f' KERNEL_PARAMS.RV_SUP_KERNEL_SIZE={sting_kernel_size}'
if sting_user_size != "":
   sting_common_args += f' KERNEL_PARAMS.RV_USER_CODE_SIZE={sting_user_size}'
if sting_kernel_slice_size != "":
   sting_common_args += f' KERNEL_PARAMS.SLICE_SIZE={sting_kernel_slice_size}'
if sting_loop_size != "":
   sting_common_args += f' TEST_PROPS.EN_TEST_LOOP=1 TEST_PROPS.NUM_TEST_LOOPS=1 ET_TEST_ENV.TEST_LOOP_SIZE={sting_loop_size}'
if sting_memory_map:
   sting_common_args += f' KERNEL_PARAMS.MEMORY_MAP=1'
if sting_n_pass_check:
   sting_common_args += f' KERNEL_PARAMS.USE_TB_MEM_RANDOMIZATION=0'

sting_saved_test_dir = MINION_DIAGS + '/sting/saved_elfs'
saved_tests = [t for t in os.listdir(sting_saved_test_dir) if t.endswith('.elf')]
for t in saved_tests:
   add_diag(builds['1s1n16t'], t, sting_saved_test_dir, force_test_name=t.replace('.elf', ''), test_plusargs=sting_plusargs + ' +thread_timeout=150000 +trans_timeout=60000 +THREAD_MASK=0x3 +MINION_MASK=0xff', test_tags=['saved_sting', 'b4c_stg3','minion_integration'], compile_diag=False)

# Tests run on SYSEMU only - 64 threads
def add_sting_test_sysemu(test_name):
   build_tags = ['random', 'sting']
   seed = etdv_randint() if user_seed is None else user_seed
   # Craft the run command
   run_cmd  = f'set -o pipefail\n'
   run_cmd += f'cd $ETDV_RUNDIR\n'
   run_cmd += f'export STING_ROOT={sting_root}\n'
   run_cmd += f'export STING_BOARD_CONF=$STING_ROOT/conf.d/platform/rv64_triton/64c_board.conf\n'
   run_cmd += f'echo {seed} > SEED\n'
   run_cmd += f'$STING_ROOT/mksting seed={seed} conf=$STING_ROOT/regress.d/confs/riscv/regress_base.conf cgf=1 {sting_common_args} &&\n'
   run_cmd += f'$REPOROOT/emu/bemu/sys_emu/build/sys_emu -reset_pc 0x{sting_start_pc:x} -elf build/sting.elf -m -minions 0xffffffff'
   # Set the check command and add test to build
   check_cmd='grep "Test Status: PASS" ${<}.log && echo pass > $TARGET || echo fail > $TARGET'
   builds['sys_emu'].add_test(new_test(name=test_name, run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags))

for ii in range(0,10):
   add_sting_test_sysemu(f'sting_test_sysemu_{ii}')

def add_build_sysemu(run_cmd):
   run_cmd += 'mkdir -p $REPOROOT/emu/bemu/sys_emu/build\n'
   run_cmd += f'[[ -f $REPOROOT/emu/bemu/sys_emu/build/sys_emu ]] || flock $REPOROOT/emu/bemu/sys_emu/build make -C $REPOROOT/emu/bemu/sys_emu &&\n'
   return run_cmd

def get_shires_mask(args=''):
   shires      = re.search("\+SHIRE_MASK=\S+", args)
   shire_mask  = shires.group().split('=', 1)[-1]  if shires  is not None else '0x1'
   return shire_mask

def get_sysemu_mkcosim_args(args=''):
   shires      = re.search("\+SHIRE_MASK=\S+", args)
   minions     = re.search("\+MINION_MASK=\S+", args)
   reset       = re.search("\+START_PC=\S+", args)
   threads     = re.search("\+THREAD_MASK=\S+", args)

   shire_mask  = shires.group().split('=', 1)[-1]  if shires  is not None else '0x1'
   minion_mask = minions.group().split('=', 1)[-1] if minions is not None else '0x1'
   reset_pc    = reset.group().split('=', 1)[-1]   if reset   is not None else '0x8000001000'
   thread_mask = threads.group().split('=', 1)[-1] if threads is not None else '0x3'
   single_thread = '-single_thread' if (int(thread_mask, 0) == 1) else '';

   run_args = f'-shires {shire_mask} -minions {minion_mask} {single_thread} -reset_pc {reset_pc} -max_cycles 1000000000000'
   return run_args

# Tests run on cosim
def add_sting_test(build, test_name, sting_args='', test_plusargs='', test_tags=[], checker=True, sting_conf=f'$MINION_DIAGS/sting/conf/regress/rand/rand_vcs.conf', board_conf='1c', sting_conf_vars=[]):
   # Sting Rand + Zebu handling
   matching_tags = [s for s in test_tags if "sting_rand" in s]
   if ((len(matching_tags) != 0) and (add_sting_zebu_hints == 1)):
      sting_conf=f'$MINION_DIAGS/sting/conf/regress/rand/rand_zebu.conf'

   num_harts = int(board_conf[:-1])
   num_minions = max(int(num_harts/2), 1)
   sting_args = sting_args + " ET_TEST_ENV.NUM_MINIONS="+str(num_minions)
   assert os.path.isfile(f'{STING_ROOT}/conf.d/platform/rv64_triton/{board_conf}_board.conf'), f'STING configuration file not found: {STING_ROOT}/conf.d/platform/rv64_triton/{board_conf}_board.conf'
   build_tags = ['random', 'sting']
   build_tags.extend(test_tags)
   seed = etdv_randint() if user_seed is None else user_seed
   test_name = f'{test_name}_{seed}'
   # Craft the run command
   run_cmd  = f'set -o pipefail\n'
   run_cmd += f'cd $ETDV_RUNDIR\n'
   run_cmd += f'export STING_ROOT={sting_root}\n'
   run_cmd += f'export STING_BOARD_CONF=$STING_ROOT/conf.d/platform/rv64_triton/{board_conf}_board.conf\n'
   run_cmd += f'export STING_TEST_PWD=$(pwd)\n'
   if sting_mtl_lib_path != '':
      run_cmd += f'export MTL_LIB_PATH=\'{sting_mtl_lib_path}\'\n'

   if sting_mtl_build:
      run_cmd += f'export STING_BASE="conf.d/platform/rv64_triton/base_onlg.conf"\n'
      assert sting_memory_map == 0, "Sting MTL build does not support secure memory."
   else:
      if sting_memory_map:
         run_cmd += f'export STING_BASE="conf.d/platform/rv64_triton/base_v2.conf"\n'
      else:
         run_cmd += f'export STING_BASE="conf.d/platform/rv64_triton/base.conf"\n'

   if (sting_mkcosim_checking and (sting_num_tests > 1)):
      assert sting_mtl_build == True, "MKCosim check with more than 1 test is only supported in Sting MTL build (--uarg sting_mtl_build)"

   # Override STING_BASE if secure memory map is enabled
   if sting_memory_map:
      run_cmd += f'export STING_BASE="conf.d/platform/rv64_triton/base_v2.conf"\n'

   run_cmd += "mkdir -p build &&\n"
   # Generate --uarg Sting Env Variables
   run_cmd += f'echo "#Auto-generated file based on the --uarg commands passed to et-dvrun" > $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # Loop Size
   if sting_loop_size != "":
      run_cmd += f'echo "TEST_PROPS.EN_TEST_LOOP=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
      run_cmd += f'echo "TEST_PROPS.NUM_TEST_LOOPS=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
      run_cmd += f'echo "ET_TEST_ENV.TEST_LOOP_SIZE={sting_loop_size}" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # Silicon Mode
   if (sting_silicon_mode == True):
      run_cmd += f'echo "ET_TEST_ENV.SILICON_MODE=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
      run_cmd += f'echo "KERNEL_PARAMS.STACK_SIZE=0x2000" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
      run_cmd += f'echo "ET_TEST_ENV.CHECKPOINT_RATE={sting_checkpoint_rate}" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   else:
      run_cmd += f'echo "ET_TEST_ENV.SILICON_MODE=0" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # Zebu Hints
   run_cmd += f'echo "ET_TEST_ENV.EOT_ZEBU_HINTS={add_sting_zebu_hints}" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # Memory Map
   if (sting_memory_map):
      run_cmd += f'echo "ET_TEST_ENV.SECURE_MEMORY=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   else:
      run_cmd += f'echo "ET_TEST_ENV.SECURE_MEMORY=0" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # N PASS CHECKING
   if sting_n_pass_check:
      run_cmd += f'echo "STING_CONF.ENABLE_N_PASS_CHECKING=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
      run_cmd += f'echo "STING_CONF.ENABLE_SNIP_MEM_N_PASS_CHECKING =1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   else:
      run_cmd += f'echo "STING_CONF.ENABLE_N_PASS_CHECKING=0" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   run_cmd += f'echo "ET_TEST_ENV.DVFS_PLL_HART=0" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   # MTL
   if sting_enable_mtl:
      run_cmd += f'echo "STING_CONF.ENABLE_MTL=1" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'
   else:
      run_cmd += f'echo "STING_CONF.ENABLE_MTL=0" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'

   # Sting Conf Variables coming from #ETDV_* tags
   for ETDV_TAG in sting_conf_vars:
      run_cmd += f'echo "{ETDV_TAG}" >> $STING_TEST_PWD/build/et_generated_sting_uargs.conf\n'

   # Generate Minion Shire Memory Set
   build_shire_mask = get_shires_mask(test_plusargs)
   run_cmd += f'$MINION_DIAGS/sting/conf/utils/gen_min_shire_mem_set.py {build_shire_mask} > $STING_TEST_PWD/build/et_generated_minion_shire_mem_set.conf\n'
   run_cmd += f'echo {seed} > SEED\n'
   if (use_mkcosim_flow or sting_mkcosim_checking):
      run_cmd  = add_build_sysemu(run_cmd)
      sting_sysemu_args = get_sysemu_mkcosim_args(test_plusargs)
      mkcosim_helper_arg = 0;
      if use_mkcosim_flow:
         mkcosim_helper_arg = mkcosim_helper_arg | 1
      if sting_mkcosim_checking:
         mkcosim_helper_arg = mkcosim_helper_arg | 2
      run_cmd += f'$REPOROOT/dv/tests/minion_core/c_asm_tests/sting/conf/mkcosim/helper_creator.py "{sting_args} {sting_common_args}" "{sting_sysemu_args}" {mkcosim_helper_arg} > build/mkcosim_helper.py &&\n'
      if sting_debug == False:
         run_cmd += f'$STING_ROOT/mkcosim helper=build/mkcosim_helper.py conf={sting_conf} bd=build seed_arg={seed} &&\n'
      else :
         run_cmd += f'$STING_ROOT/mkcosim_debug.py helper=build/mkcosim_helper.py conf={sting_conf} bd=build seed_arg={seed} full_backup=1 &&\n'
      run_cmd += 'set +x && #disable verbose\n'
      run_cmd += f'err_msg=$(cat build/uart.output | grep -m1 "ERROR\|FAIL" || true) &&\n'
      run_cmd += f'if [[ $err_msg ]]; then echo $err_msg; exit 1; fi &&\n'
      run_cmd += 'set -x && #enable  verbose\n'
   else:
      run_cmd += f'$STING_ROOT/mksting seed={seed} conf={sting_conf} cgf=1 {sting_args} {sting_common_args} &&\n'
   if debug_mode:
      sys_emu_max_cycles='100000000'
      if (sting_silicon_mode == True):
         sys_emu_max_cycles='10000000000'
      run_cmd = add_debug_steps(run_cmd, 'build/sting.elf', test_plusargs, sys_emu_max_cycles)
      run_cmd += 'set +x && #disable verbose\n'
      run_cmd += 'err_msg=$(tac sysemu.log | grep -m 1 "FAIL" || true) &&\n'
      run_cmd += 'if [[ $err_msg ]]; then echo $err_msg; else echo "TEST PASSED in sys_emu"; fi &&\n'
      run_cmd += 'set -x && #enable  verbose\n'
      if sting_avow_tensors:
         run_cmd += '$MINION_DIAGS/sting/tools/cut_avow_tensors.py sysemu.log &&\n'
         run_cmd += '$REPOROOT/dv/tools/avow/bin/avow.sh generate -l avow.sysemu.log -r et-tensor.yml > avow.log &&\n'
         run_cmd += '$REPOROOT/dv/tools/avow/bin/avow.sh report -c et-tensor-coverage.yml &&\n'
   run_cmd += f'elf2hex build/sting.elf memImage.hex && echo "ELF Load: build/sting.elf" > mem_desc.txt &&\n'
   if ddr_enable:
        run_cmd += f'$REPOROOT/test/scripts/genZebuMem.pl -micron memImage.hex &&\n'
        test_plusargs += " +ddr_init_zero=1 +ddr_auto_init=1 +memImage=memImage +memImagePath=$ETDV_RUNDIR "
   if not skip_vcs_run: run_cmd += f'$ETDV_BUILD_RUNDIR/vbuild/simv $* +ntb_random_seed={seed} {extra_sim_opts} {test_plusargs} {rand_delays} {common_sim_opts} {"+CHECKER=1" if checker else ""} \n'
   else:                run_cmd += 'echo Skip VCS\n'
   # Set the check command and add test to build
   if skip_vcs_run:
      if debug_mode:
         check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
      else:
         check_cmd='if [ -f ${script_dir}/memImage.hex ]; then \n echo PASS > ${@} ; \n fi'
   else:
      check_cmd='@et-dvrun-check-log-v2 test ${<}.log ${@} ${SHOW_FAIL}'
   build.add_test(new_test(name=test_name, run_cmd=run_cmd, check_cmd=check_cmd, tags=build_tags))

#------------------------------------------------ Sting configurations -------------------------------------------------
weekly_configs = {
   '2t': {
      'threads'    : 2,
      'build'      : '1s1n16t',
      'seeds'      : 1,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=50000 +THREAD_MASK=0x3 +MINION_MASK=0x1 ',
      'sting_args' : ''
      }
}

et_sting_configs = {
   '1t': {
      'threads'    : 1,
      'build'      : '1s1n16t',
      'seeds'      : 10,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x1 +MINION_MASK=0x1 ',
      'sting_args' : ''
   },

   '2t': {
      'threads'    : 2,
      'build'      : '1s1n16t',
      'seeds'      : 10,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0x1 ',
      'sting_args' : ''
   },

   '4t': {
      'threads'    : 4,
      'build'      : '1s1n16t',
      'seeds'      : 7,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=100000 +trans_timeout=80000 +THREAD_MASK=0x3 +MINION_MASK=0x3 ',
      'sting_args' : ''
   },

   '16t': {
      'threads'    : 16,
      'build'      : '1s1n16t',
      'seeds'      : 5,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=250000 +trans_timeout=150000 +THREAD_MASK=0x3 +MINION_MASK=0xff ',
      'sting_args' : ''
   },

   '64t': {
      'threads'    : 64,
      'build'      : 'shire',
      'seeds'      : 3,
      'ops_x'      : .5,
      'plusargs'   : ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff ',
      'sting_args' : ''
   },

   '128t': {
      'threads'    : 128,
      'build'      : 'shirex2',
      'seeds'      : 2,
      'ops_x'      : .1,
      'plusargs'   : ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3 ',
      'sting_args' : ''
   },

   '256t': {
      'threads'    : 256,
      'build'      : 'shirex4',
      'seeds'      : 1,
      'ops_x'      : .15,
      'plusargs'   : ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xf ',
      'sting_args' : ''
   },

   '512t': {
      'threads'    : 512,
      'build'      : 'shirex8',
      'seeds'      : 1,
      'ops_x'      : .1,
      'plusargs'   : ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xff ',
      'sting_args' : ''
   },

   '2048t': {
      'threads'    : 2048,
      'build'      : 'fc',
      'seeds'      : 0,
      'ops_x'      : .05,
      'plusargs'   : ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xffffffff ',
      'sting_args' : ''
   },

   '1s4n4t': {
      'threads'    : 16,
      'build'      : '1s4n4t',
      'seeds'      : 5,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=350000 +trans_timeout=350000 +THREAD_MASK=0x3 +MINION_MASK=0x03030303 ',
      'sting_args' : 'BOARD_CONF.LCPUID_TO_PCPUID_MAP=[0,1,2,3,16,17,18,19,32,33,34,35,48,49,50,51]'
   },

   '2s1n8t': {
      'threads'    : 16,
      'build'      : 'shirex2',
      'seeds'      : 5,
      'ops_x'      : 1,
      'plusargs'   : ' +thread_timeout=200000 +trans_timeout=300000  +THREAD_MASK=0x3 +MINION_MASK=0x0000000f +SHIRE_MASK=0x3 ',
      'sting_args' : 'BOARD_CONF.CLUSTERS=CLUSTER_CONF.C2 BOARD_CONF.LCPUID_TO_PCPUID_MAP=[0,1,2,3,4,5,6,7,64,65,66,67,68,69,70,71]'
   },

   '4s4n4t': {
      'threads'    : 64,
      'build'      : 'shirex4',
      'seeds'      : 5,
      'ops_x'      : .5,
      'plusargs'   : ' +thread_timeout=250000 +trans_timeout=400000  +THREAD_MASK=0x3 +MINION_MASK=0x03030303 +SHIRE_MASK=0xf ',
      'sting_args' : 'BOARD_CONF.CLUSTERS=CLUSTER_CONF.C4 BOARD_CONF.LCPUID_TO_PCPUID_MAP=[0,1,2,3,16,17,18,19,32,33,34,35,48,49,50,51,64,65,66,67,80,81,82,83,96,97,98,99,112,113,114,115,128,129,130,131,144,145,146,147,160,161,162,163,176,177,178,179,192,193,194,195,208,209,210,211,224,225,226,227,240,241,242,243]'
   },
}

build_maxharts = { 'neigh':   16,
                   '1s1n16t': 16,
                   '1s4n4t':  16,
                   'shire':   64,
                   'shirex2': 128,
                   'shirex4': 256,
                   'shirex8': 512,
                   'fc':      2048
                 }

#----------------------------------------- No bias file, everything included -------------------------------------------
sting_options = f'STING_CONF.NUM_TESTS={sting_num_tests} STING_CONF.NUM_TEST_ITERS={sting_num_test_iters}'
base_num_ops = 10000 * sting_num_ops_mult
sargs = f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS={int(10000 * sting_num_ops_mult)} {sting_options}'
for ii in range(0,25):
   add_sting_test(builds['1s1n16t'], f'sting_test_1t_{ii}',   test_tags=['sting_rand', 'sting_rand_1t'],   board_conf='1c',   sting_args=sargs, test_plusargs=(sting_plusargs + ' +thread_timeout=100000 +THREAD_MASK=0x1 +MINION_MASK=0x1 '))
   add_sting_test(builds['1s1n16t'], f'sting_test_2t_{ii}',   test_tags=['sting_rand', 'sting_rand_2t'],   board_conf='2c',   sting_args=sargs, test_plusargs=(sting_plusargs + ' +thread_timeout=100000 +THREAD_MASK=0x3 +MINION_MASK=0x1 '))
   add_sting_test(builds['1s1n16t'], f'sting_test_4t_{ii}',   test_tags=['sting_rand', 'sting_rand_4t'],   board_conf='4c',   sting_args=sargs, test_plusargs=(sting_plusargs + ' +thread_timeout=100000 +THREAD_MASK=0x3 +MINION_MASK=0x3 '))
   add_sting_test(builds['1s1n16t'], f'sting_test_16t_{ii}',  test_tags=['sting_rand', 'sting_rand_16t'],  board_conf='16c',  sting_args=sargs, test_plusargs=(sting_plusargs + ' +thread_timeout=350000 +trans_timeout=350000  +THREAD_MASK=0x3 +MINION_MASK=0xff '))
for ii in range(0,15):
   add_sting_test(builds['shire'],   f'sting_test_64t_{ii}',  test_tags=['sting_rand', 'sting_rand_64t'],  board_conf='64c',  sting_args=f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS=' + str(int(base_num_ops * et_sting_configs['64t']['ops_x'])) + f' {sting_options}',
                  test_plusargs=(sting_plusargs + ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff '))
for ii in range(0,5):
   add_sting_test(builds['shirex2'], f'sting_test_128t_{ii}', test_tags=['sting_rand', 'sting_rand_128t'], board_conf='128c', sting_args=f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS=' + str(int(base_num_ops * et_sting_configs['128t']['ops_x'])) + f' {sting_options}', 
                  test_plusargs=(sting_plusargs + ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3 '))

for ii in range(0,5):
   add_sting_test(builds['shirex4'], f'sting_test_256t_{ii}', test_tags=['sting_rand', 'sting_rand_256t'], board_conf='256c', sting_args=f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS=' + str(int(base_num_ops * et_sting_configs['256t']['ops_x'])) + f' {sting_options}', 
                  test_plusargs=(sting_plusargs + ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xf '))

for ii in range(0,5):
   add_sting_test(builds['shirex8'], f'sting_test_512t_{ii}', test_tags=['sting_rand', 'sting_rand_512t'], board_conf='512c', sting_args=f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS=' + str(int(base_num_ops * et_sting_configs['512t']['ops_x'])) + f' {sting_options}', 
                  test_plusargs=(sting_plusargs + ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xff '))

for ii in range(0,5):
   add_sting_test(builds['fc'], f'sting_test_2048t_{ii}', test_tags=['sting_rand', 'sting_rand_2048t'], board_conf='2048c', sting_args=f'CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS=' + str(int(base_num_ops * et_sting_configs['2048t']['ops_x'])) + f' {sting_options}', 
                  test_plusargs=(sting_plusargs + ' +thread_timeout=800000 +trans_timeout=800000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xffffffff '))


# Sparse build
for ii in range(0,25):
   add_sting_test(builds['1s4n4t'], f'sting_test_1s4n4t_{ii}', test_tags=['sting_rand', 'sting_rand_1s4n4t'], board_conf='16c',
                  sting_args=sargs + ' ' + et_sting_configs['1s4n4t']['sting_args'],
                  test_plusargs=sting_plusargs + ' ' + et_sting_configs['1s4n4t']['plusargs'])


#---------------------- Confs from package (RISCV only, and ET focused) and repo (ET only) -----------------------------
et_snip_dir      = MINION_DIAGS + '/sting/conf/regress'
valtrix_snip_dir = STING_ROOT   + '/regress.d/confs/riscv'
assert os.path.isdir(et_snip_dir),      f'couldn\'t find sting regression configuration files at {et_snip_dir}'
assert os.path.isdir(valtrix_snip_dir), f'couldn\'t find valtrix regression configuration files at {valtrix_snip_dir}'
excluded_confs = ['fcc_sync_threads_eot.conf', 'fcc_sync_threads.conf', 'fcc_wait_for_master.conf']

StingTestTemplate = namedtuple('StingTestTemplate', [
   'build',
   'test_name',
   'board_conf',
   'test_tags',
   'test_plusargs',
   'sting_args',
   'sting_conf',
   'sting_conf_vars'
])
sting_test_templates = []

# Go through both the package configs and repo configs
for root_dir in [valtrix_snip_dir, et_snip_dir]:
   for dir, sub_dir, files in os.walk(root_dir):
      # Categorize configs in two buckets
      #   - ET tests are the ones from repo and 'custom/triton' directory, and are run in sting_dev regression
      #   - Other tests are RISC-V only and are run in sting_regress_weekly
      et_test = (root_dir == et_snip_dir) or ('custom/triton' in dir)
      for f in [f for f in files if (f.endswith('.conf') and not f in excluded_confs)]:
         conf_plusargs   = ''
         base_tag        = 'sting_dev'  if et_test else 'sting_regress_weekly'
         sting_builds    = et_sting_configs if et_test else weekly_configs
         conf_name       = f'{dir}/{f.replace(".conf", "")}'.replace(valtrix_snip_dir+'/', '').replace(et_snip_dir+'/', '').replace('/', '_')
         conf_path       = os.path.join(dir, f)
         test_multiplier = 1
         conf_num_ops    = 10000
         min_harts       = 0
         max_harts       = 4096
         skip_build      = ''
         et_dvrun_tags   = ['sting_dev_regress']
         sting_conf_vars = []
         # Go through the config in case we need to alter some test characteristics
         try:
            for line in open(conf_path):

               #########################################
               ##            L2SCP MODES              ##
               #########################################

               if 'ETDV_L2SCP_MODE' in line:
                  exec(line.replace('#', '').lstrip())
                  sting_l2scp_mode = ETDV_L2SCP_MODE
                  assert((sting_l2scp_mode >= 0) and (sting_l2scp_mode <= 2))
                  sting_conf_vars += [f'ET_TEST_ENV.L2SCP_MODE={sting_l2scp_mode}']

               #########################################
               ##            SILICON TAGS             ##
               #########################################

               if 'ETDV_SILICON' in line:
                  et_dvrun_tags = ['sting_silicon_test']
                  if 'ETDV_SILICON_SELFCHECK' in line:
                     et_dvrun_tags += ['sting_silicon_selfcheck']
                  if 'ETDV_SILICON_ON_TARGET_GEN' in line:
                     et_dvrun_tags += ['sting_silicon_on_target_gen']

                  # If no uarg overriding silicon_mode is passed, we set it to 1
                  if usr_args.count('sting_silicon_mode') == 0:
                     sting_conf_vars += ['ET_TEST_ENV.SILICON_MODE=1']

               #########################################

               if 'ETDV_DVFS' in line:
                  et_dvrun_tags = ['sting_dvfs']

               if 'ETDV_SEQUENCE' in line:
                  et_dvrun_tags += ['sting_sequence_tests']
               # Only add if DEV tag applies
               if 'ETDV_DEV' in line:
                  et_dvrun_tags += ['sting_not_in_rand']
               # Check if any builds are disabled for this conf
               if 'ETDV_SKIP_BUILD' in line:
                  exec(line.replace('#', '').lstrip())
                  skip_build = ETDV_SKIP_BUILD
               # Run only on builds that have >= #HARTs than specified
               if 'ETDV_MIN_HARTS' in line:
                  exec(line.replace('#', '').lstrip())
                  min_harts = ETDV_MIN_HARTS
               # Run only on builds that have <= #HARTs than specified
               if 'ETDV_MAX_HARTS' in line:
                  exec(line.replace('#', '').lstrip())
                  max_harts = ETDV_MAX_HARTS
               # Add specific plusargs to the VCS command line
               if 'ETDV_PLUSARGS' in line:
                  exec(line.replace('#', '').lstrip())
                  conf_plusargs = ETDV_PLUSARGS
               # Run more or less iterations of that config
               if 'ETDV_SEED_MULTIPLIER' in line:
                  exec(line.replace('#', '').lstrip())
                  test_multiplier = ETDV_SEED_MULTIPLIER
               # Get the NUM_OPS if defined so we can apply the build multiplier
               if 'TEST_PROPS.NUM_OPS' in line:
                  if line.strip()[0] != '#':
                     #exec(re.sub( r"(?i)^.*TEST_PROPS\.", "", line))
                     conf_num_ops = int(re.sub( r".*=\s*", "", line))
         except:
            warn('Msgs-1', f'ERROR in sting config {conf_path}. Skipping.')
            continue
         # For each model add as many seeds as desired. Or skip it if there are not enough HARTs
         for b in sting_builds:
            num_harts = sting_builds[b]['threads']
            if num_harts < min_harts or num_harts > max_harts:
               continue
            num_seeds = sting_builds[b]['seeds']
            num_seeds = int(num_seeds * test_multiplier)
            num_ops   = int(conf_num_ops * sting_builds[b]['ops_x'] * sting_num_ops_mult)
            conf_build = sting_builds[b]['build']
            if not (conf_build == skip_build):
               sting_test_templates.append((num_seeds,
                  StingTestTemplate(
                                 build         = builds[conf_build],
                                 test_name     = f'{base_tag}_{b}_{conf_name}',
                                 board_conf    = f'{num_harts}c',
                                 test_tags     = [base_tag, conf_name, f'{base_tag}_{b}', f'{base_tag}_{num_harts}t_{conf_name}'] + et_dvrun_tags,
                                 test_plusargs = (conf_plusargs + ' ' + sting_plusargs + sting_builds[b]['plusargs']),
                                 sting_args    = f'{sting_options} {sting_builds[b]["sting_args"]} CPU[0-BOARD_CONF.NUM_CPUS].TEST_PROPS.NUM_OPS={num_ops}',
                                 sting_conf    = conf_path.replace(STING_ROOT, '$STING_ROOT'),
                                 sting_conf_vars = sting_conf_vars)))

if len(sting_test_templates) > 0:
   max_num_seeds = max([num_seeds for (num_seeds, _) in sting_test_templates])
   # Add tests in circular order, i.e.: foo_0, bar_0, baz_0, foo_1, bar_1, ...
   for ii in range(max_num_seeds):
      for (num_seeds, tmpl) in sting_test_templates:
         if num_seeds > ii:
            add_sting_test(build         = tmpl.build,
                           test_name     = tmpl.test_name + f'_{ii}',
                           board_conf    = tmpl.board_conf,
                           test_tags     = tmpl.test_tags,
                           test_plusargs = tmpl.test_plusargs,
                           sting_args    = tmpl.sting_args + f' ET_TEST_ENV.PMU_EVENT_BASE={ii} ',
                           sting_conf    = tmpl.sting_conf,
                           sting_conf_vars = tmpl.sting_conf_vars)

