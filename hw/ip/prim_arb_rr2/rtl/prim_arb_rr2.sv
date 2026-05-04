// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Dual-grant round-robin arbiter with registered outputs.
//
// Selects TWO distinct entries from a set of requestors using round-robin
// fairness. The primary grant is chosen by a round-robin arbiter; the
// secondary grant is picked from the remaining requests using a priority
// scheme that prefers the MSB-priority result when it differs from the
// primary, otherwise falls back to the LSB-priority result.
//
// Outputs are registered (updated when alloc_i, ~valid, ~valid2, or esr_wr_i).
// grants_id_o / grants2_id_o are combinational one-hot-to-binary of the
// registered grant vectors.
//
// Replaces: arb_rr2 + arb_grants2 + hot2bin + arb_rr + onehot_mux (etcore-soc)

module prim_arb_rr2 #(
  parameter int unsigned NumReqs = 64
) (
  input  logic                            clk_i,
  input  logic                            rst_ni,

  input  logic [NumReqs-1:0]              reqs_i,     // Free entries
  input  logic                            alloc_i,    // Advance RR pointer
  input  logic                            esr_wr_i,   // Re-evaluate grants

  output logic                            valid_o,    // At least 1 free
  output logic                            valid2_o,   // At least 2 free
  output logic [NumReqs-1:0]              grants_o,   // Primary grant (one-hot, registered)
  output logic [NumReqs-1:0]              grants2_o,  // Secondary grant (one-hot, registered)
  output logic [$clog2(NumReqs)-1:0]      grants_id_o,  // Primary binary ID (comb)
  output logic [$clog2(NumReqs)-1:0]      grants2_id_o  // Secondary binary ID (comb)
);

  localparam int unsigned IdW = $clog2(NumReqs);

  // ── Filter requests: exclude currently granted entries ──────
  logic [NumReqs-1:0] reqs_valid;
  assign reqs_valid = reqs_i
                    & ~(grants_o  & {NumReqs{valid_o}})
                    & ~(grants2_o & {NumReqs{valid2_o}});

  // ── Primary grant: round-robin arbiter (combinational) ─────
  //
  // Instantiate prim_arb_rr with Width=1 (data mux unused).
  // We only care about grants_o from it.
  logic [NumReqs-1:0] grants_pre;

  /* verilator lint_off PINCONNECTEMPTY */  // data_o unused: we only need grants from prim_arb_rr
  prim_arb_rr #(
    .NumReqs (NumReqs),
    .Width   (1)
  ) u_arb_rr (
    .clk_i,
    .rst_ni,
    .reqs_i   (reqs_valid),
    .pop_i    (alloc_i),
    .grants_o (grants_pre),
    .data_i   ({NumReqs{1'b0}}),
    .data_o   ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  // ── Secondary grant: LSB + MSB priority, pick distinct one ─
  //
  // This replicates the arb_grants2 logic inline.
  // LSB-priority: thermometer encode from bit 0 upward.
  // MSB-priority: thermometer encode from bit (NumReqs-1) downward.

  logic [NumReqs-1:0] lsb_therm, msb_therm;
  logic [NumReqs-1:0] lsb_grant, msb_grant;

  // Thermometer codes
  for (genvar i = 0; i < NumReqs; i++) begin : gen_therm
    assign lsb_therm[i] = |(reqs_valid[0 +: (i+1)]);
    assign msb_therm[i] = |(reqs_valid[NumReqs-1 : i]);
  end

  // LSB-priority one-hot: XOR thermometer with itself shifted left
  if (NumReqs == 1) begin : gen_single
    assign lsb_grant = reqs_valid;
    assign msb_grant = reqs_valid;
  end else begin : gen_multi
    logic [NumReqs-1:0] lsb_mask, msb_mask;
    assign lsb_mask  = {lsb_therm[0 +: (NumReqs-1)], 1'b0};
    assign msb_mask  = {1'b0, msb_therm[NumReqs-1 : 1]};
    assign lsb_grant = lsb_therm ^ lsb_mask;
    assign msb_grant = msb_therm ^ msb_mask;
  end

  // Choose secondary: prefer MSB-priority if it differs from primary;
  // otherwise use LSB-priority.
  logic [NumReqs-1:0] grants2_pre;
  logic                msb_valid, lsb_valid;

  assign msb_valid = |(msb_grant ^ grants_pre);
  assign lsb_valid = |(lsb_grant ^ grants_pre);
  assign grants2_pre = msb_valid ? msb_grant : lsb_grant;

  // Pre-register valid signals
  logic valid_pre, valid2_pre;
  assign valid_pre  = |grants_pre;
  assign valid2_pre = msb_valid | lsb_valid;

  // ── Register enable ────────────────────────────────────────
  logic grants_en;
  assign grants_en = alloc_i | ~valid_o | ~valid2_o | esr_wr_i;

  // ── Registered outputs ─────────────────────────────────────
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      valid_o  <= 1'b0;
      valid2_o <= 1'b0;
    end else if (grants_en) begin
      valid_o  <= valid_pre;
      valid2_o <= valid2_pre;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      grants_o  <= '0;
      grants2_o <= '0;
    end else if (grants_en) begin
      grants_o  <= grants_pre;
      grants2_o <= grants2_pre;
    end
  end

  // ── One-hot to binary (combinational) ──────────────────────
  always_comb begin
    grants_id_o = '0;
    for (int i = 0; i < NumReqs; i++) begin
      if (grants_o[i]) begin
        grants_id_o = IdW'(i);
      end
    end
  end

  always_comb begin
    grants2_id_o = '0;
    for (int i = 0; i < NumReqs; i++) begin
      if (grants2_o[i]) begin
        grants2_id_o = IdW'(i);
      end
    end
  end

endmodule : prim_arb_rr2
