// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original shire_cache_xbar vs new shirecache_xbar.
// Instantiates TWO configurations to cover both SrcArb modes:
//   Config A: SrcArb=all-ones (source-side arb), AddOutputRelay=2'b10
//   Config B: SrcArb=0 (xbar-side arb), AddOutputRelay=0
// Small config: 3 sources, 2 destinations, 16-bit data, depth 3.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */  // not all outputs used by every test phase

module cosim_xbar_tb #(
  parameter int unsigned NUM_SRC    = 3,
  parameter int unsigned NUM_DEST   = 2,
  parameter int unsigned DATA_WIDTH = 16,
  parameter int unsigned FIFO_DEPTH = 3
) (
  input  logic                                 clk_i,
  input  logic                                 rst_ni,

  // Shared stimulus (drives both configs)
  input  logic [NUM_SRC-1:0][NUM_DEST-1:0]     src_valid_i,
  input  logic [NUM_SRC-1:0][DATA_WIDTH-1:0]   src_data_i,
  input  logic [NUM_DEST-1:0]                  dest_ready_i,

  // Config A outputs (SrcArb=all-ones, AddOutputRelay=2'b10)
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     a_new_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     a_new_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  a_new_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  a_new_dest_data_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     a_orig_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     a_orig_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  a_orig_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  a_orig_dest_data_o,

  // Config B outputs (SrcArb=0, AddOutputRelay=0)
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     b_new_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     b_new_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  b_new_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  b_new_dest_data_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     b_orig_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     b_orig_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  b_orig_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  b_orig_dest_data_o,

  // Config C outputs (SrcArb=1, NumSrcMask=3'b101 — src1 disabled)
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     c_new_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     c_new_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  c_new_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  c_new_dest_data_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     c_orig_src_ready_o,
  output logic [NUM_SRC-1:0][NUM_DEST-1:0]     c_orig_src_2_credits_o,
  output logic [NUM_DEST-1:0]                  c_orig_dest_valid_o,
  output logic [NUM_DEST-1:0][DATA_WIDTH-1:0]  c_orig_dest_data_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ══════════════════════════════════════════════════════════
  // Config A: SrcArb=all-ones, AddOutputRelay=2'b10
  // ══════════════════════════════════════════════════════════

  shirecache_xbar #(
    .FifoDepth(FIFO_DEPTH), .NumSrc(NUM_SRC), .NumDest(NUM_DEST),
    .DataWidth(DATA_WIDTH), .SrcArb({NUM_SRC{1'b1}}), .AddOutputRelay(2'b10)
  ) u_new_a (
    .clk_i, .rst_ni,
    .src_ready_o(a_new_src_ready_o), .src_2_credits_o(a_new_src_2_credits_o),
    .src_valid_i, .src_data_i,
    .dest_ready_i, .dest_valid_o(a_new_dest_valid_o), .dest_data_o(a_new_dest_data_o)
  );

  shire_cache_xbar #(
    .FIFO_DEPTH(FIFO_DEPTH), .NUM_SRC(NUM_SRC), .NUM_DEST(NUM_DEST),
    .DATA_WIDTH(DATA_WIDTH), .SRC_ARB({NUM_SRC{1'b1}}), .ADD_OUTPUT_RELAY(2'b10)
  ) u_orig_a (
    .clock(clk_i), .reset(reset),
    .src_ready(a_orig_src_ready_o), .src_2_credits(a_orig_src_2_credits_o),
    .src_valid(src_valid_i), .src_data(src_data_i),
    .dest_ready(dest_ready_i), .dest_valid(a_orig_dest_valid_o), .dest_data(a_orig_dest_data_o)
  );

  // ══════════════════════════════════════════════════════════
  // Config B: SrcArb=0, AddOutputRelay=0
  // ══════════════════════════════════════════════════════════

  shirecache_xbar #(
    .FifoDepth(FIFO_DEPTH), .NumSrc(NUM_SRC), .NumDest(NUM_DEST),
    .DataWidth(DATA_WIDTH), .SrcArb('0), .AddOutputRelay('0)
  ) u_new_b (
    .clk_i, .rst_ni,
    .src_ready_o(b_new_src_ready_o), .src_2_credits_o(b_new_src_2_credits_o),
    .src_valid_i, .src_data_i,
    .dest_ready_i, .dest_valid_o(b_new_dest_valid_o), .dest_data_o(b_new_dest_data_o)
  );

  shire_cache_xbar #(
    .FIFO_DEPTH(FIFO_DEPTH), .NUM_SRC(NUM_SRC), .NUM_DEST(NUM_DEST),
    .DATA_WIDTH(DATA_WIDTH), .SRC_ARB('0), .ADD_OUTPUT_RELAY('0)
  ) u_orig_b (
    .clock(clk_i), .reset(reset),
    .src_ready(b_orig_src_ready_o), .src_2_credits(b_orig_src_2_credits_o),
    .src_valid(src_valid_i), .src_data(src_data_i),
    .dest_ready(dest_ready_i), .dest_valid(b_orig_dest_valid_o), .dest_data(b_orig_dest_data_o)
  );

  // ══════════════════════════════════════════════════════════
  // Config C: SrcArb=all-ones, NumSrcMask=3'b101 (src1 disabled)
  // ══════════════════════════════════════════════════════════

  shirecache_xbar #(
    .FifoDepth(FIFO_DEPTH), .NumSrc(NUM_SRC), .NumDest(NUM_DEST),
    .DataWidth(DATA_WIDTH), .NumSrcMask(3'b101), .SrcArb({NUM_SRC{1'b1}}), .AddOutputRelay('0)
  ) u_new_c (
    .clk_i, .rst_ni,
    .src_ready_o(c_new_src_ready_o), .src_2_credits_o(c_new_src_2_credits_o),
    .src_valid_i, .src_data_i,
    .dest_ready_i, .dest_valid_o(c_new_dest_valid_o), .dest_data_o(c_new_dest_data_o)
  );

  shire_cache_xbar #(
    .FIFO_DEPTH(FIFO_DEPTH), .NUM_SRC(NUM_SRC), .NUM_DEST(NUM_DEST),
    .DATA_WIDTH(DATA_WIDTH), .NUM_SRC_MASK(3'b101), .SRC_ARB({NUM_SRC{1'b1}}), .ADD_OUTPUT_RELAY('0)
  ) u_orig_c (
    .clock(clk_i), .reset(reset),
    .src_ready(c_orig_src_ready_o), .src_2_credits(c_orig_src_2_credits_o),
    .src_valid(src_valid_i), .src_data(src_data_i),
    .dest_ready(dest_ready_i), .dest_valid(c_orig_dest_valid_o), .dest_data(c_orig_dest_data_o)
  );

endmodule
/* verilator lint_on UNUSEDSIGNAL */
