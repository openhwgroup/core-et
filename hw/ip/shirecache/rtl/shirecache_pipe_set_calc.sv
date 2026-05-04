// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cache set index calculator.
//
// Determines the pipe_set based on the address and ESR configuration
// registers (set_base, set_size, set_mask). Supports runtime-configurable
// L2/L3/SCP cache partitioning.
//
// Pure combinational — ESR inputs are static, address is the dynamic path.

module shirecache_pipe_set_calc
  import shirecache_pkg::*;
(
  input  logic                      region_op_i,
  input  logic [SetIdSize-1:0]      address_set_i,
  input  logic [SetIdSize-1:0]      esr_set_base_i,
  input  logic [SetAllocSize-1:0]   esr_set_size_i,
  input  logic [SetIdSize-1:0]      esr_set_mask_i,
  input  logic [SetIdSize-1:0]      esr_tag_mask_i,  // assertion check only
  output logic [SetIdSize-1:0]      pipe_set_o
);

  // ── Static ESR-derived values ─────────────────────────

  // Set MSB of mask for the allocated buffer size.
  logic [SetIdSize-1:0] set_wrap_xor;
  assign set_wrap_xor = esr_set_mask_i ^ {1'b0, esr_set_mask_i[1 +: (SetIdSize-1)]};

  // Determine set above the buffer region (with carry bit).
  logic [SetAllocSize-1:0] set_compare_full;
  assign set_compare_full = esr_set_base_i + esr_set_size_i;

  logic [SetAllocSize-1:0] set_compare_full_masked;
  assign set_compare_full_masked = set_compare_full & {1'b0, esr_set_mask_i};

  logic [SetIdSize-1:0] set_compare_masked;
  assign set_compare_masked = set_compare_full_masked[SetIdSize-1:0];

  logic [SetIdSize-1:0] set_base_masked;
  assign set_base_masked = esr_set_base_i & esr_set_mask_i;

  logic base_gt_compare, base_lt_compare;
  assign base_gt_compare = (set_base_masked >  set_compare_masked);
  assign base_lt_compare = (set_base_masked <  set_compare_masked);

  // ── Dynamic address path ──────────────────────────────

  logic [SetIdSize-1:0] addr_set_masked;
  assign addr_set_masked = address_set_i & esr_set_mask_i;

  logic flip_msb;
  assign flip_msb =
    (base_gt_compare && ((addr_set_masked <  set_base_masked) &&
                         (addr_set_masked >= set_compare_masked))) ||
    (base_lt_compare && ((addr_set_masked <  set_base_masked) ||
                         (addr_set_masked >= set_compare_masked)));

  logic [SetIdSize-1:0] addr_set_mux;
  assign addr_set_mux = flip_msb ? (addr_set_masked ^ set_wrap_xor) : addr_set_masked;

  logic use_compare_msbs;
  assign use_compare_msbs = base_gt_compare &&
                            (set_compare_masked != '0) &&
                            !(|(addr_set_mux & set_wrap_xor));

  logic [SetIdSize-1:0] set_base_msbs;
  assign set_base_msbs = use_compare_msbs ? set_compare_full[SetIdSize-1:0]
                                          : esr_set_base_i;

  assign pipe_set_o = ((set_base_msbs & ~esr_set_mask_i) | addr_set_mux)
                    & {SetIdSize{region_op_i}};

  // Suppress unused warning for assertion-only input.
  logic unused;
  assign unused = ^{esr_tag_mask_i, set_compare_full_masked[SetAllocSize-1]};

endmodule
