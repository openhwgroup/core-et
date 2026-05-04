// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_fixed_mult_1st
#(parameter E = 8,
  parameter M = 23,
  parameter I = 4,
  parameter F = 0,
  parameter signedi = 0
)
(clock, reset, f, i, rm, ready, r, flags);

// LOCAL PARAMETERS
localparam WF = 1+E+M;
localparam MAX_EXP = (1<<E) - 1;
localparam EXP_OFFSET = (1<<(E-1)) - 1;
localparam MAX_MANT = (1<<M) - 1;
localparam qNaN = {1'b0,{(E+1){1'b1}},(M-1)'(0)};
localparam QUIET_BIT = WF'(1<<(M-1));
localparam WI = I+F;
localparam MULT_WIDTH = M+1+WI;

// INPUT/OUTPUT INTERFACE
input logic clock, reset;
input logic [WF-1:0] f;
input logic [WI-1:0] i;
input fp_rounding_mode rm;
input ready;
output logic [WF-1:0] r;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
// Extract floating point components
logic f_sign;
logic [E-1:0] f_exp;
logic [M-1:0] f_mant;

assign f_sign = f[WF-1];
assign f_exp  = f[M +: E];
assign f_mant = f[0 +: M];

// Make some basic checkings
logic f_is_NaN, f_is_sNaN, f_is_Inf, i_is_zero, invalid;
assign f_is_NaN = (f_exp == E'(MAX_EXP)) && ~(f_mant == M'(0));
assign f_is_sNaN = f_is_NaN & (f_mant[M-1] == 1'b0);
assign f_is_Inf = (f_exp == E'(MAX_EXP)) && (f_mant == M'(0));
assign i_is_zero = i == WI'(0);
assign invalid = f_is_sNaN;

// In case of subnormal number, readjust exponent and mantissa
logic [E-1:0] f_exp_explicit;
logic [(M+1)-1:0] f_mant_explicit;
assign f_exp_explicit = (f_exp == E'(0)) ? E'(1) : f_exp;
assign f_mant_explicit = (f_exp == E'(0)) ? {1'b0, f_mant} : {1'b1, f_mant};

// Multiply mantissas
// The result's integer part is I+1 bits wide
// The result's fractional part is M+F bits wide
logic r_sign;
logic [MULT_WIDTH-1:0] r_mant_ext;
generate if (signedi == 0)
begin
  assign r_sign = f_sign;
  assign r_mant_ext = f_mant_explicit*i;
end
else
begin
  logic signed [WI-1:0] i_s;
  logic [WI-1:0] i_pos;
  // If the input is negative, invert it before multiplication
  // and invert the sign of the result
  assign i_s = signed'(i);
  assign r_sign = (i_s >= WI'(0)) ? f_sign : ~f_sign;
  assign i_pos = (i_s >= WI'(0)) ? i_s : -i_s;
  assign r_mant_ext = f_mant_explicit*i_pos;
end
endgenerate

// Obtain the number of leading zeros
// Only the first WI+1 bits are checked. If all of them are zero, there are
// only two possibilities:
// - Either the result is zero,
// - or f was denormal (f_exp == 0)
// In whichever case, the obtained shift is unimportant
localparam LZ_BITS = $clog2(WI+1);
logic [LZ_BITS-1:0] leading_zeros;
logic is_zero, is_tiny_before_rounding;
leading_zero_counter #(.N_BITS(1+WI)) lzc (.n(r_mant_ext[MULT_WIDTH-1 -: WI+1]), .*);
logic signed [(LZ_BITS+1)-1:0] nominal_lshift, exp_lshift, mant_rshift_s;
logic [LZ_BITS-1:0] mant_rshift;
assign nominal_lshift = signed'({1'b0,leading_zeros})-signed'(I[(LZ_BITS+1)-1:0]);
// Detect tininess before rounding
assign is_tiny_before_rounding = (signed'({1'b0,f_exp_explicit}) <= (E+1)'(nominal_lshift)) | is_zero;
// Obtain the actual shift to be applied (maximum left shift will be f_exp_explicit-1)
assign exp_lshift = ~is_tiny_before_rounding ? nominal_lshift : signed'(f_exp_explicit[(LZ_BITS+1)-1:0]) - 'sd1;
// The actual shift of the mantissa must take into account the F fractional bits added in the multiplication
// As a consequence, it will be never left shifted, thus we calculate it as a right shift
assign mant_rshift_s = signed'(F[(LZ_BITS+1)-1:0]) - exp_lshift;
assign mant_rshift = mant_rshift_s[LZ_BITS-1:0];

// Apply shifting and rounding
// At this point exponent is allowed to go beyond the maximum value
logic [(E+1)-1:0] r_exp_shifted;
logic [WI-1:0] shifted_out_bits;
logic [MULT_WIDTH-1:0] r_mant_shifted;
logic round;
logic [(M+2)-1:0] r_mant_rounded;
logic inexact;
assign r_exp_shifted = is_zero ? (E+1)'(1) : signed'({1'b0,f_exp_explicit}) - (E+1)'(exp_lshift);
// The maximum number of bits to shift out is WI
assign shifted_out_bits = (mant_rshift < WI[LZ_BITS-1:0]) ? r_mant_ext[WI-1:0]<<(WI[LZ_BITS-1:0]-mant_rshift) : r_mant_ext[WI-1:0];
assign inexact = |shifted_out_bits;
assign r_mant_shifted = (mant_rshift < WI[LZ_BITS-1:0]) ? r_mant_ext>>mant_rshift : {WI'(0),r_mant_ext[MULT_WIDTH-1 -: M+1]};
assign round = (rm == RTZ) ?                   1'b0 : // Round towards Zero
               (rm == RDN) ?       r_sign & inexact : // Round Down (towards -Inf)
               (rm == RUP) ?      ~r_sign & inexact : // Round Up (towards +Inf)
               (rm == RMM) ? shifted_out_bits[WI-1] : // Round to Nearest, ties to Max Magnitude
               shifted_out_bits[WI-1] & |{r_mant_shifted[0],shifted_out_bits[WI-2:0]}; // Round to Nearest, ties to Even
assign r_mant_rounded = r_mant_shifted[(M+2)-1:0] + (M+2)'(round);

// NOTE: According to IEEE754 spec, tininess should be detected after rounding.
// However, tininess before rounding seems to be passing all tests against
// softfloat library. Thus, tininess after rounding will not be implemented
// for now in order to save area. Further investigation should be put in here
// to find out why this is happening.

// Set underflow flag
logic unf;
assign unf = is_tiny_before_rounding & ~(r_mant_ext == MULT_WIDTH'(0));

// Apply additional shifting if necessary
logic [(E+1)-1:0] r_exp_norm;
logic [(M+1)-1:0] r_mant_norm;
always_comb
begin
  // If overflow occurred during rounding, shift right and increment exponent
  if (r_mant_rounded[(M+2)-1] == 1'b1)
  begin
    r_exp_norm = r_exp_shifted + 1'b1;
    r_mant_norm = r_mant_rounded[(M+2)-1:1];
  end
  else
  begin
    r_exp_norm = r_exp_shifted;
    r_mant_norm = r_mant_rounded[(M+2)-2:0];
  end
end

// Discard implicit leading 1
logic [E-1:0] r_exp;
logic [M-1:0] r_mant;
logic ovf;
always_comb
begin
  ovf = 1'b0;
  // If the resulting exponent is greater than or equal to the maximum value,
  // then set result according to sign and rounding mode
  if (r_exp_norm >= MAX_EXP)
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
  else if ((r_exp_norm == (E+1)'(1)) && (r_mant_norm[(M+1)-1] == 1'b0))
  begin
    r_exp = E'(0);
    r_mant = r_mant_norm[M-1:0];
  end
  else
  begin
    r_exp = r_exp_norm[E-1:0];
    r_mant = r_mant_norm[M-1:0];
  end
end

// Register result
always_ff @(posedge clock)
begin
  if (reset == 1'b1)
  begin
    r <= WF'(0);
    flags <= `FP_NUM_FLAGS'b0;
  end
  else if (ready == 1'b1)
  begin
    flags <= `FP_NUM_FLAGS'b0;
    if (f_is_NaN)
    begin
      // NOTE: RISCV does not seem to propagate NaNs
      r <= qNaN; //r <= f | QUIET_BIT; // Set quiet bit in case of sNaN
      flags[`FP_FLAG_INVALID] <= invalid;
    end
    else if (f_is_Inf && i_is_zero)
    begin
      r <= qNaN;
      flags[`FP_FLAG_INVALID] <= 1'b1;
    end
    else if (f_is_Inf && !i_is_zero)
      r <= {r_sign,f[WF-2:0]};
    else
    begin
      r <= {r_sign,r_exp,r_mant};
      flags[`FP_FLAG_OVF] <= ovf;
      // Underflow flag is only set if inexact flag is also set
      flags[`FP_FLAG_UNF] <= unf&inexact;
      // Inexact flag is also set if overflow flag is set
      flags[`FP_FLAG_INEXACT] <= ovf|inexact;
    end
  end
end

endmodule
