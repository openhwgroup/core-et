// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_to_norm
#(parameter S = 1,
  parameter E = 8,
  parameter M = 23,
  parameter N = 8
)
(in, rm, is_signed, exact, out, flags);

// LOCAL PARAMETERS
localparam W = S+E+M;
localparam MAX_EXP = E'(((E+1)'(1)<<E) - 1);
localparam EXP_OFFSET = (E'(1)<<(E-1)) - 1;
localparam MAX_OUT_U = N'(((N+1)'(1)<<N) - 1);
localparam MIN_OUT_U = N'(0);
localparam MAX_OUT_S = N'((N'(1)<<(N-1)) - 1);
localparam MIN_OUT_S = N'((N'(1)<<(N-1)) + 1);

// INPUT/OUTPUT INTERFACE
input  logic [W-1:0] in;
input  fp_rounding_mode rm;
input  logic is_signed;
input  logic exact;
output logic [N-1:0] out;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
// Extract floating point components
logic in_sign;
generate if (S > 0)
begin: SIGNED_IN
  assign in_sign = in[W-1];
end
else
begin: UNSIGNED_IN
  assign in_sign = 1'b0;
end
endgenerate

logic [E-1:0] in_exp;
logic [M-1:0] in_mant;
assign in_exp  = in[M +: E];
assign in_mant = in[0 +: M];

// Make some basic checkings
logic max_exp, exp_zero, mant_zero, is_NaN, is_Inf;
assign max_exp = in_exp == MAX_EXP;
assign exp_zero = in_exp == E'(0);
assign mant_zero = in_mant == M'(0);
assign is_NaN = max_exp && ~mant_zero;
assign is_Inf = max_exp && mant_zero;

// Extract explicit exponent and mantissa
logic [E-1:0] exp_explicit;
logic [(1+M)-1:0] mant_explicit;
assign exp_explicit = exp_zero ? E'(1) : in_exp;
assign mant_explicit = exp_zero ? {1'b0, in_mant} : {1'b1, in_mant};

// The following identity is used: n = f*(2^N_ABS - 1) = f<<N_ABS - f
// where N_ABS = is_signed ? N-1 : N
// Align mantissa
localparam ALIGN_BITS = (N > (1+M)-4) ? N+4 : (1+M)+1;
logic sticky_bit;
logic [ALIGN_BITS-1:0] g_mant_aligned, l_mant_aligned;
generate if (N > (1+M)-4)
begin: N_BIG
  // Three bits are left to allocate GRS (Guard, Round and Sticky) bits for rounding
  assign g_mant_aligned = {mant_explicit, (ALIGN_BITS-(1+M))'(0)};
  // When shifting right mantissa, Sticky bit is the OR of all the shifted out bits
  assign sticky_bit = is_signed ? |mant_explicit[(1+M)-5:0] : |mant_explicit[(1+M)-4:0];
  assign l_mant_aligned = is_signed ? {(N-1)'(0), mant_explicit[(1+M)-1 -: 4], sticky_bit} :
                                      {N'(0), mant_explicit[(1+M)-1 -: 3], sticky_bit};
end
else
begin: N_SMALL
  // If N is small compared to M, all the bits of the mantissa plus
  // a Sticky bit are kept in g_mant_aligned
  assign g_mant_aligned = {mant_explicit, 1'b0};
  // When shifting right mantissa, Sticky bit is the OR of all the shifted out bits
  assign sticky_bit = is_signed ? |mant_explicit[N-2:0] : |mant_explicit[N-1:0];
  assign l_mant_aligned = is_signed ? {(N-1)'(0), mant_explicit[(1+M)-1 -: 1+M-(N-1)], sticky_bit} :
                                      {N'(0), mant_explicit[(1+M)-1 -: 1+M-N], sticky_bit};
end
endgenerate

// Subtract mantissas
logic [ALIGN_BITS-1:0] r_mant;
assign r_mant = g_mant_aligned - l_mant_aligned;

// Select appropriate bits of the mantissa
// (apply shifting according to the exponent)
logic [(1+N)-1:0] r_norm;
logic [(ALIGN_BITS+1)-1:0] shifted_out_bits;
always_comb
begin
  if (exp_explicit >= EXP_OFFSET)
  begin
    r_norm = is_signed ? {1'b0,r_mant[ALIGN_BITS-1 -: 1+(N-1)]} :
                         r_mant[ALIGN_BITS-1 -: 1+N];
    shifted_out_bits = is_signed ? {r_mant[ALIGN_BITS-1-(1+(N-1)):0],(1+(N-1)+1)'(0)} :
                                   {r_mant[ALIGN_BITS-1-(1+N):0],(1+N+1)'(0)};
  end
  else if (exp_explicit >= EXP_OFFSET-(1+N))
  begin
    r_norm = is_signed ? {1'b0,r_mant[ALIGN_BITS-1 -: 1+(N-1)]} >> (EXP_OFFSET-exp_explicit) :
                         r_mant[ALIGN_BITS-1 -: 1+N] >> (EXP_OFFSET-exp_explicit);
    shifted_out_bits = is_signed ? {1'b0,r_mant} << ((1+(N-1))- (EXP_OFFSET-exp_explicit) + 1) :
                                   {1'b0,r_mant} << ((1+N)- (EXP_OFFSET-exp_explicit) + 1);
  end
  else
  begin
    r_norm = (1+N)'(0);
    shifted_out_bits = (ALIGN_BITS+1)'(r_mant);
  end
end

// Round
logic inexact, round;
logic [(1+N)-1:0] r_rounded;
assign inexact = |shifted_out_bits;
assign round = (rm == RTZ) ?                               1'b0 : // Round towards Zero
               (rm == RDN) ?                  in_sign & inexact : // Round Down (towards -Inf)
               (rm == RUP) ?                 ~in_sign & inexact : // Round Up (towards +Inf)
               (rm == RMM) ? shifted_out_bits[(ALIGN_BITS+1)-1] : // Round to Nearest, ties to Max Magnitude
               shifted_out_bits[(ALIGN_BITS+1)-1] & |{r_norm[0],shifted_out_bits[(ALIGN_BITS+1)-2:0]}; // Round to Nearest, ties to Even
assign r_rounded = r_norm + (1+N)'(round);

// Obtain positive and negative overflow conditions
// and adjust result's sign
logic p_ovf, n_ovf;
logic [N-1:0] r_ext;
assign p_ovf = (in_sign == 1'b0) && (is_signed ? (exp_explicit > EXP_OFFSET) || (|r_rounded[(1+N)-1 -: 2] == 1'b1) :
                                                 (exp_explicit > EXP_OFFSET) || (r_rounded[(1+N)-1] == 1'b1));
assign n_ovf = (in_sign == 1'b1) && (is_signed ? (exp_explicit > EXP_OFFSET) || (|r_rounded[(1+N)-1 -: 2] == 1'b1) :
                                    // If result is unsigned, a negative number overflows whenever result is not zero
                                                 (r_rounded != (1+N)'(0)));
assign r_ext = (is_signed && in_sign) ? -r_rounded[N-1:0] : r_rounded[N-1:0];

// Assign outputs
logic [N-1:0] max_out, min_out;
assign max_out = is_signed ? MAX_OUT_S : MAX_OUT_U;
assign min_out = is_signed ? MIN_OUT_S : MIN_OUT_U;
always_comb
begin
  flags = `FP_NUM_FLAGS'b0;

  if (is_NaN)
  begin
    out = max_out;
    flags[`FP_FLAG_INVALID] = 1'b1;
  end
  else if (is_Inf)
  begin
    out = (in_sign == 1'b0) ? max_out : min_out;
    flags[`FP_FLAG_INVALID] = 1'b1;
  end
  // Positive out of bound
  else if (p_ovf)
  begin
    out = max_out;
    flags[`FP_FLAG_INVALID] = 1'b1;
  end
  // Negative out of bound
  else if (n_ovf)
  begin
    out = min_out;
    flags[`FP_FLAG_INVALID] = 1'b1;
  end
  else
  begin
    out = r_ext;
    flags[`FP_FLAG_INEXACT] = inexact & exact;
  end
end

endmodule
