// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// STRUCTS FOR ESRs
////////////////////////////////////////////////////////////////////////////////
typedef struct packed {
  logic dmactive;
  logic ndmreset;
  logic clresethaltreq;
  logic setresethaltreq;
  logic hasel;
  logic ackhavereset;
  logic hartreset;
  logic resumereq;
  logic haltreq;
  logic write_enable;
} esr_ms_dmctrl_t;

////////////////////////////////////////////////
// STRUCT FOR DATA BEING MONITORED
///////////////////////////////////////////////
typedef struct packed {
    logic    [`NEIGH_DEBUG_FILTER_WIDTH-1:0]    filter_signals;
    logic                                       filter_valid;
    logic    [`NEIGH_DEBUG_MATCH_WIDTH-1:0]     match_signals;
    logic                                       match_valid;
    logic    [`NEIGH_DEBUG_DATA_WIDTH-1:0]      data_signals;
    logic                                       data_valid;
} neigh_sm_dbg_monitor_t;

typedef struct packed {
    logic    [`SPIO_DBG_SM_FILTER_WIDTH-1:0]    filter_signals;
    logic                                       filter_valid;
    logic    [`SPIO_DBG_SM_MATCH_WIDTH-1:0]     match_signals;
    logic                                       match_valid;
    logic    [`SPIO_DBG_SM_DATA_WIDTH-1:0]      data_signals;
    logic                                       data_valid;
} spio_sm_dbg_t;

typedef struct packed{ 
    logic [`shire_te_retires_per_cycle - 1:0]               instr_valid; 

    logic [(`shire_te_retires_per_cycle * 
                `shire_te_instr_width)- 1:0]                instr_bus;

    logic [(`shire_te_retires_per_cycle * 
                `shire_te_instr_addr_width) - 1:0]          instr_addr; 
    logic [(`shire_te_retires_per_cycle * 
                `shire_te_context_width) - 1:0]             instr_context;

    logic [`shire_te_retires_per_cycle - 1:0]               exception;
    logic [(`shire_te_retires_per_cycle * 
                `shire_te_ecause_width) - 1:0]              exception_cause;

    logic [`shire_te_retires_per_cycle - 1:0]               interrupt;

    logic [(`shire_te_retires_per_cycle * 
                `shire_te_instr_addr_width) - 1:0]          exception_trap_value;

    logic [(`shire_te_retires_per_cycle * 
                `shire_te_privilege_width) - 1:0]           privilege_level;

    logic [`shire_te_status_width - 1:0]                    status_flags;

    logic                                                   cpu_halted;

    logic                                                   min_reset;
} TraceEncoder_signals;

typedef struct packed{
    logic                                   		us_event;
    logic    [(2**`neigh_sm_upstream) - 1:0]        data;
} SM0_neigh_upstream_t;

typedef struct packed{
    logic                                   		us_event;
    logic    [(2**`neigh_sm_downstream) - 1:0]      data;
} SM0_neigh_downstream_t;


typedef struct packed{
    logic   [(`SC_BANKS+1)-1:0]    neigh_sc_req_ready;
    logic   [(`SC_BANKS+1)-1:0]    neigh_sc_req_valid;
    et_link_req_info_t             neigh_sc_req_info;
    logic                          neigh_sc_rsp_ready;
    logic                          neigh_sc_rsp_valid;
    et_link_rsp_info_t             neigh_sc_rsp_info;
} Neigh_ET_Link_us_monitor_t;

typedef struct packed{
    logic [`RBOX_SM_FILTER_WIDTH-1:0]     rbox_sm_filter;
    logic                                 rbox_sm_filter_valid;
    logic [`RBOX_SM_MATCH_WIDTH-1:0]      rbox_sm_match;
    logic                                 rbox_sm_match_valid;
    logic [`RBOX_SM_DATA_TRACE_WIDTH-1:0] rbox_sm_data;
    logic                                 rbox_sm_data_valid;
} rbox_dbg_us_monitor_t;


typedef struct packed{
    logic   us_locked;
    logic   ds_locked;
} msg_lock_active_t;

typedef struct packed{
    logic   us_status;
    logic   ds_status;
} msg_lock_status_t;
////////////////////////////////////////////////
// STRUCT FOR RUN CONTROL
///////////////////////////////////////////////

typedef enum logic [1:0]
{
    resume_IDLE = 2'b00,
    resume_RESUMING = 2'b01,
    resume_WAIT = 2'b10
} resume_state_type;


typedef struct packed{
    // Global
    logic ndmreset;
    // Minions
    logic halt_req;
    logic resume_req;
    logic hartReset_req;
    // TBOX
    logic tbox_halt_req;
    logic tbox_resume_req; 
    //RBOX
    logic rbox_halt_req;
    logic rbox_resume_req;
    logic rbox_reset_req;
} bpam_ms_rc_gpio_t;

typedef struct packed{
    // Minions
    logic halt_req;
    logic resume_req;
    // TBOX
    logic tbox_halt_req;
    logic tbox_resume_req; 
    //RBOX
    logic rbox_halt_req;
    logic rbox_resume_req;
} bpam_ms_rc_event_t;

typedef struct packed{
    bpam_ms_rc_gpio_t gpio;
    bpam_ms_rc_event_t events;
} bpam_run_control_t;

typedef struct packed{
    // Global
    logic ndmreset;
    // Minions
    logic halt_req;
    logic resume_req;
    logic hartReset_req;
    // TBOX
    logic tbox_halt_req;
    logic tbox_resume_req; 
} bpam_neigh_rc_gpio_t;

typedef struct packed{
    // Minions
    logic halt_req;
    logic resume_req;
    // TBOX
    logic tbox_halt_req;
    logic tbox_resume_req; 
} bpam_neigh_rc_event_t;

typedef struct packed{
    bpam_neigh_rc_gpio_t gpio;
    bpam_neigh_rc_event_t events;
    logic   tbox_single_step;
} bpam_run_control_neigh_t;

typedef struct packed{
    // TBOX
    logic halt_req     ;
    logic resume_req   ; 
    //logic ndmreset     ;
    //logic debug_reset  ; 
    logic single_step   ; 
} tbox_bpam_run_control_t;

typedef struct packed{
   //RBOX
    logic halt_req;
    logic resume_req;
    logic reset_req;
} bpam_rbox_rc_gpio_t;

typedef struct packed{
   //RBOX
    logic halt_req;
    logic resume_req;
} bpam_rbox_rc_event_t;

typedef struct packed{
    // RBOX
    bpam_rbox_rc_gpio_t gpio;
    bpam_rbox_rc_event_t events;
    logic   single_step;
} bpam_rc_rbox_t;

typedef struct packed{
   // TBOX
    logic halt_ack;
    logic resume_ack; 
} bpam_rc_tbox_ack_t;

typedef struct packed{    
    //RBOX
    logic halt_ack;
    logic resume_ack;
    logic reset_ack;
} bpam_rc_rbox_ack_t;

typedef struct packed{
//Bus Monitor
    logic                                           BM_us_msg_ready     ;        
    logic                                           BM_us_msg_valid     ; 
    logic                                           BM_us_msg_event     ; 
    logic [(2**`shire_bm_upstream)-1:0]             BM_us_msg_data      ; 
                                                                 
    logic                                           BM_ds_msg_ready     ; 
    logic                                  	        BM_ds_msg_valid     ; 
    logic                                  	        BM_ds_msg_event     ; 
    logic [(2**`shire_bm_downstream)-1:0]	        BM_ds_msg_data      ;

//Neigh-Minion SM 
    logic [`NUM_NEIGH-1:0]                          SM0_us_msg_valid    ; 
    logic [`NUM_NEIGH-1:0]                          SM0_us_msg_event    ; 
    logic [`NUM_NEIGH-1:0]
            [(2**`neigh_sm_upstream)-1:0] 	        SM0_us_msg_data     ;
    logic [`NUM_NEIGH-1:0]                          SM0_us_msg_ready    ; 
                                            
    logic [`NUM_NEIGH-1:0]                          SM0_ds_msg_ready    ; 
    logic [`NUM_NEIGH-1:0]                          SM0_ds_msg_valid    ;
    logic [`NUM_NEIGH-1:0]                          SM0_ds_msg_event    ; 
    logic [`NUM_NEIGH-1:0]
            [(2**`neigh_sm_downstream)-1:0]	        SM0_ds_msg_data     ;

//Neigh ET-Link SM
    logic                                   		SM1_us_msg_valid    ; 
    logic                                   		SM1_us_msg_event    ; 
    logic [(2**`shire_etlink_sm_upstream)-1:0] 	    SM1_us_msg_data     ;
    logic                                   		SM1_us_msg_ready    ; 
                                                                   
    logic                                  		    SM1_ds_msg_ready    ; 
    logic                                   		SM1_ds_msg_valid    ;
    logic                                   		SM1_ds_msg_event    ; 
    logic [(2**`shire_etlink_sm_downstream)-1:0]	SM1_ds_msg_data     ;

//Shire Cache SM
    logic                                   		SM2_us_msg_valid    ; 
    logic                                   		SM2_us_msg_event    ;
    logic [(2**`shire_cache_sm_upstream)-1:0] 	    SM2_us_msg_data     ;
    logic                                   		SM2_us_msg_ready    ;
                                                                      
    logic                                  		    SM2_ds_msg_ready    ;
    logic                                   		SM2_ds_msg_valid    ;
    logic                                   		SM2_ds_msg_event    ;
    logic [(2**`shire_cache_sm_downstream)-1:0]	    SM2_ds_msg_data     ;

//BPAM
    logic                                   		BPAM_us_msg_valid   ; 
    logic                                   		BPAM_us_msg_event   ;   
    logic [(2**`shire_bpam_upstream)-1:0] 	        BPAM_us_msg_data    ;
    logic                                   		BPAM_us_msg_ready   ; 
                                                                      
    logic                                  		    BPAM_ds_msg_ready   ; 
    logic                                   		BPAM_ds_msg_valid   ;
    logic                                   		BPAM_ds_msg_event   ; 
    logic [(2**`shire_bpam_downstream)-1:0]	        BPAM_ds_msg_data    ;
} ultrasoc_am_interface_t;


typedef struct packed{    
    //RBOX
    logic [`CORE_NR_THREADS-1:0] halt;
    logic [`CORE_NR_THREADS-1:0] resume;
} neigh_ch_dbg_rc_t;

typedef struct packed{    
    //RBOX
    logic [`CORE_NR_THREADS-1:0] halt;
    logic [`CORE_NR_THREADS-1:0] resume;
    logic [`CORE_NR_THREADS-1:0] resethalt;
} svcproc_dbg_rc_t;


typedef struct packed{
    // general signals
    logic                         id_csr_interrupt;
    logic                         excl_mode_stall;
    logic [`CORE_NR_THREADS-1:0]  ipi_with_pc;
    logic                         id_xcpt;
    logic                         wb_xcpt;
    // id_ctrl_stall signals
    logic                         csr_fe_stall;
    logic                         id_do_fence;
} intpipe_dbg_match_t;

typedef struct packed{
   // general signals
   logic                         id_thread_id;
   logic [`PC_RANGE_EXT]         id_pc; 
   logic                         id_valid;
   logic                         id_valid_qual;
   logic                         id_inst_replay;
   logic                         id_take_pc;
   logic                         id_csr_interrupt;
   logic                         excl_mode_stall;
   logic                         ex_valid_qual;
   logic                         tag_valid_qual;
   logic                         mem_valid_qual;
   logic                         wb_valid;
   logic [`CORE_GSC_CNT_RANGE]   gsc_reg_gsc_cnt;
   logic                         tag_take_pc;
   logic [`CORE_NR_THREADS-1:0]  ipi_with_pc;
   logic                         wb_take_pc;
   logic                         id_xcpt;
   logic                         wb_xcpt;
   // id_ctrl_stall signals
   logic                         id_stall_fp2int; 
   logic                         ex_gsc_busy;
   logic                         csr_fe_stall;
   logic                         id_ctrl_stall_csr;
   logic                         id_ctrl_stall_dcache;
   logic                         id_do_fence;
   logic                         id_ctrl_stall_trans;
   logic                         id_ctrl_stall_div;
   logic                         id_int_sboard_hazard;
   logic                         id_ctrl_stall_hazard_vpu;
   logic                         id_ctrl_stall_hazard;
} intpipe_dbg_monitor_t;
      
typedef struct packed {
    logic local_message_ready;
    logic tbox_min_rsp_ready;
    logic fill_ff_ready_for_no_msg;
} fill_ff_dbg_sm_t;

typedef struct packed {
    icache_prefetch_state                                         ucache_f0_prefetch_state;
    icache_miss_state [`ICACHE_NR_REQS-1:0]                       ucache_f0_miss_state;
    logic [`ICACHE_NR_REQS-1:0]                                   ucache_f2_valid;
    logic [`ICACHE_NR_REQS-1:0][`ICACHE_MICRO_CACHE_SIZE-1:0]     ucache_f2_hit_array;
    logic [`ICACHE_NR_REQS-1:0][`PA_RANGE_CA]                     ucache_f2_paddr;
    logic [`ICACHE_WAYS-1:0]                                      f1_tag_hit;
    icache_miss_state                                             f0_miss_state;
    logic                                                         f0_l1_miss_req_valid;
    logic [`PA_RANGE_CA]                                          f0_l1_miss_req_addr;
    logic [`ICACHE_NR_REQS-1:0]                                   f1_req_valid;
    logic [`ICACHE_NR_REQS-1:0][`VA_RANGE_CA]                     f1_req_addr;
} icache_dbg_sm_t;
  
typedef struct packed {
   ptw_state                     state;
   logic [`CSR_SATP_MODE_SZ-1:0] req_satp_mode;
   logic [1:0]                   req_prv;
   logic [`VA_TRANS_RANGE]       req_addr;
} ptw_dbg_sm_t;  

typedef struct packed {
    logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][(`NUM_NEIGH-1)-1:0] mst_ext_is_rdy;
    logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                     slv_has_rdy_creds;
    logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`NUM_NEIGH_R]       master_neigh;
    logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                     coop_tload_local_rdy;
} coop_tload_dbg_sm_t;

typedef struct packed {
    logic [14-1:0] local_message_valid_out;
} fln_dbg_sm_t;

typedef struct packed {
    logic [22-1:0] tstore_internal_dbg_signals;
} tstore_block_dbg_sm_t;

typedef struct packed {
    tstore_block_dbg_sm_t  tstore_north_dbg_sm_signals;
    tstore_block_dbg_sm_t  tstore_south_dbg_sm_signals;
} tstore_dbg_sm_t;

typedef struct packed {
   logic [31:0] reserved2;
   logic haltreq;
   logic resumereq;
   logic hartreset;
   logic ackhavereset;
   logic reserved1;
   logic hasel;
   logic [21:0] reserved0;
   logic setresethaltreq;
   logic clrresethaltreq;
   logic ndmreset;
   logic dmactive;
} esr_dmctrl_t;

typedef struct packed {
   logic anyunavailable;
   logic allhavereset;
   logic anyhavereset;
   logic allresumeack;
   logic anyresumeack;
   logic allrunning;
   logic anyrunning;
   logic allhalted;
   logic anyhalted2;
   logic anyhalted1;
   logic anyhalted0;
} esr_and_or_tree_L2_t;

