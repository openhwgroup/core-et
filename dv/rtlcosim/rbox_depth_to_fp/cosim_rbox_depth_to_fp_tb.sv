// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_depth_to_fp vs new rbox_depth_to_fp.

`include "soc.vh"

module cosim_rbox_depth_to_fp_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus — internal depth fields driven individually
  input  logic              in_infinite,
  input  logic              in_nan,
  input  logic              in_zero,
  input  logic              in_subnorm,
  input  logic [7:0]        in_e,
  input  logic              in_s,
  input  logic [23:0]       in_m,
  input  logic [5:0]        in_e_delta,
  input  logic [4:0]        in_m_shift,

  // New module output
  output logic [31:0]       new_out_o,

  // Original module output
  output logic [31:0]       orig_out_o
);

  import rbox_pkg::*;

  // Pack fields into internal_depth_t for new module
  internal_depth_t in_packed;
  assign in_packed.infinite = in_infinite;
  assign in_packed.nan      = in_nan;
  assign in_packed.zero     = in_zero;
  assign in_packed.subnorm  = in_subnorm;
  assign in_packed.e        = in_e;
  assign in_packed.s        = in_s;
  assign in_packed.m        = in_m;
  assign in_packed.e_delta  = in_e_delta;
  assign in_packed.m_shift  = in_m_shift;

  // Pack fields into rbox_internal_depth_t for original module
  rbox_internal_depth_t orig_in;
  assign orig_in.infinite = in_infinite;
  assign orig_in.nan      = in_nan;
  assign orig_in.zero     = in_zero;
  assign orig_in.subnorm  = in_subnorm;
  assign orig_in.e        = in_e;
  assign orig_in.s        = in_s;
  assign orig_in.m        = in_m;
  assign orig_in.e_delta  = in_e_delta;
  assign orig_in.m_shift  = in_m_shift;

  // ── New module ────────────────────────────────────────

  rbox_depth_to_fp u_new (
    .in_i  (in_packed),
    .out_o (new_out_o)
  );

  // ── Original module (inlined, renamed to avoid collision) ─

  rbox_depth_to_fp_orig u_orig (
    .in  (orig_in),
    .out (orig_out_o)
  );

endmodule

// Inlined copy of original rbox_depth_to_fp (renamed to avoid module collision)
module rbox_depth_to_fp_orig (
    input rbox_internal_depth_t in,
    output logic [31:0]         out
);

logic        sign;
logic [7:0]  exp_final;
logic [23:0] mant_shifted;
logic [22:0] mant_final;
logic [4:0]  renorm_shift;

always_comb
begin
    sign = in.s;
    if (in.infinite)
        out = {sign, 8'hff, 23'h0};
    else if (in.nan)
        out = {sign, 8'hff, 23'h20_0000};
    else if (in.zero)
        out = 0;
    else
    begin
        renorm_shift = in.m_shift;
        mant_shifted = in.m << renorm_shift;
        exp_final = in.e + `SX(8, in.e_delta);
        mant_final = mant_shifted[22:0];
        out = {sign, exp_final, mant_final};
    end
end

endmodule
