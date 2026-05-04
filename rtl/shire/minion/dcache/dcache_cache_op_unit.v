// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_cache_op_unit (
  // System signals
  input  logic                                      clock,
  input  logic                                      reset,
  // Chicken bit clock gate enable
  input  logic                                      chicken_bit_dcache,
  // Request bus to perform a cache operation
  output logic                                      busy,
  output logic                                      cache_op_ready,
  input  logic                                      cache_op_valid,
  input  core_dcache_op_req                         cache_op_req,
  input  core_dcache_op_req_next                    cache_op_req_next,
  output logic [`CORE_NR_THREADS-1:0][1:0]          th_id_complete,
  // Request to read the tag state
  output logic                                      meta_read_bid,
  input  logic                                      meta_read_grant,
  output logic                                      meta_read_valid,
  output logic                                      meta_read_phys,
  output logic                                      meta_read_force_set,
  output logic                                      meta_read_thread_id,
  output logic                                      meta_read_skip_pma,
  output logic [`VA_RANGE]                          meta_read_addr,
  output logic                                      meta_read_addr_msb_err,
  input  logic [`DCACHE_WAYS-1:0]                   meta_read_resp_way_en,
  input  dcache_meta_data [`DCACHE_WAYS-1:0]        meta_read_resp,
  input  logic [`PA_TAG_EXT_RANGE]                  s1_phys_addr_tag,
  // Feedback from TLB
  input  logic                                      s1_tlb_wait,
  input  logic                                      s1_tlb_fail,
  // Request to update the tag state
  input  logic                                      meta_write_ready,
  output logic                                      meta_write_valid_next,
  output logic                                      meta_write_valid,
  output dcache_meta_write_req                      meta_write_req,
  // Notification of the set that cache ops is working on
  output logic                                      nack_set_valid,
  output logic [`DCACHE_SET_RANGE]                  nack_set_value,
  // Request to the writeback unit to evict a line
  input  logic                                      writeback_ready,
  output logic                                      writeback_valid,
  output dcache_wb_req                              writeback_req,
  input  logic                                      writeback_done,
  // Request to pipeline to prefetch cache lines
  input  logic                                      id_rq_full,
  output logic                                      id_co_pf_bid,
  input  logic                                      id_co_pf_grant,
  output logic                                      s0_co_pf_valid,
  output dcache_req                                 s0_co_pf_req,
  // Request to CacheOpL2 unit to forward commands
  input  logic                                      cache_op_ready_byp,
  output logic                                      cache_op_valid_byp,
  output core_dcache_op_req                         cache_op_req_byp,
  // Update soft locks when MH is evicting one locked line
  input  logic                                      mh_clear_slock_valid,
  input  logic [`DCACHE_SET_RANGE]                  mh_clear_slock_set,
  input  logic [`DCACHE_WAY_RANGE]                  mh_clear_slock_way,
  // Scratchpad state info
  output logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] hlock_state,
  output logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] slock_state,
  // Data array clear lines interface
  output logic                                      da_clear_en,
  output logic                                      da_clear_val,
  output logic [`DCACHE_SET_RANGE]                  da_clear_set,
  output logic [`DCACHE_WAY_RANGE]                  da_clear_way,
  output logic                                      da_clear_idx,
  input  logic                                      da_clear_rdy,
  // Some info coming from other units in the dcache
  input  logic [`DCACHE_MH_FILE_SIZE-1:0]           mh_probe_rdy,
  input  logic [`DCACHE_MH_FILE_SIZE-1:0]           mh_metaw_rdy,
  // Potential conflict with instruction in the replay queue
  input  logic                                      rq_conflict,
  // System configuration
  input  logic                                      cfg_clear_all,
  input  logic                                      cfg_clear_low,
  input  logic                                      cfg_alloc_scp,
  input  dcache_mode_t                              cfg_cache_mode,
  // Management of VM status
  input  minion_vm_status [`CORE_NR_THREADS-1:0]    vm_status,
  output minion_vm_status                           vm_status_co,
  // Error flags
  output logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] err_flags,
  // Debug signas
  output logic [`DCACHE_DBG_CSR_COU_SIZE-1:0]       csr_debug_signals,
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]      sm_match_debug_signals,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]     sm_filter_debug_signals,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]       sm_data_debug_signals
);

  // Internal signals
  // -----------------------------------------------------------------------------
  cache_op_state_t                  state;                     // State of the snoop FSM
  cache_op_state_t                  state_next;

  cache_op_req_t                    req;                       // Request FSM is working on
  cache_op_req_t                    req_next;
  logic                             req_up;                    // Update request info
  logic                             req_up_id;                 // Update Id that is running even if op is NOP

  logic [`VA_RANGE]                 req_addr;                  // Address of the request
  logic [`VA_RANGE]                 req_addr_next;

  logic                             req_addr_msb_err;          // Error when increasing address
  logic                             req_addr_msb_err_next;

  logic [`TENSOR_MASK_BITS-1:0]     req_tmask_bits;            // Saved tensor mask bits
  logic                             req_tmask_up;              // Update tensor mask

  logic [3:0]                       req_count;                 // Number of operations pending to be done
  logic [3:0]                       req_count_next;
  logic                             req_count_up;              // Update the request counters, increase addresses, etc.

  logic [`VA_RANGE_CA]              req_stride;                // Stride between the requests

  logic [`DCACHE_SET_WAY_RANGE]     req_set_way;               // Set and way of the request for counting
  logic [`DCACHE_SET_WAY_RANGE]     req_set_way_next;

  logic                             meta_state_up;             // Update the meta state response FF

  logic [`DCACHE_SET_RANGE]         meta_read_set;             // Set of the address for which meta data is being read

  logic                             meta_read_resp_match;      // Flag indicating that tag matched in any way

  logic [`PA_TAG_EXT_RANGE]         meta_read_mux_tag;         // Internally muxed/modified versions of the fields read from metadata
  dcache_line_state                 meta_read_mux_state;
  logic                             meta_read_mux_match;
  logic [`DCACHE_WAY_RANGE]         meta_read_mux_way;

  logic [`DCACHE_WAYS-1:0]          meta_force_way_en;         // Forced way_en in set/way operations

  logic [`DCACHE_WAYS-1:0]          meta_data_way_en;          // Way where the line is according to meta data read
  logic [`DCACHE_WAYS-1:0]          meta_data_way_en_next;
  logic [`PA_RANGE]                 meta_data_req_addr;        // Physical address obtained from meta data read and set value
  logic [`PA_RANGE]                 meta_data_req_addr_next;

  logic [`PA_TAG_EXT_RANGE]         phys_addr_tag;             // Saved tag taken at S1 of the translated physical address
  logic                             phys_addr_up;              // Save tag

  logic                             is_set_way_op;             // The operation is set/way, so no tag match necessary
  logic                             is_lock_xx_op;             // The operation ia any of the lock variants
  dcache_line_state                 new_coh;                   // New coherent state set after the snoop
  dcache_line_state                 new_coh_next;
  logic                             abort_operation;           // Current operation is aborted (no writeback or meta write)
  logic                             abort_operation_next;
  logic                             err_lock;                  // Error detected during a lock operation
  logic                             err_lock_next;
  logic                             err_mem;                   // Error detected during a memory operation
  logic                             err_mem_next;
  logic                             term_loop;                 // Terminate loop
  logic                             term_loop_next;
  logic                             cancel_send_to_l2;         // Cancel a possible message being sent to L2 for Evicts

  logic                             cache_op_accepted;         // Accepts the cache op
  logic                             cache_op_is_L1;            // Cache op is for L1
  logic                             cache_op_is_L1_next;
  logic                             cache_op_is_NOP;           // Requested operation does not require to do anything
  logic                             cache_op_is_NOP_next;
  logic                             cache_op_start_L1_next;    // Start level will be L1
  logic                             cache_op_dest_L1_next;     // Dest level will be L1
  logic                             cache_op_prefetch;         // Command is a prefetch for L1

  logic                             send_op_to_l2;             // State to request that operation is sent to L2 by CacheOpL2 unit
  logic                             send_op_to_l2_next;

  logic                             clear_line;                // Clears a locked line

  logic [`DCACHE_SET_RANGE]         req_set;                   // Request set and way updated with consecutive operations
  logic [`DCACHE_WAY_RANGE]         req_way;

  logic                             id_running;                // Identifier currently running

  logic                             clkgt;                     // Gated clock
  logic                             clkgt_en;                  // Clock enable signal

  logic [`DCACHE_MH_FILE_SIZE-1:0]  mh_probe_rdy_reg;          // Stored version of the MH readiness to resolve conflicts

  cache_op_conflict_t               conflict_reason;           // Reason that moved us to have a conflict
  cache_op_conflict_t               conflict_reason_next;
  logic                             conflict_reason_up;

  logic                             da_clear_idx_next;         // Next value for ports dealing with cache lines clearing upon locking them
  logic                             clear_line_on;
  logic                             clear_line_on_next;


  // Aux conditions for FSM
  // -----------------------------------------------------------------------------
  assign cache_op_start_L1_next = cache_op_req_next.start_level == cache_op_L1;
  assign cache_op_dest_L1_next  = cache_op_req_next.dest_level == cache_op_L1;
  assign cache_op_prefetch = (cache_op_req.cmd == cache_op_Pref) && (cache_op_req.dest_level == cache_op_L1);

  assign cache_op_is_L1_next =   (cache_op_req_next.cmd == cache_op_Lock_VA   )                                                       ||
                                 (cache_op_req_next.cmd == cache_op_Unlock_VA )                                                       ||
                                ((cache_op_req_next.cmd == cache_op_Flush_SW  ) && !cache_op_dest_L1_next && cache_op_start_L1_next ) ||
                                ((cache_op_req_next.cmd == cache_op_Evict_SW  ) && !cache_op_dest_L1_next && cache_op_start_L1_next ) ||
                                 (cache_op_req_next.cmd == cache_op_Lock_SW   )                                                       ||
                                 (cache_op_req_next.cmd == cache_op_Unlock_SW )                                                       ||
                                ((cache_op_req_next.cmd == cache_op_Flush_VA  ) && !cache_op_dest_L1_next && cache_op_start_L1_next ) ||
                                ((cache_op_req_next.cmd == cache_op_Evict_VA  ) && !cache_op_dest_L1_next && cache_op_start_L1_next ) ||
                                ((cache_op_req_next.cmd == cache_op_Pref      ) &&  cache_op_dest_L1_next );

assign cache_op_is_NOP_next   =  ((cache_op_req_next.cmd == cache_op_Flush_SW  ) && (cache_op_dest_L1_next || !cache_op_start_L1_next) ) ||
                                 ((cache_op_req_next.cmd == cache_op_Evict_SW  ) && (cache_op_dest_L1_next || !cache_op_start_L1_next) ) ||
                                 ((cache_op_req_next.cmd == cache_op_Flush_VA  ) &&  cache_op_dest_L1_next )                             ||  
                                 ((cache_op_req_next.cmd == cache_op_Evict_VA  ) &&  cache_op_dest_L1_next )                             ||
                                 ((cache_op_req_next.cmd == cache_op_Pref      ) && !cache_op_dest_L1_next );

  assign cache_op_accepted = cache_op_valid && !send_op_to_l2 && cache_op_is_L1;

  //   CLK    DOUT             DIN
  `FF (clock, cache_op_is_L1,  cache_op_is_L1_next)
  `FF (clock, cache_op_is_NOP, cache_op_is_NOP_next)

  // Quallified conflict

  // *****************************************************************************
  // FSM
  // *****************************************************************************
  always_comb begin
    // FSM transition
    // This is an important diagram. We need to guarantee that while the snoop
    // is going on, no miss or evict changes the state of the set we are
    // working on. We need to guarantee that there's no miss handler doing
    // work in the same set and no new operation in same set passes beyond S1.
    // While snoop unit is busy, all requests in S1 going to same set are
    // nacked.
    // Below is a diagram with cycles on the X axis and pipe stage in Y axis.
    // It is known that by the time meta_read_valid is set for first time,
    // there's no blind cycle where miss handler can move from ready to not
    // ready.
    //
    //                  Accept      Bid                         S0 Read
    // S0               Op3 -> OK!  Op4 -> OK!                  Start S0 tag read
    // S1               Op2 -> OK!  Op3 -> Killed by S1 nack    Op4 -> Killed by S1 nack
    // S2               Op1 -> OK!  Op2 -> OK!                  Op3 -> Killed in S1         Op4 -> Killed in S1
    // Miss handler     Op0 -> OK!  Op1 -> OK!                  Op2 -> OK!                  Op3 -> Killed in S1
    //
    // Therefore, when we are in S0 read stage doing the meta read valid, it
    // is guaranteed by definition that miss handler can't move from ready to
    // not ready, so it is save to wait in this state until the ready signal
    // goes down from miss handler.

    state_next = state;
    cancel_send_to_l2 = 1'b0;
    err_mem_next = 1'b0;
    conflict_reason_next = conflict_reason;

    // -------------- Cache_Op_Invalid -----------------------------------------
    if (state == Cache_Op_Invalid) begin
      if (cache_op_accepted) begin
        if (!cache_op_req.use_tmask) begin
          if (cache_op_prefetch) begin
            state_next = Cache_Op_L1_Prefetch;
          end else begin
            state_next = Cache_Op_Meta_Read;
          end
        end else begin
          state_next = Cache_Op_Wait_Tmask;
        end
      end
    end
    // -------------- Cache_Op_Wait_Tmask --------------------------------------
    else if (state == Cache_Op_Wait_Tmask) begin
      if (cache_op_req.tmask_ready) begin
        if (cache_op_req.tmask_bits[0]) begin
          if (req.prefetch) begin
            state_next = Cache_Op_L1_Prefetch;
          end else begin
            state_next = Cache_Op_Meta_Read;
          end
        end else if (req_count != req.count) begin
          state_next = Cache_Op_Next_Operation;
        end else begin
          state_next = Cache_Op_Invalid;
        end
      end
    end
    // -------------- Cache_Op_Meta_Read ---------------------------------------
    else if (state == Cache_Op_Meta_Read) begin
      if (meta_read_valid) begin                      // Stay in state until access granted
        state_next = Cache_Op_Meta_Resp_Wait;
      end
    end
    // -------------- Cache_Op_Meta_Resp_Wait ----------------------------------
    else if (state == Cache_Op_Meta_Resp_Wait) begin
      state_next = Cache_Op_Meta_Resp;
      if (s1_tlb_wait) begin                                     // TLB not ready, wait for it
        state_next = Cache_Op_Wait_TLB;
      end else if (s1_tlb_fail) begin                                // TLB exception, finish operation
        state_next   = Cache_Op_Invalid;
        err_mem_next = 1'b1;
      end
    end
    // -------------- Cache_Op_Meta_Resp ---------------------------------------
    else if (state == Cache_Op_Meta_Resp) begin
      if (!(&mh_probe_rdy)) begin                                // This is evaluated in S2 stage of the main pipeline
        state_next = Cache_Op_Wait_Conflict;
        conflict_reason_next = CO_Conflict_MH;
        cancel_send_to_l2 = 1'b1;
      end else if (rq_conflict) begin
        state_next = Cache_Op_Wait_Conflict;
        conflict_reason_next = CO_Conflict_RQ;
        cancel_send_to_l2 = 1'b1;
      end else if (req.cmd == cache_op_Evict_SW) begin               // Evict_SW behavior
        if (meta_read_mux_state == State_Modified) begin         // Victim is dirty, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else begin
          if (meta_read_mux_state == State_Invalid) begin
            cancel_send_to_l2 = 1'b1;
          end
          state_next = Cache_Op_Meta_Write;                // Otherwise go to update database
        end
      end else if (req.cmd == cache_op_Flush_SW) begin               // Flush_SW behavior
        if (meta_read_mux_state == State_Modified) begin         // Victim is dirty, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else begin
          if (meta_read_mux_state == State_Invalid) begin
            cancel_send_to_l2 = 1'b1;
          end
          state_next = Cache_Op_Meta_Write;                // Otherwise go to update database
        end
      end else if (req.cmd == cache_op_Lock_SW) begin                // Lock_SW behavior
        if (!meta_read_resp_match &&
           (meta_read_mux_state == State_Modified)) begin        // Victim is dirty for a different address, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else begin
          state_next = Cache_Op_Meta_Write;                // Otherwise go to update database
        end
      end else if (req.cmd == cache_op_Unlock_SW) begin              // Unlock_SW behavior
        state_next = Cache_Op_Meta_Write_Lock;             // Just go to update unlock flag
      end else if (req.cmd == cache_op_Evict_VA) begin               // Evict_VA behavior
        if (meta_read_mux_match &&
           (meta_read_mux_state == State_Modified)) begin        // Victim is dirty, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else if (meta_read_mux_match) begin                      // Found in L1, go to update database
          state_next = Cache_Op_Meta_Write;
        end else begin                                         // No need to update the database.
          if (req.send_to_l2) begin
            state_next = Cache_Op_Wait_Next;               // Wait for L2 operation or to next operation if any pending
          end else if (req_count != req.count) begin
            state_next = Cache_Op_Next_Operation;
          end else begin
            state_next = Cache_Op_Invalid;
          end
        end
      end else if (req.cmd == cache_op_Flush_VA) begin               // Flush_VA behavior
        if (meta_read_mux_match &&
           (meta_read_mux_state == State_Modified)) begin        // Victim is dirty, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else if (meta_read_mux_match) begin                      // Otherwise go to update database if there was a match
          state_next = Cache_Op_Meta_Write;
        end else begin                                         // No need to update the database.
          if (req.send_to_l2) begin
            state_next = Cache_Op_Wait_Next;               // Wait for L2 operation or to next operation if any pending
          end else if (req_count != req.count) begin
            state_next = Cache_Op_Next_Operation;
          end else begin
            state_next = Cache_Op_Invalid;
          end
        end
      end else if (req.cmd == cache_op_Lock_VA) begin                // Lock_VA behavior
        if (!meta_read_resp_match &&
           (meta_read_mux_state == State_Modified)) begin        // Victim is dirty for a different address, do a writeback
          state_next = Cache_Op_Writeback_Req;
        end else begin
          state_next = Cache_Op_Meta_Write;                // Otherwise go to update database
        end
      end else if (req.cmd == cache_op_Unlock_VA) begin              // Unlock_VA behavior
        if (meta_read_mux_match) begin
          state_next = Cache_Op_Meta_Write_Lock;           // Just go to update unlock flag if we got a match
        end else begin
          if (req_count != req.count) begin                      // Move to next operation
            state_next = Cache_Op_Next_Operation;
          end else begin                                             // Everything done, go to invalid if no more operations
            state_next = Cache_Op_Invalid;
          end
        end
      end else begin
        state_next = Cache_Op_Invalid;                     // Never should reach this point
      end

      // An operation that is going to be cancelled, can never trigger a message to L2
      if (abort_operation_next) begin
        cancel_send_to_l2 = 1'b1;
      end
    end
    // -------------- Cache_Op_Wait_Conflict -----------------------------------
    else if (state == Cache_Op_Wait_Conflict) begin
      if (conflict_reason == CO_Conflict_MH) begin               // We are in this state because of a conflict with MH
        if (&(mh_probe_rdy_reg | mh_metaw_rdy)) begin            // Once the MH that was not ready has no pending Meta Data write, continue
          state_next = Cache_Op_Meta_Read;
        end
      end else begin // if (conflict_reason == CO_Conflict_MH)   // We are in this state because of a conflict with RQ
        if (!rq_conflict) begin                                  // No conflict remaining in Replay-Queue
          state_next = Cache_Op_Meta_Read;
        end
      end
    end
    // -------------- Cache_Op_Writeback_Req -----------------------------------
    else if (state == Cache_Op_Writeback_Req) begin
      if (abort_operation) begin                                 // In case of error, writeback request will be skipped
        state_next = Cache_Op_Meta_Write;
      end else if (writeback_ready) begin
        state_next = Cache_Op_Writeback_Resp;
      end
    end
    // -------------- Cache_Op_Writeback_Resp ----------------------------------
    else if (state == Cache_Op_Writeback_Resp) begin
      if (writeback_done) begin
        state_next = Cache_Op_Meta_Write;
      end
    end
    // -------------- Cache_Op_Meta_Write --------------------------------------
    else if (state == Cache_Op_Meta_Write) begin
      if (meta_write_ready | abort_operation) begin    // Operation may be aborted in case of error
        if (send_op_to_l2 | clear_line) begin          // Operation ongoing to Cache Ops L2 or having to clear a locked line, wait
          state_next = Cache_Op_Wait_Next;
        end else if (req_count != req.count) begin         // Move to next operation
          state_next = Cache_Op_Next_Operation;
        end else begin                                     // Everything done, go to invalid if no more operations
          state_next = Cache_Op_Invalid;
        end
      end
    end
    // -------------- Cache_Op_Meta_Write_Lock ---------------------------------
    else if (state == Cache_Op_Meta_Write_Lock) begin
      if (clear_line) begin                            // Operation requires a line to be zeroed
        state_next = Cache_Op_Wait_Next;
      end else if (req_count != req.count) begin           // Move to next operation
        state_next = Cache_Op_Next_Operation;
      end else begin                                       // Everything done, go to invalid if no more operations
        state_next = Cache_Op_Invalid;
      end
    end
    // -------------- Cache_Op_L1_Prefetch -------------------------------------
    else if (state == Cache_Op_L1_Prefetch) begin
      if (s0_co_pf_valid) begin
        state_next = Cache_Op_NextOp_Or_Invalid; // Check if there is a s1_tlb_fail
      end
    end
    //-----------------Cache_Op_NextOp_Or_Invalid-------------------------------
    else if (state == Cache_Op_NextOp_Or_Invalid) begin
      if (~s1_tlb_fail) begin
        if (req_count != req.count) begin              // Move to next operation
          state_next = Cache_Op_Next_Operation;
        end else begin                                     // Everything done, go to invalid if no more operations
          state_next = Cache_Op_Invalid;
        end
      end else begin
        state_next   = Cache_Op_Invalid;         // if s1_tlb_fail go to invalid and report err_mem
        err_mem_next = 1'b1;
      end
    end
    // -------------- Cache_Op_Wait_Next ---------------------------------------
    else if (state == Cache_Op_Wait_Next) begin
      if (!send_op_to_l2 & !clear_line) begin          // Once command accepted by CacheOps L2 or line cleared, move to next state
        if (req_count != req.count) begin              // Move to next operation
          state_next = Cache_Op_Next_Operation;
        end else begin                                     // Everything done, go to invalid if no more operations
          state_next = Cache_Op_Invalid;
        end
      end
    end
    // -------------- Cache_Op_Wait_TLB ----------------------------------------
    else if (state == Cache_Op_Wait_TLB) begin
      if (!s1_tlb_wait) begin                          // TLB ready again, retry operation
        state_next = Cache_Op_Meta_Read;
      end
    end
    // -------------- Cache_Op_Next_Operation ----------------------------------
    else if (state == Cache_Op_Next_Operation) begin
      if (term_loop) begin                                // Condition to do not continue doing further operations
        state_next = Cache_Op_Invalid;
      end else if (!send_op_to_l2 && !clear_line) begin       // Next operation is blocked if must send operation to L2 or must clear locked line
        if (!req.use_tmask) begin
          if (req.prefetch) begin                         // When not using mask, simply go to functional states
            state_next = Cache_Op_L1_Prefetch;
          end else begin
            state_next = Cache_Op_Meta_Read;
          end
        end else begin
          if (req_tmask_bits[req_count]) begin            // Jump to functional state if bit of mask is valid
            if (req.prefetch) begin
              state_next = Cache_Op_L1_Prefetch;
            end else begin
              state_next = Cache_Op_Meta_Read;
            end
          end else if (req_count == req.count) begin          // no more operations needed to be done
            state_next = Cache_Op_Invalid;
          end
        end
      end
    end
  end

// Enable state machine
logic state_up;
assign state_up = (state != Cache_Op_Invalid) | cache_op_valid;

//         CLK    RST    EN        DOUT    DIN          DEF
`RST_EN_FF(clkgt, reset, state_up, state,  state_next,  Cache_Op_Invalid)

// Save the ready state of the Miss Handler
logic mh_probe_rdy_up;
assign mh_probe_rdy_up = (state == Cache_Op_Meta_Resp);

//     CLK    EN               DOUT              DIN
`EN_FF(clkgt, mh_probe_rdy_up, mh_probe_rdy_reg, mh_probe_rdy)

// Save the conflict reason that moved us to Cache_Op_Wait_Conflict
assign conflict_reason_up = (state == Cache_Op_Meta_Resp);

//     CLK    EN                  DOUT             DIN
`EN_FF(clkgt, conflict_reason_up, conflict_reason, conflict_reason_next)

// Validated state Meta_Resp
logic state_Meta_Resp_qual;
assign state_Meta_Resp_qual = (state == Cache_Op_Meta_Resp) && (state_next != Cache_Op_Wait_Conflict);

// Pseudo-state to send  operation towards L2
// -----------------------------------------------------------------------------

// When operation is completed in L1, i.e. data have been evicted to L2 if necessary, then send request to L2
always_comb
begin
  // Pseudo state-machine transitions
  if (!send_op_to_l2)
  begin
    send_op_to_l2_next = req.send_to_l2 && !abort_operation && !cancel_send_to_l2 &&
                          ( ( state_Meta_Resp_qual              && (state_next != Cache_Op_Writeback_Req)) ||
                           ((state == Cache_Op_Writeback_Resp) && (state_next != Cache_Op_Writeback_Resp)) );
    end else begin
      send_op_to_l2_next = !cache_op_ready_byp; // Wait until accepted
    end

    // Request to CacheOpL2 to continue operation
    cache_op_valid_byp = send_op_to_l2;

    cache_op_req_byp.cmd          = req.cmd;
    cache_op_req_byp.start_level  = req.start_level;
    cache_op_req_byp.dest_level   = req.dest_level;
    cache_op_req_byp.count        = 4'b0;
    cache_op_req_byp.addr         = `ZX(`VA_SIZE,{meta_data_req_addr[`PA_RANGE_CA],6'b0}); // Avoid reading lowest 6 bits to remove registers
    cache_op_req_byp.way          = {`DCACHE_WAY_SZ{1'b0}};
    cache_op_req_byp.stride       = {{(`VA_SIZE-1){1'b0}},id_running};
    cache_op_req_byp.thread_id    = req.thread_id;
    cache_op_req_byp.use_tmask    = 1'b0;
    cache_op_req_byp.tmask_ready  = 1'b0;
    cache_op_req_byp.tmask_bits   = `TENSOR_MASK_BITS'b0;
    cache_op_req_byp.l2_scp_fill  = 1'b0;
    cache_op_req_byp.l2_scp_dest  = {(`DCACHE_L2_SCP_DST_BITS){1'b0}};
  end

  //      CLK    RST    EN   DOUT           DIN                 DEF
  `RST_FF(clkgt, reset,      send_op_to_l2, send_op_to_l2_next, 1'b0)


  // Notify possible conflicts with the sets being modified
  // -----------------------------------------------------------------------------
  assign nack_set_valid = state_Meta_Resp_qual              ||
                         (state == Cache_Op_Writeback_Req)  ||
                         (state == Cache_Op_Writeback_Resp) ||
                         (state == Cache_Op_Meta_Write)     ||
                         (state == Cache_Op_Meta_Write_Lock)||
                         clear_line_on;

  assign nack_set_value = meta_read_set;

  // *****************************************************************************
  // Store request information and update it
  // *****************************************************************************

  // Save request data
  assign req_up       = (state == Cache_Op_Invalid) && (state_next != Cache_Op_Invalid);
  assign req_tmask_up = (state == Cache_Op_Wait_Tmask);

  // Save ID even if the operation is a NOP
  assign req_up_id   =  (state == Cache_Op_Invalid) && (cache_op_ready & cache_op_valid);

  always_comb begin
    req_next.cmd         = cache_op_req.cmd;
    req_next.snoop_type  = (cache_op_req.cmd == cache_op_Evict_VA) ? Snoop_to_Invalid   // Move to invalidate for evict with addr
                         : (cache_op_req.cmd == cache_op_Evict_SW) ? Snoop_to_Invalid   // Move to invalidate for evict with set/way
                         : (cache_op_req.cmd == cache_op_Flush_VA) ? Snoop_to_Exclusive // Move to exclusive (as memory is non-coherent) for flush with addr
                         : (cache_op_req.cmd == cache_op_Flush_SW) ? Snoop_to_Exclusive // Move to exclusive (as memory is non-coherent) for flush with set/way
                         :                                           Snoop_to_Invalid;  // Default to invalid
    req_next.thread_id   = cache_op_req.thread_id;
    req_next.use_tmask   = cache_op_req.use_tmask;
    req_next.count       = cache_op_req.count;
    req_next.start_level = cache_op_req.start_level;
    req_next.dest_level  = cache_op_req.dest_level;
    req_next.prefetch    = cache_op_prefetch;
    req_next.send_to_l2  = (cache_op_req.dest_level > cache_op_L2) &&
                             ((cache_op_req.cmd == cache_op_Flush_SW ) ||
                              (cache_op_req.cmd == cache_op_Evict_SW ) ||
                              (cache_op_req.cmd == cache_op_Flush_VA ) ||
                              (cache_op_req.cmd == cache_op_Evict_VA ));
  end

  //      CLK    EN            DOUT             DIN
  `EN_FF (clkgt, req_up,       req,             req_next                         )
  `EN_FF (clkgt, req_up,       req_stride,      cache_op_req.stride[`VA_RANGE_CA])
  `EN_FF (clkgt, req_tmask_up, req_tmask_bits,  cache_op_req.tmask_bits          )
  `EN_FF (clkgt, req_up_id,    id_running,      cache_op_req.stride[0]           )

  // Store VM status for memory access
  //       CLK     EN      DOUT          DIN
  `EN_FF ( clkgt,  req_up, vm_status_co, (cache_op_req.thread_id == 1'b1)? vm_status[1] : vm_status[0] )

  // Save and update request data that changes when number of requests (req_count) is more than one
  assign req_count_up = req_up || ((state_next == Cache_Op_Next_Operation) & !send_op_to_l2);

  // Advance operation counters and addresses
  always_comb begin
    // Initialize the address to the requested one and increase the cache aligned chunk
    // according to the stride
    if (state == Cache_Op_Invalid) begin
      req_addr_next         = cache_op_req.addr;
      req_addr_msb_err_next = 1'b0;
    end else begin
      req_addr_next         = {req_addr[`VA_RANGE_CA],6'b0} + {req_stride,6'b0};
      req_addr_msb_err_next = req_addr[`VA_MSB] & !req_addr_next[`VA_MSB];
    end

    // Set/Way index counter for SW operations
    if (state == Cache_Op_Invalid) begin
      req_set_way_next = {cache_op_req.way,cache_op_req.addr[`DCACHE_SET_ADDR_RANGE]};
    end else begin
      req_set_way_next = req_set_way + 6'b1;
    end

    // Count the number of performed operations
    if (state == Cache_Op_Invalid) begin
      req_count_next = 4'b0;
    end else begin
      req_count_next = req_count + 4'b1;
    end
  end

  //      CLK    EN            DOUT              DIN
  `EN_FF (clkgt, req_count_up, req_addr,         req_addr_next        )
  `EN_FF (clkgt, req_count_up, req_addr_msb_err, req_addr_msb_err_next)
  `EN_FF (clkgt, req_count_up, req_count,        req_count_next       )
  `EN_FF (clkgt, req_count_up, req_set_way,      req_set_way_next     )

  // Split counter into the set and way components
  assign req_way = req_set_way[`DCACHE_SET_SZ+:`DCACHE_WAY_SZ];
  assign req_set = req_set_way[`DCACHE_SET_RANGE];

  // *****************************************************************************
  // Mux and store information from a meta data read
  // *****************************************************************************

  // Update meta data read while in meta response state (1 cycle only) or when an operation
  // starts to have a valid value in the set bits
  assign meta_state_up = state_Meta_Resp_qual;

  // Get a possible match in the read meta data. If tag matches according to the input
  // meta_read_resp_way_en, ensure that matching is with a valid state
  always_comb begin
    meta_read_resp_match = 1'b0;
    for (integer i = 0; i < `DCACHE_WAYS; i++) begin
      if (meta_read_resp_way_en[i]) begin
        meta_read_resp_match |= (meta_read_resp[i].coh_state >= State_Shared);
      end
    end
  end

  // Determine tag, state and encoded way of the matching way, if any
  // Defaults to tag = 0, way = 0 and state = Invalid if no match
  logic found_free_way;
  always_comb begin
    meta_force_way_en = `DCACHE_WAYS'b0;
    found_free_way = 1'b0;

    // If way is known beforehand, just pick the response for that way, but check for errors
    if ((is_set_way_op) || (req.cmd == cache_op_Lock_SW)) begin
      meta_read_mux_tag    = meta_read_resp[req_way].tag;
      meta_read_mux_state  = meta_read_resp[req_way].coh_state;

      meta_force_way_en   = dcache_way_decode(req_way);
      meta_read_mux_way   = req_way;
      meta_read_mux_match = 1'b1;
    end
    // Otherwise get data from the response
    else begin
      meta_read_mux_match  = |meta_read_resp_way_en;
      meta_read_mux_tag    = meta_read_resp[0].tag;
      meta_read_mux_state  = meta_read_resp[0].coh_state;
      meta_read_mux_way    = dcache_way_encode(meta_read_resp_way_en);

      for (integer i = 1; i < `DCACHE_WAYS; i++) begin
        if (meta_read_resp_way_en[i]) begin
          meta_read_mux_tag   = meta_read_resp[i].tag;
          meta_read_mux_state = meta_read_resp[i].coh_state;
        end
      end

      // Lock_VA with no match has to look for a victim
      // Priority are those entries which state is invalid and which state is not soft-locked
      // Hard-locked entries cannot be taken
      if (!meta_read_mux_match && (req.cmd == cache_op_Lock_VA)) begin
        for (integer i = 0; i < `DCACHE_WAYS; i++) begin
          if ((meta_read_resp[i].coh_state == State_Invalid) && !found_free_way) begin
            found_free_way = 1'b1;
            meta_read_mux_way = i;
            meta_read_mux_tag   = meta_read_resp[i].tag;
            meta_read_mux_state = meta_read_resp[i].coh_state;
          end
        end

        // If way not found considering invalid lines, try again excluding locked lines
        if (!found_free_way) begin
          for (integer i = 0; i < `DCACHE_WAYS; i++) begin
            if (!hlock_state[meta_read_set][i] && !slock_state[meta_read_set][i] && !found_free_way) begin
              found_free_way = 1'b1;
              meta_read_mux_way = i;
              meta_read_mux_tag   = meta_read_resp[i].tag;
              meta_read_mux_state = meta_read_resp[i].coh_state;
            end
          end
        end

        // Finally, consider any non hard-locked line
        if (!found_free_way) begin
          for (integer i = 0; i < `DCACHE_WAYS; i++) begin
            if (!hlock_state[meta_read_set][i] && !found_free_way) begin
              found_free_way = 1'b1;
              meta_read_mux_way = i;
              meta_read_mux_tag   = meta_read_resp[i].tag;
              meta_read_mux_state = meta_read_resp[i].coh_state;
            end
          end
        end

      end

      // Evict VA or Flush VA with no match have to get the address to propagate the operation from the physical address
      if (!meta_read_mux_match && ((req.cmd == cache_op_Evict_VA) || (req.cmd == cache_op_Flush_VA))) begin
        meta_read_mux_tag = phys_addr_tag;
      end

    end
  end

  // Compose addr using TAG from meta data and set from request address (set is common to VA and PA)
  // "meta_data_req_addr" is updated when analizing a meta read response
  always_comb begin
    if (is_set_way_op) begin
      meta_data_req_addr_next = {meta_read_mux_tag,req_set[`DCACHE_SET_RED_RANGE],6'b0};
    end else begin
      meta_data_req_addr_next = {meta_read_mux_tag,req_addr[`DCACHE_SET_RED_ADDR_RANGE],6'b0};
    end
  end

  // Select way_en stored depending on the source. For non-set_way operations, if there is no match
  // in the read data, by default way 0 will be selected.
  assign meta_data_way_en_next = is_set_way_op ? meta_force_way_en : dcache_way_decode(meta_read_mux_way);

  // Capture physical address when present in S1 pipeline stage
  assign phys_addr_up = (state == Cache_Op_Meta_Resp_Wait);

  //     CLK    EN             DOUT                 DIN
  `EN_FF(clkgt, meta_state_up, meta_data_way_en,    meta_data_way_en_next)
  `EN_FF(clkgt, meta_state_up, meta_data_req_addr,  meta_data_req_addr_next)
  `EN_FF(clkgt, phys_addr_up,  phys_addr_tag,       s1_phys_addr_tag)

  // *****************************************************************************
  // Abort and error conditions
  // *****************************************************************************

  logic       abort_up;         // Update error flags
  logic [2:0] count_hlock_zero; // Counts number of zeros in the hlock array for a given set

  // Operation must be aborted (no effect, i.e. no write) if any error happens
  always_comb begin
    abort_operation_next = abort_operation;
    err_lock_next = 1'b0;
    abort_up = (cache_op_valid && (state == Cache_Op_Invalid)) || (state == Cache_Op_Next_Operation) || state_Meta_Resp_qual;

    // Clear abort flag when receiving a new request or when starting a new operation
    if ((cache_op_valid && (state == Cache_Op_Invalid)) || (state == Cache_Op_Next_Operation)) begin
      abort_operation_next = 1'b0;
    end else
    // Set abort flag during the meta data response cycle to avoid any write in following cycles
    if (state_Meta_Resp_qual) begin
      // On hard-locked lines, only some operations are allowed
      if (meta_read_mux_match & hlock_state[meta_read_set][meta_read_mux_way]) begin
        // Unlocks are allowed, although Unlock_VA can only remove soft lock, so not touching hard lock
        // Flushes are allowed as they only send data up to the L2
        // Lock_VA is allowed since it has to update the metadata to "modified" state
        // Evict_VA is not allowed
        if (req.cmd != cache_op_Unlock_VA & req.cmd != cache_op_Unlock_SW &
            req.cmd != cache_op_Flush_VA  & req.cmd != cache_op_Flush_SW  &
            req.cmd != cache_op_Lock_VA) begin
          abort_operation_next = 1'b1;
        end

        // Flag an error if trying to lock again
        if (req.cmd == cache_op_Lock_SW) begin
          abort_operation_next = 1'b1;
          err_lock_next = 1'b1;
        end
      end

      // An evict on a hard-locked line cannot be executed
      if (req.cmd == cache_op_Evict_SW) begin
        if (hlock_state[meta_read_set][meta_read_mux_way]) begin
          abort_operation_next = 1'b1;
        end
      end

      // During a lock SW, additional conditions can lead to set the error bit
      if (req.cmd == cache_op_Lock_SW) begin
        // There has been a matching in a different way than that specified
        if (|meta_read_resp_way_en && (dcache_way_encode(meta_read_resp_way_en) != meta_read_mux_way)) begin
          abort_operation_next = 1'b1;
          err_lock_next = 1'b1;
        end

        // Locking when only one way is not hard-locked (either trying to lock again or locking the remaining way is an error)
        count_hlock_zero = 'b0;
        for (integer i = 0; i < `DCACHE_WAYS; i++) begin
          if (hlock_state[meta_read_set][i] == 1'b0) begin
            count_hlock_zero = count_hlock_zero + 'd1;
          end
        end
        if (count_hlock_zero <= 'd1) begin
          abort_operation_next = 1'b1;
          err_lock_next = 1'b1;
        end
      end

    end

    // Loop of operations is terminated upon first error
    if (state == Cache_Op_Invalid) begin
      term_loop_next = 1'b0;
    end else begin
      term_loop_next = err_lock_next | term_loop;
    end
  end

  //      CLK    RESET  EN        DOUT             DIN                  DEF
  `EN_FF (clkgt,        abort_up, abort_operation, abort_operation_next     )
  `RST_FF(clkgt, reset,           err_lock,        err_lock_next,       1'b0)
  `RST_FF(clkgt, reset,           err_mem,         err_mem_next,        1'b0)
  `RST_FF(clkgt, reset,           term_loop,       term_loop_next,      1'b0)

  // Indicate that some error flags are active to keep clock alive
  logic err_ffs;
  assign err_ffs = err_lock | err_mem | term_loop;

  // Output error
  assign err_flags[`DCACHE_ERR_FLAG_CO_LOCK]   = {err_lock && req.thread_id, err_lock && !req.thread_id};
  assign err_flags[`DCACHE_ERR_FLAG_MEM_FAULT] = {err_mem  && req.thread_id, err_mem  && !req.thread_id};

  // *****************************************************************************
  // FSM outputs based on current state
  // *****************************************************************************

  // Pipeline meta read valid one cycle after grant (pre-arbitration at ID stage)
  //     CLK     DOUT             DIN
  `FF   (clkgt,  meta_read_valid, meta_read_grant)

  // Computes actions based on state
  // -----------------------------------------------------------------------------
  logic  meta_write_valid_lock;  // Validetes writing the lock flags

  always_comb begin
    // Ready signals
    cache_op_ready  = (state == Cache_Op_Invalid) & !send_op_to_l2 & (cache_op_is_L1 || cache_op_is_NOP);

    // Decodes functionality
    is_set_way_op = (req.cmd == cache_op_Flush_SW)
                 || (req.cmd == cache_op_Evict_SW)
                 || (req.cmd == cache_op_Unlock_SW);
    is_lock_xx_op = (req.cmd == cache_op_Lock_SW)
                 || (req.cmd == cache_op_Lock_VA);

    // Meta read request
    meta_read_bid          = (state == Cache_Op_Meta_Read) && ~meta_read_valid; // Kill the bidding if already was granted in previous cycle
    meta_read_set          = is_set_way_op ? req_set : dcache_set_correct(req_addr[`DCACHE_SET_ADDR_RANGE],cfg_cache_mode,req.thread_id);
    meta_read_addr         = is_set_way_op ?  `ZX(`VA_SIZE,{req_addr[`PA_MSB:`DCACHE_SET_SZ+6],req_set,6'b0}) : req_addr;
    meta_read_addr_msb_err = is_set_way_op ?  1'b0 : req_addr_msb_err;
    meta_read_phys         = (req.cmd == cache_op_Lock_SW);
    meta_read_force_set    = is_set_way_op;
    meta_read_thread_id    = req.thread_id;
    meta_read_skip_pma     = (state == Cache_Op_Meta_Resp_Wait) && is_set_way_op;

    // Writeback Request
    writeback_valid         = (state == Cache_Op_Writeback_Req) & !abort_operation;
    writeback_req.tag       = meta_data_req_addr[`PA_TAG_EXT_RANGE];      // Extended TAG (overlaps with set)
    writeback_req.set       = is_set_way_op ? req_set : dcache_set_correct(meta_data_req_addr[`DCACHE_SET_ADDR_RANGE],cfg_cache_mode,req.thread_id); // Full SET, writeback will take care of overlapping with tag
    writeback_req.way_en    = meta_data_way_en;
    writeback_req.is_tbox   = 1'b0;
    writeback_req.mem_idx   = 1'b0;
    writeback_req.addr_low  = {`PA_NOTAG_EXT_BITS{1'b0}};

    // Meta Write
    meta_write_valid_next         = (state_next == Cache_Op_Meta_Write) && !abort_operation; // Early version to enable clocks in metadata array
    meta_write_valid              = (state == Cache_Op_Meta_Write) && !abort_operation;
    meta_write_valid_lock         = (state == Cache_Op_Meta_Write_Lock) && !abort_operation;
    meta_write_req.set            = is_set_way_op ? req_set :
                                                    dcache_set_correct(req_addr[`DCACHE_SET_ADDR_RANGE],cfg_cache_mode,req.thread_id);
    meta_write_req.way_en         = meta_data_way_en;
    meta_write_req.data.tag       = is_lock_xx_op ? phys_addr_tag :     // In lock operations the correct physical tag must be stored
                                                    meta_data_req_addr[`PA_TAG_EXT_RANGE];
    meta_write_req.data.coh_state = is_lock_xx_op ? State_Modified :
                                                    new_coh;            // In lock operations the state must be set to modified to get a hit when reading it

    // Busy signal
    busy =  (state != Cache_Op_Invalid); // Doing an operation

    // Per thread and ID ready signal (we do not have differentiated logic for them)
    for (integer th=0; th<`CORE_NR_THREADS; th++) begin
      for (integer id=0; id<=1; id++) begin
        if ((id_running == id[0]) && (req.thread_id == th[0])) begin
          th_id_complete[th][id] = !busy;
        end else begin
          th_id_complete[th][id] = 1'b1;
        end
      end
    end
  end

// *****************************************************************************
// Computes probe response based on state
// *****************************************************************************

always_comb
begin
    // Generates the new cacheline status
    
    // Invalid state
    if (meta_read_mux_state == State_Invalid)
        new_coh_next = State_Invalid; // As we are downgrading state, we stay in invalid
    // Shared state
    else if (meta_read_mux_state == State_Shared) begin
      // If going to invalid
      if (req.snoop_type == Snoop_to_Invalid)
        new_coh_next = State_Invalid;
      else 
        new_coh_next = State_Shared;
    end  else begin
      // Exclusive and Modified take same action        // If going to invalid
        if(req.snoop_type == Snoop_to_Invalid)
            new_coh_next = State_Invalid;
        // To shared (never happens as this is never configured to be shared)
        //else if(req.snoop_type == Snoop_to_Shared)
        //    new_coh_next = State_Shared;
        // To exclusive
        else
            new_coh_next = State_Exclusive;
    end
  end

  //     CLK    EN             DOUT      DIN
  `EN_FF(clkgt, meta_state_up, new_coh,  new_coh_next)


  // *****************************************************************************
  // Updates the lock state per line
  // *****************************************************************************

  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0]   hlock_state_next;     // Per cacheline hard lock state
  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0]   slock_state_next;     // Per cacheline soft lock state
  logic                                        lock_en;              // Update the lock state of the cachelines
  logic                                        lock_zero;            // Update the content of the cacheline

  logic [`DCACHE_SET_RANGE]                    lock_set;             // Set to be updated for locking
  logic [`DCACHE_WAY_RANGE]                    lock_way;             // Way to be updated for locking

  always_comb begin
    // Default update
    hlock_state_next = hlock_state;
    slock_state_next = slock_state;

    // Update lock state during normal operation
    lock_en = (meta_write_valid || meta_write_valid_lock) &&
                    ((req.cmd == cache_op_Lock_SW)  || (req.cmd == cache_op_Unlock_SW) ||
                     (req.cmd == cache_op_Lock_VA)  || (req.cmd == cache_op_Unlock_VA) ||
                     (req.cmd == cache_op_Evict_VA) || (req.cmd == cache_op_Evict_SW));
    lock_zero = (meta_write_valid || meta_write_valid_lock) &&
                    ((req.cmd == cache_op_Lock_SW) || (req.cmd == cache_op_Lock_VA));

    // Update state when activating or deactivating Scratchpad
    lock_en |= (cfg_clear_all | cfg_clear_low | cfg_alloc_scp);

    // Lock bits update
    if (lock_en) begin
      if (cfg_alloc_scp) begin
        for (integer i=0; i<`DCACHE_SETS_FOR_SCP; i++) begin
          hlock_state_next[i] = {(`DCACHE_WAYS){1'b1}};
          slock_state_next[i] = {(`DCACHE_WAYS){1'b1}};
        end
      end else if (cfg_clear_low) begin
        for (integer i=0; i<`DCACHE_SETS_SCP_SPLIT; i++) begin
          hlock_state_next[i] = {(`DCACHE_WAYS){1'b0}};
          slock_state_next[i] = {(`DCACHE_WAYS){1'b0}};
        end
      end else if (cfg_clear_all) begin
        hlock_state_next = {(`DCACHE_SETS*`DCACHE_WAYS){1'b0}};
        slock_state_next = {(`DCACHE_SETS*`DCACHE_WAYS){1'b0}};
      end else if (req.cmd == cache_op_Lock_SW) begin
        hlock_state_next[lock_set][lock_way] = 1'b1;
        slock_state_next[lock_set][lock_way] = 1'b0;
      end else if (req.cmd == cache_op_Unlock_SW) begin
        hlock_state_next[lock_set][lock_way] = 1'b0;
        slock_state_next[lock_set][lock_way] = 1'b0;
      end else if (req.cmd == cache_op_Evict_SW) begin
        slock_state_next[lock_set][lock_way] = 1'b0;
      end else if (req.cmd == cache_op_Lock_VA) begin
        slock_state_next[lock_set][lock_way] = !hlock_state[lock_set][lock_way];
      end else if (req.cmd == cache_op_Unlock_VA) begin
        slock_state_next[lock_set][lock_way] = 1'b0;
      end else if (req.cmd == cache_op_Evict_VA) begin
        slock_state_next[lock_set][lock_way] = 1'b0;
      end
    end

    // Operation of MH and Cache Ops on the same set will never happen,
    // so the bits can be updated in parallel with no risk
    if (mh_clear_slock_valid) begin
      slock_state_next[mh_clear_slock_set][mh_clear_slock_way] = 1'b0;
    end

  end

  always_comb begin
    // Gets the set and way over which the lock operation must happen
    lock_way = dcache_way_encode(meta_data_way_en);
    lock_set = meta_write_req.set;
  end

  // Simple state machine to write zero into the locked
  // Ensures that after locking a line the read value will be zero
  always_comb begin
    da_clear_en  = lock_zero | clear_line_on;
    da_clear_set = lock_set;
    da_clear_way = lock_way;
    da_clear_val = clear_line_on;

    da_clear_idx_next  = da_clear_val & da_clear_rdy ? ~da_clear_idx : da_clear_idx;
    clear_line_on_next = lock_zero || (clear_line_on & !(da_clear_val & da_clear_rdy & da_clear_idx));
  end

  assign clear_line = da_clear_en;

  `RST_FF (clkgt, reset, clear_line_on, clear_line_on_next, 1'b0)
  `RST_FF (clkgt, reset, da_clear_idx,  da_clear_idx_next,  1'b0)

  // Update l ock info bits
  logic hlock_up;
  logic slock_up;

  assign hlock_up = lock_en;
  assign slock_up = lock_en | mh_clear_slock_valid;

  `RST_EN_FF(clock, reset, hlock_up, hlock_state,      hlock_state_next, {`DCACHE_SETS*`DCACHE_WAYS{1'b0}})
  `RST_EN_FF(clock, reset, slock_up, slock_state,      slock_state_next, {`DCACHE_SETS*`DCACHE_WAYS{1'b0}})

  // *****************************************************************************
  // Prefetch request to pipeline
  // *****************************************************************************

  // Pipeline s0 valid one cycle after grant (pre-arbitration at ID stage)
  //     CLK     DOUT            DIN
  `FF   (clkgt,  s0_co_pf_valid, id_co_pf_grant)

  always_comb begin
    // Bid to make a new request to pipeline unless replay queue is full
    // De-assert bid next cycle after granted
    id_co_pf_bid = (state == Cache_Op_L1_Prefetch) & ~id_rq_full & ~s0_co_pf_valid;

    // Fill necessary fields of core request struct to push a prefetch instruction
    s0_co_pf_req            = dcache_req'(0);

    s0_co_pf_req.addr           = {req_addr[`VA_RANGE_CA],6'b0};
    s0_co_pf_req.addr_msb_err   = req_addr_msb_err;
    s0_co_pf_req.cmd            = dcache_cmd_PFR;
    s0_co_pf_req.typ            = dcache_type_D;
    s0_co_pf_req.cacheable      = 1'b1;
    s0_co_pf_req.cache_op       = 1'b1;
    s0_co_pf_req.dest.thread_id = req.thread_id;
    s0_co_pf_req.vm_sel         = `DCACHE_VM_SEL_CO;

  end // always_comb

  ////////////////////////////////////////////////////////////////////////////////
  // Clock gating
  ////////////////////////////////////////////////////////////////////////////////

  // Activate clock once a new request arrives and until everything is completed
  assign clkgt_en = cache_op_accepted | busy | !(&th_id_complete) | reset | err_ffs;


  et_clk_gate cgate_co(
    .enclk ( clkgt                         ),
    .en    ( clkgt_en | chicken_bit_dcache ),
    .clk   ( clock                         ),
    .te    ( 1'b0                          )  // DFT has to connect test signals here
  );


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Debug signals
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  assign csr_debug_signals = state;

  /* RTLMIN-5542
  assign csr_debug_signals = {
        // csr_cou0
        req_count,
        id_running,
        req_tmask_bits,
        send_op_to_l2,
        req,
        state,
        // csr_cou1
        hlock_state
        };
  */

  assign sm_match_debug_signals = `ZX(`DCACHE_DBG_SM_MATCH_BITS,
    {
      meta_read_valid,
      err_lock,
      err_mem,
      term_loop,
      state_up,
      abort_operation,
      cache_op_accepted
    });

  assign sm_filter_debug_signals = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
    {
      err_lock,
      err_mem,
      term_loop,
      state_up,
      abort_operation,
      cache_op_accepted,
      req_addr,
      req,
      state
    });

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(3)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(1)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
    {
      err_lock,
      err_mem,
      term_loop,
      meta_read_mux_match,
      meta_read_mux_tag,
      meta_read_mux_way,
      meta_read_mux_state,
      meta_read_addr,
      meta_read_resp_way_en,
      phys_addr_tag
    });

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(0)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
    {
      cache_op_is_L1,
      cache_op_is_NOP,
      req_count,
      req_stride,
      req_set_way,
      req_addr,
      req
    });

endmodule

