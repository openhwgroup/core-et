// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Dual-port RAM wrapper — Xilinx FPGA implementation.
//
// Forces block RAM inference via ram_style attribute.
// Functionally identical to the generic behavioral model.

module prim_ram_2p
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 256,
  localparam int unsigned AddrW = $clog2(Depth)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Port A
  input  logic              a_req_i,
  input  logic              a_we_i,
  input  logic [AddrW-1:0]  a_addr_i,
  input  logic [Width-1:0]  a_wdata_i,
  output logic [Width-1:0]  a_rdata_o,

  // Port B
  input  logic              b_req_i,
  input  logic              b_we_i,
  input  logic [AddrW-1:0]  b_addr_i,
  input  logic [Width-1:0]  b_wdata_i,
  output logic [Width-1:0]  b_rdata_o,

  input  ram_cfg_t          cfg_i,
  input  dft_t              dft_i,

  output logic              alert_o
);

  (* ram_style = "block" *)
  logic [Width-1:0] mem [Depth];

  // Port A
  always_ff @(posedge clk_i) begin
    if (a_req_i) begin
      if (a_we_i) begin
        mem[a_addr_i] <= a_wdata_i;
      end else begin
        a_rdata_o <= mem[a_addr_i];
      end
    end
  end

  // Port B
  always_ff @(posedge clk_i) begin
    if (b_req_i) begin
      if (b_we_i) begin
        mem[b_addr_i] <= b_wdata_i;
      end else begin
        b_rdata_o <= mem[b_addr_i];
      end
    end
  end

  assign alert_o = 1'b0;

  logic unused;
  assign unused = ^{cfg_i, dft_i, rst_ni};

endmodule
