// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_xbar — Parameterized NxM crossbar with credit-based flow control.
//
// Routes NumSrc sources to NumDest destinations through per-destination FIFO
// banks. Credit-counting FIFOs near the source track available space in the
// data FIFOs near the destination. A relay stage sits between source
// arbitration and the destination FIFOs.
//
// Per-source arbitration can be done by the crossbar (round-robin, one-hot
// ready) or by the source itself (multi-hot ready), controlled per-source
// by SrcArb. Per-destination arbitration is always round-robin with data
// muxing.
//
// Optional output relay FIFO per destination (AddOutputRelay).
//
// Replaces: shire_cache_xbar (etcore-soc)

module shirecache_xbar #(
  parameter int unsigned FifoDepth      = 3,
  parameter int unsigned NumSrc         = 8,
  parameter int unsigned NumDest        = 8,
  parameter int unsigned DataWidth      = 128,
  parameter bit [NumSrc-1:0]  NumSrcMask    = {NumSrc{1'b1}},
  parameter bit [NumSrc-1:0]  SrcArb        = '0,
  parameter bit [NumDest-1:0] AddOutputRelay = '0
) (
  input  logic                                    clk_i,
  input  logic                                    rst_ni,

  // Source side
  output logic [NumSrc-1:0][NumDest-1:0]          src_ready_o,
  output logic [NumSrc-1:0][NumDest-1:0]          src_2_credits_o,
  input  logic [NumSrc-1:0][NumDest-1:0]          src_valid_i,
  input  logic [NumSrc-1:0][DataWidth-1:0]        src_data_i,

  // Destination side
  input  logic [NumDest-1:0]                      dest_ready_i,
  output logic [NumDest-1:0]                      dest_valid_o,
  output logic [NumDest-1:0][DataWidth-1:0]       dest_data_o
);

  // ── Source relay signals ──────────────────────────────────
  logic [NumSrc-1:0][NumDest-1:0]           src_relay_valid;
  logic [NumSrc-1:0][DataWidth-1:0]         src_relay_data;

  // ── Destination FIFO signals ──────────────────────────────
  logic [NumDest-1:0][NumSrc-1:0]                     dest_fifo_valid;
  logic [NumDest-1:0][NumSrc-1:0]                     dest_fifo_full;
  logic [NumDest-1:0][NumSrc-1:0]                     dest_fifo_pop;
  logic [NumDest-1:0][NumSrc-1:0][DataWidth-1:0]      dest_fifo_data;
  logic [NumDest-1:0][NumSrc-1:0][FifoDepth:0]        dest_fifo_level;

  // Credit-counting FIFO unused outputs
  /* verilator lint_off UNUSEDSIGNAL */  // Credit FIFOs track occupancy only; data/valid/full outputs unused
  logic [NumDest-1:0][NumSrc-1:0]                     credit_fifo_valid_unused;
  logic [NumDest-1:0][NumSrc-1:0]                     credit_fifo_full_unused;
  logic [NumDest-1:0][NumSrc-1:0][1:0]                credit_fifo_data_unused;
  logic [NumDest-1:0][NumSrc-1:0][FifoDepth:0]        credit_fifo_level_unused;
  // Data FIFO unused outputs (full/level tracked by credit FIFOs instead)
  logic [NumDest-1:0][NumSrc-1:0]                     data_fifo_full_unused;
  logic [NumDest-1:0][NumSrc-1:0][FifoDepth:0]        data_fifo_level_unused;
  /* verilator lint_on UNUSEDSIGNAL */

  // ════════════════════════════════════════════════════════════
  // Data FIFOs: NumDest x NumSrc, near destination
  // ════════════════════════════════════════════════════════════

  for (genvar gd = 0; gd < NumDest; gd++) begin : gen_dest_fifos
    for (genvar gs = 0; gs < NumSrc; gs++) begin : gen_src_fifos
      if (NumSrcMask[gs]) begin : gen_en
        prim_fifo_reg #(
          .Width (DataWidth),
          .Depth (FifoDepth)
        ) u_dest_fifo (
          .clk_i,
          .rst_ni,
          .push_i  (src_relay_valid[gs][gd]),
          .wdata_i (src_relay_data[gs]),
          .full_o  (data_fifo_full_unused[gd][gs]),
          .pop_i   (dest_fifo_pop[gd][gs]),
          .valid_o (dest_fifo_valid[gd][gs]),
          .rdata_o (dest_fifo_data[gd][gs]),
          .level_o (data_fifo_level_unused[gd][gs])
        );
      end else begin : gen_disabled
        assign data_fifo_full_unused[gd][gs]  = '0;
        assign data_fifo_level_unused[gd][gs] = '0;
        assign dest_fifo_valid[gd][gs]        = '0;
        assign dest_fifo_data[gd][gs]         = '0;
      end
    end
  end

  // ════════════════════════════════════════════════════════════
  // Credit-counting FIFOs: NumSrc x NumDest, near source
  // ════════════════════════════════════════════════════════════

  for (genvar gs = 0; gs < NumSrc; gs++) begin : gen_src_credits
    for (genvar gd = 0; gd < NumDest; gd++) begin : gen_dest_credits
      prim_fifo_reg #(
        .Width (2),
        .Depth (FifoDepth)
      ) u_credit_fifo (
        .clk_i,
        .rst_ni,
        .push_i  (src_valid_i[gs][gd] & src_ready_o[gs][gd]),
        .wdata_i (2'b0),
        .full_o  (dest_fifo_full[gd][gs]),
        .pop_i   (dest_fifo_pop[gd][gs]),
        .valid_o (credit_fifo_valid_unused[gd][gs]),
        .rdata_o (credit_fifo_data_unused[gd][gs]),
        .level_o (dest_fifo_level[gd][gs])
      );
    end
  end

  // ════════════════════════════════════════════════════════════
  // Per-source arbitration
  // ════════════════════════════════════════════════════════════

  for (genvar gs = 0; gs < NumSrc; gs++) begin : gen_src_arb

    if (SrcArb[gs]) begin : gen_multi_hot
      // ── Source does arbitration: send multi-hot ready ──
      always_comb begin
        for (int ii = 0; ii < NumDest; ii++) begin
          src_ready_o[gs][ii]     = ~dest_fifo_full[ii][gs];
          src_2_credits_o[gs][ii] = ~dest_fifo_level[ii][gs][FifoDepth]
                                  & ~dest_fifo_level[ii][gs][FifoDepth-1];
        end
      end

    end else begin : gen_one_hot
      // ── Xbar does arbitration: RR among valid & not-full ──
      logic [NumDest-1:0] rr_req;
      always_comb begin
        for (int ii = 0; ii < NumDest; ii++) begin
          rr_req[ii]              = src_valid_i[gs][ii] & ~dest_fifo_full[ii][gs];
          src_2_credits_o[gs][ii] = ~dest_fifo_level[ii][gs][FifoDepth]
                                  & ~dest_fifo_level[ii][gs][FifoDepth-1];
        end
      end

      logic [NumDest-1:0] grants;
      assign src_ready_o[gs] = grants & rr_req;

      // Use prim_arb_rr with Width=1 (data unused for grant-only arbitration)
      /* verilator lint_off PINCONNECTEMPTY */  // data_o unused — grant-only arbiter
      prim_arb_rr #(
        .NumReqs (NumDest),
        .Width   (1)
      ) u_src_arb (
        .clk_i,
        .rst_ni,
        .reqs_i  (rr_req),
        .pop_i   (|src_ready_o[gs]),
        .grants_o(grants),
        .data_i  ({NumDest{1'b0}}),
        .data_o  ()
      );
      /* verilator lint_on PINCONNECTEMPTY */

    end
  end

  // ════════════════════════════════════════════════════════════
  // Per-source relay stage (between source arb and dest FIFOs)
  // ════════════════════════════════════════════════════════════

  for (genvar gs = 0; gs < NumSrc; gs++) begin : gen_src_relay
    // Relay valid: registered version of handshake
    logic relay_en;
    assign relay_en = |(src_valid_i[gs] & src_ready_o[gs])
                    | |src_relay_valid[gs];

    for (genvar gd = 0; gd < NumDest; gd++) begin : gen_dest_relay
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)       src_relay_valid[gs][gd] <= 1'b0;
        else if (relay_en) src_relay_valid[gs][gd] <= src_valid_i[gs][gd]
                                                    & src_ready_o[gs][gd];
      end
    end

    // Relay data: enabled register
    always_ff @(posedge clk_i) begin
      if (|(src_valid_i[gs] & src_ready_o[gs])) begin
        src_relay_data[gs] <= src_data_i[gs];
      end
    end
  end

  // ════════════════════════════════════════════════════════════
  // Per-destination arbitration (RR with data mux)
  // ════════════════════════════════════════════════════════════

  for (genvar gd = 0; gd < NumDest; gd++) begin : gen_dest_arb
    logic [NumSrc-1:0]      reqs;
    logic [NumSrc-1:0]      grants;
    logic                   pop_arb;
    logic [DataWidth-1:0]   arb_dest_data;

    assign reqs                 = dest_fifo_valid[gd];
    assign dest_fifo_pop[gd]    = grants & {NumSrc{pop_arb}};

    prim_arb_rr #(
      .NumReqs (NumSrc),
      .Width   (DataWidth)
    ) u_dest_arb (
      .clk_i,
      .rst_ni,
      .reqs_i  (reqs),
      .pop_i   (pop_arb),
      .grants_o(grants),
      .data_i  (dest_fifo_data[gd]),
      .data_o  (arb_dest_data)
    );

    if (AddOutputRelay[gd]) begin : gen_out_relay
      // ── Optional output relay FIFO ──
      logic                  push_out_rly;
      logic                  out_rly_valid;
      logic                  out_rly_full;
      logic [DataWidth-1:0]  out_rly_data;
      /* verilator lint_off UNUSEDSIGNAL */  // Output relay level not used
      logic [2:0]            out_rly_level_unused;
      /* verilator lint_on UNUSEDSIGNAL */

      assign push_out_rly = |dest_fifo_valid[gd];

      prim_fifo_reg #(
        .Width (DataWidth),
        .Depth (2)
      ) u_out_relay (
        .clk_i,
        .rst_ni,
        .push_i  (push_out_rly),
        .wdata_i (arb_dest_data),
        .full_o  (out_rly_full),
        .pop_i   (dest_ready_i[gd]),
        .valid_o (out_rly_valid),
        .rdata_o (out_rly_data),
        .level_o (out_rly_level_unused)
      );

      assign pop_arb           = !out_rly_full;
      assign dest_data_o[gd]   = out_rly_data;
      assign dest_valid_o[gd]  = out_rly_valid;

    end else begin : gen_no_out_relay

      assign pop_arb           = dest_ready_i[gd];
      assign dest_data_o[gd]   = arb_dest_data;
      assign dest_valid_o[gd]  = |dest_fifo_valid[gd];

    end
  end

endmodule : shirecache_xbar
