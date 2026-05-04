// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed 2R1W register file.
// Generic/default implementation.

module prim_rf_2r1w #(
  parameter int unsigned         Width        = 32,
  parameter int unsigned         Entries      = 32,
  parameter logic [Entries-1:0] Zero         = '0,
  parameter logic [Entries-1:0] Parallel     = '0,
  parameter int unsigned         ParallelW    = 32,
  /* verilator lint_off UNUSEDPARAM */  // retained for ASIC compatibility, unused by the generic body
  parameter int unsigned         Level2CkGate = 0,
  /* verilator lint_on UNUSEDPARAM */
  localparam int unsigned        AddrW        = $clog2(Entries)
) (
  input  logic                   clk_i,
  input  logic [AddrW-1:0]       rd_addr_a_i,
  output logic [Width-1:0]       rd_data_a_o,
  input  logic [AddrW-1:0]       rd_addr_b_i,
  output logic [Width-1:0]       rd_data_b_o,
  output logic [ParallelW-1:0]   rd_par_o,
  input  logic                   wr_en_i,
  input  logic                   wr_data_en_1p_i,
  input  logic [AddrW-1:0]       wr_addr_i,
  input  logic [Width-1:0]       wr_data_i
);

  logic [Width-1:0] rf_q [Entries];
  logic [Width-1:0] wr_data_del_q;

  function automatic int unsigned par_position(input int unsigned pos);
    par_position = 0;
    for (int unsigned j = 0; j < pos; j++) begin
      if (Parallel[j]) par_position++;
    end
  endfunction

  always_ff @(negedge clk_i) begin
    if (wr_data_en_1p_i) wr_data_del_q <= wr_data_i;
  end

  for (genvar g = 0; g < Entries; g++) begin : gen_rf
    localparam logic [AddrW-1:0] EntryIdx = g;
    if (Zero[g]) begin : gen_zero
      assign rf_q[g] = '0;
    end else begin : gen_entry
      always_ff @(posedge clk_i) begin
        if (wr_en_i && (wr_addr_i == EntryIdx)) rf_q[g] <= wr_data_del_q;
      end
    end
  end

  assign rd_data_a_o = rf_q[rd_addr_a_i];
  assign rd_data_b_o = rf_q[rd_addr_b_i];

  always_comb begin
    rd_par_o = '0;
    for (int i = 0; i < Entries; i++) begin
      if (Parallel[i]) begin
        rd_par_o[Width*par_position(i) +: Width] = rf_q[i];
      end
    end
  end

endmodule : prim_rf_2r1w
