# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPOROOT = getenv('REPOROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"

if False:
    exec(open(REPOROOT+"/dv/tests/shire_cache/regress_lists/sc_dvrun_tools.py").read())
else:
    import sys, os
    sys.path.append('/Users/kpfalzer/projects/etdv/test')
    from sc_dvrun_tools import sc_add_build


#####################################################################
# Setup the regression

config_name = 'sc_sanity'

build_cmd_base = """
   ${REPOROOT}/dv/dpi/evl_tcl/src/build_dpi
   vcs -full64 -f ${REPOROOT}/dv/sample_l2/run.vcs.f \
      -debug_all -debug_access+all +memcbk +v2k -ntb_opts uvm-1.2 -sverilog -timescale=1ns/1ps  +vcs+lic+wait ./libevl_tcl.so \
      +define+ET_NEW_SHIRE_CACHE +define+SHIRE_CACHE_AXI +define+SHIRE_CACHE_STANDALONE"""

# Add builds
builds = []
fns = {'new_build': new_build, 'add_build': add_build}
sc_add_build(builds, 'sc_unit', build_cmd_base, config_name, fns)
sc_add_build(builds, 'sc_unit_cbuf4', build_cmd_base + " +define+SC_NUM_CBUF_SIZE=4 +define+SC_USE_ASIC_RAMS", config_name, fns)

test_cmd_base       = 'simv -l sim.log +verbosity=QUIET -licwait 5 '
rand_test_cmd_base  = 'simv -l sim.log +verbosity=QUIET -licwait 5 +ntb_random_seed=REPLACE_WITH_RAND '

# Add Tests

# Add each test in the tcl directory
tclSubPath = 'dv/tests/shire_cache/tcl_tests/tcl_scripts'
tclPath = '$REPOROOT/'+tclSubPath
for tcl_file in os.listdir(REPOROOT+'/'+tclSubPath):
   if tcl_file.endswith(".tcl"):
      test_name = os.path.splitext(tcl_file)[0]
      test_cmd = test_cmd_base + '+tclPath='+tclPath+' +neighStim='+tcl_file+' $*'
      sc_add_test_to_builds(builds,
         build_name_match='sc_unit',
         test_name=test_name,
         test_cmd=test_cmd)

# Run rand_all_ops.tcl on both the neighborhood and the l3 stim. 
sc_add_test_to_builds(builds, build_name_match='sc_unit', ntimes=2, test_name='rand_all_ops_on_l2_and_l3',
   test_cmd = rand_test_cmd_base + '+tclPath='+tclPath+' +neighStim=rand_all_ops.tcl +l3Stim=rand_all_ops.tcl +axi_strict_order_ar_aw $*')

def get_interesting_esr(): 
   """Each time this is called return and interesting esr plusarg to add to the test command."""
   esr_list = []
   esr_list.append("+esr_sc_l2_rbuf_enable=0")
   esr_list.append("+esr_sc_scp_rbuf_enable=0")
   esr_list.append("+esr_sc_ram_delay="+one_of(2, 3))

   esr_list.append("+esr_sc_cbuf_entry_enable="+str(random.randint(1, (2**32)-1)))
   esr_list.append("+esr_sc_cbuf_enable=1")

   # FIXME too slow and get false timeout, need to adjust for this
   #esr_list.append("+esr_sc_allow_only_1_req_per_sub_bank="+one_of([0]*4, 1))
   #esr_list.append("+esr_sc_allow_only_1_req_per_bank="+one_of([0]*4, 1))

   # FIXME cannot do l3_slave with this option, need to fixup the test witout l3Stim
   #esr_list.append("+esr_sc_force_mesh_req_to_sys=1")

   #esr_list.append("+esr_sc_l2_bypass=1")
   #esr_list.append("+esr_sc_l3_bypass=1")

   esr_list.append("+esr_sc_num_l3_reqq_entries="+one_of(list(range(2,50))))
   #esr_list.append("+esr_sc_num_l3_reqq_entries="+one_of(7, 20, 35, 51))

   return one_of(esr_list)


def get_interesting_esr_allcomb(l3stim): 
   """Each time this is called return esr plusarg to add to the test command."""
   esr_list = ""

   esr_list = esr_list + " +esr_sc_l2_rbuf_enable="+one_of([0]*2, [1]*10) 
   esr_list = esr_list + " +esr_sc_scp_rbuf_enable="+one_of([0]*2, [1]*10) 

   # FIXME ram delay of 3 gets false timeout, need longer sim time for completion
   # esr_list = esr_list + " +esr_sc_ram_delay="+one_of([1]*10, [2]*10, [3]*10) 
   esr_list = esr_list + " +esr_sc_ram_delay="+one_of([1]*10, [2]*10) 

   esr_list = esr_list + " +esr_sc_cbuf_entry_enable="+str(random.randint(1, (2**32)-1)) 
   esr_list = esr_list + " +esr_sc_cbuf_enable="+one_of([0]*10, [1]*10) 

   # FIXME too slow and get false timeout, need to adjust for this
   #esr_list = esr_list + " +esr_sc_allow_only_1_req_per_sub_bank=1"+one_of([0]*4, 1)
   #esr_list = esr_list + " +esr_sc_allow_only_1_req_per_bank=1"+one_of([0]*4, 1)

   # FIXME need update the rand_all_ops checking for lock/unlock/atomic in bypass mode
   #esr_list = esr_list + " +esr_sc_l2_bypass=1"+one_of([0]*10, 1)
   #esr_list = esr_list + " +esr_sc_l3_bypass=1"+one_of([0]*10, 1)

   esr_list = esr_list + " +esr_sc_num_l3_reqq_entries="+one_of(list(range(2,50))) 

   # Cannot do this option with l3Stim to l3_slave
   if (l3stim == 0) :
     esr_list = esr_list + " +esr_sc_force_mesh_req_to_sys=="+one_of([0]*10, [1]*10)

   return (esr_list)


for ii in range(0,5) : 
   sc_add_test_to_builds(builds, build_name_match='sc_unit', ntimes=1, test_name='rand_all_ops_with_interesting_esr',
      test_cmd = rand_test_cmd_base + '+tclPath='+tclPath+' +neighStim=rand_all_ops.tcl +l3Stim=rand_all_ops.tcl +axi_strict_order_ar_aw '+ get_interesting_esr() +' $*')

# FIXME temp disable for now
# for ii in range(0,5) : 
#    sc_add_test_to_builds(builds, build_name_match='sc_unit', ntimes=1, test_name='rand_all_ops_with_interesting_esr',
#       test_cmd = rand_test_cmd_base + '+tclPath='+tclPath+' +neighStim=rand_all_ops.tcl +l3Stim=rand_all_ops.tcl +axi_strict_order_ar_aw '+ get_interesting_esr_allcomb(0) + ' $*')
# 
# for ii in range(0,2) : 
#    sc_add_test_to_builds(builds, build_name_match='sc_unit', ntimes=1, test_name='rand_all_ops_neighonly_with_interesting_esr',
#       test_cmd = rand_test_cmd_base + '+tclPath='+tclPath+' +neighStim=rand_all_ops.tcl +axi_strict_order_ar_aw '+ get_interesting_esr_allcomb(1) + ' $*')


