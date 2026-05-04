####################################################################################################
#
# This is a background script (rather than a terminating script).
#
####################################################################################################
background_script;
use_configure_phase;
wait_time 0;


####################################################################################################
#
# If the L2 cache does not exist, there is no point in running this test.  Note that we are running
# the test code on shire 0.
#
####################################################################################################
set shire_cache_agent "S0:SHIRE_CACHE_RTL_AGENT";
if {[get_l2_set_size ${shire_cache_agent}] <= 0} {
   dut_error [format "L2 set size is 0 ... aborting the test"];
   exit_script;
}
evl_log NONE "Started the SBE/DBE script";


####################################################################################################
#
# Set the trigger instructions (and eventually wait for them).  When a CPU executes a trigger
# instruction, then it is time set the SBE/DBE.
#
####################################################################################################
set trigger_instr0 0x80002073; # this trigger instruction is slti r0,r0,0x800
set trigger_instr1 0x3212a293; # this trigger instruction is slti r0,r0,0x801


#
# Set the physical address we expect to find.
#
set phys_addr 0x8000000000;  # this is the physical address for the error


#
# Turn off errors in the scoreboards.
#
set_error_intensity pk_sc_axi_error_intensity    UVM_MEDIUM;
set_error_intensity pk_sc_etlink_error_intensity UVM_MEDIUM;
set_error_intensity pk_sc_sb_error_intensity     UVM_MEDIUM;
set phys_addr [expr ${phys_addr} & 0xffffffffc0];


#
# Wait for graduation of the second "magic" instruction.
#
evl_log LOW [format "waiting for trigger 1 (0x%08x)" [expr ${trigger_instr1} & 0xffffffff]];
wait_for_instr ${trigger_instr1}
evl_log LOW [format "found trigger 1 (0x%08x)" [expr ${trigger_instr1} & 0xffffffff]];


#
# Look for the physical address in the L2 cache.
#
set l2_way -1;
for {set way 0} {${way} < 4} {incr way} {
   set l2_way_addr [get_cache_tag ${shire_cache_agent} ram=l2 way=${way} ${phys_addr}];
   if {[expr ((${l2_way_addr} ^ ${phys_addr}) & 0xffffffffc0)] == 0} {
      set l2_way ${way};
      break;
   }
}

if {${l2_way} < 0} {
   dut_error [format "could not find 0x%010x in the L2 cache" [expr ${phys_addr} & 0xffffffffff]];
   exit_script;
}
evl_log LOW [format "found 0x%010x in L2 way %d" [expr ${phys_addr} & 0xffffffffff] ${l2_way}];


#
# Insert a double-bit errors in each of the 8 doublewords.
#
set ecc_bit0 64;
set ecc_bit1 65;
for {set dw 0} {${dw} < 8} {incr dw} {
   set l2_addr [expr (${phys_addr} | (${dw} << 3))];
   set_cache_bit_err ${shire_cache_agent} ram=l2data way=${l2_way} ${l2_addr} ${ecc_bit0} ${ecc_bit1};
   incr ecc_bit0;
   if {${ecc_bit0} >= 72} {
      set ecc_bit0 64;
   }
   incr ecc_bit1;
   if {${ecc_bit1} >= 72} {
      set ecc_bit1 64;
   }
}
