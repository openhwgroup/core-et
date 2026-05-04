// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache bank data queue (dataq).
//
// Holds the cacheline storage associated with each reqq entry.
// Features:
//   - 4 write ports (bank_l2, to_l3_mesh, bank_l3, to_sys_mesh) + pipeline write
//   - 4 read ports (to_l3_mesh, rspmux_l2, to_sys_mesh, rspmux_l3) + pipeline read
//   - Pipeline writes/reads are highest priority (never blocked)
//   - Write side: register pipeline write, FIFO others, RR arb → prio arb with pipe
//   - Read side: same structure; read data returned 3 cycles after request
//   - Per-QW register files (4x 128-bit) + BEN register file
//   - Read sink FIFOs: ctl, data, ben per read port
//
// Replaces: shire_cache_bank_dataq (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions
//   - prim_fifo_reg, prim_arb_rr, prim_arb_prio, prim_rf_1r1w primitives
//   - Package types instead of `define types
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF/FF macros
//   - DFT consolidated into dft_t

/* verilator lint_off UNUSEDSIGNAL */  // packed struct ports (data_rsp_t, reqq_bank_req_t) passed whole; dataq uses subfields only
module shirecache_bank_dataq
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  //// Writes ////

  // Pipeline write
  input  logic                        pipe_data_rsp_valid_i,      // always ready always taken
  input  data_rsp_t                   pipe_data_rsp_info_i,
  input  data_rsp_modifier_t          pipe_data_rsp_modifier_info_i,   // Reqq id fixup for atomic hit write partial
  input  logic                        mesh_bypassed_victim_i,     // The victim is already sent to mesh
  input  logic                        rspmux_bypassed_data_rsp_i, // The response is already sent to rspmux

  // Bank L2 write requests
  output logic                        dataq_bank_l2_ready_o,
  input  logic                        bank_l2_valid_i,
  input  logic                        bank_l2_ready_i,
  input  reqq_bank_req_t              bank_l2_info_i,
  input  logic [ReqqIdSize-1:0]       reqq_alloc_l2_id_ag_i, // which entry to allocate

  // Mesh to_l3 write request
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t              to_l3_mesh_rsp_info_i,
  output logic                        to_l3_mesh_rsp_ready_o,

  // Bank L3 write requests
  output logic                        dataq_bank_l3_ready_o,
  input  logic                        bank_l3_valid_i,
  input  logic                        bank_l3_ready_i,
  input  reqq_bank_req_t              bank_l3_info_i,
  input  logic [ReqqIdSize-1:0]       reqq_alloc_l3_id_ag_i, // which entry to allocate

  // Mesh to_sys write request
  input  logic                        to_sys_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t              to_sys_mesh_rsp_info_i,
  output logic                        to_sys_mesh_rsp_ready_o,

  //// Reads ////

  // Pipeline read request
  input  logic                        pipe_dataq_rd_req_valid_i,  // always ready always taken
  input  dataq_rd_req_t               pipe_dataq_rd_req_info_i,   // and the response
  output dataq_rd_rsp_t               pipe_dataq_rd_rsp_info_o,   // rsp 3 cycles after rd req

  // Mesh to_l3 read request that needs to go through dataq to read accompanying data
  input  logic                        reqq_to_l3_mesh_req_valid_wdata_i,
  input  reqq_mesh_req_t              reqq_to_l3_mesh_req_info_i,
  output logic                        reqq_to_l3_mesh_req_ready_wdata_o,
     // and the response
  output logic                        dataq_to_l3_mesh_req_valid_o,
  output reqq_mesh_req_t              dataq_to_l3_mesh_req_info_o,
  input  logic                        dataq_to_l3_mesh_req_ready_i,

  // Neigh rspmux rsp read request that needs to go through data to read rsp data
  input  logic                        reqq_rspmux_l2_valid_wdata_i,
  input  rspmux_t                     reqq_rspmux_l2_info_i,
  output logic                        reqq_rspmux_l2_ready_wdata_o,
     // and the response
  output logic                        dataq_rspmux_l2_valid_o,
  output rspmux_t                     dataq_rspmux_l2_info_o,
  input  logic                        dataq_rspmux_l2_ready_i,

  // Mesh to_sys read request that needs to go through dataq to read accompanying data
  input  logic                        reqq_to_sys_mesh_req_valid_wdata_i,
  input  reqq_mesh_req_t              reqq_to_sys_mesh_req_info_i,
  output logic                        reqq_to_sys_mesh_req_ready_wdata_o,
     // and the response
  output logic                        dataq_to_sys_mesh_req_valid_o,
  output reqq_mesh_req_t              dataq_to_sys_mesh_req_info_o,
  input  logic                        dataq_to_sys_mesh_req_ready_i,

  // L3 slave rspmux rsp read request that needs to go through data to read rsp data
  input  logic                        reqq_rspmux_l3_valid_wdata_i,
  input  rspmux_t                     reqq_rspmux_l3_info_i,
  output logic                        reqq_rspmux_l3_ready_wdata_o,
     // and the response
  output logic                        dataq_rspmux_l3_valid_o,
  output rspmux_t                     dataq_rspmux_l3_info_o,
  input  logic                        dataq_rspmux_l3_ready_i,

  // Write status to reqq
  // Necessary to tell reqq that future read data is now available
  output logic                        dataq_write_status_any_o,
  output logic [NumEntries-1:0]       dataq_write_status_hot_o,
  output logic [ReqqIdSize-1:0]       dataq_write_status_id_o,
  output logic [NumEntries-1:0]       dataq_read_status_hot_o,

  // ESRs
  input  bank_esr_ctl_t               esr_ctl_i,

  input  dft_t                        dft_i
);

  localparam int unsigned NUM_WR = 4;  // Num wr ports not including the non-blocking pipe
  localparam int unsigned NUM_RD = 4;  // Num rd ports not including the non-blocking pipe
  localparam int unsigned FIFO_DEPTH = 2;
  localparam int unsigned NUM_ENTRIES_LOG2 = $clog2(NumEntries); // used for ram address widths


  logic rst_n;
  prim_rst_sync u_rst_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );



  // **************************************************************************
  // Fifo / register write requests
  // **************************************************************************

  // Register the pipeline write
  logic         pipe_wr_valid_d0;
  dataq_wr_t    pipe_wr_info_d0;

  // Fifo the other writes
  logic         [NUM_WR-1:0] other_wr_valid_d0;
  logic         [NUM_WR-1:0] other_wr_fifo_full_d0;
  dataq_wr_t    [NUM_WR-1:0] other_wr_info_d0;

  // Pipeline
  // Don't perform the write if the response data got bypassed to the rspmux
  // or the victim got bypassed to the mesh.
  assign pipe_wr_valid_d0         = (pipe_data_rsp_valid_i & ~rspmux_bypassed_data_rsp_i & ~mesh_bypassed_victim_i);
  assign pipe_wr_info_d0.reqq_id  = pipe_data_rsp_info_i.reqq_id;
  assign pipe_wr_info_d0.dataq_id = pipe_data_rsp_modifier_info_i.dataq_id;
  assign pipe_wr_info_d0.trans_id = pipe_data_rsp_info_i.trans_id;
  assign pipe_wr_info_d0.qwen     = pipe_data_rsp_info_i.qwen | {4{pipe_data_rsp_info_i.qwen == '0}};  // If all qwen's are zero, then it's a clean line and write it all.
  assign pipe_wr_info_d0.wr_ben   = '0;
  assign pipe_wr_info_d0.data     = pipe_data_rsp_info_i.data;
  assign pipe_wr_info_d0.ben      = '0;

  logic         pipe_wr_valid_d1;
  dataq_wr_t    pipe_wr_info_d1;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                        pipe_wr_valid_d1 <= '0;
    else if (pipe_wr_valid_d0 | pipe_wr_valid_d1)      pipe_wr_valid_d1 <= pipe_wr_valid_d0;
  end
  always_ff @(posedge clk_i) begin
    if (pipe_wr_valid_d0)                               pipe_wr_info_d1 <= pipe_wr_info_d0;
  end


  // Bank L2 write
  logic bank_l2_wr_ben;
  assign bank_l2_wr_ben = (bank_l2_info_i.opcode inside {etlink_pkg::ReqWrite}) & bank_l2_info_i.write_is_partial;
  logic bank_l2_wr_all_qwen;
  assign bank_l2_wr_all_qwen = bank_l2_wr_ben & (bank_l2_info_i.size == etlink_pkg::SizeLine);
  logic en_l2_wr_mask;
  assign en_l2_wr_mask = (bank_l2_info_i.opcode inside {etlink_pkg::ReqWrite, etlink_pkg::ReqMsgSendData}) & bank_l2_info_i.write_is_partial;
  logic [LineDataSize-1:0] bank_l2_wr_data;
  assign bank_l2_wr_data = wr_data_mask(en_l2_wr_mask, bank_l2_info_i.ben) & bank_l2_info_i.data;
  // Write only for request that needs it
  assign other_wr_valid_d0[0]         = bank_l2_valid_i & bank_l2_ready_i & bank_l2_info_i.wdata &
     (bank_l2_info_i.opcode inside { etlink_pkg::ReqWrite, etlink_pkg::ReqWriteAround,
                                     etlink_pkg::ReqMsgSendData, etlink_pkg::ReqAtomic,
                                     etlink_pkg::ReqFlushToMem, etlink_pkg::ReqEvictToMem,
                                     etlink_pkg::ReqScpFill });
  assign other_wr_info_d0[0].reqq_id  = reqq_alloc_l2_id_ag_i;
  assign other_wr_info_d0[0].dataq_id = reqq_alloc_l2_id_ag_i;
  assign other_wr_info_d0[0].trans_id = bank_l2_info_i.trans_id;
  assign other_wr_info_d0[0].qwen     = (bank_l2_wr_all_qwen) ? '1 : bank_l2_info_i.qwen;
  assign other_wr_info_d0[0].wr_ben   = bank_l2_wr_ben;
  assign other_wr_info_d0[0].ben      = bank_l2_info_i.ben;
  assign other_wr_info_d0[0].data     =
     (bank_l2_info_i.opcode == etlink_pkg::ReqAtomic)
        ? ((L2AtomicDataMask & bank_l2_wr_data) |
           (LineDataSize'(bank_l2_info_i.subopcode) << AtomicConfLsb) |
           (LineDataSize'(bank_l2_info_i.address[LineOffsetIdSize-1:0]) << AtomicAddrOffsetLsb))
        : bank_l2_wr_data;
  assign dataq_bank_l2_ready_o        = ~other_wr_fifo_full_d0[0];


  // to_l3 mesh write
  assign other_wr_valid_d0[1]         = to_l3_mesh_rsp_valid_i;
  assign other_wr_info_d0[1].reqq_id  = to_l3_mesh_rsp_info_i.reqq_id;
  assign other_wr_info_d0[1].dataq_id = to_l3_mesh_rsp_info_i.dataq_id;
  assign other_wr_info_d0[1].trans_id = to_l3_mesh_rsp_info_i.trans_id;
  assign other_wr_info_d0[1].qwen     = '1;  // l2 miss/scp read is a fullline
  assign other_wr_info_d0[1].data     = to_l3_mesh_rsp_info_i.data;
  assign other_wr_info_d0[1].wr_ben   = '0;
  assign other_wr_info_d0[1].ben      = '0;
  assign to_l3_mesh_rsp_ready_o       = ~other_wr_fifo_full_d0[1];

  // Bank L3 write
  // Write only for request that needs it
  logic bank_l3_wr_ben;
  assign bank_l3_wr_ben = (bank_l3_info_i.opcode == etlink_pkg::ReqWrite) & bank_l3_info_i.write_is_partial;
  // If partial write of a line, then write all qwen.  This is necessary so that all qwens have been
  // written so that later when we read all qwen, everything is initialized because later after
  // a partial victim hit flush and fill, the qwen is reconstructed from the address and size and
  // calculated to be all 1s. At this point the read is all 1s and the write is controlled by ben.
  logic bank_l3_wr_all_qwen;
  assign bank_l3_wr_all_qwen = bank_l3_wr_ben & (bank_l3_info_i.size == etlink_pkg::SizeLine);
  logic [LineDataSize-1:0] bank_l3_wr_data;
  assign bank_l3_wr_data = wr_data_mask(bank_l3_wr_ben, bank_l3_info_i.ben) & bank_l3_info_i.data;

  assign other_wr_valid_d0[2]         = bank_l3_valid_i & bank_l3_ready_i & bank_l3_info_i.wdata &
     (bank_l3_info_i.opcode inside { etlink_pkg::ReqWrite, etlink_pkg::ReqAtomic,
                                     etlink_pkg::ReqFlushToMem, etlink_pkg::ReqEvictToMem });
  assign other_wr_info_d0[2].reqq_id  = reqq_alloc_l3_id_ag_i;
  assign other_wr_info_d0[2].dataq_id = reqq_alloc_l3_id_ag_i;
  assign other_wr_info_d0[2].trans_id = bank_l3_info_i.trans_id;
  assign other_wr_info_d0[2].qwen     = (bank_l3_wr_all_qwen) ? '1 : bank_l3_info_i.qwen;  // swiss cheese allowed if partial write around
  assign other_wr_info_d0[2].data     = bank_l3_wr_data;
  assign other_wr_info_d0[2].wr_ben   = bank_l3_wr_ben;
  assign other_wr_info_d0[2].ben      = bank_l3_info_i.ben;
  assign dataq_bank_l3_ready_o        = ~other_wr_fifo_full_d0[2];

  // to_sys mesh write
  assign other_wr_valid_d0[3]         = to_sys_mesh_rsp_valid_i;
  assign other_wr_info_d0[3].reqq_id  = to_sys_mesh_rsp_info_i.reqq_id;
  assign other_wr_info_d0[3].dataq_id = to_sys_mesh_rsp_info_i.dataq_id;
  assign other_wr_info_d0[3].trans_id = to_sys_mesh_rsp_info_i.trans_id;
  assign other_wr_info_d0[3].qwen     = '1;  // l3 miss/scp read is a fullline
  assign other_wr_info_d0[3].data     = to_sys_mesh_rsp_info_i.data;
  assign other_wr_info_d0[3].wr_ben   = '0;
  assign other_wr_info_d0[3].ben      = '0;
  assign to_sys_mesh_rsp_ready_o      = ~other_wr_fifo_full_d0[3];

  // Generate the other_wr fifos
  logic [NUM_WR-1:0] other_wr_valid_d1;
  logic [NUM_WR-1:0] other_wr_ready_d1;
  logic [NUM_WR-1:0] other_wr_grant_d1;
  dataq_wr_t [NUM_WR-1:0] other_wr_info_d1;

  for (genvar gen_it = 0; gen_it < NUM_WR; gen_it++) begin : gen_wr_fifo
    logic [FIFO_DEPTH:0] other_wr_fifo_depth_unused;
    prim_fifo_reg #(
      .Width ($bits(dataq_wr_t)),
      .Depth (FIFO_DEPTH)
    ) u_other_wr_fifo (
      .clk_i  (clk_i),
      .rst_ni (rst_n),
      .push_i (other_wr_valid_d0[gen_it]),
      .wdata_i(other_wr_info_d0[gen_it]),
      .full_o (other_wr_fifo_full_d0[gen_it]),
      .level_o(other_wr_fifo_depth_unused),
      .pop_i  (other_wr_ready_d1[gen_it]),
      .valid_o(other_wr_valid_d1[gen_it]),
      .rdata_o(other_wr_info_d1[gen_it])
    );
  end

  // **************************************************************************
  // Handle write arbitration
  // **************************************************************************
  // Round robin between non-pipe writes
  logic select_non_pipe_wr_d1;
  dataq_wr_t other_wr_winner_info_d1;
  prim_arb_rr #(
    .NumReqs (NUM_WR),
    .Width   ($bits(dataq_wr_t))
  ) u_wr_rr_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (other_wr_valid_d1),
    .grants_o (other_wr_grant_d1),
    .pop_i    (select_non_pipe_wr_d1),
    .data_i   (other_wr_info_d1),
    .data_o   (other_wr_winner_info_d1)
  );
  assign other_wr_ready_d1 = other_wr_grant_d1 & {NUM_WR{select_non_pipe_wr_d1}};

  // Priority arb between pipeline and non-pipeline writes
  logic      wr_valid_d1;
  logic      pipe_wr_ready_d1_unused;
  dataq_wr_t wr_info_d1;
  prim_arb_prio #(
    .NumClients (2),
    .Width      ($bits(dataq_wr_t))
  ) u_wr_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .bid_i    ({pipe_wr_valid_d1, |(other_wr_valid_d1)}),
    .stall_i  (1'b0),
    .grant_o  ({pipe_wr_ready_d1_unused, select_non_pipe_wr_d1}),
    .data_i   ({pipe_wr_info_d1, other_wr_winner_info_d1}),
    .data_o   (wr_info_d1)
  );
  assign wr_valid_d1 = |{pipe_wr_valid_d1, other_wr_valid_d1};

  // **************************************************************************
  // Register read requests
  // **************************************************************************
  // Register the pipeline read
  logic         pipe_rd_valid_d0;
  dataq_rd_t    pipe_rd_info_d0;

  // Fifo the other reads
  logic         [NUM_WR-1:0] other_rd_valid_d0;
  logic         [NUM_WR-1:0] other_rd_fifo_full_d0;
  dataq_rd_t    [NUM_WR-1:0] other_rd_info_d0;

  assign pipe_rd_valid_d0           = pipe_dataq_rd_req_valid_i;
  assign pipe_rd_info_d0.reqq_id    = pipe_dataq_rd_req_info_i.reqq_id;
  assign pipe_rd_info_d0.dataq_id   = pipe_dataq_rd_req_info_i.reqq_id;
  assign pipe_rd_info_d0.trans_id   = pipe_dataq_rd_req_info_i.trans_id;
  assign pipe_rd_info_d0.qwen       = pipe_dataq_rd_req_info_i.qwen;
  assign pipe_rd_info_d0.rd_port_id = DataqRdPipe;
  assign pipe_rd_info_d0.rd_ben     = pipe_dataq_rd_req_info_i.partial;

  logic         pipe_rd_valid_d1;
  dataq_rd_t    pipe_rd_info_d1;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                        pipe_rd_valid_d1 <= '0;
    else if (pipe_rd_valid_d0 | pipe_rd_valid_d1)      pipe_rd_valid_d1 <= pipe_rd_valid_d0;
  end
  always_ff @(posedge clk_i) begin
    if (pipe_rd_valid_d0)                               pipe_rd_info_d1 <= pipe_rd_info_d0;
  end

  // to_l3 mesh request read
  assign other_rd_valid_d0[0]           = reqq_to_l3_mesh_req_valid_wdata_i;
  assign other_rd_info_d0[0].reqq_id    = reqq_to_l3_mesh_req_info_i.reqq_id;
  assign other_rd_info_d0[0].dataq_id   = reqq_to_l3_mesh_req_info_i.dataq_id;
  assign other_rd_info_d0[0].trans_id   = reqq_to_l3_mesh_req_info_i.trans_id;
  assign other_rd_info_d0[0].qwen       = reqq_to_l3_mesh_req_info_i.qwen;
  assign other_rd_info_d0[0].rd_port_id = DataqRdToL3Mesh;
  assign other_rd_info_d0[0].rd_ben     = reqq_to_l3_mesh_req_info_i.rd_ben;
  assign reqq_to_l3_mesh_req_ready_wdata_o = ~other_rd_fifo_full_d0[0];

  // rspmux_l2 read
  assign other_rd_valid_d0[1]           = reqq_rspmux_l2_valid_wdata_i;
  assign other_rd_info_d0[1].reqq_id    = reqq_rspmux_l2_info_i.reqq_id;
  assign other_rd_info_d0[1].dataq_id   = reqq_rspmux_l2_info_i.reqq_id;
  assign other_rd_info_d0[1].trans_id   = reqq_rspmux_l2_info_i.trans_id;
  assign other_rd_info_d0[1].qwen       = reqq_rspmux_l2_info_i.qwen;
  assign other_rd_info_d0[1].rd_port_id = DataqRdRspmuxL2;
  assign other_rd_info_d0[1].rd_ben     = '0;
  assign reqq_rspmux_l2_ready_wdata_o   = ~other_rd_fifo_full_d0[1];

  // to_sys mesh request read
  assign other_rd_valid_d0[2]           = reqq_to_sys_mesh_req_valid_wdata_i;
  assign other_rd_info_d0[2].reqq_id    = reqq_to_sys_mesh_req_info_i.reqq_id;
  assign other_rd_info_d0[2].dataq_id   = reqq_to_sys_mesh_req_info_i.dataq_id;
  assign other_rd_info_d0[2].trans_id   = reqq_to_sys_mesh_req_info_i.trans_id;
  assign other_rd_info_d0[2].qwen       = reqq_to_sys_mesh_req_info_i.qwen;
  assign other_rd_info_d0[2].rd_port_id = DataqRdToSysMesh;
  assign other_rd_info_d0[2].rd_ben     = reqq_to_sys_mesh_req_info_i.rd_ben;
  assign reqq_to_sys_mesh_req_ready_wdata_o = ~other_rd_fifo_full_d0[2];

  // rspmux_l3 read
  assign other_rd_valid_d0[3]           = reqq_rspmux_l3_valid_wdata_i;
  assign other_rd_info_d0[3].reqq_id    = reqq_rspmux_l3_info_i.reqq_id;
  assign other_rd_info_d0[3].dataq_id   = reqq_rspmux_l3_info_i.reqq_id;
  assign other_rd_info_d0[3].trans_id   = reqq_rspmux_l3_info_i.trans_id;
  assign other_rd_info_d0[3].qwen       = reqq_rspmux_l3_info_i.qwen;
  assign other_rd_info_d0[3].rd_port_id = DataqRdRspmuxL3;
  assign other_rd_info_d0[3].rd_ben     = '0;
  assign reqq_rspmux_l3_ready_wdata_o   = ~other_rd_fifo_full_d0[3];

  // Generate the other_rd fifos
  logic [NUM_RD-1:0] other_rd_valid_d1;
  logic [NUM_RD-1:0] other_rd_ready_d1;
  logic [NUM_RD-1:0] other_rd_grant_d1;
  dataq_rd_t [NUM_RD-1:0] other_rd_info_d1;

  for (genvar gen_it = 0; gen_it < NUM_RD; gen_it++) begin : gen_rd_fifo
    logic [FIFO_DEPTH:0] other_rd_fifo_depth_unused;
    prim_fifo_reg #(
      .Width ($bits(dataq_rd_t)),
      .Depth (FIFO_DEPTH)
    ) u_other_rd_fifo (
      .clk_i  (clk_i),
      .rst_ni (rst_n),
      .push_i (other_rd_valid_d0[gen_it]),
      .wdata_i(other_rd_info_d0[gen_it]),
      .full_o (other_rd_fifo_full_d0[gen_it]),
      .level_o(other_rd_fifo_depth_unused),
      .pop_i  (other_rd_ready_d1[gen_it]),
      .valid_o(other_rd_valid_d1[gen_it]),
      .rdata_o(other_rd_info_d1[gen_it])
    );
  end

  // **************************************************************************
  // Handle read arbitration
  // **************************************************************************
  // Round robin between non-pipe reads
  logic select_non_pipe_rd_d1;
  logic [NUM_RD-1:0] rd_sink_ctl_fifo_full;
  logic [NUM_RD-1:0] other_rd_valid_and_rd_sink_fifo_room_d1;
  dataq_rd_t other_rd_winner_info_d1;

  // Don't take a request if there is no room for the read data on the other side
  assign other_rd_valid_and_rd_sink_fifo_room_d1 = other_rd_valid_d1 & ~rd_sink_ctl_fifo_full;
  prim_arb_rr #(
    .NumReqs (NUM_RD),
    .Width   ($bits(dataq_rd_t))
  ) u_rd_rr_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (other_rd_valid_and_rd_sink_fifo_room_d1),
    .grants_o (other_rd_grant_d1),
    .pop_i    (select_non_pipe_rd_d1),
    .data_i   (other_rd_info_d1),
    .data_o   (other_rd_winner_info_d1)
  );
  assign other_rd_ready_d1 = other_rd_grant_d1 & {NUM_WR{select_non_pipe_rd_d1}};

  // Priority arb between pipeline and mesh/req_xbar
  logic      rd_valid_d1;
  logic      pipe_rd_ready_d1_unused;
  dataq_rd_t rd_info_d1;
  prim_arb_prio #(
    .NumClients (2),
    .Width      ($bits(dataq_rd_t))
  ) u_rd_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .bid_i    ({pipe_rd_valid_d1, |(other_rd_valid_d1)}),
    .stall_i  (1'b0),
    .grant_o  ({pipe_rd_ready_d1_unused, select_non_pipe_rd_d1}),
    .data_i   ({pipe_rd_info_d1, other_rd_winner_info_d1}),
    .data_o   (rd_info_d1)
  );
  assign rd_valid_d1 = |{pipe_rd_valid_d1, other_rd_valid_and_rd_sink_fifo_room_d1};

  // **************************************************************************
  // Signal to the reqq when data is read / written
  // **************************************************************************
  assign dataq_write_status_any_o =  wr_valid_d1;
  assign dataq_write_status_id_o  =  wr_info_d1.reqq_id;
  always_comb begin
    dataq_write_status_hot_o = '0;
    dataq_write_status_hot_o[wr_info_d1.reqq_id] = wr_valid_d1;
  end
  always_comb begin
    dataq_read_status_hot_o = '0;
    dataq_read_status_hot_o[rd_info_d1.reqq_id] = rd_valid_d1;
  end

  // **************************************************************************
  // Remember who read requests are for
  // **************************************************************************
  logic              rd_valid_d2;
  logic              rd_valid_d3;
  dataq_rd_t         rd_info_d2;
  dataq_rd_t         rd_info_d3;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)  rd_valid_d2 <= '0;
    else         rd_valid_d2 <= rd_valid_d1;
  end
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)  rd_valid_d3 <= '0;
    else         rd_valid_d3 <= rd_valid_d2;
  end
  always_ff @(posedge clk_i) begin
    if (rd_valid_d1)  rd_info_d2 <= rd_info_d1;
  end
  always_ff @(posedge clk_i) begin
    if (rd_valid_d2)  rd_info_d3 <= rd_info_d2;
  end

  // **************************************************************************
  // d1 -> d2
  // **************************************************************************
  logic [LineQwSize-1:0]      dataq_wr_en_d1;
  logic [ReqqIdSize-1:0]      dataq_wr_addr_d1;
  logic [LineDataSize-1:0]    dataq_wr_data_d1;

  assign dataq_wr_en_d1   = {4{wr_valid_d1}} & wr_info_d1.qwen;
  assign dataq_wr_addr_d1 = wr_info_d1.dataq_id;
  assign dataq_wr_data_d1 = wr_info_d1.data;

  logic                       dataq_rd_en_d2;
  logic  [ReqqIdSize-1:0]     dataq_rd_addr_d2;
  logic [LineDataSize-1:0]    dataq_rd_data_d2;
  logic [LineQwSize-1:0]      dataq_wr_en_d2;
  logic [ReqqIdSize-1:0]      dataq_wr_addr_d2;
  logic [LineDataSize-1:0]    dataq_wr_data_d2;

  assign dataq_rd_en_d2   = rd_valid_d2;
  assign dataq_rd_addr_d2 = rd_info_d2.dataq_id;

  logic                       ben_wr_en_d1;
  logic [ReqqIdSize-1:0]      ben_wr_addr_d1;
  logic [LineByteSize-1:0]    ben_wr_data_d1;

  assign ben_wr_en_d1   = wr_valid_d1 & wr_info_d1.wr_ben;
  assign ben_wr_addr_d1 = wr_info_d1.dataq_id;
  assign ben_wr_data_d1 = wr_info_d1.ben;

  logic                       ben_rd_en_d2;
  logic  [ReqqIdSize-1:0]     ben_rd_addr_d2;
  logic [LineByteSize-1:0]    ben_rd_data_d2;
  logic                       ben_wr_en_d2;
  logic [ReqqIdSize-1:0]      ben_wr_addr_d2;
  logic [LineByteSize-1:0]    ben_wr_data_d2;

  assign ben_rd_en_d2   = rd_valid_d2 & rd_info_d2.rd_ben;
  assign ben_rd_addr_d2 = rd_info_d2.dataq_id;

  // dataq_wr_en_d1 -> dataq_wr_en_d2 (FF, unconditional)
  always_ff @(posedge clk_i) begin
    dataq_wr_en_d2 <= dataq_wr_en_d1;
  end
  // dataq_wr_addr_d1 -> dataq_wr_addr_d2 (EN_FF)
  always_ff @(posedge clk_i) begin
    if (|dataq_wr_en_d1) dataq_wr_addr_d2 <= dataq_wr_addr_d1;
  end
  // dataq_wr_data per-qw EN_FF
  for (genvar gen_qw = 0; gen_qw < LineQwSize; gen_qw++) begin : gen_wr_data_qw
    always_ff @(posedge clk_i) begin
      if (dataq_wr_en_d1[gen_qw]) dataq_wr_data_d2[gen_qw*128 +:128] <= dataq_wr_data_d1[gen_qw*128 +:128];
    end
  end
  // ben FF / EN_FF
  always_ff @(posedge clk_i) begin
    ben_wr_en_d2 <= ben_wr_en_d1;
  end
  always_ff @(posedge clk_i) begin
    if (ben_wr_en_d1) ben_wr_addr_d2 <= ben_wr_addr_d1;
  end
  always_ff @(posedge clk_i) begin
    if (ben_wr_en_d1) ben_wr_data_d2 <= ben_wr_data_d1;
  end

  // **************************************************************************
  // Instantiate the ram
  // **************************************************************************

  for (genvar gen_qw = 0; gen_qw < LineQwSize; gen_qw++) begin : gen_dataq_ram
    prim_rf_1r1w_preview #(
      .Width  (128),
      .Depth  (NumEntries)
    ) u_dataq_ram (
      .preview_clk_i        (clk_i),
      .rf_clk_i             (clk_i),
      .wr_data_en_1p_next_i (dataq_wr_en_d1[gen_qw]),
      .wr_addr_i            (dataq_wr_addr_d2[NUM_ENTRIES_LOG2-1:0]),
      .wr_en_i              (dataq_wr_en_d2[gen_qw] && !dft_i.ram_wei),
      .wr_data_i            (dataq_wr_data_d2[gen_qw*128+:128]),
      .rd_addr_i            (dataq_rd_addr_d2[NUM_ENTRIES_LOG2-1:0]),
      .rd_data_o            (dataq_rd_data_d2[gen_qw*128+:128])
    );
  end

  prim_rf_1r1w_preview #(
    .Width  (LineByteSize),
    .Depth  (NumEntries)
  ) u_ben_ram (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (ben_wr_en_d1),
    .wr_addr_i            (ben_wr_addr_d2[NUM_ENTRIES_LOG2-1:0]),
    .wr_en_i              (ben_wr_en_d2 && !dft_i.ram_wei),
    .wr_data_i            (ben_wr_data_d2),
    .rd_addr_i            (ben_rd_addr_d2[NUM_ENTRIES_LOG2-1:0]),
    .rd_data_o            (ben_rd_data_d2)
  );

  // **************************************************************************
  // Register read data
  // **************************************************************************
  logic [LineDataSize-1:0] rd_data_d3;
  logic [LineByteSize-1:0] rd_ben_d3;
  always_ff @(posedge clk_i) begin
    if (dataq_rd_en_d2) rd_data_d3 <= dataq_rd_data_d2;
  end
  always_ff @(posedge clk_i) begin
    if (ben_rd_en_d2)   rd_ben_d3 <= ben_rd_data_d2;
  end

  // **************************************************************************
  // Read responses fifos
  // **************************************************************************

  logic         [NUM_RD-1:0] rd_sink_ctl_fifo_push_d1;
  logic         [NUM_RD-1:0] rd_sink_ctl_fifo_valid_unused;  // unused because we use data fifo valid
  logic         [NUM_RD-1:0] rd_sink_ctl_fifo_pop;
  dataq_rd_t    [NUM_RD-1:0] rd_sink_ctl_fifo_info;

  logic         [NUM_RD-1:0] rd_sink_data_fifo_push_d3;
  logic         [NUM_RD-1:0] rd_sink_data_fifo_valid;
  logic         [NUM_RD-1:0] rd_sink_data_fifo_pop;
  dataq_data_t  [NUM_RD-1:0] rd_sink_data_fifo_info;

  logic         [NUM_RD-1:0] rd_sink_ben_fifo_push_d3;
  logic         [NUM_RD-1:0] rd_sink_ben_fifo_valid;
  logic         [NUM_RD-1:0] rd_sink_ben_fifo_pop;
  dataq_ben_t   [NUM_RD-1:0] rd_sink_ben_fifo_info;

  for (genvar gen_it = 0; gen_it < NUM_RD; gen_it++) begin : gen_rd_sink
    logic [FIFO_DEPTH:0] rd_sink_ctl_fifo_level_unused;
    prim_fifo_reg #(
      .Width ($bits(dataq_rd_t)),
      .Depth (FIFO_DEPTH)
    ) u_rd_sink_ctl_fifo (
      .clk_i  (clk_i),
      .rst_ni (rst_n),
      .push_i (rd_sink_ctl_fifo_push_d1[gen_it]),
      .wdata_i(rd_info_d1),
      .full_o (rd_sink_ctl_fifo_full[gen_it]),
      .level_o(rd_sink_ctl_fifo_level_unused),
      .pop_i  (rd_sink_ctl_fifo_pop[gen_it]),
      .valid_o(rd_sink_ctl_fifo_valid_unused[gen_it]),
      .rdata_o(rd_sink_ctl_fifo_info[gen_it])
    );

    logic [FIFO_DEPTH:0] rd_sink_data_fifo_level_unused;
    logic rd_sink_data_fifo_full_unused;
    prim_fifo_reg #(
      .Width ($bits(dataq_data_t)),
      .Depth (FIFO_DEPTH)
    ) u_rd_sink_data_fifo (
      .clk_i  (clk_i),
      .rst_ni (rst_n),
      .push_i (rd_sink_data_fifo_push_d3[gen_it]),
      .wdata_i(dataq_data_t'{data: rd_data_d3}),
      .full_o (rd_sink_data_fifo_full_unused),
      .level_o(rd_sink_data_fifo_level_unused),
      .pop_i  (rd_sink_data_fifo_pop[gen_it]),
      .valid_o(rd_sink_data_fifo_valid[gen_it]),
      .rdata_o(rd_sink_data_fifo_info[gen_it])
    );

    logic [FIFO_DEPTH:0] rd_sink_ben_fifo_level_unused;
    logic rd_sink_ben_fifo_full_unused;
    prim_fifo_reg #(
      .Width (LineByteSize),
      .Depth (FIFO_DEPTH)
    ) u_rd_sink_ben_fifo (
      .clk_i  (clk_i),
      .rst_ni (rst_n),
      .push_i (rd_sink_ben_fifo_push_d3[gen_it]),
      .wdata_i(rd_ben_d3),
      .full_o (rd_sink_ben_fifo_full_unused),
      .level_o(rd_sink_ben_fifo_level_unused),
      .pop_i  (rd_sink_ben_fifo_pop[gen_it]),
      .valid_o(rd_sink_ben_fifo_valid[gen_it]),
      .rdata_o(rd_sink_ben_fifo_info[gen_it])
    );
  end

  // assign to_l3_mesh_reqq output
  // --------------------------------------------------------------------------
  assign rd_sink_ctl_fifo_push_d1[0]        = rd_valid_d1 &
     (rd_info_d1.rd_port_id == DataqRdToL3Mesh);
  assign rd_sink_data_fifo_push_d3[0]       = rd_valid_d3 &
     (rd_info_d3.rd_port_id == DataqRdToL3Mesh);
  assign rd_sink_ben_fifo_push_d3[0]        = rd_valid_d3 & rd_info_d3.rd_ben &
     (rd_info_d3.rd_port_id == DataqRdToL3Mesh);
  assign rd_sink_ctl_fifo_pop[0]            = dataq_to_l3_mesh_req_valid_o & dataq_to_l3_mesh_req_ready_i;
  assign rd_sink_data_fifo_pop[0]           = dataq_to_l3_mesh_req_valid_o & dataq_to_l3_mesh_req_ready_i;
  assign rd_sink_ben_fifo_pop[0]            = rd_sink_data_fifo_pop[0] & rd_sink_ctl_fifo_info[0].rd_ben;

  assign dataq_to_l3_mesh_req_valid_o         = rd_sink_data_fifo_valid[0];        // There's stuff in the data fifo
  assign dataq_to_l3_mesh_req_info_o.trans_id = rd_sink_ctl_fifo_info[0].trans_id;
  assign dataq_to_l3_mesh_req_info_o.reqq_id  = rd_sink_ctl_fifo_info[0].reqq_id;
  assign dataq_to_l3_mesh_req_info_o.dataq_id = rd_sink_ctl_fifo_info[0].dataq_id;
  assign dataq_to_l3_mesh_req_info_o.data     = rd_sink_data_fifo_info[0].data;
  assign dataq_to_l3_mesh_req_info_o.with_data= '1;
  assign dataq_to_l3_mesh_req_info_o.qwen     = '0; // Unused
  assign dataq_to_l3_mesh_req_info_o.rd_ben   = rd_sink_ctl_fifo_info[0].rd_ben;
  assign dataq_to_l3_mesh_req_info_o.ben      = rd_sink_ben_fifo_info[0].ben;

  // assign rspmux_l2 output
  // --------------------------------------------------------------------------
  assign rd_sink_ctl_fifo_push_d1[1]       = rd_valid_d1 &
     (rd_info_d1.rd_port_id == DataqRdRspmuxL2);
  assign rd_sink_data_fifo_push_d3[1]      = rd_valid_d3 &
     (rd_info_d3.rd_port_id == DataqRdRspmuxL2);
  assign rd_sink_ben_fifo_push_d3[1]        = rd_valid_d3 & rd_info_d3.rd_ben &
     (rd_info_d3.rd_port_id == DataqRdRspmuxL2);
  assign rd_sink_ctl_fifo_pop[1]           = dataq_rspmux_l2_valid_o & dataq_rspmux_l2_ready_i;
  assign rd_sink_data_fifo_pop[1]          = dataq_rspmux_l2_valid_o & dataq_rspmux_l2_ready_i;
  assign rd_sink_ben_fifo_pop[1]           = '0;

  assign dataq_rspmux_l2_valid_o             = rd_sink_data_fifo_valid[1];        // There's stuff in the data fifo
  assign dataq_rspmux_l2_info_o.trans_id     = rd_sink_ctl_fifo_info[1].trans_id;
  assign dataq_rspmux_l2_info_o.reqq_id      = rd_sink_ctl_fifo_info[1].reqq_id;
  assign dataq_rspmux_l2_info_o.data         = rd_sink_data_fifo_info[1].data;
  assign dataq_rspmux_l2_info_o.with_data    = '1;
  assign dataq_rspmux_l2_info_o.qwen         = '0;  // Unused
  assign dataq_rspmux_l2_info_o.zero_data    = '0;  // Unused

  // assign to_sys_mesh_reqq output
  // --------------------------------------------------------------------------
  assign rd_sink_ctl_fifo_push_d1[2]        = rd_valid_d1 &
     (rd_info_d1.rd_port_id == DataqRdToSysMesh);
  assign rd_sink_data_fifo_push_d3[2]       = rd_valid_d3 &
     (rd_info_d3.rd_port_id == DataqRdToSysMesh);
  assign rd_sink_ben_fifo_push_d3[2]        = rd_valid_d3 & rd_info_d3.rd_ben &
     (rd_info_d3.rd_port_id == DataqRdToSysMesh);
  assign rd_sink_ctl_fifo_pop[2]            = dataq_to_sys_mesh_req_valid_o & dataq_to_sys_mesh_req_ready_i;
  assign rd_sink_data_fifo_pop[2]           = dataq_to_sys_mesh_req_valid_o & dataq_to_sys_mesh_req_ready_i;
  assign rd_sink_ben_fifo_pop[2]            = rd_sink_data_fifo_pop[2] & rd_sink_ctl_fifo_info[2].rd_ben;

  assign dataq_to_sys_mesh_req_valid_o         = rd_sink_data_fifo_valid[2];        // There's stuff in the data fifo
  assign dataq_to_sys_mesh_req_info_o.trans_id = rd_sink_ctl_fifo_info[2].trans_id;
  assign dataq_to_sys_mesh_req_info_o.reqq_id  = rd_sink_ctl_fifo_info[2].reqq_id;
  assign dataq_to_sys_mesh_req_info_o.dataq_id = rd_sink_ctl_fifo_info[2].dataq_id;
  assign dataq_to_sys_mesh_req_info_o.data     = rd_sink_data_fifo_info[2].data;
  assign dataq_to_sys_mesh_req_info_o.with_data= '1;
  assign dataq_to_sys_mesh_req_info_o.qwen     = '0; // Unused
  assign dataq_to_sys_mesh_req_info_o.rd_ben   = rd_sink_ctl_fifo_info[2].rd_ben;
  assign dataq_to_sys_mesh_req_info_o.ben      = rd_sink_ben_fifo_info[2].ben;


  // assign rspmux_l3 output
  // --------------------------------------------------------------------------
  assign rd_sink_ctl_fifo_push_d1[3]       = rd_valid_d1 &
     (rd_info_d1.rd_port_id == DataqRdRspmuxL3);
  assign rd_sink_data_fifo_push_d3[3]      = rd_valid_d3 &
     (rd_info_d3.rd_port_id == DataqRdRspmuxL3);
  assign rd_sink_ben_fifo_push_d3[3]        = rd_valid_d3 & rd_info_d3.rd_ben &
     (rd_info_d3.rd_port_id == DataqRdRspmuxL3);
  assign rd_sink_ctl_fifo_pop[3]           = dataq_rspmux_l3_valid_o & dataq_rspmux_l3_ready_i;
  assign rd_sink_data_fifo_pop[3]          = dataq_rspmux_l3_valid_o & dataq_rspmux_l3_ready_i;
  assign rd_sink_ben_fifo_pop[3]           = '0;

  assign dataq_rspmux_l3_valid_o             = rd_sink_data_fifo_valid[3];        // There's stuff in the data fifo
  assign dataq_rspmux_l3_info_o.trans_id     = rd_sink_ctl_fifo_info[3].trans_id;
  assign dataq_rspmux_l3_info_o.reqq_id      = rd_sink_ctl_fifo_info[3].reqq_id;
  assign dataq_rspmux_l3_info_o.data         = rd_sink_data_fifo_info[3].data;
  assign dataq_rspmux_l3_info_o.with_data    = '1;
  assign dataq_rspmux_l3_info_o.qwen         = '0;  // Unused
  assign dataq_rspmux_l3_info_o.zero_data    = '0;  // Unused

  // assign the pipeline read result
  // --------------------------------------------------------------------------
  dataq_rd_rsp_t pipe_data_rsp_info_d3;

  assign pipe_data_rsp_info_d3.trans_id = rd_info_d3.trans_id;
  assign pipe_data_rsp_info_d3.reqq_id  = rd_info_d3.reqq_id;
  assign pipe_data_rsp_info_d3.data     = rd_data_d3;
  assign pipe_data_rsp_info_d3.ben      = rd_ben_d3;
  assign pipe_data_rsp_info_d3.err      = '0; // FUTURE kristie/bill to fix later..

  assign pipe_dataq_rd_rsp_info_o = pipe_data_rsp_info_d3;


  // Functions
  // --------------------------------------------------------------------------
  // Generate the wr data mask from the wr data BEN
  function automatic [LineDataSize-1:0] wr_data_mask;
    input                          wr_data_ben_en;
    input  [LineByteSize-1:0]      wr_data_ben;
    begin
      wr_data_mask = '0;
      for (integer ii = 0; ii < LineByteSize; ii++) begin
        // If ben isn't enable, then all bytes are enabled
        wr_data_mask[ii * ByteDataSize +: ByteDataSize] = {ByteDataSize{(wr_data_ben[ii] | ~wr_data_ben_en)}};
      end
    end
  endfunction


  // ── Assertions ────────────────────────────────────────
  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_n used in both async reset and assertion checks
  always_ff @(posedge clk_i) begin
    if (rst_n && pipe_wr_valid_d1 && !pipe_wr_ready_d1_unused)
      $error("ERROR_pipe_wr_always_wins");
    if (rst_n && pipe_rd_valid_d1 && !pipe_rd_ready_d1_unused)
      $error("ERROR_pipe_rd_always_wins");
    if (rst_n && rd_valid_d1 && (|dataq_wr_en_d1) && (rd_info_d1.dataq_id == dataq_wr_addr_d1))
      $error("ERROR_dataq_read_and_write_same_addr");
    if (rst_n && rd_valid_d1 && (|dataq_wr_en_d2) && (rd_info_d1.dataq_id == dataq_wr_addr_d2))
      $error("ERROR_dataq_read_followed_write");
  end
  /* verilator lint_on SYNCASYNCNET */
`else
  assert property (@(posedge clk_i) disable iff (!rst_n) pipe_wr_valid_d1 |-> pipe_wr_ready_d1_unused)
  else $error("ERROR_pipe_wr_always_wins");
  assert property (@(posedge clk_i) disable iff (!rst_n) pipe_rd_valid_d1 |-> pipe_rd_ready_d1_unused)
  else $error("ERROR_pipe_rd_always_wins");
  assert property (@(posedge clk_i) disable iff (!rst_n) rd_valid_d1 |-> ~( (|dataq_wr_en_d1) & (rd_info_d1.dataq_id == dataq_wr_addr_d1)))
  else $error("ERROR_dataq_read_and_write_same_addr");
  assert property (@(posedge clk_i) disable iff (!rst_n) rd_valid_d1 |-> ~( (|dataq_wr_en_d2) & (rd_info_d1.dataq_id == dataq_wr_addr_d2)))
  else $error("ERROR_dataq_read_followed_write");
`endif
  // synthesis translate_on


  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    reqq_rspmux_l2_info_i.zero_data,
    reqq_rspmux_l3_info_i.zero_data,
    reqq_to_l3_mesh_req_info_i.ben,
    reqq_to_sys_mesh_req_info_i.ben,
    pipe_data_rsp_modifier_info_i,
    pipe_data_rsp_info_i.data_index,
    pipe_data_rsp_info_i.data_ram_dbe,
    pipe_data_rsp_info_i.data_ram_sbe,
    pipe_data_rsp_info_i.rsp_type,
    pipe_data_rsp_info_i.tag_sbe,
    to_l3_mesh_rsp_info_i.err,
    to_l3_mesh_rsp_info_i.zero_data,
    to_sys_mesh_rsp_info_i.err,
    to_sys_mesh_rsp_info_i.zero_data,
    rd_info_d3.dataq_id,
    rd_info_d3.qwen,
    bank_l2_info_i,
    bank_l3_info_i,
    dft_i,
    dataq_wr_addr_d2,
    dataq_rd_addr_d2,
    esr_ctl_i,
    pipe_wr_ready_d1_unused,
    pipe_rd_ready_d1_unused,
    rd_sink_ctl_fifo_valid_unused,
    rd_sink_ben_fifo_valid
  };

endmodule : shirecache_bank_dataq
/* verilator lint_on UNUSEDSIGNAL */
