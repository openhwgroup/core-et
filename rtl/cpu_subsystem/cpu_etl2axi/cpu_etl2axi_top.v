// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// Bridge coverting from ET-Link to AXI4 in the CPU subsystem
module cpu_etl2axi_top
(
  // System signals
  input  logic clock,
  input  logic reset,
  // ET-LINK INTERFACE
  // ET-Link Request bus
  input  et_link_req_info_t etl_req_info,
  input  logic              etl_req_valid,
  output logic              etl_req_ready,
  // ET-Link Response bus
  output et_link_rsp_info_t etl_rsp_info,
  output logic              etl_rsp_valid,
  input  logic              etl_rsp_ready,
  // AXI MASTER INTERFACE
  // Read address channel
  output [`CPU_AXI_ID_WIDTH-1:0]    axi_ARID,
  output [`CPU_AXI_ADDR_WIDTH-1:0]  axi_ARADDR,
  output [`CPU_AXI_LEN_WIDTH-1:0]   axi_ARLEN,
  output [`CPU_AXI_SIZE_WIDTH-1:0]  axi_ARSIZE,
  output [`CPU_AXI_BURST_WIDTH-1:0] axi_ARBURST,
  output                            axi_ARLOCK,
  output [`CPU_AXI_CACHE_WIDTH-1:0] axi_ARCACHE,
  output [`CPU_AXI_PROT_WIDTH-1:0]  axi_ARPROT,
  output                            axi_ARVALID,
  input                             axi_ARREADY,
  // Read data channel
  input  [`CPU_AXI_ID_WIDTH-1:0]    axi_RID,
  input  [`CPU_AXI_DATA_WIDTH-1:0]  axi_RDATA,
  input  [`CPU_AXI_RESP_WIDTH-1:0]  axi_RRESP,
  input                             axi_RLAST,
  input                             axi_RVALID,
  output                            axi_RREADY,
  // Write address channel
  output [`CPU_AXI_ID_WIDTH-1:0]    axi_AWID,
  output [`CPU_AXI_ADDR_WIDTH-1:0]  axi_AWADDR,
  output [`CPU_AXI_LEN_WIDTH-1:0]   axi_AWLEN,
  output [`CPU_AXI_SIZE_WIDTH-1:0]  axi_AWSIZE,
  output [`CPU_AXI_BURST_WIDTH-1:0] axi_AWBURST,
  output                            axi_AWLOCK,
  output [`CPU_AXI_CACHE_WIDTH-1:0] axi_AWCACHE,
  output [`CPU_AXI_PROT_WIDTH-1:0]  axi_AWPROT,
  output                            axi_AWVALID,
  input                             axi_AWREADY,
  // Write data channel
  output [`CPU_AXI_DATA_WIDTH-1:0]  axi_WDATA,
  output [`CPU_AXI_STRB_WIDTH-1:0]  axi_WSTRB,
  output                            axi_WLAST,
  output                            axi_WVALID,
  input                             axi_WREADY,
  // Write response channel
  input  [`CPU_AXI_ID_WIDTH-1:0]    axi_BID,
  input  [`CPU_AXI_RESP_WIDTH-1:0]  axi_BRESP,
  input                             axi_BVALID,
  output                            axi_BREADY
);

//===================================================================
// INTERNAL DECLARATIONS
//===================================================================
// Input request FIFO
logic              etl_req_fifo_push;
logic              etl_req_fifo_full;
logic              etl_req_fifo_pop;
et_link_req_info_t etl_req_info_int;
logic              etl_req_valid_int;
logic              etl_req_ready_int;
// Q0 stage
etl2axi_req_opcode_t            etl_req_op;
logic                           etl_op_err;
logic [`CPU_AXI_STRB_WIDTH-1:0] etl_req_strb;
logic [`ETL2AXI_TT_SIZE_L-1:0]  q0_req_tt_entry;
etl2axi_req_info_t              q0_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0] q0_req_data;
logic                           q0_wdata;
logic                           q0_valid;
logic                           q0_ready;
logic                           q0_acc;
// Q1 stage
logic                           q1_valid;
logic                           q1_ready;
logic [`ETL2AXI_TT_SIZE_L-1:0]  q1_req_tt_entry;
etl2axi_req_info_t              q1_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0] q1_req_data;
et_link_nodata_rsp_info_t       q1_rsp_info_msg;
et_link_nodata_rsp_info_t       q1_rsp_info_err;
et_link_nodata_rsp_info_t       q1_rsp_info_to_lb;
logic [`ET_LINE_DATA_SIZE-1:0]  q1_data_to_lb;
logic                           q1_valid_to_lb;
logic                           q1_ready_to_lb;
logic                           q1_valid_to_arb;
logic                           q1_ready_to_arb;
logic                           q1_from_tt;
logic [`ETL2AXI_TT_SIZE_L-1:0]  q1_tt_entry_to_q2;
etl2axi_req_info_t              q1_req_info_to_q2;
logic [`CPU_AXI_DATA_WIDTH-1:0] q1_req_data_to_q2;
logic                           q1_wdata_to_q2;
logic                           q1_valid_to_q2;
logic                           q1_ready_to_q2;
logic                           q1_acc_to_q2;
logic                           q1_conflict;
logic                           q1_conflict_qual;
// Q2 stage
logic [`ETL2AXI_TT_SIZE_L-1:0]  q2_tt_entry;
etl2axi_req_info_t              q2_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0] q2_req_data;
logic                           q2_valid;
logic                           q2_ready;
logic                           q2_conflict;
logic                           q2_valid_to_tt;
logic                           q2_valid_to_q3;
logic                           q2_ready_to_q3;
logic                           q2_lock;
logic                           q2_to_ar;
logic                           q2_arvalid;
logic                           q2_arready;
logic                           q2_aracc;
logic                           q2_awvalid;
logic                           q2_awready;
logic                           q2_awacc;
logic                           q2_wvalid;
logic                           q2_wready;
logic                           q2_wacc;
// Q3 stage
logic [`ETL2AXI_TT_SIZE_L-1:0]  q3_arid;
logic [`CPU_AXI_ADDR_WIDTH-1:0] q3_araddr;
logic [`CPU_AXI_SIZE_WIDTH-1:0] q3_arsize;
logic                           q3_arlock;
logic                           q3_arvalid;
logic                           q3_arready;
logic [`ETL2AXI_TT_SIZE_L-1:0]  q3_awid;
logic [`CPU_AXI_ADDR_WIDTH-1:0] q3_awaddr;
logic [`CPU_AXI_SIZE_WIDTH-1:0] q3_awsize;
logic                           q3_awlock;
logic                           q3_awvalid;
logic                           q3_awready;
logic [`CPU_AXI_DATA_WIDTH-1:0] q3_wdata;
logic [`CPU_AXI_STRB_WIDTH-1:0] q3_wstrb;
logic                           q3_wvalid;
logic                           q3_wready;
// P0 stage
logic p0_racc;
logic p0_bacc;
// P1 stage
logic [`CPU_AXI_ID_WIDTH-1:0]        p1_rid;
logic [`CPU_AXI_DATA_WIDTH-1:0]      p1_rdata;
cpu_axi_resp_t                       p1_rresp;
logic                                p1_rvalid;
logic                                p1_rgrant;
logic                                p1_rready;
logic [`CPU_AXI_ID_WIDTH-1:0]        p1_bid;
cpu_axi_resp_t                       p1_bresp;
logic                                p1_bvalid;
logic                                p1_bgrant;
logic                                p1_bready;
logic [`CPU_AXI_ID_WIDTH-1:0]        p1_axi_id;
cpu_axi_resp_t                       p1_axi_resp;
et_link_rsp_opcode_t                 p1_opcode;
logic [`ET_LINK_QWEN_SIZE-1:0]       p1_rd_qwen;
et_link_nodata_rsp_info_t            p1_rsp_info;
logic [`ET_LINE_DATA_SIZE-1:0]       p1_data;
logic                                p1_valid;
logic                                p1_ready;
logic [`ETL2AXI_TT_SIZE_L-1:0]       p1_tt_entry;
etl2axi_tt_req_info_t                p1_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0]      p1_at_rd_data;
logic [1:0]                          p1_at_retry_cnt;
logic                                p1_is_atomic_rd_exok;
logic                                p1_is_atomic_rd_ok;
logic                                p1_is_atomic_wr_ok;
logic                                p1_valid_pre_to_atomic;
logic                                p1_valid_to_atomic;
logic                                p1_valid_to_p2;
logic                                p1_ready_to_p2;
logic                                p1_acc_to_p2;
et_link_nodata_rsp_info_t            p1_rsp_info_xx;
logic [`ET_LINE_DATA_SIZE-1:0]       p1_data_xx;
logic                                p1_valid_xx;
logic                                p1_ready_xx;
logic                                p1_acc_xx;
// P2 stage
et_link_nodata_rsp_info_t       p2_rsp_info;
logic [`ET_LINE_DATA_SIZE-1:0]  p2_data;
logic                           p2_valid;
logic                           p2_ready;
// Transaction Table
etl2axi_tt_req_info_t           tt_alloc_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0] tt_alloc_req_data;
logic                           tt_alloc_valid;
logic [`ETL2AXI_TT_SIZE_L-1:0]  tt_alloc_entry;
logic                           tt_alloc_full;
logic [`ETL2AXI_TT_SIZE_L-1:0]  tt_dealloc_entry;
logic                           tt_dealloc_valid;
logic [`ETL2AXI_TT_SIZE_L-1:0]  tt_pop_entry;
etl2axi_req_info_t              tt_pop_req_info;
logic [`CPU_AXI_DATA_WIDTH-1:0] tt_pop_req_data;
logic                           tt_valid;
logic                           tt_pop;

//===================================================================
// REQUEST PIPELINE
//===================================================================
//-------------------------------------------------------------------
// Input request FIFO
//-------------------------------------------------------------------
logic etl_req_fifo_full_next_unused_ok;
logic etl_req_fifo_valid_next_unused_ok;

assign etl_req_ready     = ~etl_req_fifo_full;
assign etl_req_fifo_push = etl_req_valid & ~etl_req_fifo_full;

gen_fifo
#(
   .ELEM_SIZE ( $bits(et_link_req_info_t) ),
   .NUM_ELEMS ( 2                         )
)
etl_req_fifo
(
    // System signals
    .clock      ( clock             ),
    .reset      ( reset             ),
    // Push data
    .push_early ( 1'b0              ), // Unused
    .push       ( etl_req_fifo_push ),
    .push_data  ( etl_req_info      ),
    .full       ( etl_req_fifo_full ),
    .full_next  ( etl_req_fifo_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( etl_req_fifo_pop  ),
    .pop_data   ( etl_req_info_int  ),
    .valid      ( etl_req_valid_int ),
    .valid_next ( etl_req_fifo_valid_next_unused_ok )  // Unused
);

assign etl_req_fifo_pop = etl_req_valid_int & etl_req_ready_int;

//-------------------------------------------------------------------
// Q0 stage
//-------------------------------------------------------------------
// Generate internal request op. code
assign etl_req_op = etl_req_info_int.opcode == ET_LINK_REQ_Write  ? ETL2AXI_REQ_OP_Write
                  : etl_req_info_int.opcode == ET_LINK_REQ_Read   ? ETL2AXI_REQ_OP_Read
                  : etl_req_info_int.opcode == ET_LINK_REQ_Atomic ? ETL2AXI_REQ_OP_Atomic
                  :                                                 ETL2AXI_REQ_OP_Other;
// If it is an unsupported op. code, set the error flag
assign etl_op_err = (etl_req_info_int.opcode != ET_LINK_REQ_Write)  &&
                    (etl_req_info_int.opcode != ET_LINK_REQ_Read)   &&
                    (etl_req_info_int.opcode != ET_LINK_REQ_Atomic) &&
                    (etl_req_info_int.opcode != ET_LINK_REQ_MsgSendData);
// Generate byte enable
assign etl_req_strb = get_strb(etl_req_info_int.size,etl_req_info_int.address[`CPU_AXI_ADDR_OFFSET-1:0]);

// Allocate an entry in the Transaction Table
assign tt_alloc_req_info = '{id       : etl_req_info_int.id,
                             source   : et_link_neigh_source_t'(etl_req_info_int.source),
                             op       : etl_req_op,
                             address  : etl_req_info_int.address[`CPU_AXI_ADDR_WIDTH-1:0],
                             size     : etl_req_info_int.size,
                             strb     : etl_req_strb,
                             amo_conf : '{ op   : et_link_amo_opcode'(etl_req_info_int.subopcode[`ET_ATOMIC_OPCODE_START +: `ET_ATOMIC_OPCODE_SIZE]),
                                           size : etl2axi_tt_amosize_t'(etl_req_info_int.subopcode[`ET_ATOMIC_SIZE_START +: `ET_ATOMIC_SIZE_SIZE])
                                         }
                            };
assign tt_alloc_req_data = etl_req_info_int.data;
// Messages and unsupported op. codes do not allocate an entry in the Transaction Table
assign tt_alloc_valid    = etl_req_valid_int & etl_req_ready_int & (tt_alloc_req_info.op != ETL2AXI_REQ_OP_Other);

// If the Transaction Table is full, disable access to Q0 stage
assign q0_valid          = etl_req_valid_int & ~(tt_alloc_full & (tt_alloc_req_info.op != ETL2AXI_REQ_OP_Other));
assign etl_req_ready_int = q0_ready          & ~(tt_alloc_full & (tt_alloc_req_info.op != ETL2AXI_REQ_OP_Other));

// Obtain allocated Transaction Table entry
assign q0_req_tt_entry = tt_alloc_entry;

// Generate internal request
assign q0_req_info = '{id      : etl_req_info_int.id,
                       source  : et_link_neigh_source_t'(etl_req_info_int.source),
                       op      : etl_req_op,
                       address : etl_req_info_int.address[`CPU_AXI_ADDR_WIDTH-1:0],
                       size    : etl_req_info_int.size,
                       strb    : etl_req_strb,
                       qwen    : etl_req_info_int.qwen,
                       exop    : ETL2AXI_TT_AMOEXOP_Read, // If it is an atomic, the first step is an exclusive read operation
                       op_err  : etl_op_err
                      };
assign q0_req_data = etl_req_info_int.data; // AXI and ET-Link use the same data width

assign q0_wdata = (q0_req_info.op == ETL2AXI_REQ_OP_Write)
                  // Data is also transmitted for other operations, unless it is an unsupported op.code (i.e. for MsgSendData)
               || ((q0_req_info.op == ETL2AXI_REQ_OP_Other) && !q0_req_info.op_err);
assign q0_ready = q1_ready | ~q1_valid;
assign q0_acc   = q0_valid & q0_ready;

//-------------------------------------------------------------------
// Q1 stage
//-------------------------------------------------------------------
//         CLK    RST    EN                  DOUT             DIN       DEF
`RST_EN_FF(clock, reset, q0_ready,           q1_valid,        q0_valid, 1'b0)
`EN_FF    (clock,        q0_acc,             q1_req_tt_entry, q0_req_tt_entry)
`EN_FF    (clock,        q0_acc,             q1_req_info,     q0_req_info)
`EN_FF    (clock,        q0_acc && q0_wdata, q1_req_data,     q0_req_data)

assign q1_ready = q1_req_info.op == ETL2AXI_REQ_OP_Other ? q1_ready_to_lb : q1_ready_to_arb;

// Send error responses for requests with illegal op. codes and loopbacked messages to response datapath
assign q1_rsp_info_msg = '{id     : q1_req_info.address[`AD_ESR_MESSAGE_ID_RANGE],
                           dest   : q1_req_info.address[`AD_ESR_NEIGH_HART_ID_RANGE],
                           wdata  : 1'b1,
                           opcode : ET_LINK_RSP_MsgRcvData,
                           size   : q1_req_info.size,
                           qwen   : q1_req_info.qwen
                          };
assign q1_rsp_info_err = '{id     : q1_req_info.id,
                           dest   : q1_req_info.source,
                           wdata  : 1'b0,
                           opcode : ET_LINK_RSP_Err,
                           size   : ET_LINK_Byte,
                           qwen   : 4'b0
                          };
assign q1_rsp_info_to_lb = q1_req_info.op_err ? q1_rsp_info_err : q1_rsp_info_msg;
assign q1_data_to_lb     = q1_req_data;
assign q1_valid_to_lb    = q1_valid & (q1_req_info.op == ETL2AXI_REQ_OP_Other);
// Send reads, writes and atomics to Q1 arbiter
assign q1_valid_to_arb = q1_valid & (q1_req_info.op != ETL2AXI_REQ_OP_Other);

// Insert requests from the Transaction Table with higher priority
arb_prio_data
#(
  .WIDTH       ( `ETL2AXI_TT_SIZE_L + 
                 $bits(etl2axi_req_info_t) +
                 `CPU_AXI_DATA_WIDTH         ),
  .NUM_CLIENTS ( 2                           )
)
q1_arb
(
  // Bidding
  .bid         ( {tt_valid,
                  q1_valid_to_arb}                                       ),
  .stall       ( ~q1_ready_to_q2                                         ),
  // Data inputs
  .data_in     ( {tt_pop_entry,tt_pop_req_info,tt_pop_req_data,
                  q1_req_tt_entry,q1_req_info,q1_req_data}               ),
  // Result
  .grant       ( {tt_pop,
                  q1_ready_to_arb}                                       ),
  .data_winner ( {q1_tt_entry_to_q2,q1_req_info_to_q2,q1_req_data_to_q2} )
);

assign q1_from_tt = tt_pop;

// Conflict checking
// Qualify conflict only if request is not coming from the Transaction Table
// (otherwise conflicts have been already resolved and do not have to be checked again)
assign q1_conflict_qual = q1_conflict & ~q1_from_tt;

assign q1_wdata_to_q2   = (q1_req_info_to_q2.op == ETL2AXI_REQ_OP_Write)
                       || ((q1_req_info_to_q2.op == ETL2AXI_REQ_OP_Atomic) && (q1_req_info_to_q2.exop == ETL2AXI_TT_AMOEXOP_Write));
assign q1_valid_to_q2   = tt_valid | q1_valid_to_arb;
assign q1_ready_to_q2   = q2_ready | ~q2_valid;
assign q1_acc_to_q2     = q1_valid_to_q2 & q1_ready_to_q2;

//-------------------------------------------------------------------
// Q2 stage
//-------------------------------------------------------------------
//         CLK    RST    EN                              DOUT              DIN             DEF
`RST_EN_FF(clock, reset, q1_ready_to_q2,                 q2_valid,         q1_valid_to_q2, 1'b0)
`EN_FF    (clock,        q1_acc_to_q2,                   q2_tt_entry,      q1_tt_entry_to_q2)
`EN_FF    (clock,        q1_acc_to_q2,                   q2_req_info,      q1_req_info_to_q2)
`EN_FF    (clock,        q1_acc_to_q2 && q1_wdata_to_q2, q2_req_data,      q1_req_data_to_q2)
`EN_FF    (clock,        q1_acc_to_q2,                   q2_conflict,      q1_conflict_qual)

// Conflict checking
assign q2_ready = q2_conflict ? 1'b1 : q2_ready_to_q3;
// Send to Transaction Table if there is a conflict
assign q2_valid_to_tt = q2_valid & q2_conflict;
// Send to Q3 stage if there is no conflict
assign q2_valid_to_q3 = q2_valid & ~q2_conflict;

// Requests going to Q3 stage
// Generate LOCK flag
assign q2_lock = q2_req_info.op == ETL2AXI_REQ_OP_Atomic; // Exclusive read access

// Distribute requests onto read and write channels appropriately
// - Write requests are sent to Write address and data channels simultaneously
// - For atomic requests:
//   * The exclusive read access is sent through the Read channel
//   * The exclusive write access is sent through the Write channels
assign q2_to_ar       = (q2_req_info.op == ETL2AXI_REQ_OP_Read)
                     || ((q2_req_info.op == ETL2AXI_REQ_OP_Atomic) && (q2_req_info.exop == ETL2AXI_TT_AMOEXOP_Read));
assign q2_ready_to_q3 = q2_to_ar ? q2_arready : q2_awready & q2_wready;

assign q2_arvalid = q2_valid_to_q3 & q2_to_ar;
assign q2_arready = q3_arready | ~q3_arvalid;
assign q2_aracc   = q2_arvalid & q2_arready;
assign q2_awvalid = q2_valid_to_q3 & ~q2_to_ar & q2_wready;
assign q2_awready = q3_awready | ~q3_awvalid;
assign q2_awacc   = q2_awvalid & q2_awready;
assign q2_wvalid  = q2_valid_to_q3 & ~q2_to_ar & q2_awready;
assign q2_wready  = q3_wready | ~q3_wvalid;
assign q2_wacc    = q2_wvalid & q2_wready;

//-------------------------------------------------------------------
// Q3 stage
//-------------------------------------------------------------------
// AXI read address channel
//         CLK    RST    EN          DOUT        DIN         DEF
`RST_EN_FF(clock, reset, q2_arready, q3_arvalid, q2_arvalid, 1'b0)
`EN_FF    (clock,        q2_aracc,   q3_arid,    q2_tt_entry)
`EN_FF    (clock,        q2_aracc,   q3_araddr,  q2_req_info.address)
`EN_FF    (clock,        q2_aracc,   q3_arsize,  q2_req_info.size) // AXI and ET-Link use the same size encoding
`EN_FF    (clock,        q2_aracc,   q3_arlock,  q2_lock)


assign axi_ARID    = (`CPU_AXI_ID_WIDTH)'(q3_arid);
assign axi_ARADDR  = q3_araddr;
assign axi_ARLEN   = '0;    // Burst length = 1
assign axi_ARSIZE  = q3_arsize;
assign axi_ARBURST = 2'b01; // Burst type = INCR
assign axi_ARLOCK  = q3_arlock;
assign axi_ARCACHE = '0;    // Memory type = Device Non-bufferable
assign axi_ARPROT  = '0;    // Access permissions = Data, secure, unprivileged
assign axi_ARVALID = q3_arvalid;

assign q3_arready = axi_ARREADY;

// AXI write address channel
//         CLK    RST    EN          DOUT        DIN         DEF
`RST_EN_FF(clock, reset, q2_awready, q3_awvalid, q2_awvalid, 1'b0)
`EN_FF    (clock,        q2_awacc,   q3_awid,    q2_tt_entry)
`EN_FF    (clock,        q2_awacc,   q3_awaddr,  q2_req_info.address)
`EN_FF    (clock,        q2_awacc,   q3_awsize,  q2_req_info.size) // AXI and ET-Link use the same size encoding
`EN_FF    (clock,        q2_awacc,   q3_awlock,  q2_lock)

assign axi_AWID    = (`CPU_AXI_ID_WIDTH)'(q3_awid);
assign axi_AWADDR  = q3_awaddr;
assign axi_AWLEN   = '0;    // Burst length = 1
assign axi_AWSIZE  = q3_awsize;
assign axi_AWBURST = 2'b01; // Burst type = INCR
assign axi_AWLOCK  = q3_awlock;
assign axi_AWCACHE = '0;    // Memory type = Device Non-bufferable
assign axi_AWPROT  = '0;    // Access permissions = Data, secure, unprivileged
assign axi_AWVALID = q3_awvalid;

assign q3_awready = axi_AWREADY;

// AXI write data channel
//         CLK    RST    EN         DOUT       DIN         DEF
`RST_EN_FF(clock, reset, q2_wready, q3_wvalid, q2_wvalid,  1'b0)
`EN_FF    (clock,        q2_wacc,   q3_wdata,  q2_req_data)
`EN_FF    (clock,        q2_wacc,   q3_wstrb,  q2_req_info.strb)

assign axi_WDATA  = q3_wdata;
assign axi_WSTRB  = q3_wstrb;
assign axi_WLAST  = 1'b1; // Burst length = 1
assign axi_WVALID = q3_wvalid;

assign q3_wready = axi_WREADY;

//===================================================================
// RESPONSE PIPELINE
//===================================================================

//-------------------------------------------------------------------
// P0 stage
//-------------------------------------------------------------------
// AXI read data channel
assign axi_RREADY = p1_rready | ~p1_rvalid;
assign p0_racc    = axi_RVALID & axi_RREADY;

// AXI write response channel
assign axi_BREADY = p1_bready | ~p1_bvalid;
assign p0_bacc    = axi_BVALID & axi_BREADY;

//-------------------------------------------------------------------
// P1 stage
//-------------------------------------------------------------------
// AXI read data channel
//         CLK    RST    EN          DOUT       DIN         DEF
`RST_EN_FF(clock, reset, axi_RREADY, p1_rvalid, axi_RVALID, 1'b0)
`EN_FF    (clock,        p0_racc,    p1_rid,    axi_RID)
`EN_FF    (clock,        p0_racc,    p1_rdata,  axi_RDATA)
`EN_FF    (clock,        p0_racc,    p1_rresp,  cpu_axi_resp_t'(axi_RRESP))

// AXI write response channel
//         CLK    RST    EN          DOUT       DIN         DEF
`RST_EN_FF(clock, reset, axi_BREADY, p1_bvalid, axi_BVALID, 1'b0)
`EN_FF    (clock,        p0_bacc,    p1_bid,    axi_BID)
`EN_FF    (clock,        p0_bacc,    p1_bresp,  cpu_axi_resp_t'(axi_BRESP))

assign p1_rready = p1_rgrant & p1_ready;
assign p1_bready = p1_bgrant & p1_ready;

// Arbitrate between read and write responses
// Arbitration is done at P1 stage to avoid combinatorial paths between AXI
// xVALID and xREADY SIGNALS
arb_rr_data
#(
  .NUM_REQS ( 2                   ),
  .WIDTH    ( `CPU_AXI_ID_WIDTH + 
              `CPU_AXI_RESP_WIDTH )
)
p1_arb
(
  .clock    ( clock                   ),
  .reset    ( reset                   ),
  .grants   ( {p1_rgrant,
               p1_bgrant}             ),
  .pop      ( p1_ready                ),
  .reqs     ( {p1_rvalid,
               p1_bvalid}             ),
  .data_in  ( {p1_rid,p1_rresp,
               p1_bid,p1_bresp}       ),
  .data_out ( {p1_axi_id,p1_axi_resp} )
);

assign p1_valid = p1_rvalid | p1_bvalid;

// Get Transaction Table entry ID and retrieve information from Transaction Table
assign p1_tt_entry          = (`ETL2AXI_TT_SIZE_L)'(p1_axi_id);
assign p1_is_atomic_rd_exok = p1_rgrant && (p1_req_info.op == ETL2AXI_REQ_OP_Atomic) && (p1_rresp == axi_RESP_EXOKAY);
assign p1_is_atomic_rd_ok   = p1_rgrant && (p1_req_info.op == ETL2AXI_REQ_OP_Atomic) && (p1_rresp == axi_RESP_OKAY);
assign p1_is_atomic_wr_ok   = p1_bgrant && (p1_req_info.op == ETL2AXI_REQ_OP_Atomic) && (p1_bresp == axi_RESP_OKAY);

// Send operation to atomic port if either:
// - It is the response to an atomic's exclusive read
//   (and the slave notified that it supports exclusive accesses)
// - It is the response to a failed atomic's exclusive write
//   (and the maximum number of retries has not been reached yet)
assign p1_valid_pre_to_atomic = p1_valid | axi_RVALID | axi_BVALID;
assign p1_valid_to_atomic     = p1_is_atomic_rd_exok | (p1_is_atomic_wr_ok && (p1_at_retry_cnt != '1));
// Send other operations to P2 stage:
// - Read responses
// - Write responses
// - Error responses
// - Responses to failed atomic's exclusive reads
// - Responses to successful atomic's exclusive writes
// - Responses to failed atomic's exclusive writes if the maximum number of retries has been reached
assign p1_valid_to_p2         = p1_valid & ~p1_valid_to_atomic;
assign p1_acc_to_p2           = p1_valid_to_p2 & p1_ready_to_p2;

assign p1_ready = p1_valid_to_atomic ? 1'b1 : p1_ready_to_p2;

// Deallocate entry
assign tt_dealloc_entry = p1_tt_entry;
assign tt_dealloc_valid = p1_acc_to_p2;

// Generate ET-Link response op. code
                   // Error responses
assign p1_opcode = (p1_axi_resp == axi_RESP_SLVERR) || (p1_axi_resp == axi_RESP_DECERR) ||
                   // Responses to failed atomic's exclusive reads
                   p1_is_atomic_rd_ok ||
                   // Responses to failed atomic's exclusive writes if the maximum number of retries has been reached
                   (p1_is_atomic_wr_ok && (p1_at_retry_cnt == '1))        ? ET_LINK_RSP_Err
                   // Write responses
                 : p1_bgrant && (p1_req_info.op != ETL2AXI_REQ_OP_Atomic) ? ET_LINK_RSP_Ack
                   // Read responses and responses to successful atomic's exclusive writes
                 :                                                          ET_LINK_RSP_AckData;
// Generate ET-Link response quadword enable for reads and atomics
always_comb
begin
  for (integer unsigned i = 0; i < `ET_LINK_QWEN_SIZE; i++)
    p1_rd_qwen[i] = |p1_req_info.strb[`CPU_AXI_STRB_WIDTH/`ET_LINK_QWEN_SIZE*i +: `CPU_AXI_STRB_WIDTH/`ET_LINK_QWEN_SIZE];
end

// Generate ET-Link response
assign p1_rsp_info = '{id     : p1_req_info.id,
                       dest   : p1_req_info.source,
                       wdata  : p1_opcode == ET_LINK_RSP_AckData,
                       opcode : p1_opcode,
                       size   : p1_req_info.size,
                       qwen   : (p1_opcode == ET_LINK_RSP_AckData) ? p1_rd_qwen : '0
                      };
// For atomics, take the original read cacheline
// Otherwise, data always comes from the read response
assign p1_data = (p1_req_info.op == ETL2AXI_REQ_OP_Atomic) ? p1_at_rd_data : p1_rdata; // AXI and ET-Link use the same data width

// Insert error responses for requests with illegal op. codes and loopbacked messages
// with higher priority
arb_prio_data
#(
  .WIDTH       ( $bits(et_link_nodata_rsp_info_t)
                 + `ET_LINE_DATA_SIZE             ),
  .NUM_CLIENTS ( 2                                )
)
p1_loopback_arb
(
    // Bidding
    .bid         ( {q1_valid_to_lb,
                    p1_valid_to_p2}                  ),
    .stall       ( ~p1_ready_xx                      ),
    // Data inputs
    .data_in     ( {q1_rsp_info_to_lb,q1_data_to_lb,
                    p1_rsp_info,p1_data}             ),
    // Result
    .grant       ( {q1_ready_to_lb,
                    p1_ready_to_p2}                  ),
    .data_winner ( {p1_rsp_info_xx,p1_data_xx}       )
);

assign p1_valid_xx = q1_valid_to_lb | p1_valid_to_p2;
assign p1_ready_xx = p2_ready | ~p2_valid;
assign p1_acc_xx   = p1_valid_xx & p1_ready_xx;

//-------------------------------------------------------------------
// P2 stage
//-------------------------------------------------------------------
//         CLK    RST    EN                                 DOUT         DIN          DEF
`RST_EN_FF(clock, reset, p1_ready_xx,                       p2_valid,    p1_valid_xx, 1'b0)
`EN_FF    (clock,        p1_acc_xx,                         p2_rsp_info, p1_rsp_info_xx)
`EN_FF    (clock,        p1_acc_xx && p1_rsp_info_xx.wdata, p2_data,     p1_data_xx)

assign p2_ready = etl_rsp_ready;

// ET-Link response output
assign etl_rsp_info  = '{id     : p2_rsp_info.id,
                         dest   : p2_rsp_info.dest,
                         wdata  : p2_rsp_info.wdata,
                         opcode : p2_rsp_info.opcode,
                         data   : p2_data,
                         size   : p2_rsp_info.size,
                         qwen   : p2_rsp_info.qwen};
assign etl_rsp_valid = p2_valid;

//===================================================================
// TRANSACTION TABLES
//===================================================================
cpu_etl2axi_trans_table
trans_table
(
  // System signals
  .clock                ( clock                  ),
  .reset                ( reset                  ),
  // Allocate port
  .alloc_req_info       ( tt_alloc_req_info      ),
  .alloc_req_data       ( tt_alloc_req_data      ),
  .alloc_valid          ( tt_alloc_valid         ),
  .alloc_entry          ( tt_alloc_entry         ),
  .alloc_full           ( tt_alloc_full          ),
  // Deallocate port    
  .dealloc_entry        ( tt_dealloc_entry       ),
  .dealloc_valid        ( tt_dealloc_valid       ),
  // Read port
  .rd_entry             ( p1_tt_entry            ),
  .rd_req_info          ( p1_req_info            ),
  .rd_at_rd_data        ( p1_at_rd_data          ),
  .rd_at_retry_cnt      ( p1_at_retry_cnt        ),
  // Conflict checking port
  .chk_entry            ( q1_tt_entry_to_q2      ),
  .chk_req_info         ( q1_req_info_to_q2      ),
  .chk_valid            ( q1_acc_to_q2           ),
  .chk_conflict         ( q1_conflict            ),
  // Push port
  .push_entry           ( q2_tt_entry            ),
  .push                 ( q2_valid_to_tt         ),
  // Atomic response port
  .atomic_rsp_entry     ( p1_tt_entry            ),
  .atomic_rsp_is_rd     ( p1_is_atomic_rd_exok   ),
  .atomic_rsp_data      ( p1_rdata               ),
  .atomic_rsp_valid_pre ( p1_valid_pre_to_atomic ),
  .atomic_rsp_valid     ( p1_valid_to_atomic     ),
  // Pop port
  .pop_entry            ( tt_pop_entry           ),
  .pop_req              ( tt_pop_req_info        ),
  .pop_data             ( tt_pop_req_data        ),
  .valid                ( tt_valid               ),
  .pop                  ( tt_pop                 )
);

//===================================================================
// FUNCTIONS
//===================================================================
function automatic logic [`CPU_AXI_STRB_WIDTH-1:0] get_strb;
  input et_link_size_t                   size;
  input logic [`CPU_AXI_ADDR_OFFSET-1:0] addr_offset;

  begin
    get_strb = ( size == ET_LINK_Byte  ) ? 64'h0000000000000001
             : ( size == ET_LINK_HWord ) ? 64'h0000000000000003
             : ( size == ET_LINK_Word  ) ? 64'h000000000000000f
             : ( size == ET_LINK_DWord ) ? 64'h00000000000000ff
             : ( size == ET_LINK_QWord ) ? 64'h000000000000ffff
             : ( size == ET_LINK_HLine ) ? 64'h00000000ffffffff
             :                             64'hffffffffffffffff;
    get_strb = get_strb << addr_offset;
  end
endfunction

//===================================================================
// LINT: Unused signals
//===================================================================
wire unused_ok = &{
`ifndef ET_ASCENT_LINT
   1'b0,
`endif
   axi_RLAST,
   etl_req_info_int.wdata,
   q2_req_info.op_err,
   q2_req_info.qwen,
   q2_req_info.source,
   q2_req_info.id,
   p1_req_info.amo_conf,
   p1_req_info.address
};

endmodule
