// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_normalize
#(parameter E = 5,
  parameter S_IN = 0,
  parameter M_IN = 9,
  parameter S_OUT = 1,
  parameter M_OUT = 10
)
(in, rm, out, flags);

// LOCAL PARAMETERS
localparam W_IN = S_IN+E+M_IN;
localparam W_OUT = S_OUT+E+M_OUT;
localparam M_EXT = max(M_IN+1,M_OUT);
localparam M_EXTRA_BITS = M_EXT - M_OUT;

// INPUT/OUTPUT INTERFACE
input  logic [W_IN-1:0] in;
input  fp_rounding_mode rm;
output logic [W_OUT-1:0] out;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
// Extract floating point components
logic in_sign;
generate if (S_IN > 0)
begin: SIGNED_IN
  assign in_sign = in[W_IN-1];
end
else
begin: UNSIGNED_IN
  assign in_sign = 1'b0;
end
endgenerate

logic [E-1:0] in_exp;
logic [M_IN-1:0] in_mant;
assign in_exp  = in[M_IN +: E];
// NOTE: Fractional input mantissa is not normalized and does not include an implicit leading 1
// i.e. Real value of mantissa is 0.in_mant
assign in_mant = in[0 +: M_IN];

// Generate extended mantissa
logic [M_EXT-1:0] mant_ext;
assign mant_ext = in_mant << (M_EXT-M_IN);

// In case of exp == 0, readjust exponent and mantissa
logic [E-1:0] exp_explicit;
logic [M_EXT-1:0] mant_explicit;
assign exp_explicit = (in_exp == '0) ? E'(unsigned'(1)) : in_exp;
assign mant_explicit = (in_exp == '0) ? mant_ext >> 1 : mant_ext;

// Obtain the number of leading zeros of the mantissa
// Only the first M_IN bits are checked (the remaining bits are 0)
localparam LZ_BITS = $clog2(1+M_IN);
logic [LZ_BITS-1:0] leading_zeros, shift;
logic is_zero, is_tiny_before_rounding;
leading_zero_counter #(.N_BITS(1+M_IN)) lzc (.n({1'b0,mant_explicit[M_EXT-1 -: M_IN]}), .*);
// Detect tininess before rounding
assign is_tiny_before_rounding = exp_explicit <= leading_zeros;
// Obtain the actual shift to be applied (maximum shift will be exp_explicit-1)
assign shift = ~is_tiny_before_rounding ? leading_zeros : LZ_BITS'(exp_explicit-1'b1);

// Apply shifting and rounding
// The actual shift of the mantissa must take into account the extra fractional bits added in the concatenation
logic [E-1:0] exp_shifted;
logic inexact;
logic [(1+M_OUT)-1:0] mant_shifted;
logic [(2+M_OUT)-1:0] mant_rounded;
logic is_tiny_after_rounding;

assign exp_shifted = is_zero ? E'(unsigned'(1)) : exp_explicit - E'(shift);

generate if (M_EXTRA_BITS > 0)
begin: IN_IS_WIDER
  logic [(1+M_EXT)-1:0] shifted_out_bits;
  logic round;

  assign shifted_out_bits = (shift < M_EXTRA_BITS) ? {1'b0,mant_explicit}<<(1+M_EXT-M_EXTRA_BITS + shift) : '0;
  assign inexact = |shifted_out_bits;
  assign mant_shifted = (shift > M_EXTRA_BITS) ? (1+M_OUT)'(mant_explicit << (shift - M_EXTRA_BITS)) : (1+M_OUT)'(mant_explicit >> (M_EXTRA_BITS - shift));

  assign round = (rm == RTZ) ?                          1'b0 : // Round towards Zero
                 (rm == RDN) ?             in_sign & inexact : // Round Down (towards -Inf)
                 (rm == RUP) ?            ~in_sign & inexact : // Round Up (towards +Inf)
                 (rm == RMM) ? shifted_out_bits[(1+M_EXT)-1] : // Round to Nearest, ties to Max Magnitude
                 shifted_out_bits[(1+M_EXT)-1] & |{mant_shifted[0],shifted_out_bits[(1+M_EXT)-2:0]}; // Round to Nearest, ties to Even
  assign mant_rounded = mant_shifted + (1+M_OUT)'(round);

  // Detect tininess after rounding
  // To detect tininess after rounding, check that a non-zero result computed as though the exponent range were unbounded
  // would lie strictly between +/-2^emin
  logic [(1+M_EXT)-1:0] shifted_out_bits_tiny;
  logic inexact_tiny;
  logic [(1+M_OUT)-1:0] mant_shifted_tiny;
  logic round_tiny;
  // If exponent would be < 0, result is tiny
  // If exponent would be 0, check whether the sum of the truncated mantissa plus the round bit would overflow (i.e. they are all ones)
  // Otherwise, result is tiny
  // Obtain rounding for exp == 0
  assign shifted_out_bits_tiny = (exp_explicit < M_EXTRA_BITS) ? {1'b0,mant_explicit}<<(1+M_EXT-M_EXTRA_BITS + exp_explicit) : '0;
  assign inexact_tiny = |shifted_out_bits_tiny;
  assign mant_shifted_tiny = (exp_explicit > M_EXTRA_BITS) ? (1+M_OUT)'(mant_explicit << (exp_explicit - M_EXTRA_BITS)) : (1+M_OUT)'(mant_explicit >> (M_EXTRA_BITS - exp_explicit));
  assign round_tiny = (rm == RTZ) ?                               1'b0 : // Round towards Zero
                      (rm == RDN) ?             in_sign & inexact_tiny : // Round Down (towards -Inf)
                      (rm == RUP) ?            ~in_sign & inexact_tiny : // Round Up (towards +Inf)
                      (rm == RMM) ? shifted_out_bits_tiny[(1+M_EXT)-1] : // Round to Nearest, ties to Max Magnitude
                      shifted_out_bits_tiny[(1+M_EXT)-1] & |{mant_shifted_tiny[0],shifted_out_bits_tiny[(1+M_EXT)-2:0]}; // Round to Nearest, ties to Even
  assign is_tiny_after_rounding = (exp_explicit < leading_zeros) || ((exp_explicit == leading_zeros) && (&{mant_shifted_tiny,round_tiny} == 1'b0));
end
else
begin: OUT_IS_WIDER
  assign inexact = 1'b0;
  assign mant_shifted = (1+M_OUT)'(mant_explicit << shift);
  assign mant_rounded = (2+M_OUT)'(mant_shifted);
  // In this case, no rounding would occur were the exponent range unbounded
  // (thus no overflow which undoes tininess can occur)
  assign is_tiny_after_rounding = is_tiny_before_rounding;
end
endgenerate

// Set underflow flag
logic unf;
assign unf = is_tiny_after_rounding & ~is_zero;
  
// Apply additional shifting if necessary
logic [E-1:0] exp_norm;
logic [(1+M_OUT)-1:0] mant_norm;
always_comb
begin
  // If overflow occurred during rounding, shift right and increment exponent
  if (mant_rounded[(2+M_OUT)-1] == 1'b1)
  begin
    exp_norm = exp_shifted + 1'b1;
    mant_norm = mant_rounded[(2+M_OUT)-1:1];
  end
  else
  begin
    exp_norm = exp_shifted;
    mant_norm = mant_rounded[(2+M_OUT)-2:0];
  end
end

// Discard implicit leading 1
logic out_sign;
logic [E-1:0] out_exp;
logic [M_OUT-1:0] out_mant;
// If the resulting exponent is 1 and the mantissa is not normalized,
// then set exponent to 0
assign out_sign = in_sign;
assign out_exp = ((exp_norm == E'(unsigned'(1))) && (mant_norm[(M_OUT+1)-1] == 1'b0)) ? E'(0) : exp_norm;
assign out_mant = mant_norm[M_OUT-1:0];

// Assign outputs
assign flags[`FP_FLAG_INVALID] = 1'b0;
assign flags[`FP_FLAG_DIV0] = 1'b0;
generate if (S_OUT > 0)
begin: SIGNED_OUT
  assign out = {out_sign,out_exp,out_mant};
  assign flags[`FP_FLAG_OVF] = 1'b0;
  assign flags[`FP_FLAG_UNF] = unf&inexact;
  assign flags[`FP_FLAG_INEXACT] = inexact;
end
else
begin: UNSIGNED_OUT
  always_comb
  begin
    // If the input is negative but output does not have sign, saturate to 0
    if (in_sign == 1'b1)
    begin
      out = W_OUT'(0);
      flags[`FP_FLAG_OVF] = ~is_zero;
      flags[`FP_FLAG_UNF] = unf;
      flags[`FP_FLAG_INEXACT] = ~is_zero;
    end
    else
    begin
      out = {out_exp,out_mant};
      flags[`FP_FLAG_OVF] = 1'b0;
      flags[`FP_FLAG_UNF] = unf&inexact;
      flags[`FP_FLAG_INEXACT] = inexact;
    end
  end
end
endgenerate

// Helper functions
function automatic int max (int a, int b);
  int res;

  if (a > b)
    res = a;
  else
    res = b;

  return res;
endfunction

endmodule
