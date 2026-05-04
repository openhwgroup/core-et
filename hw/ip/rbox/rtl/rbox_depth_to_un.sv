// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX FP32 to unsigned normalized (UNORM) converter.
//
// Converts a [0.0, 1.0] FP32 depth value to an unsigned normalized
// integer of parameterized width (default 24 for D24_UNORM).
// Values outside [0, 1] are clamped. Uses round-to-nearest-even.
//
// Pure combinational logic.

module rbox_depth_to_un #(
  parameter int unsigned UnSz = 24
) (
  input  logic [31:0]     in_i,
  output logic [UnSz-1:0] out_o
);

  logic             sign;
  logic [7:0]       exp_val;
  logic [22:0]      mant;
  logic             infinite;
  logic             nan;
  logic             zero;
  logic             denorm;
  logic [23:0]      mant_full;
  logic [25:0]      mant_align;
  logic [25:0]      val_un;
  logic [UnSz-1:0]  val_out;
  logic [7:0]       align_shift;
  logic             out_round_up;

  always_comb begin
    sign    = in_i[31];
    exp_val = in_i[30:23];
    mant    = in_i[22:0];

    zero     = (exp_val == '0) && (mant == '0);
    denorm   = (exp_val == '0) && (mant != '0);
    infinite = (exp_val == '1) && (mant == '0);
    nan      = (exp_val == '1) && (mant != '0);

    mant_full = {!denorm, mant};

    align_shift  = '0;
    mant_align   = '0;
    out_round_up = '0;
    val_out      = '0;
    val_un       = '0;

    // Sign check is standalone — later conditions may override.
    // Matches original: negative values with exp >= 0x7f still clamp to MAX_DEPTH.
    if (sign)
      out_o = '0;               // Negatives → 0 (preliminary, may be overridden)
    if (infinite)
      out_o = '0;               // Infinites → 0
    else if (zero)
      out_o = '0;               // Zero is zero
    else if (nan)
      out_o = '0;               // NaNs → 0
    else if (exp_val >= 8'h7f)
      out_o = '1;               // >= 1.0 → MAX_DEPTH
    else begin
      // Align so MSB corresponds with exponent 0
      align_shift = 8'h7f - exp_val;
      mant_align = 26'({mant_full, 3'b0} >> align_shift);

      // FP32 to UNORM conversion: v_un = int(v_fp32 * (2^n - 1))
      // Implemented as: mantissa - (mantissa >> n)
      val_un = mant_align - {{UnSz{1'b0}}, mant_align[25:UnSz]};

      // Final rounding to integer value (round-to-nearest-even)
      out_round_up =     (val_un[(26-UnSz-1) -: 2] == 2'b11)
                    || (  val_un[(26-UnSz)]
                       && (val_un[(26-UnSz-1) -: 2] == 2'b10));

      // Check overflow if too close to 1.0
      val_out = (val_un[24 -: UnSz] == '1) ? {UnSz{1'b1}} : (val_un[24 -: UnSz] + {{(UnSz-1){1'b0}}, out_round_up});

      out_o = val_out;
    end
  end

endmodule
