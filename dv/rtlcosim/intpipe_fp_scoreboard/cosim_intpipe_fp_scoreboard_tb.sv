// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_fp_scoreboard (new) vs (original).
//
// Pure combinational. Drives random scoreboard states and read addresses,
// compares rd_data and rd_data_dcache outputs.

`include "soc.vh"

module cosim_intpipe_fp_scoreboard_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus — read port
  input  logic        stim_rd_thread_id,
  input  logic [4:0]  stim_rd_addr_0,
  input  logic [4:0]  stim_rd_addr_1,
  input  logic [4:0]  stim_rd_addr_2,

  // Stimulus — dcache scoreboard (packed: 64 bits)
  input  logic [63:0] stim_dcache_sb,

  // Stimulus — vpu scoreboard (164 bits)
  input  logic [163:0] stim_vpu_sb,

  // Outputs — new module
  output logic [2:0]  new_rd_data_o,
  output logic [2:0]  new_rd_data_dcache_o,

  // Outputs — original module
  output logic [2:0]  orig_rd_data_o,
  output logic [2:0]  orig_rd_data_dcache_o
);

  import minion_pkg::*;

  // Reconstruct packed struct types from flat stimulus signals.
  dcache_scoreboard_t dcache_sb;
  assign dcache_sb = stim_dcache_sb;

  vpu_scoreboard_t vpu_sb;
  assign vpu_sb = stim_vpu_sb;

  logic [2:0][XregAddrSize-1:0] rd_addr;
  assign rd_addr[0] = stim_rd_addr_0;
  assign rd_addr[1] = stim_rd_addr_1;
  assign rd_addr[2] = stim_rd_addr_2;

  // -- New module (SystemVerilog, uses minion_pkg) --
  intpipe_fp_scoreboard u_new (
    .rd_thread_id      (stim_rd_thread_id),
    .rd_addr           (rd_addr),
    .rd_data           (new_rd_data_o),
    .rd_data_dcache    (new_rd_data_dcache_o),
    .dcache_scoreboard (dcache_sb),
    .vpu_scoreboard    (vpu_sb)
  );

  // -- Original module (Verilog, uses `include "soc.vh") --
  intpipe_fp_scoreboard_orig u_orig (
    .rd_thread_id      (stim_rd_thread_id),
    .rd_addr           (rd_addr),
    .rd_data           (orig_rd_data_o),
    .rd_data_dcache    (orig_rd_data_dcache_o),
    .dcache_scoreboard (dcache_sb),
    .vpu_scoreboard    (vpu_sb)
  );

endmodule
