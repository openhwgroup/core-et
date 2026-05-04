// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_miss_handler #(
  parameter ID = 0
) (
  // System signal
  input  logic                                       clock,
  input  logic                                       reset,
  input  logic [`MIN_PER_N_R]                        neigh_min_id,
  // Request bus
  input  logic                                       req_pri_val,
  output logic                                       req_pri_rdy,
  input  logic                                       req_sec_val,
  output logic                                       req_sec_rdy,
  output logic                                       can_accept_uc,
  input  dcache_mh_req                               req_info,
  output logic                                       tag_match,
  output logic                                       evict_match,
  output logic [`DCACHE_SET_RANGE]                   set,
  input  dcache_miss_info [`DCACHE_MH_FILE_SIZE-1:0] inflight_misses,
  input  logic                                       inflight_reduce,
  output logic                                       going_to_uc_store,
  // Handshake for UCs (guarantee that they are executed isolated)
  output logic                                       idle,
  output logic                                       wait_l2_to_ba_resp,
  output logic                                       have_l2_to_ba_resp,
  // Mem write request bus to evict
  input  logic                                       l2_evict_ready,
  output logic                                       l2_evict_valid,
  output et_link_minion_evict_req_info_t             l2_evict_req,
  // Mem acquire request bus to miss
  input  logic                                       l2_miss_ready,
  output logic                                       l2_miss_valid,
  output et_link_minion_miss_req_info_t              l2_miss_req,
  // L2 fills coming back
  input  logic                                       l2_resp_valid,
  input  et_link_minion_rsp_info_t                   l2_resp_data,
  input                                              l2_resp_slot_avail,
  output logic                                       l2_resp_stall,
  // Refill control
  output logic [`DCACHE_WAY_RANGE]                   refill_way,
  output logic [`DCACHE_LRAM_ADDR_RANGE]             refill_addr,
  // Buffer array read port control for UC stores
  output logic                                       force_ba_rptr_next,
  output logic                                       force_ba_rptr_qual,
  output logic [`DCACHE_BUFFER_ID_RANGE]             ba_rptr,
  output logic                                       ba_is_amo,
  // Tag update bus
  input  logic                                       meta_write_ready,
  output logic                                       meta_write_valid_next,
  output logic                                       meta_write_valid,
  output logic                                       meta_write_clear_slock,
  output dcache_meta_write_req                       meta_write_req,
  // Writeback request bus
  input  logic                                       writeback_ready,
  output logic                                       writeback_valid,
  output dcache_wb_req                               writeback_req,
  input  logic                                       writeback_done,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]                bus_err,
  output logic [`PA_RANGE]                           bus_err_addr,
  `ifdef DCACHE_REPORT_PC
  output logic [`PC_RANGE_EXT]                       bus_err_pc,
  `endif
  output logic                                       fill_bus_error,
  output logic                                       uc_load_bus_error,
  // Misc
  output logic                                       probe_rdy,
  output logic [`CORE_NR_THREADS-1:0]                fence_rdy,
  output logic                                       metaw_rdy,
  output logic                                       ready,
  // For debug signals
  output logic [`DCACHE_DBG_CSR_MH_SIZE-1:0]         csr_debug_signals,
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]       sm_match_debug_signals,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]      sm_filter_debug_signals,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]        sm_data_debug_signals
);

  ////////////////////////////////////////////////////////////////////////////////
  // Identifier to be used with L2 requests
  ////////////////////////////////////////////////////////////////////////////////
  et_link_core_miss_id_t     my_l2_miss_id;
  et_link_core_miss_id_t     my_l2_miss_id_uc_st;
  et_link_core_miss_id_t     my_l2_miss_id_uc_ld;
  logic                      l2_resp_is_my_id;
  logic                      l2_resp_is_my_id_uc;
  logic                      l2_resp_is_my_id_uc_st;
  logic                      l2_resp_is_my_id_uc_ld;

  assign my_l2_miss_id       = et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Miss_Handler_0 + et_link_core_miss_id_t'(ID));
  assign my_l2_miss_id_uc_st = et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0 + et_link_core_miss_id_t'(ID));
  assign my_l2_miss_id_uc_ld = et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0 + et_link_core_miss_id_t'(ID));

  assign l2_resp_is_my_id       = (et_link_core_miss_id_t'(l2_resp_data.id[`ET_CORE_MISS_ID_RANGE]) == my_l2_miss_id);
  assign l2_resp_is_my_id_uc_st = (et_link_core_miss_id_t'(l2_resp_data.id[`ET_CORE_MISS_ID_RANGE]) == my_l2_miss_id_uc_st);
  assign l2_resp_is_my_id_uc_ld = (et_link_core_miss_id_t'(l2_resp_data.id[`ET_CORE_MISS_ID_RANGE]) == my_l2_miss_id_uc_ld);
  assign l2_resp_is_my_id_uc    = l2_resp_is_my_id_uc_st | l2_resp_is_my_id_uc_ld;

  ////////////////////////////////////////////////////////////////////////////////
  // Holds the current request information
  ////////////////////////////////////////////////////////////////////////////////

  logic             req_up;                // Updating the request contents
  logic             req_info_is_write;     // Incoming request is a write

  dcache_mh_req     req;                   // Request we are working on
  dcache_mh_req     req_next;

  //     CLK    EN         DOUT         DIN
  `EN_FF(clock, req_up,    req,         req_next)

  always_comb begin
    req_info_is_write = dcache_cmd_is_write(req_info.cmd);

    req_next = req;
    req_up   = 1'b0;

    if (req_pri_val && req_pri_rdy) begin
      req_next = req_info;
      req_up   = 1'b1;
    // If we get a secondary miss that needs more permissions before we've sent
    // out the primary miss's Acquire, we can upgrade the permissions we're
    // going to ask for in s_refill_req
    end else if (req_sec_val && req_sec_rdy && (dcache_cmd_to_type(req.cmd) < dcache_cmd_to_type(req_info.cmd))) begin
      req_next.cmd = req_info.cmd;
      req_up       = 1'b1;
    end

  end

  ////////////////////////////////////////////////////////////////////////////////
  // Miss Handler FSM state
  ////////////////////////////////////////////////////////////////////////////////

  dcache_mh_state   state, state_next;                         // FSM state of the miss handler

  dcache_line_state new_coh, new_coh_next;                     // Coherent state for the fill request
  dcache_line_state coh_on_grant;                              // Coherent state set for grant accesses
  logic             refill_done;                               // Refill write to data array has finished
  logic             uc_store_ack;                              // Received the UC store ACK from L2
  logic             writeback_pending;                         // Miss handler hasn't done the writeback yet
  logic             writeback_pending_next;
  logic             metawrite_pending;                         // Miss handler hasn't written the new tag yet
  logic             metawrite_pending_next;
  logic             writeback_alloc_nack;                      // Couldn't allocate writeback when needed
  logic             state_was_fill_resp;                       // Last cycle state was fill response

  logic             response_err;                              // Error received during a L2 response
  logic             response_err_fill;
  logic             response_err_uc;

  logic             clear_soft_lock;                           // Indication that soft lock has to be cleared
  logic             clear_soft_lock_next;


  //      CLK    RST    DOUT                 DIN                     DEF
  `RST_FF(clock, reset, state,               state_next,             MH_Invalid)
  `RST_FF(clock, reset, writeback_pending,   writeback_pending_next, 1'b0)
  `RST_FF(clock, reset, metawrite_pending,   metawrite_pending_next, 1'b0)
  `RST_FF(clock, reset, clear_soft_lock,     clear_soft_lock_next,   1'b0)
  `FF    (clock,        new_coh,             new_coh_next)
  `FF    (clock,        state_was_fill_resp, (state == MH_Fill_Resp))


  always_comb begin
    state_next             = state;
    new_coh_next           = new_coh;
    writeback_pending_next = writeback_pending;
    metawrite_pending_next = metawrite_pending;
    writeback_valid        = 1'b0;
    clear_soft_lock_next   = clear_soft_lock;

    // Writeback alloc has to be done ignoring the valid bit of the request,
    // otherwise we can enter in a combinational loop
    writeback_alloc_nack = !req_info.tag_match && req_info.cacheable && (req_info.old_meta.coh_state == State_Modified) &&
                           !writeback_ready && !req_info.is_bypass;

    // A new request comes in
    if (req_pri_val && req_pri_rdy) begin
      // Line is already in cache
      if (req_info.tag_match) begin
        // Move to dirty in case of write and exclusive
        if (req_info_is_write && (req_info.old_meta.coh_state == State_Exclusive)) begin
          new_coh_next           = State_Modified;
          state_next             = MH_Meta_Write_Req;
          metawrite_pending_next = 1'b1;
        // Upgrade permissions otherwise (this may not be reachable, just inherited code ... code coverage will tell)
        end else begin
          new_coh_next           = coh_on_grant;
          state_next             = MH_Fill_Req;
          metawrite_pending_next = 1'b1;
        end
      // Uncacheable request.
      //  * Atomics and Messages are marked as "not cacheable"
      //  * Special instructions bypassing L1 are marked as "not cacheable"
      end else if (!req_info.cacheable) begin
        state_next = MH_UC_Wait_Idle;
      // Writeback/evict the victim
      end else begin
        // The FSM writes the new tag and sets the coherent state right
        // away. Writeback doesn't need the old tag to do the evict. Other
        // instructions in dcache might hit with the tag of the miss which
        // hasn't finished, but they will be nacked by the miss handler
        // due an outstanding miss hit. This way we advance the write tag
        // as soon as possible and prevent having conflicts later with
        // other units
        new_coh_next = coh_on_grant; // Sets the coherent state for the missed cacheline
        clear_soft_lock_next = 1'b1; // Records that soft lock needs to be cleared
        // Dirty Evict. Acquire writeback loop. This state was added to break large timming path (RTLMIN-4047).
        if (req_info.old_meta.coh_state == State_Modified) begin
          state_next = MH_Acquire_Wb;
        // Dealing with a clean evict, go straight to request new
        // cacheline
        end else begin
          state_next             = MH_Fill_Req;
          metawrite_pending_next = 1'b1;
        end
      end
    end

    // MH_Acquire_Wb
    if (state == MH_Acquire_Wb) begin
      // Requests writeback access
      writeback_valid = 1'b1;
      // If granted, moves to next state
      if (writeback_ready) begin
        state_next             = MH_Fill_Req;
        writeback_pending_next = 1'b1;
        metawrite_pending_next = 1'b1;
      end else begin
        state_next = MH_Acquire_Wb;
      end
    end

    // Fill Flow
    if ((state == MH_Fill_Req) && l2_miss_ready) begin
      state_next = MH_Fill_Resp;
    end
    if ((state == MH_Fill_Resp) && refill_done) begin
      // If could update tags before move to invalid
      if (!metawrite_pending) begin
        state_next = MH_Invalid;
      // If writing tags in this cycle, go to the hazard cycle
      end else if (meta_write_valid && meta_write_ready) begin
        state_next = MH_Meta_Hazard;
      // Otherwise retry write next cycle
      end else begin
        state_next = MH_Meta_Write_Req;
      end
    end
    if ((state == MH_Fill_Resp) && response_err_fill) begin
      // If writing tags in this cycle, go to the metadata cleaning state
      if (meta_write_valid && meta_write_ready) begin
        state_next = MH_Fill_Clean;
      // Otherwise make sure that metadata are updated with an invalid state
      end else begin
        metawrite_pending_next = 1'b1;
        new_coh_next = State_Invalid;
        state_next = MH_Meta_Write_Req;
      end
    end
    if (state == MH_Fill_Clean) begin
      metawrite_pending_next = 1'b1;
      new_coh_next = State_Invalid;
      state_next = MH_Meta_Write_Req;
    end
    if ((state == MH_Meta_Write_Req) && meta_write_ready) begin
      state_next = MH_Meta_Hazard;
    end
    // This wait state allows us to catch RAW hazards on the tags via nack_victim
    // We need to guarantee that there's no instruction in S2
    // after the tag update that was accessing same cacheline. If this is the case,
    // it would re-miss same cacheline (because it didn't see the updated tag in S1)
    // and we would have two times the same tag in the dcache
    if (state == MH_Meta_Hazard) begin
      state_next = MH_Invalid;
    end

    // UC flows
    if ((state == MH_UC_Wait_Idle)) begin
      state_next = dcache_cmd_is_write(req.cmd) ? MH_UC_Store_Wait : MH_UC_Load_Req;
    end
    if ((state == MH_UC_Load_Req) && l2_miss_ready) begin
      state_next = MH_UC_Load_Resp;
    end
    if ((state == MH_UC_Load_Resp) && l2_resp_valid && (l2_resp_data.opcode == ET_LINK_RSP_AckData) && l2_resp_is_my_id_uc) begin
      state_next = MH_Invalid;
    end
    if ((state == MH_UC_Load_Resp) && l2_resp_valid && (l2_resp_data.opcode == ET_LINK_RSP_Err) && l2_resp_is_my_id_uc) begin  // In case of error, we just finish operation
      state_next = MH_Invalid;
    end
    if ((state == MH_UC_Store_Wait) && !inflight_reduce) begin
      state_next = MH_UC_Store_Req;
    end
    if ((state == MH_UC_Store_Req) && l2_evict_ready) begin
      state_next = req.is_amo    ? MH_UC_Load_Resp :
                   req.is_msg    ? MH_Invalid :
                                   MH_UC_Store_Ack;
    end
    if ((state == MH_UC_Store_Ack) && uc_store_ack) begin
      state_next = MH_Invalid;
    end

    // Writeback pending update
    if (writeback_pending && writeback_done) begin
      writeback_pending_next = 1'b0;
    end

    // Tag write pending clear
    if (metawrite_pending && meta_write_ready) begin
      metawrite_pending_next = 1'b0;
      clear_soft_lock_next   = 1'b0;
    end
  end

  // Metadata may be updated while we are in any of the fill flow states.
  assign metaw_rdy = !metawrite_pending && (state != MH_Acquire_Wb) && (state != MH_Fill_Req) && (state != MH_Fill_Resp) && (state != MH_Fill_Clean);

  assign meta_write_clear_slock = clear_soft_lock;

  ////////////////////////////////////////////////////////////////////////////////
  // Extracts some address info
  ////////////////////////////////////////////////////////////////////////////////

  logic [`PA_TAG_EXT_RANGE] tag;       // Tag of current request
  logic                     set_match; // Set of request matches with a new fill

  always_comb begin
    tag = req.addr[`PA_TAG_EXT_RANGE];
    set = req.set;

    // Important. Need to keep the tag match for 1 cycle after the fill
    // response to prevent having a transaction in S2 consuming fill data that was
    // written last cycle.
    set_match = (set == req_info.set) && ((state != MH_Invalid) || state_was_fill_resp);
    tag_match = (tag == req_info.addr[`PA_TAG_EXT_RANGE]) && set_match && !req_info.is_msg && !req_info.is_bypass && req_info.cacheable; // Can't have a tag match in case of message request or uncacheable

    // The line being requested in S2 is being evicted when there's
    // a writeback pending, same set and same way
    evict_match = (state != MH_Invalid) && (set == req_info.set) && (req.way_en == req_info.way_en) && !req_info.is_msg && !req_info.is_bypass && req_info.cacheable; // Can't have an evict match in case of message request or uncacheable
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Coherent state of the request
  ////////////////////////////////////////////////////////////////////////////////

  dcache_access_type acc_type;

  always_comb begin
    acc_type = dcache_cmd_to_type(req_info.cmd);

    // Move to exclusive state when reading because the memory is non-coherent, otherwise it should be shared
    if (acc_type == dcache_access_rd) begin
      coh_on_grant = State_Exclusive;
    // We move the state to modified for prefetches with write intent
    // The reason is that if software is adding such prefetches it knows that
    // it will eventually write the cacheline, so why generate another extra
    // miss when the write comes?
    end else begin
      coh_on_grant = State_Modified;
    end
  end


  ////////////////////////////////////////////////////////////////////////////////
  // We only accept secondary misses if we haven't yet sent an Acquire to outer memory
  // or if the Acquire that was sent will obtain a Grant with sufficient permissions
  // to let us replay this new request. I.e. we don't handle multiple outstanding
  // Acquires on the same block for now.
  ////////////////////////////////////////////////////////////////////////////////

  logic sec_rdy;                     // A new miss can be accepted
  logic state_before_refill;         // The miss handler hasn't sent yet the mem acquire request
  logic cmd_requires_second_acquire; // The new miss requires a higher MESI state compared to the miss handler request

  always_comb begin
    cmd_requires_second_acquire = dcache_cmd_is_write_intent(req_info.cmd) && !dcache_cmd_is_write_intent(req.cmd);

    // Remove this condition and be conservative since it is creating a long timing path as it depends on "l2_miss_ready"
    // It should not make a relevant performance drop since if request is accepted immediatelly we get no difference
    // and if the request is not accepted immediatelly because of congestion, the performance is limited by congestion
    // state_before_refill         = (state == MH_Fill_Req) && !l2_miss_ready;
    state_before_refill = 1'b0;

    // Need a set match to accept a new request and also must be cacheable
    // (uncacheable can't have a secondary hit)
    sec_rdy = tag_match && req_info.cacheable && !req_info.is_bypass;
    // If state is before, refill we are done, otherwise we need extra checks
    if (!state_before_refill) begin
      // If already sent the acquire and we need higher rights, can't accept request
      sec_rdy &= !cmd_requires_second_acquire;
    end
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Counter for the fills done of this miss. It also is used to know where to
  // write the data of the fill within the cacheline.
  ////////////////////////////////////////////////////////////////////////////////

  logic [`CORE_L2_BLOCK_EXT_PASSES_RANGE] refill_address_inc;      // Which chunk of the fill we are writing
  logic [`CORE_L2_BLOCK_EXT_PASSES_RANGE] refill_address_inc_next; // Next counting value
  logic                                   refill_address_inc_en;   // We are writing fill data
  logic                                   refill_inc_is_last;      // Flag indicating that refill is in the last cycle

  `RST_EN_FF(clock, reset, refill_address_inc_en, refill_address_inc, refill_address_inc_next, 'b0)

  assign refill_address_inc_en = l2_resp_valid && l2_resp_is_my_id && (l2_resp_data.opcode == ET_LINK_RSP_AckData);
  assign refill_done = refill_address_inc_en && refill_inc_is_last;

  assign refill_inc_is_last = refill_address_inc == (`CORE_L2_BLOCK_EXT_PASSES-1);
  assign refill_address_inc_next = (refill_inc_is_last || (state == MH_Invalid))? '0 : refill_address_inc + 1'b1;

  ////////////////////////////////////////////////////////////////////////////////
  // Detect errors
  ////////////////////////////////////////////////////////////////////////////////

  assign response_err_fill = l2_resp_valid && l2_resp_is_my_id    && (l2_resp_data.opcode == ET_LINK_RSP_Err);
  assign response_err_uc   = l2_resp_valid && l2_resp_is_my_id_uc && (l2_resp_data.opcode == ET_LINK_RSP_Err);
  assign response_err      = response_err_fill | response_err_uc;

  assign bus_err      = {response_err & req.thread_id, response_err & ~req.thread_id};
  assign bus_err_addr = req.addr;
  `ifdef DCACHE_REPORT_PC
  assign bus_err_pc   = req.pc;
  `endif

  assign fill_bus_error    = response_err_fill && (state == MH_Fill_Resp);
  assign uc_load_bus_error = response_err_uc   && (state == MH_UC_Load_Resp);


  ////////////////////////////////////////////////////////////////////////////////
  // Outputs of the module based on the state
  ////////////////////////////////////////////////////////////////////////////////

  logic                   set_way_conf; // Conflict with another inflight miss going to same set and way
  logic                   force_ba_rptr;

  logic                   l2_evict_valid_next; // Forecast value for the valid signals to evict and miss
  logic                   l2_miss_valid_next;

  //   CK     DOUT             DIN
  `FF (clock, l2_evict_valid,  l2_evict_valid_next)
  `FF (clock, l2_miss_valid,   l2_miss_valid_next)


  always_comb begin
    // Checks if there's a set & way conflict with any other miss handler
    set_way_conf = 1'b0;
    for (integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++) begin
      if (    (i != ID)                                      // Not this miss handler
          && inflight_misses[i].valid                       // Valid miss
          && (inflight_misses[i].set    == req_info.set)    // same set
          && (inflight_misses[i].way_en == req_info.way_en) // same way
      ) begin
        set_way_conf = 1'b1;
      end
    end

    // Request bus ready and other misc info
    req_pri_rdy = (state == MH_Invalid)                                      // Handler idle
                && !writeback_alloc_nack                                     // Can allocate writeback unit if evict required
                && !(set_way_conf && !req_info.is_msg && req_info.cacheable) // Cacheline not already allocated by another handler (except for msg requests and uncacheables)
                && ((req_info.thread_id == ID) || req_info.cacheable);       // Can only accep uncacheable for thread_id == ID
    req_sec_rdy = sec_rdy;
    can_accept_uc = req_pri_rdy && !req_info.cacheable;

    // MH_Fill_Resp -> Refill control. For ML requests, the set comes from
    // the request and not the address
    refill_way    = dcache_way_encode(req.way_en);
    refill_addr   = { set, refill_address_inc[0], {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}} };

    // Writeback request to do the evict
    writeback_req.tag       = req.old_meta.tag;
    writeback_req.set       = req.set;
    writeback_req.way_en    = req.way_en;
    writeback_req.is_tbox   = 1'b0;
    writeback_req.mem_idx   = 1'b0;
    writeback_req.addr_low  = {`PA_NOTAG_EXT_BITS{1'b0}};

    // Request to evict_ff
    l2_evict_valid_next    = (state_next == MH_UC_Store_Req);
    l2_evict_req.id        = my_l2_miss_id_uc_st;
    l2_evict_req.source    = 1'b0;
    l2_evict_req.wdata     = 1'b1;
    l2_evict_req.opcode    = req.is_amo ? ET_LINK_REQ_Atomic      :
                             req.is_msg ? ET_LINK_REQ_MsgSendData :
                                          ET_LINK_REQ_Write;
    l2_evict_req.subopcode = req.is_amo ? dcache_get_et_link_amo_config(req.cmd,req.typ,req.mem_g) :
                             req.is_msg ? `ET_LINK_SUBOPCODE_SIZE'b0                               : // Explicitly '0' for messages
                             req.mem_g  ? `ZX(`ET_LINK_SUBOPCODE_SIZE,ET_LINK_Write_L3)            :
                                          `ZX(`ET_LINK_SUBOPCODE_SIZE,ET_LINK_Write_L2);
    l2_evict_req.address   = req.addr;
    l2_evict_req.data      = `CORE_L2_BLOCK_EXT_SIZE'b0; // Data taken from s1_ba_data in dcache_top, which is already address-aligned
    l2_evict_req.size      = dcache_uc_size(req.typ);
    l2_evict_req.qwen      = (req.is_amo | req.is_msg) ?
                                dcache_typ2qwen(req.typ,2'b0)           // Atomics and Messages have data in LSB
                              : dcache_typ2qwen(req.typ,req.addr[5:4]); // Bypass operations have data address aligned

    // Request to miss_ff
    l2_miss_valid_next    = (state_next == MH_Fill_Req) || (state_next == MH_UC_Load_Req);
    l2_miss_req.id        = (state == MH_Fill_Req) ? my_l2_miss_id : my_l2_miss_id_uc_ld;
    l2_miss_req.source    = 1'b0; // Unused
    l2_miss_req.wdata     = 1'b0;
    l2_miss_req.opcode    = ET_LINK_REQ_Read;
    l2_miss_req.subopcode = (state == MH_Fill_Req) ? `ZX(`ET_LINK_SUBOPCODE_SIZE,ET_LINK_Read_L2) :
                            req.mem_g              ? `ZX(`ET_LINK_SUBOPCODE_SIZE,ET_LINK_Read_L3) :
                                                     `ZX(`ET_LINK_SUBOPCODE_SIZE,ET_LINK_Read_L2);
    l2_miss_req.address   = (state == MH_Fill_Req) ? { req.addr[`PA_CLINE_RANGE], 6'b0 } : // For misses, address cache line aligned
                                                     req.addr;                             // For UC, full address is sent,
    l2_miss_req.data      = `MIN_PER_N'b1 << neigh_min_id;
    l2_miss_req.size      = (state == MH_Fill_Req) ? ET_LINK_Line :
                                                     dcache_uc_size(req.typ);
    l2_miss_req.qwen      = 4'b0000;

    // MH_Meta_Write_Req -> Update the tags
    // We have to wait for the writeback to finish before writing the new tags
    //    The reason is that if we don't do this, the other handler might miss the
    //    cacheline that this handler is evicting. If so, there's a race betwen the
    //    evict and the fill that can end up with data corruption. The solution is
    //    to update the tags after the evict is done, so this way if the evicted
    //    line is accessed again, it will still hit and will only miss later when
    //    the evict is already going to L2
    meta_write_valid_next         = metawrite_pending_next && !writeback_pending_next; // Early version to enable clock in metadata array
    meta_write_valid              = metawrite_pending && !writeback_pending;
    meta_write_req.set            = set;
    meta_write_req.way_en         = req.way_en;
    meta_write_req.data.coh_state = new_coh;
    meta_write_req.data.tag       = tag;

    // MH_UC_Store_Req: sets the read pointer of the BufferArray to send the correct
    // data
    force_ba_rptr_next = (state_next == MH_UC_Store_Req);
    force_ba_rptr      = (state == MH_UC_Store_Req);
    force_ba_rptr_qual = force_ba_rptr && l2_evict_ready;
    ba_rptr            = req.buf_id;
    ba_is_amo          = req.is_amo; // Need to report if this is an AMO, so the buffer array is not deallocated

    // Reports if miss handler is idle
    idle = (state == MH_Invalid);

    // Reports that MH wants to do a UC Store
    going_to_uc_store = (state == MH_UC_Store_Wait) || (state == MH_UC_Store_Req);

    // Reports that miss handler is waiting for or has an L2 response that will produce a writing to buffer array
    wait_l2_to_ba_resp = (state == MH_UC_Load_Resp);
    have_l2_to_ba_resp = (state == MH_UC_Load_Resp) && l2_resp_valid && l2_resp_is_my_id_uc;

    // The miss handler is ready means that an instruction waiting for this
    // miss handler to finish can bid this cycle in replayQ because it will
    // find the data in the tags/data by the time it gets to s1/s2 stages.
    // Also it is used by the instructions that couldn't allocate a miss
    // handler to know they can replay and allocate the handler
    ready = (state == MH_Invalid)                                                     // It is completely empty
         || (state == MH_Meta_Hazard)                                                 // Doing the tag hazard cycle
         || (state == MH_Fill_Resp) && refill_inc_is_last && l2_resp_slot_avail;      // Doing last fill chunk this cycle

    // Stalls fills from the L2 if the evict hasn't finished yet (write new tag
    // and writeback the results)
    l2_resp_stall = metawrite_pending || (writeback_pending && !writeback_done);
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Sends ready signals for fence and snoops
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    probe_rdy = !set_match || (state == MH_Invalid);
    // Fencing is per thread. Either not doing anything or working on
    // a different thread
    for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
      fence_rdy[i] = (state == MH_Invalid) || (req.thread_id != i);
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Detects UC stores Ack
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    uc_store_ack = l2_resp_valid && l2_resp_is_my_id_uc && ((l2_resp_data.opcode == ET_LINK_RSP_Ack) || (l2_resp_data.opcode == ET_LINK_RSP_Err));
  end


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Debug CSR signals
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

assign csr_debug_signals = state;
/* RTLMIN-5542
assign csr_debug_signals = {
			    // Requests fields
			    req.is_bypass,
			    req.is_msg,
			    req.is_amo,
			    req.mem_g,
			    req.thread_id,
			    req.cacheable,
			    req.tag_match,
			    req.way_en,
			    req.buf_id,
			    req.typ,
			    req.cmd,
			    req.set,
			    // fsm state
			    state
			    };
*/

assign sm_match_debug_signals = `DCACHE_DBG_SM_MATCH_BITS'b0;

assign sm_filter_debug_signals = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
        {
          meta_write_valid,
          writeback_valid,
          bus_err,
          tag_match,
          evict_match,
          can_accept_uc,
          req_sec_val,
          req_pri_val,
          req_up,
          state
        });

assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(3)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
`ifdef DCACHE_REPORT_PC
assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          req.pc
        });
`else
assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
`endif

assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(1)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          req_pri_val,
          req_pri_rdy,
          req_sec_val,
          req_sec_rdy,
          can_accept_uc,
          tag_match,
          evict_match,
          inflight_misses,
          inflight_reduce,
          going_to_uc_store,
          meta_write_req,
          writeback_req
        });
assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(0)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          writeback_pending,
          metawrite_pending,
          new_coh,
          // Replace req by the fields of the request that we want to see
          req.addr,
          req.set,
          req.cmd,
          req.typ,
          req.buf_id,
          req.way_en,
          req.tag_match,
          req.old_meta,
          req.cacheable,
          req.thread_id,
          req.mem_g,
          req.is_amo,
          req.is_msg,
          req.is_bypass
        });


endmodule

