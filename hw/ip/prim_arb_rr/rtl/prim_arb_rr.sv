// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Round-robin arbiter with data multiplexing.
//
// Arbitrates among NumReqs requestors using round-robin fairness.
// Selects data from the granted requestor via one-hot mux.
// grants_o is one-hot when any req is active, zero otherwise.
// data_o holds the last granted data when no req is active.
//
// Replaces: arb_rr_data + arb_rr_base + onehot_mux (etcore-soc)

/* verilator lint_off UNOPTFLAT */  // Integrated users can close a false arbitration/data-mux loop after flattening, but the per-source grant and mux paths are acyclic.
module prim_arb_rr #(
  parameter int unsigned NumReqs = 4,
  parameter int unsigned Width   = 32
) (
  input  logic                            clk_i,
  input  logic                            rst_ni,

  input  logic [NumReqs-1:0]              reqs_i,
  input  logic                            pop_i,
  output logic [NumReqs-1:0]              grants_o,
  input  logic [NumReqs-1:0][Width-1:0]   data_i,
  output logic [Width-1:0]                data_o
);

  // ── Round-robin mask ──────────────────────────────────
  logic [NumReqs-1:0] mask_q;
  logic [NumReqs-1:0] masked_reqs;
  assign masked_reqs = mask_q & reqs_i;

  // Thermometer code for priority encoding
  logic [NumReqs-1:0] reqs_code, masked_reqs_code;
  for (genvar i = 0; i < NumReqs; i++) begin : gen_therm
    assign reqs_code[i]        = |(reqs_i[0 +: (i+1)]);
    assign masked_reqs_code[i] = |(masked_reqs[0 +: (i+1)]);
  end

  logic any_masked;
  assign any_masked = |masked_reqs;

  logic [NumReqs-1:0] muxed_code;
  assign muxed_code = any_masked ? masked_reqs_code : reqs_code;

  logic [NumReqs-1:0] mask_d;
  if (NumReqs == 1) begin : gen_single
    assign mask_d = 1'b0;
  end else begin : gen_multi
    assign mask_d = {muxed_code[0 +: (NumReqs-1)], 1'b0};
  end

  // Grant = one-hot from thermometer XOR
  assign grants_o = muxed_code ^ mask_d;

  // Held grant for data mux (keeps previous winner when no reqs)
  logic [NumReqs-1:0] grants_held;
  assign grants_held = (|grants_o) ? grants_o :
    mask_q ^ ({1'b1, {NumReqs-1{1'b0}}} | (mask_q >> 1));

  // Update mask on pop
  logic req_taken;
  assign req_taken = (|reqs_i) && pop_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)          mask_q <= '0;
    else if (req_taken)   mask_q <= mask_d;
  end

  // ── One-hot data mux ─────────────────────────────────
  always_comb begin
    data_o = '0;
    for (int i = 0; i < NumReqs; i++) begin
      data_o |= {Width{grants_held[i]}} & data_i[i];
    end
  end

endmodule : prim_arb_rr
/* verilator lint_on UNOPTFLAT */
