// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_pmu #(
  parameter int unsigned NrMinions             = neigh_pkg::MinPerN,
  parameter int unsigned NrThreads             = minion_pkg::NrThreads,
  parameter int unsigned XregWidth             = minion_pkg::XregSize,
  parameter int unsigned PmuMinionCountersNum  = minion_pkg::PmuMinionCountersNum,
  parameter int unsigned PmuNeighCountersNum   = minion_pkg::PmuNeighCountersNum,
  parameter int unsigned PmuTotalCountersNum   = minion_pkg::PmuTotalCountersNum,
  parameter int unsigned PmuNeighEvents        = minion_pkg::PmuNeighEvents,
  parameter int unsigned PmuNeighEventsSelBits = minion_pkg::PmuNeighEventsSelBits,
  parameter int unsigned PmuNeighEventCntSelBits = minion_pkg::PmuNeighEventCntSelBits,
  parameter int unsigned PmuCountersSelectBits = minion_pkg::PmuCountersSelectBits,
  parameter int unsigned PmuPreAdderBits       = 4,
  parameter int unsigned PmuPreCounterBits     = 7,
  parameter int unsigned PmuPostCounterBits    = XregWidth - PmuPreCounterBits,
  parameter int unsigned PmuCounterIdxBits     = 4
) (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [NrMinions-1:0][PmuMinionCountersNum-1:0] pmu_count_up_i,
  output logic [NrMinions-1:0][NrThreads-1:0][XregWidth-1:0] pmu_read_data_o,
  input  logic [NrMinions-1:0][NrThreads-1:0][PmuCountersSelectBits-1:0] pmu_read_sel_i,
  input  logic [NrMinions-1:0][PmuTotalCountersNum-1:0] pmu_write_en_i,
  input  logic [NrMinions-1:0][XregWidth-1:0] pmu_write_data_i,
  input  logic [NrMinions-1:0][PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel_i,

  input  logic [PmuNeighEvents:1] pmu_neigh_events_i,
  input  esr_pkg::esr_pmu_ctrl_t pmu_ctrl_i
);

  localparam int unsigned PmuLastCounterIdxInt = PmuTotalCountersNum - 1;
  localparam logic [31:0] PmuLastCounterIdx32 = PmuLastCounterIdxInt;
  localparam logic [31:0] PmuTotalCountersNum32 = PmuTotalCountersNum;

  logic [PmuNeighCountersNum-1:0]                 pmu_neigh_count_up_q;
  logic [PmuNeighCountersNum-1:0]                 pmu_neigh_count_up_d;
  logic [NrMinions-1:0][PmuNeighCountersNum-1:0]  pmu_neigh_minion_count_up_q;
  logic [NrMinions-1:0][PmuNeighCountersNum-1:0]  pmu_neigh_minion_count_up_d;
  logic                                           pmu_count_enable;

  logic [NrMinions-1:0][PmuNeighCountersNum-1:0][PmuNeighEventsSelBits-1:0]
      pmu_neigh_event_sel_q;

  logic [NrMinions-1:0][PmuTotalCountersNum-1:0] pmu_write_en_q;
  logic [NrMinions-1:0][XregWidth-1:0]           pmu_write_data_q;
  logic [PmuTotalCountersNum-1:0]                pmu_write_en_comb;
  logic [PmuTotalCountersNum-1:0][XregWidth-1:0] pmu_write_data_comb;

  logic [PmuTotalCountersNum-1:0][PmuPreCounterBits-1:0] pre_counters_q;
  logic [PmuTotalCountersNum-1:0][PmuPreCounterBits-1:0] pre_counters_d;
  logic [PmuTotalCountersNum-1:0]                        pre_counters_ov_q;
  logic [PmuTotalCountersNum-1:0]                        pre_counters_ov_d;
  logic [PmuTotalCountersNum-1:0]                        pre_counters_ov_clear;
  logic [PmuTotalCountersNum-1:0]                        pre_counters_en;

  logic [PmuTotalCountersNum-1:0][PmuPostCounterBits-1:0] post_counters_q;
  logic [PmuTotalCountersNum-1:0][PmuPostCounterBits-1:0] post_counters_d;
  logic [PmuTotalCountersNum-1:0]                         post_counters_en;

  logic [PmuCounterIdxBits-1:0] cnt_idx_q;
  logic [PmuCounterIdxBits-1:0] cnt_idx_d;
  logic                         cnt_idx_up;

  logic [NrMinions-1:0][NrThreads-1:0][XregWidth-1:0] pmu_read_data_d;
  logic [NrMinions-1:0][NrThreads-1:0]                pmu_read_data_up;
  logic [NrMinions-1:0][NrThreads-1:0][PmuCountersSelectBits-1:0] pmu_read_sel_q;

  assign pmu_count_enable = ~pmu_ctrl_i.disable_clock;

  function automatic logic event_selected(
    input logic [PmuNeighEventsSelBits-1:0] sel
  );
    logic selected;
    begin
      selected = 1'b0;
      if (sel != '0) begin
        for (int unsigned e = 1; e <= PmuNeighEvents; e++) begin
          if (sel == e[PmuNeighEventsSelBits-1:0]) begin
            selected = pmu_neigh_events_i[e];
          end
        end
      end
      return selected;
    end
  endfunction

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pmu_neigh_event_sel_q <= '0;
    end else if (pmu_count_enable) begin
      pmu_neigh_event_sel_q <= pmu_neigh_event_sel_i;
    end
  end

  always_comb begin
    for (int unsigned c = 0; c < PmuNeighCountersNum; c++) begin
      for (int unsigned m = 0; m < NrMinions; m++) begin
        pmu_neigh_minion_count_up_d[m][c] = event_selected(pmu_neigh_event_sel_q[m][c]);
      end
    end
  end

  always_comb begin
    pmu_neigh_count_up_d = '0;
    for (int unsigned c = 0; c < PmuNeighCountersNum; c++) begin
      for (int m = NrMinions - 1; m >= 0; m--) begin
        if (pmu_neigh_event_sel_q[m][c] != '0) begin
          pmu_neigh_count_up_d[c] = pmu_neigh_minion_count_up_q[m][c];
        end
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pmu_neigh_minion_count_up_q <= '0;
      pmu_neigh_count_up_q <= '0;
    end else if (pmu_count_enable) begin
      pmu_neigh_minion_count_up_q <= pmu_neigh_minion_count_up_d;
      pmu_neigh_count_up_q <= pmu_neigh_count_up_d;
    end
  end

  for (genvar i = 0; i < NrMinions; i++) begin : gen_pmu_write_data_ff
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        pmu_write_en_q[i] <= '0;
        pmu_write_data_q[i] <= '0;
      end else begin
        if (pmu_count_enable) begin
          pmu_write_en_q[i] <= pmu_write_en_i[i];
        end
        if (pmu_count_enable && (|pmu_write_en_i[i])) begin
          pmu_write_data_q[i] <= pmu_write_data_i[i];
        end
      end
    end
  end

  always_comb begin
    for (int unsigned c = 0; c < PmuTotalCountersNum; c++) begin
      pmu_write_en_comb[c] = 1'b0;
      pmu_write_data_comb[c] = pmu_write_data_q[0];
      for (int m = NrMinions - 1; m >= 0; m--) begin
        if (pmu_write_en_q[m][c]) begin
          pmu_write_data_comb[c] = pmu_write_data_q[m];
          pmu_write_en_comb[c] = 1'b1;
        end
      end
    end
  end

  for (genvar pc = 0; pc < PmuTotalCountersNum; pc++) begin : gen_pmu_pre_counter
    logic [PmuPreAdderBits-1:0] pre_adder;
    logic                       pre_counter_up;

    if (pc < PmuMinionCountersNum) begin : gen_pmu_pre_adders
      logic [NrMinions-1:0] per_minion_pmu_count_up;
      logic [NrMinions-1:0] per_minion_pmu_count_up_q;
      logic                 next_pre_count_up;
      logic                 pre_count_up_q;
      logic                 gated_clk;
      logic                 gate_en;
      dft_pkg::dft_t        dft_zero;

      assign dft_zero = '0;

      always_comb begin
        for (int unsigned m = 0; m < NrMinions; m++) begin
          per_minion_pmu_count_up[m] = pmu_count_up_i[m][pc];
        end
      end

      assign next_pre_count_up = |per_minion_pmu_count_up;
      assign gate_en = (|{next_pre_count_up, pre_count_up_q});

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          pre_count_up_q <= 1'b0;
        end else begin
          pre_count_up_q <= next_pre_count_up;
        end
      end

      prim_clk_gate u_cgate (
        .clk_i (clk_i),
        .en_i  (gate_en),
        .dft_i (dft_zero),
        .clk_o (gated_clk)
      );

      always_ff @(posedge gated_clk or negedge rst_ni) begin
        if (!rst_ni) begin
          per_minion_pmu_count_up_q <= '0;
        end else begin
          per_minion_pmu_count_up_q <= per_minion_pmu_count_up;
        end
      end

      always_comb begin
        pre_counter_up = pre_count_up_q;
        pre_adder = '0;
        for (int unsigned m = 0; m < NrMinions; m++) begin
          pre_adder = pre_adder + {{(PmuPreAdderBits-1){1'b0}}, per_minion_pmu_count_up_q[m]};
        end
      end
    end else begin : gen_pmu_neigh_pre_adders
      always_comb begin
        pre_counter_up = pmu_neigh_count_up_q[pc-PmuMinionCountersNum];
        pre_adder = {{(PmuPreAdderBits-1){1'b0}}, pre_counter_up};
      end
    end

    always_comb begin
      if (pmu_write_en_comb[pc]) begin
        pre_counters_d[pc] = pmu_write_data_comb[pc][PmuPreCounterBits-1:0];
        pre_counters_ov_d[pc] = 1'b0;
      end else if (pre_counter_up) begin
        if (pre_counters_ov_clear[pc]) begin
          {pre_counters_ov_d[pc], pre_counters_d[pc]} = {1'b0, pre_counters_q[pc]}
              + {{(PmuPreCounterBits+1-PmuPreAdderBits){1'b0}}, pre_adder};
        end else begin
          {pre_counters_ov_d[pc], pre_counters_d[pc]} = {pre_counters_ov_q[pc], pre_counters_q[pc]}
              + {{(PmuPreCounterBits+1-PmuPreAdderBits){1'b0}}, pre_adder};
        end
      end else begin
        if (pre_counters_ov_clear[pc]) begin
          pre_counters_ov_d[pc] = 1'b0;
        end else begin
          pre_counters_ov_d[pc] = pre_counters_ov_q[pc];
        end
        pre_counters_d[pc] = pre_counters_q[pc];
      end

      pre_counters_en[pc] = (pre_counter_up && pmu_count_enable)
                          || pmu_write_en_comb[pc]
                          || pre_counters_ov_clear[pc];
    end
  end

  always_comb begin
    cnt_idx_d = cnt_idx_q;
    if (|pre_counters_ov_q) begin
      if (cnt_idx_q == PmuLastCounterIdx32[PmuCounterIdxBits-1:0]) begin
        cnt_idx_d = '0;
      end else begin
        cnt_idx_d = cnt_idx_q + {{(PmuCounterIdxBits-1){1'b0}}, 1'b1};
      end
    end

    pre_counters_ov_clear = '0;
    pre_counters_ov_clear[cnt_idx_q] = pre_counters_ov_q[cnt_idx_q];

    post_counters_en = pmu_write_en_comb;
    post_counters_en[cnt_idx_q] = post_counters_en[cnt_idx_q]
                                | (pre_counters_ov_q[cnt_idx_q] && pmu_count_enable);
  end

  assign cnt_idx_up = (|pre_counters_ov_q) && pmu_count_enable;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      cnt_idx_q <= '0;
    end else if (cnt_idx_up) begin
      cnt_idx_q <= cnt_idx_d;
    end
  end

  logic [PmuPostCounterBits-1:0] post_counters_mux;
  logic [PmuPostCounterBits-1:0] post_counters_add;

  always_comb begin
    post_counters_mux = post_counters_q[cnt_idx_q];
    post_counters_add = post_counters_mux + {{(PmuPostCounterBits-1){1'b0}}, 1'b1};
    post_counters_d = post_counters_q;
    for (int unsigned c = 0; c < PmuTotalCountersNum; c++) begin
      if (pmu_write_en_comb[c]) begin
        post_counters_d[c] = pmu_write_data_comb[c][XregWidth-1:PmuPreCounterBits];
      end else if (post_counters_en[c]) begin
        post_counters_d[c] = post_counters_add;
      end
    end
  end

  for (genvar pc = 0; pc < PmuTotalCountersNum; pc++) begin : gen_pmu_counter_regs
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        pre_counters_q[pc] <= '0;
        pre_counters_ov_q[pc] <= 1'b0;
        post_counters_q[pc] <= '0;
      end else begin
        if (pre_counters_en[pc]) begin
          pre_counters_q[pc] <= pre_counters_d[pc];
          pre_counters_ov_q[pc] <= pre_counters_ov_d[pc];
        end
        if (post_counters_en[pc]) begin
          post_counters_q[pc] <= post_counters_d[pc];
        end
      end
    end
  end

  // Original FF storage has no reset; preserve the unreset read-select pipeline.
  always_ff @(posedge clk_i) begin
    pmu_read_sel_q <= pmu_read_sel_i;
  end

  always_comb begin
    for (int unsigned m = 0; m < NrMinions; m++) begin
      pmu_read_data_d[m] = pmu_read_data_o[m];
      pmu_read_data_up[m] = '0;
      for (int unsigned thread_id = 0; thread_id < NrThreads; thread_id++) begin
        if (pmu_read_sel_q[m][thread_id] < PmuTotalCountersNum32[PmuCountersSelectBits-1:0]) begin
          pmu_read_data_d[m][thread_id] = {
            post_counters_q[pmu_read_sel_q[m][thread_id]],
            pre_counters_q[pmu_read_sel_q[m][thread_id]]
          };
          pmu_read_data_up[m][thread_id] = 1'b1;
        end
      end
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset read data registers.
  always_ff @(posedge clk_i) begin
    for (int unsigned m = 0; m < NrMinions; m++) begin
      for (int unsigned thread_id = 0; thread_id < NrThreads; thread_id++) begin
        if (pmu_read_data_up[m][thread_id]) begin
          pmu_read_data_o[m][thread_id] <= pmu_read_data_d[m][thread_id];
        end
      end
    end
  end

endmodule : neigh_pmu
