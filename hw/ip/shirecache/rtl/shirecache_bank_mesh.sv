// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache bank mesh interface — per-bank mesh request/response handling.
//
// Arbitrates outbound requests from reqq (no-data), dataq (with-data), and
// pipeline victim data.  Maps internal opcodes to ET-Link mesh opcodes,
// computes addresses (normal, ScpFill, atomic response), and pushes through
// a request FIFO to the mesh master.
//
// Inbound responses are received into a response FIFO and fanned out to up
// to 4 destinations (reqq, dataq, rspmux_l2, l3_slave) with independent
// valid/ready handshaking and per-destination "sent" tracking.
//
// Replaces: shire_cache_bank_mesh (etcore-soc)

module shirecache_bank_mesh
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter bit          IsToL3       = 1,    // 1 = to_l3 mesh, 0 = to_sys mesh
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  dft_pkg::dft_t                     dft_i,
  input  logic                              mesh_master_bank_clk_en_i,

  // Mesh interface (to mesh master)
  output logic                              mesh_master_bank_req_valid_o,
  output mesh_master_req_t                  mesh_master_bank_req_info_o,
  input  logic                              mesh_master_bank_req_ready_i,

  input  logic                              mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t                  mesh_master_bank_rsp_info_i,
  output logic                              mesh_master_bank_rsp_ready_o,

  // Constants
  input  logic [BankIdSize-1:0]             my_bank_id_i,

  // Mesh requests from reqq (no-data) and dataq (with-data)
  input  logic                              reqq_mesh_req_valid_nodata_i,
  input  reqq_mesh_req_t                    reqq_mesh_req_info_i,
  output logic                              reqq_mesh_req_ready_nodata_o,

  input  logic                              dataq_mesh_req_valid_i,
  input  reqq_mesh_req_t                    dataq_mesh_req_info_i,
  output logic                              dataq_mesh_req_ready_o,

  // Victim from datapipe
  input  logic                              pipe_data_rsp_valid_i,
  input  data_rsp_t                         pipe_data_rsp_info_i,
  input  data_rsp_modifier_t                pipe_data_rsp_modifier_info_i,
  output logic                              mesh_bypassed_victim_o,

  // Mesh responses to reqq, dataq, rspmux, l3_slave
  output reqq_mesh_rsp_t                    mesh_rsp_info_o,
  output logic                              mesh_rsp_valid_reqq_o,
  input  logic                              mesh_rsp_ready_reqq_i,
  output logic                              mesh_rsp_valid_dataq_o,
  input  logic                              mesh_rsp_ready_dataq_i,
  output logic                              mesh_rsp_valid_rspmux_l2_o,
  input  logic                              mesh_rsp_ready_rspmux_l2_i,
  output logic                              mesh_rsp_valid_l3_slave_o,
  input  logic                              mesh_rsp_ready_l3_slave_i,

  // Reqq state (for opcode/address/qwen lookups)
  input  reqq_entry_state_t [NumEntries-1:0] reqq_state_i,

  // ESR control
  input  bank_esr_ctl_t                     esr_ctl_i
);

  /* verilator lint_off UNUSEDSIGNAL */  // packed structs carry fields unused by this module

  localparam bit IsToSys = (IsToL3 == 1'b0);

  logic mesh_rsp_fifo_valid;

  // ══════════════════════════════════════════════════════════
  // Clock gating
  // DFT fix: original hard-codes et_clk_gate .te(1'b0), so the gated clock
  // is NOT forced on during scan mode — a scan coverage gap.  We pass the
  // full dft_i struct so that scanmode=1 forces the clock on via
  // prim_clk_gate, ensuring all flops behind the gate are scan-testable.
  // ══════════════════════════════════════════════════════════

  logic clk_gated;
  logic clock_en;
  assign clock_en = ~rst_ni
                  | mesh_master_bank_clk_en_i
                  | mesh_rsp_fifo_valid;

  prim_clk_gate u_clk_gate (
    .clk_i (clk_i),
    .en_i  (clock_en),
    .dft_i (dft_i),   // DFT: scanmode forces clock on (original: te=0, no override)
    .clk_o (clk_gated)
  );

  // ══════════════════════════════════════════════════════════
  // Requests
  // ══════════════════════════════════════════════════════════

  logic mesh_req_ff_valid;
  logic mesh_req_fifo_full;

  // Convert pipeline victim into a mesh req type for arbitration
  reqq_mesh_req_t pipe_data_rsp_info_req;
  assign pipe_data_rsp_info_req.trans_id  = pipe_data_rsp_info_i.trans_id;
  assign pipe_data_rsp_info_req.reqq_id   = pipe_data_rsp_info_i.reqq_id;
  assign pipe_data_rsp_info_req.dataq_id  = pipe_data_rsp_modifier_info_i.dataq_id;
  assign pipe_data_rsp_info_req.data      = pipe_data_rsp_info_i.data;
  assign pipe_data_rsp_info_req.with_data = 1'b1;
  assign pipe_data_rsp_info_req.qwen      = '0;
  assign pipe_data_rsp_info_req.rd_ben    = '0;
  assign pipe_data_rsp_info_req.ben       = '0;

  // Pipe response bypass to mesh only if victim and headed to this mesh master
  // Note: IsToL3/IsToSys params shadow the mesh_dest_e enum values, so use literals
  logic pipe_data_rsp_to_this_mesh;
  assign pipe_data_rsp_to_this_mesh =
    (IsToL3  && (pipe_data_rsp_modifier_info_i.which_mesh == mesh_dest_e'(1'b0))) |
    (IsToSys && (pipe_data_rsp_modifier_info_i.which_mesh == mesh_dest_e'(1'b1)));

  // Normal L3/Scp atomic responses are sent to to_sys mesh as ESR writes
  logic pipe_data_rsp_victim_or_rsp_to_mesh;
  assign pipe_data_rsp_victim_or_rsp_to_mesh =
    (pipe_data_rsp_info_i.rsp_type == VictimData) |
    pipe_data_rsp_modifier_info_i.redirect_normal_rsp_to_mesh;

  logic pipe_data_rsp_valid_victim;
  assign pipe_data_rsp_valid_victim = pipe_data_rsp_valid_i
    & pipe_data_rsp_to_this_mesh
    & ~pipe_data_rsp_modifier_info_i.disable_mesh_bypass
    & pipe_data_rsp_victim_or_rsp_to_mesh;

  // Priority arbiter: reqq > dataq > pipeline victim
  logic pipe_data_rsp_ready;
  reqq_mesh_req_t mesh_req_arb_info;

  prim_arb_prio #(
    .NumClients(3),
    .Width($bits(reqq_mesh_req_t))
  ) u_req_arb (
    .clk_i  (clk_gated),
    .rst_ni (rst_ni),
    .bid_i  ({reqq_mesh_req_valid_nodata_i,
              dataq_mesh_req_valid_i,
              pipe_data_rsp_valid_victim}),
    .grant_o({reqq_mesh_req_ready_nodata_o,
              dataq_mesh_req_ready_o,
              pipe_data_rsp_ready}),
    .stall_i(mesh_req_ff_valid & mesh_req_fifo_full),
    .data_i ({reqq_mesh_req_info_i,
              dataq_mesh_req_info_i,
              pipe_data_rsp_info_req}),
    .data_o (mesh_req_arb_info)
  );
  assign mesh_bypassed_victim_o = pipe_data_rsp_ready;

  logic mesh_req_arb_valid;
  assign mesh_req_arb_valid = |{reqq_mesh_req_valid_nodata_i,
                                dataq_mesh_req_valid_i,
                                pipe_data_rsp_valid_victim};

  // Pipeline stage: RST_EN_FF for valid, EN_FF for info
  logic mesh_req_ff_valid_d;
  logic mesh_req_ff_en;
  assign mesh_req_ff_valid_d = mesh_req_arb_valid | (mesh_req_ff_valid & mesh_req_fifo_full);
  assign mesh_req_ff_en      = mesh_req_arb_valid & ~(mesh_req_ff_valid & mesh_req_fifo_full);

  reqq_mesh_req_t mesh_req_ff_info_q;

  // Valid register with clock enable = valid | valid_d
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) begin
      mesh_req_ff_valid <= 1'b0;
    end else if (mesh_req_ff_valid | mesh_req_ff_valid_d) begin
      mesh_req_ff_valid <= mesh_req_ff_valid_d;
    end
  end

  // Data register with clock enable = mesh_req_ff_en
  always_ff @(posedge clk_gated) begin
    if (mesh_req_ff_en) begin
      mesh_req_ff_info_q <= mesh_req_arb_info;
    end
  end

  // ──────────────────────────────────────────────────────────
  // Map to mesh request structure
  // ──────────────────────────────────────────────────────────

  mesh_master_req_t   selected_mesh_req_info;
  logic               selected_mesh_req_valid;

  // Opcode mapping: internal reqq opcode → ET-Link mesh opcode
  etlink_pkg::req_opcode_e selected_mesh_req_opcode;
  always_comb begin
    unique case (reqq_state_i[mesh_req_ff_info_q.reqq_id].opcode)
      MeshRead:       selected_mesh_req_opcode = etlink_pkg::ReqRead;
      MeshWrite:      selected_mesh_req_opcode = etlink_pkg::ReqWrite;
      MeshFlush:      selected_mesh_req_opcode = etlink_pkg::ReqFlush;
      MeshFlushToMem: selected_mesh_req_opcode = etlink_pkg::ReqFlushToMem;
      MeshEvict:      selected_mesh_req_opcode = etlink_pkg::ReqEvict;
      MeshEvictToMem: selected_mesh_req_opcode = etlink_pkg::ReqEvictToMem;
      MeshPrefetch:   selected_mesh_req_opcode = etlink_pkg::ReqPrefetch;
      MeshVictim:     selected_mesh_req_opcode = etlink_pkg::ReqWrite;
      MeshAtomicRsp:  selected_mesh_req_opcode = etlink_pkg::ReqAtomicRsp;
      default:        selected_mesh_req_opcode = etlink_pkg::req_opcode_e'('x);
    endcase
  end

  // ──────────────────────────────────────────────────────────
  // Address computation
  // ──────────────────────────────────────────────────────────

  // ScpFill address: stored in data field
  logic [PaSize-1:0] scp_fill_address;
  assign scp_fill_address =
    {mesh_req_ff_info_q.data[LineSelAddrMsb:LineSelAddrLsb],
     {LineOffsetIdSize{1'b0}}};

  // L3/Scp atomic response address: ESR write to originating shire
  logic use_atomic_rsp_address;
  assign use_atomic_rsp_address = IsToSys &
    (reqq_state_i[mesh_req_ff_info_q.reqq_id].opcode == MeshAtomicRsp);

  logic [PaSize-1:0] atomic_rsp_address;
  assign atomic_rsp_address =
    (PaSize'(mesh_req_ff_info_q.data[L3AtomicShireIdLsb +: L3AtomicShireIdSize])
       << L3AtomicRspShireIdShift) |
    PaSize'(mesh_req_ff_info_q.data[L3AtomicAddrLowerLsb +: L3AtomicAddrLowerSize]) |
    L3AtomicRspAddrOffset;

  // Normal reqq address: cache-line aligned
  logic [PaSize-1:0] reqq_address;
  assign reqq_address =
    {reqq_state_i[mesh_req_ff_info_q.reqq_id].address[LineSelAddrMsb:LineSelAddrLsb],
     {LineOffsetIdSize{1'b0}}};

  // Select address source
  logic req_copscpfill;
  logic req_with_data;
  logic [PaSize-1:0] req_address;

  assign req_copscpfill = (reqq_state_i[mesh_req_ff_info_q.reqq_id].orig_opcode == OrigCopScpFill);
  assign req_with_data  = ~req_copscpfill & mesh_req_ff_info_q.with_data;
  assign req_address =
    req_copscpfill         ? scp_fill_address    :
    use_atomic_rsp_address ? atomic_rsp_address  :
                             reqq_address;

  // Size: atomic rsp uses reqq entry size, others use line size
  sc_size_t req_size;
  assign req_size =
    use_atomic_rsp_address ? reqq_state_i[mesh_req_ff_info_q.reqq_id].size
                           : etlink_pkg::SizeLine;

  // QOS priority
  logic [AxiAxqosSize-1:0] req_qos;
  assign req_qos =
    use_atomic_rsp_address                            ? AxiQosAtomicRsp :
    reqq_state_i[mesh_req_ff_info_q.reqq_id].qos[0]  ? AxiQosMemHighPriority :
                                                        AxiQosMemLowPriority;

  // Quadword write enables — turn off if no data
  // Atomic response enables qw[2:0] for the atomic response ID
  logic [LineQwSize-1:0] req_qwen;
  assign req_qwen =
    use_atomic_rsp_address
      ? (LineQwSize'(3'b111) & {LineQwSize{req_with_data}})
      : (reqq_state_i[mesh_req_ff_info_q.dataq_id].qwen & {LineQwSize{req_with_data}});

  // Assemble mesh request
  assign selected_mesh_req_info.trans_id = mesh_req_ff_info_q.trans_id;
  assign selected_mesh_req_info.id       = mesh_req_ff_info_q.reqq_id;
  assign selected_mesh_req_info.port_id  = '0;
  assign selected_mesh_req_info.source   = my_bank_id_i;
  assign selected_mesh_req_info.qos      = req_qos;
  assign selected_mesh_req_info.opcode   = selected_mesh_req_opcode;
  assign selected_mesh_req_info.address  = req_address;
  assign selected_mesh_req_info.size     = req_size;
  assign selected_mesh_req_info.wdata    = req_with_data;
  assign selected_mesh_req_info.qwen     = req_qwen;
  assign selected_mesh_req_info.data     = mesh_req_ff_info_q.data;
  assign selected_mesh_req_info.ben_en   = mesh_req_ff_info_q.rd_ben;
  assign selected_mesh_req_info.ben      = mesh_req_ff_info_q.ben;
  assign selected_mesh_req_valid         = mesh_req_ff_valid;

  // ──────────────────────────────────────────────────────────
  // Request FIFO (depth 2)
  // ──────────────────────────────────────────────────────────

  logic [2:0] mesh_req_fifo_level_unused;
  prim_fifo_reg #(
    .Width($bits(mesh_master_req_t)),
    .Depth(2)
  ) u_mesh_req_fifo (
    .clk_i  (clk_gated),
    .rst_ni (rst_ni),
    .push_i (selected_mesh_req_valid),
    .wdata_i(selected_mesh_req_info),
    .full_o (mesh_req_fifo_full),
    .level_o(mesh_req_fifo_level_unused),
    .pop_i  (mesh_master_bank_req_ready_i),
    .valid_o(mesh_master_bank_req_valid_o),
    .rdata_o(mesh_master_bank_req_info_o)
  );

  // ══════════════════════════════════════════════════════════
  // Responses
  // ══════════════════════════════════════════════════════════

  // Response FIFO (depth 2)
  mesh_master_rsp_t  mesh_rsp_fifo_info_pre;
  logic              mesh_rsp_fifo_full;
  logic              mesh_rsp_fifo_pop;

  logic [2:0] mesh_rsp_fifo_level_unused;
  prim_fifo_reg #(
    .Width($bits(mesh_master_rsp_t)),
    .Depth(2)
  ) u_mesh_rsp_fifo (
    .clk_i  (clk_gated),
    .rst_ni (rst_ni),
    .push_i (mesh_master_bank_rsp_valid_i),
    .wdata_i(mesh_master_bank_rsp_info_i),
    .full_o (mesh_rsp_fifo_full),
    .level_o(mesh_rsp_fifo_level_unused),
    .pop_i  (mesh_rsp_fifo_pop),
    .valid_o(mesh_rsp_fifo_valid),
    .rdata_o(mesh_rsp_fifo_info_pre)
  );
  assign mesh_master_bank_rsp_ready_o = ~mesh_rsp_fifo_full;

  // Map external response to internal struct
  reqq_mesh_rsp_t mesh_rsp_fifo_info;
  logic [ReqqIdSize-1:0] rsp_reqq_id;
  assign rsp_reqq_id                  = mesh_rsp_fifo_info_pre.id;
  assign mesh_rsp_fifo_info.trans_id  = reqq_state_i[rsp_reqq_id].trans_id;
  assign mesh_rsp_fifo_info.reqq_id   = rsp_reqq_id;
  assign mesh_rsp_fifo_info.dataq_id  = reqq_state_i[rsp_reqq_id].dataq_id;
  assign mesh_rsp_fifo_info.data      = mesh_rsp_fifo_info_pre.data;
  assign mesh_rsp_fifo_info.zero_data = ~(|mesh_rsp_fifo_info_pre.data);
  assign mesh_rsp_fifo_info.err       = mesh_rsp_fifo_info_pre.err;

  assign mesh_rsp_info_o = mesh_rsp_fifo_info;

  // ──────────────────────────────────────────────────────────
  // Response fan-out: 4 destinations with independent valid/ready
  // Pop FIFO only after all required destinations have accepted.
  // ──────────────────────────────────────────────────────────

  // reqq — always required
  logic rsp_required_reqq;
  assign rsp_required_reqq = 1'b1;

  logic rsp_sent_reqq_q;
  logic rsp_sent_reqq_d;
  logic rsp_sent_reqq_en;
  logic rsp_sending_reqq;
  assign rsp_sending_reqq = mesh_rsp_valid_reqq_o & mesh_rsp_ready_reqq_i;

  always_comb begin
    rsp_sent_reqq_d  = 1'b0;
    rsp_sent_reqq_en = 1'b0;
    if (mesh_rsp_fifo_pop) begin
      rsp_sent_reqq_d  = 1'b0;
      rsp_sent_reqq_en = 1'b1;
    end else if (rsp_sending_reqq) begin
      rsp_sent_reqq_d  = 1'b1;
      rsp_sent_reqq_en = 1'b1;
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_sent_reqq_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sent_reqq_en)
      rsp_sent_reqq_q <= rsp_sent_reqq_d;
  end

  assign mesh_rsp_valid_reqq_o = mesh_rsp_fifo_valid & rsp_required_reqq & ~rsp_sent_reqq_q;

  // Error delay: delay rspmux/l3_slave until reqq has been notified
  logic found_err_delay_until_reqq_update;
  assign found_err_delay_until_reqq_update = mesh_rsp_fifo_info_pre.err & ~rsp_sent_reqq_q;

  // ──────────────────────────────────────────────────────────
  // dataq — required for reads that fill back into cache (not ScpRead/ReadForward, not bypass)
  // ──────────────────────────────────────────────────────────

  logic rsp_skip_dataq_l3_bypass;
  logic rsp_skip_dataq_l2_bypass;
  logic rsp_skip_dataq_l2_bypass_l3_not_enabled;
  logic rsp_skip_dataq;

  assign rsp_skip_dataq_l3_bypass =
    IsToSys & esr_ctl_i.esr_sc_l3_bypass &
    (reqq_state_i[rsp_reqq_id].address_space == AddressL3);
  assign rsp_skip_dataq_l2_bypass =
    IsToL3  & esr_ctl_i.esr_sc_l2_bypass &
    (reqq_state_i[rsp_reqq_id].address_space == AddressL2);
  assign rsp_skip_dataq_l2_bypass_l3_not_enabled =
    IsToSys & esr_ctl_i.esr_sc_l2_bypass & ~esr_ctl_i.esr_sc_remote_l3_enable &
    (reqq_state_i[rsp_reqq_id].address_space == AddressL2);
  assign rsp_skip_dataq =
    rsp_skip_dataq_l3_bypass |
    rsp_skip_dataq_l2_bypass |
    rsp_skip_dataq_l2_bypass_l3_not_enabled;

  logic rsp_required_dataq;
  assign rsp_required_dataq =
    (reqq_state_i[rsp_reqq_id].opcode == MeshRead) &
    ~(reqq_state_i[rsp_reqq_id].orig_opcode inside
        {OrigScpRead, OrigScpReadCoop, OrigReadForward}) &
    ~rsp_skip_dataq;

  logic rsp_required_dataq_r_q;
  logic rsp_sent_dataq_q;
  logic rsp_sent_dataq_d;
  logic rsp_sent_dataq_en;
  logic rsp_sending_dataq;
  assign rsp_sending_dataq = mesh_rsp_valid_dataq_o & mesh_rsp_ready_dataq_i;

  always_comb begin
    rsp_sent_dataq_d  = 1'b0;
    rsp_sent_dataq_en = 1'b0;
    if (mesh_rsp_fifo_pop) begin
      rsp_sent_dataq_d  = 1'b0;
      rsp_sent_dataq_en = 1'b1;
    end else if (rsp_sending_dataq) begin
      rsp_sent_dataq_d  = 1'b1;
      rsp_sent_dataq_en = 1'b1;
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_sent_dataq_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sent_dataq_en)
      rsp_sent_dataq_q <= rsp_sent_dataq_d;
  end

  // Register rsp_required_dataq when reqq accepts (for stable routing after reqq sent)
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_required_dataq_r_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sending_reqq)
      rsp_required_dataq_r_q <= rsp_required_dataq;
  end

  logic rsp_required_dataq_sel;
  assign rsp_required_dataq_sel = rsp_sent_reqq_q ? rsp_required_dataq_r_q : rsp_required_dataq;
  assign mesh_rsp_valid_dataq_o = mesh_rsp_fifo_valid & ~rsp_sent_dataq_q & rsp_required_dataq_sel;

  // ──────────────────────────────────────────────────────────
  // rspmux_l2 — reads and ScpReads where source == L2
  // ──────────────────────────────────────────────────────────

  logic send_rsp_to_rspmux_l2_or_l3_slave;
  assign send_rsp_to_rspmux_l2_or_l3_slave =
    (reqq_state_i[rsp_reqq_id].orig_opcode inside
       {OrigScpRead, OrigScpReadCoop, OrigReadForward}) |
    ((reqq_state_i[rsp_reqq_id].orig_opcode inside {OrigRead, OrigReadCoop}) &
     (reqq_state_i[rsp_reqq_id].opcode == MeshRead));

  logic rsp_required_rspmux_l2;
  assign rsp_required_rspmux_l2 =
    send_rsp_to_rspmux_l2_or_l3_slave &
    (reqq_state_i[rsp_reqq_id].reqq_source == ReqqSrcL2);

  logic rsp_required_rspmux_l2_r_q;
  logic rsp_sent_rspmux_l2_q;
  logic rsp_sent_rspmux_l2_d;
  logic rsp_sent_rspmux_l2_en;
  logic rsp_sending_rspmux_l2;
  assign rsp_sending_rspmux_l2 = mesh_rsp_valid_rspmux_l2_o & mesh_rsp_ready_rspmux_l2_i;

  always_comb begin
    rsp_sent_rspmux_l2_d  = 1'b0;
    rsp_sent_rspmux_l2_en = 1'b0;
    if (mesh_rsp_fifo_pop) begin
      rsp_sent_rspmux_l2_d  = 1'b0;
      rsp_sent_rspmux_l2_en = 1'b1;
    end else if (rsp_sending_rspmux_l2) begin
      rsp_sent_rspmux_l2_d  = 1'b1;
      rsp_sent_rspmux_l2_en = 1'b1;
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_sent_rspmux_l2_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sent_rspmux_l2_en)
      rsp_sent_rspmux_l2_q <= rsp_sent_rspmux_l2_d;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_required_rspmux_l2_r_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sending_reqq)
      rsp_required_rspmux_l2_r_q <= rsp_required_rspmux_l2;
  end

  logic rsp_required_rspmux_l2_sel;
  assign rsp_required_rspmux_l2_sel = rsp_sent_reqq_q ? rsp_required_rspmux_l2_r_q : rsp_required_rspmux_l2;
  assign mesh_rsp_valid_rspmux_l2_o =
    mesh_rsp_fifo_valid &
    ~rsp_sent_rspmux_l2_q &
    ~found_err_delay_until_reqq_update &
    rsp_required_rspmux_l2_sel;

  // ──────────────────────────────────────────────────────────
  // l3_slave — reads and ScpReads where source == L3
  // ──────────────────────────────────────────────────────────

  logic rsp_required_l3_slave;
  assign rsp_required_l3_slave =
    send_rsp_to_rspmux_l2_or_l3_slave &
    (reqq_state_i[rsp_reqq_id].reqq_source == ReqqSrcL3);

  logic rsp_required_l3_slave_r_q;
  logic rsp_sent_l3_slave_q;
  logic rsp_sent_l3_slave_d;
  logic rsp_sent_l3_slave_en;
  logic rsp_sending_l3_slave;
  assign rsp_sending_l3_slave = mesh_rsp_valid_l3_slave_o & mesh_rsp_ready_l3_slave_i;

  always_comb begin
    rsp_sent_l3_slave_d  = 1'b0;
    rsp_sent_l3_slave_en = 1'b0;
    if (mesh_rsp_fifo_pop) begin
      rsp_sent_l3_slave_d  = 1'b0;
      rsp_sent_l3_slave_en = 1'b1;
    end else if (rsp_sending_l3_slave) begin
      rsp_sent_l3_slave_d  = 1'b1;
      rsp_sent_l3_slave_en = 1'b1;
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_sent_l3_slave_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sent_l3_slave_en)
      rsp_sent_l3_slave_q <= rsp_sent_l3_slave_d;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)
      rsp_required_l3_slave_r_q <= 1'b0;
    else if (mesh_rsp_fifo_valid & rsp_sending_reqq)
      rsp_required_l3_slave_r_q <= rsp_required_l3_slave;
  end

  logic rsp_required_l3_slave_sel;
  assign rsp_required_l3_slave_sel = rsp_sent_reqq_q ? rsp_required_l3_slave_r_q : rsp_required_l3_slave;
  assign mesh_rsp_valid_l3_slave_o =
    mesh_rsp_fifo_valid &
    ~rsp_sent_l3_slave_q &
    ~found_err_delay_until_reqq_update &
    rsp_required_l3_slave_sel;

  // ──────────────────────────────────────────────────────────
  // Pop FIFO when all required destinations have accepted
  // ──────────────────────────────────────────────────────────

  logic rsp_sent_all;
  assign rsp_sent_all =
    mesh_rsp_fifo_valid &
    (rsp_sent_reqq_q      | rsp_sending_reqq      | ~rsp_required_reqq) &
    (rsp_sent_dataq_q     | rsp_sending_dataq     | ~rsp_required_dataq_sel) &
    (rsp_sent_rspmux_l2_q | rsp_sending_rspmux_l2 | ~rsp_required_rspmux_l2_sel) &
    (rsp_sent_l3_slave_q  | rsp_sending_l3_slave  | ~rsp_required_l3_slave_sel);

  assign mesh_rsp_fifo_pop = rsp_sent_all;

  // ══════════════════════════════════════════════════════════
  // Unused signal aggregation (Verilator lint)
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    mesh_req_fifo_level_unused,
    mesh_rsp_fifo_level_unused,
    pipe_data_rsp_info_i.tag_sbe,
    pipe_data_rsp_info_i.qwen,
    pipe_data_rsp_info_i.data_index,
    pipe_data_rsp_info_i.data_ram_sbe,
    pipe_data_rsp_info_i.data_ram_dbe,
    pipe_data_rsp_modifier_info_i.trans_id,
    pipe_data_rsp_modifier_info_i.reqq_id,
    pipe_data_rsp_modifier_info_i.address_space,
    pipe_data_rsp_modifier_info_i.reqq_source,
    pipe_data_rsp_modifier_info_i.port_id,
    pipe_data_rsp_modifier_info_i.rsp_type,
    mesh_req_ff_info_q.dataq_id,
    mesh_req_ff_info_q.qwen,
    mesh_rsp_fifo_info_pre.source,
    mesh_rsp_fifo_info_pre.port_id,
    mesh_rsp_fifo_info_pre.wdata,
    esr_ctl_i,
    reqq_state_i,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
