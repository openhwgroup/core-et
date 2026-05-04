logic _assert_after_reset=0;
initial begin
    @(posedge reset_n_system);
    _assert_after_reset=1;
end
always_comb begin
    if (_assert_after_reset) begin
        ERROR_ioshire_clk_unknown_after_reset: assert(~(reset_n_system && $isunknown(clk__noc))) else $error("ERROR: Assertion failed: IOshire meshstop clock is unknown"); 
    end
end

`ASSERT_ALWAYS_MSG(ERROR_io0_ps_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(io0_ps_AWVALID) |-> io0_ps_WVALID), "io0_ps_AWVALID went high with no io0_ps_WVALID");
`ASSERT_ALWAYS_MSG(ERROR_io0_ps_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(io0_ps_WVALID) |-> io0_ps_AWVALID), "io0_ps_WVALID went high with no io0_ps_AWVALID");

`ifdef DV_FLIT_ASSERT_ENABLE
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_0,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   0, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_0,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  0, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_1,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_1,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  1, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_2,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   2, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_2,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  2, 4'b1111)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_3,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   3, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_3,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  3, 4'b1101)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_4,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_4,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  4, 4'b1010)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_5,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_5,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  5, 4'b1110)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_6,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_6,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  6, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_7,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_7,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  7, 4'b1000)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__MST_8,   clk__noc, !reset_n_system,   west, 14, W,    east, 13, E,   8, 4'b0001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_MST_8,   clk__noc, !reset_n_system,   south, 14, S,   north, 22, N,  8, 4'b0001)

   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_0,   clk__noc, !reset_n_system,   east, 13,   west, 14,   0, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_1,   clk__noc, !reset_n_system,   east, 13,   west, 14,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_2,   clk__noc, !reset_n_system,   east, 13,   west, 14,   2, 4'b1011)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_3,   clk__noc, !reset_n_system,   east, 13,   west, 14,   3, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_4,   clk__noc, !reset_n_system,   east, 13,   west, 14,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_5,   clk__noc, !reset_n_system,   east, 13,   west, 14,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_6,   clk__noc, !reset_n_system,   east, 13,   west, 14,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_7,   clk__noc, !reset_n_system,   east, 13,   west, 14,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_WEST__SLV_8,   clk__noc, !reset_n_system,   east, 13,   west, 14,   8, 4'b0001)

   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_0,   clk__noc, !reset_n_system,   north, 22,  south, 14,  0, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_1,   clk__noc, !reset_n_system,   north, 22,  south, 14,  1, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_2,   clk__noc, !reset_n_system,   north, 22,  south, 14,  2, 4'b1111)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_3,   clk__noc, !reset_n_system,   north, 22,  south, 14,  3, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_4,   clk__noc, !reset_n_system,   north, 22,  south, 14,  4, 4'b0001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_5,   clk__noc, !reset_n_system,   north, 22,  south, 14,  5, 4'b0001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_6,   clk__noc, !reset_n_system,   north, 22,  south, 14,  6, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_7,   clk__noc, !reset_n_system,   north, 22,  south, 14,  7, 4'b1011)
   `ASSERT_ALWAYS_MESH_LAYER( u_ns_io_meshstop, IOSHIRE_SOUTH_SLV_8,   clk__noc, !reset_n_system,   north, 22,  south, 14,  8, 4'b0001)
`endif
