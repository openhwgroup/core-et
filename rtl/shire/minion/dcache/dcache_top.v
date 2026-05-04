// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_top  (
  // System signals
  input  logic                                   clock,
  input  logic                                   reset,
  input  logic [`NUM_SHIRE_IDS_R]                shire_id,
  input  logic [`MIN_PER_S_R]                    shire_min_id,
  // Is this in the IOShire
  input  logic                                   ioshire,
  // Chicken bit clock gate enable
  input  logic                                   chicken_bit_dcache,
  // Setting control
  input  core_dcache_ctrl                        core_ctrl,
  output dcache_core_ctrl                        core_ctrl_resp,
  // Core request to the DCache (aligns to EXE stage)
  input  logic                                   id_core_alloc_rq_pre,
  input  logic                                   s0_core_alloc_rq_val,
  input  logic                                   id_core_gsc,
  output logic                                   id_core_ready,
  input  logic                                   s0_core_req_valid,
  input  minion_dcache_req                       s0_core_req,
  input  logic                                   s0_core_gsc,
  input  logic                                   s1_core_kill,
  input  logic [`DCACHE_DATA_RANGE]              s1_core_store_data,
  input  esr_mprot_t                             s1_mprot,
  input  logic                                   s2_core_kill,
  input logic [`XREG_RANGE]                      s3_core_x31,
  // VPU scratchpad direct port
  input  vpu_dcache_ctrl                         s1_vpu_ctrl,
  output dcache_vpu_scp_resp                     s3_vpu_scp_resp,
  output logic [`DCACHE_DATA_RANGE]              s3_vpu_scp_data,
  output logic [`DCACHE_DATA_RANGE]              s3_vpu_tenb_data,
  // VPU reduce control
  output dcache_vpu_reduce_ctrl                  vpu_reduce_ctrl,
  // DCache response to the Core
  output logic                                   s2_core_resp_int_valid,
  output logic                                   s3_core_resp_valid,
  output dcache_minion_resp                      s3_core_resp,
  // Dcache control
  output dcache_minion_scoreboard                id_core_scoreboard,
  output [`CORE_NR_THREADS-1:0]                  id_core_sb_fp_dealloc, // pulse when a FP entry is deallocated
  output [`CORE_NR_THREADS-1:0]                  id_core_sb_int_dealloc,
  output logic                                   s1_core_replay_next,
  output dcache_tag_xcpt                         s1_core_xcpt,
  output logic                                   s2_core_flush_pipeline,
  output logic [`CORE_NR_THREADS-1:0]            s3_ordered,
  input  logic                                   s3_invalidate_lr,
  input  logic                                   wb_dcache_fp_toint,
  // Minimum page size ESR
  input  tlb_entry_type                          vmspagesize,
  // Other ESR settings
  input  logic                                   bypass_dcache,
  // Memory control override
  input  logic [`MINION_MEM_CTRL_BITS-1:0]       mem_ctrl_override,
  // TLB/PTW control
  input  minion_satp_info                        satp_info,
  input  minion_satp_info                        matp_info,
  input                                          satp_info_en,
  input                                          matp_info_en,
  input  minion_vm_status [`CORE_NR_THREADS-1:0] vm_status,
  input  logic                                   tlb_invalidate,
  // PTW request
  output minion_ptw_req                          ptw_req_data,
  output logic                                   ptw_req_valid,
  input  logic                                   ptw_req_ready,
  // PTW response
  input  minion_ptw_pte                          ptw_resp_data,
  input  logic                                   ptw_resp_valid,
  // Request to L2 evict port
  input  logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  l2_evict_req_ready,
  output logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  l2_evict_req_valid,
  output et_link_minion_evict_req_info_t         l2_evict_req,
  // Request to L2 miss port
  input  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]   l2_miss_req_ready,
  output logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]   l2_miss_req_valid,
  output et_link_minion_miss_req_info_t          l2_miss_req,
  // Response L2 port
  output logic                                   l2_resp_ready,
  input  logic                                   l2_resp_valid,
  input  et_link_minion_rsp_info_t               l2_resp,
  // Internal response L2 port
  output logic                                   l2_resp_ready_int,
  output logic                                   l2_resp_valid_int,
  output et_link_minion_rsp_info_t               l2_resp_int,
  // Breakpoint data
  output dcache_core_bp_t                        s1_bp_conf,
  output logic                                   s1_bp_conf_valid,
  // Idle flag
  output logic                                   dcache_idle_excl_mode,
  // Tensor load error bits
  output logic [`DCACHE_TL_ERROR_BITS-3:0]        tensor_load_err_flags,
  // Cache Ops error bits
  output logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] cache_ops_err_flags,
  // Tensor reduce error bits
  output logic                                    tensor_reduce_err_flags,
  // APB signals from BPAM for debug
  input  logic [`DCACHE_DBG_ADDR_RANGE]           apb_paddr,
  input  logic                                    apb_pwrite,
  input  logic                                    apb_psel,
  input  logic                                    apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0]   apb_pwdata,
  output logic                                    apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0]   apb_prdata,
  output logic                                    apb_pslverr,
  // Events for performance counters
  output logic [`CSR_NR_EVENTS_DCACHE-1:0]        io_events,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]             bus_err,
  output logic [`CORE_NR_THREADS-1:0][`PA_RANGE]  bus_err_addr,
  `ifdef DCACHE_REPORT_PC
  output logic [`CORE_NR_THREADS-1:0][`PC_RANGE_EXT] bus_err_pc,
  `endif
  // Debug signals to CSR
  output dcache_debug_sigs                        csr_debug_sigs,
  // Signals for Debug Status Monitor
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl0,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl0,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl0,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl1,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl1,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl1,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_ts,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_ts,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_ts,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_co,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_co,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_co,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_col2,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_col2,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_col2,

  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_MATCH_BITS-1:0]  sm_match_debug_signals_mh,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_debug_signals_mh,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_DATA_BITS-1:0]   sm_data_debug_signals_mh,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_dc,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_dc,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_dc
);

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Local defines,not to be used anywhere else
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  // Port assignment for L2 access arbiters
  `define DCACHE_L2_EVICT_ARB_PORTS  4
  `define DCACHE_L2_EVICT_WB_PORT    0
  `define DCACHE_L2_EVICT_MH_PORT    1
  `define DCACHE_L2_EVICT_RE_PORT    2
  `define DCACHE_L2_EVICT_PF_PORT    3

  `define DCACHE_L2_MISS_ARB_PORTS   3
  `define DCACHE_L2_MISS_MH_PORT     0
  `define DCACHE_L2_MISS_TL0_PORT    1
  `define DCACHE_L2_MISS_TL1_PORT    2

  `define DCACHE_L2_EVICT_MUX_PORTS  2
  `define DCACHE_L2_MISS_MUX_PORTS   3

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Signal Declaration
  //  * Signal declaration in this file is quite spread. Will tend to
  //  * concentrate them to this area unless its usage is very local
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  // L2 arbritration
  et_link_minion_evict_req_info_t [`DCACHE_L2_EVICT_ARB_PORTS-1:0] l2_evict_arb_data_in;    // Different L2 requests to evict arbiter
  logic                           [`DCACHE_L2_EVICT_ARB_PORTS-1:0] l2_evict_arb_bid;        // Bid signal to evict arbiter from clients
  logic                           [`DCACHE_L2_EVICT_ARB_PORTS-1:0] l2_evict_arb_grant;      // Grant signal to different evict arbiter clients

  et_link_minion_evict_req_info_t [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_pipe_data_in;   // Different L2 requests to evict pipe stage
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_pipe_valid;     // Valid signal to evict pipe stage from clients
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_pipe_ready;     // Ready signal to different evict multiplexer clients
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_pipe_valid_en;   // Clock enable signal for the valid register
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_pipe_data_en;    // Clock enable signal for the data register

  et_link_minion_evict_req_info_t [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_mux_data_in;    // Different L2 requests to evict multiplexer
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_mux_valid;      // Valid signal to evict multiplexer from pipe stage
  logic                           [`DCACHE_L2_EVICT_MUX_PORTS-1:0] l2_evict_mux_ready;      // Ready signal to evict pipe stage

  et_link_minion_miss_req_info_t  [`DCACHE_L2_MISS_ARB_PORTS-1:0]  l2_miss_arb_data_in;     // Different L2 requests to miss arbiter
  logic                           [`DCACHE_L2_MISS_ARB_PORTS-1:0]  l2_miss_arb_bid;         // Bid signal to miss arbiter from clients
  logic                           [`DCACHE_L2_MISS_ARB_PORTS-1:0]  l2_miss_arb_grant;       // Grant signal to different miss arbiter clients

  et_link_minion_miss_req_info_t  [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_pipe_data_in;    // Different L2 requests to miss pipe stage
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_pipe_valid;      // Valid signal to miss pipe stage from clients
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_pipe_ready;      // Ready signal to different miss multiplexer clients
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_pipe_valid_en;   // Clock enable signal for the valid register
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_pipe_data_en;    // Clock enable signal for the data register

  et_link_minion_miss_req_info_t  [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_mux_data_in;     // Different L2 requests to miss multiplexer
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_mux_valid;       // Valid signal to miss multiplexer from pipe stage
  logic                           [`DCACHE_L2_MISS_MUX_PORTS-1:0]  l2_miss_mux_ready;       // Ready signal to miss pipe stage

  et_link_minion_evict_req_info_t  wb_l2_evict_req;            // L2 evict request from writeback
  et_link_minion_evict_req_info_t  mh_l2_evict_req;            // L2 evict request from miss handler
  et_link_minion_evict_req_info_t  re_l2_evict_req;            // L2 evict request from reduce
  et_link_minion_evict_req_info_t  ts_l2_evict_req;            // L2 evict request from Tensor Store
  et_link_minion_evict_req_info_t  pf_l2_evict_req;            // L2 evict request from L2 prefetch

  et_link_minion_evict_req_info_t  arb_l2_evict_req;           // L2 evict request from evict arbiter

  et_link_minion_miss_req_info_t   mh_l2_miss_req;             // L2 request from Miss Handler
  et_link_minion_miss_req_info_t   tl0_l2_miss_req;            // L2 request from Tensor Load 0
  et_link_minion_miss_req_info_t   tl1_l2_miss_req;            // L2 request from Tensor Load 1

  et_link_minion_miss_req_info_t   arb_l2_miss_req;            // L2 request from miss request arbiter

  logic                            wb_l2_evict_valid;          // Writeback wants to evict data to L2
  logic                            wb_l2_evict_ready;          // Writeback gets permission to evict data to L2

  logic                            mh_l2_evict_valid;          // Miss handler wants to send data to L2
  logic                            mh_l2_evict_ready;          // Miss handler gets permission to send data to L2

  logic                            re_l2_evict_valid;          // Reduce unit wants to send a message to another minion
  logic                            re_l2_evict_ready;          // Reduce unit gets permission to send a message to another minion

  logic                            ts_l2_evict_valid;          // Tensor Store unit wants to send a message to another minion
  logic                            ts_l2_evict_ready;          // Tensor Store unit gets permission to send a message to another minion
  logic                            re_ts_l2_evict_inflight;    // Reduce/Tensor Sore unit has started sending a message

  logic                            pf_l2_evict_valid;          // L2 cache op wants to send a prefetch to L2
  logic                            pf_l2_evict_ready;          // L2 cache op gets permission to send a prefetch to L2

  logic                            arb_l2_evict_valid;         // L2 evict arbiter wants to request data to L2
  logic                            arb_l2_evict_ready;         // L2 evict arbiter gets permission to request data to L2

  logic                            mh_l2_miss_valid;           // Miss handler wants to request data to L2
  logic                            mh_l2_miss_ready;           // Miss handler gets permission to request data to L2

  logic                            tl0_l2_miss_valid;          // Tensor Load 0 wants to request data to L2
  logic                            tl0_l2_miss_ready;          // Tensor Load 0 gets permission to request data to L2

  logic                            tl0_l2_miss_valid_no_coop;  // Tensor Load 0 wants to request data to l2_miss_arb
  logic                            tl0_l2_miss_ready_no_coop;  // Tensor Load 0 gets permission to request data to l2_miss_arb

  logic                            tl1_l2_miss_valid;          // Tensor Load 1 wants to request data to L2
  logic                            tl1_l2_miss_ready;          // Tensor Load 1 gets permission to request data to L2

  logic                            tl1_l2_miss_valid_no_coop;  // Tensor Load 1 wants to request data to l2_miss_arb
  logic                            tl1_l2_miss_ready_no_coop;  // Tensor Load 1 gets permission to request data to l2_miss_arb

  logic                            arb_l2_miss_valid;          // L2_miss arbiter wants to request data to L2
  logic                            arb_l2_miss_ready;          // L2 miss arbiter gets permission to request data to L2

  logic [`DCACHE_DATA_RANGE]       s1_ba_data;                 // Data read from buffer array

  // Tensor Store address translation
  logic [`VA_RANGE]                s0_ts_addr;             // Signals to access the pipeline from Tensor Store
  logic                            s0_ts_addr_msb_err;     // Indication of address overflow error
  logic                            s0_available_to_ts;     // Address to translate, pipeline available in S0 and S1,
  logic                            s0_ts_addr_load;        // and load indication in S0 and S1 from Tensor Store
  logic                            s0_ts_thread_id;        // Thread that initiated the tensor store operation
  logic                            s1_available_to_ts;
  logic                            s1_ts_addr_load;

  // Tensor load address translation
  logic [`VA_RANGE]                s0_tl_addr;             // Signals to access the pipeline from Tensor Load
  logic                            s0_tl_addr_msb_err;     // Flag indicating overflow in the address update
  logic                            s0_available_to_tl;     // Address to translate, pipeline available in S0 and S1,
  logic                            s0_tl_addr_load;        // and load indication in S0 and S1 from Tensor Load
  logic                            s0_tl_thread_id;        // Thread that initiated the tensor load operation
  logic [`DCACHE_VM_SEL_RANGE]     s0_tl_vm_sel;           // Selection of the Virtual Memory configuration
  logic                            s1_available_to_tl;     // Signals are muxed for Tensor Load 0 and 1
  logic                            s1_tl_addr_load;

  logic [`VA_RANGE]                s0_tl0_addr;            // Same signals for Tensor Load 0
  logic                            s0_tl0_addr_msb_err;
  logic                            s0_available_to_tl0;
  logic                            s0_tl0_addr_load;
  logic                            s0_tl0_thread_id;
  logic                            s1_available_to_tl0;
  logic                            s1_tl0_addr_load;

  logic [`VA_RANGE]                s0_tl1_addr;            // Same signals for Tensor Load 1
  logic                            s0_tl1_addr_msb_err;
  logic                            s0_available_to_tl1;
  logic                            s0_tl1_addr_load;
  logic                            s0_tl1_thread_id;
  logic                            s1_available_to_tl1;
  logic                            s1_tl1_addr_load;

// Access from CacheOpsL2 to the pipeline for address translation
logic [`VA_RANGE]                s0_co_l2_addr;         // Signals to access the pipeline from CacheOpsL2
logic                            s0_co_l2_addr_msb_err; // Indication that address got an overflow error
logic                            s0_available_to_co_l2; // Address to translate, pipeline available in S0 and S1,
logic                            s0_co_l2_addr_load;    // and load indication in S0 and S1 from CacheOpsL2
logic                            s0_co_l2_thread_id;    // Thread that initiated the cache opeations
logic                            s1_available_to_co_l2;
logic                            s1_co_l2_addr_load;

// Tensor Load writing into LRAM
logic [`DCACHE_LRAM_ADDR_RANGE]  s4_tl0_load_addr;      // LRAM addr for the TL fills from L2 or from internal TL memory
logic [`DCACHE_WAY_RANGE]        s4_tl0_load_way;       // Way for the TL fills from L2 or from internal TL memory
logic [`CORE_L2_BLOCK_EXT_RANGE] s4_tl0_load_data;      // Data for the TL fills from internal TL memory
logic [`DCACHE_LRAM_BANK_RANGE]  s4_tl0_load_valid;     // Validation signal, per bank, for the TL fills from internal TL memory
logic                            s3_tl0_load_valid_pre; // Validation, one cycle before, to write into LRAM
logic                            s4_tl0_load_ready;     // Ready signal for the TL fills from internal TL memory
logic                            s4_tl0_load_capture;   // Capture fills from L2, avoid that these go into LRAM

logic [`DCACHE_LRAM_ADDR_RANGE]  s4_tl1_load_addr;      // Same as previous signals but specific for tensor load module 1

logic                            tl0_working;           // Tensor Load 0 is working
logic                            tl1_working;           // Tensor Load 1 is working

logic [1:0]                      tl0_id_complete;   // Tensor Load 0 complete signal per ID and Thread

// For performance coounters
logic                            ts_start;
logic                            ts_op;
logic                            tl0_start;
logic                            tl0_op;
logic                            tl1_start;
logic                            tl1_op;

// Other pipeline validation or enable signals
logic                            s0_valid_to_s1;        // Valid sent to S1 from S0
logic                            s0_md_read_en;         // Meta data array read enable
logic                            s1_clk_en;             // Enables the clock to the S2 registers

// Exception management
logic                            s1_has_xcpt;           // An exception has occurred in S1

dcache_tag_xcpt                  s1_core_xcpt_no_kill;  // Exceptions without the kill signal from core
logic                            s1_has_xcpt_no_kill;

// Debug signals
logic                               dbg_da_read_en;     // Data array debug read port
logic [`DCACHE_DBG_DA_ADDR_RANGE]   dbg_da_read_addr;
logic                               dbg_da_read_valid;
logic                               dbg_da_read_valid_next;
logic                               dbg_da_read_accept;
logic                               dbg_da_read_accept_next;
logic [`DCACHE_DBG_DA_DATA_RANGE]   dbg_da_read_data;

logic                               dbg_da_write_en;    // Data array debug write port
logic [`DCACHE_DBG_DA_ADDR_RANGE]   dbg_da_write_addr;
logic                               dbg_da_write_valid;
logic [`DCACHE_DBG_DA_DATA_RANGE]   dbg_da_write_data;
logic                               dbg_da_write_en_early;

logic                               dbg_md_read_en;     // Metadata debug read port
logic [`DCACHE_SET_WAY_RANGE]       dbg_md_read_addr;
logic                               dbg_md_read_valid;
logic                               dbg_md_read_accept;
logic                               dbg_md_read_accept_next;
logic [`DCACHE_DBG_MD_DATA_RANGE]   dbg_md_read_data;
logic                               id_dbg_bid;         // Bid and grant signals to access to pipeline for reading metadata
logic                               id_dbg_grant;
logic                               s0_dbg_valid;       // Validation signals in S0 and S1
logic                               s1_dbg_valid;

logic                               dbg_md_write_en;    // Metadata debug write port
logic                               dbg_md_write_en_early;
logic [`DCACHE_SET_WAY_RANGE]       dbg_md_write_addr;
logic                               dbg_md_write_valid;
logic [`DCACHE_DBG_MD_DATA_RANGE]   dbg_md_write_data;
dcache_meta_write_req               dbg_md_write_req;   // Debug meta write request

logic [`DCACHE_DBG_CSR_TL_SIZE-1:0 ] dbg_csr_tl0;
logic [`DCACHE_DBG_CSR_TL_SIZE-1:0 ] dbg_csr_tl1;
logic [`DCACHE_DBG_CSR_TS_SIZE-1:0 ] dbg_csr_ts;
logic [`DCACHE_DBG_CSR_COU_SIZE-1:0 ] dbg_csr_cou;
logic [`DCACHE_DBG_CSR_COUL2_SIZE-1:0 ] dbg_csr_coul2;
logic [2*(`DCACHE_DBG_CSR_MH_SIZE)-1:0 ] dbg_csr_mh;

// Addresses used for store bypass
logic [`DCACHE_DATA_NUM_DWORDS_RANGE][`DCACHE_BLOCK_ADDR_RANGE2] s2_block_addr;  // Block address within cacheline for the store in S2
logic [`DCACHE_DATA_NUM_DWORDS_RANGE][`DCACHE_BLOCK_ADDR_RANGE2] s3_block_addr;  // Block address within cacheline for the store in S3
logic [`DCACHE_DATA_NUM_DWORDS_RANGE][`DCACHE_BLOCK_ADDR_RANGE2] s4_block_addr;  // Block address within cacheline for the store in S4
logic [`DCACHE_DATA_NUM_DWORDS_RANGE][`DCACHE_BLOCK_ADDR_RANGE2] s5_block_addr;  // Block address within cacheline for the store in S5

// Error flags from cache ops L1
logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] cache_ops_err_flags_l1;
logic                         [`CORE_NR_THREADS-1:0] cache_ops_err_flags_l2;
logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] tensor_reduce_err_all_flags;
logic                         [`CORE_NR_THREADS-1:0] tensor_reduce_mem_err_flag;
logic                         [`CORE_NR_THREADS-1:0] tensor_load_mem_err_flags;

// Bus errors
logic [`CORE_NR_THREADS-1:0]            bus_err_co_l2;
logic [`CORE_NR_THREADS-1:0]            bus_err_mh;
logic [`PA_RANGE]                       bus_err_addr_mh;
`ifdef DCACHE_REPORT_PC
logic [`PC_RANGE_EXT]                   bus_err_pc_mh;
`endif
logic [`CORE_NR_THREADS-1:0]            bus_err_re;
logic [`CORE_NR_THREADS-1:0]            bus_err_tl0;
logic [`CORE_NR_THREADS-1:0]            bus_err_tl1;

// Signals for idle checking
logic   mh_idle;                     // Combined idle signal for all miss handler units
logic   dcache_idle;                 // Full idle condition
logic   dcache_idle_next;            // Next value of the Dcache idle signal
logic   dcache_idle_excl_mode_next;  // Next value for the idle condition to enter exclusive mode



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// No Stage
// Below is code and modules not related to regular pipeline of the dcache.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Scratchpad Control
////////////////////////////////////////////////////////////////////////////////

// This module controls several essential features of the Dcache, like the
// number of sets and the validity of the lines in local memory

logic         dcache_cfg_stall;      // Tell the Dcache to end all ongoing tasks
logic         dcache_cfg_stall_prev;
logic         dcache_cfg_sets_red;   // Reduced set mode is enabled
logic         dcache_cfg_clear_all;  // Clears all content in metadata and data arrays
logic         dcache_cfg_clear_low;  // Clears content in metadata and data arrays for lowest sets
logic         dcache_cfg_alloc_scp;  // Allocates lines for SCP
dcache_mode_t dcache_cfg_mode;       // Operating mode of the Dcache

dcache_mode_t dcache_core_ctrl_mode; // Dcache mode from core control

logic                      s3_cfg_da_clear_en;   // Enable clearing specific set and way of Data Array
logic                      s4_cfg_da_clear_val;
logic [`DCACHE_SET_RANGE]  s4_cfg_da_clear_set;
logic [`DCACHE_WAY_RANGE]  s4_cfg_da_clear_way;
logic                      s4_cfg_da_clear_idx;
logic                      s4_cfg_da_clear_rdy;
logic                      scp_ctrl_on;

assign dcache_core_ctrl_mode = dcache_mode_t'(core_ctrl.cache_ctrl[1:0]);

dcache_scratchpad_ctrl scratchpad_ctrl (
  // System signals
  .clock                ( clock                 ),
  .reset                ( reset                 ),
  // Control bits from core
  .scratchpad_ctrl      ( dcache_core_ctrl_mode ),
  // Status from Dcache
  .dcache_idle          ( dcache_idle           ),
  // Clear Dcache flags and stall operations
  .dcache_cfg_stall     ( dcache_cfg_stall      ),
  .dcache_cfg_clear_all ( dcache_cfg_clear_all  ),
  .dcache_cfg_clear_low ( dcache_cfg_clear_low  ),
  .dcache_cfg_alloc_scp ( dcache_cfg_alloc_scp  ),
  // Write zeros to Dcache lines
  .da_clear_en          ( s3_cfg_da_clear_en    ),
  .da_clear_val         ( s4_cfg_da_clear_val   ),
  .da_clear_set         ( s4_cfg_da_clear_set   ),
  .da_clear_way         ( s4_cfg_da_clear_way   ),
  .da_clear_idx         ( s4_cfg_da_clear_idx   ),
  .da_clear_rdy         ( s4_cfg_da_clear_rdy   ),
  // Dcache mode
  .dcache_cfg_mode      ( dcache_cfg_mode       )
);

assign scp_ctrl_on = (dcache_cfg_mode == DcacheMode_Scratchpad);

assign dcache_cfg_sets_red = (dcache_cfg_mode != DcacheMode_Shared);

////////////////////////////////////////////////////////////////////////////////
// L2 response registered handshake
////////////////////////////////////////////////////////////////////////////////
//`define DCACHE_USE_REGISTERED_L2_RESP
//`define DCACHE_USE_REGISTERED_L2_RESP_2_WAY
`define DCACHE_ADD_PIPE_TO_L2_RESP_FULL_PERF

logic                                   l2_resp_valid_prev_int;
logic                                   l2_resp_valid_ready_int;

`ifdef DCACHE_ADD_PIPE_TO_L2_RESP_FULL_PERF
  logic l2_resp_fifo_push;
  logic l2_resp_fifo_full;
  logic l2_resp_fifo_full_next_unused_ok;
  logic l2_resp_fifo_pop;

  // Add pipeline stage to responses to improve timing to Neigh interface
  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_minion_rsp_info_t) ),
    .NUM_ELEMS   ( 2                                ),
    .USE_LATCHES ( 0                                )
  ) l2_resp_pipe_fifo (
    // System signals
    .clock       ( clock                            ),
    .reset       ( reset                            ),
    // Push data
    .push_early  ( 1'b0 /* Only if USE_LATCHES*/    ),
    .push        ( l2_resp_fifo_push                ),
    .push_data   ( l2_resp                          ),
    .full        ( l2_resp_fifo_full                ),
    .full_next   ( l2_resp_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( l2_resp_fifo_pop                 ),
    .pop_data    ( l2_resp_int                      ),
    .valid       ( l2_resp_valid_int                ),
    .valid_next  ( l2_resp_valid_prev_int           )
  );

  assign l2_resp_fifo_push = l2_resp_valid & !l2_resp_fifo_full;
  assign l2_resp_fifo_pop  = l2_resp_valid_int & l2_resp_ready_int;
  assign l2_resp_ready     = !l2_resp_fifo_full;

`else
  `ifndef DCACHE_USE_REGISTERED_L2_RESP

      // Combinational implementation

      // Inputs
      assign l2_resp_valid_int = l2_resp_valid;
      assign l2_resp_int       = l2_resp;

      // Outputs
      assign l2_resp_ready     = l2_resp_ready_int;

  `else
      logic                                   l2_resp_valid_int_next;
      logic                                   l2_resp_up;
      et_link_minion_rsp_info_t               l2_resp_reg;

      // Registered implementation with 2-way register, one for valid, one for ready
      `ifdef DCACHE_USE_REGISTERED_L2_RESP_2_WAY
          logic                               l2_resp_ready_next;

          always_comb begin
            l2_resp_valid_int_next = l2_resp_valid && !(l2_resp_valid_int && l2_resp_ready_int) && !l2_resp_ready;
            l2_resp_ready_next     = l2_resp_valid_int && l2_resp_ready_int;
            l2_resp_up             = l2_resp_valid && !l2_resp_valid_int;
          end

          //       CLK    RST    EN          DOUT               DIN
          `RST_FF (clock, reset,             l2_resp_ready,     l2_resp_ready_next,     1'b0)

      // Registered implementation with 1-way register for valid. Includes immediate response in case of Ack or Err response.
      `else
          logic                               l2_resp_ready_immediate;
          logic                               l2_resp_ready_int_qual;

          always_comb begin
            l2_resp_valid_int_next = l2_resp_valid && !(l2_resp_valid_int && l2_resp_ready_int_qual);
            l2_resp_up             = l2_resp_valid_int_next;
          end

          // For responses that are immediate, avoid generating a double ready, so block the internal ready
          always_comb begin
            l2_resp_ready_immediate = (l2_resp.opcode == ET_LINK_RSP_Ack) || (l2_resp.opcode == ET_LINK_RSP_Err);
            l2_resp_ready_int_qual  = l2_resp_ready_int & l2_resp_valid_int & !((l2_resp_int.opcode == ET_LINK_RSP_Ack) || (l2_resp_int.opcode == ET_LINK_RSP_Err));
            l2_resp_ready           = l2_resp_ready_immediate | l2_resp_ready_int_qual;
          end
      `endif

      //       CLK    RST    EN          DOUT               DIN
      `RST_FF (clock, reset,             l2_resp_valid_int, l2_resp_valid_int_next, 1'b0)
      `EN_FF  (clock,        l2_resp_up, l2_resp_reg,       l2_resp                     )

      always_comb begin
        // Discard registered data to save flip-flops. Only control that has to go through logic is registered
        l2_resp_int      = l2_resp_reg;
        l2_resp_int.data = l2_resp.data;
      end

  `endif

  assign l2_resp_valid_prev_int = 1'b1;

`endif

// Internal combination of valid/ready
assign l2_resp_valid_ready_int = l2_resp_valid_int && l2_resp_ready_int;

////////////////////////////////////////////////////////////////////////////////
// L2 request arbiter
////////////////////////////////////////////////////////////////////////////////

// Four units want to send requests to the L2 via the evict FF
//  - Writeback: to send capacity evicts
//  - Misshandler: to send data
//  - Reduce: to send reduce related messages to other minions
//  - L2Pref: to send prefetches to L2
// Three units want to send requests to the L2 via the miss FF
//  - CacheOp: to send snoop evicts
//  - Misshandler: to send misses
//  - Tensor Load0: to send loads
// Miss FF has dual handshake interface that will mux the access
// from L2 miss arbiter and Tensor Load1

logic wb_ready;      // Writeback unit is ready
logic wb_valid;      // Request to Writeback from any unit
logic mh_wb_valid;   // Miss handler unit wants to evict a line
logic mh_wb_ready;   // Miss handler unit gets granted access to writeback

logic wb_allow_evict_bid; // WriteBack is not operating nor being asked to operate

// Two arbiters provide access to L2

// Evict interface arbiter/mux
//------------------------------------------------------------------------------
logic [$clog2(`DCACHE_L2_EVICT_ARB_PORTS)-1:0] l2_arb_to_evict_winner_unused_ok;
arb_lru_data #(
  .WIDTH       ( $bits(et_link_minion_evict_req_info_t) ),
  .NUM_CLIENTS ( `DCACHE_L2_EVICT_ARB_PORTS             )
) l2_arb_to_evict (
  // System signals
  .clock       ( clock                ),
  .reset       ( reset                ),
  // Bidding
  .bid         ( l2_evict_arb_bid     ),
  .stall       ( !arb_l2_evict_ready  ),
  // Data inputs
  .data_in     ( l2_evict_arb_data_in ),
  // Grant
  .grant       ( l2_evict_arb_grant   ),
  .data_winner ( arb_l2_evict_req     ),
  .winner      ( l2_arb_to_evict_winner_unused_ok )
);

assign arb_l2_evict_valid = |l2_evict_arb_bid;

// Pipeline stage between the arbiter and the mux

for (genvar evict_it = 0; evict_it < `DCACHE_L2_EVICT_MUX_PORTS; evict_it++) begin : L2_EVICT_PIPE
  //         CLK    RST    EN                                DOUT                            DIN                            DEF
  `RST_EN_FF(clock, reset, l2_evict_pipe_valid_en[evict_it], l2_evict_mux_valid[evict_it],   l2_evict_pipe_valid[evict_it], 1'b0)
  `EN_FF    (clock,        l2_evict_pipe_data_en[evict_it],  l2_evict_mux_data_in[evict_it], l2_evict_pipe_data_in[evict_it])
  assign l2_evict_pipe_ready[evict_it]    = l2_evict_mux_ready[evict_it] | ~l2_evict_mux_valid[evict_it];
  assign l2_evict_pipe_valid_en[evict_it] = l2_evict_pipe_ready[evict_it] & (l2_evict_mux_valid[evict_it] | l2_evict_pipe_valid[evict_it]);
  assign l2_evict_pipe_data_en[evict_it]  = l2_evict_pipe_ready[evict_it] & l2_evict_pipe_valid[evict_it];
end

mux_etl_type_b #(
  .WIDTH       ( $bits(et_link_minion_evict_req_info_t) ),
  .NUM_CLIENTS ( `DCACHE_L2_EVICT_MUX_PORTS             )
) l2_mux_to_evict (
  // Handshake ports on client side
  .valid_in    ( l2_evict_mux_valid   ),
  .ready_in    ( l2_evict_mux_ready   ),
  // Handshake ports on bus side
  .valid_bus   ( l2_evict_req_valid   ),
  .ready_bus   ( l2_evict_req_ready   ),
  // Data ports
  .data_in     ( l2_evict_mux_data_in ),
  .data_bus    ( l2_evict_req         )
);

// Miss interface arbiter/mux
//------------------------------------------------------------------------------
logic [$clog2(`DCACHE_L2_EVICT_ARB_PORTS)-1:0] l2_arb_to_miss_winner_unused_ok;

arb_lru_data #(
  .WIDTH       ( $bits(et_link_minion_miss_req_info_t) ),
  .NUM_CLIENTS ( `DCACHE_L2_MISS_ARB_PORTS             )
) l2_arb_to_miss (
  // System signals
  .clock       ( clock                ),
  .reset       ( reset                ),
  // Bidding
  .bid         ( l2_miss_arb_bid      ),
  .stall       ( !arb_l2_miss_ready   ),
  // Data inputs
  .data_in     ( l2_miss_arb_data_in  ),
  // Grant
  .grant       ( l2_miss_arb_grant    ),
  .data_winner ( arb_l2_miss_req      ),
  .winner      ( l2_arb_to_miss_winner_unused_ok )
);

assign arb_l2_miss_valid = |l2_miss_arb_bid;

// Pipeline stage between the arbiter and the mux

for (genvar miss_it = 0; miss_it < `DCACHE_L2_MISS_MUX_PORTS; miss_it++) begin : L2_MISS_PIPE
  //         CLK    RST    EN                               DOUT                          DIN                          DEF
  `RST_EN_FF(clock, reset, l2_miss_pipe_valid_en[miss_it], l2_miss_mux_valid[miss_it],   l2_miss_pipe_valid[miss_it], 1'b0)
  `EN_FF    (clock,        l2_miss_pipe_data_en[miss_it],  l2_miss_mux_data_in[miss_it], l2_miss_pipe_data_in[miss_it])

  assign l2_miss_pipe_ready[miss_it]    = l2_miss_mux_ready[miss_it] | ~l2_miss_mux_valid[miss_it];
  assign l2_miss_pipe_valid_en[miss_it] = l2_miss_pipe_ready[miss_it] & (l2_miss_mux_valid[miss_it] | l2_miss_pipe_valid[miss_it]);
  assign l2_miss_pipe_data_en[miss_it]  = l2_miss_pipe_ready[miss_it] & l2_miss_pipe_valid[miss_it];
end   

mux_etl_type_b #(
  .WIDTH       ( $bits(et_link_minion_miss_req_info_t) ),
  .NUM_CLIENTS ( `DCACHE_L2_MISS_MUX_PORTS             )
) l2_mux_to_miss (
  // Handshake ports on client side
  .valid_in    ( l2_miss_mux_valid    ),
  .ready_in    ( l2_miss_mux_ready    ),
  // Handshake ports on bus side
  .valid_bus   ( l2_miss_req_valid    ),
  .ready_bus   ( l2_miss_req_ready    ),
  // Data ports
  .data_in     ( l2_miss_mux_data_in  ),
  .data_bus    ( l2_miss_req          )
);

always_comb begin
  // Arbiter inputs to EVICT FF
  //---------------------------------------------------------------------------
  // Writeback. Always wins the arbitration. With the miss handler way of
  // dealing with misses, we need to guarantee that the evict gets out no
  // matter what (it checked that the evict FF is available). If another
  // unit wins the l2 arb, it might get nacked all the time by the L2 and we
  // can enter in a deadlock: l2 fill going to minion can't get in due evict
  // not finished, cacheop can't go to l2 because it is full, evict can't go
  // to evict FF because cacheop wins all the time
  // RTLMIN-3810: extended to not allowing any unit winning evict FF while
  // there's an inflight writeback or in the cycle that miss handler starts
  // a writeback
  //
  // Ensure that once WriteBack or Reduce win the first transaction they keep
  // the interface until the transfer is complete. The "inflight" signals
  // are activated only after winning the first transfer (ready && valid)

  wb_allow_evict_bid = wb_ready && !wb_valid;

  l2_evict_arb_bid[`DCACHE_L2_EVICT_WB_PORT]           = wb_l2_evict_valid;
  l2_evict_arb_data_in[`DCACHE_L2_EVICT_WB_PORT]       = wb_l2_evict_req;
  wb_l2_evict_ready                                    = l2_evict_arb_grant[`DCACHE_L2_EVICT_WB_PORT];

  // Misshandler (UC stores, data is in buffer array)
  l2_evict_arb_bid[`DCACHE_L2_EVICT_MH_PORT]           = mh_l2_evict_valid && wb_allow_evict_bid && !re_ts_l2_evict_inflight;
  l2_evict_arb_data_in[`DCACHE_L2_EVICT_MH_PORT]       = mh_l2_evict_req;
  l2_evict_arb_data_in[`DCACHE_L2_EVICT_MH_PORT].data  = s1_ba_data;
  mh_l2_evict_ready                                    = l2_evict_arb_grant[`DCACHE_L2_EVICT_MH_PORT];

  // Reduce/Non-Cooperative Tensor Store
  l2_evict_arb_bid[`DCACHE_L2_EVICT_RE_PORT]           = re_l2_evict_valid && wb_allow_evict_bid;
  l2_evict_arb_data_in[`DCACHE_L2_EVICT_RE_PORT]       = re_l2_evict_req;
  re_l2_evict_ready                                    = l2_evict_arb_grant[`DCACHE_L2_EVICT_RE_PORT];

  // L2 prefetcher
  l2_evict_arb_bid[`DCACHE_L2_EVICT_PF_PORT]           = pf_l2_evict_valid && wb_allow_evict_bid && !re_ts_l2_evict_inflight;
  l2_evict_arb_data_in[`DCACHE_L2_EVICT_PF_PORT]       = pf_l2_evict_req;
  pf_l2_evict_ready                                    = l2_evict_arb_grant[`DCACHE_L2_EVICT_PF_PORT];

  // Pipe stage/Mux to EVICT FF
  //--------------------------------------------------------------------------
  // Tensor Store (Cooperative Only)
  l2_evict_pipe_valid[`DCACHE_L2_EVICT_IDX_TSTORE]     = ts_l2_evict_valid;
  l2_evict_pipe_data_in[`DCACHE_L2_EVICT_IDX_TSTORE]   = ts_l2_evict_req;
  ts_l2_evict_ready                                    = l2_evict_pipe_ready[`DCACHE_L2_EVICT_IDX_TSTORE];

  // Evict Arbiter
  l2_evict_pipe_valid[`DCACHE_L2_EVICT_IDX_COMM]       = arb_l2_evict_valid;
  l2_evict_pipe_data_in[`DCACHE_L2_EVICT_IDX_COMM]     = arb_l2_evict_req;
  arb_l2_evict_ready                                   = l2_evict_pipe_ready[`DCACHE_L2_EVICT_IDX_COMM];

  // Arbiter to MISS FF
  //---------------------------------------------------------------------------
    // Misshandler
    l2_miss_arb_bid[`DCACHE_L2_MISS_MH_PORT]             = mh_l2_miss_valid;
    l2_miss_arb_data_in[`DCACHE_L2_MISS_MH_PORT]         = mh_l2_miss_req;
    mh_l2_miss_ready                                     = l2_miss_arb_grant[`DCACHE_L2_MISS_MH_PORT];
    
    // Non Coop TensorLoad 0
    l2_miss_arb_bid[`DCACHE_L2_MISS_TL0_PORT]            = tl0_l2_miss_valid_no_coop;
    l2_miss_arb_data_in[`DCACHE_L2_MISS_TL0_PORT]        = tl0_l2_miss_req;
    tl0_l2_miss_ready_no_coop                            = l2_miss_arb_grant[`DCACHE_L2_MISS_TL0_PORT];

    // Non Coop TensorLoad 1
    l2_miss_arb_bid[`DCACHE_L2_MISS_TL1_PORT]            = tl1_l2_miss_valid_no_coop;
    l2_miss_arb_data_in[`DCACHE_L2_MISS_TL1_PORT]        = tl1_l2_miss_req;
    tl1_l2_miss_ready_no_coop                            = l2_miss_arb_grant[`DCACHE_L2_MISS_TL1_PORT];

    // Pipe stage/Mux to MISS FF
    //--------------------------------------------------------------------------
    // TensorLoad 0 (Cooperative Only)
    l2_miss_pipe_valid[`DCACHE_L2_MISS_IDX_SCP]          = tl0_l2_miss_valid;
    l2_miss_pipe_data_in[`DCACHE_L2_MISS_IDX_SCP]        = tl0_l2_miss_req;
    tl0_l2_miss_ready                                    = l2_miss_pipe_ready[`DCACHE_L2_MISS_IDX_SCP];

    // TensorLoad 1 (Cooperative Only)
    l2_miss_pipe_valid[`DCACHE_L2_MISS_IDX_TENB]         = tl1_l2_miss_valid;
    l2_miss_pipe_data_in[`DCACHE_L2_MISS_IDX_TENB]       = tl1_l2_miss_req;
    tl1_l2_miss_ready                                    = l2_miss_pipe_ready[`DCACHE_L2_MISS_IDX_TENB];

    // Miss Arbiter
    l2_miss_pipe_valid[`DCACHE_L2_MISS_IDX_COMM]         = arb_l2_miss_valid;
    l2_miss_pipe_data_in[`DCACHE_L2_MISS_IDX_COMM]       = arb_l2_miss_req;
    arb_l2_miss_ready                                    = l2_miss_pipe_ready[`DCACHE_L2_MISS_IDX_COMM];

end

////////////////////////////////////////////////////////////////////////////////
// L2 Resp destination decoder
////////////////////////////////////////////////////////////////////////////////

logic                            l2_resp_is_for_tensor_0;
logic                            l2_resp_is_for_tensor_1;
logic                            l2_resp_is_for_mh_fill;   // L2 response for a message initiated by Miss Handler to request a fill
logic                            l2_resp_is_for_mh_uc;     // L2 response for a message initiated by Miss Handler to request a UC
logic                            l2_resp_is_for_mh_uc_ok;
logic                            l2_resp_is_for_reduce;    // L2 response for a message initiated by Reduce
logic                            l2_resp_is_reduce_data;
logic                            l2_resp_is_reduce_ready;
logic                            l2_fill_ready;            // Can receive a new fill from L2
logic                            l2_da_write;              // L2 is doing a write to data array
logic                            l2_resp_ready_blocked;    // Flag to stop an L2_MsgRcvData during the first cycle

logic [`DCACHE_MH_FILE_SIZE-1:0] mh_l2_resp_stall;         // Miss handler stall per miss id
logic                            re_buffer_avail;          // Reduce unit has buffers available to accept incoming data
logic                            l2_fill_slot_avail;       // L2 fill has a slot available

logic                            s3_ba_write_en;           // Doing a write to buffer array in S3 due a nacked operation in pipeline

logic                            s4_tl1_tenb_ready;        // TensorLoad1 can accept a new TENB write from L2
logic                            s4_tl1_tenb_ready_reg;
logic                            s4_valid;                 // Valid bit for S4 stage


always_comb begin
  // Computes ready signal for the l2 response port

  // Computes if there's a fill slot available for the following cycles
  //   Only computing for cycle 0 since other cases are not currently used, but keeping code as remainder
  //                       Store will use write port
  l2_fill_slot_avail  = !s4_valid                        && !mh_l2_resp_stall[l2_resp_int.id[`DCACHE_MH_FILE_RANGE]]; // Not writing from store, miss handler already has evicted victim

  // Determine source of request getting the answer
  l2_resp_is_for_tensor_0  = (l2_resp_int.opcode == ET_LINK_RSP_AckData) &&
                              ((et_link_core_miss_id_t'(l2_resp_int.id) >= ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0) &&
                               (et_link_core_miss_id_t'(l2_resp_int.id) <= ET_LINK_Core_Miss_Id_Tensor_Load_Ext_3));
  l2_resp_is_for_tensor_1  = (l2_resp_int.opcode == ET_LINK_RSP_AckData) &&
                              ((et_link_core_miss_id_t'(l2_resp_int.id) >= ET_LINK_Core_Miss_Id_Tensor_Load_Ext_4) &&
                               (et_link_core_miss_id_t'(l2_resp_int.id) <= ET_LINK_Core_Miss_Id_Tensor_Load_Ext_7));
  l2_resp_is_for_mh_fill  = (l2_resp_int.opcode == ET_LINK_RSP_AckData) &&
                             ((et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_0) ||
                              (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_1));
  l2_resp_is_for_mh_uc    = ((l2_resp_int.opcode == ET_LINK_RSP_AckData) || (l2_resp_int.opcode == ET_LINK_RSP_Err)) &&
                           ((et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1));
  l2_resp_is_for_mh_uc_ok = (l2_resp_int.opcode == ET_LINK_RSP_AckData) &&
                           ((et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0) ||
                            (et_link_core_miss_id_t'(l2_resp_int.id) == ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1));
  l2_resp_is_reduce_data  = (l2_resp_int.opcode == ET_LINK_RSP_MsgRcvData) &&
                              (et_link_min_msg_id_t'(l2_resp_int.id) == ET_LINK_Min_Msg_Id_Reduce_Data);
  l2_resp_is_reduce_ready = (l2_resp_int.opcode == ET_LINK_RSP_MsgRcvData) &&
                              (et_link_min_msg_id_t'(l2_resp_int.id) == ET_LINK_Min_Msg_Id_Reduce_Ready);
  l2_resp_is_for_reduce   =  l2_resp_is_reduce_data || l2_resp_is_reduce_ready;

  // Write to data array not set by default
  l2_da_write   = 1'b0;

  // Fill accepted
  // Fill to DCache initiated by Miss Handler
  if (l2_resp_is_for_mh_fill) begin
    l2_fill_ready = l2_fill_slot_avail;   // Can do a fill in current cycle
    l2_da_write   = l2_fill_ready;
  end else if (l2_resp_is_reduce_data) begin
    // Reduce data
    l2_fill_ready = !s3_ba_write_en && re_buffer_avail && !l2_resp_ready_blocked; // Can do a fill if buffer array not doing a write and buffer allocated
  end else if (l2_resp_is_reduce_ready) begin
    // Reduce ready
    l2_fill_ready = !l2_resp_ready_blocked; // Always can accept reduce
  end else if (l2_resp_is_for_mh_uc) begin
    // UC loads receive
    l2_fill_ready = !s3_ba_write_en; // Can do a fill if buffer array not doing a write
  end else if (l2_resp_is_for_tensor_0) begin
    // Tensor Loads from module 0 are accepted to write into Scratchpad
    l2_fill_ready = !s4_valid;            // If there's a store, wait for it to finish
    l2_da_write   = !s4_tl0_load_capture; // When capturing data into Tensor Load module 0, do not write
  end else if (l2_resp_is_for_tensor_1) begin
    l2_fill_ready = s4_tl1_tenb_ready_reg && !l2_resp_ready_blocked;
    l2_da_write   = 1'b0; // Disable the write
  end else begin
    // Other opcodes are always accepted
    l2_fill_ready = 1'b1;
  end

  l2_resp_ready_int = l2_fill_ready;
end

////////////////////////////////////////////////////////////////////////////////
// L2 Resp Blocker
////////////////////////////////////////////////////////////////////////////////
// Some responses require one extra cycle to decode the destination before providing the ready signal,
// so one "blocking" cycle is inserted
//  - A message (L2_MsgRcvData) for ports to provide time for destination address computation in intpipe_csr_file
//  - Fill data for Tensor Load 1 to provide time for ready to be obtained
// The transfer can last 2 cycles if a full line is transfered (512b) or just a cycle if 256b or fewer are transfered

`ifndef DCACHE_USE_REGISTERED_L2_RESP
  logic                            l2_resp_ready_block_op;     // We are in an operation that requires 1 cycle blocking
  logic [1:0]                      l2_resp_count;              // Counter to determine the stage in the L2 response
  logic [1:0]                      l2_resp_count_next;
  logic [1:0]                      l2_resp_count_limit;
  logic                            l2_resp_count_en;

  //         CK     RST    EN                DOUT                 DIN                      DEF
  `RST_EN_FF(clock, reset, l2_resp_count_en, l2_resp_count,       l2_resp_count_next,      2'b0)

  assign l2_resp_count_limit = (l2_resp_int.size == ET_LINK_Line) ? 2'd2: 2'd1;
  assign l2_resp_count_next = (l2_resp_count == l2_resp_count_limit) ? 2'd0 : l2_resp_count + 2'd1;

  assign l2_resp_ready_block_op     = l2_resp_valid_int && (((l2_resp_int.opcode == ET_LINK_RSP_MsgRcvData) && !l2_resp_is_for_reduce)|| l2_resp_is_for_tensor_1);
  assign l2_resp_ready_blocked      = l2_resp_ready_block_op && (l2_resp_count == 2'b0);
  assign l2_resp_count_en           = l2_resp_ready_blocked || (l2_resp_valid_int && l2_resp_ready_int && (l2_resp_count != 2'd0));

`else
  assign l2_resp_ready_blocked = 1'b0;
`endif


////////////////////////////////////////////////////////////////////////////////
// Writeback unit
////////////////////////////////////////////////////////////////////////////////

// Two units need the writeback unit:
//  - Cacheop: when dealing with snoops, cacheops and capacity evicts
//  - miss handler: when dealing with UC loads
// This arbiter grants the access to the writeback unit to the cacheop and miss
// handler

dcache_wb_req [1:0]        wb_arb_data_in;      // Different requests to writeback
dcache_wb_req              wb_arb_data_winner;  // Writeback winner request

dcache_da_read_req         s0_wb_da_req;        // Writeback wants to read from data array
dcache_da_data             s2_da_read_resp;     // Data array read response
logic [`DCACHE_DATA_RANGE] s2_data;             // Same as s2_da_read_resp in single vector
logic [`DCACHE_DATA_RANGE] s2_to_s3_store_data; // Muxed data for the store path (may come from s2_data)

dcache_da_data             s3_data;

logic [1:0]                wb_arb_grant;        // Grant for the writeback arbiter
logic [1:0]                wb_arb_bid;          // Bidding for the writeback arbiter

logic                      co_wb_valid;         // CacheOp unit wants to evict a line
logic                      co_wb_ready;         // CacheOp unit gets granted access to writeback
dcache_wb_req              co_wb_req;           // CacheOp unit request to writeback

dcache_wb_req              mh_wb_req;           // Miss handler request to writeback

logic                      id_wb_da_bid;        // Writeback unit wants to read the data array next cycle
logic                      id_wb_da_grant;      // Writeback unit can read the data array
logic                      s0_wb_da_valid;      // Writeback unit reads the data array
logic                      s1_wb_da_nack;       // Writeback unit read access is nack'ed because of a collision with VPU access

logic                      wb_arb_stall;        // Writeback cannot accept new requests

// Arbiter that grants access to a default port (Mis Handler in this case) even if there is no request
// from that port and unless there is no other request from another port.
// Miss handler has higher priority (higher port) as well.
arb_prio_def_data #(
  .WIDTH       ( $bits(dcache_wb_req)                 ),
  .NUM_CLIENTS ( 2                                    ),
  .DEF_PORT    ( 1                                    )
) writeback_arb (
  // Bidding
  .bid         ( wb_arb_bid         ),
  .stall       ( wb_arb_stall       ),
  // Data inputs
  .data_in     ( wb_arb_data_in     ),
  // Grant
  .grant       ( wb_arb_grant       ),
  .data_winner ( wb_arb_data_winner )
);

always_comb begin
  // CacheOp
  wb_arb_bid[0]     = co_wb_valid;
  wb_arb_data_in[0] = co_wb_req;
  co_wb_ready       = wb_arb_grant[0];

  // Miss handler
  wb_arb_bid[1]     = mh_wb_valid;
  wb_arb_data_in[1] = mh_wb_req;
  mh_wb_ready       = wb_arb_grant[1];
end

// Starting a writeback blocks other units going to evict FF. If in the middle of evicting something
// else, need to prevent starting a new writeback. This condition has to be used also to block requests
// to writeback unit
assign wb_arb_stall  = !wb_ready || re_ts_l2_evict_inflight;
assign wb_valid      = |wb_arb_bid & !re_ts_l2_evict_inflight;

////////////////////////////////////////////////////////////////////////////////
// Writeback unit: writes data to L2
////////////////////////////////////////////////////////////////////////////////

dcache_writeback_unit writeback_unit
(
  // System signals
  .clock           ( clock                   ),
  .reset           ( reset                   ),
  // Request to start a writeback
  .req_ready       ( wb_ready                ),
  .req_valid       ( wb_valid                ),
  .req_data        ( wb_arb_data_winner      ),
  // Port to read DCache data
  .data_req_bid    ( id_wb_da_bid            ),
  .data_req_grant  ( id_wb_da_grant          ),
  .data_req_valid  ( s0_wb_da_valid          ),
  .data_req        ( s0_wb_da_req            ),
  .data_req_nack   ( s1_wb_da_nack           ),
  .data_resp       ( s3_data                 ),
  // Writeback port to the L2
  .l2_req_ready    ( wb_l2_evict_ready       ),
  .l2_req_valid    ( wb_l2_evict_valid       ),
  .l2_req_data     ( wb_l2_evict_req         )
);

////////////////////////////////////////////////////////////////////////////////
// CacheOp unit: deals with cache operations coming from CSR
////////////////////////////////////////////////////////////////////////////////

logic                            id_rq_full;               // Replay queue is full
logic                            id_rq_half_full;          // Replay queue is half full
logic [`PA_RANGE]                s1_phys_addr;             // Physical address
logic [`PA_TAG_EXT_RANGE]        s1_phys_addr_tag;         // Physical address tag
logic [`DCACHE_SET_RANGE]        s1_phys_addr_set;         // Physical address set
dcache_meta_write_req            s1_co_mw_req;             // CacheOp unit meta write request
dcache_line_state                s2_way_hit_state;         // State of the way that hit in S2 stage
dcache_meta_data                 s2_repl_meta;             // Meta data of the replaced way
logic [`VA_RANGE]                s0_co_mr_addr;            // Address from the cacheop unit to access the meta data
logic                            s0_co_mr_addr_msb_err;    // Overflow indication for address
logic [`DCACHE_WAYS-1:0]         s2_tag_match;             // Tag match result for all the ways of the accessed set in the S2
logic                            s0_co_mr_phys;            // CacheOp meta read request is physical
logic                            s0_co_mr_force_set;       // Force reading from a concrete set for Cache Ops
logic                            s0_co_mr_thread_id;       // Set thread for which the Cache Ops are operating
logic                            s1_co_mr_skip_pma;        // Indication to do not make any PMA checking on dirty addr value
logic                            s1_co_mw_grant;           // Meta data write grant for CacheOp unit
logic [`DCACHE_MH_FILE_SIZE-1:0] mh_probe_rdy;             // Miss handler is not operating on same resource
logic [`DCACHE_MH_FILE_SIZE-1:0] mh_metaw_rdy;             // Miss handler is not going to update meta data
logic                            s0_co_mr_valid;           // CacheOp unit wants to do a meta read
logic                            id_co_mr_bid;             // CacheOp unit bids for meta read access for next cycle
logic                            id_co_mr_grant;           // CacheOp unit can do a meta read next cycle
logic                            s0_da_read_en;            // Read enable for the data array
logic                            s1_co_mw_valid;           // CacheOp unit wants to write meta tags
logic                            s1_co_mw_valid_early;     // CacheOp unit wants to write meta tags next cycle
logic                            s2_valid_to_s3;           // Valid signal to S3 stage
logic                            s2_is_write;              // S2 operation is a write
dcache_meta_data [`DCACHE_WAYS-1:0] s2_meta_read;          // Read meta data for all the ways in S2
logic                            s3_valid_to_s4;           // Valid bit to S4 stage
logic                            s3_lrsc_valid;            // Lr/Sc is valid
logic                            core_cache_op_ready;      // Ready to core from cache op

logic                            s1_tlb_wait;              // Flag from TLB to indicate that there has been a miss and fill is pending
logic                            s1_tlb_fail;              // Flag indicating that some kind of error has been reported by TLB

logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] hlock_state;    // Hard lock state
logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] slock_state;    // Soft lock state

logic [`DCACHE_DATA_NUM_WORDS_RANGE] s2_vpu_scp_read_en;   // VPU sent an scratchpad read

logic                            s3_co_da_clear_en;        // Cache ops clear specific set and way of Data Array
logic                            s4_co_da_clear_val;
logic [`DCACHE_SET_RANGE]        s4_co_da_clear_set;
logic [`DCACHE_WAY_RANGE]        s4_co_da_clear_way;
logic                            s4_co_da_clear_idx;
logic                            s4_co_da_clear_rdy;

logic                            id_co_pf_bid;             // CacheOp L1 prefetch bids for S0 pre-arbitration
logic                            id_co_pf_grant;           // CacheOp gets granting to get the pipeline
logic                            s0_co_pf_valid;           // CacheOp validates request to pipeline
dcache_req                       s0_co_pf_req;             // Request from CacheOp

logic                            cache_op_ready_byp;       // Cache op requests from CacheOp unit to CacheOpL2 unit
logic                            cache_op_valid_byp;
core_dcache_op_req               cache_op_req_byp;

logic                            cacheop_busy;             // Cacheop is busy working
logic                            cacheop_l2_busy;          // Cacheop L2 is busy working
logic                            cacheop_busy_prev;        // Cacheop is busy working, delayed 1 cycle
logic                            cacheop_l2_busy_prev;     // Cacheop L2 is busy working, delayed 1 cycle
logic [`CORE_NR_THREADS-1:0][1:0] co_l1_th_id_complete;    // Cache Ops "complete" signals per thread and ID
logic [`CORE_NR_THREADS-1:0][1:0] co_l2_th_id_complete;

logic                            s1_co_nack_set_valid;     // Validates nacking instructions colliding with set operated by Cache Ops
logic [`DCACHE_SET_RANGE]        s1_co_nack_set_value;     // Set being operated by Cache Ops

logic                            mh_clear_slock_valid;     // MH is clearing a soft-lock bit for a given set/way
logic [`DCACHE_SET_RANGE]        mh_clear_slock_set;
logic [`DCACHE_WAY_RANGE]        mh_clear_slock_way;

logic                            rq_conflict_to_co;        // Replay-Queue reports that there are possible conflicts with Cache_Ops

// VM status stored for Tensors and CacheOps
minion_vm_status                 vm_status_co;
minion_vm_status                 vm_status_tl0;
minion_vm_status                 vm_status_tl1;
minion_vm_status                 vm_status_ts;
minion_vm_status                 vm_status_l2;
minion_vm_status                 s1_vm_status;

// Valid signal gated when changing Dcache mode
logic core_cache_op_valid;
assign core_cache_op_valid = core_ctrl.cache_op_valid && !dcache_cfg_stall;

dcache_cache_op_unit cache_op_unit (
  // System signals
  .clock                   ( clock                       ),
  .reset                   ( reset                       ),
  // Chicken bit clock gate enable
  .chicken_bit_dcache      ( chicken_bit_dcache          ),
  // Request bus to perform a cache operation
  .busy                    ( cacheop_busy                ),
  .cache_op_ready          ( core_cache_op_ready         ),
  .cache_op_valid          ( core_cache_op_valid         ),
  .cache_op_req            ( core_ctrl.cache_op_req      ),
  .cache_op_req_next       ( core_ctrl.cache_op_req_next ),
  .th_id_complete          ( co_l1_th_id_complete        ),
  // Request to read the tag state
  .meta_read_bid           ( id_co_mr_bid                ),
  .meta_read_grant         ( id_co_mr_grant              ),
  .meta_read_valid         ( s0_co_mr_valid              ),
  .meta_read_addr          ( s0_co_mr_addr               ),
  .meta_read_addr_msb_err  ( s0_co_mr_addr_msb_err       ),
  .meta_read_phys          ( s0_co_mr_phys               ),
  .meta_read_force_set     ( s0_co_mr_force_set          ),
  .meta_read_thread_id     ( s0_co_mr_thread_id          ),
  .meta_read_skip_pma      ( s1_co_mr_skip_pma           ),
  .meta_read_resp_way_en   ( s2_tag_match                ),
  .meta_read_resp          ( s2_meta_read                ),
  .s1_phys_addr_tag        ( s1_phys_addr_tag            ),
  // Feedback from TLB
  .s1_tlb_wait             ( s1_tlb_wait                 ),
  .s1_tlb_fail             ( s1_tlb_fail                 ),
  // Request to update the tag state
  .meta_write_ready        ( s1_co_mw_grant              ),
  .meta_write_valid_next   ( s1_co_mw_valid_early        ),
  .meta_write_valid        ( s1_co_mw_valid              ),
  .meta_write_req          ( s1_co_mw_req                ),
  // Notification of the set that cache ops is working on
  .nack_set_valid          ( s1_co_nack_set_valid        ),
  .nack_set_value          ( s1_co_nack_set_value        ),
  // Request to the writeback unit to evict a line
  .writeback_ready         ( co_wb_ready                 ),
  .writeback_valid         ( co_wb_valid                 ),
  .writeback_req           ( co_wb_req                   ),
  .writeback_done          ( wb_ready                    ),
  // Request to pipeline to prefetch cache lines
  .id_rq_full              ( id_rq_full                  ),
  .id_co_pf_bid            ( id_co_pf_bid                ),
  .id_co_pf_grant          ( id_co_pf_grant              ),
  .s0_co_pf_valid          ( s0_co_pf_valid              ),
  .s0_co_pf_req            ( s0_co_pf_req                ),
  // Request to CacheOpL2 unit to forward commands
  .cache_op_ready_byp      ( cache_op_ready_byp          ),
  .cache_op_valid_byp      ( cache_op_valid_byp          ),
  .cache_op_req_byp        ( cache_op_req_byp            ),
  // Update soft locks when MH is evicting one locked line
  .mh_clear_slock_valid    ( mh_clear_slock_valid        ),
  .mh_clear_slock_set      ( mh_clear_slock_set          ),
  .mh_clear_slock_way      ( mh_clear_slock_way          ),
  // Lock state info
  .hlock_state             ( hlock_state                 ),
  .slock_state             ( slock_state                 ),
  // Data array clear lines interface
  .da_clear_en             ( s3_co_da_clear_en           ),
  .da_clear_val            ( s4_co_da_clear_val          ),
  .da_clear_set            ( s4_co_da_clear_set          ),
  .da_clear_way            ( s4_co_da_clear_way          ),
  .da_clear_idx            ( s4_co_da_clear_idx          ),
  .da_clear_rdy            ( s4_co_da_clear_rdy          ),
  // Collision avoidance with MH operations
  .mh_probe_rdy            ( mh_probe_rdy                ),
  .mh_metaw_rdy            ( mh_metaw_rdy                ),
  // Potential conflict with instruction in the replay queue
  .rq_conflict             ( rq_conflict_to_co           ),
  // System configuration
  .cfg_clear_all           ( dcache_cfg_clear_all        ),
  .cfg_clear_low           ( dcache_cfg_clear_low        ),
  .cfg_alloc_scp           ( dcache_cfg_alloc_scp        ),
  .cfg_cache_mode          ( dcache_cfg_mode             ),
  // Management of VM status
  .vm_status               ( vm_status                   ),
  .vm_status_co            ( vm_status_co                ),
  // Error flags
  .err_flags               ( cache_ops_err_flags_l1      ),
  // Debug signals
  .csr_debug_signals       ( dbg_csr_cou                 ),
  .sm_match_debug_signals  ( sm_match_debug_signals_co   ),
  .sm_filter_debug_signals ( sm_filter_debug_signals_co  ),
  .sm_data_debug_signals   ( sm_data_debug_signals_co    )
);

////////////////////////////////////////////////////////////////////////////////
// L2 prefetch unit: deals with cache operations related to L2 prefetches
////////////////////////////////////////////////////////////////////////////////

logic           core_co_l2_pref_ready; // Ready to core from L2 cache op
dcache_tlb_resp s1_tlb_resp_data;      // TLB response data
logic           tensorstore_busy;      // TensorStore unit is still not done with previous request
logic           reduce_ready;          // Reduce unit is ready to receive a new request
logic           l2_resp_valid_to_co_l2;

assign l2_resp_valid_to_co_l2 = l2_resp_valid_int && !s3_lrsc_valid;

// Valid signal gated when changing Dcache mode
logic core_co_l2_pref_valid;
assign core_co_l2_pref_valid = core_ctrl.cache_op_valid && !dcache_cfg_stall;

localparam REMOVE_CO_L2_UNIT = 1;
generate if (REMOVE_CO_L2_UNIT == 0) begin : CO_L2
dcache_cache_op_unit_l2 cache_op_unit_l2 (
    // System signals
    .clock                   ( clock                                ),
    .reset                   ( reset                                ),
    .shire_id                ( shire_id                             ),
    .cacheop_max             ( core_ctrl.cache_ctrl.cacheop_max     ),
    .cacheop_reprate         ( core_ctrl.cache_ctrl.cacheop_reprate ),
    // Chicken bit clock gate enable
    .chicken_bit_dcache      ( chicken_bit_dcache                   ),
    // Request bus to perform L2 operations
    .busy                    ( cacheop_l2_busy                      ),
    .th_id_complete          ( co_l2_th_id_complete                 ),
    .l2_resp_valid           ( l2_resp_valid_to_co_l2               ),
    .l2_resp                 ( l2_resp_int                          ),
    // Request bus to perform a cache operation
    .cache_op_ready          ( core_co_l2_pref_ready                ),
    .cache_op_valid          ( core_co_l2_pref_valid                ),
    .cache_op_req            ( core_ctrl.cache_op_req               ),
    .cache_op_req_next       ( core_ctrl.cache_op_req_next          ),
    // Reports to L2 the taken action
    .l2_req_ready            ( pf_l2_evict_ready                    ),
    .l2_req_valid            ( pf_l2_evict_valid                    ),
    .l2_req_data             ( pf_l2_evict_req                      ),
    // Direct interface to cache_op unit for operations to L2 and above
    .cache_op_ready_byp      ( cache_op_ready_byp                   ),
    .cache_op_valid_byp      ( cache_op_valid_byp                   ),
    .cache_op_req_byp        ( cache_op_req_byp                     ),
    // Interfaces the pipeline to use the TLB
    .s0_available            ( s0_available_to_co_l2                ),
    .s0_addr_load            ( s0_co_l2_addr_load                   ),
    .s0_addr                 ( s0_co_l2_addr                        ),
    .s0_addr_msb_err         ( s0_co_l2_addr_msb_err                ),
    .s0_thread_id            ( s0_co_l2_thread_id                   ),
    .s1_available            ( s1_available_to_co_l2                ),
    .s1_addr_load            ( s1_co_l2_addr_load                   ),
    .s1_tlb_wait             ( s1_tlb_wait                          ),
    .s1_tlb_fail             ( s1_tlb_fail                          ),
    .s1_tlb_addr             ( s1_tlb_resp_data.ppn                 ),
    // Management of VM status
    .vm_status               ( vm_status                            ),
    .vm_status_co            ( vm_status_co                         ),
    .vm_status_l2            ( vm_status_l2                         ),
    // Error flags
    .err_flags               ( cache_ops_err_flags_l2               ),
    // Bus error
    .bus_err                 ( bus_err_co_l2                        ),
    // Debug signals
    .csr_debug_signals       ( dbg_csr_coul2                        ),
    .sm_match_debug_signals  ( sm_match_debug_signals_col2          ),
    .sm_filter_debug_signals ( sm_filter_debug_signals_col2         ),
    .sm_data_debug_signals   ( sm_data_debug_signals_col2           )
);
end else begin : CO_L2 // block: CO_L2
   assign cacheop_l2_busy = 1'b0;
   assign co_l2_th_id_complete = '1;
   assign core_co_l2_pref_ready = 1'b0;
   assign pf_l2_evict_valid = '0;
   assign pf_l2_evict_req = '0;
   assign cache_op_ready_byp = '0;
   assign s0_co_l2_addr = '0;
   assign s0_co_l2_addr_msb_err ='0;
   assign s0_co_l2_thread_id = '0;
   assign s0_co_l2_addr_load = '0;
   assign s1_co_l2_addr_load = '0;
   assign vm_status_l2 = '0;
   assign cache_ops_err_flags_l2 = '0;
   assign bus_err_co_l2 = '0;
   assign dbg_csr_coul2 = '0;
   assign sm_match_debug_signals_col2 = '0;
   assign sm_filter_debug_signals_col2 = '0;
   assign sm_data_debug_signals_col2 = '0;
end   
endgenerate

logic [`DCACHE_TL_ERROR_BITS-1:0] tensor_load_err_flags_0;
logic [`DCACHE_TL_ERROR_BITS-1:0] tensor_load_err_flags_1;
logic [`CORE_NR_THREADS-1:0]      cache_op_busy_changed;
logic [`CORE_NR_THREADS-1:0]      cache_op_busy_changed_z1;

always_comb begin
    // Ready from L2 prefetch is only set if the operation is going to be handled by it.
    // This way, operations like prefetch, evict or flush not involving L1 can be executed
    // in parallel to other operations involving L1
   if (REMOVE_CO_L2_UNIT) begin
      core_ctrl_resp.cache_op_ready    = core_cache_op_ready & !dcache_cfg_stall;
   end else begin
      core_ctrl_resp.cache_op_ready    = (core_co_l2_pref_ready || core_cache_op_ready) & !dcache_cfg_stall;      
   end

  // Detect a change in the cache ops state machines.
  for (integer i=0; i<`CORE_NR_THREADS; i++) begin
    cache_op_busy_changed[i] = (!cacheop_busy           && cacheop_busy_prev    && (i == s0_co_mr_thread_id))
                            || (!cacheop_l2_busy        && cacheop_l2_busy_prev && (i == s0_co_l2_thread_id))
                            || (!dcache_cfg_stall       && dcache_cfg_stall_prev);
  end

  // Indicate a "change" first to those threads not being served and then to the thread being served,
  // but only when a change is detected
  core_ctrl_resp.cache_op_busy_changed = {`CORE_NR_THREADS{|cache_op_busy_changed}}    & ~cache_op_busy_changed
                                       | {`CORE_NR_THREADS{|cache_op_busy_changed_z1}} & cache_op_busy_changed_z1;

  // Readies and IDs for tensorwait
  core_ctrl_resp.cache_op_complete_l1 = co_l1_th_id_complete;
  core_ctrl_resp.cache_op_complete_l2 = co_l2_th_id_complete;

  // TensorStore and TensorReduce
  core_ctrl_resp.ts_ready     =  reduce_ready ? !tensorstore_busy & !dcache_cfg_stall : 1'b0;
  core_ctrl_resp.reduce_ready = tensorstore_busy? 1'b0 : reduce_ready & !dcache_cfg_stall;

  // Combine memory access faults from different sources: dcache_cache_op_unit,  dcache_cache_op_unit_l2, dcache_reduce, and dcache_tensor_load
  tensor_reduce_mem_err_flag      = tensor_reduce_err_all_flags[`DCACHE_ERR_FLAG_MEM_FAULT];
  tensor_load_mem_err_flags[1]    = tensor_load_err_flags_0[`DCACHE_TL_ERROR_ACCESS_FAULT_1] | tensor_load_err_flags_1[`DCACHE_TL_ERROR_ACCESS_FAULT_1];
  tensor_load_mem_err_flags[0]    = tensor_load_err_flags_0[`DCACHE_TL_ERROR_ACCESS_FAULT_0] | tensor_load_err_flags_1[`DCACHE_TL_ERROR_ACCESS_FAULT_0];

  cache_ops_err_flags = cache_ops_err_flags_l1;
  for (integer i=0; i<`CORE_NR_THREADS; i++) begin
    cache_ops_err_flags[`DCACHE_ERR_FLAG_MEM_FAULT][i] |= cache_ops_err_flags_l2[i] | tensor_reduce_mem_err_flag[i] | tensor_load_mem_err_flags[i];
  end
end

`FF(clock, {cacheop_busy_prev, cacheop_l2_busy_prev}, {cacheop_busy, cacheop_l2_busy})
`FF(clock, dcache_cfg_stall_prev,                     dcache_cfg_stall)
`FF(clock, cache_op_busy_changed_z1,                  cache_op_busy_changed)

////////////////////////////////////////////////////////////////////////////////
// Reduce module
// This module takes care of the control in the dcache unit of the reduce
// instructions. This instruction allows reducing the contents (or part) of
// the vector RF between two minions.
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_BUFFER_ID_RANGE] s1_re_ba_read_addr;       // Reads the buffer array
logic [`DCACHE_BUFFER_ID_RANGE] s2_ba_write_entry;        // Which entry in the buffer array is being allocated
logic [`DCACHE_BUFFER_ID_RANGE] s3_re_ba_write_addr;      // Which entry in the buffer array a new reduce data must be written
logic                           s1_reduce_send_inst;      // Reduce send instruction in S1 stage
logic                           s1_reduce_op_inst;        // Reduce op instruction in S1 stage
logic                           s1_re_ba_read_en;         // Reads the buffer array
logic                           s1_re_ba_read_en_spec;    // Reads the buffer array (speculative, better timing)
logic                           s1_mh_force_ba_rptr;      // Force the buffer array read pointer (request)
logic                           s1_mh_force_ba_rptr_qual; // Force the buffer array read pointer (granted)
logic                           s1_valid;                 // Valid transaction
logic                           s2_valid;                 // Valid transaction in S2 stage
logic                           s2_reduce_send_inst;      // Reduce send instruction in S2 stage
logic                           s2_reduce_op_inst;        // Reduce op instruction in S2 stage
logic                           s2_re_ba_write_needed;    // Allocate an specific entry in the buffer array
logic                           s3_reduce_send_inst;      // Reduce send instruction in S3 stage
logic                           s3_reduce_op_inst;        // Reduce op instruction in S3 stage
logic                           re_pipe_used;             // Pipeline is used by a reduce instruction
logic                           re_ba_alloc;              // Reduce allocates an entry in the buffer array
logic                           re_ba_full;               // Buffer array is full for reduce
logic                           re_ba_dealloc;            // Reduce deallocates an entry in the buffer array
logic                           re_wait_l2_to_ba_resp;    // Reduce is waiting for L2 response to go to buffer array
logic                           re_ba_entries_required;   // Reduce needs to use buffer array
logic                           id_ba_full;               // Buffer array is full
dcache_req                      s2_req;                   // Dcache request in S2 stage
logic                           ts_scp_read;              // TensorStore is reading data from Scratchpad
logic [`DCACHE_WAY_RANGE]       ts_scp_read_way;          // Way and Addr associated to the scratchpad line to read
logic [`DCACHE_LRAM_ADDR_RANGE] ts_scp_read_addr;
logic                           ts_col_size_is_128b;      // Tensor Store column size is 128-bit

logic                           mh_going_to_uc_store;     // MH indicates that it wants to do a US Store

// Valid signal gated when changing Dcache mode
logic reduce_start;
logic tensor_store_start;
assign reduce_start = core_ctrl.reduce_start && !dcache_cfg_stall;
assign tensor_store_start = core_ctrl.tensorstore_start & !dcache_cfg_stall;

dcache_reduce reduce (
    // System signals
    .clock                   ( clock                             ),
    .reset                   ( reset                             ),
    .global_min_id           ( {shire_id,shire_min_id}           ),
    // Chicken bit clock gate enable
    .chicken_bit_dcache      ( chicken_bit_dcache                ),
    // Request port for new reduce operation
    .reduce_ready            ( reduce_ready                      ),
    .reduce_start            ( reduce_start                      ),
    .tensorstore_busy        ( tensorstore_busy                  ),
    .tensorstore_start       ( tensor_store_start                ),
    .tensorstore_stride      ( core_ctrl.tensorstore_stride      ),
    .reduce_tensorstore_ctrl ( core_ctrl.reduce_tensorstore_ctrl ),
    .thread_id               ( core_ctrl.thread_id               ),
    // Port to get messages from sender/receiver
    .l2_resp_valid           ( l2_resp_valid_ready_int           ),
    .l2_resp                 ( l2_resp_int                       ),
    .buffer_avail            ( re_buffer_avail                   ),
    // Port to send messages to other minions or non-Cooperative Tensor Stores
    .re_l2_req_ready         ( re_l2_evict_ready                 ),
    .re_l2_req_valid         ( re_l2_evict_valid                 ),
    .re_l2_req               ( re_l2_evict_req                   ),
    // Port to do Cooperative Tensor Stores
    .ts_l2_req_ready         ( ts_l2_evict_ready                 ),
    .ts_l2_req_valid         ( ts_l2_evict_valid                 ),
    .ts_l2_req               ( ts_l2_evict_req                   ),
    // Indication that one operation is ongoing
    .re_ts_l2_req_inflight   ( re_ts_l2_evict_inflight           ),
    // Control to Dcache pipeline
    .pipe_used               ( re_pipe_used                      ),
    .s1_inst_send_val        ( s1_reduce_send_inst               ),
    .s2_inst_send_val        ( s2_reduce_send_inst               ),
    .s3_inst_send_val        ( s3_reduce_send_inst               ),
    .s1_inst_op_val          ( s1_reduce_op_inst                 ),
    .s2_inst_op_val          ( s2_reduce_op_inst                 ),
    .s3_inst_op_val          ( s3_reduce_op_inst                 ),
    .ts_scp_read             ( ts_scp_read                       ),
    .ts_scp_read_way         ( ts_scp_read_way                   ),
    .ts_scp_read_addr        ( ts_scp_read_addr                  ),
    .ts_col_size_is_128b     ( ts_col_size_is_128b               ),
    // Control to translate virtual addresses
    .s0_available            ( s0_available_to_ts                ),
    .s0_addr_load            ( s0_ts_addr_load                   ),
    .s0_addr                 ( s0_ts_addr                        ),
    .s0_addr_msb_err         ( s0_ts_addr_msb_err                ),
    .s0_thread_id            ( s0_ts_thread_id                   ),
    .s1_available            ( s1_available_to_ts                ),
    .s1_addr_load            ( s1_ts_addr_load                   ),
    .s1_tlb_wait             ( s1_tlb_wait                       ),
    .s1_tlb_fail             ( s1_tlb_fail                       ),
    .s2_addr                 ( s2_req.addr[`PA_RANGE]            ),
    // Control of buffer array
    .ba_full                 ( re_ba_full                        ),
    .ba_alloc                ( re_ba_alloc                       ),
    .ba_write_needed         ( s2_re_ba_write_needed             ),
    .s2_valid                ( s2_valid                          ),
    .s2_ba_written_entry     ( s2_ba_write_entry                 ),
    .s3_ba_write_addr        ( s3_re_ba_write_addr               ),
    .wait_l2_to_ba_resp      ( re_wait_l2_to_ba_resp             ),
    .ba_entries_required     ( re_ba_entries_required            ),
    // Buffer array read port
    .s1_valid                ( s1_valid                          ),
    .ba_read_used            ( s1_mh_force_ba_rptr               ),
    .s1_ba_data              ( s1_ba_data                        ),
    .ba_read_en              ( s1_re_ba_read_en                  ),
    .ba_read_en_spec         ( s1_re_ba_read_en_spec             ),
    .ba_read_addr            ( s1_re_ba_read_addr                ),
    // Scratchpad configuration bit
    .scp_ctrl_on             ( scp_ctrl_on                       ),
    // Control bits to VPU to execute a reduce
    .vpu_reduce_wait         ( s1_vpu_ctrl.reduce_wait           ),
    .vpu_ctrl                ( vpu_reduce_ctrl                   ),
    // Management of VM status
    .vm_status               ( vm_status                         ),
    .vm_status_ts            ( vm_status_ts                      ),
    // For performance monitors
    .ts_start                ( ts_start                          ),
    .ts_op                   ( ts_op                             ),
    // Indication from MH to avoid collisions using BA
    .mh_going_to_uc_store    ( mh_going_to_uc_store              ),
    // Error flags
    .err_flags               ( tensor_reduce_err_all_flags       ),
    // Bus error
    .bus_err                 ( bus_err_re                        ),
    // debug
    .csr_debug_signals       ( dbg_csr_ts                        ),
    .sm_match_debug_signals  ( sm_match_debug_signals_ts         ),
    .sm_filter_debug_signals ( sm_filter_debug_signals_ts        ),
    .sm_data_debug_signals   ( sm_data_debug_signals_ts          )
);

assign tensor_reduce_err_flags = tensor_reduce_err_all_flags[`DCACHE_ERR_FLAG_PARTNER_ID][0];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Allowing the access to S0 and S1 of the modules that want
// address translation
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Higher priority is for Tensor Load, followed by Tensor Store and finally CacheOps L2 (prefetch)

assign s0_available_to_tl    = !(s0_valid_to_s1 || s0_md_read_en);
assign s1_available_to_tl    = !(s1_clk_en || s1_ts_addr_load || s1_co_l2_addr_load);

assign s0_available_to_ts    = !(s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load);
assign s1_available_to_ts    = !(s1_clk_en || s1_tl_addr_load || s1_co_l2_addr_load);

assign s0_available_to_co_l2 = !(s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load || s0_ts_addr_load);
assign s1_available_to_co_l2 = !(s1_clk_en || s1_tl_addr_load || s1_ts_addr_load);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ID Stage,
// This is the pipeline arbiter, one cycle before S0
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

logic id_rq_grant;            // Grant to replay queue to replay an element
logic id_core_pipe_available; // Grant to core access to pipeline if it needs it
logic id_rq_replay_bid;       // Replay queue wants to replay an instruction next cycle

logic id_rq_replay_bid_qual;  // Qualified bid taking into account if intpitpe has a VPU
                              // instruction in WB that will write the intpipe in F8

logic id_dbg_nvpu_bid;        // Debug bidding when not VPU bidding
logic id_wb_da_nvpu_bid;      // WB bidding when not VPU bidding

logic vpu_scp_bid_1;          // Pipelined versions of VPU bid request to access the pipeline
logic vpu_scp_bid_2;

// Five units need to access the dcache pipeline:
//  - FPU reads to scratchpad
//  - CacheOps unit (two possible access requests)
//  - Replays
//  - Writebacks to send data to L2
//  - Regular requests in S0 stage
// This arbiter grants the access to all the units. As we need to send the
// stall signal to core in ID stage, this arbiter is done one cycle ahead to
// the actual access to the pipeline.
// As Gathers/Scatters are actually processed one cycle later in core,
// once they win the access, this is kept until they finish
// Writes have priority over reads, so in case that a store or fill require
// the write port next cycle, no grant is given.
// Priority is static:
//   - CacheOps (meta data read only) having higher priority
//   - Debug port (meta data read only)
//   - WriteBacks (data read only)
//   - Replays (meta data and data read)
//   - CacheOps for prefetch (meta data read only, but misses go to replay queue, so lower priority than replay)
//   - New core requests having the lowest priority.

// This is the pre-arbitration
always_comb begin
  // Writeback and debug bid are not compatible with VPU bid
  id_dbg_nvpu_bid   = id_dbg_bid   && !vpu_scp_bid_2;
  id_wb_da_nvpu_bid = id_wb_da_bid && !vpu_scp_bid_2;

  // Grants to all clients
  id_co_mr_grant         = !s0_core_gsc      && !re_pipe_used &&  id_co_mr_bid;
  id_dbg_grant           = !s0_core_gsc      && !re_pipe_used && !id_co_mr_bid &&  id_dbg_nvpu_bid;
  id_wb_da_grant         = !s0_core_gsc      && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid &&  id_wb_da_nvpu_bid;
  id_rq_grant            = !s0_core_gsc      && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid && !id_wb_da_nvpu_bid &&  id_rq_replay_bid_qual;
  id_co_pf_grant         = !s0_core_gsc      && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid && !id_wb_da_nvpu_bid && !id_rq_replay_bid_qual &&  id_co_pf_bid;
  id_core_pipe_available = !dcache_cfg_stall && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid && !id_wb_da_nvpu_bid && !id_rq_replay_bid_qual && !id_co_pf_bid;
end

always_comb begin
  // Computes the ready signal to core in ID stage. It is ready when there
  // is a replay queue entry and buffer array entry available and the core
  // has available the dcache pipeline
  // In case that reduce requests a buffer allocation, give priority to it
  // If reduce is waiting data from sender, only allow half of the replay
  // queue to be used, to ensure that buffer array keeps free entries for
  // reduce data
  id_core_ready = (re_ba_entries_required ? ~id_rq_half_full : ~id_rq_full) && ~id_ba_full && id_core_pipe_available && !re_ba_alloc && !s0_co_pf_valid;
end


// A VPU instruction in WB will write to intpipe register. Avoid that a replay instruction gets in to avoid collision.
// We cannot check if the instruction to be replayed goes to intpipe because of timing limitations
assign id_rq_replay_bid_qual = id_rq_replay_bid && !wb_dcache_fp_toint && !vpu_scp_bid_2;

// Bid from VPU when reading from SCP is "orientative" as it is aligned with S1 accesses (2 cycles later)
// A "periodic" signal with period of 2 cycles is generated from VPU so that it can align a '0' in "bid"
// at ID stage with a '0' in "read_en" at S1 stage. Then, it is safe to add a 2-cycle pipeline.
//         CLK    RST    DOUT           DIN                      DEF
`RST_FF   (clock, reset, vpu_scp_bid_1, s1_vpu_ctrl.scp_req.bid, 1'b0)
`RST_FF   (clock, reset, vpu_scp_bid_2, vpu_scp_bid_1,           1'b0)


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S0 stage
// In this stage the DCache pipeline starts. It is called "exe" stage in the
// core. The s0_cpu_req signals are used as they belong to this pipestage of
// the core.
// In this stage the meta tags are accessed in order to know which way to read
// in s1 stage. This is done to save power at cost of extra latency in the
// access.
// The access to this stage is pre-arbitrated a cycle earlier by all the users
// of the tag array. The reason is to prevent nacking instructions from core
// in EXE stage, where they have to be replayed which have a perf and energy
// cost (compared vs stalling one cycle in ID).
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_req                         s0_req;             // Request for S0 stage
logic [`VA_RANGE]                  s0_req_addr;        // Request address for S0 separate from s0_req to allow different clock enables
dcache_replay_req                  s0_rq_replay;       // Replay queue replay instruction
logic [`DCACHE_REPLAYQ_ADDR_RANGE] s0_rq_replay_entry; // Which replay entry we are replaying
logic [`DCACHE_REPLAYQ_AGE_ID]     s0_rq_replay_age;   // Age of the replay
logic                              s0_rq_replay_uc_load_ok; // If it is a UC load 2nd, this indicates if UC load went well
logic                              s0_rq_replay_miss_fill_ok; // If it is a replayed instruction, this indicates if its fill went well
logic                              s0_bp_conf_valid;
debug_bp_t                         s0_bp_conf_cmd;
`ifdef DCACHE_REPORT_PC
  logic [`PC_RANGE_EXT]              s0_req_pc;          // PC of the instruction that did the request in S0
  logic                              s0_req_pc_load;     // Enables loading PC of the instruction to S1
`endif

////////////////////////////////////////////////////////////////////////////////
// Gather/Scatter masked kills
////////////////////////////////////////////////////////////////////////////////

logic s0_rq_replay_valid;       // Replay queue wants to replay
logic s0_core_req_valid_killed; // Killed core valid request
logic s0_valid_to_s1_ordered;   // S0 valid to S1 going to S3 ordered logic (pessimistic version, better timing)
logic s0_req_thread_id_ordered; // Thread ID to take into account for S3 ordered logic (depends only on core or replay queue instructions)
logic s0_core_req_is_gsc32;     // Gather/Scatter on 32-byte block
logic s0_core_req_is_valid_gsc; // Request from core is a valid regular GSC

logic [`CORE_NR_THREADS-1:0]  s1_tlb_flush_pipeline; // Indication to flush the pipeline for new core instructions when some event happens in S1

always_comb begin
  // Check for gather/scatter on 32-byte block operations as they use the gather/scatter request type
  s0_core_req_is_gsc32 = dcache_cmd_is_gsc32(s0_core_req.cmd);

  // Check for a valid regular gather/scatter
  s0_core_req_is_valid_gsc = s0_core_req_valid && dcache_type_is_gsc(s0_core_req.typ) && !s0_core_req_is_gsc32;

  // Kill the instruction from core in S0 when:
  s0_core_req_valid_killed = s0_core_req_is_valid_gsc && !s0_core_req.ps_mask[s0_core_req.gsc_cnt]                     // Masked G/S
                          || s0_core_req_valid && (dcache_type_is_ps(s0_core_req.typ) && (s0_core_req.ps_mask == 'b0)) // Fully masked operation
                          || s1_tlb_flush_pipeline[s0_core_req.dest.thread_id];                                        // Pipeline being flushed

  // Valid transaction going to S1
  s0_valid_to_s1  = s0_core_req_valid && !s0_core_req_valid_killed // Non-killed due mask core requests
                 || s0_rq_replay_valid                             // ReplayQueue requests
                 || s0_co_pf_valid;                                // Cache Ops doing a prefetch request

  // Due timing, we send to the "s3_ordered" logic a pessimistic version
  // where ps_mask is not used
  s0_valid_to_s1_ordered = s0_core_req_valid || s0_rq_replay_valid;
  s0_req_thread_id_ordered = s0_core_req_valid ? s0_core_req.dest.thread_id : s0_rq_replay.dest.thread_id;
end

////////////////////////////////////////////////////////////////////////////////
// MetaData (tag + state) array
////////////////////////////////////////////////////////////////////////////////

// Selects the unit that access the tag read port. Access done in next cycle
//  - CacheOp Unit
//  - Replays
//  - New core requests
//  - Debug port
always_comb begin
  // Combine enable signals for regular operations and debug port
  s0_md_read_en = (s0_co_mr_valid || s0_rq_replay_valid || s0_core_req_valid || s0_co_pf_valid || s0_dbg_valid);
end

////////////////////////////////////////////////////////////////////////////////
// Data unit that stores the contents of the Dcache
////////////////////////////////////////////////////////////////////////////////

dcache_da_read_req s0_da_read_req;       // Data array read request winner

// Data read input muxing. Arbitration done in previous cycle.
//  - Replays
//  - New core requests
// Reads for writeback are registered independently
always_comb begin
  // Read enable
  s0_da_read_en  = s0_valid_to_s1;
  s0_da_read_req = {$bits(s0_da_read_req){1'b0}};

  // Replay Queue
  if (s0_rq_replay_valid) begin
    s0_da_read_en      &= !dcache_cmd_is_prefetch(s0_rq_replay.cmd);   // Disable data read for prefetches
    s0_da_read_req.way  = 'b0;
    s0_da_read_req.addr = s0_rq_replay.addr[`DCACHE_LRAM_ADDR_RANGE];
  end
  // New requests (disable bidding if g/s masked)
  else begin
    s0_da_read_en      &= !dcache_cmd_is_prefetch(s0_core_req.cmd);    // Disable data read for prefetches
    s0_da_read_req.way  = 'b0;
    s0_da_read_req.addr = s0_core_req.addr[`DCACHE_LRAM_ADDR_RANGE];
  end

  s0_da_read_req.addr[`DCACHE_SET_ADDR_RANGE] = dcache_set_correct(s0_da_read_req.addr[`DCACHE_SET_ADDR_RANGE],dcache_cfg_mode,s0_req.dest.thread_id);
end

////////////////////////////////////////////////////////////////////////////////
// We select the request that is going to be flowing through the pipeline
////////////////////////////////////////////////////////////////////////////////
   
logic s0_req_addr_load;  // Enables loading request address to S1
logic s0_req_force_set;
prv_t s0_core_vmstatus_prv;   // Privilage level for s0_thread_id
   

assign s0_req_addr_load = s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load || s0_ts_addr_load || s0_co_l2_addr_load;

// Arbitration done in previous cycle
//  - Writebacks
//  - ReplayQ
//  - New core requests
always_comb begin
  s0_req = '0; // Needed for Verilator
  
  // Do not use addr in the struct, it has a different clk enable signal
  s0_req.addr                = `VA_SIZE'b0;
  
  // By default request cannot force set, unless the operation is debug or a cache op reading metadata
  // Just set a default here for set, will be updated at the end of this combinational block
  s0_req_force_set = 1'b0;
  s0_req.set       = {`DCACHE_SET_SZ{1'b0}};

  // Replay queue won (default)
  s0_req_addr           = s0_rq_replay.addr;
  s0_req.addr_msb_err   = 1'b0;
  s0_req.phys           = s0_rq_replay.phys;
  s0_req.dest           = s0_rq_replay.dest;
  s0_req.cmd            = s0_rq_replay.cmd;
  s0_req.typ            = s0_rq_replay.typ;
  s0_req.gsc_cnt        = s0_rq_replay.gsc_cnt;
  s0_req.gsc32_idx      = s0_rq_replay.gsc32_idx;
  s0_req.ps_mask        = s0_rq_replay.ps_mask;
  s0_req.replay         = 1'b1;
  s0_req.replay_entry   = s0_rq_replay_entry;
  s0_req.replay_age     = s0_rq_replay_age;
  s0_req.cache_op       = s0_rq_replay.cache_op;
  s0_req.misaligned     = s0_rq_replay.misaligned;
  s0_req.misaligned_2nd = s0_rq_replay.misaligned_2nd;
  s0_req.buf_alloc      = s0_rq_replay.buf_alloc;
  s0_req.buf_id         = s0_rq_replay.buf_id;
  s0_req.cacheable      = s0_rq_replay.cacheable;
  s0_req.uc_load_2nd    = s0_rq_replay.uc_load_2nd;
  s0_req.uc_load_ok     = s0_rq_replay_uc_load_ok;
  s0_req.miss_fill_ok   = s0_rq_replay_miss_fill_ok;
  s0_req.vm_sel         = s0_rq_replay.vm_sel;
  s0_req.chunk_read     = 'b0; // Not used in S0
  s0_req.mem_global     = s0_rq_replay.mem_global;
  s0_req.prv            = s0_rq_replay.prv;      
  `ifdef DCACHE_REPORT_PC
    s0_req_pc             = s0_rq_replay.pc;
  `endif
  s0_bp_conf_valid      = 1'b0;
  s0_bp_conf_cmd        = s1_bp_conf.cmd;

  // New core request won
  if (s0_core_req_valid) begin
    s0_req_addr           = s0_core_req.addr;
    s0_req.addr_msb_err   = s0_core_req.addr_msb_err;
    s0_req.phys           = s0_core_req.phys;
    s0_req.dest           = s0_core_req.dest;
    s0_req.cmd            = s0_core_req.cmd;
    s0_req.typ            = s0_core_req.typ;
    s0_req.gsc_cnt        = s0_core_req.gsc_cnt;
    s0_req.ps_mask        = s0_core_req.ps_mask;
    s0_req.gsc32_idx      = s0_core_req.gsc32_idx;
    s0_req.replay         = 1'b0;
    s0_req.replay_entry   = 'b0;
    s0_req.replay_age     = 'b0;
    s0_req.cache_op       = 1'b0;
    s0_req.misaligned     = 1'b0;
    s0_req.misaligned_2nd = 1'b0;
    s0_req.buf_alloc      = 1'b0;
    s0_req.buf_id         = 'b0;
    s0_req.cacheable      = 1'b1;
    s0_req.uc_load_2nd    = 1'b0;
    s0_req.uc_load_ok     = 1'b0;
    s0_req.miss_fill_ok   = 1'b0;
    s0_req.vm_sel         = `DCACHE_VM_SEL_IP;
    s0_req.mem_global     = s0_core_req.mem_global;
    `ifdef DCACHE_REPORT_PC
      s0_req_pc             = s0_core_req.pc;
    `endif
    s0_bp_conf_valid      = !s0_core_req_valid_killed;
    s0_bp_conf_cmd        = dcache_cmd_is_amo(s0_core_req.cmd)  ? debug_bp_amo :
                            dcache_cmd_is_read(s0_core_req.cmd) ? debug_bp_load :
                            debug_bp_store;
  end
  // CacheOp unit won (only change the fields that it needs)
  else if (s0_co_mr_valid) begin
    s0_req_addr           = s0_co_mr_addr;
    s0_req.addr_msb_err   = s0_co_mr_addr_msb_err;
    s0_req.set            = s0_co_mr_addr[`DCACHE_SET_ADDR_RANGE];
    s0_req.cmd            = dcache_cmd_XRD;
    s0_req.phys           = s0_co_mr_phys;
    s0_req.replay         = 1'b0;
    s0_req.vm_sel         = `DCACHE_VM_SEL_CO;
    s0_req.dest.thread_id = s0_co_mr_thread_id;
    s0_req_force_set      = s0_co_mr_force_set;
    s0_bp_conf_valid      = 1'b0;
    s0_bp_conf_cmd        = debug_bp_co;
  end
  // Debug port reading metadata (only set is required, ensure TLB is doing nothing)
  else if (s0_dbg_valid) begin
    s0_req.set       = dbg_md_read_addr[`DCACHE_SET_RANGE];
    s0_req.phys      = 1'b1;
    s0_req.replay    = 1'b0;
    s0_bp_conf_valid = 1'b0;
    s0_req_force_set = 1'b1;
  end
  // CacheOp unit won to do a prefetch (with null destination)
  else if (s0_co_pf_valid) begin
    s0_req_addr      = s0_co_pf_req.addr;
    s0_req           = s0_co_pf_req;
    s0_bp_conf_valid = 1'b1;
    s0_bp_conf_cmd   = debug_bp_co;
  end
  // TL unit sets the address and VM status selection only
  else if (s0_tl_addr_load) begin
    s0_req_addr         = s0_tl_addr;
    s0_req.addr_msb_err = s0_tl_addr_msb_err;
    s0_req.vm_sel       = s0_tl_vm_sel;
    s0_req.dest         = '{fp: 1'b0, addr: `XREG_ADDR_SIZE'b0, thread_id: s0_tl_thread_id};
    s0_req.phys         = 1'b0;
    s0_req.replay       = 1'b0;
    s0_req.cmd          = dcache_cmd_XRD;
    s0_req.typ          = dcache_type_B;
    s0_bp_conf_valid    = 1'b1;
    s0_bp_conf_cmd      = debug_bp_tensor_load;
  end
  // TS unit sets the address and VM status selection only
  else if (s0_ts_addr_load) begin
    s0_req_addr         = s0_ts_addr;
    s0_req.addr_msb_err = s0_ts_addr_msb_err;
    s0_req.vm_sel       = `DCACHE_VM_SEL_TS;
    s0_req.dest         = '{fp: 1'b0, addr: `XREG_ADDR_SIZE'b0, thread_id: s0_ts_thread_id};
    s0_req.phys         = 1'b0;
    s0_req.replay       = 1'b0;
    s0_req.cmd          = dcache_cmd_XRD;
    s0_req.typ          = dcache_type_B;
    s0_bp_conf_valid    = 1'b1;
    s0_bp_conf_cmd      = debug_bp_tensor_store;
  end
  // L2 prefetch sets the address and VM status selection only
  else if (s0_co_l2_addr_load) begin
    s0_req_addr         = s0_co_l2_addr;
    s0_req.addr_msb_err = s0_co_l2_addr_msb_err;
    s0_req.vm_sel       = `DCACHE_VM_SEL_L2;
    s0_req.dest         = '{fp: 1'b0, addr: `XREG_ADDR_SIZE'b0, thread_id: s0_co_l2_thread_id};
    s0_req.phys         = 1'b0;
    s0_req.replay       = 1'b0;
    s0_req.cmd          = dcache_cmd_XRD;
    s0_req.typ          = dcache_type_B;
    s0_bp_conf_valid    = 1'b1;
    s0_bp_conf_cmd      = debug_bp_co;
  end

  // Ensure that set matches the current set configuration.
  // Debug uses the set value as a direct access to the LRAM
  if (!s0_req_force_set)
    s0_req.set = dcache_set_correct(s0_req_addr[`DCACHE_SET_ADDR_RANGE],dcache_cfg_mode,s0_req.dest.thread_id);

  // Data not used in S0
  s0_req.data = `DCACHE_DATA_SIZE'b0;

  // PC only loaded when instruction comes from core
  `ifdef DCACHE_REPORT_PC
    s0_req_pc_load = s0_core_req_valid | s0_rq_replay_valid;
  `endif
end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S1 stage
// It is called TAG stage in the core. During this stage, the tag compare is
// finilized in a per way fashion and the TLB is accessed (as the tags are
// stored in PA). Once is known in which way the data is, the enables are sent
// to the data array.
// Also, some exceptions are checked (misalign, TLB rights, ...)
// The LRU is access during this stage. If the LRU is updated in S2 and the
// same set is accessed in S1, the updated LRU bits are bypassed
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_da_read_req [`DCACHE_LRAM_BANK_RANGE] s1_da_read_req_hit;   // Data array read request after muxing the hit compare (one for each LRAMs block)
dcache_req                                   s1_req;               // Request in the S1 stage
dcache_req                                   s1_req_pre_addr;      // Request in the S1 stage without the address
logic [`VA_RANGE]                            s1_req_addr;          // Address part of the S1 stage request. The address is apart from the request to save power in cache_op_l2 prefetches that only need to load the address and not the rest of the request.
logic                                        s1_req_thread_id;
dcache_da_read_req                           s1_da_read_req;       // Data array read request for S1 stage from S0
logic                                        s1_md_read_en;        // Meta data array read enable
logic                                        s1_da_read_en;        // Read enable for the data array
logic                                        s1_wb_da_valid;       // Write back request validation
dcache_da_read_req                           s1_wb_da_req;         // Write back request to data array
debug_bp_t                                   s1_bp_conf_cmd;
logic                                        s1_req_force_set;     // Flag indicating if set is being forced by current operation or not
`ifdef DCACHE_REPORT_PC
  logic [`PC_RANGE_EXT]                        s1_req_pc;            // PC of the instruction that did the request in S1
  logic                                        s1_req_pc_load;       // Enables loading PC of the instruction to S2
`endif

//         CLK    RST    EN                               DOUT                  DIN                DEF
`RST_FF   (clock, reset,                                  s1_valid,             s0_valid_to_s1,    1'b0)
`EN_FF    (clock,        s0_valid_to_s1 || s0_md_read_en, s1_req_pre_addr,      s0_req)
`EN_FF    (clock,        s0_valid_to_s1 || s0_md_read_en, s1_req_force_set,     s0_req_force_set)
`EN_FF    (clock,        s0_req_addr_load,                s1_req_thread_id,     s0_req.dest.thread_id)
`EN_FF    (clock,        s0_req_addr_load,                s1_req_addr,          s0_req_addr)
`FF       (clock,                                         s1_md_read_en,        s0_md_read_en)
`FF       (clock,                                         s1_da_read_en,        s0_da_read_en)
`EN_FF    (clock,        s0_md_read_en || s0_da_read_en,  s1_da_read_req,       s0_da_read_req)
`FF       (clock,                                         s1_wb_da_valid,       s0_wb_da_valid)
`EN_FF    (clock,        s0_wb_da_valid,                  s1_wb_da_req,         s0_wb_da_req)
`RST_FF   (clock, reset,                                  s1_bp_conf_valid,     s0_bp_conf_valid,  1'b0)
`FF       (clock,                                         s1_bp_conf_cmd,       s0_bp_conf_cmd)

`ifdef DCACHE_REPORT_PC
`EN_FF    (clock,        s0_req_pc_load,                  s1_req_pc,            s0_req_pc)
`FF       (clock,                                         s1_req_pc_load,       s0_req_pc_load)
`endif

always_comb begin
   // Composes the S1 req
    s1_req                = s1_req_pre_addr;
    s1_req.addr           = s1_req_addr;      // The address comes via different path since it may be set by TL
    s1_req.dest.thread_id = s1_req_thread_id; // The thread_id is captured independently because it is needed for all breakpoints

    // and the breakpoint configuration
  s1_bp_conf.address[`VA_RANGE] = s1_req_addr;
  s1_bp_conf.address[`VA_EXT_MSB] = s1_req.addr_msb_err ^ s1_req_addr[`VA_MSB];
  s1_bp_conf.thread_id = s1_req_thread_id;
  s1_bp_conf.cmd = s1_bp_conf_cmd;
end

////////////////////////////////////////////////////////////////////////////////
// MetaData (tag + state) array
////////////////////////////////////////////////////////////////////////////////

dcache_meta_data [`DCACHE_WAYS-1:0] s1_mr_read_resp;         // Response to meta read requests
dcache_meta_write_req               s1_md_write_data;        // Meta write winner request
dcache_meta_write_req               s1_mh_mw_req;            // Miss handler meta write request
logic [`DCACHE_WAYS-1:0]            s1_mr_read_valid;        // Valid for meta read requests
logic                               s1_md_write_en;          // Meta data array write enable
logic                               s1_mh_mw_grant;          // Meta data array write grant to miss handler
logic                               s1_mh_mw_valid;          // Miss handler wants to write meta tags
logic                               s1_mh_mw_valid_early;    // Miss handler wants to write meta tags next cycle
logic                               s1_mh_mw_clear_slock;    // Miss handler to clear soft lock bit during metadata write
logic                               s1_md_write_early_en;    // Early write enable to enable latch clock in metadata array

// Several units need to write the metadata contents:
//  - Replays from Miss handler file (write new status)
//  - Cacheops and snoops from CacheOp unit (clean status)
//  - Debug interface
// This arbiter grants the access to all the units. To improve timing, the
// priority is static (CacheOp having higher prio).

always_comb begin
  // Enable and grant
  s1_md_write_early_en = s1_co_mw_valid_early || s1_mh_mw_valid_early || dbg_md_write_en_early;
  s1_md_write_en       = s1_co_mw_valid       || s1_mh_mw_valid       || dbg_md_write_en;
  s1_co_mw_grant       = 1'b1;
  s1_mh_mw_grant       = !s1_co_mw_valid;
  dbg_md_write_valid   = !s1_co_mw_valid && !s1_mh_mw_valid;

  // Build debug request
  dbg_md_write_req.set    = dbg_md_write_addr[`DCACHE_SET_RANGE];
  dbg_md_write_req.way_en = dcache_way_decode(dbg_md_write_addr[`DCACHE_SET_SZ+:`DCACHE_WAY_SZ]);
  dbg_md_write_req.data   = dbg_md_write_data;

  // Winner select
  s1_md_write_data = s1_co_mw_valid ? s1_co_mw_req        // CacheOp unit
                   : s1_mh_mw_valid ? s1_mh_mw_req        // Miss handler
                   :                  dbg_md_write_req;   // Debug
end

dcache_metadata_array metadata_array (
  // System signals
  .clock              ( clock                   ),
  .reset              ( reset                   ),
  // Read request
  .s1_read_set        ( s1_req.set              ),
  // Write request
  .s1_write_valid     ( s1_md_write_en          ),
  .s1_write_valid_prev( s1_md_write_early_en    ),
  .s1_write_req       ( s1_md_write_data        ),
  // Read output data
  .s1_read_resp_valid ( s1_mr_read_valid        ),
  .s1_read_resp       ( s1_mr_read_resp         ),
  // Clear the information
  .cfg_clear_all      ( dcache_cfg_clear_all    ),
  .cfg_clear_low      ( dcache_cfg_clear_low    )
);

////////////////////////////////////////////////////////////////////////////////
// This module is the TLB array. It is accessed in this stage as now the VA is
// coming from a flop.
////////////////////////////////////////////////////////////////////////////////

dcache_tlb_req          s1_tlb_req_data;  // TLB request data
logic [`VA_TRANS_RANGE] s1_tlb_req_va;    // Translation VA addr request to TLB
logic                   s1_is_write;      // Transaction is a write
logic                   s1_is_store;      // Transaction is a write or TensorStore or AMO
logic                   s1_mf_xcpt;       // Misaligned fault
logic                   s1_readwrite;     // Transaction is reading or writing data
logic                   s1_valid_masked;  // Valid masked due nack or exceptions
logic                   s1_valid_to_tlb;  // Valid request to TLB
logic                   s1_tlb_req_valid; // Validates the request to TLB

logic                   vm_enabled;       // Virtual memory is enabled

logic                   s1_translate_only_addr_load;  // Address load for any of the blocks willing to only translate address

// Select the VM status for the current request
assign s1_vm_status = (s1_req.vm_sel == `DCACHE_VM_SEL_CO)  ? vm_status_co
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TL0) ? vm_status_tl0
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TL1) ? vm_status_tl1
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TS)  ? vm_status_ts
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_L2)  ? vm_status_l2
                    : (s1_req.dest.thread_id == 1'b1     )  ? vm_status[1]
                    :                                         vm_status[0];

assign s1_tlb_req_va = s1_req.addr[`VA_TRANS_RANGE];
assign s1_tlb_req_data = '{ status      : s1_vm_status,
                            vpn         : s1_tlb_req_va,
                            passthrough : s1_req.phys,
                            store       : s1_is_store,
                            msb_err     : s1_req.addr_msb_err };

assign s1_translate_only_addr_load = s1_tl_addr_load || s1_ts_addr_load || s1_co_l2_addr_load;
assign s1_tlb_req_valid = (s1_valid_to_tlb && s1_readwrite) || s1_translate_only_addr_load;

dcache_tlb_array tlb_array (
  // System signals
  .clock                ( clock             ),
  .reset                ( reset             ),
  // ESRs
  .esr_vmspagesize      ( vmspagesize       ),
  // Request to the TLB
  .req_data             ( s1_tlb_req_data   ),
  .req_valid            ( s1_tlb_req_valid  ),
  // Response with the physical bits
  .resp_data            ( s1_tlb_resp_data  ),
  // TLB/PTW control
  .satp_info            ( satp_info         ),
  .satp_info_en         ( satp_info_en      ),
  .matp_info            ( matp_info         ),
  .matp_info_en         ( matp_info_en      ),
  .tlb_invalidate       ( tlb_invalidate    ),
  // PTW request
  .ptw_req_data         ( ptw_req_data      ),
  .ptw_req_valid        ( ptw_req_valid     ),
  .ptw_req_ready        ( ptw_req_ready     ),
  // PTW response
  .ptw_resp_data        ( ptw_resp_data     ),
  .ptw_resp_valid       ( ptw_resp_valid    ),
  // VM enabled
  .vm_enabled           ( vm_enabled        )
);

////////////////////////////////////////////////////////////////////////////////
// Retrieve indices for gather/scatter on 32-byte block and compute the mux
// selectors for S2 and S3 stages.
////////////////////////////////////////////////////////////////////////////////

logic [`VPU_LANE_RANGE][`CORE_GSC32_IDX_RANGE] s1_to_s2_gsc32_indices;
logic [4:0]                                    s1_block_offset;
logic [`DCACHE_LRAM_BANK_RANGE]                s1_gather32_en;

always_comb begin
  case (s1_req.typ)
    dcache_type_PS_GS8 : s1_block_offset =  s1_req.addr[4:0];
    dcache_type_PS_GS16: s1_block_offset = {s1_req.addr[4:1], 1'b0};
    dcache_type_PS_GS32: s1_block_offset = {s1_req.addr[4:2], 2'b0};
    default            : s1_block_offset = 'b0;
  endcase

  s1_gather32_en = 'h0;
  s1_to_s2_gsc32_indices = 'h0;

  for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
    if (s1_req.ps_mask[i]) begin
      case (s1_req.typ)
        dcache_type_PS_GS8 : s1_to_s2_gsc32_indices[i] =  s1_req.gsc32_idx[i*5 +: 5];
        dcache_type_PS_GS16: s1_to_s2_gsc32_indices[i] = {s1_req.gsc32_idx[i*4 +: 4], 1'b0};
        dcache_type_PS_GS32: s1_to_s2_gsc32_indices[i] = {s1_req.gsc32_idx[i*3 +: 3], 2'b0};
        default            : s1_to_s2_gsc32_indices[i] = 'b0;
      endcase

      s1_to_s2_gsc32_indices[i] = s1_to_s2_gsc32_indices[i] + s1_block_offset;
      s1_gather32_en[s1_to_s2_gsc32_indices[i][4:3]] = s1_valid && dcache_cmd_is_gsc32(s1_req.cmd);
    end
  end
end

////////////////////////////////////////////////////////////////////////////////
// Gets the tag match per way comparing the PA vs the tags read in previous
// stage and flopped.
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_WAYS-1:0]                   s1_tag_match;            // Which way had the hit
logic [`DCACHE_WAY_RANGE]                  s1_read_way;             // Encoded way for the read
logic                                      s1_misaligned_case_3;    // Case 3 of misalignment
logic                                      s1_misaligned_case_4;    // Case 4 of misalignment
logic                                      s1_req_size_128b;        // Request data size
logic                                      s1_req_size_256b;        // Request data size

logic [1:0]                                lram_bank_idx_curr;      // Aux signal to point to current and previous banks
logic [1:0]                                lram_bank_idx_prev;

// Gets the address using the TLB translation
always_comb begin
  s1_phys_addr = { s1_tlb_resp_data.ppn, s1_req.addr[`VA_UNTRANS_SIZE-1:6], s1_req.addr[5:0] };
  s1_phys_addr_tag = s1_phys_addr[`PA_TAG_EXT_RANGE];
  s1_phys_addr_set = dcache_set_correct(s1_phys_addr[`DCACHE_SET_ADDR_RANGE],dcache_cfg_mode,s1_req.dest.thread_id);

  // CAM for the tags
  for(integer i = 0; i < `DCACHE_WAYS; i++) begin
    // Tags are equal and valid
    s1_tag_match[i] = (s1_phys_addr_tag == s1_mr_read_resp[i].tag) && s1_mr_read_valid[i];
  end

  // Generates the encoded way for read
  s1_read_way = dcache_way_encode(s1_tag_match);

  // Compute the access size.
  s1_req_size_128b =  s1_misaligned_case_4 || s1_req.misaligned_2nd;
  s1_req_size_256b = (s1_req.typ == dcache_type_PS);

  // Computes the read address for the different LRAM blocks
  for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
    s1_da_read_req_hit[i] = s1_da_read_req;

    // Set valid bit for the LRAM bank (high and low part of the word simultaneously)
    lram_bank_idx_curr = i[1:0];
    lram_bank_idx_prev = i[1:0]-2'b1;
    if (dcache_cmd_is_gsc32(s1_req.cmd)) begin
      s1_da_read_req_hit[i].valid_l = s1_da_read_en && s1_gather32_en[i];
    end else begin
      s1_da_read_req_hit[i].valid_l = s1_da_read_en && (    (s1_da_read_req_hit[i].addr[4:3] == lram_bank_idx_curr)
                                                        || ((s1_da_read_req_hit[i].addr[4:3] == lram_bank_idx_prev) && s1_req_size_128b)
                                                        || s1_req_size_256b);
    end
    s1_da_read_req_hit[i].valid_h = s1_da_read_req_hit[i].valid_l;

    // In case of misalign within same cacheline but in different blocks, read next block addr (only for port 0)
    if ((i[1:0] < s1_da_read_req.addr[4:3]) && s1_misaligned_case_3 && s1_valid) begin
      s1_da_read_req_hit[i].addr[`DCACHE_BLOCK_ADDR_RANGE] += 'b1;
      s1_da_read_req_hit[i].valid_l = 1'b1;
      s1_da_read_req_hit[i].valid_h = 1'b1;
    end

    // Gets the way we need to read from
    s1_da_read_req_hit[i].way = s1_read_way;
  end
end

////////////////////////////////////////////////////////////////////////////////
// Decodes some info and computes the nacks
////////////////////////////////////////////////////////////////////////////////

dcache_req s1_req_to_s2;              // Request going to the S2 stage
logic      s1_valid_to_s2;            // Propagation of S1 valid towards S2
logic      s1_is_read;                // Transaction is a read
logic      s1_is_read_no_amo;         // Transaction is a non atomic read
logic      s1_nack;                   // Transaction is nacked due some reason
logic      s1_nack_co_collide;        // Transaction is nacked due to collision with Cache Ops operation
logic      s1_nack_vpu_rf_write;      // Transaction is nacked due to collision with VPU RF being used
logic      s1_is_amo;                 // S1 operation is an AMO operation
logic      s1_is_gsc32;               // S1 operation is a gather/scatter on 32-byte blocks
logic      s1_pma_bus_err;            // A PMA access fault has been detected during a second pass misalignment access
logic      s1_addr_clk_en;            // Enables the address clock to the S2 addr register
logic      s2_nack;                   // Instruction is nacked
logic      s1_is_bypass;              // Instruction accesses directly to L2 or L2
logic      s1_is_to_null;             // Instruction destination is integer register X0
logic      s1_rq_push_conservative;   // Especulative/conservative push to replay queue one cycle befor actual push in S2
logic      s1_rq_renack_conservative; // Especulative/conservative push to rearm queue one cycle before actual push in S2

always_comb begin
  // There's a valid transaction if:
  // - There's a valid,
  // - It is not killed by the core (unless the request comes from other units)
  // - There are no exceptions
  // - There are no TLB misses
  s1_valid_masked = s1_valid
                 && (!s1_core_kill || s1_req.replay || (s1_req.cache_op))
                 && !(|s1_core_xcpt)
                 && !s1_pma_bus_err;

  // Valid request to TLB. Do not lower it in case of exception ... it's weird
  s1_valid_to_tlb = s1_valid
                 && (!s1_core_kill || s1_req.replay || (s1_req.cache_op));

  // Propagation of valid to S2 depends on a possible TLB miss
  s1_valid_to_s2 = s1_valid_masked & !s1_tlb_resp_data.miss & (~s1_req.cache_op | ~s1_tlb_fail);

  // Conservative push and rearm to replay/rearm queue to enable latching data ...
  s1_rq_push_conservative = s1_valid_to_s2 && !s1_req.replay;
  s1_rq_renack_conservative = s1_valid_to_s2 && s1_req.replay;

  // Decodes some read/write info
  s1_is_read          = dcache_cmd_is_read(s1_req.cmd);
  s1_is_write         = dcache_cmd_is_write(s1_req.cmd) && !s1_req.uc_load_2nd
                          && !s1_translate_only_addr_load  // Needed to clear the write bit to receive the data for AMOs (are implemented as UCs) and for TL/TS/L2 Prefetch loads
                          && !s1_reduce_op_inst;           // Clear write for reduce to prevent enabling the data aligned of UC stores
  s1_is_store         = (dcache_cmd_is_write(s1_req.cmd) && !s1_req.uc_load_2nd) || s1_ts_addr_load;
  s1_readwrite        = s1_is_read || s1_is_write || dcache_cmd_is_prefetch(s1_req.cmd);
  s1_is_amo           = dcache_cmd_is_amo(s1_req.cmd) && !s1_translate_only_addr_load;
  s1_is_read_no_amo   = dcache_cmd_is_read(s1_req.cmd) & ~s1_is_amo;
  s1_is_bypass        = dcache_cmd_is_bypass(s1_req.cmd) && !s1_translate_only_addr_load;
  s1_is_gsc32         = dcache_cmd_is_gsc32(s1_req.cmd) && !s1_translate_only_addr_load;
  s1_is_to_null       = s1_is_read && !s1_req.dest.fp && (s1_req.dest.addr == `XREG_ADDR_SIZE'b0);

  // Nack the S1 entry (propagated to S2) when...
  s1_nack_co_collide = (s1_req.set == s1_co_nack_set_value) && s1_co_nack_set_valid;
  s1_nack_vpu_rf_write = s1_is_read && s1_req.dest.fp && s1_vpu_ctrl.tfma_rf_write;
  s1_nack = (s1_valid_to_s2 && s1_req.buf_alloc && (s1_mh_force_ba_rptr || s1_re_ba_read_en_spec))       // Needed access to buffer array read port but a UC store / reduce is using it
         || (s1_valid_to_s2 && s1_vpu_ctrl.scp_req.read_en && !s1_is_amo && !s1_is_bypass)               // VPU is requesting a scratchpad access
         || (s1_valid_to_s2 && s1_nack_co_collide && !s1_is_amo && !s1_is_bypass)                        // There's a hit in the same set for an inflight transaction in the CacheOp unit
         || (s1_valid_to_s2 && s1_nack_vpu_rf_write);                                                    // VPU is doing a tensor FMA and using VPU RF wen[1](load) port

  // Clock enable to S2 stage
  s1_clk_en      = s1_md_read_en || s1_da_read_en || s1_reduce_send_inst || s1_reduce_op_inst || s1_wb_da_valid;
  s1_addr_clk_en = s1_clk_en || s1_translate_only_addr_load || s1_has_xcpt_no_kill; // Use here the s1_has_xcpt_no_kill to remove the dependency with s1_core_kill in the clock enables
end

////////////////////////////////////////////////////////////////////////////////
// When an instruction gets a TLB miss the pipeline needs to be flushed,
// including the core
////////////////////////////////////////////////////////////////////////////////
logic   s1_core_flush_pipeline;  // Indication to flush the pipeline to core in stage S1
logic   s0_clear_progress;
logic   s1_clear_progress;
logic   s2_clear_progress;
logic   s1_save_progress;
logic   s2_save_progress;


always_comb begin
  // Flushing the pipeline in case of core instructions getting a TLB miss
  s1_tlb_flush_pipeline = `CORE_NR_THREADS'b0;
  if (s1_valid_to_tlb && s1_tlb_resp_data.miss && (s1_req.vm_sel == `DCACHE_VM_SEL_IP))
    s1_tlb_flush_pipeline[s1_req.dest.thread_id] = 1'b1;


  // save progress when there's a TLB miss (first line)
  // or when there is an xcpt (second line)
  if (s1_valid_to_tlb && s1_tlb_resp_data.miss && dcache_type_is_gsc(s1_req.typ) && !dcache_cmd_is_gsc32(s1_req.cmd) && (s1_req.vm_sel == `DCACHE_VM_SEL_IP) ||
      s1_has_xcpt && dcache_type_is_gsc(s1_req.typ) && !dcache_cmd_is_gsc32(s1_req.cmd) && (s1_req.vm_sel == `DCACHE_VM_SEL_IP))
    s1_save_progress = 1'b1;
  else
    s1_save_progress = 1'b0;

  // clear progress if doing the last line (and completes without TLB miss or xcpt)
  if (s0_core_req_is_valid_gsc && (&s0_req.gsc_cnt))
    s0_clear_progress = 1'b1;
  else
    s0_clear_progress = 1'b0;


  // In case of a Gather/Scatter operation the progress needs to be saved or cleared when a new instruction gets into S0 from core
  core_ctrl_resp.gsc_progress_save = `CORE_NR_THREADS'b0;
  core_ctrl_resp.gsc_progress_save[s2_req.dest.thread_id] = s2_save_progress || s2_clear_progress;
  core_ctrl_resp.gsc_progress = s2_clear_progress && !s2_save_progress? '0 : s2_req.gsc_cnt;

end // always_comb

//      CLK    RST    DOUT               DIN                                                      DEF
`RST_FF(clock, reset, s1_clear_progress, s0_clear_progress && !s1_save_progress,                  1'b0)
`RST_FF(clock, reset, s2_clear_progress, s1_clear_progress && !s1_core_kill,                      1'b0)
`RST_FF(clock, reset, s2_save_progress,  s1_save_progress  && !s1_core_kill && !s2_save_progress, 1'b0)


assign s1_core_flush_pipeline = |s1_tlb_flush_pipeline;

// Update flush enable bit when S1 is active or the bit itself is set
logic s1_core_flush_en;
assign  s1_core_flush_en = s1_valid_to_tlb | s2_core_flush_pipeline;

//         CK     RST    EN                DOUT                    DIN                      DEF
`RST_EN_FF(clock, reset, s1_core_flush_en, s2_core_flush_pipeline, s1_core_flush_pipeline,  1'b0)



////////////////////////////////////////////////////////////////////////////////
// This module is the PMA checker. It is accessed in this stage as it needs the
// PA coming out of the TLB.
////////////////////////////////////////////////////////////////////////////////

dcache_pma_req  s1_pma_req_data;
dcache_pma_resp s1_pma_resp_data;
logic tlb_valid_addr;
logic af_skip;
logic s1_pma_ts_tl_co;
prv_t s1_pma_prv;
   

assign tlb_valid_addr = !s1_tlb_wait && !(s1_tlb_resp_data.xcpt_ld || s1_tlb_resp_data.xcpt_st);

// Flag indicating that TensorStore, TensorLoad or Cache Ops are the source of the address being checked
assign s1_pma_ts_tl_co = s1_translate_only_addr_load || ((s1_req.vm_sel == `DCACHE_VM_SEL_CO) && (s1_valid || s1_md_read_en));

assign s1_pma_req_data = '{ paddr      : s1_phys_addr,
                            typ        : s1_req.typ,
                            amo        : s1_is_amo,
                            write      : s1_is_write,
                            mem_global : s1_req.mem_global,
                            ts_tl      : s1_translate_only_addr_load,
                            ts_tl_co   : s1_pma_ts_tl_co  };

assign af_skip = (s1_req.replay & !s1_req.misaligned_2nd & s1_valid) | s1_co_mr_skip_pma;

assign s1_pma_prv   = (s1_req.vm_sel == `DCACHE_VM_SEL_CO)  ? vm_status_co.prv
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TL0) ? vm_status_tl0.prv
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TL1) ? vm_status_tl1.prv
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_TS)  ? vm_status_ts.prv
                    : (s1_req.vm_sel == `DCACHE_VM_SEL_L2)  ? vm_status_l2.prv
                    : (s1_req.replay                     )  ? s1_req.prv
                    : (s1_req.dest.thread_id == 1'b1     )  ? vm_status[1].prv
                    :                                         vm_status[0].prv;
   
   
dcache_pma_unit
pma_unit
(
    // Do not generate access fault
    .af_skip    ( af_skip             ),
    // Request to the PMA
    .req_data   ( s1_pma_req_data     ),
    .mprot      ( s1_mprot            ),
    .valid_addr ( tlb_valid_addr      ),
    // VM status
    .prv        ( s1_pma_prv          ),
    //.vm_status  ( s1_vm_status        ),
    // Response
    .resp_data  ( s1_pma_resp_data    )
);

////////////////////////////////////////////////////////////////////////////////
// Data unit that stores the contents of the Dcache. The read of the data is
// started in S1 stage and the result is provided in S2. We provide both the
// set and the way that is being accessed. Write is done in S4 stage and
// also provides the set and way.
////////////////////////////////////////////////////////////////////////////////

dcache_da_write_req [`DCACHE_LRAM_BANK_RANGE] s4_da_write_data;    // Winner of the data array write port in S4
dcache_da_read_req [`DCACHE_LRAM_BANK_RANGE]  s1_da_read_req_mux;  // Data array read request after muxing the hit compare and VPU direct LRAM port
logic                                         s1_da_read_en_final; // Read enable for the data array
logic                                         s1_da_read_en_ovr;   // Overridden read signal
logic                                         s4_da_write_en_but_dbg; // Write to the data array, all cases but debug
logic                                         s4_da_write_en;      // Write to the data array
logic                                         s4_da_write_en_ovr;  // Overridden write signal
logic [`DCACHE_LRAM_BANK_RANGE]               s1_da_load_data_en;  // Enables for data from data array
logic [`DCACHE_DATA_NUM_WORDS_RANGE]          s1_vpu_scp_read_en;  // Selects what data will be delivered to vpu scp port in stage 3
logic                                         s1_vpu_en;           // Update read selection to S2

// Override read and write signals for testing
assign s1_da_read_en_ovr  = s1_da_read_en_final & !mem_ctrl_override[`MINION_MEM_CTRL_RE];
assign s4_da_write_en_ovr = s4_da_write_en      & !mem_ctrl_override[`MINION_MEM_CTRL_WE];

dcache_data_array data_array (
  // System signals
  .clock          ( clock               ),
  // Read port request
  .s1_read_valid  ( s1_da_read_en_ovr   ),
  .s1_read_req    ( s1_da_read_req_mux  ),
  .s2_read_resp   ( s2_da_read_resp     ),
  // Write port request
  .s4_write_valid ( s4_da_write_en_ovr  ),
  .s4_write_req   ( s4_da_write_data    )
);


// Muxes read access between regular operation, WriteBack, VPU, TS or Debug
dcache_s1_da_mux_sel_t   s1_da_read_mux_sel;
dcache_s1_da_mux_sel_t   s1_da_read_mux_sel_prev;

//     CK     EN                   DOUT                     DIN
`EN_FF(clock, s1_da_read_en_final, s1_da_read_mux_sel_prev, s1_da_read_mux_sel)

always_comb begin
  // Determine value of mux selection. Keep to previous value if no read access is performed in this cycle
  // Valid read signal logic operation
  // Notice the order in the if-else construction to be according to the Overriding sequence. Debug has lowest priority.
  // s1_da_read_en_final = s1_da_read_en || s1_wb_da_valid || s1_vpu_ctrl.scp_req.read_en || (ts_scp_read && s1_reduce_send_inst) || dbg_da_read_en;
  s1_da_read_en_final = 1'b1;
  if (s1_vpu_ctrl.scp_req.read_en)
    s1_da_read_mux_sel = dcache_s1_da_mux_sel_vpu;
  else if (ts_scp_read && s1_reduce_send_inst)
    s1_da_read_mux_sel = dcache_s1_da_mux_sel_ts;
  else if (s1_wb_da_valid)
    s1_da_read_mux_sel = dcache_s1_da_mux_sel_wb;
  else if (s1_da_read_en)
    s1_da_read_mux_sel = dcache_s1_da_mux_sel_core;
  else if (dbg_da_read_en)
    s1_da_read_mux_sel = dcache_s1_da_mux_sel_dbg;
  else begin
    s1_da_read_mux_sel = s1_da_read_mux_sel_prev;
    s1_da_read_en_final = 1'b0;
  end

  // Request from core or replay instructions (default)
  s1_da_read_req_mux = s1_da_read_req_hit;

  // Nack access to WB if VPU just makes a request
  s1_wb_da_nack = s1_wb_da_valid & s1_vpu_ctrl.scp_req.read_en;

  // Overriding with writeback requests to read
  if (s1_da_read_mux_sel == dcache_s1_da_mux_sel_wb) begin
    for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      s1_da_read_req_mux[i] = s1_wb_da_req;
    end
  end

  // Overriding with TensorStores from scratchpad
  if (s1_da_read_mux_sel == dcache_s1_da_mux_sel_ts) begin
    for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      // Read full LRAM line, 256b as if it was a full store from VPU
      s1_da_read_req_mux[i].valid_l =  1'b1;
      s1_da_read_req_mux[i].valid_h =  1'b1;

      s1_da_read_req_mux[i].way   = ts_scp_read_way;
      s1_da_read_req_mux[i].addr  = ts_scp_read_addr;
    end
  end

  // Overriding with scratchpad request
  if (s1_da_read_mux_sel == dcache_s1_da_mux_sel_vpu) begin
    for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      // Size 256b, real all banks, otherwise only if address bits match bank (with 32b granularity)
      s1_da_read_req_mux[i].valid_l =  s1_vpu_ctrl.scp_req.size256
                                   ||  s1_vpu_ctrl.scp_req.size64 && (s1_vpu_ctrl.scp_req.addr[4:3] == i[1:0])
                                   || (s1_vpu_ctrl.scp_req.addr[4:2] == {i[1:0],1'b0});
      s1_da_read_req_mux[i].valid_h =  s1_vpu_ctrl.scp_req.size256
                                   ||  s1_vpu_ctrl.scp_req.size64 && (s1_vpu_ctrl.scp_req.addr[4:3] == i[1:0])
                                   || (s1_vpu_ctrl.scp_req.addr[4:2] == {i[1:0],1'b1});

      s1_da_read_req_mux[i].way   = s1_vpu_ctrl.scp_req.way;
      s1_da_read_req_mux[i].addr  = s1_vpu_ctrl.scp_req.addr;
    end
  end

  // If functional modules do not access the LRAM, debug module can do it
  // The debug address points to a 64-bit word.
  // The LRAM address to access to a 32-byte row is built this way: {addr[9:5],way[1:0]}
  //   - Then, LRAM address must have at least 10 bits (1k byte)
  //   - There are 4 ways --> 4k byte in total
  //   - For debug, use MSB as the way and LSB as the remaining row address
  dbg_da_read_accept_next = 1'b0;
  if (s1_da_read_mux_sel == dcache_s1_da_mux_sel_dbg) begin
    dbg_da_read_accept_next = 1'b1;
    for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      s1_da_read_req_mux[i].valid_l = 1'b1;
      s1_da_read_req_mux[i].valid_h = 1'b1;
      s1_da_read_req_mux[i].addr    = {dbg_da_read_addr[`DCACHE_DBG_DA_ADDR_BITS-3:0],3'b0};
      s1_da_read_req_mux[i].way     = dbg_da_read_addr[`DCACHE_DBG_DA_ADDR_BITS-1:`DCACHE_DBG_DA_ADDR_BITS-2];
    end
  end

  // Determine the banks that need to be enabled for data loads
  for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++)
    s1_da_load_data_en[i] = s1_da_read_req_mux[i].valid_l || s1_da_read_req_mux[i].valid_h; // OPTIMIZE, check if it can be split

  // Keeps information regarding which Word in the 128b chunk has been read
  // For 128b accesses, all of them, for 32b accesses from vpu only the addressed chunk
  for(integer i = 0; i < `DCACHE_DATA_NUM_WORDS; i++)
    s1_vpu_scp_read_en[i] = s1_vpu_ctrl.scp_req.read_en &&
                            (   s1_vpu_ctrl.scp_req.size256                                         // 256b access
                             || s1_vpu_ctrl.scp_req.size64 && (s1_vpu_ctrl.scp_req.addr[3] == i[1]) // 64b access
                             || (s1_vpu_ctrl.scp_req.addr[4:2] == i[2:0])                           // 32b access
                            );
  s1_vpu_en = s1_vpu_ctrl.scp_req.read_en;
end

////////////////////////////////////////////////////////////////////////////////
// Misaligns.
// In S1 is where we detect the different type of misaligns and we take the
// required actions. There are the following cases:
//  1) Misalign, data in different cachelines.
//  2) Misalign, more than one cache block read needed: data is spread over
//     different cache blocks, but in same cacheline. It is a PS access and
//     data is spread over 3 blocks of 64b
//  3) Misalign, data in different cache block: data is spread over different
//     cache blocks, but in same cacheline. Also, no more than a cache block
//     of data is needed to be read to read all the data.
//  4) Misalign, data in cache block: the element is not element aligned in
//     memory, but all the data is within the cache block (the read width of the
//     data array). Simply need to make sure we read all the cache block and
//     properly align
//  5) No misalign: nothing needs to be done, regular read of cache and
//     alignment will deal with it
////////////////////////////////////////////////////////////////////////////////

`define ACCESS_SIZE_BITS 7

logic [6:0] s1_access_size;       // Size of the access
logic [6:0] s1_access_last_byte;  // Position of the last byte accessed
logic       s1_misaligned_block;  // The access is misaligned (not same block)
logic       s1_misaligned;        // The access is misaligned (requires replay)
logic       s1_misaligned_case_1; // Case 1 of misalignment
logic       s1_misaligned_case_2; // Case 2 of misalignment
logic       s1_misaligned_case_5; // Case 5 of misalignment

always_comb begin
  // Computes the size of the misalignment minus 1 to compute last byte
  // accessed
  if     (s1_req.typ == dcache_type_B      ) s1_access_size = `ACCESS_SIZE_BITS'd0;
  else if(s1_req.typ == dcache_type_H      ) s1_access_size = `ACCESS_SIZE_BITS'b1;
  else if(s1_req.typ == dcache_type_W      ) s1_access_size = `ACCESS_SIZE_BITS'd3;
  else if(s1_req.typ == dcache_type_D      ) s1_access_size = `ACCESS_SIZE_BITS'd7;
  else if(s1_req.typ == dcache_type_BU     ) s1_access_size = `ACCESS_SIZE_BITS'd0;
  else if(s1_req.typ == dcache_type_HU     ) s1_access_size = `ACCESS_SIZE_BITS'b1;
  else if(s1_req.typ == dcache_type_WU     ) s1_access_size = `ACCESS_SIZE_BITS'd3;
  else if(s1_req.typ == dcache_type_PS_BR  ) s1_access_size = `ACCESS_SIZE_BITS'd3;
  else if(s1_req.typ == dcache_type_PS_GS8 ) s1_access_size = `ACCESS_SIZE_BITS'd0;
  else if(s1_req.typ == dcache_type_PS_GS16) s1_access_size = `ACCESS_SIZE_BITS'd1;
  // Taking masking into account
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[7]) s1_access_size = `ACCESS_SIZE_BITS'd31;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[6]) s1_access_size = `ACCESS_SIZE_BITS'd27;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[5]) s1_access_size = `ACCESS_SIZE_BITS'd23;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[4]) s1_access_size = `ACCESS_SIZE_BITS'd19;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[3]) s1_access_size = `ACCESS_SIZE_BITS'd15;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[2]) s1_access_size = `ACCESS_SIZE_BITS'd11;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[1]) s1_access_size = `ACCESS_SIZE_BITS'd7;
  else if((s1_req.typ == dcache_type_PS) && s1_req.ps_mask[0]) s1_access_size = `ACCESS_SIZE_BITS'd3;
  else                                       s1_access_size = `ACCESS_SIZE_BITS'd3; // dcache_type_PS_GS32

  // Computes the last accessed byte
  s1_access_last_byte = { 1'b0, s1_req.addr[5:0] } + s1_access_size;

  s1_misaligned_case_1 = 1'b0;
  s1_misaligned_case_2 = 1'b0;
  s1_misaligned_case_3 = 1'b0;
  s1_misaligned_case_4 = 1'b0;
  s1_misaligned_case_5 = 1'b0;
  // If doing second pass, there's no misalignment. Msg requests can't misalign
  if(!s1_req.misaligned_2nd && !s1_is_gsc32) begin
    // Checks for case 1 (in two cachelines, last byte goes beyond range 0:63, bit 6 to '1')
    if(s1_access_last_byte[6])
      s1_misaligned_case_1 = 1'b1;
    // Checks for case 2 (same cacheline, different LRAM lines, PS, no LRAM width aligned
    else if((s1_req.addr[`DCACHE_BLOCK_ADDR_RANGE] != s1_access_last_byte[`DCACHE_BLOCK_ADDR_RANGE]) // Different LRAM lines
         && (s1_req.typ == dcache_type_PS)                                                           // PS
         && (|s1_req.addr[2:0]))                                                                     // Not LRAM width (64b) aligned
      s1_misaligned_case_2 = 1'b1;
    // Checks for case 3 (different LRAM lines)
    else if(s1_req.addr[`DCACHE_BLOCK_ADDR_RANGE] != s1_access_last_byte[`DCACHE_BLOCK_ADDR_RANGE])
      s1_misaligned_case_3 = 1'b1;
    // Checks for case 4 (misaligned within same cache block, but requiring 2 LRAM columns, last byte goes beyond range 0:7)
    else if(((s1_access_size[3:0] + { 1'b0, s1_req.addr[2:0] } ) > 4'd7) && (s1_req.typ != dcache_type_PS))
      s1_misaligned_case_4 = 1'b1;
    // Checks for case 5 (not element aligned)
    else if( ((s1_access_size == `ACCESS_SIZE_BITS'd1)  &&   s1_req.addr[0])
          || ((s1_access_size == `ACCESS_SIZE_BITS'd3)  && (|s1_req.addr[1:0]))
          || ((s1_access_size == `ACCESS_SIZE_BITS'd7)  && (|s1_req.addr[2:0]))
          || ((s1_access_size == `ACCESS_SIZE_BITS'd15) && (|s1_req.addr[3:0]))
          || ((s1_access_size == `ACCESS_SIZE_BITS'd31) && (|s1_req.addr[4:0])))
      s1_misaligned_case_5 = 1'b1;
  end

  s1_misaligned_block = s1_misaligned_case_1 || s1_misaligned_case_2 || s1_misaligned_case_3;
  s1_misaligned       = s1_misaligned_case_1 || s1_misaligned_case_2;
end

////////////////////////////////////////////////////////////////////////////////
// Writes the exceptions
////////////////////////////////////////////////////////////////////////////////

logic s1_access_is_page_split;
logic s1_pma_access_fault;

always_comb begin
  s1_pma_access_fault = s1_pma_resp_data.access_fault & !s1_req.misaligned_2nd & s1_valid_to_tlb;
  s1_pma_bus_err      = s1_pma_resp_data.access_fault &  s1_req.misaligned_2nd & s1_valid_to_tlb;

  s1_core_xcpt.pf_ld = (s1_is_read_no_amo || dcache_cmd_is_prefetch(s1_req.cmd)) && s1_tlb_resp_data.xcpt_ld && !s1_req.cache_op;
  s1_core_xcpt.pf_st = s1_is_store                                               && s1_tlb_resp_data.xcpt_st && !s1_req.cache_op;
  s1_core_xcpt.af_ld = (s1_is_read_no_amo || dcache_cmd_is_prefetch(s1_req.cmd)) && (s1_pma_access_fault || s1_tlb_resp_data.access_fault) && !s1_req.cache_op;
  s1_core_xcpt.af_st = s1_is_store                                               && (s1_pma_access_fault || s1_tlb_resp_data.access_fault) && !s1_req.cache_op;

  // L1 bypass or Atomic access that is not properly aligned. It was initially generating a "misaligned fault", but the spec has been
  // changed to generate an "access fault". Keeping the logic by separate to keep record of the changes
  s1_mf_xcpt =  !s1_req_to_s2.cacheable && !dcache_address_is_size_aligned(s1_req.typ,s1_req.addr[4:0]);
  s1_core_xcpt.mf_ld = 1'b0; // Was: s1_is_read_no_amo  && s1_mf_xcpt;
  s1_core_xcpt.mf_st = 1'b0; // Was: s1_is_write        && s1_mf_xcpt;
  s1_core_xcpt.af_ld |= s1_is_read_no_amo  && s1_mf_xcpt;
  s1_core_xcpt.af_st |= s1_is_write        && s1_mf_xcpt;

  // Page-split exceptions
  s1_access_is_page_split = dcache_access_is_page_split(s1_req.addr,vmspagesize,s1_misaligned_case_1);
  s1_core_xcpt.ps_ld = s1_is_read_no_amo  && s1_access_is_page_split && vm_enabled;
  s1_core_xcpt.ps_st = s1_is_write && s1_access_is_page_split && vm_enabled && !s1_mf_xcpt;

  // Exception

  // No exception if TLB has not a valid address
  s1_core_xcpt &= {$bits(dcache_tag_xcpt){!s1_tlb_wait}};

  // A version of the the s1_has_xcpt without using the s1_core_kill signal from intpipe to improve timing
  s1_core_xcpt_no_kill  = s1_core_xcpt;
  s1_core_xcpt_no_kill &= {$bits(dcache_tag_xcpt){s1_valid}};
  s1_has_xcpt_no_kill  = |s1_core_xcpt_no_kill;

  // No exception if s1 is no longer valid because it has been killed
  s1_core_xcpt &= {$bits(dcache_tag_xcpt){s1_valid && !s1_core_kill}};

  // Any exception has occurred
  s1_has_xcpt = |s1_core_xcpt;


  // Let know the core that the write port for integer is going to be used next cycle
  s1_core_replay_next = ((s1_valid && s1_req.replay) && s1_is_read && !s1_req.dest.fp && !s1_is_to_null);
end

// Report bus errors
logic [`PA_RANGE]  bus_err_addr_next;
logic [`CORE_NR_THREADS-1:0]             bus_err_next;
logic [`CORE_NR_THREADS-1:0] s1_pma_bus_err_per_thread;

assign s1_pma_bus_err_per_thread = {`CORE_NR_THREADS{s1_pma_bus_err}} & {s1_req.dest.thread_id, ~s1_req.dest.thread_id};

assign bus_err_next      = bus_err_co_l2 | bus_err_mh | bus_err_re | bus_err_tl0 | bus_err_tl1 | s1_pma_bus_err_per_thread;
assign bus_err_addr_next = |bus_err_mh    ? bus_err_addr_mh        :
                           s1_pma_bus_err ? s1_req.addr[`PA_RANGE] :
                           {$bits(bus_err_addr_next){1'b0}};

`ifdef DCACHE_REPORT_PC
  logic [`PC_RANGE_EXT] bus_err_pc_next;

  assign bus_err_pc_next = |bus_err_mh ? bus_err_pc_mh : {$bits(bus_err_pc_next){1'b0}};
`endif


for ( genvar thread = 0; thread < `CORE_NR_THREADS; thread++) begin: BUS_ERR_ADDR
  `EN_FF(clock, bus_err_next[thread], bus_err_addr[thread], bus_err_addr_next)

  `ifdef DCACHE_REPORT_PC
    `EN_FF(clock, bus_err_next[thread], bus_err_pc[thread],   bus_err_pc_next)
  `endif
end

`RST_FF(clock, reset, bus_err, bus_err_next, 2'b00)


////////////////////////////////////////////////////////////////////////////////
// Muxes the input to the s2_req structure
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_BUFFER_ID_RANGE] s1_ba_rptr;        // Read pointer for buffer array
logic [`DCACHE_BUFFER_ID_RANGE] s1_mh_ba_rptr;     // UC store buffer array pointer
logic [`VA_RANGE]               s1_req_addr_to_s2; // Address sent to S2 (selects betwen TL and regular pipe)
logic                           s1_ba_dealloc_uc;  // Dealloc a buffer array entry as it is no longer needed due UC store
logic                           s1_mh_ba_is_amo;   // Force the buffer array read pointer due an AMO

always_comb begin
  // Chooses read pointer for data buffer between S1 req and UC store
  // OPTIMIZE: we might want to clock gate this signal to prevent useless toggling
  s1_ba_rptr = s1_req.buf_id;
  // Using non qualified version of read pointer from MissHandler due timing
  if(s1_mh_force_ba_rptr)
    s1_ba_rptr = s1_mh_ba_rptr;
  else if(s1_re_ba_read_en_spec)
    s1_ba_rptr = s1_re_ba_read_addr;

  // Deallocate buffer array entry when:
  s1_ba_dealloc_uc = s1_mh_force_ba_rptr_qual && !s1_mh_ba_is_amo;            // Reading data of UC stores
  re_ba_dealloc = re_l2_evict_ready && re_l2_evict_valid && s1_re_ba_read_en  // Reading data of reduce send commands
               || ts_l2_evict_ready && ts_l2_evict_valid && s1_re_ba_read_en  // Reading data of tensor store commands
               || s1_reduce_op_inst && s1_re_ba_read_en;                      // Reading data of reduce op commands

  s1_req_to_s2 = s1_req;

  // The addr to S2 is not sent through the s1_req_to_s2, but s1_req_addr_to_s2
  s1_req_to_s2.addr = `VA_SIZE'b0;
  s1_req_to_s2.phys = s1_req.phys || !s1_tlb_resp_data.miss;
  s1_req_addr_to_s2 =   s1_has_xcpt ? s1_req.addr                 // If there is an exception, save the VA in the request
                      : s1_req.phys ? s1_req.addr                 // Address comes physical from core
                      :              `ZX(`VA_SIZE, s1_phys_addr); // Default is the translated address

  // Set is deduced from the address and masked by the current set configuration
  if (!s1_req_force_set)
    s1_req_to_s2.set = dcache_set_correct(s1_req_addr_to_s2[`DCACHE_SET_ADDR_RANGE],dcache_cfg_mode,s1_req.dest.thread_id);

  // Store data selection
  // Gets data from core when not doing replay queue
  // If doing a reduce send, always get store data
  // If doing a reduce op, always get the buffer array data
  if((!s1_req.replay || s1_reduce_send_inst) && !s1_reduce_op_inst)
    s1_req_to_s2.data = s1_core_store_data;
  else
    s1_req_to_s2.data = s1_ba_data;

  // This field is used to detect conflicts in the replayQ (block granularity). If 2nd pass for a misaligned ignore it
  s1_req_to_s2.misaligned = s1_misaligned_block && !s1_req.misaligned_2nd;

  // Computes which data blocks have been read
  for (integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
    s1_req_to_s2.chunk_read[i] = s1_da_read_req_hit[i].valid_l || s1_da_read_req_hit[i].valid_h;

  // Get cacheable from PMA
  s1_req_to_s2.cacheable = s1_pma_resp_data.cacheable && !s1_is_amo && !s1_is_bypass && !bypass_dcache;

  // Store privilage level so it's available in case of replay
  s1_req_to_s2.prv = s1_pma_prv;
end

////////////////////////////////////////////////////////////////////////////////
// This module stores the LRU for the ways within each set. It is accessed
// in S1 and is updated in S2. We bypass the data from S2 to S1 in case that
// the read and write are to the same set.
// The update is done once per request and during the first time the
// instruction goes through pipeline. So replays don't update the LRU. That's
// why s1_phys_addr is used to set the LRU array.
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_LRU_SET_RANGE] s1_lru_data;       // LRU data read from LRU array in S1 stage
logic [`DCACHE_LRU_SET_RANGE] s1_lru_data_byp;   // LRU data after muxing the S2 to S1 bypass
logic [`DCACHE_LRU_SET_RANGE] s2_lru_data_up;    // LRU data update from S2
logic [`DCACHE_WAYS-1:0]      s1_hlocked_ways;   // Ways hard locked in the set of the S1 operation
logic [`DCACHE_WAYS-1:0]      s1_slocked_ways;   // Ways soft locked in the set of the S1 operation
logic [`DCACHE_WAYS-1:0]      s1_replace_way_en; // Victim selected in S1
logic                         s2_valid_masked;   // S2 valid masked due nack (set for first instruction pass)
logic                         s2_lru_update;     // Updates the LRU of the set

dcache_lru_array lru_array(
  // System signals
  .clock      ( clock            ),
  .reset      ( reset            ),
  // Read port
  .read_set   ( s1_phys_addr_set ),
  .read_data  ( s1_lru_data      ),
  // Write port
  .write_en   ( s2_lru_update    ),
  .write_set  ( s2_req.set       ),
  .write_data ( s2_lru_data_up   )
);

always_comb begin
  // Bypasses the write data from S2 to S1
  if(s2_lru_update && (s2_req.set == s1_phys_addr_set))
      s1_lru_data_byp = s2_lru_data_up;
  else
      s1_lru_data_byp = s1_lru_data;
  // Selects the way to replace based on LRU (disable the scratchpad ways based on set)
  s1_hlocked_ways = hlock_state[s1_req_to_s2.set];
  s1_slocked_ways = slock_state[s1_req_to_s2.set];
  s1_replace_way_en = dcache_lru_victim(s1_lru_data_byp, s1_hlocked_ways, s1_slocked_ways); // Select the victim from LRU data (disabling the ways that are locked)
end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S2 stage
// In this stage of the pipeline we perform the data muxing and final hit
// detection of the dcache instruction. In case of miss, a miss handler is
// allocated and the instruction is parked there for the miss to be resolved.
// In case of hit, the data is returned to the core, and it is aligned and
// extended according to the request type.
// For stores, the atomic unit takes care of aligning the new data to write
// and merge it with the current data. So yes, we are doing a read + modify
// + write when dealing with stores.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_req                                     s2_req_pre_addr;      // S2 request before merging the real addr
logic [`VA_RANGE]                              s2_req_addr;          // Address in S2
logic [`DCACHE_LRU_SET_RANGE]                  s2_lru_data;          // LRU state for the transaction
logic [`DCACHE_WAYS-1:0]                       s2_replace_way_en;    // Victim selected in S2
logic                                          s2_reg_nack;          // S2 transaction is nacked because there was an issue in S1
logic                                          s2_nack_vpu_rf_write; // S2 transaction nacked before a conflict with VPU was detected in S1
logic                                          s2_misaligned;        // Instruction has a misalignment that requires a replay
logic                                          s2_misaligned_case_3; // Instruction has a misalignment case 3
logic                                          s2_is_read;           // S2 operation is a read
logic                                          s2_is_bypass;         // S2 operation is a bypass access
logic                                          s2_is_amo;            // S2 operation is an AMO access
logic                                          s2_valid_qual;        // Valid transaction in S2 stage after core kill and exception kill
logic [`DCACHE_LRAM_BANK_RANGE]                s2_da_load_data_en;   // Enable for data from data array
logic [`VPU_LANE_RANGE][`CORE_GSC32_IDX_RANGE] s2_gsc32_indices;     // Per lane indices for gather/scatter on 32-byte blocks
logic                                          s2_wb_da_valid;       // Write back request to data array
logic                                          s2_has_xcpt_addr;     // Address in S2 is the one that created an excetion
logic                                          s2_pma_bus_err;       // An asynchronous exception was detected for an instruction in the RQ   
`ifdef DCACHE_REPORT_PC
logic [`PC_RANGE_EXT]                          s2_req_pc;            // PC of the instruction that did the request in S1
`endif

//         CLK    RST    EN              DOUT                  DIN               DEF
`RST_FF   (clock, reset,                 s2_valid,             s1_valid_to_s2,   1'b0)
`RST_FF   (clock, reset,                 s2_pma_bus_err,       s1_pma_bus_err,   1'b0)   
`RST_EN_FF(clock, reset, s1_valid,       s2_reg_nack,          s1_nack,          1'b0)
`RST_EN_FF(clock, reset, s1_valid,       s2_has_xcpt_addr,     s1_has_xcpt,      1'b0)
`EN_FF    (clock,        s1_valid,       s2_nack_vpu_rf_write, s1_nack_vpu_rf_write) 
`EN_FF    (clock,        s1_clk_en,      s2_tag_match,         s1_tag_match)
`EN_FF    (clock,        s1_clk_en,      s2_req_pre_addr,      s1_req_to_s2)
`EN_FF    (clock,        s1_addr_clk_en, s2_req_addr,          s1_req_addr_to_s2)    // Separated as clock gate is different (used by TL)
`EN_FF    (clock,        s1_clk_en,      s2_lru_data,          s1_lru_data_byp)
`EN_FF    (clock,        s1_clk_en,      s2_misaligned,        s1_misaligned)
`EN_FF    (clock,        s1_clk_en,      s2_misaligned_case_3, s1_misaligned_case_3)
`EN_FF    (clock,        s1_clk_en,      s2_da_load_data_en,   s1_da_load_data_en)
`EN_FF    (clock,        s1_clk_en,      s2_replace_way_en,    s1_replace_way_en)
`EN_FF    (clock,        s1_clk_en,      s2_is_read,           s1_is_read)
`EN_FF    (clock,        s1_clk_en,      s2_is_write,          s1_is_write)
`EN_FF    (clock,        s1_clk_en,      s2_is_amo,            s1_is_amo)
`EN_FF    (clock,        s1_addr_clk_en, s2_is_bypass,         s1_is_bypass)
`EN_FF    (clock,        s1_clk_en,      s2_gsc32_indices,     s1_to_s2_gsc32_indices)
`EN_FF    (clock,        s1_vpu_en,      s2_vpu_scp_read_en,   s1_vpu_scp_read_en)
`FF       (clock,                        s2_wb_da_valid,       s1_wb_da_valid)

`ifdef DCACHE_REPORT_PC
`EN_FF    (clock,        s1_req_pc_load, s2_req_pc,            s1_req_pc)
`endif
// OPTIMIZE: Can we separate clock gate per way. Maybe not, if s2_meta_read is not updated it may contain incorrect data for MH replacement line

for (genvar mrw = 0; mrw < `DCACHE_WAYS; mrw++) begin: FLOP_META_READ
  dcache_meta_data s1_mr_read_resp_qual;
  
  // When not using normal set mode, the LSB of TAG must be equal to the MSB of set
  logic [`DCACHE_SET_MSB_SZ-1:0] set_msb;
  assign set_msb = dcache_cfg_sets_red ? {(`DCACHE_SET_MSB_SZ){1'b0}} : s1_req.set[`DCACHE_SET_MSB_RANGE];

  assign s1_mr_read_resp_qual.coh_state = s1_mr_read_valid[mrw] ? s1_mr_read_resp[mrw].coh_state : State_Invalid;
  assign s1_mr_read_resp_qual.tag       = s1_mr_read_valid[mrw] ? s1_mr_read_resp[mrw].tag       : {{`PA_TAG_REG_SIZE{1'b0}},set_msb};
  
  //         CLK    EN              DOUT                  DIN
  `EN_FF    (clock, s1_clk_en,      s2_meta_read[mrw],    s1_mr_read_resp_qual)
end


////////////////////////////////////////////////////////////////////////////////
// Tag check, way muxing and hit computation
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_WAYS-1:0] s2_tag_match_qual;      // Tag match result qualified with some flavors that clean the result
logic                    s2_tag_match_any;       // Any tag matched
logic                    s2_state_hit;           // MESI state of the cacheline that had the hit
logic                    s2_hit;                 // Instruction hit in the cache
logic                    s2_nack_pre_misaligned; // S2 instruction has any nack before misalignment nack

always_comb begin
  // Composes the S2 req
  s2_req      = s2_req_pre_addr;
  s2_req.addr = s2_req_addr;
  // Qualifies with core kill and no exception
  s2_valid_qual = s2_valid && (!s2_core_kill || s2_req.replay || s2_req.cache_op);
  // Valid and match
  s2_valid_masked = s2_valid_qual && !s2_nack_pre_misaligned;
  // Messages, atomics or bypass can't have a match
  s2_tag_match_qual = s2_tag_match & {`DCACHE_WAYS{!s2_is_amo}} & {`DCACHE_WAYS{!s2_is_bypass}} & {`DCACHE_WAYS{!bypass_dcache}};
  s2_tag_match_any = |s2_tag_match_qual;
end

always_comb begin
  // Gets the state for the way that hit
  s2_way_hit_state = State_Invalid;
  for(integer i = 0; i < `DCACHE_WAYS; i++)
  if(s2_tag_match_qual[i])
    s2_way_hit_state = s2_meta_read[i].coh_state;
  // Computes if command type (read, write, write intent) is ok with current
  // line MESI state
  s2_state_hit = (s2_is_read                             && (s2_way_hit_state >= State_Shared))     // Read command and state is at least shared
              || ((s2_req.cmd == dcache_cmd_PFR)         && (s2_way_hit_state >= State_Shared))     // Read prefetch and state is at least shared
              || (dcache_cmd_is_write_intent(s2_req.cmd) && (s2_way_hit_state >= State_Exclusive)); // Write intent command and state is at least exclusive
  s2_state_hit &= !s2_is_write || (s2_way_hit_state == State_Modified); // Write command requires state is modified
  // Gets the final hit
  s2_hit = s2_tag_match_any && s2_state_hit; // OPTIMIZE: likely we can only have s2_state_hit if s2_tag_match_any is also hit, so we can remove the latter
    
end

////////////////////////////////////////////////////////////////////////////////
// Checks different nack conditions
////////////////////////////////////////////////////////////////////////////////

logic s2_nack_inflight;            // Instruction is nacked due an issue with another inflight miss
logic s2_nack_miss;                // Instruction is nacked due an issue with a miss
logic s2_nack_rq_conflict;         // S2 instruction has a conflict with the replayQ
logic s2_nack_misaligned;          // S2 instruction needs to be replayed due a misalign
logic s2_mh_match_miss_line;       // Misshandler has an outstanding miss in same cache line
logic s2_nack_pre_misaligned_miss; // All nack conditions in S2 but misaligned and miss
logic s2_mh_req_ready;             // Miss handler is ready

always_comb begin
  // First condition is tricky, even if we hit we might need to nack the
  // transaction. The reason is that if there's a store that is upgrading
  // the MESI state from shared to modified, a younger load could hit the
  // cache (as state is shared) and load stale data. Also, if it was
  // a younger store, it could be executed in reverse order, ending up with
  // wrong data in the cache. This last case can happen between the tag
  // write update and the replay of the older store. That's why we need this
  // logic to lock the cacheline.
  s2_nack_inflight =  s2_hit && s2_mh_match_miss_line;                    // Can't access dcache if we are accessing same cacheline as an in-flight miss or evict
  s2_nack_miss     = !s2_hit && !s2_mh_req_ready && !s2_req.uc_load_2nd;  // If miss and can't allocate a miss handler

  // Gets all the nacks before misalignment and miss nack
  s2_nack_pre_misaligned_miss = s2_reg_nack || s2_nack_inflight || s2_nack_rq_conflict;
  // All nacks but misaligned
  s2_nack_pre_misaligned      = s2_nack_pre_misaligned_miss || s2_nack_miss;
end

always_comb begin
  // We have a misalignment nack only when there's no other nack and we need
  // to replay the instruction due a misalign (and we have a hit)
  s2_nack_misaligned = !s2_nack_pre_misaligned && s2_misaligned && !s2_req.misaligned_2nd && s2_hit;

  // Final nack
    s2_nack = s2_nack_pre_misaligned || s2_nack_misaligned;
end

////////////////////////////////////////////////////////////////////////////////
// Tensor Load module
// This module generates misses to the miss handler to download required data
////////////////////////////////////////////////////////////////////////////////

core_dcache_tensor_ctrl tensor_ctrl;           // Part of the core control specific for tensor load
logic                   s4_tenb_written;       // Pulse to indicate writing to TENB is complete

logic                   tenb_flush;            // Indication to flush the tenb state in vpu
logic                   tenb_tstore_busy;      // Tensor Store/Reduce busy for TenB control

assign s1_tlb_wait = s1_tlb_resp_data.miss || s1_tlb_resp_data.fill_pending;
assign s1_tlb_fail = s1_tlb_resp_data.xcpt_ld || s1_tlb_resp_data.xcpt_st || s1_tlb_resp_data.access_fault || s1_pma_resp_data.access_fault;

// Loads from tensor_load are equivalent to L2 fills, ensure there is no collision with L2 transfers
assign s4_tl0_load_ready = l2_fill_slot_avail & !l2_resp_valid_int;

// Local tensor load signals to mux access to pipeline
// When S0 is not used, TL0 can use it and TL1 can use it unless TL0 is using it
// S1 becomes unavailable whenever any regular instruction used S1 or when the other TL uses it
assign s0_available_to_tl0 = s0_available_to_tl;
assign s0_available_to_tl1 = s0_available_to_tl & !s0_tl0_addr_load;

assign s1_available_to_tl0 = s1_available_to_tl & !s1_tl1_addr_load;
assign s1_available_to_tl1 = s1_available_to_tl & !s1_tl0_addr_load;

assign s0_tl_addr_load     = s0_tl0_addr_load | s0_tl1_addr_load;
assign s1_tl_addr_load     = s1_tl0_addr_load | s1_tl1_addr_load;

assign s0_tl_addr          = s0_tl0_addr_load ? s0_tl0_addr         : s0_tl1_addr;
assign s0_tl_addr_msb_err  = s0_tl0_addr_load ? s0_tl0_addr_msb_err : s0_tl1_addr_msb_err;
assign s0_tl_thread_id     = s0_tl0_addr_load ? s0_tl0_thread_id    : s0_tl1_thread_id;
assign s0_tl_vm_sel        = s0_tl0_addr_load ? `DCACHE_VM_SEL_TL0  : `DCACHE_VM_SEL_TL1;

// Combine error flags
assign tensor_load_err_flags = tensor_load_err_flags_0[`DCACHE_TL_ERROR_BITS-3:0] | tensor_load_err_flags_1[`DCACHE_TL_ERROR_BITS-3:0];

// Combina busy/ready signals from Tensor Store/Reduce
assign tenb_tstore_busy = tensorstore_busy || !reduce_ready;

// Map signals from core_ctrl to tensor_ctrl
assign tensor_ctrl.ctrl            = core_ctrl.tensorload_ctrl;
assign tensor_ctrl.coop            = core_ctrl.tensorcoop_ctrl;
assign tensor_ctrl.stride          = core_ctrl.tensorload_stride;
assign tensor_ctrl.start           = core_ctrl.tensorload_start & !dcache_cfg_stall;
assign tensor_ctrl.restart_b       = core_ctrl.tensorload_restart_b;
assign tensor_ctrl.tmask_ready     = core_ctrl.tensorload_tmask_ready;
assign tensor_ctrl.tmask_bits      = core_ctrl.tensorload_tmask_bits;
assign tensor_ctrl.thread_id       = core_ctrl.thread_id;

dcache_tensor_load #(
  .MODULE_IDX  ( 0 )
) tensor_load_0 (
  // System signals
  .clock                    ( clock                            ),
  .reset                    ( reset                            ),
  .shire_min_id             ( shire_min_id                     ),
  // Chicken bit clock gate enable
  .chicken_bit_dcache       ( chicken_bit_dcache               ),
  // Request port for new tensor load
  .busy                     ( tl0_working                      ),
  .id_complete              ( tl0_id_complete                  ),
  .tensor_ctrl              ( tensor_ctrl                      ),
  // Signals to control s0 to s2 addr to do prefetches and override pipeline
  // value with TL value
  .s0_available             ( s0_available_to_tl0              ),
  .s0_addr_load             ( s0_tl0_addr_load                 ),
  .s0_addr                  ( s0_tl0_addr                      ),
  .s0_addr_msb_err          ( s0_tl0_addr_msb_err              ),
  .s0_thread_id             ( s0_tl0_thread_id                 ),
  .s1_available             ( s1_available_to_tl0              ),
  .s1_addr_load             ( s1_tl0_addr_load                 ),
  .s1_tlb_wait              ( s1_tlb_wait                      ),
  .s1_tlb_fail              ( s1_tlb_fail                      ),
  .s2_addr                  ( s2_req.addr[`PA_RANGE]           ),
  // Port to request cacheline directly to L2 (cooperative) and to l2_miss_arb (non cooperative)
  .l2_req_ready_coop        ( tl0_l2_miss_ready                ),
  .l2_req_valid_coop        ( tl0_l2_miss_valid                ),
  .l2_req_ready_no_coop     ( tl0_l2_miss_ready_no_coop        ),
  .l2_req_valid_no_coop     ( tl0_l2_miss_valid_no_coop        ),
  .l2_req                   ( tl0_l2_miss_req                  ),
  // Answer from L2
  .l2_resp_valid            ( l2_resp_valid_ready_int          ),
  .l2_resp                  ( l2_resp_int                      ),
  // Load control     
  .load_way                 ( s4_tl0_load_way                  ),
  .load_addr                ( s4_tl0_load_addr                 ),
  .load_data                ( s4_tl0_load_data                 ),
  .load_valid               ( s4_tl0_load_valid                ),
  .load_valid_pre           ( s3_tl0_load_valid_pre            ),
  .load_ready               ( s4_tl0_load_ready                ),
  .load_capture             ( s4_tl0_load_capture              ),
  // TENB logic
  .tfma_enabled             ( 1'b0                             ),
  .tenb_tstore_busy         ( 1'b0                             ),
  .tenb_credit              ( 1'b0                             ),
  .tenb_credit_entry        ( {$clog2(`VPU_TENB_SIZE/2){1'b0}} ),
  .tenb_written             ( 1'b0                             ),
  .tenb_ready               (                                  ),
  .tenb_flush               (                                  ),
  // Management of VM status
  .vm_status                ( vm_status                        ),
  .vm_status_tl             ( vm_status_tl0                    ),
  // Error output     
  .err_flags                ( tensor_load_err_flags_0          ),
  // For performance counters
  .tl_start                 ( tl0_start                        ),
  .tl_op                    ( tl0_op                           ),
  // Bus error
  .bus_err                  ( bus_err_tl0                      ),
  // For debug signals
  .csr_debug_signals        ( dbg_csr_tl0                      ),
  .sm_match_debug_signals   (  sm_match_debug_signals_tl0      ),
  .sm_filter_debug_signals  (  sm_filter_debug_signals_tl0     ),
  .sm_data_debug_signals    (  sm_data_debug_signals_tl0       )
);

dcache_tensor_load #(  
  .MODULE_IDX  ( 1 )
) tensor_load_1 (
  // System signals
  .clock                    ( clock                            ),
  .reset                    ( reset                            ),
  .shire_min_id             ( shire_min_id                     ),
  // Chicken bit clock gate enable
  .chicken_bit_dcache       ( chicken_bit_dcache               ),
  // Request port for new tensor load
  .busy                     ( tl1_working                      ),
  .tensor_ctrl              ( tensor_ctrl                      ),
  .id_complete              ( /*unused*/                       ),
  // Signals to control s0 to s2 addr to do prefetches and override pipeline
  // value with TL value
  .s0_available             ( s0_available_to_tl1              ),
  .s0_addr_load             ( s0_tl1_addr_load                 ),
  .s0_addr                  ( s0_tl1_addr                      ),
  .s0_addr_msb_err          ( s0_tl1_addr_msb_err              ),
  .s0_thread_id             ( s0_tl1_thread_id                 ),
  .s1_available             ( s1_available_to_tl1              ),
  .s1_addr_load             ( s1_tl1_addr_load                 ),
  .s1_tlb_wait              ( s1_tlb_wait                      ),
  .s1_tlb_fail              ( s1_tlb_fail                      ),
  .s2_addr                  ( s2_req.addr[`PA_RANGE]           ),
  // Port to request cacheline directly to L2 (cooperative) and to l2_miss_arb (non cooperative)
  .l2_req_ready_coop        ( tl1_l2_miss_ready                ),
  .l2_req_valid_coop        ( tl1_l2_miss_valid                ),
  .l2_req_ready_no_coop     ( tl1_l2_miss_ready_no_coop        ),
  .l2_req_valid_no_coop     ( tl1_l2_miss_valid_no_coop        ),
  .l2_req                   ( tl1_l2_miss_req                  ),
  // Answer from L2
  .l2_resp_valid            ( l2_resp_valid_ready_int          ),
  .l2_resp                  ( l2_resp_int                      ),
  // Load control           
  .load_way                 (                                  ),
  .load_addr                ( s4_tl1_load_addr                 ),
  .load_data                (                                  ),
  .load_valid               (                                  ),
  .load_valid_pre           (                                  ),
  .load_ready               ( 1'b0                             ),
  .load_capture             (                                  ),
  // TENB logic
  .tfma_enabled             ( s1_vpu_ctrl.tfma_enabled         ),
  .tenb_tstore_busy         ( tenb_tstore_busy                 ),
  .tenb_credit              ( s1_vpu_ctrl.tenb_credit          ),
  .tenb_credit_entry        ( s1_vpu_ctrl.tenb_credit_entry    ),
  .tenb_written             ( s4_tenb_written                  ),
  .tenb_ready               ( s4_tl1_tenb_ready                ),
  .tenb_flush               ( tenb_flush                       ),
  // Management of VM status
  .vm_status                ( vm_status                        ),
  .vm_status_tl             ( vm_status_tl1                    ),
  // Error output           
  .err_flags                ( tensor_load_err_flags_1          ),
  // For performance counters
  .tl_start                 ( tl1_start                        ),
  .tl_op                    ( tl1_op                           ),
  // Bus error
  .bus_err                  ( bus_err_tl1                      ),
  // For debug signals
  .csr_debug_signals        ( dbg_csr_tl1                      ),
  .sm_match_debug_signals   ( sm_match_debug_signals_tl1       ),
  .sm_filter_debug_signals  ( sm_filter_debug_signals_tl1      ),
  .sm_data_debug_signals    ( sm_data_debug_signals_tl1        )
);

// Connect core response signals
assign core_ctrl_resp.tl_ready_0    = !tl0_working & !dcache_cfg_stall;
assign core_ctrl_resp.tl_ready_1    = !tl1_working & !dcache_cfg_stall;
assign core_ctrl_resp.tl_complete_0 = tl0_id_complete;

////////////////////////////////////////////////////////////////////////////////
// Miss handling
////////////////////////////////////////////////////////////////////////////////

dcache_mh_req                    s2_mh_req_info;          // Replay info from miss handler
logic [`DCACHE_WAY_RANGE]        s4_mh_refill_way;        // Way for the MH fills from L2
logic [`DCACHE_MH_FILE_SIZE-1:0] s2_mh_handler_ready;     // A miss handler is ready (can accept new misses/miss finished)
logic                            s2_ba_write_needed_core; // Core needs to write data in the buffer array
logic                            s2_ba_write_needed;      // Needs to write data in the buffer array
logic                            s2_mh_req_valid;         // Miss handler received a new miss
logic                            s2_mh_req_valid_reg;     // Valid signal to miss handler from regular operations

logic [`DCACHE_MH_FILE_SIZE-1:0] mh_fill_bus_error;       // Miss handler got a bus error during a fill operation
logic [`DCACHE_MH_FILE_SIZE-1:0] mh_uc_load_bus_error;    // Miss handler got a bus error during a uc load operation

always_comb begin
  // Gets the meta data for the replaced way
  s2_repl_meta = 'b0;
  for(integer i = 0; i < `DCACHE_WAYS; i++)
    if(s2_replace_way_en[i]) s2_repl_meta = s2_meta_read[i];

  // Sends the miss request
  s2_mh_req_valid_reg = s2_valid_qual && !s2_nack_pre_misaligned_miss && !s2_hit                 // Must not have a hit and instruction not nack'ed 
                        && (dcache_cmd_is_prefetch(s2_req.cmd) || s2_is_read || s2_is_write)     // And must be a read/write operation
                        && !s2_req.uc_load_2nd                                                   // And must not be a message of a second pass of a UC
                        && !(s2_req.replay && !s2_req.miss_fill_ok);                             // And must not have seen a miss error in its request

  s2_mh_req_valid = s2_mh_req_valid_reg;                                                            // Regular instructions

  s2_mh_req_info.addr       = s2_req.addr[`PA_RANGE];
  s2_mh_req_info.set        = s2_req.set;
  s2_mh_req_info.cmd        = s2_req.cmd;
  s2_mh_req_info.typ        = s2_req.typ;
  s2_mh_req_info.mem_g      = s2_req.mem_global;
  s2_mh_req_info.buf_id     = !s2_req.replay ? s2_ba_write_entry : s2_req.buf_id;
  s2_mh_req_info.tag_match  = s2_tag_match_any;
  s2_mh_req_info.cacheable  = s2_req.cacheable;
  s2_mh_req_info.thread_id  = s2_req.dest.thread_id;
  s2_mh_req_info.is_amo     = s2_is_amo;
  s2_mh_req_info.is_msg     = 1'b0;
  s2_mh_req_info.is_bypass  = s2_is_bypass;
  `ifdef DCACHE_REPORT_PC
  s2_mh_req_info.pc         = s2_req_pc;
  `endif
  // Hit case (line is present, need to upgrade state)
  if(s2_tag_match_any) begin
    s2_mh_req_info.old_meta.tag       = s2_repl_meta.tag;  // Don't care for hit, send same as miss to reduce logic
    s2_mh_req_info.old_meta.coh_state = s2_way_hit_state;  // Hit state
    s2_mh_req_info.way_en             = s2_tag_match_qual; // Way that hit
  end else begin
    // Miss: send the old data info
    s2_mh_req_info.old_meta = s2_repl_meta;
    s2_mh_req_info.way_en   = s2_replace_way_en;
  end

  // Sends the LRU update to the LRU array
  s2_lru_data_up = dcache_lru_update(s2_lru_data, s2_mh_req_info.way_en);

  // We need to update the LRU when we see the first pass (valid) in the S2
  // stage. We don't update the LRU if we see that the transaction is a miss
  // and it is being merged with another outstanding miss in same set. The
  // reason is that in this case we would be updating the LRU for a selected
  // victim that is not going to be used. So we prefer to not update the LRU
  // at all for this case (the correct thing would be to update the way of
  // the outstanding miss, but this is costly)
  s2_lru_update = s2_valid_masked && (s2_hit || ~s2_mh_match_miss_line) && s2_req.cacheable;
end

////////////////////////////////////////////////////////////////////////////////
// This module deals with the misses in-flight in the dcache
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_MH_FILE_SIZE-1:0] s2_mh_req_assigned;    // To which miss handler the miss is assigned, to wait for it
logic [`DCACHE_MH_FILE_SIZE-1:0] s2_mh_req_accepted;    // Which miss handler has accepted the miss
logic [`CORE_NR_THREADS-1:0]     s2_mh_fence_rdy;       // Miss handler is fence ready
logic [`DCACHE_LRAM_ADDR_RANGE]  s4_mh_refill_addr;     // LRAM addr for the MH fills from L2
logic                            s2_mh_can_accept_uc;   // A UC can be accepted by miss handler
logic                            mh_wait_l2_to_ba_resp; // Indicates that MH is waiting for some L2 response to write into buffer array

logic [`DCACHE_BUFFER_ID_RANGE]  s3_mh_ba_wptr;         // Write pointer for BA during UC loads replied from L2

dcache_miss_handler_unit miss_handler_unit (
  // System signals
  .clock                     ( clock                      ),
  .reset                     ( reset                      ),
  .neigh_min_id              ( shire_min_id[`MIN_PER_N_R] ),
  // Miss request from dcache pipeline
  .req_ready                 ( s2_mh_req_ready            ),
  .req_valid                 ( s2_mh_req_valid            ),
  .req_info                  ( s2_mh_req_info             ),
  .req_assigned              ( s2_mh_req_assigned         ),
  .req_accepted              ( s2_mh_req_accepted         ),
  .can_accept_uc             ( s2_mh_can_accept_uc        ),
  .idle                      ( mh_idle                    ),
  .wait_l2_to_ba_resp        ( mh_wait_l2_to_ba_resp      ),
  .inflight_reduce           ( re_ts_l2_evict_inflight    ),
  .going_to_uc_store         ( mh_going_to_uc_store       ),
  // Mem write request bus to evict
  .l2_evict_ready            ( mh_l2_evict_ready          ),
  .l2_evict_valid            ( mh_l2_evict_valid          ),
  .l2_evict_req              ( mh_l2_evict_req            ),
  // Mem acquire request bus to miss
  .l2_miss_ready             ( mh_l2_miss_ready           ),
  .l2_miss_valid             ( mh_l2_miss_valid           ),
  .l2_miss_req               ( mh_l2_miss_req             ),
  // Mem grant bus
  .l2_resp_valid             ( l2_resp_valid_ready_int    ),
  .l2_resp_data              ( l2_resp_int                ),
  .l2_resp_slot_avail        ( l2_fill_slot_avail         ),
  .l2_resp_stall             ( mh_l2_resp_stall           ),
  // Refill control
  .refill_way                ( s4_mh_refill_way           ),
  .refill_addr               ( s4_mh_refill_addr          ),
  // Meta write request
  .meta_write_ready          ( s1_mh_mw_grant             ),
  .meta_write_valid_next     ( s1_mh_mw_valid_early       ),
  .meta_write_valid          ( s1_mh_mw_valid             ),
  .meta_write_clear_slock    ( s1_mh_mw_clear_slock       ),
  .meta_write_req            ( s1_mh_mw_req               ),
  // Writeback request
  .writeback_ready           ( mh_wb_ready                ),
  .writeback_valid           ( mh_wb_valid                ),
  .writeback_req             ( mh_wb_req                  ),
  .writeback_done            ( wb_ready                   ),
  // Buffer array read port control for UC stores
  .force_ba_rptr             ( s1_mh_force_ba_rptr        ),
  .force_ba_rptr_qual        ( s1_mh_force_ba_rptr_qual   ),
  .ba_rptr                   ( s1_mh_ba_rptr              ),
  .ba_is_amo                 ( s1_mh_ba_is_amo            ),
  // Buffer array write port control for UC loads
  .ba_wptr                   ( s3_mh_ba_wptr              ),
  // Collisions avoidance with Cache Ops
  .probe_rdy                 ( mh_probe_rdy               ),
  .metaw_rdy                 ( mh_metaw_rdy               ),
  // Misc
  .match_miss_line           ( s2_mh_match_miss_line      ),
  .fence_rdy                 ( s2_mh_fence_rdy            ),
  .handler_ready             ( s2_mh_handler_ready        ),
  .handler_fill_bus_error    ( mh_fill_bus_error          ),
  .handler_uc_load_bus_error ( mh_uc_load_bus_error       ),
  // Bus error (per thread_id)
  .bus_err                   ( bus_err_mh                 ),
  .bus_err_addr              ( bus_err_addr_mh            ),
  `ifdef DCACHE_REPORT_PC
  .bus_err_pc                ( bus_err_pc_mh              ),
  `endif
  // For debug signals
  .csr_debug_signals         ( dbg_csr_mh                 ),
  .sm_match_debug_signals    ( sm_match_debug_signals_mh  ),
  .sm_filter_debug_signals   ( sm_filter_debug_signals_mh ),
  .sm_data_debug_signals     ( sm_data_debug_signals_mh   )  
);

// Indicate to Cache Ops that a line is being evicted, which may require a soft-lock bit cleaning
assign mh_clear_slock_valid = s1_mh_mw_valid & s1_mh_mw_clear_slock; 
assign mh_clear_slock_set   = s1_mh_mw_req.set;
assign mh_clear_slock_way   = dcache_way_encode(s1_mh_mw_req.way_en); 

////////////////////////////////////////////////////////////////////////////////
// The replay queue stores transactions that had an issue during the execution
// and need to be replayed.
////////////////////////////////////////////////////////////////////////////////

dcache_replay_conf              s2_rq_conf_type;           // List of problems that the instruction that is being pushed has
logic [`CORE_NR_THREADS-1:0]    s2_rq_empty;               // Replay queue is empty
logic [`DCACHE_BUFFER_ID_RANGE] s3_ba_write_addr_ext;      // Write addr to buffer array due UC loads/reduce
logic                           s2_rq_push;                // Push the S2 request in replay queue
logic                           s2_rq_push_cancel;         // Cancels a push to request queue
logic                           s2_rq_dealloc;             // A used replay queue entry can be deallocated
logic                           s2_rq_renack;              // A replayed instruction must be replayed again
logic                           s2_store_misaligned_rearm; // Doing a rearm of store misaligned
logic                           s2_uc_load_1st;            // Done the 1st pass of a UC load
logic                           s2_uc_load_ok;             // A replayed instruction got an OK in a UC load
logic                           s2_miss_fill_ok;           // A replayed instruction got an OK in a Miss request
logic                           s3_ba_write_en_ext;        // Doing a write from L2 to buffer array related to UC loads
logic                           s2_ba_write_en_ext_prev;   // Forecasting that a write from L2 to buffer array will come soon
logic                           s3_ba_write_en_uc;         // Doing a write to buffer array due a UC load

logic                           id_rq_alloc_req_pre;
logic                           s0_rq_alloc_req_val;
logic                           id_rq_alloc_gsc;

logic                           rq_uc_load_fill;   
logic [`DCACHE_BUFFER_ID_RANGE] rq_uc_load_fill_id;
logic                           rq_uc_load_fill_ok;

// Core or Cache Ops may allocate entries (both can issue instructions)
// If Cache Ops is biding, instruction is never GSC
assign id_rq_alloc_req_pre = id_core_alloc_rq_pre | id_co_pf_bid;
assign s0_rq_alloc_req_val = s0_core_alloc_rq_val | s0_co_pf_valid;
assign id_rq_alloc_gsc     = id_co_pf_bid ? 1'b0 : id_core_gsc;

dcache_replay_queue replay_queue (
  // System signals
  .clock             ( clock                     ),
  .reset             ( reset                     ),
  // Control
  .full              ( id_rq_full                ),
  .half_full         ( id_rq_half_full           ),
  .empty             ( s2_rq_empty               ),
  // Alloc port
  .alloc_req_pre     ( id_rq_alloc_req_pre       ),
  .alloc_req_val     ( s0_rq_alloc_req_val       ),
  .alloc_gsc         ( id_rq_alloc_gsc           ),
  // Write port
  .push_req          ( s2_rq_push                ),
  .push_req_prev     ( s1_rq_push_conservative   ),
  .req               ( s2_req                    ),
  .cacheable         ( s2_req.cacheable          ),
  .req_buf_alloc     ( s2_ba_write_needed_core   ),
  .req_buf_entry     ( s2_ba_write_entry         ),
  .req_conf_type     ( s2_rq_conf_type           ),
  `ifdef DCACHE_REPORT_PC
  .req_pc            ( s2_req_pc                 ),
  `endif
  .misaligned_1st    ( s2_nack_misaligned        ),
  .uc_load_1st       ( s2_uc_load_1st            ),
  .uc_load_ok        ( s2_uc_load_ok             ),
  .miss_fill_ok      ( s2_miss_fill_ok           ),
  .rq_conflict       ( s2_nack_rq_conflict       ),
  .rq_conflict_to_co ( rq_conflict_to_co         ),
  // Re-arm port  (replay again a replayed instruction)
  .rearm             ( s2_rq_renack              ),
  .rearm_prev        ( s1_rq_renack_conservative ),
  .rearm_entry       ( s2_req.replay_entry       ),
  // Dealloc port
  .dealloc           ( s2_rq_dealloc             ),
  .dealloc_entry     ( s2_req.replay_entry       ),
  // Wake up signals
  .miss_available    ( s2_mh_handler_ready       ),
  .miss_fill_error   ( mh_fill_bus_error         ),
  .uc_load_fill      ( rq_uc_load_fill           ),
  .uc_load_fill_id   ( rq_uc_load_fill_id        ),
  .uc_load_fill_ok   ( rq_uc_load_fill_ok        ),
  .vpu_rf_in_use     ( s1_vpu_ctrl.tfma_rf_write ),
  // Read port
  .replay_bid        ( id_rq_replay_bid          ),
  .replay_grant      ( id_rq_grant               ),
  .replay_valid      ( s0_rq_replay_valid        ),
  .replay_req        ( s0_rq_replay              ),
  .replay_entry      ( s0_rq_replay_entry        ),
  .replay_age        ( s0_rq_replay_age          ),
  .replay_uc_ok      ( s0_rq_replay_uc_load_ok   ),
  .replay_miss_ok    ( s0_rq_replay_miss_fill_ok ),
  // Scoreboard to core to know which registers are not ready
  .scoreboard        ( id_core_scoreboard        ),
  .fp_sb_dealloc     ( id_core_sb_fp_dealloc     ),
  .int_sb_dealloc    ( id_core_sb_int_dealloc    )
);

always_comb begin
  // Computes when we are doing a store misaligned rearm
  s2_store_misaligned_rearm = s2_valid_to_s3 && s2_is_write && s2_nack_misaligned;

  // List of cases when the push to request queue is cancelled
  s2_rq_push_cancel = s2_valid_to_s3 && !s2_store_misaligned_rearm                           // Transaction sent to S3 (exception are 1st pass misaligned stores)
                     || s2_mh_can_accept_uc && s2_mh_req_valid && s2_is_write && !s2_is_amo    // UC store accepted transactions
                     || dcache_cmd_is_prefetch(s2_req.cmd) && !s2_nack_pre_misaligned          // Prefetches that were not nacked (allocated the miss handler or got a hit without conflict)
                     || s2_valid_masked && s2_is_bypass && s2_is_write                         // Write to upper memories allocated the miss handler without conflicts with previous operations
                     || s2_valid_masked && s2_req.uc_load_2nd && !s2_req.uc_load_ok            // UC read accesses that didn't finish well
                     || s2_valid_masked && s2_req.replay && !s2_req.miss_fill_ok;              // Miss Fill that didn't finish well

  // Push to replay queue when there's a valid element that is not a replay
  // and is not a cancel push case
  s2_rq_push = s2_valid_qual && !s2_req.replay && !s2_rq_push_cancel;

  // Rearm computation (replayed instruction is nacked and must be replayed
  // again)
  s2_rq_renack = s2_valid_qual && s2_req.replay && !s2_rq_push_cancel;

  // Dealloc a used replay queue entry
  s2_rq_dealloc = s2_valid_qual && s2_req.replay && s2_rq_push_cancel || s2_pma_bus_err;

  // List of problems to be used for entry wake up
  s2_rq_conf_type.miss_alloc    = s2_nack_miss       & !s2_nack_vpu_rf_write;
  s2_rq_conf_type.miss_assigned = s2_mh_req_assigned & {`DCACHE_MH_FILE_SIZE{!s2_nack_vpu_rf_write}};
  s2_rq_conf_type.miss_accepted = s2_mh_req_accepted & {`DCACHE_MH_FILE_SIZE{!s2_nack_vpu_rf_write}};
  s2_rq_conf_type.vpu_rf_write  = s2_nack_vpu_rf_write;

  // Computing when the 1st pass of a UC load is done
  s2_uc_load_1st = s2_mh_req_valid && s2_is_read && s2_mh_can_accept_uc && !s2_req.uc_load_2nd;

  // Propagate uc_load_ok flag for instructions that are re-nacked (default value is 0)
  s2_uc_load_ok = s2_req.replay ? s2_req.uc_load_ok : 1'b0;

  // Propagate miss_fill_ok flag for instructions that are re-nacked (defualt value is 1)
  s2_miss_fill_ok = s2_req.replay ? s2_req.miss_fill_ok : 1'b1;
end

////////////////////////////////////////////////////////////////////////////////
// The buffer unit holds data for outstanding transactions in dcache. Usually
// only transactions going to replayQ should use this buffer
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_DATA_RANGE] s3_ba_write_data;   // Data to be written in the buffer array
logic                      s2_ba_dealloc;      // Dealloc a buffer array entry as it is no longer needed
logic                      s2_ba_nack_needed;  // Instruction in S2 that is nacked for first time needs the buffer array entry
logic                      s2_ba_write_en;     // Writes new content to the buffer array
logic                      s2_ba_replay;       // Replay bit for buffer array
logic                      s3_ba_write_en_msg; // Doing a write to buffer array due a message

logic [`DCACHE_DATA_RANGE] s3_ba_write_data_ext; // Data from l2 responses

dcache_buffer_array buffer_array (
  // System signals
  .clock                 ( clock                   ),
  .reset                 ( reset                   ),
  // Control
  .full                  ( id_ba_full              ),
  .reduce_full           ( re_ba_full              ),
  // Alloc port
  .alloc_req_pre         ( id_core_alloc_rq_pre    ),
  .alloc_req_val         ( s0_core_alloc_rq_val    ),
  .alloc_gsc             ( id_core_gsc             ),
  .reduce_alloc_req      ( re_ba_alloc             ),
  // Write port for dcache operations
  .s2_write_needed_core  ( s2_ba_write_needed_core ),
  .s2_write_needed       ( s2_ba_write_needed      ),
  .s2_write_en           ( s2_ba_write_en          ),
  .s2_write_replay       ( s2_ba_replay            ),
  .s2_write_replay_entry ( s2_req.buf_id           ),
  .s3_write_data         ( s3_ba_write_data        ), // Data comes a cycle later, delay of write enable done internally
  .s2_write_entry        ( s2_ba_write_entry       ),
  .s3_write_en           ( s3_ba_write_en          ),
  // Write port for UC loads
  .s3_write_en_ext       ( s3_ba_write_en_ext      ),
  .s2_write_en_ext_prev  ( s2_ba_write_en_ext_prev ), // Optimistic signal to indicate that an external writing is coming
  .s3_write_entry_ext    ( s3_ba_write_addr_ext    ),
  .s3_write_data_ext     ( s3_ba_write_data_ext    ),
  // Dealloc port for regular entries
  .s2_dealloc            ( s2_ba_dealloc           ),
  .s2_dealloc_entry      ( s2_req.buf_id           ),
  // Dealloc port for UC stores
  .s1_dealloc_uc         ( s1_ba_dealloc_uc        ),
  .s1_dealloc_uc_entry   ( s1_ba_rptr              ),
  // Dealloc port for Reduce
  .reduce_dealloc_req    ( re_ba_dealloc           ),
  .reduce_dealloc_entry  ( s1_ba_rptr              ),
  // Read port
  .s1_read_entry         ( s1_ba_rptr              ),
  .s1_read_data          ( s1_ba_data              )
);

always_comb begin
  // When the buffer array is needed
  s2_ba_nack_needed  = s2_is_write; // For 1st time nack instructions that are store
  s2_ba_write_needed_core = s2_valid_qual   && !s2_req.replay && s2_nack && s2_ba_nack_needed       // First time nacked and needs buffer array
                           || s2_valid_qual   && !s2_req.replay && s2_misaligned                    // First time misaligned access
                           || s2_valid_qual   && !s2_req.replay && s2_is_read && !s2_req.cacheable  // Uncacheable loads
                           || s2_mh_req_valid && !s2_req.replay && s2_is_write;                     // Not replay store that miss
  s2_ba_write_needed = s2_ba_write_needed_core || s2_re_ba_write_needed;                            // Reduce instruction

  // Write the buffer array
  s2_ba_write_en = s2_valid_qual   && !s2_req.replay && s2_nack && s2_ba_nack_needed       // First time nacked and needs buffer array
                  || s2_valid_qual   && s2_nack_misaligned                                 // 1st pass for misaligned access
                  || s2_mh_req_valid && !s2_req.replay && s2_is_write                      // Not replay store that miss
                  || s2_reduce_send_inst;                                                  // Reduce instruction

  // Deallocate
  s2_ba_dealloc = ((s2_valid_to_s3)                                           ||     // when moving to S3 to deliver data after a hit
                   (s2_valid_masked && s2_req.uc_load_2nd)                    ||     // for any UC Load (OK or not)
                   (s2_valid_masked && s2_req.replay && !s2_req.miss_fill_ok) ||     // for any requests whose miss fill failed (do not have a hit)
                   (s2_pma_bus_err)                                                  // free entries allocated by  instructions that got an asynchronous exception
                  )
                 && !s2_store_misaligned_rearm && s2_req.buf_alloc;  // and buffer allocated

  // Replay bit cleared for reduce instructions
  s2_ba_replay = s2_req.replay && !s2_re_ba_write_needed;

  // Gets the write from external (UC load)
  s3_ba_write_en_uc    = l2_resp_valid_ready_int && l2_resp_is_for_mh_uc_ok;
  s3_ba_write_en_msg   = l2_resp_valid_ready_int && l2_resp_is_reduce_data;
  s3_ba_write_en_ext   = s3_ba_write_en_uc || s3_ba_write_en_msg;
  s3_ba_write_addr_ext = s3_ba_write_en_uc ? s3_mh_ba_wptr : s3_re_ba_write_addr;

  // Indication that buffer array may be written externally
  s2_ba_write_en_ext_prev = mh_wait_l2_to_ba_resp || re_wait_l2_to_ba_resp;
    
  // External data for buffer array writing
  s3_ba_write_data_ext = l2_resp_int.data;
end

// Fills for UC to wake-up entries in the replay queue
always_comb begin
  rq_uc_load_fill    = l2_resp_valid_ready_int && l2_resp_is_for_mh_uc;
  rq_uc_load_fill_id = s3_mh_ba_wptr;
  rq_uc_load_fill_ok = !(|mh_uc_load_bus_error);
end

////////////////////////////////////////////////////////////////////////////////
// Computes the request to S3
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_DATA_NUM_DWORDS_RANGE] s5_store_bypass;          // Store Bypass computation done in S5 (1 bit per 64b chunk) to be used in S3 next cycle
logic                                 s2_to_s3_en;              // Enable FF to S3 stage
logic                                 s2_to_s2_req_en;          // Enable FF for request to S3 stage
logic                                 s2_to_s3_en_store_data;   // Enable store data FF to S3
logic                                 s2_to_s3_en_store_byp;    // Enable store bypass data FF to S3
logic [`DCACHE_LRAM_BANK_RANGE]       s2_to_s3_en_load_data;    // Enable load data FF to S3
logic                                 s2_has_data;              // S2 has an operation that generates data

always_comb begin
  // Valid to next stage
  s2_valid_to_s3 = s2_valid_masked && (s2_hit || (s2_req.uc_load_2nd && s2_req.uc_load_ok)) // If a UC load and it didn't went well, no data to S3
                                   && (s2_is_write || !s2_nack_misaligned);                 // Regular instruction (killing misaligned 1st pass for loads)

  // To improve timing, we compute here the has_data bit sent to intpipe.
  // For AMO we need to verify that the destination is not integer register
  // zero as it is not moved to prefetch in S0 stage because we need to do
  // the AMO write always
  s2_has_data = s2_valid_to_s3 && s2_is_read && !(!s2_req.dest.fp && (s2_req.dest.addr == `XREG_ADDR_SIZE'b0));

  // Early int write
  s2_core_resp_int_valid = s2_valid_to_s3 && s2_is_read && !(s2_req.dest.fp || (s2_req.dest.addr == `XREG_ADDR_SIZE'b0));
end

always_comb begin
  // Update request contents to S3
  s2_to_s3_en  = (s2_valid_masked && (s2_hit || s2_req.uc_load_2nd)) // Regular instruction and 2nd pass UC load
                || s2_ba_write_en;                                     // For Buffer Array writes, to know if it is a read or write
  s2_to_s2_req_en = s2_to_s3_en || s2_has_xcpt_addr;                 // Propagate request to have address to S3 to report address that generated an exception

  // Update store data contents to S3
  s2_to_s3_en_store_data = (s2_valid_masked && s2_hit && s2_is_write)                         // Valid stores
                          || (s2_valid_masked && s2_req.uc_load_2nd)                            // 2nd pass UC load (data comes from buffer array)
                          || (s2_ba_write_en && s2_is_write)                                    // Stores that need a replay
                          || (s2_valid_masked && s2_hit && s2_is_read && s2_req.misaligned_2nd) // 2nd cycle for misaligned loads (1st pass data is in store data)
                          || s2_reduce_send_inst || s2_reduce_op_inst;                          // Reduce instructions

  // Update store bypass data to S3 for regular operations
  s2_to_s3_en_store_byp = (s2_valid_masked && s2_hit && (|s5_store_bypass));

  // Update load data to S3 for regular operations
  for (integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++)
    s2_to_s3_en_load_data[i] = ((s2_valid_masked && s2_hit) || s2_wb_da_valid) && s2_da_load_data_en[i];
    
  // Debug may be also reading data
  for (integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++)
    s2_to_s3_en_load_data[i] |= (dbg_da_read_addr[1:0] == i[1:0]) && dbg_da_read_accept;
end

// Copy data from Data Array in N blocks into a single array
always_comb begin
  // Get s2_data as LRAM output in single vector
  for (integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
    s2_data[i * `DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s2_da_read_resp[i];
end

// Mux data to go to store path
always_comb begin
  // By default, data to S3 store is s2_req.data
  
  // Data aligment conditions
  if (!s2_reduce_send_inst) begin
    // Core operations
    //   * In second pass of uc_load operations (bypass or uncacheable reads or atomics), data arrives address aligned, move to lsb
    //   * In "bypass" write operations, data must be sent address aligned, except for messages and atomics
    // Reduce store operations: data has not to be modified
    if (s2_reduce_op_inst)
      s2_to_s3_store_data = s2_req.data;
    else if (s2_req.uc_load_2nd) begin
      s2_to_s3_store_data = s2_req.data >> {s2_req.addr[4:0],3'b0};
    end
    else if (!s2_req.cacheable && s2_is_write && !s2_is_amo) begin
      s2_to_s3_store_data = s2_req.data << {s2_req.addr[4:0],3'b0};
    end 
    else if (s2_is_amo) begin // For AMO instructions, the data has to be extended with zeros.
      if (dcache_type_is_byte(s2_req.typ))
        s2_to_s3_store_data = `ZX(`DCACHE_DATA_SIZE,s2_req.data[`DCACHE_DATA_BYTE_SIZE-1:0]);
      else if (dcache_type_is_hword(s2_req.typ))
        s2_to_s3_store_data = `ZX(`DCACHE_DATA_SIZE,s2_req.data[`DCACHE_DATA_HWORD_SIZE-1:0]);
      else if (dcache_type_is_word(s2_req.typ))
        s2_to_s3_store_data = `ZX(`DCACHE_DATA_SIZE,s2_req.data[`DCACHE_DATA_WORD_SIZE-1:0]);
      else if (dcache_type_is_dword(s2_req.typ))
        s2_to_s3_store_data = `ZX(`DCACHE_DATA_SIZE,s2_req.data[`DCACHE_DATA_DWORD_SIZE-1:0]);
      else
        s2_to_s3_store_data = s2_req.data;
    end else begin
      s2_to_s3_store_data = s2_req.data;
    end
  end else begin
    // When TensorStore reads from scratchpad, data bus is taken from LRAM (s2_data), 
    // otherwise it is also taken from the store path fed by BA (s2_req.data).
    
    // TensorStore has resolution of 128 bits. Valid data will be in the lowest 128 bits for 
    // any address when column size is 128 bits
    if (ts_scp_read) begin
      if (!ts_col_size_is_128b)
        s2_to_s3_store_data =  s2_data;
      else
        s2_to_s3_store_data =  {s2_data[`DCACHE_DATA_SIZE/2-1:0],s2_data[`DCACHE_DATA_SIZE/2-1:0]};
    end else begin
      if (!ts_col_size_is_128b)
        s2_to_s3_store_data =  s2_req.data;
      else
        s2_to_s3_store_data =  {s2_req.data[`DCACHE_DATA_SIZE/2-1:0],s2_req.data[`DCACHE_DATA_SIZE/2-1:0]};
    end
  end
end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S3 stage
// This stage does all the datapath required after reading the data.
//   - Bypasses store data from older stores
//   - Aligns the data at a byte level
//   - Merges 1st pass and 2nd pass data for misaligned loads
//   - Finally sign extends or zero extends the data based on the request type
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_req                                     s3_req;                  // DCache request in S3 stage
logic [`DCACHE_DATA_RANGE]                     s3_store_data;           // Store data for S3
logic [`DCACHE_DATA_RANGE]                     s3_store_amo_ext_data;   // Store data for S3 with AMO extended operand, if applicable
logic [`DCACHE_DATA_RANGE]                     s3_store_bypass_data;    // Store bypass for S3 stage
logic [`DCACHE_DATA_RANGE]                     s5_store_bypass_data;    // Store bypass for S2 stage computed in S5
logic [`DCACHE_WAYS-1:0]                       s3_way;                  // Way enable for S3 stage (only needed for stores)
logic [`DCACHE_DATA_NUM_DWORDS_RANGE]          s3_store_bypass;         // Perform a store bypass in S3 (1 bit per 64b chunk)
logic                                          s3_valid;                // Valid instruction in S3
logic                                          s3_has_data;             // Instruction is a read
logic                                          s3_is_write;             // Instruction is a write
logic                                          s3_misaligned_case_3;    // Instruction has a misalignment case 3
logic [`VPU_LANE_RANGE][`CORE_GSC32_IDX_RANGE] s3_gsc32_indices;        // Per lane indices for gather/scatter on 32-byte blocks

//      CLK    RST    EN                      DOUT                  DIN                DEF
`RST_FF(clock, reset,                         s3_valid,             s2_valid_to_s3,    1'b0)
`RST_FF(clock, reset,                         s3_has_data,          s2_has_data,       1'b0)
`EN_FF (clock,        s2_to_s2_req_en,        s3_req,               s2_req)
`EN_FF (clock,        s2_to_s3_en,            s3_way,               s2_tag_match_qual)
`EN_FF (clock,        s2_to_s3_en,            s3_store_bypass,      s5_store_bypass)
`EN_FF (clock,        s2_to_s3_en,            s3_misaligned_case_3, s2_misaligned_case_3)
`EN_FF (clock,        s2_to_s3_en,            s3_is_write,          s2_is_write)
`EN_FF (clock,        s2_to_s3_en,            s3_gsc32_indices,     s2_gsc32_indices)
`EN_FF (clock,        s2_to_s3_en_store_data, s3_store_data,        s2_to_s3_store_data)  // OPTIMIZE: move to 64b granularity
`EN_FF (clock,        s2_to_s3_en_store_byp,  s3_store_bypass_data, s5_store_bypass_data) // OPTIMIZE: can we do the 2:1 mux in S2 between store bypass and load data? Move to 64b granularity

// Flop data to vpu with a 32b granularity

for (genvar num_word = 0; num_word < `DCACHE_DATA_NUM_WORDS; num_word++) begin: FLOP_LRAM_DATA_TO_VPU
    //     CLK    EN                            DOUT                                                                      DIN
    `EN_FF(clock, s2_vpu_scp_read_en[num_word], s3_vpu_scp_data[num_word*`DCACHE_DATA_WORD_SIZE+:`DCACHE_DATA_WORD_SIZE], s2_data[num_word*`DCACHE_DATA_WORD_SIZE+:`DCACHE_DATA_WORD_SIZE])
end

for(genvar  lram_bank = 0; lram_bank < `DCACHE_LRAM_NUM_BANKS; lram_bank++) begin : FLOP_LRAM_DATA_TO_S3
    //      CLK    EN                                DOUT                DIN
    `EN_FF (clock, s2_to_s3_en_load_data[lram_bank], s3_data[lram_bank], s2_da_read_resp[lram_bank])
end

////////////////////////////////////////////////////////////////////////////////
// Data muxing, rotation and sign extend
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_DATA_RANGE]            s3_data_bypassed;     // Data after store bypass
logic [`DCACHE_DATA_RANGE]            s3_data_aligned;      // Data aligned to 8b based on request address
logic [`DCACHE_DATA_RANGE]            s3_data_merged;       // Data merged between aligned and 1st pass for misalign
logic [`DCACHE_DATA_RANGE]            s3_load_data_mux1;    // Load data for S3 stage to go to BA
logic [`DCACHE_DATA_RANGE]            s3_load_data_mux2;    // Load data for S3 stage to go to Core
logic [(`DCACHE_DATA_SIZE/8)-1:0]     s3_data_merge_sel;    // Selects between regular data and 1st pass misalign
logic [`XREG_RANGE]                   s3_data_extended;     // Data is zero-extended or sign-extended depending on the request type
logic [`DCACHE_DATA_RANGE]            s3_data_gather32;     // Data muxed for gathers on 32-byte block
logic                                 s3_is_gather32;       // Operation is a gather on 32-byte block
logic [`VPU_LANE_RANGE][31:0]         s3_data_word;         // Load data for gather word on a 32-byte block
logic [`VPU_LANE_RANGE][15:0]         s3_data_half;         // Load data for gather half on a 32-byte block
logic [`VPU_LANE_RANGE][7:0]          s3_data_byte;         // Load data for gather byte on a 32-byte block

always_comb
begin
    // Selects between regular data and store to load bypass
    for(integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
      s3_data_bypassed[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s3_store_bypass[i] ? s3_store_bypass_data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] : s3_data[i];

    // Aligns the data to 8b
    s3_data_aligned = { s3_data_bypassed, s3_data_bypassed } >> { s3_req.addr[4:0], 3'b0 };

    // Generates byte selects from the 1st pass for misaligned accesses and 2nd pass UC loads
    //   * Reduce op instruction sets the merge selector to all 1s to get the store data in s3_data_merged.
    //   * UC loads sets the merge selector to all 1s to get the store data in s3_data_merged.
    //   * Misaligned accesses sets the merger selector to keep the bytes obtained in the first pass and
    //     combine them with bytes coming from s3_data_aligned (from LRAM + bypass) for second pass
    //   * In other cases, the merger selector is set to all 0s
    if (s3_req.uc_load_2nd | s3_reduce_op_inst)
      s3_data_merge_sel = {(`DCACHE_DATA_SIZE/8){1'b1}};
    else if (s3_req.misaligned_2nd)
      s3_data_merge_sel = {(`DCACHE_DATA_SIZE/8){1'b1}} >> s3_req.addr[4:0];
    else
      s3_data_merge_sel = {(`DCACHE_DATA_SIZE/8){1'b0}};

    // Selects store data when requiring 1st pass load data
    for(integer i = 0; i < (`DCACHE_DATA_SIZE/8); i++)
        s3_data_merged[i * 8 +: 8] = s3_data_merge_sel[i] ? s3_store_data[i * 8 +: 8] : s3_data_aligned[i * 8 +: 8];

    // Performs the sign extension or zero extension
    if     (s3_reduce_op_inst)                 s3_data_extended = s3_data_merged[`XREG_RANGE];                        // Do nothing for reduce operations
    else if(s3_req.typ == dcache_type_B)       s3_data_extended = { {56{s3_data_merged[7]}},  s3_data_merged[7:0]  };
    else if(s3_req.typ == dcache_type_PS_GS8)  s3_data_extended = { {56{s3_data_merged[7]}},  s3_data_merged[7:0]  }; // Same as before
    else if(s3_req.typ == dcache_type_H)       s3_data_extended = { {48{s3_data_merged[15]}}, s3_data_merged[15:0] };
    else if(s3_req.typ == dcache_type_PS_GS16) s3_data_extended = { {48{s3_data_merged[15]}}, s3_data_merged[15:0] }; // Same as before
    else if(s3_req.typ == dcache_type_W)       s3_data_extended = { {32{s3_data_merged[31]}}, s3_data_merged[31:0] };
    else if(s3_req.typ == dcache_type_PS_GS32) s3_data_extended = { {32{s3_data_merged[31]}}, s3_data_merged[31:0] }; // Same as before
    else if(s3_req.typ == dcache_type_BU)      s3_data_extended = { 56'b0,                    s3_data_merged[7:0]  };
    else if(s3_req.typ == dcache_type_HU)      s3_data_extended = { 48'b0,                    s3_data_merged[15:0] };
    else if(s3_req.typ == dcache_type_WU)      s3_data_extended = { 32'b0,                    s3_data_merged[31:0] };
    else                                       s3_data_extended = s3_data_merged[`XREG_RANGE];                         // dcache_type_D, dcache_type_PS

    // Determine if the operation is a gather on a 32-byte block
    s3_is_gather32 = (s3_req.cmd == dcache_cmd_G32) && s3_valid;

    for (integer i = 0; i < `VPU_LANE_NUM; i++)
    begin
        s3_data_word[i] = s3_data_bypassed[s3_gsc32_indices[i][4:2] * 32 +: 32];
        s3_data_half[i] = s3_data_bypassed[s3_gsc32_indices[i][4:1] * 16 +: 16];
        s3_data_byte[i] = s3_data_bypassed[s3_gsc32_indices[i]      *  8 +:  8];

        case (s3_req.typ)
            dcache_type_PS_GS8  : s3_data_gather32[i * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] = `SX(`VPU_DATA_S_SZ, s3_data_byte[i]);
            dcache_type_PS_GS16 : s3_data_gather32[i * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] = `SX(`VPU_DATA_S_SZ, s3_data_half[i]);
            dcache_type_PS_GS32 : s3_data_gather32[i * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] =                     s3_data_word[i];
            default             : s3_data_gather32[i * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] = 'b0;
        endcase
    end

    // 64 LSB are the extended ones, for core accesses. The remaining bits are the original merged bits for vector accesses.
    // As a result, for a vector access, s3_data_merged is the output to s3_load_data
    // Two muxes are differentiated given that gather32 data will never have to go to BA
    s3_load_data_mux1 = { s3_data_merged[`DCACHE_DATA_SIZE-1:`XREG_SIZE], s3_data_extended };
    s3_load_data_mux2 = s3_is_gather32 ? s3_data_gather32 : s3_load_data_mux1;

    // Sends the data to the buffer array depending on load or store
    s3_store_amo_ext_data = s3_store_data;
    if (s3_req.cmd == dcache_cmd_XA_CMPSWP)
    begin
      if (dcache_type_is_word(s3_req.typ))
        s3_store_amo_ext_data[`DCACHE_DATA_DWORD_SIZE+:`DCACHE_DATA_WORD_SIZE] = s3_core_x31[`DCACHE_DATA_WORD_SIZE-1:0];
      else
        s3_store_amo_ext_data[`DCACHE_DATA_DWORD_SIZE+:`DCACHE_DATA_DWORD_SIZE] = s3_core_x31;
    end
    s3_ba_write_data = s3_reduce_send_inst ? s3_store_data         :
                       s3_is_write         ? s3_store_amo_ext_data :
                                             s3_load_data_mux1;
end

////////////////////////////////////////////////////////////////////////////////
// Scratchpad return
////////////////////////////////////////////////////////////////////////////////

logic               vpu_scp_return_cycle_1;   // Flag indicating that first cycle of L2 response has been handled
dcache_vpu_scp_resp s3_vpu_scp_resp_tmp;      // Temporal version of Scp return information to VPU

// Some defaults for unused struct signals
assign s3_vpu_scp_resp_tmp.fill_is_tenb_early = 1'b0;
assign s3_vpu_scp_resp_tmp.tenb_flush = 1'b0;

//         CLK    RST    EN                                  DOUT                              DIN                                       DEF
`FF       (clock,                                            s3_vpu_scp_resp_tmp.fill_is_tenb, s3_vpu_scp_resp.fill_is_tenb_early            )
`EN_FF    (clock,        s3_vpu_scp_resp.fill_is_tenb_early, s3_vpu_tenb_data,                 l2_resp_int.data                              )
`EN_FF    (clock,        s3_vpu_scp_resp.fill_is_tenb_early, s3_vpu_scp_resp_tmp.tenb_line,    s4_tl1_load_addr[`DCACHE_SET_ADDR_RANGE]      )
`RST_EN_FF(clock, reset, s3_vpu_scp_resp.fill_is_tenb_early, vpu_scp_return_cycle_1,           ~vpu_scp_return_cycle_1,                  1'b0)

// For timing purposes, we need to register the ready signal for TENB to let enough time to decode the decision
`EN_FF    (clock,        l2_resp_valid_int,                  s4_tl1_tenb_ready_reg,            s4_tl1_tenb_ready                             )

always_comb begin
   // This is verilator friendly
   s3_vpu_scp_resp = s3_vpu_scp_resp_tmp;
   // By default SCP data to VPU comes from LRAM read flops
   s3_vpu_scp_resp.fill_is_tenb_early = l2_resp_valid_int && l2_resp_is_for_tensor_1 && s4_tl1_tenb_ready_reg && !l2_resp_ready_blocked;
   
   // Single pulse to indicate that TENB is written
   s4_tenb_written = s3_vpu_scp_resp.fill_is_tenb_early & vpu_scp_return_cycle_1;
   
   // Flush indication to TENB to clear the current buffer
   s3_vpu_scp_resp.tenb_flush = tenb_flush;
end

////////////////////////////////////////////////////////////////////////////////
// Store merge unit.
// This unit gets the store data, the original data from memory and generates
// the new data. The store data is shifted using the LSB of addr and only the
// new updated bytes using size, element size and mask are changed.
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_DATA_RANGE] s3_store_merge_out;       // Output of the store merge
logic                      s3_store_merge_enable_lo; // Enable the misalign

//OPTIMIZE: can we merge this with the merging done for misaligned loads??

dcache_store_merge_unit store_merge_unit (
  // Request input
  .addr       ( s3_req.addr[4:0]         ),
  .typ        ( s3_req.typ               ),
  .ps_mask    ( s3_req.ps_mask           ),
  .orig_data  ( s3_data_bypassed         ),
  .store_data ( s3_store_data            ),
  .enable_lo  ( s3_store_merge_enable_lo ),
  .enable_hi  ( !s3_req.misaligned_2nd   ),
  // Operation result
  .merge_out  ( s3_store_merge_out       )
);

assign s3_store_merge_enable_lo = s3_misaligned_case_3 || s3_req.misaligned_2nd;

////////////////////////////////////////////////////////////////////////////////
//  Store merge for scatter on a 32-byte block
////////////////////////////////////////////////////////////////////////////////

dcache_da_data                  s3_scatter32_data;

always_comb begin
  for(integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
    s3_scatter32_data[i] = s3_data_bypassed[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];

  for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
    if (s3_req.ps_mask[i]) begin
      case (s3_req.typ)
        dcache_type_PS_GS8  : s3_scatter32_data[s3_gsc32_indices[i][4:3]][s3_gsc32_indices[i][2:0] *  8 +:  8] = s3_store_data[i * 32 +:  8];
        dcache_type_PS_GS16 : s3_scatter32_data[s3_gsc32_indices[i][4:3]][s3_gsc32_indices[i][2:1] * 16 +: 16] = s3_store_data[i * 32 +: 16];
        dcache_type_PS_GS32 : s3_scatter32_data[s3_gsc32_indices[i][4:3]][s3_gsc32_indices[i][2]   * 32 +: 32] = s3_store_data[i * 32 +: 32];
      endcase
        end
    end
end


////////////////////////////////////////////////////////////////////////////////
// Generates the respond to the core
////////////////////////////////////////////////////////////////////////////////

dcache_req s4_req;     // DCache request in S4 stage
logic      s3_sc_fail; // Fail of store conditional

always_comb begin
    // Send dcache response
    s3_core_resp_valid    = s3_has_data || s3_reduce_op_inst;                                    // Generate fake valid for reduce op
    s3_core_resp.dest     = s3_req.dest;
    s3_core_resp.dest.fp  = s3_core_resp.dest.fp || s3_reduce_op_inst;                           // Force FP dest for reduce op
    s3_core_resp.typ      = (s3_reduce_op_inst || s3_is_gather32) ? dcache_type_PS : s3_req.typ; // Make operation PS for reduce op and gather on 32-byte block op
    s3_core_resp.gsc_cnt  = s3_req.gsc_cnt;
    s3_core_resp.ps_mask  = s3_req.ps_mask | {`VPU_LANE_NUM{s3_reduce_op_inst}};                 // Set all mask bits for reduce op
    s3_core_resp.data     = s3_load_data_mux2;
    s3_core_resp.data[0] |= s3_sc_fail;
    s3_core_resp.addr[`VA_RANGE]     = s3_req.addr;                                              // Address used for reporting exceptions (does not require valid flag)
    s3_core_resp.addr[`VA_EXT_MSB]   = s3_req.addr_msb_err ^ s3_req.addr[`VA_MSB];               // Address used for reporting exceptions (does not require valid flag)
   
    // Ordered when no transaction in dcache
    for(integer i = 0; i < `CORE_NR_THREADS; i++) begin
      s3_ordered[i] =  !(s0_valid_to_s1_ordered && (s0_req_thread_id_ordered == i))
                    && !(s1_valid               && (s1_req.dest.thread_id == i))
                    && !(s2_valid               && (s2_req.dest.thread_id == i))
                    && !(s3_valid               && (s3_req.dest.thread_id == i))
                    && !(s4_valid               && (s4_req.dest.thread_id == i))
                    && s2_mh_fence_rdy[i]
                    && s2_rq_empty[i]
                    && !dcache_cfg_stall;
    end
end

////////////////////////////////////////////////////////////////////////////////
// Load-reserve/store-conditional logic
////////////////////////////////////////////////////////////////////////////////

logic [`PA_CLINE_RANGE]   s3_lrsc_addr;                      // Address for LRSC
logic [`LRSC_COUNT_RANGE] s3_lrsc_count, s3_lrsc_count_next; // Counter of cycles where the address is alive
logic                     s3_lr;                             // Operation is a load-reserve
logic                     s3_sc;                             // Operation is a store conditional
logic                     s3_lrsc_addr_match;                // Address match of incoming operation with reserved line

//      CLK    RST    EN                 DOUT           DIN                 DEF
`RST_FF(clock, reset,                    s3_lrsc_count, s3_lrsc_count_next, `ZX(`LRSC_COUNT_SIZE, 1'b0))
`EN_FF (clock,        s3_valid && s3_lr, s3_lrsc_addr,  s3_req.addr[`PA_CLINE_RANGE])

always_comb begin
    s3_lrsc_valid = |s3_lrsc_count;

    // Decodes if it is a LR/SC operation
    s3_lr = (s3_req.cmd == dcache_cmd_XLR);
    s3_sc = (s3_req.cmd == dcache_cmd_XSC);
    // Match and fail check
    s3_lrsc_addr_match = s3_lrsc_valid && (s3_lrsc_addr == s3_req.addr[`PA_CLINE_RANGE]);
    s3_sc_fail = s3_sc && s3_valid && !s3_lrsc_addr_match;

    // Updates the counter and address
    s3_lrsc_count_next = s3_lrsc_count;
    // Decrements by default
    if(s3_lrsc_valid)
        s3_lrsc_count_next = s3_lrsc_count - `ZX(`LRSC_COUNT_SIZE, 1'b1);
    // When a new op comes in
    if(s3_valid)
    begin
        // Starting a load reserve
        if(s3_lr)
            s3_lrsc_count_next = `LRSC_CYCLES - `ZX(`LRSC_COUNT_SIZE, 1'b1);
        // Clean counter if it is not a load reserve
        else if(s3_lrsc_valid)
            s3_lrsc_count_next = `ZX(`LRSC_COUNT_SIZE, 1'b0);
    end
    // Clear counter when invalidated
    if(s3_invalidate_lr)
        s3_lrsc_count_next = `ZX(`LRSC_COUNT_SIZE, 1'b0);
end

////////////////////////////////////////////////////////////////////////////////
// Computes the request to S4
////////////////////////////////////////////////////////////////////////////////

dcache_req s3_req_to_s4; // DCache request going to S4 stage
logic      s3_to_s4_en;  // Enable FF to S4 stage

always_comb begin
  // Valid to next stage
  s3_valid_to_s4 = s3_valid && !s3_sc_fail && s3_is_write;

  // Update contents
  s3_to_s4_en = s3_valid && s3_is_write;

  // Muxes the store data
  s3_req_to_s4      = s3_req;
  s3_req_to_s4.data = s3_store_merge_out;
    
  if (s3_req.cmd == dcache_cmd_SC32) begin
    for(integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
      s3_req_to_s4.data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s3_scatter32_data[i];
  end
end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S4 stage
// In this stage the store to the data array is performed. All the S4 data is
// going to the data array write arbiter. Notice that stores have static prio
// so they can't be rejected.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_WAYS-1:0]        s4_way;                 // Way enable for S4 stage
logic                           s4_misaligned_case_3;   // Instruction has a misalignment case 3

//      CLK    RST    EN           DOUT                  DIN                    DEF
`RST_FF(clock, reset,              s4_valid,             s3_valid_to_s4,        1'b0)
`EN_FF (clock,        s3_to_s4_en, s4_req,               s3_req_to_s4)
`EN_FF (clock,        s3_to_s4_en, s4_way,               s3_way)
`EN_FF (clock,        s3_to_s4_en, s4_misaligned_case_3, s3_misaligned_case_3)

////////////////////////////////////////////////////////////////////////////////
// Data array
////////////////////////////////////////////////////////////////////////////////

// Two clients need the write port of the data array unit:
//  - Stores in the s4 stage
//  - Fills from SEND port
//  - Fills controlled by miss handler
// Stores have higher priority and always get access to the write port if
// required. Fills can only access the write port if there's no store and if
// the read port was not granted in previous cycle

logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_LRAM_ADDR_RANGE] s4_req_da_write_addr; // Address used to access the LRAM in case of an active s4_valid

always_comb begin
  // Write data signal for data array
  s4_da_write_en_but_dbg = s4_valid                              // Store
                || (l2_resp_valid_int && l2_da_write)            // Fills and no read granted previously
                || ((|s4_tl0_load_valid) && s4_tl0_load_ready)   // TensorLoad internal fills
                || (s4_co_da_clear_val | s4_cfg_da_clear_val);   // Cache Ops of Cache config willing to zero a cache line
  s4_da_write_en = s4_da_write_en_but_dbg 
                || (dbg_da_write_en & !dbg_da_write_valid);      // Debug write only one cycle, once valid returned, do not write anymore

  // Obtain the address that will be used for data sore bypass validated by s4_valid
  for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
    s4_req_da_write_addr[i] = { s4_req.set, s4_req.addr[5:0] };
    // Increment write address in case of misalignment
    if ((i[1:0] < s4_req.addr[4:3]) && s4_misaligned_case_3)
      s4_req_da_write_addr[i][`DCACHE_BLOCK_ADDR_RANGE] += 'b1;
  end
    
  // Default clear ready for cache ops and configuration
  s4_co_da_clear_rdy = 1'b0;
  s4_cfg_da_clear_rdy = 1'b0;

  // Select store if valid (highest priority)
  if(s4_valid) begin
    for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      // Write if contents were originally read (only read when a write is going to happen)
      s4_da_write_data[i].valid_l = s4_req.chunk_read[i];
      s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
      s4_da_write_data[i].way     = dcache_way_encode(s4_way);
      s4_da_write_data[i].addr    = s4_req_da_write_addr[i];
      s4_da_write_data[i].data    = s4_req.data[i[1:0]*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
    end
  end else if (l2_resp_valid_int && l2_da_write) begin
    // Select L2 fill, either initiated from Miss Handler or Tensor Load modules
        for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
            s4_da_write_data[i].valid_l = 1'b1;
            s4_da_write_data[i].valid_h = 1'b1;
            s4_da_write_data[i].data  = l2_resp_int.data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
            s4_da_write_data[i].way   = l2_resp_is_for_tensor_0 ? s4_tl0_load_way  : s4_mh_refill_way;
            s4_da_write_data[i].addr  = l2_resp_is_for_tensor_0 ? s4_tl0_load_addr : s4_mh_refill_addr;
        end
    end else if (|s4_tl0_load_valid) begin
    // Select TensorLoad transformation loads
        for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
            s4_da_write_data[i].valid_l = s4_tl0_load_valid[i];
            s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
            s4_da_write_data[i].data  = s4_tl0_load_data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
            s4_da_write_data[i].way   = s4_tl0_load_way;
            s4_da_write_data[i].addr  = s4_tl0_load_addr;
        end
    end else if (s4_cfg_da_clear_val) begin
        for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
            s4_da_write_data[i].valid_l = 1'b1;
            s4_da_write_data[i].valid_h = 1'b1;
            s4_da_write_data[i].data  = {`DCACHE_DATA_DWORD_SIZE{1'b0}};
            s4_da_write_data[i].way   = s4_cfg_da_clear_way;
            s4_da_write_data[i].addr  = {s4_cfg_da_clear_set,s4_cfg_da_clear_idx,5'b0};
        end
        s4_cfg_da_clear_rdy = 1'b1;
    end else if (s4_co_da_clear_val) begin
        for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
            s4_da_write_data[i].valid_l = 1'b1;
            s4_da_write_data[i].valid_h = 1'b1;
            s4_da_write_data[i].data  = {`DCACHE_DATA_DWORD_SIZE{1'b0}};
            s4_da_write_data[i].way   = s4_co_da_clear_way;
            s4_da_write_data[i].addr  = {s4_co_da_clear_set,s4_co_da_clear_idx,5'b0};
        end
        s4_co_da_clear_rdy = 1'b1;
    end else begin
    // Debug writing
    //   - Each address points to a 64-bit word, same as bank size
    // The LRAM address to access to a 32-byte row is built this way: {addr[9:5],way[1:0]} (7 bits, 128 rows)
    //   - Bits [4:0] are to point to those 32 bytes
    //   - LRAM address must have 10 bits (1k byte) (addr[9:5] used for row address)
    //   - There are 4 ways --> 4k bytes in total
    // For debug, use MSB as the way and LSB as the remaining row address
      for(integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
            s4_da_write_data[i].valid_l = dbg_da_write_en && (dbg_da_write_addr[1:0] == i[1:0]);
            s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
            s4_da_write_data[i].data    = dbg_da_write_data;
            
            // 3 bits to '0' as address is for 64-b (8 bytes)
            // 7 more bits to complete the address: `DCACHE_DBG_DA_ADDR_BITS-3:0 = 6:0
            s4_da_write_data[i].addr    = {dbg_da_write_addr[`DCACHE_DBG_DA_ADDR_BITS-3:0],3'b0};
            s4_da_write_data[i].way     = dbg_da_write_addr[`DCACHE_DBG_DA_ADDR_BITS-1:`DCACHE_DBG_DA_ADDR_BITS-2];
      end
    end

    // We need to pass down which blocks within cacheline we are writing in
    // current cycle to correctly do the store bypass computation
    for(integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++)
        s4_block_addr[i] = s4_req_da_write_addr[i][`DCACHE_BLOCK_ADDR_RANGE];
end

/////////////////////////////////////////////////////////////////////////////////
// Clock enable
////////////////////////////////////////////////////////////////////////////////

logic s4_clk_en; // Update the contents to S5 stage

always_comb begin
    s4_clk_en = s4_valid && s1_valid;
end

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S5 stage
// In this stage is computed the store to load forwarding control and also
// some preliminar data muxing. The final store bypass is done in S2 stage.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_req                                                       s5_req;                    // Request info in S5
logic                                                            s5_valid;                  // Valid in S5
logic                                                            s5_cl_addr_match_s3_s2;    // There's a cache line address match between S3 and S2
logic                                                            s5_cl_addr_match_s4_s2;    // There's a cache line address match between S4 and S2
logic                                                            s5_cl_addr_match_s5_s2;    // There's a cache line address match between S5 and S2

//      CLK    RST    EN         DOUT           DIN       DEF
`RST_FF(clock, reset,            s5_valid,      s4_valid, 1'b0)
`EN_FF (clock,        s4_clk_en, s5_req,        s4_req)         // Only need S5 to S1 bypass if there's an operation in S1
`EN_FF (clock,        s4_clk_en, s5_block_addr, s4_block_addr)

always_comb begin
    s5_store_bypass = {`DCACHE_DATA_NUM_DWORDS{1'b0}};

    // Computes the block address matches
    s5_cl_addr_match_s3_s2 = (s3_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB] == s2_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB]);
    s5_cl_addr_match_s4_s2 = (s4_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB] == s2_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB]);
    s5_cl_addr_match_s5_s2 = (s5_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB] == s2_req.addr[`PA_MSB:`DCACHE_LINE_ADDR_LSB]);

    // For all the LRAM blocks
    for(integer i = 0; i < `DCACHE_DATA_NUM_DWORDS; i++) begin
        // Compute the block within cacheline for inst in S2
        s2_block_addr[i] = s2_req.addr[`DCACHE_BLOCK_ADDR_RANGE];
        if ((i[1:0] < s2_req.addr[4:3]) && s2_misaligned_case_3)
            s2_block_addr[i] += 'b1;

        // S5 to S2 case (only stores here, no need to check command)
        // LRAM is being updated and it is going to same block within cache line
        s5_store_bypass_data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s5_req.data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
        if (s5_valid && s5_cl_addr_match_s5_s2 && s5_req.chunk_read[i] && (s5_block_addr[i] == s2_block_addr[i]))
            s5_store_bypass[i] = 1'b1;

        // S4 to S2 case (only stores here, no need to check command)
        // LRAM is being updated and it is going to same block within cache line
        if (s4_valid && s5_cl_addr_match_s4_s2 && s4_req.chunk_read[i] && (s4_block_addr[i] == s2_block_addr[i])) begin
            s5_store_bypass[i] = 1'b1;
            s5_store_bypass_data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s4_req.data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
        end

        // S3 to S2 case
        // Here we compute which block within cacheline is written by the
        // LRAM for S3 stage
        s3_block_addr[i] = s3_req_to_s4.addr[`DCACHE_BLOCK_ADDR_RANGE];
        if ((i[1:0] < s3_req_to_s4.addr[4:3]) && s3_misaligned_case_3)
            s3_block_addr[i] += 'b1;

        // LRAM is being updated and it is going to same block within
        // cache line
        if (s3_valid && s3_is_write && !s3_sc_fail && s5_cl_addr_match_s3_s2 && s3_req.chunk_read[i] && (s3_block_addr[i] == s2_block_addr[i])) begin
            s5_store_bypass[i] = 1'b1;
            s5_store_bypass_data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE] = s3_req_to_s4.data[i*`DCACHE_DATA_DWORD_SIZE +: `DCACHE_DATA_DWORD_SIZE];
        end
    end
end

////////////////////////////////////////////////////////////////////////////////
// Global Dcache Idle signal
// Combine the Idle or similar signals from all the modules
////////////////////////////////////////////////////////////////////////////////

always_comb begin
  // Full idle signal, including all state machines
  dcache_idle_next = !s0_valid_to_s1_ordered && !s1_valid && !s2_valid && !s3_valid && !s4_valid &&
                     (&s2_rq_empty) && !tl0_working && !tl1_working && !cacheop_busy && !cacheop_l2_busy &&
                     mh_idle && wb_ready && reduce_ready;

  // Idle signal to enter into exclusive mode
  dcache_idle_excl_mode_next = !s0_valid_to_s1_ordered && !s1_valid && !s2_valid && !s3_valid && !s4_valid &&
                               (&s2_rq_empty) && !tl0_working && !cacheop_busy && !cacheop_l2_busy &&
                               mh_idle && wb_ready && reduce_ready;
end

//          CLK    RST    EN    DOUT                   DIN                         DEF
`RST_FF    (clock, reset,       dcache_idle,           dcache_idle_next,           1'b0)
`RST_FF    (clock, reset,       dcache_idle_excl_mode, dcache_idle_excl_mode_next, 1'b0)

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provides access to the 4Kbytes of the LRAM (upper memory half) and to 
// metadata array (size set to maximum necessary width)
logic [`DCACHE_DBG_DA_ADDR_RANGE]  dbg_apb_paddr;
logic                              dbg_da_write_valid_next;

assign apb_pslverr = 1'b0;

// Read
// -----------------------------------------------------------------------------
logic dbg_block_sel_md;
logic dbg_block_sel_da;

always_comb begin
 
  // Generate read requests
  if (apb_psel & apb_penable & !apb_pwrite) begin
    dbg_md_read_en = dbg_block_sel_md;
    dbg_da_read_en = dbg_block_sel_da;
  end else
  begin
    dbg_md_read_en = 1'b0;
    dbg_da_read_en = 1'b0;
  end
  
  dbg_md_read_addr = dbg_apb_paddr[`DCACHE_DBG_MD_ADDR_RANGE];
  dbg_da_read_addr = dbg_apb_paddr;
    
  // Generate read replies
  apb_prdata = dbg_block_sel_md ? `ZX(`bpam_shire_apb_data_width,dbg_md_read_data) :
                                  dbg_da_read_data;
end

// Generate pready signal for LRAM read accesses
//   - By default set LRAM ready bit to '0'
//   - If access is to read LRAM, wait until we can inject read address in S1 to obtain data in S3
always_comb begin
  dbg_da_read_valid_next =  1'b0;
  if (apb_psel && apb_penable && dbg_da_read_en) begin
    dbg_da_read_valid_next = dbg_da_read_accept | dbg_da_read_valid;
  end
end


// LRAM Read pipeline (from S1 to S3)

// Reading from debug port has been accepted
`RST_EN_FF (clock, reset, apb_psel, dbg_da_read_accept, dbg_da_read_accept_next, 1'b0)

// Data read by debug is available at the output register
`RST_EN_FF (clock, reset, apb_psel, dbg_da_read_valid,  dbg_da_read_valid_next, 1'b0)

// Mux data from LRAM reading (select 64-bit word out of a 256-bit LRAM output)
assign dbg_da_read_data = s3_data[dbg_da_read_addr[1:0]];


// Metadata Read pipeline (from S0 to S2)

// Request access to pipeline for reading metadata
always_comb begin
  // Make only one request, once accepted, remove bid
  id_dbg_bid = dbg_md_read_en & !dbg_md_read_accept;
  
  // Only one access is required
  dbg_md_read_accept_next = apb_psel & (id_dbg_grant | dbg_md_read_accept);
end 

// Reading from debug port has been accepted
`RST_EN_FF (clock, reset, apb_psel | dbg_md_read_accept, dbg_md_read_accept, dbg_md_read_accept_next, 1'b0)

// Validate read in S0
`RST_EN_FF (clock, reset, apb_psel, s0_dbg_valid, id_dbg_grant, 1'b0)

// Propagate validation till S2
`RST_EN_FF (clock, reset, dbg_md_read_accept, s1_dbg_valid,      s0_dbg_valid, 1'b0)
`RST_EN_FF (clock, reset, dbg_md_read_accept, dbg_md_read_valid, s1_dbg_valid, 1'b0)

// Mux data from Metadata reading (select way)
assign dbg_md_read_data = s2_meta_read[dbg_md_read_addr[`DCACHE_SET_SZ+:`DCACHE_WAY_SZ]];


// Write
// -----------------------------------------------------------------------------
always_comb begin
  // Generate write requests
  if (apb_psel & apb_penable & apb_pwrite) begin
    dbg_da_write_en = dbg_block_sel_da;
    dbg_md_write_en = dbg_block_sel_md;
  end else begin
    dbg_da_write_en = 1'b0;
    dbg_md_write_en = 1'b0;
  end
  
  // Metadata and LRAM require an early signal
  if (apb_psel & !apb_penable & apb_pwrite) begin
    dbg_md_write_en_early = dbg_block_sel_md;
    dbg_da_write_en_early = dbg_block_sel_da;
  end else begin
    dbg_md_write_en_early = 1'b0;
    dbg_da_write_en_early = 1'b0;
  end
 
  // Obtain valid write to LRAM from S4 logic and keep it until the write signal is removed
  dbg_da_write_valid_next = dbg_da_write_en && (!s4_da_write_en_but_dbg || dbg_da_write_valid);
  
  // Write addresses
  dbg_md_write_addr  = dbg_apb_paddr[`DCACHE_DBG_MD_ADDR_RANGE];  
  dbg_da_write_addr  = dbg_apb_paddr;
end

assign dbg_md_write_data = apb_pwdata[`DCACHE_DBG_MD_DATA_RANGE];
assign dbg_da_write_data = apb_pwdata;

`RST_EN_FF (clock, reset, dbg_da_write_en | dbg_da_write_valid, dbg_da_write_valid, dbg_da_write_valid_next, 1'b0)

// Common for read/write
// -----------------------------------------------------------------------------
always_comb begin

  // Decode addresses
  dbg_block_sel_md = (apb_paddr[`DCACHE_DBG_ADDR_BITS-1] == 1'b0);
  dbg_block_sel_da = (apb_paddr[`DCACHE_DBG_ADDR_BITS-1] == 1'b1);

  // Ready reply, only if selection is set
  apb_pready  = dbg_block_sel_md ? (apb_pwrite ? dbg_md_write_valid : dbg_md_read_valid) : 
                                   (apb_pwrite ? dbg_da_write_valid : dbg_da_read_valid);
  apb_pready &= apb_psel;
end

assign dbg_apb_paddr = apb_paddr[`DCACHE_DBG_DA_ADDR_RANGE];

////////////////////////////////////////////////////////////////////////////////
// Events for performance monitor counters
////////////////////////////////////////////////////////////////////////////////
logic [`CSR_NR_EVENTS_DCACHE-1:0] io_events_next;
logic                             io_events_en;

always_comb begin
  io_events_next = `CSR_NR_EVENTS_DCACHE'b0;
  
  io_events_next[0] = s0_core_req_valid && !s0_core_req_valid_killed && (s0_core_req.dest.thread_id == 1'b0);
  io_events_next[1] = s0_core_req_valid && !s0_core_req_valid_killed && (s0_core_req.dest.thread_id == 1'b1);
  
  io_events_next[2] = s2_mh_req_valid && s2_req.cacheable && (s2_req.dest.thread_id == 1'b0);
  io_events_next[3] = s2_mh_req_valid && s2_req.cacheable && (s2_req.dest.thread_id == 1'b1);
  
  io_events_next[4] = |(l2_miss_req_valid & l2_miss_req_ready);
  io_events_next[5] = |(l2_miss_req_valid & ~l2_miss_req_ready) & !io_events_next[4];
  
  io_events_next[6] = |(l2_evict_req_valid & l2_evict_req_ready);
  io_events_next[7] = |(l2_evict_req_valid & ~l2_evict_req_ready) & !io_events_next[6];
  
  io_events_next[8] = tl0_start || tl1_start;
  io_events_next[9] = tl0_op || tl1_op;
  
  io_events_next[10] = ts_start;
  io_events_next[11] = ts_op;

  io_events_en = (|io_events_next) | (|io_events);
end

//          CK     RST    EN            DOUT        DIN             DEF
`RST_EN_FF (clock, reset, io_events_en, io_events,  io_events_next, `CSR_NR_EVENTS_DCACHE'b0)


////////////////////////////////////////////////////////////////////////////////
// Debug signals
////////////////////////////////////////////////////////////////////////////////

always_comb begin
  csr_debug_sigs.csr_signals_tl0 = dbg_csr_tl0;
  csr_debug_sigs.csr_signals_tl1 = dbg_csr_tl1;
  csr_debug_sigs.csr_signals_ts  = dbg_csr_ts;
  csr_debug_sigs.csr_signals_cou = dbg_csr_cou;
  csr_debug_sigs.csr_signals_coul2 = dbg_csr_coul2;
  csr_debug_sigs.csr_signals_txsnd = '0;
  csr_debug_sigs.csr_signals_mh0 = dbg_csr_mh[`DCACHE_DBG_CSR_MH_SIZE-1:0];
  csr_debug_sigs.csr_signals_mh1 = dbg_csr_mh[2*(`DCACHE_DBG_CSR_MH_SIZE)-1:`DCACHE_DBG_CSR_MH_SIZE];
end

// Connect SM Dcache debug signals
assign sm_match_debug_signals_dc = `DCACHE_DBG_SM_MATCH_BITS'b0;

assign sm_filter_debug_signals_dc = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
        {
          dcache_idle,
          s3_valid,
          s2_valid_to_s3,
          s2_rq_dealloc,
          s2_rq_renack,
          s2_rq_push,
          s2_valid_masked,
          s2_valid_qual,
          s2_valid,
          s1_tlb_req_valid,
          s1_valid_to_s2,
          s1_valid_masked,
          s1_valid,
          s0_co_l2_addr_load,
          s0_ts_addr_load,
          s0_tl_addr_load,
          s0_co_mr_valid,
          s0_rq_replay_valid,
          s0_core_req_valid,
          s0_co_pf_valid,
          s0_dbg_valid,
          s0_da_read_en,
          s0_wb_da_valid,
          s0_valid_to_s1,
          s2_req.addr[`PA_RANGE],
          s1_req.addr,
          s0_req_addr
        });

assign sm_data_debug_signals_dc[`DCACHE_DBG_SM_DATA_GROUP(3)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          s3_req.addr_msb_err,
          s3_req.phys,
          s3_req.dest,
          s3_req.cmd,
          s3_req.typ,
          s3_req.gsc_cnt,
          s3_req.gsc32_idx,
          s3_req.ps_mask,
          s3_req.replay,
          s3_req.replay_entry,
          s3_req.replay_age,
          s3_req.cache_op,
          s3_req.misaligned,
          s3_req.misaligned_2nd,
          s3_req.buf_alloc,
          s3_req.buf_id,
          s3_req.cacheable,
          s3_req.uc_load_2nd,
          s3_req.vm_sel,
          s3_req.chunk_read,
          s3_req.mem_global
        });

assign sm_data_debug_signals_dc[`DCACHE_DBG_SM_DATA_GROUP(2)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          s2_mh_req_valid,
          s2_tag_match_qual,
          s2_hit,
          s2_nack_inflight,
          s2_nack_miss,
          s2_nack_misaligned,
          s2_nack,
          s2_req.addr_msb_err,
          s2_req.phys,
          s2_req.dest,
          s2_req.cmd,
          s2_req.typ,
          s2_req.gsc_cnt,
          s2_req.gsc32_idx,
          s2_req.ps_mask,
          s2_req.replay,
          s2_req.replay_entry,
          s2_req.replay_age,
          s2_req.cache_op,
          s2_req.misaligned,
          s2_req.misaligned_2nd,
          s2_req.buf_alloc,
          s2_req.buf_id,
          s2_req.cacheable,
          s2_req.uc_load_2nd,
          s2_req.vm_sel,
          s2_req.chunk_read,
          s2_req.mem_global
        });

assign sm_data_debug_signals_dc[`DCACHE_DBG_SM_DATA_GROUP(1)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          {s1_misaligned_case_5,s1_misaligned_case_4,s1_misaligned_case_3,s1_misaligned_case_2,s1_misaligned_case_1},
          s1_tag_match,
          s1_translate_only_addr_load,
          s1_pma_resp_data.access_fault,
          s1_pma_resp_data.cacheable,
          s1_tlb_resp_data.xcpt_ld,
          s1_tlb_resp_data.xcpt_st,
          s1_tlb_resp_data.miss,
          s1_tlb_resp_data.fill_pending,
          s1_tlb_resp_data.access_fault,
          s1_req.addr_msb_err,
          s1_req.phys,
          s1_req.dest,
          s1_req.cmd,
          s1_req.typ,
          s1_req.gsc_cnt,
          s1_req.gsc32_idx,
          s1_req.ps_mask,
          s1_req.replay,
          s1_req.replay_entry,
          s1_req.replay_age,
          s1_req.cache_op,
          s1_req.misaligned,
          s1_req.misaligned_2nd,
          s1_req.buf_alloc,
          s1_req.buf_id,
          s1_req.cacheable,
          s1_req.uc_load_2nd,
          s1_req.vm_sel,
          s1_req.chunk_read,
          s1_req.mem_global
        });

assign sm_data_debug_signals_dc[`DCACHE_DBG_SM_DATA_GROUP(0)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          s0_req.addr_msb_err,
          s0_req.phys,
          s0_req.dest,
          s0_req.cmd,
          s0_req.typ,
          s0_req.gsc_cnt,
          s0_req.gsc32_idx,
          s0_req.ps_mask,
          s0_req.replay,
          s0_req.replay_entry,
          s0_req.replay_age,
          s0_req.cache_op,
          s0_req.misaligned,
          s0_req.misaligned_2nd,
          s0_req.buf_alloc,
          s0_req.buf_id,
          s0_req.cacheable,
          s0_req.uc_load_2nd,
          s0_req.vm_sel,
          s0_req.chunk_read,
          s0_req.mem_global
        });

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Cache Ops NOP operations are used to write marks to the log file
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

`ifdef ET_SIMULATION
`ifndef ZEBU
  always @(posedge clock)
  begin
    // Cache ops unit defines NOP operations. The WB data written in that operation is fully available in the reduce contol field
    if (cache_op_unit.req_up_id && cache_op_unit.cache_op_is_NOP)
    begin
      $display("INFO CO NOP: %0t: Minion: %0d: 0x%0x",$time,{shire_id,shire_min_id},`XREG_SIZE'(core_ctrl.reduce_tensorstore_ctrl));
    end  
  end
`endif // ifndef ZEBU
`endif

endmodule

