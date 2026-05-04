`ifndef __EVL_MINION_VC_PKG

   `define __EVL_MINION_VC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
   `include "rtl/inc/minion_defines.vh"
   `include "rtl/inc/vpu_defines.vh"
   `include "rtl/inc/frontend_defines.vh"
   `include "rtl/inc/dcache_defines.vh"
   `include "rtl/inc/csr_defines.vh"

   package evl_minion_sub_rtl_pkg;
      `include "rtl/inc/et_link_types.vh"
   endpackage : evl_minion_sub_rtl_pkg

   package evl_minion_rtl_pkg;
      import evl_minion_sub_rtl_pkg::*;
      `include "rtl/inc/esr_types_auto.vh"
      `include "rtl/inc/minion_types.vh"
      `include "rtl/inc/vpu_types.vh"
      `include "rtl/inc/frontend_types.vh"
      `include "rtl/inc/dcache_types.vh"
      `include "rtl/inc/csr_types.vh"
      `include "rtl/inc/esr_types_legacy.vh"
   endpackage : evl_minion_rtl_pkg

   package evl_minion_vc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;
      import evl_minion_rtl_pkg::*;

      `define EVL_MINION_PACKET_SIZE 512
      `define DCACHE_REPLAYQ_CONFLICT_MASK evl_vaddr_t'(16'h7fe0)

      typedef enum int {
         MINION_STALL_FE_INT,
         MINION_OPC_FE_INT,
         MINION_REDIR_INT_FE,
         MINION_ICACHE_REQ_VALID_FE,
         MINION_UNCACHEABLE_FE,
         MINION_UC_REPLAY,
         MINION_FE_ACCESS_FAULT,
         MINION_DCACHE_MW_REQ,
         MINION_DCACHE_REQ,
         MINION_DCACHE_REPLAY,
         MINION_DCACHE_S1_INFO,
         MINION_DCACHE_S2_INFO,
         MINION_CACHEOP_REQ,
         MINION_CACHEOP_S0,         // CacheOp S0 stage 
         MINION_CACHEOP_S1,         // CacheOp (involving L1, and maybe L2/L3 as well)
         MINION_CACHEOP_L2_S1,      // CacheOp_L2 (involving only L2/L3)
         MINION_CACHEOP_S2,
         MINION_CACHEOP_WAY_INFO,
         MINION_TENSOR_LOAD_REQ_CYC0,
         MINION_TENSOR_LOAD_REQ,
         MINION_TENSOR_RESTART_B,
         MINION_TENSOR_TENB_FLUSH,
         MINION_TLOAD_S1_INFO,
         MINION_TLOAD_S2_INFO,
         MINION_TENSOR_STORE_REQ,
         MINION_TSTORE_S1_INFO,       //(CYE) added
         MINION_VPU_REG_SEND,
         MINION_TENSOR_STORE_SCP_REQ,
         MINION_TENSOR_REDUCE_REQ,
         MINION_TENSOR_REDUCE_REQ_AFT_WAIT,
         MINION_CACHE_CTRL_REQ,
         MINION_CACHE_CTRL_START_ZERO,
         MINION_CACHE_CTRL_DONE,
         MINION_ESR_INFO,
         MINION_TEX_CTRL,
         MINION_FENCE,
         MINION_CANCEL_FENCE,
         MINION_IN_IOSHIRE,           //(Charles) added
         MINION_MH_L2_RESP_STALL_START,        //(Charles) added
         MINION_MH_L2_RESP_STALL_CLEAR,        //(Charles) added
         MINION_CSR_TENS_ERR_UPD,           //(Charles) added
         MINION_VPU_SCP_REQ,            // S1 stage: read req
         MINION_VPU_SCP_DATA,           // S3 stage: return data
         MINION_DCACHE_S3_CORE_X31,     // s3_core_x31
         MINION_DCACHE_S3_CORE_RESP,
         MINION_S3_VPU_TENB_DATA,       // L1D$ return TenB data to VPU (for TensorLoadTenB Req) 
         MINION_DCACHE_CORE_BUS_ERR,
         MINION_REDUCE_S2_INST_OP_VLD,  //(Charles) added
         MINION_DCACHE_S3_EXPT_FAULT,   //(Charles) added 
         MINION_TFMA_REQ,
         MINION_TFMA_PROC_EN,
         MINION_PMU_CNTR_WR,
         MINION_PMU_CNTR_RD,
         MINION_PMU_CNTR_INCR,
         MINION_PMU_EVENT_WR,
         MINION_END_OF_SIM           //(Charles) added
      } evl_minion_event_t;

      typedef enum bit [3:0] {
         DCQ_None = 0,
         DCQ_Replay,
         DCQ_Resp,
         DCQ_BusErr,
         DCQ_ToL2,
         DCQ_ToL3,
         DCQ_ToMem,
         DCQ_UnlockInv,
         DCQ_Uncached
      } evl_dcache_qual_t;

      typedef frontend_core_resp      evl_frontend_core_resp; // opcode packet
      typedef minion_fe_req           evl_minion_fe_req;      // redirect packet

      typedef minion_dcache_req       evl_minion_dcache_req;   // normal cache requests
      typedef dcache_replay_req       evl_dcache_replay_req;   // replay requests
      typedef core_dcache_op_req      evl_core_dcache_op_req;  // cacheops
      typedef tensorload_control      evl_tensorload_control;
      typedef tensorcoop_control      evl_tensorcoop_control;
      typedef dcache_meta_write_req   evl_dcache_meta_write_req;
      typedef minion_satp_info        evl_minion_satp_info;
      typedef minion_vm_status        evl_minion_vm_status;
      typedef dcache_cmd              evl_dcache_cmd;
      typedef dcache_type             evl_dcache_type;
      typedef reduce_control          evl_reduce_ctrl;
      typedef tensorstore_control     evl_tensorstore_ctrl;
      typedef tensorstore_scp_control evl_tensorstore_scp_ctrl;
      typedef cache_control           evl_cache_control;
      typedef esr_mprot_t             evl_esr_mprot_t;
      typedef et_link_core_miss_id_t  evl_etlink_id_t;
      typedef dcache_tag_xcpt         evl_dcache_tag_xcpt;
      typedef dcache_pma_resp         evl_dcache_pma_resp;
      typedef dcache_minion_resp      evl_dcache_minion_resp;
      typedef dcache_vpu_scp_resp     evl_dcache_vpu_scp_resp;
      typedef tensorfma_control       evl_minion_tfma_ctrl;
      typedef dcache_mh_state         evl_dcache_mh_state;


      typedef struct packed {
         logic  id_ctrl_stall;
         logic  id_ctrl_stall_hazard;
         logic  id_ctrl_stall_hazard_vpu;
         logic  id_int_sboard_hazard;
         logic  id_ctrl_stall_div;
         logic  id_ctrl_stall_trans;
         logic  id_do_fence;
         logic  id_ctrl_stall_dcache;
         logic  id_ctrl_stall_csr;
         logic  [`CORE_NR_THREADS-1:0] csr_fe_stall;
         logic  ex_gsc_busy;
         logic  id_stall_fp2int;
         logic  [`FE_FETCH_BUFFERS-1:0] buffer_empty_t0;
         logic  f6_stall_t0;
         logic  [`FE_FETCH_BUFFERS-1:0] buffer_empty_t1;
         logic  f6_stall_t1;
         logic  [`CORE_NR_THREADS-1:0]  inst_fifo_full;
      } evl_stall_pkt_t;


      typedef struct packed {
         evl_frontend_core_resp fe_inst_data;
         logic                  fe_thread_id;
      } evl_frontend_opcode_pkt_t;

      typedef struct packed {
         logic                  pmu_thread_id;
         logic            [3:0] pmu_cntr_id;
         logic           [63:0] pmu_cntr_data;
         logic            [4:0] pmu_event_type;
      } evl_minion_pmu_pkt_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_pmu_pkt_t):0] padding;
         evl_minion_pmu_pkt_t info;
      } evl_minion_pmu_pkt_s;

      typedef struct packed {
         evl_minion_fe_req [`CORE_NR_THREADS-1:0] fe_redirect_info;
         logic             [`CORE_NR_THREADS-1:0] fe_redirect_thread;
         logic             [`CORE_NR_THREADS-1:0] fe_redirect_ipi;
         logic					  fe_redirect_wb_xcpt;
         logic					  fe_redirect_wb_csr_eret;
         logic					  fe_redirect_wb_csr_replay;
         logic					  fe_redirect_wb_reg_replay;
         logic		   [4:0]		  icache_req_valid_hi_cycles;
         logic		   [4:0]		  icache_req_valid_to_resp_cycles;
         logic					  t0_replay_no_redir;
         logic					  t1_replay_no_redir;
      } evl_frontend_redirect_pkt_t;

      typedef struct packed {
         evl_minion_dcache_req core_req;
         evl_minion_satp_info  satp_info;
         evl_minion_vm_status  vm_status;
         evl_vaddr_t           ex_pc;
      } evl_dcache_intpipe_req_t;


      typedef struct packed {
         evl_dcache_replay_req              replay_req;
         logic [`DCACHE_REPLAYQ_ADDR_RANGE] replay_entry;
      } evl_dcache_replay_req_t;


      typedef struct packed {
         evl_core_dcache_op_req cop_req;
         evl_minion_satp_info   satp_info;
         evl_minion_vm_status   vm_status;
         evl_vaddr_t            pc;
      } evl_dcache_cop_req_t;


      typedef struct packed {
         evl_tensorload_control        ctrl;            // TensorLoad control
         evl_tensorcoop_control        coop_ctrl;       // TensorCooperation control
         logic [`VA_RANGE]             stride;          // TensorLoad row stride
         logic [`TENSOR_MASK_BITS-1:0] tmask_bits;      // TensorMask bits
         logic                         thread_id;
         evl_minion_satp_info          satp_info;
         evl_minion_vm_status          vm_status;
         evl_vaddr_t                   pc;
      } evl_dcache_tload_req_t;


      typedef struct packed {
         logic      tl_sel;           // s1: 0=TL_L1SCP 1=TL_TenB
         logic[1:0] etlink_id;        // s2
         logic      cancel;           // s2
         evl_dcache_tag_xcpt    core_xcpt;    // (s1) L1D$ Resp -> Core
         logic                  core_xcpt_mf; // (s1) exception due to misalign_fault
         evl_dcache_pma_resp    pma_resp;     // (s1) PMA_Resp
         logic                  tld0_err_mem;  // (s1) for TLoad_0
         logic                  tld1_err_mem;  // (s1) for TLoad_1
       //terror_t0_t            csr_terr_t0;  // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
       //terror_t1_t            csr_terr_t1;
      } evl_dcache_tload_s1s2_t;


      typedef struct packed {
         evl_tensorstore_ctrl   ctrl;
         logic [`VA_RANGE]      stride;
         logic                  thread_id;
         evl_minion_satp_info   satp_info;
         evl_minion_vm_status   vm_status;
         evl_vaddr_t            pc;
      } evl_dcache_tstore_req_t;


      typedef struct packed {
         evl_dcache_tag_xcpt    core_xcpt;    // (s1) L1D$ Resp -> Core
         logic                  core_xcpt_mf; // (s1) exception due to misalign_fault
         evl_dcache_pma_resp    pma_resp;     // (s1) PMA_Resp
         logic                  err_mem;      // (need s1)(but RTL only has s1 signal ???) 
       //terror_t0_t            csr_terr_t0;  // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
       //terror_t1_t            csr_terr_t1;
      } evl_dcache_tstore_s1_t;


      typedef struct packed {
         logic [`DCACHE_DATA_RANGE] data;
      } evl_dcache_vpu_reg_send_t;


      typedef struct packed {
         evl_tensorstore_scp_ctrl ctrl;
         logic [`VA_RANGE]        stride;
         logic                    thread_id;
         evl_minion_satp_info     satp_info;
         evl_minion_vm_status     vm_status;
         evl_vaddr_t              pc;
      } evl_dcache_tstore_scp_req_t;


      typedef struct packed {
         evl_reduce_ctrl      ctrl;
         logic                thread_id;
         evl_vaddr_t          pc;
         logic                err_partner;
         logic                err_operation;
         terror_t0_t          csr_terr_t0;
         terror_t1_t          csr_terr_t1;
      } evl_dcache_reduce_req_t;


      typedef struct packed {
         evl_esr_mprot_t mprot;
         logic           bypass;
      } evl_esr_config_t;


      typedef struct packed {
         logic                      kill;
         logic [`DCACHE_DATA_RANGE] store_data;
         logic                      misaligned;
         evl_dcache_tag_xcpt        core_xcpt;    // (s1) L1D$ Resp -> Core
         logic                      core_xcpt_mf; // (s1) exception due to misalign_fault
         evl_dcache_pma_resp        pma_resp;     // (s1) PMA_Resp
      } evl_dcache_s1_pipe_info_t;


      typedef struct packed {
         logic                              hit;
         logic                              nack;         // nack and need to be replayed (exclude misaligned case)
         logic [`DCACHE_REPLAYQ_ADDR_RANGE] replay_entry;
         logic                              kill;
         logic [`DCACHE_WAYS-1:0]           way_select;
         logic [`VA_RANGE]                  addr;
         logic                              mh_req_valid;   // "valid & ready"
         logic [`DCACHE_MH_FILE_SIZE-1:0]   mh_assigned; 
         logic [`DCACHE_MH_FILE_SIZE-1:0]   mh_accepted;
         evl_dcache_mh_state                mh0_state;
         evl_dcache_mh_state                mh0_state_next;
         evl_dcache_mh_state                mh1_state;
         evl_dcache_mh_state                mh1_state_next;

      } evl_dcache_s2_pipe_info_t;


      typedef struct packed {
         logic [`VA_RANGE] addr;
      } evl_core_dcache_cop_s0_t;

      typedef struct packed {
         logic [`VA_RANGE]          addr;
         evl_dcache_tag_xcpt        core_xcpt;    // (s1) L1D$ Resp -> Core
         logic                      core_xcpt_mf; // (s1) exception due to misalign_fault
         evl_dcache_pma_resp        pma_resp;     // (s1) PMA_Resp
         logic                      err_mem;      // (s1) from either Cache_Op or Cache_Op_L2
       //terror_t0_t                csr_terr_t0;  // RTL: pma_access_fault -> csr_tensor_error[7] is 1-cyc delayed
       //terror_t1_t                csr_terr_t1;
      } evl_core_dcache_cop_s1_t;

      typedef struct packed {
         logic                    conflict;
         logic [`VA_RANGE]        addr;
         logic                    lock_err;
         logic [`DCACHE_WAYS-1:0] way_select;
      } evl_core_dcache_cop_s2_t;


      typedef struct packed {
         evl_dcache_meta_write_req meta_req;
         logic                     thread_id;
      } evl_dcache_meta_write_req_t;



      //
      // Basic packet structure.
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1:0] info;
      } evl_minion_packet_s;

      // Following structure is used to collect stall packet
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_stall_pkt_t):0] padding;
         evl_stall_pkt_t info;
      } evl_stall_pkt_s;


      // Following structure is used to collect opcode packet
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_frontend_opcode_pkt_t):0] padding;
         evl_frontend_opcode_pkt_t info;
      } evl_frontend_opcode_pkt_s;

      // Following structure is used to collect redirect packet
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_frontend_redirect_pkt_t):0] padding;
         evl_frontend_redirect_pkt_t info;
      } evl_frontend_redirect_pkt_s;


      //
      // Following structure is used to specify cache meta data write event
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_meta_write_req_t):0] padding;
         evl_dcache_meta_write_req_t info;
      } evl_dcache_mw_req_s;


      //
      // Following structure is used to specify Dcache requests from intpipe
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_intpipe_req_t):0] padding;
         evl_dcache_intpipe_req_t info;
      } evl_dcache_intpipe_req_s;


      //
      // Following structure is used to specify Dcache replayed requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_replay_req_t):0] padding;
         evl_dcache_replay_req_t info;
      } evl_dcache_replay_req_s;


      //
      // Following structure is used to specify S1 event info
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_s1_pipe_info_t):0] padding;
         evl_dcache_s1_pipe_info_t info;
      } evl_dcache_s1_pipe_info_s;


      //
      // Following structure is used to specify S2 event info
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_s2_pipe_info_t):0] padding;
         evl_dcache_s2_pipe_info_t info;
      } evl_dcache_s2_pipe_info_s;


      //
      // Core->L1D$: s3_core_x31: mainly for Atomic_CAS (cmp_data); 
      //
      typedef struct packed {
         logic [63:0]  amo_cmp_data;
      } evl_dcache_s3_core_x31_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_s3_core_x31_t):0] padding;
         evl_dcache_s3_core_x31_t info;
      } evl_dcache_s3_core_x31_s;


      //
      // Following structure is used to specify CacheOp requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_cop_req_t):0] padding;
         evl_dcache_cop_req_t info;
      } evl_dcache_cop_req_s;


      //
      // Following structure is used to get info when a cacheop is granted access to the pipeline
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_core_dcache_cop_s0_t):0] padding;
         evl_core_dcache_cop_s0_t info;
      } evl_dcache_cop_s0_s;


      //
      // Following structure is used to get info when a cacheop is granted access to the pipeline
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_core_dcache_cop_s1_t):0] padding;
         evl_core_dcache_cop_s1_t info;
      } evl_dcache_cop_s1_s;


      //
      // Following structure is used to get info when a cacheop is granted access to the pipeline
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_core_dcache_cop_s2_t):0] padding;
         evl_core_dcache_cop_s2_t info;
      } evl_dcache_cop_s2_s;


      //
      // Following structure is used to specify TensorLoad requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_tload_req_t):0] padding;
         evl_dcache_tload_req_t info;
      } evl_dcache_tload_req_s;


      //
      // Following structure is used to specify TensorLoad requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_tload_s1s2_t):0] padding;
         evl_dcache_tload_s1s2_t info;
      } evl_dcache_tload_s1s2_s;


      // Following structure is used to specify TensorStore requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_tstore_req_t):0] padding;
         evl_dcache_tstore_req_t info;
      } evl_dcache_tstore_req_s;


      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_tstore_s1_t):0] padding;
         evl_dcache_tstore_s1_t info;
      } evl_dcache_tstore_s1_s;


      //
      // Following structure is used to specify TensorStore register data send from VPU
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_vpu_reg_send_t):0] padding;
         evl_dcache_vpu_reg_send_t info;
      } evl_dcache_vpu_reg_send_s;


      //
      // Following structure is used to specify Tensor Store from Scratchpad requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_tstore_scp_req_t):0] padding;
         evl_dcache_tstore_scp_req_t info;
      } evl_dcache_tstore_scp_req_s;


      //
      // Following structure is used to specify Reduce requests
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_reduce_req_t):0] padding;
         evl_dcache_reduce_req_t info;
      } evl_dcache_reduce_req_s;


      //
      // Following structure is used to specify Cache Control
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_cache_control):0] padding;
         evl_cache_control info;
      } evl_dcache_cache_ctrl_s;


      //
      // Following structure is used to specify Cache Control
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_esr_config_t):0] padding;
         evl_esr_config_t info;
      } evl_dcache_esr_info_s;


      //
      // Following structure is used to specify fence events
      //
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-2:0] padding;
         logic thread_id;
      } evl_dcache_fence_s;


      // Following structure is used to specify in_ioshire events
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-2:0] padding;
         logic in_ioshire;
      } evl_minion_ioshire_s;


      // Following structure is used to specify Dcache mh_l2_resp_stall events
      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-5:0] padding;
         logic [1:0] mh_l2_resp_stall_start;
         logic [1:0] mh_l2_resp_stall_clear;
      } evl_dcache_mh_l2_resp_stall_s;


      //
      // Following structure is used to specify csr_tensor_error_reg update events
      //
      typedef struct packed {
         logic [1:0] upd_en;
         terror_t0_t cur_val_t0;     // struct
         terror_t1_t cur_val_t1;     // struct
         logic [1:0][9:0] cur_val;   // bit-vector
         logic [9:0] new_val;        // bit-vector
      } evl_minion_csr_tens_err_upd_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_csr_tens_err_upd_t):0] padding;
         evl_minion_csr_tens_err_upd_t info;
      } evl_minion_csr_tens_err_upd_s;


      //
      // structure to specify VPU->DCU SCP_Read Req event: S1 stage;
      //
      typedef struct packed {
         logic [1:0] way;
         logic [9:0] addr;
         logic       size64;
         logic       size256;
      } evl_minion_vpu_scp_req_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_vpu_scp_req_t):0] padding;
         evl_minion_vpu_scp_req_t info;
      } evl_minion_vpu_scp_req_s;


      //
      // S3 L1D$->Core Resp
      //
      typedef struct packed {
         evl_dcache_minion_resp  s3_core_resp;   
      } evl_dcache_s3_core_resp_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_s3_core_resp_t):0] padding;
         evl_dcache_s3_core_resp_t info;
      } evl_dcache_s3_core_resp_s;


      //
      // S3 L1D$->VPU TenB Data (for TensorLoad_TenB Req)
      //
      typedef struct packed {
         evl_dcache_vpu_scp_resp     tenb_resp;   
         logic [`DCACHE_DATA_RANGE]  tenb_data;
      } evl_dcache_s3_vpu_tenb_data_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_s3_vpu_tenb_data_t):0] padding;
         evl_dcache_s3_vpu_tenb_data_t info;
      } evl_dcache_s3_vpu_tenb_data_s;


      //
      // S3: L1D$->VPU SCP_Data return for VPU->DCU (S1) SCP_Read: s3_vpu_scp_data;
      //
      typedef struct packed {
         logic [`DCACHE_DATA_RANGE] scp_data; 
      } evl_minion_vpu_scp_data_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_vpu_scp_data_t):0] padding;
         evl_minion_vpu_scp_data_t info;
      } evl_minion_vpu_scp_data_s;


      //
      // L1D$->Core Bus_Err indication:
      //
      typedef struct packed {
         logic [1:0]             bus_err;
         logic [1:0][`PA_RANGE]  bus_err_addr;
      } evl_dcache_core_bus_err_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_dcache_core_bus_err_t):0] padding;
         evl_dcache_core_bus_err_t info;
      } evl_dcache_core_bus_err_s;


      // VPU ML TensorFMA Req:
      typedef struct packed {
         evl_minion_tfma_ctrl   tfma_ctrl;
         logic                  tfma_conv_bits_rdy;   
         logic [15:0]           tfma_conv_bits;   
      } evl_minion_tfma_ctrl_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_tfma_ctrl_t):0] padding;
         evl_minion_tfma_ctrl_t info;
      } evl_minion_tfma_ctrl_s;


      //
      // Following structure is used to specify End_of_Sim (PostShutDown phase) event
      //
      typedef struct packed {
         terror_t0_t cur_val_t0;     // struct
         terror_t1_t cur_val_t1;     // struct
         logic [1:0][9:0] cur_val;   // bit-vector
         // FUTURE: add more signals to check if needed;
      } evl_minion_end_of_sim_t;

      typedef struct packed {
         bit [`EVL_MINION_PACKET_SIZE-1-$bits(evl_minion_end_of_sim_t):0] padding;
         evl_minion_end_of_sim_t info;
      } evl_minion_end_of_sim_s;



      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
         evl_minion_packet_s          packet;
         evl_stall_pkt_s              stall_packet;
         evl_frontend_opcode_pkt_s    opcode_packet;
         evl_frontend_redirect_pkt_s  redirect_packet;
         evl_dcache_mw_req_s          mw_req;
         evl_dcache_intpipe_req_s     intpipe_req;
         evl_dcache_replay_req_s      replay_req;
         evl_dcache_s1_pipe_info_s    s1_info;
         evl_dcache_s2_pipe_info_s    s2_info;
         evl_dcache_s3_core_x31_s     s3_info;
         evl_dcache_cop_req_s         cop_req;
         evl_dcache_cop_s0_s          cop_s0;
         evl_dcache_cop_s1_s          cop_s1;
         evl_dcache_cop_s2_s          cop_s2;
         evl_dcache_tload_req_s       tload_req;
         evl_dcache_tload_s1s2_s      tload_s1s2;
         evl_dcache_tstore_req_s      tstore_req;
         evl_dcache_tstore_s1_s       tstore_s1;
         evl_dcache_vpu_reg_send_s    vpu_reg_send;
         evl_dcache_tstore_scp_req_s  tstore_scp_req;
         evl_dcache_reduce_req_s      reduce_req;
         evl_dcache_cache_ctrl_s      cache_ctrl;
         evl_dcache_esr_info_s        esr_info;
         evl_dcache_fence_s           fence;
         evl_minion_ioshire_s         is_ioshire;
         evl_dcache_mh_l2_resp_stall_s  mh_l2_resp_stall;
         evl_minion_csr_tens_err_upd_s  csr_tens_err_upd;
         evl_dcache_s3_core_resp_s     s3_core_resp;
         evl_dcache_s3_vpu_tenb_data_s s3_vpu_tenb_data;
         evl_minion_vpu_scp_req_s      vpu_scp_req;
         evl_minion_vpu_scp_data_s     vpu_scp_data;
         evl_dcache_core_bus_err_s     core_bus_err;
         evl_minion_tfma_ctrl_s        tfma_ctrl;
         evl_minion_pmu_pkt_s          pmu_packet;
         evl_minion_end_of_sim_s       end_of_sim;
      } evl_minion_packet_u;


      typedef class evl_minion_packet;
      typedef class evl_minion_dcache_scoreboard;
      typedef class evl_minion_fe_scoreboard;
      typedef class evl_minion_pmu_monitor;

      `include "dv/minion_common/evl_minion_rtl_port.sv"
      `include "dv/minion_common/evl_minion_packet.sv"
      `include "dv/minion_common/evl_minion_dcache.sv"
      `include "dv/minion_common/evl_minion_dcache_scoreboard.sv"
      `include "dv/minion_common/evl_minion_l1d_scoreboard.sv"
      `include "dv/minion_common/evl_minion_fe_scoreboard.sv"
      `include "dv/minion_common/evl_minion_pmu_monitor.sv"
      // Minion core dcache coverage class
      `include "dv/minion_common/cov/minion_core_dcache_cvg.svh"
      `include "dv/minion_common/cov/minion_core_intpipe_cvg.svh" 

   endpackage : evl_minion_vc_pkg

`endif // !__EVL_MINION_VC_PKG
