// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_replay_queue (
  // System signals
  input logic                               clock,
  input logic                               reset,
  // Control
  output logic                              full,
  output logic                              half_full,
  output logic [`CORE_NR_THREADS-1:0]       empty,
  // Alloc port
  input  logic                              alloc_req_pre,
  input  logic                              alloc_req_val,
  input  logic                              alloc_gsc,
  // Write port
  input  logic                              push_req,
  input  logic                              push_req_prev,
  input  dcache_req                         req,
  input  logic                              cacheable,
  input  logic                              req_buf_alloc,
  input  logic [`DCACHE_BUFFER_ID_RANGE]    req_buf_entry,
  input  dcache_replay_conf                 req_conf_type,
  `ifdef DCACHE_REPORT_PC
  input  logic [`PC_RANGE_EXT]              req_pc,
  `endif
  input  logic                              misaligned_1st,
  input  logic                              uc_load_1st,
  input  logic                              uc_load_ok,
  input  logic                              miss_fill_ok,
  output logic                              rq_conflict,
  output logic                              rq_conflict_to_co,
  // Re-arm port (replay again a replayed instruction)
  input  logic                              rearm,
  input  logic                              rearm_prev,
  input  logic [`DCACHE_REPLAYQ_ADDR_RANGE] rearm_entry,
  // Dealloc port
  input  logic                              dealloc,
  input  logic [`DCACHE_REPLAYQ_ADDR_RANGE] dealloc_entry,
  // Wake up signals
  input  logic [`DCACHE_MH_FILE_SIZE-1:0]   miss_available,
  input  logic [`DCACHE_MH_FILE_SIZE-1:0]   miss_fill_error,
  input  logic                              uc_load_fill,
  input  logic [`DCACHE_BUFFER_ID_RANGE]    uc_load_fill_id,
  input  logic                              uc_load_fill_ok,
  input  logic                              vpu_rf_in_use,
  // Read port
  output logic                              replay_bid,
  input  logic                              replay_grant,
  output logic                              replay_valid,
  output dcache_replay_req                  replay_req,
  output logic [`DCACHE_REPLAYQ_ADDR_RANGE] replay_entry,
  output logic [`DCACHE_REPLAYQ_AGE_ID]     replay_age,
  output logic                              replay_uc_ok,
  output logic                              replay_miss_ok,
  // Scoreboard to core to know which registers are not ready
  output dcache_minion_scoreboard           scoreboard,
  // pulse when a FP entry is deallocated from sboard
  output logic [`CORE_NR_THREADS-1:0]       fp_sb_dealloc,
  output logic [`CORE_NR_THREADS-1:0]       int_sb_dealloc
);

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // This module stores all the instructions that had any issue and need to be
  // replayed through the dcache pipeline. When instructions come in the
  // pipeline they have to allocate an entry. If there's no entry available,
  // a stall is sent to core in ID stage (to prevent later nacks).
  // Information:
  // We need to keep track of which age id entries are available. The reason is
  // that instruction get pushed in the replayQ in program order, but dealloc
  // from it OoO. This means that we could have an instruction in the replayQ
  // for a lot of cycles and in the meantime push more instructions and pop them
  // than the range of age id. This could end up in a situation where the same
  // age id is reused and confuse the is_older function.
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  logic s0_alloc_req_pre; // Request was allocated for S0 transaction
  logic s0_alloc_req;     // Request was allocated for S0 transaction
  logic s1_alloc_req;     // Request was allocated for S1 transaction
  logic s2_alloc_req;     // Request was allocated for S2 transaction
  logic s3_alloc_req;     // Request was allocated for S3 transaction
  logic s0_alloc_gsc;     // Request allocated for S0 transaction is Gather/Scatter
  logic s1_alloc_gsc;     // Request allocated for S1 transaction is Gather/Scatter
  logic s2_alloc_gsc;     // Request allocated for S2 transaction is Gather/Scatter
  logic alloc_clk_en;     // Clock enable for the alloc registers

  // Validated request allocation
  assign s0_alloc_req = s0_alloc_req_pre && alloc_req_val;

  //         CLK    RST    EN            DOUT              DIN                           DEF
  `RST_EN_FF(clock, reset, alloc_clk_en, s0_alloc_req_pre, alloc_req_pre,                1'b0)
  `EN_FF    (clock,        alloc_clk_en, s1_alloc_req,     s0_alloc_req                      )
  `EN_FF    (clock,        alloc_clk_en, s2_alloc_req,     s1_alloc_req                      )
  `EN_FF    (clock,        alloc_clk_en, s3_alloc_req,     s2_alloc_req && s2_alloc_gsc      )
  `RST_EN_FF(clock, reset, alloc_clk_en, s0_alloc_gsc,     alloc_gsc,                    1'b0)
  `EN_FF    (clock,        alloc_clk_en, s1_alloc_gsc,     s0_alloc_gsc                      )
  `EN_FF    (clock,        alloc_clk_en, s2_alloc_gsc,     s1_alloc_gsc                      )

  ////////////////////////////////////////////////////////////////////////////////
  // We need to know in ID core stage if there is an entry available to alloc
  // and reserve it. So we have a counter for this purpose. When an entry is
  // actually written, a free list of available entries is used to know which
  // one to use (because deallocation is out of order).
  ////////////////////////////////////////////////////////////////////////////////

  logic [$clog2(`DCACHE_REPLAYQ_SIZE):0] entry_count_available, entry_count_available_next; // Counter of available entries at alloc time
  logic [`DCACHE_REPLAYQ_RANGE]          entry_mask_available,  entry_mask_available_next;  // Mask of available entries at write time
  logic                                  alloc_req_pre_applied, alloc_req_pre_applied_next; // Flag indicating that a resources was pre-allocated
  logic [`DCACHE_REPLAYQ_AGE_ID_MASK]    age_id_mask_available, age_id_mask_available_next; // Which age id entries are available
  logic [`DCACHE_REPLAYQ_ADDR_RANGE]     push_entry;                                        // Which entry to push data
  logic [`DCACHE_REPLAYQ_AGE_ID]         push_age, push_age_next;                           // Next age to assign
  logic [`DCACHE_REPLAYQ_AGE_ID]         push_age_check_available;                          // Age ID to be assigned by ID instruction. Check if available

  //         CLK    RST    EN            OUT                    DIN                         DEF
  `RST_EN_FF(clock, reset, alloc_clk_en, entry_count_available, entry_count_available_next,  `DCACHE_REPLAYQ_SIZE)
  `RST_EN_FF(clock, reset, alloc_clk_en, entry_mask_available,  entry_mask_available_next,  {`DCACHE_REPLAYQ_SIZE{1'b1}})
  `RST_EN_FF(clock, reset, alloc_clk_en, age_id_mask_available, age_id_mask_available_next, {`DCACHE_REPLAYQ_AGE_ID_MASK_SIZE{1'b1}})
  `EN_FF    (clock,        alloc_clk_en, alloc_req_pre_applied, alloc_req_pre_applied_next)

  always_comb begin
    // Computes the age id that could be used by ID instruction (moves the
    // pointer forward counting the allocs between ID and S2/S3)
    push_age_check_available = push_age + s0_alloc_req + s1_alloc_req + s2_alloc_req + s3_alloc_req;

    // Sends full and gets which entry to write to
    push_entry = 0;
    for (integer unsigned i = 1; i < `DCACHE_REPLAYQ_SIZE; i++) begin
      if (entry_mask_available[i]) begin
        push_entry = i;
      end
    end

    // In case of doing a rearm (using req.replay as it is an earlier signal), override push entry with the rearm entry
    if (req.replay) begin
      push_entry = rearm_entry;
    end

    // Full if no entry available in the replayQ or if the age id is not available
    full      = (entry_count_available == 0)                      || !age_id_mask_available[push_age_check_available[`DCACHE_REPLAYQ_AGE_ID_CMP]];
    // Full if half or less entries available in the replayQ or if the age id is not available
    half_full = (entry_count_available <= `DCACHE_REPLAYQ_SIZE/2) || !age_id_mask_available[push_age_check_available[`DCACHE_REPLAYQ_AGE_ID_CMP]];

    // Updates counter
    entry_count_available_next = entry_count_available;

    // Pre-allocate resource if possible
    alloc_req_pre_applied_next = 1'b0;
    if (alloc_req_pre) begin
      if (entry_count_available_next != 0) begin
        alloc_req_pre_applied_next = 1'b1;
        entry_count_available_next = entry_count_available_next - 'b1;
      end
    end

    // If allocated entry but not used, free it
    // Check if a pre-allocated resource has to be de-allocated
    if (alloc_req_pre_applied && !s0_alloc_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Gathers/Scatters use the entry one cycle later than other requests
    // Only one instruction can be pushed at a time, one of the counted entries must be freed
    if ((s2_alloc_req && !s2_alloc_gsc) && !push_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if (s3_alloc_req && !push_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if ((s2_alloc_req && !s2_alloc_gsc) && s3_alloc_req && push_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Used entry is deallocated
    if (dealloc) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Updates mask
    entry_mask_available_next = entry_mask_available;
    if (push_req) begin
      entry_mask_available_next[push_entry] = 1'b0;
    end
    if (dealloc) begin
      entry_mask_available_next[dealloc_entry] = 1'b1;
    end

    // Updates age Id
    age_id_mask_available_next = age_id_mask_available;
    if (push_req) begin
      age_id_mask_available_next[push_age[`DCACHE_REPLAYQ_AGE_ID_CMP]] = 1'b0;
    end
    if (dealloc) begin
      age_id_mask_available_next[req.replay_age[`DCACHE_REPLAYQ_AGE_ID_CMP]] = 1'b1;
    end

    alloc_clk_en = reset || alloc_req_pre || s0_alloc_req_pre || s1_alloc_req || s2_alloc_req || s3_alloc_req || dealloc || push_req;
  end


  ////////////////////////////////////////////////////////////////////////////////
  // This is the array with the actual replay queue storage
  ////////////////////////////////////////////////////////////////////////////////

  dcache_replay_req_push_t  [`DCACHE_REPLAYQ_RANGE] replayq_push;      // Storage of the replay queue that is only updated in the first push
  dcache_replay_req_rearm_t [`DCACHE_REPLAYQ_RANGE] replayq_rearm;     // Storage of the replay queue that is updated in the first push and upon a rearm
  dcache_replay_req         [`DCACHE_REPLAYQ_RANGE] replayq;           // Final storage of the replayq
  dcache_replay_req_push_t                          req_written_push;  // Request written to replayQ after some modifications
  dcache_replay_req_rearm_t                         req_written_rearm;

  // Latched version of push_req_prev
  logic push_req_prev_p2;
  `LATCH_P2(clock, push_req_prev_p2, push_req_prev)

  rf_latch_1r_1w_par #(
    .WIDTH   ( $bits(dcache_replay_req_push_t) ),
    .ENTRIES ( `DCACHE_REPLAYQ_SIZE ),
    .LEVEL2_CLK_GATE (1)
  ) rf_replayq_push (
    // System signals
    .clk             ( clock            ),
    .test_en         ( 1'b0             ),
    // Output port
    .rd_data_a       ( replayq_push     ),
    // Write port
    .wr_addr_a       ( push_entry       ),
    .wr_data_a       ( req_written_push ),
    .wr_en_a         ( push_req         ),
    .wr_data_a_en_1p ( push_req_prev_p2 )
  );

  // Latched version of rearm_prev
  logic rearm_prev_p2;
  `LATCH_P2(clock, rearm_prev_p2, rearm_prev)

  rf_latch_1r_1w_par #(
    .WIDTH   ( $bits(dcache_replay_req_rearm_t) ),
    .ENTRIES ( `DCACHE_REPLAYQ_SIZE ),
    .LEVEL2_CLK_GATE (1)
  ) rf_replayq_rearm (
    // System signals
    .clk             ( clock                             ),
    .test_en         ( 1'b0                              ),
    // Output port
    .rd_data_a       ( replayq_rearm                     ),
    // Write port
    .wr_addr_a       ( push_entry                        ),
    .wr_data_a       ( req_written_rearm                 ),
    .wr_en_a         ( push_req || rearm                 ),
    .wr_data_a_en_1p ( push_req_prev_p2 || rearm_prev_p2 )
  );

  // Assembles the final replayQ values from the push only and rearm info
  always_comb begin
    for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
      // Push only written info is baseline
      replayq[i].dest           = replayq_push[i].dest;
      replayq[i].cmd            = replayq_push[i].cmd;
      replayq[i].typ            = replayq_push[i].typ;
      replayq[i].gsc_cnt        = replayq_push[i].gsc_cnt;
      replayq[i].ps_mask        = replayq_push[i].ps_mask;
      replayq[i].buf_alloc      = replayq_push[i].buf_alloc;
      replayq[i].buf_id         = replayq_push[i].buf_id;
      replayq[i].cache_op       = replayq_push[i].cache_op;
      replayq[i].gsc32_idx      = replayq_push[i].gsc32_idx;
      replayq[i].vm_sel         = replayq_push[i].vm_sel;
      replayq[i].mem_global     = replayq_push[i].mem_global;
      replayq[i].prv            = replayq_push[i].prv;
       
      // Complete info with rearm fields
      replayq[i].addr           = replayq_rearm[i].addr;
      replayq[i].misaligned     = replayq_rearm[i].misaligned;
      replayq[i].misaligned_2nd = replayq_rearm[i].misaligned_2nd;
      replayq[i].uc_load_2nd    = replayq_rearm[i].uc_load_2nd;
      replayq[i].phys           = replayq_rearm[i].phys;
      replayq[i].cacheable      = replayq_rearm[i].cacheable;
      `ifdef DCACHE_REPORT_PC
      replayq[i].pc             = replayq_rearm[i].pc;
      `endif
    end
  end

  always_comb begin
    // By default request going to replayQ is the original req
    req_written_push.dest            = req.dest;
    req_written_push.cmd             = req.cmd;
    req_written_push.typ             = req.typ;
    req_written_push.gsc_cnt         = req.gsc_cnt;
    req_written_push.ps_mask         = req.ps_mask;
    req_written_push.buf_alloc       = req_buf_alloc;
    req_written_push.buf_id          = req_buf_entry;
    req_written_push.cache_op        = req.cache_op;
    req_written_push.gsc32_idx       = req.gsc32_idx;
    req_written_push.vm_sel          = req.vm_sel;
    req_written_push.mem_global      = req.mem_global;
    req_written_push.prv             = req.prv;   
    req_written_rearm.addr           = req.addr;
    req_written_rearm.misaligned     = req.misaligned;
    req_written_rearm.misaligned_2nd = req.misaligned_2nd;
    req_written_rearm.uc_load_2nd    = req.uc_load_2nd;
    req_written_rearm.phys           = req.phys;
    req_written_rearm.cacheable      = cacheable;
    `ifdef DCACHE_REPORT_PC
    req_written_rearm.pc             = req_pc;
    `endif

    // But... if we are hitting the 1st pass of a misaligned access, we need
    // to update the info to do the second pass!
    if (misaligned_1st) begin
      req_written_rearm.misaligned = 1'b0;

      // Transaction is now doing the 2nd misaligned pass
      req_written_rearm.misaligned_2nd = 1'b1;

      // We have to move to the next block (which might be a different
      // cacheline (case 1) or not (case 2))
      req_written_rearm.addr[`PA_SIZE-1:$clog2(`DCACHE_DATA_SIZE/8)] += 'b1;
    end

    // If 1st pass of UC load is done (sending the data request to
    // L2), then move to 2nd pass (replay instruction and read data)
    if (uc_load_1st) begin
      req_written_rearm.uc_load_2nd = 1'b1;
    end

  end


  ////////////////////////////////////////////////////////////////////////////////
  // This is the arbiter that selects which entry to replay
  ////////////////////////////////////////////////////////////////////////////////

  // Structs that holds information to know which replay queue entries are ready
  typedef struct packed {
    logic                          pending;      // Entry is pending to be replayed, but might not be ready
    logic                          store;        // Entry is an store
    logic [`DCACHE_REPLAYQ_AGE_ID] age;          // Age ID used to know which entries are younger and older
    dcache_replay_conf             reason;       // The reason why it is not ready
    logic                          wait_uc_load; // Entry is waiting for a UC load data to come back
    logic                          uc_load_ok;   // UC load completed correctly
    logic                          miss_fill_ok; // Miss Fill completed correctly
  } replayq_ready_info_t;

  replayq_ready_info_t [`DCACHE_REPLAYQ_RANGE]             replayq_ready_info;      // Info to compute the ready for all the entries
  replayq_ready_info_t [`DCACHE_REPLAYQ_RANGE]             replayq_ready_info_next;
  logic [`DCACHE_BUFFER_RANGE][`DCACHE_REPLAYQ_ADDR_RANGE] uc_load_id;              // Maps from miss ID to replayQ ID to know which UC load the fill is waking up
  logic [`DCACHE_BUFFER_RANGE][`DCACHE_REPLAYQ_ADDR_RANGE] uc_load_id_next;
  logic [`DCACHE_REPLAYQ_RANGE][`DCACHE_REPLAYQ_RANGE]     rq_conflict_matrix;      // Matrix of conflicts between replayQ entries
  logic [`DCACHE_REPLAYQ_RANGE]                            entry_ready;             // Mask of ready entries to be replayed
  logic [`DCACHE_REPLAYQ_ADDR_RANGE]                       replay_entry_next;       // Replay entry for pre-arbitration
  logic [`DCACHE_REPLAYQ_ADDR_RANGE]                       replay_entry_prev;       // Replay entry for previous cycle
  logic [`CORE_NR_THREADS-1:0]                             empty_next;              // The replayQ will be empty next cycle
  logic                                                    replay_valid_prev;       // An instruction was replayed previous cycle
  logic                                                    req_is_write;            // Request is a write command
  logic                                                    arb_clk_en;              // Clock enable for all the arbiter related logic

  //         CLK    RST    EN          DOUT                DIN                      DEF
  `RST_EN_FF(clock, reset, arb_clk_en, replay_valid,       replay_grant,            1'b0)
  `RST_EN_FF(clock, reset, arb_clk_en, replay_valid_prev,  replay_valid,            1'b0)
  `RST_EN_FF(clock, reset, arb_clk_en, push_age,           push_age_next,           'b0)
  `RST_EN_FF(clock, reset, arb_clk_en, empty,              empty_next,              2'b11)
  `EN_FF    (clock,        arb_clk_en, replayq_ready_info, replayq_ready_info_next)
  `EN_FF    (clock,        arb_clk_en, replay_entry,       replay_entry_next)
  `EN_FF    (clock,        arb_clk_en, replay_entry_prev,  replay_entry)
  `EN_FF    (clock,        arb_clk_en, uc_load_id,         uc_load_id_next)

  // Speculative push request to enable clock without dependence on all the logic associated to a push
  logic                                                    push_req_spec;

  //         CLK    RST    EN          DOUT                DIN                      DEF
  `RST_FF   (clock, reset,             push_req_spec,      push_req_prev,           1'b0)


  always_comb begin
    // Compute ready signal for all the entries
    for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
      entry_ready[i] = replayq_ready_info[i].pending && !entry_mask_available[i] // Entry is pending
                    && !(|rq_conflict_matrix[i])                                 // No conflict with other replayQ entries
                    && !replayq_ready_info[i].wait_uc_load                       // Waiting for data to come back
                    && (!replayq_ready_info[i].reason.miss_alloc)                // If waiting for miss handler available, to have one available
                    && (&(~replayq_ready_info[i].reason.miss_assigned))          // If had a miss, waits for miss handler to be available so the fill is finished.
                    && !replayq_ready_info[i].reason.vpu_rf_write;               // If VPU was busy writing register file, wait for the resource to become available
    end

    // Queue will be empty when all entries are available and no new entry being
    // pushed
    for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
      empty_next[i] = 1'b1;
      for (integer j = 0; j < `DCACHE_REPLAYQ_SIZE; j++) begin
        empty_next[i] &= entry_mask_available[j] || (replayq[j].dest.thread_id != i);
      end
      empty_next[i] &= !(push_req && (req.dest.thread_id == i));
    end

    replayq_ready_info_next = replayq_ready_info;
    push_age_next = push_age;

    // Clear the dependencies with the miss handlers once they are available.
    // If we don't do that, a miss depending on handler 0 that doesn't get
    // issued after the miss gets served and is allocated again by another
    // miss, will wait for the second miss.
    // WARNING: this must go before the push_req update "if", otherwise we
    // hit the following situation:
    //   - Instruction misses, miss handler state changes next cycle
    //   - We first set the miss_assigned to 1 through push_req "if"
    //   - But as miss handler is still available this cycle, we then clear
    //     the miss_assigned bit. So this must go before!
    // Same for rearm "if".
    for (integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (miss_available[i]) begin
        for (integer j = 0; j < `DCACHE_REPLAYQ_SIZE; j++) begin
          if (replayq_ready_info_next[j].reason.miss_assigned[i] ||
             (replayq_ready_info_next[j].reason.miss_assigned == `DCACHE_MH_FILE_SIZE'b0)) begin
            replayq_ready_info_next[j].reason.miss_alloc = 1'b0;
          end
          replayq_ready_info_next[j].reason.miss_assigned[i] = 1'b0;
        end
      end
    end

    // In case of errors hapening during a miss request, instructions that get
    // replayed need to be thrown away. Flag the error here
    for (integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (miss_fill_error[i]) begin
        for (integer j = 0; j < `DCACHE_REPLAYQ_SIZE; j++) begin
          if (replayq_ready_info_next[j].reason.miss_accepted[i]) begin
            replayq_ready_info_next[j].miss_fill_ok = 1'b0;
          end
        end
      end
    end

    // Clear the dependencies with VPU RF access
    if (!vpu_rf_in_use) begin
      for (integer j = 0; j < `DCACHE_REPLAYQ_SIZE; j++) begin
        replayq_ready_info_next[j].reason.vpu_rf_write = 1'b0;
      end
    end

    // Clear the UC load wait signal
    if (uc_load_fill) begin
      replayq_ready_info_next[uc_load_id[uc_load_fill_id]].wait_uc_load = 1'b0;
      replayq_ready_info_next[uc_load_id[uc_load_fill_id]].uc_load_ok   = uc_load_fill_ok;
    end

    // Decodes some info from request
    req_is_write   = dcache_cmd_is_write(req.cmd);

    // This sets the fields that are updated once per entry in replayQ
    if (push_req) begin
      replayq_ready_info_next[push_entry].store   = req_is_write;
      replayq_ready_info_next[push_entry].age     = push_age;
      push_age_next                               = push_age + 'b1;
    end

    // Fields updated upon push and rearm
    if (push_req || rearm) begin
      replayq_ready_info_next[push_entry].pending      = 1'b1;
      replayq_ready_info_next[push_entry].reason       = req_conf_type;
      replayq_ready_info_next[push_entry].wait_uc_load = uc_load_1st;
      replayq_ready_info_next[push_entry].uc_load_ok   = uc_load_ok;
      replayq_ready_info_next[push_entry].miss_fill_ok = miss_fill_ok;
    end

    // Upon grant, make element not ready
    if (replay_grant) begin
      replayq_ready_info_next[replay_entry_next].pending = 1'b0;
    end

    // Maps from buffer array ID to replayQ entry in order to wake up entries
    // upon fill from L2 with UC load data
    uc_load_id_next = uc_load_id;
    if (push_req && req_buf_alloc) begin
      uc_load_id_next[req_buf_entry] = push_entry;
    end

    // Clock enable for all the arbitration logic
    arb_clk_en = reset || !(&empty) || push_req_spec || replay_valid || replay_valid_prev;
  end

  // An important feature is how the priority between entries is updated. We
  // update the priority when an entry is pushed in the replayQ. It means that
  // it will have lower prio than the previous entries, and therefore we will
  // replay in program order when several entries get ready. Also, if an
  // instruction gets re-armed, it will keep the original prio and we will still
  // replay the instructions in program order.

  queue_arb_lru #(
    .NUM_CLIENTS ( `DCACHE_REPLAYQ_SIZE )
  ) arb (
    // System signals
    .clock       ( clock                ),
    .reset       ( reset                ),
    // Update priority
    .prio_up     ( push_req             ),
    .prio_entry  ( push_entry           ),
    // Bidding
    .bid         ( entry_ready          ),
    .winner      ( replay_entry_next    )
  );

  always_comb begin
    // Bids to replay an instruction when one is ready
    replay_bid = |entry_ready;
    replay_req = replayq[replay_entry];
    replay_age = replayq_ready_info[replay_entry].age;
    replay_uc_ok = replayq_ready_info[replay_entry].uc_load_ok;
    replay_miss_ok = replayq_ready_info[replay_entry].miss_fill_ok;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // This logic computes the conflicts between entries in the replayQ and the
  // incoming entry in the S2 stage. An incoming entry might have a conflict
  // with older pending operations and can create a conflict with younger
  // pending operations. When an operation is replayed, the conflict bits with
  // other instructions are cleared.
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_REPLAYQ_RANGE] rq_to_s2_conflict_array; // Instructions in replayQ create a conflict with the S2 instruction
  logic [`DCACHE_REPLAYQ_RANGE] s2_to_rq_conflict_array; // S2 instruction creates a conflict with the replayQ instructions

  always_comb begin
    rq_to_s2_conflict_array = `DCACHE_REPLAYQ_SIZE'b0;
    s2_to_rq_conflict_array = `DCACHE_REPLAYQ_SIZE'b0;
    rq_conflict             = 1'b0;
    rq_conflict_to_co       = 1'b0;

    // For all the queue entries
    for(integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin : s2_conflict_for
      logic rq_entry_is_older;       // Entry in replayQ is older than operation in S2
      logic rq_entry_is_same_thread; // Entry in replayQ has same thread id than operation in S2 (not used)
      logic rq_entry_pending;        // Entry is pending

      // Gets if the entry in replayQ is older and same thread
      rq_entry_is_older       = !req.replay || dcache_replayq_entry_is_older(replayq_ready_info[i].age, req.replay_age);
      rq_entry_is_same_thread = replayq[i].dest.thread_id == req.dest.thread_id;
      rq_entry_pending        = !entry_mask_available[i] && replayq_ready_info[i].pending;

      // If the entry is pending or is being replayed this cycle or was
      // replayed previous cycle:
      // S2 instruction is the latest stage of nacking an entry and
      // deallocating the replayQ entry. If an entry is pending, it means
      // that it will be executed later than the instruction in S2.
      // Therefore, if there's a conflict it has to be rearmed/pushed
      if(
             rq_entry_pending                                // Pending
          || (replay_valid      && (replay_entry      == i)) // Replayed this cycle (S0 stage)
          || (replay_valid_prev && (replay_entry_prev == i)) // Replayed previous cycle (S1 stage)
      ) begin
          // Need to check for problems in case that any of the two is a store
          if (req_is_write || replayq_ready_info[i].store) begin
              // Check for addr conflict (only subset of bits) or misaligned
              if((req.addr[`DCACHE_REPLAYQ_ADDR_CONF_BITS] == replayq[i].addr[`DCACHE_REPLAYQ_ADDR_CONF_BITS])
                  || req.misaligned || replayq[i].misaligned || replayq[i].misaligned_2nd || !replayq[i].cacheable && rq_entry_is_same_thread) begin
                  // Finally, is the entry older than the S2 instruction, it
                  // creates a conflict
                  if(rq_entry_is_older) begin
                      rq_to_s2_conflict_array[i] = rq_entry_pending; // Only set dependency for entries pending and not already replayed,
                                                                     // otherwise we can deadlock because entry might not be rearmed and
                                                                     // conflict bit will never be cleared
                      rq_conflict                = 1'b1;
                  // Otherwise, the S2 entry creates a conflict to the
                  // replayQ
                  end else begin
                      s2_to_rq_conflict_array[i] = 1'b1;
                  end
              end
          end

        // Simultaneously, check if any of the instructions may have some kind of conflict
        // with the ongoing cache operations. At this moment only misaligned passes that require
        // 2 passes and are in second pass will create a conflict to avoid that lines are removed
        // from cache. Logic will be simplified to avoid long timing paths, but if performance
        rq_conflict_to_co |= replayq[i].misaligned_2nd;
      end
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Stores the conflict between the different entries in the replayQ. When
  // a new instruction is pushed or rearmed, its dependencies are updated for
  // all the entries. Also, the dependency of the other replay entries against
  // the pushed/rearmed entry is recomputed.
  // When an instruction is replayed, the dependencies against the other entries
  // are cleared.
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_REPLAYQ_RANGE][`DCACHE_REPLAYQ_RANGE] rq_conflict_matrix_next; // Matrix of conflicts between replayQ entries
  logic                                                rq_conflict_matrix_up;   // Update the replayQ conflict matrix

  //     CLK    EN                     DOUT                DIN
  `EN_FF(clock, rq_conflict_matrix_up, rq_conflict_matrix, rq_conflict_matrix_next)

  always_comb begin
    rq_conflict_matrix_next = rq_conflict_matrix;
    rq_conflict_matrix_up   = push_req || rearm || replay_grant;

    // Push or rearming an entry
    if (push_req || rearm) begin
      // Write the new conflicts to this instruction
      rq_conflict_matrix_next[push_entry] = rq_to_s2_conflict_array;
      // Write conflicts from the new instrction to other entries
      for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
        rq_conflict_matrix_next[i][push_entry] = s2_to_rq_conflict_array[i];
      end
    end

    // Grant, clear dependencies
    if (replay_grant) begin
      for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
        rq_conflict_matrix_next[i][replay_entry_next] = 1'b0;
      end
    end

    // Diagonal values are not needed. Just making sure synthesis removes then
    for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
      rq_conflict_matrix_next[i][i] = 1'b0;
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Sends the list of inflight dest registers
  ////////////////////////////////////////////////////////////////////////////////
  logic [`CORE_NR_THREADS-1:0]             fp_sb_dealloc_next;
  logic [`CORE_NR_THREADS-1:0]             int_sb_dealloc_next;
  always_comb begin
    for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++) begin
      scoreboard.valid[i] = !entry_mask_available[i] && dcache_cmd_is_read(replayq[i].cmd); // Valid and generates data
      scoreboard.dest[i]  = replayq[i].dest;
    end

    for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
      fp_sb_dealloc_next[i]  = dealloc && replayq[dealloc_entry].dest.fp == 1'b1 && replayq[dealloc_entry].dest.thread_id == i;
      int_sb_dealloc_next[i] = dealloc && replayq[dealloc_entry].dest.fp == 1'b0 && replayq[dealloc_entry].dest.thread_id == i;
    end
  end

  `RST_FF(clock, reset, fp_sb_dealloc,  fp_sb_dealloc_next, 2'b0)
  `RST_FF(clock, reset, int_sb_dealloc, int_sb_dealloc_next, 2'b0)


endmodule
