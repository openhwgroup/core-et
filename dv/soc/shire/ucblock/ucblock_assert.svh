`define UCBLOCK_HIER shire_channel_wrap_inst.shire_channel_gen.channel_inst.uc_blocks

`ifndef ET_ASSERT_OFF
bit [3:0] w_credit_in[1:0]  = {'h0, 'h0};
bit [3:0] aw_credit_in[1:0] = {'h0, 'h0};
bit [3:0] credit_ret[1:0]   = {'h8, 'h8};

always @ ( posedge noc_clock ) begin
   if ( reset_cold === 1'b0 ) begin
      for ( int i = 0; i < 2; i++ ) begin
	 // Assert that when cc == MAX, A*Valid == 0
	 if ( ( aw_credit_in[i] == 8 ) & ( `UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID & `UCBLOCK_HIER.sys_mesh_slave_axi_AWvcvalid[i] ) ) begin
	    `dut_error("", $sformatf("ucblock_assert: More than 8 outstanding request for VC %d", i))
	 end
	 if ( `UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID & `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY & `UCBLOCK_HIER.sys_mesh_slave_axi_AWvcvalid[i] ) begin
	    if ( aw_credit_in[i] == 8 ) begin
	       // If there are 8 outstanding requests already assert
	       `dut_error("", $sformatf("ucblock_assert: sys_mesh_slave_axi_AWvcvalid should not be asserted because 8 credits are already in use."))
	    end else begin
	       // else increase the outstanding credit count
	       aw_credit_in[i] += 1;
	    end
	    // decrease the return credit count
	    credit_ret[i] -= 1;
	 end
	 if ( `UCBLOCK_HIER.sys_mesh_slave_axi_WVALID & `UCBLOCK_HIER.sys_mesh_slave_axi_WREADY & `UCBLOCK_HIER.sys_mesh_slave_axi_Wvcvalid[i] ) begin
	    if ( w_credit_in[i] == 8 ) begin
	       // If there are 8 outstanding requests already assert
	       `dut_error("", $sformatf("ucblock_assert: sys_mesh_slave_axi_Wvcvalid should not be asserted because 8 credits are already in use."))
	    end else begin
	       // else increase the outstanding credit count
	       w_credit_in[i] += 1;
	    end
	 end
	 if (`UCBLOCK_HIER.sys_mesh_slave_axi_AWcredit[i]) begin
	    if ( credit_ret[i] == 8 ) begin
	       // if 8 credits already returned, assert
	       `dut_error("", $sformatf("ucblock_assert: sys_mesh_slave_axi_AWvalid should not be set because 8 credits already returned."))
	    end else begin
	       credit_ret[i] += 1;
	    end
	    aw_credit_in[i] -= 1;
	    w_credit_in[i] -= 1;
	 end
	 if ( ( aw_credit_in[i] + credit_ret[i] ) != 'h8 ) begin
	    `dut_error("", $sformatf("ucblock_assert: sys_mesh_slave_axi_AWvcvalid and sys_mesh_slave_axi_AWvalid are out of sync."))
	 end
      end // for (int i = 0; i < 2; i++)
   end // if ( reset_cold === 1'b0 )
end // always @ ( posedge noclock )
   
`endif
