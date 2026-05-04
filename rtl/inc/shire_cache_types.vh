// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

// Enum of SC size  // NOTE: 4-bits in et_link to support qwens
typedef et_link_size_t sc_size_t;


// Pipe response type used for routing data
typedef enum logic {
   SC_NormalRsp       = 1'b0,
   SC_VictimData      = 1'b1
} sc_pipe_data_rsp_type_t;


// Keep L2, l3, Scp and Idx in lower 2 bits for pipeline
typedef enum logic [2:0] {
   SC_Address_L2         = 3'h0,
   SC_Address_L3         = 3'h1,
   SC_Address_Scp        = 3'h2,
   SC_Address_Idx        = 3'h3,
   SC_Address_Msg        = 3'h4 
} sc_address_space_t;

// Keep track of where the request came from
typedef enum logic [1:0] {
   SC_Reqq_L2            = 2'h0,
   SC_Reqq_L3            = 2'h1,
   SC_Reqq_FSM           = 2'h2
} sc_reqq_source_t;

// Keep CacheOp start and end levels
// Used also for Scp local or remote
typedef enum logic [1:0] {
   SC_Cache_L1           = 2'h0,
   SC_Cache_L2           = 2'h1,
   SC_Cache_L3           = 2'h2,
   SC_Cache_Mem          = 2'h3
} sc_cache_level_t;

// Keep CacheOp start and end levels
typedef enum logic  {
   SC_To_L3              = 1'b0,
   SC_To_Sys             = 1'b1
} sc_mesh_dest_t;

// Enum of SC request operations
typedef enum logic [4:0] {
   SC_Reqq_Read                = 5'h00,
   SC_Reqq_Write               = 5'h01,
   SC_Reqq_WriteAround         = 5'h02,
   SC_Reqq_WritePartial        = 5'h03,
   SC_Reqq_ReadCoop            = 5'h04, // Cooperative Read

   SC_Reqq_MsgSendData         = 5'h05, // Message send to any other minion in any neighborhood
   SC_Reqq_Atomic              = 5'h06,
   SC_Reqq_ScpRead             = 5'h07, // Scratch Pad Read
   SC_Reqq_ScpWrite            = 5'h08, // Scratch Pad Write
   SC_Reqq_ScpWritePartial     = 5'h09, // Scratch Pad Write Partials
   SC_Reqq_ScpReadCoop         = 5'h0a, // Scratch Pad Cooperative Read 

   SC_Reqq_Idx                 = 5'h0b,  // CacheOps from CacheOp FSM
   SC_Reqq_Sync                = 5'h0c,  // Sync from CacheOp FSM

   SC_Reqq_ReadForward         = 5'h0d, // Read forward from neigh to L3
   SC_Reqq_WriteForward        = 5'h0e, // Write forward from neigh to L3
   SC_Reqq_WriteForwardPartial = 5'h0f, // Write forward Partials from neigh to L3

   SC_Reqq_CopLock             = 5'h10,
   SC_Reqq_CopUnlock           = 5'h11,
   SC_Reqq_CopUnlockInv        = 5'h12,
   SC_Reqq_CopFlush            = 5'h13,
   SC_Reqq_CopEvict            = 5'h14,
   SC_Reqq_CopFlushWData       = 5'h15, // Flush with data, only received by L3
   SC_Reqq_CopEvictWData       = 5'h16, // Evict with data, only received by L3
   SC_Reqq_CopPrefetch         = 5'h17,
   SC_Reqq_CopScpFill          = 5'h18,

   // Paired second halves
   SC_Reqq_Atomic2             = 5'h19,
   SC_Reqq_WriteAround2        = 5'h1a,
   SC_Reqq_WritePartial2       = 5'h1b,

   SC_Reqq_Err                 = 5'h1c,

   // Theses 3 are just for trace.  They encode opcode and cache start level without adding more bits
   // These are only generated for trace packets by combining alloc orig_opcode with cache_start_level
   SC_Reqq_CopFlush_NextLevel    = 5'h1d,
   SC_Reqq_CopEvict_NextLevel    = 5'h1e,
   SC_Reqq_CopPrefetch_NextLevel = 5'h1f


} sc_reqq_orig_opcode_t;

// Enum of SC reqq operations
typedef enum logic [5:0] {
   // _L2 from l2 reqq to pipe:
   SC_L2_Read                   = 6'h00,
   SC_L2_Write                  = 6'h01, // full-line writes
   SC_L2_WritePartial           = 6'h02, // non-full line writes
   SC_L2_WritePartial_2nd_pass  = 6'h03, // non-full line writes
   SC_L2_WriteAround            = 6'h04,
   SC_L2_Lock                   = 6'h05,
   SC_L2_Unlock                 = 6'h06,
   SC_L2_UnlockInv              = 6'h07,
   SC_L2_Flush                  = 6'h08,
   SC_L2_Evict                  = 6'h09,
   SC_L2_Prefetch               = 6'h0a,
   SC_L2_Atomic                 = 6'h0b,
   SC_L2_Atomic_2nd_pass        = 6'h0c,  // internally generated
   SC_L2_Fill                   = 6'h0d,  // internally generated
   SC_L2_Scrub                  = 6'h0e,  // internally generated
   SC_L2_MsgSendData            = 6'h0f,

   // _L3 from l3 reqq to pipe:
   SC_L3_Read                   = 6'h10,
   SC_L3_Write                  = 6'h11,  // qw enables (full line writes and qw partial writes)
   SC_L3_WritePartial           = 6'h12,  // qw enables (sub-qw partial writes)
   SC_L3_WritePartial_2nd_pass  = 6'h13,  // qw enables (sub-qw partial writes)
   SC_L3_Flush                  = 6'h14,  // L3 Cache Op
   SC_L3_FlushWData             = 6'h15,  // L3 Cache Op (qwens sent)
   SC_L3_Evict                  = 6'h16,  // L3 Cache Op
   SC_L3_EvictWData             = 6'h17,  // L3 Cache Op (qwens sent)
   SC_L3_Prefetch               = 6'h18,  // L3 Cache Op
   SC_L3_Atomic                 = 6'h19,
   SC_L3_Atomic_2nd_pass        = 6'h1a,
   SC_L3_Fill                   = 6'h1b,
   SC_L3_Scrub                  = 6'h1c,

   // _Scratchpad from "l2" reqq to pipe:
   SC_Scp_Read                  = 6'h1d,
   SC_Scp_Write                 = 6'h1e,
   SC_Scp_WritePartial          = 6'h1f,
   SC_Scp_Atomic                = 6'h20,
   SC_Scp_Fill                  = 6'h21,
   SC_Scp_Scrub                 = 6'h22,
   SC_Scp_Zero                  = 6'h23,

   // _Idx cacheop from "l2" or "l3" reqq to pipe, address space passed separately
   SC_Idx_Abort                 = 6'h25,  // idxCopFSM abort
   SC_Idx_Read                  = 6'h26,  // read  tag state, tag and data rams at index
   SC_Idx_Write                 = 6'h27,  // write tag state, tag and data rams at index
   SC_Idx_All_Inv               = 6'h28,  // invalidate any space based on index and clear cbuf/rbuf valids (only at power up and major mode switches - only full sequence of these is possible)
   SC_Idx_L2_Inv                = 6'h29,  // invalidate L2  space based on index
   SC_Idx_L2_Flush              = 6'h2a,  // flush      L2  space based on index
   SC_Idx_L2_Evict              = 6'h2b,  // evict      L2  space based on index
   SC_Idx_L3_Inv                = 6'h2c,  // invalidate L3  space based on index
   SC_Idx_L3_Flush              = 6'h2d,  // flush      L3  space based on index
   SC_Idx_L3_Evict              = 6'h2e,  // evict      L3  space based on index
   SC_Sync                      = 6'h2f,  // reqq only request from cache op state machine 

   // _Mesh l2 reqq to mesh:
   SC_Mesh_Read                 = 6'h30,
   SC_Mesh_Write                = 6'h31,  // flushes and evicts
   SC_Mesh_Flush                = 6'h32,  // next level cacheop
   SC_Mesh_FlushToMem           = 6'h33,  // next level cacheop
   SC_Mesh_Evict                = 6'h34,  // next level cacheop
   SC_Mesh_EvictToMem           = 6'h35,  // next level cacheop
   SC_Mesh_Prefetch             = 6'h36,
   SC_Mesh_Victim               = 6'h37,  // capacity evict victims
   SC_Mesh_AtomicRsp            = 6'h38,  // L3 Atomic response

   // Reqq wait on another transaction 
   SC_Reqq_Wait                 = 6'h3a,
   SC_Reqq_Wait2                = 6'h3b,

   // All done (may be waiting to send neighborhood rsp before deallocate)
   SC_Reqq_AllocErr             = 6'h3c,
   SC_Reqq_Initial              = 6'h3d,
   SC_Reqq_Initial2             = 6'h3e,
   SC_Reqq_Done                 = 6'h3f
} sc_reqq_opcode_t;



// Enum of dataq rd ports
typedef enum logic [2:0] {
   SC_Dataq_Rd_Pipe        = 3'd0,
   SC_Dataq_Rd_To_L3_Mesh  = 3'd1,
   SC_Dataq_Rd_Rspmux_L2   = 3'd2,
   SC_Dataq_Rd_To_Sys_Mesh = 3'd3,
   SC_Dataq_Rd_Rspmux_L3   = 3'd4
} sc_dataq_rd_port_t;

// Enum of cbuf (coalesce buffer) requests
typedef enum logic [2:0] {
   SC_CBuf_Install         = 3'd0, // NOTE: CB.Evict to reqq can only occur if installing creates a cb capacity eviction
   SC_CBuf_MruUpdate       = 3'd1,
   SC_CBuf_Clear           = 3'd2,
   SC_CBuf_Replace         = 3'd3,
   SC_CBuf_Bypass          = 3'd4,
   SC_CBuf_Reserved        = 3'd5
} sc_cbuf_opcode_t;

// Enum of pipe AMO
typedef enum logic [3:0] {
   SC_AmoSwap         = 4'd0,
   SC_AmoAdd          = 4'd1,
   SC_AmoXor          = 4'd2,
   SC_AmoAnd          = 4'd3,
   SC_AmoOr           = 4'd4,
   SC_AmoMin          = 4'd5,
   SC_AmoMax          = 4'd6,
   SC_AmoMinU         = 4'd7,
   SC_AmoMaxU         = 4'd8,
   SC_AmoMinPs        = 4'd9,
   SC_AmoMaxPs        = 4'd10,
   SC_AmoCompExch     = 4'd11,
   SC_AmoReserved     = 4'd12
} sc_amo_opcode_t;


// Enum of pipe idx cop state machine
typedef enum logic [3:0] {
   SC_IdxCopSm_All_Inv        = 4'd0,
   SC_IdxCopSm_L2_Inv         = 4'd1,
   SC_IdxCopSm_L2_Flush       = 4'd2,
   SC_IdxCopSm_L2_Evict       = 4'd3,
   SC_IdxCopSm_L3_Inv         = 4'd4,
   SC_IdxCopSm_L3_Flush       = 4'd5,
   SC_IdxCopSm_L3_Evict       = 4'd6,
   SC_IdxCopSm_Dbg_Read       = 4'd7,
   SC_IdxCopSm_Dbg_Write      = 4'd8,
   SC_IdxCopSm_Scp_Zero       = 4'd9,
   SC_IdxCopSm_CB_Inv         = 4'd10
} sc_idx_cop_sm_opcode_t;


// Enum of decode err types
typedef enum logic [4:0] {

   SC_Err_None                    = 5'h0, // no error exists

   // pipe decode err types
   SC_PipeErr_L3ShireDecErr       = 5'h1, // l3 request to incorrect shire (shire_id mismatch)
   SC_PipeErr_ScpShireDecErr      = 5'h2, // scp request to incorrect shire (shire_id mismatch)
   SC_PipeErr_ScpRegionDecErr     = 5'h3, // scp request with wrong region id (region_id mismatch)
   SC_PipeErr_L2OpToNonEnRegion   = 5'h4, // l2  request when no cache allocated to l2 (normally this should never happen since l2 should always be enabled)
   SC_PipeErr_L3OpToNonEnRegion   = 5'h5, // l3  request when no cache allocated to l3
   SC_PipeErr_ScpOpToNonEnRegion  = 5'h6, // scp request when no cache allocated to scp
   SC_PipeErr_ScpSetOutOfRange    = 5'h7, // scp request to set/index that is out-of-range of allocated region
   SC_PipeErr_LockErr             = 5'h8, // attempted to lock last way

   // reqq decode err types
   SC_ReqqErr_MeshRespErr         = 5'h10, // Mesh response with error for read or write to mesh
   SC_ReqqErr_RemoteScpNotEnabled = 5'h11, // Scp request to a remote scratch pad region but esr_sc_remote_scp_is_disabled
   SC_ReqqErr_L2BypassL2Atomic    = 5'h12, // L2 atomic operation when L2 bypass is enabled
   SC_ReqqErr_L3BypassL3Atomic    = 5'h13, // L3 atomic operation when L3 bypass is enabled
   SC_ReqqErr_UnsupportedOp       = 5'h14, // Unsupported opcode
   SC_ReqqErr_IllegalPort         = 5'h15  // An L2 request came in with an illegal port (probably a message)

} sc_err_rsp_t;


// Enums of SC mesh for L2/L3 <> Mesh connections
typedef enum logic [4:0] {
   SC_L2_Mesh_Read        = 5'h00,  // msb indicates AR (0) vs AW (1) channels.  lsb 0 indicates base command Read/Write
   SC_L2_Mesh_Prefetch    = 5'h01,

   // CacheOps
   SC_L2_Mesh_Write       = 5'h10,
   SC_L2_Mesh_Flush       = 5'h11,  // write channel to push data downwards
   SC_L2_Mesh_FlushToMem  = 5'h12,  // write channel to push data downwards
   SC_L2_Mesh_Evict       = 5'h13,
   SC_L2_Mesh_EvictToMem  = 5'h14,

   // Atomics 
   SC_L2_Mesh_Atomic      = 5'h1a   // Always a write.  Sends data both ways. 
} sc_l2_mesh_opcode_t;

// Err_log code and sub-code
typedef enum logic [3:0] {
   SC_Err_Log_Ecc_SBE         = `SC_ERR_LOG_ECC_SBE,
   SC_Err_Log_Ecc_DBE         = `SC_ERR_LOG_ECC_DBE,
   SC_Err_Log_Ecc_Sat         = `SC_ERR_LOG_ECC_SAT,
   SC_Err_Log_Decode          = `SC_ERR_LOG_DECODE, 
   SC_Err_Log_Perfmon_Sat     = `SC_ERR_LOG_PERFMON_SAT 
} sc_err_log_code_t;

// Err_log code and sub-code
typedef enum logic [3:0] {
   SC_Err_Log_Tag_State       = 4'h0,
   SC_Err_Log_Tag             = 4'h1,
   SC_Err_Log_Data            = 4'h2,
   SC_Err_Log_Dataq           = 4'h3,
   SC_Err_Log_Ben             = 4'h4
} sc_err_log_ram_t;

typedef enum logic [2:0] {
   SC_Trace_None       = 3'h0,
   SC_Trace_Alloc_L2   = 3'h1,
   SC_Trace_Alloc_L3   = 3'h2,
   SC_Trace_TC         = 3'h3,
   SC_Trace_Rbuf       = 3'h4,
   SC_Trace_Mesh       = 3'h5,
   SC_Trace_Reqq       = 3'h6
} sc_trace_type_t;


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

// ESR structures
// --------------------------------------------------------------------------

// Struct for per bank ESRs
typedef struct packed {

   // reqq esrs
   logic [`SC_CGATE_DIS_SIZE-1:0]   esr_sc_clk_gate_disable;     // clock gate disable
   logic [`AXI_AXQOS_SIZE-1:0]      esr_sc_axi_qos;              // axi qos for outgoing requests from this shire
   logic                            esr_sc_cbuf_enable;
   logic                            esr_sc_remote_l3_enable;     // if enable=0, send to_l3 requests to to_sys master instead, L2 misses and evictions are sent to memory,  L3 flushes and evicts are dropped.
   logic                            esr_sc_remote_scp_enable;    // remote Scp operation are allowed
   logic                            esr_sc_l2_bypass;            // force all L2 requests to skip L2 cache and go to L3
   logic                            esr_sc_l3_bypass;            // force all L3 requests to skip L3 cache and go to mem
   logic [`SC_REQQ_ID_SIZE-1:0]     esr_sc_num_l3_reqq_entries;  // reqq L2/L3 split, zero indicates no L3 reqq entries, cannot be programmed to less than 2 L2 entries, if there is an L3 this must be programmed to at least 2 L3 entries.
   logic                            esr_sc_reqq_no_link_list;    // reqq slow-down option (do not allow link lists)
   logic                            esr_sc_ecc_scrub_enable;     // enable single bit ecc scrubbing
   logic [`SC_L3_YIELD_SIZE-1:0]    esr_sc_l3_yield_priority;    // allow l2 requests through occasionally

   // pipe esrs
   logic                                   esr_sc_idx_cop_sm_ctl_user_en;           // allow user-mode access to the esr_sc_idx_cop_sm_ctl_user register
   logic                                   esr_sc_ram_deep_sleep;                   // power down modes
   logic                                   esr_sc_ram_shut_down;                    // power down modes
   logic [`SC_RAM_DELAY_SIZE-1:0]          esr_sc_ram_delay;                        // pipe ram delay control 2,3,4 (default:2)
   logic                                   esr_sc_l2_rbuf_enable;                   // enable l2 reads to create rbuf entry
   logic                                   esr_sc_scp_rbuf_enable;                  // enable scp reads to create rbuf entry
   logic                                   esr_sc_zero_state_enable;                // enables zero_state tracking in state ram
   logic                                   esr_sc_allow_only_1_req_per_sub_bank;    // pipeline processing slow-down option
   logic                                   esr_sc_allow_only_1_req_per_bank;        // pipeline processing slow-down option
   logic [`SC_CBUF_SIZE-1:0]               esr_sc_cbuf_entry_enable;                // bit per cbuf entry enable
   logic                                   esr_sc_two_shire_aliasing_use_shire_lsb; // take two shire aliasing bit from shire_id[0] rather than shire_id[msb]
   logic                                   esr_sc_all_shire_aliasing;               // enable all shire aliasing (default: two-shire aliasing)
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_sub_bank_sel_b2;                  // l3 shire swizzle sub_bank sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_sub_bank_sel_b1;                  // l3 shire swizzle sub_bank sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_sub_bank_sel_b0;                  // l3 shire swizzle sub_bank sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_bank_sel_b2;                      // l3 shire swizzle bank     sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_bank_sel_b1;                      // l3 shire swizzle bank     sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_bank_sel_b0;                      // l3 shire swizzle bank     sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b5;                     // l3 shire swizzle shire_id sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b4;                     // l3 shire swizzle shire_id sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b3;                     // l3 shire swizzle shire_id sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b2;                     // l3 shire swizzle shire_id sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b1;                     // l3 shire swizzle shire_id sel
   logic [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0] esr_sc_shire_sel_b0;                     // l3 shire swizzle shire_id sel
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l2_set_base;                      // l2  cache config
   logic [`SC_SET_ALLOC_SIZE-1:0]          esr_sc_l2_set_size;                      // l2  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l2_set_mask;                      // l2  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l2_tag_mask;                      // l2  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l3_set_base;                      // l3  cache config
   logic [`SC_SET_ALLOC_SIZE-1:0]          esr_sc_l3_set_size;                      // l3  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l3_set_mask;                      // l3  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_l3_tag_mask;                      // l3  cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_scp_set_base;                     // scp cache config
   logic [`SC_SET_ALLOC_SIZE-1:0]          esr_sc_scp_set_size;                     // scp cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_scp_set_mask;                     // scp cache config
   logic [`SC_SET_ID_SIZE-1:0]             esr_sc_scp_tag_mask;                     // scp cache config

   // err log esrs
   logic                            esr_sc_err_rsp_enable;
   logic [`SC_ERR_INT_SIZE-1:0]     esr_sc_err_interrupt_enable;

   // trace control registers
   logic [`PA_SIZE-1:0]             esr_sc_trace_filter_address_enable; // enable bits for controlling which address is traced
   logic [`PA_SIZE-1:0]             esr_sc_trace_filter_address_value;  // trace this req if enabled address bits match this reg
   logic                            esr_sc_trace_filter_reqq_id_enable; // apply reqq_id filter to enable tracing of particular request
   logic [`SC_REQQ_ID_SIZE-1:0]     esr_sc_trace_filter_reqq_id;        // which reqq_id to trace
   logic                            esr_sc_trace_filter_port_enable;    // apply port filter to enable tracing of particular request
   logic [`SC_PORT_ID_SIZE-1:0]     esr_sc_trace_filter_port;           // which port to trace
   logic                            esr_sc_trace_filter_source_enable;  // apply source filter to enable tracing of particular request
   logic [`SC_REQQ_SOURCE_SIZE-1:0] esr_sc_trace_filter_source;         // which source to trace
   logic                            esr_sc_trace_filter_l2_enable;      // enable tracing reqs from l2
   logic                            esr_sc_trace_filter_l3_enable;      // enable tracing reqs from l3
   logic                            esr_sc_trace_filter_fsm_enable;     // enable tracing reqs from fsm
   logic [`SC_TRACE_EN_SIZE-1:0]    esr_sc_trace_type_hot_enable;       // enabled trace packet types

   // manual esr control
   logic                            wr_en_reqq_ctl; // used to know when to update L2/L3 reqq allocation at startup and register changes
   logic                            wr_en_idx_cop_sm_ctl;
   logic                            wr_en_idx_cop_sm_physical_index;
   logic                            wr_en_idx_cop_sm_data0;
   logic                            wr_en_idx_cop_sm_data1;
   logic                            wr_en_idx_cop_sm_ecc;
   logic                            wr_en_err_log_info;
   logic                            wr_en_sbe_dbe_counts;
   logic                            wr_en_reqq_debug_ctl;    // grabs and holds a reqq entry state for debug
   logic                            wr_en_perfmon_ctl_status;
   logic                            wr_en_perfmon_cyc_cntr;
   logic                            wr_en_perfmon_p0_cntr;
   logic                            wr_en_perfmon_p1_cntr;
   logic                            wr_en_perfmon_p0_qual;
   logic                            wr_en_perfmon_p1_qual;
   logic [`SC_DW_DATA_SIZE-1:0]     wr_data;
   logic                            access_in_flight;
} sc_bank_esr_ctl_t;


// Struct for pipe_idx_cop_sm ESR status
typedef struct packed {
   logic [`SC_ESR_STATUS_SIZE-1:0]  ctl;
   logic [`SC_ESR_STATUS_SIZE-1:0]  physical_index;
   logic [`SC_ESR_STATUS_SIZE-1:0]  data0;
   logic [`SC_ESR_STATUS_SIZE-1:0]  data1;
   logic [`SC_ESR_STATUS_SIZE-1:0]  ecc;
} sc_pipe_idx_cop_sm_esr_status_t;

// Struct for pipe_sbe_dbe ESR status
typedef struct packed {
   logic [`SC_SBE_COUNT_SIZE-1:0]   tag_state_ram_sbe_count; 
   logic [`SC_DBE_COUNT_SIZE-1:0]   tag_state_ram_dbe_count; 
   logic [`SC_SBE_COUNT_SIZE-1:0]   tag_ram_sbe_count;       
   logic [`SC_DBE_COUNT_SIZE-1:0]   tag_ram_dbe_count;       
   logic [`SC_SBE_COUNT_SIZE-1:0]   data_ram_sbe_count;      
   logic [`SC_DBE_COUNT_SIZE-1:0]   data_ram_dbe_count;      
} sc_pipe_sbe_dbe_esr_status_t;

// Struct for pipe ESR status
typedef struct packed {
   sc_pipe_idx_cop_sm_esr_status_t  pipe_idx_cop_sm_esr_status;
   sc_pipe_sbe_dbe_esr_status_t     pipe_sbe_dbe_esr_status;
} sc_pipe_esr_status_t;

// Struct for combined pipe/dataq sbe_dbe as per register definition
typedef struct packed {
   logic [`SC_DBE_COUNT_SIZE-1:0]   ben_ram_dbe_count;
   logic [`SC_SBE_COUNT_SIZE-1:0]   ben_ram_sbe_count;
   logic [`SC_DBE_COUNT_SIZE-1:0]   dataq_ram_dbe_count;
   logic [`SC_SBE_COUNT_SIZE-1:0]   dataq_ram_sbe_count;
   logic [`SC_DBE_COUNT_SIZE-1:0]   data_ram_dbe_count;      
   logic [`SC_SBE_COUNT_SIZE-1:0]   data_ram_sbe_count;      
   logic [`SC_DBE_COUNT_SIZE-1:0]   tag_ram_dbe_count;       
   logic [`SC_SBE_COUNT_SIZE-1:0]   tag_ram_sbe_count;       
   logic [`SC_DBE_COUNT_SIZE-1:0]   tag_state_ram_dbe_count; 
   logic [`SC_SBE_COUNT_SIZE-1:0]   tag_state_ram_sbe_count; 
} sc_sbe_dbe_esr_status_t;

// Struct for err_log ESR status
typedef struct packed {
   logic [`SC_ESR_STATUS_SIZE-1:0]  info;
   logic [`SC_ESR_STATUS_SIZE-1:0]  address;
   logic [`SC_ESR_STATUS_SIZE-1:0]  reqq_debug3;
   logic [`SC_ESR_STATUS_SIZE-1:0]  reqq_debug2;
   logic [`SC_ESR_STATUS_SIZE-1:0]  reqq_debug1;
   logic [`SC_ESR_STATUS_SIZE-1:0]  reqq_debug0;
} sc_err_log_esr_status_t;

// Struct for perfmon ESR status
typedef struct packed {
   logic [`SC_ESR_STATUS_SIZE-1:0]  ctl_status;
   logic [`SC_ESR_STATUS_SIZE-1:0]  cyc_cntr;
   logic [`SC_ESR_STATUS_SIZE-1:0]  p0_cntr;
   logic [`SC_ESR_STATUS_SIZE-1:0]  p1_cntr;
   logic [`SC_ESR_STATUS_SIZE-1:0]  p0_qual;
   logic [`SC_ESR_STATUS_SIZE-1:0]  p1_qual;
} sc_perfmon_esr_status_t;


// Struct-of-structs for response from bank to ESR interface
typedef struct packed {
   sc_pipe_idx_cop_sm_esr_status_t  pipe_idx_cop_sm;
   sc_err_log_esr_status_t          err_log;
   logic [`SC_ESR_STATUS_SIZE-1:0]  sbe_dbe_counts;
   sc_perfmon_esr_status_t          perfmon;
} sc_bank_esr_status_t;


// Struct-of-structs for ram_cfg since it rolled over a register
typedef struct packed {
   logic [`SHIRE_DFT_SCAN_DUMP_CNTL_WIDTH-1:0] dft__scan_dump_cntl;
   logic                            dft__ram_rei;
   logic                            dft__ram_wei;
   esr_shire_cache_ram_cfg1_t       cfg1;
   esr_shire_cache_ram_cfg2_t       cfg2;
   esr_shire_cache_ram_cfg3_t       cfg3;
   esr_shire_cache_ram_cfg4_t       cfg4;
} esr_shire_cache_ram_cfg_t;


// jtag control (also includes above typedef)
// --------------------------------------------------------------------------

typedef struct packed {
   // 
   logic trstn;       // resetl;
   logic ena;         // reg_en;
   logic shiftdr;     // shiftdr_en;
   logic updatedr;    // updatedr_en;
   logic capturedr;   // capturedr_en;
} et_tdr_cntl_t;


typedef struct packed {
   logic tck;
   logic resetl;
   logic reg_en;
   logic shiftdr_en;
   logic updatedr_en;
   logic capturedr_en;
} sc_jtag_ram_cfg_t;


typedef struct packed {
   logic [3:0]                      clk_sel;
   logic                            clk_off;
   logic                            pll_reset;
   logic                            dll_reset;
   logic                            dll_enable;
   logic [`NUM_NEIGH-1:0]           clk_gate_neigh_disable;
   logic                            clk_gate_debug_disable;
} shire_tdr_clk_cntl_t;

typedef struct packed {
   logic                            reset_warm_in;
   logic                            gpio_ndmreset_in;
   logic                            ndmreset_in;
   logic                            dmactive_in;
   logic                            reset_cold_in;
   logic                            reset_n_system;
   logic                            reset_n_system_debug;
} shire_tdr_reset_cntl_t;


typedef struct packed {
   logic [`NUM_NEIGH-1:0]           tbox_en;
   logic                            rbox_en;
   logic [`NUM_NEIGH-1:0]           neigh_en;
   logic                            cache_en;
} shire_tdr_enable_cntl_t;


// err logging structures
// --------------------------------------------------------------------------

// err_log_info ECC format
typedef struct packed {
   logic [11:0]                     reserved_63_52;
   sc_err_log_ram_t                 ram;
   logic [7:0]                      bits;
   logic [31:0]                     index;
   sc_err_log_code_t                code; 
   logic                            imprecise;
   logic                            enabled;
   logic                            multi;
   logic                            valid;
} sc_err_log_info_ecc_t;

// err_log_info ECC counter saturated format
typedef struct packed {
   logic [10:0]                     reserved_63_53;
   logic                            dbe;
   sc_err_log_ram_t                 ram;
   logic [39:0]                     reserved_47_8;
   sc_err_log_code_t                code; 
   logic                            imprecise;
   logic                            enabled;
   logic                            multi;
   logic                            valid;
} sc_err_log_info_ecc_sat_t;

// err_log_info decode error format
typedef struct packed {
   logic [3:0]                             reserved_63_60;
   logic [4-1:0]                           port;
   logic [12-1:0]                          et_link_source;
   logic [12-1:0]                          et_link_tag_id;
   logic [8-1:0]                           reqq_id;
   sc_reqq_source_t                        src;
   logic                                   opcode_reserved; 
   sc_reqq_orig_opcode_t                   opcode; 
   logic [8-$bits(sc_err_rsp_t)-1:0]       err_type_reserved; 
   sc_err_rsp_t                            err_type; 
   sc_err_log_code_t                       code; 
   logic                                   imprecise;
   logic                                   enabled;
   logic                                   multi;
   logic                                   valid;
} sc_err_log_info_decode_t;

// err_log_info Performance monitor counter overflow
typedef struct packed {
   logic [52:0]                     reserved_63_11;
   logic                            p1_cntr_overflow;
   logic                            p0_cntr_overflow;
   logic                            cyc_cntr_overflow;
   sc_err_log_code_t                code; 
   logic                            imprecise;
   logic                            enabled;
   logic                            multi;
   logic                            valid;
} sc_err_log_info_perfmon_sat_t;

// err_log_info union of all formats
typedef union packed {
    sc_err_log_info_ecc_t         ecc_err;
    sc_err_log_info_ecc_sat_t     ecc_sat_err;
    sc_err_log_info_decode_t      decode_err;
    sc_err_log_info_perfmon_sat_t perfmon_sat_err;
} sc_err_log_info_t;

// error logging internal struction.  One per error source
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   sc_err_log_info_t                info;
   logic                            address_valid;
   logic [`SC_ESR_STATUS_SIZE-1:0]  address;
} sc_err_log_err_t;

// Create a static version of above that has fixed info size rather 
// that a union.  The allows union size mismatches to be reported as
// human readable errors rather than a $bits() dynamic error. 
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_ESR_STATUS_SIZE-1:0]  info;
   logic                            address_valid;
   logic [`SC_ESR_STATUS_SIZE-1:0]  address;
} sc_err_log_err_static_t;

// reqq <-> xbar structures
// --------------------------------------------------------------------------

// Struct for requests from xbar to bank reqq
typedef struct packed {
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   et_link_req_info_t               req_info;
} sc_xbar_req_t;

// Struct for responses from bank reqq to xbar
typedef struct packed {
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;        // port_id is used in uncacheable only
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic                            src_is_uc;
   et_link_rsp_info_t               rsp_info;
} sc_xbar_rsp_t;

// reqq <-> pipe structures
// --------------------------------------------------------------------------

// Struct for reqq requests to pipe
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   sc_reqq_opcode_t                 opcode;         // Opcode request
   logic [`PA_SIZE-1:0]             address;        // Address of the request
   sc_address_space_t               address_space;  // Address space of request (legal subset: L2, L3, Scp, Idx)
   logic                            zero_data;      // indicates that all data being written is zero
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;           // qword enable
   sc_size_t                        size;           // Size of the transaction
   logic [`SC_SUB_BANK_ID_SIZE-1:0] sub_bank_id;    // sub_bank_id (used for timing closure in _rqa stage of pipe)
} sc_reqq_pipe_req_t;


// Struct for pipe sub_bank busy status to reqq
typedef struct packed {
   logic [`SC_SUB_BANKS-1:0]        sub_bank_busy; // multi-hot - busy signal per sub-bank indicating pipe cannot take request
} sc_pipe_busy_t;


// Struct for pipe tag ram rsp status to reqq
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            hit;
   logic [`PA_SIZE-1:0]             orig_address; // original request address, so reqq does not have to look it up
   logic                            victim;
   logic                            victim_silent;
   logic [`PA_SIZE-1:0]             victim_address;
   sc_address_space_t               victim_address_space;
   logic                            victim_write_around;
   logic [`SC_LINE_QW_SIZE-1:0]     victim_qwen; 
   sc_err_rsp_t                     err_rsp;
   logic [`SC_SET_ID_SIZE-1:0]      tag_index; // used for error logging only
   logic                            bubble; // used for perf only
   sc_reqq_opcode_t                 opcode; // used for perf only
   logic [`SC_WAY_ID_SIZE-1:0]      tag_ram_hit_way;   // used for trace only
   logic [`SC_LINE_QW_SIZE-1:0]     tag_ram_hit_qwen;  // used for trace only
   logic                            tag_state_ram_sbe;
   logic                            tag_state_ram_dbe;
   logic [`SC_WAYS-1:0]             tag_ram_sbe;
   logic [`SC_WAYS-1:0]             tag_ram_dbe;
} sc_pipe_tag_rsp_t;

// Struct for pipe data ram rsp pre_status to reqq
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   sc_pipe_data_rsp_type_t          rsp_type;           // normal rsp or victim data
} sc_pipe_data_rsp_pre_t;

// Struct for pipe data ram rsp status to reqq
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   sc_pipe_data_rsp_type_t          rsp_type;           // normal rsp or victim data
   logic                            tag_sbe;            // used for scrub
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;               // qword enables (can be partials if hit in coel buf)
   logic [`SC_DATA_RAM_ADDR_SIZE-1:0] data_index;       // used for error logging only
   logic [`SC_LINE_DW_SIZE-1:0]     data_ram_sbe;
   logic [`SC_LINE_DW_SIZE-1:0]     data_ram_dbe;
} sc_pipe_data_rsp_t;

typedef struct packed {
   logic                            tag_state_ram_sbe_overflow;
   logic                            tag_state_ram_dbe_overflow;
   logic                            tag_ram_sbe_overflow;
   logic                            tag_ram_dbe_overflow;
   logic                            data_ram_sbe_overflow;
   logic                            data_ram_dbe_overflow;
} sc_pipe_sbe_dbe_overflow_t;

typedef struct packed {
   logic                            p1_cntr_overflow;
   logic                            p0_cntr_overflow;
   logic                            cyc_cntr_overflow;
} sc_perfmon_cntr_overflow_t;

// Struct for post-pipe data ram rsp fixup generated by reqq
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;
   sc_address_space_t               address_space;// L2, L3, scp, index, msg
   sc_reqq_source_t                 reqq_source;  // L2, L3, FSM 
   sc_mesh_dest_t                   which_mesh;   // Which mesh to send a victim to
   logic                            disable_mesh_bypass; // Victim address already outstanding in the mesh
   logic                            redirect_normal_rsp_to_mesh; // L3 atomics send data responses to to_sys mesh
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;
   sc_pipe_data_rsp_type_t          rsp_type;           // normal rsp or victim data
} sc_pipe_data_rsp_modifier_t;


// Struct for pipe cbuf (coalescing buffer) to reqq
// Only sent for write arounds to indicate a cbuf (coalescing buffer) evict
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            victim;
   logic [`PA_SIZE-1:0]             victim_address;
   // implied victim_write_around = 1
} sc_pipe_cbuf_rsp_t;

// Struct for pipe dataq read request
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            partial; // indicates that the bens from the dataqb ram should be read
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;    // qword rd enables (only read the qwords required for the partial operation)
} sc_pipe_dataq_rd_req_t;

// Struct for dataq data
typedef struct packed {
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
} sc_data_t;
typedef struct packed {
   logic [`SC_LINE_BYTE_SIZE-1:0]   ben;
} sc_ben_t;

// Struct for pipe dataq read response
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
   logic [`SC_LINE_BYTE_SIZE-1:0]   ben; // byte write enable (only used for *_WritePartial requests)
   logic                            err;
} sc_pipe_dataq_rd_rsp_t;


typedef struct packed {
   // writes at head of link-lists clear entries using "hot" vector
   // the clear *_hot signals will occur before or as late as coincident with the start of the same address
   logic [`SC_RBUF_SIZE-1:0]                    rbuf_clear_mem_valid_hot;
   logic [`SC_RBUF_SIZE-1:0]                    rbuf_clear_mem_scp;                 // 0=L2, 1=Scp
   logic [`SC_RBUF_SIZE-1:0][`PA_SIZE-1:0]      rbuf_clear_mem_address;             // NOTE: scp/address only used for assertion, any clear must be in rbuf or in dv_recently_evicted_queue
   logic [`SC_RBUF_SIZE-1:0]                    rbuf_clear_pending_install_hot;
   logic [`SC_RBUF_SIZE-1:0]                    rbuf_clear_pending_install_scp;     // 0=L2, 1=Scp
   logic [`SC_RBUF_SIZE-1:0][`PA_SIZE-1:0]      rbuf_clear_pending_install_address; // NOTE: scp/address only used for assertion, any clear must be in rbuf or in dv_recently_evicted_queue
} sc_pipe_rbuf_clear_req_t;



// Struct from pipe read buffer to reqq state and status
typedef struct packed {
   // state
   logic [`SC_RBUF_SIZE-1:0]        rbuf_mem_valid;
   logic [`SC_RBUF_SIZE-1:0]        rbuf_mem_scp;        // 0=L2, 1=Scp
   logic [`SC_RBUF_SIZE-1:0][`PA_SIZE-1:0] rbuf_mem;

   // eviction notice
   logic                            rbuf_evict;
   logic                            rbuf_evict_scp;      // 0=L2, 1=Scp
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_evict_id;       // used for entry matches
   logic [`PA_SIZE-1:0]             rbuf_evict_address;  // used for alloc matches

   // install notice
   logic                            rbuf_install;
   logic                            rbuf_install_scp;     // 0=L2, 1=Scp
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_install_id;      // stored in reqq
   logic [`PA_SIZE-1:0]             rbuf_install_address; // used for all matches
                                                          
   logic                            rbuf_pending_install;
   logic                            rbuf_pending_install_scp;     // 0=L2, 1=Scp
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_pending_install_id;      // stored in reqq
   logic [`PA_SIZE-1:0]             rbuf_pending_install_address; // used for all matches
                                                                   
   logic [`SC_RBUF_PENDING_INSTALL_SIZE-1:0][`SC_RBUF_SIZE-1:0] rbuf_pending_install_pipe_valid;      // one-hot indicating which rbuf entry is going to be installed
   logic [`SC_RBUF_PENDING_INSTALL_SIZE-1:0]                    rbuf_pending_install_pipe_scp;        // 0=L2, 1=Scp
   logic [`SC_RBUF_PENDING_INSTALL_SIZE-1:0][`PA_SIZE-1:0]      rbuf_pending_install_pipe_address;
                                                         
} sc_pipe_rbuf_state_t;


// Struct reqq to read buffer read request
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            l3_source;
   logic                            scp;      // 0=L2, 1=Scp (used for assertion only)
   logic [`PA_SIZE-1:0]             address;  // used for assertion only
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_id;
} sc_pipe_rbuf_rd_req_t;

// Struct read buffer to reqq read response
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            l3_source;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
} sc_pipe_rbuf_rd_rsp_t;


// Internal structures within reqq
// --------------------------------------------------------------------------

// map l2 and l3 requests to a common type that is the superset of both
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]        trans_id;
   logic [`SC_PORT_ID_SIZE-1:0]         port_id;
   logic [`SC_REQQ_TAG_ID_SIZE-1:0]     id;               // id
   logic [`SC_REQQ_SOURCE_SIZE-1:0]     source;           // Source
   logic [`SC_REQQ_QOS_SIZE-1:0]        qos;              // QOS forwarded from L3 -> Mem
   et_link_req_opcode_t                 opcode;           // Request Opcode
   logic [`ET_LINK_SUBOPCODE_SIZE-1:0]  subopcode;        // Opcode specific field 
   logic [`PA_SIZE-1:0]                 address;          // Address of the request
   logic                                wdata;            // Request carries data
   logic [`SC_LINE_DATA_SIZE-1:0]       data;             // Write data
   logic [`SC_LINE_BYTE_SIZE-1:0]       ben;              // Byte write enable
   logic [`SC_LINE_QW_SIZE-1:0]         qwen;             // QWord Enable (for Write-Around requests, SCP or L3 or messages )
   sc_size_t                            size;             // Size of the transaction
   logic                                write_is_partial; // A write opcode is partial.
} sc_reqq_bank_req_t;

typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic                            valid;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;     // Dataq id, Atomic make use of paired reqq for dataq
   logic [`SC_REQQ_TAG_ID_SIZE-1:0] tag_id;
   logic [`SC_REQQ_SOURCE_SIZE-1:0] source;       // ET link Source
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;
   logic [`SC_REQQ_QOS_SIZE-1:0]    qos;          // QOS forwarded from L3 -> Mem
   sc_reqq_orig_opcode_t            orig_opcode;  // Original opcode
   sc_address_space_t               address_space;// L2, L3, scp, index, msg
   sc_reqq_source_t                 reqq_source;  // L2, L3, FSM
   sc_cache_level_t                 cache_level_start;  // Level the cache_op needs to operate on
   sc_cache_level_t                 cache_level_dest;   // Level the cache_op/scp needs to operate on
   logic                            scp_is_remote;   // Scp is remote
   logic [`SC_NEIGH_COOP_MASK_SIZE-1:0] readcoop_dest;// Read cooperative broadcast
   logic [`PA_SIZE-1:0]             address;      // Address of the request or generated victim
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;         // qword enable
   logic [`SC_LINE_QW_SIZE-1:0]     rspmux_qwen;  // response to send back (read response qwen)
   sc_size_t                        size;         // Size of the transaction
   logic                            zero_data;    // All enabled data is zero
   // State
   sc_reqq_opcode_t                 opcode;              // Current action
   // Responses
   logic                            rspmux_l2_eligible;  // Neigh response waiting to get picked 
   logic                            rspmux_l3_eligible;  // L3 slave response Waiting to get picked 
   logic                            rspmux_sent;         // Response has been sent
   // Eligibility
   logic                            pipe_req_eligible;       // Have something for the pipeline to do
   logic [`SC_SUB_BANK_ID_SIZE-1:0] subbank;                 // Which pipe subbank
   logic                            to_l3_mesh_req_eligible; // Have something for the mesh to do
   logic                            to_sys_mesh_req_eligible;// Have something for the mesh to do
   sc_mesh_dest_t                   which_mesh;              // Which mesh to send a victim to
   logic                            opcode_destined_for_mesh;// Opcode is destined for the mesh
   logic                            rbuf_req_eligible;       // Have something for the rbuf to do
   logic                            inflight;
   logic                            data_ready;
   logic                            wait_for_dataq;          // Wait for dataq read or write before dealloc
   // Linked list tracking
   logic                            dep_valid;               // there is an entry ahead of me
   logic [`SC_REQQ_ID_SIZE-1:0]     dep_id;                  // entry ahead of me
   logic                            dep_tail;                // youngest in list
   logic                            dep_meshll_tail;         // youngest entry headed for mesh in meshll
   logic                            dep_non_meshll_head;     // oldest entry not in meshll list
   logic                            dep_in_meshll;           // This entry is in the meshll portion of the linked list
   logic                            dont_depend_on_me;  // Entry is past the point of stalling dependencies 
                                                        // no longer a linked list dependency candidate
   // Read buffer state tracking
   logic                            rbuf_valid;            // This entry is currently in the rbuf
   logic                            rbuf_pending_valid;    // This entry is currently being added to the rbuf
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_id;               // Which rbuf ID the entry is in 

   // Paired entry tracking
   logic [`SC_REQQ_ID_SIZE-1:0]     paired_id;     // Other half if atomic or partial write pairs

   // Error 
   logic                            err_detected;  // Error detected
   logic                            err_zero_data; // Error zero data response
   logic                            err_imprecise; // Error cannot be sent back with a response

   logic                            traced;        // This entry should be traced for debug

} sc_reqq_entry_state_t;


// Information for allocating a reqq entry
typedef struct packed {
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;      // If first half Atomic or Write Around, watch for next allocate and this id is the second half pair
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_TAG_ID_SIZE-1:0] tag_id;
   logic [`SC_REQQ_SOURCE_SIZE-1:0] source;       // ET link Source
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;
   logic [`SC_REQQ_QOS_SIZE-1:0]    qos;          // QOS forwarded from L3 -> Mem
   sc_address_space_t               address_space;// L2, L3, scp, index, msg
   sc_reqq_source_t                 reqq_source;  // L2, L3, FSM
   sc_cache_level_t                 cache_level_start;  // Level the cache_op needs to operate on
   sc_cache_level_t                 cache_level_dest;   // Level the cache_op/scp needs to operate on
   logic                            scp_is_remote;      // Scp is remote
   logic [`SC_NEIGH_COOP_MASK_SIZE-1:0] readcoop_dest;// Read cooperative broadcast
   logic [`PA_SIZE-1:0]             address;      // Address of the request or generated victim
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;         // qword enable
   sc_size_t                        size;         // Size of the transaction
   sc_reqq_orig_opcode_t            orig_opcode;  // Which opcode came in
   sc_reqq_opcode_t                 opcode;       // Which initial opcode to use
   logic                            zero_data;    // Input data is all zeros
   logic                            scpFill_source_is_scp; // ScpFill PA source is to Scp address
   logic                            dep_valid;    // Dependent upon someone
   logic                            dep_non_meshll_head; // Dependent upon someone that is in meshll
   logic [`SC_REQQ_ID_SIZE-1:0]     dep_id;       // Which entry dependent upon
   logic [`SC_REQQ_ID_SIZE-1:0]     paired_id;    // When allocating a second half, this is id of first half
   logic                            rbuf_valid;   // Entry is in the read buffer 
   logic                            rbuf_pending_valid;    // This entry is currently being added to the rbuf
   logic [`SC_RBUF_ID_SIZE-1:0]     rbuf_id;      // Which read buffer entry
   logic                            err_valid;    // Alloc has a decode error
   sc_err_rsp_t                     err_type;     // Which err
   logic                            err_imprecise;// The decode error is imprecise (message to non-existent port)
   logic                            traced;       // This entry should be traced for debug
} sc_reqq_alloc_t;

// Internal structures within mesh interface
// --------------------------------------------------------------------------
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
   logic                            with_data;
   logic                            rd_ben;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;
   logic [`SC_LINE_BYTE_SIZE-1:0]   ben;
} sc_reqq_mesh_req_t;

typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
   logic                            zero_data;
   logic                            err;
} sc_reqq_mesh_rsp_t;

// Internal structures within rspmux 
// --------------------------------------------------------------------------
// rspmux structures
// Struct for responses from bank reqq to xbar
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;    // Response data
   logic                            with_data;
   logic                            zero_data;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;
} sc_rspmux_t;

typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;    // Response data
   logic                            with_data;
   logic [`SC_PORTS-1:0]            dest_hot;
   logic [`SC_PORT_ID_SIZE-1:0]     port_id;
   logic                            traced; 
   sc_xbar_rsp_t                    xbar_info;
} sc_rspmux_long_t;

// rspmux sent
typedef struct packed {
   logic                            valid;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            traced; 
} sc_rspmux_sent_t;

// Internal structures within dataq 
// --------------------------------------------------------------------------
// dataq structures
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
   logic                            wr_ben;
   logic [`SC_LINE_BYTE_SIZE-1:0]   ben;
} sc_dataq_wr_t;

typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     dataq_id;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;
   sc_dataq_rd_port_t               rd_port_id;
   logic                            rd_ben;
} sc_dataq_rd_t;

// Internal structures within pipe
// --------------------------------------------------------------------------

typedef struct packed {
   // following variables pipelined from _tap to _dc
   logic [`SC_SET_ID_SIZE-1:0]      pipe_set;
   logic [`SC_SUB_BANKS-1:0]        pipe_sub_bank;
   sc_address_space_t               pipe_idx_address_space;  // Address space of Idx_Read request
   logic                            pipe_dec_err;
   sc_err_rsp_t                     pipe_dec_err_type;
   logic                            pipe_scp_read;  // used for pipe_squash
   logic                            pipe_bubble;  // used for perf counting only


   // following variables are pipelined from _tc to _dc
   logic                            idx_write;
   logic                            tag_hit;
   logic [`SC_WAY_ID_SIZE-1:0]      tag_ram_hit_way;
   logic                            tag_sbe;
   logic                            victim;
   logic                            data_rd_req;
   logic                            data_wr_req;
   logic                            data_wr_zero;
   logic [`SC_LINE_QW_SIZE-1:0]     data_wr_qwen;
   logic                            dataq_rd_req_en;
   logic                            dataq_rd_req_partial;
   logic [`SC_LINE_QW_SIZE-1:0]     dataq_sel_qwen;
   logic                            merge_wr_data_in_rd_rsp;
   logic                            merge_rd_data_in_wr_req;
   logic                            data_rsp_req;
   logic                            data_rsp_zero;
   logic [`SC_LINE_QW_SIZE-1:0]     data_rsp_qwen;
} sc_pipe_info_t;


typedef struct packed {
   logic                            valid;
   logic                            locked;
   logic                            zero;
   logic [`SC_LINE_QW_SIZE-1:0]     qwen;
} sc_pipe_way_state;


typedef struct packed {
   sc_pipe_way_state [`SC_WAYS-1:0]   way_state;
   logic [`SC_TAG_STATE_LRU_SIZE-1:0] lru_state;
} sc_pipe_tag_state_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]           rd_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]            rd_reqq_id;
   logic [`SC_TAG_STATE_RAM_ADDR_SIZE-1:0] rd_addr;
   logic [`DV_TRANS_ID_SIZE-1:0]           wr_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]            wr_reqq_id;
   logic [`SC_TAG_STATE_RAM_ADDR_SIZE-1:0] wr_addr;
   logic [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] wr_data;
} sc_pipe_tag_state_ram_req_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]           trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]            reqq_id;
   logic [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] rd_data;
} sc_pipe_tag_state_ram_rsp_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]      rd_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       rd_reqq_id;
   logic [`SC_TAG_RAM_ADDR_SIZE-1:0]  rd_addr;
   logic [`DV_TRANS_ID_SIZE-1:0]      wr_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       wr_reqq_id;
   logic [`SC_TAG_RAM_ADDR_SIZE-1:0]  wr_addr;
   logic [`SC_TAG_RAM_DATA_SIZE-1:0]  wr_data;
} sc_pipe_tag_ram_req_t;              
                                      

typedef struct packed {               
   logic [`DV_TRANS_ID_SIZE-1:0]      trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       reqq_id;
   logic [`SC_TAG_RAM_DATA_SIZE-1:0]  rd_data;
} sc_pipe_tag_ram_rsp_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]      rd_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       rd_reqq_id;
   logic [`SC_DATA_RAM_ADDR_SIZE-1:0] rd_addr;
   logic [`DV_TRANS_ID_SIZE-1:0]      wr_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       wr_reqq_id;
   logic [`SC_DATA_RAM_ADDR_SIZE-1:0] wr_addr;
   logic [`SC_DATA_RAM_DATA_SIZE-1:0] wr_data;
   logic [`SC_LINE_QW_SIZE-1:0]       wr_qwen;
   logic                              wr_xcheck_disable; // dv-only field
   logic [`DV_TRANS_ID_SIZE-1:0]      amo_wr_trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       amo_wr_reqq_id;
   logic [`SC_DATA_RAM_ADDR_SIZE-1:0] amo_wr_addr;
   logic [`SC_DATA_RAM_DATA_SIZE-1:0] amo_wr_data;
   logic [`SC_LINE_QW_SIZE-1:0]       amo_wr_qwen;
} sc_pipe_data_ram_req_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]      trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]       reqq_id;
   logic [`SC_DATA_RAM_DATA_SIZE-1:0] rd_data;
} sc_pipe_data_ram_rsp_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            rsp_required;
   sc_cbuf_opcode_t                 opcode;
   logic [`PA_SIZE-1:0]             install_address;
   logic [`PA_SIZE-1:0]             clear_address;
} sc_pipe_cbuf_req_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            scp;     // 0=L2, 1=Scp
   logic [`PA_SIZE-1:0]             address;
} sc_pipe_rbuf_install_req_t;


typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            scp;     // 0=L2, 1=Scp
   logic [`PA_SIZE-1:0]             address;
} sc_pipe_rbuf_evict_req_t;


typedef struct packed { // used to send addr/data from sub_bank to rbuf at _dc stage
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   logic                            scp;       // 0=L2, 1=Scp
   sc_reqq_opcode_t                 opcode;    // only used for assertion
   logic [`PA_SIZE-1:0]             address;
   logic [`SC_LINE_DATA_SIZE-1:0]   data;
} sc_pipe_rbuf_data_req_t;


typedef struct packed {
   logic                            dest;    // 0=L2,     1=L3
   logic                            size;    // 0=32b,    1=64b
   logic                            sc_vec;  // 0=scalar, 1=vec/simd
   sc_amo_opcode_t                  opcode;
} sc_pipe_amo_conf_req_t;


// index cache op structures
// --------------------------------------------------------------------------

// Struct for request from pipe_idx_cop_sm to reqq
typedef struct packed {
   logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
   logic                            sync_flush_victims;  // CB_Inv requires flushing all currently outstanding mesh writes before acking SCSync
   sc_reqq_opcode_t                 opcode;              // Opcode request
   logic [`PA_SIZE-1:0]             address;             // Address of the request
} sc_pipe_idx_cop_req_t;


// Struct for pipe_idx_cop_sm Dbg_Write to pipe_sub_bank
typedef struct packed {
   logic                                   ecc_wr_en;
   logic                                   tag_state_en;
   logic [`SC_TAG_STATE_DATA_SIZE-1:0]     tag_state;
   logic [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state_and_ecc;
   logic                                   tags_en;
   logic [`SC_TAGS_DATA_SIZE-1:0]          tags;
   logic [`SC_TAG_RAM_DATA_SIZE-1:0]       tags_and_ecc;
   logic                                   data_en;
   logic [`SC_LINE_DATA_SIZE-1:0]          data;
   logic [`SC_DATA_RAM_DATA_SIZE-1:0]      data_and_ecc;
   logic [`SC_LINE_QW_SIZE-1:0]            data_qwen; 
} sc_idx_cop_sm_dbg_write_t;

// Struct for pipe_sub_bank Dbg_Read to pipe_idx_cop_sm
typedef struct packed {
   logic                                   tag_state_and_ecc_valid;
   logic [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state_and_ecc;
   sc_address_space_t                      tag_state_space;
   logic                                   tags_and_ecc_valid;
   logic [`SC_TAG_RAM_DATA_SIZE-1:0]       tags_and_ecc;
   logic                                   data_and_ecc_valid;
   logic [`SC_DATA_RAM_DATA_SIZE-1:0]      data_and_ecc;
} sc_idx_cop_sm_dbg_read_t;


// Struct from pipe coalescing buffer to idx_cop sm
typedef struct packed {
   // state
   logic [`SC_CBUF_SIZE-1:0]               cbuf_mem_valid;
   logic [`SC_CBUF_SIZE-1:0][`PA_SIZE-1:0] cbuf_mem;
} sc_pipe_cbuf_state_t;


// BIST
// --------------------------------------------------------------------------
typedef struct packed {
   logic                                mbist_on; // use for clock gaters if required
   logic                                mbs_sel;
   logic                                mbs_rd_en;
   logic                                mbs_wr_en;
   logic [`SC_MBS_ADDR_SIZE-1:0]        mbs_addr;
   logic [`SC_MBS_DATA_SIZE-1:0]        mbs_wdata;
   logic                                mbt_sel;
   logic                                mbt_rd_en;
   logic                                mbt_wr_en;
   logic [`SC_MBT_ADDR_SIZE-1:0]        mbt_addr;
   logic [`SC_MBT_DATA_SIZE-1:0]        mbt_wdata;
   logic                                mbd_sel;
   logic                                mbd_rd_en;
   logic                                mbd_wr_en;
   logic [`SC_MBD_ADDR_SIZE-1:0]        mbd_addr;
   logic [`SC_MBD_DATA_SIZE-1:0]        mbd_wdata;
} sc_pipe_bist_req_t;

typedef struct packed {
   logic [`SC_MBS_DATA_SIZE-1:0]        mbs_rdata;
   logic [`SC_MBT_DATA_SIZE-1:0]        mbt_rdata;
   logic [`SC_MBD_DATA_SIZE-1:0]        mbd_rdata;
} sc_pipe_bist_rsp_t;

typedef struct packed {
   logic                                mbist_on; // use for clock gaters if required
   logic                                mbi_sel;
   logic                                mbi_rd_en;
   logic                                mbi_wr_en;
   logic [`SC_MBI_ADDR_SIZE-1:0]        mbi_addr;
   logic [`SC_MBI_DATA_SIZE-1:0]        mbi_wdata;
} sc_icache_bist_req_t;

typedef struct packed {
   logic [`SC_MBI_DATA_SIZE-1:0]        mbi_rdata;
} sc_icache_bist_rsp_t;

// Performance monitor
// --------------------------------------------------------------------------
typedef struct packed {
   logic                                 cgater;         // Perfmon clk gater
   logic [`SC_PERFMON_EV_QUAL_SIZE-1:0]  ev_mode;        // Event mode
   logic [`SC_PERFMON_RSRC_REQ_SIZE-1:0] rsrc_mode;      // Resource mode
} sc_perfmon_req_info_t;

typedef struct packed {
   logic                            ev_rbuf_l2_hit;            // Rbuf L2 hit
   logic                            ev_rbuf_scp_hit;           // Rbuf Scp hit
   logic                            ev_msgsenddata;            // Msgsenddata allocated

   logic                            ev_tag_bubble;             // Tag bubble
   logic                            ev_tag_hit;                // Tag Hit
   logic                            ev_tag_miss;               // Tag Miss
   logic                            ev_tag_evict_dirty;        // Tag evict dirty
   logic                            ev_tag_evict_write_around; // Tag evict write around
   logic                            ev_tag_evict_none;         // Tag evict none
   logic [`SC_LINE_QW_SIZE-1:0]     ev_tag_victim_qwen;        // Victim qwen 
   sc_reqq_opcode_t                 ev_tag_opcode;             // Tag opcode 
} sc_perfmon_rsp_ev_info_t;

typedef struct packed {
   logic [`SC_PERFMON_RSRC_RSP_SIZE-1:0] rsrc_cntr;  // Resource counters
} sc_perfmon_rsp_rsrc_info_t;

typedef struct packed {
   sc_perfmon_rsp_ev_info_t         ev_rsp;         // Event Responses
   sc_perfmon_rsp_rsrc_info_t       rsrc_rsp;       // Resource Responses
} sc_perfmon_rsp_info_t;

// trace
// --------------------------------------------------------------------------
// Trace alloc
typedef struct packed {
   logic [`SC_LINE_QW_SIZE-1:0]        qwen;         // qword enable
   logic [`PA_SIZE-6-1:0]              address;      // Address of the request or generated victim
   logic [2:0]                         port_id;      // FUTURE only supports 8 neigh (not 8 neigh + RBOX)
   sc_reqq_orig_opcode_t               orig_opcode;  // Which opcode came in
   logic [`SC_REQQ_ID_SIZE-1:0]        reqq_id;
   sc_trace_type_t                     trace_type;
} sc_trace_alloc_t;

// TC status
typedef struct packed {
   logic [$bits(sc_trace_alloc_t)
          -(`PA_SIZE-6) 
          -4
          -`SC_WAY_ID_SIZE
          -2
          -`SC_REQQ_ID_SIZE
          -$bits(sc_trace_type_t)
          -1:0]                        padding;
   logic [`PA_SIZE-6-1:0]              address;
   logic [`SC_LINE_QW_SIZE-1:0]        qwen; 
   logic [`SC_WAY_ID_SIZE-1:0]         way;
   logic                               victim;
   logic                               hit;
   logic [`SC_REQQ_ID_SIZE-1:0]        reqq_id;
   sc_trace_type_t                     trace_type;
} sc_trace_tc_t;

// rbuf
typedef struct packed {
   logic [$bits(sc_trace_alloc_t)
          -(`PA_SIZE-6) 
          -`SC_REQQ_ID_SIZE
          -$bits(sc_trace_type_t)
          -1:0]                        padding;
   logic [`PA_SIZE-6-1:0]              address;
   logic [`SC_REQQ_ID_SIZE-1:0]        reqq_id;
   sc_trace_type_t                     trace_type;
} sc_trace_rbuf_t;

// mesh
typedef struct packed {
   logic [$bits(sc_trace_alloc_t)
          -(6 * `SC_REQQ_ID_SIZE)
          -6
          -$bits(sc_trace_type_t)
          -1:0]                        padding;
   logic [`SC_REQQ_ID_SIZE-1:0]        to_sys_rsp_reqq_id;
   logic                               to_sys_rsp_valid;
   logic [`SC_REQQ_ID_SIZE-1:0]        to_sys_req_reqq_id;
   logic                               to_sys_req_valid;
   logic [`SC_REQQ_ID_SIZE-1:0]        to_l3_rsp_reqq_id;
   logic                               to_l3_rsp_valid;
   logic [`SC_REQQ_ID_SIZE-1:0]        to_l3_req_reqq_id;
   logic                               to_l3_req_valid;
   logic [`SC_REQQ_ID_SIZE-1:0]        l3_slave_rsp_reqq_id;
   logic                               l3_slave_rsp_valid;
   logic [`SC_REQQ_ID_SIZE-1:0]        l2_neigh_rsp_reqq_id;
   logic                               l2_neigh_rsp_valid;
   sc_trace_type_t                     trace_type;
} sc_trace_mesh_t;

// Reqq state
typedef struct packed {
   logic [$bits(sc_trace_alloc_t)
          -`SC_REQQ_ID_SIZE 
          -$bits(sc_reqq_opcode_t)
          -$bits(sc_trace_type_t)
          -18 -1:0]                    padding;
   logic [`SC_REQQ_ID_SIZE-1:0]        reqq_id;
   sc_reqq_opcode_t                    opcode;                  // Current action
   logic                               rspmux_l2_eligible;      // Neigh response waiting to get picked 
   logic                               rspmux_l3_eligible;      // L3 slave response Waiting to get picked 
   logic                               rspmux_sent;             // Response has been sent
   logic                               pipe_req_eligible;       // Have something for the pipeline to do
   logic                               to_l3_mesh_req_eligible; // Have something for the mesh to do
   logic                               to_sys_mesh_req_eligible;// Have something for the mesh to do
   logic                               rbuf_req_eligible;       // Have something for the rbuf to do
   logic                               inflight;
   logic                               data_ready;
   logic                               wait_for_dataq;          // Wait for dataq read or write before dealloc
   logic                               dep_valid;               // there is an entry ahead of me
   logic                               dep_tail;                // youngest in list
   logic                               dep_meshll_tail;         // youngest entry headed for mesh in meshll
   logic                               dep_non_meshll_head;     // oldest entry not in meshll list
   logic                               dont_depend_on_me;       // Entry is past the point of stalling dependencies 
   logic                               rbuf_valid;              // This entry is currently in the rbuf
   logic                               rbuf_pending_valid;      // This entry is currently being added to the rbuf
   logic                               err_detected;            // Error detected
   sc_trace_type_t                     trace_type;
} sc_trace_reqq_t;

// segments of trace packets
typedef union packed {
   sc_trace_alloc_t      l2_alloc;
   sc_trace_alloc_t      l3_alloc;
   sc_trace_tc_t         tc;
   sc_trace_rbuf_t       rbuf;
   sc_trace_mesh_t       mesh;
   sc_trace_reqq_t       reqq;
} sc_trace_segment_t; 

// The whole trace packet
typedef struct packed {
   sc_trace_segment_t    segment2;
   sc_trace_segment_t    segment1;
   sc_trace_segment_t    segment0;
} sc_trace_packet_t; 


// L2HPF (L2 Hardware Prefetcher)
typedef struct packed {
   logic [`ET_LINK_SOURCE_SIZE-1:0]     source;           // Source
   logic [`ET_LINK_ID_SIZE-1:0]         id;               // id                  
   logic [`PA_SIZE-1:0]                 address;          // address
   et_link_req_opcode_t                 opcode;           // Request Opcode
   logic                                hit;
} sc_neigh_l2hpf_req_t;

// bank <-> mesh structures
// --------------------------------------------------------------------------

`ifndef SC_MESH_REQ_CMD_TYPE_DEF
`define SC_MESH_REQ_CMD_TYPE_DEF(ID_SIZE, SOURCE_SIZE) \
struct packed { \
   logic [`DV_TRANS_ID_SIZE-1:0]             trans_id; \
   logic [ID_SIZE-1:0]                       id; \
   logic [`SC_MAX_L3_SLAVE_PORT_ID_SIZE-1:0] port_id; \
   logic [SOURCE_SIZE-1:0]                   source; \
   et_link_req_opcode_t                      opcode; \
   logic [`SC_MESH_MASTER_AXI_ADDR_SIZE-1:0] address; \
   sc_size_t                                 size; \
   logic [`AXI_AXQOS_SIZE-1:0]               qos; \
   logic                                     wdata; \
   logic [`SC_LINE_QW_SIZE-1:0]              qwen;  \
   logic [`SC_LINE_DATA_SIZE-1:0]            data; \
   logic                                     ben_en; \
   logic [`SC_LINE_BYTE_SIZE-1:0]            ben; \
}
`endif
typedef `SC_MESH_REQ_CMD_TYPE_DEF(`SC_REQQ_ID_SIZE,            `SC_MESH_MASTER_SOURCE_SIZE) sc_mesh_master_req_t;
typedef `SC_MESH_REQ_CMD_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `MESH_SOURCE_BRIDGE_ID_SIZE) sc_mesh_slave_req_t;

//typedef struct packed { 
//   logic [`SC_LINE_BYTE_SIZE-1:0]            ben;  // byte write enable
//} sc_mesh_req_data_t;
//typedef sc_mesh_req_data_t sc_mesh_master_req_data_t;
//typedef sc_mesh_req_data_t sc_mesh_slave_req_data_t;

//typedef struct packed {
//   sc_mesh_master_req_cmd_t  cmd;
//   sc_mesh_master_req_data_t data;
//} sc_mesh_master_req_t;
//
//typedef struct packed {
//   sc_mesh_slave_req_cmd_t   cmd;
//   sc_mesh_slave_req_data_t  data;
//} sc_mesh_slave_req_t;

`ifndef SC_MESH_RSP_TYPE_DEF
`define SC_MESH_RSP_TYPE_DEF(ID_SIZE, SOURCE_SIZE) \
struct packed { \
   logic [`DV_TRANS_ID_SIZE-1:0]             trans_id; \
   logic [ID_SIZE-1:0]                       id; \
   logic [`SC_MAX_L3_SLAVE_PORT_ID_SIZE-1:0] port_id; \
   logic [SOURCE_SIZE-1:0]                   source; \
   logic                                     err; \
   logic                                     wdata; \
   logic [`SC_LINE_DATA_SIZE-1:0]            data; \
} 
`endif
typedef `SC_MESH_RSP_TYPE_DEF(`SC_REQQ_ID_SIZE,            `SC_MESH_MASTER_SOURCE_SIZE) sc_mesh_master_rsp_t; 
typedef `SC_MESH_RSP_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `MESH_SOURCE_BRIDGE_ID_SIZE) sc_mesh_slave_rsp_t;

typedef struct packed {
   logic [`SC_REQQ_ID_SIZE-1:0]     reqq_id;
   sc_mesh_slave_rsp_t              rsp;
} sc_mesh_slave_rsp_long_t;
 

