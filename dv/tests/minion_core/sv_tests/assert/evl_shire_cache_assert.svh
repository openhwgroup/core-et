// Note: This assertion file will be included in file "evl_shire_cache_interface.sv".

// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )


wire  reset_for_assert = reset;  // use "reset" from "evl_shire_cache_interface.sv" (which is "shire_cache.reset_w")

// These assertions below are Only for Post-Si Bringup tests Phase 1 (when NOC is unavailable, thus any L2->NOC traffic is disallowed):
bit  en_sc_l2_to_noc_traffic_err_out;
bit  en_sc_l2_to_noc_traffic_warning;

initial begin
   if (!`et_value_plusargs("en_sc_l2_to_noc_traffic_err_out=%d", en_sc_l2_to_noc_traffic_err_out)) begin
      en_sc_l2_to_noc_traffic_err_out = 0;   // disabled by default
   end
   if (!`et_value_plusargs("en_sc_l2_to_noc_traffic_warning=%d", en_sc_l2_to_noc_traffic_warning)) begin
      en_sc_l2_to_noc_traffic_warning = 0;   // disabled by default
   end
end

// it has 4 AXI ports: mux-select the address of only one valid port:
wire [39:0]  axi_AR_addr;     // only one valid port, for AXI Read
wire [39:0]  axi_AW_addr;     // only one valid port, for AXI Write


assign  axi_AR_addr = (shire_cache.to_l3_mesh_master_axi_ARREADY[0] & shire_cache.to_l3_mesh_master_axi_ARVALID[0])? 
                          shire_cache.to_l3_mesh_master_axi_AR[0].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_ARREADY[L3_PORT_1] & shire_cache.to_l3_mesh_master_axi_ARVALID[L3_PORT_1])? 
                          shire_cache.to_l3_mesh_master_axi_AR[L3_PORT_1].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_ARREADY[L3_PORT_2] & shire_cache.to_l3_mesh_master_axi_ARVALID[L3_PORT_2])? 
                          shire_cache.to_l3_mesh_master_axi_AR[L3_PORT_2].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_ARREADY[L3_PORT_3] & shire_cache.to_l3_mesh_master_axi_ARVALID[L3_PORT_3])? 
                          shire_cache.to_l3_mesh_master_axi_AR[L3_PORT_3].ADDR :
                          40'hx;

assign  axi_AW_addr = (shire_cache.to_l3_mesh_master_axi_AWREADY[0] & shire_cache.to_l3_mesh_master_axi_AWVALID[0])? 
                          shire_cache.to_l3_mesh_master_axi_AW[0].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_AWREADY[L3_PORT_1] & shire_cache.to_l3_mesh_master_axi_AWVALID[L3_PORT_1])? 
                          shire_cache.to_l3_mesh_master_axi_AW[L3_PORT_1].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_AWREADY[L3_PORT_2] & shire_cache.to_l3_mesh_master_axi_AWVALID[L3_PORT_2])? 
                          shire_cache.to_l3_mesh_master_axi_AW[L3_PORT_2].ADDR :
                      (shire_cache.to_l3_mesh_master_axi_AWREADY[L3_PORT_3] & shire_cache.to_l3_mesh_master_axi_AWVALID[L3_PORT_3])? 
                          shire_cache.to_l3_mesh_master_axi_AW[L3_PORT_3].ADDR :
                          40'hx;


// note: in module "shire_cache": when sample AXI signals, need to use "noc_clock" (instead of "clock").


// to detect any L2->NOC AXI Read request: ERROR out:
// RTL signals:
// to_l3_mesh_master_axi_ARREADY[(`SC_L3_MASTER_PORTS-1):0]  ([3:0])
// to_l3_mesh_master_axi_ARVALID[(`SC_L3_MASTER_PORTS-1):0]  ([3:0])
//
`ASSERT_ALWAYS_MSG (ERROR_L2_to_NOC_AXI_Read, shire_cache.noc_clock, (reset_for_assert | ~en_sc_l2_to_noc_traffic_err_out),
   ( |(shire_cache.to_l3_mesh_master_axi_ARREADY & shire_cache.to_l3_mesh_master_axi_ARVALID) == 1'b0 ),
   $sformatf("Shire_Cache: detect L2-to-NOC AXI Read Req: unexpected: addr=0x%h", $sampled(axi_AR_addr))
)
 
// to detect any L2->NOC AXI Write request: ERROR out:
// RTL signals:
// to_l3_mesh_master_axi_AWREADY[(`SC_L3_MASTER_PORTS-1):0]  ([3:0])
// to_l3_mesh_master_axi_AWVALID[(`SC_L3_MASTER_PORTS-1):0]  ([3:0])
//
`ASSERT_ALWAYS_MSG (ERROR_L2_to_NOC_AXI_Write, shire_cache.noc_clock, (reset_for_assert | ~en_sc_l2_to_noc_traffic_err_out),
   ( |(shire_cache.to_l3_mesh_master_axi_AWREADY & shire_cache.to_l3_mesh_master_axi_AWVALID) == 1'b0 ),
   $sformatf("Shire_Cache: detect L2-to-NOC AXI Write Req: unexpected: addr=0x%h", $sampled(axi_AR_addr))
)


// Just WARNING (not ERROR) when detect L2->NOC AXI Read:
`ifdef EVL_SIMULATION
WARN_L2_to_NOC_AXI_Read: assert 
  property ( @(posedge shire_cache.noc_clock) disable iff (reset_for_assert | ~en_sc_l2_to_noc_traffic_warning)
     ( |(shire_cache.to_l3_mesh_master_axi_ARREADY & shire_cache.to_l3_mesh_master_axi_ARVALID) == 1'b0 )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_Cache: detect L2-to-NOC AXI Read Req: unexpected: addr=0x%h", $sampled(axi_AR_addr)))

// Just WARNING (not ERROR) when detect L2->NOC AXI Write:
WARN_L2_to_NOC_AXI_Write: assert 
  property ( @(posedge shire_cache.noc_clock) disable iff (reset_for_assert | ~en_sc_l2_to_noc_traffic_warning)
     ( |(shire_cache.to_l3_mesh_master_axi_AWREADY & shire_cache.to_l3_mesh_master_axi_AWVALID) == 1'b0 )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_Cache: detect L2-to-NOC AXI Write Req: unexpected: addr=0x%h", $sampled(axi_AW_addr)))
`endif // ifdef EVL_SIMULATION




//////////////////////////////////////////////////////////////////////////


// similar monitor/checker for L3->NOC/DDR/Mem AXI traffic:
//
// signals: at module shire_channel_wrap_inst level:
// * clock:  clk_noc_lv
//
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_ARREADY_ip
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_ARVALID_op
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_AR_op (a bus)
// 
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_AWREADY_ip
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_AWVALID_op
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/to_sys_mesh_master_axi_AW_op (a bus)
// 
// If one of those signals is one then requests are being sent from L3 to the NoC
// 
// OR: signals at "module shire_cache" level:
// 
// * clock:  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/noc_clock  (and noc_reset)
// 
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_ARREADY
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_ARVALID
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_AR
// 
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_AWREADY
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_AWVALID
// *  /top/soc_top/SHIRE[0]/shire_top/shire_top/shire_channel_wrap_inst/shire_channel_gen/channel_inst/g_sc/shire_cache/to_sys_mesh_master_axi_AW


// These assertions below are Only for Post-Si Bringup tests Phase 2 (when DDR/Mem is unavailable, thus any L3->NOC/DDR/Mem traffic is disallowed):
bit  en_sc_l3_to_ddr_traffic_err_out;
bit  en_sc_l3_to_ddr_traffic_warning;

initial begin
   if (!`et_value_plusargs("en_sc_l3_to_ddr_traffic_err_out=%d", en_sc_l3_to_ddr_traffic_err_out)) begin
      en_sc_l3_to_ddr_traffic_err_out = 0;   // disabled by default
   end
   if (!`et_value_plusargs("en_sc_l3_to_ddr_traffic_warning=%d", en_sc_l3_to_ddr_traffic_warning)) begin
      en_sc_l3_to_ddr_traffic_warning = 0;   // disabled by default
   end
end


// to detect any L3->NOC/DDR/Mem AXI Read request: ERROR out:
// RTL signals:
// to_sys_mesh_master_axi_ARREADY[(NUM_SYS_PORTS-1):0]  ([0:0] only one Sys AXI port)
// to_sys_mesh_master_axi_ARVALID[(NUM_SYS_PORTS-1):0]  ([0:0] only one Sys AXI port)
//
`ASSERT_ALWAYS_MSG (ERROR_L3_to_DDR_AXI_Read, shire_cache.noc_clock, (reset_for_assert | ~en_sc_l3_to_ddr_traffic_err_out),
   ( (shire_cache.to_sys_mesh_master_axi_ARREADY[0] & shire_cache.to_sys_mesh_master_axi_ARVALID[0]) == 1'b0 ),
   $sformatf("Shire_Cache: detect L3-to-DDR AXI Read Req: unexpected: addr=0x%h", $sampled(shire_cache.to_sys_mesh_master_axi_AR[0].ADDR))
)
 
// to detect any L3->NOC/DDR/Mem AXI Write request: ERROR out:
// RTL signals:
// to_sys_mesh_master_axi_AWREADY[(NUM_SYS_PORTS-1):0]  ([0:0] only one Sys AXI port)
// to_sys_mesh_master_axi_AWVALID[(NUM_SYS_PORTS-1):0]  ([0:0 ]only one Sys AXI port)
// Note:
// special: AXI write for global Atomic Response (carrying data) going back to UC_block;
//   (paddr[39:32] == 8'b00000001) && (paddr[31:30] == 2'b10) && (paddr[21:17] == 5'b11111) && (paddr[16:6] == 11'b0);
//   need to be excluded.
//   See UC Spec, PRM-19 Memory Map (4.1), NoC Spec.
//   See JIRA VERIF-3802.
//
`ASSERT_ALWAYS_MSG (ERROR_L3_to_DDR_AXI_Write, shire_cache.noc_clock, (reset_for_assert | ~en_sc_l3_to_ddr_traffic_err_out),
   ( ((shire_cache.to_sys_mesh_master_axi_AWREADY[0] & shire_cache.to_sys_mesh_master_axi_AWVALID[0]) == 1'b0) || 
     ((shire_cache.to_sys_mesh_master_axi_AWREADY[0] & shire_cache.to_sys_mesh_master_axi_AWVALID[0]) &&
      (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[39:32] == 8'b00000001) &&
      (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[31:30] == 2'b10) &&
      (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[21:17] == 5'b11111) &&
      (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[16: 6] == 11'b0)) 
   ),
   $sformatf("Shire_Cache: detect L3-to-DDR AXI Write Req: unexpected: addr=0x%h (excluding AXI write for Global_Atomic Response)", $sampled(shire_cache.to_sys_mesh_master_axi_AW[0].ADDR))
)


`ifdef EVL_SIMULATION
// Just WARNING (not ERROR) when detect L3->NOC/DDR/Mem AXI Read:
WARN_L3_to_DDR_AXI_Read: assert 
  property ( @(posedge shire_cache.noc_clock) disable iff (reset_for_assert | ~en_sc_l3_to_ddr_traffic_warning)
     ( (shire_cache.to_sys_mesh_master_axi_ARREADY[0] & shire_cache.to_sys_mesh_master_axi_ARVALID[0]) == 1'b0 )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_Cache: detect L3-to-DDR AXI Read Req: unexpected: addr=0x%h", $sampled(shire_cache.to_sys_mesh_master_axi_AR[0].ADDR)))

// Just WARNING (not ERROR) when detect L3->NOC/DDR/Mem AXI Write:
// Note:
// special: AXI write for global Atomic Response (carrying data) going back to UC_block;
//   (paddr[39:32] == 8'b00000001) && (paddr[31:30] == 2'b10) && (paddr[21:17] == 5'b11111) && (paddr[16:6] == 11'b0);
//   need to be excluded.
//   See UC Spec, PRM-19 Memory Map (4.1), NoC Spec.
//   See JIRA VERIF-3802.
WARN_L3_to_DDR_AXI_Write: assert 
  property ( @(posedge shire_cache.noc_clock) disable iff (reset_for_assert | ~en_sc_l3_to_ddr_traffic_warning)
     ( ((shire_cache.to_sys_mesh_master_axi_AWREADY[0] & shire_cache.to_sys_mesh_master_axi_AWVALID[0]) == 1'b0) || 
       ((shire_cache.to_sys_mesh_master_axi_AWREADY[0] & shire_cache.to_sys_mesh_master_axi_AWVALID[0]) &&
        (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[39:32] == 8'b00000001) &&
        (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[31:30] == 2'b10) &&
        (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[21:17] == 5'b11111) &&
        (shire_cache.to_sys_mesh_master_axi_AW[0].ADDR[16: 6] == 11'b0)) 
     )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_Cache: detect L3-to-DDR AXI Write Req: unexpected: addr=0x%h (excluding AXI write for Global_Atomic Response)", $sampled(shire_cache.to_sys_mesh_master_axi_AW[0].ADDR)))
`endif // ifdef EVL_SIMULATION
