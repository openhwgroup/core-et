// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

typedef enum logic [`FP_RM_BITS-1:0] {
 RNE = `FP_RM_BITS'd0, // Round to Nearest, ties to Even
 RTZ = `FP_RM_BITS'd1, // Round towards Zero
 RDN = `FP_RM_BITS'd2, // Round Down (towards -Inf)
 RUP = `FP_RM_BITS'd3, // Round Up (towards +Inf)
 RMM = `FP_RM_BITS'd4  // Round to Nearest, ties to Max Magnitude
} fp_rounding_mode;

// FP_ADDER enum
typedef enum logic {ADD = 1'b0, SUB = 1'b1} fp_add_op;



////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////


typedef struct packed {
  logic sign;
  logic [`FLOAT16_E-1:0] e;
  logic [`FLOAT16_M-1:0] m;
} float16_t;

typedef struct packed {
  logic sign;
  logic [`FLOAT32_E-1:0] e;
  logic [`FLOAT32_M-1:0] m;
} float32_t;


