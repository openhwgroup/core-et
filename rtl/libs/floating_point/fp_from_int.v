// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_from_int
#(parameter N = 32,
  parameter S = 1,
  parameter E = 8,
  parameter M = 23
)
(in, is_signed, rm, out, flags);

// LOCAL PARAMETERS
localparam W = S+E+M;
localparam EXP_OFFSET = (1<<(E-1))-1; 
localparam MAX_EXP = (1<<E)-1;
localparam MAX_MANT = (1<<M) - 1;

// INPUT/OUTPUT INTERFACE
input  logic [N-1:0] in;
input  logic is_signed;
input  fp_rounding_mode rm;
output logic [W-1:0] out;
output logic [`FP_NUM_FLAGS-1:0] flags;

// CODE
// Obtain sign
logic r_sign;
assign r_sign = is_signed & in[N-1];

// Obtain absolute value from input
logic [N-1:0] in_abs;
assign in_abs = (r_sign == 1'b1) ? -in : in;

// Obtain the number of leading zeros
localparam LZ_BITS = $clog2(N);
logic [LZ_BITS-1:0] leading_zeros;
logic is_zero, ovf_shift;
leading_zero_counter #(.N_BITS(N)) lzc (.n(in_abs), .*);

// Detect overflow, if applicable
generate if (EXP_OFFSET+1 >= N-1)
begin: EXP_BIG
  assign ovf_shift = 1'b0;
end
else
begin: EXP_SMALL
  assign ovf_shift = leading_zeros < N-1-(EXP_OFFSET+1);
end
endgenerate

// Apply shifting and rounding
logic inexact;
logic [E-1:0] exp_shifted;
logic [(E+1)-1:0] exp_norm;
logic [N-1:0] in_shifted;
logic [(1+M)-1:0] mant_norm;

assign exp_shifted = ovf_shift ? E'(MAX_EXP) :
                     is_zero ?   E'(1) : E'(EXP_OFFSET+N-1) - E'(leading_zeros);

assign in_shifted = in_abs << leading_zeros;
generate if (1+M > N)
begin: M_BIG
  // In this case, no rounding will happen
  assign mant_norm = in_shifted << (1+M-N);
  assign exp_norm = {1'b0,exp_shifted};
  assign inexact = 1'b0;
end
else
begin: M_SMALL
  logic [N-(1+M)-1:0] shifted_out_bits;
  logic [(1+M)-1:0] mant_shifted;
  logic round;
  logic [(M+2)-1:0] mant_rounded;

  assign shifted_out_bits = in_shifted[N-(1+M)-1:0];
  assign inexact = |shifted_out_bits;
  assign mant_shifted = in_shifted[N-1 -: 1+M];

  assign round = (rm == RTZ) ?                        1'b0 : // Round towards Zero
                 (rm == RDN) ?            r_sign & inexact : // Round Down (towards -Inf)
                 (rm == RUP) ?           ~r_sign & inexact : // Round Up (towards +Inf)
                 (rm == RMM) ? shifted_out_bits[N-(1+M)-1] : // Round to Nearest, ties to Max Magnitude
                 shifted_out_bits[N-(1+M)-1] & |{mant_shifted[0],shifted_out_bits[N-(1+M)-2:0]}; // Round to Nearest, ties to Even
  assign mant_rounded = mant_shifted + (M+1)'(round);

  // Apply additional shifting if necessary
  always_comb
  begin
    // If overflow occurred during rounding, shift right and increment exponent
    if (mant_rounded[(M+2)-1] == 1'b1)
    begin
      exp_norm = exp_shifted + 1'b1;
      mant_norm = mant_rounded[(M+2)-1:1];
    end
    else
    begin
      exp_norm = {1'b0,exp_shifted};
      mant_norm = mant_rounded[(M+2)-2:0];
    end
  end
end
endgenerate

// Discard implicit leading 1
logic [E-1:0] r_exp;
logic [M-1:0] r_mant;
logic ovf;
always_comb
begin
  ovf = 1'b0;
  // If the resulting exponent is greater than or equal to the maximum value,
  // then set result according to sign and rounding mode
  if (exp_norm >= (E+1)'(MAX_EXP))
  begin
    case (rm)
      RTZ:
      begin
        // Set result to the format's largest finite number with the sign of the input
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
        // Set result to Inf with the sign of the input
        r_exp = E'(MAX_EXP);
        r_mant = M'(0);
      end
    endcase
    ovf = 1'b1;
  end
  // Else if the resulting exponent is 1 and the mantissa is not normalized,
  // then set exponent to 0
  else if ((exp_norm == (E+1)'(1)) && (mant_norm[(1+M)-1] == 1'b0))
  begin
    r_exp = E'(0);
    r_mant = mant_norm[M-1:0];
  end
  else
  begin
    r_exp = exp_norm[E-1:0];
    r_mant = mant_norm[M-1:0];
  end
end

// Assign outputs
assign flags[`FP_FLAG_INVALID] = 1'b0;
assign flags[`FP_FLAG_DIV0] = 1'b0;
assign flags[`FP_FLAG_UNF] = 1'b0;
generate if (S > 0)
begin: SIGNED_OUT
  assign out = {r_sign,r_exp,r_mant};
  assign flags[`FP_FLAG_OVF] = ovf;
  assign flags[`FP_FLAG_INEXACT] = ovf|inexact;
end
else
begin: UNSIGNED_OUT
  always_comb
  begin
    // If the input is negative but output does not have sign, saturate to 0
    if (r_sign == 1'b1)
    begin
      out = W'(0);
      flags[`FP_FLAG_OVF] = 1'b1;
      flags[`FP_FLAG_INEXACT] = 1'b1;
    end
    else
    begin
      out = {r_exp,r_mant};
      flags[`FP_FLAG_OVF] = ovf;
      flags[`FP_FLAG_INEXACT] = ovf|inexact;
    end
  end
end
endgenerate

endmodule
