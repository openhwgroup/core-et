// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_from_norm_3st
#(parameter N = 8,
  parameter S = 1,
  parameter E = 8,
  parameter M = 23
)
(clock, ready, in, is_signed, rm, out, flags);

// LOCAL PARAMETERS
localparam W = S+E+M;
localparam EXP_OFFSET = unsigned'((1<<(E-1))-1);
localparam MIN_REPS_U = cdiv(1+M+1,N);
localparam MIN_REPS_S = cdiv(1+M+1,N-1);
localparam M_EXT_U = MIN_REPS_U*N;
localparam M_EXT_S = MIN_REPS_S*(N-1);
localparam M_EXT = max(M_EXT_U,M_EXT_S);
localparam N_REPS_U = idiv(M_EXT,N);
localparam REM_U = irem(M_EXT,N);
localparam N_REPS_S = idiv(M_EXT,N-1);
localparam REM_S = irem(M_EXT,N-1);
localparam M_EXTRA_BITS = M_EXT - (1+M);
localparam SPARE_BITS_E_BOUND = M_EXTRA_BITS-(signed'(EXP_OFFSET)-2);
localparam SPARE_BITS_N_BOUND = M_EXTRA_BITS-(N-1);
localparam E_RNG_SMALL = EXP_OFFSET <= N;

// INPUT/OUTPUT INTERFACE
input  logic clock;
input  logic ready;
input  logic [N-1:0] in;
input  logic is_signed;
input  fp_rounding_mode rm;
output logic [W-1:0] out;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE

// Stage 0
// -------
// Obtain sign
logic sign_ext;
assign sign_ext = is_signed & in[N-1];

// Obtain absolute value from input
logic [N-1:0] in_bounded, in_abs;
generate if (N>2)
begin: N_GT_2
  assign in_bounded = (is_signed & (in == {1'b1,(N-1)'(0)})) ? {1'b1,(N-2)'(0),1'b1} : in;
end
else
begin: N_EQ_2
  assign in_bounded = (is_signed & (in == {1'b1,(N-1)'(0)})) ? {N{1'b1}} : in;
end
endgenerate
assign in_abs = (sign_ext == 1'b0) ? in_bounded : -in_bounded;

// Generate not normalized floating point value
// The following identity is used: f = in/(2^N_ABS - 1) = in/2^N_ABS + in/2^(2*N_ABS) + in/2^(3*N_ABS) + ...
// where N_ABS = is_signed ? N-1 : N
logic [E-1:0] ct_exp_ext;
logic [M_EXT-1:0] mant_ext_u, mant_ext_s, mant_ext;
logic is_one_u, is_one_s, is_one;

// Full exponent is -1
assign ct_exp_ext = EXP_OFFSET-1;
// Extended mantissa equals the fractional bits from 'in' replicated N_REPS times
// (so that it is at least as long as a full mantissa plus a round bit)
// Sticky bit is implicitly 1 (except if the result is 0, as in the real result the fractional bits are
// replicated infinitely)
generate
if (REM_U > 0)
begin: REM_U_BITS
  assign mant_ext_u = {{N_REPS_U{in_abs}},in_abs[N-1 -: REM_U]};
end
else
begin: NO_REM_U_BITS
  assign mant_ext_u = {N_REPS_U{in_abs}};
end

if (REM_S > 0)
begin: REM_S_BITS
  assign mant_ext_s = {{N_REPS_S{in_abs[N-2:0]}},in_abs[N-2 -: REM_S]};
end
else
begin: NO_REM_S_BITS
  assign mant_ext_s = {N_REPS_S{in_abs[N-2:0]}};
end
endgenerate

assign mant_ext = is_signed ? mant_ext_s : mant_ext_u;
assign is_one_s = &in_abs[N-2:0];
assign is_one_u = is_one_s & in_abs[N-1];
assign is_one = is_signed ? is_one_s : is_one_u;

// Stage 1
// -------
logic s1_sign_ext;
logic [M_EXT-1:0] s1_mant_ext;
logic s1_is_one;
fp_rounding_mode s1_rm;
always_ff @(posedge clock)
begin
  if (ready)
  begin
    s1_sign_ext <= sign_ext;
    s1_mant_ext <= mant_ext;
    s1_is_one <= is_one;
    s1_rm <= rm;
  end
end

// Obtain the number of leading zeros of the mantissa
// Only the first N bits are checked. If all of them are zero, the result is zero
localparam LZ_BITS = $clog2(N);
logic [LZ_BITS-1:0] leading_zeros, shift;
logic is_zero, is_tiny_before_rounding, inexact;
leading_zero_counter #(.N_BITS(N)) lzc (.n(s1_mant_ext[M_EXT-1 -: N]), .*);
generate if (E_RNG_SMALL)
begin: E_RNG_IS_SMALL
  // Detect tininess before rounding
  assign is_tiny_before_rounding = ct_exp_ext <= leading_zeros;
  // Obtain the actual shift to be applied (maximum shift will be ct_exp_ext-1)
  assign shift = ~is_tiny_before_rounding ? leading_zeros : ct_exp_ext-1;
end
else
begin: E_RNG_IS_BIG
  // If exponent range is big enough, the result cannot be tiny
  assign is_tiny_before_rounding = 1'b0;
  assign shift = leading_zeros;
end
endgenerate
assign inexact = ~is_zero && ~s1_is_one;

// Stage 2
// -------
logic s2_sign_ext;
logic [M_EXT-1:0] s2_mant_ext;
logic [LZ_BITS-1:0] s2_leading_zeros, s2_shift;
logic s2_is_zero, s2_is_one;
logic s2_is_tiny_before_rounding, s2_inexact;
fp_rounding_mode s2_rm;
always_ff @(posedge clock)
begin
  if (ready)
  begin
    s2_sign_ext <= s1_sign_ext;
    s2_mant_ext <= s1_mant_ext;
    s2_leading_zeros <= leading_zeros;
    s2_shift <= shift;
    s2_is_zero <= is_zero;
    s2_is_one <= s1_is_one;
    s2_is_tiny_before_rounding <= is_tiny_before_rounding;
    s2_inexact <= inexact;
    s2_rm <= s1_rm;
  end
end

// Apply shifting and rounding
// The actual shift of the mantissa must take into account the extra fractional bits added in the concatenation
logic [E-1:0] exp_shifted;
logic shifted_out_bit;
logic [(1+M)-1:0] mant_shifted;
logic round;
logic [(M+2)-1:0] mant_rounded;

assign exp_shifted = s2_is_zero ? E'(unsigned'(1)) : ct_exp_ext - E'(s2_shift);

generate if ((SPARE_BITS_E_BOUND > 0) || (SPARE_BITS_N_BOUND > 0))
begin: SPARE_MANT_BITS
  // In any of these cases, the shift to be applied will always be less than the number
  // of extra bits in the mantissa
  assign shifted_out_bit = s2_mant_ext[M_EXTRA_BITS-1-s2_shift];
  assign mant_shifted = (1+M)'(s2_mant_ext >> (M_EXTRA_BITS - s2_shift));
end
else if ((SPARE_BITS_E_BOUND == 0) || (SPARE_BITS_N_BOUND == 0))
begin: ENOUGH_MANT_BITS
  assign shifted_out_bit = (s2_shift < M_EXTRA_BITS) ? s2_mant_ext[M_EXTRA_BITS-1-s2_shift] : 1'b0;
  assign mant_shifted = (1+M)'(s2_mant_ext >> (M_EXTRA_BITS - s2_shift));
end
else
begin: NOT_ENOUGH_MANT_BITS
  assign shifted_out_bit = (s2_shift < M_EXTRA_BITS) ? s2_mant_ext[M_EXTRA_BITS-1-s2_shift] : 1'b0;
  assign mant_shifted = (s2_shift >= M_EXTRA_BITS) ? (1+M)'(s2_mant_ext << (s2_shift - M_EXTRA_BITS)) : (1+M)'(s2_mant_ext >> (M_EXTRA_BITS - s2_shift));
end
endgenerate

assign round = s2_is_one ? 1'b1 : // If it must be exactly one, we must always round
               (s2_rm == RTZ) ?                      1'b0 : // Round towards Zero
               (s2_rm == RDN) ?  s2_sign_ext & s2_inexact : // Round Down (towards -Inf)
               (s2_rm == RUP) ? ~s2_sign_ext & s2_inexact : // Round Up (towards +Inf)
               (s2_rm == RMM) ?           shifted_out_bit : // Round to Nearest, ties to Max Magnitude
               shifted_out_bit & |{mant_shifted[0],s2_inexact}; // Round to Nearest, ties to Even
assign mant_rounded = mant_shifted + (M+1)'(round);

// Detect tininess after rounding
logic is_tiny_after_rounding;
generate if ((SPARE_BITS_E_BOUND >= 0) && E_RNG_SMALL)
begin: TINY_AFTER_ROUNDING
  logic shifted_out_bit_tiny, mant_shifted_tiny, round_tiny;
  // If exponent would be < 0, result is tiny
  // If exponent would be 0, check whether the sum of the truncated mantissa plus the round bit would overflow (i.e. they are all ones)
  // Otherwise, result is tiny
  // Obtain rounding for exp == 0
  if (SPARE_BITS_E_BOUND == 0) assign shifted_out_bit_tiny = 1'b0;
  else                         assign shifted_out_bit_tiny = s2_mant_ext[SPARE_BITS_E_BOUND-1];
  assign mant_shifted_tiny = s2_mant_ext[SPARE_BITS_E_BOUND];
  assign round_tiny = s2_is_one ? 1'b1 : // If it must be exactly one, we must always round
                      (s2_rm == RTZ) ?                      1'b0 : // Round towards Zero
                      (s2_rm == RDN) ?  s2_sign_ext & s2_inexact : // Round Down (towards -Inf)
                      (s2_rm == RUP) ? ~s2_sign_ext & s2_inexact : // Round Up (towards +Inf)
                      (s2_rm == RMM) ?      shifted_out_bit_tiny : // Round to Nearest, ties to Max Magnitude
                      shifted_out_bit_tiny & |{mant_shifted_tiny,s2_inexact}; // Round to Nearest, ties to Even
  assign is_tiny_after_rounding = (ct_exp_ext < s2_leading_zeros) || ((ct_exp_ext == s2_leading_zeros) && (&{s2_mant_ext[(1+M)-1 +SPARE_BITS_E_BOUND -: M+1],round_tiny} == 1'b0));
end
else
begin: TINY_BEFORE_ROUNDING
  // In this case, no rounding would occur were the exponent range unbounded
  // (thus no overflow which undoes tininess can occur)
  assign is_tiny_after_rounding = s2_is_tiny_before_rounding;
end
endgenerate

// Set underflow flag
logic unf;
assign unf = is_tiny_after_rounding & ~s2_is_zero;

// Stage 3
// -------
logic s3_sign_ext;
logic [E-1:0] s3_exp_shifted;
logic [(M+2)-1:0] s3_mant_rounded;
logic s3_is_zero;
logic s3_inexact, s3_unf;
always_ff @(posedge clock)
begin
  if (ready)
  begin
    s3_sign_ext <= s2_sign_ext;
    s3_exp_shifted <= exp_shifted;
    s3_mant_rounded <= mant_rounded;
    s3_is_zero <= s2_is_zero;
    s3_inexact <= s2_inexact;
    s3_unf <= unf;
  end
end

// Apply additional shifting if necessary
logic [E-1:0] exp_norm;
logic [(M+1)-1:0] mant_norm;
always_comb
begin
  // If overflow occurred during rounding, shift right and increment exponent
  if (s3_mant_rounded[(M+2)-1] == 1'b1)
  begin
    exp_norm = s3_exp_shifted + 1'b1;
    mant_norm = s3_mant_rounded[(M+2)-1:1];
  end
  else
  begin
    exp_norm = s3_exp_shifted;
    mant_norm = s3_mant_rounded[(M+2)-2:0];
  end
end

// Discard implicit leading 1
logic out_sign;
logic [E-1:0] out_exp;
logic [M-1:0] out_mant;
// If the resulting exponent is 1 and the mantissa is not normalized,
// then set exponent to 0
assign out_sign = s3_sign_ext;
assign out_exp = ((exp_norm == E'(unsigned'(1))) && (mant_norm[(M+1)-1] == 1'b0)) ? E'(0) : exp_norm;
assign out_mant = mant_norm[M-1:0];

// Assign outputs
assign flags[`FP_FLAG_INVALID] = 1'b0;
assign flags[`FP_FLAG_DIV0] = 1'b0;
generate if (S > 0)
begin: SIGNED_OUT
  assign out = {out_sign,out_exp,out_mant};
  assign flags[`FP_FLAG_OVF] = 1'b0;
  assign flags[`FP_FLAG_UNF] = s3_unf&s3_inexact;
  assign flags[`FP_FLAG_INEXACT] = s3_inexact;
end
else
begin: UNSIGNED_OUT
  always_comb
  begin
    // If the input is negative but output does not have sign, saturate to 0
    if (s3_sign_ext == 1'b1)
    begin
      out = W'(0);
      flags[`FP_FLAG_OVF] = ~s3_is_zero;
      flags[`FP_FLAG_UNF] = s3_unf;
      flags[`FP_FLAG_INEXACT] = ~s3_is_zero;
    end
    else
    begin
      out = {out_exp,out_mant};
      flags[`FP_FLAG_OVF] = 1'b0;
      flags[`FP_FLAG_UNF] = s3_unf&s3_inexact;
      flags[`FP_FLAG_INEXACT] = s3_inexact;
    end
  end
end
endgenerate

// Helper functions
function automatic int cdiv (int n, int d);
  int res;
  int n_tmp;

  res = 0;
  n_tmp = n;
  while (n_tmp > 0)
  begin
    n_tmp = n_tmp - d;
    res = res + 1;
  end

  return res;
endfunction

function automatic int idiv (int n, int d);
  int res;
  int n_tmp;

  res = 0;
  n_tmp = n;
  while (n_tmp >= d)
  begin
    n_tmp = n_tmp - d;
    res = res + 1;
  end

  return res;
endfunction

function automatic int irem (int n, int d);
  int res;
  int n_tmp;

  res = 0;
  n_tmp = n;
  while (n_tmp >= d)
  begin
    n_tmp = n_tmp - d;
    res = res + 1;
  end

  return n_tmp;
endfunction

function automatic int max (int a, int b);
  int res;

  if (a > b)
    res = a;
  else
    res = b;

  return res;
endfunction

endmodule
