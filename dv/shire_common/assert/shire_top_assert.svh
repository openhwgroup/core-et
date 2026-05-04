// Note: This assertion file will be included in file ":/dv/soc/shire/evl_shire_interface.sv".

// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset_cold, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )


// clock:  use shire_top.clk__noc_lv 
// reset:  use shire_top.reset_n_system


// Following checkers and monitors are for PostSi_UST testing.

bit        en_axi_trans_to_other_shires_warning;
bit [5:0]  own_minion_shire_id;  
initial begin
   if (!`et_value_plusargs("en_axi_trans_to_other_shires_warning=%d", en_axi_trans_to_other_shires_warning)) begin
      en_axi_trans_to_other_shires_warning = 0;   // disabled by default
   end
   if (!`et_value_plusargs("own_minion_shire_id=%d", own_minion_shire_id)) begin
      own_minion_shire_id = 5'h0;   // Shire_0 by default
   end
end



// to detect AXI Read Req transactions (Cached) from Shire_0 to other Shires:
//  shire_top.to_l3_mesh_master_axi_ARVALID[3:0],
//  shire_top.to_l3_mesh_master_axi_ARREADY[3:0],
//  shire_top.to_l3_mesh_master_axi_AR[3:0]:  ADDR[10:6]= shire_id

// it has 4 AXI ports: mux-select the address of only one valid port:
wire [39:0]  to_l3_mesh_master_axi_AR_addr;     // only one valid port, for AXI Read
wire [39:0]  to_l3_mesh_master_axi_AW_addr;     // only one valid port, for AXI Write

assign  to_l3_mesh_master_axi_AR_addr = 
                      (shire_top.to_l3_mesh_master_axi_ARREADY[0] & shire_top.to_l3_mesh_master_axi_ARVALID[0])? 
                          shire_top.to_l3_mesh_master_axi_AR[0].ADDR :
                      (shire_top.to_l3_mesh_master_axi_ARREADY[1] & shire_top.to_l3_mesh_master_axi_ARVALID[1])? 
                          shire_top.to_l3_mesh_master_axi_AR[1].ADDR :
                      (shire_top.to_l3_mesh_master_axi_ARREADY[2] & shire_top.to_l3_mesh_master_axi_ARVALID[2])? 
                          shire_top.to_l3_mesh_master_axi_AR[2].ADDR :
                      (shire_top.to_l3_mesh_master_axi_ARREADY[3] & shire_top.to_l3_mesh_master_axi_ARVALID[3])? 
                          shire_top.to_l3_mesh_master_axi_AR[3].ADDR :
                          40'hx;

assign  to_l3_mesh_master_axi_AW_addr = 
                      (shire_top.to_l3_mesh_master_axi_AWREADY[0] & shire_top.to_l3_mesh_master_axi_AWVALID[0])? 
                          shire_top.to_l3_mesh_master_axi_AW[0].ADDR :
                      (shire_top.to_l3_mesh_master_axi_AWREADY[1] & shire_top.to_l3_mesh_master_axi_AWVALID[1])? 
                          shire_top.to_l3_mesh_master_axi_AW[1].ADDR :
                      (shire_top.to_l3_mesh_master_axi_AWREADY[2] & shire_top.to_l3_mesh_master_axi_AWVALID[2])? 
                          shire_top.to_l3_mesh_master_axi_AW[2].ADDR :
                      (shire_top.to_l3_mesh_master_axi_AWREADY[3] & shire_top.to_l3_mesh_master_axi_AWVALID[3])? 
                          shire_top.to_l3_mesh_master_axi_AW[3].ADDR :
                          40'hx;

// note: in module "shire_top": when sample AXI signals, need to use "clk__noc_lv" and "reset_n_system" (instead of other clocks).

// Just WARNING (not ERROR) when detect such (above) AXI Read (cached):
WARN_AXI_Read_to_Other_Shire: assert 
  property ( @(posedge shire_top.clk__noc_lv) disable iff (~shire_top.reset_n_system | ~en_axi_trans_to_other_shires_warning)
     ( (|(shire_top.to_l3_mesh_master_axi_ARREADY & shire_top.to_l3_mesh_master_axi_ARVALID) == 1'b1) 
       |->
       (to_l3_mesh_master_axi_AR_addr[10:6] == own_minion_shire_id)  // assume the test always runss in Shire_0 (by default) 
     )
  ) else 
     `uvm_warning("SVA", $sformatf("%t (WARNING) Shire_top: detect (cached) AXI Read to other Shires: unexpected: addr[39:0]=0x%h (expected [10:6]=shire_id=0x%h)", $time, $sampled(to_l3_mesh_master_axi_AR_addr), own_minion_shire_id))



// to detect AXI Write Req transactions (Cached) from Shire_0 to other Shires:
//  shire_top.to_l3_mesh_master_axi_AWVALID[3:0],
//  shire_top.to_l3_mesh_master_axi_AWREADY[3:0],
//  shire_top.to_l3_mesh_master_axi_AW[3:0]:  ADDR[10:6]= shire_id

// Just WARNING (not ERROR) when detect such (above) AXI Write (cached):
WARN_AXI_Write_to_Other_Shire: assert 
  property ( @(posedge shire_top.clk__noc_lv) disable iff (~shire_top.reset_n_system | ~en_axi_trans_to_other_shires_warning)
     ( (|(shire_top.to_l3_mesh_master_axi_AWREADY & shire_top.to_l3_mesh_master_axi_AWVALID) == 1'b1) 
       |->
       (to_l3_mesh_master_axi_AW_addr[10:6] == own_minion_shire_id)  // assume the test always runss in Shire_0 (by default) 
     )
  ) else 
     `uvm_warning("SVA", $sformatf("%t (WARNING) Shire_top: detect (cached) AXI Write to other Shires: unexpected: addr[39:0]=0x%h (expected [10:6]=shire_id=0x%h)", $time, $sampled(to_l3_mesh_master_axi_AW_addr), own_minion_shire_id))




`ifdef EVL_SIMULATION
// to detect UnCached AXI Read Req transactions from Shire_0 to other Shires:
//  shire_top.uc_to_l3_mesh_master_axi_ARVALID,
//  shire_top.uc_to_l3_mesh_master_axi_ARREADY,
//  shire_top.uc_to_l3_mesh_master_axi_AR:  ADDR[10:6]= shire_id

// Just WARNING (not ERROR) when detect such UC (uncached) AXI Read (above):
WARN_UC_AXI_Read_to_Other_Shire: assert 
  property ( @(posedge shire_top.clk__noc_lv) disable iff (~shire_top.reset_n_system | ~en_axi_trans_to_other_shires_warning)
     ( ((shire_top.uc_to_l3_mesh_master_axi_ARREADY & shire_top.uc_to_l3_mesh_master_axi_ARVALID) == 1'b1) 
       |->
       (shire_top.uc_to_l3_mesh_master_axi_AR.ADDR[10:6] == own_minion_shire_id)  // assume the test always runss in Shire_0 (by default) 
     )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_top: detect UC (uncached) AXI Read to other Shires: unexpected: addr[39:0]=0x%h (expected [10:6]=shire_id=0x%h)", $sampled(shire_top.uc_to_l3_mesh_master_axi_AR.ADDR), own_minion_shire_id))


// to detect UnCached AXI Write Req transactions from Shire_0 to other Shires:
//  shire_top.uc_to_l3_mesh_master_axi_AWVALID,
//  shire_top.uc_to_l3_mesh_master_axi_AWREADY,
//  shire_top.uc_to_l3_mesh_master_axi_AW:  ADDR[10:6]= shire_id

// Just WARNING (not ERROR) when detect such UC (uncached) AXI Write (above):
WARN_UC_AXI_Write_to_Other_Shire: assert 
  property ( @(posedge shire_top.clk__noc_lv) disable iff (~shire_top.reset_n_system | ~en_axi_trans_to_other_shires_warning)
     ( ((shire_top.uc_to_l3_mesh_master_axi_AWREADY & shire_top.uc_to_l3_mesh_master_axi_AWVALID) == 1'b1) 
       |->
       (shire_top.uc_to_l3_mesh_master_axi_AW.ADDR[10:6] == own_minion_shire_id)  // assume the test always runss in Shire_0 (by default) 
     )
  ) else 
     `dut_warning("SVA", $sformatf("Shire_top: detect UC (uncached) AXI Write to other Shires: unexpected: addr[39:0]=0x%h (expected [10:6]=shire_id=0x%h)", $sampled(shire_top.uc_to_l3_mesh_master_axi_AW.ADDR), own_minion_shire_id))
`endif // ifdef EVL_SIMULATION
