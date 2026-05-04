// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module fp_classify #(parameter S = 1, parameter E = 8, parameter M = 23)
(in, out);

// LOCAL PARAMETERS
localparam W = S+E+M;
localparam MAX_EXP = E'((1<<E) - 1);

// INPUT/OUTPUT INTERFACE
input logic [W-1:0] in;
output logic [`FP_CLASS_BITS-1:0] out;

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

// Checkings
logic in_max_exp, in_exp_zero, in_mant_zero;
logic is_Inf, is_normal, is_subnormal, is_zero, is_NaN, is_sNaN, is_qNaN;
assign in_max_exp = in_exp == MAX_EXP;
assign in_exp_zero = in_exp == E'(0);
assign in_mant_zero = in_mant == M'(0);
assign is_normal = ~in_max_exp & ~in_exp_zero;
assign is_subnormal = in_exp_zero & ~in_mant_zero;
assign is_zero = in_exp_zero & in_mant_zero;
assign is_Inf = in_max_exp & in_mant_zero;
assign is_NaN = in_max_exp & ~in_mant_zero;
assign is_sNaN = is_NaN & ~in_mant[M-1];
assign is_qNaN = is_NaN & in_mant[M-1];

// Assign outputs
assign out[`FP_CLASS_NEG_INF]       = in_sign & is_Inf;
assign out[`FP_CLASS_NEG_NORMAL]    = in_sign & is_normal;
assign out[`FP_CLASS_NEG_DENORMAL] = in_sign & is_subnormal;
assign out[`FP_CLASS_NEG_ZERO]      = in_sign & is_zero;
assign out[`FP_CLASS_POS_ZERO]      = ~in_sign & is_zero;
assign out[`FP_CLASS_POS_DENORMAL] = ~in_sign & is_subnormal;
assign out[`FP_CLASS_POS_NORMAL]    = ~in_sign & is_normal;
assign out[`FP_CLASS_POS_INF]       = ~in_sign & is_Inf;
assign out[`FP_CLASS_SNAN]          = is_sNaN;
assign out[`FP_CLASS_QNAN]          = is_qNaN;

endmodule
