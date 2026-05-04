// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_scratchpad_ctrl.

module minion_dcache_scratchpad_ctrl_tb
  import minion_dcache_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [1:0] scratchpad_ctrl_i,
  input  logic dcache_idle_i,
  output logic dcache_cfg_stall_o,
  output logic dcache_cfg_clear_all_o,
  output logic dcache_cfg_clear_low_o,
  output logic dcache_cfg_alloc_scp_o,
  output logic da_clear_en_o,
  output logic da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0] da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0] da_clear_way_o,
  output logic da_clear_idx_o,
  input  logic da_clear_rdy_i,
  output logic [1:0] dcache_cfg_mode_o
);

  dcache_mode_t dcache_cfg_mode;

  minion_dcache_scratchpad_ctrl u_dut (
    .clk_i,
    .rst_ni,
    .scratchpad_ctrl_i   (dcache_mode_t'(scratchpad_ctrl_i)),
    .dcache_idle_i,
    .dcache_cfg_stall_o,
    .dcache_cfg_clear_all_o,
    .dcache_cfg_clear_low_o,
    .dcache_cfg_alloc_scp_o,
    .da_clear_en_o,
    .da_clear_val_o,
    .da_clear_set_o,
    .da_clear_way_o,
    .da_clear_idx_o,
    .da_clear_rdy_i,
    .dcache_cfg_mode_o   (dcache_cfg_mode)
  );

  assign dcache_cfg_mode_o = dcache_cfg_mode;

endmodule : minion_dcache_scratchpad_ctrl_tb
