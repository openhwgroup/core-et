logic _assert_after_reset=0;
initial begin
    #1;
    @(posedge reset_n_system);
    _assert_after_reset=1;
end
always_comb begin
    if (_assert_after_reset) begin
        ERROR_memshire_clk_unknown_after_reset: assert(~(reset_n_system && $isunknown(clk__noc))) else $error("ERROR: Assertion failed: Memshire meshstop clock is unknown"); 
    end
end

`ASSERT_ALWAYS_MSG(ERROR_mem_mesh_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(mem_mesh_slave_axi_AWVALID) |-> mem_mesh_slave_axi_WVALID), "mem_mesh_slave_axi_AWVALID went high with no mem_mesh_slave_axi_WVALID");
`ASSERT_ALWAYS_MSG(ERROR_mem_mesh_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(mem_mesh_slave_axi_WVALID) |-> mem_mesh_slave_axi_AWVALID), "mem_mesh_slave_axi_WVALID went high with no mem_mesh_slave_axi_AWVALID");

`ASSERT_ALWAYS_MSG(ERROR_sbm_mesh_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(sbm_mesh_slave_axi_AWVALID) |-> sbm_mesh_slave_axi_WVALID), "sbm_mesh_slave_axi_AWVALID went high with no sbm_mesh_slave_axi_WVALID");
`ASSERT_ALWAYS_MSG(ERROR_sbm_mesh_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(sbm_mesh_slave_axi_WVALID) |-> sbm_mesh_slave_axi_AWVALID), "sbm_mesh_slave_axi_WVALID went high with no sbm_mesh_slave_axi_AWVALID");

`ifdef DV_FLIT_ASSERT_ENABLE
   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__MST_2,   clk__noc, !reset_n_system,   east, 16,   west, 17,    2, 4'b0010) 
   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_2,   clk__noc, !reset_n_system,   west, 17,   east, 16,    2, 4'b0100) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_3,   clk__noc, !reset_n_system,   west, 17,   east, 16,    3, 4'b0100) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_4,   clk__noc, !reset_n_system,   west, 17,   east, 16,    4, 4'b0010) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_5,   clk__noc, !reset_n_system,   west, 17,   east, 16,    5, 4'b0110) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__MST_6,   clk__noc, !reset_n_system,   east, 16,   west, 17,    6, 4'b1000) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__MST_7,   clk__noc, !reset_n_system,   east, 16,   west, 17,    7, 4'b1010) 
   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_7,   clk__noc, !reset_n_system,   west, 17,   east, 16,    7, 4'b0000) 

   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__MST_8,   clk__noc, !reset_n_system,   east, 16,   west, 17,    8, 4'b0001) 
   `ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MEMSHIRE_EAST__SLV_8,   clk__noc, !reset_n_system,   west, 17,   east, 16,    8, 4'b0001) 
`endif

