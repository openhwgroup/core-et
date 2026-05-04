// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_scratchpad_ctrl vs new
// minion_dcache_scratchpad_ctrl.

`include "soc.vh"

module cosim_minion_dcache_scratchpad_ctrl_tb
  import minion_dcache_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic [1:0]                 scratchpad_ctrl_i,
  input  logic                       dcache_idle_i,
  input  logic                       da_clear_rdy_i,
  output logic                       orig_dcache_cfg_stall_o,
  output logic                       new_dcache_cfg_stall_o,
  output logic                       orig_dcache_cfg_clear_all_o,
  output logic                       new_dcache_cfg_clear_all_o,
  output logic                       orig_dcache_cfg_clear_low_o,
  output logic                       new_dcache_cfg_clear_low_o,
  output logic                       orig_dcache_cfg_alloc_scp_o,
  output logic                       new_dcache_cfg_alloc_scp_o,
  output logic                       orig_da_clear_en_o,
  output logic                       new_da_clear_en_o,
  output logic                       orig_da_clear_val_o,
  output logic                       new_da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0] orig_da_clear_set_o,
  output logic [DcacheSetIdxWidth-1:0] new_da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0] orig_da_clear_way_o,
  output logic [DcacheWayIdxWidth-1:0] new_da_clear_way_o,
  output logic                       orig_da_clear_idx_o,
  output logic                       new_da_clear_idx_o,
  output logic [1:0]                 orig_dcache_cfg_mode_o,
  output logic [1:0]                 new_dcache_cfg_mode_o
);

  dcache_mode_t new_dcache_cfg_mode;
  dcache_mode_t orig_dcache_cfg_mode;

  minion_dcache_scratchpad_ctrl u_new (
    .clk_i,
    .rst_ni,
    .scratchpad_ctrl_i      (dcache_mode_t'(scratchpad_ctrl_i)),
    .dcache_idle_i,
    .dcache_cfg_stall_o     (new_dcache_cfg_stall_o),
    .dcache_cfg_clear_all_o (new_dcache_cfg_clear_all_o),
    .dcache_cfg_clear_low_o (new_dcache_cfg_clear_low_o),
    .dcache_cfg_alloc_scp_o (new_dcache_cfg_alloc_scp_o),
    .da_clear_en_o          (new_da_clear_en_o),
    .da_clear_val_o         (new_da_clear_val_o),
    .da_clear_set_o         (new_da_clear_set_o),
    .da_clear_way_o         (new_da_clear_way_o),
    .da_clear_idx_o         (new_da_clear_idx_o),
    .da_clear_rdy_i,
    .dcache_cfg_mode_o      (new_dcache_cfg_mode)
  );

  dcache_scratchpad_ctrl u_orig (
    .clock                  (clk_i),
    .reset                  (!rst_ni),
    .scratchpad_ctrl        (dcache_mode_t'(scratchpad_ctrl_i)),
    .dcache_idle            (dcache_idle_i),
    .dcache_cfg_stall       (orig_dcache_cfg_stall_o),
    .dcache_cfg_clear_all   (orig_dcache_cfg_clear_all_o),
    .dcache_cfg_clear_low   (orig_dcache_cfg_clear_low_o),
    .dcache_cfg_alloc_scp   (orig_dcache_cfg_alloc_scp_o),
    .da_clear_en            (orig_da_clear_en_o),
    .da_clear_val           (orig_da_clear_val_o),
    .da_clear_set           (orig_da_clear_set_o),
    .da_clear_way           (orig_da_clear_way_o),
    .da_clear_idx           (orig_da_clear_idx_o),
    .da_clear_rdy           (da_clear_rdy_i),
    .dcache_cfg_mode        (orig_dcache_cfg_mode)
  );

  assign orig_dcache_cfg_mode_o = orig_dcache_cfg_mode;
  assign new_dcache_cfg_mode_o = new_dcache_cfg_mode;

endmodule : cosim_minion_dcache_scratchpad_ctrl_tb
