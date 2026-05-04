// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

//===================================================================
// AXI
//===================================================================
// Enum of AXI response
typedef enum logic [1:0] {
  axi_RESP_OKAY   = 2'd0,
  axi_RESP_EXOKAY = 2'd1,
  axi_RESP_SLVERR = 2'd2,
  axi_RESP_DECERR = 2'd3
} cpu_axi_resp_t;

//===================================================================
// ETL2AXI BRIDGE
//===================================================================
// Enum of request operations
typedef enum logic [1:0] {
  ETL2AXI_REQ_OP_Write  = 2'd0,
  ETL2AXI_REQ_OP_Read   = 2'd1,
  ETL2AXI_REQ_OP_Atomic = 2'd2,
  ETL2AXI_REQ_OP_Other  = 2'd3
} etl2axi_req_opcode_t;

// Enum of Transaction Table request states
typedef enum logic [1:0] {
  ETL2AXI_TT_ST_Pending          = 2'd0,
  ETL2AXI_TT_ST_Conflict         = 2'd1,
  ETL2AXI_TT_ST_Ready            = 2'd2,
  ETL2AXI_TT_ST_AtomicWriteReady = 2'd3
} etl2axi_tt_state_t;

// Enum of Atomic's exclusive operations
typedef enum logic {
  ETL2AXI_TT_AMOEXOP_Read = 1'b0,
  ETL2AXI_TT_AMOEXOP_Write = 1'b1
} etl2axi_tt_amoexop_t;

// Enum of Atomic sizes
typedef enum logic {
  ETL2AXI_TT_AMOSIZE_SWord = 1'b0,
  ETL2AXI_TT_AMOSIZE_DWord = 1'b1
} etl2axi_tt_amosize_t;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

//===================================================================
// ETL2AXI BRIDGE
//===================================================================
// ETL2AXI bridge internal request
typedef struct packed {
  logic [`ET_LINK_ID_SIZE-1:0]    id;      // id
  et_link_neigh_source_t          source;  // Source
  etl2axi_req_opcode_t            op;      // Request internal Op. code
  logic [`CPU_AXI_ADDR_WIDTH-1:0] address; // Address of the request
  et_link_size_t                  size;    // Size of the transaction
  logic [`CPU_AXI_STRB_WIDTH-1:0] strb;    // Byte Enable
  logic [`ET_LINK_QWEN_SIZE-1:0]  qwen;    // QWord Enable
  etl2axi_tt_amoexop_t            exop;
  logic                           op_err;
} etl2axi_req_info_t;

// Atomic configuration
typedef struct packed {
  et_link_amo_opcode   op;
  etl2axi_tt_amosize_t size;
} etl2axi_amo_conf_t;

// ETL2AXI Transaction Table request
typedef struct packed {
  logic [`ET_LINK_ID_SIZE-1:0]    id;       // id
  et_link_neigh_source_t          source;   // Source
  etl2axi_req_opcode_t            op;       // Request internal Op. code
  logic [`CPU_AXI_ADDR_WIDTH-1:0] address;  // Address of the request
  et_link_size_t                  size;     // Size of the transaction
  logic [`CPU_AXI_STRB_WIDTH-1:0] strb;     // Byte Enable
  etl2axi_amo_conf_t              amo_conf; // Atomic configuration
} etl2axi_tt_req_info_t;

// Transaction Table entry
typedef struct packed {
  etl2axi_tt_req_info_t                            req;
  logic [1:0][`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0] data_entry;
} etl2axi_tt_entry_t;

//===================================================================
// APB MUX
//===================================================================
typedef struct packed {
    logic    [`CPU_APB_ADDR_WIDTH-1:0] apb_paddr;     // Address of the request
    logic                              apb_penable;   // Enable
    logic                              apb_psel;      // Select
    logic    [`CPU_APB_DATA_WIDTH-1:0] apb_pwdata;    // Write data
    logic                              apb_pwrite;    // Write enable
} APB_to_cpu_t;

typedef struct packed {
    logic    [`CPU_APB_DATA_WIDTH-1:0] apb_prdata;    // Read data
    logic                              apb_pready;    // Ready
    logic                              apb_pslverr;   // Slave error
} APB_from_cpu_t;
