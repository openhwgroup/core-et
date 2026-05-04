logic _assert_after_reset=0;
initial begin
    @(posedge reset_n_system);
    _assert_after_reset=1;
end
always_comb begin
    if (_assert_after_reset) begin
        ERROR_pshire_clk_unknown_after_reset: assert(~(reset_n_system && $isunknown(clk__noc))) else $error("ERROR: Assertion failed: Pshire meshstop clock is unknown"); 
    end
end

`ASSERT_ALWAYS_MSG(ERROR_ps0_ps_awvalid_not_wvalid, clk__noc, !reset_n_system, ($rose(ps0_ps_AWVALID) |-> ps0_ps_WVALID), "ps0_ps_AWVALID went high with no ps0_ps_WVALID");
`ASSERT_ALWAYS_MSG(ERROR_ps0_ps_wvalid_not_awvalid, clk__noc, !reset_n_system, ($rose(ps0_ps_WVALID) |-> ps0_ps_AWVALID), "ps0_ps_WVALID went high with no ps0_ps_AWVALID");

`ifdef DV_FLIT_ASSERT_ENABLE
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_0,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   0, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_0,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   0, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_0,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  0, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_1,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_1,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_1,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  1, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_2,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   2, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_2,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   2, 4'b1011)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_2,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  2, 4'b1110)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_3,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   3, 4'b1101)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_3,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   3, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_3,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  3, 4'b1100)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_4,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_4,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_4,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  4, 4'b1010)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_5,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_5,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_5,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  5, 4'b1010)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_6,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_6,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_6,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  6, 4'b1001)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_7,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_7,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_MST_7,   clk__noc, !reset_n_system,   south, 13, S,   north, 21, N,  7, 4'b1000)

   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__MST_8,   clk__noc, !reset_n_system,   west, 13, W,    east, 12, E,   8, 4'b0001)
   `ASSERT_ALWAYS_MESH_PPLN_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__MST_8,   clk__noc, !reset_n_system,   east, 13, E,    west, 14, W,   8, 4'b0001)

   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_0,   clk__noc, !reset_n_system,   north, 21,   south, 13,  0, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_1,   clk__noc, !reset_n_system,   north, 21,   south, 13,  1, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_2,   clk__noc, !reset_n_system,   north, 21,   south, 13,  2, 4'b0111)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_3,   clk__noc, !reset_n_system,   north, 21,   south, 13,  3, 4'b0101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_4,   clk__noc, !reset_n_system,   north, 21,   south, 13,  4, 4'b0001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_5,   clk__noc, !reset_n_system,   north, 21,   south, 13,  5, 4'b0001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_6,   clk__noc, !reset_n_system,   north, 21,   south, 13,  6, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_SOUTH_SLV_7,   clk__noc, !reset_n_system,   north, 21,   south, 13,  7, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_0,   clk__noc, !reset_n_system,   east, 12,    west, 13,   0, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_1,   clk__noc, !reset_n_system,   east, 12,    west, 13,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_2,   clk__noc, !reset_n_system,   east, 12,    west, 13,   2, 4'b1011)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_3,   clk__noc, !reset_n_system,   east, 12,    west, 13,   3, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_4,   clk__noc, !reset_n_system,   east, 12,    west, 13,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_5,   clk__noc, !reset_n_system,   east, 12,    west, 13,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_6,   clk__noc, !reset_n_system,   east, 12,    west, 13,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_7,   clk__noc, !reset_n_system,   east, 12,    west, 13,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_WEST__SLV_8,   clk__noc, !reset_n_system,   east, 12,    west, 13,   8, 4'b0001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_0,   clk__noc, !reset_n_system,   west, 14,    east, 13,   0, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_1,   clk__noc, !reset_n_system,   west, 14,    east, 13,   1, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_2,   clk__noc, !reset_n_system,   west, 14,    east, 13,   2, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_3,   clk__noc, !reset_n_system,   west, 14,    east, 13,   3, 4'b1101)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_4,   clk__noc, !reset_n_system,   west, 14,    east, 13,   4, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_5,   clk__noc, !reset_n_system,   west, 14,    east, 13,   5, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_6,   clk__noc, !reset_n_system,   west, 14,    east, 13,   6, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_7,   clk__noc, !reset_n_system,   west, 14,    east, 13,   7, 4'b1001)
   `ASSERT_ALWAYS_MESH_LAYER(u_ns_pci_meshstop, PSHIRE_EAST__SLV_8,   clk__noc, !reset_n_system,   west, 14,    east, 13,   8, 4'b0001)
`endif
