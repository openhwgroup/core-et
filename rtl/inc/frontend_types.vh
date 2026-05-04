// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

// State of the Icache prefetch FSM
typedef enum logic [2:0]
{
    icache_prefetch_state_Idle          = 3'b000, // FSM is idle
    icache_prefetch_state_Request       = 3'b001, // FSM is generating a prefetching request
    icache_prefetch_state_Check_Hit     = 3'b010, // Check tag hit
    icache_prefetch_state_Fill_Wait     = 3'b011, // FSM is waiting to receive a fill
    icache_prefetch_state_FillDone_Wait = 3'b100, // FSM is waiting to see f5_resp_fill_done
    icache_prefetch_state_PreReq_Pause  = 3'b101  // FSM is pausing for some cycles before generating a new request
} icache_prefetch_state;

// State of the Icache FSM tracking state of misses not being attended to
typedef enum logic [1:0]
{
    icache_prefetch_miss_nomh_state_NotPending    = 2'b00, // No miss pending that was not attended to
    icache_prefetch_miss_nomh_state_FillDone_Wait = 2'b01, // Miss that was not attended to is pending. Waiting for fill done to be notified
    icache_prefetch_miss_nomh_state_Req_Wait      = 2'b10  // Miss that was not attended to is pending. Waiting for a new request to be received
} icache_prefetch_miss_nomh_state;

// State of the Icache miss FSM
typedef enum logic [1:0]
{
    icache_miss_state_Ready     = 2'b00, // ICache is ready to accept new requests
    icache_miss_state_Request   = 2'b01, // ICache is sending a request to an upper level
    icache_miss_state_Fill_Wait = 2'b10  // ICache is waiting to receive a fill
} icache_miss_state;

// Icache error logging
typedef enum logic [3:0]
{
    icache_err_code_SBE        = 4'd0,
    icache_err_code_DBE        = 4'd1,
    icache_err_code_ECC_cnt_ov = 4'd2
} icache_err_code_t;

typedef enum logic [2:0]
{
    icache_data_dbg_state_Idle           = 3'b000,
    icache_data_dbg_state_WaitReadReady  = 3'b001,
    icache_data_dbg_state_Read           = 3'b010,
    icache_data_dbg_state_WaitWriteReady = 3'b011,
    icache_data_dbg_state_Write          = 3'b100,
    icache_data_dbg_state_Done           = 3'b101
} icache_data_dbg_state_t;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

// Frontend internal operation
typedef struct packed
{
   logic [`PC_RANGE_EXT] pc;           // PC of the fetch
   logic [`INST_RANGE]   inst_bits;    // Inst data
   logic                 page_fault0;   // Fetch page fault (TLB)
   logic                 access_fault0; // Fetch access fault (PMA)
   logic                 page_fault1;   // Fetch page fault (TLB) (in second 16b)
   logic                 access_fault1; // Fetch access fault (PMA)    (in second 16b)
   logic                 replay;       // Replay the inst due to a fetch issue
   logic                 bus_err;
   logic                 ecc_err;
} frontend_operation;


// Frontend response to core
typedef struct packed
{
    logic [`PC_RANGE_EXT]   pc;            // PC of the fetch
    logic [`INST_RANGE]     inst_bits;     // Inst data
    logic                   pf0;           // Page fault in first chunk of the instruction
    logic                   pf1;           // Page fault in second chunk of the instruction
    logic                   af0;           // Access fault in first chunk of the instruction
    logic                   af1;           // Access fault in second chunk of the instruction
    logic                   replay;        // Replay the inst due a fetch issue
    logic                   rvc;           // The instruction is risc V compressed
    logic                   bus_error;     // received a bus error for this address
    logic                   ecc_error;     // received an ecc error for this address
} frontend_core_resp;

typedef struct packed {
   frontend_core_resp  core_resp;
   vpu_ctrl_sigs_t     vpu_ctrl_sigs;
   minion_control      intpipe_ctrl;
} frontend_thread_data;


// Frontend request to Icache
typedef struct packed
{
    logic [`CORE_NR_THREADS_R] thread_id;  // Thread ID
    minion_vm_status           vm_status;  // Status bits for VM
    logic [`VA_RANGE_EXT]      addr;       // Fetch address
} frontend_icache_req;

// Core control to front end
typedef struct packed
{
    logic wfi;        // in WFI instruction
} core_frontend_ctrl;

// Icache prefetch service configuration
typedef struct packed
{
    logic [`CSR_PRV_SZ-1:0] prv;
    logic [`VA_RANGE_CA]    start_addr;
    logic [5:0]             num_lines;
} icache_prefetch_conf_t;

// Icache response to frontend
typedef struct packed
{
    logic [`FE_FETCH_READ_RANGE] data;         // Inst data fetched
    logic                        page_fault;   // Fetch page fault exception (TLB)
    logic                        access_fault; // Fetch access fault exception (PMA)
    logic                        cacheable;    // Fetched data is cacheable
    logic                        bus_err;    // line has L2 bus error
    logic                        ecc_err;    // line had ecc error
} icache_frontend_resp;

// Tag write request
typedef struct packed
{
    logic                        valid; // Set tag to valid/invalid
    logic [`ICACHE_SET_RANGE]    idx;   // Set
    logic [`ICACHE_WAY_RANGE]    way;   // Way
    logic [`ICACHE_PA_TAG_RANGE] data;  // Tag
} icache_tag_write_req;

// Request to the TLB
typedef struct packed
{
    minion_vm_status        status;
    logic [`VA_TRANS_RANGE] vpn; // Virtual Page Number
    logic passthrough;           // Disable virtual memory
    logic msb_err;
} icache_tlb_req;

// TLB response
typedef struct packed
{
    logic                   fill_pending; // TLB fill is pending after a TLB miss
    logic                   miss;         // TLB miss
    logic [`PA_TRANS_RANGE] ppn;          // Physical Page Number
    logic                   xcpt_if;      // Fetch exception
    logic                   access_fault; // Exception is "Access fault" or "Page fault"
} icache_tlb_resp;

// Error log info sent from icache
typedef struct packed
{
  logic [`ICACHE_WAY_RANGE]      way;
  logic [`ICACHE_SET_RANGE]      set;
  logic [`ICACHE_ECC_BLOCKS-1:0] dbe_per_block;
  logic [`ICACHE_ECC_BLOCKS-1:0] sbe_per_block;
  logic [`PA_RANGE_CA]           address;
} icache_err_log_info_t;

// Error log info saved into ESR
typedef struct packed
{
    logic                          dbe_ov;
    logic [51:48]                  reserved_51_48;
    logic [`ICACHE_ECC_BLOCKS-1:0] err_bits;
    logic [39:17]                  reserved_39_17;
    logic [`ICACHE_WAY_RANGE]      way;
    logic [`ICACHE_SET_RANGE]      set;
    icache_err_code_t              err_code;
    logic                          reserved;
    logic                          enabled;
    logic                          multi;
    logic                          valid;
} esr_icache_err_log_info_t;

////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// LRU victim selection
function automatic [`ICACHE_WAYS-1:0] icache_lru_victim;
    input [`ICACHE_LRU_SET_RANGE]              lru_in;     // LRU input bits
    logic [`ICACHE_WAYS-1:0][`ICACHE_WAYS-1:0] lru_in_tmp; // Triangle matrix view of input bits

    lru_in_tmp = lru_in;

    // Second, checking for all the ways which is the one with older access
    for(integer i = 0; i < `ICACHE_WAYS; i++)
    begin
        icache_lru_victim[i] = 1'b1;
        // For all the lru bits on the "left" (ways with lower ids)
        // check if this way has older access (LRU bit set to 1)
        for(integer j = 0; j < i; j++)
        begin
            icache_lru_victim[i] = icache_lru_victim[i] && lru_in_tmp[i][j];
        end
        // For all the lru bits "below" (ways with higher ids)
        // check if this way has older access (LRU bit set to 0)
        for(integer j = i + 1; j < `ICACHE_WAYS; j++)
        begin
            icache_lru_victim[i] = icache_lru_victim[i] && ~lru_in_tmp[j][i];
        end
    end

endfunction

// LRU update. Returns the LRU bits after accessing a way
function automatic [`ICACHE_LRU_SET_RANGE] icache_lru_update;
    input [`ICACHE_LRU_SET_RANGE]              lru_in;  // LRU input bits
    input [`ICACHE_WAYS-1:0]                   way_en;  // Which way was accessed
    logic [`ICACHE_WAYS-1:0][`ICACHE_WAYS-1:0] lru_out; // Triangle matrix view of output LRU bits

    lru_out = lru_in;
    // For all the ways
    for(int i = 0; i < `ICACHE_WAYS; i++)
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
            for(int j = i + 1; j < `ICACHE_WAYS; j++)
            begin
                lru_out[j][i] = 1'b1;
            end
        end
    end
    icache_lru_update = lru_out;
endfunction

