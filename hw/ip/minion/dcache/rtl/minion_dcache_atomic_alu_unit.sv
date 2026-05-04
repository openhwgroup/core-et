// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache atomic ALU.
//
// Reimplements `dcache_atomic_alu_unit` from the original CORE-ET minion.
// The logic is purely combinational, but the original interface carried the
// minion clock/reset pins, so they are preserved here for compatibility.

/* verilator lint_off UNUSED */  // clk/reset preserved to match the original interface
module minion_dcache_atomic_alu_unit
  import minion_dcache_pkg::*;
(
  input  logic              clk_i,
  input  logic              rst_ni,
  input  logic [2:0]        addr_i,
  input  dcache_cmd_e       cmd_i,
  input  logic [1:0]        typ_i,
  input  logic [XregSize-1:0] lhs_i,
  input  logic [XregSize-1:0] rhs_i,
  output logic [XregSize-1:0] out_o
);

  logic sgned;
  logic max_sel;
  logic min_sel;

  logic [XregSize-1:0] adder_out;
  logic [XregSize-1:0] adder_mask;
  logic [XregSize-1:0] rhs_int;
  logic                word;
  logic                cmp_lhs;
  logic                cmp_rhs;
  logic                lt_lo;
  logic                lt_hi;
  logic                eq_hi;
  logic                lt;
  logic                less;

  logic [XregSize-1:0] out_mux;
  logic [7:0]          wmask_byte;

  always_comb begin
    sgned = (cmd_i == DcCmdXaMin) || (cmd_i == DcCmdXaMax);
    max_sel = (cmd_i == DcCmdXaMax) || (cmd_i == DcCmdXaMaxu);
    min_sel = (cmd_i == DcCmdXaMin) || (cmd_i == DcCmdXaMinu);

    if (typ_i == 2'b00) begin
      rhs_int = {8{rhs_i[7:0]}};
    end else if (typ_i == 2'b01) begin
      rhs_int = {4{rhs_i[15:0]}};
    end else if (typ_i == 2'b10) begin
      rhs_int = {2{rhs_i[31:0]}};
    end else begin
      rhs_int = rhs_i;
    end

    adder_mask = {XregSize{1'b1}} ^ ({{(XregSize-1){1'b0}}, addr_i[2]} << 31);
    adder_out = (lhs_i & adder_mask) + (rhs_int & adder_mask);

    word = !typ_i[0];
    cmp_lhs = (word && !addr_i[2]) ? lhs_i[31] : lhs_i[XregSize-1];
    cmp_rhs = (word && !addr_i[2]) ? rhs_int[31] : rhs_int[XregSize-1];
    lt_lo = lhs_i[31:0] < rhs_int[31:0];
    lt_hi = lhs_i[63:32] < rhs_int[63:32];
    eq_hi = lhs_i[63:32] == rhs_int[63:32];
    lt = word ? (addr_i[2] ? lt_hi : lt_lo) : (lt_hi || (eq_hi && lt_lo));
    less = (cmp_lhs == cmp_rhs) ? lt : (sgned ? cmp_lhs : cmp_rhs);

    if (cmd_i == DcCmdXaAdd) begin
      out_mux = adder_out;
    end else if (cmd_i == DcCmdXaAnd) begin
      out_mux = lhs_i & rhs_int;
    end else if (cmd_i == DcCmdXaOr) begin
      out_mux = lhs_i | rhs_int;
    end else if (cmd_i == DcCmdXaXor) begin
      out_mux = lhs_i ^ rhs_int;
    end else begin
      out_mux = ((less ? min_sel : max_sel) ? lhs_i : rhs_int);
    end

    if (typ_i == 2'b00) begin
      wmask_byte = 8'b0000_0001;
    end else if (typ_i == 2'b01) begin
      wmask_byte = 8'b0000_0011;
    end else if (typ_i == 2'b10) begin
      wmask_byte = 8'b0000_1111;
    end else begin
      wmask_byte = 8'b1111_1111;
    end
    wmask_byte = wmask_byte << addr_i;

    out_o = lhs_i;
    for (int unsigned i = 0; i < 8; i++) begin
      out_o[i*8 +: 8] = (out_mux[i*8 +: 8] & {8{wmask_byte[i]}})
                      | (lhs_i  [i*8 +: 8] & {8{~wmask_byte[i]}});
    end
  end

endmodule
