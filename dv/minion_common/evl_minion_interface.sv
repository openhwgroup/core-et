`include "rtl/inc/soc.vh"

`define MIN_IFC_TOP     minion_top
`define MIN_IFC_CORE    minion_top.core
`define MIN_IFC_INTPIPE minion_top.core.intpipe
`define MIN_IFC_DCACHE  minion_top.core.dcache
`define MIN_IFC_FE      minion_top.core.frontend
`define MIN_IFC_VPU     minion_top.vpu
`define MIN_VPU         minion_top.vpu
`define MIN_VPU_CTRL    `MIN_VPU.ctrl
`define ARCH_STATE_MON  minion.arch_state_mon
`define TB_TOP          top

module evl_minion_interface #(parameter MINION_ID = 0)
                            (input reset,
                             input isolate,
                             input clock
                            );
   import evl_base_pkg::*;
   import evl_minion_vc_pkg::*;

   int pk_ex_tima_valid_unqual              = evl_param_xref::get_param_key("pk_ex_tima_valid_unqual");
   int pk_ex_txfma_valid_all                = evl_param_xref::get_param_key("pk_ex_txfma_valid_all");
   int pk_ex_vpu_ctrl_valid_qual            = evl_param_xref::get_param_key("pk_ex_vpu_ctrl_valid_qual");
   int pk_ex_vpu_ctrl_cmd                   = evl_param_xref::get_param_key("pk_ex_vpu_ctrl_cmd");
   int pk_ex_vpu_ctrl_dtype                 = evl_param_xref::get_param_key("pk_ex_vpu_ctrl_dtype");
   int pk_vpu_ctrl_ml_tensorquant_enabled   = evl_param_xref::get_param_key("pk_vpu_ctrl_ml_tensorquant_enabled");
   int pk_vpu_ctrl_ml_tensorfma_enabled     = evl_param_xref::get_param_key("pk_vpu_ctrl_ml_tensorfma_enabled");
   int pk_virus70_window_start              = evl_param_xref::get_param_key("pk_virus70_window_start");
   int pk_virus70_window_end                = evl_param_xref::get_param_key("pk_virus70_window_end");
   int pk_sparse_nn_mlp_window_start        = evl_param_xref::get_param_key("pk_sparse_nn_mlp_window_start"); 
   int pk_sparse_nn_mlp_window_end          = evl_param_xref::get_param_key("pk_sparse_nn_mlp_window_end");
   int pk_sparse_nn_emb_window_start = evl_param_xref::get_param_key("pk_sparse_nn_emb_window_start"); 
   int pk_sparse_nn_emb_window_end   = evl_param_xref::get_param_key("pk_sparse_nn_emb_window_end");


   reg  start_monitor = 1'b0;
   reg  mon_reset_on  = 1'b0;
   reg  mon_reset_off = 1'b0;
   reg  mon_power_on  = 1'b0;
   reg  mon_power_off = 1'b0;

   wire my_end_of_sim;
   reg  my_end_of_sim_d1;

   bit  m_start_watching   = 1'b0;
   wire m_clock            = clock & start_monitor;

   // Thread enabled
   reg [`CORE_NR_THREADS-1:0]            mon_thread_enabled = '0;

   wire  mon_ioshire;            // minion in IOshire (Service Processor)
   assign  mon_ioshire = minion_top.ioshire;
   logic  mon_ioshire_sampled = 0;

   // FE signals
   wire                                  fe_mon_id_inst_ready;
   wire                                  fe_mon_id_inst_valid;
   wire                                  fe_mon_id_inst_thread_id;
   wire frontend_core_resp               fe_mon_id_inst_data;
   wire               [`CORE_NR_THREADS-1:0]   fe_mon_f0_core_req_valid;
   wire               [`CORE_NR_THREADS-1:0]   core_in_debug_mode;
   wire minion_fe_req [`CORE_NR_THREADS-1:0]   fe_mon_f0_core_req;
   wire               [`CORE_NR_THREADS-1:0]   fe_mon_ipi_with_pc;
   logic                                       fe_mon_wb_xcpt;
   logic                                       fe_mon_wb_csr_eret;
   logic                                       fe_mon_wb_csr_replay;
   logic                                       fe_mon_wb_reg_replay;

   // FE coverage signals
   wire f1_icache_req_ready;
   wire f1_icache_req_valid;
   wire frontend_icache_req f1_icache_req;
   wire f5_icache_resp_valid;
   wire f5_icache_resp_miss;
   wire f6_icache_fill_done;
   wire icache_frontend_resp f5_icache_resp;
   wire minion_vm_status t0_vm_status;
   wire minion_vm_status t1_vm_status;
   wire [`CORE_NR_THREADS-1:0] f0_icache_req_valid_arb;
   wire t0_f0_icache_req_valid;
   wire t1_f0_icache_req_valid;
   wire [`FE_FETCH_BUFFERS-1:0] t0_buffer_cacheable;
   wire [`FE_FETCH_BUFFERS-1:0] t1_buffer_cacheable;
   wire t0_f7_buffer_id;
   wire t1_f7_buffer_id;
   wire t0_f6_valid;
   wire t0_f6_buffer_wr;
   wire t1_f6_valid;
   wire t1_f6_buffer_wr;
   wire t0_f6_icache_resp_cacheable;
   wire t1_f6_icache_resp_cacheable;
   wire t0_icache_inflight;
   wire t1_icache_inflight;
   wire t0_miss_pending;
   wire t1_miss_pending;


   // FE helper signals
   logic f5_icache_resp_miss_seen;
   logic prev_icache_resp_was_uncacheable;
   logic prev_icache_resp_was_cacheable;
   logic first_t0_arb_req = 0;
   logic first_t1_arb_req = 0;
   logic [2:0] consecutive_t0_arb_req = 0;
   logic [2:0] consecutive_t1_arb_req = 0;
   logic first_t0_inst = 0;
   logic first_t1_inst = 0;
   logic [2:0] consecutive_t0_inst = 0;
   logic [2:0] consecutive_t1_inst = 0;

   logic [4:0] icache_req_valid_hi_cycles = 0;
   logic [4:0] icache_req_valid_to_resp_cycles = 0;
   logic [4:0] icache_req_to_redir_cycles = 0;
   logic uncacheable_inst_t0 = 0;
   logic uncacheable_inst_t1 = 0;
   logic t0_f6_bypass;
   logic t1_f6_bypass;
   logic t0_f7_bypass;
   logic t1_f7_bypass;
   logic t0_inst_from_bypass;
   logic t1_inst_from_bypass;
   logic icache_request_made = 0;
   logic icache_ready_seen = 0;

   logic replay_inst_t0 = 0;
   logic replay_inst_t1 = 0;

   logic t0_inst_uc_from_bypass;
   logic t1_inst_uc_from_bypass;

   logic [4:0] inst_ready_lo_cycles = 0;
   logic [4:0] redir0_to_redir1_cycles = 0;
   logic redir0_seen;


   // Stall signals
   wire  stall_mon_id_ctrl_stall;
   wire  stall_mon_id_ctrl_stall_hazard;
   wire  stall_mon_id_ctrl_stall_hazard_vpu;
   wire  stall_mon_id_int_sboard_hazard;
   wire  stall_mon_id_ctrl_stall_div;
   wire  stall_mon_id_ctrl_stall_trans;
   wire  stall_mon_id_do_fence;
   wire  stall_mon_id_ctrl_stall_dcache;
   wire  stall_mon_id_ctrl_stall_csr;
   wire  [`CORE_NR_THREADS-1:0] stall_mon_csr_fe_stall;
   wire  stall_mon_ex_gsc_busy;
   wire  stall_mon_id_stall_fp2int;
   wire  [`FE_FETCH_BUFFERS-1:0] stall_mon_buffer_empty_t0;
   wire  stall_mon_f6_stall_t0;
   wire  [`FE_FETCH_BUFFERS-1:0] stall_mon_buffer_empty_t1;
   wire  stall_mon_f6_stall_t1;
   wire  [`CORE_NR_THREADS-1:0]  stall_mon_inst_fifo_full;

   // CSR stuff
   wire [11:0]                           mon_csr_mem_addr;
   reg  [11:0]                           mon_csr_wb_addr;
   wire [2:0]                            mon_csr_wb_cmd;
   wire [`XREG_RANGE]                    mon_csr_wb_wdata;
   wire                                  mon_csr_wb_thread_id;
   wire                                  mon_csr_pmu_read_en_wb;
   wire [`CORE_NR_THREADS-1:0] [3:0]     mon_csr_pmu_read_sel;
   wire [`CORE_NR_THREADS-1:0] [63:0]    mon_csr_pmu_read_data;
   wire [`CORE_NR_THREADS-1:0] [8:3]     mon_csr_pmu_write_en;
   wire [63:0]                           mon_csr_pmu_write_data;
   // CSR request to DCache
   wire core_dcache_ctrl                 mon_core_ctrl;
   wire dcache_core_ctrl                 mon_core_ctrl_resp;
   reg                                   waiting_cop_tmask;
   reg                                   waiting_tload_tmask;
   reg                                   tload_0_waiting_tmask_or_previous;   // TL0 (L1SCP) waiting for either tmask or previous_tload
   reg                                   tload_1_waiting_tmask_or_previous;   // TL1 (TenB)  waiting for either tmask or previous_tload
   reg                                   tload_0_waiting_tmask;               // TL0 (L1SCP) waiting for tmask 
   reg                                   tload_1_waiting_tmask;               // TL1 (TenB)  waiting for either tmask 
   // Core request to the DCache
   wire                                  mon_id_core_alloc_rq_pre;
   wire                                  mon_s0_core_alloc_rq_val;
   wire                                  mon_id_core_gsc;
   wire                                  mon_id_core_ready;
   wire                                  mon_s0_core_req_valid;
   logic                                 mon_s1_core_req_valid;
   logic                                 mon_s2_core_req_valid;
   wire minion_dcache_req                mon_s0_core_req;
   wire                                  mon_s0_core_gsc;
   wire                                  mon_s1_core_kill;
   wire       [`DCACHE_DATA_RANGE]       mon_s1_core_store_data;
   wire                                  mon_s2_core_kill;
   // VPU scratchpad direct port
   wire vpu_dcache_ctrl                  mon_s1_vpu_scp_req;
   logic                                 mon_s1_vpu_scp_read_en;      // = mon_s1_vpu_scp_req.scp_req.read_en
   logic                                 mon_s1_vpu_scp_read_en_d1;   // 1-cycle delay (now S2)
   logic                                 mon_s1_vpu_scp_read_en_d2;   // 2-cycle delay (now S3)
   wire       [`DCACHE_DATA_RANGE]       mon_s3_vpu_scp_data;

   // TenB data return for TensorLoad_TenB Req:
   wire dcache_vpu_scp_resp              mon_s3_vpu_scp_resp;         // only for TenB resp data?
   wire       [`DCACHE_DATA_RANGE]       mon_s3_vpu_tenb_data;

   // Tensor_Reduce Req: VPU wait flag:
   wire                                  mon_vpu_reduce_wait;
   // VPU reduce control
   wire dcache_vpu_reduce_ctrl           mon_vpu_reduce_ctrl;
   // DCache response to the Core
   wire                                  mon_s2_core_resp_int_valid;
   wire                                  mon_s3_core_resp_valid;
   wire dcache_minion_resp               mon_s3_core_resp;

   // DCache return Bus_Err indication to Core:
   wire  [1:0]                           mon_bus_err;
   wire  [1:0][`PA_RANGE]                mon_bus_err_addr;

   // Dcache control
   wire dcache_minion_scoreboard         mon_id_core_scoreboard;
   wire                                  mon_s1_core_replay_next;
   wire dcache_tag_xcpt                  mon_s1_core_xcpt;
   wire       [`CORE_NR_THREADS-1:0]     mon_s3_ordered;
   wire                                  mon_s3_invalidate_lr;
   // TLB/PTW control
   wire minion_satp_info                 mon_satp_info;
   wire minion_vm_status[`CORE_NR_THREADS-1:0] mon_vm_status;
   // PTW request
   wire minion_ptw_req                   mon_ptw_req_data;
   wire                                  mon_ptw_req_valid;
   wire                                  mon_ptw_req_ready;
   // PTW response
   wire minion_ptw_pte                   mon_ptw_resp_data;
   wire                                  mon_ptw_resp_valid;
   // Request to L2 evict port
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  mon_l2_evict_req_ready;
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  mon_l2_evict_req_valid;
   wire et_link_minion_evict_req_info_t   mon_l2_evict_req;
   // Request to L2 miss port
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]  mon_l2_miss_req_ready;
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]  mon_l2_miss_req_valid;
   wire et_link_minion_miss_req_info_t   mon_l2_miss_req;
   // Response L2 port
   wire                                  mon_l2_resp_ready;
   wire                                  mon_l2_resp_valid;
   wire et_link_minion_rsp_info_t        mon_l2_resp;
   // Idle flag
   wire                                  mon_dcache_idle;
   // DCache S2 internals
   wire                                  mon_s2_hit;
   wire [`DCACHE_WAYS-1:0]               mon_s2_replace_way_en;
   wire [`DCACHE_WAYS-1:0]               mon_s2_tag_match;
   wire [`DCACHE_WAYS-1:0]               mon_s2_way_select;
   wire                                  mon_s2_valid;
   wire                                  mon_s2_nack;           // Nack and need to be replayed (again) (exclude misaligned case)
   wire [`DCACHE_REPLAYQ_ADDR_RANGE]     mon_s2_rq_push_entry;
   wire [`VA_RANGE]                      mon_s2_req_addr;

   wire                                  mon_s2_mh_req_ready;
   wire                                  mon_s2_mh_req_valid;
   wire [`DCACHE_MH_FILE_SIZE-1:0]       mon_s2_mh_assigned;    // [1:0]; [0]=MH0 [1]=MH1 (s2_mh_req_assigned[1:0])
   wire [`DCACHE_MH_FILE_SIZE-1:0]       mon_s2_mh_accepted;    // [1:0]; [0]=MH0 [1]=MH1 (s2_mh_req_accepted[1:0])
   wire dcache_mh_state                  mon_mh0_state, mon_mh0_state_next;
   wire dcache_mh_state                  mon_mh1_state, mon_mh1_state_next;
   wire dcache_line_state                mon_s2_obs_state;
   logic                                 mon_s2_valid_d1;       // 1-cycle delay version (at S3 stage)

   // Replay Queue arbitration
   wire                                  mon_s0_rq_replay_valid;
   wire dcache_replay_req                mon_s0_rq_replay;
   wire [`DCACHE_REPLAYQ_ADDR_RANGE]     mon_s0_rq_replay_entry;
   wire                                  mon_s1_valid;
   wire                                  mon_s1_misaligned;
   wire                                  mon_metadata_wen;
   wire dcache_meta_write_req            mon_metadata;
   logic                                 mon_mw_thread;
   logic                                 mon_metadata_wen_s2;
   dcache_meta_write_req                 mon_metadata_s2;
   logic                                 mon_mw_thread_s2;
   wire                                  mon_s0_co_mr_valid;   // "mr" - Meta-Read
   wire                                  mon_s0_co_pf_valid;   // Prefetch_L1
   wire                                  mon_s0_cop_valid;
   reg                                   mon_s1_co_mr_valid;
   wire                                  mon_cop_conflict;
   wire [`DCACHE_WAYS-1:0]               mon_cop_way;
   cache_control                         mon_cache_ctrl;
   cache_control                         mon_cache_ctrl_st;
   esr_mprot_t                           mon_s1_mprot;
   esr_mprot_t                           mon_s1_mprot_st;
   logic                                 mon_dcache_bypass_st;

   logic [`CORE_NR_THREADS-1:0]          mon_id_fence;
   logic [`CORE_NR_THREADS-1:0]          mon_ex_fence;

   logic                                 mon_vpu_send_reg;
   logic                                 mon_vpu_send_reg_m1; // one cycle before
   logic                                 mon_vpu_send_reg_m2; // 2 cycles before
   logic                                 mon_vpu_send_reg_m3; // 3 cycles before (the dcache ask VPU to send its register)

   // L1D$ MissHandler stall L2_Resp (FillData):
   wire [1:0]                            mon_mh_l2_resp_stall;
   wire [1:0]                            mon_mh_writeback_pending;
   wire [1:0]                            mon_mh_metawrite_pending;
   wire [1:0]                            mon_mh_meta_write_valid;
   wire                                  mon_l2_fill_slot_avail;
   wire                                  mon_s4_da_write_en_but_dbg;
   reg                                   mon_l2_resp_valid_d1;

   // my own local signals:
   reg  [1:0]                            my_mh_l2_resp_stall_state;
   wire [1:0]                            my_mh_l2_resp_stall_start;
   wire [1:0]                            my_mh_l2_resp_stall_clear;
   reg  [1:0]                            my_mh_l2_resp_stall_clear_d1;

   // my own local signals:
   reg                                   my_reduce_req_vpu_wait;
   reg                                   my_reduce_new_req;       // my own signal to line up with RTL Reduce FSM state Reduce_New_Req

   // Tensor Error:
   wire [`TENSOR_ERROR_SZ-1:0]           mon_csr_tensor_error [2];     // content is "sticky" (until being cleared)
   terror_t0_t                           mon_csr_tensor_err_t0_m1;     // 1-cycle earlier (but still "sticky") 
   terror_t1_t                           mon_csr_tensor_err_t1_m1;     // 1-cycle earlier (but still "sticky")
   terror_t0_t                           mon_tensor_err_t0_m1_1cyc;    // 1-cycle earlier and 1-cycle pulse 
   terror_t1_t                           mon_tensor_err_t1_m1_1cyc;    // 1-cycle earlier and 1-cycle pulse
   wire                                  mon_reduce_err_partner_id;
   wire                                  mon_reduce_err_operation;


   reg  [`CORE_NR_THREADS-1:0][8:3]      mhpmevent_en;

   wire [`CORE_NR_THREADS-1:0][8:3][4:0] mhpmevent;

   reg  [`CORE_NR_THREADS-1:0][6:3][4:0] mon_mhpmevent;

   reg  [`CORE_NR_THREADS-1:0][8:3]      pmu_event_seen_st1;
   reg  [`CORE_NR_THREADS-1:0][8:3]      pmu_event_seen_st2;
   reg  [`CORE_NR_THREADS-1:0][8:3]      pmu_event_seen_st3;

   reg  [`CORE_NR_THREADS-1:0][8:3][4:0] pmu_event_type_st1;
   reg  [`CORE_NR_THREADS-1:0][8:3][4:0] pmu_event_type_st2;
   reg  [`CORE_NR_THREADS-1:0][8:3][4:0] pmu_event_type_st3;


   assign mon_vpu_send_reg_m3 = `MIN_IFC_DCACHE.vpu_reduce_ctrl.send_reg;


   //      CK      RST    DOUT                  DIN                   DEF
   `RST_FF(clock,  reset, mon_vpu_send_reg_m2,  mon_vpu_send_reg_m3,  1'b0)
   `RST_FF(clock,  reset, mon_vpu_send_reg_m1,  mon_vpu_send_reg_m2,  1'b0)
   `RST_FF(clock,  reset, mon_vpu_send_reg,     mon_vpu_send_reg_m1,  1'b0)


   //
   // Monitor signals, drill down in the RTL
   // FUTURE: Emmanuel Marie not all of those will be used, cleanup
   //

   assign f1_icache_req_ready           = `MIN_IFC_FE.f1_icache_req_ready;
   assign f1_icache_req_valid           = `MIN_IFC_FE.f1_icache_req_valid;
   assign f1_icache_req                 = `MIN_IFC_FE.f1_icache_req;
   assign f5_icache_resp_valid          = `MIN_IFC_FE.f5_icache_resp_valid;
   assign f5_icache_resp_miss           = `MIN_IFC_FE.f5_icache_resp_miss;
   assign f6_icache_fill_done           = `MIN_IFC_FE.f6_icache_fill_done;
   assign f5_icache_resp                = `MIN_IFC_FE.f5_icache_resp;
   assign t0_vm_status                  = `MIN_IFC_FE.vm_status[0];
   assign t1_vm_status                  = `MIN_IFC_FE.vm_status[1];
   assign f0_icache_req_valid_arb       = `MIN_IFC_FE.f0_icache_req_valid_arb;
   assign t0_f0_icache_req_valid        = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f0_icache_req_valid;
   assign t1_f0_icache_req_valid        = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f0_icache_req_valid;
   assign t0_buffer_cacheable           = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.buffer_cacheable;
   assign t1_buffer_cacheable           = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.buffer_cacheable;
   assign t0_f7_buffer_id               = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f7_buffer_id;
   assign t1_f7_buffer_id               = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f7_buffer_id;
   assign t0_f6_valid                   = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_valid;
   assign t0_f6_buffer_wr               = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr;
   assign t1_f6_valid                   = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_valid;
   assign t1_f6_buffer_wr               = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_buffer_wr;
   assign t0_f6_icache_resp_cacheable   = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_icache_resp.cacheable;
   assign t1_f6_icache_resp_cacheable   = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_icache_resp.cacheable;

   assign t0_icache_inflight            = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.icache_inflight;
   assign t1_icache_inflight            = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.icache_inflight;
   assign t0_f6_bypass                  = t0_f6_valid & t0_f6_buffer_wr;
   assign t1_f6_bypass                  = t1_f6_valid & t1_f6_buffer_wr;

   assign t0_miss_pending               = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f0_miss_pending;
   assign t1_miss_pending               = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f0_miss_pending;

   assign fe_mon_id_inst_ready           = `MIN_IFC_FE.id_inst_ready;
   assign fe_mon_id_inst_valid           = `MIN_IFC_FE.id_inst_valid;
   assign fe_mon_id_inst_thread_id       = `MIN_IFC_FE.id_inst_thread_id;
   assign fe_mon_id_inst_data            = `MIN_IFC_FE.id_inst_data;
   assign fe_mon_f0_core_req_valid       = `MIN_IFC_FE.f0_core_req_valid;
   assign fe_mon_f0_core_req             = `MIN_IFC_FE.f0_core_req;
   assign fe_mon_ipi_with_pc             = `MIN_IFC_INTPIPE.ipi_with_pc;
   assign fe_mon_wb_xcpt                 = `MIN_IFC_INTPIPE.wb_xcpt;
   assign fe_mon_wb_csr_eret             = `MIN_IFC_INTPIPE.wb_csr_eret;
   assign fe_mon_wb_csr_replay           = `MIN_IFC_INTPIPE.wb_csr_replay;
   assign fe_mon_wb_reg_replay           = `MIN_IFC_INTPIPE.wb_reg_replay;

   assign core_in_debug_mode                 = `MIN_IFC_INTPIPE.in_debug_mode;

   assign stall_mon_id_ctrl_stall            = `MIN_IFC_INTPIPE.id_ctrl_stall;
   assign stall_mon_id_ctrl_stall_hazard     = `MIN_IFC_INTPIPE.id_ctrl_stall_hazard;
   assign stall_mon_id_ctrl_stall_hazard_vpu = `MIN_IFC_INTPIPE.id_ctrl_stall_hazard_vpu;
   assign stall_mon_id_int_sboard_hazard     = `MIN_IFC_INTPIPE.id_int_sboard_hazard;
   assign stall_mon_id_ctrl_stall_div        = `MIN_IFC_INTPIPE.id_ctrl_stall_div;
   assign stall_mon_id_ctrl_stall_trans      = `MIN_IFC_INTPIPE.id_ctrl_stall_trans;
   assign stall_mon_id_do_fence              = `MIN_IFC_INTPIPE.id_ctrl_stall_trans;
   assign stall_mon_id_ctrl_stall_dcache     = `MIN_IFC_INTPIPE.id_ctrl_stall_dcache;
   assign stall_mon_id_ctrl_stall_csr        = `MIN_IFC_INTPIPE.id_ctrl_stall_csr;
   assign stall_mon_csr_fe_stall             = `MIN_IFC_INTPIPE.csr_fe_stall;
   assign stall_mon_ex_gsc_busy              = `MIN_IFC_INTPIPE.ex_gsc_busy;
   assign stall_mon_id_stall_fp2int          = `MIN_IFC_INTPIPE.id_stall_fp2int;
   assign stall_mon_buffer_empty_t0          = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.buffer_empty;
   assign stall_mon_buffer_empty_t1          = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.buffer_empty;
   assign stall_mon_f6_stall_t0              = `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_stall;
   assign stall_mon_f6_stall_t1              = `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_stall;
   assign stall_mon_inst_fifo_full           = `MIN_IFC_FE.inst_fifo_full;


   assign mon_core_ctrl              = `MIN_IFC_DCACHE.core_ctrl;
   assign mon_core_ctrl_resp         = `MIN_IFC_DCACHE.core_ctrl_resp;
   assign mon_id_core_alloc_rq_pre   = `MIN_IFC_DCACHE.id_core_alloc_rq_pre;
   assign mon_s0_core_alloc_rq_val   = `MIN_IFC_DCACHE.s0_core_alloc_rq_val;
   assign mon_id_core_gsc            = `MIN_IFC_DCACHE.id_core_gsc;
   assign mon_id_core_ready          = `MIN_IFC_DCACHE.id_core_ready;
   assign mon_s0_core_req_valid      = `MIN_IFC_DCACHE.s0_core_req_valid;
   assign mon_s0_core_req            = `MIN_IFC_DCACHE.s0_core_req;
   assign mon_s0_core_gsc            = `MIN_IFC_DCACHE.s0_core_gsc;
   assign mon_s1_core_kill           = `MIN_IFC_DCACHE.s1_core_kill;
   assign mon_s1_core_store_data     = `MIN_IFC_DCACHE.s1_core_store_data;
   assign mon_s2_core_kill           = `MIN_IFC_DCACHE.s2_core_kill;
   assign mon_s1_vpu_scp_req         = `MIN_IFC_DCACHE.s1_vpu_ctrl;
   assign mon_s1_vpu_scp_read_en     = mon_s1_vpu_scp_req.scp_req.read_en;
   assign mon_s3_vpu_scp_data        = `MIN_IFC_DCACHE.s3_vpu_scp_data;
   assign mon_vpu_reduce_ctrl        = `MIN_IFC_DCACHE.vpu_reduce_ctrl;
   assign mon_s2_core_resp_int_valid = `MIN_IFC_DCACHE.s2_core_resp_int_valid;
   assign mon_s3_core_resp_valid     = `MIN_IFC_DCACHE.s3_core_resp_valid;
   assign mon_s3_core_resp           = `MIN_IFC_DCACHE.s3_core_resp;

   assign mon_s3_vpu_scp_resp        = `MIN_IFC_DCACHE.s3_vpu_scp_resp;
   assign mon_s3_vpu_tenb_data       = `MIN_IFC_DCACHE.s3_vpu_tenb_data;

   assign mon_bus_err                = `MIN_IFC_DCACHE.bus_err;
   assign mon_bus_err_addr           = `MIN_IFC_DCACHE.bus_err_addr;

   assign mon_id_core_scoreboard     = `MIN_IFC_DCACHE.id_core_scoreboard;
   assign mon_s1_core_replay_next    = `MIN_IFC_DCACHE.s1_core_replay_next;
   assign mon_s1_core_xcpt           = `MIN_IFC_DCACHE.s1_core_xcpt;
   assign mon_s3_ordered             = `MIN_IFC_DCACHE.s3_ordered;
   assign mon_s3_invalidate_lr       = `MIN_IFC_DCACHE.s3_invalidate_lr;
   assign mon_satp_info              = `MIN_IFC_DCACHE.satp_info;
   assign mon_vm_status              = `MIN_IFC_DCACHE.vm_status;
   assign mon_ptw_req_data           = `MIN_IFC_DCACHE.ptw_req_data;
   assign mon_ptw_req_valid          = `MIN_IFC_DCACHE.ptw_req_valid;
   assign mon_ptw_req_ready          = `MIN_IFC_DCACHE.ptw_req_ready;
   assign mon_ptw_resp_data          = `MIN_IFC_DCACHE.ptw_resp_data;
   assign mon_ptw_resp_valid         = `MIN_IFC_DCACHE.ptw_resp_valid;
   assign mon_l2_evict_req_ready     = `MIN_IFC_DCACHE.l2_evict_req_ready;
   assign mon_l2_evict_req_valid     = `MIN_IFC_DCACHE.l2_evict_req_valid;
   assign mon_l2_evict_req           = `MIN_IFC_DCACHE.l2_evict_req;
   assign mon_l2_miss_req_ready      = `MIN_IFC_DCACHE.l2_miss_req_ready;
   assign mon_l2_miss_req_valid      = `MIN_IFC_DCACHE.l2_miss_req_valid;
   assign mon_l2_miss_req            = `MIN_IFC_DCACHE.l2_miss_req;
   assign mon_l2_resp_ready          = `MIN_IFC_DCACHE.l2_resp_ready;
   assign mon_l2_resp_valid          = `MIN_IFC_DCACHE.l2_resp_valid;
   assign mon_l2_resp                = `MIN_IFC_DCACHE.l2_resp;
   assign mon_dcache_idle            = `MIN_IFC_DCACHE.dcache_idle;
   assign mon_s2_hit                 = `MIN_IFC_DCACHE.s2_hit;
   assign mon_s2_replace_way_en      = `MIN_IFC_DCACHE.s2_mh_req_info.way_en;
   assign mon_s2_tag_match           = `MIN_IFC_DCACHE.s2_tag_match;
   assign mon_s2_valid               = `MIN_IFC_DCACHE.s2_valid;
   assign mon_s2_nack                = `MIN_IFC_DCACHE.s2_nack_pre_misaligned;   // exclude misaligned case; should we include s2_nack_misaligned? (FUTURE)
   assign mon_s2_rq_push_entry       = `MIN_IFC_DCACHE.replay_queue.push_entry;
   assign mon_s2_req_addr            = `MIN_IFC_DCACHE.s2_req_addr;

   assign mon_s2_mh_req_ready        = `MIN_IFC_DCACHE.s2_mh_req_ready;
   assign mon_s2_mh_req_valid        = `MIN_IFC_DCACHE.s2_mh_req_valid;
   assign mon_s2_mh_assigned         = `MIN_IFC_DCACHE.s2_mh_req_assigned;   // [0]=MH0 [1]=MH1; How about signal "s2_mh_req_accepted[1:0]"? 
   assign mon_s2_mh_accepted         = `MIN_IFC_DCACHE.s2_mh_req_accepted;
   assign mon_mh0_state              = dcache_mh_state'(`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state[3:0]);
   assign mon_mh0_state_next         = dcache_mh_state'(`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state_next[3:0]);
   assign mon_mh1_state              = dcache_mh_state'(`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state[3:0]);
   assign mon_mh1_state_next         = dcache_mh_state'(`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state_next[3:0]);

   assign mon_s2_obs_state           = `MIN_IFC_DCACHE.s2_way_hit_state;
   assign mon_s0_rq_replay_valid     = `MIN_IFC_DCACHE.s0_rq_replay_valid;
   assign mon_s0_rq_replay           = `MIN_IFC_DCACHE.s0_rq_replay;
   assign mon_s0_rq_replay_entry     = `MIN_IFC_DCACHE.s0_rq_replay_entry;
   assign mon_s1_valid               = `MIN_IFC_DCACHE.s1_valid;
   assign mon_s1_misaligned          = `MIN_IFC_DCACHE.s1_misaligned;


   // Combine miss and hit case into one
   assign mon_s2_way_select          = mon_s2_replace_way_en | ({`DCACHE_WAYS{mon_s2_hit}} & mon_s2_tag_match);
   // CSR accesses
   assign mon_csr_mem_addr           = `MIN_IFC_INTPIPE.csr_file.io_rw_mem_addr;
   assign mon_csr_wb_cmd             = `MIN_IFC_INTPIPE.csr_file.io_rw_wb_cmd;
   assign mon_csr_wb_wdata           = `MIN_IFC_INTPIPE.csr_file.io_rw_wb_wdata;
   assign mon_csr_wb_thread_id       = `MIN_IFC_INTPIPE.csr_file.wb_thread_id;
   // Tag updates from Miss Handler only
   // assign mon_metadata_wen           = `MIN_IFC_DCACHE.s1_md_write_en;
   assign mon_metadata_wen           = `MIN_IFC_DCACHE.s1_mh_mw_valid & `MIN_IFC_DCACHE.s1_mh_mw_grant;
   assign mon_metadata               = `MIN_IFC_DCACHE.s1_mh_mw_req;


   assign mon_csr_pmu_read_en_wb     = `MIN_IFC_INTPIPE.csr_file.pmu_read_en_wb;
   assign mon_csr_pmu_read_sel       = `MIN_IFC_INTPIPE.csr_file.pmu_read_sel;
   assign mon_csr_pmu_read_data      = `MIN_IFC_INTPIPE.csr_file.pmu_read_data;

   assign mon_csr_pmu_write_data     = `MIN_IFC_INTPIPE.csr_file.pmu_write_data;

   assign mon_csr_pmu_write_en       = '{{`MIN_IFC_INTPIPE.csr_file.pmu_write_en[11], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[9], 
                                          `MIN_IFC_INTPIPE.csr_file.pmu_write_en[7], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[5], 
                                          `MIN_IFC_INTPIPE.csr_file.pmu_write_en[3], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[1]},
                                         {`MIN_IFC_INTPIPE.csr_file.pmu_write_en[10], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[8], 
                                          `MIN_IFC_INTPIPE.csr_file.pmu_write_en[6], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[4], 
                                          `MIN_IFC_INTPIPE.csr_file.pmu_write_en[2], `MIN_IFC_INTPIPE.csr_file.pmu_write_en[0]}};


   always_comb
   begin
      if (`MIN_IFC_DCACHE.miss_handler_unit.mw_arb_winner == 0) begin
         mon_mw_thread = `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.req.thread_id;
      end
      else begin
         mon_mw_thread = `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.req.thread_id;
      end
   end

   // Cacheop grant
   assign mon_s0_co_mr_valid         = `MIN_IFC_DCACHE.s0_co_mr_valid;
   assign mon_s0_co_pf_valid         = `MIN_IFC_DCACHE.s0_co_pf_valid;    // Prefetch_L1
   assign mon_s0_cop_valid           = `MIN_IFC_DCACHE.s0_co_mr_valid | `MIN_IFC_DCACHE.s0_co_pf_valid;
   assign mon_cop_conflict           = ~(&`MIN_IFC_DCACHE.mh_probe_rdy) | `MIN_IFC_DCACHE.rq_conflict_to_co;
   // Cacheop way select
   assign mon_cop_way                = `MIN_IFC_DCACHE.cache_op_unit.meta_data_way_en_next;
   // Scratchpad Control
   assign mon_cache_ctrl             = mon_core_ctrl.cache_ctrl;
   assign mon_s1_mprot               = `MIN_IFC_DCACHE.s1_mprot;


   // FENCE seen in ID stage
   always_comb
   begin
      for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
         mon_id_fence[i] = `MIN_IFC_INTPIPE.id_fence_new && `MIN_IFC_INTPIPE.id_valid_qual && (i == `MIN_IFC_INTPIPE.id_thread_id);
      end
   end


   // L1D$ MissHandler stall L2_Resp (FillData):
   assign mon_mh_l2_resp_stall       = `MIN_IFC_DCACHE.mh_l2_resp_stall;
   assign mon_l2_fill_slot_avail     = `MIN_IFC_DCACHE.l2_fill_slot_avail;
   assign mon_s4_da_write_en_but_dbg = `MIN_IFC_DCACHE.s4_da_write_en_but_dbg;

   assign mon_mh_writeback_pending[0] = `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.writeback_pending;
   assign mon_mh_writeback_pending[1] = `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.writeback_pending;
   assign mon_mh_metawrite_pending[0] = `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.metawrite_pending;
   assign mon_mh_metawrite_pending[1] = `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.metawrite_pending;

   // my own local signals : 
   assign my_mh_l2_resp_stall_start[0] = mon_mh_l2_resp_stall[0] & mon_l2_resp_valid & mon_l2_resp_ready
                                       & (mon_l2_resp.id[5:0] == 5'h02)      // for MissHandler_0
                                       & ~my_mh_l2_resp_stall_state[0] ;     // "start" only 1-cyc 

   assign my_mh_l2_resp_stall_start[1] = mon_mh_l2_resp_stall[1] & mon_l2_resp_valid & mon_l2_resp_ready
                                       & (mon_l2_resp.id[5:0] == 5'h03)      // for MissHandler_1
                                       & ~my_mh_l2_resp_stall_state[1] ;     // "start" only 1-cyc

   assign my_mh_l2_resp_stall_clear[0] = my_mh_l2_resp_stall_state[0] & ~mon_mh_l2_resp_stall[0] & mon_l2_fill_slot_avail;
   assign my_mh_l2_resp_stall_clear[1] = my_mh_l2_resp_stall_state[1] & ~mon_mh_l2_resp_stall[1] & mon_l2_fill_slot_avail;

   assign mon_vpu_reduce_wait          = `MIN_IFC_DCACHE.s1_vpu_ctrl.reduce_wait;

   assign mon_csr_tensor_error[0]      = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0];       // "sticky"
   assign mon_csr_tensor_error[1]      = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1];       // "sticky"
   assign mon_csr_tensor_err_t0_m1     = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t0_next;   // 1-cycle earlier (but still "sticky")  
   assign mon_csr_tensor_err_t1_m1     = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t1_next;   // 1-cycle earlier (but still "sticky")

   // 1-cycle pulse:
   assign mon_tensor_err_t0_m1_1cyc.tload     = `MIN_IFC_INTPIPE.csr_file.tensor_load_err_flags;
   assign mon_tensor_err_t0_m1_1cyc.fcc       = |(`MIN_IFC_INTPIPE.csr_file.fcc_overflow[`FCC_PER_MIN-1:0]);
   assign mon_tensor_err_t0_m1_1cyc.scp_unset = `MIN_IFC_INTPIPE.csr_file.scpbit_unset_err_flag && (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b0);
   assign mon_tensor_err_t0_m1_1cyc.co        = `MIN_IFC_INTPIPE.csr_file.cache_ops_err_flags[`DCACHE_ERR_FLAG_CO_LOCK][0];
   assign mon_tensor_err_t0_m1_1cyc.tenb      = `MIN_IFC_INTPIPE.csr_file.tenb_cfg_err;
   assign mon_tensor_err_t0_m1_1cyc.mem_fault = `MIN_IFC_INTPIPE.csr_file.cache_ops_err_flags[`DCACHE_ERR_FLAG_MEM_FAULT][0];
   assign mon_tensor_err_t0_m1_1cyc.tstore    = `MIN_IFC_INTPIPE.csr_file.tensor_store_cfg_err;
   assign mon_tensor_err_t0_m1_1cyc.treduce   = `MIN_IFC_INTPIPE.csr_file.tensor_reduce_err_flags;

   assign mon_tensor_err_t1_m1_1cyc.fcc       = |(`MIN_IFC_INTPIPE.csr_file.fcc_overflow[`CORE_NR_THREADS*`FCC_PER_MIN-1:`FCC_PER_MIN]);
   assign mon_tensor_err_t1_m1_1cyc.scp_unset = `MIN_IFC_INTPIPE.csr_file.scpbit_unset_err_flag && (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b1);
   assign mon_tensor_err_t1_m1_1cyc.co        = `MIN_IFC_INTPIPE.csr_file.cache_ops_err_flags[`DCACHE_ERR_FLAG_CO_LOCK][1];
   assign mon_tensor_err_t1_m1_1cyc.mem_fault = `MIN_IFC_INTPIPE.csr_file.cache_ops_err_flags[`DCACHE_ERR_FLAG_MEM_FAULT][1];

   assign mon_reduce_err_partner_id    = `MIN_IFC_DCACHE.reduce.err_partner_id;
   assign mon_reduce_err_operation     = `MIN_IFC_DCACHE.reduce.err_operation;

   assign my_end_of_sim  = `TB_TOP.m_end_of_sim;


   assign mhpmevent  = '{'{`MIN_IFC_INTPIPE.csr_file.reg_mhpmevent8[1],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent7[1],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent6[1],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent5[1],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent4[1],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent3[1]},
                         '{`MIN_IFC_INTPIPE.csr_file.reg_mhpmevent8[0],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent7[0],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent6[0],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent5[0],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent4[0],
                           `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent3[0]}};

   wire   mon_s1_xcpt_access_or_misalign_fault;
   reg    mon_s2_xcpt_access_or_misalign_fault;
   reg    mon_s3_xcpt_access_or_misalign_fault;
   assign mon_s1_xcpt_access_or_misalign_fault = (mon_s1_core_xcpt.af_ld | mon_s1_core_xcpt.af_st);  // include s1_mf_xcpt case

   reg    mon_tfma_enabled_d1;  // 1-cyc delay version of "Dcache.s1_vpu_ctrl.tfma_enabled";
 

   always @(posedge clock) begin
      if (reset == 1'b1) begin 
         mon_s1_co_mr_valid <= 1'b0;
         my_mh_l2_resp_stall_state[1:0] <= 2'b00;
         mon_l2_resp_valid_d1 <= 1'b0;
         my_mh_l2_resp_stall_clear_d1 <= 1'b0; 
         my_reduce_req_vpu_wait <= 1'b0;
         my_reduce_new_req <= 1'b0;
         mon_s1_vpu_scp_read_en_d1 <= 1'b0;
         mon_s1_vpu_scp_read_en_d2 <= 1'b0;
         mon_s2_xcpt_access_or_misalign_fault <= 1'b0;
         mon_s3_xcpt_access_or_misalign_fault <= 1'b0;
         mon_tfma_enabled_d1 <= 1'b0;
         my_end_of_sim_d1 <= 1'b0;
         mon_mhpmevent <= '{'{5'b0, 5'b0, 5'b0, 5'b0},'{5'b0, 5'b0, 5'b0, 5'b0}};
         mhpmevent_en <= '{6'b0, 6'b0};
      end
   end

   always @(posedge clock) begin
      if (reset == 1'b0) begin
         mon_s1_co_mr_valid <= mon_s0_co_mr_valid; 
         mon_l2_resp_valid_d1 <= mon_l2_resp_valid;
         my_mh_l2_resp_stall_clear_d1 <= my_mh_l2_resp_stall_clear;
         mon_s2_valid_d1 <= mon_s2_valid;

         if (my_mh_l2_resp_stall_start[0] === 1'b1) begin
            //if(my_mh_l2_resp_stall_state[0] === 1'b1) begin     // condition always true (checker no need)
            //   `dut_error("evl_minion_interface", $sformatf("signal my_mh_l2_resp_stall_start[0] asserted: try to set my_mh_l2_resp_stall_state[0]=1 but it's already 1"))
            //end           
            my_mh_l2_resp_stall_state[0] <= 1'b1;
         end
         if (my_mh_l2_resp_stall_start[1] === 1'b1) begin
            //if(my_mh_l2_resp_stall_state[1] === 1'b1) begin     // condition always true (checker no need)
            //   `dut_error("evl_minion_interface", $sformatf("signal my_mh_l2_resp_stall_start[1] asserted: try to set my_mh_l2_resp_stall_state[1]=1 but it's already 1"))
            //end           
            my_mh_l2_resp_stall_state[1] <= 1'b1;
         end

         if (my_mh_l2_resp_stall_clear[0] === 1'b1) begin 
            //if(my_mh_l2_resp_stall_state[0] === 1'b0) begin     // condition always true (checker no need)
            //   `dut_error("evl_minion_interface", $sformatf("signal my_mh_l2_resp_stall_clear[0] asserted: try to clear my_mh_l2_resp_stall_state[0]=0 but it's already 0"))
            //end           
            my_mh_l2_resp_stall_state[0] <= 1'b0;
         end 
         if (my_mh_l2_resp_stall_clear[1] === 1'b1) begin 
            //if(my_mh_l2_resp_stall_state[1] === 1'b0) begin     // condition always true (checker no need)
            //   `dut_error("evl_minion_interface", $sformatf("signal my_mh_l2_resp_stall_clear[1] asserted: try to clear my_mh_l2_resp_stall_state[1]=0 but it's already 0"))
            //end           
            my_mh_l2_resp_stall_state[1] <= 1'b0;
         end 

         mon_s1_vpu_scp_read_en_d1 <= mon_s1_vpu_scp_read_en;
         mon_s1_vpu_scp_read_en_d2 <= mon_s1_vpu_scp_read_en_d1;

         mon_s2_xcpt_access_or_misalign_fault <= mon_s1_xcpt_access_or_misalign_fault;
         mon_s3_xcpt_access_or_misalign_fault <= mon_s2_xcpt_access_or_misalign_fault;

         mon_tfma_enabled_d1 <= `MIN_IFC_DCACHE.s1_vpu_ctrl.tfma_enabled; 

         my_end_of_sim_d1 <= my_end_of_sim;
      end
   end



`ifdef L1SB_EVL_MINION_ENV
 
  // Request to minion_top L2 evict port
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  mon_l2_dcache_evict_req_ready;
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]  mon_l2_dcache_evict_req_valid;
   wire et_link_minion_evict_req_info_t   mon_l2_dcache_evict_req;
   // Request to minion_top L2 miss port
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]   mon_l2_dcache_miss_req_ready;
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]   mon_l2_dcache_miss_req_valid;
   wire et_link_minion_miss_req_info_t    mon_l2_dcache_miss_req;
   // Response minion_top L2 port
   wire                                   mon_l2_dcache_resp_ready;
   wire                                   mon_l2_dcache_resp_valid;
   wire et_link_minion_rsp_info_t         mon_l2_dcache_resp;

   // DCache PTW request
   wire minion_ptw_req                    mon_dc_ptw_req_data;
   wire                                   mon_dc_ptw_req_valid;
   wire                                   mon_dc_ptw_req_ready;
   // DCache PTW response
   wire minion_ptw_pte                    mon_ptw_dc_resp_data;
   wire                                   mon_ptw_dc_resp_valid;

   assign  mon_l2_dcache_evict_req_ready = minion_top.l2_dcache_evict_req_ready;
   assign  mon_l2_dcache_evict_req_valid = minion_top.l2_dcache_evict_req_valid;
   assign  mon_l2_dcache_evict_req       = minion_top.l2_dcache_evict_req;

   assign  mon_l2_dcache_miss_req_ready  = minion_top.l2_dcache_miss_req_ready;
   assign  mon_l2_dcache_miss_req_valid  = minion_top.l2_dcache_miss_req_valid;
   assign  mon_l2_dcache_miss_req        = minion_top.l2_dcache_miss_req;

   assign  mon_l2_dcache_resp_ready      = minion_top.l2_dcache_resp_ready;
   assign  mon_l2_dcache_resp_valid      = minion_top.l2_dcache_resp_valid;
   assign  mon_l2_dcache_resp            = minion_top.l2_dcache_resp;

   assign mon_dc_ptw_req_ready           = minion_top.dc_ptw_req_ready;
   assign mon_dc_ptw_req_valid           = minion_top.dc_ptw_req_valid;
   assign mon_dc_ptw_req_data            = minion_top.dc_ptw_req_data;
   assign mon_ptw_dc_resp_valid          = minion_top.ptw_dc_resp_valid;
   assign mon_ptw_dc_resp_data           = minion_top.ptw_dc_resp_data;

   tri0 [`EVL_TRANS_ID_MSB:0]        unused_miss_req_trans_id;
 //tri0 [`EVL_TRANS_ID_MSB:0]        unused_miss_tenb_req_trans_id;
 //tri0 [`EVL_TRANS_ID_MSB:0]        unused_miss_tenscp_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]        unused_evict_req_trans_id;


      //
      // L1->L2 Miss requests interface
      //
      evl_etlink_interface #(
                             .AGENT_ID            ( MINION_ID                         ),
                             .USE_BANKS           ( 1                              ),
                             .NUM_BANKS           ( 3                              ),
                             .NUM_UC              ( 0                              ),
                             .ETLINK_COV_TYPE     ( int'(EVL_COV_ETLINK_MINION)    ),
                             .REQ_TYPE            ( et_link_minion_miss_req_info_t ),
                             .RSP_TYPE            ( et_link_minion_rsp_info_t      ),
                             .IGNORE_SRC_DST      ( 1                              ),
                             .IGNORE_MESSAGE      ( 1                              ),
                             .IGNORE_DST_RSP      ( 1                              ),
                             .IGNORED_RSP_IDS     ({ET_LINK_Core_Miss_Id_Reduce,
                                                    ET_LINK_Core_Miss_Id_Cache_Ops,
                                                    ET_LINK_Core_Miss_Id_WriteBack,
                                                    ET_LINK_Core_Miss_Id_Tensor_Store_Coop,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_0,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_1,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_2,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_3
                                                   })
                            )
      m_miss_req (
         .isolate              ( isolate                              ),
         .reset                ( reset                                ),
         .clk                  ( clock                                ),
         .req_valid            ( mon_l2_dcache_miss_req_valid         ),      // ( minion_top.l2_dcache_miss_req_valid  ),
         .req_ready            ( mon_l2_dcache_miss_req_ready         ),      // ( minion_top.l2_dcache_miss_req_ready  ),
         .req_info             ( mon_l2_dcache_miss_req               ),      // ( minion_top.l2_dcache_miss_req        ),
         .req_trans_id         ( unused_miss_req_trans_id             ),
         .rsp_valid            ( mon_l2_dcache_resp_valid             ),      // ( minion_top.l2_dcache_resp_valid      ),
         .rsp_ready            ( mon_l2_dcache_resp_ready             ),      // ( minion_top.l2_dcache_resp_ready      ),
         .rsp_info             ( mon_l2_dcache_resp                   ),      // ( minion_top.l2_dcache_resp            ),
         .use_rsp_source_is_uc ( 1'b0                                 ),
         .rsp_source_is_uc     ( 1'b0                                 )
         );


      //
      // L1->L2 Evict requests (and anything with data) interface
      //
      evl_etlink_interface #(
                             .AGENT_ID            ( MINION_ID                          ),
                             .USE_BANKS           ( 1                               ),
                             .NUM_BANKS           ( 2                               ),
                             .NUM_UC              ( 0                               ),
                             .ETLINK_COV_TYPE     ( int'(EVL_COV_ETLINK_MINION)     ),
                             .REQ_TYPE            ( et_link_minion_evict_req_info_t ),
                             .RSP_TYPE            ( et_link_minion_rsp_info_t       ),
                             .IGNORE_SRC_DST      ( 1                               ),
                             .IGNORE_MESSAGE      ( 0                               ),
                             .IGNORE_DST_RSP      ( 1                               ),
                             .IGNORED_RSP_IDS     ({ET_LINK_Core_Miss_Id_Tbox,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_1,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_1,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_2,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_3,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_4,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_5,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_6,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_7
                                                   })
                            )
      m_evict_req (
         .isolate              ( isolate                               ),
         .reset                ( reset                                 ),
         .clk                  ( clock                                 ),
         .req_valid            ( mon_l2_dcache_evict_req_valid         ),      // ( minion_top.l2_dcache_evict_req_valid  ),
         .req_ready            ( mon_l2_dcache_evict_req_ready         ),      // ( minion_top.l2_dcache_evict_req_ready  ),
         .req_info             ( mon_l2_dcache_evict_req               ),      // ( minion_top.l2_dcache_evict_req        ),
         .req_trans_id         ( unused_evict_req_trans_id             ),
         .rsp_valid            ( mon_l2_dcache_resp_valid              ),      // ( minion_top.l2_dcache_resp_valid      ),
         .rsp_ready            ( mon_l2_dcache_resp_ready              ),      // ( minion_top.l2_dcache_resp_ready      ),
         .rsp_info             ( mon_l2_dcache_resp                    ),      // ( minion_top.l2_dcache_resp            ),
         .use_rsp_source_is_uc ( 1'b0                                  ),
         .rsp_source_is_uc     ( 1'b0                                  )
         );


      //
      // PTW requests interface
      //
      evl_ptw_interface #(
                          .AGENT_ID ( MINION_ID )
                         )
      m_ptw_req (
         .isolate   ( isolate                      ),
         .reset     ( reset                        ),
         .clock     ( clock                        ),
         .req_valid ( mon_dc_ptw_req_valid         ),    // ( minion_top.dc_ptw_req_valid  ),
         .req_ready ( mon_dc_ptw_req_ready         ),    // ( minion_top.dc_ptw_req_ready  ),
         .req_info  ( mon_dc_ptw_req_data          ),    // ( minion_top.dc_ptw_req_data   ),
         .rsp_valid ( mon_ptw_dc_resp_valid        ),    // ( minion_top.ptw_dc_resp_valid ),
         .rsp_info  ( mon_ptw_dc_resp_data         )     // ( minion_top.ptw_dc_resp_data  )
      );


      //
      // Debug APB interface
      //
      wire                                   mon_apb_penable;
      wire                                   mon_apb_psel;
      wire                                   mon_apb_pwrite;
      wire [`MIN_DM_APB_ADDR_WIDTH-1:0]      mon_apb_paddr;
      wire [`ESR_APB_D_WIDTH-1:0]            mon_apb_pwdata;
      wire                                   mon_apb_pready;
      wire [`ESR_APB_D_WIDTH - 1:0]          mon_apb_prdata;
      wire                                   mon_apb_pslverr;

      assign mon_apb_penable          = minion_top.apb_penable;
      assign mon_apb_psel             = minion_top.apb_psel;
      assign mon_apb_pwrite           = minion_top.apb_pwrite;
      assign mon_apb_paddr            = minion_top.apb_paddr;
      assign mon_apb_pwdata           = minion_top.apb_pwdata;
      assign mon_apb_pready           = minion_top.apb_pready;
      assign mon_apb_prdata           = minion_top.apb_prdata;
      assign mon_apb_pslverr          = minion_top.apb_pslverr;

      evl_apb_interface #(.AGENT_ID    ( MINION_ID ),
                          .ADDR_WIDTH  ( `MIN_DM_APB_ADDR_WIDTH ),
                          .DATA_WIDTH  ( `ESR_APB_D_WIDTH       )
                         )
      m_debug_itf (
         .isolate ( isolate                  ),
         .pclk    ( clock                    ),
         .presetn ( ~reset                   ),
         .penable ( mon_apb_penable          ),
         .psel    ( mon_apb_psel             ),
         .pwrite  ( mon_apb_pwrite           ),
         .paddr   ( mon_apb_paddr            ),
         .pwdata  ( mon_apb_pwdata           ),
         .pready  ( mon_apb_pready           ),
         .prdata  ( mon_apb_prdata           ),
         .pslverr ( mon_apb_pslverr          )
      );


`endif  // ifdef L1SB_EVL_MINION_ENV



   //
   // EVL Proxy Code: add this interface instance into evl_rtl_instance_map ... ???
   //
   int m_minion_id;
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), CORE_RTL_AGENT, MINION_ID, "evl_minion_env", "", "", "", { 1 });

   evl_minion_rtl_port m_rtl_port = new($sformatf("%m"), MINION_ID, { 1 });


   evl_minion_packet_u pending_cop_packet = 0;
   evl_minion_packet_u pending_tload_0_packet = 0;     // TL_L1SCP
   evl_minion_packet_u pending_tload_1_packet = 0;     // TL_TenB

   bit [63:0] validation1_reg;
   //
   // This small block of code is used to prevent any evaluation at time 0.  Because an x->1
   // transition is considered a positive edge in Verilog, this block forces us to see a ->0
   // transition first (this only applies if the clock is asserted at time 0).
   //
   initial begin
      @(negedge clock);
      m_minion_id = MINION_ID;
      start_monitor <= 1'b1;
   end

   //      CK      RST    DOUT                   DIN                 DEF
   `RST_FF(clock,  reset, mon_csr_wb_addr,       mon_csr_mem_addr,   1'b0)

   // Delay meta data write until S2 as this is where the model does the state lookup for requests
   `RST_FF(clock,  reset, mon_metadata_wen_s2,   mon_metadata_wen,   1'b0)
   `RST_FF(clock,  reset, mon_metadata_s2,       mon_metadata,       1'b0)
   `RST_FF(clock,  reset, mon_mw_thread_s2,      mon_mw_thread,      1'b0)

   // Forward fence to exe stage (S0 in dcache)
   `RST_FF(clock,  reset, mon_ex_fence, mon_id_fence, 1'b0)

   // Forward S0 valid to S1 and S2 to mask kill signals
   `RST_FF(clock,  reset, mon_s1_core_req_valid,  mon_s0_core_req_valid,   1'b0)
   `RST_FF(clock,  reset, mon_s2_core_req_valid,  mon_s1_core_req_valid,   1'b0)


   bit                               m_ex_tima_valid_unqual_r;
   bit                               m_ex_txfma_valid_all_r;
   bit                               m_ex_ctrl_valid_qual_r;
   bit [`VPU_FCMD_SZ-1:0]            m_ex_vpu_ctrl_cmd_r;
   bit [`VPU_CTRL_SIGS_DTYPE_SZ-1:0] m_ex_vpu_ctrl_dtype_r;
   bit                               m_vpu_ctrl_ml_tensorfma_enabled_r;
   bit                               m_vpu_ctrl_ml_tensorquant_enabled_r;
   bit                               m_virus70_window_start_r;             
   bit                               m_virus70_window_end_r;               
   bit                               m_sparse_nn_mlp_window_start_r;       
   bit                               m_sparse_nn_mlp_window_end_r;         
   bit                               m_sparse_nn_emb_window_start_r;
   bit                               m_sparse_nn_emb_window_end_r;  


   //
   // Monitor the signals here and send appropriate info to the RTL port proxy.
   //
   initial begin
      #1ns;
      m_ex_tima_valid_unqual_r              <= 1'b0;
      m_ex_txfma_valid_all_r                <= 1'b0;
      m_ex_ctrl_valid_qual_r                <= 1'b0;
      m_ex_vpu_ctrl_cmd_r                   <= `MIN_VPU_CTRL.ex_vpu_ctrl.sigs.cmd;
      m_ex_vpu_ctrl_dtype_r                 <= `MIN_VPU_CTRL.ex_vpu_ctrl.sigs.dtype;
      m_vpu_ctrl_ml_tensorfma_enabled_r     <= 1'b0;
      m_vpu_ctrl_ml_tensorquant_enabled_r   <= 1'b0;
      m_virus70_window_start_r              <= 1'b0;             
      m_virus70_window_end_r                <= 1'b0;               
      m_sparse_nn_mlp_window_start_r        <= 1'b0;       
      m_sparse_nn_mlp_window_end_r          <= 1'b0;        
      m_sparse_nn_emb_window_start_r <= 1'b0;
      m_sparse_nn_emb_window_end_r   <= 1'b0;  

      m_rtl_port.set_int_param(pk_ex_tima_valid_unqual, 0);
      m_rtl_port.set_int_param(pk_ex_txfma_valid_all, 0);
      m_rtl_port.set_int_param(pk_ex_vpu_ctrl_valid_qual, 0);
      m_rtl_port.set_int_param(pk_ex_vpu_ctrl_cmd, -1);
      m_rtl_port.set_int_param(pk_ex_vpu_ctrl_dtype, -1);
      m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorfma_enabled, 0);
      m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorquant_enabled, 0);
      m_rtl_port.set_int_param(pk_virus70_window_start, 0);
      m_rtl_port.set_int_param(pk_virus70_window_end, 0);
      m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_start, 0);
      m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_end, 0);
      m_rtl_port.set_int_param(pk_sparse_nn_emb_window_start, 0);
      m_rtl_port.set_int_param(pk_sparse_nn_emb_window_end, 0);

      forever begin
         #0 wait ((isolate === 1'b1) && (mon_power_off === 1'b0));
         m_rtl_port.power_off();
         mon_power_off = 1'b1;
         mon_power_on  = 1'b0;
         #0 wait (isolate !== 1'b1);
      end
   end

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b0) && (mon_power_on === 1'b0));
         m_rtl_port.power_on();
         mon_power_on  = 1'b1;
         mon_power_off = 1'b0;
         #0 wait (isolate !== 1'b0);
      end
   end

   always @(posedge clock) begin
      if (start_monitor === 1'b1) begin
         if ((reset === 1'b1) && (mon_reset_on === 1'b0)) begin
            m_ex_tima_valid_unqual_r              <= 1'b0;
            m_ex_txfma_valid_all_r                <= 1'b0;
            m_ex_ctrl_valid_qual_r                <= 1'b0;
            m_virus70_window_start_r              <= 1'b0;             
            m_virus70_window_end_r                <= 1'b0;               
            m_sparse_nn_mlp_window_start_r        <= 1'b0;       
            m_sparse_nn_mlp_window_end_r          <= 1'b0;        
            m_sparse_nn_emb_window_start_r        <= 1'b0;
            m_sparse_nn_emb_window_end_r          <= 1'b0;  

            m_rtl_port.set_int_param(pk_ex_tima_valid_unqual, 0);
            m_rtl_port.set_int_param(pk_ex_txfma_valid_all, 0);
            m_rtl_port.set_int_param(pk_ex_vpu_ctrl_valid_qual, 0);
            m_rtl_port.set_int_param(pk_ex_vpu_ctrl_cmd, -1);
            m_rtl_port.set_int_param(pk_ex_vpu_ctrl_dtype, -1);
            m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorfma_enabled, 0);
            m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorquant_enabled, 0);
            m_rtl_port.set_int_param(pk_virus70_window_start, 0);
            m_rtl_port.set_int_param(pk_virus70_window_end, 0);
            m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_start, 0);
            m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_end, 0);
            m_rtl_port.set_int_param(pk_sparse_nn_emb_window_start, 0);
            m_rtl_port.set_int_param(pk_sparse_nn_emb_window_end, 0);

            m_rtl_port.assert_reset();
         end
         else if ((reset === 1'b0) && (mon_reset_off === 1'b0)) begin
            m_rtl_port.deassert_reset();
         end
         mon_reset_on  <= ~reset;
         mon_reset_off <= reset;

         //
         // Monitor thread events
         //
         if ((reset === 1'b0) && (isolate !== 1'b1)) begin
            // Enabling or disabling threads
            for (int i = 0; i < `CORE_NR_THREADS; i++) begin
               if ((mon_thread_enabled[i] === 1'b0) && (minion_top.enabled[i] === 1'b1)) begin
                  m_rtl_port.enable_thread(i);
               end
               if ((mon_thread_enabled[i] === 1'b1) && (minion_top.enabled[i] === 1'b0)) begin
                  m_rtl_port.disable_thread(i);
               end
            end
            mon_thread_enabled <= minion_top.enabled;

            if (mon_csr_wb_cmd === csr_cmd_W) begin
               case (mon_csr_wb_addr)

                  `REG_VALIDATION0: begin // Test flow control
                     case (mon_csr_wb_wdata[31:0])
                        `EVL_THREAD_BEGIN      : m_rtl_port.thread_begin(mon_csr_wb_thread_id);
                        `EVL_THREAD_BOOT_BEGIN : m_rtl_port.thread_boot_begin(mon_csr_wb_thread_id);
                        `EVL_THREAD_BOOT_END   : m_rtl_port.thread_boot_end(mon_csr_wb_thread_id);
                        `EVL_THREAD_PASS       : m_rtl_port.thread_pass(mon_csr_wb_thread_id);
                        `EVL_THREAD_FAIL       : m_rtl_port.thread_fail(mon_csr_wb_thread_id);
                        default:;
                     endcase
                  end
                  `REG_VALIDATION1: begin // Send various commands
                     validation1_reg = m_rtl_port.process_diag_cmd(mon_csr_wb_thread_id, mon_csr_wb_wdata);
                     drive_validation1_reg(validation1_reg,  mon_csr_wb_thread_id);
                  end
                  `REG_VALIDATION2: begin // Additional info for commands
                     m_rtl_port.set_val2(mon_csr_wb_thread_id, mon_csr_wb_wdata);
                  end
               endcase
            end
            if (testend_mon.wb_inst === `EVL_THREAD_BOOT_BEGIN_HINT && `MIN_IFC_INTPIPE.wb_valid) begin
               m_rtl_port.thread_boot_begin(`MIN_IFC_INTPIPE.wb_thread_id);
            end
            if (testend_mon.wb_inst === `EVL_THREAD_BOOT_END_HINT && `MIN_IFC_INTPIPE.wb_valid) begin
               m_rtl_port.thread_boot_end(`MIN_IFC_INTPIPE.wb_thread_id);
            end
            if (testend_mon.wb_inst === `EVL_CORE_RESET_W_HINT && `MIN_IFC_INTPIPE.wb_valid) begin
               reset_w_core();
            end

            //
            // VPU signals for power extraction support.
            //
            if ((`MIN_VPU_CTRL.ex_tima_valid_unqual === 1'b1) && (m_ex_tima_valid_unqual_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_ex_tima_valid_unqual, 1);
            end
            else if ((`MIN_VPU_CTRL.ex_tima_valid_unqual === 1'b0) && (m_ex_tima_valid_unqual_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_ex_tima_valid_unqual, 0);
            end

            if ((`MIN_VPU_CTRL.ex_txfma_valid_all === 1'b1) && (m_ex_txfma_valid_all_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_ex_txfma_valid_all, 1);
            end
            else if ((`MIN_VPU_CTRL.ex_txfma_valid_all === 1'b0) && (m_ex_txfma_valid_all_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_ex_txfma_valid_all, 0);
            end

            if (`MIN_VPU_CTRL.ex_ctrl_valid_qual === 1'b1) begin
               if ( (m_ex_ctrl_valid_qual_r === 1'b0) ||
                    (`MIN_VPU_CTRL.ex_vpu_ctrl.sigs.cmd   !== m_ex_vpu_ctrl_cmd_r) ||
                    (`MIN_VPU_CTRL.ex_vpu_ctrl.sigs.dtype !== m_ex_vpu_ctrl_dtype_r) ) begin
                  m_rtl_port.set_int_param(pk_ex_vpu_ctrl_cmd, int'(`MIN_VPU_CTRL.ex_vpu_ctrl.sigs.cmd));
                  m_rtl_port.set_int_param(pk_ex_vpu_ctrl_dtype, int'(`MIN_VPU_CTRL.ex_vpu_ctrl.sigs.dtype));
                  m_rtl_port.set_int_param(pk_ex_vpu_ctrl_valid_qual, 1);
               end
            end
            else if (m_ex_ctrl_valid_qual_r === 1'b1) begin
               m_rtl_port.set_int_param(pk_ex_vpu_ctrl_valid_qual, 0);
               m_rtl_port.set_int_param(pk_ex_vpu_ctrl_cmd, -1);
               m_rtl_port.set_int_param(pk_ex_vpu_ctrl_dtype, -1);
            end

            if ((`MIN_VPU_CTRL.ml.tensorfma.enabled === 1'b1) && (m_vpu_ctrl_ml_tensorfma_enabled_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorfma_enabled, 1);
            end
            else if ((`MIN_VPU_CTRL.ml.tensorfma.enabled === 1'b0) && (m_vpu_ctrl_ml_tensorfma_enabled_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorfma_enabled, 0);
            end

            if ((`MIN_VPU_CTRL.ml.tensorquant.enabled === 1'b1) && (m_vpu_ctrl_ml_tensorquant_enabled_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorquant_enabled, 1);
            end
            else if ((`MIN_VPU_CTRL.ml.tensorquant.enabled === 1'b0) && (m_vpu_ctrl_ml_tensorquant_enabled_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_vpu_ctrl_ml_tensorquant_enabled, 0);
            end

            m_ex_tima_valid_unqual_r              <= `MIN_VPU_CTRL.ex_tima_valid_unqual;
            m_ex_txfma_valid_all_r                <= `MIN_VPU_CTRL.ex_txfma_valid_all;
            m_ex_ctrl_valid_qual_r                <= `MIN_VPU_CTRL.ex_ctrl_valid_qual;
            m_ex_vpu_ctrl_cmd_r                   <= `MIN_VPU_CTRL.ex_vpu_ctrl.sigs.cmd;
            m_ex_vpu_ctrl_dtype_r                 <= `MIN_VPU_CTRL.ex_vpu_ctrl.sigs.dtype;
            m_vpu_ctrl_ml_tensorfma_enabled_r     <= `MIN_VPU_CTRL.ml.tensorfma.enabled;
            m_vpu_ctrl_ml_tensorquant_enabled_r   <= `MIN_VPU_CTRL.ml.tensorquant.enabled;


`ifdef ENABLE_BEMU 
            if ((`ARCH_STATE_MON.virus70_window_start === 1'b1) && (m_virus70_window_start_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_virus70_window_start, 1);
            end
            else if ((`ARCH_STATE_MON.virus70_window_start === 1'b0) && (m_virus70_window_start_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_virus70_window_start, 0);
            end
           
            if ((`ARCH_STATE_MON.virus70_window_end === 1'b1) && (m_virus70_window_end_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_virus70_window_end, 1);
            end
            else if ((`ARCH_STATE_MON.virus70_window_end === 1'b0) && (m_virus70_window_end_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_virus70_window_end, 0);
            end
            
            if ((`ARCH_STATE_MON.sparse_nn_mlp_window_start === 1'b1) && (m_sparse_nn_mlp_window_start_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_start, 1);
            end
            else if ((`ARCH_STATE_MON.sparse_nn_mlp_window_start === 1'b0) && (m_sparse_nn_mlp_window_start_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_start, 0);
            end

            if ((`ARCH_STATE_MON.sparse_nn_mlp_window_end === 1'b1) && (m_sparse_nn_mlp_window_end_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_end, 1);
            end
            else if ((`ARCH_STATE_MON.sparse_nn_mlp_window_end === 1'b0) && (m_sparse_nn_mlp_window_end_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_mlp_window_end, 0);
            end

            if ((`ARCH_STATE_MON.sparse_nn_emb_window_start === 1'b1) && (m_sparse_nn_emb_window_start_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_emb_window_start, 1);
            end
            else if ((`ARCH_STATE_MON.sparse_nn_emb_window_start === 1'b0) && (m_sparse_nn_emb_window_start_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_emb_window_start, 0);
            end

            if ((`ARCH_STATE_MON.sparse_nn_emb_window_end === 1'b1) && (m_sparse_nn_emb_window_end_r === 1'b0)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_emb_window_end, 1);
            end
            else if ((`ARCH_STATE_MON.sparse_nn_emb_window_end === 1'b0) && (m_sparse_nn_emb_window_end_r === 1'b1)) begin
               m_rtl_port.set_int_param(pk_sparse_nn_emb_window_end, 0);
            end

            m_virus70_window_start_r              <= `ARCH_STATE_MON.virus70_window_start;             
            m_virus70_window_end_r                <= `ARCH_STATE_MON.virus70_window_end;  
            m_sparse_nn_mlp_window_start_r        <= `ARCH_STATE_MON.sparse_nn_mlp_window_start;       
            m_sparse_nn_mlp_window_end_r          <= `ARCH_STATE_MON.sparse_nn_mlp_window_end;        
            m_sparse_nn_emb_window_start_r        <= `ARCH_STATE_MON.sparse_nn_emb_window_start;
            m_sparse_nn_emb_window_end_r          <= `ARCH_STATE_MON.sparse_nn_emb_window_end;  
`endif // ifdef ENABLE_BEMU         
         end

         //
         // Monitor signals and generate events
         // !-- Do not reorder events --!
         //
         if ((reset === 1'b0) && (isolate !== 1'b1)) begin
            if (redir0_seen === 1)
               redir0_to_redir1_cycles = redir0_to_redir1_cycles + 1 ;

            if (fe_mon_f0_core_req_valid[0] === 1'b1) begin
               redir0_seen = 1;
               redir0_to_redir1_cycles = 0;
            end

            if (fe_mon_f0_core_req_valid[1] === 1'b1) begin
               redir0_seen = 0;
               redir0_to_redir1_cycles = 0;
            end

            if (replay_inst_t0 === 1'b1 && fe_mon_f0_core_req_valid[0] === 1'b1 && fe_mon_wb_reg_replay === 1'b1)
               replay_inst_t0 = 0;

            if (replay_inst_t1 === 1'b1 && fe_mon_f0_core_req_valid[1] === 1'b1 && fe_mon_wb_reg_replay === 1'b1)
               replay_inst_t1 = 0;

            if (fe_mon_id_inst_ready === 0)
               inst_ready_lo_cycles = inst_ready_lo_cycles + 1;

            if (fe_mon_id_inst_ready === 1)
               inst_ready_lo_cycles = 0;

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && fe_mon_id_inst_data.replay === 0) begin
               uncacheable_inst_t0 = 0;
               uncacheable_inst_t1 = 0;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && fe_mon_id_inst_data.replay === 1) begin
               uncacheable_inst_t0 = 1;
               replay_inst_t0 = 1;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && fe_mon_id_inst_data.replay === 0) begin
               uncacheable_inst_t1 = 0;
               uncacheable_inst_t0 = 0;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && fe_mon_id_inst_data.replay === 1) begin
               uncacheable_inst_t1 = 1;
               replay_inst_t1 = 1;
            end


            if (icache_request_made === 1)
               icache_req_to_redir_cycles = icache_req_to_redir_cycles + 1 ;

            if (fe_mon_f0_core_req_valid[0] === 1'b1 || fe_mon_f0_core_req_valid[1] === 1'b1 ) begin
               icache_request_made = 0;
               icache_req_to_redir_cycles = 0;
            end

            if (f1_icache_req_valid === 1 && f1_icache_req_ready === 0) begin
               icache_request_made = 0;
               icache_req_to_redir_cycles = 0;
            end
            if (f1_icache_req_valid === 1 && f1_icache_req_ready === 1) begin
               icache_request_made = 1;
               icache_req_to_redir_cycles = 0;
            end

            t0_f7_bypass = t0_f6_bypass;
            t1_f7_bypass = t1_f6_bypass;

            if (f5_icache_resp_valid === 1'b0 && icache_ready_seen === 1'b1)
               icache_req_valid_to_resp_cycles = icache_req_valid_to_resp_cycles + 1 ;

            if (f5_icache_resp_valid === 1'b1 && icache_ready_seen === 1'b1) begin
               icache_ready_seen = 0;
               icache_req_valid_to_resp_cycles = 0;
            end

            if (f1_icache_req_valid === 0)
               icache_req_valid_hi_cycles = 0;

            if (f1_icache_req_valid === 1)
               icache_req_valid_hi_cycles = icache_req_valid_hi_cycles + 1;

            if (f1_icache_req_ready === 1) begin
               icache_req_valid_hi_cycles = 0;
               icache_ready_seen = 1;
               icache_req_valid_to_resp_cycles = 0;
            end

            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp_miss === 1'b1)
               f5_icache_resp_miss_seen = 1'b1;
            if (f6_icache_fill_done === 1'b1)
               f5_icache_resp_miss_seen = 1'b0;
            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp.cacheable === 1'b1)begin
               prev_icache_resp_was_cacheable = 1'b1;
               prev_icache_resp_was_uncacheable = 1'b0;
            end
            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp.cacheable === 1'b0)begin
               prev_icache_resp_was_cacheable = 1'b0;
               prev_icache_resp_was_uncacheable = 1'b1;
            end

            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 0 && first_t0_arb_req === 1'b1) begin
               consecutive_t0_arb_req = consecutive_t0_arb_req + 1;
            end
            if (t0_f0_icache_req_valid === 0 &&  t1_f0_icache_req_valid === 1 && first_t1_arb_req === 1'b1) begin
               consecutive_t1_arb_req = consecutive_t1_arb_req + 1;
            end

            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 0 && first_t0_arb_req === 1'b0) begin
               first_t0_arb_req = 1'b1;
               first_t1_arb_req = 1'b0;
               consecutive_t0_arb_req = 0;
               consecutive_t1_arb_req = 0;
            end
            if (t0_f0_icache_req_valid === 0 &&  t1_f0_icache_req_valid === 1 && first_t1_arb_req === 1'b0) begin
               first_t1_arb_req = 1'b1;
               first_t0_arb_req = 1'b0;
               consecutive_t1_arb_req = 0;
               consecutive_t0_arb_req = 0;
            end
            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 1) begin
               first_t1_arb_req = 1'b0;
               first_t0_arb_req = 1'b0;
               consecutive_t0_arb_req = 0;
               consecutive_t1_arb_req = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && first_t0_inst === 1'b1) begin
               consecutive_t0_inst = consecutive_t0_inst + 1;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && first_t1_inst === 1'b1) begin
               consecutive_t1_inst = consecutive_t1_inst + 1;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && first_t0_inst === 1'b0) begin
               first_t0_inst = 1'b1;
               first_t1_inst = 1'b0;
               consecutive_t0_inst = 0;
               consecutive_t1_inst = 0;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && first_t1_inst == 1'b0) begin
               first_t1_inst = 1'b1;
               first_t0_inst = 1'b0;
               consecutive_t0_inst = 0;
               consecutive_t1_inst = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 0) begin
               t0_inst_from_bypass = 0;
               t1_inst_from_bypass = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 1) begin
               t0_inst_from_bypass = 1;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 0) begin
               t0_inst_from_bypass = 0;
               t1_inst_from_bypass = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 1) begin
               t1_inst_from_bypass = 1;
            end


            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 0 && fe_mon_id_inst_data.replay === 0) begin
               t0_inst_uc_from_bypass = 0;
               t1_inst_uc_from_bypass = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 1 && fe_mon_id_inst_data.replay === 1) begin
               t0_inst_uc_from_bypass = 1;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 0 && fe_mon_id_inst_data.replay === 0) begin
               t0_inst_uc_from_bypass = 0;
               t1_inst_uc_from_bypass = 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 1 && fe_mon_id_inst_data.replay === 1) begin
               t1_inst_uc_from_bypass = 1;
            end


            // Stall happened in Core/FrontEnd
            if (stall_mon_id_ctrl_stall === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.stall_packet.info.id_ctrl_stall            = stall_mon_id_ctrl_stall;
               pkt.stall_packet.info.id_ctrl_stall_hazard     = stall_mon_id_ctrl_stall_hazard;
               pkt.stall_packet.info.id_ctrl_stall_hazard_vpu = stall_mon_id_ctrl_stall_hazard_vpu;
               pkt.stall_packet.info.id_int_sboard_hazard     = stall_mon_id_int_sboard_hazard;
               pkt.stall_packet.info.id_ctrl_stall_div        = stall_mon_id_ctrl_stall_div;
               pkt.stall_packet.info.id_ctrl_stall_trans      = stall_mon_id_ctrl_stall_trans;
               pkt.stall_packet.info.id_do_fence              = stall_mon_id_do_fence;
               pkt.stall_packet.info.id_ctrl_stall_dcache     = stall_mon_id_ctrl_stall_dcache;
               pkt.stall_packet.info.id_ctrl_stall_csr        = stall_mon_id_ctrl_stall_csr;
               pkt.stall_packet.info.csr_fe_stall             = stall_mon_csr_fe_stall;
               pkt.stall_packet.info.ex_gsc_busy              = stall_mon_ex_gsc_busy;
               pkt.stall_packet.info.id_stall_fp2int          = stall_mon_id_stall_fp2int;
               pkt.stall_packet.info.buffer_empty_t0          = stall_mon_buffer_empty_t0;
               pkt.stall_packet.info.f6_stall_t0              = stall_mon_f6_stall_t0;
               pkt.stall_packet.info.buffer_empty_t1          = stall_mon_buffer_empty_t1;
               pkt.stall_packet.info.f6_stall_t1              = stall_mon_f6_stall_t1;
               pkt.stall_packet.info.inst_fifo_full           = stall_mon_inst_fifo_full;
               m_rtl_port.send_event_fe(MINION_STALL_FE_INT, pkt);
            end


            // FrontEnd sending opcode to INTPIPE
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 ) begin
               evl_minion_packet_u pkt;
               pkt.opcode_packet.info.fe_inst_data =  fe_mon_id_inst_data;
               pkt.opcode_packet.info.fe_thread_id =  fe_mon_id_inst_thread_id;
               m_rtl_port.send_event_fe(MINION_OPC_FE_INT, pkt);
            end

            // FronEnd generates access fault
            if (fe_mon_id_inst_data.af0 ===1'b1 || fe_mon_id_inst_data.af1 === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.opcode_packet.info.fe_inst_data = fe_mon_id_inst_data;
               pkt.opcode_packet.info.fe_thread_id = fe_mon_id_inst_thread_id;
               m_rtl_port.send_event_fe(MINION_FE_ACCESS_FAULT, pkt);

            end




            // INTPIPE sending redirect to FrontEnd
            if ((fe_mon_f0_core_req_valid[0] === 1'b1 && core_in_debug_mode[0] === 1'b0) || (fe_mon_f0_core_req_valid[1] === 1'b1 && core_in_debug_mode[1] === 1'b0)) begin
               evl_minion_packet_u pkt;
               pkt.redirect_packet.info.fe_redirect_info   =  fe_mon_f0_core_req;
               pkt.redirect_packet.info.fe_redirect_thread =  fe_mon_f0_core_req_valid;
               pkt.redirect_packet.info.fe_redirect_ipi    =  fe_mon_ipi_with_pc;
               pkt.redirect_packet.info.fe_redirect_wb_xcpt       =  fe_mon_wb_xcpt;
               pkt.redirect_packet.info.fe_redirect_wb_csr_eret   =  fe_mon_wb_csr_eret;
               pkt.redirect_packet.info.fe_redirect_wb_csr_replay =  fe_mon_wb_csr_replay;
               pkt.redirect_packet.info.fe_redirect_wb_reg_replay =  fe_mon_wb_reg_replay;
               m_rtl_port.send_event_fe(MINION_REDIR_INT_FE, pkt);
            end

            if (icache_req_valid_hi_cycles >= 15 || icache_req_valid_to_resp_cycles > 3 ) begin
               evl_minion_packet_u pkt;
               pkt.redirect_packet.info.icache_req_valid_hi_cycles  =  icache_req_valid_hi_cycles;
               pkt.redirect_packet.info.icache_req_valid_to_resp_cycles  =  icache_req_valid_to_resp_cycles;
               m_rtl_port.send_event_fe(MINION_ICACHE_REQ_VALID_FE, pkt);
            end

            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp.cacheable === 1'b0 && !f5_icache_resp_miss)begin
               evl_minion_packet_u pkt;
               m_rtl_port.send_event_fe(MINION_UNCACHEABLE_FE, pkt);
            end


            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && (replay_inst_t0 === 1'b1  || replay_inst_t1 === 1'b1) ) begin
               evl_minion_packet_u pkt;
               pkt.redirect_packet.info.t0_replay_no_redir  =  replay_inst_t0;
               pkt.redirect_packet.info.t1_replay_no_redir  =  replay_inst_t1;
               m_rtl_port.send_event_fe(MINION_UC_REPLAY, pkt);
            end

            //
            // Monitor for writes to PMU Event CSRs
            //
            mhpmevent_en   <= '{{`MIN_IFC_INTPIPE.csr_file.reg_mhpmevent8_en[1],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent7_en[1],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent6_en[1],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent5_en[1],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent4_en[1],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent3_en[1]},
                                {`MIN_IFC_INTPIPE.csr_file.reg_mhpmevent8_en[0],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent7_en[0],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent6_en[0],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent5_en[0],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent4_en[0],
                                 `MIN_IFC_INTPIPE.csr_file.reg_mhpmevent3_en[0]}};


            for (int ii = 0; ii < `CORE_NR_THREADS; ii = ii + 1) begin
               for (int jj = 3; jj < 7; jj = jj + 1) begin
                  if (mhpmevent_en[ii][jj] === 1'b1) begin
                     evl_minion_packet_u pkt;
                     pkt.pmu_packet.info.pmu_thread_id = ii;
                     pkt.pmu_packet.info.pmu_cntr_id = jj;
                     pkt.pmu_packet.info.pmu_event_type = mhpmevent[ii][jj];
                     m_rtl_port.send_event_pmu(MINION_PMU_EVENT_WR, pkt);
                     if ((jj >= 3) && (jj <= 6)) begin
                        mon_mhpmevent[ii][jj] <= mhpmevent[ii][jj];
                     end
                  end
               end
            end


            //
            // Reads From PMU Counters
            //
            if ((mon_csr_wb_cmd === csr_cmd_R) && (`MIN_IFC_INTPIPE.csr_file.pmu_read_mem_ready[mon_csr_wb_thread_id])) begin
               evl_minion_packet_u  pkt;
               logic [3:0]          cntr_id;
               case (mon_csr_wb_addr)
                  csr_ad_HPMCOUNTER3, csr_ad_MHPMCOUNTER3 : begin
                                                             cntr_id = 3;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
                  csr_ad_HPMCOUNTER4, csr_ad_MHPMCOUNTER4 : begin
                                                             cntr_id = 4;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
                  csr_ad_HPMCOUNTER5, csr_ad_MHPMCOUNTER5 : begin
                                                             cntr_id = 5;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
                  csr_ad_HPMCOUNTER6, csr_ad_MHPMCOUNTER6 : begin
                                                             cntr_id = 6;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
                  csr_ad_HPMCOUNTER7, csr_ad_MHPMCOUNTER7 : begin
                                                             cntr_id = 7;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
                  csr_ad_HPMCOUNTER8, csr_ad_MHPMCOUNTER8 : begin
                                                             cntr_id = 8;
                                                             pkt.pmu_packet.info.pmu_thread_id = mon_csr_wb_thread_id;
                                                             pkt.pmu_packet.info.pmu_cntr_id = cntr_id;
                                                             pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_read_data[mon_csr_wb_thread_id];
                                                             m_rtl_port.send_event_pmu(MINION_PMU_CNTR_RD, pkt);
                                                            end
               endcase
            end



            pmu_event_seen_st3 <= pmu_event_seen_st2;
            pmu_event_seen_st2 <= pmu_event_seen_st1;

            pmu_event_type_st3 <= pmu_event_type_st2;
            pmu_event_type_st2 <= pmu_event_type_st1;
            pmu_event_type_st1 <= mon_mhpmevent;


            //
            // Monitor PMU Events
            //
            for (int ii = 0; ii < `CORE_NR_THREADS; ii = ii + 1) begin
               for (int jj = 3; jj < 7; jj = jj + 1) begin
                  case (mon_mhpmevent[ii][jj])
                     pmu_event_MCYCLES          :  begin    //= 5'd1,  // Mcycles 
                                                      pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                   end
                     pmu_event_RETIRED_INST0    :  begin    //= 5'd2,  // Retired Inst TH0
                                                      if ((`MIN_IFC_INTPIPE.wb_valid === 1'b1) && (`MIN_IFC_INTPIPE.wb_thread_id === 1'b0)) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_RETIRED_INST1    :  begin    //= 5'd3,  // Retired Inst TH1
                                                      if ((`MIN_IFC_INTPIPE.wb_valid === 1'b1) && (`MIN_IFC_INTPIPE.wb_thread_id === 1'b1)) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_BRANCHES0        :  begin    //= 5'd4,  // Branches taken TH0
                                                   end
                     pmu_event_BRANCHES1        :  begin    //= 5'd5,  // Branches taken TH1
                                                   end

                     pmu_event_DCACHE_ACCESS0   :  begin    //= 5'd6,  // Dcache Accesses TH0
                                                   end
                     pmu_event_DCACHE_ACCESS1   :  begin    //= 5'd7,  // Dcache Accesses TH1
                                                   end
                     pmu_event_DCACHE_MISSES0   :  begin    //= 5'd8,  // Dcache Misses TH0
                                                   end
                     pmu_event_DCACHE_MISSES1   :  begin    //= 5'd9,  // Dcache Misses TH1
                                                   end
                     pmu_event_L2_MISS_REQ      :  begin    //= 5'd10, // L2 Miss requests sent
                                                      if ((mon_l2_miss_req_valid[0] && mon_l2_miss_req_ready[0]) || 
                                                          (mon_l2_miss_req_valid[1] && mon_l2_miss_req_ready[1]) || 
                                                          (mon_l2_miss_req_valid[2] && mon_l2_miss_req_ready[2])) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_L2_MISS_REQ_REJ  :  begin    //= 5'd11, // L2 Miss requests rejected
                                                      if ((mon_l2_miss_req_valid[0] && !mon_l2_miss_req_ready[0]) || 
                                                          (mon_l2_miss_req_valid[1] && !mon_l2_miss_req_ready[1]) || 
                                                          (mon_l2_miss_req_valid[2] && !mon_l2_miss_req_ready[2])) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_L2_EVICT_REQ     :  begin    //= 5'd12, // L2 Evict requests sent
                                                      if ((mon_l2_evict_req_valid[0] && mon_l2_evict_req_ready[0]) || 
                                                          (mon_l2_evict_req_valid[1] && mon_l2_evict_req_ready[1])) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_L2_EVICT_REQ_REJ :  begin    //= 5'd13, // L2 Evict requests rejected
                                                      if ((mon_l2_evict_req_valid[0] && !mon_l2_evict_req_ready[0]) || 
                                                          (mon_l2_evict_req_valid[1] && !mon_l2_evict_req_ready[1])) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TL_INST          :  begin    //= 5'd14, // Tensor Load Instruction
                                                      if (`MIN_IFC_INTPIPE.csr_file.dcache_ctrl.tensorload_start) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TL_OPS           :  begin    //= 5'd15, // Tensor Load Operation
                                                      if (`MIN_IFC_DCACHE.tensor_load_0.tl_op || 
                                                          `MIN_IFC_DCACHE.tensor_load_1.tl_op) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TS_INST          :  begin    //= 5'd16, // Tensor Store Instruction
                                                      if (`MIN_IFC_INTPIPE.csr_file.dcache_ctrl.tensorstore_start) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TS_OPS           :  begin    //= 5'd17, // Tensor Store Operation
                                                      if (`MIN_IFC_DCACHE.ts_op) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end

                     pmu_event_TFMA_WAIT_TENB   :  begin    //= 5'd18, // Tensor FMA waiting TENB
                                                      if (`MIN_VPU_CTRL.ml.tensorfma.io_events_wait_tenb) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TIMA_OPS         :  begin    //= 5'd19, // TIMA Operation
                                                      if (|`MIN_VPU_CTRL.ex_tima_valid) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TXFMA_3216_OPS   :  begin    //= 5'd20, // TXFMA Operation fp16a32
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && (`MIN_VPU_CTRL.f8_ctrl_sigs.dtype == `VPU_DTYPE_F16_F32) && `MIN_VPU_CTRL.f8_ctrl_sigs.txfma) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TXFMA_32_OPS     :  begin    //= 5'd21, // TXFMA Operation fp32
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && `MIN_VPU_CTRL.f8_ctrl_sigs.txfma && !`MIN_VPU_CTRL.f8_ctrl_sigs.trans) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TXFMA_INT_OPS    :  begin    //= 5'd22, // TXFMA Operation int
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && (`MIN_VPU_CTRL.f8_ctrl_sigs.dtype == `VPU_DTYPE_INT) && `MIN_VPU_CTRL.f8_ctrl_sigs.txfma) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TRANS_OPS        :  begin    //= 5'd23, // TRANS Operation
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && `MIN_VPU_CTRL.f8_trans_wen_ctrl_en && !`MIN_VPU_CTRL.f8_ctrl_sigs.trans) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_SHORT_OPS        :  begin    //= 5'd24, // SHORT Operation
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && `MIN_VPU_CTRL.f8_ctrl_sigs.shsw) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_MASK_OPS         :  begin    //= 5'd25, // MASK Operation
                                                      if (`MIN_VPU_CTRL.f8_ctrl_valid_qual && `MIN_VPU_CTRL.f8_ctrl_sigs.maskop) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TFMA_INST        :  begin    //= 5'd26, // Tensor Fma Instruction
                                                      if (`MIN_IFC_INTPIPE.csr_file.tensor_fma_wen_next) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TREDUCE_INST     :  begin    //= 5'd27, // Tensor Reduce Instruction
                                                      if (`MIN_IFC_INTPIPE.csr_file.vpu_ctrl.reduce_start) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     pmu_event_TQUANT_INST      :  begin    //= 5'd28, // Tensor Quant Instruction
                                                      if ((`MIN_IFC_INTPIPE.csr_file.reg_tensor_quant_en) && (!`MIN_IFC_INTPIPE.csr_file.tensor_quant_replay_condition)) begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b1;
                                                      end
                                                      else begin
                                                         pmu_event_seen_st1[ii][jj]  <= 1'b0;
                                                      end
                                                   end
                     default                    :  pmu_event_seen_st1[ii][jj]  <= 1'b0;
                  endcase

                  if (pmu_event_seen_st3[ii][jj]) begin
                     evl_minion_packet_u pkt;
                     pkt.pmu_packet.info.pmu_thread_id = ii;
                     pkt.pmu_packet.info.pmu_cntr_id = jj;
                     pkt.pmu_packet.info.pmu_event_type = pmu_event_type_st3[ii][jj];
                     m_rtl_port.send_event_pmu(MINION_PMU_CNTR_INCR, pkt);
                  end
               end
            end


            //
            // Writes to PMU Counters
            //
            for (int ii = 0; ii < `CORE_NR_THREADS; ii = ii + 1) begin
               for (int jj = 3; jj < 7; jj = jj + 1) begin
                  if (mon_csr_pmu_write_en[ii][jj] === 1'b1) begin
                     evl_minion_packet_u pkt;
                     pkt.pmu_packet.info.pmu_thread_id = ii;
                     pkt.pmu_packet.info.pmu_cntr_id = jj;
                     pkt.pmu_packet.info.pmu_cntr_data = mon_csr_pmu_write_data;
                     m_rtl_port.send_event_pmu(MINION_PMU_CNTR_WR, pkt);
                  end
               end
            end


         //
         // Monitor signals and generate events: for DCache trans;
         // !-- Again: Do not reorder events --!
         // !-- Must follow this order: S3, S2, S1, S0 ... in order for evl_minion_dcache_scoreboard to work correctly --!
         //

            // Note: must sample this MINION_CSR_TENS_ERR_UPD before all other events.
            //
            // minion update CSR_Tensor_Error_Reg: 
            //
            if ( |(`MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[1:0]) === 1'b1 ) begin   // Q: will both en[1:0] bits be asserted at same time ???
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.csr_tens_err_upd.info.upd_en = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[1:0] ;
               if (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[0] === 1'b1) begin
                  pkt.csr_tens_err_upd.info.cur_val_t0 = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t0 ;    // struct "terror_t0_t"
                  pkt.csr_tens_err_upd.info.cur_val[0] = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0] ;    // bit-vector
               end 
               if (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[1] === 1'b1) begin
                  pkt.csr_tens_err_upd.info.cur_val_t1 = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t1 ;    // struct "terror_t0_t"
                  pkt.csr_tens_err_upd.info.cur_val[1] = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1] ;    // bit-vector
               end
               pkt.csr_tens_err_upd.info.new_val = `MIN_IFC_INTPIPE.csr_file.wb_wdata[9:0];
               m_rtl_port.send_event(MINION_CSR_TENS_ERR_UPD, pkt);
            end


            //
            // L1D$->Core Bus_Err indication:
            //
            if (|mon_bus_err[1:0] == 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.core_bus_err.info.bus_err       = mon_bus_err;
               pkt.core_bus_err.info.bus_err_addr  = mon_bus_err_addr;
               m_rtl_port.send_event(MINION_DCACHE_CORE_BUS_ERR, pkt);
            end

            //
            // minion S3_DCU_VPU_SCP_RESP: S3 stage: include both scp_resp, scp_data, and tenb_data;
            //
            if (mon_s1_vpu_scp_read_en_d2 === 1'b1) begin  // S3 stage
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.vpu_scp_data.info.scp_data   = mon_s3_vpu_scp_data; 
               m_rtl_port.send_event(MINION_VPU_SCP_DATA, pkt);
            end

            //
            // S3 L1D$->VPU TenB Data return (for TensorLoad_TenB Req) 
            //
            if (mon_s3_vpu_scp_resp.fill_is_tenb === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s3_vpu_tenb_data.info.tenb_resp  = mon_s3_vpu_scp_resp;
               pkt.s3_vpu_tenb_data.info.tenb_data  = mon_s3_vpu_tenb_data;
               m_rtl_port.send_event(MINION_S3_VPU_TENB_DATA, pkt);
            end

            //
            // S3 L1D$->Core Resp (including Data)
            //
            if (mon_s3_core_resp_valid === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s3_core_resp.info.s3_core_resp  = mon_s3_core_resp;
               m_rtl_port.send_event(MINION_DCACHE_S3_CORE_RESP, pkt);
            end

            //
            // Core S3 stage info: s3_core_x31 (mainly for Atomic_CAS cmp_data)
            //
            if (mon_s2_valid_d1 === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s3_info.info.amo_cmp_data = `MIN_IFC_DCACHE.s3_core_x31; 
               m_rtl_port.send_event(MINION_DCACHE_S3_CORE_X31, pkt);
            end

            //
            // dcache.reduce.s2_inst_op_valid info asserts:  (Charles 11/14/2019) added 
            // note: it has signals s{1,2,3}_inst_op_valid; we only need "s2" stage (in order to get to s3_core_resp);
            //
            if (`MIN_IFC_DCACHE.reduce.s2_inst_op_val === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               // do we need any RTL info ???
               m_rtl_port.send_event(MINION_REDUCE_S2_INST_OP_VLD, pkt);
            end

            //
            // Core S2 stage info
            //
            if (mon_s2_valid === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s2_info.info.hit          = mon_s2_hit;
               pkt.s2_info.info.nack         = mon_s2_nack;            // nack and need to be replayed (exclude misaligned case)
               pkt.s2_info.info.replay_entry = mon_s2_rq_push_entry;
               pkt.s2_info.info.kill         = mon_s2_core_req_valid & mon_s2_core_kill;
               pkt.s2_info.info.way_select   = mon_s2_way_select;
               pkt.s2_info.info.addr         = mon_s2_req_addr;

               pkt.s2_info.info.mh_req_valid   = mon_s2_mh_req_valid & mon_s2_mh_req_ready;     // "valid & ready"
               pkt.s2_info.info.mh_assigned    = mon_s2_mh_assigned;     // [0]=MH0 [1]=MH1 (signal "s2_mh_req_assigned[1:0]")
               pkt.s2_info.info.mh_accepted    = mon_s2_mh_accepted;     // [0]=MH0 [1]=MH1 (signal "s2_mh_req_accepted[1:0]")
               pkt.s2_info.info.mh0_state      = evl_dcache_mh_state'(mon_mh0_state);
               pkt.s2_info.info.mh0_state_next = evl_dcache_mh_state'(mon_mh0_state_next);
               pkt.s2_info.info.mh1_state      = evl_dcache_mh_state'(mon_mh1_state);
               pkt.s2_info.info.mh1_state_next = evl_dcache_mh_state'(mon_mh1_state_next);

               m_rtl_port.send_event(MINION_DCACHE_S2_INFO, pkt);
            end

            //
            // Core S1 stage info
            //
            if (mon_s1_valid === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s1_info.info.kill       = mon_s1_core_req_valid & mon_s1_core_kill;
               pkt.s1_info.info.store_data = mon_s1_core_store_data;
               pkt.s1_info.info.misaligned = mon_s1_misaligned;
               pkt.s1_info.info.core_xcpt    = evl_dcache_tag_xcpt'(mon_s1_core_xcpt);
               pkt.s1_info.info.core_xcpt_mf = `MIN_IFC_DCACHE.s1_mf_xcpt;             // exception due to misalign_fault 
               pkt.s1_info.info.pma_resp     = `MIN_IFC_DCACHE.s1_pma_resp_data; 
               m_rtl_port.send_event(MINION_DCACHE_S1_INFO, pkt);
            end

            //
            // Core requests to DCache (S0)
            //
            if (mon_s0_core_req_valid === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.intpipe_req.info.core_req  = mon_s0_core_req;
               pkt.intpipe_req.info.satp_info = mon_satp_info;
               pkt.intpipe_req.info.vm_status = mon_vm_status[mon_s0_core_req.dest.thread_id];
               pkt.intpipe_req.info.ex_pc     = `MIN_IFC_INTPIPE.ex_reg_pc;
               m_rtl_port.send_event(MINION_DCACHE_REQ, pkt);
            end

            //
            // Replays
            //
            if (mon_s0_rq_replay_valid === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.replay_req.info.replay_req   = mon_s0_rq_replay;
               pkt.replay_req.info.replay_entry = mon_s0_rq_replay_entry;
               m_rtl_port.send_event(MINION_DCACHE_REPLAY, pkt);
            end


            //
            // Cacheop tmask ready
            //
            if ((waiting_cop_tmask === 1'b1) && (mon_core_ctrl.cache_op_req.tmask_ready === 1'b1)) begin
               pending_cop_packet.cop_req.info.cop_req.tmask_bits = mon_core_ctrl.cache_op_req.tmask_bits;
               waiting_cop_tmask <= 1'b0;
               m_rtl_port.send_event(MINION_CACHEOP_REQ, pending_cop_packet);
            end

            //
            // Cacheop requests (CSR to DCache)
            //
            if (mon_core_ctrl.cache_op_valid === 1'b1 && mon_core_ctrl_resp.cache_op_ready === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.cop_req.info.cop_req  = mon_core_ctrl.cache_op_req;
               pkt.cop_req.info.satp_info = mon_satp_info;
               pkt.cop_req.info.vm_status = mon_vm_status[mon_core_ctrl.cache_op_req.thread_id];
               pkt.cop_req.info.pc        = `MIN_IFC_INTPIPE.wb_reg_pc;
               if (mon_core_ctrl.cache_op_req.use_tmask === 1'b1) begin
                  pending_cop_packet = pkt;
                  waiting_cop_tmask <= 1'b1;
               end
               else begin
                  m_rtl_port.send_event(MINION_CACHEOP_REQ, pkt);
               end
            end


            //
            // cacheop S2 stage
            //
            if (`MIN_IFC_DCACHE.cache_op_unit.state == Cache_Op_Meta_Resp) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.cop_s2.info.conflict   = mon_cop_conflict;
               pkt.cop_s2.info.addr       = `MIN_IFC_DCACHE.cache_op_unit.meta_data_req_addr;
               pkt.cop_s2.info.lock_err   = `MIN_IFC_DCACHE.cache_op_unit.err_lock_next;
               pkt.cop_s2.info.way_select = mon_cop_way;
               m_rtl_port.send_event(MINION_CACHEOP_S2, pkt);
            end


            //
            // cacheop pipeline grant (S1): for CacheOp_FSM (RTL) involving L1 (and maybe L2/L3 as well)
            //
            if (mon_s1_co_mr_valid === 1'b1) begin   // exclude s1_co_pf_valid: Prefetch_L1 is handled like normal Load
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.cop_s1.info.addr = `MIN_IFC_DCACHE.s1_req_addr;
               pkt.cop_s1.info.core_xcpt    = evl_dcache_tag_xcpt'(mon_s1_core_xcpt); 
               pkt.cop_s1.info.core_xcpt_mf = `MIN_IFC_DCACHE.s1_mf_xcpt; 
               pkt.cop_s1.info.pma_resp     = `MIN_IFC_DCACHE.s1_pma_resp_data; 
               pkt.cop_s1.info.err_mem      = `MIN_IFC_DCACHE.cache_op_unit.err_mem_next;
             //pkt.cop_s1.info.csr_terr_t0  = mon_tensor_err_t0_m1_1cyc;   // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
             //pkt.cop_s1.info.csr_terr_t1  = mon_tensor_err_t1_m1_1cyc;
               m_rtl_port.send_event(MINION_CACHEOP_S1, pkt);
            end


            //
            // cacheop pipeline grant (S0):
            //
            if ((mon_s0_co_mr_valid | mon_s0_co_pf_valid) === 1'b1) begin   // include both s0_co_mr_valid and s0_co_pf_valid: 
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.cop_s0.info.addr = `MIN_IFC_DCACHE.s0_req_addr;
               m_rtl_port.send_event(MINION_CACHEOP_S0, pkt);
            end


            //
            // Meta data write from miss handler
            //
            if (mon_metadata_wen_s2 === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.mw_req.info.meta_req  = mon_metadata_s2;
               pkt.mw_req.info.thread_id = mon_mw_thread_s2;
               m_rtl_port.send_event(MINION_DCACHE_MW_REQ, pkt);
            end


            //
            // Tensor Load when tmask_ready or previous_ready (after tmask_wait or previous_wait)
            //
            //if ((waiting_tload_tmask === 1'b1) && (mon_core_ctrl.tensorload_tmask_ready === 1'b1)) begin
            //   pending_tload_packet.tload_req.info.tmask_bits = mon_core_ctrl.tensorload_tmask_bits;
            //   waiting_tload_tmask <= 1'b0;
            //   m_rtl_port.send_event(MINION_TENSOR_LOAD_REQ, pending_tload_packet);
            //end

            // for TLoad_L1SCP
            if (tload_0_waiting_tmask_or_previous === 1'b1) begin
               if ((tload_0_waiting_tmask === 1'b1) && (mon_core_ctrl.tensorload_tmask_ready === 1'b1)) begin
                  pending_tload_0_packet.tload_req.info.tmask_bits = mon_core_ctrl.tensorload_tmask_bits;
                  tload_0_waiting_tmask <= 1'b0;
               end
               if (`MIN_IFC_DCACHE.tensor_load_0.ctrl_state_next !== 3'b001) begin  // 3'b001=ML_CTRL_Wait_Start
                  tload_0_waiting_tmask_or_previous <= 1'b0;
                  m_rtl_port.send_event(MINION_TENSOR_LOAD_REQ, pending_tload_0_packet);
               end
            end 

            // for TLoad_TenB
            if (tload_1_waiting_tmask_or_previous === 1'b1) begin
               if ((tload_1_waiting_tmask === 1'b1) && (mon_core_ctrl.tensorload_tmask_ready === 1'b1)) begin
                  pending_tload_1_packet.tload_req.info.tmask_bits = mon_core_ctrl.tensorload_tmask_bits;
                  tload_1_waiting_tmask <= 1'b0;
               end
               if (`MIN_IFC_DCACHE.tensor_load_1.ctrl_state_next !== 3'b001) begin  // 3'b001=ML_CTRL_Wait_Start
                  tload_1_waiting_tmask_or_previous <= 1'b0;
                  m_rtl_port.send_event(MINION_TENSOR_LOAD_REQ, pending_tload_1_packet);
               end
            end 

            //
            // Tensor Load requests
            //
            if (mon_core_ctrl.tensorload_start === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_req.info.ctrl        = mon_core_ctrl.tensorload_ctrl;
               pkt.tload_req.info.coop_ctrl   = mon_core_ctrl.tensorcoop_ctrl;
               pkt.tload_req.info.stride      = mon_core_ctrl.tensorload_stride;
               pkt.tload_req.info.tmask_bits  = mon_core_ctrl.tensorload_tmask_bits;
               pkt.tload_req.info.thread_id   = mon_core_ctrl.thread_id;
               pkt.tload_req.info.satp_info   = mon_satp_info;
               pkt.tload_req.info.vm_status   = mon_vm_status[mon_core_ctrl.thread_id];
               pkt.tload_req.info.pc          = `MIN_IFC_INTPIPE.wb_reg_pc;

               //if (mon_core_ctrl.tensorload_ctrl.use_tmask === 1'b1) begin
               //   pending_tload_packet = pkt;
               //   waiting_tload_tmask <= 1'b1;
               //end
               //else begin
               //   m_rtl_port.send_event(MINION_TENSOR_LOAD_REQ, pkt);
               //end

               // change to: always wait (at least) 1 cycle: either for tmask or previous tload; 
               if (pkt.tload_req.info.ctrl.tl_sel == 1'b0) begin
                  pending_tload_0_packet = pkt;
                  tload_0_waiting_tmask_or_previous <= 1'b1;     // L1SCP
                  // separate flag for wait_tmask:
                  if (mon_core_ctrl.tensorload_ctrl.use_tmask === 1'b1) begin
                     tload_0_waiting_tmask <= 1'b1;
                  end
               end else begin
                  pending_tload_1_packet = pkt;
                  tload_1_waiting_tmask_or_previous <= 1'b1;     // TenB
                  // separate flag for wait_tmask:
                  if (mon_core_ctrl.tensorload_ctrl.use_tmask === 1'b1) begin
                     tload_1_waiting_tmask <= 1'b1;
                  end
               end
               m_rtl_port.send_event(MINION_TENSOR_LOAD_REQ_CYC0, pkt);
            end

            // TensorLoad_TenB restart & flush:
            if (`MIN_IFC_DCACHE.tenb_flush === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               m_rtl_port.send_event(MINION_TENSOR_TENB_FLUSH, pkt);
            end

            // note: we need to exclude the cycle when (tenb_flush==1);
            if ((`MIN_IFC_DCACHE.tensor_ctrl.restart_b === 1'b1) && (`MIN_IFC_DCACHE.tenb_flush === 1'b0)) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               m_rtl_port.send_event(MINION_TENSOR_RESTART_B, pkt);
            end


            // Note: important: in code below: must sample MINION_TFMA_REQ before MINION_TFMA_PROC_EN;
            // TFMA Req start:
            if (`MIN_IFC_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_start === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tfma_ctrl.info.tfma_ctrl          = `MIN_IFC_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_ctrl;
               pkt.tfma_ctrl.info.tfma_conv_bits_rdy = `MIN_IFC_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_conv_bits_ready;
               pkt.tfma_ctrl.info.tfma_conv_bits     = `MIN_IFC_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_conv_bits;
               // ...
               m_rtl_port.send_event(MINION_TFMA_REQ, pkt);
            end

            // TFMA process starts/enabled: paired with previous TL_TenB: need only 1-cyc pulse;
            //if ((`MIN_IFC_DCACHE.tensor_load_1.tfma_enabled === 1'b1) && (mon_tfma_enabled_d1 === 1'b0)) begin
            if ((`MIN_IFC_DCACHE.s1_vpu_ctrl.tfma_enabled === 1'b1) && (mon_tfma_enabled_d1 === 1'b0)) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               // ...
               m_rtl_port.send_event(MINION_TFMA_PROC_EN, pkt);
            end


            //
            // Tensor Load S1 Info
            //
            if (`MIN_IFC_DCACHE.s1_tl_addr_load === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_s1s2.info.tl_sel = `MIN_IFC_DCACHE.s1_tl1_addr_load;
               pkt.tload_s1s2.info.core_xcpt    = evl_dcache_tag_xcpt'(mon_s1_core_xcpt); 
               pkt.tload_s1s2.info.core_xcpt_mf = `MIN_IFC_DCACHE.s1_mf_xcpt; 
               pkt.tload_s1s2.info.pma_resp     = `MIN_IFC_DCACHE.s1_pma_resp_data; 
               pkt.tload_s1s2.info.tld0_err_mem  = `MIN_IFC_DCACHE.tensor_load_0.err_mem_next; 
               pkt.tload_s1s2.info.tld1_err_mem  = `MIN_IFC_DCACHE.tensor_load_1.err_mem_next; 
             //pkt.tload_s1s2.info.csr_terr_t0  = mon_tensor_err_t0_m1_1cyc;    // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
             //pkt.tload_s1s2.info.csr_terr_t1  = mon_tensor_err_t1_m1_1cyc;
               m_rtl_port.send_event(MINION_TLOAD_S1_INFO, pkt);
            end

            //
            // Tensor Load 0 S2 Info
            //
            if ((`MIN_IFC_DCACHE.tensor_load_0.s2_req_valid & `MIN_IFC_DCACHE.tensor_load_0.s2_req_ready) === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_s1s2.info.tl_sel = 1'b0;
               pkt.tload_s1s2.info.etlink_id = $clog2(`MIN_IFC_DCACHE.tensor_load_0.new_l2_req);
               m_rtl_port.send_event(MINION_TLOAD_S2_INFO, pkt);
            end

            //
            // Tensor Load 1 S2 Info
            //
            if ((`MIN_IFC_DCACHE.tensor_load_1.s2_req_valid & `MIN_IFC_DCACHE.tensor_load_1.s2_req_ready) === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_s1s2.info.tl_sel = 1'b1;
               pkt.tload_s1s2.info.etlink_id = $clog2(`MIN_IFC_DCACHE.tensor_load_1.new_l2_req);
               m_rtl_port.send_event(MINION_TLOAD_S2_INFO, pkt);
            end

            //
            // Tensor Load 0 S2 Cancel
            //
            if ((`MIN_IFC_DCACHE.tensor_load_0.s2_req_valid & ~`MIN_IFC_DCACHE.tensor_load_0.s2_req_ready & ~`MIN_IFC_DCACHE.tensor_load_0.s2_req_valid_next) === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_s1s2.info.tl_sel  = 1'b0;
               pkt.tload_s1s2.info.cancel = 1'b1;
               m_rtl_port.send_event(MINION_TLOAD_S2_INFO, pkt);
            end

            //
            // Tensor Load 1 S2 Cancel
            //
            if ((`MIN_IFC_DCACHE.tensor_load_1.s2_req_valid & ~`MIN_IFC_DCACHE.tensor_load_1.s2_req_ready & ~`MIN_IFC_DCACHE.tensor_load_1.s2_req_valid_next) === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tload_s1s2.info.tl_sel  = 1'b1;
               pkt.tload_s1s2.info.cancel = 1'b1;
               m_rtl_port.send_event(MINION_TLOAD_S2_INFO, pkt);
            end

            //
            // Tensor Store
            //
            if (mon_core_ctrl.tensorstore_start === 1'b1 && mon_core_ctrl_resp.ts_ready === 1'b1 && mon_core_ctrl.reduce_tensorstore_ctrl.tensorstore.scp === 1'b0) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tstore_req.info.ctrl = mon_core_ctrl.reduce_tensorstore_ctrl.tensorstore;
               pkt.tstore_req.info.stride = mon_core_ctrl.tensorstore_stride;
               pkt.tstore_req.info.thread_id = mon_core_ctrl.thread_id;
               pkt.tstore_req.info.satp_info = mon_satp_info;
               pkt.tstore_req.info.vm_status = mon_vm_status[mon_core_ctrl.thread_id];
               pkt.tstore_req.info.pc        = `MIN_IFC_INTPIPE.wb_reg_pc;
               m_rtl_port.send_event(MINION_TENSOR_STORE_REQ, pkt);
            end

            //
            // Tensor Store S1:
            //
            if ((`MIN_IFC_DCACHE.s1_ts_addr_load === 1'b1) && (`MIN_IFC_DCACHE.s1_available_to_ts === 1'b1)) begin
               evl_minion_packet_u pkt; 
               pkt.packet = 0;
               pkt.tstore_s1.info.core_xcpt    = evl_dcache_tag_xcpt'(mon_s1_core_xcpt); 
               pkt.tstore_s1.info.core_xcpt_mf = `MIN_IFC_DCACHE.s1_mf_xcpt; 
               pkt.tstore_s1.info.pma_resp     = `MIN_IFC_DCACHE.s1_pma_resp_data; 
               pkt.tstore_s1.info.err_mem      = `MIN_IFC_DCACHE.reduce.err_mem_next;  // but "reduce.err_mem_next" is S2 signal (too late), we need S1; 
             //pkt.tstore_s1.info.csr_terr_t0  = mon_tensor_err_t0_m1_1cyc;   // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
             //pkt.tstore_s1.info.csr_terr_t1  = mon_tensor_err_t1_m1_1cyc;
               m_rtl_port.send_event(MINION_TSTORE_S1_INFO, pkt);  
            end

            //
            // DCache report exception/fault: PMA access)fault or misalign_fault: s1->s2->s3: will check s3_addr[]; 
            //
            if (mon_s3_xcpt_access_or_misalign_fault === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.s3_core_resp.info.s3_core_resp  = mon_s3_core_resp;  // only need "s3_core_resp.addr[]"
               m_rtl_port.send_event(MINION_DCACHE_S3_EXPT_FAULT, pkt);
            end

            //
            // Tensor Store register sent by VPU
            //
            if (mon_vpu_send_reg === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.vpu_reg_send.info.data = mon_s1_core_store_data;
               m_rtl_port.send_event(MINION_VPU_REG_SEND, pkt);
            end

            //
            // Tensor Store From Scratchpad
            //
            if (mon_core_ctrl.tensorstore_start === 1'b1 && mon_core_ctrl_resp.ts_ready === 1'b1 && mon_core_ctrl.reduce_tensorstore_ctrl.tensorstore.scp === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.tstore_scp_req.info.ctrl = mon_core_ctrl.reduce_tensorstore_ctrl.tensorstore_scp;
               pkt.tstore_scp_req.info.stride = mon_core_ctrl.tensorstore_stride;
               pkt.tstore_scp_req.info.thread_id = mon_core_ctrl.thread_id;
               pkt.tstore_scp_req.info.satp_info = mon_satp_info;
               pkt.tstore_scp_req.info.vm_status = mon_vm_status[mon_core_ctrl.thread_id];
               pkt.tstore_scp_req.info.pc        = `MIN_IFC_INTPIPE.wb_reg_pc;
               m_rtl_port.send_event(MINION_TENSOR_STORE_SCP_REQ, pkt);
            end

            //
            // Tensor Reduce: reduce_req_start (first cycle; might see s1_vpu_ctrl.reduce_wait) 
            //
            if (mon_core_ctrl.reduce_start === 1'b1 && mon_core_ctrl_resp.reduce_ready === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.reduce_req.info.ctrl = mon_core_ctrl.reduce_tensorstore_ctrl.reduce;
               pkt.reduce_req.info.thread_id = mon_core_ctrl.thread_id;
               pkt.reduce_req.info.pc        = `MIN_IFC_INTPIPE.wb_reg_pc;
               m_rtl_port.send_event(MINION_TENSOR_REDUCE_REQ, pkt);
               my_reduce_req_vpu_wait <= 1'b1;
            end

            //
            // Tensor Reduce: reduce_req after vpu_dcu_ctrl.reduce_wait deasserts (from VPU->DCU/LSU): see RTL tensor_reduce FSM;
            // note: generate signal my_reduce_new_req (1-cyc pulse), to line up with RTL FSM state Reduce_New_Req.
            //
            if ((my_reduce_req_vpu_wait === 1'b1) && (mon_vpu_reduce_wait === 1'b0)) begin
               my_reduce_new_req <= 1'b1; 
               my_reduce_req_vpu_wait <= 1'b0;
            end
            if (my_reduce_new_req === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.reduce_req.info.err_partner          = mon_reduce_err_partner_id;
               pkt.reduce_req.info.err_operation        = mon_reduce_err_operation;
               pkt.reduce_req.info.csr_terr_t0          = mon_tensor_err_t0_m1_1cyc;
               pkt.reduce_req.info.csr_terr_t1          = mon_tensor_err_t1_m1_1cyc;
               m_rtl_port.send_event(MINION_TENSOR_REDUCE_REQ_AFT_WAIT, pkt);
               my_reduce_new_req <= 1'b0;
            end
            
            //
            // Cache control (cache partitionning / cacheop limits): request
            //
            if (mon_cache_ctrl_st !== mon_cache_ctrl) begin
               evl_minion_packet_u pkt;
               mon_cache_ctrl_st <= mon_cache_ctrl;
               pkt.packet = 0;
               pkt.cache_ctrl.info = mon_cache_ctrl;
               m_rtl_port.send_event(MINION_CACHE_CTRL_REQ, pkt);
            end

            //
            // Cache control (cache partitionning / cacheop limits): when Dcache scratchpad_ctrl FSM state reaches SCP_CTRL_Zero 
            //
            if ((`MIN_IFC_DCACHE.scratchpad_ctrl.state_next == 2'b10) && (`MIN_IFC_DCACHE.scratchpad_ctrl.state != 2'b10)) begin  // SCP_CTRL_Zero (first cycle)
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               m_rtl_port.send_event(MINION_CACHE_CTRL_START_ZERO, pkt);
            end

            //
            // Cache control (cache partitionning / cacheop limits): when Dcache scratchpad_ctrl FSM state reaches SCP_CTRL_All_Set (when zero_count_end==1) 
            //
            if ((`MIN_IFC_DCACHE.scratchpad_ctrl.state_next == 2'b11) && (`MIN_IFC_DCACHE.scratchpad_ctrl.state != 2'b11)) begin  // reach SCP_CTRL_All_Set
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.cache_ctrl.info = mon_cache_ctrl;    // need to sample cache_ctrl again
               m_rtl_port.send_event(MINION_CACHE_CTRL_DONE, pkt);
            end

            //
            // ESR info
            //
            if ((mon_s1_mprot_st !== mon_s1_mprot) || (mon_dcache_bypass_st !== `MIN_IFC_DCACHE.bypass_dcache))  begin
               evl_minion_packet_u pkt;
               mon_s1_mprot_st <= mon_s1_mprot;
               mon_dcache_bypass_st <= `MIN_IFC_DCACHE.bypass_dcache;
               pkt.packet = 0;
               pkt.esr_info.info.mprot = mon_s1_mprot;
               pkt.esr_info.info.bypass = `MIN_IFC_DCACHE.bypass_dcache;
               m_rtl_port.send_event(MINION_ESR_INFO, pkt);
            end

            //
            // Fence
            //
            for (integer i = 0; i < `CORE_NR_THREADS; i++) begin
               if (mon_ex_fence[i] === 1'b1) begin
                  evl_minion_packet_u pkt;
                  pkt.packet = 0;
                  pkt.fence.thread_id = i;
                  m_rtl_port.send_event(MINION_FENCE, pkt);
               end
               if (`MIN_IFC_INTPIPE.id_fe_req_valid[i]) begin
                  evl_minion_packet_u pkt;
                  pkt.packet = 0;
                  pkt.fence.thread_id = i;
                 m_rtl_port.send_event(MINION_CANCEL_FENCE, pkt);
               end
            end

            //
            // minion is in ioshire: just need to sample once (right after reset): 
            //
            if ((mon_ioshire === 1'b1) && (mon_ioshire_sampled === 1'b0)) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.is_ioshire.in_ioshire = 1;
               m_rtl_port.send_event(MINION_IN_IOSHIRE, pkt);
               mon_ioshire_sampled = 1'b1;
            end

            //
            // my_mh_l2_resp_stall[1:0] (for MissHandle[1:0]) start
            //
            if (|my_mh_l2_resp_stall_start[1:0] === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.mh_l2_resp_stall.mh_l2_resp_stall_start[1:0] = my_mh_l2_resp_stall_start[1:0];
               m_rtl_port.send_event(MINION_MH_L2_RESP_STALL_START, pkt);
            end

            //
            // my_mh_l2_resp_stall[1:0] (for MissHandle[1:0]) clear: we only care about CLine ReFill: need one extra cyc delay 
            //
            if (|my_mh_l2_resp_stall_clear_d1[1:0] === 1'b1) begin         // xxx_d1 is enough 
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.mh_l2_resp_stall.mh_l2_resp_stall_clear[1:0] = my_mh_l2_resp_stall_clear_d1[1:0];
               m_rtl_port.send_event(MINION_MH_L2_RESP_STALL_CLEAR, pkt);
            end

            //
            // minion VPU_DCU_SCP_Read_Req: S1 stage;
            //
            if (mon_s1_vpu_scp_req.scp_req.read_en === 1'b1) begin
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.vpu_scp_req.info.way[1:0]   = mon_s1_vpu_scp_req.scp_req.way[1:0];
               pkt.vpu_scp_req.info.addr[9:0]  = mon_s1_vpu_scp_req.scp_req.addr[9:0];
               pkt.vpu_scp_req.info.size64     = mon_s1_vpu_scp_req.scp_req.size64;
               pkt.vpu_scp_req.info.size256    = mon_s1_vpu_scp_req.scp_req.size256;
               m_rtl_port.send_event(MINION_VPU_SCP_REQ, pkt);
            end


            //
            // End_Of_Sim: sample CSR_TensorError[] reg info 
            //
            if ( (my_end_of_sim === 1'b1) && (my_end_of_sim_d1 === 1'b0) ) begin   // just need 1-cycle 
               evl_minion_packet_u pkt;
               pkt.packet = 0;
               pkt.end_of_sim.info.cur_val_t0 = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t0 ;
               pkt.end_of_sim.info.cur_val_t1 = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_t1 ;
               pkt.end_of_sim.info.cur_val[0] = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0] ;
               pkt.end_of_sim.info.cur_val[1] = `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1] ;
               m_rtl_port.send_event(MINION_END_OF_SIM, pkt);
            end

         end
         m_rtl_port.ping_clock();
      end
   end  // always @(posedge clock) begin


   task drive_validation1_reg(logic [63:0] reg_data, bit tid);
      if (tid == 1'b0) begin
         force `MIN_IFC_INTPIPE.csr_file.reg_validation1[0] = reg_data;
      end
      else begin
         force `MIN_IFC_INTPIPE.csr_file.reg_validation1[1] = reg_data;
      end
   endtask // drive_validation1_reg

   task reset_w_core();
     force `MIN_IFC_TOP.reset_non_debug = 1'b1;
     repeat (10) @ (posedge clock);
     force `MIN_IFC_TOP.reset_non_debug = 1'b0;
   endtask // reset_w_core


  wire reset_for_assert = ((reset | isolate) !== 1'b0) ? 1'b1 : 1'b0;

  //
  // DCache functional coverage and assertions
  `include "dv/minion_common/cov/minion_core_dcache_cvg_inc.svh"
  `include "dv/minion_common/assert/minion_core_dcache_assert.svh"

  // VPU assertions
  `include "dv/minion_common/assert/minion_vpu_assert.svh"

  //Intpipe functional coverage and assertions
  `include "dv/minion_common/cov/minion_core_intpipe_cvg_inc.svh"
  `include "dv/minion_common/assert/minion_core_intpipe_assert.svh"  

  // frontend assertions
  `include "dv/minion_common/assert/minion_core_frontend_assert.svh"

  // Utility assertions
  `include "dv/minion_common/assert/minion_core_utility_assert.svh"

endmodule : evl_minion_interface
