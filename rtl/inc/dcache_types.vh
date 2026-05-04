// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////


// Line status
typedef enum logic [1:0]
{
    State_Invalid   = 2'b00,
    State_Shared    = 2'b01,
    State_Exclusive = 2'b10,
    State_Modified  = 2'b11
} dcache_line_state;

// Snoop requests
typedef enum logic [1:0]
{
    Snoop_to_Exclusive = 2'b00,
    Snoop_to_Shared    = 2'b01,
    Snoop_to_Invalid   = 2'b10
} snoop_request;

// Grow types
/*typedef enum logic [2:0]
{
    Grow_I_to_S = 3'b000,
    Grow_I_to_E = 3'b001,
    Grow_S_to_E = 3'b010
} dcache_grow_cmd;*/

// DCache access type
typedef enum logic [1:0]
{
    dcache_access_rd = 2'b00, // Read only
    dcache_access_wi = 2'b01, // Write intent
    dcache_access_wr = 2'b11  // Write
} dcache_access_type;

// Miss Handler FSM state
typedef enum logic [3:0]
{
    MH_Invalid         = 4'b0000, // Miss handler is available
    MH_Acquire_Wb      = 4'b0001, // Miss handler is available
    MH_Fill_Req        = 4'b0010, // Mem acquire
    MH_Fill_Resp       = 4'b0011, // Waiting for the fill to finish
    MH_Meta_Write_Req  = 4'b0100, // Updates the meta state to the fill one
    MH_Meta_Hazard     = 4'b0101, // Hazard after meta write
    MH_UC_Wait_Idle    = 4'b0110, // Waits until other UCs are empty before starting a UC load
    MH_UC_Load_Req     = 4'b0111, // Requests the UC load data
    MH_UC_Load_Resp    = 4'b1000, // Waits for UC load data simply to keep the UC as outstanding
    MH_UC_Store_Wait   = 4'b1001, // Sends the store UC request to the L2
    MH_UC_Store_Req    = 4'b1010, // Sends the store UC request to the L2
    MH_UC_Store_Ack    = 4'b1011, // Waits for the L2 ack
    MH_Fill_Clean      = 4'b1100  // Cleans the content of metafata if fill gets and error
} dcache_mh_state;

// DCache read request size
typedef enum logic [1:0]
{
    Size_64b  = 2'b00, // Read 64 bits
    Size_128b = 2'b01, // Read 128 bits
    Size_256b = 2'b10, // Read 256 bits
    Size_rsvd = 2'b11  // Reserved
} dcache_read_req_size;

// Cache Op operations
typedef enum logic [3:0]
{
    cache_op_Lock_VA   = 4'b0000, // Locks a specified Virtual address
    cache_op_Unlock_VA = 4'b0001, // Unlocks a specified Virtual address
    cache_op_Flush_SW  = 4'b0010, // Flushes a Set/Way of the cache hierarchy
    cache_op_Evict_SW  = 4'b0011, // Evicts a Set/Way of the cache hierarchy
    cache_op_Pref      = 4'b0100, // Prefetching
    cache_op_Snoop     = 4'b0101, // Snoops: for internal use only
    cache_op_Flush_VA  = 4'b0110, // Flushes a VA of the cache hierarchy
    cache_op_Evict_VA  = 4'b0111, // Evicts a VA of the cache hierarchy
    cache_op_Lock_SW   = 4'b1000, // Locks a Physical Address in a given Set/Way
    cache_op_Unlock_SW = 4'b1001  // Unlocks a Physical Address in a given Set/Way
} CacheOpControl_t;

// Cache Op Level
typedef enum logic [1:0]
{
  cache_op_L1  = 2'b00,  // Level 1
  cache_op_L2  = 2'b01,  // Level 2
  cache_op_L3  = 2'b10,  // Level 3
  cache_op_MEM = 2'b11   // Memory

} CacheOpLevel_t;

typedef enum  logic [2:0]
{
    TensorTrans_None  = 3'b000, // No transformation
    TensorTrans_Int8  = 3'b001, // Interleave 8 bits
    TensorTrans_Int16 = 3'b010, // Interleave 16 bits
    TensorTrans_Trp8  = 3'b101, // Transpose 8 bits
    TensorTrans_Trp16 = 3'b110, // Transpose 16 bits
    TensorTrans_Trp32 = 3'b111  // Transpose 32 bits
} tensorload_transform;

typedef enum logic [1:0]
{
  TransOffset0  = 2'b00,
  TransOffset16 = 2'b01,
  TransOffset32 = 2'b10,
  TransOffset48 = 2'b11
} tensorload_offset;

// Dcache operation mode
typedef enum logic [1:0]
{
  DcacheMode_Shared     = 2'b00,
  DcacheMode_Split      = 2'b01,
  DcacheMode_Invalid    = 2'b10,
  DcacheMode_Scratchpad = 2'b11
} dcache_mode_t;

typedef enum logic [2:0]
{
  dcache_s1_da_mux_sel_core = 3'd0, // Core reading
  dcache_s1_da_mux_sel_wb   = 3'd1, // WriteBack reading
  dcache_s1_da_mux_sel_vpu  = 3'd2, // VPU reading SCP
  dcache_s1_da_mux_sel_ts   = 3'd3, // TensorStore reading SCP
  dcache_s1_da_mux_sel_dbg  = 3'd4  // Debug reading
  
} dcache_s1_da_mux_sel_t;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

//// BTB update structure
//typedef struct packed
//{
//    frontend_btb_resp prediction; // Original prediction
//    logic [39:0]    pc;         // PC of the instruction
//    logic [39:0]    target;     // Target of the instruction
//    logic           taken;      // The jump is taken
//    logic           valid;      // If it is valid
//    logic           jump;       // Is a jump
//    logic           return;     // Is a return
//    logic [39:0]    br_pc;      // PC of branch
//} minion_btb;


// Writeback Request struct
typedef struct packed
{
    logic [`PA_TAG_EXT_RANGE]   tag;       // Physical tag (extended)
    logic [`DCACHE_SET_RANGE]   set;       // Set
    logic [`DCACHE_WAYS-1:0]    way_en;    // Way
    logic                       is_tbox;   // Flag to indicate that request is to send data to Tbox
    logic                       mem_idx;   // Part of the cacheline to be accessed (for Tbox)
    logic [`PA_NOTAG_EXT_RANGE] addr_low;  // Part of the address not in TAG (for Tbox)
} dcache_wb_req;

// Data read request
typedef struct packed
{
    logic                           valid_l; // Read data for low half
    logic                           valid_h; // Read data for high half
    logic [`DCACHE_WAY_RANGE]       way;     // Which ways to read from
    logic [`DCACHE_LRAM_ADDR_RANGE] addr;    // Which block to read from
} dcache_da_read_req;

// Data read request data
typedef logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_LRAM_DATA_RANGE] dcache_da_data;

// Data write request
typedef struct packed
{
    logic                           valid_l; // Write data in lower half word
    logic                           valid_h; // Write data in upper half word
    logic [`DCACHE_WAY_RANGE]       way;   // Which ways to write to
    logic [`DCACHE_LRAM_ADDR_RANGE] addr;  // Which block to write to
    logic [`DCACHE_LRAM_DATA_RANGE] data;  // Data to write
} dcache_da_write_req;

// Meta contents for a cacheline
typedef struct packed
{
    dcache_line_state         coh_state; // MESI state
    logic [`PA_TAG_EXT_RANGE] tag;       // Tag of the cacheline (with extended bits)
} dcache_meta_data;

// Meta Write Request struct
typedef struct packed
{
    logic [`DCACHE_SET_RANGE] set;
    logic [`DCACHE_WAYS-1:0]  way_en;
    dcache_meta_data          data;
} dcache_meta_write_req;

// Miss handler inflight info
typedef struct packed
{
    logic                     valid;             // If the miss is in-flight
    logic [`DCACHE_WAYS-1:0]  way_en;            // Way for the miss
    logic [`DCACHE_SET_RANGE] set;               // Set of the miss
    //logic                     writeback_pending; // The miss handler has a pending writeback
} dcache_miss_info;

// Dcache exceptions in TAG stage
typedef struct packed
{
    logic pf_ld; // Page fault load
    logic pf_st; // Page fault store
    logic mf_ld; // Misaligned fault load
    logic mf_st; // Misaligned fault store
    logic af_ld; // Access fault load
    logic af_st; // Access fault store
    logic ps_ld; // Page-split fault load
    logic ps_st; // Page-split fault store
} dcache_tag_xcpt;

// Instruction replay problems
typedef struct packed
{
    logic                            vpu_rf_write;  // VPU RF write port is busy, cannot access it
    logic                            miss_alloc;    // Couldn't alloc a miss handler
    logic [`DCACHE_MH_FILE_SIZE-1:0] miss_accepted; // Miss handler that accepted the miss request, if any
    logic [`DCACHE_MH_FILE_SIZE-1:0] miss_assigned; // Miss handler assigned to the entry in case of miss
} dcache_replay_conf;

// Regular dcache instruction request flowing through pipeline
typedef struct packed
{
    logic [`VA_RANGE]                     addr;           // Physical address of the request
    logic                                 addr_msb_err;   // bits addr[63:48] != addr[47]
    logic [`DCACHE_SET_RANGE]             set;            // Set associated to the address of this request
    logic                                 phys;           // Is the access physical
    logic                                 misaligned;     // Access is misaligned (used for block check in replayQ)
    logic                                 misaligned_2nd; // Transaction is a second 2nd pass for a misaligned
    minion_reg_dest                       dest;           // Destination register for loads
    dcache_cmd                            cmd;            // Command coming from core
    dcache_type                           typ;            // Type 
    logic [`CORE_GSC_CNT_RANGE]           gsc_cnt;        // Gather/Scatter element
    logic [`DMEM_REQ_PS_MASK_SZ-1:0]      ps_mask;        // Mask for PS accesses
    logic [`DCACHE_DATA_RANGE]            data;           // Data for stores
    logic                                 replay;         // It comes from replay queue
    logic [`DCACHE_REPLAYQ_ADDR_RANGE]    replay_entry;   // Which replay entry request comes from
    logic [`DCACHE_REPLAYQ_AGE_ID]        replay_age;     // Age of the replay, to order different requests
    logic                                 cache_op;       // It comes from cache ops unit
    logic                                 buf_alloc;      // A buffer entry is allocated and needs to be released when transaction finishes
    logic [`DCACHE_BUFFER_ID_RANGE]       buf_id;         // Entry in buffer array
    logic                                 cacheable;      // If the request is cacheable
    logic [`DCACHE_DATA_NUM_DWORDS_RANGE] chunk_read;     // Set to 1 if the different parts of a block of data were read and therefore can be bypassed from a store to a load
    logic                                 uc_load_2nd;    // Second pass for a UC load, simply read the data from buffer array
    logic                                 uc_load_ok;     // UC load went well
    logic                                 miss_fill_ok;   // Miss Fill went well
    logic [`CORE_GSC32_IDX_V_RANGE]       gsc32_idx;      // Indices for gather/scatter on 32-byte blocks
    logic [`DCACHE_VM_SEL_RANGE]          vm_sel;         // Selects the VM status that has to be used for TLB translation
    logic                                 mem_global;     // Flag indicating if memory access is global or local
    prv_t                                 prv;            // Privilage level in which the operation was executed. Consumed by PMA on misalign second pas
} dcache_req;

// Cache operation request
typedef struct packed
{
    CacheOpControl_t                  cmd;         // Command for the cache control
    CacheOpLevel_t                    start_level; // Level that the cache op starts taking effects
    CacheOpLevel_t                    dest_level;  // Up to which level the cache op is effecting
    logic [3:0]                       count;       // Number of operations to do minus 1
    logic [`VA_RANGE]                 addr;        // Address for the cache control
    logic [`DCACHE_WAY_RANGE]         way;         // Way of the operation for Way ops
    logic [`VA_RANGE]                 stride;      // Stride between requests
    logic                             thread_id;   // Thread ID that request the cacheop
    logic                             use_tmask;   // Indication to use the tensor mask
    logic                             tmask_ready; // Indication that tmask is ready
    logic [`TENSOR_MASK_BITS-1:0]     tmask_bits;  // Tensor mask bits
    logic [`DCACHE_L2_SCP_DST_RANGE]  l2_scp_dest; // Scratchpad destination entry
    logic                             l2_scp_fill; // Operation is to fill L2 Scp
} core_dcache_op_req;

typedef struct packed
{
    CacheOpControl_t                  cmd;         // Command for the cache control
    CacheOpLevel_t                    start_level; // Level that the cache op starts taking effects
    CacheOpLevel_t                    dest_level;  // Up to which level the cache op is effecting
} core_dcache_op_req_next;

typedef struct packed
{
    logic                                     use_tmask;   // Use the TensorMask
    logic                                     use_coop;    // Use cooperation CSR
    tensorload_transform                      transform;   // Tensor load transformation
    logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]    scp_dest;    // Scratchpad destination entry
    logic                                     tl_sel;      // Selects TL module
    logic [`VA_RANGE_CA]                      addr;        // Cacheline aligned address
    tensorload_offset                         offset;      // Byte offset identifier 
    logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]  num_lines;   // Number of lines to prefetch minus 1
} tensorload_control;

typedef struct packed
{
    logic [7:0]  row_step_offset; // Amount of rows that one step advances (needed for dilution)
    logic [7:0]  padding2;        // CSR padding
    logic [15:0] row_size;        // Size in rows of the map
    logic [7:0]  col_step_offset; // Amount of cols that one step advances (needed for dilution)
    logic [7:0]  padding1;        // CSR padding
    logic [15:0] col_size;        // Size in cols of the map
} tensorconv_size;

typedef struct packed
{
    logic [15:0] row_pos;  // Row position in the map for 1st pass
    logic [15:0] padding1; // CSR padding
    logic [15:0] col_pos;  // Col position in the map for 1st pass
} tensorconv_control;

typedef struct packed
{
    logic [`SHIRE_COOP_NEIGH_MASK_SIZE-1:0] neigh_mask;   // Neigh Mask of the TensorLoad
    logic [`SHIRE_COOP_MIN_MASK_SIZE-1:0]   minion_mask;  // Minion Mask of the TensorLoad
    logic [2:0]                             padding;
    logic [`SHIRE_COOP_ID_RANGE]            id;           // ID of the TensorLoad
} tensorcoop_control;

typedef struct packed
{
  logic [`DCACHE_CO_L2_MAX_OPS_BITS-1:0]  cacheop_max;
  logic                                   padding;
  logic [`DCACHE_CO_L2_REPRATE_BITS-1:0]  cacheop_reprate;
  logic                                   scpenable;
  logic                                   d1split;
} cache_control;

typedef struct packed
{
   tensorload_control              tensorload_ctrl;         // TensorLoad control
   tensorcoop_control              tensorcoop_ctrl;         // TensorCooperation control
   logic [`VA_RANGE]               tensorload_stride;       // TensorLoad address stride
   logic                           tensorload_start;        // Start a new TensorLoad
   logic                           tensorload_restart_b;    // Re-Start a TensorLoadB
   logic                           tensorload_tmask_ready;  // TensorMask bits are ready and can be used
   logic [`TENSOR_MASK_BITS-1:0]   tensorload_tmask_bits;   // TensorMask bits
   logic                           cache_op_valid;          // Starts a new cache operation
   core_dcache_op_req              cache_op_req;            // Cache operation
   core_dcache_op_req_next         cache_op_req_next;       // Request information in previous cycle
   logic                           reduce_start;            // Starts a reduce operation
   logic                           tensorstore_start;       // Starts a TensorStore operation
   reduce_tensorstore_control      reduce_tensorstore_ctrl; // Information of the Reduce or TensorStore requests
   logic [`VA_RANGE]               tensorstore_stride;      // TensorStore address stride
   cache_control                   cache_ctrl;              // Cache and Scratchpad control
   logic                           thread_id;               // Thread Id
} core_dcache_ctrl;

// Struct containing the tensor subset of signals of core_dcache_ctrl
typedef struct packed
{
   tensorload_control            ctrl;
   tensorcoop_control            coop;
   logic [`VA_RANGE]             stride;
   logic                         start;
   logic                         restart_b;
   logic                         tmask_ready;
   logic [`TENSOR_MASK_BITS-1:0] tmask_bits;
   logic                         thread_id; 
} core_dcache_tensor_ctrl;

// Core control response from dcache
typedef struct packed
{
    logic                             cache_op_ready;       // Cache op request was accepted
    logic [`CORE_NR_THREADS-1:0]      cache_op_busy_changed;// state of the cache op unit has changed
    logic [`CORE_NR_THREADS-1:0][1:0] cache_op_complete_l1; // Cache operation for L1 is complete (one bit for every ID)
    logic [`CORE_NR_THREADS-1:0][1:0] cache_op_complete_l2; // Cache operation for L2 is complete (one bit for every ID)
    logic                             tl_ready_0;           // TensorLoad module 0 can accept a new tensor load instruction
    logic                             tl_ready_1;           // TensorLoad module 1 can accept a new tensor load instruction
    logic                             ts_ready;             // TensorStore is ready
    logic [1:0]                       tl_complete_0;        // TensorLoad module 0 operation is complete (one bit for every ID)
    logic                             reduce_ready;         // Reduce module can accept a new instruction
    logic                             tex_send_ready;       // TexSend module can accept a new instruction
    logic [`CORE_NR_THREADS-1:0]      gsc_progress_save;    // Save progress state for GSC operations
    logic [`CORE_GSC_CNT_RANGE]       gsc_progress;         // Progress for the failed GSC operation
} dcache_core_ctrl;

// Request to the TLB
typedef struct packed
{
    minion_vm_status        status;
    logic [`VA_TRANS_RANGE] vpn;         // Virtual Page Number
    logic                   passthrough; // Disable virtual memory
    logic                   store;       // Request is a store
    logic                   msb_err;     // not good sign ext for bits  63:48
} dcache_tlb_req;

// TLB response
typedef struct packed
{
    logic                   fill_pending; // TLB fill is pending after a TLB miss
    logic                   miss;         // TLB miss
    logic [`PA_TRANS_RANGE] ppn;          // Physical Page Number
    logic                   xcpt_ld;      // Load exception
    logic                   xcpt_st;      // Store exception
    logic                   access_fault; // Exception is "Access fault" or "Page fault"
} dcache_tlb_resp;

// Replay Info
typedef struct packed
{
    `ifdef DCACHE_REPORT_PC
    logic [`PC_RANGE_EXT]              pc;             // PC of the instruction
    `endif
    logic [`VA_RANGE]                  addr;           // Address of the request
    minion_reg_dest                    dest;           // Destination register for loads
    dcache_cmd                         cmd;            // Command done
    dcache_type                        typ;            // Size of operation
    logic [`CORE_GSC_CNT_RANGE]        gsc_cnt;        // Gather scatter element count
    logic [`DMEM_REQ_PS_MASK_SZ-1:0]   ps_mask;        // Mask for PS accesses
    logic                              phys;           // Is a physical address
    logic                              buf_alloc;      // A buffer entry is allocated and needs to be released when transaction finishes
    logic [`DCACHE_BUFFER_ID_RANGE]    buf_id;         // Entry in buffer array
    logic                              cache_op;       // This is a request from cache ops
    logic                              misaligned;     // If the request is misaligned
    logic                              misaligned_2nd; // Transaction is a second 2nd pass for a misaligned
    logic                              cacheable;      // If the request is cacheable
    logic                              uc_load_2nd;    // Second pass for a UC load, simply read the data from buffer array
    logic [`CORE_GSC32_IDX_V_RANGE]    gsc32_idx;      // Indices for gather/scatter on 32-byte blocks
    logic [`DCACHE_VM_SEL_RANGE]       vm_sel;         // Selects the VM status that has to be used for TLB translation
    logic                              mem_global;     // Flag indicating if memory access is global or local
    prv_t                              prv;            // Privilage level in which the operation was executed. Consumed by PMA on misalign second pas} dcache_replay_req;   
} dcache_replay_req;

typedef struct packed
{
    minion_reg_dest                    dest;
    dcache_cmd                         cmd;
    dcache_type                        typ;
    logic [`CORE_GSC_CNT_RANGE]        gsc_cnt;
    logic [`DMEM_REQ_PS_MASK_SZ-1:0]   ps_mask;
    logic                              buf_alloc;
    logic [`DCACHE_BUFFER_ID_RANGE]    buf_id;
    logic                              cache_op;
    logic [`CORE_GSC32_IDX_V_RANGE]    gsc32_idx;
    logic [`DCACHE_VM_SEL_RANGE]       vm_sel;
    logic                              mem_global;
    prv_t                              prv;
} dcache_replay_req_push_t;  // Reduced version of dcache_replay_req_t for push data array

typedef struct packed
{
    `ifdef DCACHE_REPORT_PC
    logic [`PC_RANGE_EXT]              pc;             // PC of the instruction
    `endif
    logic [`VA_RANGE]                  addr;           // Address of the request
    logic                              phys;           // Is a physical address
    logic                              misaligned;     // If the request is misaligned
    logic                              misaligned_2nd; // Transaction is a second 2nd pass for a misaligned
    logic                              cacheable;      // If the request is cacheable
    logic                              uc_load_2nd;    // Second pass for a UC load, simply read the data from buffer array
} dcache_replay_req_rearm_t;


// Miss Handler Request state
typedef struct packed
{
    `ifdef DCACHE_REPORT_PC
    logic [`PC_RANGE_EXT]           pc;               // PC of the instruction
    `endif
    logic [`PA_RANGE]               addr;             // Address of the request
    logic [`DCACHE_SET_RANGE]       set;              // Set associated to the address of this request
    dcache_cmd                      cmd;              // Command done
    dcache_type                     typ;              // Size of operation
    logic [`DCACHE_BUFFER_ID_RANGE] buf_id;           // Entry in buffer array
    logic [`DCACHE_WAYS-1:0]        way_en;           // "Replace way"/"way that needs permision upgrade"
    logic                           tag_match;        // If it is a hit or miss
    dcache_meta_data                old_meta;         // Tag of the line that hit or the victim
    logic                           cacheable;        // The request is cacheable
    logic                           thread_id;        // Thread ID that requested the miss
    logic                           mem_g;            // Global memory access
    logic                           is_amo;           // Operation is atomic
    logic                           is_msg;           // Operation has to generate a message
    logic                           is_bypass;        // Operation is bypass L1
} dcache_mh_req;

typedef struct packed
{
  logic [`DCACHE_DBG_CSR_TL_SIZE-1:0]    csr_signals_tl0;
  logic [`DCACHE_DBG_CSR_TL_SIZE-1:0]    csr_signals_tl1;
  logic [`DCACHE_DBG_CSR_TS_SIZE-1:0]    csr_signals_ts;
  logic [`DCACHE_DBG_CSR_COU_SIZE-1:0]   csr_signals_cou;
  logic [`DCACHE_DBG_CSR_COUL2_SIZE-1:0] csr_signals_coul2;
  logic [`DCACHE_DBG_CSR_TXSND_SIZE-1:0] csr_signals_txsnd;
  logic [`DCACHE_DBG_CSR_MH_SIZE-1:0]    csr_signals_mh0;
  logic [`DCACHE_DBG_CSR_MH_SIZE-1:0]    csr_signals_mh1;  
} dcache_debug_sigs;

// Cache Ops request state
typedef enum logic [3:0]
{
    Cache_Op_Invalid,           // Unit is idle
    Cache_Op_Meta_Read,         // Reads the tags to check which way the line is (if present) (S0 stage)
    Cache_Op_Meta_Resp_Wait,    // Waits for the tag response (S1 stage)
    Cache_Op_Meta_Resp,         // Receiving the tag response (S2 stage)
    Cache_Op_Writeback_Req,     // Starts a writeback
    Cache_Op_Writeback_Resp,    // Waits for writeback response
    Cache_Op_Meta_Write,        // Updates the tags
    Cache_Op_Meta_Write_Lock,   // Updates the lock status
    Cache_Op_Wait_Conflict,     // Waits for Miss Handler to finish an operation that had dependencies
    Cache_Op_Next_Operation,    // Moves to next operation of the request
    Cache_Op_Wait_Tmask,        // Waits for tensor mask to be ready
    Cache_Op_L1_Prefetch,       // Injects a prefetch request into pipeline
    Cache_Op_NextOp_Or_Invalid, // Intermediate state for prefetch to wait for s1_tlb_fail
    Cache_Op_Wait_Next,         // Waits for cache operation doing other side actions
    Cache_Op_Wait_TLB,          // Waits for TLB to become ready
    Cache_Op_Unused             // Downgrades the state of the clean data
} cache_op_state_t;

// Cache Ops conflict states
typedef enum logic
{
  CO_Conflict_MH,    // Conflict with Miss Handler operation
  CO_Conflict_RQ     // Conflict with Replay Queue content
} cache_op_conflict_t;

// Cache Ops struct that holds the ongoing request
typedef struct packed
{
    CacheOpControl_t           cmd;         // Cache op command
    snoop_request              snoop_type;  // Type of snoop
    logic                      thread_id;   // Thread ID that requested the operation
    logic                      use_tmask;   // Flag indicating that tensor mask has to be used
    logic [3:0]                count;       // Number of operations to do
    CacheOpLevel_t             start_level; // Operation start level
    CacheOpLevel_t             dest_level;  // Operation destination level
    logic                      prefetch;    // Needs to do L1 prefetch operations
    logic                      send_to_l2;  // Request must be sent to L2
} cache_op_req_t;

// Cache Ops L2 Request state
typedef enum logic [2:0]
{
    L2_Cache_Op_Invalid,        // Unit is idle
    L2_Cache_Op_Translate,      // Unit requests for address translation
    L2_Cache_Op_Translate_Wait, // Unit waits for address translation
    L2_Cache_Op_Request,        // Does an L2 cache op
    L2_Cache_Op_Next_Operation, // Moves to next operation of the request
    L2_Cache_Op_Wait_Tmask      // Waits tensor mask bits ready
} L2_Cache_Op_state;

// Cache Ops L2 struct that holds the ongoing request
typedef struct packed
{
    logic                thread_id;   // Thread ID that requested the operation
    CacheOpControl_t     cmd;         // Type of cache op
    logic [1:0]          start_level; // Start level of the request
    logic [1:0]          dest_level;  // Dest level of the request
    logic [`VA_RANGE_CA] stride;      // Stride between the requests
    logic                use_tmask;   // Flag indicating the use ot tensor mask
    logic [3:0]          count;       // Number of operations to do
    logic                is_byp;      // Indicates that processed request comes from a bypass
    logic                is_l2_scp;   // Indicates that operation is to fill L2 Scp
} l2_cache_op_req;


////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

// Gets some info from the dcache command
function automatic bit dcache_cmd_is_amo;
    input dcache_cmd cmd;

    dcache_cmd_is_amo = (cmd == dcache_cmd_XA_ADD)    ||
                        (cmd == dcache_cmd_XA_XOR)    ||
                        (cmd == dcache_cmd_XA_OR)     ||
                        (cmd == dcache_cmd_XA_AND)    ||
                        (cmd == dcache_cmd_XA_MIN)    ||
                        (cmd == dcache_cmd_XA_MAX)    ||
                        (cmd == dcache_cmd_XA_MINU)   ||
                        (cmd == dcache_cmd_XA_MAXU)   ||
                        (cmd == dcache_cmd_XA_SWAP)   ||
                        (cmd == dcache_cmd_XA_CMPSWP) ||
                        (cmd == dcache_cmd_XA_MAX_PS) ||
                        (cmd == dcache_cmd_XA_MIN_PS);
endfunction 

function automatic bit dcache_cmd_is_prefetch;
    input dcache_cmd cmd;

    dcache_cmd_is_prefetch = (cmd == dcache_cmd_PFR) || (cmd == dcache_cmd_PFW);
endfunction

function automatic bit dcache_cmd_is_read;
    input dcache_cmd cmd;

    dcache_cmd_is_read =  (cmd == dcache_cmd_XRD)    || (cmd == dcache_cmd_XLR)   || (cmd == dcache_cmd_XSC) 
                       ||  dcache_cmd_is_amo(cmd)    || (cmd == dcache_cmd_G32)   || (cmd == dcache_cmd_XRDB);
endfunction

function automatic bit dcache_cmd_is_write;
    input dcache_cmd cmd;

    dcache_cmd_is_write = (cmd == dcache_cmd_XWR)     || (cmd == dcache_cmd_XSC)     || dcache_cmd_is_amo(cmd)
                       || (cmd == dcache_cmd_SC32)    || (cmd == dcache_cmd_XWRB);
endfunction

function automatic bit dcache_cmd_is_bypass;
    input dcache_cmd cmd;

    dcache_cmd_is_bypass = (cmd == dcache_cmd_XWRB) || (cmd == dcache_cmd_XRDB);
endfunction

function automatic bit dcache_cmd_is_write_intent;
    input dcache_cmd cmd;

    dcache_cmd_is_write_intent = dcache_cmd_is_write(cmd) || (cmd == dcache_cmd_PFW) || (cmd == dcache_cmd_XLR);
endfunction

function automatic bit dcache_cmd_is_gsc32;
    input dcache_cmd cmd;

    dcache_cmd_is_gsc32 = (cmd == dcache_cmd_G32) || (cmd == dcache_cmd_SC32);    
endfunction

function automatic bit dcache_type_is_gsc;
    input dcache_type typ;
    
    dcache_type_is_gsc = (typ == dcache_type_PS_GS8) || (typ == dcache_type_PS_GS16) || (typ == dcache_type_PS_GS32);
    
endfunction

function automatic bit dcache_type_is_byte;
    input dcache_type typ;
    
    dcache_type_is_byte = (typ == dcache_type_B) || (typ == dcache_type_BU) || (typ == dcache_type_PS_GS8);
    
endfunction

function automatic bit dcache_type_is_hword;
    input dcache_type typ;
    
    dcache_type_is_hword = (typ == dcache_type_H) || (typ == dcache_type_HU) || (typ == dcache_type_PS_GS16);
    
endfunction

function automatic bit dcache_type_is_word;
    input dcache_type typ;
    
    dcache_type_is_word = (typ == dcache_type_W) || (typ == dcache_type_WU) || (typ == dcache_type_PS_BR) || (typ == dcache_type_PS_GS32);
    
endfunction

function automatic bit dcache_type_is_dword;
    input dcache_type typ;
    
    dcache_type_is_dword = (typ == dcache_type_D);  
endfunction

function automatic bit dcache_type_is_ps;
    input dcache_type typ;
    
    dcache_type_is_ps = (typ == dcache_type_PS)     || (typ == dcache_type_PS_BR)   || 
                        (typ == dcache_type_PS_GS8) || (typ == dcache_type_PS_GS16) || (typ == dcache_type_PS_GS32);
    
endfunction

function automatic bit dcache_address_is_size_aligned;
    input dcache_type typ;
    input [4:0]       addr;
    
    dcache_address_is_size_aligned = dcache_type_is_byte(typ)                         ||
                                     dcache_type_is_hword(typ) && (addr[0]   == 1'b0) ||
                                     dcache_type_is_word(typ)  && (addr[1:0] == 2'b0) ||
                                     dcache_type_is_dword(typ) && (addr[2:0] == 3'b0) ||
                                     (typ == dcache_type_PS)   && (addr[4:0] == 5'b0);
endfunction

function automatic bit dcache_access_is_page_split;
    input [`VA_RANGE]     addr;
    input tlb_entry_type  vmspagesize;
    input                 misaligned_2_cachelines;
    
    // Check a crossing in the page for next cache line, if misalignment causes a 2 line access
    // For every page size, check if adding one cacheline would produce a "page overflow"
    if (misaligned_2_cachelines)
    begin
      if (vmspagesize == tlb_entry_type_4K) // 12-bit addressing
        dcache_access_is_page_split = &addr[11:`DCACHE_LINE_ADDR_BITS];
      else if (vmspagesize == tlb_entry_type_2M) // 21-bit addressing
        dcache_access_is_page_split = &addr[20:`DCACHE_LINE_ADDR_BITS];
      else if (vmspagesize == tlb_entry_type_1G) // 30-bit addressing
        dcache_access_is_page_split = &addr[29:`DCACHE_LINE_ADDR_BITS];
      else
        dcache_access_is_page_split = 1'b0;
    end
    else
      dcache_access_is_page_split = 1'b0;
endfunction

function automatic [3:0] dcache_typ2qwen;
    input dcache_type typ;
    input [1:0] qwaddr;
    
    if (typ == dcache_type_PS)
      dcache_typ2qwen = 4'b0011;
    else
      dcache_typ2qwen = 4'b0001;
    
    dcache_typ2qwen = dcache_typ2qwen << qwaddr;

endfunction    

function automatic dcache_access_type dcache_cmd_to_type;
    input dcache_cmd cmd;
    logic [1:0] temp;

    temp = { dcache_cmd_is_write(cmd), dcache_cmd_is_write_intent(cmd) };
    // Warning: notice that this function is in sync with the dcache_access_type
    dcache_cmd_to_type = dcache_access_type'(temp);
endfunction

// Encode way
function automatic [`DCACHE_WAY_RANGE] dcache_way_encode;
    input [`DCACHE_WAYS-1:0] ways;

    dcache_way_encode = 0;
    for(integer i = 0; i < `DCACHE_WAYS; i++)
        if(ways[i])
            dcache_way_encode |= i[`DCACHE_WAY_RANGE];
endfunction

// Decode way
function automatic [`DCACHE_WAYS-1:0] dcache_way_decode;
    input [`DCACHE_WAY_RANGE] way;

    dcache_way_decode = `DCACHE_WAYS'b1 << way;
endfunction

//
// Decode way
// LRU victim selection
function automatic [`DCACHE_WAYS-1:0] dcache_lru_victim;
    input [`DCACHE_LRU_SET_RANGE]              lru_in;      // LRU input bits
    input [`DCACHE_WAYS-1:0]                   way_dis;     // Disabled ways for the set
    input [`DCACHE_WAYS-1:0]                   way_prio;    // Prioritized ways for the set to remain
    logic [`DCACHE_WAYS-1:0][`DCACHE_WAYS-1:0] lru_in_tmp0; // Triangle matrix view of input bits
    logic [`DCACHE_WAYS-1:0][`DCACHE_WAYS-1:0] lru_in_tmp1; // Triangle matrix view of input bits
    logic [`DCACHE_WAYS-1:0]                   dcache_lru_victim0;
    logic [`DCACHE_WAYS-1:0]                   dcache_lru_victim1;    
    
    lru_in_tmp0 = lru_in;
    lru_in_tmp1 = lru_in;
    // First is to "disable" the ways by marking them as recently used so they
    // don't get selected
    for(integer i = 0; i < `DCACHE_WAYS; i++)
    begin
        // tmp0 will consider the disables and the priority cases
        if(way_dis[i] || way_prio[i])
        begin
            // For all the lru bits on the "left" (ways with lower ids)
            // mark it as younger access (LRU bit set to 0)
            for(integer j = 0; j < i; j++)
                lru_in_tmp0[i][j] = 1'b0;
            // For all the lru bits "below" (ways with higher ids)
            // mark it as younger access (LRU bit set to 1)
            for(integer j = i + 1; j < `DCACHE_WAYS; j++)
                lru_in_tmp0[j][i] = 1'b1;
        end
        
        // tmp1 will consider only the the disables
        if(way_dis[i])
        begin
            // For all the lru bits on the "left" (ways with lower ids)
            // mark it as younger access (LRU bit set to 0)
            for(integer j = 0; j < i; j++)
                lru_in_tmp1[i][j] = 1'b0;
            // For all the lru bits "below" (ways with higher ids)
            // mark it as younger access (LRU bit set to 1)
            for(integer j = i + 1; j < `DCACHE_WAYS; j++)
                lru_in_tmp1[j][i] = 1'b1;
        end
    end

    // Second, checking for all the ways which is the one with older access
    for(integer i = 0; i < `DCACHE_WAYS; i++)
    begin
        dcache_lru_victim0[i] = 1'b1;
        dcache_lru_victim1[i] = 1'b1;
        // For all the lru bits on the "left" (ways with lower ids)
        // check if this way has older access (LRU bit set to 1)
        for(integer j = 0; j < i; j++)
        begin
            dcache_lru_victim0[i] = dcache_lru_victim0[i] && lru_in_tmp0[i][j];
            dcache_lru_victim1[i] = dcache_lru_victim1[i] && lru_in_tmp1[i][j];
        end
        // For all the lru bits "below" (ways with higher ids)
        // check if this way has older access (LRU bit set to 0)
        for(integer j = i + 1; j < `DCACHE_WAYS; j++)
        begin
            dcache_lru_victim0[i] = dcache_lru_victim0[i] && ~lru_in_tmp0[j][i];
            dcache_lru_victim1[i] = dcache_lru_victim1[i] && ~lru_in_tmp1[j][i];
        end
    end
    
    // If no victim can be selected considering disables and prioritized ways,
    // look for a victim only considering the disabled ways
    if (|(dcache_lru_victim0 & ~way_dis & ~way_prio))
      dcache_lru_victim = dcache_lru_victim0;
    else
      dcache_lru_victim = dcache_lru_victim1;

endfunction

// LRU update. Returns the LRU bits after accessing a way
function automatic [`DCACHE_LRU_SET_RANGE] dcache_lru_update;
    input [`DCACHE_LRU_SET_RANGE]              lru_in;  // LRU input bits
    input [`DCACHE_WAYS-1:0]                   way_en;  // Which way was accessed
    logic [`DCACHE_WAYS-1:0][`DCACHE_WAYS-1:0] lru_out; // Triangle matrix view of output LRU bits

    lru_out = lru_in;
    // For all the ways
    for(int i = 0; i < `DCACHE_WAYS; i++)
    begin
        // If way was accessed
        if(way_en[i])
        begin
            // For all the LRU bits on the "left" (ways with lower ids)
            // set LRU to 0 to mark as latest access
            for(int j = 0; j < i; j++)
            begin
                lru_out[i][j] = 1'b0;
            end
            // For all the LRU bits below (ways with higher ids)
            // set LRU to 1 to mark as latest access
            for(int j = i + 1; j < `DCACHE_WAYS; j++)
            begin
                lru_out[j][i] = 1'b1;
            end
        end
    end
    dcache_lru_update = lru_out;
endfunction

// Gets some info from the dcache command
function automatic et_link_size_t dcache_uc_size;
    input dcache_type typ;

    if     (typ == dcache_type_B)       dcache_uc_size = ET_LINK_Byte;
    else if(typ == dcache_type_H)       dcache_uc_size = ET_LINK_HWord;
    else if(typ == dcache_type_W)       dcache_uc_size = ET_LINK_Word;
    else if(typ == dcache_type_D)       dcache_uc_size = ET_LINK_DWord;
    else if(typ == dcache_type_BU)      dcache_uc_size = ET_LINK_Byte;
    else if(typ == dcache_type_HU)      dcache_uc_size = ET_LINK_HWord;
    else if(typ == dcache_type_WU)      dcache_uc_size = ET_LINK_Word;
    else if(typ == dcache_type_PS)      dcache_uc_size = ET_LINK_HLine;
    else if(typ == dcache_type_PS_BR)   dcache_uc_size = ET_LINK_Word;
    else if(typ == dcache_type_PS_GS8)  dcache_uc_size = ET_LINK_Byte;
    else if(typ == dcache_type_PS_GS16) dcache_uc_size = ET_LINK_HWord;
    else                                dcache_uc_size = ET_LINK_Word;
endfunction 

function automatic bit dcache_replayq_entry_is_older;
    input [`DCACHE_REPLAYQ_AGE_ID] entry1;
    input [`DCACHE_REPLAYQ_AGE_ID] entry2;

    logic cmp;

    cmp = (entry1[`DCACHE_REPLAYQ_AGE_ID_CMP] < entry2[`DCACHE_REPLAYQ_AGE_ID_CMP]);
    // Same age
    if(entry1[`DCACHE_REPLAYQ_AGE_ID_ERA] == entry2[`DCACHE_REPLAYQ_AGE_ID_ERA])
        dcache_replayq_entry_is_older = cmp;
    // Different age
    else
        dcache_replayq_entry_is_older = !cmp;
endfunction

function automatic [`DCACHE_SET_RANGE] dcache_set_msb;
    input cfg_sets_red;

    // Set upper bits to '1' when the configuration is set to use a reduced number of sets
    dcache_set_msb = {(`DCACHE_SET_SZ){1'b0}};
    dcache_set_msb[`DCACHE_SET_MSB_RANGE] = {(`DCACHE_SET_MSB_SZ){cfg_sets_red}};
    
endfunction

function automatic [`DCACHE_SET_RANGE] dcache_set_correct;
    input [`DCACHE_SET_RANGE] set;
    input dcache_mode_t       dcache_mode;
    input                     thread_id;
    
    if (dcache_mode == DcacheMode_Split)
    begin
      if (thread_id == 1'b0)  // thread 0 uses sets 0-7
        dcache_set_correct = {1'b0,set[$clog2(`DCACHE_SETS)-2:0]};
      else                    // thread 1 uses sets 14-15
        dcache_set_correct = {{($clog2(`DCACHE_SETS)-1){1'b1}},set[0]};
    end
    else if (dcache_mode == DcacheMode_Scratchpad)
    begin
      if (thread_id == 1'b0)  // thread 0 uses sets 12-13
        dcache_set_correct = {{($clog2(`DCACHE_SETS)-2){1'b1}},1'b0,set[0]};
      else                    // thread 1 uses sets 14-15
        dcache_set_correct = {{($clog2(`DCACHE_SETS)-1){1'b1}},set[0]};
    end
    else // (dcache_mode == DcacheMode_Shared)
    begin
      dcache_set_correct = set;
    end
    
endfunction

function automatic [`ET_ATOMIC_CONFIG_SIZE-1:0] dcache_get_et_link_amo_config;
  input dcache_cmd cmd;
  input dcache_type typ;
  input logic mem_global;
  
  et_link_amo_opcode opcode;
  logic vector;
  logic size;
  
  if      (cmd == dcache_cmd_XA_SWAP   ) opcode = ET_LINK_AMO_Swap;
  else if (cmd == dcache_cmd_XA_ADD    ) opcode = ET_LINK_AMO_Add;
  else if (cmd == dcache_cmd_XA_XOR    ) opcode = ET_LINK_AMO_Xor;
  else if (cmd == dcache_cmd_XA_AND    ) opcode = ET_LINK_AMO_And;
  else if (cmd == dcache_cmd_XA_OR     ) opcode = ET_LINK_AMO_Or;
  else if (cmd == dcache_cmd_XA_MIN    ) opcode = ET_LINK_AMO_Min;
  else if (cmd == dcache_cmd_XA_MAX    ) opcode = ET_LINK_AMO_Max;
  else if (cmd == dcache_cmd_XA_MINU   ) opcode = ET_LINK_AMO_MinU;
  else if (cmd == dcache_cmd_XA_MAXU   ) opcode = ET_LINK_AMO_MaxU;
  else if (cmd == dcache_cmd_XA_MIN_PS ) opcode = ET_LINK_AMO_Min_PS;
  else if (cmd == dcache_cmd_XA_MAX_PS ) opcode = ET_LINK_AMO_Max_PS;
  else if (cmd == dcache_cmd_XA_CMPSWP ) opcode = ET_LINK_AMO_Cmp_Swap;
  else                                   opcode = ET_LINK_AMO_Swap;  // Any default
  
  if      (typ == dcache_type_W) size = 1'b0;
  else if (typ == dcache_type_D) size = 1'b1;
  else                           size = 1'b0;
  
  // Gather/Scatter operations cannot be considered as vector operations
  vector = 1'b0;
  // if (typ == dcache_type_PS_GS32) vector = 1'b1;
  // else                            vector = 1'b0;
  
  dcache_get_et_link_amo_config = {mem_global,size,vector,opcode};

endfunction


// Map SCP index to set and way
// Valid index to SCP must be in range 0 ... DCACHE_TL_SCP_MAX_IDX-1
//    Sets 12, 13, 14 and 15 cannot be used, we have to wrap around
//    Each time we wrap the way is increased
// Index: 0 ...11: set 0..11, way 0
// Index: 12...23: set 0..11, way 1
// Index: 24...35: set 0..11, way 2
// Index: 36...47: set 0..11, way 3

function automatic [`DCACHE_SET_RANGE] get_set_from_scp_dest;
  input [`DCACHE_TENSOR_CTRL_DEST_RANGE] dest;
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE] dest_wrap;
  
  // Implemented function is dest % DCACHE_TL_SCP_MOD
  if (dest < `DCACHE_TL_SCP_MOD)
    dest_wrap  = dest;
  else if (dest < 2*`DCACHE_TL_SCP_MOD)
    dest_wrap  = dest - `DCACHE_TL_SCP_MOD;
  else if (dest < 3*`DCACHE_TL_SCP_MOD)
    dest_wrap  = dest - 2*`DCACHE_TL_SCP_MOD;
  else if (dest < 4*`DCACHE_TL_SCP_MOD)
    dest_wrap  = dest - 3*`DCACHE_TL_SCP_MOD;
  else if (dest < 5*`DCACHE_TL_SCP_MOD)
    dest_wrap  = dest - 4*`DCACHE_TL_SCP_MOD;
  else
    dest_wrap  = dest - 5*`DCACHE_TL_SCP_MOD;

  get_set_from_scp_dest = dest_wrap[`DCACHE_SET_RANGE];
  
endfunction

function automatic [`DCACHE_WAY_RANGE] get_way_from_scp_dest;
  input [`DCACHE_TENSOR_CTRL_DEST_RANGE] dest;
  
  // Implemented function is (dest / DCACHE_TL_SCP_MOD) % 4
  if (dest < `DCACHE_TL_SCP_MOD)
    get_way_from_scp_dest  = 2'd0;
  else if (dest < 2*`DCACHE_TL_SCP_MOD)
    get_way_from_scp_dest  = 2'd1;
  else if (dest < 3*`DCACHE_TL_SCP_MOD)
    get_way_from_scp_dest  = 2'd2;
  else if (dest < 4*`DCACHE_TL_SCP_MOD)
    get_way_from_scp_dest  = 2'd3;
  else if (dest < 5*`DCACHE_TL_SCP_MOD)
    get_way_from_scp_dest  = 2'd0;
  else
    get_way_from_scp_dest  = 2'd1;

endfunction

//function automatic [`DCACHE_REDUCE_ACTION_RANGE] dcache_get_reduce_action;
//  input [`DCACHE_REDUCE_ACTION_RANGE] action;
//  input [`DCACHE_REDUCE_LEVEL_RANGE]  level; 
//  input [`MAX_MINION_ID_R]            global_min_id; 
//
//  logic [`MAX_MINION_ID_R]  hartmask;          // Which bits of the hart to look at
//  logic [`MAX_MINION_ID_R]  distance;          // Distance based on level
//  logic                     invert;            // Invert the reduce to a broadcast
//  logic                     pairing_auto;      // Auto pairing method selected
//  
//  // Obtain parameters to compute partner minion and action to implement
//  //   Computation of distance and hartmask strictly speaking may overflow as level can do more 
//  //   shifting than bits in global_min_id bits. Configuration from SW must be correct, otherwise behavior 
//  //   is "undefined"
//  //   
//  //   Since `MAX_MINION_ID_L is defined as $clog2(`MAX_MINION_ID), it can't be used to specify bitlength
//  //   Implicit arithmetic zero extension creates a linter warning but dimensions of operands are well determined
//  
//  distance       = `ZX(`MAX_MINION_ID_L,1'b1) << level;
//  hartmask       = (`ZX(`MAX_MINION_ID_L,1'b1) << (level + `DCACHE_REDUCE_LEVEL_BITS'b1)) - `ZX(`MAX_MINION_ID_L,1'b1);
//  invert         = ~action[0];
//
//  // Pairing mode
//  pairing_auto = (action != `DCACHE_REDUCE_SEND) && (action != `DCACHE_REDUCE_RECV);
//
//  // Do auto decode of action based on minion ID and level
//  if (pairing_auto)
//  begin
//      if((global_min_id & hartmask) == distance)
//      begin
//          // Reverse the behaviour in case of doing broadcast instead of reduce
//          dcache_get_reduce_action = {1'b0, invert}; // 00=>sender, 01=>receiver (invert==1)
//      end
//      else if((global_min_id & hartmask) == 0)
//      begin
//          // Reverse the behaviour in case of doing broadcast instead of reduce
//          dcache_get_reduce_action = {1'b0,~invert}; // 01=>receiver, 00=>sender (invert==1)
//      end
//      else
//      begin
//          dcache_get_reduce_action = `DCACHE_REDUCE_DO_NOTHING; // do nothing
//      end
//  end else begin
//      dcache_get_reduce_action = action; 
//  end
//
//endfunction
