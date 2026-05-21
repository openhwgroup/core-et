// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncacheable_flb
  import dft_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned NumNeighP = NumNeigh,
  localparam int unsigned BarrierNumWidth = CsrFlBarrierNumWidth
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,
  input  uc_esr_flb_enable_sigs_t                   esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i,
  output uc_esr_barrier_values_t                    esr_barrier_values_o,

  input  logic [NumNeighP-1:0]                      flb_neigh_l2_req_valid_i,
  input  logic [NumNeighP-1:0][CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  output logic [NumNeighP-1:0]                      flb_l2_neigh_resp_valid_o,
  output logic [NumNeighP-1:0]                      flb_l2_neigh_resp_data_o
);

  logic clk_gt;
  logic clock_enable;
  dft_t dft_zero;
  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_wdata;
  assign unused_wdata = ^esr_wdata_i[shire_esr_pkg::EsrApbDataWidth-1:BarrierValueWidth];
  /* verilator lint_on UNUSEDSIGNAL */
  assign dft_zero = '0;

  logic [NumNeighP-1:0] flb_neigh_l2_req_valid_q;
  logic [NumNeighP-1:0][CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_q;

  for (genvar gen_it = 0; gen_it < NumNeighP; gen_it++) begin : gen_neigh_ff
    always_ff @(posedge clk_i) begin
      flb_neigh_l2_req_valid_q[gen_it] <= flb_neigh_l2_req_valid_i[gen_it];
      if (flb_neigh_l2_req_valid_i[gen_it]) begin
        flb_neigh_l2_req_data_q[gen_it] <= flb_neigh_l2_req_data_i[gen_it];
      end
    end
  end

  logic [NumNeighP-1:0][CsrFlBarrierSize-1:0] flb_pop_data;
  logic [NumNeighP-1:0] flb_pop_entry;
  logic [NumNeighP-1:0] flb_pending;
  logic [NumNeighP-1:0] flb_fifo_full_unused;
  logic [NumNeighP-1:0][FlbElemsPerNeigh:0] flb_fifo_level_unused;

  for (genvar gen_it = 0; gen_it < NumNeighP; gen_it++) begin : gen_input_fifo
    prim_fifo_reg #(
      .Width(CsrFlBarrierSize),
      .Depth(FlbElemsPerNeigh)
    ) u_req_fifo (
      .clk_i(clk_gt),
      .rst_ni(rst_ni),
      .push_i(flb_neigh_l2_req_valid_q[gen_it]),
      .wdata_i(flb_neigh_l2_req_data_q[gen_it]),
      .full_o(flb_fifo_full_unused[gen_it]),
      .pop_i(flb_pop_entry[gen_it]),
      .valid_o(flb_pending[gen_it]),
      .rdata_o(flb_pop_data[gen_it]),
      .level_o(flb_fifo_level_unused[gen_it])
    );
  end

  logic [NumNeighP-1:0] flb_arb_grant;
  logic [$clog2(NumNeighP)-1:0] flb_arb_winner;
  logic [BarrierNumWidth-1:0] flb_barrier_num;
  logic [BarrierRegisters-1:0][BarrierValueWidth-1:0] barrier_regs_q;
  logic [BarrierValueWidth-1:0] atomic_res;
  logic [BarrierRegisters-1:0] barrier_regs_en;
  logic flb_update_regs;
  logic [BarrierValueWidth-1:0] flb_barrier_value_next;
  logic [BarrierValueWidth-1:0] flb_update_barrier_value;

  for (genvar i = 0; i < BarrierRegisters; i++) begin : gen_barrier_reg
    always_ff @(posedge clk_gt or negedge rst_ni) begin
      if (!rst_ni) begin
        barrier_regs_q[i] <= '0;
      end else if (esr_enables_i[i] || barrier_regs_en[i]) begin
        barrier_regs_q[i] <= esr_enables_i[i] ? esr_wdata_i[BarrierValueWidth-1:0] : atomic_res;
      end
    end
    assign esr_barrier_values_o[i] = barrier_regs_q[i];
  end

  always_comb begin
    atomic_res = '0;
    barrier_regs_en = '0;
    if (flb_update_regs) begin
      barrier_regs_en[flb_barrier_num] = 1'b1;
      atomic_res = flb_barrier_value_next;
    end
  end

  prim_arb_lru #(
    .NumClients(NumNeighP)
  ) u_arb_to_l2 (
    .clk_i(clk_gt),
    .rst_ni(rst_ni),
    .bid_i(flb_pending),
    .stall_i(1'b0),
    .grant_o(flb_arb_grant),
    .winner_o(flb_arb_winner)
  );

  logic [CsrFlBarrierSize-1:0] flb_winner;
  logic [NumNeighP-1:0] flb_merge_winner;
  logic [NumNeighP-1:0] flb_merge_popcount;
  logic [BarrierValueWidth-1:0] flb_barrier_value;
  logic [BarrierValueWidth-1:0] flb_barrier_limit;
  logic flb_resp_data_prev;

  always_comb begin
    flb_resp_data_prev = 1'b0;
    flb_update_regs = 1'b0;
    flb_barrier_value_next = '0;
    flb_winner = flb_pop_data[flb_arb_winner];

    for (int unsigned i = 0; i < NumNeighP; i++) begin
      flb_merge_winner[i] = (flb_winner == flb_pop_data[i]) & flb_pending[i];
    end

    flb_merge_popcount = '0;
    for (int unsigned i = 0; i < NumNeighP; i++) begin
      if (flb_merge_winner[i]) begin
        flb_merge_popcount = flb_merge_popcount + {{NumNeighP-1{1'b0}}, 1'b1};
      end
    end

    flb_barrier_num = flb_winner[CsrFlBarrierNumLsb +: CsrFlBarrierNumWidth];
    flb_barrier_limit = flb_winner[CsrFlBarrierLimitLsb +: CsrFlBarrierLimitWidth];
    flb_barrier_value = barrier_regs_q[flb_barrier_num];
    flb_pop_entry = '0;
    flb_update_barrier_value = flb_barrier_value +
        {{(BarrierValueWidth-NumNeighP){1'b0}}, flb_merge_popcount};

    if (!esr_enables_i[flb_barrier_num]) begin
      flb_update_regs = |flb_pending;
      if (flb_update_barrier_value >= flb_barrier_limit) begin
        if (flb_barrier_value == flb_barrier_limit) begin
          flb_barrier_value_next = '0;
          flb_resp_data_prev = 1'b1;
        end else begin
          flb_barrier_value_next = flb_barrier_value + {{BarrierValueWidth-1{1'b0}}, 1'b1};
        end
        flb_pop_entry = flb_arb_grant;
      end else begin
        flb_barrier_value_next = flb_update_barrier_value;
        flb_pop_entry = flb_merge_winner;
      end
    end
  end

  logic flb_resp_data_q;

  always_ff @(posedge clk_gt or negedge rst_ni) begin
    if (!rst_ni) begin
      flb_l2_neigh_resp_valid_o <= '0;
    end else begin
      flb_l2_neigh_resp_valid_o <= flb_pop_entry;
    end
  end

  always_ff @(posedge clk_gt) begin
    if (|flb_pop_entry) begin
      flb_resp_data_q <= flb_resp_data_prev;
    end
  end

  assign flb_l2_neigh_resp_data_o = flb_l2_neigh_resp_valid_o & {NumNeighP{flb_resp_data_q}};

  assign clock_enable = !rst_ni || (|flb_neigh_l2_req_valid_q) || (|esr_enables_i) ||
                        flb_update_regs || (|flb_pop_entry) || (|flb_l2_neigh_resp_valid_o);

  prim_clk_gate u_clk_gate (
    .clk_i(clk_i),
    .en_i(clock_enable),
    .clk_o(clk_gt),
    .dft_i(dft_zero)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_fifo;
  assign unused_fifo = ^{flb_fifo_full_unused, flb_fifo_level_unused};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : uncacheable_flb
