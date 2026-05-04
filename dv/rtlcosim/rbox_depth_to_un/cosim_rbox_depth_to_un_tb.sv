// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_depth_to_un vs new rbox_depth_to_un.
//
// Both modules convert FP32 to unsigned normalized integer (UNORM).
// Both are purely combinational.

`include "soc.vh"

module cosim_rbox_depth_to_un_tb #(
  parameter int unsigned UN_SZ = 24
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic [31:0]       in_i,

  // New module output
  output logic [UN_SZ-1:0]  new_out_o,

  // Original module output
  output logic [UN_SZ-1:0]  orig_out_o
);

  // ── New module ────────────────────────────────────────

  rbox_depth_to_un #(
    .UnSz (UN_SZ)
  ) u_new (
    .in_i  (in_i),
    .out_o (new_out_o)
  );

  // ── Original module (inlined, renamed to avoid collision) ─

  rbox_depth_to_un_orig #(
    .UN_SZ (UN_SZ)
  ) u_orig (
    .in  (in_i),
    .out (orig_out_o)
  );

endmodule

// Inlined copy of original rbox_depth_to_un (renamed to avoid module collision)
module rbox_depth_to_un_orig
#(
    parameter UN_SZ = 24
)
(
    input  logic [31:0]       in,
    output logic [UN_SZ-1:0]  out
);

logic             sign;
logic [7:0]       exp;
logic [22:0]      mant;
logic             infinite;
logic             nan;
logic             zero;
logic             denorm;
logic [23:0]      mant_full;
logic [25:0]      mant_align;
logic [25:0]      val_un;
logic [UN_SZ-1:0] val_out;
logic [7:0]       align_shift;
logic             un_round;
logic             out_round_up;


always_comb
begin
    sign = in[31];
    exp  = in[30:23];
    mant = in[22:0];

    zero     = (exp == '0) && (mant == '0);
    denorm   = (exp == '0) && (mant != '0);
    infinite = (exp == '1) && (mant == '0);
    nan      = (exp == '1) && (mant != '0);

    mant_full = {!denorm, mant};

    align_shift  = '0;
    mant_align   = '0;
    out_round_up = '0;
    val_out      = '0;

    if (sign)               // Negatives to 0, shouldn't be a legal depth value
        out = '0;
    if (infinite)        // Infinites to 0, shouldn't be a legal depth value
        out = '0;
    else if (zero)       // Zero is zero ... in this case a +0 because we already tested sign
        out = '0;
    else if (nan)        // NaNs to 0, shouldn't be a legal depth value
        out = '0;
    else if (exp >= 8'h7f) // Anything larger than 1.0 becomes MAX_DEPTH
        out = '1;
    else
    begin
        // Align so MSB corresponds with exponent 0
        align_shift = 8'h7f - exp;
        mant_align = {mant_full, 3'b0} >> align_shift;

        // fp32 to unorm conversion : v_un = int(v_fp32*(2^n-1))
        // implemented as : mantissa - (mantissa >> n)
        val_un = mant_align - {{UN_SZ{1'b0}}, {mant_align[25:UN_SZ]}};

        // Final rounding to integer value (round-to-nearest-even)
        out_round_up =      (val_un[(26-UN_SZ-1) -: 2] == 2'b11)
                     || (    val_un[(26-UN_SZ  )     ]
                         && (val_un[(26-UN_SZ-1) -: 2] == 2'b10));

        // Check overflow if too close to 1.0
        val_out = (val_un[24 -: UN_SZ] == '1) ? '1 : (val_un[24 -: UN_SZ] + out_round_up);

        out = val_out;
    end

end

endmodule
