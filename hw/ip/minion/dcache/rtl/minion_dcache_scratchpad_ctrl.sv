// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache scratchpad mode controller.
//
// Replaces: dcache_scratchpad_ctrl

module minion_dcache_scratchpad_ctrl
  import minion_dcache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  dcache_mode_t                scratchpad_ctrl_i,
  input  logic                        dcache_idle_i,
  output logic                        dcache_cfg_stall_o,
  output logic                        dcache_cfg_clear_all_o,
  output logic                        dcache_cfg_clear_low_o,
  output logic                        dcache_cfg_alloc_scp_o,
  output logic                        da_clear_en_o,
  output logic                        da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0] da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0] da_clear_way_o,
  output logic                        da_clear_idx_o,
  input  logic                        da_clear_rdy_i,
  output dcache_mode_t                dcache_cfg_mode_o
);

  typedef enum logic [1:0] {
    ScpCtrlInitial = 2'b00,
    ScpCtrlClear   = 2'b01,
    ScpCtrlZero    = 2'b10,
    ScpCtrlAllSet  = 2'b11
  } dcache_scp_ctrl_e;

  localparam int unsigned DcacheLramRowsLast = DcacheLramRows - 1;
  localparam int unsigned DcacheLramRowsSplitLast = DcacheLramRowsSplit - 1;

  dcache_scp_ctrl_e                 state_d;
  dcache_scp_ctrl_e                 state_q;
  dcache_mode_t                     current_scp_ctrl_d;
  dcache_mode_t                     current_scp_ctrl_q;
  logic                             ctrl_up;
  logic                             zero_count_end;
  logic                             zero_count_up;
  logic [DcacheLramRowAddrWidth-1:0] zero_counter_d;
  logic [DcacheLramRowAddrWidth-1:0] zero_counter_q;
  logic [DcacheLramRowAddrWidth-1:0] zero_count_limit;

  always_comb begin
    state_d = state_q;
    current_scp_ctrl_d = current_scp_ctrl_q;

    if (state_q == ScpCtrlInitial) begin
      if (dcache_idle_i) begin
        state_d = ScpCtrlClear;
      end
    end else if (state_q == ScpCtrlClear) begin
      state_d = ScpCtrlZero;
    end else if (state_q == ScpCtrlZero) begin
      if (zero_count_end) begin
        state_d = ScpCtrlAllSet;
        current_scp_ctrl_d = scratchpad_ctrl_i;
      end
    end else if (state_q == ScpCtrlAllSet) begin
      state_d = ScpCtrlInitial;
    end
  end

  assign ctrl_up = (state_q != ScpCtrlAllSet) || (current_scp_ctrl_q != scratchpad_ctrl_i);

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      state_q <= ScpCtrlInitial;
      current_scp_ctrl_q <= DcacheModeShared;
    end else if (ctrl_up) begin
      state_q <= state_d;
      current_scp_ctrl_q <= current_scp_ctrl_d;
    end
  end

  assign dcache_cfg_stall_o = (state_q != ScpCtrlAllSet);
  assign dcache_cfg_clear_all_o = (state_q == ScpCtrlClear) &&
                                  ((current_scp_ctrl_q == DcacheModeShared) ||
                                   (scratchpad_ctrl_i == DcacheModeShared));
  assign dcache_cfg_clear_low_o = (state_q == ScpCtrlClear) &&
                                  !((current_scp_ctrl_q == DcacheModeShared) ||
                                    (scratchpad_ctrl_i == DcacheModeShared));
  assign dcache_cfg_alloc_scp_o = (state_q == ScpCtrlClear) &&
                                  (current_scp_ctrl_q == DcacheModeSplit) &&
                                  (scratchpad_ctrl_i == DcacheModeScratchpad);
  assign dcache_cfg_mode_o = current_scp_ctrl_q;

  assign zero_count_limit =
      ((current_scp_ctrl_q == DcacheModeShared) || (scratchpad_ctrl_i == DcacheModeShared)) ?
      DcacheLramRowsLast[DcacheLramRowAddrWidth-1:0] :
      DcacheLramRowsSplitLast[DcacheLramRowAddrWidth-1:0];

  always_comb begin
    zero_count_up = (da_clear_val_o && da_clear_rdy_i) || (state_q == ScpCtrlClear);
    if (state_q == ScpCtrlClear) begin
      zero_counter_d = '0;
    end else begin
      zero_counter_d = zero_counter_q + 1'b1;
    end
    zero_count_end = (zero_counter_q == zero_count_limit) && zero_count_up;
  end

  assign da_clear_val_o = (state_q == ScpCtrlZero);
  assign da_clear_en_o = (state_q == ScpCtrlClear) || (state_q == ScpCtrlZero);
  assign {da_clear_set_o, da_clear_idx_o, da_clear_way_o} = zero_counter_q;

  always_ff @(posedge clk_i) begin
    if (zero_count_up) begin
      zero_counter_q <= zero_counter_d;
    end
  end

endmodule : minion_dcache_scratchpad_ctrl
