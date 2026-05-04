// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache bank request queue allocator.
//
// Receives a new shire cache bank entry and allocates it into the reqq.
// The module selects which entry to allocate, performs opcode mapping, and
// figures out the allocation linked list state.
//
// Replaces: shire_cache_bank_reqq_alloc (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions (_i/_o)
//   - prim_clk_gate primitive instead of et_clk_gate
//   - Package types instead of `define types
//   - Explicit always_ff instead of RST_FF/EN_FF/FF macros

/* verilator lint_off UNUSEDSIGNAL */  // packed struct ports passed whole; alloc uses subfields only
/* verilator lint_off UNOPTFLAT */     // alloc_info_ag_o struct fields written by independent always_comb blocks; no true circular dependency
module shirecache_bank_reqq_alloc
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter bit          IsL2       = 1,
  parameter int unsigned NumEntries = ReqqDepth,
  parameter int unsigned NumPorts   = Ports,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  dft_t                          dft_i,
  input  logic [NumEntries-1:0]         reqq_allowed_entries_i,
  output logic                          reqq_full_o,

  // L2 Requests to bank
  output logic                          bank_ready_o,
  input  logic                          bank_valid_i,
  input  reqq_bank_req_t                bank_info_i,
  output logic [NumEntriesId-1:0]       reqq_alloc_id_ag_o,
  input  logic                          dataq_bank_ready_i,  // dataq has room for incoming write

  // Idx Requests from CacheOp FSM
  input  logic                          pipe_idx_cop_aborted_i,
  input  logic                          pipe_idx_cop_l2_busy_i,
  input  logic                          pipe_idx_cop_l3_busy_i,
  output logic                          pipe_idx_cop_req_ready_o,
  input  logic                          pipe_idx_cop_req_valid_i,
  input  idx_cop_req_t                  pipe_idx_cop_req_info_i,

  // Interfaces with the pipeline
  input  logic                          pipe_tag_rsp_valid_i,
  input  tag_rsp_t                      pipe_tag_rsp_info_i,

  // Interfaces with the read buffer
  input  rbuf_clear_req_t               pipe_rbuf_clear_req_info_i,
  input  rbuf_state_t                   pipe_rbuf_state_i,

  // Alloc matching victims
  output logic                          alloc_match_tag_victim_ag_o,

  // reqq state
  input  logic [NumEntries-1:0]         reqq_entry_valid_i,
  input  logic [NumEntries-1:0]         upgrade_to_meshll_hot_i,
  input  logic [NumEntries-1:0]         entry_is_non_meshll_i,
  input  logic [NumEntries-1:0]         clear_dep_hot_i,
  input  reqq_entry_state_t [NumEntries-1:0] reqq_state_i,

  // Alloc outputs
  output logic                          alloc_valid_ag_o,
  output logic [NumEntries-1:0]         alloc_1hot_ag_o,
  output reqq_alloc_t                   alloc_info_ag_o,

  output logic                          alloc_valid_ad_o,
  output logic [NumEntries-1:0]         alloc_1hot_ad_o,
  output reqq_alloc_t                   alloc_info_ad_o,

  // Alloc paired outputs
  output logic [NumEntries-1:0]         alloc_1hot_paired_ag_o,
  output reqq_alloc_t                   alloc_info_paired_ag_o,

  // mbist_on stalls l2 and l3 inputs
  input  logic                          mbist_on_i,

  // ESRs
  input  esr_shire_config_t             esr_shire_config_i,
  input  logic                          esr_wr_reqq_entries_i,
  input  bank_esr_ctl_t                 esr_ctl_i
);

  localparam bit IsL3 = (IsL2 == 1'b0);
  localparam logic [PortIdSize-1:0] IllegalPortId = {PortIdSize{1'b1}};

  // Subopcode (0=L2/SCP, 1=L3)
  localparam logic SubOpcodeL3 = 1'b1;

  // Trace enable index for reqq
  localparam int unsigned TraceEnReqq = 7;

  logic alloc_2nd_half_needed_ad;

  // ── Clock gate ───────────────────────────────────────────
  wire clock_en = ~rst_ni
    | bank_valid_i
    | pipe_idx_cop_req_valid_i
    | alloc_valid_ad_o
    | alloc_2nd_half_needed_ad
    | esr_wr_reqq_entries_i
    | esr_ctl_i.esr_sc_clk_gate_disable[CgateDisReqqAlloc];
  logic clk_gated;
  prim_clk_gate u_clk_gate (
    .clk_i  (clk_i),
    .en_i   (clock_en),
    .dft_i  (dft_i),
    .clk_o  (clk_gated)
  );


  // **************************************************************************
  // Cop FSM Idx CacheOps
  // **************************************************************************
  // Index cacheOps from FSM have priority
  // The L2 alloc module is responsible for holding off Neigh traffic while
  // Cop FSM is running as well as allocating all Cop FSM requests to L2 reqq entries.
  // The L3 alloc module has to holdoff L3 slave requests if the COP FSM is
  // running an L3 Inv, Flush, or Evict.
  wire idx_cop_req_waiting_on_sync;
  wire idx_cop_req_waiting_on_sync_plus_victims;
  wire idx_cop_req_waiting_on_l2;
  wire idx_cop_req_waiting_on_l3;

  // Don't allow neighborhood requests if Cop FSM is running.
  // Also don't allow L3 slave requests if Cop FSM is running an L3/All Inv, Evict, Flush
  // Can't stall L3 if the L2 is also being stalled and still has requests outstanding.
  //    Stalling both L2 and L3 while L2 still has requests in flight would be a deadlock.
  // Cop FSM abort removes the idx cop stall
  logic mbist_on_r;
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) mbist_on_r <= '0;
    else         mbist_on_r <= mbist_on_i;
  end
  wire idx_cop_stall_bank_l2 = IsL2 & pipe_idx_cop_l2_busy_i;
  wire idx_cop_stall_bank_l3 = IsL3 & pipe_idx_cop_l3_busy_i & ~idx_cop_req_waiting_on_l2;
  wire idx_cop_stall_bank = (idx_cop_stall_bank_l2 | idx_cop_stall_bank_l3 | mbist_on_r) & ~pipe_idx_cop_aborted_i;


  // Decide whether we allocate into L2 or L3.
  // L3 Inv, Evict, Flush allocate into L3, all others allocate into L2.
  // If pipe_idx_cop_l3_busy and ~pipe_idx_cop_l2_busy, then allocate into L3.
  // If we allocate into L2 with L3 busy and ~L2 busy, then we can get into
  // deadlock because all L3 reqq entries are free, but L2 entries are all full
  // and waiting on mesh responses that are stalled because L3 is busy.
  wire idx_in_this_alloc = (pipe_idx_cop_l3_busy_i & ~pipe_idx_cop_l2_busy_i) ? IsL3 : IsL2;

  wire sel_idx  = idx_in_this_alloc & pipe_idx_cop_req_valid_i;
  wire take_idx = idx_in_this_alloc & pipe_idx_cop_req_valid_i & pipe_idx_cop_req_ready_o;

  // Some of the Idx CacheOps wait until all Neigh instructions are
  // finished and deallocated.
  // L3 Inv, Evict, Flush also wait until all L3 instructions are
  // finished and deallocated.
  logic [NumEntries-1:0] valid_l2_entries;
  logic [NumEntries-1:0] valid_l3_entries;
  logic [NumEntries-1:0] valid_fsm_entries;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      valid_l2_entries[ii] = reqq_entry_valid_i[ii] &
        (reqq_state_i[ii].reqq_source == ReqqSrcL2);
      valid_l3_entries[ii] = reqq_entry_valid_i[ii] &
        (reqq_state_i[ii].reqq_source == ReqqSrcL3);
      valid_fsm_entries[ii] = reqq_entry_valid_i[ii] &
        (reqq_state_i[ii].reqq_source == ReqqSrcFsm);
    end
  end

  // Accept a Sync if all FSM entries in front of it are done.
  assign idx_cop_req_waiting_on_sync =
    (pipe_idx_cop_req_info_i.opcode == OpSync) &
    ~pipe_idx_cop_req_info_i.sync_flush_victims &
    (|valid_fsm_entries);

  // Accept a Sync if all FSM entries in front of it are done
  // and all in-flight WAs are sent to Mesh.
  logic wait_for_write_arounds;
  assign idx_cop_req_waiting_on_sync_plus_victims =
    (pipe_idx_cop_req_info_i.opcode == OpSync) &
    pipe_idx_cop_req_info_i.sync_flush_victims &
    (|{valid_fsm_entries, wait_for_write_arounds});

  // Accept an Idx Inv/Flush/Evict if all L2 reqs in front are done.
  assign idx_cop_req_waiting_on_l2 =
    pipe_idx_cop_l2_busy_i &
    (|valid_l2_entries);

  // Accept a L3 Idx Inv/Flush/Evict if all L3 reqs in front are done.
  assign idx_cop_req_waiting_on_l3 =
    pipe_idx_cop_l3_busy_i &
    (|valid_l3_entries);

  // Accept a pipe idx request if not second half of atomic / writeAround in front of it,
  // There is room in reqq and the Cop opcode stall conditions have been met.
  // Cop FSM abort any outstanding idx cop request waiting to alloc to reqq
  logic [NumEntries-1:0] reqq_alloc_winner_1hot;
  logic [NumEntries-1:0] reqq_alloc_winner_paired_1hot;
  logic reqq_alloc_valid;
  assign pipe_idx_cop_req_ready_o =
    idx_in_this_alloc &
    pipe_idx_cop_req_valid_i &
    ~pipe_idx_cop_aborted_i &
    reqq_alloc_valid &
    ~alloc_2nd_half_needed_ad &  // need a second reqq entry for atomic or writeAround
    ~idx_cop_req_waiting_on_sync &
    ~idx_cop_req_waiting_on_sync_plus_victims &
    ~idx_cop_req_waiting_on_l3 &
    ~idx_cop_req_waiting_on_l2;


  // Select the destination level if the Cop Index generates a victim.
  cache_level_e idx_cop_dest_level;
  assign idx_cop_dest_level =
    (pipe_idx_cop_req_info_i.opcode inside {IdxL3Inv, IdxL3Flush, IdxL3Evict}) ? CacheMem : CacheL3;


  // When we first get a pipe_idx_cop_req_valid with sync_flush_victims set capture which reqq entries
  // may be working with a WriteAround.  All of these entries must complete before the
  // coalescing buffer invalidate Sync can be acknowledged.
  // Watch for rising edge of coalescing buffer sync.
  wire cb_sync =
    (pipe_idx_cop_req_info_i.opcode == OpSync) &
     pipe_idx_cop_req_info_i.sync_flush_victims &
     pipe_idx_cop_req_valid_i;
  logic cb_sync_r1;
  always_ff @(posedge clk_gated) cb_sync_r1 <= cb_sync;
  wire cb_sync_re = cb_sync & ~cb_sync_r1;

  // Write arounds that are in-flight
  logic [NumEntries-1:0] possible_write_arounds;
  logic [NumEntries-1:0] write_arounds_in_flight;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      possible_write_arounds[ii] = reqq_entry_valid_i[ii] &
        ((reqq_state_i[ii].orig_opcode inside {OrigWriteAround, OrigWriteAround2}) |
         (reqq_state_i[ii].opcode inside {MeshWrite, MeshEvict, MeshVictim}));
    end
  end

  logic [NumEntries-1:0] write_arounds_state_next;
  // If starting a coalescing buffer invalidate sync, then capture the write arounds.
  // Turn them off as the reqq entries go invalid.
  assign write_arounds_state_next =
    (cb_sync_re) ? possible_write_arounds :                        // initial snapshot
                   write_arounds_in_flight & reqq_entry_valid_i;   // turn off bits as they finish

  always_ff @(posedge clk_gated) begin
    if (cb_sync) write_arounds_in_flight <= write_arounds_state_next;
  end

  // Wait for write around to clear.
  // Also wait during cb_sync_re while snapshotting state
  assign wait_for_write_arounds = cb_sync_re | (|write_arounds_in_flight);

  // **************************************************************************
  // Allocate
  // **************************************************************************

  // Full if all valid or not allowed for this request type
  assign reqq_full_o = &(reqq_entry_valid_i | ~reqq_allowed_entries_i);

  // Active idx cop override incoming request
  wire idx_cop_req_valid = pipe_idx_cop_req_valid_i & idx_in_this_alloc;

  // Bank ready
  assign bank_ready_o = reqq_alloc_valid &
    dataq_bank_ready_i &   // dataq has room
    ~alloc_2nd_half_needed_ad & // need a second reqq entry for atomic or writeAround
    ~idx_cop_stall_bank &  // Cop FSM has priority over neighborhood requests or l3_slave requests
    ~idx_cop_req_valid;    // Cop FSM active

  // We need to allocate if something is coming in from neigh or idx CacheOp FSM
  // or if there is an Atomic or WriteAround waiting for a second reqq entry to be allocated
  assign alloc_valid_ag_o =
    ((bank_valid_i & bank_ready_o) |
     (pipe_idx_cop_req_valid_i & pipe_idx_cop_req_ready_o) |
     (alloc_2nd_half_needed_ad & reqq_alloc_valid));

  // Binary index of the reqq we're allocating
  logic [$clog2(NumEntries)-1:0] alloc_id_ag_bin;
  logic [$clog2(NumEntries)-1:0] alloc_id_paired_ag_bin;

  logic [NumEntriesId-1:0] alloc_id_ag;
  logic [NumEntriesId-1:0] alloc_id_paired_ag;

  // Opcodes that need a second reqq_entry
  wire alloc_orig_opcode_needs_paired =
    (alloc_info_ag_o.orig_opcode inside {OrigWriteAround, OrigAtomic, OrigWritePartial});

  logic reqq_alloc_paired_valid;
  wire alloc_valid_paired_ag =
    reqq_alloc_paired_valid &
    (bank_valid_i & bank_ready_o) &
    ~alloc_info_ag_o.err_valid &
    alloc_orig_opcode_needs_paired;

  // Round robin on non-valid entries.
  prim_arb_rr2 #(
    .NumReqs(NumEntries)
  ) u_arb_alloc (
    .clk_i      (clk_gated),
    .rst_ni     (rst_ni),
    .reqs_i     (~reqq_entry_valid_i & reqq_allowed_entries_i),  // non-valid entries that are allowed
    .valid_o    (reqq_alloc_valid),
    .valid2_o   (reqq_alloc_paired_valid),
    .grants_o   (reqq_alloc_winner_1hot),
    .grants2_o  (reqq_alloc_winner_paired_1hot),
    .grants_id_o  (alloc_id_ag_bin),
    .grants2_id_o (alloc_id_paired_ag_bin),
    .alloc_i    (alloc_valid_ag_o),
    .esr_wr_i   (esr_wr_reqq_entries_i)
  );

  assign alloc_id_ag = NumEntriesId'(alloc_id_ag_bin);
  assign alloc_id_paired_ag = NumEntriesId'(alloc_id_paired_ag_bin);
  assign reqq_alloc_id_ag_o = alloc_id_ag;

  assign alloc_1hot_ag_o =
    {NumEntries{alloc_valid_ag_o}} & reqq_alloc_winner_1hot;

  assign alloc_1hot_paired_ag_o =
    {NumEntries{alloc_valid_paired_ag}} & reqq_alloc_winner_paired_1hot;

  // Map incoming req and opcode to flat orig_opcode
  // --------------------------------------------------------------------------
  assign alloc_info_ag_o.reqq_id   = alloc_id_ag;
  assign alloc_info_ag_o.paired_id = alloc_2nd_half_needed_ad ? alloc_info_ad_o.reqq_id : alloc_id_paired_ag;

  // SCP shire id
  wire [ScpShireIdSize-1:0] my_scp_virtual_shire_id = esr_shire_config_i.shire_id;

  localparam logic [ScpShireIdSize-1:0] LocalShireId = {ScpShireIdSize{1'b1}};
  wire address_is_scp_ag             = (alloc_info_ag_o.address[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);
  wire scp_is_remote_ag              = (alloc_info_ag_o.address[ScpShireLsb +: ScpShireIdSize] != my_scp_virtual_shire_id) &
                                       (alloc_info_ag_o.address[ScpShireLsb +: ScpShireIdSize] != LocalShireId);
  wire bank_address_is_scp_ag        = (bank_info_i.address[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);
  wire bank_scp_is_remote_ag         = (bank_info_i.address[ScpShireLsb +: ScpShireIdSize] != my_scp_virtual_shire_id) &
                                       (bank_info_i.address[ScpShireLsb +: ScpShireIdSize] != LocalShireId);

  wire request_is_scp_to_l2_ag       = bank_scp_is_remote_ag &
    (bank_info_i.opcode inside {etlink_pkg::ReqWriteAround, etlink_pkg::ReqEvict});

  // Remap local shire id to my_scp_virtual_shire_id
  wire [PaSize-1:0] bank_info_address_ag = (bank_address_is_scp_ag & ~bank_scp_is_remote_ag)
    ? {bank_info_i.address[PaSize-1:ScpShireMsb+1], my_scp_virtual_shire_id, bank_info_i.address[ScpShireLsb-1:0]}
    : bank_info_i.address;

  cache_level_e scp_dest_level_ag;
  assign scp_dest_level_ag =
    (IsL3)            ? CacheL3 :
    (scp_is_remote_ag) ? CacheL3 :
                         CacheL2;


  // Determine address_space
  always_comb begin
    if (~alloc_2nd_half_needed_ad) begin
      // All CacheOp FSM requests are index except for L2_Evict (coalescing buffer evict) and Scp_Zero
      if (idx_cop_req_valid) begin
        if (pipe_idx_cop_req_info_i.opcode == L2Evict) begin
          alloc_info_ag_o.address_space = AddressL2;
        end
        else if (pipe_idx_cop_req_info_i.opcode == ScpZero) begin
          alloc_info_ag_o.address_space = AddressScp;
        end
        else begin
          alloc_info_ag_o.address_space = AddressIdx;
        end
      end
      // Messages
      else if (IsL2 & (alloc_info_ag_o.orig_opcode inside {OrigMsgSendData})) begin
        alloc_info_ag_o.address_space = AddressMsg;
      end
      // PAs that are in the SCP address space
      else if (address_is_scp_ag) begin
        if (request_is_scp_to_l2_ag) begin
          // Scp WriteAround and Evict is treated as L2 address space.
          alloc_info_ag_o.address_space = AddressL2;
        end
        else begin
          alloc_info_ag_o.address_space = AddressScp;
        end
      end
      // Else L2
      else if (IsL2) begin
        alloc_info_ag_o.address_space = AddressL2;
      end
      // Or L3
      else begin
        alloc_info_ag_o.address_space = AddressL3;
      end
    end
    else begin
      alloc_info_ag_o.address_space = alloc_info_ad_o.address_space;
    end
  end

  wire alloc_is_scp_ag = alloc_info_ag_o.address_space == AddressScp;

  assign alloc_info_ag_o.scp_fill_source_is_scp =
    (bank_info_i.data[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);

  assign alloc_info_ag_o.reqq_source =
    (pipe_idx_cop_req_valid_i & pipe_idx_cop_req_ready_o) ? ReqqSrcFsm :
    (IsL2) ? ReqqSrcL2 :
             ReqqSrcL3 ;

  wire forward_to_next_level = ~address_is_scp_ag & (bank_info_i.subopcode[0] == SubOpcodeL3);

  always_comb begin
    alloc_info_ag_o.orig_opcode = reqq_orig_opcode_e'('0);
    alloc_info_ag_o.opcode = ReqqInitial;
    // If allocating the second half of an opcode that needs two entries (Atomic, WriteAround, WritePartials)
    if (alloc_2nd_half_needed_ad && (alloc_info_ad_o.orig_opcode == OrigAtomic)) begin
      alloc_info_ag_o.orig_opcode = OrigAtomic2;
    end
    else if (IsL2 & alloc_2nd_half_needed_ad && (alloc_info_ad_o.orig_opcode == OrigWriteAround)) begin
      alloc_info_ag_o.orig_opcode = OrigWriteAround2;
    end
    else if (alloc_2nd_half_needed_ad && (alloc_info_ad_o.orig_opcode inside {OrigWritePartial, OrigScpWritePartial})) begin
      alloc_info_ag_o.orig_opcode = OrigWritePartial2;
    end
    else if (sel_idx) begin
      alloc_info_ag_o.opcode = pipe_idx_cop_req_info_i.opcode;
      case (pipe_idx_cop_req_info_i.opcode)
        IdxRead:      alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxWrite:     alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxAllInv:    alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL2Inv:     alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL2Flush:   alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL2Evict:   alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL3Inv:     alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL3Flush:   alloc_info_ag_o.orig_opcode = OrigIdx;
        IdxL3Evict:   alloc_info_ag_o.orig_opcode = OrigIdx;
        L2Evict:      alloc_info_ag_o.orig_opcode = OrigIdx;   // Coalescing buffer evicts.
        ScpZero:      alloc_info_ag_o.orig_opcode = OrigIdx;   // Scratchpad zero
        OpSync:       alloc_info_ag_o.orig_opcode = OrigSync;
        default:      alloc_info_ag_o.orig_opcode = OrigSync;
      endcase
    end
    else begin
      if (IsL2) begin
        case (bank_info_i.opcode)
          etlink_pkg::ReqRead:         alloc_info_ag_o.orig_opcode =
            (forward_to_next_level) ? OrigReadForward :
            (address_is_scp_ag)     ? OrigScpRead :
                                      OrigRead;
          etlink_pkg::ReqReadCoop:     alloc_info_ag_o.orig_opcode =
            (address_is_scp_ag) ? OrigScpReadCoop : OrigReadCoop;
          etlink_pkg::ReqWrite:        alloc_info_ag_o.orig_opcode =
            (forward_to_next_level &  bank_info_i.write_is_partial) ? OrigWriteForwardPart :
            (forward_to_next_level & ~bank_info_i.write_is_partial) ? OrigWriteForward :
            ( address_is_scp_ag &  bank_info_i.write_is_partial)    ? OrigScpWritePartial :
            ( address_is_scp_ag & ~bank_info_i.write_is_partial)    ? OrigScpWrite :
            (~address_is_scp_ag &  bank_info_i.write_is_partial)    ? OrigWritePartial :
                                                                      OrigWrite;
          etlink_pkg::ReqWriteAround:  alloc_info_ag_o.orig_opcode = OrigWriteAround;
          etlink_pkg::ReqLock:         alloc_info_ag_o.orig_opcode = OrigCopLock;
          etlink_pkg::ReqUnlock:       alloc_info_ag_o.orig_opcode =
            (bank_info_i.data[UnlockInvBit]) ? OrigCopUnlock : OrigCopUnlockInv;
          etlink_pkg::ReqFlush:        alloc_info_ag_o.orig_opcode = OrigCopFlush;
          //etlink_pkg::ReqFlushToMem:   alloc_info_ag_o.orig_opcode = OrigCopFlushWData;
          etlink_pkg::ReqEvict:        alloc_info_ag_o.orig_opcode = OrigCopEvict;
          //etlink_pkg::ReqEvictToMem:   alloc_info_ag_o.orig_opcode = OrigCopEvictWData;
          etlink_pkg::ReqPrefetch:     alloc_info_ag_o.orig_opcode = OrigCopPrefetch;
          etlink_pkg::ReqScpFill:      alloc_info_ag_o.orig_opcode = OrigCopScpFill;
          etlink_pkg::ReqMsgSendData:  alloc_info_ag_o.orig_opcode = OrigMsgSendData;
          etlink_pkg::ReqAtomic:       alloc_info_ag_o.orig_opcode = OrigAtomic;
          default:                     alloc_info_ag_o.orig_opcode = OrigErr;
        endcase
      end
      else begin // IsL3
        case (bank_info_i.opcode)
          etlink_pkg::ReqRead:         alloc_info_ag_o.orig_opcode =
            (address_is_scp_ag) ? OrigScpRead : OrigRead;
          etlink_pkg::ReqWrite:        alloc_info_ag_o.orig_opcode =
            ( address_is_scp_ag &  bank_info_i.write_is_partial) ? OrigScpWritePartial :
            ( address_is_scp_ag & ~bank_info_i.write_is_partial) ? OrigScpWrite :
            (~address_is_scp_ag &  bank_info_i.write_is_partial) ? OrigWritePartial :
                                                                   OrigWrite;
          etlink_pkg::ReqFlush:        alloc_info_ag_o.orig_opcode = OrigCopFlush;
          etlink_pkg::ReqFlushToMem:   alloc_info_ag_o.orig_opcode = OrigCopFlushWData;
          etlink_pkg::ReqEvict:        alloc_info_ag_o.orig_opcode = OrigCopEvict;
          etlink_pkg::ReqEvictToMem:   alloc_info_ag_o.orig_opcode = OrigCopEvictWData;
          etlink_pkg::ReqPrefetch:     alloc_info_ag_o.orig_opcode = OrigCopPrefetch;
          etlink_pkg::ReqAtomic:       alloc_info_ag_o.orig_opcode = OrigAtomic;
          default:                     alloc_info_ag_o.orig_opcode = OrigErr;
        endcase
      end
    end
    // Set to ReqqInitial2 if the opcode that needs a pair is getting it this cycle
    if (alloc_valid_paired_ag & alloc_orig_opcode_needs_paired) begin
      alloc_info_ag_o.opcode = ReqqInitial2;
    end
  end

  // Zero data is being used to set the zero bit in the tag state ram.
  // For WriteAround the zero bit is the accumulation of continued zero.
  // Partial writes do not set the zero data bit.  Don't know if the backing data is zero.
  wire bank_info_zero_data_pre =
    (alloc_info_ag_o.orig_opcode inside {OrigWrite, OrigScpWrite, OrigWriteAround}) &
    (~bank_info_i.qwen[3] | ~(|bank_info_i.data[511:384])) &
    (~bank_info_i.qwen[2] | ~(|bank_info_i.data[383:256])) &
    (~bank_info_i.qwen[1] | ~(|bank_info_i.data[255:128])) &
    (~bank_info_i.qwen[0] | ~(|bank_info_i.data[127:0]));

  // L2 WriteAround and ScpWrite can have swiss cheese quad word enables
  // L2 MsgSendData and Atomic can have non-full quad word enables
  // L3 Writes, FlushToMem and EvictToMem can have swiss cheese quad word enables
  // L2/L3 Writes can have swiss cheese ben
  logic [LineQwSize-1:0] bank_info_qwen_pre;
  assign bank_info_qwen_pre =
    (alloc_2nd_half_needed_ad) ? LineQwSize'('1) :
    (bank_info_i.opcode inside {
       etlink_pkg::ReqWriteAround,
       etlink_pkg::ReqMsgSendData,
       etlink_pkg::ReqAtomic,
       etlink_pkg::ReqFlushToMem,
       etlink_pkg::ReqEvictToMem,
       etlink_pkg::ReqWrite}) ? bank_info_i.qwen : LineQwSize'('1);

  // Size is always a line.
  // Except for
  // - atomics where size is encoded with the atomic data.
  // - msgRecvData where size is from msgSendData.
  amo_conf_req_t         atomic_conf;
  sc_size_t              bank_info_size_pre;
  always_comb begin
    bank_info_size_pre         = bank_info_i.size;
    atomic_conf                = '0;
    if (bank_info_i.opcode == etlink_pkg::ReqAtomic) begin
      // For L3 atomic and Scp atomic
      atomic_conf             = IsL2
        ? amo_conf_req_t'(bank_info_i.subopcode)
        : amo_conf_req_t'(bank_info_i.data[AtomicConfLsb +: AtomicConfSize]);
      bank_info_size_pre      = atomic_conf.sc_vec ? ((AtomicSimdSize == 4) ? etlink_pkg::SizeQWord : etlink_pkg::SizeHLine) :
                                atomic_conf.size   ? etlink_pkg::SizeDWord : etlink_pkg::SizeWord;
    end
  end

  // Overwrite alloc_info_ag fields if necessary.
  always_comb begin
    // if allocating a second half, then use address from first half
    if (alloc_2nd_half_needed_ad) begin
      alloc_info_ag_o.address   = alloc_info_ad_o.address;
      alloc_info_ag_o.trans_id  = alloc_info_ad_o.trans_id;
      alloc_info_ag_o.zero_data = '0;
      alloc_info_ag_o.qwen      = bank_info_qwen_pre;
      alloc_info_ag_o.size      = etlink_pkg::SizeLine;
      alloc_info_ag_o.qos       = alloc_info_ad_o.qos;
    end
    else if (sel_idx) begin
      alloc_info_ag_o.address   = pipe_idx_cop_req_info_i.address;
      alloc_info_ag_o.trans_id  = pipe_idx_cop_req_info_i.trans_id;
      alloc_info_ag_o.zero_data = '0;
      alloc_info_ag_o.qwen      = '0;
      alloc_info_ag_o.size      = etlink_pkg::SizeLine;
      alloc_info_ag_o.qos       = (esr_ctl_i.esr_sc_axi_qos == AxiQosMemHighPriority) ? AxiAxqosSize'(1) : '0;
    end
    else begin
      alloc_info_ag_o.address   = bank_info_address_ag;
      alloc_info_ag_o.trans_id  = bank_info_i.trans_id;
      alloc_info_ag_o.zero_data = bank_info_zero_data_pre;
      alloc_info_ag_o.qwen      = bank_info_qwen_pre;
      alloc_info_ag_o.size      = bank_info_size_pre;
      alloc_info_ag_o.qos       = bank_info_i.qos;
    end
  end

  // Read Coop multi-hot destination
  assign alloc_info_ag_o.readcoop_dest = (IsL3) ? '0 : bank_info_i.data[NeighCoopMaskLsb +: NeighCoopMaskSize];

  // If L3 then assign the start and dest levels.
  // We can do that because Flushes and Evicts don't go beyond L3.  If they got here, they are L3.
  // For Prefetches the Level was handled at L2 and sent to the correct to_l3 or to_sys port.
  // For Scratchpad the Level depends on shire_id.  Either request L2 or send to mesh.
  // For Index, the level controls which mesh port to send MESH_Write to.
  assign alloc_info_ag_o.cache_level_start = (IsL3) ? CacheL2 : cache_level_e'(bank_info_i.data[StartLevelLsb +: 2]);
  assign alloc_info_ag_o.cache_level_dest  =
    (sel_idx) ? idx_cop_dest_level :
    (IsL3) ? CacheL3 :
    (IsL2 & alloc_is_scp_ag) ? scp_dest_level_ag :
    cache_level_e'(bank_info_i.data[DestLevelLsb +: 2]);
  assign alloc_info_ag_o.scp_is_remote = scp_is_remote_ag;

  // MsgSendData uses the address field to indicate
  // - destination port ID
  // - destination ID
  // - destination source
  always_comb begin
    alloc_info_ag_o.port_id = bank_info_i.port_id;
    alloc_info_ag_o.tag_id  = bank_info_i.id;
    alloc_info_ag_o.source  = bank_info_i.source;
    if (bank_info_i.opcode == etlink_pkg::ReqMsgSendData) begin
      if (bank_info_i.address[MsgNeighIdLsb +: MsgNeighIdSize] >= MsgNeighIdSize'(NumPorts)) begin
        alloc_info_ag_o.port_id = IllegalPortId;
      end
      else begin
        alloc_info_ag_o.port_id = PortIdSize'(bank_info_i.address[MsgNeighIdLsb +: MsgNeighIdSize]);
      end
      alloc_info_ag_o.tag_id  = ReqqTagIdSize'(bank_info_i.address[MsgIdLsb +: MsgIdSize]);
      alloc_info_ag_o.source  = ReqqSourceSize'(bank_info_i.address[MsgSourceLsb +: MsgSourceSize]);
    end
  end

  // Determine whether req should be traced.
  // --------------------------------------------------------------------------
  // address
  wire trace_address_match =
    (esr_ctl_i.esr_sc_trace_filter_address_enable & alloc_info_ag_o.address) ==
    (esr_ctl_i.esr_sc_trace_filter_address_enable & esr_ctl_i.esr_sc_trace_filter_address_value);
  // et_link port
  wire trace_port_match = ~esr_ctl_i.esr_sc_trace_filter_port_enable |
    (bank_info_i.port_id == esr_ctl_i.esr_sc_trace_filter_port);
  // et_link source
  wire trace_source_match = ~esr_ctl_i.esr_sc_trace_filter_source_enable |
    (bank_info_i.source == esr_ctl_i.esr_sc_trace_filter_source);
  // l2, l3, fsm
  // Note, that esr_sc_trace_filter_l2_enable set to 1 disables tracing l2.
  wire trace_reqq_source_match =
    (~esr_ctl_i.esr_sc_trace_filter_l2_enable  & (alloc_info_ag_o.reqq_source == ReqqSrcL2)) |
    (~esr_ctl_i.esr_sc_trace_filter_l3_enable  & (alloc_info_ag_o.reqq_source == ReqqSrcL3)) |
    (~esr_ctl_i.esr_sc_trace_filter_fsm_enable & (alloc_info_ag_o.reqq_source == ReqqSrcFsm));
  // reqq_id
  wire trace_reqq_id_match = ~esr_ctl_i.esr_sc_trace_filter_reqq_id_enable |
    (alloc_info_ag_o.reqq_id == esr_ctl_i.esr_sc_trace_filter_reqq_id);

  // If tracing reqq state, then only allow one reqq to be traced at a time.
  logic [NumEntries-1:0] reqq_traced_hot;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      reqq_traced_hot[ii] = reqq_entry_valid_i[ii] & reqq_state_i[ii].traced;
    end
  end
  wire limited_to_single_trace = esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnReqq] &
    (|reqq_traced_hot);

  assign alloc_info_ag_o.traced =
    trace_address_match &
    trace_port_match &
    trace_source_match &
    trace_reqq_source_match &
    trace_reqq_id_match &
    ~limited_to_single_trace;

  // Error detection
  // --------------------------------------------------------------------------
  // Illegal incoming commands

  // scp op to remote shire while scp remote is not enabled.
  wire err_scp_remote_not_enabled = alloc_valid_ag_o & bank_valid_i & bank_ready_o &
    alloc_is_scp_ag &
    scp_is_remote_ag &
    !esr_ctl_i.esr_sc_remote_scp_enable;

  // Illegal opcode
  wire err_unexpected_opcode = alloc_valid_ag_o & bank_valid_i & bank_ready_o & (
    // Illegal opcode covered by OrigErr
    //    Neigh     FlushToMem, EvictToMem, AtomicRsp
    //    L3Slave   WriteAround, ReadCoop, MsgSendData, Lock, UnLock, ScpFill, AtomicRsp
    (alloc_info_ag_o.orig_opcode == OrigErr) |
    IsL2 & (
    // Illegal opcode from Neigh
      //  - Atomic            SCP
      //  - Flush             SCP
      //  - Lock              SCP
      //  - UnLock            SCP
      //  - Prefetch          SCP
      //  - WriteAround       SCP(local)
      //  - Evict             SCP(local) SCP(remote otherlevels not l2 to l3)
      (bank_address_is_scp_ag & (
        (bank_info_i.opcode == etlink_pkg::ReqAtomic) |
        (bank_info_i.opcode == etlink_pkg::ReqFlush) |
        (bank_info_i.opcode == etlink_pkg::ReqLock) |
        (bank_info_i.opcode == etlink_pkg::ReqUnlock) |
        (bank_info_i.opcode == etlink_pkg::ReqPrefetch) |
        ((bank_info_i.opcode == etlink_pkg::ReqWriteAround) & ~bank_scp_is_remote_ag) |
        ((bank_info_i.opcode == etlink_pkg::ReqEvict) & (~bank_scp_is_remote_ag |
                                                         ~((alloc_info_ag_o.cache_level_start == CacheL2) &
                                                           (alloc_info_ag_o.cache_level_dest == CacheL3)))))) |
      //  - ScpFill           SCP(remote)+L2
      ((bank_info_i.opcode == etlink_pkg::ReqScpFill) & (~bank_address_is_scp_ag | bank_scp_is_remote_ag))) |
    IsL3 & (
    // Illegal opcode from L3Slave
      //  - Read              SCP(remote)
      //  - Write             SCP(remote)
      //  - Atomic            SCP(remote)
      (bank_address_is_scp_ag & bank_scp_is_remote_ag & (
        (bank_info_i.opcode == etlink_pkg::ReqRead) |
        (bank_info_i.opcode == etlink_pkg::ReqWrite) |
        (bank_info_i.opcode == etlink_pkg::ReqAtomic))) |
      //  - Flush             SCP
      //  - FlushToMem        SCP
      //  - Evict             SCP
      //  - EvictToMem        SCP
      //  - Prefetch          SCP
      (bank_address_is_scp_ag & (
        (bank_info_i.opcode == etlink_pkg::ReqFlush) |
        (bank_info_i.opcode == etlink_pkg::ReqFlushToMem) |
        (bank_info_i.opcode == etlink_pkg::ReqEvict)      |
        (bank_info_i.opcode == etlink_pkg::ReqEvictToMem) |
        (bank_info_i.opcode == etlink_pkg::ReqPrefetch)))));

  // L2 Atomic while L2 bypassed
  wire err_l2_atomic_while_l2_bypass = alloc_valid_ag_o & bank_valid_i & bank_ready_o &
    (alloc_info_ag_o.orig_opcode == OrigAtomic) & IsL2 &
    esr_ctl_i.esr_sc_l2_bypass;

  // L3 Atomic while L3 bypassed
  wire err_l3_atomic_while_l3_bypass = alloc_valid_ag_o & bank_valid_i & bank_ready_o &
    (alloc_info_ag_o.orig_opcode == OrigAtomic) & IsL3 &
    (alloc_info_ag_o.address_space == AddressL3) & esr_ctl_i.esr_sc_l3_bypass;

  // Message to illegal port
  wire err_msg_illegal_port = alloc_valid_ag_o & bank_valid_i & bank_ready_o & IsL2 &
    (alloc_info_ag_o.port_id == IllegalPortId);

  // Read Coop failed to map virtual mask to physical ids
  wire err_readcoop_illegal_port = alloc_valid_ag_o & bank_valid_i & bank_ready_o & IsL2 &
    (bank_info_i.opcode == etlink_pkg::ReqReadCoop) &
    (alloc_info_ag_o.readcoop_dest == NeighCoopMaskSize'(0));

  // Or all of the errors together
  assign alloc_info_ag_o.err_valid =
    err_scp_remote_not_enabled |
    err_unexpected_opcode |
    err_l2_atomic_while_l2_bypass |
    err_l3_atomic_while_l3_bypass |
    err_msg_illegal_port |
    err_readcoop_illegal_port;

  assign alloc_info_ag_o.err_type =
    (err_scp_remote_not_enabled)    ? ReqqErrRemoteScpDisabled :
    (err_unexpected_opcode)         ? ReqqErrUnsupported :
    (err_l2_atomic_while_l2_bypass) ? ReqqErrL2BypassAtomic :
    (err_l3_atomic_while_l3_bypass) ? ReqqErrL3BypassAtomic :
    (err_msg_illegal_port)          ? ReqqErrIllegalPort :
    (err_readcoop_illegal_port)     ? ReqqErrIllegalPort :
    ErrNone;

  // Do not send a response back for this decode error
  // because there is no port to send the response back to.
  assign alloc_info_ag_o.err_imprecise = err_msg_illegal_port | err_readcoop_illegal_port;


  // Alloc paired Reqq entry for writearound and atomic if Reqq available
  always_comb begin
    alloc_info_paired_ag_o = alloc_info_ag_o;

    alloc_info_paired_ag_o.opcode      = ReqqInitial2;
    alloc_info_paired_ag_o.reqq_id     = alloc_id_paired_ag;
    alloc_info_paired_ag_o.paired_id   = alloc_id_ag;
    alloc_info_paired_ag_o.qwen        = '1;
    alloc_info_paired_ag_o.zero_data   = '0;

    if (IsL2) begin
      case (alloc_info_ag_o.orig_opcode)
        OrigWriteAround:   alloc_info_paired_ag_o.orig_opcode = OrigWriteAround2;
        OrigAtomic:        alloc_info_paired_ag_o.orig_opcode = OrigAtomic2;
        OrigWritePartial:  alloc_info_paired_ag_o.orig_opcode = OrigWritePartial2;
        default:           alloc_info_paired_ag_o.orig_opcode = OrigErr;
      endcase
    end
    else begin // IsL3
      case (alloc_info_ag_o.orig_opcode)
        OrigAtomic:        alloc_info_paired_ag_o.orig_opcode = OrigAtomic2;
        OrigWritePartial:  alloc_info_paired_ag_o.orig_opcode = OrigWritePartial2;
        default:           alloc_info_paired_ag_o.orig_opcode = OrigErr;
      endcase
    end

    alloc_info_paired_ag_o.dep_valid           = 1'b0;
    alloc_info_paired_ag_o.dep_non_meshll_head = 1'b0;
    alloc_info_paired_ag_o.dep_id              = '0;

    alloc_info_paired_ag_o.rbuf_valid          = 1'b0;
    alloc_info_paired_ag_o.rbuf_pending_valid  = 1'b0;
    alloc_info_paired_ag_o.rbuf_id             = '0;

    alloc_info_paired_ag_o.traced              = trace_address_match &
                                                 trace_port_match &
                                                 trace_source_match &
                                                 trace_reqq_source_match &
                                                 (~esr_ctl_i.esr_sc_trace_filter_reqq_id_enable |
                                                   (alloc_info_paired_ag_o.reqq_id == esr_ctl_i.esr_sc_trace_filter_reqq_id)) &
                                                 ~limited_to_single_trace;
  end


  // Address matching for dependencies.
  // --------------------------------------------------------------------------
  // An incoming request that matches an in-flight request pends behind the
  // tail of that request list.
  // Address matches take into account the address_space of the request.
  logic [NumEntries-1:0] address_match_hot_ag;
  logic [NumEntries-1:0] address_match_tail_1hot_ag;

  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      // Address match
      // and linkable address_space and source
      // and entry still stalling entries behind it.
      // and not allocating a second half (Atomic2, WriteAround2, WritePartial).
      address_match_hot_ag[ii] = reqq_entry_valid_i[ii] &
        ~alloc_2nd_half_needed_ad &
        ~alloc_info_ag_o.err_valid &
        masked_address_compare(alloc_info_ag_o.address, reqq_state_i[ii].address, alloc_info_ag_o.address_space) &
        linkable(alloc_info_ag_o.address_space, reqq_state_i[ii].address_space,
                 alloc_info_ag_o.reqq_source, reqq_state_i[ii].reqq_source) &
        (~reqq_state_i[ii].dont_depend_on_me & ~clear_dep_hot_i[ii]);

      // And there is a tail.
      address_match_tail_1hot_ag[ii] = address_match_hot_ag[ii] & reqq_state_i[ii].dep_tail;
    end
  end

  // Binary index of the reqq we're dependent upon — inline one-hot-to-binary
  logic [$clog2(NumEntries)-1:0] address_match_tail_id_ag_bin;
  logic [NumEntriesId-1:0] address_match_tail_id_ag;
  always_comb begin
    address_match_tail_id_ag_bin = '0;
    for (int i = 0; i < NumEntries; i++) begin
      if (address_match_tail_1hot_ag[i]) begin
        address_match_tail_id_ag_bin = $clog2(NumEntries)'(i);
      end
    end
  end
  assign address_match_tail_id_ag = NumEntriesId'(address_match_tail_id_ag_bin);

  // Also dependent if there is a match against a reqq_id currently transitioning to a victim.
  // Victim match if not allocating scratchpad and address matches the current victim.
  assign alloc_match_tag_victim_ag_o =
    alloc_valid_ag_o & ~alloc_2nd_half_needed_ad & ~alloc_info_ag_o.err_valid &
    (alloc_info_ag_o.address_space == pipe_tag_rsp_info_i.victim_address_space) &
    pipe_tag_rsp_valid_i & pipe_tag_rsp_info_i.victim &
    masked_address_compare(alloc_info_ag_o.address, pipe_tag_rsp_info_i.victim_address, pipe_tag_rsp_info_i.victim_address_space);

  // Dependent if there was any reqq match or victim match.
  // No dependencies on second half allocs
  assign alloc_info_ag_o.dep_valid = |{address_match_hot_ag, alloc_match_tag_victim_ag_o} & ~alloc_2nd_half_needed_ad;

  // Check if there is a non-meshll match to attach to before attaching to a tag victim.
  wire address_match_non_meshll_ag = |(address_match_hot_ag & entry_is_non_meshll_i);

  // Check if there is a non-meshll match not including the entry that is transitioning in this cycle to meshll
  wire address_match_non_meshll_minus_upgrade_ag = |(address_match_hot_ag & (entry_is_non_meshll_i & ~upgrade_to_meshll_hot_i));

  // Non-meshll-head if no non-meshll ahead of you or if the meshll ahead of you is being upgraded.
  assign alloc_info_ag_o.dep_non_meshll_head = ~address_match_non_meshll_minus_upgrade_ag & ~alloc_2nd_half_needed_ad;

  // If there is a linked list of regular (non-meshll ops) append to that.
  // Else, if there is a new victim append to that.
  // Else, append to the existing victim linked list.
  assign alloc_info_ag_o.dep_id =
    address_match_non_meshll_ag ? address_match_tail_id_ag :  // there are regular matching ops
    alloc_match_tag_victim_ag_o ? pipe_tag_rsp_info_i.reqq_id : // there is a new victim and every other match is a victim too
                                  address_match_tail_id_ag;   // attach to end of list (or don't care if there is no list)

  // Address match for read buffer hit
  // --------------------------------------------------------------------------
  logic [RbufSize-1:0]                 alloc_rbuf_hit_hot_ag;
  logic [RbufPendingInstallSize-1:0]   alloc_rbuf_pending_install_hit_hot_ag;
  logic alloc_rbuf_install_hit_ag;
  logic alloc_rbuf_pending_install_hit_ag;
  logic alloc_rbuf_evict_hit_ag;

  // Compare the alloc address against the addresses that are in the rbuf.
  // Compare the alloc address against the addresses that are pending install.
  always_comb begin
    for (int ii = 0; ii < RbufSize; ii++) begin
      alloc_rbuf_hit_hot_ag[ii] =
        pipe_rbuf_state_i.rbuf_mem_valid[ii] &                             // valid in rbuf
        ~pipe_rbuf_clear_req_info_i.rbuf_clear_mem_valid_hot[ii] &         // not being cleared
        (alloc_info_ag_o.address_space inside {AddressL2, AddressScp}) &   // L2 or Scp
        (alloc_is_scp_ag == pipe_rbuf_state_i.rbuf_mem_scp[ii]) &
        masked_address_compare(alloc_info_ag_o.address, pipe_rbuf_state_i.rbuf_mem[ii], alloc_info_ag_o.address_space);
    end
    for (int ii = 0; ii < RbufPendingInstallSize; ii++) begin
      alloc_rbuf_pending_install_hit_hot_ag[ii] =
        (|(pipe_rbuf_state_i.rbuf_pending_install_pipe_valid[ii] &          // valid is one-hot for each reqq_id
           ~pipe_rbuf_clear_req_info_i.rbuf_clear_pending_install_hot)) &   // not being cleared
        (alloc_info_ag_o.address_space inside {AddressL2, AddressScp}) &    // L2 or Scp
        (alloc_is_scp_ag == pipe_rbuf_state_i.rbuf_pending_install_pipe_scp[ii]) & // same address space
        masked_address_compare(alloc_info_ag_o.address, pipe_rbuf_state_i.rbuf_pending_install_pipe_address[ii], alloc_info_ag_o.address_space);
    end
  end

  // Binary index of the rbuf we've hit — inline one-hot-to-binary
  logic [RbufIdSize-1:0] alloc_rbuf_hit_id_ag;
  always_comb begin
    alloc_rbuf_hit_id_ag = '0;
    for (int i = 0; i < RbufSize; i++) begin
      if (alloc_rbuf_hit_hot_ag[i]) begin
        alloc_rbuf_hit_id_ag = RbufIdSize'(i);
      end
    end
  end

  // Binary index of the pending install rbuf_id — inline one-hot mux + one-hot-to-binary
  logic [RbufSize-1:0] hit_rbuf_pending_install_pipe_valid;
  always_comb begin
    hit_rbuf_pending_install_pipe_valid = '0;
    for (int i = 0; i < RbufPendingInstallSize; i++) begin
      if (alloc_rbuf_pending_install_hit_hot_ag[i]) begin
        hit_rbuf_pending_install_pipe_valid = hit_rbuf_pending_install_pipe_valid |
          pipe_rbuf_state_i.rbuf_pending_install_pipe_valid[i];
      end
    end
  end
  logic [RbufIdSize-1:0] alloc_rbuf_pending_install_hit_id_ag;
  always_comb begin
    alloc_rbuf_pending_install_hit_id_ag = '0;
    for (int i = 0; i < RbufSize; i++) begin
      if (hit_rbuf_pending_install_pipe_valid[i]) begin
        alloc_rbuf_pending_install_hit_id_ag = RbufIdSize'(i);
      end
    end
  end

  // Compare against the rbuf install, pending install, evict addresses as well.
  assign alloc_rbuf_install_hit_ag =
    pipe_rbuf_state_i.rbuf_install &
    (alloc_info_ag_o.address_space inside {AddressL2, AddressScp}) &
    (alloc_is_scp_ag == pipe_rbuf_state_i.rbuf_install_scp) &
    masked_address_compare(alloc_info_ag_o.address, pipe_rbuf_state_i.rbuf_install_address, alloc_info_ag_o.address_space);

  assign alloc_rbuf_pending_install_hit_ag =
    pipe_rbuf_state_i.rbuf_pending_install &
    (alloc_info_ag_o.address_space inside {AddressL2, AddressScp}) &
    (alloc_is_scp_ag == pipe_rbuf_state_i.rbuf_pending_install_scp) &
    masked_address_compare(alloc_info_ag_o.address, pipe_rbuf_state_i.rbuf_pending_install_address, alloc_info_ag_o.address_space);
  assign alloc_rbuf_evict_hit_ag =
    pipe_rbuf_state_i.rbuf_evict &
    (alloc_info_ag_o.address_space inside {AddressL2, AddressScp}) &
    (alloc_is_scp_ag == pipe_rbuf_state_i.rbuf_evict_scp) &
    masked_address_compare(alloc_info_ag_o.address, pipe_rbuf_state_i.rbuf_evict_address, alloc_info_ag_o.address_space);

  // Alloc address is in the read buffer
  assign alloc_info_ag_o.rbuf_valid =
    (|{alloc_rbuf_hit_hot_ag, alloc_rbuf_install_hit_ag}) &
    ~alloc_rbuf_evict_hit_ag &
    ~alloc_2nd_half_needed_ad;

  // Alloc address is pending install into the read buffer
  assign alloc_info_ag_o.rbuf_pending_valid =
    (|{alloc_rbuf_pending_install_hit_hot_ag, alloc_rbuf_pending_install_hit_ag}) &
    ~alloc_rbuf_install_hit_ag &
    ~alloc_rbuf_evict_hit_ag &
    ~alloc_2nd_half_needed_ad;

  // Which read buffer entry
  assign alloc_info_ag_o.rbuf_id =
    (alloc_rbuf_install_hit_ag)         ? pipe_rbuf_state_i.rbuf_install_id :
    (|alloc_rbuf_hit_hot_ag)            ? alloc_rbuf_hit_id_ag :
    (alloc_rbuf_pending_install_hit_ag) ? pipe_rbuf_state_i.rbuf_pending_install_id :
                                          alloc_rbuf_pending_install_hit_id_ag;

  // Allocate second cycle
  // --------------------------------------------------------------------------
  wire alloc_valid_ad_en = alloc_valid_ag_o | alloc_valid_ad_o;

  // pipeline ag -> ad
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) alloc_valid_ad_o <= '0;
    else if (alloc_valid_ad_en) alloc_valid_ad_o <= alloc_valid_ag_o;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) alloc_1hot_ad_o <= '0;
    else if (alloc_valid_ad_en) alloc_1hot_ad_o <= alloc_1hot_ag_o;
  end

  always_ff @(posedge clk_gated) begin
    if (alloc_valid_ag_o) alloc_info_ad_o <= alloc_info_ag_o;
  end


  // Set this if allocating an Atomic or a WriteAround
  // Block ready if this is set.
  // Clear when the second half is allocated.
  wire alloc_2nd_half_needed_ad_en = alloc_valid_ag_o;
  wire alloc_2nd_half_needed_ad_next = alloc_valid_ag_o & ~alloc_info_ag_o.err_valid &
    ~reqq_alloc_paired_valid &
    alloc_orig_opcode_needs_paired;
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) alloc_2nd_half_needed_ad <= '0;
    else if (alloc_2nd_half_needed_ad_en) alloc_2nd_half_needed_ad <= alloc_2nd_half_needed_ad_next;
  end

  // --------------------------------------------------------------------------
  // Compare the cache line address bits.  Mask off sub cache line bits.
  function automatic bit masked_address_compare;
    input logic [PaSize-1:0] address_a;
    input logic [PaSize-1:0] address_b;
    input address_space_e address_space;
    begin
      masked_address_compare = '0;
      if (address_space == AddressL2) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressL3) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressScp) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressMsg) begin
        masked_address_compare |= (address_a[MsgNeighIdLsb +: MsgNeighIdSize] == address_b[MsgNeighIdLsb +: MsgNeighIdSize]);
      end
    end
  endfunction

  function automatic bit linkable;
    input address_space_e address_space_a;
    input address_space_e address_space_b;
    input reqq_source_e source_a;
    input reqq_source_e source_b;
    begin
      linkable = '0;
      // Linkable if same source and address space.
      if ((address_space_a == address_space_b) &&
          (source_a == source_b)) begin
        linkable = '1;
      end
      // Also Cop FSM cbuf evict links to L2.
      if ((address_space_a == AddressL2) &&
          (address_space_b == AddressL2) &&
          ((source_a == ReqqSrcFsm) | (source_b == ReqqSrcFsm))) begin
        linkable = '1;
      end
    end
  endfunction


  wire unused_ok = &{
    1'b0,
    bank_info_i.wdata,
    bank_info_i.ben,
    bank_info_i.subopcode,
    pipe_rbuf_state_i,
    pipe_tag_rsp_info_i,
    pipe_rbuf_clear_req_info_i,
    atomic_conf.dest,
    atomic_conf.opcode,
    esr_shire_config_i,
    pipe_idx_cop_req_valid_i,
    pipe_idx_cop_req_info_i,
    esr_ctl_i,
    take_idx,
    reqq_state_i
  };

endmodule
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
