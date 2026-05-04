// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Dual-port RAM wrapper — iCE40 FPGA implementation.
//
// iCE40 EBR inference wants a true 1W1R/simple-dual-port shape, not a
// fully symmetric dual-port behavioral model. This body therefore realizes
// the current system use pattern directly:
//   - Port A is the write port
//   - Port B is the read port
//
// Read output is registered with one-cycle latency and holds its previous
// value when b_req_i=0.

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

  always_ff @(posedge clk_i) begin
    if (a_req_i && a_we_i) mem[a_addr_i] <= a_wdata_i;
  end

  always_ff @(posedge clk_i) begin
    if (b_req_i && !b_we_i) b_rdata_o <= mem[b_addr_i];
  end

  assign a_rdata_o = '0;

  assign alert_o = 1'b0;

  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_ni && a_req_i && !a_we_i)
      $error("prim_ram_2p (ice40): port A read is unsupported; use port B for reads");
    if (rst_ni && b_req_i && b_we_i)
      $error("prim_ram_2p (ice40): port B write is unsupported; use port A for writes");
  end
`endif
  // synthesis translate_on

  logic unused;
  assign unused = ^{cfg_i, dft_i, rst_ni, b_wdata_i};

endmodule
