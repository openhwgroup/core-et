// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original gen_mem1p vs new prim_ram_1p.

`include "soc.vh"

module cosim_ram_1p_tb #(
  parameter int unsigned WIDTH = 32,
  parameter int unsigned DEPTH = 256
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        req_i,
  input  logic                        we_i,
  input  logic [$clog2(DEPTH)-1:0]    addr_i,
  input  logic [WIDTH-1:0]            wdata_i,
  output logic [WIDTH-1:0]            new_rdata_o,
  output logic [WIDTH-1:0]            orig_rdata_o
);

  prim_ram_1p #(.Width(WIDTH), .Depth(DEPTH)) u_new (
    .clk_i,
    .rst_ni,
    .req_i,
    .we_i,
    .addr_i,
    .wdata_i,
    .rdata_o (new_rdata_o),
    .cfg_i   (ram_cfg_pkg::ram_cfg_t'('0)),
    .dft_i   (dft_pkg::dft_t'('0)),
    .alert_o ()
  );

  gen_mem1p #(.WIDTH(WIDTH), .WORDS(DEPTH)) u_orig (
    .clock   (clk_i),
    .address (addr_i),
    .en      (req_i),
    .we      (we_i),
    .din     (wdata_i),
    .dout    (orig_rdata_o)
  );

endmodule
