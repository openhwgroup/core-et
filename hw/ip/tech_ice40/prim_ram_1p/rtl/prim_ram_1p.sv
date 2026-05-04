// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Single-port RAM wrapper — iCE40 FPGA implementation.
//
// Forces block RAM inference via ram_style attribute, mapping to
// SB_RAM40_4K primitives. Functionally identical to the generic
// behavioral model.

module prim_ram_1p
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 256,
  localparam int unsigned AddrW = $clog2(Depth)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic              req_i,
  input  logic              we_i,
  input  logic [AddrW-1:0]  addr_i,
  input  logic [Width-1:0]  wdata_i,
  output logic [Width-1:0]  rdata_o,

  input  ram_cfg_t          cfg_i,
  input  dft_t              dft_i,

  output logic              alert_o
);

  (* ram_style = "block" *)
  logic [Width-1:0] mem [Depth];

  always_ff @(posedge clk_i) begin
    if (req_i) begin
      if (we_i) begin
        mem[addr_i] <= wdata_i;
      end else begin
        rdata_o <= mem[addr_i];
      end
    end
  end

  assign alert_o = 1'b0;

  logic unused;
  assign unused = ^{cfg_i, dft_i, rst_ni};

endmodule
