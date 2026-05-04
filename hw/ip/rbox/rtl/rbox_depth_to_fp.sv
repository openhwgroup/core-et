// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX internal depth to IEEE754 FP32 converter.
//
// Converts the internal depth representation (decomposed mantissa,
// exponent, flags) back to a standard 32-bit floating-point value.
// Uses Round-To-Zero (truncation).
//
// Pure combinational logic.

module rbox_depth_to_fp
  import rbox_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */ // internal_depth_t.subnorm unused by this converter
  input  internal_depth_t in_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic [31:0]     out_o
);

  logic        sign;
  logic [7:0]  exp_final;
  /* verilator lint_off UNUSEDSIGNAL */ // mant_shifted[23] is the implicit 1 bit, not part of IEEE754 mantissa
  logic [23:0] mant_shifted;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [22:0] mant_final;
  logic [4:0]  renorm_shift;

  always_comb begin
    sign         = in_i.s;
    renorm_shift = '0;
    mant_shifted = '0;
    exp_final    = '0;
    mant_final   = '0;

    if (in_i.infinite)
      out_o = {sign, 8'hff, 23'h0};
    else if (in_i.nan)
      out_o = {sign, 8'hff, 23'h20_0000};
    else if (in_i.zero)
      out_o = '0;
    else begin
      renorm_shift = in_i.m_shift;
      mant_shifted = in_i.m << renorm_shift;

      // Overflow detection is handled by the adder.
      exp_final = in_i.e + 8'($signed(in_i.e_delta));

      // Depth interpolation uses Round-To-Zero.
      mant_final = mant_shifted[22:0];

      out_o = {sign, exp_final, mant_final};
    end
  end

endmodule
