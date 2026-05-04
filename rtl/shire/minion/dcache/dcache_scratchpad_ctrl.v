// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_scratchpad_ctrl (
  // System signals
  input  logic                            clock,
  input  logic                            reset,
  // Control bit from core
  input  dcache_mode_t                    scratchpad_ctrl,
  // Status from Dcache
  input  logic                            dcache_idle,
  // Clear Dcache flags and stall operations
  output logic                            dcache_cfg_stall,
  output logic                            dcache_cfg_clear_all,
  output logic                            dcache_cfg_clear_low,
  output logic                            dcache_cfg_alloc_scp,
  // Writes zeros to cleared Dcache lines
  output logic                            da_clear_en,
  output logic                            da_clear_val,
  output logic [`DCACHE_SET_RANGE]        da_clear_set,
  output logic [`DCACHE_WAY_RANGE]        da_clear_way,
  output logic                            da_clear_idx,
  input  logic                            da_clear_rdy,
  // Dcache mode
  output dcache_mode_t                    dcache_cfg_mode
);

//******************************************************************************
//  Internal signals
//******************************************************************************
  typedef enum logic [1:0] {
    SCP_CTRL_Initial   = 2'b00,  // Initial state after reset
    SCP_CTRL_Clear     = 2'b01,  // Clear Dcache for new configuration
    SCP_CTRL_Zero      = 2'b10,  // Write Zeros to cleared cache lines
    SCP_CTRL_All_Set   = 2'b11   // Everything ready, can operate again
  } dcache_scp_ctrl_t;

  dcache_scp_ctrl_t     state;       // Scratchpad control state
  dcache_scp_ctrl_t     state_next;

  logic                 ctrl_up;     // Update control registers

  dcache_mode_t         current_scp_ctrl;      // Current value for scratchpad control
  dcache_mode_t         current_scp_ctrl_next;

  logic                                   zero_count_end;
  logic                                   zero_count_up;
  logic [`DCACHE_LRAM_ROW_ADDR_BITS-1:0]  zero_counter;
  logic [`DCACHE_LRAM_ROW_ADDR_BITS-1:0]  zero_counter_next;
  logic [`DCACHE_LRAM_ROW_ADDR_BITS-1:0]  zero_count_limit;


//******************************************************************************
//  State Machine
//******************************************************************************

  // When a change in 'scratchpad_ctrl' bits is detected we go to 'initial state'
  // as in case of reset
  // From 'initial state', we wait until dcache is idle while we 'stall' the
  // pipeline (no new requests are allowed)
  // Then, Dcache is cleared and Scratchpad and reduced mode activated or
  // disabled depending on 'scratchpad_ctrl' bits

  always_comb begin
    state_next = state;
    current_scp_ctrl_next = current_scp_ctrl;

    // --------- SCP_CTRL_Initial ------------------------------------------------
    if (state == SCP_CTRL_Initial) begin
      if (dcache_idle) begin                // Wait for Dcache to be idle
        state_next = SCP_CTRL_Clear;
      end
    end else if (state == SCP_CTRL_Clear) begin
    // -------------- SCP_CTRL_Clear ---------------------------------------------
      state_next = SCP_CTRL_Zero;   // Go to write zero to cache lines
    end else if (state == SCP_CTRL_Zero) begin
    // -------------- SCP_CTRL_Zero ---------------------------------------------
      if (zero_count_end) begin
        state_next = SCP_CTRL_All_Set;   // Go to "all set" and store current 'scratchpad_ctrl' bit
        current_scp_ctrl_next = scratchpad_ctrl;
      end
    end else if (state == SCP_CTRL_All_Set) begin
    // -------------- SCP_CTRL_All_Set -------------------------------------------
      state_next = SCP_CTRL_Initial;   // Will go to initial state only if 'scratchpad_ctrl' bit changes
    end

  end

  // Allow updating control registers
  assign ctrl_up = (state != SCP_CTRL_All_Set) | (current_scp_ctrl != scratchpad_ctrl);

  //          CLK    RESET  EN       DOUT              DIN                    DEF
  `RST_EN_FF (clock, reset, ctrl_up, state,            state_next,            SCP_CTRL_Initial)
  `RST_EN_FF (clock, reset, ctrl_up, current_scp_ctrl, current_scp_ctrl_next, DcacheMode_Shared)

  // Control output for clearing flags
  //  Clear all sets when current or future state is default state
  //  Clear only low part of sets when neither current nor future states are the default one
  assign dcache_cfg_stall = (state != SCP_CTRL_All_Set);
  assign dcache_cfg_clear_all = (state == SCP_CTRL_Clear) &&  ((current_scp_ctrl == DcacheMode_Shared) || (scratchpad_ctrl == DcacheMode_Shared));
  assign dcache_cfg_clear_low = (state == SCP_CTRL_Clear) && !((current_scp_ctrl == DcacheMode_Shared) || (scratchpad_ctrl == DcacheMode_Shared));
  assign dcache_cfg_alloc_scp = (state == SCP_CTRL_Clear) &&  ((current_scp_ctrl == DcacheMode_Split)  && (scratchpad_ctrl == DcacheMode_Scratchpad));

  assign dcache_cfg_mode = current_scp_ctrl;

  // Control writing zeros to cache lines
  assign zero_count_limit = (((current_scp_ctrl == DcacheMode_Shared) || (scratchpad_ctrl == DcacheMode_Shared))) ? (`DCACHE_LRAM_ROWS-1) :
                                                                                                                    (`DCACHE_LRAM_ROWS_SPLIT-1);

  always_comb begin
    zero_count_up = (da_clear_val & da_clear_rdy) || (state == SCP_CTRL_Clear);

    zero_counter_next = (state == SCP_CTRL_Clear) ? {`DCACHE_LRAM_ROW_ADDR_BITS{1'b0}} : zero_counter + 'b1;
    zero_count_end = (zero_counter == zero_count_limit) && zero_count_up;
  end

  assign da_clear_val = (state == SCP_CTRL_Zero);
  assign da_clear_en  = (state == SCP_CTRL_Clear) || (state == SCP_CTRL_Zero);
  assign {da_clear_set,da_clear_idx,da_clear_way} = zero_counter;

  `EN_FF(clock, zero_count_up, zero_counter, zero_counter_next)

endmodule
