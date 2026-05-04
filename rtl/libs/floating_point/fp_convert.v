// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_convert
#(parameter S_IN = 1,
  parameter E_IN = 5,
  parameter M_IN = 10,
  parameter S_OUT = 1,
  parameter E_OUT = 8,
  parameter M_OUT = 23,
  parameter USE_EXT_FCLASS = 0
)
(in, rm, bypass_nan, fclass, out, flags);

// LOCAL PARAMETERS
localparam W_IN = S_IN+E_IN+M_IN;
localparam MAX_EXP_IN = unsigned'((1<<E_IN) - 1);
localparam EXP_OFFSET_IN = unsigned'((1<<(E_IN-1))-1);
localparam W_OUT = S_OUT+E_OUT+M_OUT;
localparam MAX_EXP_OUT = unsigned'((1<<E_OUT) - 1);
localparam EXP_OFFSET_OUT = unsigned'((1<<(E_OUT-1))-1);
localparam MAX_MANT_OUT = unsigned'((1<<M_OUT) - 1);
localparam QUIET_BIT = M_OUT'(unsigned'(1<<(M_OUT-1)));

// INPUT/OUTPUT INTERFACE
input  logic [W_IN-1:0] in;
input  fp_rounding_mode rm;
input  logic bypass_nan;
input  logic [`FP_CLASS_BITS-1:0] fclass;
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

logic [E_IN-1:0] in_exp;
logic [M_IN-1:0] in_mant;
assign in_exp  = in[M_IN +: E_IN];
assign in_mant = in[0 +: M_IN];

// Make some basic checkings
logic in_max_exp, in_is_NaN, in_is_sNaN, in_is_Inf, in_is_zero;
generate if (USE_EXT_FCLASS)
begin: EXT_FCLASS
  assign in_is_NaN = fclass[`FP_CLASS_SNAN] | fclass[`FP_CLASS_QNAN];
  assign in_is_sNaN = fclass[`FP_CLASS_SNAN];
  assign in_is_Inf = fclass[`FP_CLASS_NEG_INF] | fclass[`FP_CLASS_POS_INF];
  assign in_max_exp = in_is_NaN | in_is_Inf;
  assign in_is_zero = fclass[`FP_CLASS_NEG_ZERO] | fclass[`FP_CLASS_POS_ZERO];
end
else
begin: IN_FCLASS
  logic in_mant_zero;
  assign in_max_exp = in_exp == E_IN'(MAX_EXP_IN);
  assign in_mant_zero = in_mant == M_IN'(0);
  assign in_is_NaN = in_max_exp & ~in_mant_zero;
  assign in_is_sNaN = in_is_NaN & (in_mant[M_IN-1] == 1'b0);
  assign in_is_Inf = in_max_exp & in_mant_zero;
  assign in_is_zero = (in_exp == E_IN'(0)) & in_mant_zero;
end
endgenerate

// Generate output exponent and mantissa
logic out_sign;
logic [E_OUT-1:0] out_exp;
logic [M_OUT-1:0] out_mant;
logic invalid, ovf, unf, inexact;

// Assign invalid flag
assign invalid = in_is_sNaN;

// E_OUT > E_IN
// ------------
generate if (E_OUT > E_IN)
begin: E_OUT_WIDER
  // It is assummed that M_OUT >= M_IN
  localparam M_DIFF = unsigned'(M_OUT - M_IN);
  localparam EXP_OFFSET_DIFF = EXP_OFFSET_OUT - EXP_OFFSET_IN;

  // Adjust exponent to new offset
  logic [E_OUT-1:0] exp_adjusted;
  assign exp_adjusted = E_OUT'(in_exp) + E_OUT'(EXP_OFFSET_DIFF);

  // Obtain the number of leading zeros of the mantissa, in case exponent is zero
  // and can be decremented on the output
  localparam LZ_BITS = $clog2(1+M_IN);
  logic [LZ_BITS-1:0] leading_zeros, shift;
  logic is_zero;

  leading_zero_counter
  #(
      .N_BITS         ( 1+M_IN         )
  )
  lzc
  (
       .n             ( {1'b0,in_mant} ),
       .leading_zeros ( leading_zeros  ),
       .is_zero       ( is_zero        )
  );

  // Obtain the actual shift to be applied (maximum shift will be EXP_OFFSET_DIFF)
  assign shift = (EXP_OFFSET_DIFF >= E_OUT'(leading_zeros)) ? leading_zeros : LZ_BITS'(EXP_OFFSET_DIFF);

  // In that case, shift mantissa and adjust exponent
  logic [(1+M_OUT)-1:0] mant_denormal_shifted;
  logic [E_OUT-1:0] exp_denormal_shifted;
  assign mant_denormal_shifted = {1'b0,in_mant,{M_DIFF{1'b0}}} << shift;
  assign exp_denormal_shifted = is_zero ? E_OUT'(unsigned'(1)) : E_OUT'(EXP_OFFSET_DIFF+1) - E_OUT'(shift);

  // Assign output values
  logic [M_OUT-1:0] mant_shifted;
  assign mant_shifted = in_mant << M_DIFF;

  always_comb
  begin
    unf = 1'b0;
    if (in_max_exp)
    begin
      out_sign = (in_is_NaN && ~bypass_nan) ? 1'b0 : in_sign;
      out_exp = E_OUT'(MAX_EXP_OUT);
      // NOTE: RISCV does not seem to propagate NaNs
      out_mant = (in_is_NaN && ~bypass_nan) ? QUIET_BIT : mant_shifted;
      //out_mant = (in_is_NaN && ~bypass_nan) ? mant_shifted | QUIET_BIT : mant_shifted; // Set quiet bit in case of sNaN
    end
    else if (in_exp == E_IN'(0))
    begin
      out_sign = in_sign;
      // If the resulting exponent is 1 and the mantissa is not normalized,
      // then set exponent to 0
      if ((exp_denormal_shifted == E_OUT'(unsigned'(1))) && (mant_denormal_shifted[(1+M_OUT)-1] == 1'b0))
      begin
        out_exp = E_OUT'(0);
        unf = mant_denormal_shifted[M_OUT-1:0] != 0;
      end
      else
        out_exp = exp_denormal_shifted;
      out_mant = mant_denormal_shifted[M_OUT-1:0];
    end
    else
    begin
      out_sign = in_sign;
      out_exp = exp_adjusted;
      out_mant = mant_shifted;
    end
  end

  // Assign flags
  assign ovf = 1'b0;
  assign inexact = 1'b0;
end
// E_OUT == E_IN, M_OUT >= M_IN
// ----------------------------
else if ((E_OUT == E_IN) && (M_OUT >= M_IN))
begin: W_OUT_WIDER
  localparam M_DIFF = unsigned'(M_OUT - M_IN);

  // Assign output values
  assign out_sign = (in_is_NaN && ~bypass_nan) ? 1'b0 : in_sign;
  // Directly assign input exponent into output
  assign out_exp = in_exp;
  // Shift mantissa appropriately
  logic [M_OUT-1:0] mant_shifted;
  assign mant_shifted = (in_mant << M_DIFF);
  // NOTE: RISCV does not seem to propagate NaNs
  assign out_mant = (in_is_NaN && ~bypass_nan) ? QUIET_BIT : mant_shifted;
  //assign out_mant = (in_is_NaN && ~bypass_nan) ? mant_shifted | QUIET_BIT : mant_shifted; // Set quiet bit in case of sNaN

  assign ovf = 1'b0;
  assign unf = (out_exp == E_OUT'(0)) && (out_mant != M_OUT'(0));
  assign inexact = 1'b0;
end
// E_OUT <= E_IN, M_OUT <= M_IN
// ----------------------------
else
begin: W_OUT_NARROWER
  // It is assummed that M_IN >= M_OUT
  localparam M_DIFF = M_IN - M_OUT;
  localparam EXP_OFFSET_DIFF = E_IN'(unsigned'(EXP_OFFSET_IN - EXP_OFFSET_OUT));

  // Detect tininess before rounding
  logic is_tiny_before_rounding;
  assign is_tiny_before_rounding = in_exp <= EXP_OFFSET_DIFF;

  // Adjust exponent to new offset
  logic [E_IN-1:0] exp_adjusted;
  assign exp_adjusted = is_tiny_before_rounding ? E_IN'(unsigned'(1)) : in_exp - EXP_OFFSET_DIFF;

  // Add explicit leading 1
  // In case of denormal numbers, readjust exponent and mantissa
  logic [E_IN-1:0] exp_explicit;
  logic [(1+M_IN)-1:0] mant_explicit;
  assign exp_explicit = (in_exp == E_IN'(0)) ? E_IN'(unsigned'(1)) : in_exp;
  assign mant_explicit = (in_exp == E_IN'(0)) ? {1'b0, in_mant} : {1'b1, in_mant};

  // Apply shifting and rounding
  localparam SHIFT_BITS = $clog2(M_IN+3);
  logic [SHIFT_BITS-1:0] shift;
  assign shift = (signed'({1'b0,exp_explicit}) <= EXP_OFFSET_DIFF-(M_OUT+1)) ? SHIFT_BITS'(unsigned'(M_IN+2)) :
                 (exp_explicit <= EXP_OFFSET_DIFF) ? SHIFT_BITS'(M_DIFF+EXP_OFFSET_DIFF+1)-SHIFT_BITS'(exp_explicit) :
                                                     SHIFT_BITS'(unsigned'(M_DIFF));

  logic [(M_IN+2)-1:0] shifted_out_bits;
  logic [(M_OUT+1)-1:0] mant_shifted;
  logic round;
  logic [(M_OUT+2)-1:0] mant_rounded;
  assign shifted_out_bits = (shift < M_IN+2) ? {1'b0,mant_explicit}<<((M_IN+2)-shift) : {1'b0,mant_explicit};
  assign inexact = |shifted_out_bits;
  assign mant_shifted = (shift < M_IN+1) ? (M_OUT+1)'(mant_explicit >> shift) : (M_OUT+1)'(0);
  assign round = (rm == RTZ) ?                         1'b0 : // Round towards Zero
                 (rm == RDN) ?            in_sign & inexact : // Round Down (towards -Inf)
                 (rm == RUP) ?           ~in_sign & inexact : // Round Up (towards +Inf)
                 (rm == RMM) ? shifted_out_bits[(M_IN+2)-1] : // Round to Nearest, ties to Max Magnitude
                 shifted_out_bits[(M_IN+2)-1] & |{mant_shifted[0],shifted_out_bits[(M_IN+2)-2:0]}; // Round to Nearest, ties to Even
  assign mant_rounded = mant_shifted + (M_OUT+1)'(round);

  // Detect tininess after rounding
  logic is_tiny_after_rounding;
  // To detect tininess after rounding, check that a non-zero result computed as though the exponent range were unbounded
  // would lie strictly between +/-2^emin
  if ((M_OUT == M_IN) || ((E_OUT == E_IN) && (M_OUT == M_IN-1)))
  begin: TINY_BEFORE_ROUNDING
    // In these cases, no rounding would occur were the exponent range unbounded
    // (thus no overflow which undoes tininess can occur)
    assign is_tiny_after_rounding = is_tiny_before_rounding;
  end
  else
  begin: TINY_AFTER_ROUNDING
    localparam TRUNC_BITS_TINY = (E_OUT == E_IN) ? M_DIFF-1 : M_DIFF;

    logic [(TRUNC_BITS_TINY+1)-1:0] shifted_out_bits_tiny;
    logic inexact_tiny, round_tiny;
    // If exponent were < 0, result is tiny
    // If exponent were 0, check whether the sum of the truncated manitssa plus the round bit would overflow (i.e. they are all ones)
    // Otherwise, result is tiny
    // Obtain rounding for exp == 0
    assign shifted_out_bits_tiny = {mant_explicit[TRUNC_BITS_TINY-1:0],1'b0};
    assign inexact_tiny = |mant_explicit[TRUNC_BITS_TINY-1:0];
    assign round_tiny = (rm == RTZ) ?                                         1'b0 : // Round towards Zero
                        (rm == RDN) ?                       in_sign & inexact_tiny : // Round Down (towards -Inf)
                        (rm == RUP) ?                      ~in_sign & inexact_tiny : // Round Up (towards +Inf)
                        (rm == RMM) ? shifted_out_bits_tiny[(TRUNC_BITS_TINY+1)-1] : // Round to Nearest, ties to Max Magnitude
                        shifted_out_bits_tiny[(TRUNC_BITS_TINY+1)-1] & |{mant_explicit[TRUNC_BITS_TINY],shifted_out_bits_tiny[(TRUNC_BITS_TINY+1)-2:0]}; // Round to Nearest, ties to Even
    assign is_tiny_after_rounding = (in_exp < EXP_OFFSET_DIFF) || ((in_exp == EXP_OFFSET_DIFF) && (&{mant_explicit[(M_IN+1)-1 -: M_OUT+1],round_tiny} == 1'b0));
  end

  // Set underflow flag
  assign unf = is_tiny_after_rounding & ~in_is_zero;

  // Apply additional shifting if necessary
  logic [(E_IN+1)-1:0] exp_norm;
  logic [(M_OUT+1)-1:0] mant_norm;
  always_comb
  begin
    // If overflow occurred during rounding, shift right and increment exponent
    if (mant_rounded[(M_OUT+2)-1] == 1'b1)
    begin
      exp_norm = exp_adjusted + 1'b1;
      mant_norm = mant_rounded[(M_OUT+2)-1:1];
    end
    else
    begin
      exp_norm = {1'b0,exp_adjusted};
      mant_norm = mant_rounded[(M_OUT+2)-2:0];
    end
  end

  // Discard implicit leading 1
  logic [E_OUT-1:0] r_exp;
  logic [M_OUT-1:0] r_mant;
  logic ovf_round;
  always_comb
  begin
    ovf_round = 1'b0;
    // If the resulting exponent is greater than or equal to the maximum value,
    // then set result according to sign and rounding mode
    if (exp_norm >= (E_IN+1)'(MAX_EXP_OUT))
    begin
      case (rm)
        RTZ:
        begin
          // Set result to the format's largest finite number with the sign of the intermediate result
          r_exp = E_OUT'(MAX_EXP_OUT-1);
          r_mant = M_OUT'(MAX_MANT_OUT);
        end

        RDN:
        begin
          // Set result to the format's largest finite number if it is positive
          // or -Inf if it is negative
          r_exp = (in_sign == 1'b0) ? E_OUT'(MAX_EXP_OUT-1) : E_OUT'(MAX_EXP_OUT);
          r_mant = (in_sign == 1'b0) ? M_OUT'(MAX_MANT_OUT) : M_OUT'(0);
        end

        RUP:
        begin
          // Set result to +Inf if it is positive
          // or to the format's most negative finite number if it is negative
          r_exp = (in_sign == 1'b0) ? E_OUT'(MAX_EXP_OUT) : E_OUT'(MAX_EXP_OUT-1);
          r_mant = (in_sign == 1'b0) ? M_OUT'(0) : M_OUT'(MAX_MANT_OUT);
        end

        default:
        begin
          // Set result to Inf with the sign of the intermediate result
          r_exp = E_OUT'(MAX_EXP_OUT);
          r_mant = M_OUT'(0);
        end
      endcase
      ovf_round = 1'b1;
    end
    // Else if the resulting exponent is 1 and the mantissa is not normalized,
    // then set exponent to 0
    else if ((exp_norm == (E_IN+1)'(unsigned'(1))) && (mant_norm[(M_OUT+1)-1] == 1'b0))
    begin
      r_exp = E_OUT'(0);
      r_mant = mant_norm[M_OUT-1:0];
    end
    else
    begin
      r_exp = exp_norm[E_OUT-1:0];
      r_mant = mant_norm[M_OUT-1:0];
    end

  end

  // Assign output values
  always_comb
  begin
    if (in_max_exp)
    begin
      out_sign = (in_is_NaN && ~bypass_nan) ? 1'b0 : in_sign;
      out_exp = E_OUT'(MAX_EXP_OUT);
      // NOTE: RISCV does not seem to propagate NaNs
      out_mant = (in_is_NaN && ~bypass_nan) ? QUIET_BIT : in_mant[M_IN-1 -: M_OUT];
      //out_mant = (in_is_NaN && ~bypass_nan) ? in_mant[M_IN-1 -: M_OUT] | QUIET_BIT : in_mant[M_IN-1 -: M_OUT]; // Set quiet bit in case of sNaN
      ovf = 1'b0;
    end
    else
    begin
      out_sign = in_sign;
      out_exp = r_exp;
      out_mant = r_mant;
      ovf = ovf_round;
    end
  end
end
endgenerate

// Assign outputs
generate if (S_OUT > 0)
begin: SIGNED_OUT
  assign out = {out_sign,out_exp,out_mant};
  assign flags[`FP_FLAG_INVALID] = invalid;
  assign flags[`FP_FLAG_DIV0] = 1'b0;
  assign flags[`FP_FLAG_OVF] = (in_is_NaN || in_is_Inf) ? 1'b0 : ovf;
  assign flags[`FP_FLAG_UNF] = (in_is_NaN || in_is_Inf) ? 1'b0 : unf&inexact;
  assign flags[`FP_FLAG_INEXACT] = (in_is_NaN || in_is_Inf) ? 1'b0 : ovf|inexact;
end
else
begin: UNSIGNED_OUT
  always_comb
  begin
    flags = `FP_NUM_FLAGS'b0;

    // If the input is negative but output does not have sign, saturate to 0
    if ((in_sign == 1'b1) && !in_is_NaN)
    begin
      out = W_OUT'(0);
      flags[`FP_FLAG_INVALID] = in_is_Inf;
      flags[`FP_FLAG_OVF] = ~in_is_zero;
      flags[`FP_FLAG_UNF] = unf;
      flags[`FP_FLAG_INEXACT] = ~in_is_zero;
    end
    else
    begin
      out = {out_exp,out_mant};
      flags[`FP_FLAG_INVALID] = invalid;
      flags[`FP_FLAG_OVF] = (in_is_NaN || in_is_Inf) ? 1'b0 : ovf;
      flags[`FP_FLAG_UNF] = (in_is_NaN || in_is_Inf) ? 1'b0 : unf&inexact;
      flags[`FP_FLAG_INEXACT] = (in_is_NaN || in_is_Inf) ? 1'b0 : ovf|inexact;
    end
  end
end
endgenerate

endmodule
