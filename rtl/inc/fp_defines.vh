// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _FP_DEFINES_
`define _FP_DEFINES_

// Rounding modes
`define FP_RM_BITS 3

// Flags
`define FP_FLAG_INVALID 4
`define FP_FLAG_DIV0    3
`define FP_FLAG_OVF     2
`define FP_FLAG_UNF     1
`define FP_FLAG_INEXACT 0
`define FP_NUM_FLAGS    5

// FLOAT10 defines
`define FLOAT10_S  0
`define FLOAT10_E  5
`define FLOAT10_M  5
`define FLOAT10_W  (`FLOAT10_S+`FLOAT10_E+`FLOAT10_M)
`define FLOAT10_POS_CQNAN 10'h3f0
`define FLOAT10_INF 10'h3e0
`define FLOAT10_MAX_NUM 10'h3df

// FLOAT11 defines
`define FLOAT11_S  0
`define FLOAT11_E  5
`define FLOAT11_M  6
`define FLOAT11_W  (`FLOAT11_S+`FLOAT11_E+`FLOAT11_M)
`define FLOAT11_POS_CQNAN 11'h7e0
`define FLOAT11_INF 11'h7c0
`define FLOAT11_MAX_NUM 11'h7bf

// FLOAT16 defines
`define FLOAT16_S  1
`define FLOAT16_E  5
`define FLOAT16_M 10
`define FLOAT16_W  (`FLOAT16_S+`FLOAT16_E+`FLOAT16_M)
`define FLOAT16_E_BIAS 15
`define FLOAT16_E_BIAS_HEX 4'hf
`define FLOAT16_E_MAX 5'h1f
`define FLOAT16_MAX_NUM 15'h7bff
`define FLOAT16_POS_CQNAN 16'h7e00
`define FLOAT16_INF 15'h7c00
`define FLOAT16_MAX 5'd30

// FLOAT16 defines in FLOAT32 format
`define FLOAT16_32_MAX 10'd142 // 15 + 127

// FLOAT32 defines
`define FLOAT32_S  1
`define FLOAT32_E  8
`define FLOAT32_M 23
`define FLOAT32_W  (`FLOAT32_S+`FLOAT32_E+`FLOAT32_M)
`define FLOAT32_E_BIAS 127
`define FLOAT32_E_BIAS_HEX 7'h7f
`define FLOAT32_NEG_E_BIAS_HEX 8'h81
`define FLOAT32_E_MAX 8'hff
`define FLOAT32_MAX_NUM 31'h7f7fffff
`define FLOAT32_POS_CQNAN 32'h7fc00000
`define FLOAT32_INF 31'h7f800000


// FLOAT64 defines
`define FLOAT64_S  1
`define FLOAT64_E 11
`define FLOAT64_M 52
`define FLOAT64_W  (`FLOAT64_S+`FLOAT64_E+`FLOAT64_M)

// FP_CLASSIFY defines
`define FP_CLASS_BITS         10
`define FP_CLASS_NEG_INF       0
`define FP_CLASS_NEG_NORMAL    1
`define FP_CLASS_NEG_DENORMAL  2
`define FP_CLASS_NEG_ZERO      3
`define FP_CLASS_POS_ZERO      4
`define FP_CLASS_POS_DENORMAL  5
`define FP_CLASS_POS_NORMAL    6
`define FP_CLASS_POS_INF       7
`define FP_CLASS_SNAN          8
`define FP_CLASS_QNAN          9

`endif // _FP_DEFINES_
