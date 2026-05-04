// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache bank request queue entry.
//
// Holds a single request through the lifespan of that request including
// request side effects such as victims.  The entry is allocated at alloc_ag.
// All entries go to state Initial/Initial2.  At alloc_ad some entry types
// can transition to a state to do something.  Atomics, WriteArounds, and
// WritePartials stay in state Initial until a second reqq entry is allocated.
// A subsequent alloc_ag is the second paired entry.
//
// An entry is eligible (pipe_eligible or mesh_eligible) to go to the pipeline
// or mesh if the next state indicates there is work to do, the entry is head
// of its dependency list, its data is valid if needed, and it is not already
// in flight due to a bypass to mesh.  Picking the entry causes inflight to be
// set and the entry waits for a response from the pipeline or mesh.
//
// Responses cause state transitions to the next action.  Responses can also
// set rspmux_eligible indicating that a response can now be sent back to
// neighborhoods.
//
// When the neighborhood rsp has been sent rspmux_sent is set.
//
// Eventually no more actions are needed and the state transitions to Done.
//
// The reqq entry is deallocated when rspmux_sent, head of linked list, and
// Done.
//
// Replaces: shire_cache_bank_reqq_entry (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions (_i/_o/_d/_q)
//   - prim_rst_sync, prim_clk_gate primitives
//   - Package types instead of `define types
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF macros
//   - DFT consolidated into dft_t

/* verilator lint_off UNUSEDSIGNAL */  // packed struct ports passed whole; entry uses subfields only
module shirecache_bank_reqq_entry
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [ReqqIdSize-1:0]       my_reqq_id_i,

  // Requests to bank ag
  input  logic                        alloc_me_ag_i,       // I am being allocated
  input  reqq_alloc_t                 alloc_info_ag_i,
  input  logic                        alloc_any_ag_i,      // Someone is being allocated
  input  reqq_alloc_t                 alloc_any_info_ag_i,
  input  logic                        coalesce_victim_match_alloc_ag_i,

  // alloc ad
  input  logic                        alloc_me_ad_i,       // I was allocated (used for matching up paired_id)

  // dependency updating
  output logic                        clear_dep_behind_me_o, // This entry no longer needs to stall deps
  input  logic [NumEntries-1:0]       clear_dep_hot_i,       // Check if dep ahead of me cleared
  input  logic [NumEntries-1:0]       clear_dep_rbuf_1hot_pre_i, // Check if rbuf dep ahead of me cleared (pre-flop)

  // Victim dependency management
  input  logic                        tag_victim_dep_valid_i,
  input  logic [ReqqIdSize-1:0]       tag_victim_dep_id_i,
  input  logic                        tag_victim_dep_tail_i,
  input  logic                        tag_victim_insert_behind_me_i,   // Clear tail if new victim behind this victim
  input  logic                        tag_victim_insert_ahead_of_me_i, // Set dep_valid to new victim ahead of me
  input  logic                        tag_victim_upgrade_behind_me_i,  // New victim inserted in front of entry upgrading
  input  logic                        tag_victim_address_match_i,      // Matching the current victim
  input  logic                        upgrade_me_to_meshll_i,
  input  logic                        upgrade_ahead_of_me_to_meshll_i,

  // Coalescing buffer victims
  input  logic                        coalesce_victim_me_i,
  input  logic                        coalesce_victim_dep_valid_i,
  input  logic [ReqqIdSize-1:0]       coalesce_victim_dep_id_i,
  input  logic                        coalesce_victim_dep_non_meshll_head_i,
  input  logic                        coalesce_victim_insert_behind_me_i,

  // dataq status
  input  logic                        dataq_write_status_any_i,
  input  logic                        dataq_write_status_me_i,
  input  logic [ReqqIdSize-1:0]       dataq_write_status_id_i,
  input  logic                        dataq_read_status_me_i,

  // Interfaces with the pipeline
  output logic                        pipe_req_eligible_o,   // This req is eligible
  output reqq_pipe_req_t              pipe_req_info_me_o,
  input  logic                        pipe_req_picked_me_i,  // This req was picked
  input  logic [ReqqIdSize-1:0]       pipe_req_reqq_id_any_i,
  input  logic                        pipe_req_picked_any_i,

  input  logic                        pipe_tag_rsp_valid_any_i, // Someone is getting pipe rsp
  input  logic                        pipe_tag_rsp_valid_me_i,
  input  tag_rsp_t                    pipe_tag_rsp_info_i,
  input  logic                        pipe_data_rsp_valid_me_i,
  input  data_rsp_t                   pipe_data_rsp_info_i,
  input  logic                        pipe_cbuf_rsp_valid_any_i, // Someone is getting rsp
  input  cbuf_rsp_t                   pipe_cbuf_rsp_info_i,

  // Interfaces with rbuf
  output logic                        rbuf_req_eligible_o,   // This req is eligible
  output rbuf_rd_req_t                rbuf_req_info_me_o,
  input  logic                        rbuf_req_picked_me_i,  // This req was picked
  input  rbuf_state_t                 pipe_rbuf_state_i,
  input  rbuf_clear_req_t             pipe_rbuf_clear_req_info_i,

  // Interfaces with mesh (L3)
  output logic                        to_l3_mesh_clk_en_o,
  output logic                        to_l3_mesh_req_eligible_o,  // Req is eligible
  output reqq_mesh_req_t              to_l3_mesh_req_info_o,
  input  logic                        to_l3_mesh_req_picked_i,    // Req was picked
  input  logic                        to_l3_mesh_rsp_valid_me_i,
  input  logic                        to_l3_mesh_rsp_valid_any_i,
  input  reqq_mesh_rsp_t              to_l3_mesh_rsp_info_i,

  // Interfaces with mesh (Sys)
  output logic                        to_sys_mesh_clk_en_o,
  output logic                        to_sys_mesh_req_eligible_o, // Req is eligible
  output reqq_mesh_req_t              to_sys_mesh_req_info_o,
  input  logic                        to_sys_mesh_req_picked_i,   // Req was picked
  input  logic                        to_sys_mesh_rsp_valid_me_i,
  input  logic                        to_sys_mesh_rsp_valid_any_i,
  input  reqq_mesh_rsp_t              to_sys_mesh_rsp_info_i,

  input  logic                        mesh_bypassed_victim_i,     // Victim from pipeline already sent to mesh

  // Responses back to neighborhoods
  output logic                        rspmux_l2_eligible_o,
  output rspmux_t                     rspmux_l2_info_o,
  input  logic                        rspmux_l2_picked_i,

  // Responses back to L3 slave
  output logic                        rspmux_l3_eligible_o,
  output rspmux_t                     rspmux_l3_info_o,
  input  logic                        rspmux_l3_picked_i,

  input  logic                        rspmux_bypassed_data_rsp_i,
  input  logic                        rspmux_sent_valid_i,

  // Other higher priority ops
  input  logic                        reqq_2nd_pass_need_priority_i,
  input  logic                        reqq_idx_cop_need_priority_i,

  // Abort idx cop
  input  logic                        pipe_idx_cop_aborted_i,

  // All internal state
  output logic                        reqq_entry_valid_o,
  output reqq_entry_state_t           reqq_entry_state_o,

  // For DV
  output logic                        reqq_entry_alloc_o,
  output logic                        reqq_entry_dealloc_o,

  // err_logger detected an error
  input  logic                        set_reqq_err_detected_me_i,
  input  logic                        set_reqq_err_zero_data_me_i,

  // ESRs
  input  bank_esr_ctl_t               esr_ctl_i,
  input  l3_swizzle_ctl_t             esr_sc_l3_shire_swizzle_ctl_i,

  input  dft_t                        dft_i
);

  // ════════════════════════════════════════════════════════
  // Local parameters
  // ══��══════════════���══════════════════════════════════════

  // SC_CLK_GATE_DISABLE_REQQ_ENTRIES = 2 in original defines
  localparam int unsigned CgateDisReqqEntries = 2;

  // SC_REQQ_INFO_SIZE = max(ReqqIdSize, NeighCoopMaskSize+1) = max(6, 5) = 6
  localparam int unsigned ReqqInfoSize = (ReqqIdSize > (NeighCoopMaskSize + 1))
                                       ? ReqqIdSize : (NeighCoopMaskSize + 1);

  // ══���═════════════════════════════════════════════════════
  // Reset synchronizer
  // ═══════════════════��════════════════════════════════════

  logic rst_n;
  prim_rst_sync u_rst_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );

  // ══════════���═════════════════════════════════════════════
  // Clock gate
  // ════════════════���═══════════════════════════════════════

  logic valid;

  logic clock_en;
  assign clock_en = valid | alloc_me_ag_i | alloc_me_ad_i | (~rst_n)
                  | esr_ctl_i.esr_sc_clk_gate_disable[CgateDisReqqEntries];

  logic clk_gated;
  prim_clk_gate u_clk_gate (
    .clk_i  (clk_i),
    .en_i   (clock_en),
    .dft_i  (dft_i),
    .clk_o  (clk_gated)
  );

  // ═════════��══════════════════════════════════════════════
  // Immutable fields — captured at allocation
  // ═════════════════��══════════════════════════════════════

  logic [DvTransIdSize-1:0]      trans_id_q;
  logic [PortIdSize-1:0]         port_id_q;
  logic                          zero_data;
  logic [ReqqTagIdSize-1:0]      tag_id_q;
  logic [ReqqSourceSize-1:0]     source_q;
  logic [AxiAxqosSize-1:0]       qos_q;
  reqq_orig_opcode_e             orig_opcode_q;
  address_space_e                address_space_q;
  reqq_source_e                  reqq_source_q;
  logic [PaSize-1:0]             address;
  logic [LineQwSize-1:0]         qwen;
  logic [LineQwSize-1:0]         rspmux_qwen;
  sc_size_t                      size_q;
  reqq_opcode_e                  opcode;
  logic                          rspmux_sent;
  logic [ReqqInfoSize-1:0]       paired_id;
  logic [ReqqIdSize-1:0]         dataq_id;
  cache_level_e                  cache_level_start;
  cache_level_e                  cache_level_dest;
  logic                          scp_is_remote;
  logic [NeighCoopMaskSize-1:0]  readcoop_dest;
  logic                          wait_for_dataq;
  logic                          err_detected;
  logic                          traced_q;

  // To save flops cache_op levels are stored in the paired_id registers
  assign cache_level_start = cache_level_e'(paired_id[1:0]);
  assign cache_level_dest  = cache_level_e'(paired_id[3:2]);

  assign scp_is_remote     = paired_id[0];
  assign readcoop_dest     = paired_id[NeighCoopMaskSize:1];

  // ═���══════════════════════════════════════════════════════
  // Dependency tracking
  // ════════════════════���═══════════════════════════════════

  logic                          dep_valid;              // there is an entry ahead of me
  logic [ReqqIdSize-1:0]         dep_id;                 // entry ahead of me
  logic                          dep_tail;               // youngest at this address
  logic                          dep_head;
  assign dep_head = ~dep_valid;                          // oldest at this address
  logic                          dep_meshll_tail;        // youngest victim
  logic                          dep_non_meshll_head;    // oldest non-victim
  logic                          dep_in_meshll;          // in the meshll portion of the linked list

  // Inflight indicates the current opcode is already ongoing in the pipe or mesh
  logic inflight;

  logic entry_is_scp;
  assign entry_is_scp = (address_space_q == AddressScp);

  logic mesh_rsp_valid_me;
  assign mesh_rsp_valid_me = to_l3_mesh_rsp_valid_me_i | to_sys_mesh_rsp_valid_me_i;

  logic mesh_rsp_zero_data;
  assign mesh_rsp_zero_data = (to_l3_mesh_rsp_valid_me_i  & to_l3_mesh_rsp_info_i.zero_data)
                            | (to_sys_mesh_rsp_valid_me_i & to_sys_mesh_rsp_info_i.zero_data);

  logic pipe_tag_rsp_decode_err;
  assign pipe_tag_rsp_decode_err = (pipe_tag_rsp_info_i.err_rsp != ErrNone);

  // ═���═══════════════════════════��══════════════════════════
  // Valid logic
  // ════════════════════���═══════════════════════════════════
  //
  // Dealloc happens after state Done, response sent back, and head of list.
  // Also wait for pipe to read dataq if needed.

  logic valid_d;
  logic valid_en;
  logic dont_depend_on_me;
  logic setting_dont_depend_on_me;

  always_comb begin
    valid_d  = 1'b0;
    valid_en = 1'b0;
    if (alloc_me_ag_i) begin
      valid_d  |= 1'b1;
      valid_en |= 1'b1;
    end
    if (valid & rspmux_sent
        & (opcode == ReqqDone)
        & ~wait_for_dataq
        & (setting_dont_depend_on_me | dont_depend_on_me)
        & ~dep_valid) begin
      // valid_d |= 1'b0 is implicit (default)
      valid_en |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n) begin
      valid <= '0;
    end else if (valid_en) begin
      valid <= valid_d;
    end
  end

  assign reqq_entry_valid_o   = valid;
  assign reqq_entry_alloc_o   = valid_en & valid_d;
  assign reqq_entry_dealloc_o = valid_en & ~valid_d;

  // ════════════════════════════════��═══════════════════════
  // Constants set at allocation (EN_FF: gated clock, enable, no reset)
  // ════════════════════════════════════════════════════════

  always_ff @(posedge clk_gated) begin
    if (alloc_me_ag_i) begin
      tag_id_q        <= alloc_info_ag_i.tag_id;
      source_q        <= alloc_info_ag_i.source;
      trans_id_q      <= alloc_info_ag_i.trans_id;
      port_id_q       <= alloc_info_ag_i.port_id;
      qos_q           <= alloc_info_ag_i.qos;
      orig_opcode_q   <= alloc_info_ag_i.orig_opcode;
      size_q          <= alloc_info_ag_i.size;
      address_space_q <= alloc_info_ag_i.address_space;
      reqq_source_q   <= alloc_info_ag_i.reqq_source;
      traced_q        <= alloc_info_ag_i.traced;
    end
  end

  // ═���══════════════════════════════════════════════════════
  // Subbank calculation with L3 swizzle
  // ════════════════════════════════════════════════════════

  logic [SubBankIdSize-1:0] subbank;

  // L3 swizzle address decode at allocation time
  logic [SubBankIdSize-1:0]   l3_sub_bank_id;
  logic [BankIdSize-1:0]      l3_bank_id_unused;
  logic [L3ShireIdSize-1:0]   l3_shire_id_unused;

  shirecache_pipe_l3_swizzle_get u_l3_swizzle_get (
    .address_i      (alloc_info_ag_i.address),
    .swizzle_ctl_i  (esr_sc_l3_shire_swizzle_ctl_i),
    .sub_bank_id_o  (l3_sub_bank_id),
    .bank_id_o      (l3_bank_id_unused),
    .shire_id_o     (l3_shire_id_unused)
  );

  // Calculate the subbank destination at allocation
  // Or when WriteAround2 becomes a coalesce victim
  logic [SubBankIdSize-1:0] subbank_d;
  assign subbank_d =
    (alloc_info_ag_i.address_space == AddressL3)
      ? l3_sub_bank_id :
    (coalesce_victim_me_i)
      ? pipe_cbuf_rsp_info_i.victim_address[SubBankLsb +: SubBankIdSize] :
    alloc_info_ag_i.address[SubBankLsb +: SubBankIdSize];

  always_ff @(posedge clk_gated) begin
    if (alloc_me_ag_i | coalesce_victim_me_i) begin
      subbank <= subbank_d;
    end
  end

  // ═══════════��══════════════════════════��═════════════════
  // Opcode state machine — next-state signals
  // ════════════════════���═══════════════════════════════════

  logic [PaSize-1:0]             address_d;
  logic [LineQwSize-1:0]         qwen_d;
  logic [$bits(reqq_opcode_e)-1:0] opcode_d;
  logic [ReqqInfoSize-1:0]       paired_id_d;
  logic [ReqqIdSize-1:0]         dataq_id_d;
  logic                          zero_data_d;

  logic                          address_en;
  logic                          qwen_en;
  logic                          opcode_en;
  logic                          paired_id_en;
  logic                          dataq_id_en;
  logic                          zero_data_en;

  // rspmux_eligible happens in parallel with next opcode state changes.
  // Setting rspmux_eligible often happens when transitioning to state done.
  // Sometimes rspmux can happen earlier (writes with victims, fills with victims).
  // Therefore, this state machine indicates that rspmux_eligible can be set.
  // rspmux_eligible and rspmux_sent tracking happens further down.
  logic set_rspmux_eligible;

  // set_wait_for_dataq is set when we are possibly transitioning to Reqq_Done, but
  // before we can deallocate, we need to see that the dataq was read by the pipeline.
  // Set to indicate that we need to wait for a dataq read to deallocate.
  // Clr when the dataq read is seen.
  // This prevents the reqq from reallocating a previous Write/Fill entry too quickly
  // if the pipe is running with slow rams.
  // Wait_for_dataq is also used for allocation errors to make sure that the write
  // has happened before deallocating the entry.
  logic set_wait_for_dataq;

  // data_ready indicates that the dataq has written data to ram and it can now be read out
  logic data_ready;

  // set_data_ready is used for SC_L3_FlushWData and SC_L3_EvictWData when the request
  // misses but needs to forward the Cop data to the next level
  logic set_data_ready;

  // set_err_imprecise indicates the error detected is past the point of being able to
  // be returned with the response.  Instead signal global error and interrupt.
  logic set_err_imprecise;

  // set_rspmux_sent if dont need to response for Atomic2, WriteAround2, Cop FSMs
  logic set_rspmux_sent;

  always_comb begin
    address_d               = '0;
    qwen_d                  = '0;
    opcode_d                = '0;
    paired_id_d             = '0;
    dataq_id_d              = '0;
    zero_data_d             = '0;

    address_en              = 1'b0;
    qwen_en                 = 1'b0;
    opcode_en               = 1'b0;
    paired_id_en            = 1'b0;
    dataq_id_en             = 1'b0;
    zero_data_en            = 1'b0;

    set_rspmux_eligible     = 1'b0;
    set_wait_for_dataq      = 1'b0;
    set_data_ready          = 1'b0;
    set_rspmux_sent         = 1'b0;
    set_err_imprecise       = 1'b0;

    // Allocate me
    if (alloc_me_ag_i) begin
      address_d             |= alloc_info_ag_i.address;
      qwen_d                |= alloc_info_ag_i.qwen;
      opcode_d              |= (alloc_info_ag_i.err_valid) ? ReqqAllocErr : alloc_info_ag_i.opcode;

      paired_id_d           |=
        // hold paired id
        ((alloc_info_ag_i.opcode == ReqqInitial2) |
         alloc_info_ag_i.orig_opcode inside {OrigWriteAround2, OrigWritePartial2, OrigAtomic2}) ? ReqqInfoSize'(alloc_info_ag_i.paired_id) :
        (alloc_info_ag_i.orig_opcode inside {OrigScpRead, OrigScpWrite, OrigScpWritePartial})   ? ReqqInfoSize'(alloc_info_ag_i.scp_is_remote) :
        (alloc_info_ag_i.orig_opcode inside {OrigReadCoop, OrigScpReadCoop}) ? ReqqInfoSize'({alloc_info_ag_i.readcoop_dest, alloc_info_ag_i.scp_is_remote}) :
        // or overload with cacheop start and end level
        ReqqInfoSize'({alloc_info_ag_i.cache_level_dest, alloc_info_ag_i.cache_level_start});

      dataq_id_d            |= my_reqq_id_i;
      zero_data_d           |= alloc_info_ag_i.zero_data;

      address_en            |= 1'b1;
      qwen_en               |= 1'b1;
      opcode_en             |= 1'b1;
      paired_id_en          |=
        ( (alloc_info_ag_i.opcode == ReqqInitial2) |
          (alloc_info_ag_i.orig_opcode inside
             {OrigWriteAround2, OrigWritePartial2, OrigAtomic2,    // really a paired_id
              OrigCopFlush, OrigCopEvict, OrigCopPrefetch, // overloaded with start and dest levels
              OrigScpRead, OrigScpWrite, OrigScpWritePartial, // overloaded with remote or local scp
              OrigReadCoop, OrigScpReadCoop,    // overloaded with multi-hot destination
              OrigIdx})); // overloaded with cache level to direct port to send flush/evict to
      dataq_id_en           |= 1'b1;
      zero_data_en          |= 1'b1;
    end

    // If alloc decode error or err valid then just send response and complete
    if (valid &
        (~wait_for_dataq | data_ready) &
        ((opcode == ReqqAllocErr) | (orig_opcode_q == OrigErr))) begin
      opcode_en             |= 1'b1;
      opcode_d              |= ReqqDone;
      set_rspmux_eligible   |= ~rspmux_sent;
    end

    // Normal L2 and L3 operation without L2 or L3 bypass
    // Scp and MsgSendData operation
    // Idx CacheOp operation regardless of L2 or L3 bypass
    // --------------------------------------------------------------------------
    if (valid &
        ~((opcode == ReqqAllocErr) | (orig_opcode_q == OrigErr)) &
        ~(esr_ctl_i.esr_sc_l2_bypass & (address_space_q == AddressL2) & ~(reqq_source_q == ReqqSrcFsm))  &
        ~(esr_ctl_i.esr_sc_l3_bypass & (address_space_q == AddressL3) & ~(reqq_source_q == ReqqSrcFsm))) begin
      /* verilator lint_off CASEINCOMPLETE */  // only valid {address_space, orig_opcode} pairs handled
      case ({address_space_q, orig_opcode_q})

        // L2 address
        {AddressL2, OrigRead},
        {AddressL2, OrigReadCoop}: begin
          // If just got started (initializing, then go to state L2_Read)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2Read;
          end
          if (opcode == L2Read) begin
            // Rbuf hit.  We're done.  The response will automatically get
            // sent eventually from the rbuf fifo.  We do need to wait until
            // after the response is sent to deallocate.
            if (rbuf_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Miss. If miss then we can transition to sending the request to the mesh.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
            // Hit WriteAround.
            // If read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we go to L2_Read which will miss and refetch the full line.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error. Done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit. Done and response can be sent back.
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_data_rsp_valid_me_i & (pipe_data_rsp_info_i.rsp_type == NormalRsp)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the read hit WriteAround data, go read it back
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
            end
          end
          // Fill, when we get the mesh response send the fill down the pipeline
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              zero_data_en          |= 1'b1;
              zero_data_d           |= mesh_rsp_zero_data;
              set_err_imprecise     |= 1'b1;
            end
          end
          if (opcode == L2Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            // Maintain the sub-line address bits from original request in order to
            // create read partial response qwens.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL2, OrigReadForward}: begin
          // If just got started (initializing, then go to Mesh_Read)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshRead;
          end
          // Forward L3 Read, when we get the mesh response, we're done
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL2, OrigWrite}: begin
          // If just got started (initializing, then go to state L2_Write)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2Write;
          end
          if (opcode == L2Write) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // There is no victim, we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // There is a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        // WritePartial
        {AddressL2, OrigWritePartial}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2WritePartial;
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
          end
          if (opcode == L2WritePartial) begin
            // Miss. If miss then fill the line from next level cache
            // The miss request is generated by this reqq but the reqq_id
            // on the mesh read is the paired id.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Hit WriteAround.
            // If writePartial hits a writeAround, the pipeline signals a tag hit and evicts the line.
            // When L3 Write returns, we go and refetch the full line.
            // The write request is generated by this reqq but the reqq_id
            // on the mesh write is the paired id.
            // Address has to be retained when victimizing writeAround but the qwen has to
            // be captured for the victim and then regenerated from size and address after the
            // WriteAround victim is flushed.
            // The write address is fixed up be cache aligned on the way to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit. Done and response can be sent back.
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= my_reqq_id_i;
              set_wait_for_dataq    |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the writeAround data, go read the line back
          // The miss request is generated by this reqq but the reqq_id
          // on the mesh read is the paired id.
          // Reproduce the WritePartial qwen from size and address
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
          end
          // Fill, when we get the mesh response, wait for paired reqq to
          // issue the fill to the pipeline.
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == ReqqWait) begin
            // Wait. If paired reqq is picked to send fill request down the
            // pipe, we transition to L2WritePartial2nd and this time
            // we should make it
            // The opcode to the pipeline has to be fixed back to
            // L2WritePartial but internally kept as L2WritePartial2nd
            if (pipe_req_picked_any_i &
                (pipe_req_reqq_id_any_i == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2WritePartial2nd;
              dataq_id_d            |= my_reqq_id_i;
              dataq_id_en           |= 1'b1;
            end
          end
          if (opcode == L2WritePartial2nd) begin
            // Picked, L2WritePartial2nd is used to set the arbitration
            // priority to the pipeline and so the 2nd_pass is guaranteed to hit
            // because it followed the fill request.
            // After it is picked, return opcode back to L2WritePartial.
            if (pipe_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2WritePartial;
            end
          end
        end
        {AddressL2, OrigWritePartial2}: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing in case we need to fill or hold eviction
          // data. If not needed can deallocate early.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            // Hit. If paired hit we are done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Miss. If paired miss then we can transition to waiting on the response from the mesh.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               ~pipe_tag_rsp_decode_err &
               ~pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // Hit WriteAround.
            // If paired read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait2;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // Fill, when we get the mesh response, send the fill down the pipeline from this paired entry.
          if (opcode == ReqqWait) begin
            if (to_l3_mesh_rsp_valid_any_i & (to_l3_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_l3_mesh_rsp_info_i.zero_data;
            end
            // If getting a to_sys response because to_l3 was forced to_sys
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
            end
          end
          // Hit WriteAround, when we get the mesh response, go read it
          // Other reqq issues the write request and swap in paired id to mesh
          if (opcode == ReqqWait2) begin
            if (to_l3_mesh_rsp_valid_any_i & (to_l3_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // If getting a to_sys response because to_l3 was forced to_sys
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == L2Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL2, OrigWriteForward},
        {AddressL2, OrigWriteForwardPart}: begin
          // If just got started (initializing, then go to state Mesh_Write)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshWrite;
          end
          // Forward L3 Write, when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end

        {AddressL2, OrigCopLock}: begin
          // If just got started (initializing, then go to state L2_Lock)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2Lock;
          end
          if (opcode == L2Lock) begin
            // There is no victim, we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // There is a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Lock victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL2, OrigCopUnlockInv}: begin
          // Send it down the pipe and we are done
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2UnlockInv;
          end
          if (opcode == L2UnlockInv) begin
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigCopUnlock}: begin
          // Send it down the pipe and may get a victim
          // Send it down the pipe and we are done
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2Unlock;
          end
          if (opcode == L2Unlock) begin
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigCopEvict}: begin
          // If just got started (initializing, then go to state L2_Evict or the Mesh)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            if (cache_level_start inside {CacheL1, CacheL2}) begin
              opcode_d            |= L2Evict;
            end
            // If Evicting from cache above us, then forward the request on to the mesh if there is a next level cache
            else if ( esr_ctl_i.esr_sc_remote_l3_enable) begin
              opcode_d              |= MeshEvict;
            end
            // Else Done
            else begin
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          if (opcode == L2Evict) begin
            // There is miss or no dirty data, then send to the next cache level or we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim & ~pipe_tag_rsp_decode_err) begin
              opcode_en               |= 1'b1;
              if ((cache_level_dest inside {CacheMem}) &
                 esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then send cacheop on to L3, else done
                opcode_d              |= MeshEvict;
              end
              else begin
                opcode_d              |= ReqqDone;
                set_rspmux_eligible   |= 1'b1;
              end
            end
            // Hit dirty.  Write to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              if ((cache_level_dest inside {CacheMem}) &
                 esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then evict to l3, else send write to mem (to_sys)
                opcode_d            |= MeshEvictToMem;
              end
              else begin
                opcode_d            |= MeshWrite;
              end
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error.  We're done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Evict victim, when we get the mesh response, we're done
          // Cop FSM evict cbuf does not respond to neigh
          if (opcode inside {MeshWrite, MeshEvict, MeshEvictToMem}) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigCopFlush}: begin
          // If just got started (initializing, then go to state L2_Flush or the Mesh)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            if (cache_level_start inside {CacheL1, CacheL2}) begin
              opcode_d            |= L2Flush;
            end
            // If Flushing from cache above us, then forward the request on to the mesh if there is a next level cache
            else if ( esr_ctl_i.esr_sc_remote_l3_enable) begin
              opcode_d            |= MeshFlush;
            end
            // Else Done
            else begin
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          if (opcode == L2Flush) begin
            // There is miss or no dirty data, then send to the next cache level or we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              if ((cache_level_dest inside {CacheMem}) &
                 esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then send flush to l3, else done
                opcode_d            |= MeshFlush;
              end
              else begin
                opcode_d            |= ReqqDone;
                set_rspmux_eligible |= 1'b1;
              end
            end
            // Hit dirty.  Write to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              if ((cache_level_dest inside {CacheMem}) &
                 esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then send flush to l3, else write to mem (to_sys)
                opcode_d            |= MeshFlushToMem;
              end
              else begin
                opcode_d            |= MeshWrite;
              end
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error.  We're done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode inside {MeshWrite, MeshFlush, MeshFlushToMem}) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigCopPrefetch}: begin
          // If just got started (initializing, then go to state L2_Prefetch or Mesh)
          if (opcode == ReqqInitial) begin
            opcode_en               |= 1'b1;
            // If dest level is L2, then send the prefetch to the pipe
            if (cache_level_dest == CacheL2) begin
              opcode_d              |= L2Prefetch;
            end
            else if (cache_level_dest == CacheL3 & esr_ctl_i.esr_sc_remote_l3_enable) begin
            // If dest level is L3 or MEM then send the prefetch to the mesh
              opcode_d              |= MeshPrefetch;
            end
            // Dest level Mem doesn't mean anything and is just turned around.
            else begin
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          if (opcode == L2Prefetch) begin
            // Rbuf hit, we're done
            if (rbuf_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit, we're done (full hit)
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit WriteAround.
            // If read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When Mesh_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Miss. Go get it.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
            // Decode Error, we're done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the read hit WriteAround data, go read it back
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
            end
          end
          // Fill, when we get the mesh response send the fill down the pipeline
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              zero_data_en          |= 1'b1;
              zero_data_d           |= mesh_rsp_zero_data;
            end
          end
          if (opcode == L2Fill) begin
            // Once picked for the pipe, send response back
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // If dest level is L3 or MEM then sent the prefetch to the mesh
          // Receive acknowledges for Prefetches above our level forwarded to the Mesh
          if (opcode == MeshPrefetch) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigWriteAround}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            if (esr_ctl_i.esr_sc_cbuf_enable) begin
              opcode_d            |= L2WriteAround;
            end
            else begin
              opcode_d            |= MeshWrite;
            end
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
          end
          if (opcode == L2WriteAround) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // If there is no victim (hit or miss), then we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // If there is a victim, send it back to Mesh
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // If CB not enabled, we forward the write to the mesh and when
          // mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL2, OrigWriteAround2}: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing until we're told to handle
          // a cbuf buffer evict or not and deallocate.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            if (~esr_ctl_i.esr_sc_cbuf_enable) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // No coalescing buffer evict. We're done.
            if (pipe_cbuf_rsp_valid_any_i &
               (pipe_cbuf_rsp_info_i.reqq_id == paired_id) & ~pipe_cbuf_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // A coalescing buffer evict.  Handle it.
            if (pipe_cbuf_rsp_valid_any_i &
               (pipe_cbuf_rsp_info_i.reqq_id == paired_id) &  pipe_cbuf_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Evict;
              address_en            |= 1'b1;
              address_d             |= pipe_cbuf_rsp_info_i.victim_address;
            end
            // Decode error for paired entry, done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          if (opcode == L2Evict) begin
            // There is miss or no dirty data, we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Hit dirty.  Write to mesh.
            // The Evict address is unchanged and so dont need to update from pipe_tag_rsp_info
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL2, OrigAtomic}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2Atomic;
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
          end
          if (opcode == L2Atomic) begin
            // Miss. If miss then we can transition to sending the request to the mesh.
            // The miss request is generated by this reqq but the reqq_id
            // on the mesh read is the paired id.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Hit WriteAround.
            // If atomic hits a writeAround, the pipeline signals a tag hit and evicts the line.
            // When L3 Write returns, we go and refetch the full line.
            // The write request is generated by this reqq but the reqq_id
            // on the mesh write is the paired id.
            // Atomic address has to be retained when victimizing writeAround but the qwen has to
            // be captured. The write address is fixed up to be cache aligned on the way to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit. Done and response can be sent back.
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_data_rsp_valid_me_i & (pipe_data_rsp_info_i.rsp_type == NormalRsp)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= my_reqq_id_i;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the read hit WriteAround data, go read it back
          // The miss request is generated by this reqq but the reqq_id
          // on the mesh read is the paired id.
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
          end
          // Fill, when we get the mesh response, wait for paired reqq to
          // issue the fill to the pipeline.
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == ReqqWait) begin
            // Wait. If paired reqq is picked to send fill request down the
            // pipe, we transition to L2Atomic2nd and this time
            // we should make it
            // The opcode to the pipeline has to be fixed back to
            // L2Atomic but internally kept as L2Atomic2nd
            if (pipe_req_picked_any_i &
                (pipe_req_reqq_id_any_i == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Atomic2nd;
              dataq_id_d            |= my_reqq_id_i;
              dataq_id_en           |= 1'b1;
            end
          end
          if (opcode == L2Atomic2nd) begin
            // Picked, L2Atomic2nd is used to set the arbitration
            // priority to the pipeline and is guarantee to hit
            // because it followed the fill request.
            // After it is picked, return opcode back to L2Atomic.
            if (pipe_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Atomic;
            end
          end
        end
        {AddressL2, OrigAtomic2}: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing in case we need to fill or hold eviction
          // data. If not needed can deallocate early.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            // Hit. If paired hit we are done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Miss. If paired miss then we can transition to waiting on the response from the mesh.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               ~pipe_tag_rsp_decode_err &
               ~pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // Hit WriteAround.
            // If paired read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait2;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // Fill, when we get the mesh response, send the fill down the pipeline from this paired entry.
          if (opcode == ReqqWait) begin
            if (to_l3_mesh_rsp_valid_any_i & (to_l3_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_l3_mesh_rsp_info_i.zero_data;
            end
            // If getting a to_sys response because to_l3 was forced to_sys
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L2Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
            end
          end
          // Hit write WriteAround, when we get the mesh response, go read it
          // Other reqq issues the write request and swap in paired id to mesh
          if (opcode == ReqqWait2) begin
            if (to_l3_mesh_rsp_valid_any_i & (to_l3_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // If getting a to_sys response because to_l3 was forced to_sys
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == L2Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressScp, OrigCopScpFill}: begin
          // If just got started (initializing, then go to state Mesh_Read)
          // Set qwen to 4'b1 because only qw[0] is needed for the fill address.
          if (opcode == ReqqInitial) begin
            opcode_en               |= 1'b1;
            opcode_d                |= MeshRead;
            qwen_en                 |= 1'b1;
            qwen_d                  |= 4'b1;
          end
          // When we get the mesh response send the scp fill down the pipeline
          // Set qwen back to line
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ScpFill;
              zero_data_en          |= 1'b1;
              zero_data_d           |= mesh_rsp_zero_data;
              qwen_en               |= 1'b1;
              qwen_d                |= 4'hf;
            end
          end
          if (opcode == ScpFill) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // There is never a victim, we're done
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressScp, OrigScpWrite}: begin
          // If just got started (initializing, then go to state SC_Scp_Write, Mesh_Write or Reqq_Done)
          if (opcode == ReqqInitial) begin
            // If this is an L2 request of a remote shire, send it to the mesh
            if ((reqq_source_q == ReqqSrcL2) & scp_is_remote) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
            end
            else begin
              opcode_en             |= 1'b1;
              opcode_d              |= ScpWrite;
            end
          end
          if (opcode == ScpWrite) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // There is never a victim, we're done
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // When we get the mesh response, then done.
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressScp, OrigScpWritePartial}: begin
          // If just got started (initializing, then go to state SC_Scp_Write, Mesh_Write or Reqq_Done)
          if (opcode == ReqqInitial) begin
            // If this is an L2 request of a remote shire, send it to the mesh
            if ((reqq_source_q == ReqqSrcL2) & scp_is_remote) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
            end
            else begin
              opcode_en             |= 1'b1;
              opcode_d              |= ScpWritePartial;
            end
          end
          if (opcode == ScpWritePartial) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // There is never a victim, we're done
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // When we get the mesh response, then done.
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressScp, OrigScpRead},
        {AddressScp, OrigScpReadCoop}: begin
          // If just got started (initializing, then go to state SC_Scp_Read or Mesh_Read or Reqq_Done)
          if (opcode == ReqqInitial) begin
            opcode_en               |= 1'b1;
            // If this is an L2 request of a remote shire, send it to the mesh
            if ((reqq_source_q == ReqqSrcL2) & scp_is_remote) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
            else begin
              opcode_en             |= 1'b1;
              opcode_d              |= ScpRead;
            end
          end
          if (opcode == ScpRead) begin
            // Rbuf hit.  We're done.  The response will automatically get
            // sent eventually from the rbuf fifo.  We do need to wait until
            // after the response is sent to deallocate.
            if (rbuf_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // If miss because of decode error like incorrect target shire id
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // If hit we're done
            if (pipe_data_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // When we get the mesh response, then done.  Read data went back via fill path.
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressScp, OrigAtomic}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= ScpAtomic;
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
            // Scp atomic is treated as imprecise unless we can send error on the AXI write with the AtomicRsp.
            set_err_imprecise     |= 1'b1;
            // The Acknowledge back to the L3_Slave can happen immediately.
            // Later a new write requests to a UC Atomic response ESR will return the atomic data.
            set_rspmux_eligible   |= 1'b1;
          end
          if (opcode == ScpAtomic) begin
            // If Miss because of decode error like incorrect target shire id,
            // there is no way to pass error info thr the atomic response.
            // Pipe updates the error logger and Reqq still has to perform the atomic response
            // If Hit. Done and response can be sent back via to_sys mesh write request.
            // Wait for data_rsp to guarantee data is in dataq because pipe has priority
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_tag_rsp_valid_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshAtomicRsp;  // Scp atomic response write to ESR
              qwen_en               |= 1'b1;
              qwen_d                |= 4'b0111;        // Read atomic id from qw[2]
              set_data_ready        |= pipe_tag_rsp_decode_err;
            end
          end
          // The final atomic response was sent to UC ESR with a Mesh AtomicRsp.
          // Upon response we're done.
          if (opcode == MeshAtomicRsp) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressScp, OrigAtomic2}: begin
          // Free up the Reqq immediately
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= ReqqDone;
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
        end
        // Idx Flush, Evict, Inv CacheOps
        {AddressIdx, OrigIdx},
        {AddressScp, OrigIdx},
        {AddressL2,  OrigIdx} : begin
          // These already started with opcode set to the desired pipe opcode at alloc.
          // Note that copfsm evict cbuf is treated as l2 space and not idx space
          // Wait for tag response.
          // If there is no victim, we're done
          if (alloc_me_ad_i) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          if (opcode inside {
            IdxRead,
            IdxWrite,
            IdxAllInv,
            IdxL2Inv,
            IdxL2Flush,
            IdxL2Evict,
            IdxL3Inv,
            IdxL3Flush,
            IdxL3Evict,
            ReqqWait,
            L2Evict,
            ScpZero
          }) begin
            // Cop FSM abort
            // When the cop FSM abort is seen, we need to wait a cycle to make sure the
            // reqq did not get picked since the pipe_req_picked_me is a flop version
            // by transitioning to IdxAbort. If pipe_req_picked_me is not set, then
            // the abort takes effect, else go to ReqqWait state to finish the
            // Idx operation. The opcode is overwritten when we transition
            // to IdxAbort and ReqqWait when an abort is detected, the
            // assumption is that the abort is either going to take effect or the
            // Reqq has been picked with the original opcode to go down the pipe
            if (pipe_idx_cop_aborted_i & ~dep_valid & ~(pipe_req_picked_me_i | inflight)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= IdxAbort;
            end
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // There is a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Wait for 1 cyc to see if picked
          if (opcode == IdxAbort) begin
            if (~pipe_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            else begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          // CacheOp flush / evict, when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        // A Sync once allocated just finishes.
        {AddressIdx, OrigSync} : begin
          if (alloc_me_ad_i) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
            opcode_en             |= 1'b1;
            opcode_d              |= ReqqDone;
          end
        end
        {AddressMsg, OrigMsgSendData}: begin
          // If just got started (initializing, then go to state SC_L2_MsgSendData)
          if (alloc_me_ad_i) begin
            set_err_imprecise     |= 1'b1;
          end
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L2MsgSendData;
          end
          if (opcode == L2MsgSendData) begin
            if (data_ready & dep_head) begin
              opcode_en           |= 1'b1;
              opcode_d            |= ReqqWait;
              set_rspmux_eligible |= 1'b1;
            end
          end
          if (opcode == ReqqWait) begin
            if (rspmux_l2_picked_i) begin
              opcode_en           |= 1'b1;
              opcode_d            |= ReqqDone;
            end
          end
        end

        // L3 address
        {AddressL3, OrigRead}: begin
          // If just got started (initializing, then go to state L3_Read)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Read;
          end
          if (opcode == L3Read) begin
            // Miss. If miss then we can transition to sending the request to the mesh.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
            // Hit WriteAround.
            // If read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When Mem_Write returns, we go to L3_Read which will miss and refetch the full line.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit. Done and response can be sent back.
            // Wait for data_rsp to guarantee data is in dataq because pipe has priority
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_data_rsp_valid_me_i & (pipe_data_rsp_info_i.rsp_type == NormalRsp)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the read hit WriteAround data, go read it back
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
            end
          end
          // Fill, when we get the mesh response send the fill down the pipeline
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Fill;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
              set_err_imprecise     |= 1'b1;
            end
          end
          if (opcode == L3Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL3, OrigWrite}: begin
          // If just got started (initializing, then go to state L3_Write)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Write;
          end
          if (opcode == L3Write) begin
            // Pipe_req_picked send the response back to neighborhood
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // There is no victim, we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // There is a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        // WritePartial
        {AddressL3, OrigWritePartial}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3WritePartial;
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
          end
          if (opcode == L3WritePartial) begin
            // Miss. If miss then fill the line from next level cache
            // The miss request is generated by this reqq but the reqq_id
            // on the mesh read is the paired id.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Hit Partial.
            // If writePartial hits a partial, the pipeline signals a tag hit and evicts the line.
            // When Mem Write returns, we go and refetch the full line.
            // The write request is generated by this reqq but the reqq_id
            // on the mesh write is the paired id.
            // Address has to be retained when victimizing partial but the qwen has to
            // be captured for the victim and then regenerated from size and address after the
            // WriteAround victim is flushed.
            // The write address is fixed up be cache aligned on the way to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit. Done and response can be sent back.
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= my_reqq_id_i;
              set_wait_for_dataq    |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the writeAround data, go read the line back
          // The miss request is generated by this reqq but the reqq_id
          // on the mesh read is the paired id.
          // Reproduce the WritePartial qwen from size and address
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
          end
          // Fill, when we get the mesh response, wait for paired reqq to
          // issue the fill to the pipeline.
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == ReqqWait) begin
            // Wait. If paired reqq is picked to send fill request down the
            // pipe, we transition to L3WritePartial2nd and this time
            // we should make it
            // The opcode to the pipeline has to be fixed back to
            // L3WritePartial but internally kept as L3WritePartial2nd
            if (pipe_req_picked_any_i &
                (pipe_req_reqq_id_any_i == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3WritePartial2nd;
              dataq_id_d            |= my_reqq_id_i;
              dataq_id_en           |= 1'b1;
            end
          end
          if (opcode == L3WritePartial2nd) begin
            // Picked, L3WritePartial2nd is used to set the arbitration
            // priority to the pipeline and so the 2nd_pass is guaranteed to hit
            // because it followed the fill request.
            // After it is picked, return opcode back to L3WritePartial.
            if (pipe_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3WritePartial;
            end
          end
        end
        {AddressL3, OrigWritePartial2}: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing in case we need to fill or hold eviction
          // data. If not needed can deallocate early.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            // Hit. If paired hit we are done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Miss. If paired miss then we can transition to waiting on the response from the mesh.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               ~pipe_tag_rsp_decode_err &
               ~pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // Hit WriteAround.
            // If paired read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait2;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // Fill, when we get the mesh response, send the fill down the pipeline from this paired entry.
          if (opcode == ReqqWait) begin
            // If getting a to_sys response
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
            end
          end
          // Hit WriteAround, when we get the mesh response, go read it
          // Other reqq issues the write request and swap in paired id to mesh
          if (opcode == ReqqWait2) begin
            // If getting a to_sys response
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == L3Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL3, OrigCopEvict}: begin
          // If just got started
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Evict;
          end
          if (opcode == L3Evict) begin
            // There is miss or no dirty data, then we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit dirty.  Write to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Evict victim, when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL3, OrigCopEvictWData}: begin
          // If just got started
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3EvictWData;
          end
          if (opcode == L3EvictWData) begin
            // If miss, then write the data to the Mem.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              set_data_ready        |= 1'b1;
            end
            // If hit, then grab merged qwen
            if (pipe_tag_rsp_valid_me_i &  pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // When Mesh response comes back, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL3, OrigCopFlush}: begin
          // If just got started
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Flush;
          end
          if (opcode == L3Flush) begin
            // There is miss or no dirty data, then we're done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit dirty.  Write to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Flush victim, when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL3, OrigCopFlushWData}: begin
          // If just got started
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3FlushWData;
          end
          if (opcode == L3FlushWData) begin
            // If miss, then write the data to the Mem.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              set_data_ready        |= 1'b1;
            end
            // If hit, then grab merged qwen
            if (pipe_tag_rsp_valid_me_i &  pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // When Mesh response comes back, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        {AddressL3, OrigCopPrefetch}: begin
          // If just got started
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Prefetch;
          end
          if (opcode == L3Prefetch) begin
            // Hit, we're done (full hit)
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Hit WriteAround.
            // If read hits a WriteAround, the pipeline signals a tag hit and evicts the line.
            // When Mesh_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Miss. Go get it.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
            // Decode error, we're done
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // After evicting the read hit WriteAround data, go read it back
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
            end
          end
          // Fill, when we get the mesh response send the fill down the pipeline
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Fill;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
            end
          end
          if (opcode == L3Fill) begin
            // Once picked for the pipe, send response back
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
              set_err_imprecise     |= 1'b1;
              set_rspmux_eligible   |= 1'b1;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        {AddressL3, OrigAtomic}: begin
          // Stay in state initializing until we see a second allocate.
          // The reqq_entry being allocated is the second half pair.
          if (((opcode == ReqqInitial) & (alloc_any_ag_i)) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= L3Atomic;
            if (opcode == ReqqInitial) begin
              paired_id_en        |= 1'b1;
              paired_id_d         |= alloc_any_info_ag_i.reqq_id;
            end
            // FEATURE: L3 atomic is treated as imprecise unless we can send error on the AXI write with the AtomicRsp.
            set_err_imprecise     |= 1'b1;
            // The Acknowledge back to the L3_Slave can happen immediately.
            // Later a new write requests to a UC Atomic response ESR will return the atomic data.
            set_rspmux_eligible   |= 1'b1;
          end
          if (opcode == L3Atomic) begin
            // Miss. If miss then we can transition to sending the request to the mesh.
            // The miss request is generated by this reqq but the reqq_id
            // on the mesh read is the paired id.
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Hit partial.
            // If atomic hits a partial, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we Mesh Read to refetch the full line.
            // The write request is generated by this reqq but the reqq_id
            // on the mesh write is the paired id.
            // Atomic address has to be retained when victimizing hit partial but the qwen has to
            // be captured. The write address is fixed up to be cache aligned on the way to mesh.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshWrite;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= paired_id;
            end
            // Hit. Done and response can be sent back via to_sys mesh write request.
            // Wait for data_rsp to guarantee data is in dataq because pipe has priority
            // Use data_rsp in order to wait until last pipe signal before going to done.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshAtomicRsp;  // L3 atomic response write to ESR
              qwen_en               |= 1'b1;
              qwen_d                |= 4'b0111;
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= my_reqq_id_i;
            end
            // Decode err, we're done.  Send a response back to UC ESR.
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshAtomicRsp;  // L3 atomic response write to ESR
              qwen_en               |= 1'b1;
              qwen_d                |= 4'b0111;        // Read atomic id from qw[2]
              dataq_id_en           |= 1'b1;
              dataq_id_d            |= my_reqq_id_i;
              set_data_ready        |= 1'b1;
            end
          end
          // The final atomic response was sent to UC ESR with a Mesh AtomicRsp.
          // Upon response we're done.
          if (opcode == MeshAtomicRsp) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // After evicting the read hit partial data, go read it back
          // The miss request is generated by this reqq but the reqq_id
          // on the mesh read is the paired id.
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshRead;
            end
          end
          // Fill, when we get the mesh response, wait for paired reqq to
          // issue the fill to the pipeline.
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == ReqqWait) begin
            // Wait. If paired reqq is picked to send fill request down the
            // pipe, we transition to L3Atomic2nd and this time
            // we should make it
            // The opcode to the pipeline has to be fixed back to
            // L3Atomic but internally kept as L3Atomic2nd
            if (pipe_req_picked_any_i &
                (pipe_req_reqq_id_any_i == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Atomic2nd;
              dataq_id_d            |= my_reqq_id_i;
              dataq_id_en           |= 1'b1;
            end
          end
          if (opcode == L3Atomic2nd) begin
            // Picked, L3Atomic2nd is used to set the arbitration
            // priority to the pipeline and is guarantee to hit
            // because it followed the fill request.
            // After it is picked, return opcode back to L3Atomic.
            if (pipe_req_picked_me_i) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Atomic;
            end
          end
        end
        {AddressL3, OrigAtomic2}: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing in case we need to fill or hold eviction
          // data. If not needed can deallocate early.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            // Hit. If paired hit we are done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Miss. If paired miss then we can transition to waiting on the response from the mesh.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
               ~pipe_tag_rsp_decode_err &
               ~pipe_tag_rsp_info_i.hit) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
            // Hit partial.
            // If paired read hits a partial, the pipeline signals a tag hit and evicts the line.
            // When L3_Write returns, we refetch the full line.
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait2;
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
            // Decode err, we're done
            if (pipe_tag_rsp_valid_any_i &
               (pipe_tag_rsp_info_i.reqq_id == paired_id) &
                pipe_tag_rsp_decode_err) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
          // Fill, when we get the mesh response send the fill down the pipeline
          // Other reqq issues the read request and swap in paired id to mesh
          // The mesh response zero data is updated here instead of the other reqq that issues the fill
          if (opcode == ReqqWait) begin
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= L3Fill;
              qwen_en               |= 1'b1;
              qwen_d                |= '1;
              zero_data_en          |= 1'b1;
              zero_data_d           |= to_sys_mesh_rsp_info_i.zero_data;
            end
          end
          // Hit write partial, when we get the mesh response, go read it
          // Other reqq issues the write request and swap in paired id to mesh
          if (opcode == ReqqWait2) begin
            if (to_sys_mesh_rsp_valid_any_i & (to_sys_mesh_rsp_info_i.reqq_id == paired_id)) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqWait;
            end
          end
          if (opcode == L3Fill) begin
            if (pipe_req_picked_me_i) begin
              set_wait_for_dataq    |= 1'b1;
            end
            // Fill and no victim, then we are done
            if (pipe_tag_rsp_valid_me_i & ~pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
            // Fill and victim, we are going to become a victim, grab the address
            if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
              opcode_en             |= 1'b1;
              opcode_d              |= MeshVictim;
              address_en            |= 1'b1;
              address_d             |= pipe_tag_rsp_info_i.victim_address | PaSize'(address[LineOffsetIdSize-1:0]);
              qwen_en               |= 1'b1;
              qwen_d                |= pipe_tag_rsp_info_i.victim_qwen;
            end
          end
          // Fill victim, when we get the mesh response, we're done
          if (opcode == MeshVictim) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
      endcase // case ({address_space_q, orig_opcode_q})
      /* verilator lint_on CASEINCOMPLETE */
    end

    // L2 and L3 Bypass cases
    // --------------------------------------------------------------------------
    // Request in L2 address space and ESR configured for L2 bypass
    if (valid & esr_ctl_i.esr_sc_l2_bypass &
        ~((opcode == ReqqAllocErr) | (orig_opcode_q == OrigErr)) &
         (address_space_q == AddressL2) & ~(reqq_source_q == ReqqSrcFsm)) begin
      /* verilator lint_off CASEINCOMPLETE */  // only bypass-eligible orig_opcodes handled
      case (orig_opcode_q)
        OrigReadForward,
        OrigReadCoop,
        OrigRead: begin
          // If just got started (initializing, forward request to mesh)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshRead;
          end
          // Rsp with the mesh data
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        OrigWriteAround,
        OrigWriteForward,
        OrigWriteForwardPart,
        OrigWrite,
        OrigWritePartial: begin
          // If just got started (initializing, forward request to mesh)
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshWrite;
          end
          // When we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        // Below opcodes are treated as no-op and
        // - atomic, respond with error
        // - copfsm evict cbuf, response is dropped
        // - and all others, response with ack
        OrigCopLock,
        OrigCopUnlock,
        OrigCopUnlockInv,
        OrigAtomic: begin
          // If just got started (initializing, return to requestor as noop)
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en               |= 1'b1;
            opcode_d                |= ReqqDone;
          end
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_rspmux_eligible     |= 1'b1;
            if (orig_opcode_q == OrigIdx) begin
              set_err_imprecise     |= 1'b1;
            end
          end
        end
        OrigCopEvict: begin
          // Need to handle multi-level operation for Evict
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            if ((cache_level_dest inside {CacheMem}) &
                esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then send evict to l3, else done
              opcode_d            |= MeshEvict;
            end
            else begin
              opcode_d            |= ReqqDone;
              set_rspmux_eligible |= 1'b1;
            end
          end
          // Evict , when we get the mesh response, we're done
          if (opcode == MeshEvict) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        OrigCopFlush: begin
          // Need to handle multi-level operation for Flush
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            if ((cache_level_dest inside {CacheMem}) &
                esr_ctl_i.esr_sc_remote_l3_enable) begin // if remote_l3_enable, then send flush to l3, else done
              opcode_d            |= MeshFlush;
            end
            else begin
              opcode_d            |= ReqqDone;
              set_rspmux_eligible |= 1'b1;
            end
          end
          // Flush , when we get the mesh response, we're done
          if (opcode == MeshFlush) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        OrigCopPrefetch: begin
          // Need to handle multi-level operation for Prefetch
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            // If dest level is L3, then send the prefetch to the mesh
            // Dest level Mem doesn't mean anything and is just turned around.
            if (cache_level_dest == CacheL3 & esr_ctl_i.esr_sc_remote_l3_enable) begin
              opcode_d            |= MeshPrefetch;
            end
            else begin
              opcode_d            |= ReqqDone;
              set_rspmux_eligible |= 1'b1;
            end
          end
          // Receive acknowledges for Prefetches above our level forwarded to the Mesh
          if (opcode == MeshPrefetch) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        OrigWriteAround2,
        OrigWritePartial2,
        OrigAtomic2: begin
          if (alloc_me_ad_i |
              (opcode == ReqqInitial2)) begin
            set_err_imprecise     |= 1'b1;
            set_rspmux_sent       |= 1'b1;
          end
          // Wait in state Initializing until allocated and deallocate
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= ReqqDone;
          end
        end
      endcase
      /* verilator lint_on CASEINCOMPLETE */
    end

    // Request in L3 address space and ESR configured for L3 bypass
    if (valid & esr_ctl_i.esr_sc_l3_bypass &
        ~((opcode == ReqqAllocErr) | (orig_opcode_q == OrigErr)) &
         (address_space_q == AddressL3) & ~(reqq_source_q == ReqqSrcFsm)) begin
      /* verilator lint_off CASEINCOMPLETE */  // only bypass-eligible orig_opcodes handled
      case (orig_opcode_q)
        OrigRead: begin
          // If just got started (initializing, then go to state L3_Read)
          if (opcode == ReqqInitial) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshRead;
          end
          // when we get the mesh response response
          if (opcode == MeshRead) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
            end
          end
        end
        OrigCopEvictWData,
        OrigCopFlushWData,
        OrigWritePartial,
        OrigWrite: begin
          // If just got started
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= MeshWrite;
          end
          // when we get the mesh response, we're done
          if (opcode == MeshWrite) begin
            if (mesh_rsp_valid_me) begin
              opcode_en             |= 1'b1;
              opcode_d              |= ReqqDone;
              set_rspmux_eligible   |= 1'b1;
            end
          end
        end
        // Below opcodes are treated as no-op and
        // - atomic, response with error
        // - evict, flush and prefetch, response with ack
        OrigCopEvict,
        OrigCopFlush,
        OrigCopPrefetch,
        OrigAtomic: begin
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en               |= 1'b1;
            opcode_d                |= ReqqDone;
            set_rspmux_eligible     |= 1'b1;
          end
        end
        OrigWritePartial2,
        OrigAtomic2: begin
          // Wait in state Initializing in case we need to fill or hold eviction
          // data. If not needed can deallocate early.
          if ((opcode == ReqqInitial) |
              (opcode == ReqqInitial2)) begin
            opcode_en             |= 1'b1;
            opcode_d              |= ReqqDone;
            set_rspmux_sent       |= 1'b1;
            set_err_imprecise     |= 1'b1;
          end
        end
      endcase // case (orig_opcode_q)
      /* verilator lint_on CASEINCOMPLETE */
    end
  end

  // ═══════════════════════════════��════════════════════════
  // Opcode state machine — flops for mutable fields
  // ═════════════════════���══════════════════════════════════

  always_ff @(posedge clk_gated) begin
    if (address_en)   address   <= address_d;
    if (qwen_en)      qwen      <= qwen_d;
    if (opcode_en)    opcode    <= reqq_opcode_e'(opcode_d);
    if (paired_id_en) paired_id <= paired_id_d;
    if (dataq_id_en)  dataq_id  <= dataq_id_d;
    if (zero_data_en) zero_data <= zero_data_d;
  end

  // ═══════════════════════════════════════════════════════
  // Helper functions
  // ═══════════════════════════════════════════════════════

  // Address range constants for masked_address_compare
  // SC_REQQ_SEL_ADDR_RANGE = (PaSize-1) : BankLsb = 39:6
  // SC_MSG_NEIGH_ID_ADDR_RANGE = 16 +: 4 (neigh id bits within msg address)
  localparam int unsigned MsgNeighIdLsb  = 16;
  localparam int unsigned MsgNeighIdSize = 4;

  // Compare the cache line address bits.  Mask off sub cache line bits.
  function automatic bit masked_address_compare;
    input [PaSize-1:0]      address_a;
    input [PaSize-1:0]      address_b;
    input address_space_e   addr_space;
    begin
      masked_address_compare = '0;
      if (addr_space == AddressL2) begin
        masked_address_compare |= (address_a[PaSize-1:BankLsb] == address_b[PaSize-1:BankLsb]);
      end
      if (addr_space == AddressL3) begin
        masked_address_compare |= (address_a[PaSize-1:BankLsb] == address_b[PaSize-1:BankLsb]);
      end
      if (addr_space == AddressScp) begin
        masked_address_compare |= (address_a[PaSize-1:BankLsb] == address_b[PaSize-1:BankLsb]);
      end
      if (addr_space == AddressMsg) begin
        masked_address_compare |= (address_a[MsgNeighIdLsb +: MsgNeighIdSize]
                                == address_b[MsgNeighIdLsb +: MsgNeighIdSize]);
      end
    end
  endfunction

  // Generate qwen from size and address
  function automatic [LineQwSize-1:0] gen_qwen;
    input sc_size_t         sz;
    input [PaSize-1:0]      addr;
    begin
      case (sz)
        etlink_pkg::SizeByte:  gen_qwen = LineQwSize'({1{1'b1}}) << addr[5:4];
        etlink_pkg::SizeHWord: gen_qwen = LineQwSize'({1{1'b1}}) << addr[5:4];
        etlink_pkg::SizeWord:  gen_qwen = LineQwSize'({1{1'b1}}) << addr[5:4];
        etlink_pkg::SizeDWord: gen_qwen = LineQwSize'({1{1'b1}}) << addr[5:4];
        etlink_pkg::SizeQWord: gen_qwen = LineQwSize'({1{1'b1}}) << addr[5:4];
        etlink_pkg::SizeHLine: gen_qwen = LineQwSize'({2{1'b1}}) << {addr[5], 1'b0};
        etlink_pkg::SizeLine:  gen_qwen = {LineQwSize{1'b1}};
        default:               gen_qwen = {LineQwSize{1'b1}};
      endcase
    end
  endfunction

  // ═══════════════════════════════════════════════════════
  // Read buffer tracking
  // ═══════════════════════════════════════════════════════

  logic                    rbuf_valid;
  logic                    rbuf_valid_d;
  logic                    rbuf_valid_en;
  logic                    rbuf_pending_valid;
  logic                    rbuf_pending_valid_d;
  logic                    rbuf_pending_valid_en;
  logic [RbufIdSize-1:0]   rbuf_id;
  logic [RbufIdSize-1:0]   rbuf_id_d;

  // match rbuf_pending_install
  // Based upon address match and both scp or both L2.
  logic match_rbuf_pending_install;
  assign match_rbuf_pending_install = valid &
    pipe_rbuf_state_i.rbuf_pending_install &
    masked_address_compare(address, pipe_rbuf_state_i.rbuf_pending_install_address, address_space_q) &
    (address_space_q inside {AddressL2, AddressScp}) &
    (pipe_rbuf_state_i.rbuf_pending_install_scp == entry_is_scp);

  logic rbuf_id_en;
  always_comb begin
    rbuf_valid_d           = '0;
    rbuf_valid_en          = '0;
    rbuf_pending_valid_d   = '0;
    rbuf_pending_valid_en  = '0;
    rbuf_id_d              = '0;
    rbuf_id_en             = '0;
    // Initialize at alloc
    if (alloc_me_ag_i) begin
      rbuf_valid_d           |= alloc_info_ag_i.rbuf_valid;
      rbuf_valid_en          |= 1'b1;
      rbuf_pending_valid_d   |= alloc_info_ag_i.rbuf_pending_valid;
      rbuf_pending_valid_en  |= 1'b1;
      rbuf_id_d              |= alloc_info_ag_i.rbuf_id;
      rbuf_id_en             |= 1'b1;
    end
    // Rbuf pending install.
    if (match_rbuf_pending_install) begin
      rbuf_pending_valid_d   |= 1'b1;
      rbuf_pending_valid_en  |= 1'b1;
      rbuf_id_d              |= pipe_rbuf_state_i.rbuf_pending_install_id;
      rbuf_id_en             |= 1'b1;
    end
    // Rbuf install.  Move rbuf_pending_valid to rbuf_valid
    // Based upon rbuf id match if already in state pending_valid
    if (valid & rbuf_pending_valid &
        pipe_rbuf_state_i.rbuf_install &
        (address_space_q inside {AddressL2, AddressScp}) &
        (pipe_rbuf_state_i.rbuf_install_id == rbuf_id) &
        (pipe_rbuf_state_i.rbuf_install_scp == entry_is_scp)
    ) begin
      rbuf_valid_d           |= 1'b1;
      rbuf_valid_en          |= 1'b1;
      rbuf_pending_valid_d   |= 1'b0;
      rbuf_pending_valid_en  |= 1'b1;
    end
    // Rbuf evict.  Capacity eviction.
    // Based upon rbuf id.
    if (valid &
        pipe_rbuf_state_i.rbuf_evict &
        (rbuf_id == pipe_rbuf_state_i.rbuf_evict_id)) begin
      rbuf_valid_d           |= 1'b0;
      rbuf_valid_en          |= 1'b1;
      rbuf_pending_valid_d   |= 1'b0;
      rbuf_pending_valid_en  |= 1'b1;
    end
    // Rbuf clear.  This is signaled when a non-read is eligible.
    // Based upon rbuf id.
    if (valid &
        (pipe_rbuf_clear_req_info_i.rbuf_clear_mem_valid_hot[rbuf_id] |
         pipe_rbuf_clear_req_info_i.rbuf_clear_pending_install_hot[rbuf_id])) begin
      rbuf_valid_d           |= 1'b0;
      rbuf_valid_en          |= 1'b1;
      rbuf_pending_valid_d   |= 1'b0;
      rbuf_pending_valid_en  |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated) begin
    if (rbuf_valid_en)         rbuf_valid         <= rbuf_valid_d;
    if (rbuf_pending_valid_en) rbuf_pending_valid <= rbuf_pending_valid_d;
    if (rbuf_id_en)            rbuf_id            <= rbuf_id_d;
  end

  // ═══════════════════════════════════════════════════════
  // Dependency tracking
  // ═══════════════════════════════════════════════════════
  //
  // Set initial dependencies at alloc
  // Clear tail if someone allocated behind me
  // Clear dependencies if the reqq we're dependent upon signals dont_depend_on_me
  //
  // Victims
  // If becoming a victim, set dependencies based upon tag_victim_dep*
  // If victim comes in behind me clear dep_meshll_tail.
  // If victim comes in ahead of me point to if non_victim_head.

  logic                          dep_valid_d;
  logic                          dep_valid_en;
  logic [ReqqIdSize-1:0]         dep_id_d;
  logic                          dep_id_en;
  logic                          dep_tail_d;
  logic                          dep_tail_en;
  logic                          dep_meshll_tail_d;
  logic                          dep_meshll_tail_en;
  logic                          dep_non_meshll_head_d;
  logic                          dep_non_meshll_head_en;
  logic                          dep_in_meshll_d;
  logic                          dep_in_meshll_en;

  always_comb begin
    dep_valid_d             = 1'b0;
    dep_id_d                = '0;
    dep_tail_d              = 1'b0;
    dep_meshll_tail_d       = 1'b0;
    dep_non_meshll_head_d   = 1'b0;
    dep_in_meshll_d         = 1'b0;

    dep_valid_en            = 1'b0;
    dep_id_en               = 1'b0;
    dep_tail_en             = 1'b0;
    dep_meshll_tail_en      = 1'b0;
    dep_non_meshll_head_en  = 1'b0;
    dep_in_meshll_en        = 1'b0;

    // Allocate me
    if (alloc_me_ag_i) begin
      // Allocating a dependency and we're not currently clearing it
      dep_valid_d             |= alloc_info_ag_i.dep_valid;
      dep_id_d                |= alloc_info_ag_i.dep_id;
      dep_tail_d              |= ~coalesce_victim_match_alloc_ag_i;
      dep_valid_en            |= 1'b1;
      dep_id_en               |= alloc_info_ag_i.dep_valid;
      dep_tail_en             |= 1'b1;

      dep_in_meshll_d         |= 1'b0;
      dep_in_meshll_en        |= 1'b1;

      dep_meshll_tail_d       |= 1'b0;
      dep_meshll_tail_en      |= 1'b1;
      dep_non_meshll_head_d   |= alloc_info_ag_i.dep_non_meshll_head;
      dep_non_meshll_head_en  |= 1'b1;
    end

    // Allocate someone behind me
    if (valid & alloc_any_ag_i & ~alloc_me_ag_i) begin
      // If anyone else is being allocated pointing to me, then I am
      // no longer the tail.
      if (alloc_any_info_ag_i.dep_valid & (~alloc_any_info_ag_i.err_valid) &
          (alloc_any_info_ag_i.dep_id == my_reqq_id_i)) begin
        dep_tail_d              |= 1'b0;
        dep_tail_en             |= 1'b1;
      end
    end

    // Dependency clear
    if (valid & dep_valid & clear_dep_hot_i[dep_id]) begin
      dep_valid_d             |= 1'b0;
      dep_valid_en            |= 1'b1;
      if (~dep_in_meshll) begin
        dep_non_meshll_head_d |= 1'b1;
        dep_non_meshll_head_en|= 1'b1;
      end
    end

    // If becoming a victim, set dependencies based upon tag_victim_dep*
    // Self-victim case
    if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.victim) begin
      dep_valid_d             |= tag_victim_dep_valid_i;
      dep_valid_en            |= 1'b1;
      dep_id_d                |= tag_victim_dep_id_i;
      dep_id_en               |= tag_victim_dep_valid_i;
      dep_tail_d              |= tag_victim_dep_tail_i;
      dep_tail_en             |= 1'b1;
      dep_meshll_tail_d       |= ~tag_victim_upgrade_behind_me_i;
      dep_meshll_tail_en      |= 1'b1;
      dep_non_meshll_head_d   |= 1'b0;
      dep_non_meshll_head_en  |= 1'b1;
      dep_in_meshll_d         |= 1'b1;
      dep_in_meshll_en        |= 1'b1;
    end

    // If victim comes in behind me clear both dep_tail and dep_meshll_tail.
    if (tag_victim_insert_behind_me_i) begin
      dep_meshll_tail_d       |= 1'b0;
      dep_meshll_tail_en      |= 1'b1;
      dep_tail_d              |= 1'b0;
      dep_tail_en             |= 1'b1;
    end

    // If victim comes in ahead of me (because I'm not a victim) point to it
    // if non_victim_head.
    if (tag_victim_insert_ahead_of_me_i) begin
      dep_valid_d             |= 1'b1;
      dep_valid_en            |= 1'b1;
      dep_id_d                |= pipe_tag_rsp_info_i.reqq_id;
      dep_id_en               |= 1'b1;
    end

    // If I am the front of the linked list and headed to mesh then upgrade
    // to meshll portion of the linked list.
    // I am no longer non_meshll head, I am meshll tail.
    if (upgrade_me_to_meshll_i) begin
      dep_meshll_tail_d       |= 1'b1;
      dep_meshll_tail_en      |= 1'b1;
      dep_non_meshll_head_d   |= 1'b0;
      dep_non_meshll_head_en  |= 1'b1;
      dep_in_meshll_d         |= 1'b1;
      dep_in_meshll_en        |= 1'b1;
    end

    // If the entry in front of me upgrades to meshll, then I'm now the non_victim_head
    if (upgrade_ahead_of_me_to_meshll_i) begin
      dep_non_meshll_head_d   |= 1'b1;
      dep_non_meshll_head_en  |= 1'b1;
    end

    // Becoming coalescing buffer evict.  Put it at the end of the linked list.
    if (coalesce_victim_me_i) begin
      dep_valid_d             |= coalesce_victim_dep_valid_i;
      dep_valid_en            |= 1'b1;
      dep_id_d                |= coalesce_victim_dep_id_i;
      dep_id_en               |= 1'b1;
      dep_tail_d              |= 1'b1;
      dep_tail_en             |= 1'b1;

      dep_meshll_tail_d       |= 1'b0;
      dep_meshll_tail_en      |= 1'b1;
      dep_non_meshll_head_d   |= coalesce_victim_dep_non_meshll_head_i;
      dep_non_meshll_head_en  |= 1'b1;
    end

    // If coalesce victim comes in behind me clear dep_tail.
    if (coalesce_victim_insert_behind_me_i) begin
      dep_tail_d              |= 1'b0;
      dep_tail_en             |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated) begin
    if (dep_valid_en)           dep_valid           <= dep_valid_d;
    if (dep_id_en)              dep_id              <= dep_id_d;
    if (dep_tail_en)            dep_tail            <= dep_tail_d;
    if (dep_meshll_tail_en)     dep_meshll_tail     <= dep_meshll_tail_d;
    if (dep_non_meshll_head_en) dep_non_meshll_head <= dep_non_meshll_head_d;
    if (dep_in_meshll_en)       dep_in_meshll       <= dep_in_meshll_d;
  end

  // ═══════════════════════════════════════════════════════
  // set_dont_depend_on_me logic
  // ═══════════════════════════════════════════════════════
  //
  // Past the point of needing to stall dependencies
  //   read                -> rbuf hit, read hit (not partial) or fill pipe_req_picked_me
  //   readForward         -> Mesh response
  //   write               -> pipe_req_picked_me
  //   writePartial        -> hit (not partial)
  //   writeForward        -> Mesh response
  //   writeForwardPartial -> Mesh response
  //   flush, evict        -> done and head or mesh response
  //   prefetch            -> rbuf hit, read hit (not partial), fill pipe_req_picked_me
  //                          or forwarded prefetch mesh response
  //   lock and unlock     -> pipe_req_picked_me
  //   atomic              -> read hit (not partial)
  //   write around        -> pipe_req_picked_me
  //   scpRead             -> rbuf hit, pipe_req_picked_me, mesh response
  //   scpWrite            -> pipe_req_picked_me
  //   scpWritePartial     -> pipe_req_picked_me
  //   scpFill             -> pipe_req_picked_me
  //   msgSendData         -> rspmux_l2_picked
  //   victims             -> mesh response

  logic set_dont_depend_on_me;
  always_comb begin
    set_dont_depend_on_me = '0;

    // If we're done and no longer waiting on anything ahead, then set dont_depend_on_me
    // and deallocate.
    // This is for flushes that are kept alive and dependable if a victim occurs just
    // ahead of them.
    // Also for reads that are a hit with a victim behind them
    set_dont_depend_on_me |= (opcode == ReqqDone) & dep_head;

    // Throttle the same address from being issued down the pipe when esr is set
    // Otherwise issue the same address as soon as possible
    if (~esr_ctl_i.esr_sc_reqq_no_link_list) begin
      // read            -> rbuf hit, read hit (not partial) or fill pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigRead, OrigReadCoop}) &
        (rbuf_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigRead, OrigReadCoop}) &
        (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) & dep_head;
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigRead, OrigReadCoop}) &
        ((opcode inside {L2Fill, L3Fill}) & pipe_req_picked_me_i);

      // readForward    -> Mesh response
      set_dont_depend_on_me |= (orig_opcode_q == OrigReadForward) &
        mesh_rsp_valid_me;

      // write           -> pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigWrite) &
        (pipe_req_picked_me_i);

      // writePartial    -> hit (not partial)
      set_dont_depend_on_me |= (orig_opcode_q == OrigWritePartial) &
        (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) & dep_head;

      // writeForward        -> Mesh response
      // writeForwardPartial -> Mesh response
      set_dont_depend_on_me |= (orig_opcode_q == OrigWriteForward) &
        mesh_rsp_valid_me;
      set_dont_depend_on_me |= (orig_opcode_q == OrigWriteForwardPart) &
        mesh_rsp_valid_me;

      // flush, evict    -> done and head or mesh response
      // If a victim gets created just before the flush/evict, the flush/evict
      // will be a miss and considered done.  However, the victim is placed in front of
      // the flush/evict.  Therefore, the flush evict must be kept alive and dependable
      // until the victim dependency ahead of it.
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopFlush) &
        (mesh_rsp_valid_me) & dep_head;
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopEvict) &
        (mesh_rsp_valid_me) & dep_head;

      // prefetch        -> rbuf hit, read hit (not partial), fill pipe_req_picked_me
      //                    or forwarded prefetch mesh rsp
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopPrefetch) &
        (rbuf_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopPrefetch) &
        (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) & dep_head;
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopPrefetch) &
        ((opcode inside {L2Fill, L3Fill}) & pipe_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopPrefetch) &
        (opcode == MeshPrefetch) & mesh_rsp_valid_me;

      // lock and unlock -> on pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopLock) &
        (pipe_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopUnlock) &
        (pipe_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopUnlockInv) &
        (pipe_req_picked_me_i);

      // atomic          -> read hit (not partial)
      set_dont_depend_on_me |= (orig_opcode_q == OrigAtomic) &
        ~(address_space_q == AddressScp) &
        (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_info_i.hit & ~pipe_tag_rsp_info_i.victim) & dep_head;
      set_dont_depend_on_me |= (orig_opcode_q == OrigAtomic) &
        (address_space_q == AddressScp) &
        (pipe_tag_rsp_valid_me_i) & dep_head;

      // write around    -> pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigWriteAround) &
        (pipe_req_picked_me_i);

      // scpRead         -> rbuf hit, pipe_req_picked_me, mesh rsp
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigScpRead, OrigScpReadCoop}) &
        (pipe_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigScpRead, OrigScpReadCoop}) &
        (rbuf_req_picked_me_i);
      set_dont_depend_on_me |= (orig_opcode_q inside {OrigScpRead, OrigScpReadCoop}) &
        (mesh_rsp_valid_me);

      // scpWrite        -> pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigScpWrite) &
        (pipe_req_picked_me_i);

      // scpWritePartial -> pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigScpWritePartial) &
        (pipe_req_picked_me_i);

      // scpFill         -> pipe_req_picked_me
      set_dont_depend_on_me |= (orig_opcode_q == OrigCopScpFill) &
        (pipe_req_picked_me_i);

      // msgSendData     -> rspmux_l2_picked
      set_dont_depend_on_me |= (orig_opcode_q == OrigMsgSendData) &
        (rspmux_l2_picked_i);

      // victims         -> mesh response
      set_dont_depend_on_me |= (opcode == MeshVictim) &
        (mesh_rsp_valid_me);
    end
  end

  // ═══════════════════════════════════════════════════════
  // dont_depend_on_me + setting_dont_depend_on_me registers
  // ═══════════════════════════════════════════════════════
  //
  // If this request is past the point of clearing dependencies behind it,
  // then new requests coming in should also not see this entry as a
  // dependency candidate.
  // If this entry becomes a victim, then it gets added to the front of
  // linked lists and needs to be dependable again.

  logic dont_depend_on_me_en;
  logic dont_depend_on_me_d;
  logic setting_dont_depend_on_me_en;
  logic setting_dont_depend_on_me_d;

  always_comb begin
    dont_depend_on_me_en   = 1'b0;
    dont_depend_on_me_d    = 1'b0;
    setting_dont_depend_on_me_en   = 1'b0;
    setting_dont_depend_on_me_d    = 1'b0;
    // alloc
    if (alloc_me_ag_i) begin
      dont_depend_on_me_en   |= 1'b1;
      dont_depend_on_me_d    |= alloc_info_ag_i.orig_opcode inside {OrigWriteAround2, OrigWritePartial2, OrigAtomic2} | alloc_info_ag_i.err_valid;
      setting_dont_depend_on_me_en   |= dont_depend_on_me_en;
      setting_dont_depend_on_me_d    |= dont_depend_on_me_d;
    end
    // victim
    if (address_en & ~alloc_me_ag_i) begin
      dont_depend_on_me_en   |= 1'b1;
      dont_depend_on_me_d    |= 1'b0;
      setting_dont_depend_on_me_en   |= dont_depend_on_me_en;
      setting_dont_depend_on_me_d    |= dont_depend_on_me_d;
    end
    // becoming not dependable
    // clear_dep_behind_me must be a function of dont_depend_on_me and transitioning to
    // dont_depend_on_me.  It's a timing problem to make clear_dep_behind_me be a
    // combinational function of set_dont_depend_on_me, so for timing set
    // setting_dont_depend_on_me and then the cycle after set dont_depend_on_me.
    // clear_dep_behind_me is the or of setting_dont_depend_on_me and dont_depend_on_me
    if (set_dont_depend_on_me & valid & dep_head) begin
      setting_dont_depend_on_me_en   |= 1'b1;
      setting_dont_depend_on_me_d    |= 1'b1;
    end
    // set dont_depend_on_me 1 cycle after setting_dont_depend_on_me
    if (setting_dont_depend_on_me & ~dont_depend_on_me_en & valid & dep_head) begin
      dont_depend_on_me_en   |= 1'b1;
      dont_depend_on_me_d    |= 1'b1;
    end
  end

  // RST_EN_FF for dont_depend_on_me
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)                        dont_depend_on_me <= '0;
    else if (dont_depend_on_me_en)     dont_depend_on_me <= dont_depend_on_me_d;
  end

  // RST_EN_FF for setting_dont_depend_on_me
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)                                  setting_dont_depend_on_me <= '0;
    else if (setting_dont_depend_on_me_en)       setting_dont_depend_on_me <= setting_dont_depend_on_me_d;
  end

  // Clear dependencies behind me if I'm past needing to stall dependencies
  // and I'm the head of the list.
  // It's possible to be past needing to stall dependencies and not be the front
  // of the linked list if I was already running down the pipeline as an evict
  // and a victim was generated just before me.  Now, I'm the tail of the linked
  // list even though, I'm also Reqq_Done.
  assign clear_dep_behind_me_o = (setting_dont_depend_on_me | dont_depend_on_me) & ~dep_valid;

  // ═══════════════════════════════════════════════════════
  // Inflight tracking
  // ═══════════════════════════════════════════════════════
  //
  // Inflight indicates the current opcode is already ongoing in the pipe or mesh.
  // Request is not eligible if already inflight.
  // Cleared at allocation
  // Cleared whenever the opcode changes.
  // Set if the request is picked by pipe or mesh.
  // Set if bypass victim to mesh.

  logic inflight_d;
  logic inflight_en;

  always_comb begin
    inflight_d  = '0;
    inflight_en = '0;
    if (alloc_me_ag_i) begin
      inflight_d  |= 1'b0;
      inflight_en |= 1'b1;
    end
    if (valid & opcode_en) begin
      inflight_d  |= 1'b0;
      inflight_en |= 1'b1;
    end
    if (rbuf_req_picked_me_i | pipe_req_picked_me_i | to_l3_mesh_req_picked_i | to_sys_mesh_req_picked_i) begin
      inflight_d  |= 1'b1;
      inflight_en |= 1'b1;
    end
    if (pipe_data_rsp_valid_me_i & mesh_bypassed_victim_i) begin
      inflight_d  |= 1'b1;
      inflight_en |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated) begin
    if (inflight_en) inflight <= inflight_d;
  end

  // ═══════════════════════════════════════════════════════
  // data_ready tracking
  // ═══════════════════════════════════════════════════════
  //
  // data_ready indicates that the dataq has written data to ram and it can now be read out.
  // Clr at allocation.
  // Set if dataq signals dataq_bank_ready.
  // Set for atomic2/writePartial2 in fill state when Mesh_Read initiated
  //    by primary reqq entry is written to dataq
  // Set if l3 cop flushWData or evictWData misses
  // Clr if picked which means we're using the data.

  logic data_ready_d;
  logic data_ready_en;

  always_comb begin
    data_ready_d  = '0;
    data_ready_en = '0;
    if (alloc_me_ag_i) begin
      data_ready_d  |= 1'b0;
      data_ready_en |= 1'b1;
    end
    if (dataq_write_status_me_i) begin
      data_ready_d  |= 1'b1;
      data_ready_en |= 1'b1;
    end
    if (valid & dataq_write_status_any_i & (dataq_write_status_id_i == paired_id[ReqqIdSize-1:0]) &
        (orig_opcode_q inside {OrigAtomic2, OrigWritePartial2}) &
        (opcode inside {L2Fill, L3Fill})) begin
      data_ready_d  |= 1'b1;
      data_ready_en |= 1'b1;
    end
    if (set_data_ready) begin
      data_ready_d  |= 1'b1;
      data_ready_en |= 1'b1;
    end
    if (pipe_req_picked_me_i | to_l3_mesh_req_picked_i | to_sys_mesh_req_picked_i) begin
      data_ready_d  |= 1'b0;
      data_ready_en |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated) begin
    if (data_ready_en) data_ready <= data_ready_d;
  end

  // ═══════════════════════════════════════════════════════
  // wait_for_dataq tracking
  // ═══════════════════════════════════════════════════════
  //
  // wait_for_dataq indicates that we can't deallocate this entry until the dataq
  // has been read or written
  // Set if allocating an entry expecting data.
  // Set if FSM set set_wait_for_dataq.
  // Clr if dataq read is seen.
  // Clr if pipe_tag_rsp_decode_err.  If there is a decode error, the pipe won't read

  logic orig_opcode_wait_for_dataq_write;
  assign orig_opcode_wait_for_dataq_write = alloc_info_ag_i.orig_opcode inside {
    OrigWrite,
    OrigWritePartial,
    OrigWriteAround,
    OrigWriteForward,
    OrigWriteForwardPart,
    OrigMsgSendData,
    OrigAtomic,
    OrigScpWrite,
    OrigScpWritePartial,
    OrigCopFlushWData,
    OrigCopEvictWData,
    OrigCopScpFill};

  logic wait_for_dataq_d;
  logic wait_for_dataq_en;

  always_comb begin
    wait_for_dataq_d  = '0;
    wait_for_dataq_en = '0;
    if (alloc_me_ag_i) begin
      wait_for_dataq_d  |= orig_opcode_wait_for_dataq_write;
      wait_for_dataq_en |= 1'b1;
    end
    if (dataq_read_status_me_i | dataq_write_status_me_i) begin
      wait_for_dataq_d  |= 1'b0;
      wait_for_dataq_en |= 1'b1;
    end
    if (pipe_tag_rsp_valid_me_i & pipe_tag_rsp_decode_err) begin
      wait_for_dataq_d  |= 1'b0;
      wait_for_dataq_en |= 1'b1;
    end
    if (set_wait_for_dataq) begin
      wait_for_dataq_d  |= 1'b1;
      wait_for_dataq_en |= 1'b1;
    end
  end

  always_ff @(posedge clk_gated) begin
    if (wait_for_dataq_en) wait_for_dataq <= wait_for_dataq_d;
  end

  // ═══════════════════════════════════════════════════════
  // Eligibility outputs
  // ═══════════════════════════════════════════════════════

  // pipe or rbuf eligible
  // opcode is destined for pipe
  // & head of linked list
  // & data_ready or not needed
  // & not already inflight;
  // & not block by other atomic2 request
  logic opcode_destined_for_pipe;
  assign opcode_destined_for_pipe = opcode inside {
    L2Read,
    L2Write,
    L2WritePartial,
    L2WritePartial2nd,
    L2WriteAround,
    L2Lock,
    L2Unlock,
    L2UnlockInv,
    L2Flush,
    L2Evict,
    L2Prefetch,
    L2Atomic,
    L2Atomic2nd,
    L2Fill,

    L3Read,
    L3Write,
    L3WritePartial,
    L3WritePartial2nd,
    L3Flush,
    L3FlushWData,
    L3Evict,
    L3EvictWData,
    L3Prefetch,
    L3Atomic,
    L3Atomic2nd,
    L3Fill,

    IdxRead,
    IdxWrite,
    IdxAllInv,
    IdxL2Inv,
    IdxL2Flush,
    IdxL2Evict,
    IdxL3Inv,
    IdxL3Flush,
    IdxL3Evict,

    ScpRead,
    ScpWrite,
    ScpWritePartial,
    ScpAtomic,
    ScpZero,
    ScpFill
  };

  logic pipe_opcode_needs_data;
  assign pipe_opcode_needs_data = opcode inside {
    L2Write,
    L2WritePartial,
    L2WriteAround,
    L2Atomic,
    L2Fill,

    L3Write,
    L3WritePartial,
    L3FlushWData,
    L3EvictWData,
    L3Atomic,
    L3Fill,

    ScpWrite,
    ScpWritePartial,
    ScpAtomic,
    ScpFill
  };

  // Holdoff eligibility to issue to the pipeline
  //   - when there is outstanding atomic_2nd_pass
  //   - when there is outstanding writePartial_2nd_pass
  //   - when there is outstanding idx read or write
  //   - when a victim to the same address attaches to the head of the list.

  // Someone needs priority and it isn't me.
  // Holdoff if there is an outstanding Atomic_2nd_pass or WritePartial_2nd_pass that isn't me.
  logic pipe_req_2nd_pass_holdoff_eligible;
  assign pipe_req_2nd_pass_holdoff_eligible = reqq_2nd_pass_need_priority_i & valid &
    ~(opcode inside {L2Atomic2nd,
                     L3Atomic2nd,
                     L2WritePartial2nd,
                     L3WritePartial2nd});

  // Holdoff if there is an outstanding Idx read/write that isn't me.
  logic pipe_req_idx_cop_holdoff_eligible;
  assign pipe_req_idx_cop_holdoff_eligible = reqq_idx_cop_need_priority_i & valid &
    ~(opcode inside {IdxRead, IdxWrite});

  // Priority Holdoff from Atomic_2nd_pass, WritePartial_2nd_pass or Idx Read/Write
  logic pipe_req_priority_holdoff_eligible;
  assign pipe_req_priority_holdoff_eligible =
    pipe_req_2nd_pass_holdoff_eligible |
    pipe_req_idx_cop_holdoff_eligible;

  // If the request is a read that will soon be in the read buffer, wait to read it from the read buffer.
  logic pipe_read_and_rbuf_pending_holdoff;
  assign pipe_read_and_rbuf_pending_holdoff = (opcode inside {L2Read, ScpRead}) &
    (rbuf_pending_valid | match_rbuf_pending_install);

  logic pipe_or_rbuf_req_eligible_d;
  assign pipe_or_rbuf_req_eligible_d =
    valid &
    opcode_destined_for_pipe &
    ~pipe_req_priority_holdoff_eligible &
    ~pipe_read_and_rbuf_pending_holdoff &
    ~tag_victim_address_match_i &
    (data_ready | ~pipe_opcode_needs_data) &
    ~(pipe_idx_cop_aborted_i & (reqq_source_q == ReqqSrcFsm)) &
    (~inflight & ~inflight_d);

  // pipe eligible if not in rbuf or not a read
  logic send_to_rbuf;
  assign send_to_rbuf = ((rbuf_valid_en) ? rbuf_valid_d : rbuf_valid) &
    (opcode inside {L2Read, ScpRead});

  logic pipe_req_eligible;
  logic pipe_req_eligible_d;
  assign pipe_req_eligible_d = pipe_or_rbuf_req_eligible_d & ~send_to_rbuf &
    ~dep_valid;

  // RST_FF for pipe_req_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)  pipe_req_eligible <= '0;
    else         pipe_req_eligible <= pipe_req_eligible_d;
  end

  // rbuf_req_eligible
  logic rbuf_req_eligible;
  logic rbuf_req_eligible_d;
  assign rbuf_req_eligible_d = pipe_or_rbuf_req_eligible_d & send_to_rbuf &
    (~dep_valid | clear_dep_rbuf_1hot_pre_i[dep_id]);

  // RST_FF for rbuf_req_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)  rbuf_req_eligible <= '0;
    else         rbuf_req_eligible <= rbuf_req_eligible_d;
  end

  // mesh_req_eligible
  // opcode is destined for mesh
  // & head of linked list
  // & data_ready or not needed
  // & not already in flight
  logic opcode_destined_for_mesh;
  assign opcode_destined_for_mesh = opcode inside {
    MeshRead,
    MeshWrite,
    MeshFlush,
    MeshFlushToMem,
    MeshEvict,
    MeshEvictToMem,
    MeshPrefetch,
    MeshAtomicRsp,
    MeshVictim
  };

  logic mesh_opcode_needs_data;
  assign mesh_opcode_needs_data = opcode inside {
    // MeshAtomic -- L3 atomics don't need data.
    MeshFlushToMem,
    MeshEvictToMem,
    MeshWrite,
    MeshAtomicRsp,
    MeshVictim
  } | (orig_opcode_q == OrigCopScpFill);

  // Which mesh port to go to
  logic request_destined_for_to_sys;
  assign request_destined_for_to_sys =
    // All L3 things when L3 is disabled
    ((alloc_info_ag_i.address_space == AddressL2) & ~esr_ctl_i.esr_sc_remote_l3_enable) |
    // ScpFill when L3 is disabled
    ((alloc_info_ag_i.orig_opcode == OrigCopScpFill) & ~alloc_info_ag_i.scp_fill_source_is_scp & ~esr_ctl_i.esr_sc_remote_l3_enable) |
    // All L3 things to to_sys because mem is next level cache
    ( alloc_info_ag_i.address_space == AddressL3) |
    // Scp Atomic response is sent to UC via to_sys
    ((alloc_info_ag_i.orig_opcode == OrigAtomic) & (alloc_info_ag_i.address_space == AddressScp)) |
    // Idx Cop FSM L3 requests
    ((alloc_info_ag_i.orig_opcode inside {OrigIdx}) & (alloc_info_ag_i.cache_level_dest == CacheMem)) |
    // Idx Cop FSM requests that can skip L3
    ((alloc_info_ag_i.orig_opcode inside {OrigIdx}) & ~esr_ctl_i.esr_sc_remote_l3_enable);

  // Also need to tell pipe victims where to go if they are bypassed.
  mesh_dest_e which_mesh;
  mesh_dest_e which_mesh_d;
  assign which_mesh_d = (request_destined_for_to_sys) ? ToSys : ToL3;

  always_ff @(posedge clk_gated) begin
    if (alloc_me_ag_i) which_mesh <= which_mesh_d;
  end

  logic mesh_req_eligible_d;
  assign mesh_req_eligible_d =
    valid &
    opcode_destined_for_mesh &
    ~dep_valid &
    ~tag_victim_insert_ahead_of_me_i &
    (data_ready | ~mesh_opcode_needs_data) &
    (~inflight & ~to_l3_mesh_req_picked_i & ~to_sys_mesh_req_picked_i);

  logic to_l3_mesh_req_eligible;
  logic to_l3_mesh_req_eligible_d;
  assign to_l3_mesh_req_eligible_d = mesh_req_eligible_d & (which_mesh == ToL3);

  // RST_FF for to_l3_mesh_req_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)  to_l3_mesh_req_eligible <= '0;
    else         to_l3_mesh_req_eligible <= to_l3_mesh_req_eligible_d;
  end

  logic to_sys_mesh_req_eligible;
  logic to_sys_mesh_req_eligible_d;
  assign to_sys_mesh_req_eligible_d = mesh_req_eligible_d & (which_mesh == ToSys);

  // RST_FF for to_sys_mesh_req_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)  to_sys_mesh_req_eligible <= '0;
    else         to_sys_mesh_req_eligible <= to_sys_mesh_req_eligible_d;
  end

  assign to_l3_mesh_clk_en_o  = valid & opcode_destined_for_mesh & (which_mesh == ToL3);
  assign to_sys_mesh_clk_en_o = valid & opcode_destined_for_mesh & (which_mesh == ToSys);

  // ═══════════════════════════════════════════════════════
  // rspmux eligible / sent
  // ═══════════════════════════════════════════════════════
  //
  // neigh rsp eligible is not waiting on data_ready because the only data comes
  // from the pipe which has guaranteed highest priority.
  // neigh rsp eligible for certain atomic does not have data response
  // rsp data from a fill has a separate path from mesh to rspmux_l2.

  // L2 responses to neigh
  logic rspmux_l2_eligible;
  logic rspmux_l2_eligible_d;
  logic rspmux_l2_eligible_en;

  always_comb begin
    rspmux_l2_eligible_d  = '0;
    rspmux_l2_eligible_en = '0;
    if (alloc_me_ag_i) begin
      rspmux_l2_eligible_en |= 1'b1;
      rspmux_l2_eligible_d  |= 1'b0;
    end
    if (valid & set_rspmux_eligible & (reqq_source_q == ReqqSrcL2)) begin
      rspmux_l2_eligible_en |= 1'b1;
      rspmux_l2_eligible_d  |= ~(pipe_data_rsp_valid_me_i & rspmux_bypassed_data_rsp_i);
    end
    if (valid & rspmux_l2_picked_i) begin
      rspmux_l2_eligible_en |= 1'b1;
      rspmux_l2_eligible_d  |= 1'b0;
    end
  end

  // RST_EN_FF for rspmux_l2_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)                        rspmux_l2_eligible <= '0;
    else if (rspmux_l2_eligible_en)    rspmux_l2_eligible <= rspmux_l2_eligible_d;
  end

  // L3 responses to L3 slave
  logic rspmux_l3_eligible;
  logic rspmux_l3_eligible_d;
  logic rspmux_l3_eligible_en;

  always_comb begin
    rspmux_l3_eligible_d  = '0;
    rspmux_l3_eligible_en = '0;
    if (alloc_me_ag_i) begin
      rspmux_l3_eligible_en |= 1'b1;
      rspmux_l3_eligible_d  |= 1'b0;
    end
    if (valid & set_rspmux_eligible & (reqq_source_q == ReqqSrcL3)) begin
      rspmux_l3_eligible_en |= 1'b1;
      rspmux_l3_eligible_d  |= ~(pipe_data_rsp_valid_me_i & rspmux_bypassed_data_rsp_i);
    end
    if (valid & rspmux_l3_picked_i) begin
      rspmux_l3_eligible_en |= 1'b1;
      rspmux_l3_eligible_d  |= 1'b0;
    end
  end

  // RST_EN_FF for rspmux_l3_eligible
  always_ff @(posedge clk_gated or negedge rst_n) begin
    if (!rst_n)                        rspmux_l3_eligible <= '0;
    else if (rspmux_l3_eligible_en)    rspmux_l3_eligible <= rspmux_l3_eligible_d;
  end

  // rspmux_sent - indicates that the response has been sent to the neighborhood
  // Clear at alloc
  // Set when set_rspmux_sent for cop FSM evict cbuf
  // Set at rspmux saying it was sent
  logic rspmux_sent_d;
  logic rspmux_sent_en;

  always_comb begin
    rspmux_sent_d  = '0;
    rspmux_sent_en = '0;
    if (alloc_me_ag_i) begin
      rspmux_sent_en |= 1'b1;
      // If there is a port error on a message, then can't send a response
      if (alloc_info_ag_i.err_valid & alloc_info_ag_i.err_imprecise) begin
        rspmux_sent_d  |= 1'b1;
      end
    end
    if (set_rspmux_sent) begin
      rspmux_sent_en |= 1'b1;
      rspmux_sent_d  |= 1'b1;
    end
    if (rspmux_sent_valid_i) begin
      rspmux_sent_en |= 1'b1;
      rspmux_sent_d  |= 1'b1;
    end
  end

  // EN_FF for rspmux_sent
  always_ff @(posedge clk_gated) begin
    if (rspmux_sent_en) rspmux_sent <= rspmux_sent_d;
  end

  // ═══════════════════════════════════════════════════════
  // Error tracking
  // ═══════════════════════════════════════════════════════

  // err_imprecise - indicates that an error detected will not be forwarded to the requestor
  // Clear at alloc
  // Set when set_err_imprecise
  logic err_imprecise;
  logic err_imprecise_d;
  logic err_imprecise_en;

  always_comb begin
    err_imprecise_d  = '0;
    err_imprecise_en = '0;
    if (alloc_me_ag_i) begin
      err_imprecise_en |= 1'b1;
      err_imprecise_d  |= alloc_info_ag_i.err_imprecise;
    end
    if (set_err_imprecise) begin
      err_imprecise_en |= 1'b1;
      err_imprecise_d  |= 1'b1;
    end
  end

  // EN_FF for err_imprecise
  always_ff @(posedge clk_gated) begin
    if (err_imprecise_en) err_imprecise <= err_imprecise_d;
  end

  // err_detected - indicates that an error has been detected
  // Clear at alloc
  // Set when reqq_err_logger detects a precise error
  logic err_detected_d;
  logic err_detected_en;

  always_comb begin
    err_detected_d  = '0;
    err_detected_en = '0;
    if (alloc_me_ag_i) begin
      err_detected_en |= 1'b1;
      err_detected_d  |= 1'b0;
    end
    if (set_reqq_err_detected_me_i) begin
      err_detected_en |= 1'b1;
      err_detected_d  |= 1'b1;
    end
  end

  // EN_FF for err_detected
  always_ff @(posedge clk_gated) begin
    if (err_detected_en) err_detected <= err_detected_d;
  end

  // err_zero_data - used to zero data response
  // Clear at alloc
  // Set when reqq_err_logger detects a decode error (alloc decode, tag_rsp decode, mesh response error)
  logic err_zero_data;
  logic err_zero_data_d;
  logic err_zero_data_en;

  always_comb begin
    err_zero_data_d  = '0;
    err_zero_data_en = '0;
    if (alloc_me_ag_i) begin
      err_zero_data_en |= 1'b1;
      err_zero_data_d  |= 1'b0;
    end
    if (set_reqq_err_zero_data_me_i) begin
      err_zero_data_en |= 1'b1;
      err_zero_data_d  |= 1'b1;
    end
  end

  // EN_FF for err_zero_data
  always_ff @(posedge clk_gated) begin
    if (err_zero_data_en) err_zero_data <= err_zero_data_d;
  end

  // ═══════════════════════════════════════════════════════
  // rspmux_qwen — combinational
  // ═══════════════════════════════════════════════════════
  //
  // rspmux will send back read response qwens that are function of the size and address.
  // This is different from the qwen in the entry which may be all 1 due to a fill response at the
  // same time as the rspmux et-link response.

  assign rspmux_qwen =
    (reqq_source_q == ReqqSrcL3)          ? '1 :
    (orig_opcode_q == OrigMsgSendData)    ? qwen :
                                            gen_qwen(size_q, address);

  // ═══════════════════════════════════════════════════════
  // reqq_entry_state_o output struct
  // ═══════════════════════════════════════════════════════

  assign reqq_entry_state_o.trans_id                = trans_id_q;
  assign reqq_entry_state_o.valid                   = valid;
  assign reqq_entry_state_o.reqq_id                 = my_reqq_id_i;
  assign reqq_entry_state_o.dataq_id                = dataq_id;
  assign reqq_entry_state_o.tag_id                  = tag_id_q;
  assign reqq_entry_state_o.source                  = source_q;
  assign reqq_entry_state_o.port_id                 = port_id_q;
  assign reqq_entry_state_o.qos                     = qos_q;
  assign reqq_entry_state_o.orig_opcode             = orig_opcode_q;
  assign reqq_entry_state_o.address_space           = address_space_q;
  assign reqq_entry_state_o.reqq_source             = reqq_source_q;
  assign reqq_entry_state_o.cache_level_start       = cache_level_start;
  assign reqq_entry_state_o.cache_level_dest        = cache_level_dest;
  assign reqq_entry_state_o.scp_is_remote           = scp_is_remote;
  assign reqq_entry_state_o.readcoop_dest           = readcoop_dest;
  assign reqq_entry_state_o.address                 = address;
  assign reqq_entry_state_o.qwen                    = qwen;
  assign reqq_entry_state_o.rspmux_qwen             = rspmux_qwen;
  assign reqq_entry_state_o.size                    = size_q;
  assign reqq_entry_state_o.zero_data               = zero_data;
  assign reqq_entry_state_o.opcode                  = opcode;
  assign reqq_entry_state_o.rspmux_l2_eligible      = rspmux_l2_eligible;
  assign reqq_entry_state_o.rspmux_l3_eligible      = rspmux_l3_eligible;
  assign reqq_entry_state_o.rspmux_sent             = rspmux_sent;
  assign reqq_entry_state_o.pipe_req_eligible       = pipe_req_eligible;
  assign reqq_entry_state_o.subbank                 = subbank;
  assign reqq_entry_state_o.rbuf_req_eligible       = rbuf_req_eligible;
  assign reqq_entry_state_o.to_l3_mesh_req_eligible = to_l3_mesh_req_eligible;
  assign reqq_entry_state_o.to_sys_mesh_req_eligible= to_sys_mesh_req_eligible;
  assign reqq_entry_state_o.which_mesh              = which_mesh;
  assign reqq_entry_state_o.opcode_destined_for_mesh= opcode_destined_for_mesh;
  assign reqq_entry_state_o.inflight                = inflight;
  assign reqq_entry_state_o.data_ready              = data_ready;
  assign reqq_entry_state_o.wait_for_dataq          = wait_for_dataq;
  assign reqq_entry_state_o.dep_valid               = dep_valid;
  assign reqq_entry_state_o.dep_id                  = dep_id;
  assign reqq_entry_state_o.dep_tail                = dep_tail;
  assign reqq_entry_state_o.dep_meshll_tail         = dep_meshll_tail;
  assign reqq_entry_state_o.dep_non_meshll_head     = dep_non_meshll_head;
  assign reqq_entry_state_o.dep_in_meshll           = dep_in_meshll;
  assign reqq_entry_state_o.dont_depend_on_me       = dont_depend_on_me;
  assign reqq_entry_state_o.rbuf_valid              = rbuf_valid;
  assign reqq_entry_state_o.rbuf_pending_valid      = rbuf_pending_valid;
  assign reqq_entry_state_o.rbuf_id                 = rbuf_id;
  assign reqq_entry_state_o.paired_id               = paired_id[ReqqIdSize-1:0];
  assign reqq_entry_state_o.err_detected            = err_detected;
  assign reqq_entry_state_o.err_zero_data           = err_zero_data;
  assign reqq_entry_state_o.err_imprecise           = err_imprecise;
  assign reqq_entry_state_o.traced                  = traced_q;

  // ═══════════════════════════════════════════════════════
  // Output info structs
  // ═══════════════════════════════════════════════════════

  assign pipe_req_info_me_o.trans_id       = trans_id_q;
  assign pipe_req_info_me_o.reqq_id        = my_reqq_id_i;
  // SC_L2_Atomic_2nd_pass is used to clear arbitration priority after the request is picked
  // but pipeline only sees SC_L2_Atomic
  assign pipe_req_info_me_o.opcode         =
    (opcode == L2Atomic2nd)       ? L2Atomic :
    (opcode == L3Atomic2nd)       ? L3Atomic :
    (opcode == L2WritePartial2nd) ? L2WritePartial :
    (opcode == L3WritePartial2nd) ? L3WritePartial :
                                    opcode;
  assign pipe_req_info_me_o.address        = address;
  assign pipe_req_info_me_o.address_space  = address_space_q;
  assign pipe_req_info_me_o.zero_data      = zero_data;
  assign pipe_req_info_me_o.qwen           = qwen;
  assign pipe_req_info_me_o.size           = size_q;
  assign pipe_req_info_me_o.sub_bank_id    = subbank;

  assign rbuf_req_info_me_o.trans_id       = trans_id_q;
  assign rbuf_req_info_me_o.reqq_id        = my_reqq_id_i;
  assign rbuf_req_info_me_o.address        = address;
  assign rbuf_req_info_me_o.rbuf_id        = rbuf_id;
  assign rbuf_req_info_me_o.scp            = (opcode == ScpRead);
  assign rbuf_req_info_me_o.l3_source      = (reqq_source_q == ReqqSrcL3);

  // Atomic issues request from the primary reqq_id and accesses the dataq_id of the paired entry
  assign to_l3_mesh_req_info_o.trans_id    = trans_id_q;
  assign to_l3_mesh_req_info_o.reqq_id     = my_reqq_id_i;
  assign to_l3_mesh_req_info_o.dataq_id    = dataq_id;
  assign to_l3_mesh_req_info_o.data        = '0; // Intentionally driven 0.  Will be filled in by dataq
  // Determine if the picked info needs a dataq read.
  assign to_l3_mesh_req_info_o.with_data   = mesh_opcode_needs_data;
  assign to_l3_mesh_req_info_o.qwen        = '0; // Intentionally driven 0.  Will be filled in by reqq from reqq_entry[dataq_id].qwen
  assign to_l3_mesh_req_info_o.rd_ben      =
    ((orig_opcode_q inside {OrigWriteForwardPart, OrigScpWritePartial}) |
     ((orig_opcode_q == OrigWritePartial) & esr_ctl_i.esr_sc_l2_bypass & (address_space_q == AddressL2)) |
     ((orig_opcode_q == OrigWritePartial) & esr_ctl_i.esr_sc_l3_bypass & (address_space_q == AddressL3)));
  assign to_l3_mesh_req_info_o.ben         = '0; // Intentionally driven 0.  Will be filled in by dataq
  assign to_sys_mesh_req_info_o            = to_l3_mesh_req_info_o;

  assign rspmux_l2_info_o.trans_id   = trans_id_q;
  assign rspmux_l2_info_o.reqq_id    = my_reqq_id_i;
  assign rspmux_l2_info_o.data       = '0;  // Intentionally driven 0.  Will be filled in by dataq
  assign rspmux_l2_info_o.qwen       = rspmux_qwen;
  assign rspmux_l2_info_o.with_data  = (orig_opcode_q inside {
    OrigMsgSendData,
    OrigRead,
    OrigReadCoop,
    OrigScpRead,
    OrigScpReadCoop
  }) |
    // L3 Atomic and Scp Atomic data goes back with to_sys ESR write
    ((orig_opcode_q == OrigAtomic) & (address_space_q == AddressL2));
  assign rspmux_l2_info_o.zero_data  = err_zero_data;
  assign rspmux_l3_info_o            = rspmux_l2_info_o;

  // ═══════════════════════════════════════════════════════
  // Output eligibility assignments
  // ═══════════════════════════════════════════════════════

  assign pipe_req_eligible_o         = pipe_req_eligible;
  assign rbuf_req_eligible_o         = rbuf_req_eligible;
  assign to_l3_mesh_req_eligible_o   = to_l3_mesh_req_eligible;
  assign to_sys_mesh_req_eligible_o  = to_sys_mesh_req_eligible;
  assign rspmux_l2_eligible_o        = rspmux_l2_eligible;
  assign rspmux_l3_eligible_o        = rspmux_l3_eligible;

  // ═══════════════════════════════════════════════════════
  // Unused signals
  // ═══════════════════════════════════════════════════════

  logic unused_signals;
  assign unused_signals = &{1'b0,
    l3_bank_id_unused,
    l3_shire_id_unused,
    1'b0
  };

endmodule
/* verilator lint_on UNUSEDSIGNAL */
