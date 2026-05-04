// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original gen_mem2p vs new prim_ram_2p.

`include "soc.vh"

module cosim_ram_2p_tb #(
  parameter int unsigned WIDTH = 32,
  parameter int unsigned DEPTH = 256
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  // Port A
  input  logic                        a_req_i,
  input  logic                        a_we_i,
  input  logic [$clog2(DEPTH)-1:0]    a_addr_i,
  input  logic [WIDTH-1:0]            a_wdata_i,
  output logic [WIDTH-1:0]            new_a_rdata_o,
  output logic [WIDTH-1:0]            orig_a_rdata_o,
  // Port B
  input  logic                        b_req_i,
  input  logic                        b_we_i,
  input  logic [$clog2(DEPTH)-1:0]    b_addr_i,
  input  logic [WIDTH-1:0]            b_wdata_i,
  output logic [WIDTH-1:0]            new_b_rdata_o,
  output logic [WIDTH-1:0]            orig_b_rdata_o
);

  prim_ram_2p #(.Width(WIDTH), .Depth(DEPTH)) u_new (
    .clk_i,
    .rst_ni,
    .a_req_i, .a_we_i, .a_addr_i, .a_wdata_i, .a_rdata_o(new_a_rdata_o),
    .b_req_i, .b_we_i, .b_addr_i, .b_wdata_i, .b_rdata_o(new_b_rdata_o),
    .cfg_i   (ram_cfg_pkg::ram_cfg_t'('0)),
    .dft_i   (dft_pkg::dft_t'('0)),
    .alert_o ()
  );

  gen_mem2p #(.WIDTH(WIDTH), .WORDS(DEPTH)) u_orig (
    .clock    (clk_i),
    .addressA (a_addr_i),
    .enA      (a_req_i),
    .weA      (a_we_i),
    .dinA     (a_wdata_i),
    .doutA    (orig_a_rdata_o),
    .addressB (b_addr_i),
    .enB      (b_req_i),
    .weB      (b_we_i),
    .dinB     (b_wdata_i),
    .doutB    (orig_b_rdata_o)
  );

endmodule
