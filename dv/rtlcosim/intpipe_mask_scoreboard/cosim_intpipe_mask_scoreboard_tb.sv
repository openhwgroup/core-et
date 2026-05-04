// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_mask_scoreboard (new) vs (original).
//
// Pure combinational. Drives random VPU mask scoreboard states and
// rd/wd addresses, compares rd_data and wd_data outputs.

`include "soc.vh"

module cosim_intpipe_mask_scoreboard_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus
  input  logic        stim_rd_thread_id,
  input  logic [7:0]  stim_rd_addr,
  input  logic [7:0]  stim_wd_addr,

  // Stimulus — vpu scoreboard (164 bits)
  input  logic [163:0] stim_vpu_sb,

  // Outputs — new module
  output logic [7:0]  new_rd_data_o,
  output logic [7:0]  new_wd_data_o,

  // Outputs — original module
  output logic [7:0]  orig_rd_data_o,
  output logic [7:0]  orig_wd_data_o
);

  import minion_pkg::*;

  vpu_scoreboard_t vpu_sb;
  assign vpu_sb = stim_vpu_sb;

  // -- New module (SystemVerilog, uses minion_pkg) --
  intpipe_mask_scoreboard u_new (
    .rd_thread_id   (stim_rd_thread_id),
    .rd_addr        (stim_rd_addr),
    .wd_addr        (stim_wd_addr),
    .rd_data        (new_rd_data_o),
    .wd_data        (new_wd_data_o),
    .vpu_scoreboard (vpu_sb)
  );

  // -- Original module (Verilog, uses `include "soc.vh") --
  intpipe_mask_scoreboard_orig u_orig (
    .rd_thread_id   (stim_rd_thread_id),
    .rd_addr        (stim_rd_addr),
    .wd_addr        (stim_wd_addr),
    .rd_data        (orig_rd_data_o),
    .wd_data        (orig_wd_data_o),
    .vpu_scoreboard (vpu_sb)
  );

endmodule
