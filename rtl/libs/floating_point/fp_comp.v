// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_comp
#(parameter E = 8,
  parameter M = 23,
  parameter USE_EXT_FCLASS = 0
)
(a, b, signaling, fclassa, fclassb, gt, eq, lt, flags);

// LOCAL PARAMETERS
localparam W = 1+E+M;
localparam MAX_EXP = (1<<E) - 1;

// INPUT/OUTPUT INTERFACE
input  logic [W-1:0] a, b;
input  logic signaling;
input  logic [`FP_CLASS_BITS-1:0] fclassa;
input  logic [`FP_CLASS_BITS-1:0] fclassb;
output logic gt, eq, lt;
output logic [`FP_NUM_FLAGS-1:0] flags;

// Extract floating point components
logic a_sign, b_sign;
logic [E-1:0] a_exp, b_exp;
logic [M-1:0] a_mant, b_mant;

assign a_sign = a[W-1];
assign a_exp  = a[M +: E];
assign a_mant = a[0 +: M];
assign b_sign = b[W-1];
assign b_exp  = b[M +: E];
assign b_mant = b[0 +: M];

// Make some basic checkings
logic a_is_NaN, a_is_sNaN, a_is_zero;
logic b_is_NaN, b_is_sNaN, b_is_zero;
logic invalid;

generate if (USE_EXT_FCLASS)
begin: EXT_FCLASS
  assign a_is_NaN = fclassa[`FP_CLASS_SNAN] | fclassa[`FP_CLASS_QNAN];
  assign b_is_NaN = fclassb[`FP_CLASS_SNAN] | fclassb[`FP_CLASS_QNAN];
  assign a_is_sNaN = fclassa[`FP_CLASS_SNAN];
  assign b_is_sNaN = fclassb[`FP_CLASS_SNAN];
  assign a_is_zero = fclassa[`FP_CLASS_NEG_ZERO] | fclassa[`FP_CLASS_POS_ZERO];
  assign b_is_zero = fclassb[`FP_CLASS_NEG_ZERO] | fclassb[`FP_CLASS_POS_ZERO];
end
else
begin: IN_FCLASS
  logic a_max_exp, b_max_exp, a_mant_zero, b_mant_zero;
  assign a_max_exp = a_exp == MAX_EXP;
  assign b_max_exp = b_exp == MAX_EXP;
  assign a_mant_zero = a_mant == M'(0);
  assign b_mant_zero = b_mant == M'(0);
  assign a_is_NaN = a_max_exp & ~a_mant_zero;
  assign b_is_NaN = b_max_exp & ~b_mant_zero;
  assign a_is_sNaN = a_is_NaN & (a_mant[M-1] == 1'b0);
  assign b_is_sNaN = b_is_NaN & (b_mant[M-1] == 1'b0);
  assign a_is_zero = (a_exp == E'(0)) & a_mant_zero;
  assign b_is_zero = (b_exp == E'(0)) & b_mant_zero;
end
endgenerate

assign invalid = signaling ? a_is_NaN | b_is_NaN : a_is_sNaN | b_is_sNaN;

// Compare
always_comb
begin
  gt = 1'b0;
  eq = 1'b0;
  lt = 1'b0;
  flags = `FP_NUM_FLAGS'b0;

  // If any of the numbers is a NaN, they are not comparable
  if (~a_is_NaN & ~b_is_NaN)
  begin
    if (a_is_zero & b_is_zero)
      // If both are zero, they equal each other
      eq = 1'b1;
    else if (a_sign != b_sign)
    begin
      // If they have different sign, the one with positive sign is greater
      if (a_sign == 1'b0)
        gt = 1'b1;
      else
        lt = 1'b1;
    end
    else if ({a_exp,a_mant} != {b_exp,b_mant})
    begin
      // If they have different exponent or mantissa, the one with greater exponent/mantissa is greater
      // (unless the sign is negative, in which case it is the other way around)
      if ({a_exp,a_mant} > {b_exp,b_mant})
      begin
        gt = 1'b1 ^ a_sign;
        lt = 1'b0 ^ a_sign;
      end
      else
      begin
        gt = 1'b0 ^ a_sign;
        lt = 1'b1 ^ a_sign;
      end
    end
    else
      // Otherwise, they equal each other
      eq = 1'b1;
  end

  // Flags
  flags[`FP_FLAG_INVALID] = invalid;
end

endmodule
