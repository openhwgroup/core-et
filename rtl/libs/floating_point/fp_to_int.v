// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_to_int
#(parameter S = 1,
  parameter E = 8,
  parameter M = 23,
  parameter N = 8,
  parameter USE_EXT_FCLASS = 0
)
(in, rm, is_signed, exact, fclass, out, flags);

// LOCAL PARAMETERS
localparam W=S+E+M;
localparam MAX_EXP = E'(((E+1)'(1)<<E) - 1);
localparam EXP_OFFSET = (E'(1)<<(E-1)) - 1;
localparam MAX_OUT_U = N'(((N+1)'(1)<<N) - 1);
localparam MIN_OUT_U = N'(0);
localparam MAX_OUT_S = N'((N'(1)<<(N-1)) - 1);
localparam MIN_OUT_S = N'(N'(1)<<(N-1));

// INPUT/OUTPUT INTERFACE
input  logic [W-1:0] in;
input  fp_rounding_mode rm;
input  logic is_signed;
input  logic exact;
input  logic [`FP_CLASS_BITS-1:0] fclass;
output logic [N-1:0] out;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
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
logic exp_zero, is_NaN, is_Inf;
generate if (USE_EXT_FCLASS)
begin: EXT_CLASS
  assign exp_zero = fclass[`FP_CLASS_NEG_DENORMAL] | fclass[`FP_CLASS_NEG_ZERO] |
                    fclass[`FP_CLASS_POS_DENORMAL] | fclass[`FP_CLASS_POS_ZERO];
  assign is_NaN = fclass[`FP_CLASS_SNAN] | fclass[`FP_CLASS_QNAN];
  assign is_Inf = fclass[`FP_CLASS_NEG_INF] | fclass[`FP_CLASS_POS_INF];
end
else
begin: IN_CLASS
  logic max_exp, mant_zero;
  assign max_exp = in_exp == MAX_EXP;
  assign exp_zero = in_exp == E'(0);
  assign mant_zero = in_mant == M'(0);
  assign is_NaN = max_exp && ~mant_zero;
  assign is_Inf = max_exp && mant_zero;
end
endgenerate

// Extract explicit mantissa
logic [(1+M)-1:0] mant_explicit;
assign mant_explicit = exp_zero ? {1'b0, in_mant} : {1'b1, in_mant};

// Obtain integer and fractional parts
// (apply shifting according to the exponent)
logic [N-1:0] r_int;
logic [(M+2)-1:0] shifted_out_bits;
generate if (N > 1+M)
begin: N_BIG
  assign r_int = (in_exp > EXP_OFFSET+N-1) ? mant_explicit << (N-(1+M)) :
                 (in_exp > EXP_OFFSET+M) ? mant_explicit << (in_exp - (EXP_OFFSET+M)) :
                 (in_exp >= EXP_OFFSET) ? mant_explicit >> ((M+EXP_OFFSET) - in_exp) :
                 N'(0);
  assign shifted_out_bits = (in_exp > EXP_OFFSET+M) ? (M+2)'(0) :
                            (in_exp >= EXP_OFFSET-1) ? {1'b0,mant_explicit} << (in_exp - (EXP_OFFSET-2)) :
                            {1'b0,mant_explicit};
end
else
begin: N_SMALL
  assign r_int = (in_exp > EXP_OFFSET+N-1) ? mant_explicit >> ((1+M)-N) :
                 (in_exp >= EXP_OFFSET) ? mant_explicit >> ((M+EXP_OFFSET) - in_exp) :
                 N'(0);
  assign shifted_out_bits = (in_exp > EXP_OFFSET+N-1) ? {1'b0,mant_explicit} << (N+1) :
                            (in_exp >= EXP_OFFSET-1) ? {1'b0,mant_explicit} << (in_exp - (EXP_OFFSET-2)) :
                            {1'b0,mant_explicit};
end
endgenerate

// Round
logic inexact, round;
logic [(1+N)-1:0] r_rounded;
assign inexact = |shifted_out_bits;
assign round = (rm == RTZ) ?                      1'b0 : // Round towards Zero
               (rm == RDN) ?         in_sign & inexact : // Round Down (towards -Inf)
               (rm == RUP) ?        ~in_sign & inexact : // Round Up (towards +Inf)
               (rm == RMM) ? shifted_out_bits[(M+2)-1] : // Round to Nearest, ties to Max Magnitude
               shifted_out_bits[(M+2)-1] & |{r_int[0],shifted_out_bits[(M+2)-2:0]}; // Round to Nearest, ties to Even
assign r_rounded = r_int + N'(round);

// Obtain positive and negative overflow conditions
// and adjust result's sign
logic p_ovf, n_ovf;
logic [N-1:0] r_ext;
assign p_ovf = (in_sign == 1'b0) && (is_signed ? (in_exp > EXP_OFFSET+N-2) || (|r_rounded[(1+N)-1 -: 2] == 1'b1) :
                                                 (in_exp > EXP_OFFSET+N-1) || (r_rounded[(1+N)-1] == 1'b1));
                                    // If result is signed, a negative number overflows when its absolute value
                                    // is greater than maximum absolute value
assign n_ovf = (in_sign == 1'b1) && (is_signed ? (in_exp > EXP_OFFSET+N-1) || (r_rounded > MAX_OUT_S+1) :
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
