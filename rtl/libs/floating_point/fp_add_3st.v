// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_add_3st #(parameter E = 8, parameter M = 23)
(clock, reset, a, b, op, rm, ready, c, flags);

// LOCAL PARAMETERS
localparam W = 1+E+M;
localparam MAX_EXP = (1<<E) - 1;
localparam EXP_OFFSET = (1<<(E-1)) - 1;
localparam MAX_MANT = (1<<M) - 1;
localparam qNaN = {1'b0,{(E+1){1'b1}},(M-1)'(0)};
localparam QUIET_BIT = W'(1<<(M-1));

// INPUT/OUTPUT INTERFACE
input logic clock, reset;
input logic [W-1:0] a, b;
input fp_add_op op;
input fp_rounding_mode rm;
input ready;
output logic [W-1:0] c;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
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

// STAGE 1: Substract exponents and align mantissas
// Make some basic checkings
logic a_max_exp, a_mant_zero, a_is_NaN, a_is_sNaN, a_is_Inf;
logic b_max_exp, b_mant_zero, b_is_NaN, b_is_sNaN, b_is_Inf;
logic invalid;
assign a_max_exp = a_exp == E'(MAX_EXP);
assign b_max_exp = b_exp == E'(MAX_EXP);
assign a_mant_zero = a_mant == M'(0);
assign b_mant_zero = b_mant == M'(0);
assign a_is_sNaN = a_max_exp & ~a_mant_zero & (a_mant[M-1] == 1'b0);
assign b_is_sNaN = b_max_exp & ~b_mant_zero & (b_mant[M-1] == 1'b0);

always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    a_is_NaN <= 1'b0;
    b_is_NaN <= 1'b0;
    a_is_Inf <= 1'b0;
    b_is_Inf <= 1'b0;
    invalid <= 1'b0;
  end
  else if (ready == 1'b1)
  begin
    a_is_NaN <= a_max_exp & ~a_mant_zero;
    b_is_NaN <= b_max_exp & ~b_mant_zero;
    a_is_Inf <= a_max_exp & a_mant_zero;
    b_is_Inf <= b_max_exp & b_mant_zero;
    invalid <= a_is_sNaN | b_is_sNaN;
  end
end

// In case of subnormal numbers, readjust exponent and mantissa
logic [E-1:0] a_exp_explicit, b_exp_explicit;
assign a_exp_explicit = (a_exp == E'(0)) ? E'(1) : a_exp;
assign b_exp_explicit = (b_exp == E'(0)) ? E'(1) : b_exp;

// Extract max exponent
logic signed [(E+1)-1:0] exp_diff;
logic exp_diff_sign, a_is_greater;
logic [E-1:0] abs_exp_diff;
assign exp_diff = signed'({1'b0,a_exp_explicit}) - signed'({1'b0,b_exp_explicit});
assign exp_diff_sign = exp_diff[E];
assign a_is_greater = exp_diff_sign == 1'b0;
assign abs_exp_diff = a_is_greater ? exp_diff[E-1:0] : -exp_diff[E-1:0];

// Add explicit leading 1
logic [(M+1)-1:0] a_mant_explicit, b_mant_explicit;
assign a_mant_explicit = (a_exp == E'(0)) ? {1'b0, a_mant} : {1'b1, a_mant};
assign b_mant_explicit = (b_exp == E'(0)) ? {1'b0, b_mant} : {1'b1, b_mant};

// Select greater and lower number mantissas
logic [(M+1)-1:0] g_mant, l_mant;
assign g_mant = a_is_greater ? a_mant_explicit : b_mant_explicit;
assign l_mant = a_is_greater ? b_mant_explicit : a_mant_explicit;

// Align mantissas
// When shifting right l_mant, Sticky bit is the OR of all the shifted out bits
// Thus, pre-store shifted out bits
logic [(M+2)-1:0] shifted_out_bits;
always_comb
begin
  if (abs_exp_diff >= (M+2)+2)
    shifted_out_bits = l_mant;
  else if (abs_exp_diff >= 3)
    shifted_out_bits = {1'b0,l_mant}<<(M+2-abs_exp_diff+2);
  else
    shifted_out_bits = (M+2)'(0);
end

// Obtain effective sign of the second operand
// (it is inverted if operation is subtraction)
logic b_sign_eff;
assign b_sign_eff = b_sign^(op==SUB);

// Register result of first stage
logic [W-1:0] a_eff, b_eff;
logic [(M+4)-1:0] g_mant_aligned, l_mant_aligned;
logic [E-1:0] g_exp;
logic g_sign, eop;
always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    a_eff <= W'(0);
    b_eff <= W'(0);
    g_mant_aligned <= (M+4)'(0);
    l_mant_aligned <= (M+4)'(0);
    g_exp <= E'(0);
    g_sign <= 1'b0;
    eop <= 1'b0;
  end
  else if (ready == 1'b1)
  begin
    a_eff <= a;
    b_eff <= {b_sign_eff,b_exp,b_mant};
    // Align mantissas
    // Three extra bits are added to allocate GRS (Guard, Round and Sticky) bits for rounding
    g_mant_aligned <= {g_mant, 3'b0};
    // When shifting right, Sticky bit is the OR of all the shifted out bits
    l_mant_aligned <= (abs_exp_diff >= 3) ? {2'b0, l_mant>>(abs_exp_diff-2), |shifted_out_bits} : {3'b0,l_mant}<<(3-abs_exp_diff);
    // Select greater number exponent
    g_exp <= a_is_greater ? a_exp_explicit : b_exp_explicit;
    // Select greater number sign
    // The operand with greater exponent will be the first operand.
    // Thus, if operation is subtraction and the operands are swapped, the resulting sign must be inverted
    g_sign <= a_is_greater ? a_sign : b_sign_eff;
    // Obtain effective operation
    eop <= ((op == ADD) && (a_sign == b_sign)) || ((op == SUB) && (a_sign != b_sign)) ? ADD : SUB;
  end
end

// STAGE 2: Add/subtract operands
// Propagate basic checkings
logic a_is_NaN_r2, a_is_Inf_r2;
logic b_is_NaN_r2, b_is_Inf_r2;
logic eop_r2, invalid_r2;
always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    a_is_NaN_r2 <= 1'b0;
    b_is_NaN_r2 <= 1'b0;
    a_is_Inf_r2 <= 1'b0;
    b_is_Inf_r2 <= 1'b0;
    eop_r2 <= 1'b0;
    invalid_r2 <= 1'b0;
  end
  else if (ready == 1'b1)
  begin
    a_is_NaN_r2 <= a_is_NaN;
    b_is_NaN_r2 <= b_is_NaN;
    a_is_Inf_r2 <= a_is_Inf;
    b_is_Inf_r2 <= b_is_Inf;
    eop_r2 <= eop;
    invalid_r2 <= invalid;
  end
end

// Add/subtract mantissas
logic signed [(M+6)-1:0] r_mant_ext_signed;
logic r_mant_ext_sign;
assign r_mant_ext_signed = (eop == ADD) ? signed'({1'b0,g_mant_aligned}) + signed'({1'b0,l_mant_aligned}) :
                                          signed'({1'b0,g_mant_aligned}) - signed'({1'b0,l_mant_aligned});
assign r_mant_ext_sign = r_mant_ext_signed[(M+6)-1];

// Register result of the second stage
logic [W-1:0] a_eff_r2, b_eff_r2;
logic [(M+5)-1:0] r_mant_ext;
logic [E-1:0] r_exp_ext;
logic r_sign;
always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    a_eff_r2 <= W'(0);
    b_eff_r2 <= W'(0);
    r_mant_ext <= (M+5)'(0);
    r_exp_ext <= E'(0);
    r_sign <= 1'b0;
  end
  else if (ready == 1'b1)
  begin
    a_eff_r2 <= a_eff;
    b_eff_r2 <= b_eff;
    // If the result is negative, invert it and update resulting sign
    r_mant_ext <= (r_mant_ext_sign == 1'b0) ? r_mant_ext_signed[(M+6)-2:0] : -r_mant_ext_signed[(M+6)-2:0];
    r_exp_ext <= g_exp;
    // When the sum of two operands with opposite signs (or the difference of two operands with like signs) is
    // exactly zero, the sign of that sum (or difference) shall be +0 in all rounding modes except
    // roundTowardNegative; under that attribute, the sign of an exact zero sum (or difference) shall be -0
    r_sign <= (a_eff=={~b_eff[W-1],b_eff[W-2:0]}) ? (rm == RDN) : g_sign^r_mant_ext_sign;
  end
end

// STAGE 3: Normalize (shift and round) result and handle special cases
// Obtain the number of leading zeros
// - MSB is not checked, as it is an overflow bit
// - Round and Sticky bits are not checked either, as they will be zero if the previous ones are all zero
localparam LZ_BITS = $clog2(M+2);
logic [LZ_BITS-1:0] leading_zeros, shift;
logic is_zero, is_tiny_before_rounding;
leading_zero_counter #(.N_BITS(M+2)) lzc (.n(r_mant_ext[(M+5)-2:2]), .*);
// Detect tininess before rounding
assign is_tiny_before_rounding = (r_mant_ext[(M+5)-1] == 1'b0) && (r_exp_ext <= E'(leading_zeros));
// Obtain the actual shift to be applied (maximum shift will be r_exp_ext-1)
assign shift = ~is_tiny_before_rounding ? leading_zeros : r_exp_ext[LZ_BITS-1:0]-1;

// Shift mantissa
// Two shifters are implemented in parallel in order to reduce latency:
// - The first shifter only shifts one bit to either the left or the right
//   This will be later fed to the rounding stage
logic [(M+4)-1:0] r_mant_l1r1shifted;
logic [E-1:0] r_exp_l1r1shifted;
always_comb
begin
  // If overflow occurred during addition, shift right and increment exponent
  if (r_mant_ext[(M+5)-1] == 1'b1)
  begin
    // Update Sticky bit with additional shifted out bit
    r_mant_l1r1shifted = {r_mant_ext[(M+5)-1:2],|r_mant_ext[1:0]};
    r_exp_l1r1shifted = r_exp_ext + 1;
  end
  // Else if there are no leading zeros (or there are but exponent cannot be
  // decremented anymore), just discard overflow bit
  else if ((r_mant_ext[(M+5)-2] == 1'b1) || (r_exp_ext == E'(1)))
  begin
    r_mant_l1r1shifted = r_mant_ext[(M+5)-2:0];
    r_exp_l1r1shifted = r_exp_ext;
  end
  // Otherwise, shift left and decrement exponent to remove leading zero
  else
  begin
    r_mant_l1r1shifted = {r_mant_ext[(M+5)-3:0],1'b0};
    r_exp_l1r1shifted = r_exp_ext - 1;
  end
end
// - The second shifter shifts two or more bits to the left
//   The result of this bigger shifter does not need to be rounded
logic [(M+4)-1:0] r_mant_lshifted;
logic [E-1:0] r_exp_lshifted;
assign r_mant_lshifted = r_mant_ext[(M+5)-2:0]<<shift;
assign r_exp_lshifted = is_zero ? E'(1) : r_exp_ext - E'(shift);

// Round mantissa
// The three GRS bits are removed, and an extra bit is added to allow overflow
logic round;
logic [(M+2)-1:0] r_mant_rounded;
logic inexact_round;
assign inexact_round = |r_mant_l1r1shifted[2:0];
assign round = (rm == RTZ) ?                    1'b0 : // Round towards Zero
               (rm == RDN) ?  r_sign & inexact_round : // Round Down (towards -Inf)
               (rm == RUP) ? ~r_sign & inexact_round : // Round Up (towards +Inf)
               (rm == RMM) ?   r_mant_l1r1shifted[2] : // Round to Nearest, ties to Max Magnitude
               r_mant_l1r1shifted[2] & |{r_mant_l1r1shifted[3],r_mant_l1r1shifted[1:0]}; // Round to Nearest, ties to Even
assign r_mant_rounded = r_mant_l1r1shifted[(M+4)-1:3] + (M+1)'(round);

// NOTE: According to IEEE754 spec, tininess should be detected after rounding.
// However, tininess before rounding seems to be passing all tests against
// softfloat library. Thus, tininess after rounding will not be implemented
// for now in order to save area. Further investigation should be put in here
// to find out why this is happening.

// Set underflow flag
logic unf;
assign unf = is_tiny_before_rounding & ~is_zero;
             
// Apply additional shifting if necessary
logic [(M+1)-1:0] r_mant_norm;
logic [(E+1)-1:0] r_exp_norm;
always_comb
begin
  // If overflow occurred during rounding, shift right and increment exponent
  if (r_mant_rounded[(M+2)-1] == 1'b1)
  begin
    r_mant_norm = r_mant_rounded[(M+2)-1:1];
    r_exp_norm = r_exp_l1r1shifted + 1'b1;
  end
  else
  begin
    r_mant_norm = r_mant_rounded[(M+2)-2:0];
    r_exp_norm = {1'b0,r_exp_l1r1shifted};
  end
end
  
// Select output of proper shifter
logic [(M+1)-1:0] r_mant_shifted;
logic [(E+1)-1:0] r_exp_shifted;
logic inexact;
always_comb
begin
  // If there were no overflow and there were at least two leading zeros,
  // the left shifter output is selected
  if (r_mant_ext[(M+5)-1 -: 3] == 3'b0)
  begin
    // The three GRS bits are discarded (they must be necessarily 0)
    r_mant_shifted = r_mant_lshifted[(M+4)-1:3];
    r_exp_shifted = {1'b0,r_exp_lshifted};
    inexact = 1'b0;
  end
  // Otherwise, the L1/R1 shifter + rounding stage output is selected
  else
  begin
    r_mant_shifted = r_mant_norm;
    r_exp_shifted = r_exp_norm;
    inexact = inexact_round;
  end
end

// Discard implicit leading 1
logic [M-1:0] r_mant;
logic [E-1:0] r_exp;
logic ovf;
always_comb
begin
  ovf = 1'b0;
  // If the resulting exponent is greater than or equal to the maximum value,
  // then set result according to sign and rounding mode
  if (r_exp_shifted >= MAX_EXP)
  begin
    case (rm)
      RTZ:
      begin
        // Set result to the format's largest finite number with the sign of the intermediate result
        r_exp = E'(MAX_EXP-1);
        r_mant = M'(MAX_MANT);
      end

      RDN:
      begin
        // Set result to the format's largest finite number if it is positive
        // or -Inf if it is negative
        r_exp = (r_sign == 1'b0) ? E'(MAX_EXP-1) : E'(MAX_EXP);
        r_mant = (r_sign == 1'b0) ? M'(MAX_MANT) : M'(0);
      end

      RUP:
      begin
        // Set result to +Inf if it is positive
        // or to the format's most negative finite number if it is negative
        r_exp = (r_sign == 1'b0) ? E'(MAX_EXP) : E'(MAX_EXP-1);
        r_mant = (r_sign == 1'b0) ? M'(0) : M'(MAX_MANT);
      end

      default:
      begin
        // Set result to Inf with the sign of the intermediate result
        r_exp = E'(MAX_EXP);
        r_mant = M'(0);
      end
    endcase
    ovf = 1'b1;
  end
  // Else if the resulting exponent is 1 and the mantissa is not normalized,
  // then set exponent to 0
  else if ((r_exp_shifted == (E+1)'(1)) && (r_mant_shifted[(M+1)-1] == 1'b0))
  begin
    r_exp = E'(0);
    r_mant = r_mant_shifted[M-1:0];
  end
  else
  begin
    r_exp = r_exp_shifted[E-1:0];
    r_mant = r_mant_shifted[M-1:0];
  end
end

// Register result of the third stage
always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    c <= W'(0);
    flags <= `FP_NUM_FLAGS'b0;
  end
  else if (ready == 1'b1)
  begin
    flags <= `FP_NUM_FLAGS'b0;
    if (a_is_Inf_r2 & b_is_Inf_r2 & (eop_r2 == SUB))
    begin
      c <= qNaN;
      flags[`FP_FLAG_INVALID] <= 1'b1;
    end
    else if (a_is_NaN_r2)
    begin
      // NOTE: RISCV does not seem to propagate NaNs
      c <= qNaN; //c <= a_eff_r2 | QUIET_BIT; // Set quiet bit in case of sNaN
      flags[`FP_FLAG_INVALID] <= invalid_r2;
    end
    else if (b_is_NaN_r2)
    begin
      // NOTE: RISCV does not seem to propagate NaNs
      c <= qNaN; //c <= b_eff_r2 | QUIET_BIT; // Set quiet bit in case of sNaN
      flags[`FP_FLAG_INVALID] <= invalid_r2;
    end
    else if (a_is_Inf_r2)
      c <= a_eff_r2;
    else if (b_is_Inf_r2)
      c <= b_eff_r2;
    else
    begin
      c <= {r_sign,r_exp,r_mant};
      flags[`FP_FLAG_OVF] <= ovf;
      // Underflow flag is only set if inexact flag is also set
      flags[`FP_FLAG_UNF] <= unf&inexact;
      // Inexact flag is also set if overflow flag is set
      flags[`FP_FLAG_INEXACT] <= ovf|inexact;
    end
  end
end

endmodule
