// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_miss_handler_unit (
  // System signals
  input  logic                            clock,
  input  logic                            reset,
  input  logic [`MIN_PER_N_R]             neigh_min_id,
  // Miss request from dcache pipeline
  output logic                            req_ready,
  input  logic                            req_valid,
  input  dcache_mh_req                    req_info,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] req_assigned,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] req_accepted,
  output logic                            can_accept_uc,
  output logic                            wait_l2_to_ba_resp,
  input  logic                            inflight_reduce,
  output logic                            going_to_uc_store,
  // Mem write request bus to evict
  input  logic                            l2_evict_ready,
  output logic                            l2_evict_valid,
  output et_link_minion_evict_req_info_t  l2_evict_req,
  // Mem acquire request bus to miss
  input  logic                            l2_miss_ready,
  output logic                            l2_miss_valid,
  output et_link_minion_miss_req_info_t   l2_miss_req,
  // Mem grant bus
  input  logic                            l2_resp_valid,
  input  et_link_minion_rsp_info_t        l2_resp_data,
  input  logic                            l2_resp_slot_avail,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] l2_resp_stall,
  // Refill control
  output logic [`DCACHE_WAY_RANGE]        refill_way,
  output logic [`DCACHE_LRAM_ADDR_RANGE]  refill_addr,
  // Meta write request
  input  logic                            meta_write_ready,
  output logic                            meta_write_valid_next,
  output logic                            meta_write_valid,
  output logic                            meta_write_clear_slock,
  output dcache_meta_write_req            meta_write_req,
  // Writeback request
  input  logic                            writeback_ready,
  output logic                            writeback_valid,
  output dcache_wb_req                    writeback_req,
  input  logic                            writeback_done,
  // Buffer array read port control for UC stores
  output logic                            force_ba_rptr,
  output logic                            force_ba_rptr_qual,
  output logic [`DCACHE_BUFFER_ID_RANGE]  ba_rptr,
  output logic                            ba_is_amo,
  // Buffer array write port control for UC loads
  output logic [`DCACHE_BUFFER_ID_RANGE]  ba_wptr,
  // Global idle
  output                                  idle,
  // Collisions avoidance with Cache Ops
  output logic [`DCACHE_MH_FILE_SIZE-1:0] probe_rdy,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] metaw_rdy,
  // Fence and general ready/error info
  output logic [`CORE_NR_THREADS-1:0]     fence_rdy,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] handler_ready,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] handler_fill_bus_error,
  output logic [`DCACHE_MH_FILE_SIZE-1:0] handler_uc_load_bus_error,
  // Misc
  output logic                            match_miss_line,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]     bus_err,
  output logic [`PA_RANGE]                bus_err_addr,
  `ifdef DCACHE_REPORT_PC
  output logic [`PC_RANGE_EXT]            bus_err_pc,
  `endif
  // For debug signals
  output logic [2*(`DCACHE_DBG_CSR_MH_SIZE)-1:0]                          csr_debug_signals,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_MATCH_BITS-1:0]  sm_match_debug_signals,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_debug_signals,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_DATA_BITS-1:0]   sm_data_debug_signals
);

  ////////////////////////////////////////////////////////////////////////////////
  // Re-arranges the input request and generates some valids
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_MH_FILE_SIZE-1:0][`CORE_NR_THREADS-1:0] mh_fence_rdy;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_probe_rdy;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_metaw_rdy;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_req_pri_val;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_req_pri_rdy;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_tag_match;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_evict_match;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_req_sec_rdy;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_can_accept_uc;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_idle;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_wait_l2_to_ba_resp;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                       mh_have_l2_to_ba_resp;
  logic                                                  req_sec_val;

  always_comb begin
    // Second valid is same to all miss handlers, only the one with mh_tag_match will use it
    req_sec_val = req_valid && (|mh_tag_match);

    // Ready signal
    if (|mh_tag_match) begin
      // We allow secondary miss if going to same cacheline
      req_ready = |(mh_tag_match & mh_req_sec_rdy);
    end else begin
      // Otherwise send the primary ready
      req_ready = |mh_req_pri_rdy;
    end

    // Other outputs
    match_miss_line = (|mh_tag_match) || (|mh_evict_match); // Checks if current transaction is accessing same cacheline than an outstanding miss or evict
    probe_rdy       =  mh_probe_rdy;
    metaw_rdy       =  mh_metaw_rdy;
    can_accept_uc   = |mh_can_accept_uc;

    wait_l2_to_ba_resp = |mh_wait_l2_to_ba_resp;

    for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
      fence_rdy[i] = 1'b1;
      for (integer j = 0; j < `DCACHE_MH_FILE_SIZE; j++) begin
        fence_rdy[i] &= mh_fence_rdy[j][i];
      end
    end

    for (integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin
      // Reports to which miss handler the miss is waiting for
      req_assigned[i] = (mh_req_pri_val[i] && mh_req_pri_rdy[i])               // Going as primary miss
                     || (req_sec_val       && mh_req_sec_rdy[i])               // Going as secondary miss
                     || mh_tag_match[i]                                        // If going to same cacheline as another miss, wait until it finishes
                     || mh_evict_match[i]                                      // If handler is evicting the cacheline that it is accessing, wait until it finishes
                     || ((req_info.thread_id == i[0]) && !req_info.cacheable); // An uncacheable request, wait for MH with same index as thread id

      // Reports which miss handler, if any, has accepted the request
      req_accepted[i] = (mh_req_pri_val[i] && mh_req_pri_rdy[i])  // Going as primary miss
                     || (req_sec_val       && mh_req_sec_rdy[i]); // Going as secondary miss
    end
  end

  assign idle = &mh_idle;


  ////////////////////////////////////////////////////////////////////////////////
  // This is the arbiter for the mem acquire request bus
  ////////////////////////////////////////////////////////////////////////////////

  et_link_minion_evict_req_info_t [`DCACHE_MH_FILE_SIZE-1:0] mh_l2_evict_req;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           mh_l2_evict_valid;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           mh_l2_evict_ready;

  et_link_minion_miss_req_info_t [`DCACHE_MH_FILE_SIZE-1:0]  mh_l2_miss_req;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           mh_l2_miss_valid;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           mh_l2_miss_ready;

  et_link_minion_evict_req_info_t                            ma_evict_arb_data_winner;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           ma_evict_arb_grant;
  logic [`DCACHE_MH_FILE_RANGE]                              ma_evict_arb_winner;

  et_link_minion_miss_req_info_t                             ma_miss_arb_data_winner;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                           ma_miss_arb_grant;
  logic [`DCACHE_MH_FILE_RANGE]                              ma_miss_arb_winner;


  arb_lru_data #(
    .WIDTH       ( $bits(et_link_minion_evict_req_info_t) ),
    .NUM_CLIENTS ( `DCACHE_MH_FILE_SIZE                   )
  ) l2_evict_arb (
    // System signals
    .clock       ( clock                    ),
    .reset       ( reset                    ),
    // Bidding
    .bid         ( mh_l2_evict_valid        ),
    .stall       ( !l2_evict_ready          ),
    // Data inputs
    .data_in     ( mh_l2_evict_req          ),
    // Grant
    .grant       ( ma_evict_arb_grant       ),
    .data_winner ( ma_evict_arb_data_winner ),
    .winner      ( ma_evict_arb_winner      )
  );

  arb_lru_data #(
    .WIDTH       ( $bits(et_link_minion_miss_req_info_t) ),
    .NUM_CLIENTS ( `DCACHE_MH_FILE_SIZE                  )
  ) l2_miss_arb (
    // System signals
    .clock       ( clock                   ),
    .reset       ( reset                   ),
    // Bidding
    .bid         ( mh_l2_miss_valid        ),
    .stall       ( !l2_miss_ready          ),
    // Data inputs
    .data_in     ( mh_l2_miss_req          ),
    // Grant
    .grant       ( ma_miss_arb_grant       ),
    .data_winner ( ma_miss_arb_data_winner ),
    .winner      ( ma_miss_arb_winner      )
  );

  always_comb begin
    l2_evict_valid    = mh_l2_evict_valid[ma_evict_arb_winner];
    l2_evict_req      = ma_evict_arb_data_winner;
    mh_l2_evict_ready = ma_evict_arb_grant;

    l2_miss_valid     = mh_l2_miss_valid[ma_miss_arb_winner];
    l2_miss_req       = ma_miss_arb_data_winner;
    mh_l2_miss_ready  = ma_miss_arb_grant;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // This is the arbiter for the meta write request bus
  ////////////////////////////////////////////////////////////////////////////////

  dcache_meta_write_req [`DCACHE_MH_FILE_SIZE-1:0] mh_meta_write_req;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                 mw_arb_grant;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                 mh_meta_write_valid;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                 mh_meta_write_valid_next;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                 mh_meta_write_clear_slock;
  logic [$clog2(`DCACHE_MH_FILE_SIZE)-1:0]         mw_arb_winner;

  arb_lru #(
    .NUM_CLIENTS ( `DCACHE_MH_FILE_SIZE )
  ) meta_write_arb (
    // System signals
    .clock       ( clock                ),
    .reset       ( reset                ),
    // Bidding
    .bid         ( mh_meta_write_valid  ),
    .stall       ( !meta_write_ready    ),
    // Grant
    .grant       ( mw_arb_grant         ),
    .winner      ( mw_arb_winner        )
  );

  always_comb begin
    meta_write_valid       = |mh_meta_write_valid;
    meta_write_valid_next  = |mh_meta_write_valid_next;
    meta_write_clear_slock = mh_meta_write_clear_slock[mw_arb_winner];
    meta_write_req         = mh_meta_write_req[mw_arb_winner];
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Muxes which miss handler is accessing the writeback unit (only one can be
  // set per cycle)
  ////////////////////////////////////////////////////////////////////////////////

  dcache_wb_req [`DCACHE_MH_FILE_SIZE-1:0] mh_wb_req;
  logic [`DCACHE_MH_FILE_SIZE-1:0]         mh_wb_valid;

  always_comb begin
    writeback_valid = |mh_wb_valid;
    writeback_req = 'b0;
    for (integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (mh_wb_valid[i]/*mh_req_pri_val[i]*/) begin // Using pri_val as an earlier signal
        writeback_req |= mh_wb_req[i];
      end
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Selects the refill control
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_WAY_RANGE]       mh_refill_way;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_LRAM_ADDR_RANGE] mh_refill_addr;

  always_comb begin
    refill_way  = mh_refill_way[l2_resp_data.id[`DCACHE_MH_FILE_RANGE]];
    refill_addr = mh_refill_addr[l2_resp_data.id[`DCACHE_MH_FILE_RANGE]];
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Bus error report
  ////////////////////////////////////////////////////////////////////////////////
  logic [`DCACHE_MH_FILE_SIZE-1:0][`CORE_NR_THREADS-1:0]  mh_bus_err;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`PA_RANGE]             mh_bus_err_addr;
  `ifdef DCACHE_REPORT_PC
  logic [`DCACHE_MH_FILE_SIZE-1:0][`PC_RANGE_EXT]         mh_bus_err_pc;
  `endif

  always_comb begin
    // Combine the errors of all handlers for every thread
    bus_err = mh_bus_err[0];
    for (integer i = 1; i < `DCACHE_MH_FILE_SIZE; i++) begin
      bus_err |= mh_bus_err[i];
    end

    bus_err_addr = mh_bus_err_addr[0];
    for (integer i = 1; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (|mh_bus_err[i]) begin
        bus_err_addr = mh_bus_err_addr[i];
      end
    end

    `ifdef DCACHE_REPORT_PC
    bus_err_pc = mh_bus_err_pc[0];
    for (integer i = 1; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (|mh_bus_err[i]) begin
        bus_err_pc = mh_bus_err_pc[i];
      end
    end
    `endif
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Instantiates the miss handlers
  ////////////////////////////////////////////////////////////////////////////////

  dcache_miss_info [`DCACHE_MH_FILE_SIZE-1:0]               mh_inflight_misses;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_BUFFER_ID_RANGE] mh_ba_rptr;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                          mh_force_ba_rptr_next;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                          mh_force_ba_rptr_qual;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                          mh_ba_is_amo;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                          mh_req_pri_rdy_sel;
  logic [`DCACHE_MH_FILE_SIZE-1:0]                          mh_going_to_uc_store;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_CSR_MH_SIZE-1:0] mh_debug_signals;


  always_comb begin
    going_to_uc_store = mh_going_to_uc_store[0];
    for (integer i = 1; i < `DCACHE_MH_FILE_SIZE; i++) begin
      going_to_uc_store |= mh_going_to_uc_store[i];
    end
  end



  for (genvar i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin : handlers
    // Primary valid
    // Arbitrates which entry is going to win
    // We give priority to entries that are not the zero, as we want to have
    // it available for UC operations
    always_comb begin
      mh_req_pri_rdy_sel[i] = mh_req_pri_rdy[i];
      for (integer j = i + 1; j < `DCACHE_MH_FILE_SIZE; j++) begin
        mh_req_pri_rdy_sel[i] = mh_req_pri_rdy_sel[i] && !mh_req_pri_rdy[j];
      end

      mh_req_pri_val[i] = req_valid && !(|mh_tag_match) && mh_req_pri_rdy_sel[i];
    end

    dcache_miss_handler #(
      .ID                         ( i                           )
    ) miss_handler (
      // System signal
      .clock                      ( clock                       ),
      .reset                      ( reset                       ),
      .neigh_min_id               ( neigh_min_id                ),
      // Request bus
      .req_pri_val                ( mh_req_pri_val[i]           ),
      .req_pri_rdy                ( mh_req_pri_rdy[i]           ),
      .req_sec_val                ( req_sec_val                 ),
      .req_sec_rdy                ( mh_req_sec_rdy[i]           ),
      .can_accept_uc              ( mh_can_accept_uc[i]         ),
      .req_info                   ( req_info                    ),
      .tag_match                  ( mh_tag_match[i]             ),
      .evict_match                ( mh_evict_match[i]           ),
      .set                        ( mh_inflight_misses[i].set   ),
      .inflight_misses            ( mh_inflight_misses          ),
      .inflight_reduce            ( inflight_reduce             ),
      .going_to_uc_store          ( mh_going_to_uc_store[i]     ),
      // Handshake for UCs
      .idle                       ( mh_idle[i]                  ),
      .wait_l2_to_ba_resp         ( mh_wait_l2_to_ba_resp[i]    ),
      .have_l2_to_ba_resp         ( mh_have_l2_to_ba_resp[i]    ),
      // Mem write request bus to evict
      .l2_evict_ready             ( mh_l2_evict_ready[i]        ),
      .l2_evict_valid             ( mh_l2_evict_valid[i]        ),
      .l2_evict_req               ( mh_l2_evict_req[i]          ),
      // Mem acquire request bus to miss
      .l2_miss_ready              ( mh_l2_miss_ready[i]         ),
      .l2_miss_valid              ( mh_l2_miss_valid[i]         ),
      .l2_miss_req                ( mh_l2_miss_req[i]           ),
      // Memory grant
      .l2_resp_valid              ( l2_resp_valid               ),
      .l2_resp_data               ( l2_resp_data                ),
      .l2_resp_slot_avail         ( l2_resp_slot_avail          ),
      .l2_resp_stall              ( l2_resp_stall[i]            ),
      // Refill control
      .refill_way                 ( mh_refill_way[i]            ),
      .refill_addr                ( mh_refill_addr[i]           ),
      // SDQ read port control for UC stores
      .force_ba_rptr_next         ( mh_force_ba_rptr_next[i]    ),
      .force_ba_rptr_qual         ( mh_force_ba_rptr_qual[i]    ),
      .ba_rptr                    ( mh_ba_rptr[i]               ),
      .ba_is_amo                  ( mh_ba_is_amo[i]             ),
      // Tag update bus
      .meta_write_ready           ( mw_arb_grant[i]             ),
      .meta_write_valid_next      ( mh_meta_write_valid_next[i] ),
      .meta_write_valid           ( mh_meta_write_valid[i]      ),
      .meta_write_clear_slock     ( mh_meta_write_clear_slock[i]),
      .meta_write_req             ( mh_meta_write_req[i]        ),
      // Writeback request bus
      .writeback_ready            ( writeback_ready             ),
      .writeback_valid            ( mh_wb_valid[i]              ),
      .writeback_req              ( mh_wb_req[i]                ),
      .writeback_done             ( writeback_done              ),
      // Bus error
      .bus_err                    ( mh_bus_err[i]               ),
      .bus_err_addr               ( mh_bus_err_addr[i]          ),
      `ifdef DCACHE_REPORT_PC
      .bus_err_pc                 ( mh_bus_err_pc[i]            ),
      `endif
      .fill_bus_error             ( handler_fill_bus_error[i]   ),
      .uc_load_bus_error          ( handler_uc_load_bus_error[i]),
      // Collisions avoidance with Cache Ops
      .probe_rdy                  ( mh_probe_rdy[i]             ),
      .metaw_rdy                  ( mh_metaw_rdy[i]             ),
      // Fence and general ready info
      .fence_rdy                  ( mh_fence_rdy[i]             ),
      .ready                      ( handler_ready[i]            ),
      // Debug signals
      .csr_debug_signals          ( mh_debug_signals[i]         ),
      .sm_match_debug_signals     ( sm_match_debug_signals[i]   ),
      .sm_filter_debug_signals    ( sm_filter_debug_signals[i]  ),
      .sm_data_debug_signals      ( sm_data_debug_signals[i]    )
      );

      always_comb begin
        // Gets the inflight miss info sent to the other miss handlers
        mh_inflight_misses[i].valid             = !mh_idle[i];
        mh_inflight_misses[i].way_en            = dcache_way_decode(mh_refill_way[i]);
        //mh_inflight_misses[i].writeback_pending = mh_writeback_pending[i];
      end
    end
  

  ////////////////////////////////////////////////////////////////////////////////
  // Buffer array override is controlled here to execute UC stores
  ////////////////////////////////////////////////////////////////////////////////

  // To improve timing, the force_ba_rptr is flopped based on a "next" value
  //  CK     DOUT           DIN
  `FF(clock, force_ba_rptr, |mh_force_ba_rptr_next)

  always_comb begin
    //force_ba_rptr      = |mh_force_ba_rptr;
    force_ba_rptr_qual = |mh_force_ba_rptr_qual;
    ba_rptr            = mh_ba_rptr[ma_evict_arb_winner];
    ba_is_amo          = mh_ba_is_amo[ma_evict_arb_winner];
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Buffer array override is controlled here to execute UC loads
  ////////////////////////////////////////////////////////////////////////////////

  // Write signal is generated in dcache_top from L2 responses . The ba_rptr
  // is not m odified until the operation ends, so it can be reused for wptr
  always_comb begin
    ba_wptr = mh_ba_rptr[0];
    for (integer h = 1; h < `DCACHE_MH_FILE_SIZE; h++) begin
      if (mh_have_l2_to_ba_resp[h]) begin
        ba_wptr = mh_ba_rptr[h];
      end
    end
  end

  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  // Debug signals
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

  assign csr_debug_signals = { mh_debug_signals[1], mh_debug_signals[0] };


endmodule
