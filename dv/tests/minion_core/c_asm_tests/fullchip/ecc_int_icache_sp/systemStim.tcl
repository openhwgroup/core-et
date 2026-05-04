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
# If the neighborhood does not exist, there is no point in running this test.  Note that we are
# running the test code on shire 0.
#
####################################################################################################
set neigh_agent "S0:NEIGH_RTL_AGENT-0";
if {[have_agent ${neigh_agent}] <= 0} {
   dut_error [format "%s does not exists ... aborting the test" ${neigh_agent}];
   exit_script;
}
evl_log NONE "Started the SBE/DBE script";


####################################################################################################
#
# Set the trigger instructions (and eventually wait for them).  When a CPU executes a trigger
# instruction, then it is time set the SBE/DBE.
#
####################################################################################################
set trigger_instr0 0x7002a293; # this trigger instruction is slti t0,t0,0x700


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
evl_log LOW [format "waiting for trigger 0 (0x%08x)" [expr ${trigger_instr0} & 0xffffffff]];
wait_for_instr ${trigger_instr0}
set last_pc     [get_last_pc];
set last_instr  [get_last_instr];
set last_source [get_last_source];
set phys_addr   [expr (${last_pc} - 0x40) & 0xffffffffc0];
evl_log LOW [format "found trigger 0 (0x%08x at virtual address 0x%010x on hart %d)" [expr ${last_instr} & 0xffffffff] [expr ${last_pc} & 0xffffffffff] ${last_source}];
evl_log LOW [format "inserting ECC err for I-cache line at address 0x%010x" ${phys_addr}];


#
# Look for the physical address in the I-cache.
#
set icache_way -1;
for {set way 0} {${way} < 4} {incr way} {
   set icache_way_addr [get_cache_tag ${neigh_agent} ram=icache way=${way} ${phys_addr}];
   if {[expr ((${icache_way_addr} ^ ${phys_addr}) & 0xffffffffc0)] == 0} {
      set icache_way ${way};
      break;
   }
}

if {${icache_way} < 0} {
   dut_error [format "could not find 0x%010x in the I-cache" [expr ${phys_addr} & 0xffffffffff]];
   exit_script;
}
evl_log LOW [format "found 0x%010x in I-cache way %d" [expr ${phys_addr} & 0xffffffffff] ${icache_way}];


#
# Insert a double-bit errors in each of the 8 doublewords.
#
set ecc_bit0 64;
set ecc_bit1 65;
for {set dw 0} {${dw} < 8} {incr dw} {
   set icache_addr [expr (${phys_addr} | (${dw} << 3))];
   set_cache_bit_err ${neigh_agent} ram=icache way=${icache_way} ${icache_addr} ${ecc_bit0} ${ecc_bit1};
   incr ecc_bit0;
   if {${ecc_bit0} >= 72} {
      set ecc_bit0 64;
   }
   incr ecc_bit1;
   if {${ecc_bit1} >= 72} {
      set ecc_bit1 64;
   }
}
