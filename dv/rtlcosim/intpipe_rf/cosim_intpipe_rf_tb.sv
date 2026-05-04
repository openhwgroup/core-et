// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_rf (new) vs intpipe_rf (original).
//
// Sequential register file wrapper. 2 read ports, 1 write port.
// Uses prim_rf_2r1w (new) vs rf_latch_2r_1w (original).
// Tests x0 hardwired zero, x31 parallel read, random write/read.

`include "soc.vh"

module cosim_intpipe_rf_tb (
  input  logic         clk_i,
  input  logic         rst_ni,

  // Stimulus — read ports
  input  logic [1:0]   stim_rd_en,
  input  logic         stim_rd_thread_id,
  input  logic [4:0]   stim_rd_addr_0,
  input  logic [4:0]   stim_rd_addr_1,

  // Stimulus — write port
  input  logic         stim_wr_en,
  input  logic         stim_wr_en_early,
  input  logic         stim_wr_thread_id,
  input  logic [4:0]   stim_wr_addr,
  input  logic [63:0]  stim_wr_data,

  // Outputs — new module
  output logic [63:0]  new_rd_data_0_o,
  output logic [63:0]  new_rd_data_1_o,
  output logic [127:0] new_wb_x31_reg_o,

  // Outputs — original module
  output logic [63:0]  orig_rd_data_0_o,
  output logic [63:0]  orig_rd_data_1_o,
  output logic [127:0] orig_wb_x31_reg_o
);

  import minion_pkg::*;

  // Assemble packed arrays for the new module interface
  logic [1:0][XregAddrSize-1:0]  rd_addr;
  logic [0:0][XregAddrSize-1:0]  wr_addr;
  logic [0:0][XregSize-1:0]      wr_data;
  logic [0:0]                    wr_en;
  logic [0:0]                    wr_en_early;

  assign rd_addr[0] = stim_rd_addr_0;
  assign rd_addr[1] = stim_rd_addr_1;
  assign wr_addr[0] = stim_wr_addr;
  assign wr_data[0] = stim_wr_data;
  assign wr_en[0]       = stim_wr_en;
  assign wr_en_early[0] = stim_wr_en_early;

  // New module outputs (packed arrays)
  logic [1:0][XregSize-1:0]               new_rd_data;
  logic [NrThreadsDefault-1:0][XregSize-1:0] new_wb_x31;

  assign new_rd_data_0_o = new_rd_data[0];
  assign new_rd_data_1_o = new_rd_data[1];
  assign new_wb_x31_reg_o = new_wb_x31;

  // Original module outputs (packed arrays)
  logic [1:0][XregSize-1:0]               orig_rd_data;
  logic [NrThreadsDefault-1:0][XregSize-1:0] orig_wb_x31;

  assign orig_rd_data_0_o = orig_rd_data[0];
  assign orig_rd_data_1_o = orig_rd_data[1];
  assign orig_wb_x31_reg_o = orig_wb_x31;

  // -- New module (SystemVerilog, uses minion_pkg + prim_rf_2r1w) --
  intpipe_rf u_new (
    .clk_i,
    .rd_en        (stim_rd_en),
    .rd_thread_id (stim_rd_thread_id),
    .rd_addr      (rd_addr),
    .rd_data      (new_rd_data),
    .wb_x31_reg   (new_wb_x31),
    .wr_en        (wr_en),
    .wr_en_early  (wr_en_early),
    .wr_thread_id (stim_wr_thread_id),
    .wr_addr      (wr_addr),
    .wr_data      (wr_data)
  );

  // -- Original module (Verilog, uses rf_latch_2r_1w) --
  intpipe_rf_orig u_orig (
    .clock        (clk_i),
    .rd_en        (stim_rd_en),
    .rd_thread_id (stim_rd_thread_id),
    .rd_addr      (rd_addr),
    .rd_data      (orig_rd_data),
    .wb_x31_reg   (orig_wb_x31),
    .wr_en        (wr_en),
    .wr_en_early  (wr_en_early),
    .wr_thread_id (stim_wr_thread_id),
    .wr_addr      (wr_addr),
    .wr_data      (wr_data)
  );

endmodule
