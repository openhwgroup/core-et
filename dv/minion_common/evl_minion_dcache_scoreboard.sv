//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_dcache_scoreboard
//
// Postponed
//    - Page split exceptions
//    - Texture requests
//
// Tasks: 
// *) to support RTL changes in PMA check (Jira RTLMIN-5656):
//    Completed, see Jira VERIF- 3135, 3132, 3122, 3113.
// *) to support handling "Bus_Err_Resp" and self-checking on D$->Core "bus_err" indication signals:
//    Completed, see Jira VERIF-3152, 3074, 3137 for details; under testing in regression (mainly in AAPG).
// *) to support DataArray model and, for all Load-type, self-check on D$->Core Resp+Data "s3_core_resp":
//    many different cases (cmd types and sizes), very involved;
//    Completed (see Jira VERIF-3153 for details); all local testing passed; pushed to master. 
// *) to support self-checking on D$->VPU signals "s3_vpu_tenb_data" (for TensorLoad_TenB):
//    Completed (see Jira VERIF-3213); all local testing passed; pushed to master.
// *) to support modeling L1ScratchPad and TensorLoad_L1SCP fill data into L1SCP, and for VPU_SCP_Read Req, 
//    self-checking on D$->VPU signals "s3_vpu_scp_data",
//    also to check on conflict between TensorLoad_L1SCP and VPU_SCP_Read Req;
//    Completed (see Jira VERIF-3214); all local testing passed; pushed to master.

// *) for TensorReduce_receiver: 
//    D$ receives reduce data from L2 message, then forward it to Core on "s3_core_resp" bus:
//    to support self-checking on the outgoing data (matching incoming data);

// *) for Capacity Eviction: self-check the Evict/WB data going out on L2_Evict Intf; --> working!
// *) for TensorReduce_sender: 
//    D$ receives data from VPU (vpu_reg_send), and then send the data out on L2_Evict Intf ... self-check the data; --> working!
// *) for TensorStore (from VPU_Reg): 
//    D$ receives data from VPU (vpu_reg_send), and then send the data out on L2_Evict Intf ... self-check the data; --> working!
// *) for TensorStore_SCP (from L1SCP): 
//    D$ receives data from L1SCP, and then send the data out on L2_Evict Intf ... self-check the data; --> working!

// *) check_s3_core_resp: for dcache_cmd_G32: s3_core_resp.type[3:0] is always dcache_type_PS (regardless of original req access_type):
//    Completed; see Jira VERIF-3348
// *) check_s3_core_resp: check (s3_core_resp.ps_mask == orig_req.ps_mask):
//    Completed; only for dcache_cmd_G32 and/or dcache_type_PS;
//
// *) check_s3_addr_when_expt_fault: when report s1_core_xcpt, check "s3_core_resp.addr[]": 
//    for misaligned_access: s3_core_resp.addr[] is special (coming from 2nd-pass):
//    Completed; see Jira VERIF-3341.
//
// *) check CacheOp_Max and CacheOp_RepRate:
//    Completed; see Jira VERIF-2195, VERIF-3374 (RepRate is tricky, especially decide when RTL starts counting a stream of L2 reqs ...)
//
// *) work on checking s3_core_resp.addr[48] (addr-overflow): 
//    Jira VERIF-3400, RTLMIN-6004, RTLMIN-6005;

// *) VERIF-3520: update to support data & byte_en checking on Atomic_CAS amo_cmp_data (besides amo_new_data);

// *) 3/18/2020: check TensorStore_From_L1SCP conflict with on-going TensorLoad_L1SCP (on same range of SCP_lines): see Jira VERIF-3525;

// *) 4/29/2020: VERIF-3630, 3631, 3632, RTLMIN-6148: in SvcProc (IOshire), for any CacheOp accesses (including Prefetch) to uncached region 
//    (like "high mem region"), DCache PMA check should flag "access_fault", and IntPipe/csr_file should set TensorError[7](?).-- FUTURE
//    --> 6/02/2020: in function check_pma: error out when hitting this case;

// *) 6/07/2020: for RTLMIN-6173: update for TLB miss (Virtual Memory: VA->PA): involve m_tlb_ptw_in_process: change to 2-bit; per thread; 
//    How does RTL handle TLB-Lookup for 2 threads? 
//    For example: after Thread 0 gets TLB miss (TLB fill_pending), what if a Thread 1 req comes? Will it get handled independently?
//    Yes, any subsequent Core IntPipe Req which didn't get killed by s0_core_req_valid_killed or s1_core_kill, will do TLB Lookup;
//    if TLB hit, the req will continue;
//    if TLB miss (and for a different page) and there is already a PTW still in process, this new req will do nothing (our TLB module 
//    can only handle one outstanding PTW).
//    --> seems working!

// *) 4/28/2021: VERIF-3538: younger Cached req still need to check for address-collision against older (UC) ST_WA and LD_RA still pending;
//    Note: ST_WA case has been supported previouly; adding LD_RA case this time. 


// *) FUTURE: after ->ev_s1_tlb_flush_pipeline[thread_id];
//    expect that RTL L1D$ sends s2_core_flush_pipeline (merged) to Core IntPipe:



// *) search for keyword "FUTURE" ...




// by default:  +define ET_AMOCMPEXCH_FIX = 1 : enable m_amocmpexch_fix_en=1 (by default)
`ifndef ET_AMOCMPEXCH_FIX
   `define ET_AMOCMPEXCH_FIX 1
`endif // ifndef ET_AMOCMPEXCH_FIX



class evl_minion_dcache_scoreboard #(type CACHE_TYPE = evl_minion_dcache_scoreboard) extends evl_scoreboard_mem;

   `uvm_component_param_utils(evl_minion_dcache_scoreboard #(CACHE_TYPE))

   `evl_param_key(pk_mem_bus_rsp_delay)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_l1sb_error_intensity)
   `evl_param_key(pk_cop_way_sel)
   `evl_param_key(pk_intpipe_req)             // indicate IntPipe Req
   `evl_param_key(pk_misaligned_2nd)          // trans_desc object
   `evl_param_key(pk_misaligned_1st)          // trans_desc object
   `evl_param_key(pk_this_is_misaligned_access)      // a flag (misaligned_1st_part)
   `evl_param_key(pk_this_is_misaligned_2nd_part)    // a flag (misaligned_2nd_part)
   `evl_param_key(pk_vector_ps)
   `evl_param_key(pk_replay_ready)            // req in ReplayQ is ready to be replayed
   `evl_param_key(pk_req_replayed)            // replayed req (v.s. new incoming req) 

   `evl_param_key(pk_tensorid)
   `evl_param_key(pk_tload_sel)               // pk_tload_sel=1 indicates TLoad_TenB
   `evl_param_key(pk_tload_num_lines)
   `evl_param_key(pk_tload_tenb_line_id)
   `evl_param_key(pk_tload_transform)         // for TLoad_L1SCP 
   `evl_param_key(pk_tload_coop)
   `evl_param_key(pk_tload_scp_dest)          // TLoad_L1SCP starting line
   `evl_param_key(pk_tload_scp_line_id)       // TLoad L1SCP actual line_id
   `evl_param_key(pk_tload_spawn_req_idx)     // TLoad L1SCP: L2 Req/Resp ID 
   `evl_param_key(pk_tload_byte_offset)

   `evl_param_key(pk_tload_use_tmask)
   `evl_param_key(pk_tload_tensor_mask)
   `evl_param_key(pk_tload_active_req_count)         // actual active/outstanding L2 reqs count
   `evl_param_key(pk_tload_resp_line_count)          // actual L2 resp_data line count
   `evl_param_key(pk_tload_resp_line_count_max)

   `evl_param_key(pk_tstore_scp_line_id)       // TStore L1SCP actual line_id
   `evl_param_key(pk_tenb_expect_restart)      // TL_TenB expect "restart_b" (replay) 

   `evl_param_key(pk_uc_resp)
   `evl_param_key(pk_capacity_evict)
   `evl_param_key(pk_tlb_req)
   `evl_param_key(pk_soft_locked)
   `evl_param_key(pk_gsc32_idx_lo)       // gsc32_idx is [39:0] but integer is only 32-bit (hence we need 2 parts)
   `evl_param_key(pk_gsc32_idx_hi)
   `evl_param_key(pk_insn_length)
   `evl_param_key(pk_bus_err)
   `evl_param_key(pk_coop_tstore)
   `evl_param_key(pk_mh_assigned)      // [1:0]: [0]=MH0 or [1]=MH1
   `evl_param_key(pk_mh_accepted)      // [1:0]: [0]=MH0 or [1]=MH1

   `evl_param_key(pk_dcache_cmd)        // for RTL dcache_cmd (enum type), different from our own trans_desc evl_cmd_t;
   `evl_param_key(pk_access_type)       // meaning access_size
   `evl_param_key(pk_dcache_cmd_is_write)
   `evl_param_key(pk_ps_mask)
   `evl_param_key(pk_mem_global)
   `evl_param_key(pk_dest_reg)          // instruction dest_register (5-bit; 0~31) 
   `evl_param_key(pk_dest_is_fp)        // instruction dest_is_floating_point (1-bit) 

   `evl_param_key(pk_cop_first_line)    // CacheOp: it's first line (of multiple lines)
   `evl_param_key(pk_cop_end_line)      // CacheOp: it's last (end) line (of multiple lines)
   `evl_param_key(pk_cop_actual_rep_cnt)   // CacheOp: actual repeat count (multiple lines) 
   `evl_param_key(pk_scp_way)           // for VPU_SCP_Read
   `evl_param_key(pk_scp_addr)          // for VPU_SCP_Read
   `evl_param_key(pk_scp_size64)          // for VPU_SCP_Read
   `evl_param_key(pk_scp_size256)          // for VPU_SCP_Read

   `evl_param_key(pk_treduce_send)
   `evl_param_key(pk_access_esr_region)
   `evl_param_key(pk_addr_msb_err)      // address overflow


   `include "dv/common/base/evl_mem_map_pk_inc.svh"

   localparam TENSOR_REDUCE_MAX_PARTNER_ID = 7;
   localparam TENSOR_REDUCE_MAX_TREE_DEPTH = 2;
   
   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_minion_rtl_port                   m_minion_rtl_port;

   // port for receiving observed requests to the neighbourhood
   uvm_analysis_export   #(evl_req_desc) m_neigh_miss_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_neigh_miss_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_neigh_evict_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_neigh_evict_req_fifo;
   uvm_analysis_export   #(evl_ptw_desc) m_neigh_ptw_req_export;
   uvm_tlm_analysis_fifo #(evl_ptw_desc) m_neigh_ptw_req_fifo;

   // port for receiving observed responses from the neighbourhood
   uvm_analysis_export   #(evl_req_desc) m_neigh_miss_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_neigh_miss_rsp_fifo;
   uvm_analysis_export   #(evl_req_desc) m_neigh_evict_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_neigh_evict_rsp_fifo;
   uvm_analysis_export   #(evl_ptw_desc) m_neigh_ptw_rsp_export;
   uvm_tlm_analysis_fifo #(evl_ptw_desc) m_neigh_ptw_rsp_fifo;

   // Templates to create objects from
   evl_line_data     m_template_req_data_neigh;
   evl_line_desc     m_template_line_desc_neigh;
   evl_trans_desc    m_template_trans_desc_neigh;
   evl_req_desc      m_template_req_desc_neigh;
   evl_trans_desc    m_template_trans_desc;

   evl_trans_desc    m_s1_queue[$];
   evl_trans_desc    m_s2_queue[$];
   evl_trans_desc    m_replay_queue[int];
   evl_trans_desc    m_amo_cas_s3_queue[$];      // Atomic_CAS S3 tage (for Cmp_data)

   evl_trans_desc    m_cacheop_s0_queue[$];      // CacheOp (S0: line up with CoreReq and ReplayReq)
   evl_trans_desc    m_cacheop_s1_queue[$];      // CacheOp_FSM (RTL);
   evl_trans_desc    m_cacheop_l2_s1_queue[$];   // CacheOp_L2_FSM (RTL)
   evl_trans_desc    m_cacheop_s2_queue[$];

   evl_trans_desc    m_tload_wait_queue[2][$];   // TL waiting ... q[0] for TL_L1SCP, q[1] for TL_TenB
   evl_trans_desc    m_tload_s1_queue[2][$];     // 2 independent TensorLoad FSMs: TL0(matrixA into L1SCP) and TL1(matrixB into TenB_RF)
   evl_trans_desc    m_tload_s2_queue[2][$];
   evl_trans_desc    m_mh_tag_update_q[$];

   evl_trans_desc    m_vpu_scp_read_q[$];

   evl_trans_desc    m_reduce_wait_queue[$];     // TensorReduce waiting while (vpu_reduce_wait==1) 
   evl_trans_desc    m_reduce_queue[$];          // actually only one outstanding TensorReduce (per minion)
   evl_trans_desc    m_reduce_recv_spawn_pend[$];  // as receiver, spawn-off, waiting for data from sender;
   evl_req_desc      m_reduce_send_data_pend[$];   // as sender (for outgoing reduce data on L2_Evict_Req)
   evl_trans_desc    m_reduce_recv_data_retn[$]; // as receiver: received data from sender and ready to return to VPU ;

   evl_trans_desc    m_tstore_s1_queue[$];       // Tensor_Store: add S1 stage
   evl_trans_desc    m_tstore_scp_s1_queue[$];   // Tensor_Store_SCP: add S1 stage
   evl_trans_desc    m_tstore_vpu_reg_queue[$];  // Tensor_Store waiting for VPU Reg data  // Or: could reuse m_reduce_queue[] for TensorStore

   evl_trans_desc    m_s3_core_resp_queue[$];   
   evl_trans_desc    m_core_bus_err_queue[$];   
   evl_trans_desc    m_s3_vpu_tenb_data_queue[$];
   int               m_check_s3_vpu_tenb_data_cnt;   // count of how many s3_vpu_tenb_data have been checked (starts from 0)   

   evl_req_desc      m_pending_miss_reqs[$];
   evl_req_desc      m_pending_evict_reqs[$];
   evl_ptw_desc      m_pending_ptw_reqs[$];

   evl_req_desc      m_active_miss_reqs[$];
   evl_req_desc      m_active_evict_reqs[$];
   evl_ptw_desc      m_active_ptw_reqs[$];

   evl_trans_desc    m_pma_or_misalign_fault_q[$];


   // for Uncached or Atomic:
   //evl_line_data     m_uc_l2_retn_line_data [2][$];  // one queue per Thread; could be possible multiple return_line_data for each Thread

   // Tensor_Load_L1SCP (excluding TenB): max only one outstanding? 
   // (Not really: No_Transform could have multiple outstanding and overlapping!!!)
   evl_trans_desc    m_tload_scp_outstanding_q[$];
   evl_line_data     m_tload_resp_line_data [64];  // Interleave needs max [4], Transpose needs max [64];

   // Tensor_Load_TenB: max only one outstanding? (let's assume it could be more than one)
   evl_trans_desc    m_tload_tenb_outstanding_q[$];
   // the latest Tensor_Load_TenB:
   evl_trans_desc    m_latest_tload_tenb;

   //
   // Cache objects to store action tables and cache state.
   // We have two of those, because the dcache can operate in split mode,
   // where each thread has some sets reserved to it, essentially having
   // its own cache
   // When the cache is in shared mode, only m_cache[0] is used for both
   // threads. When in split or SCP mode, a given thread uses m_cache[thread_id]
   //
   evl_minion_dcache #(CACHE_TYPE) m_cache[2];
   evl_cache                       m_scp;             // not being actually used -> see "m_l1_scp" below 
   bit [511:0]                     m_l1_scp [4][12];  // L1SCP: 12 sets, 4 ways, 64-byte cline

   // FUTURE: VM support: some support has already been done; need to be verified! 
   evl_tlb              m_tlb;
   evl_page_table_entry m_template_tlb_entry;

   bit [1:0]  m_fence        = 2'b00;       // Fence ongoing (per thread)
   bit [1:0]  m_tlb_ptw_in_process = 2'b0;  // TLB miss causing Pipe being flushed and PTW in process (per thread)
   event      ev_s1_tlb_flush_pipeline [2]; // event to model RTL signal s1_tlb_flush_pipeline (1-cyc pulse); per thread; 
   bit        m_scp_en       = 1'b0;        // dcache is in scratchpad mode
   bit        m_split_en     = 1'b0;        // dcache is in split mode
   bit [16:0] m_reduce_ready = 17'h0;       // Reduce partner is ready to receive data

   int        m_cacheop_bw_rate = 0;           // CacheOp_BandWidth_RepRate 
   int        m_cacheop_bw_max  = 0;           // CacheOp_BandWidth_Max
   int        m_cacheop_bw_rate_cyc = 1;       // in cycles; 

   int        m_cacheop_bw_ctrl_count = 0;        // 
   int        m_cacheop_bw_ctrl_first_cyc_q [$];  // need a Queue (because CacheOp_L2 Req streams could possibly overlap)
   int        m_cacheop_bw_ctrl_end_cyc;          // for the end (last one) of a CacheOp_L2_Req stream 

   int        m_num_sets     = 16;
   int        m_num_ways     = 4;
   int        m_threads_split_offset   [`CORE_NR_THREADS] = { 0,  14 };  // Split mode: set_or_index: T0={0,1,2,..7}, T1={14,15} 
   int        m_threads_split_num_sets [`CORE_NR_THREADS] = { 8,  2  };
   int        m_threads_scp_offset     [`CORE_NR_THREADS] = { 12, 14 };  // SCP mode: set_or_index: T0={12,13}, T1={14,15}; 
   int        m_threads_scp_num_sets   [`CORE_NR_THREADS] = { 2,  2  };

   evl_paddr_t  m_cache_set_mask;
   evl_paddr_t  m_thread_set_mask_split [`CORE_NR_THREADS];
   evl_paddr_t  m_thread_set_mask_scp   [`CORE_NR_THREADS];

   mailbox #(evl_cache_control) m_cache_ctrl_req_mbox;
   mailbox #(evl_cache_control) m_cache_ctrl_done_mbox;
   event                        m_cache_ctrl_start_zero;
   event                        m_cache_ctrl_done;
   logic                        m_cache_ctrl_change_in_prog = 1'b0;
   bit                          m_cache_cfg_first_after_reset = 1'b1;   // initialized to 1

   event                        m_tenb_flush_done;
   int                          m_restart_tenb_pend = 0;

   evl_esr_mprot_t              m_mprot_esr; // FUTURE: Emmanuel Marie use RAL when available
          // typedef struct packed {
          //    logic enable_secure_memory;
          //    logic [1:0] dram_size;
          //    logic disable_osbox_access;
          //    logic disable_pcie_access;
          //    logic [1:0] io_access_mode;
          // } esr_mprot_t;

   bit                          m_bypass;    // 

   //
   // Reduce parameters:  max only one outstanding Reduce or TStore;
   //
   semaphore   m_reduce_lock      = new(1);
   int         m_reduce_regs      = -1;
   int         m_reduce_ready_idx = -1;
   evl_xword_t m_reduce_data;
   int         m_reduce_partner   = -1;     // (CYE/Charles) added 9/03/2019
   evl_dcache_reduce_req_t   m_tensor_reduce_req_rtl_info;      // CYE 

   // TensorStore: max only one outstanding TStore or Reduce;
   int         m_tstore_cols      = -1;
   int         m_tstore_regs      = -1;
   int         m_tstore_reg_idx   = -1;
   int         m_tstore_req_idx   = -1;
   int         m_tstore_num_lines = -1;
   evl_xword_t m_tstore_data;
   evl_vaddr_t m_tstore_stride    = 0;


   // ioshire indicate Service Processor:
   bit        in_ioshire = 1'b0;

   bit        m_allow_bus_err = 1'b0;
   bit        m_en_s3_core_resp_chk = 1'b0;
   bit        m_en_s3_vpu_tenb_data_chk = 1'b0;
   bit        m_en_s3_vpu_scp_data_chk = 1'b0;
   bit        m_en_vpu_scp_read_conflict_tload_scp_chk = 1'b0;
   bit        m_en_tstore_scp_conflict_tload_scp_chk = 1'b0;
   bit        m_rtl_s3_core_data_ignore_x = 1'b0;
   bit        m_en_chk_cacheop_bw_rate_n_max = 1'b0;
   bit        m_en_chk_l1scp_mode_when_vpu_read_scp = 1'b0;

   // about MissHandler stall L2_Resp:
   bit [1:0]  m_mh_l2_resp_stall;                 // stall state 
   event      event_mh_l2_resp_stall_clear [2];   // event to clear stall  

   // TensorErrorCSR:
   bit [`TENSOR_ERROR_SZ-1:0]  m_TensorError_CSR [2];

   // TensorFMA flags: for RTLMIN-6078 and VERIF-3508:
   bit        m_tfma_req_but_not_proc_en = 1'b0;    // TFMA Req arrives but not yet start processing/enable 

   // enable amocmpexch_fix (VERIF-3520):
   bit        m_amocmpexch_fix_en = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_minion_id
   //
   //  Returns the full ID of that Minion
   //
   function int get_minion_id();
      return (get_shire_id() << 5) + (get_neighborhood_id() << 3) + (get_core_id());
   endfunction: get_minion_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id_from_minion_id
   //
   //  Returns the full ID of that Minion
   //
   function int get_shire_id_from_minion_id(input int minion_id);
      return (minion_id >> 5);
   endfunction: get_shire_id_from_minion_id


   //-----------------------------------------------------------------------------------------------
   task watch_for_bus_requests();
      watch_for_neigh_miss_requests();
      watch_for_neigh_evict_requests();
      watch_for_neigh_ptw_requests();
      watch_for_neigh_miss_responses();
      watch_for_neigh_evict_responses();
      watch_for_neigh_ptw_responses();
   endtask : watch_for_bus_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_ptw_requests
   //
   task watch_for_neigh_ptw_requests();
      evl_ptw_desc rcv_req;
      evl_ptw_desc exp_ptw_req;
      fork
         forever begin
            m_neigh_ptw_req_fifo.get(rcv_req);

            if (m_pending_ptw_reqs.size() == 0) begin
               sb_error($sformatf("Observed a PTW request but none was expected (%0s)", rcv_req.sprint_rsp()), rcv_req, {}, "", `__FILE__, `__LINE__);
            end
            else begin
               exp_ptw_req = m_pending_ptw_reqs.pop_front();
               // rcv_req.compare_attrs(exp_ptw_req); // FUTURE: Emmanuel Marie
            end
            m_active_ptw_reqs.push_back(rcv_req);
         end
      join_none
   endtask : watch_for_neigh_ptw_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_ptw_responses
   //
   task watch_for_neigh_ptw_responses();
      evl_ptw_desc rcv_rsp;
      fork
         forever begin
            m_neigh_ptw_rsp_fifo.get(rcv_rsp);

            if (m_active_ptw_reqs.size() == 0) begin
               sb_error($sformatf("Observed a PTW response but none was expected (%0s)", rcv_rsp.sprint_rsp()), rcv_rsp, {}, "", `__FILE__, `__LINE__);
            end
            else begin
               evl_ptw_desc pending_ptw_req;
               pending_ptw_req = m_active_ptw_reqs.pop_front();
               pending_ptw_req.set_paddr(rcv_rsp.get_paddr());
               pending_ptw_req.set_rsp_pte(rcv_rsp.get_rsp_pte());
               if (rcv_rsp.has_access_fault() == 1'b1) pending_ptw_req.set_access_fault();
               process_ptw_response(pending_ptw_req);
            end
         end
      join_none
   endtask : watch_for_neigh_ptw_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_evict_requests
   //
   // This task monitors the requests observed by the ETLink EVICT interface between the core and
   // the neighbourhood, and checks that they were expected and correctly formed
   //
   task watch_for_neigh_evict_requests();
      evl_req_desc   rcv_req_desc;
      evl_req_desc   src_req_desc;
      evl_trans_desc trans_desc;

      fork
         forever begin
            m_neigh_evict_req_fifo.get(rcv_req_desc);

            if (rcv_req_desc != null) begin
               process_predicted_request(rcv_req_desc, m_pending_evict_reqs, m_active_evict_reqs, 1'b0, 1'b1);

               //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), m_active_evict_reqs[$], $sformatf("(CYE DEBUG) watch_for_neigh_evict_requests: m_active_evict_reqs[last]: %s cmd=%0p",  m_active_evict_reqs[$].sprint_obj(), m_active_evict_reqs[$].get_cmd() ))
            
               // (CYE) check CacheOp_Max and CacheOp_RepRate:
               // only if this new EVICT Req whose parent is CacheOp_L2 (EvictVA, FlushVA, PrefVA, TLOAD_L2SCP);
               // note: exclude WriteBack Req caused by EvictVA or FlushVA;

               // for the last entry of m_active_evict_reqs[] which has parent_desc:
               if (m_active_evict_reqs[$].get_parent_desc() != null) begin 
                  evl_mem_desc    parent_mem_desc;
                  evl_trans_desc  parent_trans_desc;

                  parent_mem_desc = m_active_evict_reqs[$].get_parent_desc(); // get_parent() returns "mem_desc" type, but actual parent is "trans_desc"
                  if ($cast(parent_trans_desc, parent_mem_desc) == 0) begin
                     sb_error($sformatf("$cast(parent_trans_desc, parent_mem_desc) failed ..."), null, {}, "", `__FILE__, `__LINE__);
                  end

                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_trans_desc, $sformatf("(CYE DEBUG) watch_for_neigh_evict_requests: m_active_evict_reqs[last]: parent_trans_desc: %s cmd=%0p",  parent_trans_desc.sprint_obj(), parent_trans_desc.get_cmd() ))

                  // note: exclude WriteBack Req caused by EvictVA or FlushVA;
                  if (/*((m_active_evict_reqs[$].get_bus_cmd() inside {ET_LINK_REQ_Evict, ET_LINK_REQ_Flush}) &&
                       (m_active_evict_reqs[$].get_bus_cop_dest_level() > 1)) ||*/
                      ((m_active_evict_reqs[$].get_bus_cmd() == ET_LINK_REQ_Prefetch) && 
                       (m_active_evict_reqs[$].get_bus_cop_dest_level() > 0)) ||
                      ( m_active_evict_reqs[$].get_bus_cmd() == ET_LINK_REQ_ScpFill ) ) 
                  begin
                     // only interested in multi-cline stream: ???
                     if ((/*((parent_trans_desc.get_cmd() inside {CACHE_EVICT, CACHE_FLUSH}) &&
                           (parent_trans_desc.get_bus_cop_dest_level() > 1)) ||*/
                          ((parent_trans_desc.get_cmd() == CACHE_PREFETCH) &&
                           (parent_trans_desc.get_bus_cop_dest_level() > 0)) ||   // CacheOp_Pref_L2L3
                          ( parent_trans_desc.get_cmd() == CACHE_SCP_FILL ) )     // Tensor_Load_L2SCP (CacheOp_Pref_L2SCP)
                         &&
                         ~((parent_trans_desc.get_int_param(pk_cop_first_line, 0) == 1) &&
                           (parent_trans_desc.get_int_param(pk_cop_end_line, 0) == 1))      // exclude single-cline case 
                     )
                     begin
                        // if it's the first_line of the above CacheOp (multiple_lines): reset the count and cycles:
                        if (parent_trans_desc.get_int_param(pk_cop_first_line, 0) == 1) begin
                           m_cacheop_bw_ctrl_count = 0;
                           `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) going out on Evict Intf: it is first_line: reset m_cacheop_bw_ctrl_count=0"))

                          /**** // change: to match RTL beavior: move code below to function process_request(); 
                           //m_cacheop_bw_ctrl_first_cyc_q.push_back(m_minion_rtl_port.get_cycle_count());
                           //`evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) going out on Evict Intf: it is first_line: m_cacheop_bw_ctrl_first_cyc_q[]=%0d", m_cacheop_bw_ctrl_first_cyc_q[$]))
                          ****/
                        end

                        // if it's a multi-cline stream of L2_Reqs and it's the last (end) of the stream: check average RepRate;
                        if ((parent_trans_desc.get_int_param(pk_cop_end_line, 0) == 1) && 
                            (parent_trans_desc.get_int_param(pk_cop_actual_rep_cnt, 0) > 1)) 
                        begin
                           int  cop_actual_rep_cnt = parent_trans_desc.get_int_param(pk_cop_actual_rep_cnt, 0);
                           shortreal  average_rate_cyc;   // real RTL rate will not be a whole number in most cases;
                           int  my_cacheop_bw_ctrl_first_cyc; 

                           // simulation cycles for the "end (last one)" of multi-cline stream:
                           m_cacheop_bw_ctrl_end_cyc = m_minion_rtl_port.get_cycle_count();
                           `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) going out on Evict Intf: it is last_cline: m_cacheop_bw_ctrl_end_cyc=%0d", m_cacheop_bw_ctrl_end_cyc))

                           // note: "first_cyc" and "end_cyc" have to be 1-to-1 pairing.
                           my_cacheop_bw_ctrl_first_cyc = m_cacheop_bw_ctrl_first_cyc_q.pop_front();
                       
                           // Note: If the pk_cop_actual_rep_cnt is too small (like < 5): the average rate could be of too much variation ...
                           //       Let's only check when (pk_cop_actual_rep_cnt >= 5).
                           // Note: now we change to use new method to determine "first_cyc" which better match RTL behavior, 
                           //       so we can check smaller cop_actual_rep_cnt cases;
                           // Note: When CacheOp_RepRate and CacheOp_Max get changed in the middle of CacheOp_L2 request stream,
                           //       we set my_cacheop_bw_ctrl_first_cyc to (-1), and we skip the checker;
                           if ((cop_actual_rep_cnt > 1) && (m_cacheop_bw_rate > 0) && (my_cacheop_bw_ctrl_first_cyc > 0)) begin
                              // note: integer division result truncates any fractional part; 
                              average_rate_cyc = (m_cacheop_bw_ctrl_end_cyc - my_cacheop_bw_ctrl_first_cyc) / (cop_actual_rep_cnt - 1);
                              //if ((m_cacheop_bw_ctrl_end_cyc - my_cacheop_bw_ctrl_first_cyc) % (cop_actual_rep_cnt - 1) > 0) begin 
                              //   average_rate_cyc += 1;
                              //end

                              // note: there could be some +/-1 cycle margin compare with actual RTL behavior;
                              // note: now we change to use new method to determine "first_cyc" which better match RTL behavior, so probably no need of "margin";
                              if (average_rate_cyc < m_cacheop_bw_rate_cyc) begin
                                 if (m_en_chk_cacheop_bw_rate_n_max == 1'b1) begin
                                    sb_error($sformatf("m_cacheop_bw_ctrl: rate violation: rtl_rate_cyc=%0.2f exp_rate_cyc=%0d", average_rate_cyc, m_cacheop_bw_rate_cyc), null, {}, "", `__FILE__, `__LINE__);
                                 end else begin
                                    `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE WARNING) m_cacheop_bw_ctrl: rate violation: rtl_rate_cyc=%0.2f exp_rate_cyc=%0d", average_rate_cyc, m_cacheop_bw_rate_cyc))
                                 end
                              end
                              // print more info:
                              `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) m_cacheop_bw_ctrl: m_cacheop_bw_ctrl_first_cyc=%0d m_cacheop_bw_ctrl_end_cyc=%0d cop_actual_rep_cnt=%0d average_rate_cyc=%0.2f", my_cacheop_bw_ctrl_first_cyc, m_cacheop_bw_ctrl_end_cyc, cop_actual_rep_cnt, average_rate_cyc))
                           end
                        end // if ((pk_cop_end_line == 1) && (pk_cop_actual_rep_cnt > 1))


                        // increment number of outstanding CacheOp_L2 (PrefVA, TLOAD_L2SCP): exclude {EvictVA, FlushVA};
                        m_cacheop_bw_ctrl_count++;
                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) going out on Evict Intf: incremented m_cacheop_bw_ctrl_count=%0d", m_cacheop_bw_ctrl_count))

                        if (m_cacheop_bw_max > 0) begin
                           if (m_cacheop_bw_ctrl_count > m_cacheop_bw_max) begin
                              if (m_en_chk_cacheop_bw_rate_n_max == 1'b1) begin
                                 sb_error($sformatf("m_cacheop_bw_ctrl_count=%0d is more than m_cacheop_bw_max=%0d", m_cacheop_bw_ctrl_count, m_cacheop_bw_max), null, {}, "", `__FILE__, `__LINE__);
                              end else begin
                                 `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE WARNING) m_cacheop_bw_ctrl_count=%0d is more than m_cacheop_bw_max=%0d", m_cacheop_bw_ctrl_count, m_cacheop_bw_max))
                              end
                           end
                        end
                     end
                  end

               end  // if (m_active_evict_reqs[$].get_parent_desc() != null) begin 

               if (rcv_req_desc.get_bus_cmd() == ET_LINK_REQ_MsgSendData) begin
                  void'(m_active_evict_reqs.pop_back());
                  src_req_desc = rcv_req_desc.get_parent_req_desc();
                  trans_desc   = rcv_req_desc.get_root_trans_desc();
                  if (rcv_req_desc != null) rcv_req_desc.set_done();
                  if (src_req_desc != null) src_req_desc.set_done();
                  delete_active_desc(trans_desc);
               end
            end
         end
      join_none
   endtask : watch_for_neigh_evict_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_miss_requests
   //
   // This task monitors the requests observed by the ETLink MISS interface between the core and
   // the neighbourhood, and checks that they were expected and correctly formed
   //
   task watch_for_neigh_miss_requests();
      evl_req_desc rcv_req_desc;
      fork
         forever begin
            m_neigh_miss_req_fifo.get(rcv_req_desc);

            if (rcv_req_desc != null) begin
               process_predicted_request(rcv_req_desc, m_pending_miss_reqs, m_active_miss_reqs, 1'b0, 1'b1);
            end
         end
      join_none
   endtask : watch_for_neigh_miss_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_miss_responses
   //
   // This task monitors the responses observed by the ETLink MISS interface between the core and
   // the neighbourhood, and acts on the internal state of the model accordingly.
   //
   task watch_for_neigh_miss_responses();
      evl_req_desc rcv_req_desc;
      fork
         forever begin
            m_neigh_miss_rsp_fifo.get(rcv_req_desc);

            if (rcv_req_desc != null) begin
               evl_req_desc src_req_desc;
               bit found;

               src_req_desc = rcv_req_desc.get_parent_req_desc();
               if (src_req_desc == null) begin
                  src_req_desc = rcv_req_desc;
               end

               found = 1'b0;
               foreach (m_active_miss_reqs[ii]) begin
                  if (m_active_miss_reqs[ii] == src_req_desc) begin
                     evl_trans_desc trans_desc;
                     m_active_miss_reqs.delete(ii);
                     found = 1'b1;

                     // (Charles) consider mh_l2_resp_stall case: only for Cached (CLine ReFill):
                     if ((rcv_req_desc.get_bus_rsp_id() == 5'h2) && (m_mh_l2_resp_stall[0] == 1'b1)) begin  // MissHandler_0 (Cached)
                        `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("received bus_resp %s: for MissHandler_0: but m_mh_l2_resp_stall[0]==1: will wait for event_mh_l2_resp_stall_clear[0]", rcv_req_desc.sprint_obj() ))
                        fork
                           process_bus_resp_stalled(rcv_req_desc);    // OR: inside task: fork off a thread
                        join_none
                     end 
                     else if ((rcv_req_desc.get_bus_rsp_id() == 5'h3) && (m_mh_l2_resp_stall[1] == 1'b1)) begin  // MissHandler_1 (Cached)
                        `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("received bus_resp %s: for MissHandler_1: but m_mh_l2_resp_stall[1]==1: will wait for event_mh_l2_resp_stall_clear[1]", rcv_req_desc.sprint_obj() ))
                        fork
                           process_bus_resp_stalled(rcv_req_desc);    // OR: inside task: fork off a thread
                        join_none
                     end
                     else begin
                        process_bus_response(rcv_req_desc);
                     end

                     trans_desc = src_req_desc.get_root_trans_desc();
                     break;
                  end
               end
               if (!found) begin
                  sb_error($sformatf("no response with ID 0x%0x expected (%0s)", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_rsp()), rcv_req_desc, {}, "", `__FILE__, `__LINE__);
               end
            end
         end
      join_none
   endtask : watch_for_neigh_miss_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_neigh_evict_responses
   //
   // This task monitors the responses observed by the ETLink EVICT interface between the core and
   // the neighbourhood, and acts on the internal state of the model accordingly.
   //
   task watch_for_neigh_evict_responses();
      evl_req_desc rcv_req_desc;
      fork
         forever begin
            m_neigh_evict_rsp_fifo.get(rcv_req_desc);

            if (rcv_req_desc != null) begin
               evl_req_desc src_req_desc;
               bit found;

               if ((rcv_req_desc.get_bus_rsp_type() == ET_LINK_RSP_MsgRcvData) || (rcv_req_desc.get_bus_cmd() == ET_LINK_REQ_MsgSendData)) begin
                  process_rcv_msg(rcv_req_desc);
                  continue;
               end

               src_req_desc = rcv_req_desc.get_parent_req_desc();
               if (src_req_desc == null) begin
                  src_req_desc = rcv_req_desc;
               end

               //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), src_req_desc, $sformatf("(CYE DEBUG) watch_for_neigh_evict_responses: src_req_desc: %s cmd=%0p",  src_req_desc.sprint_obj(), src_req_desc.get_cmd() ))

               // (CYE) CacheControl: ucache_control: for RepRate and Max check:
               // only if this src EVICT Req whose parent is CacheOp_L2 (EvictVA, FlushVA, PrefVA, TLOAD_L2SCP);
               // Note: should we better move this code below into function process_bus_response() ??? ... FUTURE 

               if (src_req_desc.get_parent_desc() != null) begin 
                  evl_mem_desc    parent_mem_desc;
                  evl_trans_desc  parent_trans_desc;

                  parent_mem_desc = src_req_desc.get_parent_desc();   // get_parent() returns "mem_desc" type, but actual parent is "trans_desc" 
                  if ($cast(parent_trans_desc, parent_mem_desc) == 0) begin
                     sb_error($sformatf("$cast(parent_trans_desc, parent_mem_desc) failed ..."), null, {}, "", `__FILE__, `__LINE__);
                  end

                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_trans_desc, $sformatf("(CYE DEBUG) watch_for_neigh_evict_responses: src_req_desc: parent_trans_desc: %s cmd=%0p",  parent_trans_desc.sprint_obj(), parent_trans_desc.get_cmd() ))

                  // decrement m_cacheop_bw_ctrl_count: only for CacheOp_L2 (EvictVA, FlushVA, PrefVA, TLoad_L2SCP):
                  // note: exclude WriteBack Req caused by EvictVA or FlushVA;

                  if (/*((src_req_desc.get_bus_cmd() inside {ET_LINK_REQ_Evict, ET_LINK_REQ_Flush}) &&
                       (src_req_desc.get_bus_cop_dest_level() > 1)) ||*/
                      ((src_req_desc.get_bus_cmd() == ET_LINK_REQ_Prefetch) && 
                       (src_req_desc.get_bus_cop_dest_level() > 0)) ||
                      ( src_req_desc.get_bus_cmd() == ET_LINK_REQ_ScpFill ) )                  
                  begin
                     if (/*((parent_trans_desc.get_cmd() inside {CACHE_EVICT, CACHE_FLUSH}) &&
                          (parent_trans_desc.get_bus_cop_dest_level() > 1)) ||*/
                         ((parent_trans_desc.get_cmd() == CACHE_PREFETCH) &&
                          (parent_trans_desc.get_bus_cop_dest_level() > 0)) ||
                         ( parent_trans_desc.get_cmd() == CACHE_SCP_FILL ) )     // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP) 
                     begin
                        // note: need to exclude the end/last one of the stream: bcoz it could overlap with the beginning of a subsequent stream;
                        if (parent_trans_desc.get_int_param(pk_cop_end_line, 0) != 1) begin 
                           m_cacheop_bw_ctrl_count--;
                           `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) got Resp from ETLink Evict Intf: decremented m_cacheop_bw_ctrl_count=%0d", m_cacheop_bw_ctrl_count))
                        end

                        //if (m_cacheop_bw_ctrl_count == 0) begin
                        //   m_cacheop_bw_ctrl_first_cyc = 0;
                        //   `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP) got Resp from ETLink Evict Intf: because now m_cacheop_bw_ctrl_count=0: should also clear m_cacheop_bw_ctrl_first_cyc=0"))
                        //end
                     end
                  end
               end  // if (src_req_desc.get_parent_desc() != null) begin 


               found = 1'b0;
               foreach (m_active_evict_reqs[ii]) begin
                  if (m_active_evict_reqs[ii] == src_req_desc) begin
                     evl_trans_desc trans_desc;
                     m_active_evict_reqs.delete(ii);
                     found = 1'b1;
                     process_bus_response(rcv_req_desc);
                     trans_desc = src_req_desc.get_root_trans_desc();
                     break;
                  end
               end
               if (!found) begin
                  sb_error($sformatf("no response with ID 0x%0x expected (%0s)", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_rsp()), rcv_req_desc, {}, "", `__FILE__, `__LINE__);
               end
            end
         end
      join_none
   endtask : watch_for_neigh_evict_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_rtl_events
   //
   //    This task gets events coming from the RTL monitor and calls the appropriate routine.
   //
   task watch_for_rtl_events();
      evl_minion_packet rtl_event;

      forever begin
         evl_minion_packet_u packet_info;

         m_minion_rtl_port.get_event_minion(rtl_event);
         packet_info = rtl_event.get_info();

         case (rtl_event.get_packet_type())
            MINION_DCACHE_MW_REQ        : process_tag_update(packet_info.mw_req.info);
            MINION_DCACHE_REQ           : process_intpipe_req(packet_info.intpipe_req.info);   // S0
            MINION_DCACHE_REPLAY        : process_replay_req(packet_info.replay_req.info);     // S0
            MINION_DCACHE_S1_INFO       : process_s1_req(packet_info.s1_info.info);
            MINION_DCACHE_S2_INFO       : process_s2_req(packet_info.s2_info.info);
            MINION_DCACHE_S3_CORE_X31   : process_s3_core_x31(packet_info.s3_info.info);

            MINION_CACHEOP_REQ          : process_cop_req(packet_info.cop_req.info);
            MINION_CACHEOP_S0           : process_s0_cop(packet_info.cop_s0.info);
            MINION_CACHEOP_S1           : process_s1_cop(packet_info.cop_s1.info);    // (CYE/Charles) for CacheOp_FSM 
            MINION_CACHEOP_L2_S1        : process_s1_cop_l2(packet_info.cop_s1.info); // (CYE/Charles) for CacheOp_L2 only
            MINION_CACHEOP_S2           : process_s2_cop(packet_info.cop_s2.info);

            MINION_TENSOR_LOAD_REQ_CYC0 : process_tensor_load_cyc0(packet_info.tload_req.info);  // TL first cycle 
            MINION_TENSOR_LOAD_REQ      : process_tensor_load(packet_info.tload_req.info);  // include TLoad_L1SCP (all transforms) and TLoad_TenB

          //MINION_TENSOR_RESTART_B     : restart_tenb();           // change to process_restart_tenb
          //MINION_TENSOR_TENB_FLUSH    : ->m_tenb_flush_done;      // change to process_tenb_flush
            MINION_TENSOR_RESTART_B     : process_restart_tenb();
            MINION_TENSOR_TENB_FLUSH    : process_tenb_flush(); 

            MINION_TLOAD_S1_INFO        : process_tload_s1(packet_info.tload_s1s2.info);
            MINION_TLOAD_S2_INFO        : process_tload_s2(packet_info.tload_s1s2.info);
            MINION_TENSOR_STORE_REQ     : process_tensor_store(packet_info.tstore_req.info);
            MINION_TSTORE_S1_INFO       : parse_tstore_s1(packet_info.tstore_s1.info); 
            MINION_VPU_REG_SEND         : process_vpu_reg_send(packet_info.vpu_reg_send.info.data);
            MINION_TENSOR_STORE_SCP_REQ : process_tensor_store_scp(packet_info.tstore_scp_req.info);
            MINION_TENSOR_REDUCE_REQ    : process_tensor_reduce(packet_info.reduce_req.info);       // (CYE) first cycle
            MINION_TENSOR_REDUCE_REQ_AFT_WAIT    : process_tensor_reduce_aft_wait(packet_info.reduce_req.info);   // (CYE) after vpu_reduce_wait

            MINION_VPU_SCP_REQ           : process_vpu_scp_req(packet_info.vpu_scp_req.info);
            MINION_REDUCE_S2_INST_OP_VLD : process_reduce_recv_data_retn();    // ? Do we need any more RTL info

            MINION_CACHE_CTRL_REQ       : process_cache_ctrl_req(packet_info.cache_ctrl.info);   // initial cache_ctrl request packet 
            MINION_CACHE_CTRL_START_ZERO: ->m_cache_ctrl_start_zero;
            MINION_CACHE_CTRL_DONE      : process_cache_ctrl_done(packet_info.cache_ctrl.info);   // second (final) cache_ctrl packet (when "Done")

            MINION_ESR_INFO             : process_esr_info(packet_info.esr_info.info);
            MINION_TEX_CTRL             : `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Minion SB received a Texture request event"))
            MINION_FENCE                : process_fence(evl_thread_id_t'(packet_info.fence.thread_id));
            MINION_CANCEL_FENCE         : begin 
                                             m_fence[evl_thread_id_t'(packet_info.fence.thread_id)] = 1'b0;
                                             //`evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(thread %0d) RTL IntPipe asserted id_fe_req_valid[]=1: cancel fence: clear m_fence[]=0", packet_info.fence.thread_id))
                                          end
            MINION_IN_IOSHIRE           : begin
                                             in_ioshire = packet_info.is_ioshire.in_ioshire;     // should be just once
                                             `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Minion L1SB received an event (from RTL) in_ioshire=1"))
                                          end

            MINION_MH_L2_RESP_STALL_START: process_mh_l2_resp_stall_start(packet_info.mh_l2_resp_stall);
            MINION_MH_L2_RESP_STALL_CLEAR: process_mh_l2_resp_stall_clear(packet_info.mh_l2_resp_stall); 

            MINION_CSR_TENS_ERR_UPD     : process_csr_tens_err_upd(packet_info.csr_tens_err_upd.info);
            MINION_DCACHE_S3_CORE_RESP  : check_s3_core_resp(packet_info.s3_core_resp.info);
            MINION_S3_VPU_TENB_DATA     : check_s3_vpu_tenb_data(packet_info.s3_vpu_tenb_data.info);

            MINION_DCACHE_S3_EXPT_FAULT : check_s3_addr_when_expt_fault(packet_info.s3_core_resp.info);
            MINION_DCACHE_CORE_BUS_ERR  : check_core_bus_err(packet_info.s3_core_resp.info);
            MINION_VPU_SCP_DATA         : check_s3_vpu_scp_data(packet_info.vpu_scp_data.info);

            MINION_TFMA_REQ             : process_tfma_req(packet_info.tfma_ctrl.info);
            MINION_TFMA_PROC_EN         : process_tfma_proc_en();

            MINION_END_OF_SIM           : process_end_of_sim_check(packet_info.end_of_sim.info);

            default                     : sb_error($sformatf("Received an unknown RTL event!"), null, {}, "", `__FILE__, `__LINE__);
         endcase
      end
   endtask : watch_for_rtl_events




   //----------------------------------------------
   // Function:  process_vpu_scp_req
   //
   function void process_vpu_scp_req (input evl_minion_vpu_scp_req_t rtl_info);
      evl_trans_desc  trans_desc;
      bit [1:0]       vpu_read_scp_way;
      int             vpu_read_scp_set;
      int             vpu_read_scp_line;
      string          extra_info = "";

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_trans_id();
      trans_desc.set_cmd(VPU_SCP_READ);
      trans_desc.set_paddr(rtl_info.addr);
      trans_desc.set_int_param(pk_scp_way, rtl_info.way);
      trans_desc.set_int_param(pk_scp_addr, rtl_info.addr);   // only addr[9:0]
      trans_desc.set_int_param(pk_scp_size64, rtl_info.size64);
      trans_desc.set_int_param(pk_scp_size256, rtl_info.size256);

      vpu_read_scp_way = rtl_info.way; 
      vpu_read_scp_set = rtl_info.addr[9:6]; 
      vpu_read_scp_line = (vpu_read_scp_way * 12) + vpu_read_scp_set;

      extra_info = $sformatf("(scp_way=%0d scp_addr=0x%3h scp_set=%0d scp_line=%0d)(size64=%0b size256=%0b)", vpu_read_scp_way, rtl_info.addr, vpu_read_scp_set, vpu_read_scp_line, rtl_info.size64, rtl_info.size256);    

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received (RTL) s1_vpu_scp_req_read event: %0s%0s", trans_desc.sprint_obj(), extra_info))

      if (m_scp_en == 1'b0) begin
         if (m_en_chk_l1scp_mode_when_vpu_read_scp == 1'b0) begin
            // special case: certain TensorQuant transformations don't need to read L1SCP, but VPU 
            // RTL still sends VPU_SCP_READ Req anyway (due to timing reason): so don't flag error;  
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("for the above new incoming VPU_SCP_READ: but L1D$ is NOT in Scratchpad mode: ignore it"))  
         end else begin
            // It's an ERROR case in most csaes; see RTLMIN-6244 or DOC-137;
            sb_error($sformatf("for the above new incoming VPU_SCP_READ: but L1D$ is NOT in Scratchpad mode: something wrong"), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
         return;
      end

      // (CYE) check VPU_SCP_Read conflict with on-going TensorLoad_L1SCP (for overlap range of SCP_lines):
      // check against outstanding/ongoing m_tload_scp_outstanding_q[]: for overlap range of L1SCP entry: if match found, error;
      if (m_tload_scp_outstanding_q.size() > 0) begin
         evl_trans_desc  my_tload_scp;
         int  tload_scp_line_start;
         int  tload_scp_num_lines;     // max=16
         int  tload_scp_line_end;
         bit  conflict = 1'b0;

         foreach(m_tload_scp_outstanding_q[ii]) begin
            my_tload_scp = m_tload_scp_outstanding_q[ii];
            tload_scp_line_start = my_tload_scp.get_int_param(pk_tload_scp_dest);
            tload_scp_line_start = tload_scp_line_start % 48;     // max=48
            tload_scp_num_lines = my_tload_scp.get_int_param(pk_tload_num_lines);   // max=16
            tload_scp_line_end = tload_scp_line_start + tload_scp_num_lines - 1;      // inclusive
            tload_scp_line_end = tload_scp_line_end % 48;     // max=48
   
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), my_tload_scp, $sformatf("on-going TensorLoad_L1SCP: %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end))
   
            // figure out whether vpu_read_scp_line is within range of {tload_scp_line_start, tload_scp_line_end};
            if (tload_scp_line_end >= tload_scp_line_start) begin   // "end" is no wraparound
               if ((vpu_read_scp_line >= tload_scp_line_start) && (vpu_read_scp_line <= tload_scp_line_end)) begin
                  conflict = 1'b1;
               end
            end 
            else begin   // "end" is wraparound
               if (vpu_read_scp_line >= tload_scp_line_start) begin
                  conflict = 1'b1;
               end
               else begin  // if (vpu_read_scp_line < tload_scp_line_start)
                  if (((vpu_read_scp_line+48) >= tload_scp_line_start) && ((vpu_read_scp_line+48) <= (tload_scp_line_end+48))) begin
                     conflict = 1'b1;
                  end
               end 
            end
            if (conflict == 1'b1) begin
               if (m_en_vpu_scp_read_conflict_tload_scp_chk == 1'b1) begin
                  sb_error($sformatf("s1_vpu_scp_req_read (above): its scp_line=%0d conflict with on-going TensorLoad_L1SCP %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", vpu_read_scp_line, my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end), trans_desc, {}, "", `__FILE__, `__LINE__);
               end
               else begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) s1_vpu_scp_req_read (above): its scp_line=%0d conflict with on-going TensorLoad_L1SCP %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", vpu_read_scp_line, my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end))
               end
            end
         end  // foreach(m_tload_scp_outstanding_q[ii])
      end // if (m_tload_scp_outstanding_q.size() > 0)

      m_vpu_scp_read_q.push_back(trans_desc);

   endfunction: process_vpu_scp_req



   //----------------------------------------------
   // Function: process_csr_tens_err_upd: RTL update CSR_TensorError_Reg, also cross-check L1SB against RTL on current value
   //
   function void process_csr_tens_err_upd (input evl_minion_csr_tens_err_upd_t rtl_info);
      // note: exclude CSR_TensorError bit[3](FCC ovfl) and bit[6](TenB & FMA pair) and bit[8](TSt Cfg Err) ... (not detected in DCache) ?

      if (rtl_info.upd_en[0] === 1'b1) begin  // Thread 0
         // cross-check on current value:
       
         //if ( (rtl_info.cur_val_t0.tload[2:0] != m_TensorError_CSR[0][2:0]) ||
         //   /*(rtl_info.cur_val_t0.fcc        != m_TensorError_CSR[0][3]) ||*/   // exclude bit[3] (FCC overflow)
         //     (rtl_info.cur_val_t0.scp_unset  != m_TensorError_CSR[0][4]) ||
         //     (rtl_info.cur_val_t0.co         != m_TensorError_CSR[0][5]) ||
         //   /*(rtl_info.cur_val_t0.tenb       != m_TensorError_CSR[0][6]) ||*/   // exclude bit[6] (illegal TenB & FMA pairing)
         //     (rtl_info.cur_val_t0.mem_fault  != m_TensorError_CSR[0][7]) ||
         //   /*(rtl_info.cur_val_t0.tstore     != m_TensorError_CSR[0][8]) ||*/   // exclude bit[8] (TensorSt config invalid)
         //     (rtl_info.cur_val_t0.treduce    != m_TensorError_CSR[0][9]) )
         //begin 
         //   //sb_error($sformatf("when RTL update CSR_TensorError_T0: cur_val mismatch (exclude FCC bit[3][6][8]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val_t0), null, {}, "", `__FILE__, `__LINE__); 
         //   `evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) when RTL update CSR_TensorError_T0: cur_val mismatch (exclude FCC bit[3][6][8]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val_t0))
         //end
         //else begin
         //   `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("when RTL update CSR_TensorError_T0: cur_val match (exclude FCC bit[3][6][8]): L1SB=RTL=0x%h", m_TensorError_CSR[0]))
         //end

         if ( ((rtl_info.cur_val[0] & 10'b10_1010_0111) != (m_TensorError_CSR[0] & 10'b10_1010_0111)) || // exclude bit[3[6][8] (handle [4] separate)
              ((m_TensorError_CSR[0][4] == 1'b1) && (rtl_info.cur_val[0][4] == 1'b0)) )                  // [4] is special: csr_file also set [4] 
         begin 
            sb_error($sformatf("when RTL update CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6])[8]: L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val[0]), null, {}, "", `__FILE__, `__LINE__); 
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) when RTL update CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6])[8]: L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val[0]))
         end
         else begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("when RTL update CSR_TensorError_T0: cur_val match (exclude bit[3][6])[8]: L1SB=RTL=0x%h", m_TensorError_CSR[0]))
         end

         // update with new_val:
         m_TensorError_CSR[0][9:0] = rtl_info.new_val[9:0];
      end  // if (rtl_info.upd_en[0] === 1'b1)

      if (rtl_info.upd_en[1] === 1'b1) begin  // Thread 1
         // cross-check on current value: only compare bit [4][5][7]:

         //if ( (rtl_info.cur_val_t1.scp_unset  != m_TensorError_CSR[1][4]) ||
         //     (rtl_info.cur_val_t1.co         != m_TensorError_CSR[1][5]) ||
         //     (rtl_info.cur_val_t1.mem_fault  != m_TensorError_CSR[1][7]) ) 
         //begin 
         //   //sb_error($sformatf("when RTL update CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=4'b%4b RTL=4'b%4b", {m_TensorError_CSR[1][7],m_TensorError_CSR[1][5],m_TensorError_CSR[1][4],m_TensorError_CSR[1][3]}, rtl_info.cur_val_t1), null, {}, "", `__FILE__, `__LINE__); 
         //   `evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) when RTL update CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=4'b%4b RTL=4'b%4b", {m_TensorError_CSR[1][7],m_TensorError_CSR[1][5],m_TensorError_CSR[1][4],m_TensorError_CSR[1][3]}, rtl_info.cur_val_t1))
         //end
         //else begin
         //   `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("when RTL update CSR_TensorError_T1: cur_val match (exclude bit[3]): L1SB=RTL=4'b%4b", rtl_info.cur_val_t1))
         //end

         // cross-check on current value: actually only bit[4][5][7] should be valid:  exclude bit[3] FCC_Overflow;
         if ( ((rtl_info.cur_val[1] & 10'b11_1110_0111) != (m_TensorError_CSR[1] & 10'b11_1110_0111)) || // exclude bit[3] (handle [4] separate) 
              ((m_TensorError_CSR[1][4] == 1'b1) && (rtl_info.cur_val[1][4] == 1'b0)) )                  // [4] is special: csr_file also set [4] 
         begin 
            sb_error($sformatf("when RTL update CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[1], rtl_info.cur_val[1]), null, {}, "", `__FILE__, `__LINE__); 
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) when RTL update CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[1], rtl_info.cur_val[1]))
         end
         else begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("when RTL update CSR_TensorError_T1: cur_val match (exclude bit[3]): L1SB=RTL=0x%h", m_TensorError_CSR[1]))
         end

         // update with new_val:
         m_TensorError_CSR[1][4] = rtl_info.new_val[4];
         m_TensorError_CSR[1][5] = rtl_info.new_val[5];
         m_TensorError_CSR[1][7] = rtl_info.new_val[7];
      end  // if (rtl_info.upd_en[1] === 1'b1)

   endfunction: process_csr_tens_err_upd


   //
   // Function: check_s3_vpu_scp_data
   //
   function void check_s3_vpu_scp_data (input evl_minion_vpu_scp_data_t rtl_info);
      evl_trans_desc  trans_desc;
      bit [1:0]   scp_way;
      bit [9:0]   scp_addr;
      bit [9:6]   scp_set;
      bit         scp_size64;
      bit         scp_size256;
      string      extra_info;
      bit [511:0]  exp_data_64byte;
      bit [255:0]  exp_data_32byte;
      bit [127:0]  exp_data_16byte;
      bit  [63:0]  exp_data_8byte;
      bit  [31:0]  exp_data_4byte;
      bit  [63:0]  rtl_data_8byte;
      bit  [31:0]  rtl_data_4byte;
      string       scp_info = "";

      scp_info = (m_scp_en == 1'b0)? ": but L1D$ is NOT in Scratchpad mode: ignore it" : "";
      `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Received (RTL) s3_vpu_scp_data event (S3): scp_data[255:0]=0x%h %s",  rtl_info.scp_data, scp_info))

      // ignore it when (m_scp_en == 1'b0):
      if ((m_en_s3_vpu_scp_data_chk == 1'b0) || (m_scp_en == 1'b0))  begin
         return;
      end

      if (m_vpu_scp_read_q.size() == 0) begin
         sb_error("Received an unexpected s3_vpu_scp_data event: m_vpu_scp_read_q is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      trans_desc = m_vpu_scp_read_q.pop_front();
      scp_way = trans_desc.get_int_param(pk_scp_way);
      scp_addr = trans_desc.get_int_param(pk_scp_addr);
      scp_set  = scp_addr[9:6];
      scp_size64 = trans_desc.get_int_param(pk_scp_size64);
      scp_size256 = trans_desc.get_int_param(pk_scp_size256);
      extra_info = $sformatf("(way=%0d addr[9:0]=0x%h set=%0d size64=%0b size256=%0b)", scp_way, scp_addr, scp_set, scp_size64, scp_size256);
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("original vpu_scp_read_req (from m_vpu_scp_read_q[]): %s %s", trans_desc.sprint_obj(), extra_info ))

      // read from m_l1_scp[]: 
      exp_data_64byte = m_l1_scp[scp_way][scp_set];
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("exp_data_64byte (from m_l1_scp[]): 0x%h", exp_data_64byte))

      // cross-check data against RTL: based on (size64, size256):
      if (scp_size256 == 1'b1) begin  // 256-bits
         exp_data_32byte = scp_addr[5]?  exp_data_64byte[511:256] : exp_data_64byte[255:0];
         if (rtl_info.scp_data[255:0] == exp_data_32byte) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_vpu_scp_data: scp_data[255:0] match: RTL=L1SB=0x%h", exp_data_32byte))
         end else begin
            sb_error($sformatf("(S3) check_s3_vpu_scp_data: scp_data[255:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.scp_data, exp_data_32byte), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end
      else if (scp_size64 == 1'b1) begin   // 64-bits
         exp_data_8byte = exp_data_64byte[(scp_addr[5:3])*64 +: 64];
         rtl_data_8byte = rtl_info.scp_data[(scp_addr[4:3])*64 +: 64]; 
         if (rtl_data_8byte == exp_data_8byte) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_vpu_scp_data: scp_data[63:0] match: RTL=L1SB=0x%h", exp_data_8byte))
         end else begin
            sb_error($sformatf("(S3) check_s3_vpu_scp_data: scp_data[[63:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_data_8byte, exp_data_8byte), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end
      else begin  // 32-bits 
         exp_data_4byte = exp_data_64byte[(scp_addr[5:2])*32 +: 32];
         rtl_data_4byte = rtl_info.scp_data[(scp_addr[4:2])*32 +: 32]; 
         if (rtl_data_4byte == exp_data_4byte) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_vpu_scp_data: scp_data[31:0] match: RTL=L1SB=0x%h", exp_data_4byte))
         end else begin
            sb_error($sformatf("(S3) check_s3_vpu_scp_data: scp_data[[31:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_data_4byte, exp_data_4byte), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end

   endfunction: check_s3_vpu_scp_data


   //
   // Function: check_s3_vpu_tenb_data: (note: it's 256-bit, half a cline size)
   //
   function void check_s3_vpu_tenb_data (input evl_dcache_s3_vpu_tenb_data_t rtl_info);
      bit [3:0]  tenb_line  = rtl_info.tenb_resp.tenb_line; 
      bit        tenb_flush = rtl_info.tenb_resp.tenb_flush;
      evl_trans_desc  trans_desc;
      bit [511:0]  exp_data_64byte;
      bit [255:0]  exp_data_32byte;
      int          exp_tenb_line;   // line_id

      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Received (RTL) s3_vpu_tenb_data event: fill_is_tenb=1: tenb_data[255:0]=0x%h tenb_line[3:0]=%0d tenb_flush=%0b",  rtl_info.tenb_data, rtl_info.tenb_resp.tenb_line, rtl_info.tenb_resp.tenb_flush))

      if (m_en_s3_vpu_tenb_data_chk == 1'b0) begin
         return;
      end

      if (m_s3_vpu_tenb_data_queue.size() == 0) begin
         sb_error("Received an unexpected s3_vpu_tenb_data event: m_s3_vpu_tenb_data_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      // note: s3_vpu_tenb_data is 256-bit (half-a-cline); 
      // so, need two s3_vpu_tenb_data packets for one entire cline (each TensorLoad_TenB spawn-off trans);
      // note: m_check_s3_vpu_tenb_data_cnt starts from 0;

      if ((m_check_s3_vpu_tenb_data_cnt % 2) == 0) begin     // first s3_vpu_tenb_data
         trans_desc = m_s3_vpu_tenb_data_queue[0];
      end else begin                                         // second s3_vpu_tenb_data
         trans_desc = m_s3_vpu_tenb_data_queue.pop_front();
      end

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3): (individual) TensorLoad_TenB from m_s3_vpu_tenb_data_queue[] is: %s; its m_read_data is as below:", trans_desc.sprint_obj() ))
      if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         trans_desc.get_read_data().print_obj(get_abstract_name(), trans_desc.get_paddr());
      end

      exp_data_64byte = trans_desc.get_read_data().get_xword(0);
      if ((m_check_s3_vpu_tenb_data_cnt % 2) == 0) begin  // first-half-cline  
         exp_data_32byte = exp_data_64byte[255:0];
      end else begin                                      // second-half-cline
         exp_data_32byte = exp_data_64byte[511:256];
      end

      // cross-check tenb_data[255:0] against RTL:
      if (rtl_info.tenb_data == exp_data_32byte) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3): check_s3_vpu_tenb_data: tenb_data[255:0] match: RTL=L1SB=0x%h", exp_data_32byte))
      end else begin
         sb_error($sformatf("(S3) check_s3_vpu_tenb_data: tenb_data[255:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.tenb_data, exp_data_32byte), trans_desc, {}, "", `__FILE__, `__LINE__);
      end

      // cross-check tenb_line[3:0] against RTL:
      exp_tenb_line = trans_desc.get_int_param(pk_tload_tenb_line_id);
      if (rtl_info.tenb_resp.tenb_line == exp_tenb_line) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3): check_s3_vpu_tenb_data: tenb_line match: RTL=L1SB=%0d", exp_tenb_line)) 
      end else begin
         sb_error($sformatf("(S3) check_s3_vpu_tenb_data: tenb_line mismatch: RTL=%0d L1SB=%0d", rtl_info.tenb_resp.tenb_line, exp_tenb_line), trans_desc, {}, "", `__FILE__, `__LINE__);
      end
 
      // FUTURE: more ...

      m_check_s3_vpu_tenb_data_cnt++;

   endfunction: check_s3_vpu_tenb_data



   //
   //  check_s3_core_resp: L1SB cross-check against RTL
   //  note: temp: ignore sign-extension for now;
   //
   function void check_s3_core_resp (input evl_dcache_s3_core_resp_t rtl_info);
      evl_trans_desc   trans_desc;
      evl_trans_desc   misaligned_2nd_desc;
      bit              thread_id;
      bit [4:0]        dest_reg;
      bit              dest_is_fp;
      evl_vaddr_t      orig_vaddr;         // [47:0]
      evl_paddr_t      orig_paddr;         // [39:0]
      evl_dcache_cmd   dcache_cmd;
      evl_dcache_type  access_type;        // mean access_size
      bit    [ 7:0]    ps_mask;            // Packed-Single mask
      bit              is_intpipe_req = 1'b0;
      bit              is_treduce_recv = 1'b0;
      string           intpipe_extra_info = "";

      evl_line_data    exp_read_data;
      evl_line_data    exp_read_data_2nd_cline;

      bit  [39:0]      gsc32_idx;          // total 40 bits for all 8 index (each is [4:0] for each word) 
      bit  [31:0]      gsc32_idx_lo;
      bit  [31:0]      gsc32_idx_hi;
      bit [7:0][4:0]   gsc32_indices;      // gsc32_idx[4:0] for each word
      bit   [4:0]      gsc32_block_offset;

      evl_paddr_t      aligned_addr;       // 8-byte-aligned  // [39:0]
      bit              cross_64byt = 0;    // load bytes cross cline 64-byte
      evl_paddr_t      nxt_cline_pa;       // [39:0]
      bit              cross_page = 0;     // default 4K page
      evl_vaddr_t      orig_va;            // = ld_trans.dbg_va;  [47:0]
      evl_vaddr_t      converted_va; 
      evl_vaddr_t      nxt_page_va;
      evl_paddr_t      nxt_page_pa;        // [39:0]

      logic [511:0]    mem_data_64byte;     // 64-byte data from L1D$ model 
      logic [511:0]    mem_data_nxt_64byte; // next 64-byte data from L1D$ model (when across 64-byte)
      logic [1023:0]   mem_data_128byte;    // = {mem_data_nxt_64byte, mem_data_64byte} 

      logic [255:0]    exp_data_32byte;
      logic [127:0]    exp_data_qword;      // expected quad-word (including the double-word);
      logic  [63:0]    exp_data_dword;      // expected double-word

      bit   [255:0]    rtl_data_ignore_x;   // "bit" type with only 0 or 1;
      logic [255:0]    rtl_data_32byte;     // might have "x"

      bit                s3_type_is_gsc = 1'b0;   // Operation is Gather/Scatter
      bit                s3_is_gather32 = 1'b0;   // Operation is a gather on 32-byte block
      logic [255:0]      s3_data_gather32;     // Data muxed for gathers on 32-byte block
      logic [255:0]      s3_data_bypassed;     // Data after store bypass 
      logic [7:0][31:0]  s3_data_word;         // Load data for gather word on a 32-byte block
      logic [7:0][15:0]  s3_data_half;         // Load data for gather half on a 32-byte block
      logic [7:0][ 7:0]  s3_data_byte;         // Load data for gather byte on a 32-byte block

      logic  [31:0]    byte_mask;
      logic [255:0]    bit_mask;

      bit              bypass_this_checker = 1'b0;  // show mismatch as WARNING instead of ERROR (not being used)

      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Received (RTL) s3_core_resp event: data[255:0]=0x%h addr[48:0]=0x%h size_type[3:0]=%p thread=%0d dest_fp=%0b dest_reg=%0d ...",  rtl_info.s3_core_resp.data, rtl_info.s3_core_resp.addr, rtl_info.s3_core_resp.typ, rtl_info.s3_core_resp.dest.thread_id, rtl_info.s3_core_resp.dest.fp, rtl_info.s3_core_resp.dest.addr[4:0] ))

      if (m_en_s3_core_resp_chk == 1'b0) begin
         return;
      end

      trans_desc = m_s3_core_resp_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected s3_core_resp event: m_s3_core_resp_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      thread_id = trans_desc.get_thread_id();
      orig_vaddr = trans_desc.get_vaddr();
      orig_paddr = trans_desc.get_paddr();
      dcache_cmd = evl_dcache_cmd'(trans_desc.get_int_param(pk_dcache_cmd)); 
      access_type = evl_dcache_type'(trans_desc.get_int_param(pk_access_type));
      ps_mask = trans_desc.get_int_param(pk_ps_mask);
      dest_reg = trans_desc.get_int_param(pk_dest_reg);
      dest_is_fp = trans_desc.get_int_param(pk_dest_is_fp);

      s3_is_gather32 = (dcache_cmd == dcache_cmd_G32);
      s3_type_is_gsc = (access_type == dcache_type_PS_GS8) || (access_type == dcache_type_PS_GS16) || (access_type == dcache_type_PS_GS32);
      is_treduce_recv = (trans_desc.get_cmd() == TENSOR_REDUCE) && (trans_desc.get_int_param(pk_treduce_send) == 0);
      is_intpipe_req = (trans_desc.get_int_param(pk_intpipe_req) == 1);  // correct?

      //if (is_treduce_recv == 1'b0) begin
      if (is_intpipe_req == 1'b1) begin
         intpipe_extra_info = $sformatf(" (%p %p)", dcache_cmd, access_type);
      end

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: orig trans_desc from m_s3_core_resp_queue: %s%s (is_intpipe_req=%0b is_treduce_recv=%0b)", trans_desc.sprint_obj(), intpipe_extra_info, is_intpipe_req, is_treduce_recv))

      if (dcache_cmd == dcache_cmd_G32) begin
         gsc32_idx_lo = trans_desc.get_int_param(pk_gsc32_idx_lo);
         gsc32_idx_hi = trans_desc.get_int_param(pk_gsc32_idx_hi);
         gsc32_idx    = {gsc32_idx_hi[7:0], gsc32_idx_lo[31:0]}; 
      end

      // cross-check against RTL:

      // compare thread_id:
      if (rtl_info.s3_core_resp.dest.thread_id == thread_id) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: thread_id match: L1SB=RTL=%0d", thread_id ))
      end 
      else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore thread_id;
            sb_error($sformatf("(S3) check_s3_core_resp: thread_id mismatch: RTL=%0d L1SB=%0d", rtl_info.s3_core_resp.dest.thread_id, thread_id), trans_desc, {}, "", `__FILE__, `__LINE__);
      end

      // compare dest.fp 
      if (rtl_info.s3_core_resp.dest.fp == dest_is_fp) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: dest_fp match: L1SB=RTL=%0b", dest_is_fp ))
      end 
      else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore dest_fp;
            sb_error($sformatf("(S3) check_s3_core_resp: dest_fp mismatch: RTL=%0b L1SB=%0b", rtl_info.s3_core_resp.dest.fp, dest_is_fp), trans_desc, {}, "", `__FILE__, `__LINE__);
      end

      // compare dest.addr[4:0] (reg_id) 
      if (rtl_info.s3_core_resp.dest.addr[4:0] == dest_reg) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: dest_reg match: L1SB=RTL=%0d", dest_reg ))
      end 
      else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore dest_reg;
            sb_error($sformatf("(S3) check_s3_core_resp: dest_reg mismatch: RTL=%0d L1SB=%0d", rtl_info.s3_core_resp.dest.addr[4:0], dest_reg), trans_desc, {}, "", `__FILE__, `__LINE__);
      end


      if (rtl_info.s3_core_resp.typ[3:0] == trans_desc.get_int_param(pk_access_type)) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: access_type match: L1SB=RTL=%p", rtl_info.s3_core_resp.typ ))
      end 
      else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore access_type;
         // for Gather32: RTL always return s3_core_resp.typ[3:0] as dcache_type_PS: regardless original core_req; ??? Yes (see Jira VERIF-3348)
         if (dcache_cmd == dcache_cmd_G32) begin   // known issue: ask Xavier -> see Jira VERIF-3348 
            if (rtl_info.s3_core_resp.typ[3:0] == dcache_type_PS) begin    // dcache_type_PS = 4'hb
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("dcache_cmd_G32: (S3) check_s3_core_resp: access_type match: L1SB=RTL=%p (dcache_type_PS)", rtl_info.s3_core_resp.typ ))
            end else begin
               sb_error($sformatf("dcache_cmd_G32: (S3) check_s3_core_resp: access_type mismatch: RTL=0x%h L1SB=0x%h(dcache_type_PS)", rtl_info.s3_core_resp.typ, dcache_type_PS), trans_desc, {}, "", `__FILE__, `__LINE__);
            end
         end else begin 
            sb_error($sformatf("(S3) check_s3_core_resp: access_type mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.typ, access_type), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end


    /****  // Note: s3_core_resp.addr is only for Dcache exception cases, NOT for valid data return when s3_core_resp_valid==1.
      // compare addr[]: use paddr[39:0]   // whether paddr or vaddr?
      if (rtl_info.s3_core_resp.addr[39:0] == trans_desc.get_paddr()) begin 
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: paddr[39:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.addr[39:0] ))
      end 
      else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore paddr;
         // in case of misaligned access: L1D$ splits it into two sub_reqs internally; but returns only one s3_core_resp;
         // and resp_addr could be a little different from req_addr (on lower 8-bit);
         if (trans_desc.have_object_param(pk_misaligned_2nd) == 1) begin  // known issue: ask Xavier -> see Jira VERIF-3341
            bit [39:0]  misalign_exp_paddr;

            // expected addr is from the 2nd-pass (if both passes have gone down Pipeline), plus the original bit[4:0]; (see Jira VERIF-3341)
            $cast(misaligned_2nd_desc, trans_desc.get_object_param(pk_misaligned_2nd));
            misalign_exp_paddr = misaligned_2nd_desc.get_paddr();
            misalign_exp_paddr[(`EVL_OWORD_ADDR_LSB-1):0] = orig_paddr[(`EVL_OWORD_ADDR_LSB-1):0];   // bit[4:0]

            if (rtl_info.s3_core_resp.addr[39:0] == misalign_exp_paddr) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("misaligned access: (S3) check_s3_core_resp: paddr[39:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.addr[39:0] ))
            end else begin
               sb_error($sformatf("misaligned access: (S3) check_s3_core_resp: paddr[39:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.addr[39:0], misalign_exp_paddr), trans_desc, {}, "", `__FILE__, `__LINE__);
            end 
         end else begin 
            sb_error($sformatf("(S3) check_s3_core_resp: paddr[39:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.addr[39:0], trans_desc.get_paddr()), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end
    *****/

      // compare ps_mask[7:0]: only for dcache_cmd=dcache_cmd_G32 and/or access_type=dcache_type_PS; 
      if ((dcache_cmd == dcache_cmd_G32) || (access_type == dcache_type_PS)) begin
         if (rtl_info.s3_core_resp.ps_mask == ps_mask) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: ps_mask[7:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.ps_mask[7:0] ))
         end 
         else if (is_treduce_recv == 1'b0) begin   // for reduce_recv: ignore ps_mask;
            sb_error($sformatf("(S3) check_s3_core_resp: ps_mask[7:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.ps_mask, ps_mask), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end

      // now compare data: based on size_type and paddr[4:0] ... 

      //exp_read_data = trans_desc.m_read_data;
      exp_read_data = trans_desc.get_read_data();
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: exp_read_data is as below:")) 
      if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         exp_read_data.print_obj(get_abstract_name(), trans_desc.get_paddr());
      end
      mem_data_64byte = exp_read_data.get_xword(0);
      mem_data_nxt_64byte = 512'h0;

      if (trans_desc.get_int_param(pk_this_is_misaligned_access) == 1) begin
         // need both clines (2x64=128 bytes):
         $cast(misaligned_2nd_desc, trans_desc.get_object_param(pk_misaligned_2nd));
         //exp_read_data_2nd_cline = misaligned_2nd_desc.m_read_data;
         exp_read_data_2nd_cline = misaligned_2nd_desc.get_read_data();
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: misaligned_2nd: exp_read_data_2nd_cline is as below:")) 
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            exp_read_data_2nd_cline.print_obj(get_abstract_name(), misaligned_2nd_desc.get_paddr());
         end
         mem_data_nxt_64byte = exp_read_data_2nd_cline.get_xword(0);
      end

      mem_data_128byte = {mem_data_nxt_64byte, mem_data_64byte};

      // From RTL code & comment:
      // Return data: 64 LSB are the extended ones, for core accesses. 
      // The remaining bits are the original merged bits for vector accesses.
      // As a result, for a vector access, s3_data_merged is the output to s3_load_data.
      // Two muxes are differentiated given that gather32 data will never have to go to BA.
      // s3_load_data_mux1 = { s3_data_merged[`DCACHE_DATA_SIZE-1:`XREG_SIZE], s3_data_extended };
      // s3_load_data_mux2 = s3_is_gather32 ? s3_data_gather32 : s3_load_data_mux1;
      // s3_core_resp.data = s3_load_data_mux2;

      // special: for dcache_cmd_G32 (Gather32): data format is very different: 
      if (dcache_cmd == dcache_cmd_G32) begin
         // based on paddr[5]: choose which 32-byte (256-bit):
         exp_data_32byte[255:0] = (orig_paddr[5] == 1'b0)? mem_data_64byte[255:0] : mem_data_64byte[511:256]; 

         case (access_type)
             dcache_type_PS_GS8  : gsc32_block_offset =  orig_paddr[4:0];
             dcache_type_PS_GS16 : gsc32_block_offset = {orig_paddr[4:1], 1'b0};
             dcache_type_PS_GS32 : gsc32_block_offset = {orig_paddr[4:2], 2'b0};
             default             : gsc32_block_offset = 'b0;
         endcase

         for (int i = 0; i < 8; i++) begin
            gsc32_indices[i] = 5'h0; 
            if (ps_mask[i]) begin
               case (access_type)
                   dcache_type_PS_GS8  : gsc32_indices[i] =  gsc32_idx[i*5 +: 5];
                   dcache_type_PS_GS16 : gsc32_indices[i] = {gsc32_idx[i*4 +: 4], 1'b0};
                   dcache_type_PS_GS32 : gsc32_indices[i] = {gsc32_idx[i*3 +: 3], 2'b0};
                   default             : gsc32_indices[i] = 'b0;
               endcase
               gsc32_indices[i] = gsc32_indices[i] + gsc32_block_offset;
            end
         end

         s3_data_bypassed = exp_data_32byte;
         for (int i = 0; i < 8; i++) begin
             s3_data_word[i] = s3_data_bypassed[gsc32_indices[i][4:2] * 32 +: 32];
             s3_data_half[i] = s3_data_bypassed[gsc32_indices[i][4:1] * 16 +: 16];
             s3_data_byte[i] = s3_data_bypassed[gsc32_indices[i]      *  8 +:  8];

             case (access_type)
                 dcache_type_PS_GS8  : s3_data_gather32[i*32 +: 32] = {24'h0, s3_data_byte[i]};
                 dcache_type_PS_GS16 : s3_data_gather32[i*32 +: 32] = {16'h0, s3_data_half[i]};
                 dcache_type_PS_GS32 : s3_data_gather32[i*32 +: 32] =         s3_data_word[i];
                 default             : s3_data_gather32[i*32 +: 32] = 'h0;
             endcase
         end

         exp_data_32byte = s3_data_gather32;
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: for dcache_cmd_G32 (Gather32): exp_data_32byte=0x%h", exp_data_32byte))

         // based on access_size_type: decide byte_mask (how many bytes/bits we need to compare):
         case (access_type) 
            dcache_type_PS_GS8:      // 8-bit for each word
               // consider ps_mask[7:0]: one bit for a word (4-byte);
               byte_mask = {{3'b0,{ps_mask[7]}}, {3'b0,{ps_mask[6]}}, {3'b0,{ps_mask[5]}}, {3'b0,{ps_mask[4]}},
                            {3'b0,{ps_mask[3]}}, {3'b0,{ps_mask[2]}}, {3'b0,{ps_mask[1]}}, {3'b0,{ps_mask[0]}}};
            dcache_type_PS_GS16:    // 16-bit for each word
               // consider ps_mask[7:0]: one bit for a word (4-byte);
               byte_mask = {{2'b0,{2{ps_mask[7]}}}, {2'b0,{2{ps_mask[6]}}}, {2'b0,{2{ps_mask[5]}}}, {2'b0,{2{ps_mask[4]}}},
                            {2'b0,{2{ps_mask[3]}}}, {2'b0,{2{ps_mask[2]}}}, {2'b0,{2{ps_mask[1]}}}, {2'b0,{2{ps_mask[0]}}}};
            dcache_type_PS_GS32:     // 32-bit for each word
               // consider ps_mask[7:0]: one bit for a word (4-byte);
               byte_mask = {{4{ps_mask[7]}}, {4{ps_mask[6]}}, {4{ps_mask[5]}}, {4{ps_mask[4]}},
                            {4{ps_mask[3]}}, {4{ps_mask[2]}}, {4{ps_mask[1]}}, {4{ps_mask[0]}}};
            default:  
               sb_error($sformatf("(S3) check_s3_core_resp: dcache_cmd_G32: access_type=0x%h is invalid", access_type), trans_desc, {}, "", `__FILE__, `__LINE__);
         endcase
 
      end  // dcache_cmd_G32

      else if (is_treduce_recv == 1'b1) begin   // special
         exp_data_32byte[255:0] = mem_data_64byte[255:0]; 
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: exp_data_32byte=0x%h", exp_data_32byte))

         byte_mask = 32'hffff_ffff;
      end

      // if not dcache_cmd_G32: based on vaddr[5:0], right-shift to least-significant-byte; and keep only 32-byte (256-bit):
      else begin
         exp_data_32byte[255:0] = mem_data_128byte >> (orig_paddr[5:0]*8);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: exp_data_32byte=0x%h", exp_data_32byte))

         // Return data: 64 LSB are the extended ones, for core accesses. 
         // The remaining bits [255:64] are the original merged bits, for vector accesses.

         // based on access_size_type: decide byte_mask (how many bytes/bits we need to compare):
         // note: ignore sign-extension for now;
         case (access_type) 
            dcache_type_B, dcache_type_BU:    // Byte (8-bit)
               byte_mask = 32'h0000_0001;
            dcache_type_H, dcache_type_HU:    // Half-Word (2-byte 16-bit)
               byte_mask = 32'h0000_0003;
            dcache_type_W, dcache_type_WU:    // Word (4-byte 32-bit)
               byte_mask = 32'h0000_000f;
            dcache_type_D:                    // Double-Word (8-byte 64-bit)
               byte_mask = 32'h0000_00ff;
            dcache_type_PS: begin             // 8-Word (32-byte 256-bit) 
               // consider ps_mask[7:0]: one bit for a word (4-byte):
               byte_mask = {{4{ps_mask[7]}}, {4{ps_mask[6]}}, {4{ps_mask[5]}}, {4{ps_mask[4]}},
                            {4{ps_mask[3]}}, {4{ps_mask[2]}}, {4{ps_mask[1]}}, {4{ps_mask[0]}}};
            end
            dcache_type_PS_BR:       // Broadcast 32b to eight 32b regs
               byte_mask = 32'h0000_000f;     // is this correct???
            dcache_type_PS_GS8:      // 8-bit for each word
               byte_mask = 32'h0000_0001;   //???
            dcache_type_PS_GS16:    // 16-bit for each word
               byte_mask = 32'h0000_0003;   //???
            dcache_type_PS_GS32:     // 32-bit for each word
               byte_mask = 32'h0000_000f;   //???
            default:  
               sb_error($sformatf("(S3) check_s3_core_resp: access_type=0x%h is invalid", access_type), trans_desc, {}, "", `__FILE__, `__LINE__);
         endcase 

      end // if (not dcache_cmd_G32)

      bit_mask[255:0] = {{8{byte_mask[31]}}, {8{byte_mask[30]}}, {8{byte_mask[29]}}, {8{byte_mask[28]}},
                         {8{byte_mask[27]}}, {8{byte_mask[26]}}, {8{byte_mask[25]}}, {8{byte_mask[24]}},
                         {8{byte_mask[23]}}, {8{byte_mask[22]}}, {8{byte_mask[21]}}, {8{byte_mask[20]}},
                         {8{byte_mask[19]}}, {8{byte_mask[18]}}, {8{byte_mask[17]}}, {8{byte_mask[16]}},
                         {8{byte_mask[15]}}, {8{byte_mask[14]}}, {8{byte_mask[13]}}, {8{byte_mask[12]}},
                         {8{byte_mask[11]}}, {8{byte_mask[10]}}, {8{byte_mask[9]}},  {8{byte_mask[8]}}, 
                         {8{byte_mask[7]}},  {8{byte_mask[6]}},  {8{byte_mask[5]}},  {8{byte_mask[4]}},
                         {8{byte_mask[3]}},  {8{byte_mask[2]}},  {8{byte_mask[1]}},  {8{byte_mask[0]}} };

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: byte_mask[31:0]=0x%h bit_mask[255:0]=0x%h", byte_mask, bit_mask))

      //// apply bit_mask on exp_data_32byte[]:
      //exp_data_32byte = exp_data_32byte & bit_mask;
      //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: after apply byte_mask (bit_mask): exp_data_32byte=0x%h", exp_data_32byte))

      // note: RTL data might contain "x"; our TB data is "bit" type with only "0 or 1";
      // note: for Read to ESR: treat RTL "x" as "0" (in order to match RTL value with TB expected;
      if ((trans_desc.get_int_param(pk_access_esr_region) == 1) || (m_rtl_s3_core_data_ignore_x == 1'b1)) begin
         rtl_data_ignore_x = rtl_info.s3_core_resp.data;    // logic (converted to) -> bit: ignore x
         rtl_data_32byte = rtl_data_ignore_x;  
      end else begin
         rtl_data_32byte = rtl_info.s3_core_resp.data;      // "logic" type might have "x";
      end

      // compare to RTL s3_core_resp.data: (apply byte_mask/bit_mask)
      if ((exp_data_32byte & bit_mask) == (rtl_data_32byte & bit_mask)) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_core_resp: after above byte_mask[31:0]=0x%h: data[255:0] match: L1SB=RTL=0x%h", byte_mask, (exp_data_32byte & bit_mask)))
      end 
      else begin
            sb_error($sformatf("(S3) check_s3_core_resp: after above byte_mask[31:0]=0x%h: data[255:0] mismatch: RTL=0x%h L1SB=0x%h", byte_mask, rtl_data_32byte, exp_data_32byte), trans_desc, {}, "", `__FILE__, `__LINE__); 
      end

      delete_active_desc(trans_desc); 

   endfunction: check_s3_core_resp


   //
   // check_s3_addr_when_expt_fault: L1SB cross-check against RTL
   //
   function void check_s3_addr_when_expt_fault (input evl_dcache_s3_core_resp_t rtl_info);
      evl_trans_desc   trans_desc;
      evl_vaddr_t      vaddr;
      bit              my_addr_msb_err;
      bit              exp_addr_overflow;

      trans_desc = m_pma_or_misalign_fault_q.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected s3_expt_fault event: m_pma_or_misalign_fault_q is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      // Note: s3_core_resp.addr[48] is an extra bit which means addr-overflow (or addr_msb_err).
      // Let's extract this info from our trans_desc model too:
      my_addr_msb_err = trans_desc.get_int_param(pk_addr_msb_err, 0);
      if (my_addr_msb_err == 1'b1) begin
         `evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("WARNING (S3) check_s3_addr_when_expt_fault: orig trans_desc %s: pk_addr_msb_err=1", trans_desc.sprint_obj()))
      end

      // check "s3_core_resp.addr[47:0]" (pure address part):
      if (trans_desc.get_int_param(pk_this_is_misaligned_2nd_part) == 1) begin // misaligned second-part
         evl_trans_desc  misaligned_1st_desc;  // first-part trans
         evl_vaddr_t  orig_addr;           // [47:0]
         evl_vaddr_t  misalign_exp_addr;   // [47:0]
            
         // expected addr is from the 2nd-part, plus the original bit[4:0]; (see Jira VERIF-3341)
         misalign_exp_addr = trans_desc.get_vaddr();
         $cast(misaligned_1st_desc, trans_desc.get_object_param(pk_misaligned_1st));
         orig_addr = misaligned_1st_desc.get_vaddr();
         misalign_exp_addr[(`EVL_OWORD_ADDR_LSB-1):0] = orig_addr[(`EVL_OWORD_ADDR_LSB-1):0];   // bit[4:0]

         if (rtl_info.s3_core_resp.addr[`VA_MSB:0] == misalign_exp_addr) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("misaligned access 2nd part: (S3) check_s3_addr_when_expt_fault: vaddr[47:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0]))
         end else begin
            sb_error($sformatf("misaligned access 2nd part: (S3) check_s3_addr_when_expt_fault: vaddr[47:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0], misalign_exp_addr), trans_desc, {}, "", `__FILE__, `__LINE__);
         end 
      end
      else if (trans_desc.get_int_param(pk_this_is_misaligned_access) == 1) begin  // misaligned first-part
         if (rtl_info.s3_core_resp.addr[`VA_MSB:0] == trans_desc.get_vaddr()) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("misaligned access 1st part: (S3) check_s3_addr_when_expt_fault: vaddr[47:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0]))
         end else begin
            sb_error($sformatf("misaligned access 1st part: (S3) check_s3_addr_when_expt_fault: vaddr[47:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0], trans_desc.get_vaddr()), trans_desc, {}, "", `__FILE__, `__LINE__);
         end 
      end
      else begin
         if (rtl_info.s3_core_resp.addr[`VA_MSB:0] == trans_desc.get_vaddr()) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_addr_when_expt_fault: vaddr[47:0] match: L1SB=RTL=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0]))
         end else begin
            sb_error($sformatf("(S3) check_s3_addr_when_expt_fault: vaddr[47:0] mismatch: RTL=0x%h L1SB=0x%h", rtl_info.s3_core_resp.addr[`VA_MSB:0], trans_desc.get_vaddr()), trans_desc, {}, "", `__FILE__, `__LINE__);
         end 
      end

      // check "s3_core_resp.addr[48]" bit (addr-overflow):
      // current RTL code: 
      //    s3_core_resp.addr[`VA_EXT_MSB] = s3_req.addr_msb_err ^ s3_req.addr[`VA_MSB];  // XOR; why?
      // So, should we try to match the RTL behavior in our checker (as below):
      vaddr = trans_desc.get_vaddr();
      exp_addr_overflow = my_addr_msb_err ^ vaddr[`VA_MSB];

      //if (rtl_info.s3_core_resp.addr[`VA_EXT_MSB] == my_addr_msb_err) begin
      if (rtl_info.s3_core_resp.addr[`VA_EXT_MSB] == exp_addr_overflow) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) check_s3_addr_when_expt_fault: addr-overflow (addr_msb_err) match: L1SB=RTL=%b", rtl_info.s3_core_resp.addr[`VA_EXT_MSB]))
      end else begin
         sb_error($sformatf("(S3) check_s3_addr_when_expt_fault: addr-overflow (addr_msb_err) mismatch: RTL=%b L1SB=%b", rtl_info.s3_core_resp.addr[`VA_EXT_MSB], exp_addr_overflow), trans_desc, {}, "", `__FILE__, `__LINE__);
      end

   endfunction: check_s3_addr_when_expt_fault



   //
   //  check_core_bus_err: L1SB cross-check against RTL: 
   //  note: only assert one bus_err at any cycle? Is it possible two threads have assert bus_err at a same cycle?
   //
   function void check_core_bus_err (input evl_dcache_core_bus_err_t rtl_info);
      evl_trans_desc   trans_desc;
      bit              orig_thread_id;
      evl_vaddr_t      orig_vaddr;         // [47:0]
      evl_paddr_t      orig_paddr;         // [39:0]
      evl_cmd_t        orig_trans_cmd;
      bit              orig_is_tensor_cacheop = 1'b0;  // original trans is Tensor or CacheOp
      bit              ignore_err_addr = 1'b0;

      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Received (RTL) core_bus_err event: bus_err[0]=%0b bus_err_addr[0]=0x%h  bus_err[1]=%0b bus_err_addr[1]=0x%h ...", rtl_info.bus_err[0], rtl_info.bus_err_addr[0], rtl_info.bus_err[1], rtl_info.bus_err_addr[1])) 

      trans_desc = m_core_bus_err_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected core_bus_err event: m_core_bus_err_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      orig_thread_id = trans_desc.get_thread_id();
      orig_vaddr = trans_desc.get_vaddr();
      orig_paddr = trans_desc.get_paddr();
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("check_core_bus_err: orig trans_desc from m_core_bus_err_queue: %s (thread_id=%0b vaddr=0x%h paddr=0x%h)", trans_desc.sprint_obj(), orig_thread_id, orig_vaddr, orig_paddr))

      // (Per Xavier) only Miss Handler reports the address that got the bus error.
      // For the other operations (Tensor and CacheOp) it was decided to do NOT store the addresses 
      // for all the outstanding operations that could get an error ...
      // (CYE 10/13/2020) ... including CACHE_SCP_FILL (aka Tensor_Load_L2SCP).
      if(trans_desc.get_cmd() inside {CACHE_LOCK_SW, CACHE_UNLOCK_SW, CACHE_LOCK, CACHE_UNLOCK, CACHE_FLUSH_SW, CACHE_EVICT_SW, CACHE_PREFETCH, CACHE_FLUSH, CACHE_EVICT, TENSOR_LOAD, TENSOR_STORE, TENSOR_STORE_SCP, TENSOR_REDUCE, CACHE_SCP_FILL})
      begin
         orig_is_tensor_cacheop = 1'b1;
         ignore_err_addr = 1'b1;
      end

      // cross-check against RTL signals: based on orig_thread_id:
      if ((rtl_info.bus_err[orig_thread_id] == 1'b1) && 
          ((rtl_info.bus_err_addr[orig_thread_id] == orig_paddr) || (ignore_err_addr == 1'b1)) ) 
      begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("check_core_bus_err: thread %0b: L1SB and RTL match: bus_err=1 bus_err_addr[]=0x%h (ignore err_addr for Tensor or CacheOp)", orig_thread_id, orig_paddr))
      end
      else begin
         sb_error($sformatf("check_core_bus_err: thread %0b: L1SB and RTL mismatch: RTL: bus_err=%0b bus_err_addr[]=0x%h ... but L1SB: orig_thread_id=%0b orig_paddr=0x%h (ignore err_addr for Tensor or CacheOp)", orig_thread_id, rtl_info.bus_err[orig_thread_id], rtl_info.bus_err_addr[orig_thread_id], orig_thread_id, orig_paddr), trans_desc, {}, "", `__FILE__, `__LINE__);
      end

      // FUTURE: ... more ???
      // what if both bits of rtl_info.bus_err[1:0] are asserted at same cycle ???  FUTURE ...

   endfunction: check_core_bus_err



   // TensorFMA Req arrives:
   function void process_tfma_req (input evl_minion_tfma_ctrl_t rtl_info);
      logic        conv_bits_rdy;
      logic [15:0] conv_bits;
      logic        is_conv;    // Use convolution CSR to skip computation of rows
      logic [5:0]  padding3;   // Padding to align with CSR
      logic [1:0]  cols_b;     // Number of rows of B matrix to be computed (writing a 15 means 16 rows)
      logic [3:0]  rows_a;     // Number of rows of A matrix to be computed (writing a 7 means 8 rows)
      logic [3:0]  cols_a;     // Number of cols of A matrix to be computed (writing a 15 means 16 rows)
      logic [3:0]  start_a;    // Position within cacheline where the A element is at 32b granularity
      logic [18:0] padding2;   // Padding to align with CSR
      logic        to_vrf;     // Store results to VPU RF (IMA only)
      logic        u_b;        // Matrix B is unsigned (IMA only)
      logic        u_a;        // Matrix A is unsigned (IMA only)
      logic        ten_b;      // Read data straight from TENB
      logic [1:0]  padding1;   // Padding to align with CSR
      logic [5:0]  scp_b;      // Where in the scratchpad the matrix B is stored
      logic [1:0]  padding0;   // Padding to align with CSR
      logic [5:0]  scp_a;      // Where in the scratchpad the matrix A is stored
      logic [2:0]  mode;       // Doing an 000 -> FP32, 001 -> *FP16+FP32, 010 -> FP16, 011 -> *INT8+INT32 operation
      logic        first_pass; // 1 -> 1st pass is a MUL, 0 -> 1st pass is an FMA
      string       mode_string;
      string       first_pass_string;

      conv_bits_rdy = rtl_info.tfma_conv_bits_rdy;
      conv_bits = rtl_info.tfma_conv_bits;

      is_conv = rtl_info.tfma_ctrl.is_conv ;
      cols_b = rtl_info.tfma_ctrl.cols_b ;
      rows_a = rtl_info.tfma_ctrl.rows_a ;
      cols_a = rtl_info.tfma_ctrl.cols_a ;
      start_a = rtl_info.tfma_ctrl.start_a ;
      to_vrf = rtl_info.tfma_ctrl.to_vrf ;
      u_b = rtl_info.tfma_ctrl.u_b ;
      u_a = rtl_info.tfma_ctrl.u_a ;
      ten_b = rtl_info.tfma_ctrl.ten_b ;
      scp_b = rtl_info.tfma_ctrl.scp_b ;
      scp_a = rtl_info.tfma_ctrl.scp_a ;
      mode = rtl_info.tfma_ctrl.mode ;
      first_pass = rtl_info.tfma_ctrl.first_pass ;

      case(mode) 
         3'b000:  mode_string = "FP32";
         3'b001:  mode_string = "FP16+FP32";
         3'b010:  mode_string = "FP16";
         3'b011:  mode_string = "INT8+INT32";
         default: mode_string = "Illegal";
      endcase 

      first_pass_string = (first_pass == 1'b0)? "FMA" : "MUL";

      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TensorFMA or TensorIMA Req [vpu.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_ctrl] event: mode=0x%h(%s) first_pass=%0b(%s) bcols=%0d acols=%0d arows=%0d acol_start=%0d tenb=%0b scp_bstart=%0d scp_astart=%0d is_conv=%0b", mode, mode_string, first_pass, first_pass_string, cols_b, cols_a, rows_a, start_a, ten_b, scp_b, scp_a, is_conv))

      // set flag: for RTLMIN-6078 and VERIF-3508:
      if (ten_b == 1'b1) begin
         m_tfma_req_but_not_proc_en = 1'b1;
      end

      // FUTURE: for TFMA_TenB, can we try to pair it to TL_TenB ... ??? and do some checking ???
      if (ten_b == 1'b1) begin
         if (m_latest_tload_tenb == null) begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("for above TFMA Req (TenB=1): pairing m_latest_tload_tenb=null"))
         end else begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("for above TFMA Req (TenB=1): pairing m_latest_tload_tenb=%s", m_latest_tload_tenb.sprint_obj() ))
         end
      end
   endfunction: process_tfma_req


   // TensorFMA starts processing/enabling:  
   function void process_tfma_proc_en ();

      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TensorFMA or TensorIMA start processing [Dcache.s1_vpu_ctrl.tfma_enabled] event: ..."))

      // clear flag:
      m_tfma_req_but_not_proc_en = 1'b0;

      // FUTURE: can we try to pair it to TL_TenB ... ??? and do some checking ??? 

   endfunction: process_tfma_proc_en




   //----------------------------------------------
   // Function: process_end_of_sim_check: cross-check L1SB against RTL on TensorError CSR current value
   //
   function void process_end_of_sim_check (input evl_minion_end_of_sim_t rtl_info);
      // note: exclude CSR_TensorError bit[3](FCC ovfl) and bit[6](TenB & FMA pair) and bit[8](TSt Cfg Err) ... (not detected in DCache) ?
      `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("End_Of_Sim: will do L1SB~RTL cross-check on CSR_TensorError_T0 and CSR_TensorError_T1 ..."))

         // cross-check on current value:  exclude bit[3] FCC_Overflow;
         //if ( (rtl_info.cur_val_t0.tload[2:0] != m_TensorError_CSR[0][2:0]) ||
         //   /*(rtl_info.cur_val_t0.fcc        != m_TensorError_CSR[0][3]) ||*/    // exclude bit[3] (FCC overflow)
         //     (rtl_info.cur_val_t0.scp_unset  != m_TensorError_CSR[0][4]) ||
         //     (rtl_info.cur_val_t0.co         != m_TensorError_CSR[0][5]) ||
         //   /*(rtl_info.cur_val_t0.tenb       != m_TensorError_CSR[0][6]) ||*/    // exclude bit[6] (illegal TenB & FMA pairing)
         //     (rtl_info.cur_val_t0.mem_fault  != m_TensorError_CSR[0][7]) ||
         //   /*(rtl_info.cur_val_t0.tstore     != m_TensorError_CSR[0][8]) ||*/    // exclude bit[8] (TensorSt config invalid)``
         //     (rtl_info.cur_val_t0.treduce    != m_TensorError_CSR[0][9]) )
         //begin 
         //   //sb_error($sformatf("End_Of_Sim: CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val_t0), null, {}, "", `__FILE__, `__LINE__); 
         //   `evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) End_Of_Sim: CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val_t0))
         //end
         //else begin
         //   `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("End_Of_Sim: CSR_TensorError_T0: cur_val match (exclude bit[3][6]): L1SB=RTL=0x%h", m_TensorError_CSR[0]))
         //end

         if ( ((rtl_info.cur_val[0] & 10'b10_1010_0111) != (m_TensorError_CSR[0] & 10'b10_1010_0111)) ||  // exclude bit[3][6][8] (handle [4] separate) 
              ((m_TensorError_CSR[0][4] == 1'b1) && (rtl_info.cur_val[0][4] == 1'b0)) )                   // [4] is special: csr_file also set [4] 
         begin 
            sb_error($sformatf("End_Of_Sim: CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6][8]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val[0]), null, {}, "", `__FILE__, `__LINE__); 
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) End_Of_Sim: CSR_TensorError_T0: cur_val mismatch (exclude bit[3][6][8]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[0], rtl_info.cur_val[0]))
         end
         else begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("End_Of_Sim: CSR_TensorError_T0: cur_val match (exclude bit[3][6][8]): L1SB=RTL=0x%h", m_TensorError_CSR[0]))
         end

         // cross-check on current value:  exclude bit[3] FCC_Overflow;
         //if ( (rtl_info.cur_val_t1.scp_unset  != m_TensorError_CSR[1][4]) ||
         //     (rtl_info.cur_val_t1.co         != m_TensorError_CSR[1][5]) ||
         //     (rtl_info.cur_val_t1.mem_fault  != m_TensorError_CSR[1][7]) ) 
         //begin 
         //   //sb_error($sformatf("End_Of_Sim: CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=4'b%4b RTL=4'b%4b", {m_TensorError_CSR[1][7],m_TensorError_CSR[1][5],m_TensorError_CSR[1][4],m_TensorError_CSR[1][3]}, rtl_info.cur_val_t1), null, {}, "", `__FILE__, `__LINE__); 
         //   `evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) End_Of_Sim: CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=4'b%4b RTL=4'b%4b", {m_TensorError_CSR[1][7],m_TensorError_CSR[1][5],m_TensorError_CSR[1][4],m_TensorError_CSR[1][3]}, rtl_info.cur_val_t1))
         //end
         //else begin
         //   `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("End_Of_Sim: CSR_TensorError_T1: cur_val match (exclude bit[3]): L1SB=RTL=4'b%4b", rtl_info.cur_val_t1))
         //end

         // cross-check on current value: actually only bit[4][5][7] should be valid:  exclude bit[3] FCC_Overflow;
         if ( ((rtl_info.cur_val[1] & 10'b11_1110_0111) != (m_TensorError_CSR[1] & 10'b11_1110_0111)) || // exclude bit[3] (handle [4] separate) 
              ((m_TensorError_CSR[1][4] == 1'b1) && (rtl_info.cur_val[1][4] == 1'b0)) )                  // [4] is special: csr_file also set [4] 
         begin 
            sb_error($sformatf("End_Of_Sim: CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[1], rtl_info.cur_val[1]), null, {}, "", `__FILE__, `__LINE__); 
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING) End_Of_Sim: CSR_TensorError_T1: cur_val mismatch (exclude bit[3]): L1SB=0x%h RTL=0x%h", m_TensorError_CSR[1], rtl_info.cur_val[1]))
         end
         else begin
            `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("End_Of_Sim: CSR_TensorError_T1: cur_val match (exclude bit[3]): L1SB=RTL=0x%h", m_TensorError_CSR[1]))
         end

   endfunction: process_end_of_sim_check




   //----------------------------------------------
   // Function: process_mh_l2_resp_stall_start
   //
   function void process_mh_l2_resp_stall_start(input evl_dcache_mh_l2_resp_stall_s rtl_info);

       if(rtl_info.mh_l2_resp_stall_start[0] === 1'b1) begin
          m_mh_l2_resp_stall[0] = 1'b1;
          `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Minion L1SB received an RTL_event mh_l2_resp_stall_start[0]=1"))
       end
       if(rtl_info.mh_l2_resp_stall_start[1] === 1'b1) begin
          m_mh_l2_resp_stall[1] = 1'b1;
          `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Minion L1SB received an RTL_event mh_l2_resp_stall_start[1]=1"))
       end

   endfunction: process_mh_l2_resp_stall_start


   //----------------------------------------------
   // Function: process_mh_l2_resp_stall_clear
   //
   function void process_mh_l2_resp_stall_clear(input evl_dcache_mh_l2_resp_stall_s rtl_info);

       if(rtl_info.mh_l2_resp_stall_clear[0] === 1'b1) begin
           m_mh_l2_resp_stall[0] = 1'b0;
          ->>event_mh_l2_resp_stall_clear[0];
          `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Minion L1SB received an RTL_event mh_l2_resp_stall_clear[0]=1"))
       end
       if(rtl_info.mh_l2_resp_stall_clear[1] === 1'b1) begin
           m_mh_l2_resp_stall[1] = 1'b0;
          ->>event_mh_l2_resp_stall_clear[1];
          `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Minion L1SB received an RTL_event mh_l2_resp_stall_clear[1]=1"))
       end

   endfunction: process_mh_l2_resp_stall_clear



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_esr_info
   //
   function void process_esr_info(input evl_esr_config_t rtl_info);
      m_mprot_esr = rtl_info.mprot;
      m_bypass    = rtl_info.bypass;
      // TODO: Ying to fix mprot
      `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Dcache ESR config update: MPROT=0x%0x BYPASS=%0d en_secure_mem=%0b", m_mprot_esr, m_bypass, 1'b0/*m_mprot_esr.enable_secure_memory*/))

      // temp: cannot yet support m_mprot_esr.enable_secure_memory=1 
      //if (m_mprot_esr.enable_secure_memory == 1'b1) begin
      //   sb_error($sformatf("RTL set mprot_esr.enable_secure_memory=1: L1SB does not support it yet"), null, {}, "", `__FILE__, `__LINE__);
      //end
   endfunction: process_esr_info



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_fence
   //
   //   This function is called when a FENCE instruction is seen at the EX stage of the intpipe
   //   which corresponds to S0 stage of the dcache. When a fence is received, no more Loads/Stores
   //   can be sent from the intpipe until the dcache has serviced all previous transactions from
   //   the thread.
   //
   function void process_fence(input evl_thread_id_t thread_id);
      if (keep_fence(thread_id)) begin
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Minion SB received a fence for thread %0d - Waiting: set m_fence[]=1", thread_id))
         m_fence[thread_id] = 1'b1;
      end
      else begin
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Minion SB received a fence for thread %0d - no outstanding trans in L1SB: fence done (not to set m_fence[]): OK to accept new intpipe requests", thread_id))
      end
   endfunction: process_fence



   //-----------------------------------------------------------------------------------------------
   //
   // Function: keep_fence
   //
   //   This function is called to determine if a fence is done or still needs to wait.
   //   As long as the thread has transactions in flight (somewhere in the dcache pipe or in the
   //   replay queue) then the fence isn't done and no new requests should be seen.
   //
   function bit keep_fence(input evl_thread_id_t thread_id);
      evl_trans_desc s1_outstanding[$];
      evl_trans_desc s2_outstanding[$];
      evl_trans_desc_queue replay_outstanding[$];
      bit outstanding_ops = 1'b0;
      int ii;

      s1_outstanding = m_s1_queue.find(trans_desc) with (trans_desc.get_thread_id() == thread_id);
      if (s1_outstanding.size() != 0) return 1'b1;
      s2_outstanding = m_s2_queue.find(trans_desc) with (trans_desc.get_thread_id() == thread_id);
      if (s2_outstanding.size() != 0) return 1'b1;
      foreach (m_replay_queue[ii]) begin
         if (   (m_replay_queue[ii].get_thread_id() == thread_id)
             && (m_replay_queue[ii].get_cmd() != CACHE_PREFETCH))       // exclude CacheOp_Prefetch?
         begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction: keep_fence


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_store
   //
   //   This function is called when a tensor store is accepted by the dcache. This function decodes
   //   the attributes of that tensor store and creates the necessary transaction descriptors.
   //
   function void process_tensor_store(input evl_dcache_tstore_req_t rtl_info);
      evl_trans_desc  trans_desc;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      evl_vaddr_t     vaddr;
      evl_vaddr_t     addr_stride;        // addr stride
      string          extra_info;
      string          coop_type;
      int             start_reg;
      int             reg_stride;
      int             num_cols;
      int             num_regs;
      int             num_rows;
      int             num_coop;
      int             bytes;
      bit             invalid_coop_comb = 1'b0;   // was: nop = 1'b0 
      int             req_size;

      int             my_num_regs;
      int             my_num_cols;
      int             my_num_rows;
      evl_vaddr_t     my_va_stride;           // [47:0] 
      evl_vaddr_t     my_vaddr;               // [47:0] 
    //evl_vaddr_t     my_vaddr_next;          // [47:0] 
      bit [`VA_EXT_MSB:0]  my_vaddr_next;     // [48:0] one extra bit (for addr overflow)
      bit             my_addr_msb_err;        // addr overflow
      int             my_spawn_req_idx;       // idx of (multiple) individual reqs that one TensoStore will spawn off 

      thread_id  = rtl_info.thread_id;
      insn_pc    = rtl_info.pc;
      start_reg  = rtl_info.ctrl.start_reg;
      reg_stride = int'(rtl_info.ctrl.src_inc) + 1;
      num_cols   = int'(rtl_info.ctrl.cols) + 1;
      num_rows   = int'(rtl_info.ctrl.rows) + 1;
      num_coop   = int'(rtl_info.ctrl.coop) + 1;
      vaddr      = evl_vaddr_t'(rtl_info.ctrl.addr) << 4;
      bytes      = num_cols * 16;
      addr_stride = evl_vaddr_t'(rtl_info.stride) & 'hfffffffff0;
      num_regs   = num_rows * ((num_cols+1) / 2);

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_cmd(TENSOR_STORE);
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_vaddr(vaddr);
    //trans_desc.set_paddr(evl_paddr_t'(vaddr));  // should get from TLB VA->PA
      trans_desc.set_privilege((rtl_info.vm_status.mprv) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);
      trans_desc.set_cached();  // (CYE) TensorStore is catogrized as Cached

      extra_info = $sformatf("(Num regs %0d, Start reg %0d, Rows %0d, Columns %0d, Coop %0d, Reg stride %0d, Addr stride 0x%0x)", num_regs, start_reg, num_rows, num_cols, num_coop, reg_stride, addr_stride);

      case ({num_coop, num_cols})
         {int'(1), int'(1)},
         {int'(1), int'(2)},
         {int'(1), int'(4)}: coop_type = " (No-coop)";
         {int'(2), int'(1)}: coop_type = " (Coop Pair-128)";
         {int'(2), int'(2)}: coop_type = " (Coop Pair-256)";
         {int'(4), int'(1)}: coop_type = " (Coop Quad-128)";
         default: begin
            coop_type = " (Coop INVALID)";
            invalid_coop_comb = 1'b1;   // was: nop = 1'b1;
         end
      endcase
      extra_info = $sformatf("%0s%0s", extra_info, coop_type);

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("New request (thread %0d) (PC=0x%12x): %0s %0s",  trans_desc.get_thread_id(), insn_pc, trans_desc.sprint_obj(), extra_info))

      if (invalid_coop_comb == 1'b1) begin    // was: if (nop == 1'b1) begin
         //`evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStore: invalid Coop combination: Dropping trans; set m_TensorError_CSR[%0d][8]=1", thread_id))
         //m_TensorError_CSR[thread_id][8] = 1'b1;
         // Note: actually RTL intpipe_csr_file detects such invalid coop_config and cancel the instruction (before sending it to DCache):
         sb_error($sformatf("TensorStore (thread %0d): invalid Coop combination config: unexpected by DCache", thread_id), trans_desc, {}, "", `__FILE__, `__LINE__);
         return;
      end

      if (num_coop > 1) trans_desc.set_int_param(pk_coop_tstore, 1);

      if (m_tstore_s1_queue.size() != 0) begin
         sb_error("received a Tensor Store while another Tensor Store was already ongoing (in S1 stage)", null, {}, "", `__FILE__, `__LINE__);
         foreach (m_tstore_s1_queue[ii]) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), m_tstore_s1_queue[ii], $sformatf("this guy was pending: %0s", m_tstore_s1_queue[ii].sprint_obj()))
         end
      end

      if (m_reduce_queue.size() != 0) begin
         sb_error("received a Tensor Store while a Tensor Reduce was already ongoing", null, {}, "", `__FILE__, `__LINE__);
         foreach (m_reduce_queue[ii]) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), m_reduce_queue[ii], $sformatf("this guy was pending: %0s", m_reduce_queue[ii].sprint_obj()))
         end
      end

      m_tstore_reg_idx = 0;
      m_tstore_req_idx = 0;
      m_tstore_regs    = num_regs;
      m_tstore_stride  = addr_stride;
      m_tstore_cols    = num_cols;

      // note: Virtual Memory needs an S1 event: see code below;

      // (CYE) change to:
      // spawn off (multiple) individual TensorStore pkts into S1 stage queue: then do VA->PA (TLB) and PMA check;
      // note: base on num_regs, num_cols: 
      my_num_regs = num_regs;
      my_num_cols = num_cols;
      my_num_rows = num_rows;
      my_va_stride   = addr_stride;    // addr stride
      my_spawn_req_idx = 0;       // idx of (multiple) individual reqs that one TensoStore will spawn off 
      my_vaddr = vaddr;           // starting value; 
     
      req_size = (my_num_cols == 4) ? 6 :
                 (my_num_cols == 2) ? 5 :
                 (my_num_cols == 1) ? 4 : 0;

      while (my_num_regs > 0) begin   
         // decrement the number of registers still to send
         my_num_regs -= 1;

         if (((my_num_regs % 2) == 0) || (my_num_cols != 4)) begin
            my_vaddr_next = (vaddr + (my_spawn_req_idx * my_va_stride)) & (~evl_vaddr_t'((1 << req_size) - 1));
            // check for addr overflow:
            //my_addr_msb_err = my_vaddr[`VA_MSB] & ~my_vaddr_next[`VA_MSB];  // this is RTL behavior  
            // Question: Can we detect the extra bit my_vaddr_next[`VA_EXT_MSB] ([48]) ???
            my_addr_msb_err = my_vaddr_next[`VA_EXT_MSB];

            my_vaddr = my_vaddr_next[`VA_MSB:0]; 
            //`evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStore %s: ready to spawn off individual reqs: vaddr=0x%h spawn_idx=%0d", trans_desc.sprint_obj(), my_vaddr, my_spawn_req_idx ))
            spawn_tstore(trans_desc, my_vaddr, my_spawn_req_idx, my_addr_msb_err);
            my_spawn_req_idx++;
         end
      end 

   endfunction: process_tensor_store


   //---------------------------------------------------------------------
   //
   // Function : spawn_tstore  (CYE)
   //
   function void spawn_tstore (input evl_trans_desc trans_desc,     
                               input evl_vaddr_t vaddr,
                               input int spawn_req_idx,
                               input bit addr_msb_err );
      evl_trans_desc  next_trans_desc;

      // Spawn a child transaction
      $cast(next_trans_desc, trans_desc.clone());
      next_trans_desc.set_cmd(trans_desc.get_cmd());
      next_trans_desc.set_vaddr(vaddr);
// The clone will copy the following fields:
//    next_trans_desc.set_trans_id(trans_desc.get_trans_id());
//    next_trans_desc.set_thread_id(trans_desc.get_thread_id());
      next_trans_desc.set_privilege(trans_desc.get_privilege());
      next_trans_desc.set_int_param(pk_coop_tstore, trans_desc.get_int_param(pk_coop_tstore, 0));
      next_trans_desc.set_int_param(pk_addr_msb_err, addr_msb_err);
      next_trans_desc.set_cached();

      add_active_mem_desc(next_trans_desc);
      m_tstore_s1_queue.push_back(next_trans_desc);
      //`evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorStore req: %0s (spawn_idx=%0d)(addr_msb_err=%0b)", next_trans_desc.sprint_obj(), spawn_req_idx, addr_msb_err ))
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorStore req: %0s (spawn_idx=%0d)", next_trans_desc.sprint_obj(), spawn_req_idx))    // note: we are not testing "addr_msb_err=1" case 

   endfunction: spawn_tstore


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_store_scp
   //
   //   This function is called when a tensor store from scratchpad is accepted by the dcache.
   //   This function decodes the attributes of that tensor store and creates the necessary
   //   transaction descriptors.
   //
   function void process_tensor_store_scp(input evl_dcache_tstore_scp_req_t rtl_info);
      evl_trans_desc  trans_desc;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      evl_vaddr_t     vaddr;
      evl_vaddr_t     addr_stride;
      string          extra_info;
      int             scp_start_line;
      int             scp_stride;
      int             num_lines;
      evl_vaddr_t     my_vaddr;              // [47:0]
    //evl_vaddr_t     my_vaddr_next;         // [47:0]
      bit [`VA_EXT_MSB:0]  my_vaddr_next;    // [48:0] one extra bit (for addr overflow)
      bit             my_addr_msb_err;       // addr overflow
      int             my_spawn_req_idx;
      int             my_scp_line_id;        // actual scp_line_# (0~47) 

      thread_id      = rtl_info.thread_id;
      insn_pc        = rtl_info.pc;
      scp_start_line = int'(rtl_info.ctrl.start_entry % 48);      // SCP max = 48 lines (valid range 0-47)
      scp_stride     = int'(rtl_info.ctrl.stride_entry) + 1;
      num_lines      = int'(rtl_info.ctrl.rows) + 1;
      vaddr          = evl_vaddr_t'(rtl_info.ctrl.addr) << 6;
      addr_stride    = evl_vaddr_t'(rtl_info.stride) & const_line_vaddr_mask;

      m_tstore_num_lines = num_lines;
      m_tstore_stride = addr_stride;

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_cmd(TENSOR_STORE_SCP);
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_vaddr(vaddr);
    //trans_desc.set_paddr(evl_paddr_t'(vaddr));    // should get from TLB VA->PA
      trans_desc.set_privilege((rtl_info.vm_status.mprv) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);

      extra_info = $sformatf("(SCP start line: %0d) (num_lines: %0d)", scp_start_line, num_lines);
      if (num_lines > 0) extra_info = $sformatf("%0s (SCP stride: %0d) (Addr stride: 0x%0x)", extra_info, scp_stride, addr_stride);
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("New request (thread %0d) (PC=0x%12x): %0s %0s",  trans_desc.get_thread_id(), insn_pc, trans_desc.sprint_obj(), extra_info))

      // Scratchpad must be enabled, otherwise request is dropped and TensorErrorCSR[4] is set.
      if (m_scp_en == 1'b0) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStoreSCP while scratchpad disabled: Dropping trans; set m_TensorError_CSR[%0d][4]=1", thread_id))
         m_TensorError_CSR[thread_id][4] = 1'b1;
         // FUTURE: ... add cross-check against RTL ... probably do it at a later cycle (depending on RTL cycle)
         return;
      end

      // note: Virtual Memory needs an S1 event: see code below;

      // (CYE) change to:
      // spawn off (multiple) individual TensorStoreScp pkts into S1 stage queue: then do VA->PA (TLB) and PMA check;
      my_spawn_req_idx = 0;
      my_vaddr = vaddr;     // starting value
      for (int ii = 0; ii < num_lines; ii++) begin
         my_vaddr_next = vaddr + (ii * addr_stride);
         // check for addr overflow: 
         //my_addr_msb_err = my_vaddr[`VA_MSB] & ~my_vaddr_next[`VA_MSB];  // this is RTL behavior
         // Question: Can we detect the extra bit my_vaddr_next[`VA_EXT_MSB] ([48]) ???
         my_addr_msb_err = my_vaddr_next[`VA_EXT_MSB];  

         my_vaddr = my_vaddr_next[`VA_MSB:0];
         my_scp_line_id = scp_start_line + (ii * scp_stride);
         my_scp_line_id = (my_scp_line_id % 48);          // valid range: 0~47    // (CYE) added
         //`evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStoreScp %s: ready to spawn off individual reqs: vaddr=0x%h spawn_idx=%0d scp_line_id=%0d", trans_desc.sprint_obj(), my_vaddr, my_spawn_req_idx, my_scp_line_id ))
         spawn_tstore_scp(trans_desc, my_vaddr, my_spawn_req_idx, my_scp_line_id, my_addr_msb_err);
         my_spawn_req_idx++;
      end 

   endfunction: process_tensor_store_scp


   //-----------------------------------------------------------------------------
   //
   // Function : spawn_tstore_scp  (CYE)
   //
   function void spawn_tstore_scp (input evl_trans_desc trans_desc,     
                                   input evl_vaddr_t vaddr,
                                   input int spawn_req_idx, 
                                   input int scp_line_id,
                                   input bit addr_msb_err );
      evl_trans_desc  next_trans_desc;

      // Spawn a child transaction
      $cast(next_trans_desc, trans_desc.clone());
      next_trans_desc.set_cmd(trans_desc.get_cmd());
      next_trans_desc.set_vaddr(vaddr);
// The clone will copy the following fields:
//    next_trans_desc.set_trans_id(trans_desc.get_trans_id());
//    next_trans_desc.set_thread_id(trans_desc.get_thread_id());
      next_trans_desc.set_privilege(trans_desc.get_privilege());
      next_trans_desc.set_int_param(pk_tstore_scp_line_id, scp_line_id);     // (CYE) added
      next_trans_desc.set_int_param(pk_addr_msb_err, addr_msb_err);
      next_trans_desc.set_cached();

      add_active_mem_desc(next_trans_desc);
      m_tstore_scp_s1_queue.push_back(next_trans_desc);
      //`evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorStoreSCP req: %0s (spawn_idx=%0d)(scp_line_id=%0d)(addr_msb_err=%0b)", next_trans_desc.sprint_obj(), spawn_req_idx, scp_line_id, addr_msb_err ))
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorStoreSCP req: %0s (spawn_idx=%0d)(scp_line_id=%0d)", next_trans_desc.sprint_obj(), spawn_req_idx, scp_line_id))     // note: we are not testing "addr_msb_err=1" case

   endfunction: spawn_tstore_scp


   //-----------------------------------------------------------------------------
   // 
   // Function: parse_tstore_s1   (CYE)
   //   parse the tensor_store S1 event to call either process_tstore_s1 or process_tstore_scp_s1 
   //
   function void parse_tstore_s1(input evl_dcache_tstore_s1_t rtl_info);  // need any new info for S1 from RTL ???
      if(m_tstore_s1_queue.size() > 0) begin
         process_tstore_s1(rtl_info);
      end 
      else if(m_tstore_scp_s1_queue.size() > 0) begin
         process_tstore_scp_s1(rtl_info);
      end else begin
         sb_error($sformatf("Received MINION_TSTORE_S1_INFO event, but both m_tstore_s1_queue and m_tstore_scp_s1_queue are empty"), null, {}, "", `__FILE__, `__LINE__);
      end
   endfunction: parse_tstore_s1


   //-----------------------------------------------------------------------------
   // 
   // Function: process_tstore_s1   (CYE)
   //   mainly to handle TLB VA->PA, and PMA checking; and then push into m_tstore_vpu_reg_queue[] waiting for VPU_reg_send
   //
   function void process_tstore_s1(input evl_dcache_tstore_s1_t rtl_info);   // need any new info for S1 from RTL ???
      evl_trans_desc trans_desc;
      bit            l1sb_pma_fault = 1'b0;
      bit            thread_id;
      bit            rtl_tensor_err_mem = 1'b0; 

      trans_desc = m_tstore_s1_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected S1 tensor_store event: m_tstore_s1_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      thread_id = trans_desc.get_thread_id();

      // VA->PA (TLB):
      if (va_to_pa(trans_desc) == 1'b0) begin
         // tensor unit simply waits and will retry so push the descriptor at the front of queue
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("tensor store TLB miss"))
         m_tstore_s1_queue.push_front(trans_desc);
         return;
      end
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStore S1 %0s: TLB VA->PA: VA=0x%h PA=0x%h", trans_desc.sprint_obj(), trans_desc.get_vaddr(), trans_desc.get_paddr()))

      // PMA check: 
      if (check_pma(trans_desc) == 1'b0) begin
         l1sb_pma_fault = 1'b1;
      end

      // cross_check against RTL: it will not trigger exception;
      l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, 1'b0, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   // xcpt_qaulifier=0


      // cross_check against RTL on tensor_err_mem:
      // Problem: RTL signal "reduce.err_mem_next" is S2 (too late), we need S1: so cannot do such checking here: 
      //rtl_tensor_err_mem = rtl_info.err_mem; 
      //if (l1sb_pma_fault != rtl_tensor_err_mem) begin
      //   //sb_error($sformatf("TensorStore: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem), trans_desc, {}, "", `__FILE__, `__LINE__);
      //   `evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) TensorStore: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem))
      //end 

      // if L1SB PMA finds access_fault, drop it; also drop all the younger ones still in m_tstore_s1_queue[];
      if (l1sb_pma_fault == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStore got PMA access fault: Dropping trans; set m_TensorError_CSR[%0d][7]=1", thread_id))
         m_TensorError_CSR[thread_id][7] = 1'b1;

         delete_active_desc(trans_desc);
         // also drop all the younger ones still in m_tstore_s1_queue[]:
         while (m_tstore_s1_queue.size() > 0) begin   
            trans_desc = m_tstore_s1_queue.pop_front();
            delete_active_desc(trans_desc);
         end
         return;
      end

      // new: final version: push into m_tstore_vpu_reg_queue waiting for VPU_reg_send:
      m_tstore_vpu_reg_queue.push_back(trans_desc);
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("push_back into m_m_tstore_vpu_reg_queue[]: TensorStore %s", trans_desc.sprint_obj())) 
      
   endfunction: process_tstore_s1 


   //-----------------------------------------------------------------------------
   // 
   // Function: process_tstore_scp_s1  (CYE)
   //   mainly to handle TLB VA->PA, and PMA checking, and then generate expected L2_Req
   //
   function void process_tstore_scp_s1(input evl_dcache_tstore_s1_t rtl_info);   // need any new info for S1 from RTL ???
      evl_trans_desc trans_desc;
      bit            l1sb_pma_fault = 1'b0;
      bit            thread_id;
      bit            rtl_tensor_err_mem = 1'b0; 
      int            tstore_scp_line_id;

      trans_desc = m_tstore_scp_s1_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected S1 tensor_store_scp event: m_tstore_scp_s1_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      thread_id = trans_desc.get_thread_id();
      tstore_scp_line_id = trans_desc.get_int_param(pk_tstore_scp_line_id);

      // VA->PA (TLB):
      if (va_to_pa(trans_desc) == 1'b0) begin
         // tensor unit simply waits and will retry so push the descriptor at the front of queue
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("tensor store TLB miss"))
         m_tstore_scp_s1_queue.push_front(trans_desc);
         return;
      end
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStoreSCP S1 %0s:TLB VA->PA: VA=0x%h PA=0x%h", trans_desc.sprint_obj(), trans_desc.get_vaddr(), trans_desc.get_paddr()))

      // PMA check: 
      if (check_pma(trans_desc) == 1'b0) begin
         l1sb_pma_fault = 1'b1;
      end

      // cross_check against RTL: it will not trigger exception;
      l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, 1'b0, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   // xcpt_qaulifier=0

      // cross_check against RTL on tensor_err_mem:
      // Problem: RTL signal "reduce.err_mem_next" is S2 (too late), we need S1: so cannot do such checking here: 
      //rtl_tensor_err_mem = rtl_info.err_mem;
      //if (l1sb_pma_fault != rtl_tensor_err_mem) begin
      //   //sb_error($sformatf("TensorStoreSCP: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem), trans_desc, {}, "", `__FILE__, `__LINE__);
      //   `evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) TensorStoreSCP: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem))
      //end 

      // if L1SB PMA finds access_fault, drop it; also drop all the younger ones still in m_tstore_scp_s1_queue[];
      if (l1sb_pma_fault == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorStoreSCP got PMA access fault: Dropping trans; set m_TensorError_CSR[%0d][7]=1", thread_id))
         m_TensorError_CSR[thread_id][7] = 1'b1;

         delete_active_desc(trans_desc);
         // also drop all the younger ones still in m_tstore_scp_s1_queue[]
         while (m_tstore_scp_s1_queue.size() > 0) begin 
            trans_desc = m_tstore_scp_s1_queue.pop_front();
            delete_active_desc(trans_desc);
         end
         return;
      end

      
      // (CYE) 3/18/2020: check TensorStore_From_L1SCP conflict with on-going TensorLoad_L1SCP (for overlap range of SCP_lines):
      // check against outstanding/ongoing m_tload_scp_outstanding_q[]: for overlap range of L1SCP entry: if match found, error;
      if (m_tload_scp_outstanding_q.size() > 0) begin
         evl_trans_desc  my_tload_scp;
         int  tload_scp_line_start;
         int  tload_scp_num_lines;     // max=16
         int  tload_scp_line_end;
         bit  conflict = 1'b0;

         foreach(m_tload_scp_outstanding_q[ii]) begin
            my_tload_scp = m_tload_scp_outstanding_q[ii];
            tload_scp_line_start = my_tload_scp.get_int_param(pk_tload_scp_dest);
            tload_scp_line_start = tload_scp_line_start % 48;     // max=48
            tload_scp_num_lines = my_tload_scp.get_int_param(pk_tload_num_lines);   // max=16
            tload_scp_line_end = tload_scp_line_start + tload_scp_num_lines - 1;      // inclusive
            tload_scp_line_end = tload_scp_line_end % 48;     // max=48
   
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), my_tload_scp, $sformatf("on-going TensorLoad_L1SCP: %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end))
   
            // figure out whether tstore_scp_line_id is within range of {tload_scp_line_start, tload_scp_line_end};
            if (tload_scp_line_end >= tload_scp_line_start) begin   // "end" is no wraparound
               if ((tstore_scp_line_id >= tload_scp_line_start) && (tstore_scp_line_id <= tload_scp_line_end)) begin
                  conflict = 1'b1;
               end
            end 
            else begin   // "end" is wraparound
               if (tstore_scp_line_id >= tload_scp_line_start) begin
                  conflict = 1'b1;
               end
               else begin  // if (tstore_scp_line_id < tload_scp_line_start)
                  if (((tstore_scp_line_id+48) >= tload_scp_line_start) && ((tstore_scp_line_id+48) <= (tload_scp_line_end+48))) begin
                     conflict = 1'b1;
                  end
               end 
            end
            if (conflict == 1'b1) begin
               if (m_en_tstore_scp_conflict_tload_scp_chk == 1'b1) begin
                  sb_error($sformatf("TensorStoreSCP S1 (above): its scp_line=%0d conflict with on-going TensorLoad_L1SCP %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", tstore_scp_line_id, my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end), trans_desc, {}, "", `__FILE__, `__LINE__);
               end
               else begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) TensorStoreSCP S1 (above): its scp_line=%0d conflict with on-going TensorLoad_L1SCP %s (start_scp_line=%0d num_lines=%0d end_scp_line=%0d)", tstore_scp_line_id, my_tload_scp.sprint_obj(), tload_scp_line_start, tload_scp_num_lines, tload_scp_line_end))
               end
            end
         end  // foreach(m_tload_scp_outstanding_q[ii])
      end // if (m_tload_scp_outstanding_q.size() > 0)


      // handle each individual TS_Scp req one at a time: generate expected L2_Req; 
      ////for (int ii = 0; ii < m_tstore_num_lines; ii++) 
      begin
         evl_req_desc  l2_req_desc;
         evl_line_data line_data;
         int           scp_line_id;    // max = (48-1)
         bit [1:0]     scp_way;        // max = (4-1)
         int           scp_set;        // max = (12-1)
         bit [511:0]   scp_data_64byte;

         //
         // Predict the WriteAround on EVICT interface:
         // Note: RTL probably does this step in S2 or S3 stage, but this case does not need to TagLookUp, so ScBD can do it in S1 stage.
         //
         $cast(l2_req_desc, m_template_req_desc_neigh.clone());
         trans_desc.copy_to_desc(l2_req_desc);
         void'(trans_desc.add_child_desc(l2_req_desc));
         l2_req_desc.set_trans_id(trans_desc.get_trans_id());
         l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(BusWriteAround));

         l2_req_desc.set_paddr(trans_desc.get_paddr());    // VA and PA are already calculated for each individual req

         l2_req_desc.set_bus_req_size(6);      // always size 64-byte
         l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Reduce);

         // Request data is unknown at this point for lack of SCP modelling: --> change: see code below (Work-in-progress)
         //line_data = l2_req_desc.get_req_data(0, 1'b1);
         //line_data.validate_line();
         //line_data.make_line_unknown();    

         // CYE/Charles: now that L1SCP is modeled: assign expected data to the Request:
         scp_line_id = trans_desc.get_int_param(pk_tstore_scp_line_id);
         scp_line_id = scp_line_id % 48;     // max 48 clines
         scp_way = scp_line_id / 12;
         scp_set = scp_line_id % 12;  
         scp_data_64byte = m_l1_scp[scp_way][scp_set];
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("for this TStore_SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d scp_data_64byte=0x%h",  scp_line_id, scp_way, scp_set, scp_data_64byte ))

         // populate l2_req_desc: expected TStore_SCP data to be sent: will be self-checked in evl_etlink_verif_comp.sv
         l2_req_desc.set_req_xword(0, scp_data_64byte);  

         //// (CYE) to test self-checking: change the above data to be some wrong value: --> self-checking on TStore data is working!
         //l2_req_desc.get_req_data().zero_all_data(); 
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("for this TStore_SCP Data to L2: zero out its data (for testing)" ))

         setup_bus_req_dependencies(l2_req_desc);
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Predicting this WriteAround (by TStore_SCP) to EVICT interface: %0s (data=0x%h)", l2_req_desc.sprint_obj(), scp_data_64byte))
         //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         //   l2_req_desc.get_req_data().print_obj(get_abstract_name(), l2_req_desc.get_paddr());
         //end

         m_pending_evict_reqs.push_back(l2_req_desc);

         // Note: also need to populate m_write_data of the trans_desc; ??? 
         //       similar as for ST, ST_WA, ATOMIC (in function process_s1_req); -->> Don't need to!
         //trans_desc.set_write_bytes(0, 64, scp_data_64byte, 64'hffff_ffff_ffff_ffff);   // entire cline 64-byte

         delete_active_desc(trans_desc);
      end

   endfunction: process_tstore_scp_s1 



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_reduce: (CYE) first cycle, before qualified by vpu_reduce_wait
   //
   //   This function is called when a tensor reduce is accepted by the dcache. This function decodes
   //   the attributes of that tensor reduce and creates the necessary transaction descriptors.
   //
   function void process_tensor_reduce(input evl_dcache_reduce_req_t rtl_info);
      evl_trans_desc  trans_desc;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      evl_paddr_t     paddr;
      bit [3:0]       opcode;
      int             num_regs;
      int             start_reg;
      int             partner;
      int             depth;
      int             xform;
      bit             send;
      bit             illegal_op = 1'b0;

      m_tensor_reduce_req_rtl_info = rtl_info;

      thread_id = rtl_info.thread_id;
      insn_pc   = rtl_info.pc;
      num_regs  = rtl_info.ctrl.num_regs;
      start_reg = rtl_info.ctrl.start_reg;
      partner   = rtl_info.ctrl.partner;
      depth     = rtl_info.ctrl.partner[3:0];
      xform     = rtl_info.ctrl.action;
      opcode    = rtl_info.ctrl.op;

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_cmd(TENSOR_REDUCE);
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(Cyc0) New request (thread %0d) (PC=0x%12x): TENSOR_REDUCE (first cycle, before vpu_reduce_wait)",  trans_desc.get_thread_id(), insn_pc))

      m_reduce_wait_queue.push_back(trans_desc);

   /*********     // move the following code to function process_tensor_reduce_aft_wait;
   **********/

   endfunction: process_tensor_reduce



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_reduce_aft_wait: (CYE) after qualified by vpu_reduce_wait
   //
   //   This function is called when a tensor reduce is accepted by the dcache. This function decodes
   //   the attributes of that tensor reduce and creates the necessary transaction descriptors.
   //
   function void process_tensor_reduce_aft_wait(input evl_dcache_reduce_req_t err_rtl_info);
      evl_trans_desc  trans_desc;
      evl_req_desc    l2_req_desc;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      evl_paddr_t     paddr;
      string          extra_info;
      string          op_string;
      bit [3:0]       opcode;
      int             num_regs;
      int             start_reg;
      int             partner;
      int             depth;
      int             xform;
      bit             send;
      bit             invalid_partner = 1'b0;
      bit             invalid_depth   = 1'b0;      
      bit             illegal_op = 1'b0;
      bit             recv_illegal_op = 1'b0;       // "receiver" gets illegal_op
      evl_dcache_reduce_req_t  orig_rtl_info;
      bit             tensor_reduce_err = 1'b0;

      trans_desc = m_reduce_wait_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an unexpected tensor_reduce_after_wait event: m_reduce_wait_queue is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      orig_rtl_info   = m_tensor_reduce_req_rtl_info;
      thread_id = orig_rtl_info.thread_id;
      insn_pc   = orig_rtl_info.pc;
      num_regs  = orig_rtl_info.ctrl.num_regs;
      start_reg = orig_rtl_info.ctrl.start_reg;
      partner   = orig_rtl_info.ctrl.partner;
      depth     = orig_rtl_info.ctrl.partner[3:0];
      xform     = orig_rtl_info.ctrl.action;
      opcode    = orig_rtl_info.ctrl.op;

    /****
      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_cmd(TENSOR_REDUCE);
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);
    ****/

      op_string = (opcode == 4'b0000) ? "fadd"   :
                  (opcode == 4'b0010) ? "fmax"   :
                  (opcode == 4'b0011) ? "fmin"   :
                  (opcode == 4'b0100) ? "iadd"   :
                  (opcode == 4'b0110) ? "imax"   :
                  (opcode == 4'b0111) ? "imin"   :
                  (opcode == 4'b1000) ? "fget"   :
                                        "illegal";

      illegal_op = (op_string == "illegal");

      case (xform)
         0: begin // SEND
            extra_info = "(send)";
            send       = 1'b1;
         end
         1: begin // RECEIVE
            extra_info = "(recv)";    // was receive
            send       = 1'b0;
         end
         2: begin // BROADCAST
            //
            // If minionID % 2^(depth+1) == 0
            //  - the hart is a sender
            //  - it receives data from Minion with receiverID = minionID+2^depth.
            //
            if (get_minion_id() % (1 << (depth + 1)) == 0) begin
               extra_info = $sformatf("(broadcast depth %0d -> send)", depth);
               partner    = get_minion_id() + (1 << depth);
               send       = 1'b1;
            end
            //
            // If minionID % 2^(depth+1) == 2^depth
            //  - the hart is a receiver
            //  - it sends data to Minion with senderID = minionID-2^depth.
            //
            else if (get_minion_id() % (1 << (depth + 1)) == (1 << depth)) begin
               extra_info = $sformatf("(broadcast depth %0d -> recv)", depth);     // was receive
               partner    = get_minion_id() - (1 << depth);
               send       = 1'b0;
            end
            //
            // Otherwise the hart does not participate in the reduction,
            // and it neither sends, nor receives data
            //
            else begin
               extra_info = $sformatf("(broadcast depth %0d -> not involved)", depth);
               partner    = -1;
               send       = 1'b0;
            end
         end
         3: begin // AUTO-REDUCE
            //
            // If minionID % 2^(depth+1) == 0
            //  - the hart is a receiver
            //  - it receives data from Minion with senderID = minionID+2^depth.
            //
            if (get_minion_id() % (1 << (depth + 1)) == 0) begin
               extra_info = $sformatf("(auto depth %0d -> recv)", depth);      // was receive
               partner    = get_minion_id() + (1 << depth);
               send       = 1'b0;
            end
            //
            // If minionID % 2^(depth+1) == 2^depth
            //  - the hart is a sender
            //  - it sends data to Minion with receiverID = minionID-2^depth.
            //
            else if (get_minion_id() % (1 << (depth + 1)) == (1 << depth)) begin
               extra_info = $sformatf("(auto depth %0d -> send)", depth);
               partner    = get_minion_id() - (1 << depth);
               send       = 1'b1;
            end
            //
            // Otherwise the hart does not participate in the reduction,
            // and it neither sends, nor receives data
            //
            else begin
               extra_info = $sformatf("(auto depth %0d -> not involved)", depth);
               partner    = -1;
               send       = 1'b0;
            end
         end
         default: sb_error($sformatf("should not get here: TensorReduce: xform=%0d (illegal)", xform), null, {}, "", `__FILE__, `__LINE__);
      endcase

      m_reduce_partner = partner;   // (CYE/Charles) save it (only one outstanding tensor_reduce)
      trans_desc.set_int_param(pk_treduce_send, send);

      paddr =  `EVL_MSG_REDUCE_OR_MASK |
              (evl_paddr_t'(send ? 1 : 0) << 5) |
              (evl_paddr_t'(get_shire_id_from_minion_id(partner)) << `EVL_MSG_SHIRE_ID_LSB) |
              (evl_paddr_t'(partner % `EVL_MINION_PER_SHIRE) << `EVL_MSG_MINION_ID_LSB);
      trans_desc.set_paddr(paddr);

      if (partner != -1) 
         extra_info = $sformatf("%0s (Partner:%0d %0s%0s)", extra_info, partner, convert_partner_to_S_N_C(partner), (partner == get_minion_id()) ? " -> Invalid" : "");
      if ((xform >= 1) && (xform <= 3))       // note: reduce-sender don't-care opcode;
         extra_info = $sformatf("%0s (Op:%0s)", extra_info, op_string);

      extra_info = $sformatf("%0s (Start reg %0d, Num regs %0d)", extra_info, start_reg, num_regs);

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("New request (thread %0d) (PC=0x%12x): TENSOR_REDUCE %0s(after vpu_reduce_wait)",  trans_desc.get_thread_id(), insn_pc, extra_info))

      // if this hart/minion not participate in the reduction (not-involved): drop the trans:
      if (partner  == -1) return;

      //
      // Invalid Cases -> NOP
      //
      if (partner == get_minion_id() || partner > TENSOR_REDUCE_MAX_PARTNER_ID) begin   // ??? usually only happen in Send-Receive mode ???
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorReduce: receiver MinionID same as sender MinionID: Dropping trans; set m_TensorError_CSR[%0d][9]=1", thread_id))
         m_TensorError_CSR[thread_id][9] = 1'b1;
         invalid_partner = 1'b1;
         tensor_reduce_err = 1'b1;
         //return;     // move "return" later (see code below)
      end

      //if (reduce_receive | auto_reduce(receive) | reduce_broadcast(receive)) begin    // only for "receiver"
      if ((xform == 1) || ((xform == 2) && (send == 1'b0)) || ((xform == 3) && (send == 1'b0))) begin   
         if (illegal_op == 1'b1) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorReduce (as receiver): illegal Opcode=0x%h: Dropping trans; set m_TensorError_CSR[%0d][9]=1", opcode, thread_id))
            m_TensorError_CSR[thread_id][9] = 1'b1;
            recv_illegal_op = 1'b1;
            tensor_reduce_err = 1'b1;
            // should NOT drop trans -> change to "drop" to follow RTL behavior; 
         end
      end
      // Tensor broadcast or reduce-autopair can't have tree level bigger than 2
      if (xform inside {2,3} && depth > TENSOR_REDUCE_MAX_TREE_DEPTH) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorReduce/Broadcast: Tree level %0dis invalid : Dropping trans; set m_TensorError_CSR[%0d][9]=1", depth, thread_id))
         m_TensorError_CSR[thread_id][9] = 1'b1;
         invalid_depth = 1;
         tensor_reduce_err = 1'b1;
      end
      // cross-check RTL signal "tensor_reduce_err_flags" (1-cycle pulse): (which goes into CSR_tensor_error[9]):
      if (tensor_reduce_err != err_rtl_info.csr_terr_t0.treduce) begin
         sb_error($sformatf("TensorReduce: tensor_reduce_err_flags (csr_tensor_error[9]) mismatch: L1SB=%0b RTL=%0b", tensor_reduce_err, err_rtl_info.csr_terr_t0.treduce), trans_desc, {}, "", `__FILE__, `__LINE__);
         //`evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) TensorReduce: tensor_reduce_err_flags (csr_tensor_error[9]) mismatch: L1SB=%0b RTL=%0b", tensor_reduce_err, err_rtl_info.csr_terr_t0.treduce))
      end

      //
      // Invalid Cases -> NOP
      //
      if (recv_illegal_op == 1'b1)     return;    // (on "receiver") to follow RTL behavior (but could cause "sender" to hang)
      if (invalid_partner == 1'b1)     return;
      if (invalid_depth   == 1'b1)     return;      
      if (num_regs ==  0)              return;

      m_reduce_regs      = num_regs;
      m_reduce_ready_idx = (xform < 2) ? 16 : depth;

      if (m_reduce_queue.size() != 0) begin
         sb_error("received a tensor reduce while another store/reduce was already ongoing (m_reduce_queue not empty)", null, {}, "", `__FILE__, `__LINE__);
         foreach (m_reduce_queue[ii]) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), m_reduce_queue[ii], $sformatf("this guy was pending: %0s", m_reduce_queue[ii].sprint_obj()))
         end
      end

      if (send == 1'b1) begin      // TensorReduce sender
         add_active_mem_desc(trans_desc);
         m_reduce_queue.push_back(trans_desc);    // for sender
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(TensorReduce as sender) push into m_reduce_queue: TensorReduce %s", trans_desc.sprint_obj()))
      end
      else begin        // TensorReduce receiver: will send out "READY" message (to sender)
         //
         // Predict the Message Ready on EVICT interface
         //
         $cast(l2_req_desc, m_template_req_desc_neigh.clone());
         trans_desc.copy_to_desc(l2_req_desc);
         void'(trans_desc.add_child_desc(l2_req_desc));
         l2_req_desc.set_trans_id(trans_desc.get_trans_id());
         l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(BusWriteMsg));
         l2_req_desc.set_paddr(trans_desc.get_paddr());
         l2_req_desc.set_bus_req_size(0);
         l2_req_desc.set_bus_req_id(0); // Unused by ETLink but RTL puts 0

         // Data in message contains:
         // 1) for auto-pair mode: the "level" to let partner minion know which minion is ready to receive; OR,
         // 2) for send-receive mode: just bit DCACHE_REDUCE_MSG_DATA_MSB to '1' to indicate a peer-to-peer connection
         l2_req_desc.set_req_byte(0, (xform < 2) ? 8'h10 : depth);

         setup_bus_req_dependencies(l2_req_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Predicting this Reduce Ready message to L2 EVICT interface: %0s", l2_req_desc.sprint_obj()))
         m_pending_evict_reqs.push_back(l2_req_desc);

         // as receiver: also want to put trans_desc into m_reduce_queue[]: to wait for reduce_data from sender;
         add_active_mem_desc(trans_desc);
         m_reduce_queue.push_back(trans_desc);    // as receiver 
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(TensorReduce as receiver) push into m_reduce_queue: TensorReduce %s", trans_desc.sprint_obj()))

      end

   endfunction: process_tensor_reduce_aft_wait



   //
   // convert (tensor reduce) partner number to SxNxCx format (string)
   //
   function string convert_partner_to_S_N_C (int partner);
      string  s_n_c;
      int  s_id;   // shire ID
      int  n_id;   // neigh ID
      int  c_id;   // core ID
      int  my_partner;

      s_id = partner / 32;          // 32 minions per Shire
      my_partner = partner % 32;    // modulu (remainder)
      n_id = my_partner / 8;        // 8 minions per Neigh
      c_id = my_partner % 8;        // modulu (remainder)

      s_n_c = $sformatf("S%0d:N%0d:C%0d", s_id, n_id, c_id);
      return s_n_c;
   endfunction: convert_partner_to_S_N_C





   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_vpu_reg_send 
   //
   //   This function is called when the next register is sent by the VPU for a tensor store or tensor reduce (as sender)
   //   Protocol:
   //    a) Core/D$->VPU: send signal vpu_reduce_ctrl.send_reg;
   //    b) 3 cycles later: VPU->Core/D$: return data on s1_core_store_data (when our TB samples this event) 
   //
   task process_vpu_reg_send(input evl_oword_t rtl_data);
      evl_trans_desc  trans_desc;
      evl_req_desc    l2_req_desc;

      if ((m_reduce_queue.size() == 0) && (m_tstore_vpu_reg_queue.size() == 0)) begin
         sb_error("Received VPU register but no Tensor Reduce/Store was pending (both m_reduce_queue[] and m_tstore_vpu_reg_queue[] are empty)", null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      else if ((m_reduce_queue.size() > 0) && (m_tstore_vpu_reg_queue.size() > 0)) begin
         sb_error("Received VPU register but both Tensor Reduce & Tensor Store were pending (both m_reduce_queue[] and m_tstore_vpu_reg_queue[] are non-empty)", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      //
      // Tensor Reduce (as sender) processing: sending out reduce data;
      //
      if ((m_reduce_queue[0] != null) && (m_reduce_queue[0].get_cmd() == TENSOR_REDUCE)) begin   // TensorReduce should have only one outstanding 
         m_reduce_lock.get(1);
         trans_desc = m_reduce_queue[0];
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(Observed) outgoing VPU_Reg_Send (3 cycles ago): incoming return_data=0x%0x: for TensorReduce (as sender) %s", rtl_data, trans_desc.sprint_obj()))
         
         if (m_reduce_regs <= 0) begin
            sb_error("Reduce request appears to have sent all its registers already. Something went wrong", trans_desc, {}, "", `__FILE__, `__LINE__);
         end

         //
         // Decrement the number of registers still to send
         // If zero, we're done. Delete this transaction and mark as done
         //
         m_reduce_regs -= 1;
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("after decremented: m_reduce_num_regs=%0d (remained)", m_reduce_regs))

         //
         // Predict the Reduce Data message on EVICT interface:  ET_LINK_REQ_MsgSendData?
         //
         $cast(l2_req_desc, m_template_req_desc_neigh.clone());
         trans_desc.copy_to_desc(l2_req_desc);
         void'(trans_desc.add_child_desc(l2_req_desc));
         l2_req_desc.set_trans_id(trans_desc.get_trans_id());
         l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(BusWriteMsg));   // ET_LINK_REQ_MsgSendData?
         l2_req_desc.set_paddr(trans_desc.get_paddr());
         l2_req_desc.set_bus_req_size(5);               // always size 32-byte
         l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Reduce); // Unused by ETLink
         l2_req_desc.set_req_oword(0, rtl_data);        // Reduce Data (32-byte): will be self-checked in evl_etlink_verif_comp.sv

         //// (CYE) to test self-checking: change the above Reduce Data to some wrong value: --> self-checking Reduce Data is working!
         //l2_req_desc.get_req_data().zero_all_data();
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("for this Reduce Data to L2: zero out its data (for testing)" ))

         //
         // If we've already received the 'READY' message from the partner Minion
         // go ahead and predict the DATA message on ETLink. Otherwise push on the
         // pending queue to wait until we did receive it.
         //
         if (m_reduce_ready[m_reduce_ready_idx] == 1'b1) begin
            setup_bus_req_dependencies(l2_req_desc);
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Predicting this Reduce Data message to L2 EVICT interface: %0s", l2_req_desc.sprint_obj()))
            m_pending_evict_reqs.push_back(l2_req_desc);

            if (m_reduce_regs == 0) begin
               delete_active_desc(trans_desc);
               m_reduce_queue.delete(0);
               m_reduce_ready[m_reduce_ready_idx] = 0;
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("bcoz m_reduce_regs==0(num_regs): delete m_reduce_queue[0] and clear m_reduce_ready[]=0 (idx=%0d)", m_reduce_ready_idx)) 
            end
         end
         else begin
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Pending this Reduce Data message until Reduce Ready message for index %0d is received: %0s", m_reduce_ready_idx, l2_req_desc.sprint_obj()))
            m_reduce_send_data_pend.push_back(l2_req_desc);
         end
         m_reduce_lock.put(1);
      end
      //
      // Tensor Store processing:
      // new scheme: m_tstore_vpu_reg_queue[] contains multiple spawn-off TensorStore Reqs (after S1): one corresponds to each Writearound Req which D$ will send out; 
      //
      else if ((m_tstore_vpu_reg_queue[0] != null) && (m_tstore_vpu_reg_queue[0].get_cmd() == TENSOR_STORE)) begin   // TensorStore should have only one outstanding 
         trans_desc = m_tstore_vpu_reg_queue[0];
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(Observed) outgoing VPU_Reg_Send (3 cycles ago): incoming return_data=0x%0x: for TensorStore %s", rtl_data, trans_desc.sprint_obj()))

         if (m_tstore_regs <= 0) begin
            sb_error("Tensor store request appears to have sent all its registers already. Something went wrong", trans_desc, {}, "", `__FILE__, `__LINE__);
         end

         m_tstore_data |= (rtl_data << (`EVL_OWORD_BYTES * 8 * (m_tstore_reg_idx % 2) * (m_tstore_cols / 4))); 
         m_tstore_reg_idx++;

         // Decrement the number of registers still to send
         m_tstore_regs -= 1; 
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("after decremented: m_tstore_num_regs=%0d (remained)", m_tstore_regs)) 

         if (((m_tstore_regs % 2) == 0) || (m_tstore_cols != 4)) begin 
            int req_size;
            req_size = (m_tstore_cols == 4) ? 6 :
                       (m_tstore_cols == 2) ? 5 :
                       (m_tstore_cols == 1) ? 4 : 0;

            trans_desc = m_tstore_vpu_reg_queue.pop_front();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("m_tstore_vpu_reg_queue.pop_front(): TensorStore %s", trans_desc.sprint_obj() )) 

            //
            // Predict the WriteAround on EVICT interface
            //
            $cast(l2_req_desc, m_template_req_desc_neigh.clone());
            trans_desc.copy_to_desc(l2_req_desc);
            void'(trans_desc.add_child_desc(l2_req_desc));
            l2_req_desc.set_trans_id(trans_desc.get_trans_id());
            l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(BusWriteAround));

            // changed: now the trans_desc is the spawn-off TS req which already has correctly calculated VA and PA:
            l2_req_desc.set_paddr(trans_desc.get_paddr());

            l2_req_desc.set_bus_req_size(req_size);
            if (trans_desc.get_int_param(pk_coop_tstore, 0) == 1) begin
               l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Tensor_Store_Coop);
            end
            else begin
               l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Reduce);
            end

            // TStore Data (coming from VPU_Reg): will be self-checked in in evl_etlink_verif_comp.sv 
            if (req_size == 4) begin
               l2_req_desc.set_req_qword(-1, m_tstore_data); 
            end
            else if (req_size == 5) begin
               l2_req_desc.set_req_oword(-1, m_tstore_data); 
            end
            else if (req_size == 6) begin
               l2_req_desc.set_req_xword(-1, m_tstore_data); 
            end

            //// (CYE) to test self-checking: change the above Reduce Data to some wrong value: --> self-checking Reduce Data is working!
            //l2_req_desc.get_req_data().zero_all_data();
            //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("for this TStore Data to L2: zero out its data (for testing)" ))

            setup_bus_req_dependencies(l2_req_desc);
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Predicting this WriteAround (by TStore) to EVICT interface: %0s (data=0x%h)", l2_req_desc.sprint_obj(), m_tstore_data))
            //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            //   l2_req_desc.get_req_data().print_obj(get_abstract_name(), l2_req_desc.get_paddr());
            //end

            m_pending_evict_reqs.push_back(l2_req_desc);

            if (m_tstore_regs == 0) begin 
               // m_tstore_vpu_reg_queue should be also empty: otherwise error:
               if (m_tstore_vpu_reg_queue.size() > 0) begin
                  sb_error($sformatf("for above TensorStore VPU_reg_send: m_tstore_regs=0: but m_tstore_vpu_reg_queue[] still not empty"), null, {}, "", `__FILE__, `__LINE__); 
               end
            end

            // Note: also need to populate m_write_data of the trans_desc; ??? 
            //       similar as for ST, ST_WA, ATOMIC (in function process_s1_req); -->> Don't need to!
            //trans_desc.set_write_bytes(0, 64, m_tstore_data, 64'hffff_ffff_ffff_ffff);   // entire cline 64-byte

            delete_active_desc(trans_desc);

            m_tstore_req_idx++;
            m_tstore_data = 0; 
         end  // if (((m_tstore_regs % 2) == 0) || (m_tstore_cols != 4))
      end  // if (Tensor_Reduce) ... else if (Tensor_Store)

   endtask: process_vpu_reg_send


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_rcv_msg
   //
   //   This function is called when an ETLink message is received.
   //   It could be for a Tensor Reduce (ready or data), port messages, or texture commands
   //
   task process_rcv_msg(input evl_req_desc req_desc);
      evl_line_data msg_data;
      string        source;
      byte          src_id;   // actually: pairing_info: 1) auto-pair: tree-depth (level); 2) send-receive (peer-to-peer): =5'h10;
      int           partner;

      // Message Reduce Ready
      if (req_desc.get_bus_rsp_id() == `EVL_MSG_REDUCE_READY_ID) begin

         m_reduce_lock.get(1);

         // RTL comment:
         // Data in "READY" message contains:
         // 1) Auto-Pairing mode: the "level" to let partner minion know which minion is ready to receive; OR,
         // 2) Send-Receive mode: just bit DCACHE_REDUCE_MSG_DATA_MSB to '1' to indicate a peer-to-peer connection;

         msg_data = req_desc.get_rsp_data();
         src_id   = msg_data.get_byte(0);
         src_id   = src_id[4:0];

         //partner  = get_minion_id() + (1 << src_id);  // FUTURE ??? why not use the equation in function process_tensor_reduce() ... ???
         partner  = m_reduce_partner;     // CYE/Charles: change to use common variable m_reduce_partner (for this Core)

         source   = (src_id == 16) ? "Peer-to-Peer" : $sformatf("Tree depth %0d (Minion %0d)", src_id, partner);

         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("Received L2 message Reduce Ready from %0s (src_id=%0d partner=%0d)", source, src_id, partner))

         if (src_id > 16) begin
            sb_error($sformatf("malformed ready message received: msg data = %5b", src_id), null, {}, "", `__FILE__, `__LINE__);
         end
         if (m_reduce_ready[src_id] == 1'b1) begin
            sb_error($sformatf("Received message ready from %0s (src_id=%0d) while another ready (from same src_id) was still outstanding! Probably bad software and this may hang the machine...", source, src_id), req_desc, {}, "", `__FILE__, `__LINE__);
         end

         m_reduce_ready[src_id] = 1'b1;
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("set m_reduce_ready[]=1 (src_id=%0d) for Reduce Ready from %0s", src_id, source))

         while (m_reduce_send_data_pend.size() > 0) begin
            evl_req_desc   reduce_req;
            evl_trans_desc root_desc;
            reduce_req = m_reduce_send_data_pend[0];
            root_desc  = reduce_req.get_root_trans_desc();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), reduce_req, $sformatf("Predicting this Reduce Data message to L2 EVICT interface: %0s", reduce_req.sprint_obj()))

            setup_bus_req_dependencies(reduce_req);
            m_pending_evict_reqs.push_back(reduce_req);
            m_reduce_send_data_pend.delete(0);

            if (m_reduce_queue.size() == 0) begin
               delete_active_desc(root_desc);
               m_reduce_ready[src_id] = 0;
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("but bcoz m_reduce_queue is empty: clear m_reduce_ready[]=0 (src_id=%0d)", src_id))
            end
         end
         m_reduce_lock.put(1);
      end

      // Message Reduce Data
      else if (req_desc.get_bus_rsp_id() == `EVL_MSG_REDUCE_DATA_ID) begin
         evl_trans_desc  orig_treduce_trans_desc;
         evl_trans_desc  treduce_data_trans_desc;

         msg_data = req_desc.get_rsp_data();
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("Received L2 message Reduce Data (from partner=%0d): see below:",  m_reduce_partner))
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            msg_data.print_obj(get_abstract_name(), req_desc.get_paddr());
         end

         // m_reduce_queue[0] (receiver) (max only one outstanding) must be expecting this reduce data: 
         if ((m_reduce_queue[0] == null) ||
             (m_reduce_queue[0].get_cmd() != TENSOR_REDUCE) ||
             (m_reduce_queue[0].get_int_param(pk_treduce_send) != 0))
         begin
            sb_error($sformatf("for above message reduce data: m_reduce_queue[0] is NOT a TENSOR_REDUCE receiver: something wrong"), null, {}, "", `__FILE__, `__LINE__);
            return;
         end

         orig_treduce_trans_desc = m_reduce_queue[0];
         if (m_reduce_regs <= 0) begin
            sb_error("Reduce receiver appears to have already received data for all its registers: something wrong", orig_treduce_trans_desc, {}, "", `__FILE__, `__LINE__);
            return;
         end

         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), orig_treduce_trans_desc, $sformatf("(CYE DEBUG) for above Tensor Reduce Data: pending m_reduce_queue[0] (receiver) is waiting for data: %s", orig_treduce_trans_desc.sprint_obj() )) 

         // from m_reduce_queue[0]:
         // spawn off a child (pseudo) trans_desc (with this above reduce data) and enqueue into m_reduce_recv_data_retn_queue[]:
         $cast(treduce_data_trans_desc, orig_treduce_trans_desc.clone());
         treduce_data_trans_desc.set_cmd(orig_treduce_trans_desc.get_cmd());
// The clone will copy the following fields:
//       treduce_data_trans_desc.set_trans_id(orig_treduce_trans_desc.get_trans_id());
//       treduce_data_trans_desc.set_vaddr(orig_treduce_trans_desc.get_vaddr());
//       treduce_data_trans_desc.set_paddr(orig_treduce_trans_desc.get_paddr());
//       treduce_data_trans_desc.set_thread_id(orig_treduce_trans_desc.get_thread_id());
         treduce_data_trans_desc.set_int_param(pk_treduce_send, orig_treduce_trans_desc.get_int_param(pk_treduce_send));
         treduce_data_trans_desc.set_read_data(msg_data);  // expected read return data (m_read_data)

         m_reduce_recv_data_retn.push_back(treduce_data_trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), treduce_data_trans_desc, $sformatf("(CYE DEBUG) for above Tensor Reduce Data: from pending m_reduce_queue[0] created a pseudo treduce_data_trans_desc (with m_read_data) and push it into m_reduce_recv_data_retn[](queue): %s", treduce_data_trans_desc.sprint_obj() )) 

         m_reduce_regs -= 1;
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), orig_treduce_trans_desc, $sformatf("after decremented: m_reduce_num_regs=%0d (left)", m_reduce_regs))

         if (m_reduce_regs == 0) begin
            delete_active_desc(orig_treduce_trans_desc);
            m_reduce_queue.delete(0);
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), orig_treduce_trans_desc, $sformatf("bcoz m_reduce_regs==0(num_regs): delete m_reduce_queue[0]" )) 
         end

      end // `EVL_MSG_REDUCE_DATA_ID

      // Nothing for Port Message
      else begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("Received Port Message : %0s", req_desc.sprint_obj()))
      end
   endtask: process_rcv_msg


   //
   // Function:  process_reduce_recv_data_retn:  upon receiving signal dcache.reduce.s2_inst_op_valid
   //
   function void process_reduce_recv_data_retn();     // ? Do we need other RTL info ? 
      evl_trans_desc treduce_data_trans_desc;

      treduce_data_trans_desc = m_reduce_recv_data_retn.pop_front();
      if (treduce_data_trans_desc == null) begin
         sb_error("Received an <dcache.reduce.s2_inst_op_valid> event, but m_reduce_recv_data_retn[] is empty", null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), treduce_data_trans_desc, $sformatf("(CYE DEBUG) uopn receive RTL signal dcache.reduce.s2_inst_op_valid: expected treduce_data_trans_desc (from m_reduce_recv_data_retn[]) is: %s", treduce_data_trans_desc.sprint_obj() )) 

      if (m_en_s3_core_resp_chk == 1'b1) begin
         m_s3_core_resp_queue.push_back(treduce_data_trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), treduce_data_trans_desc, $sformatf("(CYE DEBUG) push the above treduce_data_trans_desc (with m_read_data) into m_s3_core_resp_queue[]: %s", treduce_data_trans_desc.sprint_obj() )) 
      end

   endfunction: process_reduce_recv_data_retn




   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_load_cye0     // include TLoad_L1SCP (all transforms) and TLoad_TenB
   //
   //   This function is called when a tensor load just arrives at D$ at its very first cycle;
   //   will wait 1 cycle (or more) before enter next function process_tensor_load.
   //   This function decodes the attributes of that tensor load.
   //
   function void process_tensor_load_cyc0(input evl_dcache_tload_req_t rtl_info);
      evl_trans_desc  trans_desc;
      evl_vaddr_t     addr;
      evl_vaddr_t     stride;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      string          extra_info;
      int             num_lines;        // number of L1Scp lines (after +1)
      bit             use_tmask;
      int             tensor_mask;          // might not be "ready" yet (tmask_ready==0)
      int             scp_dest_line;        // starting cline_idx of L1SCP
      int             coop_neigh_mask;
      int             coop_min_mask;
      int             coop_id;
      int             seq_id;
      bit             use_coop;
      bit             wait_id;
      bit             tl_sel;
      int             byte_offset;
      int             transform;
      int             transform_lines;
      int             seq_id_max;
      bit             illegal_transform;
      //int             spawn_req_idx;         // probably same as "seq_id"

      addr            = evl_vaddr_t'(rtl_info.ctrl.addr) << 6;
      stride          = evl_vaddr_t'(rtl_info.stride & const_line_vaddr_mask);
      wait_id         = bit'(rtl_info.stride[0]);
      num_lines   = int'(rtl_info.ctrl.num_lines+1);
      use_tmask       = bit'(rtl_info.ctrl.use_tmask);         // only for TL_L1SCP (TenB never use tmask)
      tensor_mask     = int'(rtl_info.tmask_bits);             // might not be "ready" yet (tmask_ready==0)
      scp_dest_line   = int'(rtl_info.ctrl.scp_dest % 48);     // starting cline_idx of L1SCP out of total 48 clines (12 sets, 4 ways)
      use_coop        = bit'(rtl_info.ctrl.use_coop);
      tl_sel          = bit'(rtl_info.ctrl.tl_sel);            // 1=TENB(MatrixB) 0=L1SCP(MatrixA)
      coop_neigh_mask = int'(rtl_info.coop_ctrl.neigh_mask);
      coop_min_mask   = int'(rtl_info.coop_ctrl.minion_mask);
      coop_id         = int'(rtl_info.coop_ctrl.id);
      thread_id       = evl_thread_id_t'(rtl_info.thread_id);
      byte_offset     = int'(rtl_info.ctrl.offset) * 16;
      transform       = (tl_sel == 1'b0) ? int'(rtl_info.ctrl.transform) : 0;   // only TLoad_L1SCP (MatrixA) does Transform
      insn_pc         = rtl_info.pc;
      extra_info      = "";
      seq_id          = 0;
      illegal_transform = 1'b0;
      //spawn_req_idx   = 0;

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_trans_id();
      trans_desc.set_vaddr(evl_vaddr_t'(addr));
      trans_desc.set_cmd(TENSOR_LOAD);
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_privilege((rtl_info.vm_status.mprv) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);

      trans_desc.set_bus_minion_coop_mask(coop_min_mask);
      trans_desc.set_bus_neigh_coop_mask(coop_neigh_mask);
      trans_desc.set_int_param(pk_tensorid,   wait_id);
      trans_desc.set_int_param(pk_tload_sel,  tl_sel);
      trans_desc.set_int_param(pk_tload_coop, use_coop);
      trans_desc.set_int_param(pk_tload_scp_dest, scp_dest_line);      // starting scp line
      trans_desc.set_int_param(pk_tload_num_lines, num_lines);
      trans_desc.set_int_param(pk_tload_transform, transform);
      trans_desc.set_int_param(pk_tload_byte_offset, byte_offset);
      trans_desc.set_cached();    // TensorLoad is catogarized as "Cached"

      extra_info = $sformatf(" (to %0s)", tl_sel ? "TENB" : "L1SCP");
      if (tl_sel == 1'b0) begin  // only for TLoad L1SCP:
         extra_info = $sformatf("%0s (starting scp_dest_line=%0d)", extra_info, scp_dest_line);
      end
      if (num_lines > 1) begin
         extra_info = $sformatf("%0s (num_lines %0d, stride 0x%0x)", extra_info, num_lines, stride);
      end else begin    // if (num_lines==1)
         extra_info = $sformatf("%0s (num_lines %0d)", extra_info, num_lines);
      end
      extra_info = $sformatf("%0s (use_tmask=%0b)", extra_info, use_tmask);
      if (use_coop) begin
         extra_info = $sformatf("%0s (Cooperative - Neigh Mask:0x%0x, Min Mask:0x%0x, ID:0x%0x)", extra_info, coop_neigh_mask, coop_min_mask, coop_id);
      end
      if (tl_sel == 1'b0) begin  // transformation is WARL for TENB
         case (transform)
            0: extra_info = $sformatf("%0s (transform=%0d: No transform)",       extra_info, transform);
            1: extra_info = $sformatf("%0s (transform=%0d: Interleave 8 bits)",  extra_info, transform);
            2: extra_info = $sformatf("%0s (transform=%0d: Interleave 16 bits)", extra_info, transform);
            5: extra_info = $sformatf("%0s (transform=%0d: Transpose 8 bits)",   extra_info, transform);
            6: extra_info = $sformatf("%0s (transform=%0d: Transpose 16 bits)",  extra_info, transform);
            7: extra_info = $sformatf("%0s (transform=%0d: Transpose 32 bits)",  extra_info, transform);
            default: begin
               extra_info = $sformatf("%0s (transform=%0d: Illegal transformation)", extra_info, transform);
               illegal_transform = 1'b1;
            end
         endcase
      end
      if ((tl_sel == 1'b0) && (transform inside {1,2,5,6})) begin
         extra_info = $sformatf("%0s (byte_offset=%0d)", extra_info, byte_offset);
      end

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(Cyc0) New request (thread %0d) (PC=0x%12x): %0s%0s",  trans_desc.get_thread_id(), insn_pc, trans_desc.sprint_obj(), extra_info))

      m_tload_wait_queue[tl_sel].push_back(trans_desc);

      // assign TL_TenB to m_latest_tload_tenb:
      if (tl_sel == 1'b1) begin
         m_latest_tload_tenb = trans_desc;

         // check against m_tfma_req_but_not_proc_en: for RTLMIN-6078 and VERIF-3508:
         if (m_tfma_req_but_not_proc_en == 1'b1) begin
            // this TL_TenB must be replayed (restart_b) w/o going into "s1" stage:
            trans_desc.set_int_param(pk_tenb_expect_restart, 1);
         end
      end

   endfunction: process_tensor_load_cyc0 




   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tensor_load     // include TLoad_L1SCP (all transforms) and TLoad_TenB
   //
   //   This function is called when a tensor load is accepted by dcache (after "wait_tmask" and "wait_previous").
   //   Note: This cycle provides correct tensor_mask (if use_tmask).
   //   This function decodes the attributes of that tensor load and creates the necessary transaction descriptors.
   //
   function void process_tensor_load(input evl_dcache_tload_req_t rtl_info);
      evl_trans_desc  trans_desc;
      evl_vaddr_t     vaddr;
      evl_vaddr_t     stride;
      evl_thread_id_t thread_id;
      evl_vaddr_t     insn_pc;
      string          extra_info;
      int             num_lines;        // number of L1Scp lines (after +1)
      int             tensor_mask;
      int             scp_dest_line;        // starting cline_idx of L1SCP
      int             coop_neigh_mask;
      int             coop_min_mask;
      int             coop_id;
      int             seq_id;
      bit             use_coop;
      bit             use_tmask;
      bit             wait_id;
      bit             tl_sel;
      int             byte_offset;
      int             transform;
      int             transform_lines;
      int             seq_id_max;
      bit             illegal_transform;
      int             spawn_req_idx;         // probably same as "seq_id"

      evl_vaddr_t     my_vaddr;              // [47:0] 
    //evl_vaddr_t     my_vaddr_next;         // [47:0] 
      bit [`VA_EXT_MSB:0]  my_vaddr_next;    // [48:0] one extra bit (for addr overflow)
      bit             my_addr_msb_err;       // addr overflow
      int             my_seq_id;

      vaddr           = evl_vaddr_t'(rtl_info.ctrl.addr) << 6;
      stride          = evl_vaddr_t'(rtl_info.stride & const_line_vaddr_mask);
      wait_id         = bit'(rtl_info.stride[0]);
      num_lines   = int'(rtl_info.ctrl.num_lines+1);
      tensor_mask     = int'(rtl_info.tmask_bits);
      use_tmask       = bit'(rtl_info.ctrl.use_tmask);
      scp_dest_line   = int'(rtl_info.ctrl.scp_dest % 48);     // starting cline_idx of L1SCP out of total 48 clines (12 sets, 4 ways)
      use_coop        = bit'(rtl_info.ctrl.use_coop);
      tl_sel          = bit'(rtl_info.ctrl.tl_sel);            // 1=TENB(MatrixB) 0=L1SCP(MatrixA)
      coop_neigh_mask = int'(rtl_info.coop_ctrl.neigh_mask);
      coop_min_mask   = int'(rtl_info.coop_ctrl.minion_mask);
      coop_id         = int'(rtl_info.coop_ctrl.id);
      thread_id       = evl_thread_id_t'(rtl_info.thread_id);
      byte_offset     = int'(rtl_info.ctrl.offset) * 16;
      transform       = (tl_sel == 1'b0) ? int'(rtl_info.ctrl.transform) : 0;   // only TLoad_L1SCP (MatrixA) does Transform
      insn_pc         = rtl_info.pc;
      extra_info      = "";
      seq_id          = 0;
      illegal_transform = 1'b0;
      spawn_req_idx   = 0;

    /******* // move following code into process_tensor_load_cyc0 ... and use m_tload_wait_queue[]
      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_trans_id();
      trans_desc.set_vaddr(evl_vaddr_t'(vaddr));
      trans_desc.set_cmd(TENSOR_LOAD);
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_privilege((rtl_info.vm_status.mprv) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);

      trans_desc.set_bus_minion_coop_mask(coop_min_mask);
      trans_desc.set_bus_neigh_coop_mask(coop_neigh_mask);
      trans_desc.set_int_param(pk_tensorid,   wait_id);
      trans_desc.set_int_param(pk_tload_sel,  tl_sel);
      trans_desc.set_int_param(pk_tload_coop, use_coop);
      trans_desc.set_int_param(pk_tload_scp_dest, scp_dest_line);      // starting scp line
      trans_desc.set_int_param(pk_tload_num_lines, num_lines);
      trans_desc.set_int_param(pk_tload_transform, transform);
      trans_desc.set_int_param(pk_tload_byte_offset, byte_offset);
      trans_desc.set_cached();    // TensorLoad is catogarized as "Cached"
    ********/

      trans_desc = m_tload_wait_queue[tl_sel].pop_front();

      extra_info = $sformatf(" (to %0s)", tl_sel ? "TENB" : "L1SCP");
      if (tl_sel == 1'b0) begin  // only for TLoad L1SCP:
         extra_info = $sformatf("%0s (starting scp_dest_line=%0d)", extra_info, scp_dest_line);
      end
      if (num_lines > 1) begin
         extra_info = $sformatf("%0s (num_lines %0d, stride 0x%0x)", extra_info, num_lines, stride);
      end else begin    // if (num_lines==1)
         extra_info = $sformatf("%0s (num_lines %0d)", extra_info, num_lines);
      end
      if (use_tmask) begin
         extra_info = $sformatf("%0s (tmask 0x%0x)", extra_info, tensor_mask);
      end
      if (use_coop) begin
         extra_info = $sformatf("%0s (Cooperative - Neigh Mask:0x%0x, Min Mask:0x%0x, ID:0x%0x)", extra_info, coop_neigh_mask, coop_min_mask, coop_id);
      end
      if (tl_sel == 1'b0) begin  // transformation is WARL for TENB
         case (transform)
            0: extra_info = $sformatf("%0s (transform=%0d: No transform)",       extra_info, transform);
            1: extra_info = $sformatf("%0s (transform=%0d: Interleave 8 bits)",  extra_info, transform);
            2: extra_info = $sformatf("%0s (transform=%0d: Interleave 16 bits)", extra_info, transform);
            5: extra_info = $sformatf("%0s (transform=%0d: Transpose 8 bits)",   extra_info, transform);
            6: extra_info = $sformatf("%0s (transform=%0d: Transpose 16 bits)",  extra_info, transform);
            7: extra_info = $sformatf("%0s (transform=%0d: Transpose 32 bits)",  extra_info, transform);
            default: begin
               extra_info = $sformatf("%0s (transform=%0d: Illegal transformation)", extra_info, transform);
               illegal_transform = 1'b1;
            end
         endcase
      end
      if ((tl_sel == 1'b0) && (transform inside {1,2,5,6})) begin
         extra_info = $sformatf("%0s (byte_offset=%0d)", extra_info, byte_offset);
      end

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(After wait) New request (thread %0d) (PC=0x%12x): %0s%0s",  trans_desc.get_thread_id(), insn_pc, trans_desc.sprint_obj(), extra_info))

      //
      // Scratchpad must be enabled, otherwise request is dropped and TensorErrorCSR[4] is set.
      //
      if (m_scp_en == 1'b0) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorLoad while scratchpad disabled: Dropping trans; set m_TensorError_CSR[%0d][4]=1", thread_id))
         m_TensorError_CSR[thread_id][4] = 1'b1;
         return;
      end

      // An illegal transformation is a NOP and set TensorError[1]
      if (illegal_transform == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorLoad configured with wrong transform field: Dropping trans; set m_TensorError_CSR[%0d][1]=1", thread_id))
         m_TensorError_CSR[thread_id][1] = 1'b1;
         return;
      end

      // save Tensor_Load L1SCP object: assume could be multiple outstanding;
      if (tl_sel == 1'b0) begin 
         // assign all info to this original parent_tload_trans_desc:
         trans_desc.set_int_param(pk_tload_use_tmask, use_tmask);
         trans_desc.set_int_param(pk_tload_tensor_mask, tensor_mask);
         trans_desc.set_int_param(pk_tload_active_req_count, 0);
         trans_desc.set_int_param(pk_tload_resp_line_count, 0);
         // add it to m_tload_scp_outstanding_q[]:
         m_tload_scp_outstanding_q.push_back(trans_desc);
      end

    /*** temp: work-in-progress
      // save Tensor_Load TenB object:
      if (tl_sel == 1'b1) begin
         // assign all info to this original parent_tload_trans_desc:
         trans_desc.set_int_param(pk_tload_active_req_count, 0);
         trans_desc.set_int_param(pk_tload_resp_line_count, 0);
         // add it to m_tload_tenb_outstanding_q[]:
         m_tload_tenb_outstanding_q.push_back(trans_desc);
      end
    ***/

      // 1) for TENB, need to only predict a max of 4 reqs as FMA needs to consume it: but sometimes could be max 6 ...
      // 2) and support reset when getting a new TENB -->> Done: see function restart_tenb();

      // various transforms: only for Matrix_A:
      if (transform inside { 0, 1, 2 }) begin
         transform_lines = (transform == 0) ? 1 :  // No transform
                           (transform == 2) ? 2 :  // Interleave16
                                              4 ;  // Interleave8

         seq_id_max = (transform == 0) ? 8 : transform_lines;

         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad: transform_lines=%0d seq_id_max=%0d", transform_lines, seq_id_max))

         my_vaddr = vaddr;   // starting value
         for (int ii = 0; ii < num_lines; ii++) begin
            if ((use_tmask == 1'b0) || (tensor_mask[ii] == 1)) begin
               for (int jj = 0; jj < transform_lines; jj++) begin
                  my_vaddr_next = vaddr + ((ii*transform_lines+jj)*stride);
                  // check for addr overflow: 
                  //my_addr_msb_err = my_vaddr[`VA_MSB] & ~my_vaddr_next[`VA_MSB];  // this is RTL behavior;
                  // Question: Can we detect the extra bit my_vaddr_next[`VA_EXT_MSB] ([48]) ???
                  my_addr_msb_err = my_vaddr_next[`VA_EXT_MSB];   

                  my_vaddr = my_vaddr_next[`VA_MSB:0]; 
                  my_seq_id = (use_coop) ? ((coop_id << 3) + (seq_id % seq_id_max)) : 0;
                  spawn_tload(trans_desc, my_vaddr, my_seq_id, tl_sel, ((ii+scp_dest_line)%48), spawn_req_idx, my_addr_msb_err);
                  seq_id++;
                  spawn_req_idx++;    // probably same as "seq_id"
               end
            end
         end
      end
      else if (transform inside { 5, 6, 7 }) begin
         transform_lines = (transform == 7) ? 16 :  // Transpose32
                           (transform == 6) ? 32 :  // Transpose16
                                              64 ;  // Transpose8

         seq_id_max      = (transform == 7) ? 2 :   // Transpose32
                           (transform == 6) ? 4 :   // Transpose16
                                              8 ;   // Transpose8

         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad: transform_lines=%0d seq_id_max=%0d", transform_lines, seq_id_max))

         // Transpose always downloads all lines regardless of tmask;
         my_vaddr = vaddr;   // starting value
         for (int jj = 0; jj < transform_lines; jj++) begin
            my_vaddr_next = vaddr + (jj*stride);
            // check for addr overflow:
            //my_addr_msb_err = my_vaddr[`VA_MSB] & ~my_vaddr_next[`VA_MSB];  // this is RTL behavior; 
            // Question: Can we detect the extra bit my_vaddr_next[`VA_EXT_MSB] ([48]) ???
            my_addr_msb_err = my_vaddr_next[`VA_EXT_MSB];  

            my_vaddr = my_vaddr_next[`VA_MSB:0]; 
            my_seq_id = (use_coop) ? ((coop_id << 3) + (seq_id % seq_id_max)) : 0;
            spawn_tload(trans_desc, my_vaddr, my_seq_id, tl_sel, scp_dest_line, spawn_req_idx, my_addr_msb_err);
            seq_id++;
            spawn_req_idx++;    // probably same as "seq_id"
         end
      end

      if (tl_sel == 1'b0) begin   // TensorLoad_L1SCP
         trans_desc.set_int_param(pk_tload_resp_line_count_max, spawn_req_idx);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: set pk_tload_resp_line_count_max=%0d", spawn_req_idx))

         if (spawn_req_idx == 0) begin  // meaning after tensor_mask: no line will be filled into L1SCP
            // remove newly-added tload from m_tload_scp_outstanding_q[]: 
            void'(m_tload_scp_outstanding_q.pop_back());
         end
      end

   endfunction: process_tensor_load


   //-----------------------------------------------------------------------------------------------
   //
   // Function: spawn_tload
   //
   //   This function is called when we predict a tensor fill request to the next level cache
   //
   function void spawn_tload(input evl_trans_desc trans_desc,
                             input evl_vaddr_t    vaddr,
                             input int            seq_id,
                             input bit            tl_sel,
                             input int            scp_line_id,
                             input int            spawn_req_idx,
                             input bit            addr_msb_err );
      evl_trans_desc  next_trans_desc;
      string          extra_info;

      // Spawn a child transaction
      $cast(next_trans_desc, trans_desc.clone());

      next_trans_desc.set_cmd(trans_desc.get_cmd());
      next_trans_desc.set_vaddr(vaddr);
// The clone will copy the following fields:
//    next_trans_desc.set_trans_id(trans_desc.get_trans_id());
//    next_trans_desc.set_thread_id(trans_desc.get_thread_id());
      next_trans_desc.set_paddr(evl_paddr_t'(next_trans_desc.get_vaddr()));
      next_trans_desc.set_privilege(trans_desc.get_privilege());
      next_trans_desc.set_bus_minion_coop_mask(trans_desc.get_bus_minion_coop_mask());
      next_trans_desc.set_bus_neigh_coop_mask(trans_desc.get_bus_neigh_coop_mask());
      next_trans_desc.set_bus_coop_id(seq_id);
      next_trans_desc.set_int_param(pk_addr_msb_err, addr_msb_err);
      next_trans_desc.set_cached();

      next_trans_desc.set_int_param(pk_tensorid,   trans_desc.get_int_param(pk_tensorid));
      next_trans_desc.set_int_param(pk_tload_transform,  trans_desc.get_int_param(pk_tload_transform));
      next_trans_desc.set_int_param(pk_tload_scp_dest, trans_desc.get_int_param(pk_tload_scp_dest));      // starting scp_dest_line 
      next_trans_desc.set_int_param(pk_tload_sel,  trans_desc.get_int_param(pk_tload_sel));
      next_trans_desc.set_int_param(pk_tload_byte_offset,  trans_desc.get_int_param(pk_tload_byte_offset));
      next_trans_desc.set_int_param(pk_tload_coop, trans_desc.get_int_param(pk_tload_coop));
      next_trans_desc.set_int_param(pk_tload_num_lines, trans_desc.get_int_param(pk_tload_num_lines));
      if (tl_sel == 1'b1) begin
         next_trans_desc.set_int_param(pk_tload_tenb_line_id, spawn_req_idx);  // for TLoad_TenB
      end else begin
         next_trans_desc.set_int_param(pk_tload_scp_line_id, scp_line_id);     // for TLoad_L1SCP 
         next_trans_desc.set_int_param(pk_tload_spawn_req_idx, spawn_req_idx);   // for TLoad_L1SCP: corresponding to L2 Req/Resp ID (L2 Resp could be out-of-order) 
      end

      extra_info = $sformatf(" (to %0s)", tl_sel ? "TENB" : "L1SCP");

      //// CYE: build a transaction_thread_chain: L1D$ RTL process these TLoads in same order as this "spawn-off" sequence;
      //void'( trans_desc.add_child_desc(next_trans_desc) );  // cause problem!!!

      // CYE: create parent-child relation (but not root-child relation?) between trans_desc and next_trans_desc:
      next_trans_desc.set_parent_desc(trans_desc);
      //next_trans_desc.set_root_desc(trans_desc);

      add_active_mem_desc(next_trans_desc);  // FUTURE (CYE): change to start timer when Req ready to go out onto ET_Link/L2 ???    

      m_tload_s1_queue[tl_sel].push_back(next_trans_desc);

      if (tl_sel == 1'b1) begin  // TenB
            extra_info = $sformatf("%s (tenb_line_id=%0d)", extra_info, next_trans_desc.get_int_param(pk_tload_tenb_line_id));
      end else begin
         if (next_trans_desc.get_int_param(pk_tload_transform) inside {0,1,2}) begin  // either No_transform or Interleave
            extra_info = $sformatf("%s (scp_line_id=%0d)", extra_info, next_trans_desc.get_int_param(pk_tload_scp_line_id));
         end
         // note: for Transpose: all (64, 32, or 16) L2_clines eventually fill into each one of targeted scp_lines.
      end 

      //`evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorLoad req: %0s%0s (spawn_idx=%0d)(addr_msb_err=%0b)", next_trans_desc.sprint_obj(), extra_info, spawn_req_idx, addr_msb_err))
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off individual TensorLoad req: %0s%0s (spawn_idx=%0d)", next_trans_desc.sprint_obj(), extra_info, spawn_req_idx))     // note: we are not testing "addr_msb_err=1" case

   endfunction: spawn_tload



/**********
   //-----------------------------------------------------------------------------------------------
   //
   // Function: restart_tenb
   //
   //   This function is called when receive "tensor_load_restart_tenb" and the TENB FSM is being required to restart.
   //
   function void restart_tenb();
      evl_trans_desc trans_desc;
      `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TENB restart event"))
      fork begin
         wait (m_tenb_flush_done.triggered);
         #0;
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TENB flush done"))

         while (m_tload_s1_queue[1].size() > 0) begin
            trans_desc = m_tload_s1_queue[1].pop_front();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_tload_s1_queue[1]) Cancelling this TenB request: %0s", trans_desc.sprint_obj()))
            delete_active_desc(trans_desc);
         end
         while (m_tload_s2_queue[1].size() > 0) begin
            trans_desc = m_tload_s2_queue[1].pop_front();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_tload_s2_queue[1]) Cancelling this TenB request: %0s", trans_desc.sprint_obj()))
            delete_active_desc(trans_desc);
         end

         for (int ii = m_pending_miss_reqs.size()-1; ii >= 0; ii--) begin
            evl_req_desc pending_desc;

            pending_desc = m_pending_miss_reqs[ii];
            trans_desc   = pending_desc.get_root_trans_desc();
            if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_pending_miss_reqs queue) Cancelling this TenB pending miss request: %0s", pending_desc.sprint_obj()))
               delete_active_desc(trans_desc);
               m_pending_miss_reqs.delete(ii);
            end
         end
      end
      join_none
   endfunction: restart_tenb
***********/


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_restart_tenb
   //
   //   when receive "intpipe.tensor_load_restart_b" and then expect "dcache.tensor_load_1.tenb_flush" to assert (in following cycles) 
   //
   function void process_restart_tenb();
      evl_req_desc    pending_req;
      evl_trans_desc  trans_desc;
      bit     tl_tenb_pend = 0;
      string  tl_tenb_pend_info = "";

      // check if there is any pending/outstanding TL_TenB trans:
      tl_tenb_pend = 1'b0;
      if (m_tload_s1_queue[1].size() > 0) begin
         tl_tenb_pend = 1'b1;
      end
      else if (m_tload_s2_queue[1].size() > 0) begin
         tl_tenb_pend = 1'b1;
      end
      else if (m_pending_miss_reqs.size() > 0) begin
         for (int ii = m_pending_miss_reqs.size()-1; ii >= 0; ii--) begin
            pending_req = m_pending_miss_reqs[ii];
            trans_desc  = pending_req.get_root_trans_desc();
            if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               tl_tenb_pend = 1'b1;
               break;   // out of for() loop
            end
         end 
      end
      tl_tenb_pend_info = (tl_tenb_pend == 1'b1)? "TL_TenB pending" : "No TL_TenB pending"; 
      
      `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TENB_restart event (from IntPipe): %s", tl_tenb_pend_info))

      // set m_restart_tenb_pend: usually we should only have one "restart_tenb" outstanding:
      m_restart_tenb_pend++;

      // usually we get only one "restart_tenb" outstanding; but sometimes could be multiple;
      if(m_restart_tenb_pend > 1) begin
         //sb_error($sformatf("m_restart_tenb_pend=%0d (max should be 1)", m_restart_tenb_pend), null, {}, "", `__FILE__, `__LINE__);
         `evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("WARNING: m_restart_tenb_pend=%0d (max=1)", m_restart_tenb_pend))
      end
   endfunction: process_restart_tenb


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tenb_flush
   //
   //   when receive "dcache.tensor_load_1.tenb_flush" and the TENB FSM is being reset (and then restart).
   //
   function void process_tenb_flush();
      evl_req_desc   pending_req;
      evl_trans_desc trans_desc;
      bit     tl_tenb_pend = 0;
      string  tl_tenb_pend_info = "";

      // check if there is any pending/outstanding TL_TenB trans:
      tl_tenb_pend = 1'b0;
      if (m_tload_s1_queue[1].size() > 0) begin
         tl_tenb_pend = 1'b1;
      end
      else if (m_tload_s2_queue[1].size() > 0) begin
         tl_tenb_pend = 1'b1;
      end
      else if (m_pending_miss_reqs.size() > 0) begin
         for (int ii = m_pending_miss_reqs.size()-1; ii >= 0; ii--) begin
            pending_req = m_pending_miss_reqs[ii];
            trans_desc   = pending_req.get_root_trans_desc();
            if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               tl_tenb_pend = 1'b1;
               break;   // out of for() loop
            end
         end 
      end
      tl_tenb_pend_info = (tl_tenb_pend == 1'b1)? "TL_TenB pending" : "No TL_TenB pending"; 

      `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received (RTL) TENB_flush event (from DCache): %s", tl_tenb_pend_info))

      if(m_restart_tenb_pend == 0) begin
         sb_error($sformatf("Received an unexpected TENB_flush event: m_restart_tenb_pend==0 (no restart_tenb pending)"), null, {}, "", `__FILE__, `__LINE__);
      end else begin
         // clear m_restart_tenb_pend: (could be multiple outstanding)
         m_restart_tenb_pend = 0;

         while (m_tload_s1_queue[1].size() > 0) begin
            trans_desc = m_tload_s1_queue[1].pop_front();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_tload_s1_queue[1]) Cancelling this TenB request: %0s", trans_desc.sprint_obj()))
            delete_active_desc(trans_desc);
         end
         while (m_tload_s2_queue[1].size() > 0) begin
            trans_desc = m_tload_s2_queue[1].pop_front();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_tload_s2_queue[1]) Cancelling this TenB request: %0s", trans_desc.sprint_obj()))
            delete_active_desc(trans_desc);
         end

         for (int ii = m_pending_miss_reqs.size()-1; ii >= 0; ii--) begin
            pending_req = m_pending_miss_reqs[ii];
            trans_desc  = pending_req.get_root_trans_desc();
            if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(m_pending_miss_reqs queue) Cancelling this TenB pending miss request: %0s", pending_req.sprint_obj()))
               delete_active_desc(trans_desc);
               m_pending_miss_reqs.delete(ii);
            end
         end
      end

      // clear m_latest_tload_tenb:
      m_latest_tload_tenb = null;

   endfunction: process_tenb_flush




   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tload_s1 : handle both TensoLoad_L1SCP and TensorLoad_TenB
   //
   //   This function is called when a tensor load is granted access to S1
   //
   function void process_tload_s1(input evl_dcache_tload_s1s2_t rtl_info);
      evl_trans_desc trans_desc;
      logic          tload_sel;    // 0=TL_L1SCP 1=TL_TenB
      bit            l1sb_pma_fault = 1'b0;
      bit            thread_id;
      bit            rtl_tensor_err_mem = 1'b0; 

      tload_sel = rtl_info.tl_sel;
      trans_desc = m_tload_s1_queue[tload_sel].pop_front();

      if (trans_desc == null) begin
         sb_error($sformatf("Received an unexpected S1 tensor_load event (TenB=%0b): m_tload_s1_queue is empty", tload_sel), null, {}, "", `__FILE__, `__LINE__);
         foreach (m_tload_s1_queue[ii]) begin
            foreach (m_tload_s1_queue[ii][jj]) begin
               `evl_log_mdesc(UVM_NONE, m_vmod, get_abstract_name(), m_tload_s1_queue[ii][jj], $sformatf("Tensor Load %0d ready: %0s", ii, m_tload_s1_queue[ii][jj].sprint_obj()))
            end
         end
         return;
      end
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("Tensor Load S1 (%0s) %0s", tload_sel ? "TENB" : "L1SCP", trans_desc.sprint_obj()))

      thread_id = trans_desc.get_thread_id();

      // FUTURE: need to check whether rtl_info(s1) matches trans_desc ... ???

      // for TL_TenB: check against pk_tenb_expect_retart: for RTLMIN-6078 and VERIF-3508:
      if (tload_sel == 1'b1) begin
         // equivalent to SVA ERROR_tl_tenb_enter_ML_CTRL_NoT_Req_when_prev_tfma_not_start_process: 
         if ((trans_desc.get_int_param(pk_tenb_expect_restart, 0) == 1) &&
             (m_tfma_req_but_not_proc_en == 1'b1)) 
         begin
            sb_error($sformatf("for above TensorLoad_TenB: Req arrived and proceeded to S1 stage when (previous) m_tfma_req_but_not_proc_en==1: expected to be restarted/replayed (but was not)"), trans_desc, {}, "", `__FILE__, `__LINE__);
         end

         // equivalent to SVA ERROR_tl_tenb_should_replay_if_prev_tfma_not_start_process:
         if (trans_desc.get_int_param(pk_tenb_expect_restart, 0) == 1) begin
            sb_error($sformatf("for above TensorLoad_TenB: Req arrived when (previous) m_tfma_req_but_not_proc_en==1: expected to be restarted/replayed (but was not) but RTL proceeded to S1 stage ..."), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end

      if (va_to_pa(trans_desc) == 1'b0) begin
         // tensor unit simply waits and will retry so push the descriptor at the front of queue
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("tensor load TLB miss"))
         m_tload_s1_queue[tload_sel].push_front(trans_desc);
         return;
      end

      // PMA check: 
      if (check_pma(trans_desc) == 1'b0) begin
         l1sb_pma_fault = 1'b1;
      end

      // cross_check against RTL: it will not trigger exception;
      l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, 1'b0, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   // xcpt_qaulifier=0

      // cross_check against RTL on tensor_err_mem:
      rtl_tensor_err_mem = (tload_sel == 1'b0)? rtl_info.tld0_err_mem : rtl_info.tld1_err_mem ;
      if (l1sb_pma_fault != rtl_tensor_err_mem) begin
         sb_error($sformatf("TensorLoad: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem), trans_desc, {}, "", `__FILE__, `__LINE__);
         //`evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) TensorLoad: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem))
      end 

      // if L1SB PMA finds access_fault, drop it; 
      if (l1sb_pma_fault == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorLoad got PMA access fault: Dropping trans; set m_TensorError_CSR[%0d][7]=1", thread_id))
         m_TensorError_CSR[thread_id][7] = 1'b1;
         delete_active_desc(trans_desc);

         // if it's TensorLoad: also drop all the younger ones still in m_tload_s1_queue[];
         if (trans_desc.get_cmd() == TENSOR_LOAD) begin
            evl_trans_desc  young_trans_desc;
            while (m_tload_s1_queue[tload_sel].size() > 0) begin
               young_trans_desc = m_tload_s1_queue[tload_sel].pop_front();
               delete_active_desc(young_trans_desc);
            end

            // if it's TensorLoad_L1SCP: find its parent_tload_trans, adjust its pk_tload_resp_line_count_max; 
            // and delete it from m_tload_scp_outstanding_q[] if needed; 
            if (trans_desc.get_int_param(pk_tload_sel) == 0) begin
               evl_trans_desc  parent_tload_trans;
               int  tload_active_req_count;
               int  tload_resp_line_count_max;

               $cast(parent_tload_trans, trans_desc.get_parent_desc());
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for the above canceled TensorLoad_L1SCP: find its parent_tload_trans: %s", parent_tload_trans.sprint_obj())) 
               
               tload_active_req_count = parent_tload_trans.get_int_param(pk_tload_active_req_count);
               parent_tload_trans.set_int_param(pk_tload_resp_line_count_max, tload_active_req_count); 
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("due to above PMA fault: the TensorLoad_L1SCP (and all younger ones) are canceled: adjust its parent_tload_trans: pk_tload_resp_line_count_max=pk_tload_active_req_count=%0d", tload_active_req_count))
             
               if (parent_tload_trans.get_int_param(pk_tload_resp_line_count_max) == 0) begin
                  bit  match_found = 1'b0; 
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_tload_trans, $sformatf("due to above PMA fault: now the parent_tload_trans pk_tload_resp_line_count_max=0: meaning this entire parent TensorLoad_L1SCP is canceled" ))
                  foreach(m_tload_scp_outstanding_q[ii]) begin
                     if (m_tload_scp_outstanding_q[ii] == parent_tload_trans) begin
                        match_found = 1'b1;
                        `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_tload_trans, $sformatf("delete the above TensorLoad_L1SCP (parent_tload_trans) from m_tload_scp_outstanding_q[]: %s", m_tload_scp_outstanding_q[ii].sprint_obj() ))
                        m_tload_scp_outstanding_q.delete(ii);
                        break;
                     end
                  end
                  if (match_found == 1'b0) begin
                     sb_error($sformatf("cannot find the above parent_tload_trans %s in m_tload_scp_outstanding_q[]", parent_tload_trans.sprint_obj()), parent_tload_trans, {}, "", `__FILE__, `__LINE__);
                  end
               end
            end  // if (TL_L1SCP)

           /*** temp: work-in-progress
            // if it's TensorLoad_TenB: find its parent_tload_trans, adjust its pk_tload_resp_line_count_max; 
            // and delete it from m_tload_tenb_outstanding_q[] if needed; 
            if (trans_desc.get_int_param(pk_tload_sel) == 1) begin
               evl_trans_desc  parent_tload_trans;
               int  tload_active_req_count;
               int  tload_resp_line_count_max;

               $cast(parent_tload_trans, trans_desc.get_parent_desc());
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for the above canceled TensorLoad_TenB: find its parent_tload_trans: %s", parent_tload_trans.sprint_obj())) 
              
               // ... ???
 
            end // if (TL_TenB)
           ***/

         end  // if (TensorLoad)

         return;
      end

      // if NO PMA access fault: continue ...

      // for TensorLoad_L1SCP:
      if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 0)) begin
         evl_trans_desc  parent_tload_trans;
         $cast(parent_tload_trans, trans_desc.get_parent_desc());
         parent_tload_trans.set_int_param(pk_tload_active_req_count, parent_tload_trans.get_int_param(pk_tload_active_req_count)); 
      end

      m_tload_s2_queue[tload_sel].push_back(trans_desc);

      // (CYE) for TensorLoad TenB: max outstanding is 6 (sometimes is 4): 
      // note: including those TenB in m_pending_miss_reqs[] and m_active_miss_reqs[];
      // note: see RTL dcache_tensor_load tenb_credits[2:0] for details;
      if (tload_sel == 1) begin
         int  outstanding_tenb_count = 0;
         evl_req_desc   my_req_desc;
         evl_trans_desc my_trans_desc;
         
         outstanding_tenb_count = m_tload_s2_queue[1].size();

         foreach (m_pending_miss_reqs[ii]) begin
            my_req_desc = m_pending_miss_reqs[ii];
            my_trans_desc = my_req_desc.get_root_trans_desc();
            if ((my_trans_desc.get_cmd() == TENSOR_LOAD) && (my_trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               outstanding_tenb_count++; 
            end
         end

         foreach (m_active_miss_reqs[ii]) begin
            my_req_desc = m_active_miss_reqs[ii];
            my_trans_desc = my_req_desc.get_root_trans_desc();
            if ((my_trans_desc.get_cmd() == TENSOR_LOAD) && (my_trans_desc.get_int_param(pk_tload_sel) == 1)) begin
               outstanding_tenb_count++; 
            end
         end

         if (outstanding_tenb_count > 6) begin
            sb_error($sformatf("above TensorLoad_TenB will move from S1 to S2: but outstanding=%0d (max=6)", outstanding_tenb_count), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
         else begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("above TensorLoad_TenB will move from S1 to S2 (outstanding=%0d)", outstanding_tenb_count))
         end
      end

   endfunction: process_tload_s1


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tload_s2
   //
   //   This function is called when a tensor load is granted access to S2
   //
   function void process_tload_s2(input evl_dcache_tload_s1s2_t rtl_info);
      evl_trans_desc   trans_desc;
      evl_req_desc     l2_req_desc;
      bit              tload_sel;    // 0=TL_L1SCP 1=TL_TenB
      bit              cancel;
      int              etlink_id;
      evl_bus_req_id_t tload_req_id;

      tload_sel  = rtl_info.tl_sel;
      etlink_id = rtl_info.etlink_id;
      cancel    = rtl_info.cancel;

      if (cancel == 1'b1) begin
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received S2 tensor_load event: TenB=%b Cancel=1", tload_sel)) 
      end else begin
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received S2 tensor_load event: TenB=%b ETLinkID=%0d", tload_sel, etlink_id)) 
      end 

      trans_desc = m_tload_s2_queue[tload_sel].pop_front();

      if (trans_desc == null) begin
         if (cancel == 1'b1) begin
            `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Received S2 tensor_load event: TenB=%b Cancel=1: m_tload_s2_queue is empty: just drop it", tload_sel))
         end else begin
            sb_error($sformatf("Received an unexpected S2 tensor_load event (TenB=%0b): m_tload_s2_queue is empty", tload_sel), null, {}, "", `__FILE__, `__LINE__);
            foreach (m_tload_s2_queue[ii]) begin
               foreach (m_tload_s2_queue[ii][jj]) begin
                  `evl_log_mdesc(UVM_NONE, m_vmod, get_abstract_name(), m_tload_s2_queue[ii][jj], $sformatf("Tensor Load %0d ready at S2: %0s", ii, m_tload_s2_queue[ii][jj].sprint_obj()))
               end
            end
         end
         return;
      end

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Tensor Load S2 (%0s) %0s%0s", tload_sel ? "TENB" : "L1SCP", trans_desc.sprint_obj(), cancel ? " RTL indicates retry: Pushing back on m_tload_s1_queue[]" : ""))
      if (cancel) begin
         m_tload_s1_queue[tload_sel].push_front(trans_desc);
         return;
      end

      //
      // Predict the Read request on the MISS interface
      //
      $cast(l2_req_desc, m_template_req_desc_neigh.clone());
      trans_desc.copy_to_desc(l2_req_desc);
      void'(trans_desc.add_child_desc(l2_req_desc));
      l2_req_desc.set_trans_id(trans_desc.get_trans_id());
      l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'((trans_desc.get_int_param(pk_tload_coop, 0) == 1) ? BusReadCoop : BusRead));
      l2_req_desc.set_paddr(trans_desc.get_paddr() & const_line_paddr_mask);
      l2_req_desc.set_bus_req_size(6);
      tload_req_id = evl_bus_req_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0 + (tload_sel * 4) + etlink_id);
      l2_req_desc.set_bus_req_id(tload_req_id);

      // setup_bus_req_dependencies(l2_req_desc);
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("Predicting this tensor load request to MISS interface: %0s", l2_req_desc.sprint_obj()))
      m_pending_miss_reqs.push_back(l2_req_desc);

   endfunction: process_tload_s2


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_idle
   //
   task wait_for_idle();
      if (m_active_desc_count <= 0) begin
         return;
      end
      wait (m_active_desc_count <= 0);
   endtask: wait_for_idle


   //-----------------------------------------------------------------------------------------------
   // function: process_cache_ctrl_req
   //   It monitors RTL events of initial cache_ctrl request packet: 
   //
   function process_cache_ctrl_req(evl_cache_control cache_ctrl_info);
      bit [4:0]  cacheop_bw_max; 
      bit [2:0]  cacheop_bw_rate;
      int        cacheop_bw_rate_cyc;
      bit        split;
      bit        scp_enable;
      
      cacheop_bw_max  = cache_ctrl_info.cacheop_max;
      cacheop_bw_rate = cache_ctrl_info.cacheop_reprate;
      cacheop_bw_rate_cyc = 2**(cacheop_bw_rate);
      split        = cache_ctrl_info.d1split;       // new cache_ctrl
      scp_enable   = cache_ctrl_info.scpenable;     // new cache_ctrl: final value will be sampled when Dcache "scratchpad_ctrl" FSM finishes

      `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("CacheOp_Ctrl event: new Req: split=%0d scp_enable=%0d cacheop_bw_rate=%0d(cycles=%0d) cacheop_bw_max=%0d", split, scp_enable, cacheop_bw_rate, cacheop_bw_rate_cyc, cacheop_bw_max))
 
      // separate processing CacheOp_Bandwidth_Ctrl bits (CacheOp_Max and CacheOp_RepRate) and Cache_Cfg_Ctrl bits (ScpEn, Split):
      if ({m_cacheop_bw_rate, m_cacheop_bw_max} != {cacheop_bw_rate, cacheop_bw_max}) begin 
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("CacheOp_Bandwidth_Ctrl change: current cacheop_bw_rate=%0d(cycles=%0d) cacheop_bw_max=%0d; new cacheop_bw_rate=%0d(cycles=%0d) cacheop_bw_max=%0d", m_cacheop_bw_rate, m_cacheop_bw_rate_cyc, m_cacheop_bw_max, cacheop_bw_rate, cacheop_bw_rate_cyc, cacheop_bw_max))

         m_cacheop_bw_rate = cacheop_bw_rate;
         m_cacheop_bw_max  = cacheop_bw_max;
         m_cacheop_bw_rate_cyc = 2**(m_cacheop_bw_rate);

         // Note: It's allowed that SW/Test changes their values in middle of a stream of L2 requests of a CacheOp_L2 instruction.
         // In that case, RTL will use the old values for early part of the stream, and the new values for the later part of the stream.
         // Let's skip the checker when this occurs.
         if (m_cacheop_bw_ctrl_first_cyc_q.size() > 0) begin
            foreach(m_cacheop_bw_ctrl_first_cyc_q[ii]) begin
               m_cacheop_bw_ctrl_first_cyc_q[ii] = -1;        // set it to an invalid number
            end
         end
         // What to do with checking on m_cacheop_bw_max???
      end

      // process Cache_Cfg_Ctrl bits (ScpEn, Split) update: 
      // If Cache_Cfg transitions to a new mode; or first time Cfg_mode after reset;
      if (({m_scp_en, m_split_en} != {scp_enable, split}) ||
          (m_cache_cfg_first_after_reset == 1'b1) )
      begin
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Cache_Cfg_Ctrl change: current scp_en=%b split=%b; new scp_en=%b split=%b", m_scp_en, m_split_en, scp_enable, split))
         // ignore if there is already a cache_cfg_ctrl change in progress; 
         if (m_cache_ctrl_change_in_prog == 1'b0) begin
            m_cache_ctrl_req_mbox.try_put(cache_ctrl_info);     // initial cache_ctrl request packet  // try_put() is non-blocking
         end
      end
   endfunction: process_cache_ctrl_req


   //-----------------------------------------------------------------------------------------------
   // function: process_cache_ctrl_done
   //   It monitors RTL events of second (final) cache_ctrl packet (when "Done")
   //
   function process_cache_ctrl_done(evl_cache_control cache_ctrl_info);   
      m_cache_ctrl_done_mbox.try_put(cache_ctrl_info);   // second (final) cache_ctrl packet (when "Done")   // try_put() is non-blocking
      ->m_cache_ctrl_done;
   endfunction: process_cache_ctrl_done


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_cache_ctrl: state machine handle dcache_config_partition changes
   //
   //   This task monitors cache control events and modifies the cache configuration accordingly.
   //
   task process_cache_ctrl();
      fork
         begin
            evl_cache_control ctrl;
            bit split;
            bit scp_enable;
            evl_cache_control final_ctrl;
            bit final_split;
            bit final_scp_enable;

            forever begin
               m_cache_ctrl_req_mbox.get(ctrl);    // initial cache_ctrl request packet
               split          = ctrl.d1split;       // new cache_ctrl
               scp_enable     = ctrl.scpenable;     // new cache_ctrl: final value will be sampled when Dcache "scratchpad_ctrl" FSM finishes 
                                                    // (reach zero_count_end and state=SCP_CTRL_All_Set)

               //
               // Check for illegal cache mode transitions: ??? Is this still true ???
               //
               if (!({m_scp_en, m_split_en, scp_enable, split} inside {4'b0000, 4'b0001, 4'b0100, 4'b0101, 4'b0111, 4'b1111, 4'b1101, 4'b1100})) begin
                  sb_error($sformatf("Illegal Cache_Ctrl event: current SCPEnable=%0d && D1Split=%0d; new SCPEnable=%0d && D1Split=%0d", m_scp_en, m_split_en, scp_enable, split), null, {}, "", `__FILE__, `__LINE__);
               end

               //
               // If Cache_Cfg transitions to a new mode; or first time Cfg_mode after reset;
               //
               if (({m_scp_en, m_split_en} != {scp_enable, split}) ||
                   (m_cache_cfg_first_after_reset == 1'b1) )
               begin
                  m_cache_ctrl_change_in_prog <= 1'b1;
                  if (m_cache_cfg_first_after_reset == 1'b1) begin
                     m_cache_cfg_first_after_reset = 1'b0;
                  end

                  `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Cache_Cfg_Ctrl event: Request: changing mode from %0s to %0s", get_cache_mode_name(), get_cache_mode_name(int'({scp_enable, split})) ))

                  // when "scratchpad_ctrl" FSM starts "zero_count":
                  wait (m_cache_ctrl_start_zero.triggered);

                  // Check that everything is done when we change mode (exception: cacheop messages to next level cache, evictions)
                  begin: check_dcache_idle
                     evl_req_desc all_reqs[$];

                     all_reqs = {m_pending_miss_reqs, m_pending_evict_reqs, m_active_miss_reqs, m_active_evict_reqs};
                     foreach (m_active_mem_descs[ii]) begin
                        if (!(m_active_mem_descs[ii].get_cmd() inside {EVICT, CACHE_EVICT, CACHE_FLUSH, CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_PREFETCH})) begin
                           sb_error($sformatf("DCache changing mode (start zero) while transaction %0s was outstanding!", m_active_mem_descs[ii].sprint_obj()), m_active_mem_descs[ii], {}, "", `__FILE__, `__LINE__);
                        end
                     end
                     foreach (all_reqs[ii]) begin
                        if ((all_reqs[ii].bus_cmd_is_cache_op() == 1'b0) && !(all_reqs[ii].get_root_trans_desc().get_cmd() inside {EVICT, CACHE_EVICT, CACHE_FLUSH, CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_PREFETCH})) begin
                           sb_error($sformatf("DCache changing mode (start zero) while request %0s was outstanding!", all_reqs[ii].sprint_obj()), all_reqs[ii], {}, "", `__FILE__, `__LINE__);
                        end
                     end
                  end   // check_dcache_idle

                  // when "scratchpad_ctrl" FSM completes SCP_CTRL_Zero and reaches SCP_CTRL_All_Set: 
                  // let's to sample again {split and scp_en} from the latest "cache_ctrl":
                  wait (m_cache_ctrl_done.triggered);
                  m_cache_ctrl_change_in_prog <= 1'b0;     // important to use non-blocking to match RTL behavior
                  m_cache_ctrl_done_mbox.get(final_ctrl);         // sample the latest (new) cache_ctrl packet (final value) 
                  final_split          = ctrl.d1split; 
                  final_scp_enable     = ctrl.scpenable;

                  `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Cache_Cfg_Ctrl event: Done: changed mode from %0s to %0s", get_cache_mode_name(), get_cache_mode_name(int'({final_scp_enable, final_split}))))

                  // VERIF-3295: check {final_scp_enable, final_split} must be same as {scp_enable, split} (cache_ctrl start req value):
                  if ({final_scp_enable, final_split} != {scp_enable, split}) begin 
                     sb_error($sformatf("during above Dcache config mode change process: new {scp_enable, split} value changed: start=%2b end=%2b", {scp_enable, split}, {final_scp_enable, final_split}), null,  {}, "", `__FILE__, `__LINE__);
                  end

                  // Thread 0's cache is reset for all transitions
                  m_cache[0].reset();
                  // Thread 1's cache is reset when split changes
                  if (m_split_en != final_split) begin
                     m_cache[1].reset();
                  end

                  m_scp_en   = final_scp_enable;
                  m_split_en = final_split;

                  if (m_scp_en == 1'b1) begin // in SCP mode, thread 0 has 2 sets
                     m_cache[0].set_cache_config(m_num_ways, m_thread_set_mask_scp[0]);
                  end
                  else if (m_split_en == 1'b1) begin // in split mode, thread 0 has 8 sets
                     m_cache[0].set_cache_config(m_num_ways, m_thread_set_mask_split[0]);
                  end
                  else begin // in shared mode, we use all 16 sets
                     m_cache[0].set_cache_config(m_num_ways, m_cache_set_mask);
                  end
               end
            end
         end
      join_none
   endtask: process_cache_ctrl





   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_mode_name
   //
   function string get_cache_mode_name(input int mode_in = -1);
      int mode;
      mode = (mode_in == -1) ? int'({m_scp_en, m_split_en}) : mode_in;
      case (mode)
         0: return "Shared (default)";
         1: return "Split";
         3: return "Scratchpad";
         default: return "Unknown";
      endcase
   endfunction: get_cache_mode_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache
   //
   function evl_minion_dcache #(CACHE_TYPE) get_cache(input evl_mem_desc mem_desc);
      evl_thread_id_t tid = mem_desc.get_thread_id();
      return (m_split_en) ? m_cache[tid] : m_cache[0];
   endfunction: get_cache


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set_addr
   //
   function evl_paddr_t get_cache_set_addr(input evl_mem_desc mem_desc);
      evl_thread_id_t tid = mem_desc.get_thread_id();
      case ({m_scp_en, m_split_en})
         2'b00: begin // Shared mode
            return (mem_desc.get_paddr() & m_cache[0].get_index_mask()) >> `EVL_LINE_ADDR_LSB;
         end
         2'b01: begin // Split mode
            return ((mem_desc.get_paddr() & m_cache[tid].get_index_mask()) >> `EVL_LINE_ADDR_LSB) + m_threads_split_offset[tid];
         end
         default: begin // SCP mode
            return ((mem_desc.get_paddr() & m_cache[tid].get_index_mask()) >> `EVL_LINE_ADDR_LSB) + m_threads_scp_offset[tid];
         end
      endcase
   endfunction: get_cache_set_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cop_level_name
   //
   //   This function converts a cacheop destination level to a string
   //
   function string get_cop_level_name(input evl_bus_cop_dest_level_t cop_lvl);
      case (cop_lvl)
         0:       return "L1";
         1:       return "L2";
         2:       return "L3";
         3:       return "Mem";
         default: return "--";
      endcase
   endfunction: get_cop_level_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_cop_req
   //
   //   This function is called when a cache operation is accepted by the dcache. This function
   //   decodes the attributes of that cacheop and creates the necessary transaction descriptors.
   //
   function void process_cop_req(input evl_dcache_cop_req_t rtl_info);
      evl_bus_cop_dest_level_t dest_level;
      evl_trans_desc           trans_desc;
      evl_trans_desc           next_trans_desc;
      evl_vaddr_t              addr;
      evl_vaddr_t              stride;
      evl_way_t                way;
      evl_thread_id_t          thread_id;
      evl_cmd_t                cmd;
      string                   extra_info;
      int                      repeat_count;
      int                      actual_rep_cnt;  // after consider tmask[] value
      int                      tensor_mask;
      bit                      use_tmask;
      bit                      wait_id;
      bit                      way_auto;
      bit                      l2_scp_fill;   // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
      evl_paddr_t              l2_scp_addr;
      evl_vaddr_t              insn_pc;
      string                   set_way_info;
      bit                      pk_cop_first_line_is_already_set = 1'b0;
      bit [22:0]               l2_scp_offset;     // = l2_scp_addr[22:0]

    //evl_vaddr_t              my_vaddr_next;     // [47:0] 
      bit [`VA_EXT_MSB:0]      my_vaddr_next;     // [48:0] one extra bit (for addr overflow)
      bit                      my_addr_msb_err;   // addr overflow

      addr          = evl_vaddr_t'(rtl_info.cop_req.addr & const_line_vaddr_mask);
      way           = evl_way_t'(rtl_info.cop_req.way[`DCACHE_WAY_RANGE]);
      stride        = evl_vaddr_t'(rtl_info.cop_req.stride & const_line_vaddr_mask);
      wait_id       = bit'(rtl_info.cop_req.stride[0]);
      repeat_count  = int'(rtl_info.cop_req.count+1);
      tensor_mask   = int'(rtl_info.cop_req.tmask_bits);
      use_tmask     = bit'(rtl_info.cop_req.use_tmask);
      thread_id     = evl_thread_id_t'(rtl_info.cop_req.thread_id);
      dest_level    = evl_bus_cop_dest_level_t'(rtl_info.cop_req.dest_level);
      l2_scp_fill   = bit'(rtl_info.cop_req.l2_scp_fill);    // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
      l2_scp_addr   = evl_paddr_t'('0);
      l2_scp_offset = l2_scp_addr[22:0];
      insn_pc       = rtl_info.pc;
      extra_info    = "";

      case (rtl_info.cop_req.cmd)
         cache_op_Lock_SW   : cmd = CACHE_LOCK_SW;     // Hard locks a Physical address to a specified way (set determined by current indexing function)
         cache_op_Unlock_SW : cmd = CACHE_UNLOCK_SW;   // Unlocks a specified set/way
         cache_op_Lock_VA   : cmd = CACHE_LOCK;        // Soft locks a Virtual address
         cache_op_Unlock_VA : cmd = CACHE_UNLOCK;      // Unlocks a Virtual address if present and soft locked
         cache_op_Flush_SW  : cmd = CACHE_FLUSH_SW;    // Flushes a Set/Way of the cache hierarchy
         cache_op_Evict_SW  : cmd = CACHE_EVICT_SW;    // Evicts a Set/Way of the cache hierarchy
         cache_op_Pref      : cmd = CACHE_PREFETCH;    // Prefetching (include CacheOp_Pref_L2SCP)
         cache_op_Flush_VA  : cmd = CACHE_FLUSH;       // Flushes a VA of the cache hierarchy
         cache_op_Evict_VA  : cmd = CACHE_EVICT;       // Evicts a VA of the cache hierarchy
         default : begin
            sb_error($sformatf("minion_dcache_scoreboard: Unsupported CacheOp 0x%0x!", rtl_info.cop_req.cmd), null, {}, "", `__FILE__, `__LINE__);
            return;
         end
      endcase

      if (l2_scp_fill) begin    // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
         if (cmd == CACHE_PREFETCH) begin
            cmd = CACHE_SCP_FILL;    // better name it as "CACHE_L2SCP_FILL" or "CACHE_PREF_L2SCP"
         end
         else begin
            sb_error($sformatf("L2 SCP Fill is not expected to be set for command %0p", rtl_info.cop_req.cmd), null, {}, "", `__FILE__, `__LINE__);
         end
      end

      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_cmd(cmd);
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_vaddr(evl_vaddr_t'(addr));
      trans_desc.set_paddr(evl_paddr_t'(addr));   // FUTURE: shoulg get from TLB VA->PA (for most VA-based CacheOp)
      trans_desc.set_privilege(((rtl_info.vm_status.mprv == 1'b1) && !(cmd inside {CACHE_LOCK_SW, CACHE_UNLOCK_SW})) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);

      //
      // Lock_sw does not have a repeat count, override it in case RTL doesn't set it properly
      //
      if (cmd inside {CACHE_LOCK_SW, CACHE_UNLOCK_SW}) begin
         repeat_count = 1;
      end

      //
      // Gather the info we're going to print
      //
      extra_info = $sformatf("%0s (thread %0d)", extra_info, thread_id);

      if (cmd inside {CACHE_PREFETCH, CACHE_FLUSH, CACHE_EVICT, CACHE_LOCK, CACHE_UNLOCK}) begin
         extra_info = $sformatf("%0s (VA 0x%0x)", extra_info, trans_desc.get_vaddr());
      end
      else if (cmd inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW}) begin
         //extra_info = $sformatf("%0s (set %0d)", extra_info, get_cache_set_addr(trans_desc));  // (CYE) wrong? should change to code below? see PRM-8 EVICT_SW, FLUSH_SW, UNLOCK_SW.
         extra_info = $sformatf("%0s (set %0d)", extra_info, ((evl_paddr_t'(trans_desc.get_vaddr) >> `EVL_LINE_ADDR_LSB) % 16));
      end
      else if (cmd == CACHE_LOCK_SW) begin
         extra_info = $sformatf("%0s (PA 0x%0x)", extra_info, evl_paddr_t'(trans_desc.get_vaddr()));
      end

      if (cmd inside {CACHE_LOCK_SW, CACHE_UNLOCK_SW, CACHE_FLUSH_SW, CACHE_EVICT_SW}) begin
         extra_info = $sformatf("%0s (way %0s)", extra_info, get_way_name(way));
      end
      if (cmd == CACHE_PREFETCH) begin
         extra_info = $sformatf("%0s (to %0s)", extra_info, get_cop_level_name(dest_level));
      end
      if (cmd inside {CACHE_FLUSH_SW, CACHE_FLUSH, CACHE_EVICT_SW, CACHE_EVICT}) begin
         extra_info = $sformatf("%0s (L1 -> %0s)", extra_info, get_cop_level_name(dest_level));
      end
      if ((cmd == CACHE_SCP_FILL) && (l2_scp_fill == 1'b1)) begin
         extra_info = $sformatf("%0s (l2_scp_addr 0x%h)(l2scp_dest_start_ln 0x%h)", extra_info, l2_scp_addr, rtl_info.cop_req.l2_scp_dest); 
      end 
      if (repeat_count > 1) begin
         extra_info = $sformatf("%0s (repeat %0d stride 0x%0x)", extra_info, repeat_count, stride);
      end
      if (use_tmask == 1'b1) begin
         extra_info = $sformatf("%0s (tensor mask 0x%0x)", extra_info, tensor_mask);
      end
       
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("New request (thread %0d) (PC=0x%12x) CacheOp event: %0s%0s", trans_desc.get_thread_id(), insn_pc, trans_desc.sprint_obj(), extra_info))

      // Set start / dest levels: (always start from L1)
      trans_desc.set_bus_cop_start_level(0);         // alway start_level = L1
      trans_desc.set_bus_cop_dest_level(dest_level);
      if (cmd == CACHE_SCP_FILL) begin      // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
         trans_desc.set_bus_cop_dest_level(0);      // why set its dest_level = L1 ???
      end

      // Cacheops are always cached by definition, and the size is a cacheline
      trans_desc.set_cached();
      trans_desc.set_cmd_length(`EVL_LINE_BYTES);

      //
      // Check for NOP cases:
      //   - Evicts/Flushes with dest = L1 (note: its start_level is always L1)
      //   - Prefetch with dest = Mem
      //
      if (cmd inside {CACHE_FLUSH, CACHE_EVICT, CACHE_FLUSH_SW, CACHE_EVICT_SW}) begin
         if ((trans_desc.get_bus_cop_dest_level() == 0) || (trans_desc.get_bus_cop_dest_level() <= trans_desc.get_bus_cop_start_level())) begin
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("Dropping CacheOp because of Dst/Src: %0s", trans_desc.sprint_obj()))
            return;
         end
      end
      if ((cmd == CACHE_PREFETCH) && (trans_desc.get_bus_cop_dest_level() > 0))begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("Dropping prefetch to memory: %0s", trans_desc.sprint_obj()))
         return;
      end

      //
      // We're gonna spawn a new (cloned) transaction for each repeat count
      // Most attribute will be inherited from the initial descriptor but we'll
      // need to set the ones that aren't
      //
      actual_rep_cnt = 0;
      my_vaddr_next = addr;   // starting value
      my_addr_msb_err = 1'b0;
      for (int ii = 0; ii < (repeat_count); ii++) begin
         // skip if required to use_tmask but masked out by the tensor_mask CSR
         if ((use_tmask == 1'b1) && (tensor_mask[ii] == 0)) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("spawn off (ii=%0d): use_tmask=1 but tensor_mask[ii]=0: skip", ii))
         end

         // Only do stuff if not masked by the tensor_mask CSR
         else if ((use_tmask == 1'b0) || (tensor_mask[ii] == 1)) begin

            $cast(next_trans_desc, trans_desc.clone());
            next_trans_desc.set_cmd(cmd);
            next_trans_desc.set_privilege(trans_desc.get_privilege());

            // CYE: to make original CacheOp trans_desc to be the root_desc of all spawn-off "next_trans_desc";
            void'(trans_desc.add_child_desc(next_trans_desc));
 
            // CYE: for first line:
            if (pk_cop_first_line_is_already_set == 1'b0) begin
               next_trans_desc.set_int_param(pk_cop_first_line, 1);
               pk_cop_first_line_is_already_set = 1'b1;
            end

            actual_rep_cnt++; 

            // L2 Scratchpad Fill holds the L2 scratchpad address in its address field,
            // and holds the cacheline address to load in the data field, abstracted in 'bus_scp_fill_addr' field;
            // CYE: Question: 
            //      Why do we assign "addr" to bus_scp_fill_addr, and assign "l2_scp_addr" to vaddr/paddr?
            //      Because that's what the ET_Link Mon/SB expects, otherwise we will get mismatch.
            if (cmd == CACHE_SCP_FILL) begin
               next_trans_desc.set_bus_scp_fill_addr(addr);
               next_trans_desc.set_vaddr(evl_vaddr_t'(l2_scp_addr));
               next_trans_desc.set_paddr(evl_paddr_t'(l2_scp_addr));
            end
            else begin
               next_trans_desc.set_vaddr(evl_vaddr_t'(addr));
               next_trans_desc.set_paddr(evl_paddr_t'(addr));
            end

            // Some cacheops explicitely select the way they work on, record it here
            if (cmd inside {CACHE_LOCK_SW, CACHE_UNLOCK_SW, CACHE_FLUSH_SW, CACHE_EVICT_SW}) begin
               next_trans_desc.set_int_param(pk_cop_way_sel, way);
            end

            next_trans_desc.set_int_param(pk_tensorid, wait_id);
            next_trans_desc.set_int_param(pk_addr_msb_err, my_addr_msb_err);
            add_active_mem_desc(next_trans_desc);

            set_way_info = "";
            if (cmd inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW}) begin   // these CacheOp_SW: "set" is provided (as if in "Shared-mode", regardless of actual Cache mode)
               set_way_info = $sformatf(" (set=%0d way=%0d)", ((addr >> `EVL_LINE_ADDR_LSB) % 16), way);
            end
            else if (cmd inside {CACHE_LOCK_SW}) begin                                // Lock_SW comes with actual PA
               set_way_info = $sformatf(" (set=%0d way=%0d)", get_cache_set_addr(next_trans_desc), way);
            end

            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("spawn off: queuing CacheOp: %0s%0s", next_trans_desc.sprint_obj(), set_way_info))

            // CacheOps that don't impact the L1 are handled by a specific state machine (RTL CacheOp_L2_FSM)
            // which will only inject at S1 to do address translation (skip S0 stage?) 
            if (    (cmd == CACHE_SCP_FILL)                                   // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
                || ((cmd == CACHE_PREFETCH) && (trans_desc.get_bus_cop_dest_level() > 0))
               )
            begin
               m_cacheop_l2_s1_queue.push_back(next_trans_desc); // for CacheOp_L2_FSM (RTL) involving L2 only
            end
            else begin
               m_cacheop_s0_queue.push_back(next_trans_desc); // for CacheOp_FSM involving L1 (RTL): must go to S0 (bcoz it involves ReplayQ like regular incoming IntPipe Req)
            end

            if (cmd inside {CACHE_SCP_FILL, CACHE_PREFETCH, CACHE_FLUSH, CACHE_EVICT, CACHE_LOCK, CACHE_UNLOCK}) begin
               // If virtual memory is enabled, create a potential page table walk request
               // unless we are in machine mode, which doesn't go through translation
               if ((rtl_info.vm_status.prv != 2'b11) && (rtl_info.satp_info.mode != 0)) begin
                  evl_ptw_desc ptw_req;
                  ptw_req = new();
                  ptw_req.set_vaddr(trans_desc.get_vaddr());
                  ptw_req.set_vm_mode(rtl_info.satp_info.mode);
                  ptw_req.set_root_ppn(rtl_info.satp_info.ppn);
                  ptw_req.set_privilege(trans_desc.get_privilege());
                  trans_desc.set_object_param(pk_tlb_req, ptw_req);
               end
            end
         end

         //
         // Increment the address: for next one in this for() loop;
         //
         if (cmd inside {CACHE_EVICT_SW, CACHE_FLUSH_SW}) begin
            // go to the next set (regardless of cache mode) and wrap around to set 0 (next way) after set 15
            addr = (addr + (1 << `EVL_LINE_ADDR_LSB)) & m_cache_set_mask;
            if (addr == 0) begin
               way = evl_way_t'((int'(way) + 1) % m_cache[0].get_num_ways());
            end
            // will never have addr overflow:
            my_addr_msb_err = 1'b0;      // for next one
         end
         else begin
            //addr += stride;
            my_vaddr_next += stride; 
            // check for addr overflow: 
            //my_addr_msb_err = addr[`VA_MSB] & ~my_vaddr_next[`VA_MSB];  // this is RTL behavior 
            // Question: Can we detect the extra bit my_vaddr_next[`VA_EXT_MSB] ([48]) ???
            my_addr_msb_err = my_vaddr_next[`VA_EXT_MSB];

            addr = my_vaddr_next[`VA_MSB:0];
            //l2_scp_addr += `EVL_LINE_BYTES;      // `define EVL_LINE_BYTES 'h40
            //(CYE) code above will NOT work when "l2_scp_addr" reaches its max and wraparound; change to code below:
            l2_scp_offset += `EVL_LINE_BYTES;      // `define EVL_LINE_BYTES 'h40
            l2_scp_addr[22:0] = l2_scp_offset;
         end
      end  // for (int ii = 0; ii < (repeat_count); ii++) begin

      if (actual_rep_cnt > 0) begin
         // set up some info in the last (end) "child" request:
         next_trans_desc.set_int_param(pk_cop_end_line, 1);
         next_trans_desc.set_int_param(pk_cop_actual_rep_cnt, actual_rep_cnt);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), next_trans_desc, $sformatf("for the above last(end) of stream spawn-off CacheOp %s: set pk_cop_end_line=1 pk_cop_actual_rep_cnt=%0d", next_trans_desc.sprint_obj(), actual_rep_cnt))

         trans_desc.set_int_param(pk_cop_actual_rep_cnt, actual_rep_cnt);
      end

   endfunction: process_cop_req



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s0_cop
   //
   //   This function is called when a cacheop is granted access to the dcache pipeline (reading the
   //   tag RAMs) and predicts the outcome of the transaction
   //
   function void process_s0_cop(input evl_core_dcache_cop_s0_t rtl_info);
      evl_trans_desc    trans_desc;

      trans_desc = m_cacheop_s0_queue.pop_front();
      if (trans_desc == null) begin
         sb_error($sformatf("Received a CacheOp S0 event for addr 0x%0x but nothing was expected", rtl_info.addr), null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received a CacheOp S0 event (vaddr=0x%h) for Trans: %0s", rtl_info.addr, trans_desc.sprint_obj()))

      if (!(trans_desc.get_cmd() inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW, CACHE_LOCK_SW})) begin
         if ((rtl_info.addr & const_line_vaddr_mask) != trans_desc.get_vaddr()) begin
            sb_error($sformatf("(S0) CacheOp address in S0 was expected to be 0x%0x, not 0x%0x", trans_desc.get_vaddr(), (rtl_info.addr & const_line_vaddr_mask)), null, {}, "", `__FILE__, `__LINE__);
            return;
         end
      end

      //
      // Prefetches to L1 are just inserted in the pipe as loads to x0
      //
      if ((trans_desc.get_cmd() == CACHE_PREFETCH) && (trans_desc.get_bus_cop_dest_level() == 0)) begin
         // VERIF-3630: special case: in SP, when Pref_L1 targets Uncached high mem region, Dcache RTL sends it out as UC_Load Req 
         // (which is undesired, should be flagged as PMA access_fault).
         // As a result, as a workaround in L1D_SB, we have to assign a default "size/type" (dcache_type_D, DWord, 8-byte) to Pref_L1 req 
         // (in order to match RTL behavior).
         trans_desc.set_int_param(pk_access_type, dcache_type_D);
         trans_desc.set_cmd_length(8);  // 8-byte, DWord
         trans_desc.set_int_param(pk_insn_length, trans_desc.get_cmd_length()); 

         m_s1_queue.push_back(trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) CacheOp Prefetch_L1 push into m_s1_queue: %0s", trans_desc.sprint_obj()))
      end else begin
         m_cacheop_s1_queue.push_back(trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) CacheOp (excluding Pref_L1) push into m_cacheop_s1_queue: %0s", trans_desc.sprint_obj()))
      end

   endfunction: process_s0_cop



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s1_cop  (8/31/2019: CYE/Charles)
   //
   //   This function is called when a cacheop is granted access to the dcache pipeline (reading the
   //   tag RAMs) and predicts the outcome of the transaction
   // Note: does not include CacheOp_Pref_L1 (which goes to normal S1 pipeline function;
   //  
   function void process_s1_cop(input evl_core_dcache_cop_s1_t rtl_info);
      evl_trans_desc    trans_desc;
      bit               tlb_hit;
      bit               l1sb_pma_fault = 1'b0;
      bit               thread_id;
      bit               rtl_tensor_err_mem = 1'b0; 

      trans_desc = m_cacheop_s1_queue.pop_front();
      if (trans_desc == null) begin
         sb_error($sformatf("Received a cacheop S1 event for addr 0x%0x but nothing was expected", rtl_info.addr), null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received a CacheOp S1 event for Trans: %0s", trans_desc.sprint_obj()))

      if (!(trans_desc.get_cmd() inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW, CACHE_LOCK_SW})) begin
         if ((rtl_info.addr & const_line_vaddr_mask) != trans_desc.get_vaddr()) begin
            sb_error($sformatf("CacheOp address in S1 was expected to be 0x%0x, not 0x%0x", trans_desc.get_vaddr(), (rtl_info.addr & const_line_vaddr_mask)), null, {}, "", `__FILE__, `__LINE__);
            return;
         end
      end

      thread_id = trans_desc.get_thread_id(); 

      // (CYE) 6/09/2020: ??? Do we really need this code below ???  Should we simply always let it go to TLB Lookup? 
      if (m_tlb_ptw_in_process[thread_id] == 1'b1) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above CacheOp: but m_tlb_ptw_in_process[%0d]=1 (due to previous trans TLB miss PTW still pending): flush pipe, CacheOp FSM will retry (from S0)", thread_id ))
         //m_cacheop_s1_queue.push_front(trans_desc);
         m_cacheop_s0_queue.push_front(trans_desc);  // RTL retry from S0 stage ???
         return;
      end

      tlb_hit = 1'b1;
      if (trans_desc.cmd_is_idx_cacheop() == 1'b0) begin
         tlb_hit = va_to_pa(trans_desc);
      end

      // If TLB missed, push the cacheop back to the front of the ready ops - It will be retried later
      if (tlb_hit == 1'b0) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above CacheOp: TLB miss: flush pipe, CacheOp FSM will retry (from S0)" ))
         //m_cacheop_s1_queue.push_front(trans_desc);
         m_cacheop_s0_queue.push_front(trans_desc);  // RTL retry from S0 stage ???
         return;
      end

      //
      // after TLB hit:
      // Check PMA for access faults: ignore CacheOp SetWay (except for Lock_SW which comes with an actual PA)
      //
      if (!(trans_desc.get_cmd() inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW})) begin   // note: Not exclude "CACHE_LOCK_SW" which comes with PA
         if (check_pma(trans_desc) == 1'b0) begin
            l1sb_pma_fault = 1'b1;
         end

         // cross_check to RTL:  it will not trigger exception;
         l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, 1'b0, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   // xcpt_qaulifier=0

         // cross_check against RTL on tensor_err_mem:
         rtl_tensor_err_mem = rtl_info.err_mem; 
         if (l1sb_pma_fault != rtl_tensor_err_mem) begin
            sb_error($sformatf("CacheOp: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem), trans_desc, {}, "", `__FILE__, `__LINE__);
            //`evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) CacheOp: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem))
         end 

         // if L1SB PMA finds access_fault, drop it; also drop all the younger ones still in m_cacheop_s1_queue[];
         if (l1sb_pma_fault == 1'b1) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("CacheOp got PMA access fault: Dropping trans; set m_TensorError_CSR[%0d][7]=1", thread_id))
            m_TensorError_CSR[thread_id][7] = 1'b1;

            delete_active_cacheops_s1_queue(trans_desc);   
               // the above function deletes "trans_desc" and also all rest trans which spawned from the same CacheOp Inst;

            // note: important: also delete all rest spawn-off trans from same CacheOp Inst remaining in m_cacheop_s0_queue[]:
            delete_active_cacheops_s0_queue(trans_desc);   
            return;
         end
      end

      m_cacheop_s2_queue.push_back(trans_desc);

   endfunction: process_s1_cop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s1_cop_l2   (CYE/Charles: added) (only involving L2/L3 but not L1)
   //
   //   This function is called when a cacheop from the L2 cop FSM is granted access to the S1 stage
   //   of the dcache pipeline for memory translation and predicts the outcome of the transaction
   //
   function void process_s1_cop_l2(input evl_core_dcache_cop_s1_t rtl_info);
      evl_trans_desc    trans_desc;
      bit               tlb_hit;
      bit               l1sb_pma_fault = 1'b0;
      bit               thread_id;
      bit               rtl_tensor_err_mem = 1'b0;

      //`evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) received CacheOp RTL S1 event: rtl_info.addr=0x%h const_line_vaddr_mask=0x%h", rtl_info.addr, const_line_vaddr_mask))

      trans_desc = m_cacheop_l2_s1_queue.pop_front();
      if (trans_desc == null) begin
         sb_error($sformatf("Received a cacheop S1 event for addr 0x%0x but nothing was expected", rtl_info.addr), null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received a CacheOp_L2 S1 event for Trans: %0s", trans_desc.sprint_obj()))

      thread_id = trans_desc.get_thread_id();

      if (trans_desc.get_cmd() != CACHE_SCP_FILL) begin
         if ((rtl_info.addr & const_line_vaddr_mask) != trans_desc.get_vaddr()) begin
            sb_error($sformatf("CacheOp_L2 address in S1 was expected to be 0x%0x, not 0x%0x", trans_desc.get_vaddr(), (rtl_info.addr & const_line_vaddr_mask)), null, {}, "", `__FILE__, `__LINE__);
            //return;   // (CYE) let the function continue even if detect this error (?)
         end
      end

      tlb_hit = va_to_pa(trans_desc);
      // Charles: Question: Why not handle the case if(tlb_hit==0)... here right away???  

      // only when (tlb_hit == 1):
      //
      // Check PMA for access faults
      //
      if (check_pma(trans_desc) == 1'b0) begin
         l1sb_pma_fault = 1'b1;
      end

      // cross_check to RTL: 
      //  for Prefetch_L2L3: if PMA fault (signal s1_pma_resp_data.access_fault asserts),
      //                     just drop it, but will not send back core_xcpt 
      l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, 1'b0, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   // xcpt_qaulifier=0

         // cross_check against RTL on tensor_err_mem:
         rtl_tensor_err_mem = rtl_info.err_mem; 
         if (l1sb_pma_fault != rtl_tensor_err_mem) begin
            sb_error($sformatf("CacheOp_L2: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem), trans_desc, {}, "", `__FILE__, `__LINE__);
            //`evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING) CacheOp_L2: tensor_err_mem mismatch: L1SB=%0b RTL=%0b", l1sb_pma_fault, rtl_tensor_err_mem))
         end 


      // if L1SB PMA finds access_fault, drop it; also drop all the younger ones still in m_cacheop_l2_s1_queue[];
      if (l1sb_pma_fault == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("CacheOp_L2 got PMA access fault: Dropping this trans (and all the rest spawn-off trans from the same CacheOp Inst); set m_TensorError_CSR[%0d][7]=1", thread_id))
         m_TensorError_CSR[thread_id][7] = 1'b1; 

         delete_active_cacheops_l2_s1_queue(trans_desc);    // (CYE) VERIF-2128
            // the above function deletes "trans_desc" and also all the rest descriptors which spawned from the same CacheOp request 

         // note: important: also delete all rest spawn-off trans from same CacheOp Inst remaining in m_cacheop_s0_queue[]:
         delete_active_cacheops_s0_queue(trans_desc);  

         if (trans_desc.get_cmd() == CACHE_SCP_FILL) begin  // CacheOp_Prefetch_L2SCP (TensorLoad_L2SCP) 
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("The above PMA fault is for a TensorLoad_L2SCP (CacheOp_Pref_L2SCP): set m_TensorError_CSR[%0d][7]=1", thread_id))
            m_TensorError_CSR[thread_id][7] = 1'b1; 
         end

         // VERIF-3402: 
         // need to clear its "first_cyc" entry in m_cacheop_bw_ctrl_first_cyc_q[] (must be the last entry);
         if ((((trans_desc.get_cmd() == CACHE_PREFETCH) &&
               (trans_desc.get_bus_cop_dest_level() > 0)) ||   // CacheOp_Pref_L2L3
              ( trans_desc.get_cmd() == CACHE_SCP_FILL ) )     // Tensor_Load_L2SCP (CacheOp_Pref_L2SCP)
             &&
             ~((trans_desc.get_int_param(pk_cop_first_line, 0) == 1) &&
               (trans_desc.get_int_param(pk_cop_end_line, 0) == 1))      // exclude single-cline case 
         )
         begin 
            // Note: If this is the first_line of the above CacheOp (multiple_lines): 
            //       we have NOT yet set its "first_cyc" in m_cacheop_bw_ctrl_first_cyc_q[].
            // Note: If this is NOT the first_line: 
            //       we have already set its "first_cyc" in m_cacheop_bw_ctrl_first_cyc_q[], and now need to clear it.  
            if (trans_desc.get_int_param(pk_cop_first_line, 0) != 1) begin
               if (m_cacheop_bw_ctrl_first_cyc_q.size() > 0) begin
                  void'(m_cacheop_bw_ctrl_first_cyc_q.pop_back());
                  `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) The above PMA fault is for a CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP): and not first_line: clear its first_cyc from m_cacheop_bw_ctrl_first_cyc_q[] (last entry)"))
               end
            end
         end

         return;
      end

      // Charles: Question: Why not handle the case if(tlb_hit==0)...immediate after function va_to_pa (see above)? 
      // If TLB missed, push the cacheop back to the front of the ready ops - It will be retried later.
      if (tlb_hit == 1'b0) begin
         m_cacheop_l2_s1_queue.push_front(trans_desc);
         return;
      end

      process_l23_prefetch(trans_desc);  // note: this is still S1 stage

   endfunction: process_s1_cop_l2


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s2_cop
   //
   //   This function is called 1 cycles after process_s1_cop when the cacheop is able to be
   //   serviced and we have additional info from the RTL (auto-select way for LockVA) for checking
   //
   function void process_s2_cop(input evl_core_dcache_cop_s2_t rtl_info);
      evl_minion_dcache #(CACHE_TYPE) cache;
      evl_trans_desc    trans_desc;
      evl_line_desc     line_desc;
      evl_table_desc    table_desc;
      evl_way_t         selected_way;
      evl_dcache_qual_t qualifier = DCQ_None;
      bit               lock_err = 1'b0;
      evl_paddr_t       set;
      bit               thread_id;

      trans_desc = m_cacheop_s2_queue.pop_front();
      if (trans_desc == null) begin
         sb_error($sformatf("Received a CacheOp S2 event but nothing was expected"), null, {}, "", `__FILE__, `__LINE__);
         return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received a CacheOp S2 event for Trans: %s", trans_desc.sprint_obj()))
      thread_id = trans_desc.get_thread_id();

      // RTL forces a retry if there are resource conflicts
      if ((rtl_info.conflict) == 1'b1) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(RTL forced) CacheOp will retry because of resource conflict: %0s", trans_desc.sprint_obj()))
         m_cacheop_s0_queue.push_front(trans_desc);   // ? should we change it to m_cacheop_s1_queue ???
         return;
      end

      cache = get_cache(trans_desc);

      // Get the targetted cache line
      selected_way = evl_way_t'(trans_desc.get_int_param(pk_cop_way_sel, $clog2(rtl_info.way_select)));

      // These cacheops address set/way based on the physical cache specs (regardless of shared/split mode)
      // need to compute which cache model the targetted set/way falls in
      if (trans_desc.get_cmd() inside {CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_UNLOCK_SW}) begin   // (CYE) why not include "CACHE_LOCK_SW"? Bcoz it comes with a actual PA
         evl_paddr_t     cache_idx;
         evl_thread_id_t tid;
         int             cache_model;
         tid = trans_desc.get_thread_id();
         set = (trans_desc.get_paddr() >> `EVL_LINE_ADDR_LSB) % 16;   // for set-way-based CacheOp: always set=paddr[9:6] (as if in "Shared-mode", regardless of actual Cache mode) 

         case ({m_scp_en, m_split_en})
            // Shared mode
            2'b00: begin
               cache_idx   = trans_desc.get_paddr();
               cache_model = 0;
            end
            // Split mode
            2'b01: begin
               if (set < 8) begin        // T0 access only set=0~7
                  cache_idx   = set << `EVL_LINE_ADDR_LSB;
                  cache_model = 0;
               end
               else if (set > 13) begin  // T1 access only set=14~15
                  cache_idx   = (set % 2) << `EVL_LINE_ADDR_LSB;
                  cache_model = 1;
               end
               else begin                // none can access set=8~13
                  // Empty space. nothing to do
                  delete_active_desc(trans_desc);
                  return;
               end
            end
            // SCP mode
            default: begin
               if (set <= 11) begin       // L1SCP: set=0~11 (only by T0 for TensorOp)
                  // SCP space. nothing to do (FUTURE: Emmanuel Marie clean SCP when modelled)
                  delete_active_desc(trans_desc);
                  return;
               end
               else if (set <= 13) begin  // T0 access only set=12,13
                  cache_idx   = (set % 2) << `EVL_LINE_ADDR_LSB;
                  cache_model = 0;
               end
               else begin                 // T1 access only set=14,15
                  cache_idx   = (set % 2) << `EVL_LINE_ADDR_LSB;
                  cache_model = 1;
               end
            end
         endcase

         line_desc = m_cache[cache_model].get_line_desc_by_set_way(cache_idx, selected_way, 1'b0, 1'b0);
      end
      else begin   // for all addr-based CacheOp and Lock-SW (which comes with PA)
         line_desc = cache.get_line_desc(trans_desc.get_paddr());
         set = get_cache_set_addr(trans_desc);
      end

      if (line_desc == null) begin
         sb_error($sformatf("(S2) Got a NULL line descriptor for %0s", trans_desc.sprint_obj()), null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      trans_desc.set_line_desc(line_desc);
      if (trans_desc.cmd_is_idx_cacheop() == 1'b1) begin
         trans_desc.set_paddr(line_desc.get_paddr());
      end

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Processing CacheOp: %0s (set=%0d way=%0d)", trans_desc.sprint_obj(), set, selected_way))

      //
      // Check if hard-lock is allowed to proceed
      //
      if (trans_desc.get_cmd() == CACHE_LOCK_SW) begin  // hard-lock
         evl_line_desc set_way_line;

         //
         // Fail hard lock if selected way is greater than cache associativity: ?not possible?  selected_way is only 2 bits [56:55]; 
         //
         if (selected_way > cache.get_num_ways()) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Hard lock failing because way=%0d is illegal", selected_way))
            lock_err = 1'b1;
         end

         // Locking a set that already has num_ways-1 locked ways is an error
         if (cache.get_locked_way_count(trans_desc.get_paddr()) == (cache.get_num_ways() - 1)) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Hard lock failing because max number of ways are already locked"))
            lock_err = 1'b1;
         end

         // Locking an address that is already present in a different way is an error
         if (!(line_desc.get_cache_way() inside {EVL_WAY_NONE, selected_way})) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Hard lock failing because address is already present in way=%0d", line_desc.get_cache_way()))
            lock_err = 1'b1;
         end

         // Locking a set/way that is already locked is an error
         set_way_line = cache.get_line_desc_by_set_way(trans_desc.get_paddr(), selected_way, 1'b0, 1'b0);
         if (set_way_line != null) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Current resident of this set/way: %0s", set_way_line.sprint_obj()))
            if (set_way_line.get_cache_st() inside {L, D}) begin
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Hard lock failing because targetted set/way is already locked"))
               lock_err = 1'b1;
            end
         end
      end

      //
      // Check if RTL agrees about the Lock Error
      //
      if (rtl_info.lock_err != lock_err) begin
         sb_error($sformatf("(S2) Lock err mismatch: RTL says %0d but L1SB expects %0d", rtl_info.lock_err, lock_err), null, {}, "", `__FILE__, `__LINE__);
      end

      if (lock_err == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("CacheOp LockSW: got Lock error: Dropping trans; set m_TensorError_CSR[%0d][5]=1", thread_id))
         m_TensorError_CSR[thread_id][5] = 1'b1; 

         delete_active_cacheops_s2_queue(trans_desc);    // VERIF-2128
            // the above function deletes "trans_desc" and also all the rest descriptors which spawned from the same CacheOp request 
         return;
      end

      if (trans_desc.get_cmd() inside {CACHE_PREFETCH, CACHE_FLUSH, CACHE_EVICT, CACHE_FLUSH_SW, CACHE_EVICT_SW}) begin
         case (trans_desc.get_bus_cop_dest_level())
            1: qualifier = DCQ_ToL2;
            2: qualifier = DCQ_ToL3;
            3: qualifier = DCQ_ToMem;
            default: qualifier = DCQ_None;
         endcase
      end

      table_desc = process_request(trans_desc, line_desc, selected_way, qualifier, set);

      //
      // If the line was present in L1, forward Evicts and Flushes if they need to go further than L2
      //
      if (table_desc.get_err_code() == EC_FwdReq) begin
         bit tid = trans_desc.get_thread_id();
         bit wid = trans_desc.get_int_param(pk_tensorid);
         evl_req_desc l2_req_desc;
         $cast(l2_req_desc, m_template_req_desc_neigh.clone());
         trans_desc.copy_to_desc(l2_req_desc);

         l2_req_desc.set_parent_desc(trans_desc);   // CYE (for ucache_control: RepRate & Max)

         l2_req_desc.set_trans_id(trans_desc.get_trans_id());
         void'(trans_desc.add_child_desc(l2_req_desc));
         if (trans_desc.get_cmd() inside {CACHE_FLUSH, CACHE_FLUSH_SW}) begin
            l2_req_desc.set_abstract_bus_cmd(BusFlush);
         end
         else if (trans_desc.get_cmd() inside {CACHE_EVICT, CACHE_EVICT_SW}) begin
            l2_req_desc.set_abstract_bus_cmd(BusEvict);
         end
         l2_req_desc.set_paddr(trans_desc.get_paddr() & const_line_paddr_mask);
         l2_req_desc.set_bus_req_size(0);
         l2_req_desc.set_bus_cop_start_level(trans_desc.get_bus_cop_start_level());
         l2_req_desc.set_bus_cop_dest_level(trans_desc.get_bus_cop_dest_level());
         // l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Cache_Ops);
         case ({tid, wid})
            2'b00:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_0);
            2'b01:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_1);
            2'b10:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_2);
            default: l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_3);
         endcase

         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("(S2) Predicting this CacheOp request to L2: %0s", l2_req_desc.sprint_obj()))
         setup_bus_req_dependencies(l2_req_desc);
         m_pending_evict_reqs.push_back(l2_req_desc);
      end

      delete_active_desc(trans_desc);

   endfunction: process_s2_cop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_l23_prefetch  (note: this is still S1 stage)
   //
   //   This function is called for Prefetches to L2/L3/SCP, or VA_Flush/Evict starting from L2/L3. 
   //   No cache lookup is necessary so this doesn't share the method used by other cacheops.
   //
   function void process_l23_prefetch(input evl_trans_desc trans_desc);
      evl_line_desc     line_desc;
      evl_table_desc    table_desc;
      evl_dcache_qual_t qualifier;

      if (trans_desc.get_cmd() == CACHE_SCP_FILL) begin  // CacheOp_Pref_L2SCP (Tensor_Load_L2SCP)
         qualifier = DCQ_None;
      end
      else begin
         case (trans_desc.get_bus_cop_dest_level())
            1: qualifier = DCQ_ToL2;
            2: qualifier = DCQ_ToL3;
            3: qualifier = DCQ_ToMem;
            default: qualifier = DCQ_None;
         endcase
      end

      line_desc = get_cache(trans_desc).get_line_desc(trans_desc.get_paddr());
      table_desc = process_request(trans_desc, line_desc, EVL_WAY_NONE, qualifier);  // note: still S1 stage;

      delete_active_desc(trans_desc);
   endfunction: process_l23_prefetch


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_cacheops (on m_cacheop_s0_queue) -- (CYE) VERIF-2128 
   //
   // This function deletes the provided descriptor and all related descriptors (ones spawned from
   // the same cacheop request) from the list of active cacheops.
   //
   function void delete_active_cacheops_s0_queue(input evl_trans_desc trans_desc);
      for (int ii = m_cacheop_s0_queue.size()-1; ii >= 0; ii--) begin
         if (m_cacheop_s0_queue[ii].get_root_trans_desc() == trans_desc.get_root_trans_desc()) begin
            delete_active_desc(m_cacheop_s0_queue[ii]);
            m_cacheop_s0_queue.delete(ii);
         end
      end
      delete_active_desc(trans_desc);
   endfunction: delete_active_cacheops_s0_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_cacheops (on m_cacheop_s1_queue) -- (CYE) VERIF-2128 
   //
   // This function deletes the provided descriptor and all related descriptors (ones spawned from
   // the same cacheop request) from the list of active cacheops.
   //
   function void delete_active_cacheops_s1_queue(input evl_trans_desc trans_desc);
      for (int ii = m_cacheop_s1_queue.size()-1; ii >= 0; ii--) begin
         if (m_cacheop_s1_queue[ii].get_root_trans_desc() == trans_desc.get_root_trans_desc()) begin
            delete_active_desc(m_cacheop_s1_queue[ii]);
            m_cacheop_s1_queue.delete(ii);
         end
      end
      delete_active_desc(trans_desc);
   endfunction: delete_active_cacheops_s1_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_cacheops (on m_cacheop_l2_s1_queue) -- (CYE) VERIF-2128 
   //
   // This function deletes the provided descriptor and all related descriptors (ones spawned from
   // the same cacheop request) from the list of active cacheops.
   //
   function void delete_active_cacheops_l2_s1_queue(input evl_trans_desc trans_desc);
      for (int ii = m_cacheop_l2_s1_queue.size()-1; ii >= 0; ii--) begin
         if (m_cacheop_l2_s1_queue[ii].get_root_trans_desc() == trans_desc.get_root_trans_desc()) begin
            delete_active_desc(m_cacheop_l2_s1_queue[ii]);
            m_cacheop_l2_s1_queue.delete(ii);
         end
      end
      delete_active_desc(trans_desc);
   endfunction: delete_active_cacheops_l2_s1_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_cacheops (on m_cacheop_s2_queue) -- (CYE) VERIF-2128 
   //
   // This function deletes the provided descriptor and all related descriptors (ones spawned from
   // the same cacheop request) from the list of active cacheops.
   //
   function void delete_active_cacheops_s2_queue(input evl_trans_desc trans_desc);
      for (int ii = m_cacheop_s2_queue.size()-1; ii >= 0; ii--) begin
         if (m_cacheop_s2_queue[ii].get_root_trans_desc() == trans_desc.get_root_trans_desc()) begin
            delete_active_desc(m_cacheop_s2_queue[ii]);
            m_cacheop_s2_queue.delete(ii);
         end
      end
      delete_active_desc(trans_desc);
   endfunction: delete_active_cacheops_s2_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_pref_l1_m_s1_queue (on m_s1_queue) 
   //
   // This function deletes the provided descriptor and all related descriptors (ones spawned from
   // the same cacheop request) from the list of active cacheops.
   //
   function void delete_active_pref_l1_m_s1_queue(input evl_trans_desc trans_desc);
      for (int ii = m_s1_queue.size()-1; ii >= 0; ii--) begin
         if (m_s1_queue[ii].get_root_trans_desc() == trans_desc.get_root_trans_desc()) begin
            delete_active_desc(m_s1_queue[ii]);
            m_s1_queue.delete(ii);
         end
      end
      delete_active_desc(trans_desc);
   endfunction: delete_active_pref_l1_m_s1_queue



   //-----------------------------------------------------------------------------------------------
   //
   // Function: update_fence
   //
   // This function deletes the provided descriptor from the list of active mem descs,
   // and also updates the state of pending fences.
   //
   function void update_fence(input evl_trans_desc trans_desc);
      evl_thread_id_t thread_id = trans_desc.get_thread_id();
      if ((m_fence[thread_id] == 1'b1) && (keep_fence(thread_id) == 1'b0)) begin
         m_fence[thread_id] = 1'b0;
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(thread %0d) All trans in L1SB completed: fence done: clear m_fence[]=0 - ready to accept new intpipe requests", thread_id))
      end
   endfunction: update_fence


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_desc
   //
   // This function deletes the provided descriptor from the list of active mem descs,
   // and also updates the state of pending fences.
   //
   function void delete_active_desc(input evl_trans_desc trans_desc);
      delete_active_mem_desc(trans_desc);
      update_fence(trans_desc);
      trans_desc.set_done();
   endfunction: delete_active_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_replay_req
   //
   function void process_replay_req(input evl_dcache_replay_req_t rtl_info);
      evl_trans_desc line_queue[];
      evl_trans_desc trans_desc;
      evl_line_desc  line_desc;
      evl_paddr_t    replay_paddr;
      evl_vaddr_t    replay_pc;
      int            queue_size;
      int            rq_entry;
      int            ii;

      trans_desc   = null;
      rq_entry     = rtl_info.replay_entry;
      replay_paddr = rtl_info.replay_req.addr;
      replay_pc    = rtl_info.replay_req.pc;

      if (!m_replay_queue.exists(rq_entry)) begin
         sb_error($sformatf("(S0) Received a replay event for entry %0d (%0p, 0x%0x) but did not find it in the replay queue", rq_entry, rtl_info.replay_req.cmd, replay_paddr), null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      trans_desc = m_replay_queue[rq_entry];

      if (trans_desc.get_paddr() & const_oword_paddr_mask != replay_paddr & const_oword_paddr_mask) begin
         sb_error($sformatf("(S0) Replay queue entry %0d was expected to be %0s, not (%0p, 0x%0x)", rq_entry, trans_desc.sprint_obj(), rtl_info.replay_req.cmd, replay_paddr), null, {}, "", `__FILE__, `__LINE__);
      end
      if (trans_desc.get_int_param(pk_replay_ready, 0) == 0) begin
         sb_error($sformatf("(S0) Replay queue entry %0d (%0x) was not ready to be replayed", rq_entry, replay_paddr), null, {}, "", `__FILE__, `__LINE__);
      end
      trans_desc.set_int_param(pk_replay_ready, 0);

      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) Replaying request: %0s (from rq_entry=%0d)(PC=0x%h)", trans_desc.sprint_obj(), rq_entry, replay_pc))

      trans_desc.set_int_param(pk_req_replayed, 1);  // indicate a replay_req from ReplayQ
      m_s1_queue.push_back(trans_desc);
      //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) push into m_s1_queue: %0s", trans_desc.sprint_obj()))

   endfunction: process_replay_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_intpipe_req
   //
   function void process_intpipe_req(input evl_dcache_intpipe_req_t rtl_info);
      evl_trans_desc   trans_desc;
      evl_vaddr_t      vaddr;
      evl_thread_id_t  thread_id;
      evl_dcache_cmd   dcache_cmd;    // it's RTL dcache_cmd (see "dcache_cmd" in "rtl/inc/minion_types.vh")
      evl_dcache_type  access_type;
      bit              mem_global;
      bit              global_amo;
      int              dest_reg;
      bit              dest_is_fp;
      bit [7:0]        packed_single_mask;
      bit [2:0]        gsc_cnt;
      bit [39:0]       gsc32_idx;
      string           extra_info;
      evl_vaddr_t      insn_pc;
      int              ps_length;
      bit              addr_msb_err;   // incoming/input from IntPipe

      vaddr                = evl_vaddr_t'(rtl_info.core_req.addr);
      thread_id            = evl_thread_id_t'(rtl_info.core_req.dest.thread_id);
      dcache_cmd           = evl_dcache_cmd'(rtl_info.core_req.cmd);
      access_type          = evl_dcache_type'(rtl_info.core_req.typ);
      mem_global           = bit'(rtl_info.core_req.mem_global);
      global_amo           = mem_global; 
      dest_reg             = int'(rtl_info.core_req.dest.addr);
      dest_is_fp           = bit'(rtl_info.core_req.dest.fp);
      packed_single_mask   = rtl_info.core_req.ps_mask;
      gsc_cnt              = rtl_info.core_req.gsc_cnt;
      gsc32_idx            = rtl_info.core_req.gsc32_idx;
      addr_msb_err         = bit'(rtl_info.core_req.addr_msb_err);
      insn_pc              = rtl_info.ex_pc;
      extra_info           = "";

      while (packed_single_mask != 0) begin
         ps_length += `EVL_WORD_BYTES;
         packed_single_mask >>=1;
      end
      packed_single_mask = rtl_info.core_req.ps_mask;

      //
      // Create a transaction descriptor from the RTL info
      //
      $cast(trans_desc, m_template_trans_desc.clone());
      trans_desc.set_trans_id();
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_privilege((rtl_info.vm_status.mprv) ? rtl_info.vm_status.mpp : rtl_info.vm_status.prv);

      // note: dcache_cmd is from RTL enum type, different from our own trans_desc evl_cmd_t;
      case (dcache_cmd)
         dcache_cmd_XRD       : trans_desc.set_cmd(LD);   // it assign "m_cmd" (evl_cmd_t) in trans_desc
         dcache_cmd_XWR       : trans_desc.set_cmd(ST);
         dcache_cmd_G32       : trans_desc.set_cmd(LD);   // Gather-32
         dcache_cmd_SC32      : trans_desc.set_cmd(ST);   // Scatter-32
         // dcache_cmd_XA: Atomics: 2 types: 1) LR-SC pair; 2) RMW (Read-Modify-Write);
         dcache_cmd_XA_SWAP,                              
         dcache_cmd_XA_CMPSWP,
         dcache_cmd_XLR,                                  // Load-Reserved
         dcache_cmd_XSC,                                  // Store-Conditional (LR-SC pair)
         dcache_cmd_XA_ADD,
         dcache_cmd_XA_XOR,
         dcache_cmd_XA_OR,
         dcache_cmd_XA_AND,
         dcache_cmd_XA_MIN,
         dcache_cmd_XA_MAX,
         dcache_cmd_XA_MIN_PS,
         dcache_cmd_XA_MAX_PS,
         dcache_cmd_XA_MINU,
         dcache_cmd_XA_MAXU   : trans_desc.set_cmd(ATOMIC);
         // See :/rtl/shire/minion/intpipe/intpipe_decode.v (e.g. `FSWL_PS and `FSWG_PS, SBL, SBG, etc);
         dcache_cmd_XWRB      : trans_desc.set_cmd(ST_WA);  // meaning: Stores which bypass L1D$ (in PRM-7 Atomic: Coherent Mem Op)
         // See :/rtl/shire/minion/intpipe/intpipe_decode.v (e.g. `FLWL_PS and `FLWG_PS, etc);
         dcache_cmd_XRDB      : trans_desc.set_cmd(LD_RA);  // meaning: Loads  which bypass L1D$ (in PRM-7 Atomic: Coherent Mem Op)
         default              : sb_error($sformatf("(S0) Unknown command %0p", dcache_cmd), null, {}, "", `__FILE__, `__LINE__);
      endcase

      // determine pk_dcache_cmd_is_write: to match RTL signal s1_is_write;
      if ( (trans_desc.get_cmd() inside {ST, ST_WA, ATOMIC}) &&
           (dcache_cmd != dcache_cmd_XLR) ) 
      begin
         trans_desc.set_int_param(pk_dcache_cmd_is_write, 1);
      end else begin
         trans_desc.set_int_param(pk_dcache_cmd_is_write, 0);
      end

      // based on RTL access_type (size): set our "cmm_length" (number of bytes);
      case (access_type)
         dcache_type_B       : trans_desc.set_cmd_length(1);           // Byte sign extend
         dcache_type_H       : trans_desc.set_cmd_length(2);           // Half word sign extend
         dcache_type_W       : trans_desc.set_cmd_length(4);           // Word sign extend
         dcache_type_D       : trans_desc.set_cmd_length(8);           // DWord
         dcache_type_BU      : trans_desc.set_cmd_length(1);           // Byte zero extend
         dcache_type_HU      : trans_desc.set_cmd_length(2);           // Half word zero extend
         dcache_type_WU      : trans_desc.set_cmd_length(4);           // Word zero extend
         dcache_type_PS      : trans_desc.set_cmd_length(ps_length);   // up to 256-bit, depending on mask
         dcache_type_PS_BR   : trans_desc.set_cmd_length(4);           // Broadcast 32b to all VPU registers
         dcache_type_PS_GS8  : trans_desc.set_cmd_length(1);           // gather/scatter 8 bits
         dcache_type_PS_GS16 : trans_desc.set_cmd_length(2);           // gather/scatter 16 bits
         dcache_type_PS_GS32 : trans_desc.set_cmd_length(4);           // gather/scatter 32 bits
         default             : trans_desc.set_cmd_length(4);
      endcase

      trans_desc.set_int_param(pk_dcache_cmd, dcache_cmd);
      trans_desc.set_int_param(pk_access_type, access_type);
      trans_desc.set_int_param(pk_dest_reg, dest_reg);
      trans_desc.set_int_param(pk_dest_is_fp, dest_is_fp);
      trans_desc.set_int_param(pk_mem_global, mem_global);
      trans_desc.set_int_param(pk_addr_msb_err, addr_msb_err);

      trans_desc.set_vaddr(vaddr);
      trans_desc.set_paddr(evl_paddr_t'(vaddr));     // FUTURE: from TLB VA->PA

      trans_desc.set_int_param(pk_insn_length, ((access_type == dcache_type_PS) || (dcache_cmd inside {dcache_cmd_G32, dcache_cmd_SC32})) ? 32 : trans_desc.get_cmd_length());

    /********  // move to place after message "(S0) New request from IntPipe ..."
      // Drop request if it's a masked packed single
      if (   ((access_type inside {dcache_type_PS, dcache_type_PS_BR, dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) && (packed_single_mask == 0))
          || (!(dcache_cmd inside {dcache_cmd_SC32, dcache_cmd_G32}) && (access_type inside {dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) && (packed_single_mask[gsc_cnt] == 0)))
      begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) Dropping due to PS masking: %0s", trans_desc.sprint_obj()))
         return;
      end
    ********/

      if ((access_type inside {dcache_type_PS, dcache_type_PS_BR}) || (dcache_cmd inside {dcache_cmd_G32, dcache_cmd_SC32})) begin
         trans_desc.set_int_param(pk_ps_mask, packed_single_mask);
      end
      if (access_type == dcache_type_PS) begin
         trans_desc.set_int_param(pk_vector_ps, 1);
      end

      //
      // Gather/Scatter in a restricted 32 byte block gets sent as one chunk of data
      // with the indices specified by gsc32_idx
      if (dcache_cmd inside {dcache_cmd_G32, dcache_cmd_SC32}) begin
         trans_desc.set_int_param(pk_gsc32_idx_lo, gsc32_idx[31:0]);    // integer is only 32-bit (but gsc32_idx is [39:0])
         trans_desc.set_int_param(pk_gsc32_idx_hi, gsc32_idx[39:32]);
         case (access_type)
            dcache_type_PS_GS8  : extra_info = " (GSC32 byte)";
            dcache_type_PS_GS16 : extra_info = " (GSC32 half)";
            dcache_type_PS_GS32 : extra_info = " (GSC32 word)";
            default: sb_error("illegal command type for restricted gather/scatter", null, {}, "", `__FILE__, `__LINE__);
         endcase
      end

      if (access_type inside {dcache_type_PS, dcache_type_PS_BR, dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) begin
         extra_info = $sformatf("%0s (PS_mask=%8b)", extra_info, packed_single_mask);
         if (access_type inside {dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) begin
            extra_info = $sformatf("%0s (GSC_cnt=%0d)", extra_info, gsc_cnt);
         end
      end

      if (trans_desc.get_cmd() == ATOMIC) begin
         evl_bus_atomic_conf_t amo_conf;

         amo_conf[3:0] = (dcache_cmd == dcache_cmd_XA_SWAP)   ? 0  :
                         (dcache_cmd == dcache_cmd_XA_ADD)    ? 1  :
                         (dcache_cmd == dcache_cmd_XA_XOR)    ? 2  :
                         (dcache_cmd == dcache_cmd_XA_AND)    ? 3  :
                         (dcache_cmd == dcache_cmd_XA_OR)     ? 4  :
                         (dcache_cmd == dcache_cmd_XA_MIN)    ? 5  :
                         (dcache_cmd == dcache_cmd_XA_MAX)    ? 6  :
                         (dcache_cmd == dcache_cmd_XA_MINU)   ? 7  :
                         (dcache_cmd == dcache_cmd_XA_MAXU)   ? 8  :
                         (dcache_cmd == dcache_cmd_XA_MIN_PS) ? 9  :
                         (dcache_cmd == dcache_cmd_XA_MAX_PS) ? 10 :
                         (dcache_cmd == dcache_cmd_XA_CMPSWP) ? 11 : 15;

         amo_conf[4] = 1'b0;
         amo_conf[5] = (trans_desc.get_cmd_length() == 8) ? 1'b1 : 1'b0;
         amo_conf[6] = global_amo;
         trans_desc.set_bus_atomic_conf(amo_conf);
         trans_desc.set_bus_atomic_offset(vaddr & `EVL_LINE_OFFSET_MASK);
         extra_info = $sformatf("%0s (global_amo=%b)", extra_info, global_amo);
      end

      if (trans_desc.get_cmd() inside {LD_RA, ST_WA}) begin
         extra_info = $sformatf("%0s (Coherent_Mem_Op mem_global=%b)", extra_info, mem_global);
      end

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) New request from IntPipe (thread %0d) (PC=0x%12x): cmd=%0p type=%0p --> %0s%0s", trans_desc.get_thread_id(), insn_pc, dcache_cmd, access_type, trans_desc.sprint_obj(), extra_info))

      // Drop request if it's a masked packed single
      if (   ((access_type inside {dcache_type_PS, dcache_type_PS_BR, dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) && (packed_single_mask == 0))
          || (!(dcache_cmd inside {dcache_cmd_SC32, dcache_cmd_G32}) && (access_type inside {dcache_type_PS_GS8, dcache_type_PS_GS16, dcache_type_PS_GS32}) && (packed_single_mask[gsc_cnt] == 0)))
      begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) Dropping due to PS masking: %0s", trans_desc.sprint_obj()))
         return;
      end

      // VERIF-861 (PRM-0 Errata):
      // check: FSWL.PS (local_mem) or FSWG.PS (global_mem) with mask different than 0xFF: flag error;
      // ...  // But: Core Req could be kiiled by IntPipe at S1 ... So move this check to S1 stage!!!

      // (CYE) 6/09/2020: change to use "ev_s1_tlb_flush_pipeline[n].triggered":  event.triggered state/property, 
      // for this current time stamp, to model the 1-cycle signal "s1_tlb_flush_pipeline[n]" begin asserted in this cycle;
      // Important: must ensure that we do function process_s1_req() BEFORE function process_intpipe_req()!!!
      if (ev_s1_tlb_flush_pipeline[thread_id].triggered) begin   // for this current time stamp
         // drop this S0 request:
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("due to ev_s1_tlb_flush_pipeline[%0d].triggered=1 (previous trans getting TLB miss in this same cycle): dropping this S0 request ...", thread_id))
         return;  

         // DCache RTL behaves differently in some case: 
         // after a previous trans gets TLB miss (in its S1 stage):
         // 1) if next trans follows in back-to-back cycle: it will be killed in S0 by signal s1_tlb_flush_pipeline[] (and will NOT move to S1);
         // 2) if next trans follows 2 or more cycles after: it will continue to S1, and then will be killed by s1_core_kill from IntPipe;
      end

      // If virtual memory is enabled, create a potential page table walk request;
      // unless we are in machine mode, which doesn't go through translation.
      // Note: vm_status.prv=2'b11 means M-mode; satp_info.mode=0 means "No VM (Bare)";
      if ((rtl_info.vm_status.prv != 2'b11) && (rtl_info.satp_info.mode != 0)) begin
         evl_ptw_desc ptw_req;
         ptw_req = new();
         ptw_req.set_vaddr(trans_desc.get_vaddr());
         ptw_req.set_vm_mode(rtl_info.satp_info.mode);
         ptw_req.set_root_ppn(rtl_info.satp_info.ppn);
         ptw_req.set_privilege(trans_desc.get_privilege());
         trans_desc.set_object_param(pk_tlb_req, ptw_req);     // set pk_tlb_req = ptw_req  
      end

      if (m_fence[thread_id] == 1'b1) begin
         if (m_allow_bus_err == 1'b0) begin
            sb_error($sformatf("(S0) fence is still pending for thread %0d - Should not have received a new request", thread_id), trans_desc, {}, "", `__FILE__, `__LINE__);
         end else begin   // when bus_err occurs: sometimes L1SB FENCE check not working ?
            `evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) fence is still pending for thread %0d - Should not have received a new request", thread_id))
         end
      end
      add_active_mem_desc(trans_desc);
      trans_desc.set_int_param(pk_intpipe_req, 1);

      m_s1_queue.push_back(trans_desc);
      //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S0) push into m_s1_queue: %0s", trans_desc.sprint_obj()))

   endfunction: process_intpipe_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_ptw_response
   //
   function bit process_ptw_response(input evl_ptw_desc ptw_desc);
      evl_page_table_entry pte;

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), ptw_desc, $sformatf("processing PTW response %0s", ptw_desc.sprint_obj()))
      pte = m_tlb.get_line_desc(ptw_desc.get_vaddr());
      case (pte.get_cache_st())
         IE_Bsy: begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), ptw_desc, $sformatf("pte: %s; pte.get_cache_st: IE_Bsy -> upgrading to E", pte.sprint_obj()))
            pte.do_copy(ptw_desc.get_rsp_pte());
            pte.set_cache_st(E);

            // it's possible that 2 bits are asserted, but only one bit is actually waiting for PTW resp;
            if (|m_tlb_ptw_in_process[1:0] == 1'b1) begin  
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), ptw_desc, $sformatf("with above PTW resp: clear m_tlb_ptw_in_process[1:0]=0")) 
               m_tlb_ptw_in_process[1:0] = 2'b0;     // clear both bits
            end
         end
         default: begin
            sb_error("No action for PTW response", null, {}, "", `__FILE__, `__LINE__);
         end
      endcase

   endfunction: process_ptw_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: va_to_pa: (Note: It also handles when VM is disable: it returns PA=VA as success.) 
   //
   function bit va_to_pa(input evl_trans_desc trans_desc);
      evl_page_table_entry pte;
      evl_page_table_entry prev_pte;
      evl_ptw_desc         ptw_req;

      if (trans_desc.have_object_param(pk_tlb_req) == 1'b0) begin   // (trans_desc.pk_tlb_req == NULL) indicates VM disabled
         trans_desc.set_paddr(trans_desc.get_vaddr());
         return 1'b1;
      end

      pte = m_tlb.get_line_desc(trans_desc.get_vaddr());
      case (pte.get_cache_st())
         I: begin
            // (CYE) 6/09/2020: update: when TLB is processing a PTW, if another Req comes in and is also a TLB miss (and different page): 
            //  our DCache TLB module can only handle one outstanding PTW req: so do nothing to the 2nd TLB miss;
            if (|m_tlb_ptw_in_process[1:0] == 1'b1) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s had a TLB miss: but there is already a pending PTW being processed (m_tlb_ptw_in_process[]=non_0)(different page): so do nothing with this new req", trans_desc.sprint_obj()))
               return 1'b0;
            end

            // Otherwise: continue to process the TLB miss: 
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s had a TLB miss ... (PTE current state I)", trans_desc.sprint_obj()))

            // (Charles) TLB is fully associative; will replace a valid entry (oldest/LRU) if TLB full;
            // TLB replacement policy - LRU; 
            prev_pte = m_tlb.allocate_way(EVL_WAY_0, pte); 
               // ... ??? This current code is incorrect; need to be fixed!!! - 
               // FUTURE: Idea: build an age-queue of all valid TLB entries ...

            pte.set_cache_st(IE_Bsy);
            if (prev_pte != null) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("replaced previous PTE %0s", prev_pte.sprint_obj()))
            end
            // expect to send out a PTW req:
            $cast(ptw_req, trans_desc.get_object_param(pk_tlb_req));
            m_pending_ptw_reqs.push_back(ptw_req);
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) Predicting this PTW Req to PTW_IFC: %s",  ptw_req.sprint_obj()))
            return 1'b0;
         end
         IE_Bsy: begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s had a TLB miss (already fetching PTE) ... (PTE current state IE_Bsy)", trans_desc.sprint_obj()))
            return 1'b0;
         end
         E: begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s had a TLB hit: id %0x  %0s ... (PTE current state E)", trans_desc.sprint_obj(), pte.get_inst_id(), pte.sprint_obj()))
            // obtain PA = PPN + page_offset;
            trans_desc.set_paddr((trans_desc.get_vaddr() & ~const_page_vaddr_mask) | pte.get_paddr());
            // FUTURE: Emmanuel Marie check page attrs
            return 1'b1;
         end
      endcase
   endfunction: va_to_pa


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s1_req:
   // Note: also include CacheOp_Pref_L1 case
   //
   function void process_s1_req(input evl_dcache_s1_pipe_info_t rtl_info);
      evl_trans_desc      trans_desc;
      evl_vaddr_t         base_addr;
      evl_vaddr_t         top_addr;
      bit                 exp_misaligned;
      evl_oword_byte_en_t ps_mask;
      int                 cmd_length;    // number of data bytes
      evl_dword_t         rsp_delay;
      bit                 thread_id;
      bit                 l1sb_pma_fault = 1'b0;
      bit                 l1sb_misalign_fault = 1'b0;
      bit                 rtl_core_xcpt_mf = 1'b0;
      bit                 xcpt_qaulifier;

      trans_desc = m_s1_queue.pop_front();

      if (trans_desc == null) begin
            sb_error("Received an S1 event but no request is expected at S1", null, {}, "", `__FILE__, `__LINE__);
            return;
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received an S1 event for Trans: %s", trans_desc.sprint_obj()))

      thread_id = trans_desc.get_thread_id();

      // IntPipe could kill (s1_core_kill) this request: only on new incoming Req (first Pass) and from IntPipe;
      //if ((rtl_info.kill == 1'b1) && (trans_desc.get_int_param(pk_intpipe_req) == 1)) begin     // (Charles 11/14/2019) change; testing;
      if ((rtl_info.kill == 1'b1) && (trans_desc.get_int_param(pk_intpipe_req) == 1) && (trans_desc.get_int_param(pk_req_replayed, 0) == 0)) begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("Transaction killed by intpipe at S1 (s1_core_kill)"))
         delete_active_desc(trans_desc);
         return;
      end

      // if this trans is NOT killed by IntPipe: continue ...

      // VERIF-861 (PRM-0 Errata):
      // check: FSWL.PS (local_mem) or FSWG.PS (global_mem) with mask different than 0xFF: flag error 
      // This impacts:
      // 1) FSWL.PS (mem_global==0) and FSWG.PS (mem_global==1);
      // 2) Any contiguous masked vector store while the L1 Data cache is in bypass mode
      // Question: 
      //    Besides FSWL.PS and FSWG.PS, is there any other instruction which might be decoded as dcache_cmd_XWRB and dcache_type_PS ... ??? No.
      //    See :/rtl/shire/minion/intpipe/intpipe_decode.v (search for `FSWL_PS and `FSWG_PS);
      if ((trans_desc.get_int_param(pk_dcache_cmd) == dcache_cmd_XWRB) && (trans_desc.get_int_param(pk_access_type) == dcache_type_PS)) begin
         ps_mask = trans_desc.get_int_param(pk_ps_mask, 0);
         if (ps_mask != 8'hff) begin
            sb_error($sformatf("(S1) for Trans: %s: cmd=dcache_cmd_XWRB type=dcache_type_PS (mem_global=%0b): FSWL.PS or FSWG.PS: but its ps_mask=0x%h (expected all 1s)", trans_desc.sprint_obj(), trans_desc.get_int_param(pk_mem_global), ps_mask), trans_desc, {}, "", `__FILE__, `__LINE__);
         end
      end

      // VM: TLB Lookup and VA->PA convert: (CYE) 6/09/2020 update; 
      // (Note: Function va_to_pa also handles when VM is disabled.)
      if (va_to_pa(trans_desc) == 1'b0) begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) TLB miss: Flushing pipe; dropping trans_desc; set m_tlb_ptw_in_process[%0d]=1; trigger ev_s1_tlb_flush_pipeline[%0d]", thread_id, thread_id))
         delete_active_desc(trans_desc);
         m_tlb_ptw_in_process[thread_id] = 1'b1;  // note: our model's m_tlb_ptw_in_process behaves different from RTL's signal?
         ->ev_s1_tlb_flush_pipeline[thread_id];
         // expect that RTL L1D$ sends s2_core_flush_pipeline (merged) to Core IntPipe:
         // FUTURE ... 
         return;
      end

      //
      // Check if this response should be delayed
      //
      rsp_delay = m_minion_rtl_port.get_rsp_delay(trans_desc.get_paddr());
      if (rsp_delay > 0) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) Delay response by %0d cycles", rsp_delay))
         trans_desc.add_delay(pk_mem_bus_rsp_delay, rsp_delay);
      end

      //
      // Check PMA for access faults (only on first pass in the pipe)
      //
      if (trans_desc.get_int_param(pk_req_replayed, 0) == 0) begin 
         // PMA check: 
         if (check_pma(trans_desc) == 1'b0) begin
            l1sb_pma_fault = 1'b1;
         end

         // cross_check to RTL:
         // note: PMA_Access_Fault will trigger exception, but here exclude CacheOp_Pref_L1 case:
         if (trans_desc.get_cmd() == CACHE_PREFETCH) begin
            xcpt_qaulifier = 1'b0;
         end else begin
            xcpt_qaulifier = 1'b1;
         end
         l1sb_pma_cross_check_rtl(l1sb_pma_fault, rtl_info.pma_resp, xcpt_qaulifier, rtl_info.core_xcpt, rtl_info.core_xcpt_mf);   
              // xcpt_qaulifier=1 (exclude CacheOp_Pref_L1)

         // enqueue in l1sb_pma_or_misalign_fault_q[]:
         if ((l1sb_pma_fault == 1'b1) && (xcpt_qaulifier == 1'b1)) begin
            m_pma_or_misalign_fault_q.push_back(trans_desc);    
         end

         // if L1SB PMA finds access_fault, drop it; 
         if (l1sb_pma_fault == 1'b1) begin
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) PMA check Access_Fault Exception: will be dropped"))
            delete_active_desc(trans_desc);

            // if it's Prefetch_L1: need to delete all younger trans spawn-off from same CacheOp Prefetch_L1 Inst (still in m_s1_queue[]):
            if (trans_desc.get_cmd() == CACHE_PREFETCH) begin 
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("CacheOp Prefetch_L1 got PMA access fault: Dropping trans; set m_TensorError_CSR[%0d][7]=1", thread_id))
               m_TensorError_CSR[thread_id][7] = 1'b1; 

               delete_active_pref_l1_m_s1_queue(trans_desc);
               // there also could be some spawn-off remaining in m_cacheop_s0_queue[]: 
               delete_active_cacheops_s0_queue(trans_desc);
               // FUTURE: how about any active spawn-off Pref_L1 still remaining in ReplayQ ? should we delete them as well ??? 
            end
  
            return;
         end
      end  // if (trans_desc.get_int_param(pk_req_replayed, 0) == 0)

      //
      // Writes targetting the HART ports memory space are transformed into MESSAGE
      //
      if ((trans_desc.get_cmd() == ST) && (trans_desc.get_paddr() & `EVL_MSG_PORT_MATCH_MASK) == `EVL_MSG_PORT_MATCH_VAL) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above ST targetting the HART ports memory space: transform into MESSAGE"))
         trans_desc.set_cmd(MESSAGE);
      end

      // Note: we must check misaligned_fault AFTER check_pma() is done (which determine is_cached or is_uncached in some cases);

      // Check for misaligned_fault (exception) - (only on first pass in the pipe?)
      //
      // Uncached requests (include AMO, ReadAround/WriteAround, L1D$ Bypass mode) must be aligned to their size or they cause an exception;
      // Note: "trans_desc.is_uncached()" here include AMO, ReadAround/WriteAround, and L1D$ Bypass mode; 
      // Note: "trans_desc.pk_insn_length" here means access size (as in byte);
      //
      if ((trans_desc.is_uncached() == 1'b1) && ((trans_desc.get_paddr() & (trans_desc.get_int_param(pk_insn_length) - 1)) != 0)) begin
         l1sb_misalign_fault = 1'b1;
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) Non cacheable access (include AMO, RA/WA, Bypass) %0s is misaligned to its size (%0d) - Misaligned_Fault Exception: will be dropped", trans_desc.sprint_obj(), trans_desc.get_int_param(pk_insn_length)))
      end

      // cross-check RTL: signal s1_mf_xcpt (and also mreged in s1_core_xcpt.af):
      rtl_core_xcpt_mf = (rtl_info.core_xcpt.af_ld | rtl_info.core_xcpt.af_st) & rtl_info.core_xcpt_mf;
      if ( l1sb_misalign_fault != rtl_core_xcpt_mf ) begin
         sb_error($sformatf("(for above Core Req) Misalign_Fault->Expt mismatch: L1SB=%0b RTL=%0b (s1_core_xcpt.af & s1_mf_xcpt)", l1sb_misalign_fault, rtl_core_xcpt_mf), trans_desc, {}, "", `__FILE__, `__LINE__);
         //`evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), trans_desc, $sformatf("(WARNING)(for above Core Req) Misalign_Fault->Expt mismatch: L1SB=%0b RTL=%0b (s1_core_xcpt.af & s1_mf_xcpt)", l1sb_misalign_fault, rtl_core_xcpt_mf))
      end

      // enqueue in l1sb_pma_or_misalign_fault_q[]:
      if ((l1sb_misalign_fault == 1'b1)) begin
         m_pma_or_misalign_fault_q.push_back(trans_desc);    
      end

      if (l1sb_misalign_fault == 1'b1) begin   // drop/delete the trans_desc
         delete_active_desc(trans_desc);
         return;
      end

      //
      // L1D_SB detect misaligned accesses crossing cline (64-byte) boundary:
      //
      cmd_length = trans_desc.get_cmd_length();   // number of data bytes
      base_addr  = trans_desc.get_paddr();
      if (trans_desc.have_int_param(pk_gsc32_idx_lo) == 0) begin // by definition restricted GSC cannot be misaligned
         top_addr  = base_addr + cmd_length - 1;
         exp_misaligned = (((base_addr ^ top_addr) & const_line_paddr_mask) != evl_vaddr_t'(0)) ? 1'b1 : 1'b0;
         if ({exp_misaligned, rtl_info.misaligned} == 2'b10) begin
            sb_error($sformatf("(S1) Request is crossing cache line boundary but RTL didn't mark it as misaligned - base_addr = 0x%0x  top_addr = 0x%0x", base_addr, top_addr), null, {}, "", `__FILE__, `__LINE__);
         end
      end

      // Get misalignment info from RTL: crossing 32-byte boundary (not 64-byte) ??? 
      if ((rtl_info.misaligned == 1'b1) && (trans_desc.have_object_param(pk_misaligned_2nd) == 1'b0)) begin
         evl_trans_desc misaligned_desc;
         evl_vaddr_t misaligned_addr;
         int misaligned_length;

        // note: (RTL) increment merely ensures access to next LRAM block (32 bytes, not 64 bytes)
         misaligned_addr   = (trans_desc.get_vaddr() + `EVL_OWORD_BYTES) & const_oword_vaddr_mask; 
         misaligned_length = trans_desc.get_cmd_length() - (misaligned_addr - trans_desc.get_vaddr());
         trans_desc.set_int_param(pk_this_is_misaligned_access, 1);   // Charles (CYE) added

         $cast(misaligned_desc, trans_desc.clone());
         misaligned_desc.set_vaddr(misaligned_addr);
         misaligned_desc.set_paddr(misaligned_addr);       // FUTURE: change to VA->PA;
         misaligned_desc.set_cmd(trans_desc.get_cmd());

         // need to pass "pk_dest_reg" and "pk_dest_is_fp" info:
         misaligned_desc.set_int_param(pk_dest_reg, trans_desc.get_int_param(pk_dest_reg));  
         misaligned_desc.set_int_param(pk_dest_is_fp, trans_desc.get_int_param(pk_dest_is_fp));

         misaligned_desc.set_cmd_length(misaligned_length);
         misaligned_desc.set_int_param(pk_this_is_misaligned_2nd_part, 1);  // Charles (CYE) added
         misaligned_desc.set_object_param(pk_misaligned_1st, trans_desc);
         trans_desc.set_object_param(pk_misaligned_2nd, misaligned_desc);
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("S1 indicates misaligned - prepare a spawn-off 2nd trans_desc: %0s (as pk_misaligned_2nd of original trans_desc)", misaligned_desc.sprint_obj()))
         if (trans_desc.have_object_param(pk_tlb_req)) begin
            evl_ptw_desc ptw_req;
            evl_ptw_desc ptw_req_misaligned;
            $cast(ptw_req, trans_desc.get_object_param(pk_tlb_req));
            $cast(ptw_req_misaligned, ptw_req.clone());
            ptw_req_misaligned.set_vaddr(misaligned_desc.get_vaddr());
            misaligned_desc.set_object_param(pk_tlb_req, ptw_req_misaligned);
         end
      end

      //
      // For transaction with write_data, capture the data and populate m_write_data of the trans_desc;
      // note: only for new incoming Req (first Pass);
      //
      if ((trans_desc.get_cmd() inside {ST, ST_WA, ATOMIC, MESSAGE}) && 
          (trans_desc.get_int_param(pk_intpipe_req) == 1) && 
          (trans_desc.get_int_param(pk_req_replayed, 0) == 0))  // (Charles 11/14/2019) only for new incoming Req (first Pass) 
      begin
         evl_xword_byte_en_t byte_en;

         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("S1 stage data: 0x%h", rtl_info.store_data))
         ps_mask  = trans_desc.get_int_param(pk_ps_mask, 0);

         if (trans_desc.have_int_param(pk_gsc32_idx_lo)) begin
            evl_paddr_t gsc32_idx;
            evl_paddr_t gsc32_idx_lo;
            evl_paddr_t gsc32_idx_hi;
            evl_vaddr_t addr;
            int         offset;

            gsc32_idx_lo = trans_desc.get_int_param(pk_gsc32_idx_lo);
            gsc32_idx_hi = trans_desc.get_int_param(pk_gsc32_idx_hi);
            gsc32_idx    = gsc32_idx_lo[31:0] | (gsc32_idx_hi[31:0] << 32);
            addr         = trans_desc.get_vaddr();

            for (int ii = 0; ii < `VPU_LANE_NUM; ii++) begin
               if (ps_mask[ii] == 1'b0) continue;
               case (cmd_length)
                  1 : begin offset =  (gsc32_idx >> (ii * 5)) & 'h1f       ;  offset = (addr[5] << 5) | ((addr + offset) & 'h1f); end
                  2 : begin offset = ((gsc32_idx >> (ii * 4)) & 'h0f) << 1 ;  offset = (addr[5] << 5) | ((addr + offset) & 'h1e); end
                  4 : begin offset = ((gsc32_idx >> (ii * 3)) & 'h07) << 2 ;  offset = (addr[5] << 5) | ((addr + offset) & 'h1c); end
                  default: sb_error("unexpected command length for restricted scatter", null, {}, "", `__FILE__, `__LINE__);
               endcase

               byte_en = evl_xword_byte_en_t'((1 << cmd_length) - 1);
               trans_desc.set_write_bytes(offset, cmd_length, (evl_xword_t'(rtl_info.store_data) >> (ii*32)) & 32'hffffffff, byte_en);
            end
         end
         else begin
            // if ((trans_desc.is_uncached() == 1'b1) && (ps_mask != (~evl_oword_byte_en_t'(0)))) begin
            //    sb_error("ARCHSIM-400: software is sending a non-cacheable vector store with swiss cheese mask. RTL does not support this.", null, {}, "", `__FILE__, `__LINE__);
            // end
            byte_en = evl_xword_byte_en_t'(0);
            // expand ps_mask (word mask) to be a byte mask
            if (ps_mask != evl_oword_byte_en_t'(0)) begin
               int ii = 0;
               while (ps_mask != evl_xword_byte_en_t'(0)) begin
                  byte_en |= {4{ps_mask[0]}} << (ii * 4);
                  ps_mask >>= 1;
                  ii++;
               end
            end
            else begin
               byte_en = evl_xword_byte_en_t'((1 << cmd_length) - 1);
            end

            if (trans_desc.get_cmd() == ATOMIC) begin
               // Atomic data is always in LSBs: assign to m_write_data;
               trans_desc.set_write_bytes(0, cmd_length, evl_xword_t'(rtl_info.store_data), byte_en);
               //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
               //   trans_desc.get_write_data().print_obj(get_abstract_name());
               //end

               // special for Atomic_CAS (dcache_cmd_XA_CMPSWP): 
               // (another) expected_compare_data comes later as s3_core_x31 (S3 stage);
            end
            else begin
               trans_desc.set_write_bytes(-1, cmd_length, evl_xword_t'(rtl_info.store_data), byte_en);
            end
         end

         //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         //   trans_desc.get_write_data().print_obj(get_abstract_name());
         //end

         //
         // Unaligned/misaligned crossing line boundary
         //
         if (trans_desc.have_object_param(pk_misaligned_2nd) == 1) begin
            evl_trans_desc misaligned_desc;
            int bytes_done;
            $cast(misaligned_desc, trans_desc.get_object_param(pk_misaligned_2nd));
            bytes_done = `EVL_LINE_BYTES - (trans_desc.get_paddr() & `EVL_LINE_OFFSET_MASK);
            misaligned_desc.set_write_bytes(-1, trans_desc.get_cmd_length() - bytes_done, evl_xword_t'(rtl_info.store_data >> (bytes_done << 3)), byte_en >> bytes_done);
            if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
               misaligned_desc.get_write_data().print_obj(get_abstract_name());
            end
         end
      end  // if ((trans_desc.get_cmd() inside {ST, ST_WA, ATOMIC, MESSAGE}) ... 

      m_s2_queue.push_back(trans_desc);
   endfunction: process_s1_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_s2_req
   //
   function void process_s2_req(input evl_dcache_s2_pipe_info_t rtl_info);
      evl_line_desc      line_desc;
      evl_trans_desc     trans_desc;
      evl_way_t          obs_way;
      evl_paddr_t        obs_addr;
      bit                obs_kill;
      bit                obs_nack;
      bit                obs_hit;
      evl_dcache_qual_t  qualifier;
      evl_table_desc     table_desc;
      int                replayq_entry;
      string             extra_info = "";

      trans_desc = m_s2_queue.pop_front();
      if (trans_desc == null) begin
         sb_error("Received an S2 event but no request is expected at S2", null, {}, "", `__FILE__, `__LINE__);
         return;
      end

      obs_way       = evl_way_t'($clog2(rtl_info.way_select));   // our SB model simply use RTL way_select
      obs_addr      = evl_paddr_t'(rtl_info.addr);
      obs_kill      = rtl_info.kill;       // s2_core_kill
      obs_nack      = rtl_info.nack;       // nack and need to be replayed (exclude misaligned case)("s2_nack_pre_misaligned")
      obs_hit       = rtl_info.hit;
      replayq_entry = int'(rtl_info.replay_entry);
      qualifier     = DCQ_None;

      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Received an S2 event for Trans: %s; allocated ReplayQueue entry=%0d (if needed)", trans_desc.sprint_obj(), replayq_entry))

      //
      // Intpipe could kill (s2_core_kill) this request: check that it actually is an intpipe request and it isn't being replayed;
      //
      //if ((obs_kill == 1'b1) && (trans_desc.get_int_param(pk_intpipe_req) == 1)) begin      //(Charles 11/14/2019) change; testing;
      if ((obs_kill == 1'b1) && (trans_desc.get_int_param(pk_intpipe_req) == 1) && (trans_desc.get_int_param(pk_req_replayed, 0) == 0)) begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("Transaction killed by intpipe at S2 (s2_core_kill)"))
         delete_active_desc(trans_desc);
         return;
      end

      ////trans_desc.set_int_param(pk_intpipe_req, 0);    // (Charles 11/14/2019) change; testing; 
          // change to: once set pk_intpipe_req=1, don't clear it any more; 
          // instead, use "pk_req_replayed=1" to indicate Replay Req. 


      // special for Atomic_CAS: only for First Pass (not for Replay from ReplayQ):
      if (trans_desc.get_int_param(pk_req_replayed, 0) == 0) begin
         if ((trans_desc.get_cmd() == ATOMIC) && 
             (trans_desc.get_int_param(pk_dcache_cmd) == dcache_cmd_XA_CMPSWP)) 
         begin
            m_amo_cas_s3_queue.push_back(trans_desc);   // to wait for s3_core_data in S3 stage 
         end
      end

      // Note: Atomic bypass L1D$, so is handled like "uncached"; and it has both load part and store part. 

      //
      // Create the dependency list for the current descriptor, based upon what's in the Replay_Queue
      // (the list of transactions that must complete before this one is allowed to be processed or completed)
      //
      if (trans_desc.get_int_param(pk_req_replayed, 0) == 0) begin  // only on its first Pass

         // new trans checks against m_replay_queue[] (older trans): 
         foreach (m_replay_queue[ii]) begin
            //
            // Stores to the same cacheline cannot be reordered (with respect to other transactions, either Loads or Stores):
            //
            if ((trans_desc.get_cmd() inside {ST, ST_WA, ATOMIC}) || (m_replay_queue[ii].get_cmd() inside {ST, ST_WA, ATOMIC})) begin

               // except for: Prefetches can end up in replay queue but there is no ordering requirement for them
               if ((trans_desc.get_cmd() == CACHE_PREFETCH) || (m_replay_queue[ii].get_cmd() == CACHE_PREFETCH)) begin
                  continue;
               end

               //
               // If new trans is cached (regardless older trans is cached or uncached):
               // If current PA matches an older transaction's PA: address collision: mark as dependency;
               //
               if (trans_desc.is_cached() && (is_cache_collision(trans_desc, m_replay_queue[ii]) == 1'b1)) begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Adding %0s(TrID=%h) as dependency because of address collision (with an older trans in ReplayQ)", m_replay_queue[ii].sprint_obj(), m_replay_queue[ii].get_trans_id() ))
                  trans_desc.add_dep_desc(m_replay_queue[ii]);
               end
               //
               // Uncacheable from same thread must remain in order (regardless of PA match or not):
               // Note: all Uncached Loadt-ypes are inserted in ReplayQ (but not for Uncached Store-types);
               //
               if (m_replay_queue[ii].is_uncached() && trans_desc.is_uncached() &&
                   (m_replay_queue[ii].get_thread_id() == trans_desc.get_thread_id())) 
               begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Adding %0s(TrID=%h) as dependency because of UC ordering (from same thread)(older UC in ReplayQ)", m_replay_queue[ii].sprint_obj(), m_replay_queue[ii].get_trans_id() ))
                  trans_desc.add_dep_desc(m_replay_queue[ii]);
               end
               //
               // Also check for address collision with misaligned (2nd_part):
               //
               if (trans_desc.have_object_param(pk_misaligned_2nd) == 1) begin
                  evl_trans_desc misaligned_desc;
                  $cast(misaligned_desc, trans_desc.get_object_param(pk_misaligned_2nd));
                  if (is_cache_collision(misaligned_desc, m_replay_queue[ii]) == 1'b1) begin
                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), misaligned_desc, $sformatf("(S2) Adding %0s(TrID=%h) as dependency because of address collision (due to new trans misaligned_2nd_part)", m_replay_queue[ii].sprint_obj(), m_replay_queue[ii].get_trans_id() ))
                     misaligned_desc.add_dep_desc(m_replay_queue[ii]);
                  end
               end
               if (m_replay_queue[ii].have_object_param(pk_misaligned_2nd) == 1) begin
                  evl_trans_desc misaligned_desc;
                  $cast(misaligned_desc, m_replay_queue[ii].get_object_param(pk_misaligned_2nd));
                  if (is_cache_collision(misaligned_desc, trans_desc) == 1'b1) begin
                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Adding %0s(TrID=%h) as dependency because of address collision (due to an older trans misaligned_2nd_part)", misaligned_desc.sprint_obj(), misaligned_desc.get_trans_id() ))
                     trans_desc.add_dep_desc(misaligned_desc);
                  end
               end
            end
         end   // foreach (m_replay_queue[ii])

         //
         // All UC from same thread should stay in strict order (regardless of whether address collision):
         // Add a dependency on every uncached request from the same thread that is currently outstanding;
         // Note: Uncached Store-type which enters MissHandler is NOT inserted in ReplayQ; 
         //
         if (trans_desc.is_uncached()) begin
            evl_req_desc outstanding_uc_reqs[$] = {m_pending_miss_reqs, m_pending_evict_reqs, m_active_miss_reqs, m_active_evict_reqs};

            foreach (outstanding_uc_reqs[ii]) begin
               if (   (outstanding_uc_reqs[ii].is_uncached() == 1'b1)
                   && (outstanding_uc_reqs[ii].get_root_trans_desc().get_cmd() inside {ST, LD, ST_WA, LD_RA, ATOMIC, MESSAGE})
                   && (outstanding_uc_reqs[ii].get_thread_id() == trans_desc.get_thread_id()) )
               begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Adding %0s as dependency because of UC ordering (from same thread)(with an older UC in MH)", outstanding_uc_reqs[ii].sprint_obj()))
                  trans_desc.add_dep_desc(outstanding_uc_reqs[ii]);
               end
            end
         end  // if (trans_desc.is_uncached())


         // Special: for VERIF-3538:
         // Those so-called "Local/Global/ Coherent Memory Operations (either scalar or vector)" in PRM-7 Atomic Extension 
         // (e.g. FLWL_PS, FSWL_PS, SBL, ... and FLWG_PS, FSWG_PS, SBG, ... etc):
         // They bypass L1D$, directly access L2 (local) or L3 (gobal); so L1D$ handles them like "uncached":
         // as dcache_cmd_XRDB (LD_RA) and dcache_cmd_XWRB (ST_WA).
         // Among them, the Store_Bypass will enter MissHandler, but will not be inserted in ReplayQ (like other "uncached" Stores).
         // In our tests: it isn't illegal for them to mix with normal cached requests and they both access same clines (although
         // it probably does not have much meaningful practical usage).
         // L1D RTL still checks for address collision for a new incoming cached Req against any older Load_Byp (in ReplayQ) or Store_Byp (in MH);
         // so this Scoreboard needs to model that too (for both cases above: older Load_Byp and Store_Byp). 

         if (trans_desc.is_cached()) begin
            evl_req_desc    outstanding_uc_reqs[$] = {m_pending_miss_reqs, m_pending_evict_reqs, m_active_miss_reqs, m_active_evict_reqs};
            evl_trans_desc  outstanding_uc_tran;

            foreach (outstanding_uc_reqs[ii]) begin
               outstanding_uc_tran = outstanding_uc_reqs[ii].get_root_trans_desc();
               if (   (outstanding_uc_reqs[ii].is_uncached() == 1'b1)
                   && (   (outstanding_uc_tran.get_cmd() == ST_WA)         // should we include other uncached_ST ?
                       || (outstanding_uc_tran.get_cmd() == LD_RA) )       // (Charles) added 4/28/2021
                   && (is_cache_collision(trans_desc, outstanding_uc_tran) == 1'b1) )
               begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Adding %0s as dependency because of address collision (with an older ST_WA or LD_RA still pending)", outstanding_uc_reqs[ii].sprint_obj()))
                  trans_desc.add_dep_desc(outstanding_uc_reqs[ii]);
               end
            end
         end  // if (trans_desc.is_cached())

      end  // if (trans_desc.get_int_param(pk_req_replayed, 0) == 0) 

      // For requests that have had a bus error
      // Simply drop it.
      if (trans_desc.get_int_param(pk_bus_err) == 1) begin
         qualifier = DCQ_BusErr;
      end
      // For Uncached (and Atomics) that have seen their response,
      // set the qualifier to complete the transaction
      else if (trans_desc.get_int_param(pk_uc_resp) == 1) begin
         qualifier = DCQ_Resp;
      end
      else if (trans_desc.is_uncached() == 1'b1) begin
         qualifier = DCQ_Uncached;
      end


      //
      // If older transactions still need to be processed first, force a replay of new trans;
      //
      if (trans_desc.has_dependency()) begin
         int            my_dep_q_size;  // size of m_depend_queue[] of trans_desc 
         evl_mem_desc   my_dep_mem_desc;    // the last element of m_depend_queue[] of trans_desc
         evl_trans_desc my_dep_trans_desc;
         evl_req_desc   my_dep_req_desc;
         dcache_mh_state  my_mh_state, my_mh_state_next;

         // special for VERIF-3538:
         // If curret dependent is ST_WA or LD_RA: special: RTL signal "s2_nack_pre_misaligned" does not assert;
         // Instead, need to cross-check with MissHandler signals (should not start MH FSM);

         my_dep_q_size   = trans_desc.depend_size();
         my_dep_mem_desc = trans_desc.get_dep_desc(my_dep_q_size-1);  // the last element is the current dependent

         // note: my_dep_mem_desc could be either evl_trans_desc or evl_req_desc:
         if($cast(my_dep_trans_desc, my_dep_mem_desc) == 0) begin
            $cast(my_dep_req_desc, my_dep_mem_desc);
            my_dep_trans_desc = my_dep_req_desc.get_root_trans_desc();
         end 

         //
         // Check that RTL is Nacking it too (will replay it): 
         //
         // special for VERIF-3538: for both ST_WA and LD_RA (as older reqs):
         if((my_dep_trans_desc.get_cmd() == ST_WA) || (my_dep_trans_desc.get_cmd() == LD_RA)) begin
            bit  mh_accepted_not_assert = 1'b0;
            int  my_mh_id;

            // special: RTL signal "s2_nack_pre_misaligned" does not assert;
            // Instead, need to cross-check with MissHandler signals (should not start MH FSM);

            if(rtl_info.mh_accepted[1:0] == 2'b01) begin   // MH_0
               my_mh_state      = rtl_info.mh0_state;
               my_mh_state_next = rtl_info.mh0_state_next;
               my_mh_id = 0;
            end 
            else if(rtl_info.mh_accepted[1:0] == 2'b10) begin  // MH_1
               my_mh_state      = rtl_info.mh1_state;
               my_mh_state_next = rtl_info.mh1_state_next;
               my_mh_id = 1;
            end 
            else begin
               mh_accepted_not_assert = 1'b1;
            end
 
            if((mh_accepted_not_assert == 1'b0) && 
               ((my_mh_state == 4'h0) && (my_mh_state_next != 4'h0)))
            begin
               sb_error($sformatf("(S2) SB expected this %0s to be NACK'ed but RTL accept it in MH(%0d) and starting MH FSM", trans_desc.sprint_obj(), my_mh_id), trans_desc, {}, "", `__FILE__, `__LINE__);
               if (m_enabled == 1'b1) trans_desc.print_dep_descs(1'b1, UVM_LOW, m_vmod);
            end
            else begin
               qualifier = DCQ_Replay;
               `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Will replay this %0s because a dependency (older) is not completed yet", trans_desc.sprint_obj()))
               trans_desc.print_dep_descs(1'b1, UVM_DEBUG, m_vmod);
            end
         end
         else begin  // if dependent is NOT a ST_WA or LD_RA:
            if (obs_nack == 1'b0) begin
               sb_error($sformatf("(S2) SB expected this %0s to be NACK'ed but RTL is not asserting s2_nack", trans_desc.sprint_obj()), trans_desc, {}, "", `__FILE__, `__LINE__);
               if (m_enabled == 1'b1) trans_desc.print_dep_descs(1'b1, UVM_LOW, m_vmod);
            end
            else begin
               qualifier = DCQ_Replay;
               `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Will replay this %0s because a dependency (older) is not completed yet", trans_desc.sprint_obj()))
               trans_desc.print_dep_descs(1'b1, UVM_DEBUG, m_vmod);
            end
         end
      end
      //
      // As we don't track resource availability, RTL might have another
      // reason to replay the request, so just accept it if observed
      //
      else if (obs_nack == 1'b1) begin
         // (CYE/Charles 11/06/19): don't drop bus_err if RTL decides to replay (for any reason);
         //if (qualifier == DCQ_BusErr) begin
         //   sb_error($sformatf("(S2) Expected this %0s to be dropped because of BusErr response but RTL is going to replay it", trans_desc.sprint_obj()), trans_desc, {}, "", `__FILE__, `__LINE__);
         //end
         //else begin
            qualifier = DCQ_Replay;
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Will replay this %0s because RTL didn't have available resource", trans_desc.sprint_obj()))
         //end
      end
      else if (qualifier == DCQ_BusErr) begin
         `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) will drop this above trans due to BusErr response"))
         //delete_replay_item(trans_desc);
         //return;
      end

      // if (qualifier != DCQ_Replay): to call function process_request (to do Tag+State Lookup etc); 
      if (qualifier != DCQ_Replay) begin
         line_desc = get_cache(trans_desc).get_line_desc(trans_desc.get_paddr());
         trans_desc.set_line_desc(line_desc);

         trans_desc.set_int_param(pk_mh_assigned, rtl_info.mh_assigned);  // 1=MH0 2=MH1 (original RTL [1:0] one-hot signal)
         trans_desc.set_int_param(pk_mh_accepted, rtl_info.mh_accepted);  // 1=MH0 2=MH1 (original RTL [1:0] one-hot signal)

         table_desc = process_request(trans_desc, line_desc, obs_way, qualifier);
      end

      // if (qualifier == DCQ_Replay): don't need to call function process_request (to do Tag+State Lookup etc); 
      // Instead we could simply push it to replay queue and then stop ... See code below.
      //
      // If request will need to be replayed or it is misaligned and the second part will need to be replayed, push it to the replay queue
      //
      if ((qualifier == DCQ_Replay) ||       // case of "decide to Replay before Tag+State LookUp"
          ((table_desc.get_err_code() inside {EC_Replay}) || (trans_desc.have_object_param(pk_misaligned_2nd) == 1))) 
      begin
         // Push request to the replay queue if it's not already there
         if (trans_desc.get_int_param(pk_req_replayed, 0) == 0) begin
            if (!m_replay_queue.exists(replayq_entry)) begin
               m_replay_queue[replayq_entry] = trans_desc;
               `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Pushing this %0s to ReplayQueue entry %0d", trans_desc.sprint_obj(), replayq_entry))
            end
            else if (m_replay_queue[replayq_entry] != trans_desc) begin
               sb_error($sformatf("(S2) Trying to push this %0s on a replay queue entry (%0d) currently used by %0s", trans_desc.sprint_obj(), replayq_entry, m_replay_queue[replayq_entry].sprint_obj()), trans_desc, {}, "", `__FILE__, `__LINE__);
            end
         end
         trans_desc.set_int_param(pk_replay_ready, 1);
      end
      else begin
         delete_replay_item(trans_desc);
      end

      // check actual core response: see code below: done;

      if ((qualifier != DCQ_Replay) &&        // case of "decide to Replay before Tag+State LookUp"
          (table_desc.get_bus_rsp() != RspNull)) 
      begin
         if (trans_desc.has_dependency()) begin
            sb_error($sformatf("(S2) Request completed before some of its dependencies (older): see below:"), trans_desc, {}, "", `__FILE__, `__LINE__);
            if (m_enabled == 1'b1) trans_desc.print_dep_descs(1'b1, UVM_LOW, m_vmod);
         end
         delete_replay_item(trans_desc);
         //delete_active_desc(trans_desc);   // changed due to code below

         if (qualifier == DCQ_BusErr) begin
            delete_active_desc(trans_desc);
            // FUTURE: expect L1D$ to send signals "bus_err" and "bus_err_addr" to IntPipe ... 
         end
         // regarding s3_core_resp:
         else if (   ((trans_desc.get_cmd() == LD) && (table_desc.get_data_action() == Data_MemRd))         // for Load-type (cached)
                  || ((trans_desc.get_cmd() inside {LD, LD_RA}) && (table_desc.get_bus_rsp() == RspWData))  // for Load-type (uncached)
                  || ((trans_desc.get_cmd() == ATOMIC) && (table_desc.get_bus_rsp() == RspWData)) )        // for Atomic (return original memory value)
         begin
            // exclude: dest_reg is x0 and desp_is_not_for_FP:
            if ((trans_desc.get_int_param(pk_dest_reg) == 5'h0) && (trans_desc.get_int_param(pk_dest_is_fp) == 0)) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) for above Load-type or Atomic: read data is ready: but dest_reg is x0 (not for FP): will not return s3_core_resp" )) 
               delete_active_desc(trans_desc);
            end

            // for misaligned access (crossing cline boundary): L1D$ splits into 2 trans internally; 
            // but will not return s3_core_resp when 1st part is completed;
            else if (trans_desc.get_int_param(pk_this_is_misaligned_access) == 1) begin  // misaligned first part
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) for above Load-type or Atomic: read data is ready: but this_is_misaligned_access (1st_part): does not return s3_core_resp (at this time) yet" )) 
               //delete_active_desc(trans_desc);   // will delete later (together with 2nd_part)
            end

            // for misaligned access (crossing cline boundary): L1D$ splits into 2 trans internally; 
            // but return only one s3_core_resp after 2nd part is completed;
            else if (trans_desc.get_int_param(pk_this_is_misaligned_2nd_part) == 1) begin
               evl_trans_desc  orig_misaligned_trans_desc;

               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) for above Load-type or Atomic: read data is ready: this_is_misaligned_2nd_part: will return s3_core_resp ..."))

               // find the original Load-type trans_desc: 1st_part: 
               $cast(orig_misaligned_trans_desc, trans_desc.get_object_param(pk_misaligned_1st)); 

               if (m_en_s3_core_resp_chk == 1'b1) begin
                  // (CYE) enqueue the orig_misaligned_trans_desc into m_s3_core_resp_queue[]: for S3 self-checking: 
                  m_s3_core_resp_queue.push_back(orig_misaligned_trans_desc);      // with m_read_data 
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) ... push orig_misaligned_trans_desc into m_s3_core_resp_queue[]: %s", orig_misaligned_trans_desc.sprint_obj() ))
                  delete_active_desc(trans_desc);  // delete 2nd_part here; will delete 1st_part in function check_s3_core_resp
               end
               else begin
                  delete_active_desc(trans_desc);                  // 2nd_part
                  delete_active_desc(orig_misaligned_trans_desc);  // 1st_part
               end
            end

            else begin
               if (m_en_s3_core_resp_chk == 1'b1) begin
                  // (CYE) enqueue the trans_desc into m_s3_core_resp_queue[]: for S3 self-checking: 
                  m_s3_core_resp_queue.push_back(trans_desc);      // with m_read_data
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) for above Load-type or Atomic: read data is ready: push trans_desc (with m_read_data) into m_s3_core_resp_queue[]")) 
               end 
               else begin
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG) for above Load-type or Atomic: read data is ready ...")) 
                  delete_active_desc(trans_desc);
               end
            end
         end
         else begin
            delete_active_desc(trans_desc);
         end
      end  // if (table_desc.get_bus_rsp() != RspNull)

      update_fence(trans_desc);

   endfunction: process_s2_req


   //
   // Function: process_s3_core_x31: for Atomic_CAS: assign/update m_write_data for trans_desc 
   // 
   function void process_s3_core_x31(input evl_dcache_s3_core_x31_t rtl_info);
      evl_trans_desc  trans_desc;
      bit [63:0]   amo_cmp_data;
      bit [63:0]   amo_new_data;
      evl_xword_t  atomic_data;    // hexword (64-byte)
      int          cmd_length;
      bit [63:0]   byte_en;

      if (m_amo_cas_s3_queue.size() == 0) begin
         return;   // if there is no amo_cas_s3 pending: skip this function;
      end

      // continue only if it's for a pending Atomic_CAS:
      trans_desc = m_amo_cas_s3_queue.pop_front();
      amo_cmp_data  = rtl_info.amo_cmp_data;   // s3_core_x31 only available at S3 stage (and beyond)
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) Received s3_core_x31=0x%h for Atomic_CAS %0s", amo_cmp_data, trans_desc.sprint_obj())) 
     
      if (m_amocmpexch_fix_en == 1'b1) begin
 
         // obtain from m_write_data: Atomic (new) data is always in LSBs:
         atomic_data = trans_desc.get_write_xword(0);  // get entire 64-byte hexword (contain only amo_new_data right now)
         amo_new_data = atomic_data[63:0];

         // special for Atomic_CAS: put amo_cmp_data on second double-word; 
         atomic_data |= (amo_cmp_data[63:0] << 64);
         // need max 16 bytes to cover TWO double-words (amo_new_data[63:0] and amo_cmp_data[63:0]);
         cmd_length = 16;    

         // size: word or double-word;
         case(trans_desc.get_int_param(pk_access_type))
           dcache_type_W, dcache_type_WU:  // word
                 byte_en = 64'h0f0f;
           dcache_type_D:                  // double-word
                 byte_en = 64'hffff;
           default:    // should be an Error? 
                 byte_en = 64'hffff;
         endcase
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S3) Atomic_CAS: amo_new_data=0x%h amo_cmp_data=0x%h atomic_data=0x%h cmd_length=%0d byte_en=0x%h: will update/assign to data and valid bits of evl_line_data (via m_write_data) of this trans_desc", amo_new_data, amo_cmp_data, atomic_data, cmd_length, byte_en))

         // assign/update m_write_data (and m_valid bits):
         trans_desc.set_write_bytes(0, cmd_length, atomic_data, byte_en);
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            trans_desc.get_write_data().print_obj(get_abstract_name());
         end

      end  // if (m_amocmpexch_fix_en == 1'b1)

   endfunction: process_s3_core_x31
   


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_amo_tensor
   //
   //   This function returns 1 if the transaction passed is either a tensor operation or an atomic
   //   else it returns 0.
   //
   function bit is_amo_tensor(input evl_trans_desc trans_desc);
      if (trans_desc.get_cmd() inside {`EVL_TENSOR_CMDS, ATOMIC}) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction: is_amo_tensor


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_amo_tensor_cop
   //
   //   This function returns 1 if the transaction passed is either a tensor operation, an atomic
   //   or a cache operation, else it returns 0.
   //
   function bit is_amo_tensor_cop(input evl_trans_desc trans_desc);
      if (trans_desc.get_cmd() inside {`EVL_COP_CMDS, `EVL_TENSOR_CMDS, ATOMIC}) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction: is_amo_tensor_cop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_amo
   //
   //   This function returns 1 if the transaction passed is either a tensor operation, an atomic
   //   or a cache operation, else it returns 0.
   //
   function bit is_amo(input evl_trans_desc trans_desc);
      if (trans_desc.get_cmd() inside {ATOMIC}) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction: is_amo


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_pma
   //
   //   This function checks if an access is legal or should cause a fault.
   //   Additionally it determines the cacheability.
   //
   //   See PRM 19. Chapter 2.7 Physical Memory Attributes
   //   Also see:
   //    :/dv/common/base/evl_base_pkg.sv: for ET-SOC Memory Map;
   //    :/dv/top/evl_top_env.sv: for all add_addr_region() calls;
   //    :/dv/common/base/evl_mem_map_pk_inc.svh : for all pk_xxx_region ... declaration/definition;
   //    :/dv/soc/ioshire/standalone_minion/evl_standalone_minion_env.sv : also for pk_xxx_region ... declaration/definition; (NOT needed?)
   //
   function bit check_pma(input evl_trans_desc trans_desc);
      evl_paddr_t paddr;
      int         region_key;
      string      region_info;
      bit         access_allowed;
      bit         cached;
      string      pma_resp_info;
      bit         rtlmin_6148_bug = 1'b0;

      paddr = trans_desc.get_paddr();
      region_key = get_addr_region_key(paddr);
      region_info = $sformatf("%0s", evl_param_xref::get_param_name(region_key)); 

      access_allowed = 1'b0;

      //
      // IO Region
      //
      //   - No AMOs, Tensors, CacheOps
      //   - Access is 64-bit aligned.
      //   - The read or write access is from a 32-wide load or store.
      //   - Uncached
      //
      if (addr_in_region(pk_io_region, paddr)) begin
         // TODO: Ying to review mprot
         access_allowed = (is_amo_tensor_cop(trans_desc) == 1'b1)                   ? 1'b0 :
                          ((paddr & ~const_word_paddr_mask) != 0)                   ? 1'b0 :
                          (trans_desc.get_cmd_length() != 8)                        ? 1'b0 :
                          /*(m_mprot_esr.io_access_mode == prv_hypervisor)            ? 1'b0 :*/
                          (prv_supervisor > trans_desc.get_privilege())             ? 1'b0 : 1'b1;
         trans_desc.set_uncached();
      end
      //
      // System Registers region
      //
      //   - The harts execution mode is equal or higher to the S privilege mode.
      //   - Access is 64-bit aligned.
      //   - The read or write access is from a 32-wide load or store.
      //   - The instruction generating the access is not an AMO, or a TensorOp, or a CacheOp.
      //   - Uncached
      //
      else if (addr_in_region(pk_systemreg_region, paddr)) begin
         access_allowed = (is_amo_tensor_cop(trans_desc) == 1'b1)                   ? 1'b0 :
                          ((paddr & ~const_word_paddr_mask) != 0)                   ? 1'b0 :
                          (trans_desc.get_cmd_length() != 8)                        ? 1'b0 :
                          (prv_supervisor > trans_desc.get_privilege())             ? 1'b0 : 1'b1;
         trans_desc.set_uncached();
      end
      //
      // MRAM Registers region
      //
      //   - The harts execution mode is equal or higher to the M privilege mode.
      //   - Access is 64-bit aligned.
      //   - The read or write access is from a 64-wide load or store.
      //   - The instruction generating the access is not an AMO, or a TensorOp, or a CacheOp.
      //   - Uncached
      //
      else if (addr_in_region(pk_mramreg_region, paddr)) begin
         access_allowed = (is_amo_tensor_cop(trans_desc) == 1'b1)                   ? 1'b0 :
                          ((paddr & ~const_dword_paddr_mask) != 0)                  ? 1'b0 :
                          (trans_desc.get_cmd_length() != 8)                        ? 1'b0 :
                          (prv_machine != trans_desc.get_privilege())               ? 1'b0 : 1'b1;
         trans_desc.set_uncached();
      end
      //
      // Hyperbus Registers region
      //
      //   - The harts execution mode is equal or higher to the M privilege mode.
      //   - Access is 64-bit aligned.
      //   - The read or write access is from a 32-wide load or store.
      //   - The instruction generating the access is not an AMO, or a TensorOp, or a CacheOp.
      //   - Uncached
      //
      else if (addr_in_region(pk_hyperbus_region, paddr)) begin
         access_allowed = (is_amo_tensor_cop(trans_desc) == 1'b1)                   ? 1'b0 :
                          ((paddr & ~const_word_paddr_mask) != 0)                   ? 1'b0 :
                          (trans_desc.get_cmd_length() != 8)                        ? 1'b0 :
                          (prv_machine != trans_desc.get_privilege())               ? 1'b0 : 1'b1;
         trans_desc.set_uncached();
      end
      //
      // BootROM region
      //
      //   - The harts execution mode is any privilege mode.
      //   - The instruction generating the access is not an AMO, or a TensorOp.
      //   - Cached
      //
      else if (addr_in_region(pk_bootrom_region, paddr)) begin
         access_allowed = (is_amo_tensor(trans_desc) == 1'b1)                       ? 1'b0 :
                          (trans_desc.get_int_param(pk_dcache_cmd_is_write) == 1)   ? 1'b0 : 1'b1;
         trans_desc.set_cached();
      end
      //
      // SRAM region
      //
      //   - The harts execution mode is any privilege mode.
      //   - The instruction generating the access is not an AMO.
      //   - Cached
      //
      else if (addr_in_region(pk_sram_region, paddr)) begin
         access_allowed = (is_amo(trans_desc) == 1'b1)                       ? 1'b0 : 1'b1;
         trans_desc.set_cached();
      end
      //
      // ESR region
      //
      //   - The harts execution mode is equal or higher to the ESR privilege mode.
      //   - If the ESR privilege mode is 2, the access is only permitted if issued by the SvcProc
      //   - Access is 64-bit aligned.
      //   - The read or write access is from a 64-wide load or store.
      //   - The instruction generating the access is not an AMO, or a TensorOp, or a CacheOp.
      //   - Uncached
      //
      else if (addr_in_region(pk_esr_region, paddr)) begin
         access_allowed = (evl_privilege_t'(paddr[`EVL_ESR_PP_BITS])  > trans_desc.get_privilege())               ? 1'b0 :
                          ((evl_privilege_t'(paddr[`EVL_ESR_PP_BITS]) == prv_hypervisor) && (in_ioshire == 1'b0)) ? 1'b0 :
                          ((paddr & ~const_dword_paddr_mask) != 0)                                                ? 1'b0 :
                          (trans_desc.get_cmd_length() != 8)                                                      ? 1'b0 :
                          (is_amo_tensor_cop(trans_desc) == 1'b1)                                                 ? 1'b0 : 1'b1;
         trans_desc.set_uncached();
         if (access_allowed == 1'b1) begin
            trans_desc.set_int_param(pk_access_esr_region, 1);
         end
      end      
      
      // MRAM
      //
      // TODO: Ying to add pmp regions
      else if (addr_in_region(pk_mram_region, paddr)) begin
         access_allowed = 1'b1;
         trans_desc.set_cached();
         region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_mram_region));
      end

      //
      // mprot[6] secure enabled:  (RTLMIN-5656)
      // DRAM: Low/cached M code subregion
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  R/X(I-Fetch) accessible in M-mode only; (L1D$ does not care about "X"(I-Fetch) ...)
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_secure_c_m_code_region, paddr)) begin
      //   access_allowed = (in_ioshire == 1'b1)                        ? 1'b1 :   
      //                    (trans_desc.get_privilege() == prv_machine) ? ((trans_desc.get_int_param(pk_dcache_cmd_is_write) == 1'b0)? 1'b1 : 1'b0) 
      //                                                                : 1'b0 ;
      //   trans_desc.set_cached();
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_secure_c_m_code_region));
      //end
      //
      // mprot[6] secure enabled:
      // DRAM: Low/cached M data subregion
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  R/W accessible in M-mode only;
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_secure_c_m_data_region, paddr)) begin
      //   access_allowed = (in_ioshire == 1'b1)                        ? 1'b1 :   
      //                    (trans_desc.get_privilege() == prv_machine) ? 1'b1 : 1'b0;
      //   trans_desc.set_cached();
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_secure_c_m_data_region));
      //end
      //
      // mprot[6] secure enabled:
      // DRAM: Low/cached S code subregion
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  R/W in M-mode, R/X(I-Fetch) in S-mode; (L1D$ does not care about "X"(I-Fetch) ...)
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_secure_c_s_code_region, paddr)) begin
      //   access_allowed = (in_ioshire == 1'b1)                           ? 1'b1 :   
      //                    (trans_desc.get_privilege() == prv_machine)    ? 1'b1 : 
      //                    (trans_desc.get_privilege() == prv_supervisor) ? ((trans_desc.get_int_param(pk_dcache_cmd_is_write) == 1'b0)? 1'b1 : 1'b0) 
      //                                                                   : 1'b0 ;
      //   trans_desc.set_cached();
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_secure_c_s_code_region));
      //end
      //
      // mprot[6] secure enabled:
      // DRAM: Low/cached S data subregion
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  R/W in M-mode and S-mode;
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_secure_c_s_data_region, paddr)) begin
      //   access_allowed = (in_ioshire == 1'b1)                             ? 1'b1 :   
      //                    ((trans_desc.get_privilege() == prv_machine) ||
      //                     (trans_desc.get_privilege() == prv_supervisor)) ? 1'b1 : 1'b0;
      //   trans_desc.set_cached();
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_secure_c_s_data_region));
      //end
      //
      // mprot[6] secure enabled:
      // DRAM: Low/cached OS subregion
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  when mprot[3]==0: R/W in M/S-mode, R/W/X in U-mode; (L1D$ does not care about "X"(I-Fetch) ...)
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_secure_c_os_region, paddr)) begin
      //   access_allowed = (in_ioshire == 1'b1)                       ? 1'b1 : 
      //                    (m_mprot_esr.disable_osbox_access == 1'b0) ? 1'b1 : 1'b0;
      //   trans_desc.set_cached();
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_secure_c_os_region));
      //end
      //
      // mprot[6] secure enabled:
      // DRAM: Low/cached memory region
      //   - SvcProc: R/W accessable in all modes;
      //   - Minion:  R/W in M/S-mode, R/W/X in U-mode; (but L1D$ does not care about "X"(I-Fetch) ...)
      //
      // TODO: Ying to fix mprot
      //else if ((m_mprot_esr.enable_secure_memory == 1'b1) && addr_in_region(pk_c_mem_region, paddr)) begin
      //   access_allowed = 1'b1;
      //   trans_desc.set_cached();
      //   if (((m_mprot_esr.dram_size[1:0] == 2'b00) && (paddr[37:30] >= 'd8))  ||       // size=8G  but PA is over 8G
      //       ((m_mprot_esr.dram_size[1:0] == 2'b01) && (paddr[37:30] >= 'd16)) ||       // size=16G but PA is over 16G
      //       ((m_mprot_esr.dram_size[1:0] == 2'b10) && (paddr[37:30] >= 'd24)) ||       // size=24G but PA is over 24G
      //       ((m_mprot_esr.dram_size[1:0] == 2'b11) && (paddr[37:30] >= 'd32)) )        // size=32G but PA is over 32G
      //   begin
      //      access_allowed = 1'b0;
      //   end
      //   region_info = $sformatf("%0s (%0s)", region_info, evl_param_xref::get_param_name(pk_c_mem_region));
      //end

      //
      // AMOs and Read-around/Write-around are never cached in L1
      //
      if (trans_desc.get_cmd() inside {ATOMIC, LD_RA, ST_WA}) begin
         trans_desc.set_uncached();
      end

      //
      // When in bypass mode, normal loads/stores are uncached
      //
      if ((trans_desc.get_cmd() inside {ST, LD}) && (m_bypass == 1'b1)) begin
         trans_desc.set_uncached();
      end

      pma_resp_info = access_allowed ? $sformatf("legal%0s", trans_desc.is_cached() ? " (cached)" : " (uncached)") : "FAULT"; 
      //`evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s - PMA check: accessing %0s with privilege %0s -> %0s", trans_desc.sprint_obj(), evl_param_xref::get_param_name(region_key), trans_desc.get_privilege().name, pma_resp_info ))
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S1) %0s - PMA check: accessing %0s with privilege %0s -> %0s", trans_desc.sprint_obj(), region_info, trans_desc.get_privilege().name, pma_resp_info ))

      if (rtlmin_6148_bug == 1'b1) begin
         sb_error($sformatf("(S1) hit RTLMIN-6148 bug or VERIF-3630: SvcProc CacheOp_Pref or other CacheOps accessing Uncached DRAM High memory region"), trans_desc, {}, "", `__FILE__, `__LINE__);  
      end

      return access_allowed;
   endfunction: check_pma


   //-----------------------------------------------------------------------------------------------
   //
   // Function: l1sb_pma_cross_check_rtl 
   //   L1SB PMA check result cross-check against RTL PMA check
   // 
   function void l1sb_pma_cross_check_rtl(input bit                 l1sb_pma_fault, 
                                          input evl_dcache_pma_resp rtl_pma_resp,
                                          input bit                 xcpt_qualifier = 1'b0,
                                          input evl_dcache_tag_xcpt rtl_core_xcpt = 'h0,
                                          input bit                 rtl_core_xcpt_mf = 1'b0
                                         );
      bit  rtl_core_xcpt_af;    // only for pma_access_fault, exclude rtl_core_xcpt_mf (misalign_fault)

      if (l1sb_pma_fault == 1'b1) begin         // L1SB PMA check finds fault
         if (rtl_pma_resp.access_fault == 1'b0) begin      // RTL shows no pma access_fault
            sb_error($sformatf("(for above Core Req) L1SB PMA check found Access_Fault but RTL did not (see signal s1_pma_resp_data)"), null, {}, "", `__FILE__, `__LINE__);
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING)(for above Core Req) L1SB PMA check found Access_Fault but RTL did not (see signal s1_pma_resp_data)")) 
         end
      end
      else begin    // L1SB PMA check finds NO fault
         if (rtl_pma_resp.access_fault == 1'b1) begin    // RTL shows pma access_fault
            sb_error($sformatf("(for above Core Req) L1SB PMA check found Legal but RTL found Access_Fault (see signal s1_pma_resp_data)"), null, {}, "", `__FILE__, `__LINE__);
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING)(for above Core Req) L1SB PMA check found Legal but RTL found Access_Fault (see signal s1_pma_resp_data)")) 
         end
      end

      // cross-check RTL s1_core_xcpt Resp (L1D->Core): 
      // cross-check RTL signal: s1_core_xcpt only for pma_access_fault (and ignore s1_mf_xcpt (misalign_fault)):
      ////rtl_core_xcpt_af = (rtl_core_xcpt.af_ld | rtl_core_xcpt.af_st) & ~rtl_core_xcpt_mf;  
          // the above is incorrect: pma_access_fault and misalign_fault both could occur at same time for a same trans;
      rtl_core_xcpt_af = (rtl_core_xcpt.af_ld | rtl_core_xcpt.af_st) & rtl_pma_resp.access_fault; 

      // when (xcpt_qualifier==1): only for Core_Req (not_killed), Replay_Req, CacheOp_Pref_L1;
      if (xcpt_qualifier == 1'b1) begin
         if (l1sb_pma_fault != rtl_core_xcpt_af) begin
            sb_error($sformatf("(for above Core Req) PMA Access_Fault->Expt mismatch: L1SB=%0b RTL=%0b (s1_core_xcpt.af for pma_access_fault)", l1sb_pma_fault, rtl_core_xcpt_af), null, {}, "", `__FILE__, `__LINE__);
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING)(for above Core Req) PMA Access_Fault->Expt mismatch: L1SB=%0b RTL=%0b (s1_core_xcpt.af for pma_access_fault)", l1sb_pma_fault, rtl_core_xcpt_af))
         end 
      end
      else begin   // do not expect Dcache raise exception, regardless of PMA access_fault;
         if (rtl_core_xcpt_af != 1'b0) begin   // not expect RTL dcache raise exception
            sb_error($sformatf("(for above Req) Dcache RTL returned Exception Access_Fault (s1_core_xcpt.af for pma_access_fault): unexpected"), null, {}, "", `__FILE__, `__LINE__);
            //`evl_log(UVM_WARNING, m_vmod, get_abstract_name(), $sformatf("(WARNING)(for above Req) Dcache RTL returned Exception Access_Fault (s1_core_xcpt.af for pma_access_fault): unexpected"))
         end
      end
 
   endfunction: l1sb_pma_cross_check_rtl



   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_cache_collision
   //
   //   This function determines if two accesses collide (=they access the same LRAM block) which
   //   would mean they cannot be reordered.
   //
   function bit is_cache_collision(input evl_trans_desc trans_desc,
                                   input evl_trans_desc other_desc);
      evl_paddr_t base_0;
      evl_paddr_t base_1;
      evl_paddr_t top_0;
      evl_paddr_t top_1;
      bit         collides;

      // LRAM blocks are 8 bytes wide. Calculate the boundaries of the accesses accordingly.
      base_0 = trans_desc.get_paddr() & ~(evl_paddr_t'(7));
      base_1 = other_desc.get_paddr() & ~(evl_paddr_t'(7));
      top_0 = (trans_desc.get_paddr() + trans_desc.get_cmd_length() - 1) | evl_paddr_t'(7);
      top_1 = (other_desc.get_paddr() + other_desc.get_cmd_length() - 1) | evl_paddr_t'(7);

      if (base_1 >= base_0) begin
         return (top_0 >= base_1) ? 1'b1 : 1'b0;
      end
      else begin
         return (top_1 >= base_0) ? 1'b1 : 1'b0;
      end
   endfunction: is_cache_collision


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_request: Dcache S2 stage:
   //
   function evl_table_desc process_request(input evl_trans_desc    trans_desc,
                                           input evl_line_desc     line_desc,
                                           input evl_way_t         way,
                                           input evl_dcache_qual_t qualifier,
                                           input evl_paddr_t       set_in = ~evl_paddr_t'(0)
                                          );
      evl_table_desc                 table_desc;        // output return
      evl_line_desc                  prev_line_desc;    // replaced cline
      //string                         action_str;
      //evl_abstract_bus_cmd_t         abstract_bus_cmd;
      //evl_abstract_rsp_type_t        abstract_bus_rsp;
      //evl_error_code                 error_code;
      evl_minion_dcache#(CACHE_TYPE) cache;
      evl_paddr_t                    set;
      int                            miss_handler_id;   // 0=MH0 1=MH1 

      cache = get_cache(trans_desc);
      set   = (set_in != ~evl_paddr_t'(0)) ? set_in : get_cache_set_addr(trans_desc);

      //
      // Perform the cache Tag+State lookup.
      //
      cache.req_lookup(table_desc, trans_desc.get_cmd(), line_desc.get_cache_st(), qualifier);

      //abstract_bus_cmd  = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
      //abstract_bus_rsp  = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
      //error_code        = evl_error_code'(table_desc.get_err_code());
      //action_str        = $sformatf("tag action=%0p data_action=%0p bus_cmd=%0s bus_rsp=%0s err_code=%0s", table_desc.get_tag_action(), table_desc.get_data_action(), abstract_bus_cmd.name, abstract_bus_rsp.name, error_code.name);
      //`evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Tag+State Lookup for %0s (%0p) --> table_desc: %0s", trans_desc.sprint_obj(), line_desc.get_cache_st(), action_str))

      // changed to use table_desc.ssprint_obj() as below:
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) Tag+State Lookup for %0s (%0p) --> table_desc: %0s", trans_desc.sprint_obj(), line_desc.get_cache_st(), table_desc.ssprint_obj()))

      //
      // Process the tag action as necessary.
      //
      if (table_desc.get_tag_action() != St_Ignore) begin
         if (line_desc.get_cache_st() == I) begin
            string  prev_line_info = "";
            prev_line_desc = cache.allocate_way(way, line_desc);
            if (prev_line_desc != null) begin
               prev_line_info = $sformatf(" (replaced cline PA=0x%x State=%p)", prev_line_desc.get_paddr(), prev_line_desc.get_cache_st());
               // OR: code as below (same info):
               // prev_line_info = $sformatf(" (replaced %s)", prev_line_desc.sprint_obj());  // same info
            end 
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("(S2) allocated set=%0d way=%0s for PA=0x%x%s", set, get_way_name(way), line_desc.get_paddr(), prev_line_info))
         end
         if (table_desc.get_tag_action() inside {IE_Bsy, IM_Bsy, EM_Bsy, LD_Bsy, CT_Bsy}) begin
            m_mh_tag_update_q.push_back(trans_desc);
            //
            // When a way is busy with a miss handler, it might as well be treated as soft locked until
            // the Miss Handler updates the tag, as it cannot be used for way allocation.
            // Mark the way as "soft locked" and we will remove that flag once the MH has updated the tag.
            //
            trans_desc.get_line_desc().set_int_param(pk_soft_locked, 1);
         end

         //
         // If we have replaced a cache line, enqueue an eviction if it was modified.
         //
         if (prev_line_desc != null) begin
            capacity_evict(trans_desc, prev_line_desc, way);
         end

         //
         // If we are invalidating the line, deallocate the way
         //
         if (table_desc.get_tag_action() inside {I}) begin
            void'(cache.deallocate_way(line_desc));
         end
         line_desc.set_cache_st(table_desc.get_tag_action());
      end

      //
      // Process the bus action as necessary: predict sending out L2_Req descriptor;
      //
      if (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) != BusIdle) begin
         evl_req_desc l2_req_desc;
         evl_paddr_t  paddr;
         bit [1:0]    my_mh_assigned;
         bit [1:0]    my_mh_accepted;

         // double-check that RTL "s2_mh_req_assigned[1:0]" and "s2_mh_req_accepted[1:0]" are consistent: (both should be 1-hot)
         my_mh_assigned = trans_desc.get_int_param(pk_mh_assigned, 0);
         my_mh_accepted = trans_desc.get_int_param(pk_mh_accepted, 0);

         //if(rtl_info.mh_req_valid == 1'b1) begin
            if(my_mh_assigned != my_mh_accepted) begin
               sb_error($sformatf("(S2) MissHandler RTL signals mismatch: s2_mh_req_assigned[1:0]=%2b s2_mh_req_accepted[1:0]=%2b", my_mh_assigned, my_mh_accepted), trans_desc, {}, "", `__FILE__, `__LINE__);
            end
         //end

         // note: pk_mh_assigned: 1=MH0 2=MH1 (bcoz original RTL signal is [1:0] one-hot)
         miss_handler_id = (trans_desc.get_int_param(pk_mh_assigned, 0) == 1) ? 0 : 1;    // convert to: 0=MH0 1=MH1

         // req_id = gen_reqq_index(bank_id, reqq_id);
         $cast(l2_req_desc, m_template_req_desc_neigh.clone());
         trans_desc.copy_to_desc(l2_req_desc);
         void'(trans_desc.add_child_desc(l2_req_desc));

         l2_req_desc.set_parent_desc(trans_desc);    // CYE (for ucache_control: RepRate & Max)

         l2_req_desc.set_trans_id(trans_desc.get_trans_id());
         l2_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));

         // (CYE) Should we set the source field based on the core agent id ???  No, we cannot. 
         // If we set it, we will get mismatch on ETLINK checker, because ELINK IFC/MON does not extract/assign correct minion ID, it always assign "minion0"!  
         //l2_req_desc.set_bus_req_source(get_core_id());   // or: better to use get_minion_id()? 

         if ((evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) == BusWriteAtomic) || (trans_desc.is_uncached() == 1)) begin
            l2_req_desc.set_paddr(trans_desc.get_paddr());
            if (trans_desc.have_int_param(pk_vector_ps)) begin
               l2_req_desc.set_bus_req_size(5);
            end
            else begin
               l2_req_desc.set_bus_req_size($clog2(trans_desc.get_cmd_length()));
            end
         end
         else begin
            l2_req_desc.set_paddr(trans_desc.get_paddr() & const_line_paddr_mask);
            if (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) == BusPrefetch) begin
               l2_req_desc.set_bus_req_size(0);
            end
            else if (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) == BusSysPrefetch) begin
               l2_req_desc.set_bus_req_size(3);
            end
            else begin
               l2_req_desc.set_bus_req_size(6);
            end
         end

         // If its not a cache op then the dest level should be 0
         // This is to address an issue where an evict from L1->L2 issue an
         // ET-LINK_Req_Write with the wrong dest field.
         // Note that L1->L3 Evicts generate 2 transactions hence (Write followed by Evict Cache op),
         // and as such the dest_level cannot be changed earlier in the scoreboard.
         if (!l2_req_desc.bus_cmd_is_cache_op()) begin
            l2_req_desc.set_bus_cop_dest_level(0);
         end

         if (table_desc.get_bus_cmd() inside {BusPrefetch, BusSysPrefetch}) begin
            bit tid = trans_desc.get_thread_id();
            bit wid = trans_desc.get_int_param(pk_tensorid);
            case ({tid, wid})
               2'b00:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_0);
               2'b01:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_1);
               2'b10:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_2);
               default: l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_L2_Prefetch_3);
            endcase
         end
         else if (table_desc.get_bus_cmd() inside {BusWriteMsg, BusWrite, BusWriteAtomic, BusRead}) begin
            if (table_desc.get_data_action() == Data_ReqWrBack) begin
               l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_WriteBack);
            end
            else if (trans_desc.is_uncached()) begin
               bit tid   = trans_desc.get_thread_id();
               bit store = (table_desc.get_bus_cmd() inside {BusWriteMsg, BusWrite, BusWriteAtomic}) ? 1'b1 : 1'b0;
               case ({store, tid})
                  2'b00:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0);
                  2'b01:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1);
                  2'b10:   l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0);
                  default: l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1);
               endcase
            end
            else begin
               // MissHandler 0 or 1: 
               l2_req_desc.set_bus_req_id((miss_handler_id == 0) ? ET_LINK_Core_Miss_Id_Miss_Handler_0 : ET_LINK_Core_Miss_Id_Miss_Handler_1);
            end
         end

         setup_bus_req_dependencies(l2_req_desc);
         if (table_desc.get_bus_cmd() inside {BusWriteMsg, BusWriteAtomic, BusWrite, BusPrefetch, BusSysPrefetch }) begin
            if (table_desc.get_data_action() == Data_ReqWrBack) begin
               l2_req_desc.set_req_data(line_desc.get_line_data_copy());
            end
            else if (table_desc.get_data_action() == Data_FwdWr) begin
               l2_req_desc.set_req_data(trans_desc.get_write_data());
            end

            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("(S2) Predicting this request to EVICT interface: %0s (MH=%0d)", l2_req_desc.sprint_obj(), miss_handler_id))
            m_pending_evict_reqs.push_back(l2_req_desc);

            // (Charles 1/27/2020) 
            // for checking CacheOp_RepRate: first of multi-cline stream:
            // only for CacheOp_L2: PrefVA, TLOAD_L2SCP (excluding EvictVA and FlushVA as RTL does);
            // note: to match RTL behavior: start counting "first_cyc" at this time;
            if ((((trans_desc.get_cmd() == CACHE_PREFETCH) &&
                  (trans_desc.get_bus_cop_dest_level() > 0)) ||   // CacheOp_Pref_L2L3
                 ( trans_desc.get_cmd() == CACHE_SCP_FILL ) )     // Tensor_Load_L2SCP (CacheOp_Pref_L2SCP)
                &&
                ~((trans_desc.get_int_param(pk_cop_first_line, 0) == 1) &&
                  (trans_desc.get_int_param(pk_cop_end_line, 0) == 1))      // exclude single-cline case 
            )
            begin 
               // if it's the first_line of the above CacheOp (multiple_lines): set the "first_cyc":
               if (trans_desc.get_int_param(pk_cop_first_line, 0) == 1) begin
                  m_cacheop_bw_ctrl_first_cyc_q.push_back(m_minion_rtl_port.get_cycle_count());
                  `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("(CYE DEBUG) for above CacheOp_L2 (Pref_L2L3 or TLoad_L2SCP): predicting: it is first_line: starting: set m_cacheop_bw_ctrl_first_cyc_q[]=%0d", m_cacheop_bw_ctrl_first_cyc_q[$]))
               end
            end
         end
         else begin
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("(S2) Predicting this request to MISS interface: %0s (MH=%0d)", l2_req_desc.sprint_obj(), miss_handler_id))
            m_pending_miss_reqs.push_back(l2_req_desc);
         end
      end   // if (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) != BusIdle) 

      //
      // Process the data action as necessary.
      //
      if (table_desc.get_data_action() inside { Data_MemWr, Data_MrgWrBack }) begin  // for Store-type
         if (trans_desc.get_write_data() == null) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) process_request: for data_action=Data_MemWr: for above trans_desc: get_write_data returned null" ))
         end

         if (trans_desc.get_write_data() != null) begin
            // write/update/populate new (store) data into line_desc (into Dcache model):
            line_desc.merge_data_from_line(trans_desc.get_write_data());
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) process_request: for above Store-type (new store data): after merge_data: line_desc: %s (m_line_data is as below:)", line_desc.sprint_obj()))

            //if (line_desc.m_line_data == null) begin   // ??? should it be an error ???
            //   `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) for above line_desc: but m_line_data is null"))
            //end else begin
            //   `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) for above line_desc: m_line_data is following:"))
               if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
                  line_desc.m_line_data.print_obj(get_abstract_name(), trans_desc.get_paddr());
                  //line_desc.print_obj(get_abstract_name());
               end
            //end
         end
      end

      //else if (table_desc.get_data_action() == Data_MemRd) begin   // for Load-type
      else if ((trans_desc.get_cmd() == LD) && (table_desc.get_data_action() == Data_MemRd))   // for Load-type (cached)
      begin
         trans_desc.set_read_data(line_desc.get_line_data_copy());  // expected read return data (m_read_data)

         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) process_request: for above Load-type (cached): assign line_data to m_read_data: %s:", line_desc.sprint_obj()))
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            trans_desc.get_read_data().print_obj(get_abstract_name(), trans_desc.get_paddr());
         end

         // (CYE) enqueue the trans_desc into m_s3_core_resp_queue[]: for S3 self-checking:... 
         // ... Do this in function process_s2_req. 
      end

      // for Load-type (uncached), and for Atomic (return original memory value):
      else if (   ((trans_desc.get_cmd() inside {LD, LD_RA}) && (trans_desc.is_uncached()) && (table_desc.get_bus_rsp() == RspWData)) 
               || ((trans_desc.get_cmd() == ATOMIC) && (table_desc.get_bus_rsp() == RspWData)) )   
      begin
         bit thread_id = trans_desc.get_thread_id();
         evl_line_data  uc_retn_line_data;

        /***  // remove: change about using m_uc_l2_retn_line_data[] (see comment at other places)
         //uc_retn_line_data = m_uc_l2_retn_line_data[thread_id].pop_front();
         //trans_desc.set_read_data(uc_retn_line_data);  // expected read return data (m_read_data)
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("(CYE DEBUG)(S2) process_request: for above Load-type uncached or Atomic (thread=%0b): pop (front) m_uc_l2_retn_line_data[%0b] and assign to m_read_data: as below:", thread_id, thread_id ))
         //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         //   trans_desc.get_read_data().print_obj(get_abstract_name(), trans_desc.get_paddr());
         //end
        ***/

         // (CYE) enqueue the trans_desc into m_s3_core_resp_queue[]: for S3 self-checking:... 
         // ... Do this in function process_s2_req. 
      end

      else if (table_desc.get_data_action() == Data_ZeroFill) begin
         line_desc.zero_all_line_data();
      end

      return table_desc;

   endfunction: process_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capacity_evict
   //
   function void capacity_evict(input evl_trans_desc trans_desc, input evl_line_desc prev_line_desc, input evl_way_t way);
      evl_minion_dcache#(CACHE_TYPE) cache;

      cache = get_cache(trans_desc);

      if ((prev_line_desc != null) && (trans_desc != null)) begin
         if (!(prev_line_desc.get_cache_st() inside { E, M, C, T })) begin
            sb_error("L1D replaced line with outstanding operation", null, {}, "", `__FILE__, `__LINE__);
         end
         if (prev_line_desc.get_cache_st() inside {E, C}) begin
            `evl_log_addr(UVM_HIGH, m_vmod, get_abstract_name(), prev_line_desc.get_paddr(), $sformatf("(S2) Silent eviction: invalidate prev_cline_paddr=0x%h",  prev_line_desc.get_paddr()))
         end
         if (prev_line_desc.get_cache_st() inside {M, T}) begin  // need dirty writeback
            evl_trans_desc evict_desc;
            evl_req_desc   l2_req_desc;

            $cast(evict_desc, m_template_trans_desc.clone());
            evict_desc.set_trans_id();
            evict_desc.set_paddr(evl_paddr_t'(prev_line_desc.get_paddr()));
            evict_desc.set_cmd(EVICT);
            evict_desc.set_thread_id(trans_desc.get_thread_id());
            // (CYE) added: for Evict/WB data (prev_line_desc.m_line_data; now evict_desc.m_line_desc.m_line_data)
            evict_desc.set_line_desc(prev_line_desc);    
            evict_desc.set_write_data(prev_line_desc.get_line_data());   // assign WB data to m_write_data
            // (CYE) end 
            evict_desc.set_cached();

            $cast(l2_req_desc, m_template_req_desc_neigh.clone());
            evict_desc.copy_to_desc(l2_req_desc);
            l2_req_desc.set_trans_id(evict_desc.get_trans_id());
            l2_req_desc.set_abstract_bus_cmd(BusWrite);
            l2_req_desc.set_paddr(prev_line_desc.get_paddr() & const_line_paddr_mask);
            l2_req_desc.set_bus_req_size(6);
            l2_req_desc.set_bus_req_id(ET_LINK_Core_Miss_Id_WriteBack);
            l2_req_desc.set_req_data(prev_line_desc.get_line_data_copy());  // Evict/WB data: will be self-checked in evl_etlink_verif_comp.sv 

            // set up relation: parent (evict_desc) ~ child (l2_req_desc):
            l2_req_desc.set_parent_desc(evict_desc);         // (CYE) added: set l2_req_desc parent -> evict_desc
            void'(evict_desc.add_child_desc(l2_req_desc));

            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("(S2) Dirty writeback: Predicting this EVICT request to L2: %0s", l2_req_desc.sprint_obj()))
            //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            //   l2_req_desc.get_req_data().print_obj(get_abstract_name(), l2_req_desc.get_paddr());
            //end
            
            //// (CYE) to test self-checking WB data: change the WB data to some wrong value: --> self-check WB data is working!
            //l2_req_desc.get_req_data().zero_all_data();
            //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), l2_req_desc, $sformatf("for above Evict/WB Req to L2: zero out its data  (for testing)" ))

            l2_req_desc.set_int_param(pk_capacity_evict, 1);
            setup_bus_req_dependencies(l2_req_desc);
            add_active_mem_desc(evict_desc);
            m_pending_evict_reqs.push_back(l2_req_desc);
         end

         //
         // If evicted line is soft locked, and the eviction is not because of a hard lock,
         // check that all other ways are either soft or hard locked
         //
         if (prev_line_desc.get_cache_st() inside {C, T}) begin
            if (trans_desc.get_cmd() != CACHE_LOCK_SW) begin
               for (int w = 0; w < cache.get_num_ways(); w++) begin
                  evl_line_desc set_way_desc;
                  if (way == w) continue;
                  set_way_desc = cache.get_line_desc_by_set_way(trans_desc.get_paddr(), w, 1'b0, 1'b0);
                  if (!(set_way_desc.get_cache_st() inside {L, D}) && !(set_way_desc.get_cache_st() inside {C, T, CT_Bsy}) && (set_way_desc.get_int_param(pk_soft_locked, 0) == 0)) begin
                     sb_error($sformatf("RTL chose soft locked way=%0d for capacity eviction but way=%0d isn't locked so could have been chosen", way, w), null, {}, "", `__FILE__, `__LINE__);
                     break;
                  end
               end
            end
         end

         void'(cache.deallocate_way(prev_line_desc));
         prev_line_desc.set_cache_st(I);
      end
      else begin
         if (prev_line_desc == null) begin
            sb_error("L1SB requested capacity eviction of a null line", null, {}, "", `__FILE__, `__LINE__);
         end
         if (trans_desc == null) begin
            sb_error("L1SB requested capacity eviction without an evicter", null, {}, "", `__FILE__, `__LINE__);
         end
      end
   endfunction: capacity_evict


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_replay_item
   //
   function void delete_replay_item(input evl_trans_desc trans_desc, input int kill = 0);
      int ii;
      bit found = 1'b0;

      // If misaligned access crossing lines: delete the 1st part, and point m_replay_queue[] entry to 2nd part and mark 2nd part ready:
      if (   (trans_desc.have_object_param(pk_misaligned_2nd) == 1)
          && (kill == 0)
          && (trans_desc.get_int_param(pk_bus_err, 0) == 0))
      begin
         evl_trans_desc misaligned_desc;
         $cast(misaligned_desc, trans_desc.get_object_param(pk_misaligned_2nd));
         // Charles (CYE) added:
         misaligned_desc.set_int_param(pk_this_is_misaligned_2nd_part, 1);
         foreach (m_replay_queue[ii]) begin
            if (m_replay_queue[ii] == trans_desc) begin
               m_replay_queue[ii] = misaligned_desc;
               found = 1'b1;
               break;
            end
         end
         if (found == 1'b0) begin
            sb_error($sformatf("Request is misaligned but was not found in the replay queue"), null, {}, "", `__FILE__, `__LINE__);
         end
         else begin
            misaligned_desc.set_int_param(pk_replay_ready, 1);
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), trans_desc, $sformatf("misaligned access: 2nd part ready to replay %0s", misaligned_desc.sprint_obj()))
            add_active_mem_desc(misaligned_desc);
         end
      end
      else begin
         foreach (m_replay_queue[ii]) begin
            if (m_replay_queue[ii] == trans_desc) begin
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Deleting from ReplayQueue: %0s", trans_desc.sprint_obj()))
               m_replay_queue.delete(ii);
               break;
            end
         end
      end

   endfunction: delete_replay_item


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bus_resp_stalled
   //
   task process_bus_resp_stalled(input evl_req_desc req_desc);
      //fork
      if (req_desc.get_bus_rsp_id() == 5'h02) begin         // MissHandler_0
         wait (event_mh_l2_resp_stall_clear[0].triggered);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("received RTL event_mh_l2_resp_stall_clear[0]: send bus_resp %s to function process_bus_response()", req_desc.sprint_obj() ))
         process_bus_response(req_desc);
      end
      if (req_desc.get_bus_rsp_id() == 5'h03) begin         // MissHandler_1
         wait (event_mh_l2_resp_stall_clear[1].triggered);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("received RTL event_mh_l2_resp_stall_clear[1]: send bus_resp %s to function process_bus_response()", req_desc.sprint_obj() ))
         process_bus_response(req_desc);
      end
      //join_none
   endtask: process_bus_resp_stalled



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bus_response
   //
   function void process_bus_response(input evl_req_desc req_desc);
      evl_table_desc    table_desc;
      evl_line_desc     line_desc;
      evl_req_desc      src_req_desc;
      evl_trans_desc    trans_desc;
      evl_mem_desc      parent_mem_desc;
      evl_trans_desc    parent_trans_desc;
      evl_dcache_qual_t qualifier;
      string            rsp_data_info;
      //evl_abstract_rsp_type_t  table_bus_rsp;

      if (req_desc.get_rsp_data() == null) begin
          rsp_data_info = "rsp_data is null";
      end else begin
          rsp_data_info = "rsp_data is as below";
      end
      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), req_desc, $sformatf("Received bus response for req_desc: %0s (%0s)",  req_desc.sprint_obj(), rsp_data_info))
      if (req_desc.get_rsp_data() != null) begin
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            req_desc.get_rsp_data().print_obj(get_abstract_name(), req_desc.get_paddr());
         end
      end

      if (req_desc.get_bus_rsp_type() == ET_LINK_RSP_Err) begin
         if (m_allow_bus_err == 1'b0) begin
            sb_error($sformatf("Received a Bus_Err response but this scoreboard does not have full support yet!"), null, {}, "", `__FILE__, `__LINE__);
         end else begin
            `evl_log_mdesc(UVM_WARNING, m_vmod, get_abstract_name(), req_desc, $sformatf("Received a Bus_Err response for %0s", req_desc.sprint_obj()))
         end
      end

      trans_desc   = req_desc.get_root_trans_desc();   // root_trans
      src_req_desc = req_desc.get_parent_req_desc();   // parent_req --> null
      parent_mem_desc = req_desc.get_parent_desc();
      $cast(parent_trans_desc, parent_mem_desc); 

      if (trans_desc != null) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("for above req_desc: its root_trans_desc = %s", trans_desc.sprint_obj() ))
      end
      if (src_req_desc != null) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("for above req_desc: its src_req_desc = %s", src_req_desc.sprint_obj() ))
      end
      if (parent_trans_desc != null) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("for above req_desc: its parent_trans_desc = %s", parent_trans_desc.sprint_obj() ))
      end

      qualifier = DCQ_None;
      if (trans_desc.is_uncached() == 1) begin
         trans_desc.set_int_param(pk_uc_resp, 1);
         qualifier = DCQ_Resp;
      end

      // now call function bus_rsp_lookup() in "evl_minion_dcache":  return table_desc, line_desc ...
      get_cache(req_desc).bus_rsp_lookup(table_desc, line_desc, req_desc, qualifier, req_desc.get_bus_rsp_type());
         // note: returned "table_desc" get_err_code() could carry info EC_ErrRsp (Bus_Err) for some Req Cmd/Type ... 

      `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), req_desc, $sformatf("processing bus response for req_desc: %0s --> table_desc: %0s --> trans_desc: %s --> line_desc: %s", req_desc.sprint_obj(), table_desc.ssprint_obj(), trans_desc.sprint_obj(), line_desc.sprint_obj() ))

      //table_bus_rsp = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
      //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) for above bus response for req_desc: table_desc.get_bus_rsp()=%s", table_bus_rsp.name() ))

      if (line_desc.m_line_data == null) begin
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) for above line_desc: but m_line_data is null"))
      end else begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) for above line_desc: m_line_data is following:"))
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            line_desc.m_line_data.print_obj(get_abstract_name(), req_desc.get_paddr());
         end
      end

  
      // for TensorLoad_TenB: assign return data to trans_desc.m_read_data; 
      // and then expect it be sent to VPU as "s3_vpu_tenb_data" (in next couple of cycles);
      if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 1)) begin
         trans_desc.set_read_data(req_desc.get_rsp_data()); 
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for TensorLoad_TenB corresponding to above bus_resp: assign cline_data from req_desc to the corresponding trans_desc m_read_data: as below:")) 
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            trans_desc.get_read_data().print_obj(get_abstract_name(), trans_desc.get_paddr());
         end

         m_s3_vpu_tenb_data_queue.push_back(trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("push above TensorLoad_TenB (with its m_read_data populated) into m_s3_vpu_tenb_data_queue[]"))
      end  // TLoad_TenB

      // for TensorLoad L1SCP (not TenB): fill into L1SCP (based on various transform types);
      if ((trans_desc.get_cmd() == TENSOR_LOAD) && (trans_desc.get_int_param(pk_tload_sel) == 0)) begin
         tensor_load_fill_l1scp(req_desc, trans_desc);
      end  // TLoad_L1Scp

      if (table_desc.get_tag_action() != St_Ignore) begin
         if (line_desc.get_cache_st() != table_desc.get_tag_action()) begin
            `evl_log_addr_trid_id(UVM_DEBUG, m_vmod, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), req_desc.get_bus_req_id(), $sformatf("modifying cache state (for PA=0x%x) from %p to %p", line_desc.get_paddr(), line_desc.get_cache_st(), table_desc.get_tag_action()))
         end

         //
         // If line must be invalidated (error response), queue a miss handler tag update
         //
         if (table_desc.get_tag_action() inside {MI_Bsy, EI_Bsy}) begin
            `evl_log_addr_trid_id(UVM_DEBUG, m_vmod, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), req_desc.get_bus_req_id(), $sformatf("(due to BusRspErr) above table_desc tag_action is MI_Bsy or EI_Bsy: cline will be invalidated: push trans_desc into m_mh_tag_update_q[]" ))
            m_mh_tag_update_q.push_back(trans_desc);
         end

         line_desc.set_cache_st(table_desc.get_tag_action());
      end
      if (table_desc.get_tag_action() == I) begin
         line_desc.invalidate_line_data();
      end

      // for cached: L2-return Fill data needs to fill into Dcache model:
      if (table_desc.get_data_action() == Data_MemWrInst) begin
         if (req_desc.get_rsp_data() == null) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) process_bus_response: for data_action=Data_MemWrInst: for above req_desc: get_rsp_data returned null" ))
         end

         if (req_desc.get_rsp_data() != null) begin
            // write/update/populate L2-return Fill data into line_desc (into Dcache model):
            line_desc.merge_data_from_line(req_desc.get_rsp_data());
            `evl_log_mdesc(UVM_HIGH, m_vmod, get_abstract_name(), req_desc, $sformatf("process_bus_response: for above Cache-ReFill (fill data): after merge_data: line_desc: %s (m_line_data is as below:)", line_desc.sprint_obj()))

            //if (line_desc.m_line_data == null) begin   // ??? should it be an error ???
            //   `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) for above line_desc: but m_line_data is null"))
            //end else begin
            //   `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) for above line_desc: m_line_data is following:"))
               if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
                  line_desc.m_line_data.print_obj(get_abstract_name(), req_desc.get_paddr());
                  //line_desc.print_obj(get_abstract_name());
               end
            //end
         end
      end

      // for Uncached Load-type or Atomic: save L2-return data in m_uc_l2_retn_line_data[n] (could be multiple outstanding per Thread):
      if (((trans_desc.get_cmd() inside {LD, LD_RA}) && (trans_desc.is_uncached()) /*&& (table_desc.get_bus_rsp() == RspWData)*/) ||
          ((trans_desc.get_cmd() == ATOMIC) /*&& (table_desc.get_bus_rsp() == RspWData)*/) ) 
      begin
         bit  thread_id = trans_desc.get_thread_id();
         evl_line_data  uc_retn_line_data;

         uc_retn_line_data = evl_line_data::type_id::create();
         uc_retn_line_data.copy_data_from_line(req_desc.get_rsp_data());

        /*** 
         //m_uc_l2_retn_line_data[thread_id].push_back(uc_retn_line_data);
         //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) process_bus_response: for above req_desc and trans_desc (Uncached or Atomic)(thread=%0b): push bus_rsp_data into m_uc_l2_retn_line_data[%0b]: as below:", thread_id, thread_id))
         //if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
         //   m_uc_l2_retn_line_data[thread_id][$].print_obj(get_abstract_name(), trans_desc.get_paddr());
         //end
        ***/
         // Important note: Replay of Uncached Reqs could be out of order, hence using m_uc_l2_retn_line_data[] queue (in-order) 
         // will not work, unless we attach a TransID to each retn_line_data ...
         // So, let's change to: assign the retn_line_data now directly to the original trans_desc:
         trans_desc.set_read_data(uc_retn_line_data);  // expected read return data (m_read_data)
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("(CYE DEBUG) process_bus_response: for above req_desc and trans_desc (Uncached or Atomic)(thread=%0b): assign bus_rsp_data to original trans_desc: as below:", thread_id))
         if (`evl_log_check_verbosity(UVM_DEBUG, m_vmod) != 0) begin
            uc_retn_line_data.print_obj(get_abstract_name(), trans_desc.get_paddr());
         end
      end  // Uncached or Atomic


      //
      // Mark the request descriptor as done, to unblock dependencies
      //
      req_desc.set_done();
      if (src_req_desc != null) begin
         src_req_desc.set_done();
      end

      //if (table_desc.get_err_code() == EC_ErrRsp) begin         // "table_desc" carry EC_ErrRsp only for some Req Cmd/Type:
      if (req_desc.get_bus_rsp_type() == ET_LINK_RSP_Err) begin   // so change to all "ET_LINK_RSP_Err" cases;
         trans_desc.set_int_param(pk_bus_err, 1);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("due to above Bus Err response: set pk_bus_err=1"))

         // put this trans_desc in m_core_bus_err_queue[]: to self-check against RTL:
         m_core_bus_err_queue.push_back(trans_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("add this trans_desc (ET_LINK_RSP_Err) to m_core_bus_err_queue: %s",  trans_desc.sprint_obj() ))
      end

      //
      // If this is a tensor store response, retire the transaction when all the requests
      // spawned from it have come back and are done
      //
      if (trans_desc.get_cmd() inside {TENSOR_STORE_SCP, TENSOR_STORE}) begin
         if (trans_desc.all_children_are_done()) begin
            delete_active_desc(trans_desc);
         end
      end
      //
      // If this doesn't need to be replayed, delete from replay queue and active descs
      //
      else if (!(table_desc.get_err_code() inside {EC_Replay, EC_ErrRsp})) begin
         delete_replay_item(trans_desc);
         delete_active_desc(trans_desc);
      end
      //
      // Mark transaction as done for bus errors even though they will be replayed
      // but the replay is only to clean up. Dependencies are free to go.
      //
      else if (table_desc.get_err_code() == EC_ErrRsp) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("due to above Bus Err response: mark trans as Done"))
         trans_desc.set_done();
      end

   endfunction: process_bus_response



   //----------------------------------------------------
   //
   // Function: tensor_load_fill_l1scp
   //
   function void tensor_load_fill_l1scp(evl_req_desc req_desc, evl_trans_desc trans_desc);
         evl_trans_desc parent_tload_trans;     // original parent TensorLoad_L1SCP trans_desc
         int            parent_tload_resp_line_count;
         int            parent_tload_scp_start_line; 
         int            parent_tload_num_scp_lines; 
         bit            parent_tload_use_tmask; 
         bit [15:0]     parent_tload_tensor_mask; 

         evl_line_data  resp_line_data;
         bit [511:0]    resp_data_64byte;
         int            transform;
         string         extra_info; 
         int            tload_spawn_req_idx;
         int            scp_line_id;  // max (48-1)
         int            scp_way;        // max (4-1)
         int            scp_set;        // max (12-1)
         bit [1:0]      qword_offset;   // for each quad-word (16-byte)
         bit            oword_offset;   // for each octal-word (32-byte) 

         bit [511:0]    resp_data_clines [64];  // each is a cline 64-byte; Interleave needs max [4], Transpose needs max [64]
         bit [127:0]    resp_data_qwords [64];  // each is quad-word 16-byte; Interleave-8 needs [4], Transpose-8 needs [64]
         bit [255:0]    resp_data_owords [32];  // each is octal-word 32-byte; Interleave-16 needs [2], Transpose-16 needs [32]
         bit  [31:0]    intlv_data_word [16];   // after interleave, each is a word (4-byte) 
         bit  [15:0]    intlv_data_hword [32];  // after interleave, each is half-word (2-byte)
         bit [511:0]    trnps_data_clines [16];       // after transpose

         resp_line_data = req_desc.get_rsp_data();
         resp_data_64byte = resp_line_data.get_xword(0);
         transform = trans_desc.get_int_param(pk_tload_transform);
         tload_spawn_req_idx = trans_desc.get_int_param(pk_tload_spawn_req_idx);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("TensorLoad_L1SCP corresponding to above bus_resp: %s: transform=%0d", trans_desc.sprint_obj(), transform ))

         $cast(parent_tload_trans, trans_desc.get_parent_desc());
         parent_tload_resp_line_count = parent_tload_trans.get_int_param(pk_tload_resp_line_count);
         parent_tload_scp_start_line  = parent_tload_trans.get_int_param(pk_tload_scp_dest); 
         parent_tload_num_scp_lines   = parent_tload_trans.get_int_param(pk_tload_num_lines);
         parent_tload_use_tmask       = parent_tload_trans.get_int_param(pk_tload_use_tmask);
         parent_tload_tensor_mask     = parent_tload_trans.get_int_param(pk_tload_tensor_mask);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP (spawned): its original parent_tload_trans is %s: pk_tload_resp_line_count=%0d", parent_tload_trans.sprint_obj(), parent_tload_resp_line_count ))
         
         if (trans_desc.get_int_param(pk_tload_transform) == 0) begin   // No transform
            // no_transform: fill into L1SCP whenever each cline returns;
            // note: have to take into account of tensor_mask when decide which scp_line_id (might need to skip) ...
            scp_line_id = trans_desc.get_int_param(pk_tload_scp_line_id);
            scp_line_id = scp_line_id % 48;     // max 48 clines
            scp_way = scp_line_id / 12;
            scp_set = scp_line_id % 12;  

            m_l1_scp[scp_way][scp_set] = resp_data_64byte;
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: No_transform: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%h", resp_data_64byte))
         end
         else if (trans_desc.get_int_param(pk_tload_transform) == 1) begin   // Interleave8
            // Note: PRM-10 Tensor Extension, $1.2.3: the figures might be incorrect; the equations look correct.

            // Interleave8: process every 4 clines (L2 req & data) as a group;
            // collect 4 L2 resp_line_data:
            //problem: L2 Resp (data) could come back out-of-order: change to code below:
            m_tload_resp_line_data[(tload_spawn_req_idx % 4)] = req_desc.get_rsp_data();

            // every time when reach 4 lines: interleave and fill into L1SCP:
            if (((parent_tload_resp_line_count+1) % 4) == 0) begin
               // extract the designated Quad-Word (16-byte) from each of the 4 clines: the rest data are thrown away (?)
               qword_offset = trans_desc.get_int_param(pk_tload_byte_offset) / 16;
               for (int ii=0; ii<4; ii++) begin
                  resp_data_qwords[ii] = m_tload_resp_line_data[ii].get_qword(qword_offset);
               end
               // interleave each byte from the above 4 QWords and form a word (4-byte), so generate total 16 words: 
               for (int jj=0; jj<16; jj++) begin
                  intlv_data_word[jj] = {resp_data_qwords[3][8*jj +: 8], resp_data_qwords[2][8*jj +: 8], 
                                         resp_data_qwords[1][8*jj +: 8], resp_data_qwords[0][8*jj +: 8]};
               end

               // note: need to take into account of tensor_mask when decide which scp_line_id (might need to skip) ...
               scp_line_id = trans_desc.get_int_param(pk_tload_scp_line_id);
               scp_line_id = scp_line_id % 48;     // max 48 clines
               scp_way = scp_line_id / 12;
               scp_set = scp_line_id % 12; 
 
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Interleave 8-bit: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h", intlv_data_word[15], intlv_data_word[14], intlv_data_word[13], intlv_data_word[12], intlv_data_word[11], intlv_data_word[10], intlv_data_word[9],  intlv_data_word[8], intlv_data_word[7],  intlv_data_word[6],  intlv_data_word[5],  intlv_data_word[4], intlv_data_word[3],  intlv_data_word[2],  intlv_data_word[1],  intlv_data_word[0]))

               // fill the above 16 words (after interleave) into the designated L1SCP line:
               for (int jj=0; jj<16; jj++) begin
                  m_l1_scp[scp_way][scp_set][32*jj +: 32] = intlv_data_word[jj];
                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("word[%2d]: 0x%h", jj, intlv_data_word[jj]))
               end
            end  // if (((parent_tload_resp_line_count+1) % 4) == 0)

         end
         else if (trans_desc.get_int_param(pk_tload_transform) == 2) begin   // Interleave16
            // Note: PRM-10 Tensor Extension, $1.2.4: the figures might be incorrect; the equations look correct.

            // Interleave16: process every 2 clines (L2 req & data) as a group;
            // collect 2 L2 resp_line_data:
            //problem: L2 Resp (data) could come back out-of-order: change to code below:
            m_tload_resp_line_data[(tload_spawn_req_idx % 2)] = req_desc.get_rsp_data();

            // every time when reach 2 lines: interleave and fill into L1SCP:
            if (((parent_tload_resp_line_count+1) % 2) == 0) begin
               // extract the designated Octal-Word (32-byte) from each of the 2 clines: the rest data are thrown away (?)
               oword_offset = trans_desc.get_int_param(pk_tload_byte_offset) / 32;
               for (int ii=0; ii<2; ii++) begin
                  resp_data_owords[ii] = m_tload_resp_line_data[ii].get_oword(oword_offset);
               end
               // interleave each 2-byte from the above 2 OWords and form a word (4-byte), so generate total 16 words: 
               for (int jj=0; jj<16; jj++) begin
                  intlv_data_word[jj] = {resp_data_owords[1][16*jj +: 16], resp_data_owords[0][16*jj +: 16]};
               end

               // note: need to take into account of tensor_mask when decide which scp_line_id (might need to skip) ...
               scp_line_id = trans_desc.get_int_param(pk_tload_scp_line_id);
               scp_line_id = scp_line_id % 48;     // max 48 clines
               scp_way = scp_line_id / 12;
               scp_set = scp_line_id % 12; 

               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Interleave 16-bit: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h_%8h", intlv_data_word[15], intlv_data_word[14], intlv_data_word[13], intlv_data_word[12], intlv_data_word[11], intlv_data_word[10], intlv_data_word[9],  intlv_data_word[8], intlv_data_word[7],  intlv_data_word[6],  intlv_data_word[5],  intlv_data_word[4], intlv_data_word[3],  intlv_data_word[2],  intlv_data_word[1],  intlv_data_word[0]))

               // fill the above 16 words (after interleave) into the designated L1SCP line:
               for (int jj=0; jj<16; jj++) begin
                  m_l1_scp[scp_way][scp_set][32*jj +: 32] = intlv_data_word[jj];
                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("word[%2d]: 0x%h", jj, intlv_data_word[jj]))
               end
            end  // if (((parent_tload_resp_line_count+1) % 2) == 0)

         end
         else if (trans_desc.get_int_param(pk_tload_transform) == 5) begin   // Transpose8
            // always need total 64 L2 resp_line_data;
            // RTL collects every 8 L2 resp_line_data and processes them;
            // in this L1D_SB model: to simplify, we collect all 64 L2 resp_line_data and then process them all together;
             
            //problem: L2 Resp (data) could come back out-of-order: change to code below:
            m_tload_resp_line_data[tload_spawn_req_idx] = req_desc.get_rsp_data();
             
            // after collect all 64 clines: transpose and fill into L1SCP:
            if ((parent_tload_resp_line_count+1) == 64) begin
               // extract 16-bytes (quad-word) from each cline:
               qword_offset = trans_desc.get_int_param(pk_tload_byte_offset) / 16;
               for (int ii=0; ii<64; ii++) begin
                  resp_data_qwords[ii] = m_tload_resp_line_data[ii].get_qword(qword_offset);
               end

               // process for each scp_line (considering tensor_mask):
               scp_line_id = parent_tload_scp_start_line;    // starting line 
               scp_line_id = scp_line_id % 48;     // max 48 clines

               // for multiple scp lines:
               for (int ii=0; ii<parent_tload_num_scp_lines; ii++) begin      // num_scp_lines: max=16 
                  if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1)) begin
                     // transpose: each byte (8-bit): to construct a cline:
                     //trnps_data_clines[ii] = {
                     //                        resp_data_qwords[60+3][8*ii +: 8], resp_data_qwords[60+2][8*ii +: 8], resp_data_qwords[60+1][8*ii +: 8], resp_data_qwords[60][8*ii +: 8],
                     //                        resp_data_qwords[56+3][8*ii +: 8], resp_data_qwords[56+2][8*ii +: 8], resp_data_qwords[56+1][8*ii +: 8], resp_data_qwords[56][8*ii +: 8],
                     //                        resp_data_qwords[52+3][8*ii +: 8], resp_data_qwords[52+2][8*ii +: 8], resp_data_qwords[52+1][8*ii +: 8], resp_data_qwords[52][8*ii +: 8],
                     //                        resp_data_qwords[48+3][8*ii +: 8], resp_data_qwords[48+2][8*ii +: 8], resp_data_qwords[48+1][8*ii +: 8], resp_data_qwords[48][8*ii +: 8],
                     //                        resp_data_qwords[44+3][8*ii +: 8], resp_data_qwords[44+2][8*ii +: 8], resp_data_qwords[44+1][8*ii +: 8], resp_data_qwords[44][8*ii +: 8],
                     //                        resp_data_qwords[40+3][8*ii +: 8], resp_data_qwords[40+2][8*ii +: 8], resp_data_qwords[40+1][8*ii +: 8], resp_data_qwords[40][8*ii +: 8],
                     //                        resp_data_qwords[36+3][8*ii +: 8], resp_data_qwords[36+2][8*ii +: 8], resp_data_qwords[36+1][8*ii +: 8], resp_data_qwords[36][8*ii +: 8],
                     //                        resp_data_qwords[32+3][8*ii +: 8], resp_data_qwords[32+2][8*ii +: 8], resp_data_qwords[32+1][8*ii +: 8], resp_data_qwords[32][8*ii +: 8],
                     //                        resp_data_qwords[28+3][8*ii +: 8], resp_data_qwords[28+2][8*ii +: 8], resp_data_qwords[28+1][8*ii +: 8], resp_data_qwords[28][8*ii +: 8],
                     //                        resp_data_qwords[24+3][8*ii +: 8], resp_data_qwords[24+2][8*ii +: 8], resp_data_qwords[24+1][8*ii +: 8], resp_data_qwords[24][8*ii +: 8],
                     //                        resp_data_qwords[20+3][8*ii +: 8], resp_data_qwords[20+2][8*ii +: 8], resp_data_qwords[20+1][8*ii +: 8], resp_data_qwords[20][8*ii +: 8],
                     //                        resp_data_qwords[16+3][8*ii +: 8], resp_data_qwords[16+2][8*ii +: 8], resp_data_qwords[16+1][8*ii +: 8], resp_data_qwords[16][8*ii +: 8],
                     //                        resp_data_qwords[12+3][8*ii +: 8], resp_data_qwords[12+2][8*ii +: 8], resp_data_qwords[12+1][8*ii +: 8], resp_data_qwords[12][8*ii +: 8],
                     //                        resp_data_qwords[ 8+3][8*ii +: 8], resp_data_qwords[ 8+2][8*ii +: 8], resp_data_qwords[ 8+1][8*ii +: 8], resp_data_qwords[ 8][8*ii +: 8],
                     //                        resp_data_qwords[ 4+3][8*ii +: 8], resp_data_qwords[ 4+2][8*ii +: 8], resp_data_qwords[ 4+1][8*ii +: 8], resp_data_qwords[ 4][8*ii +: 8],
                     //                        resp_data_qwords[ 0+3][8*ii +: 8], resp_data_qwords[ 0+2][8*ii +: 8], resp_data_qwords[ 0+1][8*ii +: 8], resp_data_qwords[ 0][8*ii +: 8]
                     //                       };

                     for (int jj=0; jj<64; jj++) begin
                        trnps_data_clines[ii][8*jj +: 8] = resp_data_qwords[jj][8*ii +: 8];                     
                     end

                     // fill into L1SCP:
                     scp_way = scp_line_id / 12;
                     scp_set = scp_line_id % 12; 
                     m_l1_scp[scp_way][scp_set] = trnps_data_clines[ii];

                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Transpose 8-bit: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%h", trnps_data_clines[ii])) 

                  end // if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1))

                  // increment for next scp_line:
                  scp_line_id++;
                  scp_line_id = scp_line_id % 48;     // max 48 clines
               end  // for (int ii=0; ii<parent_tload_num_scp_lines; ii++)
            end  // if ((parent_tload_resp_line_count+1) == 64)
         end
         else if (trans_desc.get_int_param(pk_tload_transform) == 6) begin   // Transpose16
            // always need total 32 L2 resp_line_data;
            // RTL collects every 4 L2 resp_line_data and processes them;
            // in this L1D_SB model: to simplify, we collect all 32 L2 resp_line_data and then process them all together;
             
            //problem: L2 Resp (data) could come back out-of-order: change to code below:
            m_tload_resp_line_data[tload_spawn_req_idx] = req_desc.get_rsp_data();
             
            // after collect all 32 clines: transpose and fill into L1SCP:
            if ((parent_tload_resp_line_count+1) == 32) begin
               // extract 32-bytes (octal-word) from each cline:
               oword_offset = trans_desc.get_int_param(pk_tload_byte_offset) / 32;
               for (int ii=0; ii<32; ii++) begin
                  resp_data_owords[ii] = m_tload_resp_line_data[ii].get_oword(oword_offset);
                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP (Transpose 16): resp_data_owords[%0d]=0x%h", ii, resp_data_owords[ii]))
               end

               // process for each scp_line (considering tensor_mask):
               scp_line_id = parent_tload_scp_start_line;    // starting line
               scp_line_id = scp_line_id % 48;     // max 48 clines

               // for multiple scp lines:
               for (int ii=0; ii<parent_tload_num_scp_lines; ii++) begin      // num_scp_lines: max=16
                  if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1)) begin 
                     // transpose: each 2-byte (16-bit): to construct a cline:
                     //trnps_data_clines[ii] = {
                     //                        resp_data_owords[28+3][16*ii +: 16], resp_data_owords[28+2][16*ii +: 16], resp_data_owords[28+1][16*ii +: 16], resp_data_owords[28][16*ii +: 16],
                     //                        resp_data_owords[24+3][16*ii +: 16], resp_data_owords[24+2][16*ii +: 16], resp_data_owords[24+1][16*ii +: 16], resp_data_owords[24][16*ii +: 16],
                     //                        resp_data_owords[20+3][16*ii +: 16], resp_data_owords[20+2][16*ii +: 16], resp_data_owords[20+1][16*ii +: 16], resp_data_owords[20][16*ii +: 16],
                     //                        resp_data_owords[16+3][16*ii +: 16], resp_data_owords[16+2][16*ii +: 16], resp_data_owords[16+1][16*ii +: 16], resp_data_owords[16][16*ii +: 16],
                     //                        resp_data_owords[12+3][16*ii +: 16], resp_data_owords[12+2][16*ii +: 16], resp_data_owords[12+1][16*ii +: 16], resp_data_owords[12][16*ii +: 16],
                     //                        resp_data_owords[ 8+3][16*ii +: 16], resp_data_owords[ 8+2][16*ii +: 16], resp_data_owords[ 8+1][16*ii +: 16], resp_data_owords[ 8][16*ii +: 16],
                     //                        resp_data_owords[ 4+3][16*ii +: 16], resp_data_owords[ 4+2][16*ii +: 16], resp_data_owords[ 4+1][16*ii +: 16], resp_data_owords[ 4][16*ii +: 16],
                     //                        resp_data_owords[ 0+3][16*ii +: 16], resp_data_owords[ 0+2][16*ii +: 16], resp_data_owords[ 0+1][16*ii +: 16], resp_data_owords[ 0][16*ii +: 16]
                     //                       };

                     for (int jj=0; jj<32; jj++) begin
                        trnps_data_clines[ii][16*jj +: 16] = resp_data_owords[jj][16*ii +: 16];                     
                     end
                     //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Transpose 16: trnps_data_clines[%0d]=0x%h", ii, trnps_data_clines[ii]))
                     
                     // fill into L1SCP:
                     scp_way = scp_line_id / 12;
                     scp_set = scp_line_id % 12; 
                     m_l1_scp[scp_way][scp_set] = trnps_data_clines[ii];

                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Transpose 16-bit: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%h", trnps_data_clines[ii])) 

                  end // if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1))

                  // increment for next scp_line:
                  scp_line_id++;
                  scp_line_id = scp_line_id % 48;     // max 48 clines
               end  // for (int ii=0; ii<parent_tload_num_scp_lines; ii++)
            end  // if ((parent_tload_resp_line_count+1) == 32) 
         end
         else if (trans_desc.get_int_param(pk_tload_transform) == 7) begin   // Transpose32
            // always need total 16 L2 resp_line_data;
            // RTL collects every 2 L2 resp_line_data and processes them;
            // in this L1D_SB model: to simplify, we collect all 16 L2 resp_line_data and then process them all together;
             
            //problem: L2 Resp (data) could come back out-of-order: change to code below:
            m_tload_resp_line_data[tload_spawn_req_idx] = req_desc.get_rsp_data();
             
            // after collect all 16 clines: transpose and fill into L1SCP:
            if ((parent_tload_resp_line_count+1) == 16) begin
               // extract 64-bytes (hex-word) from each cline:
               for (int ii=0; ii<16; ii++) begin
                  resp_data_clines[ii] = m_tload_resp_line_data[ii].get_xword(0);
                  //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP (Transpose 32): resp_data_clines[%0d]=0x%h", ii, resp_data_clines[ii]))
               end

               // process for each scp_line (considering tensor_mask):
               scp_line_id = parent_tload_scp_start_line;    // starting line
               scp_line_id = scp_line_id % 48;     // max 48 clines

               // for multiple scp lines:
               for (int ii=0; ii<parent_tload_num_scp_lines; ii++) begin      // num_scp_lines: max=16
                  if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1)) begin 
                     // transpose: each 4-byte (32-bit): to construct a cline:
                     //trnps_data_clines[ii] = {
                     //                        resp_data_clines[12+3][32*ii +: 32], resp_data_clines[12+2][32*ii +: 32], resp_data_clines[12+1][32*ii +: 32], resp_data_clines[12][32*ii +: 32],
                     //                        resp_data_clines[ 8+3][32*ii +: 32], resp_data_clines[ 8+2][32*ii +: 32], resp_data_clines[ 8+1][32*ii +: 32], resp_data_clines[ 8][32*ii +: 32],
                     //                        resp_data_clines[ 4+3][32*ii +: 32], resp_data_clines[ 4+2][32*ii +: 32], resp_data_clines[ 4+1][32*ii +: 32], resp_data_clines[ 4][32*ii +: 32],
                     //                        resp_data_clines[ 0+3][32*ii +: 32], resp_data_clines[ 0+2][32*ii +: 32], resp_data_clines[ 0+1][32*ii +: 32], resp_data_clines[ 0][32*ii +: 32]
                     //                       };

                     for (int jj=0; jj<16; jj++) begin
                        trnps_data_clines[ii][32*jj +: 32] = resp_data_clines[jj][32*ii +: 32]; 
                     end
                     //`evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Transpose 32: trnps_data_clines[%0d]=0x%h", ii, trnps_data_clines[ii]))

                     // fill into L1SCP:
                     scp_way = scp_line_id / 12;
                     scp_set = scp_line_id % 12; 
                     m_l1_scp[scp_way][scp_set] = trnps_data_clines[ii];

                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("for above TensorLoad_L1SCP: after Transpose 32-bit: fill cline data into L1SCP: scp_line_id=%0d scp_way=%0d scp_set=%0d: as below:", scp_line_id, scp_way, scp_set))
                     `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("0x%h", trnps_data_clines[ii])) 

                  end // if ((parent_tload_use_tmask == 1'b0) || (parent_tload_tensor_mask[ii] == 1))

                  // increment for next scp_line:
                  scp_line_id++;
                  scp_line_id = scp_line_id % 48;     // max 48 clines
               end  // for (int ii=0; ii<parent_tload_num_scp_lines; ii++)
            end  // if ((parent_tload_resp_line_count+1) == 16) 
         end

         parent_tload_resp_line_count++;
         parent_tload_trans.set_int_param(pk_tload_resp_line_count, parent_tload_resp_line_count);
         
         // when a TensorLoad_L1SCP completes processing all its L2 Resp data: delete/remove/clear parent_tload_trans: 
         if (parent_tload_trans.get_int_param(pk_tload_resp_line_count) == parent_tload_trans.get_int_param(pk_tload_resp_line_count_max)) begin
            bit  match_found = 1'b0;
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_tload_trans, $sformatf("This entire TensorLoad_L1SCP %s has completed", parent_tload_trans.sprint_obj() ))
            foreach(m_tload_scp_outstanding_q[ii]) begin
               if (m_tload_scp_outstanding_q[ii] == parent_tload_trans) begin
                  match_found = 1'b1;
                  `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), parent_tload_trans, $sformatf("delete the above TensorLoad_L1SCP (parent_tload_trans) from m_tload_scp_outstanding_q[]: %s", m_tload_scp_outstanding_q[ii].sprint_obj() ))
                  m_tload_scp_outstanding_q.delete(ii);
                  break;  
               end
            end
            if (match_found == 1'b0) begin
               sb_error($sformatf("cannot find the above parent_tload_trans %s in m_tload_scp_outstanding_q[]", parent_tload_trans.sprint_obj()), parent_tload_trans, {}, "", `__FILE__, `__LINE__);
            end
         end
          
   endfunction: tensor_load_fill_l1scp



   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tag_update
   //
   //   Tags can be written asynchronously by the miss handler
   //   This events arrives delayed by one cycle because RTL reads tags at S1 but the model
   //   performs the lookup at S2
   //
   function void process_tag_update(input evl_dcache_meta_write_req_t rtl_info);
      evl_paddr_t     set;
      evl_paddr_t     tag;
      evl_paddr_t     addr;
      evl_way_t       way;
      evl_line_desc   line_desc;
      evl_table_desc  table_desc;
      evl_trans_desc  trans_desc;
      evl_thread_id_t tid;
      bit             found = 1'b0;

      tid  = evl_thread_id_t'(rtl_info.thread_id);
      tag  = evl_paddr_t'(rtl_info.meta_req.data.tag) << 7;
      set  = evl_paddr_t'(rtl_info.meta_req.set);
      way  = evl_way_t'($clog2(rtl_info.meta_req.way_en));
      addr = tag | evl_paddr_t'(rtl_info.meta_req.set[0]) << `EVL_LINE_ADDR_LSB;

      foreach (m_mh_tag_update_q[ii]) begin
         trans_desc = m_mh_tag_update_q[ii];
         line_desc = trans_desc.get_line_desc();

         if (addr != (trans_desc.get_paddr() & const_line_paddr_mask)) continue;
         if (tid  != trans_desc.get_thread_id())                       continue;
         if (way  != line_desc.get_cache_way())                        continue;

         m_mh_tag_update_q.delete(ii);
         found = 1'b1;
         break;
      end
      if (found == 1'b0) begin
         sb_error($sformatf("Got a tag update event from Miss Handler for addr=0x%0x (set=%0d way=%0d) thread %0d, but none was expected", addr, set, way, tid), null, {}, "", `__FILE__, `__LINE__);
         foreach (m_mh_tag_update_q[ii]) begin
            trans_desc = m_mh_tag_update_q[ii];
            line_desc = trans_desc.get_line_desc();
            `evl_log_mdesc(UVM_NONE, m_vmod, get_abstract_name(), trans_desc, $sformatf("Pending tag update for %0s (way=%0d thread=%0d)", trans_desc.sprint_obj(), line_desc.get_cache_way(), trans_desc.get_thread_id()))
         end
         return;
      end

      get_cache(trans_desc).req_lookup(table_desc, NOCMD, line_desc.get_cache_st(), DCQ_None);
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), trans_desc, $sformatf("Tag update from miss handler %0p => %0s (set=%0d way=%0d Tag=0x%0x State=%0p)", line_desc.get_cache_st(), table_desc.get_tag_action(), set, way, tag, rtl_info.meta_req.data.coh_state))

      //
      // Process the tag action as necessary.
      //
      if (table_desc.get_tag_action() != St_Ignore) begin
         if (table_desc.get_tag_action() == I) begin
            void'(get_cache(trans_desc).deallocate_way(line_desc));
         end
         line_desc.set_cache_st(table_desc.get_tag_action());
      end

      //
      // If this had been temporarilly soft locked (to accomodate RTL) remove it now
      //
      line_desc.set_int_param(pk_soft_locked, 0);

   endfunction: process_tag_update


   //-----------------------------------------------------------------------------------------------
   //
   // Function: setup_bus_req_dependencies
   //
   // This scoreboard doesn't try to predict the exact order in which requests will be observed on
   // downstream busses. Instead, we setup dependencies between requests that we do want to see
   // ordered and only those dependencies will be checked when observing something on the bus.
   //
   // - Uncacheable traffic must remain totally ordered (or per thread?)
   // - Loads/Evicts requests to same line
   // - Requests from same functionnal unit ?
   //   (reduce_unit, l2_cacheop_unit, tex_unit, tensorload0, tensorload1)
   //
   function void setup_bus_req_dependencies(input evl_req_desc req_desc);
      evl_req_desc   all_reqs[$];
      evl_req_desc   miss_reqs[$];
      evl_req_desc   evict_reqs[$];
      evl_trans_desc root_trans;

      root_trans = req_desc.get_root_trans_desc();
      all_reqs   = {m_pending_miss_reqs, m_pending_evict_reqs, m_active_miss_reqs, m_active_evict_reqs};

      foreach (all_reqs[ii]) begin
         evl_trans_desc tmp_root_trans = all_reqs[ii].get_root_trans_desc();
         if (root_trans == null) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), req_desc, $sformatf("req_desc %0s has null root trans. This isn't expected", req_desc.sprint_obj()))
            continue;
         end
         if ( tmp_root_trans == null) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), all_reqs[ii], $sformatf("all_reqs[%0d] %0s has null root trans. This isn't expected", ii, all_reqs[ii].sprint_obj()))
            continue;
         end
         if (   (   (root_trans.get_cmd()     inside {LD, ST, LD_RA, ST_WA})
                 && (tmp_root_trans.get_cmd() inside {LD, ST, LD_RA, ST_WA})
                 && (req_desc.is_uncached() && all_reqs[ii].is_uncached()))
             || ((root_trans.get_cmd() == TENSOR_REDUCE) && (tmp_root_trans.get_cmd() == TENSOR_REDUCE))
             // || ((req_desc.get_int_param(pk_l2cop) == 1) && (all_reqs[ii].get_int_param(pk_l2cop) == 1))
             // || (req_desc.get_int_param(pk_tload_sel, -1) == all_reqs[ii].get_int_param(pk_tload_sel, -2))
             || ((root_trans.get_paddr() == tmp_root_trans.get_paddr()) && (all_reqs[ii].get_int_param(pk_capacity_evict) == 1) && (root_trans.get_cmd() == LD)))
         begin
            req_desc.add_dep_desc(all_reqs[ii]);
         end
      end
   endfunction: setup_bus_req_dependencies


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_req_act_nc
   //
   function void add_req_act_nc(input evl_cmd_t          cmd_list_in[],
                                input evl_cache_st_t     model_st_list_in[],
                                input evl_dcache_qual_t  qualifier_in,
                                input evl_cache_st_t     tag_action_in,
                                input evl_data_action_t  data_action_in,
                                input evl_bus_cmd_t      bus_cmd_in,
                                input evl_bus_rsp_type_t bus_rsp_in,
                                input int                err_code_in);
      foreach (m_cache[ii]) begin
         m_cache[ii].add_req_act(cmd_list_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, bus_rsp_in, err_code_in, 1'b1);
      end
   endfunction: add_req_act_nc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_req_act
   //
   function void add_req_act(input evl_cmd_t          cmd_list_in[],
                             input evl_cache_st_t     model_st_list_in[],
                             input evl_dcache_qual_t  qualifier_in,
                             input evl_cache_st_t     tag_action_in,
                             input evl_data_action_t  data_action_in,
                             input evl_bus_cmd_t      bus_cmd_in,
                             input evl_bus_rsp_type_t bus_rsp_in,
                             input int                err_code_in);
      foreach (m_cache[ii]) begin
         m_cache[ii].add_req_act(cmd_list_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, bus_rsp_in, err_code_in, 1'b0);
      end
   endfunction: add_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act_nc
   //
   function void add_bus_rsp_act_nc(input evl_cmd_t          cmd_list_in[],
                                    input evl_bus_cmd_t      bus_cmd_list_in[],
                                    input evl_cache_st_t     model_st_list_in[],
                                    input evl_bus_rsp_type_t bus_rsp_in[],
                                    input evl_dcache_qual_t  qualifier_in,
                                    input evl_cache_st_t     tag_action_in,
                                    input evl_data_action_t  data_action_in,
                                    input int                err_code_in);
      foreach (m_cache[ii]) begin
         m_cache[ii].add_bus_rsp_act(cmd_list_in, bus_cmd_list_in, model_st_list_in, bus_rsp_in, qualifier_in, tag_action_in, data_action_in, err_code_in, 1'b1);
      end
   endfunction: add_bus_rsp_act_nc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_cmd_t          cmd_list_in[],
                                 input evl_bus_cmd_t      bus_cmd_list_in[],
                                 input evl_cache_st_t     model_st_list_in[],
                                 input evl_bus_rsp_type_t bus_rsp_in[],
                                 input evl_dcache_qual_t  qualifier_in,
                                 input evl_cache_st_t     tag_action_in,
                                 input evl_data_action_t  data_action_in,
                                 input int                err_code_in);
      foreach (m_cache[ii]) begin
         m_cache[ii].add_bus_rsp_act(cmd_list_in, bus_cmd_list_in, model_st_list_in, bus_rsp_in, qualifier_in, tag_action_in, data_action_in, err_code_in, 1'b0);
      end
   endfunction: add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_neigh_miss_req_export") || (port_name == "neigh_miss_req_export")) begin
         return m_neigh_miss_req_export;
      end
      if ((port_name == "m_neigh_evict_req_export") || (port_name == "neigh_evict_req_export")) begin
         return m_neigh_evict_req_export;
      end
      if ((port_name == "m_neigh_miss_rsp_export") || (port_name == "neigh_miss_rsp_export")) begin
         return m_neigh_miss_rsp_export;
      end
      if ((port_name == "m_neigh_evict_rsp_export") || (port_name == "neigh_evict_rsp_export")) begin
         return m_neigh_evict_rsp_export;
      end
      return null;
   endfunction : get_req_desc_export


   // FUTURE: Emmanuel Marie use port wrappers
   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ptw_desc_export
   //
   virtual function uvm_analysis_export#(evl_ptw_desc) get_ptw_desc_export(input string port_name   = "unknown",
                                                                           input int    port_number = 0);
      if ((port_name == "m_neigh_ptw_req_export") || (port_name == "neigh_ptw_req_export")) begin
         return m_neigh_ptw_req_export;
      end
      if ((port_name == "m_neigh_ptw_rsp_export") || (port_name == "neigh_ptw_rsp_export")) begin
         return m_neigh_ptw_rsp_export;
      end
      return null;
   endfunction: get_ptw_desc_export


   //-----------------------------------------------------------------------------------------------
   function string get_way_name(input evl_way_t way_in);
      case (way_in)
         EVL_WAY_0: return "0";
         EVL_WAY_1: return "1";
         EVL_WAY_2: return "2";
         EVL_WAY_3: return "3";
         default:   return "none";
      endcase
   endfunction : get_way_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      evl_req_desc     first_req_desc;
      evl_req_desc     match_req_desc;
      evl_bus_cmd_t    bus_cmd;
      evl_paddr_t      address;
      evl_bus_req_id_t req_id;

      if (paired_req_desc == null) begin
         bus_cmd  = bus_cmd_in;
         req_id   = req_id_in;
         address  = address_in;
      end
      else begin
         bus_cmd  = (bus_cmd_in  != `EVL_Bus_Idle)         ? bus_cmd_in  : paired_req_desc.get_bus_cmd();
         req_id   = (req_id_in   != ~evl_bus_req_id_t'(0)) ? req_id_in   : paired_req_desc.get_bus_req_id();
         address  = (address_in  != ~evl_paddr_t'(0))      ? address_in  : paired_req_desc.get_paddr();
      end

      if ((port_in & 1) == 0) begin
         find_pending_request(m_pending_miss_reqs, first_req_desc, match_req_desc, req_id, bus_cmd, address, evl_bus_req_source_t'(0), 1'b0);
      end
      else if ((port_in & 1) == 1) begin
         find_pending_request(m_pending_evict_reqs, first_req_desc, match_req_desc, req_id, bus_cmd, address, evl_bus_req_source_t'(0), 1'b0);
      end
      if (match_req_desc != null) begin
         if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc)) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   //    The ETLink bus VC will ask if observed requests are to be checked.
   //    The L1SB scoreboard doesn't model messages and tensor stores so don't check them.
   //    Similarly if the scoreboard is disabled, no request should cause an error.
   //
   function bit is_req_checked(input evl_req_desc req_desc_in);
      return (m_enabled == 1'b1) ? 1'b1 : 1'b0;
   endfunction : is_req_checked


   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_minion_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         sb_error("minion_dcache_scoreboard: m_rtl_port is null", null, {}, "", `__FILE__, `__LINE__);
      end

      m_neigh_miss_req_export  = new("m_neigh_miss_req_export",  this);
      m_neigh_miss_req_fifo    = new("m_neigh_miss_req_fifo",    this);
      m_neigh_miss_rsp_export  = new("m_neigh_miss_rsp_export",  this);
      m_neigh_miss_rsp_fifo    = new("m_neigh_miss_rsp_fifo",    this);
      m_neigh_evict_req_export = new("m_neigh_evict_req_export", this);
      m_neigh_evict_req_fifo   = new("m_neigh_evict_req_fifo",   this);
      m_neigh_evict_rsp_export = new("m_neigh_evict_rsp_export", this);
      m_neigh_evict_rsp_fifo   = new("m_neigh_evict_rsp_fifo",   this);
      m_neigh_ptw_req_export   = new("m_neigh_ptw_req_export", this);
      m_neigh_ptw_req_fifo     = new("m_neigh_ptw_req_fifo",   this);
      m_neigh_ptw_rsp_export   = new("m_neigh_ptw_rsp_export", this);
      m_neigh_ptw_rsp_fifo     = new("m_neigh_ptw_rsp_fifo",   this);

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      m_neigh_miss_req_export.connect(m_neigh_miss_req_fifo.analysis_export);
      m_neigh_miss_rsp_export.connect(m_neigh_miss_rsp_fifo.analysis_export);
      m_neigh_evict_req_export.connect(m_neigh_evict_req_fifo.analysis_export);
      m_neigh_evict_rsp_export.connect(m_neigh_evict_rsp_fifo.analysis_export);
      m_neigh_ptw_req_export.connect(m_neigh_ptw_req_fifo.analysis_export);
      m_neigh_ptw_rsp_export.connect(m_neigh_ptw_rsp_fifo.analysis_export);
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data_neigh   = evl_line_data::type_id::create($sformatf("S%0d:N%0d:C%0d(ETLINK)", get_shire_id(), get_neighborhood_id(), get_core_id()));
      m_template_line_desc_neigh  = evl_line_desc::type_id::create($sformatf("S%0d:N%0d:C%0d(ETLINK)", get_shire_id(), get_neighborhood_id(), get_core_id()));
      m_template_trans_desc_neigh = evl_trans_desc::type_id::create($sformatf("S%0d:N%0d:C%0d(ETLINK)", get_shire_id(), get_neighborhood_id(), get_core_id()));
      m_template_req_desc_neigh   = evl_req_desc::type_id::create($sformatf("S%0d:N%0d:C%0d(ETLINK)", get_shire_id(), get_neighborhood_id(), get_core_id()));
      m_template_tlb_entry        = evl_page_table_entry::type_id::create($sformatf("S%0d:N%0d:C%0d(PTE)", get_shire_id(), get_neighborhood_id(), get_core_id()));

      m_template_trans_desc_neigh.set_shire_id(get_shire_id());
      m_template_trans_desc_neigh.set_neighborhood_id(get_neighborhood_id());
      m_template_trans_desc_neigh.set_core_id(get_core_id());

      m_template_req_desc_neigh.set_shire_id(get_shire_id());
      m_template_req_desc_neigh.set_neighborhood_id(get_neighborhood_id());
      m_template_req_desc_neigh.set_core_id(get_core_id());

      m_template_line_desc_neigh.set_data_template(m_template_req_data_neigh);
      m_template_trans_desc_neigh.set_data_template(m_template_req_data_neigh);
      m_template_req_desc_neigh.set_data_template(m_template_req_data_neigh);
      m_template_req_desc_neigh.set_bus_type(EVL_BUS_TYPE_ETLINK);

      m_template_trans_desc_neigh.set_line_template(m_template_line_desc_neigh);
      m_template_req_desc_neigh.set_line_template(m_template_line_desc_neigh);


      m_template_trans_desc = evl_trans_desc::type_id::create ($sformatf("S%0d:N%0d:C%0d", get_shire_id(), get_neighborhood_id(), get_core_id()));
      m_template_trans_desc.set_shire_id(get_shire_id());
      m_template_trans_desc.set_neighborhood_id(get_neighborhood_id());
      m_template_trans_desc.set_core_id(get_core_id());
      m_template_trans_desc.set_data_template(m_template_req_data_neigh);
      m_template_trans_desc.set_line_template(m_template_line_desc_neigh);

      // L1 cache is 4 ways, 16 sets for the shared model.
      // 4 ways 2 sets for thread 1 only model.
      m_cache[0].set_cache_config(m_num_ways, m_cache_set_mask);
      m_cache[1].set_cache_config(m_num_ways, m_thread_set_mask_split[1]);

      // TLB is direct mapped (1 way), 8 sets
      m_tlb.set_cache_config(1, ((evl_paddr_t'(1) << 3) - 1) << `EVL_PAGE_ADDR_LSB);
      m_tlb.set_line_template(m_template_tlb_entry);

   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);

      `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("UVM Post_Shutdown Phase: check that all L1SB model queues should be empty ..."))

      foreach (m_replay_queue[ii]) begin
         sb_error($sformatf("m_replay_queue is not empty: %0s is still pending (%0s)", m_replay_queue[ii].sprint_obj(), ((m_replay_queue[ii].get_int_param(pk_replay_ready) == 1) ? "ready" : "not ready")), null, {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_pending_miss_reqs[ii]) begin
         sb_error($sformatf("m_pending_miss_reqs is not empty: %0s is still pending", m_pending_miss_reqs[ii].sprint_obj()), m_pending_miss_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_pending_evict_reqs[ii]) begin
         sb_error($sformatf("m_pending_evict_reqs is not empty: %0s is still pending", m_pending_evict_reqs[ii].sprint_obj()), m_pending_evict_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_pending_ptw_reqs[ii]) begin
         sb_error($sformatf("m_pending_ptw_reqs is not empty: %0s is still pending", m_pending_ptw_reqs[ii].sprint_obj()), m_pending_ptw_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_active_miss_reqs[ii]) begin
         sb_error($sformatf("m_active_miss_reqs is not empty: %0s is still active", m_active_miss_reqs[ii].sprint_obj()), m_active_miss_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_active_evict_reqs[ii]) begin
         sb_error($sformatf("m_active_evict_reqs is not empty: %0s is still active", m_active_evict_reqs[ii].sprint_obj()), m_active_evict_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_active_ptw_reqs[ii]) begin
         sb_error($sformatf("m_active_ptw_reqs is not empty: %0s is still active", m_active_ptw_reqs[ii].sprint_obj()), m_active_ptw_reqs[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_s1_queue[ii]) begin
         sb_error($sformatf("m_s1_queue is not empty: %0s is still pending", m_s1_queue[ii].sprint_obj()), m_s1_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_s2_queue[ii]) begin
         sb_error($sformatf("m_s2_queue is not empty: %0s is still pending", m_s2_queue[ii].sprint_obj()), m_s2_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_cacheop_s0_queue[ii]) begin
         sb_error($sformatf("m_cacheop_s0_queue is not empty: %0s is still pending", m_cacheop_s0_queue[ii].sprint_obj()), m_cacheop_s0_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_cacheop_s1_queue[ii]) begin
         sb_error($sformatf("m_cacheop_s1_queue is not empty: %0s is still pending", m_cacheop_s1_queue[ii].sprint_obj()), m_cacheop_s1_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_cacheop_l2_s1_queue[ii]) begin
         sb_error($sformatf("m_cacheop_l2_s1_queue is not empty: %0s is still pending", m_cacheop_l2_s1_queue[ii].sprint_obj()), m_cacheop_l2_s1_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_cacheop_s2_queue[ii]) begin
         sb_error($sformatf("m_cacheop_s2_queue is not empty: %0s is still pending", m_cacheop_s2_queue[ii].sprint_obj()), m_cacheop_s2_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_mh_tag_update_q[ii]) begin
         sb_error($sformatf("m_mh_tag_update_q is not empty: %0s is still pending", m_mh_tag_update_q[ii].sprint_obj()), m_mh_tag_update_q[ii], {}, "", `__FILE__, `__LINE__);
      end

      for (int jj = 0; jj < 2; jj++) begin
         foreach (m_tload_wait_queue[jj][ii]) begin
            sb_error($sformatf("m_tload_wait_queue is not empty: %0s is still pending", m_tload_wait_queue[jj][ii].sprint_obj()), m_tload_wait_queue[jj][ii], {}, "", `__FILE__, `__LINE__);
         end
         foreach (m_tload_s1_queue[jj][ii]) begin
            sb_error($sformatf("m_tload_s1_queue is not empty: %0s is still pending", m_tload_s1_queue[jj][ii].sprint_obj()), m_tload_s1_queue[jj][ii], {}, "", `__FILE__, `__LINE__);
         end
         foreach (m_tload_s2_queue[jj][ii]) begin
            sb_error($sformatf("m_tload_s2_queue is not empty: %0s is still pending", m_tload_s2_queue[jj][ii].sprint_obj()), m_tload_s2_queue[jj][ii], {}, "", `__FILE__, `__LINE__);
         end
      end

      if (m_en_s3_vpu_scp_data_chk == 1'b1) begin
         foreach (m_vpu_scp_read_q[ii]) begin
            sb_error($sformatf("m_vpu_scp_read_q is not empty: %0s is still pending", m_vpu_scp_read_q[ii].sprint_obj()), m_vpu_scp_read_q[ii], {}, "", `__FILE__, `__LINE__);
         end
      end

      foreach (m_reduce_wait_queue[ii]) begin
         sb_error($sformatf("m_reduce_wait_queue is not empty: %0s is still pending", m_reduce_wait_queue[ii].sprint_obj()), m_reduce_wait_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_reduce_queue[ii]) begin
         sb_error($sformatf("m_reduce_queue is not empty: %0s is still pending", m_reduce_queue[ii].sprint_obj()), m_reduce_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_reduce_recv_spawn_pend[ii]) begin
         sb_error($sformatf("m_reduce_recv_spawn_pend is not empty: %0s is still pending", m_reduce_recv_spawn_pend[ii].sprint_obj()), m_reduce_recv_spawn_pend[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_reduce_send_data_pend[ii]) begin
         sb_error($sformatf("m_reduce_send_data_pend is not empty: %0s is still pending", m_reduce_send_data_pend[ii].sprint_obj()), m_reduce_send_data_pend[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_reduce_recv_data_retn[ii]) begin
         sb_error($sformatf("m_reduce_recv_data_retn is not empty: %0s is still pending", m_reduce_recv_data_retn[ii].sprint_obj()), m_reduce_recv_data_retn[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_tstore_s1_queue[ii]) begin
         sb_error($sformatf("m_tstore_s1_queue is not empty: %0s is still pending", m_tstore_s1_queue[ii].sprint_obj()), m_tstore_s1_queue[ii], {}, "", `__FILE__, `__LINE__);
      end
      
      foreach (m_tstore_scp_s1_queue[ii]) begin
         sb_error($sformatf("m_tstore_scp_s1_queue is not empty: %0s is still pending", m_tstore_scp_s1_queue[ii].sprint_obj()), m_tstore_scp_s1_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_tstore_vpu_reg_queue[ii]) begin
         sb_error($sformatf("m_tstore_vpu_reg_queue is not empty: %0s is still pending", m_tstore_vpu_reg_queue[ii].sprint_obj()), m_tstore_vpu_reg_queue[ii], {}, "", `__FILE__, `__LINE__);
      end

      if (m_en_s3_core_resp_chk == 1'b1) begin
         foreach (m_s3_core_resp_queue[ii]) begin
            sb_error($sformatf("m_s3_core_resp_queue is not empty: %0s is still pending", m_s3_core_resp_queue[ii].sprint_obj()), m_s3_core_resp_queue[ii], {}, "", `__FILE__, `__LINE__);
         end
      end
 
      foreach (m_core_bus_err_queue[ii]) begin
         sb_error($sformatf("m_core_bus_err_queue is not empty: %0s is still pending", m_core_bus_err_queue[ii].sprint_obj()), m_core_bus_err_queue[ii], {}, "", `__FILE__, `__LINE__);
      end
   
      if (m_en_s3_vpu_tenb_data_chk == 1'b1) begin 
         foreach (m_s3_vpu_tenb_data_queue[ii]) begin
            sb_error($sformatf("m_s3_vpu_tenb_data_queue is not empty: %0s is still pending", m_s3_vpu_tenb_data_queue[ii].sprint_obj()), m_s3_vpu_tenb_data_queue[ii], {}, "", `__FILE__, `__LINE__);
         end
      end

      foreach (m_tload_scp_outstanding_q[ii]) begin
         sb_error($sformatf("m_tload_scp_outstanding_q is not empty: %0s is still pending", m_tload_scp_outstanding_q[ii].sprint_obj()), m_tload_scp_outstanding_q[ii], {}, "", `__FILE__, `__LINE__);
      end

      foreach (m_cacheop_bw_ctrl_first_cyc_q[ii]) begin
         sb_error($sformatf("m_cacheop_bw_ctrl_first_cyc_q is not empty: still pending: ii=%0d first_cyc=%0d", ii, m_cacheop_bw_ctrl_first_cyc_q[ii]), null, {}, "", `__FILE__, `__LINE__);
      end
      
   endtask : post_shutdown_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard is %0s", m_enabled ? "enabled" : "disabled"))
      if (m_enabled == 1'b1) begin
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard: m_allow_bus_err=%0b", m_allow_bus_err))
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard: m_en_s3_core_resp_chk=%0b", m_en_s3_core_resp_chk))

         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard: Shared mode: m_num_sets=%0d m_cache_set_mask=0x%h", m_num_sets,  m_cache_set_mask))
         for (int ii = 0; ii < `CORE_NR_THREADS; ii++) begin
            `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard: Split mode: Thread=%0d: m_threads_split_num_sets[]=%0d m_thread_set_mask_split[]=0x%h", ii, m_threads_split_num_sets[ii],  m_thread_set_mask_split[ii]))
         end
         for (int ii = 0; ii < `CORE_NR_THREADS; ii++) begin
            `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("L1 Dcache Scoreboard: SCP mode: Thread=%0d: m_threads_scp_num_sets[]=%0d m_thread_set_mask_scp[]=0x%h", ii, m_threads_scp_num_sets[ii],  m_thread_set_mask_scp[ii]))
         end

      end // if (m_enabled == 1'b1)

      fork
         super.run_phase(phase);
         fork
            watch_for_timeouts();
            watch_for_bus_requests();
            watch_for_rtl_events();
            process_cache_ctrl();
         join_none;
      join_none;
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("L1D(SB)");
      m_cache_ctrl_req_mbox = new();
      m_cache_ctrl_done_mbox = new();

      m_vmod   = VMOD_L1_SB;
      m_err_pk = pk_l1sb_error_intensity;

      if (!(`et_value_plusargs("l1d_sb_en=%d", m_enabled))) begin
         m_enabled = 1'b1;
      end
      if (!(`et_value_plusargs("allow_bus_err=%d", m_allow_bus_err))) begin
         m_allow_bus_err = 1'b1;    // default enabled
      end
      `evl_set_error_intensity(m_err_pk, m_enabled ? -1 : UVM_HIGH);

      if (!(`et_value_plusargs("en_s3_core_resp_chk=%d", m_en_s3_core_resp_chk))) begin
         m_en_s3_core_resp_chk = 1'b1;     // default disabled -> changed to enabled
      end

      if (!(`et_value_plusargs("en_s3_vpu_tenb_data_chk=%d", m_en_s3_vpu_tenb_data_chk))) begin
         m_en_s3_vpu_tenb_data_chk = 1'b1;     // default disabled -> changed to enabled
      end

      if (!(`et_value_plusargs("en_s3_vpu_scp_data_chk=%d", m_en_s3_vpu_scp_data_chk))) begin
         m_en_s3_vpu_scp_data_chk = 1'b1;     // default disabled -> changed to enabled
      end

      if (!(`et_value_plusargs("en_vpu_scp_read_conflict_tload_scp_chk=%d", m_en_vpu_scp_read_conflict_tload_scp_chk))) begin
         m_en_vpu_scp_read_conflict_tload_scp_chk = 1'b1;     // default enabled
      end

      if (!(`et_value_plusargs("en_tstore_scp_conflict_tload_scp_chk=%d", m_en_tstore_scp_conflict_tload_scp_chk))) begin
         m_en_tstore_scp_conflict_tload_scp_chk = 1'b1;     // default enabled
      end

      if (!(`et_value_plusargs("rtl_s3_core_data_ignore_x=%d", m_rtl_s3_core_data_ignore_x))) begin
         m_rtl_s3_core_data_ignore_x = 1'b0;     // default disabled
      end

      if (!(`et_value_plusargs("en_chk_cacheop_bw_rate_n_max=%d", m_en_chk_cacheop_bw_rate_n_max))) begin
         m_en_chk_cacheop_bw_rate_n_max = 1'b1;    // default enabled
      end

      if (!(`et_value_plusargs("en_chk_l1scp_mode_when_vpu_read_scp=%d", m_en_chk_l1scp_mode_when_vpu_read_scp))) begin
         m_en_chk_l1scp_mode_when_vpu_read_scp = 1'b1;    // default enabled
      end

      `ifdef ET_AMOCMPEXCH_FIX
         m_amocmpexch_fix_en = 1'b1;
      `else
         m_amocmpexch_fix_en = `et_test_plusargs("amocmpexch_fix");
      `endif

      m_cache_set_mask = ((evl_paddr_t'(1) << $clog2(m_num_sets)) - 1) << `EVL_LINE_ADDR_LSB;   // 40'b0011_1100_0000 (for both threads)
      for (int ii = 0; ii < `CORE_NR_THREADS; ii++) begin
         m_thread_set_mask_split[ii] = ((evl_paddr_t'(1) << $clog2(m_threads_split_num_sets[ii])) - 1) << `EVL_LINE_ADDR_LSB;  // T0: 40'b0001_1100_0000; T1: 40'b0100_0000
         m_thread_set_mask_scp  [ii] = ((evl_paddr_t'(1) << $clog2(m_threads_scp_num_sets[ii]))   - 1) << `EVL_LINE_ADDR_LSB;  // 40'b0100_0000 (for both threads)
      end

      m_TensorError_CSR[0] = 'h0;
      m_TensorError_CSR[1] = 'h0;
      m_check_s3_vpu_tenb_data_cnt = 0;

   endfunction : new

endclass : evl_minion_dcache_scoreboard
