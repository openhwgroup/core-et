// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

// Enum of request operations
typedef enum logic [4:0] {
   ET_LINK_REQ_Write           = 5'd0,
   ET_LINK_REQ_Read            = 5'd1,
   ET_LINK_REQ_WriteAround     = 5'd2,
   ET_LINK_REQ_ReadCoop        = 5'd3, // Cooperative Read
   ET_LINK_REQ_MsgSendData     = 5'd4, // Message send
   ET_LINK_REQ_Atomic          = 5'd5,
   ET_LINK_REQ_ScpRead         = 5'd6, // Scratch Pad Read  (Unused)
   ET_LINK_REQ_ScpWrite        = 5'd7, // Scratch Pad Write (Unused)
   ET_LINK_REQ_Flush           = 5'd8,
   ET_LINK_REQ_FlushToMem      = 5'd9, // Flush with dirty data from lower cache level
   ET_LINK_REQ_Evict           = 5'd10,
   ET_LINK_REQ_EvictToMem      = 5'd11,// Evict with dirty data from lower cache level
   ET_LINK_REQ_Lock            = 5'd12,
   ET_LINK_REQ_Unlock          = 5'd13,
   ET_LINK_REQ_ScpFill         = 5'd14,// Scratch Pad Fill
   ET_LINK_REQ_Prefetch        = 5'd15,
   ET_LINK_REQ_AtomicRsp       = 5'd16 // Only seen on AXI atomic response path
} et_link_req_opcode_t;

// Enum of WriteAround suboperations
typedef enum logic [6:0] {
   ET_LINK_WriteAround     = 7'd0,
   ET_LINK_WriteAround2Way = 7'd1,
   ET_LINK_WriteAround4Way = 7'd2
} et_link_req_WriteAround_subopcode_t;

// Enum of ET link response operations
typedef enum logic [1:0] {
   ET_LINK_RSP_Ack         = 2'd0,
   ET_LINK_RSP_AckData     = 2'd1,
   ET_LINK_RSP_MsgRcvData  = 2'd2,
   ET_LINK_RSP_Err         = 2'd3
} et_link_rsp_opcode_t;

// source for neighborhood ET link requests and responses
typedef enum logic [3:0] {
   ET_LINK_NEIGH_SRC_Minion0 = 4'd0,
   ET_LINK_NEIGH_SRC_Minion1 = 4'd1,
   ET_LINK_NEIGH_SRC_Minion2 = 4'd2,
   ET_LINK_NEIGH_SRC_Minion3 = 4'd3,
   ET_LINK_NEIGH_SRC_Minion4 = 4'd4,
   ET_LINK_NEIGH_SRC_Minion5 = 4'd5,
   ET_LINK_NEIGH_SRC_Minion6 = 4'd6,
   ET_LINK_NEIGH_SRC_Minion7 = 4'd7,
   ET_LINK_NEIGH_SRC_Icache  = 4'd8,
   ET_LINK_NEIGH_SRC_Ptw0    = 4'd9,
   ET_LINK_NEIGH_SRC_Ptw1    = 4'd10,
   ET_LINK_NEIGH_SRC_Tbox    = 4'd11,
   ET_LINK_NEIGH_SRC_Coop    = 4'd12
} et_link_neigh_source_t;

// size for ET_Link responses and some ET_Link requests
typedef enum logic [2:0] {
   ET_LINK_Byte            = 3'b000, // 8b
   ET_LINK_HWord           = 3'b001, // 16b
   ET_LINK_Word            = 3'b010, // 32b
   ET_LINK_DWord           = 3'b011, // 64b
   ET_LINK_QWord           = 3'b100, // 128b
   ET_LINK_HLine           = 3'b101, // 256b
   ET_LINK_Line            = 3'b110, // 512b
   ET_LINK_IllegalSize     = 3'b111  // Unused
} et_link_size_t;

// Which cache to direct a cacheable write to
typedef enum logic {
   ET_LINK_Write_L2       = 1'b0, // Write to L2 or L2 SCP
   ET_LINK_Write_L3       = 1'b1  // Skip L2 and write to L3
} et_link_write_cache_t;

// Which cache to direct a cacheable read to
typedef enum logic {
   ET_LINK_Read_L2        = 1'b0, // Read from L2 or L2 SCP
   ET_LINK_Read_L3        = 1'b1  // Skip L2 and read from L3
} et_link_read_cache_t;

// ID field for misses coming from core
typedef enum logic [4:0]
{
    ET_LINK_Core_Miss_Id_Reserved             = 5'b00000, // All 0's case is reserved
    ET_LINK_Core_Miss_Id_Tensor_Store_Coop    = 5'b00001, // Tensor Store Cooperative
    ET_LINK_Core_Miss_Id_Miss_Handler_0       = 5'b00010, // Miss Handler 0
    ET_LINK_Core_Miss_Id_Miss_Handler_1       = 5'b00011, // Miss Handler 1

    ET_LINK_Core_Miss_Id_Tbox                 = 5'b00100, // Tbox interface
    ET_LINK_Core_Miss_Id_Reduce               = 5'b00101, // Reduce
    ET_LINK_Core_Miss_Id_Cache_Ops            = 5'b00110, // Cache Ops
    ET_LINK_Core_Miss_Id_WriteBack            = 5'b00111, // WriteBack

    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0 = 5'b01000, // Miss Handler for UC Load accesses 0
    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1 = 5'b01001, // Miss Handler for UC Load accesses 1
    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0 = 5'b01010, // Miss Handler for UC Store accesses 0
    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1 = 5'b01011, // Miss Handler for UC Store accesses 1

    ET_LINK_Core_Miss_Id_L2_Prefetch_0        = 5'b01100, // L2 Prefetcher 0
    ET_LINK_Core_Miss_Id_L2_Prefetch_1        = 5'b01101, // L2 Prefetcher 1
    ET_LINK_Core_Miss_Id_L2_Prefetch_2        = 5'b01110, // L2 Prefetcher 2
    ET_LINK_Core_Miss_Id_L2_Prefetch_3        = 5'b01111, // L2 Prefetcher 3

    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0    = 5'b10000, // Tensor Load Extension 0
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_1    = 5'b10001, // Tensor Load Extension 1
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_2    = 5'b10010, // Tensor Load Extension 2
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_3    = 5'b10011, // Tensor Load Extension 3

    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_4    = 5'b10100, // Tensor Load Extension 4
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_5    = 5'b10101, // Tensor Load Extension 5
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_6    = 5'b10110, // Tensor Load Extension 6
    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_7    = 5'b10111  // Tensor Load Extension 7
} et_link_core_miss_id_t;

// ID field for messages going to minion
typedef enum logic [7:0]
{
    ET_LINK_Min_Msg_Id_Regular_Port    = 8'b00000000, // Regular port message
    ET_LINK_Min_Msg_Id_Tbox_Reply_Port = 8'b10000000, // Tbox reply port message
    ET_LINK_Min_Msg_Id_Reduce_Ready    = 8'b11000000, // Reduce Ready message
    ET_LINK_Min_Msg_Id_Reduce_Data     = 8'b11000100, // Reduce Data message
    ET_LINK_Min_Msg_Id_Tbox_Pull       = 8'b11001000  // Tbox pull message
} et_link_min_msg_id_t;

typedef enum logic [3:0]
{
    ET_LINK_AMO_Swap     = 4'b0000,
    ET_LINK_AMO_Add      = 4'b0001,
    ET_LINK_AMO_Xor      = 4'b0010,
    ET_LINK_AMO_And      = 4'b0011,
    ET_LINK_AMO_Or       = 4'b0100,
    ET_LINK_AMO_Min      = 4'b0101,
    ET_LINK_AMO_Max      = 4'b0110,
    ET_LINK_AMO_MinU     = 4'b0111,
    ET_LINK_AMO_MaxU     = 4'b1000,
    ET_LINK_AMO_Min_PS   = 4'b1001,
    ET_LINK_AMO_Max_PS   = 4'b1010,
    ET_LINK_AMO_Cmp_Swap = 4'b1011
} et_link_amo_opcode;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

//
// request info type definition
//
`ifndef ET_LINK_REQ_INFO_TYPE_DEF
`define ET_LINK_REQ_INFO_TYPE_DEF(ET_LINK_ID_SIZE, ET_LINK_SOURCE_SIZE, DATA_SIZE) \
struct packed { \
   logic [ET_LINK_ID_SIZE-1:0]         id;        // id \
   logic [ET_LINK_SOURCE_SIZE-1:0]     source;    // Source \
   logic                               wdata;     // Request carries data \
   et_link_req_opcode_t                opcode;    // Request Opcode \
   logic [`PA_SIZE-1:0]                address;   // Address of the request \
   logic [DATA_SIZE-1:0]               data;      // Write data - variable width per interface \
   et_link_size_t                      size;      // Size of the transaction \
   logic [`ET_LINK_QWEN_SIZE-1:0]      qwen;      // QWord Enable \
   logic [`ET_LINK_SUBOPCODE_SIZE-1:0] subopcode; // Opcode specific field \
}
`endif

typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_LINK_ID_SIZE     , `ET_LINK_SOURCE_SIZE, `ET_LINE_DATA_SIZE      ) et_link_req_info_t;
typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_CORE_MISS_ID_SIZE, 1                   , `SHIRE_COOP_DATA_SIZE   ) et_link_minion_miss_req_info_t;
typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_CORE_MISS_ID_SIZE, 1                   , `CORE_L2_BLOCK_EXT_SIZE ) et_link_minion_evict_req_info_t;
typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_LINK_ID_SIZE     , `ET_LINK_SOURCE_SIZE, `NEIGH_DATA_SIZE        ) et_link_neigh_req_info_t;
typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_LINK_ID_SIZE     , `ET_LINK_SOURCE_SIZE, `UC_DATA_SIZE           ) et_link_uc_req_info_t;


//
// request info without data type definition
//
`ifndef ET_LINK_NODATA_REQ_INFO_TYPE_DEF
`define ET_LINK_NODATA_REQ_INFO_TYPE_DEF(ET_LINK_ID_SIZE, ET_LINK_SOURCE_SIZE) \
struct packed { \
   logic [ET_LINK_ID_SIZE-1:0]         id;        // id \
   logic [ET_LINK_SOURCE_SIZE-1:0]     source;    // Source \
   logic                               wdata;     // Request carries data \
   et_link_req_opcode_t                opcode;    // Request Opcode \
   logic [`PA_SIZE-1:0]                address;   // Address of the request \
   et_link_size_t                      size;      // Size of the transaction \
   logic [`ET_LINK_QWEN_SIZE-1:0]      qwen;      // QWord Enable \
   logic [`ET_LINK_SUBOPCODE_SIZE-1:0] subopcode; // Opcode specific field \
   //logic                               cached;    // 0: uncached, 1: cached \
}
`endif

typedef `ET_LINK_NODATA_REQ_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, `ET_LINK_SOURCE_SIZE ) et_link_nodata_req_info_t;

//
// response info type definition
//
`ifndef ET_LINK_RSP_INFO_TYPE_DEF
`define ET_LINK_RSP_INFO_TYPE_DEF(ET_LINK_ID_SIZE, ET_LINK_DEST_SIZE, DATA_SIZE) \
struct packed { \
   logic [ET_LINK_ID_SIZE-1:0]    id;     // id \
   logic [ET_LINK_DEST_SIZE-1:0]  dest;   // Destination \
   logic                          wdata;  // Response carries data \
   et_link_rsp_opcode_t           opcode; // Response Opcode \
   logic [DATA_SIZE-1:0]          data;   // Read data \
   et_link_size_t                 size;   // Size of the transaction \
   logic [`ET_LINK_QWEN_SIZE-1:0] qwen;   // QWord Enable \
}
`endif

typedef `ET_LINK_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, `ET_LINK_DEST_SIZE, `ET_LINE_DATA_SIZE      ) et_link_rsp_info_t;

typedef `ET_LINK_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, 1                 , `CORE_L2_BLOCK_EXT_SIZE ) et_link_minion_rsp_info_t;
typedef `ET_LINK_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, `ET_LINK_DEST_SIZE, `NEIGH_DATA_SIZE        ) et_link_neigh_rsp_info_t;
typedef `ET_LINK_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, `ET_LINK_DEST_SIZE, `UC_DATA_SIZE           ) et_link_uc_rsp_info_t;

`ifndef ET_LINK_NODATA_RSP_INFO_TYPE_DEF
`define ET_LINK_NODATA_RSP_INFO_TYPE_DEF(ET_LINK_ID_SIZE, ET_LINK_DEST_SIZE) \
struct packed { \
   logic [ET_LINK_ID_SIZE-1:0]    id;     // id \
   logic [ET_LINK_DEST_SIZE-1:0]  dest;   // Destination \
   logic                          wdata;  // Response carries data \
   et_link_rsp_opcode_t           opcode; // Response Opcode \
   et_link_size_t                 size;   // Size of the transaction \
   logic [`ET_LINK_QWEN_SIZE-1:0] qwen;   // QWord Enable \
}
`endif //  `ifndef ET_LINK_NODATA_RSP_INFO_TYPE_DEF

typedef `ET_LINK_NODATA_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, `ET_LINK_DEST_SIZE  ) et_link_nodata_rsp_info_t;
typedef `ET_LINK_NODATA_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE, 1                   ) et_link_minion_nodata_rsp_info_t;

//
// Tbox id type definition
//
typedef struct packed
{
    logic [`TBOX_PER_SHIRE_R]              tbox_nr;  // tbox number
    logic                                  img_data; // 0 for img information table, 1 for data
    logic [`TBOX_L2_FIFO_DEPTH_IDX_SZ-1:0] data_id;  // request number (used for data)
} et_link_tbox_id_t;

//
// Neighborhood message dest type definition
//
typedef struct packed
{
    logic [`MIN_PER_N_R]       min_id;
    logic [`CORE_NR_THREADS_R] thread_id;
} et_link_neigh_msg_dest_t;
