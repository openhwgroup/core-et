logic _assert_after_reset=0;
initial begin
    #1;
    @(posedge reset_n_system);
    _assert_after_reset=1;
end
always_comb begin
    if (_assert_after_reset) begin
        ERROR_minionshire_clk_unknown_after_reset: assert(~(reset_n_system && $isunknown(clk__noc))) else $error("ERROR: Assertion failed: Minion shire meshstop clock is unknown"); 
    end
end

`ASSERT_ALWAYS_MSG(ERROR_l3_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_AWVALID[0]) |-> meshstop_wrapper.l3_mesh_slave_axi_WVALID[0]), "l3_mesh_slave_AWVALID[0] went high with no l3_mesh_slave_WVALID[0]");
`ASSERT_ALWAYS_MSG(ERROR_l3_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_WVALID[0]) |-> meshstop_wrapper.l3_mesh_slave_axi_AWVALID[0]),  "l3_mesh_slave_WVALID[0] went high with no l3_mesh_slave_AWVALID[0]");

`ASSERT_ALWAYS_MSG(ERROR_l3b_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_AWVALID[1]) |-> meshstop_wrapper.l3_mesh_slave_axi_WVALID[1]), "l3_mesh_slave_AWVALID[1] went high with no l3_mesh_slave_WVALID[1]");
`ASSERT_ALWAYS_MSG(ERROR_l3b_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_WVALID[1]) |-> meshstop_wrapper.l3_mesh_slave_axi_AWVALID[1]),  "l3_mesh_slave_WVALID[1] went high with no l3_mesh_slave_AWVALID[1]");

`ASSERT_ALWAYS_MSG(ERROR_l3c_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_AWVALID[2]) |-> meshstop_wrapper.l3_mesh_slave_axi_WVALID[2]), "l3_mesh_slave_AWVALID[2] went high with no l3_mesh_slave_WVALID[2]");
`ASSERT_ALWAYS_MSG(ERROR_l3c_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_WVALID[2]) |-> meshstop_wrapper.l3_mesh_slave_axi_AWVALID[2]),  "l3_mesh_slave_WVALID[2] went high with no l3_mesh_slave_AWVALID[2]");

`ASSERT_ALWAYS_MSG(ERROR_l3d_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_AWVALID[3]) |-> meshstop_wrapper.l3_mesh_slave_axi_WVALID[3]), "l3_mesh_slave_AWVALID[3] went high with no l3_mesh_slave_WVALID[3]");
`ASSERT_ALWAYS_MSG(ERROR_l3d_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.l3_mesh_slave_axi_WVALID[3]) |-> meshstop_wrapper.l3_mesh_slave_axi_AWVALID[3]),  "l3_mesh_slave_WVALID[3] went high with no l3_mesh_slave_AWVALID[3]");

`ASSERT_ALWAYS_MSG(ERROR_sys_mesh_slave_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.sys_mesh_slave_axi_AWVALID) |-> meshstop_wrapper.sys_mesh_slave_axi_WVALID), "sys_mesh_slave_axi_AWVALID went high with no sys_mesh_slave_axi_WVALID");
`ASSERT_ALWAYS_MSG(ERROR_sys_mesh_slave_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(meshstop_wrapper.sys_mesh_slave_axi_WVALID) |-> meshstop_wrapper.sys_mesh_slave_axi_AWVALID), "sys_mesh_slave_axi_WVALID went high with no sys_mesh_slave_axi_AWVALID");

/*
`ASSERT_ALWAYS_MSG(ERROR_to_sys_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.to_sys_mesh_master_axi_AWVALID) && !($fell(meshstop_wrapper.to_sys_mesh_master_axi_WVALID) || meshstop_wrapper.to_sys_mesh_master_axi_WVALID) && meshstop_wrapper.to_sys_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.to_sys_mesh_master_axi_AWREADY || $rose(meshstop_wrapper.to_sys_mesh_master_axi_WVALID) || $fell(meshstop_wrapper.to_sys_mesh_master_axi_AWVALID))), "to_sys_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_to_sys_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.to_sys_mesh_master_axi_WVALID) && !($fell(meshstop_wrapper.to_sys_mesh_master_axi_AWVALID) || meshstop_wrapper.to_sys_mesh_master_axi_AWVALID) && meshstop_wrapper.to_sys_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.to_sys_mesh_master_axi_WREADY || $rose(meshstop_wrapper.to_sys_mesh_master_axi_AWVALID) || $fell(meshstop_wrapper.to_sys_mesh_master_axi_WVALID))), "to_sys_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");

`ASSERT_ALWAYS_MSG(ERROR_uc_to_l3_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID) && !($fell(meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID) || meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID) && meshstop_wrapper.uc_to_l3_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.uc_to_l3_mesh_master_axi_AWREADY || $rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID) || $fell(meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID))), "uc_to_l3_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_uc_to_l3_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID) && !($fell(meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID) || meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID) && meshstop_wrapper.uc_to_l3_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.uc_to_l3_mesh_master_axi_WREADY || $rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID) || $fell(meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID))), "uc_to_l3_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");

`ASSERT_ALWAYS_MSG(ERROR_uc_to_sys_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID) && !($fell(meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID) || meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID) && meshstop_wrapper.uc_to_sys_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.uc_to_sys_mesh_master_axi_AWREADY || $rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID) || $fell(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID))), "uc_to_sys_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_uc_to_sys_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID) && !($fell(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID) || meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID) && meshstop_wrapper.uc_to_sys_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.uc_to_sys_mesh_master_axi_WREADY || $rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID) || $fell(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID))), "uc_to_sys_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");
*/

`ASSERT_ALWAYS_MSG(ERROR_to_sys_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.to_sys_mesh_master_axi_AWVALID) && !meshstop_wrapper.to_sys_mesh_master_axi_WVALID && meshstop_wrapper.to_sys_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.to_sys_mesh_master_axi_AWREADY)), "to_sys_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_to_sys_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.to_sys_mesh_master_axi_WVALID) && !meshstop_wrapper.to_sys_mesh_master_axi_AWVALID && meshstop_wrapper.to_sys_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.to_sys_mesh_master_axi_WREADY)), "to_sys_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");

`ASSERT_ALWAYS_MSG(ERROR_uc_to_l3_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID) && !meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID && meshstop_wrapper.uc_to_l3_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.uc_to_l3_mesh_master_axi_AWREADY)), "uc_to_l3_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_uc_to_l3_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_l3_mesh_master_axi_WVALID) && !meshstop_wrapper.uc_to_l3_mesh_master_axi_AWVALID && meshstop_wrapper.uc_to_l3_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.uc_to_l3_mesh_master_axi_WREADY)), "uc_to_l3_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");

`ASSERT_ALWAYS_MSG(ERROR_uc_to_sys_master_awready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID) && !meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID && meshstop_wrapper.uc_to_sys_mesh_master_axi_WREADY) |=> (!meshstop_wrapper.uc_to_sys_mesh_master_axi_AWREADY)), "uc_to_sys_mesh_master received AWVALID signal before a WVALID and didn't drop its AWREADY signal");
`ASSERT_ALWAYS_MSG(ERROR_uc_to_sys_master_wready_not_drop, clk__noc, !reset_n_system, (($rose(meshstop_wrapper.uc_to_sys_mesh_master_axi_WVALID) && !meshstop_wrapper.uc_to_sys_mesh_master_axi_AWVALID && meshstop_wrapper.uc_to_sys_mesh_master_axi_AWREADY) |=> (!meshstop_wrapper.uc_to_sys_mesh_master_axi_WREADY)), "uc_to_sys_mesh_master received WVALID signal before an AWVALID and didn't drop its WREADY signal");


`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_0,   clk__noc, !reset_n_system,   west, 10,   east, 9,    0, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_0,   clk__noc, !reset_n_system,   east, 9,    west, 10,   0, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_0,   clk__noc, !reset_n_system,   north, 17,  south, 9,   0, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_0,   clk__noc, !reset_n_system,   south, 9,   north, 17,  0, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_0,   clk__noc, !reset_n_system,   east, 8,    west, 9,    0, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_0,   clk__noc, !reset_n_system,   west, 9,    east, 8,    0, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_0,   clk__noc, !reset_n_system,   south, 1,   north, 9,   0, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_0,   clk__noc, !reset_n_system,   north, 9,   south, 1,   0, 4'b1101) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_1,   clk__noc, !reset_n_system,   west, 10,   east, 9,    1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_1,   clk__noc, !reset_n_system,   east, 9,    west, 10,   1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_1,   clk__noc, !reset_n_system,   north, 17,  south, 9,   1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_1,   clk__noc, !reset_n_system,   south, 9,   north, 17,  1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_1,   clk__noc, !reset_n_system,   east, 8,    west, 9,    1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_1,   clk__noc, !reset_n_system,   west, 9,    east, 8,    1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_1,   clk__noc, !reset_n_system,   south, 1,   north, 9,   1, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_1,   clk__noc, !reset_n_system,   north, 9,   south, 1,   1, 4'b1101) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_2,   clk__noc, !reset_n_system,   west, 10,   east, 9,    2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_2,   clk__noc, !reset_n_system,   east, 9,    west, 10,   2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_2,   clk__noc, !reset_n_system,   north, 17,  south, 9,   2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_2,   clk__noc, !reset_n_system,   south, 9,   north, 17,  2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_2,   clk__noc, !reset_n_system,   east, 8,    west, 9,    2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_2,   clk__noc, !reset_n_system,   west, 9,    east, 8,    2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_2,   clk__noc, !reset_n_system,   south, 1,   north, 9,   2, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_2,   clk__noc, !reset_n_system,   north, 9,   south, 1,   2, 4'b1111) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_3,   clk__noc, !reset_n_system,   west, 10,   east, 9,    3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_3,   clk__noc, !reset_n_system,   east, 9,    west, 10,   3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_3,   clk__noc, !reset_n_system,   north, 17,  south, 9,   3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_3,   clk__noc, !reset_n_system,   south, 9,   north, 17,  3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_3,   clk__noc, !reset_n_system,   east, 8,    west, 9,    3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_3,   clk__noc, !reset_n_system,   west, 9,    east, 8,    3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_3,   clk__noc, !reset_n_system,   south, 1,   north, 9,   3, 4'b1101) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_3,   clk__noc, !reset_n_system,   north, 9,   south, 1,   3, 4'b1101) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_4,   clk__noc, !reset_n_system,   west, 10,   east, 9,    4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_4,   clk__noc, !reset_n_system,   east, 9,    west, 10,   4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_4,   clk__noc, !reset_n_system,   north, 17,  south, 9,   4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_4,   clk__noc, !reset_n_system,   south, 9,   north, 17,  4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_4,   clk__noc, !reset_n_system,   east, 8,    west, 9,    4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_4,   clk__noc, !reset_n_system,   west, 9,    east, 8,    4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_4,   clk__noc, !reset_n_system,   south, 1,   north, 9,   4, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_4,   clk__noc, !reset_n_system,   north, 9,   south, 1,   4, 4'b1011) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_5,   clk__noc, !reset_n_system,   west, 10,   east, 9,    5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_5,   clk__noc, !reset_n_system,   east, 9,    west, 10,   5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_5,   clk__noc, !reset_n_system,   north, 17,  south, 9,   5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_5,   clk__noc, !reset_n_system,   south, 9,   north, 17,  5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_5,   clk__noc, !reset_n_system,   east, 8,    west, 9,    5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_5,   clk__noc, !reset_n_system,   west, 9,    east, 8,    5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_5,   clk__noc, !reset_n_system,   south, 1,   north, 9,   5, 4'b1111) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_5,   clk__noc, !reset_n_system,   north, 9,   south, 1,   5, 4'b1111) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_6,   clk__noc, !reset_n_system,   west, 10,   east, 9,    6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_6,   clk__noc, !reset_n_system,   east, 9,    west, 10,   6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_6,   clk__noc, !reset_n_system,   north, 17,  south, 9,   6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_6,   clk__noc, !reset_n_system,   south, 9,   north, 17,  6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_6,   clk__noc, !reset_n_system,   east, 8,    west, 9,    6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_6,   clk__noc, !reset_n_system,   west, 9,    east, 8,    6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_6,   clk__noc, !reset_n_system,   south, 1,   north, 9,   6, 4'b1001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_6,   clk__noc, !reset_n_system,   north, 9,   south, 1,   6, 4'b1001) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_7,   clk__noc, !reset_n_system,   west, 10,   east, 9,    7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_7,   clk__noc, !reset_n_system,   east, 9,    west, 10,   7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_7,   clk__noc, !reset_n_system,   north, 17,  south, 9,   7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_7,   clk__noc, !reset_n_system,   south, 9,   north, 17,  7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_7,   clk__noc, !reset_n_system,   east, 8,    west, 9,    7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_7,   clk__noc, !reset_n_system,   west, 9,    east, 8,    7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_7,   clk__noc, !reset_n_system,   south, 1,   north, 9,   7, 4'b1011) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_7,   clk__noc, !reset_n_system,   north, 9,   south, 1,   7, 4'b1011) 

`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__SLV_8,   clk__noc, !reset_n_system,   west, 10,   east, 9,    8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_EAST__MST_8,   clk__noc, !reset_n_system,   east, 9,    west, 10,   8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_SLV_8,   clk__noc, !reset_n_system,   north, 17,  south, 9,   8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_SOUTH_MST_8,   clk__noc, !reset_n_system,   south, 9,   north, 17,  8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__SLV_8,   clk__noc, !reset_n_system,   east, 8,    west, 9,    8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_WEST__MST_8,   clk__noc, !reset_n_system,   west, 9,    east, 8,    8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_SLV_8,   clk__noc, !reset_n_system,   south, 1,   north, 9,   8, 4'b0001) 
`ASSERT_ALWAYS_MESH_LAYER(meshstop_wrapper, MINSHIRE_NORTH_MST_8,   clk__noc, !reset_n_system,   north, 9,   south, 1,   8, 4'b0001) 

