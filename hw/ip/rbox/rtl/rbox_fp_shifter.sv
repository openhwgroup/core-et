// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX floating-point barrel shifter.
//
// Bidirectional shifter used by both rbox_fp_adder_2stg and
// rbox_fp_fast_adder for mantissa alignment.
//
// The shift input is in sign-magnitude format:
//   shift[ShiftSz]   = direction (1 = right-shift, 0 = left-shift)
//   shift[ShiftSz-1:0] = magnitude
//
// Left-shift: out = in >> shift_value (with optional sign extension)
// Right-shift: out = in << shift_value
// sticky: OR-reduction of bits shifted out during left-shift
//
// Pure combinational logic.

module rbox_fp_shifter #(
  parameter int unsigned Sz         = 16,
  parameter bit          SignExtend = 0,
  parameter int unsigned ShiftSz    = 4
) (
  input  logic [Sz-1:0]      in_i,
  input  logic [ShiftSz:0]   shift_i,    // Sign-magnitude: [ShiftSz]=dir, [ShiftSz-1:0]=amount

  output logic [Sz-1:0]      out_o,
  output logic                sticky_o
);

  logic                  shift_right;
  logic [ShiftSz-1:0]   shift_value;

  /* verilator lint_off UNUSEDSIGNAL */ // Upper Sz bits of out_left_shift not used — only middle Sz bits selected
  logic [3*Sz-1:0]       out_left_shift;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [Sz-1:0]         out_right_shift;

  always_comb begin
    // Negative shift values mean shift direction is to the right.
    shift_right = shift_i[ShiftSz];
    shift_value = shift_i[ShiftSz-1:0];

    out_left_shift  = {{Sz{SignExtend ? in_i[Sz-1] : 1'b0}}, in_i, {Sz{1'b0}}} >> shift_value;
    out_right_shift = in_i << shift_value;

    out_o    = shift_right ? out_right_shift : out_left_shift[2*Sz-1:Sz];
    sticky_o = shift_right ? 1'b0           : |out_left_shift[Sz-1:0];
  end

endmodule
