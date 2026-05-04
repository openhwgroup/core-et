// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Single-port RAM wrapper.
//
// Behavioral (generic) implementation: inferred RAM with registered
// read output. For ASIC, swap for foundry SRAM macro + ECC + BIST.
//
// Read output is registered (1-cycle read latency). When req_i is
// low, the output holds its previous value.

module prim_ram_1p
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 256,
  // Derived — do not override.
  localparam int unsigned AddrW = $clog2(Depth)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic              req_i,           // request enable
  input  logic              we_i,            // write enable
  input  logic [AddrW-1:0]  addr_i,
  input  logic [Width-1:0]  wdata_i,
  output logic [Width-1:0]  rdata_o,

  input  ram_cfg_t          cfg_i,           // SRAM timing config (ASIC)
  input  dft_t              dft_i,           // DFT control

  output logic              alert_o          // ECC error alert (future)
);

  // ── Behavioral implementation ─────────────────────────

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

  // No ECC in behavioral model.
  assign alert_o = 1'b0;

  // cfg_i and dft_i are used by ASIC implementation only.
  // Suppress unused warnings for behavioral model.
  logic unused;
  assign unused = ^{cfg_i, dft_i, rst_ni};

endmodule
