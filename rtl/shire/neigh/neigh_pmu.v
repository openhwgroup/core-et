// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_pmu (
  // System signals
  input  logic                                                                      clock,
  input  logic                                                                      reset,

  // Minion interface
  input  logic [`MIN_PER_N-1:0][`PMU_MINION_COUNTERS_RANGE]                         pmu_count_up,
  output logic [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`XREG_RANGE]                  pmu_read_data,
  input  logic [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE]   pmu_read_sel,
  input  logic [`MIN_PER_N-1:0][`PMU_TOTAL_COUNTERS_RANGE]                          pmu_write_en,
  input  logic [`MIN_PER_N-1:0][`XREG_RANGE]                                        pmu_write_data,
  input  logic [`MIN_PER_N-1:0][`PMU_NEIGH_EVENT_CNT_SEL_RANGE]                     pmu_neigh_event_sel,

  // Neigh interface
  input logic [`PMU_NEIGH_EVENTS:1]                                                  pmu_neigh_events,
     
  // Control                    
  input esr_pmu_ctrl_t                                                               pmu_ctrl
  );

  // Internal signals
  // -----------------------------------------------------------------------------
  logic [`PMU_NEIGH_COUNTERS_RANGE]                  pmu_neigh_count_up;
  logic [`PMU_NEIGH_COUNTERS_RANGE]                  pmu_neigh_count_up_next;
  logic [`MIN_PER_N-1:0][`PMU_NEIGH_COUNTERS_RANGE]  pmu_neigh_minion_count_up;
  logic [`MIN_PER_N-1:0][`PMU_NEIGH_COUNTERS_RANGE]  pmu_neigh_minion_count_up_next;

  logic                                              pmu_count_enable;

  // Get control bits/fields
  // -----------------------------------------------------------------------------
  assign pmu_count_enable = ~pmu_ctrl.disable_clock;

  // Mux neighborhood events
  // -----------------------------------------------------------------------------
  // Use pmu_neigh_event_sel from minions to mux events that will be counted in neigh counters
  // Selection made by lower index minions has precedence
  logic [`MIN_PER_N-1:0][`PMU_NEIGH_COUNTERS_NUM-1:0][`PMU_NEIGH_EVENTS_SEL_BITS-1:0] 
        pmu_neigh_event_sel_reg;

  // CLK    RST    EN                DOUT                     DIN                     DEF
  `RST_EN_FF(clock, reset, pmu_count_enable, pmu_neigh_event_sel_reg, pmu_neigh_event_sel,     '0)

  always_comb begin
    for (integer c=0; c<`PMU_NEIGH_COUNTERS_NUM; c++) begin
      for (integer m=0; m<`MIN_PER_N; m++) begin
        pmu_neigh_minion_count_up_next[m][c] = pmu_neigh_events[pmu_neigh_event_sel_reg[m][c]];
      end
    end
  end

  always_comb begin
    pmu_neigh_count_up_next = '0;

    for (integer c=0; c<`PMU_NEIGH_COUNTERS_NUM; c++) begin
      for (integer m=`MIN_PER_N-1; m>=0; m--) begin
        if (pmu_neigh_event_sel_reg[m][c] != '0)
        pmu_neigh_count_up_next[c] = pmu_neigh_minion_count_up[m][c];
      end
    end
  end


  `RST_EN_FF(clock, reset, pmu_count_enable, pmu_neigh_minion_count_up, pmu_neigh_minion_count_up_next, '0)
  `RST_EN_FF(clock, reset, pmu_count_enable, pmu_neigh_count_up,        pmu_neigh_count_up_next,        '0)

  // Combine write signal from minions
  // -----------------------------------------------------------------------------
  // When more than one minion tries to write, the one with lower index will have priority
  logic [`MIN_PER_N-1:0][`PMU_TOTAL_COUNTERS_RANGE] pmu_write_en_reg;
  logic [`MIN_PER_N-1:0][`XREG_RANGE]               pmu_write_data_reg;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                 pmu_write_en_comb;
  logic [`PMU_TOTAL_COUNTERS_RANGE][`XREG_RANGE]    pmu_write_data_comb;

  for (genvar i = 0; i < `MIN_PER_N; i++) begin : pmu_write_data_ff
    // CLK    RST    EN                                   DOUT                   DIN                DEF
    `RST_EN_FF(clock, reset, pmu_count_enable,                    pmu_write_en_reg[i],   pmu_write_en[i],   '0)
    `RST_EN_FF(clock, reset, pmu_count_enable & |pmu_write_en[i], pmu_write_data_reg[i], pmu_write_data[i], '0)
  end
  

  always_comb begin
    for (integer c=0; c<`PMU_TOTAL_COUNTERS_NUM; c++) begin
      pmu_write_en_comb[c] = 1'b0;
      pmu_write_data_comb[c] = pmu_write_data_reg[0];
      for (integer m=`MIN_PER_N-1; m>=0; m--) begin
        if (pmu_write_en_reg[m][c]) begin
          pmu_write_data_comb[c] = pmu_write_data_reg[m];
          pmu_write_en_comb[c] = 1'b1;
        end
      end
    end
  end

  // Implement pre-counters
  // -----------------------------------------------------------------------------
  logic [`PMU_TOTAL_COUNTERS_RANGE][`PMU_PRE_COUNTER_BITS-1:0] pre_counters;
  logic [`PMU_TOTAL_COUNTERS_RANGE][`PMU_PRE_COUNTER_BITS-1:0] pre_counters_next;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                            pre_counters_ov;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                            pre_counters_ov_next;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                            pre_counters_ov_clear;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                            pre_counters_en;


 
  for (genvar pc=0; pc<`PMU_TOTAL_COUNTERS_NUM; pc++) begin : pmu_pre_counter
    logic [`PMU_PRE_ADDER_BITS-1:0] pre_adder;
    logic pre_counter_up;

    // Minion counters
    if (pc < `PMU_MINION_COUNTERS_NUM) begin : pmu_pre_adders

      // Register pmu_count_up signal to improve timing
      logic [`MIN_PER_N-1:0]           per_minion_pmu_count_up;
      logic [`MIN_PER_N-1:0]           per_minion_pmu_count_up_reg;
      logic                            next_pre_count_up;
      logic                            pre_count_up_reg;

      always_comb begin
        for (integer m=0; m<`MIN_PER_N; m++)
        per_minion_pmu_count_up[m] = pmu_count_up[m][pc];
      end

      assign next_pre_count_up = |per_minion_pmu_count_up;

      `RST_FF(clock, reset, pre_count_up_reg, next_pre_count_up, 1'b0)

      logic gated_clock;

      et_clk_gate cgate(
        .enclk  ( gated_clock ),
        .en     ( {next_pre_count_up, pre_count_up_reg} != 2'b00 ),
        .clk    ( clock    ),
        .te     ( 1'b0     )
      );

      `RST_FF(gated_clock, reset, per_minion_pmu_count_up_reg, per_minion_pmu_count_up, `PMU_MINION_COUNTERS_NUM'b0)

      always_comb begin
        pre_counter_up = pre_count_up_reg;

        // Sum the contributions of inputs
        pre_adder = `PMU_PRE_ADDER_BITS'b0;

        for (integer m=0; m<`MIN_PER_N; m++)
        pre_adder = pre_adder + per_minion_pmu_count_up_reg[m];
      end
    end else begin
    // Neigh counters
      always_comb begin
        pre_counter_up = pmu_neigh_count_up[pc-`PMU_MINION_COUNTERS_NUM];
        pre_adder = {{(`PMU_PRE_ADDER_BITS-1){1'b0}},pre_counter_up};
      end
    end

    // Common for both counter types
    always_comb begin
      // Update counters
      if (pmu_write_en_comb[pc]) begin
        pre_counters_next[pc] = pmu_write_data_comb[pc][`PMU_PRE_COUNTER_BITS-1:0];
        pre_counters_ov_next[pc] = 1'b0;
      end else if (pre_counter_up) begin
        if (pre_counters_ov_clear[pc])
        {pre_counters_ov_next[pc],pre_counters_next[pc]} = {1'b0,pre_counters[pc]} + pre_adder;
        else
        {pre_counters_ov_next[pc],pre_counters_next[pc]} = {pre_counters_ov[pc],pre_counters[pc]} + pre_adder;
      end else begin
        if (pre_counters_ov_clear[pc])
        pre_counters_ov_next[pc] = 1'b0;
        else
        pre_counters_ov_next[pc] = pre_counters_ov[pc];
        pre_counters_next[pc] = pre_counters[pc];
      end

      pre_counters_en[pc] = (pre_counter_up & pmu_count_enable) | pmu_write_en_comb[pc] | pre_counters_ov_clear[pc];
    end

    // CLK    RESET  EN                   DOUT                 DIN                       DEF
    `RST_EN_FF (clock, reset, pre_counters_en[pc], pre_counters[pc],    pre_counters_next[pc],    `PMU_PRE_COUNTER_BITS'b0)
    `RST_EN_FF (clock, reset, pre_counters_en[pc], pre_counters_ov[pc], pre_counters_ov_next[pc], 1'b0)
  end


  // Implement counters with the control of increment and adder
  // -----------------------------------------------------------------------------
  logic [`PMU_TOTAL_COUNTERS_RANGE][`PMU_POST_COUNTER_BITS-1:0] post_counters;
  logic [`PMU_TOTAL_COUNTERS_RANGE][`PMU_POST_COUNTER_BITS-1:0] post_counters_next;
  logic [`PMU_TOTAL_COUNTERS_RANGE]                             post_counters_en;

  logic [`PMU_COUNTER_IDX_RANGE] cnt_idx;
  logic [`PMU_COUNTER_IDX_RANGE] cnt_idx_next;
  logic                          cnt_idx_up;

  // Track modifications to ov bits of pre-counters
  always_comb begin
    // Index of counter to be updated
    cnt_idx_next = cnt_idx;
    if (|pre_counters_ov) begin
      if (cnt_idx == (`PMU_TOTAL_COUNTERS_NUM-1))
      cnt_idx_next = `PMU_COUNTER_IDX_BITS'b0;
      else
      cnt_idx_next = cnt_idx + `PMU_COUNTER_IDX_BITS'b1;
    end

    // Clear ov bit
    pre_counters_ov_clear = {`PMU_TOTAL_COUNTERS_NUM{1'b0}};
    pre_counters_ov_clear[cnt_idx] = pre_counters_ov[cnt_idx];

    // Increment or modify counter
    post_counters_en = pmu_write_en_comb;
    post_counters_en[cnt_idx] |= (pre_counters_ov[cnt_idx] & pmu_count_enable);
  end

  // Update count index
  assign cnt_idx_up = |pre_counters_ov & pmu_count_enable;

  // CLK    RESET  EN          DOUT     DIN           DEF
  `RST_EN_FF (clock, reset, cnt_idx_up, cnt_idx, cnt_idx_next, `PMU_COUNTER_IDX_BITS'b0)

  // Generate the counters with a single adder
  logic [`PMU_POST_COUNTER_BITS-1:0] post_counters_mux;
  logic [`PMU_POST_COUNTER_BITS-1:0] post_counters_add;

  always_comb begin
    // Increment selected counter
    post_counters_mux = post_counters[cnt_idx];
    post_counters_add = post_counters_mux + {{(`PMU_POST_COUNTER_BITS-1){1'b0}},1'b1};

    // Modify counters
    post_counters_next = post_counters;
    for (integer c=0; c<`PMU_TOTAL_COUNTERS_NUM; c++) begin
      if (pmu_write_en_comb[c])
      post_counters_next[c] = pmu_write_data_comb[c][`XREG_SIZE-1:`PMU_PRE_COUNTER_BITS];
      else if (post_counters_en[c])
      post_counters_next[c] = post_counters_add;
    end
  end

  for (genvar pc=0; pc<`PMU_TOTAL_COUNTERS_NUM; pc++) begin : pmu_post_counter 
    `RST_EN_FF (clock, reset, post_counters_en[pc], post_counters[pc], post_counters_next[pc], {`PMU_POST_COUNTER_BITS{1'b0}})
  end
 

  // Connect counters to output
  // -----------------------------------------------------------------------------
  logic [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`XREG_RANGE] pmu_read_data_next;
  logic [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0]              pmu_read_data_up;

  // Register select data that comes from a very long distance (minion)
  logic [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE]      pmu_read_sel_reg;

  `FF (clock, pmu_read_sel_reg, pmu_read_sel)

  // Mux for register to read
  always_comb begin
    for (integer m=0; m<`MIN_PER_N; m++) begin

      pmu_read_data_next[m] = pmu_read_data[m];
      pmu_read_data_up[m]   = {`CORE_NR_THREADS{1'b0}};

      for (integer thread_id=0; thread_id<`CORE_NR_THREADS; thread_id++) begin
        // The number of counters is less than the total selectable range
        // Higher selectable index is used to do not make any selection
        if (pmu_read_sel_reg[m][thread_id] < `PMU_TOTAL_COUNTERS_NUM) begin
          pmu_read_data_next[m][thread_id] = {post_counters[pmu_read_sel_reg[m][thread_id]],
                                             pre_counters[pmu_read_sel_reg[m][thread_id]]};
          pmu_read_data_up[m][thread_id]   = 1'b1;
        end
      end
    end
  end

 for (genvar nm=0; nm<`MIN_PER_N; nm++) begin : pmu_read_register
    for (genvar th=0; th<`CORE_NR_THREADS; th++) begin
      `EN_FF (clock, pmu_read_data_up[nm][th], pmu_read_data[nm][th], pmu_read_data_next[nm][th])
    end
  end

endmodule
