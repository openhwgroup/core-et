# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPOROOT = getenv('REPOROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"

exec(open(REPOROOT+"/dv/tests/shire_cache/regress_lists/sc_dvrun_tools.py").read())

#####################################################################
# Setup the regression

config_name = 'sc_pipe'

def has_xprop(): return (0 < len(get_user_arg())) and get_user_arg()[0].startswith('XPROP=')


# build command
build_cmd_base =  'vcs -full64 -f ${REPOROOT}/dv/tests/unit_tests/pipe/run.vcs.f -sverilog -debug_all -timescale=1ns/1ps  +vcs+lic+wait '
if has_xprop(): build_cmd_base += f"-xprop={get_user_arg()[0].split('=')[-1]} "

# Add builds
builds = []
sc_add_build(builds, 'config_8m_8b_8sb_32sh_',         build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8                                       ")
sc_add_build(builds, 'config_8m_8b_4sb_32sh_',         build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4                                       ")
sc_add_build(builds, 'config_8m_4b_8sb_32sh_',         build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8                                       ")
sc_add_build(builds, 'config_8m_4b_4sb_32sh_',         build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4                                       ")
sc_add_build(builds, 'config_8m_8b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_8m_8b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_8m_4b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_8m_4b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_6m_8b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_6m_8b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_6m_4b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_6m_4b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_4m_8b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_4m_8b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_4m_4b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_4m_4b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_3m_8b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=3 +define+SC_BANKS=8 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_3m_8b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=3 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_3m_4b_8sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=3 +define+SC_BANKS=4 +define+SC_SUB_BANKS=8 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_3m_4b_4sb_32sh_asic_mem', build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=3 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_USE_ASIC_MEMS              ")
sc_add_build(builds, 'config_4m_4b_4sb_32sh_cbuf4',    build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_CBUF_SIZE=4                ")
sc_add_build(builds, 'config_4m_4b_4sb_32sh_sptagst',  build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_TAG_STATE_RAM_SINGLE_PORT  ")
sc_add_build(builds, 'config_6m_4b_4sb_32sh_sptagst',  build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_TAG_STATE_RAM_SINGLE_PORT  ")
sc_add_build(builds, 'config_8m_4b_4sb_32sh_sptagst',  build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=6 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_TAG_STATE_RAM_SINGLE_PORT  ")
sc_add_build(builds, 'config_8m_8b_4sb_32sh_sptagst',  build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=8 +define+SC_BANKS=8 +define+SC_SUB_BANKS=4 +define+SC_TAG_STATE_RAM_SINGLE_PORT  ")
sc_add_build(builds, 'config_4m_4b_4sb_32sh',          build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_L3_SHIRES=32               ")
sc_add_build(builds, 'config_4m_4b_4sb_16sh',          build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_L3_SHIRES=16               ")
sc_add_build(builds, 'config_4m_4b_4sb_8sh',           build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_L3_SHIRES=8                ")
sc_add_build(builds, 'config_4m_4b_4sb_4sh',           build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_L3_SHIRES=4                ")
sc_add_build(builds, 'config_4m_4b_4sb_2sh',           build_cmd_base + "+define+SC_CACHE_SIZE_IN_MB=4 +define+SC_BANKS=4 +define+SC_SUB_BANKS=4 +define+SC_L3_SHIRES=2                ")

# test commands
test_cmd_base       = '${ETDV_BUILD_RUNDIR}/vbuild/simv $* +verbosity=NONE +verbositySC_SB_WARN=MEDIUM -licwait 5 '
if has_xprop(): test_cmd_base += '-report=xprop '
rand_test_cmd_base  = test_cmd_base + ' +ntb_random_seed=REPLACE_WITH_RAND '

# Add Tests

# Add directed tests in pipe/cmd directory
cmdSubPath = 'dv/tests/unit_tests/pipe/cmd'
cmdPath = '$REPOROOT/'+cmdSubPath
for cmd_file in os.listdir(REPOROOT+'/'+cmdSubPath):
   if cmd_file.endswith(".cmd"):
      test_name = os.path.splitext(cmd_file)[0]
      test_cmd = test_cmd_base + '  +directed +cmd_file=' + cmdPath + '/' + cmd_file
      if 'l3_alias' in test_name:
         sc_add_test_to_builds(builds,    build_name_match='config_.*32sh',         test_name=test_name,  test_cmd=test_cmd)
      elif 'dbg_read_write' in test_name:
         sc_add_test_to_builds(builds,    build_name_match='config_4m_4b_4sb_32sh', test_name=test_name,  test_cmd=test_cmd + '  +sim_time=20000 +force_idx_cop_sm_req_clocks=200 +force_dbg_read_write_seq  +force_dbg_read_write_reporoot=$REPOROOT ' )
      elif 'pipe_dec_err' in test_name:
         sc_add_test_to_builds(builds,    build_name_match='config_4m_4b_4sb_32sh', test_name=test_name,  test_cmd=test_cmd + '  +esr_sc_cache_alloc_mode=3  +allow_cmds_to_zero_size_region ')
         sc_add_test_to_builds(builds,    build_name_match='config_4m_4b_4sb_32sh', test_name=test_name,  test_cmd=test_cmd + '  +esr_sc_cache_alloc_mode=4  +allow_cmds_to_zero_size_region ')
      else:
         sc_add_test_to_builds(builds,    build_name_match='config_',               test_name=test_name,  test_cmd=test_cmd)

# Add random tests

test1_list_options = [
   " +sim_time=3000 +use_rand_data +esr_sc_cache_alloc_mode=1",
   " +sim_time=3000 +use_rand_data +esr_sc_cache_alloc_mode=2",
   " +sim_time=3000 +use_rand_data +esr_sc_cache_alloc_mode=3",
   " +sim_time=3000 +use_rand_data +esr_sc_ram_delay=3",
   " +sim_time=3000 +use_rand_data +esr_sc_ram_delay=4",
   " +sim_time=3000 +use_rand_data +esr_sc_all_shire_aliasing=1 +my_bank_id=3 +my_l3_virtual_shire_id=1"]

test_idx_cop_sm_list_options = [
   " +use_rand_data +sim_time=12000 +et_disable_init_rams",
   " +use_rand_data +sim_time=10000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L2_Inv",
   " +use_rand_data +sim_time=10000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L2_Flush",
   " +use_rand_data +sim_time=10000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L2_Evict",
   " +use_rand_data +sim_time=14000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L3_Inv",
   " +use_rand_data +sim_time=14000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L3_Flush",
   " +use_rand_data +sim_time=14000 +force_idx_cop_sm_req_clocks=2000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_L3_Evict",
   " +use_rand_data +sim_time=30000 +force_idx_cop_sm_req_clocks=5000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_Scp_Zero",
   " +use_rand_data +sim_time=10000 +force_idx_cop_sm_req_clocks=4000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_CB_Inv",
   " +use_rand_data +sim_time=14000 +force_idx_cop_sm_req_clocks=4000 +force_idx_cop_sm_req_opcode=SC_IdxCopSm_All_Inv",
   " +use_rand_data +sim_time=500   +force_idx_cop_sm_req_clocks=200  +force_cache_op_to_non_en_regions +esr_sc_cache_alloc_mode=4",
   " +use_rand_data +sim_time=1000  +force_idx_cop_sm_req_clocks=200  +force_cache_op_aborts",
   " +et_disable_init_rams +force_fake_mbist_en",
   " +et_disable_init_rams +force_power_up_reset_abort",
   " +et_disable_init_rams +force_power_up_all_inv_abort"]

test_dec_err_options = [
   " +esr_sc_cache_alloc_mode=3  +allow_cmds_to_zero_size_region ",
   " +esr_sc_cache_alloc_mode=4  +allow_cmds_to_zero_size_region "]


Ntimes = 4
testN_list_options = [
   " +sim_time=5000",
   " +sim_time=5000 +use_rand_data"]

for ii in test1_list_options :
   if 'mode=2' in ii:
      sc_add_test_to_builds(builds, build_name_match='config_.*(32|16)',            ntimes=1,      test_name='rand1',          test_cmd = rand_test_cmd_base + ii )
   else:                                                                                                                       
      sc_add_test_to_builds(builds, build_name_match='config_',                     ntimes=1,      test_name='rand1',          test_cmd = rand_test_cmd_base + ii )
                                                                                                                               
for ii in testN_list_options :                                                                                                 
   sc_add_test_to_builds(builds, build_name_match='config_',                        ntimes=Ntimes, test_name='randN',          test_cmd = rand_test_cmd_base + ii )
                                                                                                                               
for ii in test_idx_cop_sm_list_options :                                                                                       
   sc_add_test_to_builds(builds, build_name_match='config_4m_4b_4sb_32sh_asic_mem', ntimes=1,      test_name='rand_idx',       test_cmd = rand_test_cmd_base + ii )

for ii in test_dec_err_options :
   sc_add_test_to_builds(builds, build_name_match='config_4m_4b_4sb_32sh_asic_mem', ntimes=1,      test_name='rand_dec_err',   test_cmd = rand_test_cmd_base + ii )

