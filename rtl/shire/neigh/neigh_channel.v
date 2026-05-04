// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// This is the channel of the neighborhood. It contains the Icaches, PTWs, some
// arbiters and flip flops to access the L2
module neigh_channel (
  // North HiV interface
  // -------------------
  // System signals
  input  logic                                             clock,              // HiV neigh clock 
  input  logic                                             reset_cold,         // Global HiV reset_cold from IOShire 
  input  logic                                             reset_warm,         // Global HiV reset_warm from IOShire
  output logic                                             reset_w_icache,     // Warm reset     HiV reset (to shire Icache)

  // DFT signals
  input  logic                                             dft__reset_byp_hv,      //Reset byp HiV
  input  logic                                             dft__reset_hv,          //DFT mode reset HiV
  input  logic                                             dft__use_reset_cntl_hv, // DFT reset control over sys_gasket_lov
  input  shire_tdr_reset_cntl_t                            dft__reset_cntl_hv,     // DFT reset control over sys_gasket_lov

  // Clock gate Ctrl override
  input  minion_chicken_bits_vc_t                          chicken_bits_vc,  
  // Debug
  output bpam_rc_tbox_ack_t                                bpam_rc_tbox_ack_hi, //from neigh channel to shire
    
  // ESRs
  input  logic [`NUM_SHIRE_IDS_R]                          shire_id,
  input  logic [`NUM_NEIGH_R]                              neigh_id,
  input logic [`TBOX_PER_SHIRE_R]                          shire_tbox_id,
  input logic                                              shire_tbox_en,
  input  logic [`MIN_PER_N-1:0]                            esr_thread0_enable,
  input  logic [`MIN_PER_N-1:0]                            esr_thread1_enable,
  input  logic [`ESR_MINION_FEATURE_SZ-1:0]                esr_minion_features,
  input  icache_prefetch_conf_t                            esr_icache_prefetch_conf,
  input  logic                                             esr_icache_prefetch_start,
  output logic                                             esr_icache_prefetch_done,
  output logic                                             esr_icache_err_detected_op,
  output logic                                             esr_icache_err_logged_op,
  input  esr_ms_dmctrl_t                                   dmctrl,
  output esr_and_or_tree_L0_t                              esr_and_or_tree_L0_op,
  input  logic                                             esr_shire_coop_mode,
  input  logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]       esr_minion_mem_override,

  // L2 Request bus
  input  logic [(`SC_BANKS+1)-1:0]                         neigh_sc_req_ready,
  output logic [(`SC_BANKS+1)-1:0]                         neigh_sc_req_valid,
  output et_link_req_info_t                                neigh_sc_req_info,
  // L2 Response bus
  output logic                                             neigh_sc_rsp_ready,
  input  logic                                             neigh_sc_rsp_valid,
  input  et_link_rsp_info_t                                neigh_sc_rsp_info,

  // UltraSoc Status monitor signals
  input  logic                                             hw_dbg_sm_monitor_enabled, 
  input  logic [`neigh_sm_gpio_width - 1:0]                neigh_sm_gpio,
  output neigh_sm_dbg_monitor_t                            neigh_sm_signals,
  // APB from Shire Bus Master or Shire BPAM to read/write ESR or I$
  input   APB_to_neigh_t                                   APB_req_from_shire,
  output  APB_from_neigh_t                                 APB_rsp_to_shire,

  // Interrupts
  input  logic [`THREADS_PER_N-1:0]                        int_mtip,
  input  logic [`THREADS_PER_N-1:0]                        int_meip,
  input  logic [`THREADS_PER_N-1:0]                        int_seip,
  // IPI
  input  logic [`THREADS_PER_N-1:0]                        ipi_msip,
  input  logic [`THREADS_PER_N-1:0]                        ipi_redirect_trigger,
  input  logic [`MIN_PER_N-1:0]                            uc_to_neigh_fcc,
  input  logic [`UC_FCC_TARGET_SIZE-1:0]                   uc_to_neigh_fcc_target,
  // Fast Local Barrier request interface between neigh and L2
  output logic                                             flb_neigh_l2_req_valid,
  output logic [`CSR_FL_BARRIER_RANGE]                     flb_neigh_l2_req_data,
  // Fast Local Barrier response interface between neigh and L2
  input  logic                                             flb_l2_neigh_resp_valid,
  input  logic                                             flb_l2_neigh_resp_data,
  // Request to L1 SRAM blocks
  output logic                                             icache_f2_sram_req_write,
  output logic [`ICACHE_SRAM_ADD_WIDTH-1:0]                icache_f2_sram_req_addr,
  output logic                                             icache_f2_sram_req_valid,
  input  logic                                             icache_f2_sram_req_ready,
  // Response from L1 SRAM blocks
  input  logic [`ICACHE_SRAM_DATA_WIDTH-1:0]               icache_f0_sram_resp_dout,
  input  logic                                             icache_f0_sram_resp_valid,
  output logic                                             icache_f0_sram_resp_ready,

  // North LoV interface
  // -------------------
  // Cooperative TLoad slave ready output bus
  output coop_tload_slv_rdy_req_t                          coop_tload_slv_rdy_out_data,
  output logic                                             coop_tload_slv_rdy_out_valid,
  // Cooperative TLoad slave ready input buses
  input  coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]     coop_tload_slv_rdy_in_data,
  input  logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_slv_rdy_in_valid,
  // Cooperative TLoad master done output bus
  output logic [`SHIRE_COOP_ID_RANGE]                      coop_tload_mst_done_out_coop_id,
  output logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_out_valid,
  // Cooperative TLoad master done input buses
  input  logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]  coop_tload_mst_done_in_coop_id,
  input  logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_in_valid,

  // Minion LoV interface
  // --------------------
  // Sytem signals
  output logic                                                              clock_minion,       // LoV minion clock 
  output logic  [`MIN_PER_N-1:0]                                            reset_c_per_minion, // Global         LoV reset
  output logic  [`MIN_PER_N-1:0]                                            reset_d_per_minion, // Debug reset    LoV reset
  output logic  [`MIN_PER_N-1:0]                                            reset_w_per_minion, // Warm reset     LoV resets per minions
  // DFT signals
  output logic                                                              dft__reset_byp_lv,  //Reset byp LoV
  output logic                                                              dft__reset_lv,      //DFT mode reset HoV
  // Clock gate Ctrl override
  output minion_chicken_bits_vc_t                                           chicken_bits_vc_lo,
  // ESRs
  output logic [`NUM_SHIRE_IDS_R]                                           shire_id_lo,
  output logic [`MIN_PER_N-1:0][`MIN_PER_S_R]                               shire_min_id,
  output logic [`MIN_PER_N-1:0]                                             thread0_enable,
  output logic [`MIN_PER_N-1:0]                                             thread1_enable,
  output logic [`ESR_MINION_FEATURE_SZ-1:0]                                 esr_minion_features_lo,
  output logic                                                              esr_shire_coop_mode_lo,
  output logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]                        esr_minion_mem_override_lo,
    
  // Neighborhood ESRs broadcast to the Minions
  output logic                                                              esr_bypass_dcache,
  output logic [`VA_SIZE-1:0]                                               minion_boot_addr,
  output esr_mprot_t                                                        mprot,
  output tlb_entry_type                                                     vmspagesize,
  // Interrupts
  output minion_interrupts [`MIN_PER_N-1:0]                                 interrupts_per_minion,
  // Fast Local Barrier request interface between cores and neigh
  input  logic [`MIN_PER_N-1:0]                                             flb_core_neigh_req_valid,
  input  logic [`MIN_PER_N-1:0][`CSR_FL_BARRIER_RANGE]                      flb_core_neigh_req_data,
  // Fast Local Barrier response interface between cores and neigh
  output logic [`MIN_PER_N-1:0]                                             flb_neigh_core_resp_valid,
  output logic                                                              flb_neigh_core_resp_data,
  // Minion Dcache L2 request to evict
  output logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0]             l2_dcache_evict_req_ready,
  input  logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0]             l2_dcache_evict_req_valid,
  input  et_link_minion_evict_req_info_t [`MIN_PER_N-1:0]                   l2_dcache_evict_req,
  // Minion Dcache L2 request to miss
  output logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]              l2_dcache_miss_req_ready,
  input  logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]              l2_dcache_miss_req_valid,
  input  et_link_minion_miss_req_info_t [`MIN_PER_N-1:0]                    l2_dcache_miss_req,
  // Minion Dcache L2 response
  input  logic [`MIN_PER_N-1:0]                                             l2_dcache_resp_ready,
  output logic [`MIN_PER_N-1:0]                                             l2_dcache_resp_valid,
  output et_link_minion_rsp_info_t [`MIN_PER_N-1:0]                         l2_dcache_resp,
  // Minion Icache request
  output logic [`MIN_PER_N-1:0]                                             icache_req_ready,
  input  logic [`MIN_PER_N-1:0]                                             icache_req_valid,
  input  frontend_icache_req [`MIN_PER_N-1:0]                               icache_req,
  // Minion Icache response
  output logic [`ICACHE_NR_REQS-1:0]                                        icache_resp_valid,
  output logic [`ICACHE_NR_REQS-1:0]                                        icache_resp_miss,
  output icache_frontend_resp [`ICACHE_NR_REQS-1:0]                         icache_resp,
  output logic [`ICACHE_NR_REQS-1:0]                                        icache_fill_done,
  // Minion Icache control
  input  logic [`MIN_PER_N-1:0]                                             icache_flush_data,
  // TLB/PTW control
  input  minion_satp_info [`MIN_PER_N-1:0]                                  satp_info,
  input  minion_satp_info [`MIN_PER_N-1:0]                                  matp_info,
  input  logic [`MIN_PER_N-1:0]                                             tlb_invalidate,
  // Minion Dcache PTW request
  input  minion_ptw_req [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                  dc_ptw_req_data,
  input  logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                           dc_ptw_req_valid,
  output logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                           dc_ptw_req_ready,
  // Minion Dcache PTW response
  output minion_ptw_pte [`PTW_PER_N-1:0]                                    ptw_dc_resp_data,
  output logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                           ptw_dc_resp_valid,
  // APB connection with the minions
  input  logic [`MIN_PER_N-1:0][`ESR_APB_D_WIDTH - 1:0]                     minion_apb_prdata,
  input  logic [`MIN_PER_N-1:0]                                             minion_apb_pready,
  input  logic [`MIN_PER_N-1:0]                                             minion_apb_pslverr,
  output logic                                                              minion_apb_penable,
  output logic [`MIN_PER_N-1:0]                                             minion_apb_psel,
  output logic                                                              minion_apb_pwrite,
  output logic [`MIN_DM_APB_ADDR_WIDTH-1:0]                                 minion_apb_paddr,
  output logic [`ESR_APB_D_WIDTH - 1:0]                                     minion_apb_pwdata,
  // Minion run control signals
  output minion_debug_in_t [`MIN_PER_N-1:0]                                 debug_minion_in,
  input  minion_debug_out_t [`MIN_PER_N-1:0]                                debug_minion_out,
  // Status Monitor signals   
  output logic [`MIN_PER_N-1:0]                                             minion_dbg_signals_en,
  input  neigh_sm_dbg_monitor_t [`MIN_PER_N-1:0]                            minion_dbg_signals,
  output logic [`MIN_PER_N-1:0][`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]              minion_dbg_signals_mux,
  // PMU Interface
  input  [`MIN_PER_N-1:0][`PMU_MINION_COUNTERS_RANGE]                       pmu_count_up,
  output [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`XREG_RANGE]                pmu_read_data,
  input  [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE] pmu_read_sel,
  input  [`MIN_PER_N-1:0][`PMU_TOTAL_COUNTERS_RANGE]                        pmu_write_en,
  input  [`MIN_PER_N-1:0][`XREG_RANGE]                                      pmu_write_data,
  input  [`MIN_PER_N-1:0][`PMU_NEIGH_EVENT_CNT_SEL_RANGE]                   pmu_neigh_event_sel,

  // South TBOX LoV interface
  // ------------------------
  // System signals
  output logic                                 clock_tbox,         // LoV tbox clock 
  output logic                                 tbox_reset_c,       // Cold  reset    LoV reset (to TBox)
  output logic                                 tbox_reset_d,       // Debug reset    LoV reset (to TBox)
  output logic                                 tbox_reset_w,       // Warm  reset    LoV reset (to TBox)
  // TBOX ESRs
  output logic [`TBOX_PER_SHIRE_R]             tbox_id_lo,
  output logic [15:0]                          tbox_control_esr,
  output logic [`VA_SIZE-1:0]                  tbox_image_table_ptr_esr,
  // TBOX neighborhood request bus
  output logic                                 tbox_neigh_req_ready,
  input  logic                                 tbox_neigh_req_valid,
  input  et_link_neigh_req_info_t              tbox_neigh_req_info,
  // TBOX neighborhood response bus
  input  logic                                 tbox_neigh_rsp_ready,
  output logic                                 tbox_neigh_rsp_valid,
  output et_link_rsp_info_t                    tbox_neigh_rsp_info,
  // TBOX monitor signals
  output logic                                 tbox_dbg_signals_en,
  input  neigh_sm_dbg_monitor_t                tbox_dbg_signals,
  output logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0] tbox_dbg_signals_mux,
  // APB from Shire Bus Master or BPAM to read/write tbox
  output APB_to_neigh_tbox_t                   tbox_APB_req,
  input  APB_from_neigh_t                      tbox_APB_rsp,
  // Debug
  output tbox_bpam_run_control_t               tbox_bpam_run_control,
  input  bpam_rc_tbox_ack_t                    tbox_bpam_run_control_ack //from TBOX to neigh channel
);

// Bank FIFOs + UC FIFO
localparam OUT_FIFOS   = `SC_BANKS+1;
localparam OUT_FIFOS_L = $clog2(OUT_FIFOS);
localparam UC_FIFO     = unsigned'(`SC_BANKS);
// OUT FIFOs + TBOX FIFO
localparam DEST_FIFOS   = OUT_FIFOS+1;
localparam DEST_FIFOS_L = $clog2(DEST_FIFOS);
localparam TBOX_FIFO    = unsigned'(OUT_FIFOS);

// INTERNAL DECLARATIONS
// Minion miss FF request
et_link_neigh_req_info_t [`MIN_PER_N-1:0]                                  min_miss_req;
logic [`MIN_PER_N-1:0]                                                     min_miss_req_valid;
logic [`MIN_PER_N-1:0]                                                     min_miss_req_ready;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0] min_coop_miss_req;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                          min_coop_miss_req_valid;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                          min_coop_miss_req_ready;
// Minion evict FF request
et_link_neigh_req_info_t [`MIN_PER_N-1:0]           min_evict_req;
logic [`MIN_PER_N-1:0]                              min_evict_req_valid;
logic [`MIN_PER_N-1:0]                              min_evict_req_ready;
et_link_neigh_req_info_t [`MIN_PER_N-1:0]           coop_evict_req;
logic [`MIN_PER_N-1:0]                              coop_evict_valid;
logic [`MIN_PER_N-1:0]                              coop_evict_ready;
// Common minion request
et_link_nodata_req_info_t [`MIN_PER_N-1:0]          min_l2_req_info;
logic [`MIN_PER_N-1:0][`NEIGH_DATA_RANGE]           min_l2_req_data;
logic [`MIN_PER_N-1:0]                              min_l2_req_is_evict;
logic [`MIN_PER_N-1:0]                              min_l2_req_valid;
logic [`MIN_PER_N-1:0]                              min_l2_req_bid;
logic [`MIN_PER_N-1:0]                              min_l2_req_ready;
// Cooperative TLoad request
et_link_neigh_req_info_t                            coop_tload_miss_req_info;
logic                                               coop_tload_miss_req_valid;
logic                                               coop_tload_miss_req_ready;
// Cooperative TLoad tag table CAM port
logic                                               coop_tload_tag_table_cam_pop;
logic [`SHIRE_COOP_LOAD_ID_RANGE]                   coop_tload_tag_table_cam_tag;
coop_tload_tag_table_data_t                         coop_tload_tag_table_cam_data;
// TStore evict request
logic                                               tstore_evict_valid;
logic                                               tstore_miss_req_ready;
et_link_neigh_req_info_t                            tstore_evict_req;
// TStore response to minions
logic [`MIN_PER_N-1:0]                              tstore_min_resp_ready;
logic [`MIN_PER_N-1:0]                              tstore_min_resp_valid;
et_link_minion_rsp_info_t [`MIN_PER_N-1:0]          tstore_min_resp;
// Icache request
et_link_nodata_req_info_t                           icache_l2_miss_req_info;
logic                                               icache_l2_miss_req_valid;
logic                                               icache_l2_miss_req_ready;
// PTW request
et_link_neigh_req_info_t [`PTW_PER_N-1:0]           ptw_l2_miss_req_info;
logic [`PTW_PER_N-1:0]                              ptw_l2_miss_req_valid;
logic [`PTW_PER_N-1:0]                              ptw_l2_miss_req_ready;
// TBOX request FIFO
et_link_nodata_req_info_t                           tbox_miss_req_info;
logic                                               tbox_miss_req_valid;
logic                                               tbox_miss_req_ready;
// Miss request Round-Robin arbiter signals
logic [`NEIGH_NUM_REQ_AGENTS-1:0]                   miss_req_arb_bid;
et_link_nodata_req_info_t [3:0]                     miss_req_info_xx;
et_link_nodata_req_info_t [3:0]                     miss_req_info_xx_next;
logic [3:0][`NEIGH_DATA_RANGE]                      miss_req_data_xx;
logic [3:0][`NEIGH_DATA_RANGE]                      miss_req_data_xx_next;
et_link_nodata_req_info_t                           miss_req_info;
logic [`NEIGH_DATA_RANGE]                           miss_req_data;
logic [`NEIGH_NUM_REQ_AGENTS-1:0]                   miss_req_arb_grant;
logic [`MIN_PER_N-1:0]                              min_l2_req_arb_grant;
logic                                               icache_l2_miss_req_arb_grant;
logic [`PTW_PER_N-1:0]                              ptw_l2_miss_req_arb_grant;
logic                                               tbox_l2_miss_req_arb_grant;
logic                                               coop_tload_l2_miss_req_arb_grant;
logic                                               tstore_l2_miss_req_arb_grant;
logic [3:0]                                         miss_req_valid_xx;
logic [3:0]                                         miss_req_valid_xx_next;
logic [3:0]                                         miss_req_accepted_xx;
logic                                               miss_req_valid;
logic                                               miss_req_accepted;
logic                                               miss_req_ready;
// Flopped and address processed miss request
et_link_nodata_req_info_t                           miss_req_info_ff;
logic [`NEIGH_DATA_RANGE]                           miss_req_data_ff;
logic                                               miss_req_valid_ff;
logic                                               miss_req_accepted_ff;
et_link_nodata_req_info_t                           miss_req_info_ff_proc;
et_link_nodata_req_info_t                           miss_req_info_ff_proc_next;
logic [`NEIGH_DATA_RANGE]                           miss_req_data_ff_proc;
logic [`NEIGH_DATA_RANGE]                           miss_req_data_ff_proc_next;
logic                                               miss_req_valid_ff_proc;
logic [DEST_FIFOS_L-1:0]                            miss_req_info_dest_fifo;
logic [DEST_FIFOS_L-1:0]                            miss_req_info_dest_fifo_next;
logic                                               miss_req_goes_to_tbox;
logic                                               miss_req_goes_to_tbox_next;
logic                                               miss_req_goes_to_int_fifo;
// Intermediate FIFO
logic [OUT_FIFOS-1:0][`NEIGH_INT_FIFO_SIZE_L:0]     int_req_fifo_counter;
logic [OUT_FIFOS-1:0][`NEIGH_INT_FIFO_SIZE_L:0]     int_req_fifo_counter_next;
et_link_nodata_req_info_t                           int_req_info_in;
logic [`NEIGH_DATA_RANGE]                           int_req_data_in;
logic [OUT_FIFOS_L-1:0]                             int_req_info_push_out_fifo;
logic                                               int_req_push_early;
logic                                               int_req_push;
logic [OUT_FIFOS-1:0]                               int_req_push_per_out_fifo;
logic                                               int_req_fifo_full;
logic                                               int_req_info_pop;
logic                                               int_req_data_pop;
logic [OUT_FIFOS-1:0]                               int_req_pop_per_out_fifo;
et_link_nodata_req_info_t                           int_req_info_out;
logic [`NEIGH_DATA_RANGE]                           int_req_data_out;
logic [OUT_FIFOS_L-1:0]                             int_req_info_pop_out_fifo;
logic                                               int_req_info_valid;
// Output FIFOs
et_link_nodata_req_info_t                           out_req_info_in;
logic                                               out_req_is_line_evict;
logic [`NEIGH_DATA_RANGE]                           out_req_data_in;
logic [OUT_FIFOS-1:0]                               out_req_push;
logic [OUT_FIFOS-1:0]                               out_req_fifo_full;
// Miss request per-bank info to the L2
et_link_req_info_t [`SC_BANKS-1:0]                  miss_req_bank_info;
logic [`SC_BANKS-1:0]                               miss_req_bank_valid;
logic [`SC_BANKS-1:0]                               miss_req_bank_ready;
// Minion to TBOX requests
et_link_neigh_req_info_t                            min_tbox_req_info;
logic                                               min_tbox_req_valid_early;
logic                                               min_tbox_req_valid;
logic                                               min_tbox_req_ready;
// TBOX to minion replies
et_link_neigh_rsp_info_t                            tbox_min_rsp_info;
logic                                               tbox_min_rsp_valid;
logic                                               tbox_min_rsp_ready;
et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]           tbox_min_rsp_info_in;
logic [`MIN_PER_N-1:0]                              tbox_min_rsp_valid_in;
logic [`MIN_PER_N-1:0]                              tbox_min_rsp_ready_in;
// Fill FIFO
et_link_rsp_info_t                                  fill_fifo_in_rsp_info;
logic                                               fill_fifo_in_rsp_valid;
logic                                               fill_fifo_in_rsp_ready;
logic                                               fill_fifo_push;
logic                                               fill_fifo_full;
et_link_rsp_info_t [`NEIGH_NUM_RESP_AGENTS-1:0]     fill_fifo_out_rsp_info;
logic [`NEIGH_NUM_RESP_AGENTS-1:0]                  fill_fifo_out_rsp_valid;
logic [`NEIGH_NUM_RESP_AGENTS-1:0]                  fill_ff_available;
// Minion Fill FF response
et_link_rsp_info_t [`MIN_PER_N-1:0]                 fill_fifo_min_resp;
logic [`MIN_PER_N-1:0]                              fill_fifo_min_valid;
logic [`MIN_PER_N-1:0]                              fill_fifo_min_ready;
// ESRs
logic [`NUM_NEIGH_R]                                neigh_id_lo;
esr_neigh_chicken_t                                 esr_neigh_chicken;
// PMU Unit
esr_pmu_ctrl_t                                      esr_pmu_ctrl;
logic  [`PMU_NEIGH_EVENTS:1]                        pmu_neigh_events;
logic  [`PMU_NEIGH_EVENTS:1]                        pmu_neigh_events_next;
// FCCs
logic [`MIN_PER_N-1:0]                              uc_to_neigh_fcc_lo;
logic [`UC_FCC_TARGET_SIZE-1:0]                     uc_to_neigh_fcc_target_lo;
// Interrupts (low voltage)
logic [`THREADS_PER_N-1:0]                          int_mtip_lo;
logic [`THREADS_PER_N-1:0]                          int_meip_lo;
logic [`THREADS_PER_N-1:0]                          int_seip_lo;
logic [`THREADS_PER_N-1:0]                          ipi_msip_lo;
logic [`THREADS_PER_N-1:0]                          ipi_redirect_trigger_lo;  
//IPI program counter
logic [`PC_RANGE]                                   esr_ipi_redirect_pc;
// Fast local barrier
logic                                               flb_neigh_l2_req_valid_lo;
logic [`CSR_FL_BARRIER_RANGE]                       flb_neigh_l2_req_data_lo;
logic                                               flb_l2_neigh_resp_valid_lo;
logic                                               flb_l2_neigh_resp_data_lo;
// Request to L1 SRAM blocks (low voltage)
logic                                               icache_f2_sram_req_write_lo;
logic [`ICACHE_SRAM_ADD_WIDTH-1:0]                  icache_f2_sram_req_addr_lo;
logic                                               icache_f2_sram_req_valid_lo;
logic                                               icache_f2_sram_req_ready_lo;
// Response from L1 SRAM blocks (low voltage)
logic [`ICACHE_SRAM_DATA_WIDTH-1:0]                 icache_f0_sram_resp_dout_lo;
logic                                               icache_f0_sram_resp_valid_lo;
logic                                               icache_f0_sram_resp_ready_lo;
// Icache ESRs (low voltage)
icache_prefetch_conf_t                              esr_icache_prefetch_conf_lo; 
logic                                               esr_icache_prefetch_start_lo; 
logic                                               esr_icache_prefetch_done_lo; 
logic                                               esr_icache_err_detected_lo;
logic                                               esr_icache_err_logged_lo;
// TBOX ESRs
logic [15:0]                                        esr_texture_control_op;
logic [`VA_SIZE-1:0]                                esr_texture_image_table_ptr_op;
logic [15:0]                                        esr_texture_control_op_ff;
logic [`VA_SIZE-1:0]                                esr_texture_image_table_ptr_op_ff;
// L2 Response bus (low voltage)
et_link_rsp_info_t                                  neigh_sc_rsp_info_lo;
logic                                               neigh_sc_rsp_valid_lo;
logic                                               neigh_sc_rsp_ready_lo;
// Debug
esr_ms_dmctrl_t                                     dmctrl_lo;
logic                                               hw_dbg_sm_monitor_enabled_lo;
// Debug signals going to Status Monitor
logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0] miss_ff_dbg_sm_signals;
logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] evict_ff_dbg_sm_signals;
fill_ff_dbg_sm_t [`MIN_PER_N-1:0]                   fill_ff_dbg_sm_signals;
icache_dbg_sm_t                                     icache_dbg_sm_signals;
ptw_dbg_sm_t [`PTW_PER_N-1:0]                       ptw_dbg_sm_signals;
coop_tload_dbg_sm_t                                 coop_tload_dbg_sm_signals;
tstore_dbg_sm_t                                     tstore_dbg_sm_signals;
fln_dbg_sm_t                                        fln_dbg_sm_signals;

// Low voltage (internal) signals for Cooperative Tensor Load
coop_tload_slv_rdy_req_t                          coop_tload_slv_rdy_out_data_lv;
logic                                             coop_tload_slv_rdy_out_valid_lv;
coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]     coop_tload_slv_rdy_in_data_lv;
logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_slv_rdy_in_valid_lv;
logic [`SHIRE_COOP_ID_RANGE]                      coop_tload_mst_done_out_coop_id_lv;
logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_out_valid_lv;
logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]  coop_tload_mst_done_in_coop_id_lv;
logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_in_valid_lv;

// Reset pipeline signals
logic reset_w;
logic reset_w_dbg;
logic reset_w_neigh;
logic reset_w_neigh_ff;
logic reset_d_neigh_ff;
logic reset_c_neigh_ff;
logic reset_w_neigh_pmu;
logic tbox_en_lo;

//////////////////////////////////////////////////
// Generate resets
//////////////////////////////////////////////////

// Change voltage of Neigh clock
wire clock_lv;
assign clock_minion = clock_lv; 
assign clock_tbox   = clock_lv; 

logic reset_c_neigh;
logic reset_d_neigh;

sys_gasket_lov sys_gasket_rst_mngr (
  //inputs
  .clock                ( clock                          ), // high voltage clock
  .clock_fb_ref         ( clock_lv                       ), // high voltage clock
  .reset_warm_in        ( reset_warm                     ), // high voltage input 
  .gpio_ndmreset_in     ( 1'b0                           ), // high voltage input
  .ndmreset_in          ( dmctrl.ndmreset                ), // high voltage input
  .dmactive_in          ( dmctrl.dmactive                ), // high voltage input
  .reset_cold_in        ( reset_cold                     ), // high voltage input
  .dft__reset_byp       ( dft__reset_byp_hv              ), // high voltage input
  .dft__reset           ( dft__reset_hv                  ), // high voltage input
  .use_shire_tdr_reset_cntl (dft__use_reset_cntl_hv      ), // high voltage input 
  .shire_tdr_reset_cntl ( dft__reset_cntl_hv             ), // high voltage input
  .clock_lv             ( clock_lv                       ), // low voltage clock output
  .reset_w              ( reset_w                        ), // LoV reset for non-debug blocks (includes global reset and ndm_reset)  
  .reset_c              ( reset_c_neigh                  ), // LoV Global reset              
  .reset_d              ( reset_d_neigh                  ), // LoV reset for debug blocks (includes global reset and ndm_reset)   
  .dft__reset_byp_lv    ( dft__reset_byp_lv              ), // low voltage output
  .dft__reset_lv        ( dft__reset_lv                  )  // low voltage output 
);

// Local Pipeline stage of reset_w signal to neigh_ch_dbg
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_dbg_stg  (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w), .reset_out(reset_w_dbg));

// Local Pipeline stage of reset signals to Minions which have a pipeline stage in the Minions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w_neigh), .reset_out(reset_w_neigh_ff));
rst_repeat rst_repeat_reset_w_pmu_stg  (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w_neigh), .reset_out(reset_w_neigh_pmu));
rst_repeat rst_repeat_reset_d_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_d_neigh), .reset_out(reset_d_neigh_ff)); 
rst_repeat rst_repeat_reset_c_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_c_neigh), .reset_out(reset_c_neigh_ff)); 

// Pipeline stage of reset signals to Tbox
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_w_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_w));
rst_repeat rst_repeat_reset_d_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_d_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_d)); 
rst_repeat rst_repeat_reset_c_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_c_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_c)); 

//////////////////////////////////////////////////
// Memory override signals from TDR
/////////////////////////////////////////////////

// Just a level shifter instantiated
vclevel_shft_h2l #( 
  .WIDTH   ( `SHIRE_MINION_MEM_OVERRIDE_BITS ))
ls_h2l_mem_override_pd_hv (
  .inp_hv  ( esr_minion_mem_override    ),
  .out_lv  ( esr_minion_mem_override_lo )
);     

//////////////////////////////////////////////////
//APB MUX
/////////////////////////////////////////////////
APB_to_neigh_t                                      APB_req_from_shire_lo;
APB_from_neigh_t                                    APB_rsp_to_shire_lo;
APB_to_neigh_esrs_t                                 APB_req_struct_ESR   ;
APB_to_neigh_icache_t                               APB_req_struct_icache;
APB_from_neigh_channel_t                            APB_rsp_struct_ESR   ; 
APB_from_neigh_channel_t                            APB_rsp_struct_icache;

neigh_ch_apb_mux neigh_ch_apb_mux (
  // System signals
  .clock                  ( clock_lv              ),
  .reset                  ( reset_c_neigh_ff      ),

  // APB connection with the shire
  .APB_req_from_shire     ( APB_req_from_shire_lo ),
  .APB_rsp_to_shire       ( APB_rsp_to_shire_lo   ),
       
  // APB connection with the minions
  .minion_apb_prdata      ( minion_apb_prdata     ), 
  .minion_apb_pready      ( minion_apb_pready     ),
  .minion_apb_pslverr     ( minion_apb_pslverr    ),
  .minion_apb_penable     ( minion_apb_penable    ),
  .minion_apb_psel        ( minion_apb_psel       ),
  .minion_apb_pwrite      ( minion_apb_pwrite     ),
  .minion_apb_paddr       ( minion_apb_paddr      ),
  .minion_apb_pwdata      ( minion_apb_pwdata     ),

  // APB connection with ESR
  .APB_req_struct_ESR     ( APB_req_struct_ESR    ), 
  .APB_req_struct_icache  ( APB_req_struct_icache ),                   
  // APB connection with icache
  .APB_rsp_struct_ESR     ( APB_rsp_struct_ESR    ),                    
  .APB_rsp_struct_icache  ( APB_rsp_struct_icache ),

  // APB connection with TBOX
  .APB_req_struct_tbox    ( tbox_APB_req          ),
  .APB_rsp_struct_tbox    ( tbox_APB_rsp          )
);

////////////////////////////////////////////////////////////////////////////////
// Miss request Round-Robin arbiter to the intermediate/per-bank FIFOs
////////////////////////////////////////////////////////////////////////////////
logic [`MIN_PER_N-1:0] min_evict_second_access;
logic [`MIN_PER_N-1:0] min_evict_second_access_next;
logic                  min_evict_second_access_pending;
logic                  tstore_second_access;
logic                  tstore_second_access_next;
logic                  evict_second_access_pending;

// If evict requests are to write 512 bits, they must be assigned two consecutive grants,
// so that they are stored in two consecutive 256-bit FIFO entries
// Thus, we disable other bids during the second evict access
always_comb begin
  min_evict_second_access_next = min_evict_second_access;
  for (integer unsigned i = 0; i < `MIN_PER_N; i++) begin
    if (min_l2_req_ready[i] && min_l2_req_is_evict[i] &&
       (min_l2_req_info[i].size == ET_LINK_Line) && !min_evict_second_access[i]
       )
        min_evict_second_access_next[i] = 1'b1;
    else if (min_l2_req_ready[i] && min_evict_second_access[i])
        min_evict_second_access_next[i] = 1'b0;
  end 
    tstore_second_access_next = tstore_second_access;
    
    if (tstore_miss_req_ready && (tstore_evict_req.size == ET_LINK_Line) && !tstore_second_access)
        tstore_second_access_next = 1'b1;
    else if (tstore_miss_req_ready && tstore_second_access)
        tstore_second_access_next = 1'b0;
end

assign min_evict_second_access_pending = |min_evict_second_access_next;
assign evict_second_access_pending     = |{tstore_second_access_next,min_evict_second_access_next};

//       CLK       RST               DOUT                      DIN                           DEF
`RST_FF (clock_lv, reset_w_neigh_ff, min_evict_second_access,  min_evict_second_access_next, '0)
`RST_FF (clock_lv, reset_w_neigh_ff, tstore_second_access,     tstore_second_access_next,    1'b0)

// TensorStore requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_TSTORE +: 1]             = tstore_evict_valid;
// Cooperative TensorLoad requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_TLOAD +: 1]              = coop_tload_miss_req_valid;
// TBOX requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_TBOX +: 1]               = tbox_miss_req_valid;
// PTW requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_PTW +: `PTW_PER_N]       = ptw_l2_miss_req_valid;
// Icache requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_ICACHE +: `ICACHE_PER_N] = icache_l2_miss_req_valid;
// Minion requests to arbiter
assign miss_req_arb_bid[`NEIGH_AGENT_MINION +: `MIN_PER_N]    = min_l2_req_bid;

// Round-robin arbiter between the different requestors
arb_rr
#(
  .NUM_REQS ( `NEIGH_NUM_REQ_AGENTS )
) miss_req_arbiter (
  // System signals
  .clock    ( clock_lv              ),
  .reset    ( reset_w_neigh_ff      ),
  // Bidding
  .reqs     ( miss_req_arb_bid      ),
  .pop      ( miss_req_ready        ),
  // Grant
  .grants   ( miss_req_arb_grant    )
);

// Grant signals for requestors
// They indicate next granted requestor but do not necessarily grant
// current cycle (only if miss_req_ready is enabled)
assign min_l2_req_arb_grant             = miss_req_arb_grant[`NEIGH_AGENT_MINION +: `MIN_PER_N];
assign icache_l2_miss_req_arb_grant     = miss_req_arb_grant[`NEIGH_AGENT_ICACHE +: `ICACHE_PER_N];
assign ptw_l2_miss_req_arb_grant        = miss_req_arb_grant[`NEIGH_AGENT_PTW    +: `PTW_PER_N];
assign tbox_l2_miss_req_arb_grant       = miss_req_arb_grant[`NEIGH_AGENT_TBOX   +: 1];
assign coop_tload_l2_miss_req_arb_grant = miss_req_arb_grant[`NEIGH_AGENT_TLOAD  +: 1];
assign tstore_l2_miss_req_arb_grant     = miss_req_arb_grant[`NEIGH_AGENT_TSTORE +: 1];

// Ready signals for requestors
// They do grant this cycle to the indicated requestor
assign min_l2_req_ready          = miss_req_ready ? min_l2_req_arb_grant             : '0;
assign icache_l2_miss_req_ready  = miss_req_ready ? icache_l2_miss_req_arb_grant     : '0;
assign ptw_l2_miss_req_ready     = miss_req_ready ? ptw_l2_miss_req_arb_grant        : '0;
assign tbox_miss_req_ready       = miss_req_ready ? tbox_l2_miss_req_arb_grant       : '0;
assign coop_tload_miss_req_ready = miss_req_ready ? coop_tload_l2_miss_req_arb_grant : '0;
assign tstore_miss_req_ready     = miss_req_ready ? tstore_l2_miss_req_arb_grant     : '0;

// Miss arbiter request muxing
// Multiplexing is done in two stages, as a 16:1 is too big for timing closure
always_comb begin
  // Stage 1
  // -------
  // Multiplexer 0 (4:1 for minions 0-3)
  // -----------------------------------
  // Default to minion 0
  miss_req_info_xx_next[0] = min_l2_req_info[0];
  miss_req_data_xx_next[0] = min_l2_req_data[0];
      
  // Requests from other minions
  for(integer unsigned i = 1; i < `MIN_PER_N/2; i++)
  begin : miss_req_info_min0_3_for
      if(min_l2_req_arb_grant[i])
      begin
          miss_req_info_xx_next[0] = min_l2_req_info[i];
          miss_req_data_xx_next[0] = min_l2_req_data[i];
      end
  end

    // Valid for FIFOs
  miss_req_valid_xx_next[0] = |miss_req_arb_grant[`NEIGH_AGENT_MINION +: `MIN_PER_N/2];

  // Multiplexer 1 (4:1 for minions 4-7)
  // -----------------------------------
  // Default to minion 4
  miss_req_info_xx_next[1] = min_l2_req_info[`MIN_PER_N/2];
  miss_req_data_xx_next[1] = min_l2_req_data[`MIN_PER_N/2];
      
   // Requests from other minions
   for(integer unsigned i = `MIN_PER_N/2+1; i < `MIN_PER_N; i++) begin : miss_req_info_min4_7_for
     if(min_l2_req_arb_grant[i]) begin
       miss_req_info_xx_next[1] = min_l2_req_info[i];
       miss_req_data_xx_next[1] = min_l2_req_data[i];
     end
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[1] = |miss_req_arb_grant[`NEIGH_AGENT_MINION+`MIN_PER_N/2 +: `MIN_PER_N/2];

    // Multiplexer 2 (4:1 for Icache, PTWs and TBOX)
    // ---------------------------------------------
    // Default to miss request from Icache
    //if (icache_l2_miss_req_arb_grant)
    miss_req_info_xx_next[2].id        = icache_l2_miss_req_info.id;
    miss_req_info_xx_next[2].source    = ET_LINK_NEIGH_SRC_Icache;
    miss_req_info_xx_next[2].wdata     = icache_l2_miss_req_info.wdata;
    miss_req_info_xx_next[2].opcode    = icache_l2_miss_req_info.opcode;
    miss_req_info_xx_next[2].subopcode = icache_l2_miss_req_info.subopcode;
    miss_req_info_xx_next[2].address   = icache_l2_miss_req_info.address;
    miss_req_info_xx_next[2].size      = icache_l2_miss_req_info.size;
    miss_req_info_xx_next[2].qwen      = icache_l2_miss_req_info.qwen;
    // Data not used in this multiplexer
    miss_req_data_xx_next[2]           = '0;

    // Miss request from PTWs
    for (integer unsigned i = 0; i < `PTW_PER_N; i++) begin : miss_req_info_ptw_for
      if (ptw_l2_miss_req_arb_grant[i]) begin
        miss_req_info_xx_next[2].id        = ptw_l2_miss_req_info[i].id;
        miss_req_info_xx_next[2].source    = ET_LINK_NEIGH_SRC_Ptw0 + i[`PTW_PER_N_R];
        miss_req_info_xx_next[2].wdata     = ptw_l2_miss_req_info[i].wdata;
        miss_req_info_xx_next[2].opcode    = ptw_l2_miss_req_info[i].opcode;
        miss_req_info_xx_next[2].subopcode = ptw_l2_miss_req_info[i].subopcode;
        miss_req_info_xx_next[2].address   = ptw_l2_miss_req_info[i].address;
        miss_req_info_xx_next[2].size      = ptw_l2_miss_req_info[i].size;
        miss_req_info_xx_next[2].qwen      = ptw_l2_miss_req_info[i].qwen;
      end
    end

    // Miss request from TBOX
    if (tbox_l2_miss_req_arb_grant) begin
      miss_req_info_xx_next[2].id        = tbox_miss_req_info.id;
      miss_req_info_xx_next[2].source    = ET_LINK_NEIGH_SRC_Tbox;
      miss_req_info_xx_next[2].wdata     = tbox_miss_req_info.wdata;
      miss_req_info_xx_next[2].opcode    = tbox_miss_req_info.opcode;
      miss_req_info_xx_next[2].subopcode = tbox_miss_req_info.subopcode;
      miss_req_info_xx_next[2].address   = tbox_miss_req_info.address;
      miss_req_info_xx_next[2].size      = tbox_miss_req_info.size;
      miss_req_info_xx_next[2].qwen      = tbox_miss_req_info.qwen;
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[2] = |miss_req_arb_grant[`NEIGH_AGENT_ICACHE +: `ICACHE_PER_N + `PTW_PER_N + 1];

    // Multiplexer 3 (2:1 for cooperative TLoad and TStore)
    // ----------------------------------------------------
    // Default to miss request from cooperative TLoad
    //if (coop_tload_l2_miss_req_arb_grant)
    miss_req_info_xx_next[3].id        = coop_tload_miss_req_info.id;
    miss_req_info_xx_next[3].source    = ET_LINK_NEIGH_SRC_Coop;
    miss_req_info_xx_next[3].wdata     = coop_tload_miss_req_info.wdata;
    miss_req_info_xx_next[3].opcode    = coop_tload_miss_req_info.opcode;
    miss_req_info_xx_next[3].subopcode = coop_tload_miss_req_info.subopcode;
    miss_req_info_xx_next[3].address   = coop_tload_miss_req_info.address;
    miss_req_data_xx_next[3]           = coop_tload_miss_req_info.data;
    miss_req_info_xx_next[3].size      = coop_tload_miss_req_info.size;
    miss_req_info_xx_next[3].qwen      = coop_tload_miss_req_info.qwen;

    // Miss request from TStore
    if (tstore_l2_miss_req_arb_grant) begin
      miss_req_info_xx_next[3].id        = tstore_evict_req.id;
      miss_req_info_xx_next[3].source    = tstore_evict_req.source;
      miss_req_info_xx_next[3].wdata     = tstore_evict_req.wdata;
      miss_req_info_xx_next[3].opcode    = tstore_evict_req.opcode;
      miss_req_info_xx_next[3].subopcode = tstore_evict_req.subopcode;
      miss_req_info_xx_next[3].address   = tstore_evict_req.address;
      miss_req_data_xx_next[3]           = tstore_evict_req.data;
      miss_req_info_xx_next[3].size      = tstore_evict_req.size;
      miss_req_info_xx_next[3].qwen      = tstore_evict_req.qwen;
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[3] = |miss_req_arb_grant[`NEIGH_AGENT_TLOAD +: 2];

    // Stage 2: Multiplexer 4:1
    // ------------------------
    miss_req_info = miss_req_info_xx[0];
    miss_req_data = miss_req_data_xx[0];

    for (integer unsigned i = 1; i < 4; i++) begin
      if (miss_req_valid_xx[i])begin : miss_req_info_for
        miss_req_info = miss_req_info_xx[i];
        miss_req_data = miss_req_data_xx[i];
      end
    end

    // Valid for FIFOs
    miss_req_valid = |miss_req_valid_xx;

    // Miss request is not accepted:
    // - while intermediate FIFO is full if going to output FIFOs
    // - while TBOX FIFO is full if going to TBOX
    miss_req_ready = miss_req_goes_to_tbox ? min_tbox_req_ready
                                           : ~int_req_fifo_full;
end

assign miss_req_accepted_xx = miss_req_valid_xx_next & {4{miss_req_ready}};
assign miss_req_accepted    = miss_req_valid & miss_req_ready;

for (genvar miss_req_xx_idx = 0; miss_req_xx_idx < 4; miss_req_xx_idx++) begin : MISS_REQ_INFO_XX_FF
  //          CLK       RST               EN                                                                                    DOUT                                DIN                                      DEF
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_xx[miss_req_xx_idx],                                                miss_req_info_xx[miss_req_xx_idx],  miss_req_info_xx_next[miss_req_xx_idx],  '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_xx[miss_req_xx_idx] & miss_req_info_xx_next[miss_req_xx_idx].wdata, miss_req_data_xx[miss_req_xx_idx],  miss_req_data_xx_next[miss_req_xx_idx],  '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_ready,                                                                       miss_req_valid_xx[miss_req_xx_idx], miss_req_valid_xx_next[miss_req_xx_idx], 1'b0)
end

//          CLK    RST               EN                                       DOUT                DIN             DEF
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted,                       miss_req_info_ff,   miss_req_info,  '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted & miss_req_info.wdata, miss_req_data_ff,   miss_req_data,  '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_ready,                          miss_req_valid_ff,  miss_req_valid, 1'b0)

assign miss_req_accepted_ff = miss_req_valid_ff & miss_req_ready;

// Process request to adapt it to output format before being sent
always_comb begin
  miss_req_info_ff_proc_next = miss_req_info_ff;
  miss_req_data_ff_proc_next = miss_req_data_ff;

  // If it is a WriteAround, convert it into regular write
  // (WriteArounds not supported in Erbium project)
  if (miss_req_info_ff_proc_next.opcode == ET_LINK_REQ_WriteAround)
    miss_req_info_ff_proc_next.opcode = ET_LINK_REQ_Write;
end

// Miss request destination FIFO
assign miss_req_info_dest_fifo_next = destFifo(miss_req_info_ff_proc_next,shire_id_lo[`AD_ESR_SHIRE_ID_SIZE-1:0],esr_neigh_chicken);

// Check whether the miss request goes into the TBOX FIFO
assign miss_req_goes_to_tbox_next = miss_req_info_dest_fifo_next == DEST_FIFOS_L'(TBOX_FIFO);

//          CLK       RST               EN                                                       DOUT                     DIN                           DEF
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_ff,                                    miss_req_info_ff_proc,   miss_req_info_ff_proc_next,   '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_ff & miss_req_info_ff_proc_next.wdata, miss_req_data_ff_proc,   miss_req_data_ff_proc_next,   '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_ff,                                    miss_req_info_dest_fifo, miss_req_info_dest_fifo_next, '0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_accepted_ff,                                    miss_req_goes_to_tbox,   miss_req_goes_to_tbox_next,   1'b0)
`RST_EN_FF (clock_lv, reset_w_neigh_ff, miss_req_ready,                                          miss_req_valid_ff_proc,  miss_req_valid_ff,            1'b0)

////////////////////////////////////////////////////////////////////////////////
// Intermediate FIFO before going to output (per-bank/UC) FIFOs
////////////////////////////////////////////////////////////////////////////////

// The miss request goes into the intermediate FIFO if:
// - It does not go to TBOX FIFO and
// - There is not room in the corresponding output FIFO or
// - There are pending requests to the same output FIFO in the intermediate FIFO or
// - The intermediate FIFO is writing to any output FIFO
assign miss_req_goes_to_int_fifo = !miss_req_goes_to_tbox &&
                                   (out_req_fifo_full[miss_req_info_dest_fifo[OUT_FIFOS_L-1:0]] ||
                                    (int_req_fifo_counter[miss_req_info_dest_fifo[OUT_FIFOS_L-1:0]] != '0) ||
                                    int_req_info_pop);

// Miss request is pushed into the intermediate FIFO if it goes there
assign int_req_push       = miss_req_valid_ff_proc & miss_req_goes_to_int_fifo & ~int_req_fifo_full;
// Generate early valid for intermediate FIFO
assign int_req_push_early = miss_req_accepted_ff | (miss_req_valid_ff_proc & miss_req_goes_to_int_fifo & int_req_fifo_full);
// FIFO push data
assign int_req_info_in = miss_req_info_ff_proc;
assign int_req_data_in = miss_req_data_ff_proc;

// FIFO push data output FIFO
assign int_req_info_push_out_fifo = miss_req_info_dest_fifo[OUT_FIFOS_L-1:0];

always_comb begin
  for (integer fifo_idx = 0; fifo_idx < OUT_FIFOS; fifo_idx++) begin: int_fifo_per_out_fifo_push_pop_for
    // FIFO push per output FIFO
    int_req_push_per_out_fifo[fifo_idx] = int_req_push && (int_req_info_push_out_fifo == fifo_idx[OUT_FIFOS_L-1:0]);
        // FIFO pop per output FIFO
    int_req_pop_per_out_fifo[fifo_idx] = int_req_info_pop && (int_req_info_pop_out_fifo == fifo_idx[OUT_FIFOS_L-1:0]);
  end
end

// Destination FIFO counters
//       CLK       RST               DOUT                  DIN                        DEF
`RST_FF (clock_lv, reset_w_neigh_ff, int_req_fifo_counter, int_req_fifo_counter_next, '0)

always_comb begin
  int_req_fifo_counter_next = int_req_fifo_counter;
  for (integer fifo_idx = 0; fifo_idx < OUT_FIFOS; fifo_idx++) begin: int_fifo_counter
    // Incremented when pushing an element for this FIFO
    if (int_req_push_per_out_fifo[fifo_idx] && !int_req_pop_per_out_fifo[fifo_idx])
        int_req_fifo_counter_next[fifo_idx] = int_req_fifo_counter[fifo_idx] + 1'b1;
    // Decremented when popping an element for this FIFO
    else if (!int_req_push_per_out_fifo[fifo_idx] && int_req_pop_per_out_fifo[fifo_idx])
        int_req_fifo_counter_next[fifo_idx] = int_req_fifo_counter[fifo_idx] - 1'b1;
  end
end

logic int_req_info_fifo_full_next_unused_ok;
logic int_req_info_fifo_valid_next_unused_ok;
gen_fifo #(
  .ELEM_SIZE   ( OUT_FIFOS_L +
                 $bits(et_link_nodata_req_info_t) ),
  .NUM_ELEMS   ( `NEIGH_INT_FIFO_SIZE             ),
  .USE_LATCHES ( 1                                )
) int_req_info_fifo (
  // System signals
  .clock       ( clock_lv                         ),
  .reset       ( reset_w_neigh_ff                 ),
  // Push data
  .push_early  ( int_req_push_early               ),
  .push        ( int_req_push                     ),
  .push_data   ( {int_req_info_push_out_fifo,
                  int_req_info_in}                ),
  .full        ( int_req_fifo_full                ),
  .full_next   ( int_req_info_fifo_full_next_unused_ok ),
  // Pop data
  .pop         ( int_req_info_pop                 ),
  .pop_data    ( {int_req_info_pop_out_fifo,
                 int_req_info_out}                ),
  .valid       ( int_req_info_valid               ),
  .valid_next  ( int_req_info_fifo_valid_next_unused_ok )
);

logic int_req_data_fifo_full_unused_ok;
logic int_req_data_fifo_full_next_unused_ok;
logic int_req_data_fifo_valid_unused_ok;
logic int_req_data_fifo_valid_next_unused_ok;
gen_fifo #(
  .ELEM_SIZE   ( `NEIGH_DATA_SIZE      ),
  .NUM_ELEMS   ( `NEIGH_INT_FIFO_SIZE  ),
  .USE_LATCHES ( 1                     )
) int_req_data_fifo (
  // System signals
  .clock       ( clock_lv              ),
  .reset       ( reset_w_neigh_ff      ),
  // Push data
  .push_early  ( int_req_push_early    ),
  .push        ( int_req_push & 
                 int_req_info_in.wdata ),
  .push_data   ( int_req_data_in       ),
  .full        ( int_req_data_fifo_full_unused_ok ),
  .full_next   ( int_req_data_fifo_full_next_unused_ok ),
  // Pop data
  .pop         ( int_req_data_pop      ),
  .pop_data    ( int_req_data_out      ),
  .valid       ( int_req_data_fifo_valid_unused_ok ),
  .valid_next  ( int_req_data_fifo_valid_next_unused_ok )
);


// Miss request is accepted into the destination FIFO if it is not full
assign int_req_info_pop = int_req_info_valid && !out_req_fifo_full[int_req_info_pop_out_fifo];
assign int_req_data_pop = int_req_info_pop & int_req_info_out.wdata;

// Output FIFO push
always_comb begin
  for (integer unsigned fifo_idx = 0; fifo_idx < OUT_FIFOS; fifo_idx++)
    // Miss request is accepted into this FIFO if it is accepted from either
    // the intermediate FIFO or the miss request arbiter
    out_req_push[fifo_idx] = (int_req_info_pop && (int_req_info_pop_out_fifo == fifo_idx[OUT_FIFOS_L-1:0])) ||
                             (!int_req_info_pop && miss_req_valid_ff_proc && miss_req_ready && !miss_req_goes_to_int_fifo && (miss_req_info_dest_fifo == fifo_idx[DEST_FIFOS_L-1:0]));
end

// Output FIFO push data
assign out_req_info_in = int_req_info_pop ? int_req_info_out : miss_req_info_ff_proc;
assign out_req_data_in = int_req_data_pop ? int_req_data_out : miss_req_data_ff_proc;

// Check whether this is one half of a full line evict
// ReadCoop operation carries configuration in the data field but specified size is for read data returned in the response
assign out_req_is_line_evict = out_req_info_in.wdata && (out_req_info_in.size == ET_LINK_Line) && (out_req_info_in.opcode != ET_LINK_REQ_ReadCoop);

////////////////////////////////////////////////////
// Per-bank FIFOs that store the requests to the L2 
////////////////////////////////////////////////////

generate for (genvar l2_bank = 0; l2_bank < `SC_BANKS; l2_bank++) begin: BANK_FIFOS
  logic                     bank_req_info_push;
  logic                     bank_req_in_is_first_half;
  logic                     bank_req_is_second_half;
  logic                     bank_req_data_push;
  logic                     bank_req_info_pop;
  logic                     bank_req_data_pop;
  logic                     bank_req_out_is_first_half;
  et_link_nodata_req_info_t bank_req_info_out;
  logic [`NEIGH_DATA_RANGE] bank_req_data_out;
    
  logic                     bank_req_info_valid;
    
  // Push logic for LV
  // -------------------------------------------------------------------------
  //          CLK       RST               EN                                          DOUT                     DIN                       DEF
  `RST_EN_FF (clock_lv, reset_w_neigh_ff, bank_req_info_push & out_req_is_line_evict, bank_req_is_second_half, ~bank_req_is_second_half, 1'b0)

   assign bank_req_info_push         = out_req_push[l2_bank];
   assign bank_req_data_push         = out_req_push[l2_bank] & out_req_info_in.wdata;
   // Obtain and store whether this is the first half of a full line evict
   assign bank_req_in_is_first_half  = out_req_is_line_evict & ~bank_req_is_second_half;

    // -------------------------------------------------------------------------
    // ET-Link request info FIFO
    logic bank_req_info_vcfifo_wr_lov_full_next_unused_ok;
    logic bank_req_info_vcfifo_wr_lov_valid_next_unused_ok;
    gen_fifo  #(
      .ELEM_SIZE ( $bits(et_link_nodata_req_info_t) + 1           ),
      .NUM_ELEMS ( `NEIGH_BANK_FIFO_SIZE                          )
    ) bank_req_info_vcfifo_wr_lov (
      // System signals
      .clock      ( clock_lv                                       ),
      .reset      ( reset_w_neigh_ff                               ),
      // Push data
      .push_early ( 1'b0                                           ), // Unused
      .push       ( bank_req_info_push                             ),
      .push_data  ( {bank_req_in_is_first_half,out_req_info_in}    ),
      .full       ( out_req_fifo_full[l2_bank]                     ),
      .full_next  ( bank_req_info_vcfifo_wr_lov_full_next_unused_ok ), // Unused
      // Pop data
      .pop        ( bank_req_info_pop                              ),
      .pop_data   ( {bank_req_out_is_first_half,bank_req_info_out} ),
      .valid      ( bank_req_info_valid                            ),
      .valid_next ( bank_req_info_vcfifo_wr_lov_valid_next_unused_ok )  // Unused
    );

    // ET-Link request data FIFO
    logic bank_req_data_bank_vcfifo_wr_lov_full_unused_ok;
    logic bank_req_data_bank_vcfifo_wr_lov_full_next_unused_ok;
    logic bank_req_data_bank_vcfifo_wr_lov_valid_unused_ok;
    logic bank_req_data_bank_vcfifo_wr_lov_valid_next_unused_ok;
    gen_fifo #(
      .ELEM_SIZE ( `NEIGH_DATA_SIZE      ),
      .NUM_ELEMS ( `NEIGH_BANK_FIFO_SIZE )
    ) bank_req_data_bank_vcfifo_wr_lov (
      // System signals
      .clock      ( clock_lv           ),
      .reset      ( reset_w_neigh_ff   ),
      // Push data
      .push_early ( 1'b0               ), // Unused
      .push       ( bank_req_data_push ),
      .push_data  ( out_req_data_in    ),
      .full       ( bank_req_data_bank_vcfifo_wr_lov_full_unused_ok ), // Unused
      .full_next  ( bank_req_data_bank_vcfifo_wr_lov_full_next_unused_ok ), // Unused
      // Pop data
      .pop        ( bank_req_data_pop  ),
      .pop_data   ( bank_req_data_out  ),
      .valid      ( bank_req_data_bank_vcfifo_wr_lov_valid_unused_ok ), // Unused
      .valid_next ( bank_req_data_bank_vcfifo_wr_lov_valid_next_unused_ok )  // Unused
    );

    // Pop logic for HV
    // -------------------------------------------------------------------------
    neigh_hv_logic_l2_bank neigh_hv_logic_l2_bank_pd_hv (
      .clock                      ( clock_lv                     ),
      .reset                      ( reset_w_neigh_ff             ),
      // BANK FIFO output (256 bits)
      .bank_req_out_is_first_half ( bank_req_out_is_first_half   ),
      .bank_req_info_out          ( bank_req_info_out            ),
      .bank_req_data_out          ( bank_req_data_out            ),
      .bank_req_info_valid        ( bank_req_info_valid          ),
      .bank_req_info_pop          ( bank_req_info_pop            ),
      .bank_req_data_pop          ( bank_req_data_pop            ),
      // BANK FIFO output (512 bits)
      .miss_req_bank_info         ( miss_req_bank_info[l2_bank]  ),
      .miss_req_bank_valid        ( miss_req_bank_valid[l2_bank] ),
      .miss_req_bank_ready        ( miss_req_bank_ready[l2_bank] )
    );

end
endgenerate

///////////////////////////////////////////////////
// UC FIFO that stores requests to uncached blocks 
///////////////////////////////////////////////////
logic                     uc_req_info_push;
logic                     uc_req_data_push;
et_link_nodata_req_info_t uc_req_info_in;
logic [`UC_DATA_SIZE-1:0] uc_req_data_in;
logic                     uc_req_info_pop;
logic                     uc_req_data_pop;
et_link_nodata_req_info_t uc_req_info_out;
logic [`UC_DATA_SIZE-1:0] uc_req_data_out;
logic                     uc_req_info_pop_valid;

// UC FIFO push data
assign uc_req_info_push = out_req_push[UC_FIFO];
assign uc_req_data_push = out_req_push[UC_FIFO] & uc_req_info_in.wdata;
assign uc_req_info_in   = out_req_info_in;
assign uc_req_data_in   = out_req_data_in;

// UC request info FIFO
logic uc_req_info_vcfifo_wr_lov_full_next_unused_ok;
logic uc_req_info_vcfifo_wr_lov_valid_next_unused_ok;
gen_fifo #(
   .ELEM_SIZE          ( $bits(et_link_nodata_req_info_t) ),
   .NUM_ELEMS          ( `NEIGH_BANK_FIFO_SIZE            )
) uc_req_info_vcfifo_wr_lov (
  // System signals
  .clock      ( clock_lv                   ),
  .reset      ( reset_w_neigh_ff           ),
  // Push data
  .push_early ( 1'b0                       ), // Unused
  .push       ( uc_req_info_push           ),
  .push_data  ( uc_req_info_in             ),
  .full       ( out_req_fifo_full[UC_FIFO] ),
  .full_next  ( uc_req_info_vcfifo_wr_lov_full_next_unused_ok ), // Unused
  // Pop data
  .pop        ( uc_req_info_pop            ),
  .pop_data   ( uc_req_info_out            ),
  .valid      ( uc_req_info_pop_valid      ),
  .valid_next ( uc_req_info_vcfifo_wr_lov_valid_next_unused_ok )  // Unused
);

// UC request data FIFO
logic uc_req_data_vcfifo_wr_lov_full_unused_ok;
logic uc_req_data_vcfifo_wr_lov_full_next_unused_ok;
logic uc_req_data_vcfifo_wr_lov_valid_unused_ok;
logic uc_req_data_vcfifo_wr_lov_valid_next_unused_ok;
gen_fifo #(
  .ELEM_SIZE ( `NEIGH_DATA_SIZE      ),
  .NUM_ELEMS ( `NEIGH_BANK_FIFO_SIZE )
)
uc_req_data_vcfifo_wr_lov(
  // System signals
  .clock      ( clock_lv         ),
  .reset      ( reset_w_neigh_ff ),
  // Push data
  .push_early ( 1'b0             ), // Unused
  .push       ( uc_req_data_push ),
  .push_data  ( uc_req_data_in   ),
  .full       ( uc_req_data_vcfifo_wr_lov_full_unused_ok ), // Unused
  .full_next  ( uc_req_data_vcfifo_wr_lov_full_next_unused_ok ), // Unused
  // Pop data
  .pop        ( uc_req_data_pop  ),
  .pop_data   ( uc_req_data_out  ),
  .valid      ( uc_req_data_vcfifo_wr_lov_valid_unused_ok ), // Unused
  .valid_next ( uc_req_data_vcfifo_wr_lov_valid_next_unused_ok )  // Unused
);

///////////////////////////////////////////////////////////////////////////////////
// Logic for HV interface:
// - UC FIFO pop logic
// - BANK/UC FIFO output request arbiter to the L2
///////////////////////////////////////////////////////////////////////////////////
neigh_hv_logic_neigh_sc neigh_hv_logic_neigh_sc_pd_hv
(
  .clock                 ( clock_lv              ),
  .reset                 ( reset_w_neigh_ff      ),
   // BANK FIFO outputs (512 bits)
  .miss_req_bank_info    ( miss_req_bank_info    ),
  .miss_req_bank_valid   ( miss_req_bank_valid   ),
  .miss_req_bank_ready   ( miss_req_bank_ready   ),
   // UC FIFO output
  .uc_req_info_out       ( uc_req_info_out       ),
  .uc_req_data_out       ( uc_req_data_out       ),
  .uc_req_info_pop_valid ( uc_req_info_pop_valid ),
  .uc_req_info_pop       ( uc_req_info_pop       ),
  .uc_req_data_pop       ( uc_req_data_pop       ),
  // Output to Shire Cache
  .neigh_sc_req_info     ( neigh_sc_req_info     ),
  .neigh_sc_req_valid    ( neigh_sc_req_valid    ),
  .neigh_sc_req_ready    ( neigh_sc_req_ready    )
);

////////////////////////////////////////////////////////////////////////////////
// Arbiter and FIFO for L2/TBOX responses
////////////////////////////////////////////////////////////////////////////////
assign neigh_sc_rsp_ready_lo = fill_fifo_in_rsp_ready;

//         CLK    RST               EN                                              DOUT                    DIN                    DEF
`EN_FF    (clock_lv,                   fill_fifo_in_rsp_ready & neigh_sc_rsp_valid_lo, fill_fifo_in_rsp_info,  neigh_sc_rsp_info_lo)
`RST_EN_FF(clock_lv, reset_w_neigh_ff, fill_fifo_in_rsp_ready,                         fill_fifo_in_rsp_valid, neigh_sc_rsp_valid_lo, 1'b0)

// FIFO
assign fill_fifo_in_rsp_ready = ~fill_fifo_full;
assign fill_fifo_push         = fill_fifo_in_rsp_valid & ~fill_fifo_full;

// Obtain cooperative information from cooperative TLoad tag table
assign coop_tload_tag_table_cam_pop = fill_fifo_push
                                    & (fill_fifo_in_rsp_info.opcode != ET_LINK_RSP_MsgRcvData)
                                    & (et_link_neigh_source_t'(fill_fifo_in_rsp_info.dest) == ET_LINK_NEIGH_SRC_Coop);
// Take tag to CAM from fill response id field
assign coop_tload_tag_table_cam_tag = fill_fifo_in_rsp_info.id;

neigh_fill_fifo fill_fifo
(
  // System signals
  .clock         ( clock_lv                      ),
  .reset         ( reset_w_neigh_ff              ),
  // Push data       
  .push          ( fill_fifo_push                ),
  .push_data     ( fill_fifo_in_rsp_info         ),
  .coop_data     ( coop_tload_tag_table_cam_data ),
  .full          ( fill_fifo_full                ),
  // Pop data    
  .out_rsp_ready ( fill_ff_available             ),
  .out_rsp_valid ( fill_fifo_out_rsp_valid       ),
  .out_rsp_info  ( fill_fifo_out_rsp_info        )
);

////////////////////////////////////////////////////////////////////////////////
// This is the icache, shared between all the minions in the neigh
////////////////////////////////////////////////////////////////////////////////
esr_icache_err_log_ctl_t        esr_icache_err_log_ctl;
logic                           esr_icache_err_log_sbe;
logic                           esr_icache_err_log_dbe;
icache_err_log_info_t           esr_icache_err_log_info;
logic                           esr_icache_ecc_count_ov;
minion_ptw_req [`PTW_PER_N-1:0] ic_ptw_req_data;
logic [`PTW_PER_N-1:0]          ic_ptw_req_valid;
logic [`PTW_PER_N-1:0]          ic_ptw_req_ready;
logic [`PTW_PER_N-1:0]          ic_ptw_invalidate;
minion_ptw_pte [`PTW_PER_N-1:0] ptw_ic_resp_data;
logic [`PTW_PER_N-1:0]          ptw_ic_resp_valid;
logic                           icache_l2_miss_resp_ready;
logic                           icache_l2_miss_resp_valid;
et_link_rsp_info_t              icache_l2_miss_resp_info;

neigh_shared_icache #(
  .NR_MINIONS                  ( `MIN_PER_ICACHE                   ),
  .NR_REQUESTORS               ( `ICACHE_NR_REQS                   )
) shared_icache (
  // System signals
  .clock                       ( clock_lv                          ),
  .reset                       ( reset_w_neigh_ff                  ),
  // ESRs
  .esr_prefetch_conf           ( icache_prefetch_conf_t'(esr_icache_prefetch_conf_lo) ),
  .esr_prefetch_start          ( esr_icache_prefetch_start_lo      ),
  .esr_prefetch_done           ( esr_icache_prefetch_done_lo       ),
  .esr_err_log_ctl             ( esr_icache_err_log_ctl            ),
  .esr_err_log_sbe             ( esr_icache_err_log_sbe            ),
  .esr_err_log_dbe             ( esr_icache_err_log_dbe            ),
  .esr_err_log_info            ( esr_icache_err_log_info           ),
  .esr_mprot                   ( mprot                             ),
  .esr_vmspagesize             ( vmspagesize                       ),
  .esr_bypass_icache           ( esr_neigh_chicken.bypass_icache   ),
  .esr_shire_coop_mode         ( esr_shire_coop_mode_lo            ),
  // Request port
  .f0_req_ready                ( icache_req_ready                  ),
  .f0_req_valid                ( icache_req_valid                  ),
  .f0_req                      ( icache_req                        ),
  // Response
  .f4_resp_valid               ( icache_resp_valid                 ),
  .f4_resp_miss                ( icache_resp_miss                  ),
  .f4_resp                     ( icache_resp                       ),
  .f5_resp_fill_done           ( icache_fill_done                  ),
  // Flush control
  .f0_flush_data               ( icache_flush_data                 ),
  // Request to L2 port
  .f0_l2_miss_req_disable_next ( evict_second_access_pending       ),
  .f0_l2_miss_req_ready        ( icache_l2_miss_req_ready          ),
  .f0_l2_miss_req_valid        ( icache_l2_miss_req_valid          ),
  .f0_l2_miss_req              ( icache_l2_miss_req_info           ),
  // Fill from L2 port
  .f0_l2_miss_resp_ready       ( icache_l2_miss_resp_ready         ),
  .f0_l2_miss_resp_valid       ( icache_l2_miss_resp_valid         ),
  .f0_l2_miss_resp             ( icache_l2_miss_resp_info          ),
  .satp_info                   ( satp_info                         ),
  .matp_info                   ( matp_info                         ),
  .tlb_invalidate              ( tlb_invalidate                    ),
  // PTW request
  .ptw_req_data                ( ic_ptw_req_data                   ),
  .ptw_req_valid               ( ic_ptw_req_valid                  ),
  .ptw_req_ready               ( ic_ptw_req_ready                  ),
  .ptw_invalidate              ( ic_ptw_invalidate                 ),
  // PTW response
  .ptw_resp_valid              ( ptw_ic_resp_valid                 ),
  .ptw_resp_data               ( ptw_ic_resp_data                  ),
  // Request to L1 SRAM blocks
  .icache_f2_sram_req_write    ( icache_f2_sram_req_write_lo       ),
  .icache_f2_sram_req_addr     ( icache_f2_sram_req_addr_lo        ),
  .icache_f2_sram_req_valid    ( icache_f2_sram_req_valid_lo       ),
  .icache_f2_sram_req_ready    ( icache_f2_sram_req_ready_lo       ),
  // Response from L1 SRAM blocks
  .icache_f0_sram_resp_dout    ( icache_f0_sram_resp_dout_lo       ),
  .icache_f0_sram_resp_valid   ( icache_f0_sram_resp_valid_lo      ),
  .icache_f0_sram_resp_ready   ( icache_f0_sram_resp_ready_lo      ),
  // APB signals from BPAM for debug 
  .apb_paddr                   ( APB_req_struct_icache.apb_paddr   ),
  .apb_pwrite                  ( APB_req_struct_icache.apb_pwrite  ),
  .apb_psel                    ( APB_req_struct_icache.apb_psel    ),
  .apb_penable                 ( APB_req_struct_icache.apb_penable ),
  .apb_pwdata                  ( APB_req_struct_icache.apb_pwdata  ),
  .apb_pready                  ( APB_rsp_struct_icache.apb_pready  ),
  .apb_prdata                  ( APB_rsp_struct_icache.apb_prdata  ),
  .apb_pslverr                 ( APB_rsp_struct_icache.apb_pslverr ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals              ( icache_dbg_sm_signals             )
); //shared_icache

assign icache_l2_miss_resp_info = fill_fifo_out_rsp_info[`NEIGH_AGENT_ICACHE];
assign icache_l2_miss_resp_valid = fill_fifo_out_rsp_valid[`NEIGH_AGENT_ICACHE];
assign fill_ff_available[`NEIGH_AGENT_ICACHE] = icache_l2_miss_resp_ready;

////////////////////////////////////////////////////////////////////////////////
// This is the PTW, shared between all the minions in the neigh
////////////////////////////////////////////////////////////////////////////////
et_link_rsp_info_t [`PTW_PER_N-1:0] ptw_l2_miss_resp_info;
logic [`PTW_PER_N-1:0]              ptw_l2_miss_resp_valid;
logic [`PTW_PER_N-1:0]              ptw_l2_miss_resp_ready;
minion_ptw_pte [`PTW_PER_N-1:0]     ptw_resp_data;


for (genvar ptw_it = 0 ; ptw_it < `PTW_PER_N ; ptw_it ++ ) begin : PTW
    neigh_shared_ptw #(
      .N_REQS                    ( `MIN_PER_PTW + 1                                       )
    ) shared_ptw (
      // System signals
      .clock                     ( clock_lv                                               ),
      .reset                     ( reset_w_neigh_ff                                       ),
      // Is this in the IOShire
      .ioshire                   ( 1'b0                                                   ),
      // Requests
      .req_data                  ( {ic_ptw_req_data[ptw_it], dc_ptw_req_data[ptw_it]}     ),
      .req_valid                 ( {ic_ptw_req_valid[ptw_it], dc_ptw_req_valid[ptw_it]}   ),
      .req_ready                 ( {ic_ptw_req_ready[ptw_it], dc_ptw_req_ready[ptw_it]}   ),
      // Invalidates from dcache are not necessary,
      // as its origin is the same one as that from icache
      .invalidate                ( {ic_ptw_invalidate[ptw_it], (`MIN_PER_PTW)'(0)}        ),
      // Neighborhood ESRs       
      .mprot                     ( mprot                                                  ),
      .vmspagesize               ( vmspagesize                                            ),
      // Responses
      .resp_data                 ( ptw_resp_data[ptw_it]                                  ),
      .resp_valid                ( {ptw_ic_resp_valid[ptw_it], ptw_dc_resp_valid[ptw_it]} ),
      // Memory interface
      // L2 Request bus
      .mem_miss_req_disable_next ( evict_second_access_pending                            ),
      .mem_miss_req_info         ( ptw_l2_miss_req_info[ptw_it]                           ),
      .mem_miss_req_valid        ( ptw_l2_miss_req_valid[ptw_it]                          ),
      .mem_miss_req_ready        ( ptw_l2_miss_req_ready[ptw_it]                          ),
      // L2 Fill/Ack bus
      .mem_fill_req_info         ( ptw_l2_miss_resp_info[ptw_it]                          ),
      .mem_fill_req_valid        ( ptw_l2_miss_resp_valid[ptw_it]                         ),
      .mem_fill_req_ready        ( ptw_l2_miss_resp_ready[ptw_it]                         ),
      // Output debug signals going to Status Monitor
      .dbg_sm_signals            ( ptw_dbg_sm_signals[ptw_it]                             )
    );

    assign ptw_dc_resp_data[ptw_it] = ptw_resp_data[ptw_it];
    assign ptw_ic_resp_data[ptw_it] = ptw_resp_data[ptw_it];

    assign ptw_l2_miss_resp_info[ptw_it] = fill_fifo_out_rsp_info[`NEIGH_AGENT_PTW + ptw_it];
    assign ptw_l2_miss_resp_valid[ptw_it] = fill_fifo_out_rsp_valid[`NEIGH_AGENT_PTW + ptw_it];
    assign fill_ff_available[`NEIGH_AGENT_PTW + ptw_it] = ptw_l2_miss_resp_ready[ptw_it];

end // block: PTW


////////////////////////////////////////////////////////////////////////////////
// Fast local message minion interconnection
////////////////////////////////////////////////////////////////////////////////
logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_channel_mask;
logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0][`MIN_PER_N_R] local_dest_id;
logic [`MIN_PER_N-1:0]                                             local_message_ready_out;
logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_message_valid_out;
et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]                          local_message_info_out;
logic [`MIN_PER_N-1:0]                                             local_message_ready_in;
logic [`MIN_PER_N-1:0]                                             local_message_valid_in;
et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]                          local_message_info_in;

neigh_local_message_network local_message_network (
  // System signals
  .clock                   ( clock_lv                ),
  .reset                   ( reset_w_neigh_ff        ),
  // Configuration signals
  .local_channel_mask      ( local_channel_mask      ),
  .local_dest_id           ( local_dest_id           ),
  // Local message outputs from evict FFs
  .local_message_ready_out ( local_message_ready_out ),
  .local_message_valid_out ( local_message_valid_out ),
  .local_message_info_out  ( local_message_info_out  ),
  // Local message inputs to fill FFs
  .local_message_ready_in  ( local_message_ready_in  ),
  .local_message_valid_in  ( local_message_valid_in  ),
  .local_message_info_in   ( local_message_info_in   ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals          ( fln_dbg_sm_signals      )
);

////////////////////////////////////////////////////////////////////////////////
// Generates the minions
////////////////////////////////////////////////////////////////////////////////
esr_hactrl_t           esr_hactrl;
esr_hastatus0_t        esr_hastatus0;
esr_hastatus1_t        esr_hastatus1_min;
logic [`MIN_PER_N-1:0] l2_dcache_resp_tstore_ack;
logic [`MIN_PER_N-1:0] esr_thread0_enable_lo;
logic [`MIN_PER_N-1:0] esr_thread1_enable_lo;

for(genvar min_it = 0; min_it < `MIN_PER_N; min_it++) begin : MINION_FF
  logic [`MIN_PER_N_R]            neigh_min_id;

    // Minion ID within the neighborhood
  assign neigh_min_id = min_it[`MIN_PER_N_R];

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has its own FF to store a miss request. It waits until the
    // request is stored before going to arbiter to get access to the L2.
    // A given minion can't request a new cacheline until the FF is emptied
    ////////////////////////////////////////////////////////////////////////////
    neigh_miss_ff miss_ff (
      // System signals
      .clock                          ( clock_lv                         ),
      .reset                          ( reset_w_neigh_ff                 ),
      // Miss request from minion
      .l2_dcache_req                  ( l2_dcache_miss_req[min_it]       ),
      .l2_dcache_req_valid            ( l2_dcache_miss_req_valid[min_it] ),
      .l2_dcache_req_ready            ( l2_dcache_miss_req_ready[min_it] ),
      // Data to L2
      .miss_req                       ( min_miss_req[min_it]             ),
      .miss_valid                     ( min_miss_req_valid[min_it]       ),
      .miss_ready                     ( min_miss_req_ready[min_it]       ),
      // Data to cooperative tload module
      .coop_miss_req                  ( min_coop_miss_req[min_it]        ),
      .coop_miss_valid                ( min_coop_miss_req_valid[min_it]  ),
      .coop_miss_ready                ( min_coop_miss_req_ready[min_it]  ),
      // Output debug signals going to Status Monitor
      .dbg_sm_signals                 ( miss_ff_dbg_sm_signals[min_it]   )
    );

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has its own FF to store the evict. It waits until the 512b
    // of data are available before going to arbiter to get access to the L2.
    // A given minion can't evict a new cacheline until the FF is emptied
    ////////////////////////////////////////////////////////////////////////////

    neigh_evict_ff evict_ff (
      // System signals
      .clock                  ( clock_lv                           ),
      .reset                  ( reset_w_neigh_ff                   ),
      .shire_id               ( shire_id_lo[`AD_ESR_SHIRE_ID_SIZE-1:0] ),
      .neigh_id               ( neigh_id_lo                        ),
      // Evict from minion
      .l2_dcache_req          ( l2_dcache_evict_req[min_it]        ),
      .l2_dcache_req_valid    ( l2_dcache_evict_req_valid[min_it]  ),
      .l2_dcache_req_ready    ( l2_dcache_evict_req_ready[min_it]  ),
      // Reduce local interconnect
      .local_channel_mask     ( local_channel_mask[min_it]         ),
      .local_dest_id          ( local_dest_id[min_it]              ),
      .local_message_ready    ( local_message_ready_out[min_it]    ),
      .local_message_valid    ( local_message_valid_out[min_it]    ),
      .local_message_info     ( local_message_info_out[min_it]     ),
      // Data to L2
      .evict_req              ( min_evict_req[min_it]              ),
      .evict_valid            ( min_evict_req_valid[min_it]        ),
      .evict_ready            ( min_evict_req_ready[min_it]        ),
      // Data to coopertaive TStore module
      .coop_evict_req         ( coop_evict_req[min_it]             ),
      .coop_evict_valid       ( coop_evict_valid[min_it]           ),
      .coop_evict_ready       ( coop_evict_ready[min_it]           ),
      // Output debug signals going to Status Monitor
      .dbg_sm_signals         ( evict_ff_dbg_sm_signals[min_it]    )
    );

    ////////////////////////////////////////////////////////////////////////////
    // Combine and flop miss and evict requests from minions
    ////////////////////////////////////////////////////////////////////////////
    logic                     min_miss_req_bid;
    logic                     min_evict_req_bid;
    logic                     min_evict_second_access_xx;
    logic                     min_evict_second_access_xx_next;
    et_link_nodata_req_info_t min_l2_req_info_xx;
    logic [`NEIGH_DATA_RANGE] min_l2_req_data_xx;
    logic                     min_l2_req_is_evict_xx;
    logic                     min_l2_req_valid_xx;
    logic                     min_l2_req_bid_xx;
    logic                     min_l2_req_ready_xx;

    //         CLK       RST               EN                                         DOUT                         DIN                              DEF
    `RST_FF   (clock_lv, reset_w_neigh_ff,                                            min_evict_second_access_xx,  min_evict_second_access_xx_next, '0)
    `EN_FF    (clock_lv,                   min_l2_req_ready_xx & min_l2_req_valid_xx, min_l2_req_info[min_it],     min_l2_req_info_xx)
    `EN_FF    (clock_lv,                   min_l2_req_ready_xx & min_l2_req_valid_xx, min_l2_req_data[min_it],     min_l2_req_data_xx)
    `EN_FF    (clock_lv,                   min_l2_req_ready_xx & min_l2_req_valid_xx, min_l2_req_is_evict[min_it], min_l2_req_is_evict_xx)
    `RST_EN_FF(clock_lv, reset_w_neigh_ff, min_l2_req_ready_xx,                       min_l2_req_valid[min_it],    min_l2_req_valid_xx,             1'b0)
    `RST_FF   (clock_lv, reset_w_neigh_ff,                                            min_l2_req_bid[min_it],      min_l2_req_bid_xx,               1'b0)

    // If evict requests are to write 512 bits, they must be assigned two consecutive grants,
    // so that they are stored in two consecutive 256-bit FIFO entries
    // Thus, we disable miss request bids during the second evict access
    always_comb begin
      min_evict_second_access_xx_next = min_evict_second_access_xx;
    
      if (min_evict_req_ready[min_it] && (min_evict_req[min_it].size == ET_LINK_Line) && !min_evict_second_access_xx)
          min_evict_second_access_xx_next = 1'b1;
      else if (min_evict_req_ready[min_it] && min_evict_second_access_xx)
          min_evict_second_access_xx_next = 1'b0;
    end

    assign min_miss_req_bid  = min_miss_req_valid[min_it] & ~min_evict_second_access_xx;
    assign min_evict_req_bid = min_evict_req_valid[min_it];

    // Priority arbiter between evict and miss requests
    // with priority updated only if ready is given
    logic min_req_arbiter_c_winner_unused_ok;
    arb_prio_dly_rdy #(
      .NUM_CLIENTS ( 2                            )
    ) min_req_arbiter (
      // System signals
      .clock      ( clock_lv                      ),
      .reset      ( reset_w_neigh_ff              ),
    
      // client side
      .c_valid    ( {min_evict_req_bid,
                       min_miss_req_bid}            ),
      .c_ready    ( {min_evict_req_ready[min_it],
                       min_miss_req_ready[min_it]}  ),
      .c_winner   ( min_req_arbiter_c_winner_unused_ok ),
        
       // destination side
       .valid      ( min_l2_req_valid_xx           ),
       .ready      ( min_l2_req_ready_xx           )
    );

    // minion miss/evict arbiter request muxing
    always_comb begin
      min_l2_req_info_xx = '{source  : ET_LINK_NEIGH_SRC_Minion0 + min_it[`MIN_PER_N_R],
                             opcode  : et_link_req_opcode_t'(0),
                             size    : et_link_size_t'(0),
                             default : '0
                            };
    
      // Evict request from minions
      if (min_evict_req_ready[min_it]) begin
        min_l2_req_info_xx.id        = min_evict_req[min_it].id;
        min_l2_req_info_xx.wdata     = min_evict_req[min_it].wdata;
        min_l2_req_info_xx.opcode    = min_evict_req[min_it].opcode;
        min_l2_req_info_xx.subopcode = min_evict_req[min_it].subopcode;
        min_l2_req_info_xx.address   = min_evict_req[min_it].address;
        min_l2_req_data_xx           = min_evict_req[min_it].data;
        min_l2_req_info_xx.size      = min_evict_req[min_it].size;
        min_l2_req_info_xx.qwen      = min_evict_req[min_it].qwen;
      end else begin
      // Miss request from minions
        min_l2_req_info_xx.id        = min_miss_req[min_it].id;
        min_l2_req_info_xx.wdata     = min_miss_req[min_it].wdata;
        min_l2_req_info_xx.opcode    = min_miss_req[min_it].opcode;
        min_l2_req_info_xx.subopcode = min_miss_req[min_it].subopcode;
        min_l2_req_info_xx.address   = min_miss_req[min_it].address;
        min_l2_req_data_xx           = min_miss_req[min_it].data;
        min_l2_req_info_xx.size      = min_miss_req[min_it].size;
        min_l2_req_info_xx.qwen      = min_miss_req[min_it].qwen;
      end  
    end

    // Bids from miss and evict FF to miss request arbiter
    assign min_l2_req_is_evict_xx = min_evict_req_ready[min_it];
    assign min_l2_req_bid_xx      = (min_l2_req_ready_xx ? min_l2_req_valid_xx : min_l2_req_valid[min_it])
                                  & (~evict_second_access_pending | min_evict_second_access_next[min_it]);

    // Grant from miss request arbiter
    assign min_l2_req_ready_xx = min_l2_req_ready[min_it] | ~min_l2_req_valid[min_it];

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has a FF to store a fill coming from the L2. When a fill is
    // received (512b) it sequences the data to the minion in a 256b basis. Once
    // the data is drained, a new fill can be accepted for this minion
    ////////////////////////////////////////////////////////////////////////////

    // L2 response ( and Tbox requests) from fill FIFO to fill FF
    assign fill_fifo_min_resp[min_it]  = fill_fifo_out_rsp_info[`NEIGH_AGENT_MINION + min_it];
    assign fill_fifo_min_valid[min_it] = fill_fifo_out_rsp_valid[`NEIGH_AGENT_MINION + min_it];
    assign fill_ff_available[`NEIGH_AGENT_MINION + min_it] = fill_fifo_min_ready[min_it];

    neigh_fill_ff fill_ff (
      // System signals
      .clock                     ( clock_lv                          ),
      .reset                     ( reset_w_neigh_ff                  ),
      // Fills from L2
      .fill_fifo_resp            ( fill_fifo_min_resp[min_it]        ),
      .fill_fifo_valid           ( fill_fifo_min_valid[min_it]       ),
      .fill_fifo_ready           ( fill_fifo_min_ready[min_it]       ),
      // Response from cooperative TensorStore
      .tstore_min_resp           ( tstore_min_resp[min_it]           ),
      .tstore_min_resp_valid     ( tstore_min_resp_valid[min_it]     ),
      .tstore_min_resp_ready     ( tstore_min_resp_ready[min_it]     ),
      // Reduce local interconnect
      .local_message_info        ( local_message_info_in[min_it]     ),
      .local_message_valid       ( local_message_valid_in[min_it]    ),
      .local_message_ready       ( local_message_ready_in[min_it]    ),
      // Messages from TBOX
      .tbox_min_rsp_info         ( tbox_min_rsp_info_in[min_it]      ),
      .tbox_min_rsp_valid        ( tbox_min_rsp_valid_in[min_it]     ),
      .tbox_min_rsp_ready        ( tbox_min_rsp_ready_in[min_it]     ),
      // Data to minion
      .l2_resp                   ( l2_dcache_resp[min_it]            ),
      .l2_resp_valid             ( l2_dcache_resp_valid[min_it]      ),
      .l2_resp_ready             ( l2_dcache_resp_ready[min_it]      ),
      // Indication to tensor store buffer of the Ack
      .tstore_min_ack            ( l2_dcache_resp_tstore_ack[min_it] ),
      // Output debug signals going to Status Monitor
      .dbg_sm_signals            ( fill_ff_dbg_sm_signals[min_it]    )
    );

    // Minion ID within the shire
    assign shire_min_id[min_it] = { neigh_id_lo, neigh_min_id };

    // FF for thread enables
    //       CLK       RST               DOUT                    DIN                            DEF
    `RST_FF (clock_lv, reset_w_neigh_ff, thread0_enable[min_it], esr_thread0_enable_lo[min_it], '0)
    `RST_FF (clock_lv, reset_w_neigh_ff, thread1_enable[min_it], esr_thread1_enable_lo[min_it], '0)

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has a FF for interrupt lines
    ////////////////////////////////////////////////////////////////////////////
    minion_interrupts interrupts_next;

    //       CLK       RST               DOUT                           DIN              DEF
    `RST_FF (clock_lv, reset_w_neigh_ff, interrupts_per_minion[min_it], interrupts_next, '0)

    always_comb begin
      interrupts_next.fcc = '0;
      interrupts_next.fcc[uc_to_neigh_fcc_target_lo] = uc_to_neigh_fcc_lo[min_it];
      interrupts_next.mtip           = int_mtip_lo[min_it*`CORE_NR_THREADS +: `CORE_NR_THREADS];
      interrupts_next.meip           = int_meip_lo[`CORE_NR_THREADS*min_it+:2];
      interrupts_next.seip           = int_seip_lo[`CORE_NR_THREADS*min_it+:2];
      interrupts_next.msip           = ipi_msip_lo[min_it*`CORE_NR_THREADS +: `CORE_NR_THREADS];

      for(integer i = 0; i < `CORE_NR_THREADS; i++) begin
        interrupts_next.ipi_with_pc[i] = ipi_redirect_trigger_lo[2*min_it+i];
        // IECO only connected to thread 0 of minion 0
        interrupts_next.mieco[i] = (min_it == 0) && (i == 0) && esr_icache_ecc_count_ov;
      end
        interrupts_next.ipi_pc         = esr_ipi_redirect_pc[`PC_SIZE-1:1]; // not connecting LSB, which is always 0
    end
end
 // minions

////////////////////////////////////////////////////////////////////////////////
// TBOX router
// Routes TBOX-L2 requests and replies and minion-TBOX messages back and forth
// through the TBOX buses
////////////////////////////////////////////////////////////////////////////////
et_link_rsp_info_t       sc_tbox_rsp_info;
logic                    sc_tbox_rsp_valid;
logic                    sc_tbox_rsp_ready;
// TBOX message response intermediate north and sought stages
et_link_neigh_rsp_info_t tbox_min_north_ff_rsp;
et_link_neigh_msg_dest_t tbox_min_north_ff_rsp_msg_dest;
logic                    tbox_min_north_ff_available;
logic                    tbox_min_north_ff_available_next;
logic                    tbox_min_north_ff_in_accepted;
logic                    tbox_min_north_ff_out_accepted;
et_link_neigh_rsp_info_t tbox_min_south_ff_rsp;
et_link_neigh_msg_dest_t tbox_min_south_ff_rsp_msg_dest;
logic                    tbox_min_south_dest_is_north;
logic                    tbox_min_south2north_valid;
logic                    tbox_min_south_ff_available;
logic                    tbox_min_south_ff_available_next;
logic                    tbox_min_south_ff_in_accepted;
logic                    tbox_min_south_ff_out_accepted;

// TBOX FIFO push data
always_comb begin
  min_tbox_req_info.id        = miss_req_info_ff_proc.id;
  min_tbox_req_info.source    = miss_req_info_ff_proc.source;
  min_tbox_req_info.wdata     = miss_req_info_ff_proc.wdata;
  min_tbox_req_info.opcode    = miss_req_info_ff_proc.opcode;
  min_tbox_req_info.subopcode = miss_req_info_ff_proc.subopcode;
  min_tbox_req_info.address   = miss_req_info_ff_proc.address;
  min_tbox_req_info.size      = miss_req_info_ff_proc.size;
  min_tbox_req_info.qwen      = miss_req_info_ff_proc.qwen;
  min_tbox_req_info.data      = miss_req_data_ff_proc;
end
assign min_tbox_req_valid_early = miss_req_accepted_ff | (miss_req_valid_ff_proc & miss_req_goes_to_tbox & ~min_tbox_req_ready);
assign min_tbox_req_valid       = miss_req_valid_ff_proc & miss_req_goes_to_tbox;

// TBOX L2 replies
assign sc_tbox_rsp_info = fill_fifo_out_rsp_info[`NEIGH_AGENT_TBOX];
assign sc_tbox_rsp_valid = fill_fifo_out_rsp_valid[`NEIGH_AGENT_TBOX];
assign fill_ff_available[`NEIGH_AGENT_TBOX] = sc_tbox_rsp_ready;

`ifdef GFX_ENABLED
    neigh_tbox_router tbox_router (
      // System signals
      .clock                    ( clock_lv                    ),
      .reset                    ( tbox_reset_w                ),
      // Minion requests
      .min_tbox_req_info        ( min_tbox_req_info           ),
      .min_tbox_req_valid_early ( min_tbox_req_valid_early    ),
      .min_tbox_req_valid       ( min_tbox_req_valid          ),
      .min_tbox_req_ready       ( min_tbox_req_ready          ),
      // Minion TBOX reply bus
      .tbox_min_rsp_info        ( tbox_min_rsp_info           ),
      .tbox_min_rsp_valid       ( tbox_min_rsp_valid          ),
      .tbox_min_rsp_ready       ( tbox_min_rsp_ready          ),
      // TBOX L2 request
      .tbox_sc_req_disable_next ( evict_second_access_pending ),
      .tbox_sc_req_info         ( tbox_miss_req_info          ),
      .tbox_sc_req_valid        ( tbox_miss_req_valid         ),
      .tbox_sc_req_ready        ( tbox_miss_req_ready         ),
      // TBOX L2 reply
      .sc_tbox_rsp_info         ( sc_tbox_rsp_info            ),
      .sc_tbox_rsp_valid        ( sc_tbox_rsp_valid           ),
      .sc_tbox_rsp_ready        ( sc_tbox_rsp_ready           ),
      // TBOX neighborhood request bus
      .tbox_neigh_req_info      ( tbox_neigh_req_info         ),
      .tbox_neigh_req_valid     ( tbox_neigh_req_valid        ),
      .tbox_neigh_req_ready     ( tbox_neigh_req_ready        ),
      // TBOX neighborhood response bus
      .tbox_neigh_rsp_info      ( tbox_neigh_rsp_info         ),
      .tbox_neigh_rsp_valid     ( tbox_neigh_rsp_valid        ),
      .tbox_neigh_rsp_ready     ( tbox_neigh_rsp_ready        )
    );
`else // GFX_ENABLED
    assign min_tbox_req_ready   = 1'b0;
    assign tbox_min_rsp_info    = '0;
    assign tbox_min_rsp_valid   = 1'b0;
    assign tbox_miss_req_info   = '0;
    assign tbox_miss_req_valid  = 1'b0;
    assign sc_tbox_rsp_ready    = 1'b0;
    assign tbox_neigh_req_ready = 1'b0;
    assign tbox_neigh_rsp_info  = '0;
    assign tbox_neigh_rsp_valid = 1'b0;

    wire tbox_unused_ok = &{
    `ifndef ET_ASCENT_LINT
      1'b0,
    `endif
      min_tbox_req_valid_early,
      tbox_neigh_req_info,
      tbox_neigh_req_valid,
      tbox_neigh_rsp_ready
    };
`endif // GFX_ENABLED

// Flop TBOX response in south and north intermediate stages
// connected in daisy chain from south to north

//      CLK       RST               EN                             DOUT                         DIN                               DEF
`RST_FF(clock_lv, reset_w_neigh_ff,                                tbox_min_north_ff_available, tbox_min_north_ff_available_next, 1'b1)
`EN_FF (clock_lv,                   tbox_min_north_ff_in_accepted, tbox_min_north_ff_rsp,       tbox_min_south_ff_rsp)
`RST_FF(clock_lv, reset_w_neigh_ff,                                tbox_min_south_ff_available, tbox_min_south_ff_available_next, 1'b1)
`EN_FF (clock_lv,                   tbox_min_south_ff_in_accepted, tbox_min_south_ff_rsp,       tbox_min_rsp_info)

assign tbox_min_north_ff_rsp_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_north_ff_rsp.dest);
assign tbox_min_north_ff_in_accepted  = tbox_min_south2north_valid & tbox_min_north_ff_available;
assign tbox_min_south_ff_rsp_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_south_ff_rsp.dest);
assign tbox_min_south_ff_in_accepted  = tbox_min_rsp_valid & tbox_min_south_ff_available;
assign tbox_min_south_dest_is_north   = tbox_min_south_ff_rsp_msg_dest.min_id < `MIN_PER_N/2;
assign tbox_min_south2north_valid     = ~tbox_min_south_ff_available & tbox_min_south_dest_is_north;

assign tbox_min_rsp_ready = tbox_min_south_ff_available;

always_comb begin
  // North FF
  // --------
  // If FF still available
  if (tbox_min_north_ff_available)
      // Not available anymore when input is valid
      tbox_min_north_ff_available_next = ~tbox_min_south2north_valid;
  // Not available, wait for output to be accepted
  else
      // Grant received, FF available again
      tbox_min_north_ff_available_next = tbox_min_north_ff_out_accepted;  
  // South FF
  // --------
  // If FF still available
  if (tbox_min_south_ff_available)
      // Not available anymore when input is valid
      tbox_min_south_ff_available_next = ~tbox_min_rsp_valid;
  // Not available, wait for output to be accepted
  else
      // Grant received, FF available again
      tbox_min_south_ff_available_next = tbox_min_south_ff_out_accepted;
end

assign tbox_min_north_ff_out_accepted = tbox_min_rsp_ready_in[tbox_min_north_ff_rsp_msg_dest.min_id];
assign tbox_min_south_ff_out_accepted = tbox_min_south_dest_is_north ? tbox_min_north_ff_available
                                                                     : tbox_min_rsp_ready_in[tbox_min_south_ff_rsp_msg_dest.min_id];

// Connect to minion inputs
always_comb begin
  // Nort minions
  for (integer i = 0; i < `MIN_PER_N/2; i++)
  begin
      tbox_min_rsp_info_in[i]  = tbox_min_north_ff_rsp;
      tbox_min_rsp_valid_in[i] = ~tbox_min_north_ff_available & (tbox_min_north_ff_rsp_msg_dest.min_id == i[`MIN_PER_N_R]);
  end

  // South minions
  for (integer i = `MIN_PER_N/2; i < `MIN_PER_N; i++)
  begin
      tbox_min_rsp_info_in[i]  = tbox_min_south_ff_rsp;
      tbox_min_rsp_valid_in[i] = ~tbox_min_south_ff_available & (tbox_min_south_ff_rsp_msg_dest.min_id == i[`MIN_PER_N_R]);
  end
end

////////////////////////////////////////////////////////////////////////////
// Fast Local Barrier unit
////////////////////////////////////////////////////////////////////////////

neigh_fl_barrier fl_barrier (
  // System signals
  .clock                     ( clock_lv                  ),
  .reset                     ( reset_w_neigh_ff          ),
  // Fast Local Barrier request interface between cores and neigh
  .flb_core_neigh_req_valid  ( flb_core_neigh_req_valid  ),
  .flb_core_neigh_req_data   ( flb_core_neigh_req_data   ),
  // Fast Local Barrier response interface between cores and neigh
  .flb_neigh_core_resp_valid ( flb_neigh_core_resp_valid ),
  .flb_neigh_core_resp_data  ( flb_neigh_core_resp_data  ),
  // Fast Local Barrier request interface between neigh and L2
  .flb_neigh_l2_req_valid    ( flb_neigh_l2_req_valid_lo ),
  .flb_neigh_l2_req_data     ( flb_neigh_l2_req_data_lo  ),
  // Fast Local Barrier response interface between neigh and L2
  .flb_l2_neigh_resp_valid   ( flb_l2_neigh_resp_valid_lo),
  .flb_l2_neigh_resp_data    ( flb_l2_neigh_resp_data_lo )
);

////////////////////////////////////////////////////////////////////////////
// Cooperative Tensor Load
////////////////////////////////////////////////////////////////////////////

neigh_coop_tload coop_tload (
  // System signals
  .clock                           ( clock_lv                           ),
  .reset                           ( reset_w_neigh_ff                   ),
  .neigh_id                        ( neigh_id_lo                        ),
  // Cooperative requests from minions          
  .min_coop_miss_req               ( min_coop_miss_req                  ),
  .min_coop_miss_req_valid         ( min_coop_miss_req_valid            ),
  .min_coop_miss_req_ready         ( min_coop_miss_req_ready            ),
  // Combined cooperative request to L2
  .coop_tload_disable_next         ( evict_second_access_pending        ),
  .coop_tload_req                  ( coop_tload_miss_req_info           ),
  .coop_tload_req_bid              ( coop_tload_miss_req_valid          ),
  .coop_tload_req_ready            ( coop_tload_miss_req_ready          ),
  // Cooperative TLoad slave ready output bus
  .coop_tload_slv_rdy_out_data     ( coop_tload_slv_rdy_out_data_lv     ),
  .coop_tload_slv_rdy_out_valid    ( coop_tload_slv_rdy_out_valid_lv    ),
  // Cooperative TLoad slave ready input buses
  .coop_tload_slv_rdy_in_data      ( coop_tload_slv_rdy_in_data_lv      ),
  .coop_tload_slv_rdy_in_valid     ( coop_tload_slv_rdy_in_valid_lv     ),
  // Cooperative TLoad master done output bus
  .coop_tload_mst_done_out_coop_id ( coop_tload_mst_done_out_coop_id_lv ),
  .coop_tload_mst_done_out_valid   ( coop_tload_mst_done_out_valid_lv   ),
  // Cooperative TLoad master done input buses
  .coop_tload_mst_done_in_coop_id  ( coop_tload_mst_done_in_coop_id_lv  ),
  .coop_tload_mst_done_in_valid    ( coop_tload_mst_done_in_valid_lv    ),
  // Cooperative TLoad tag table CAM port
  .tag_table_cam_pop               ( coop_tload_tag_table_cam_pop       ),
  .tag_table_cam_tag               ( coop_tload_tag_table_cam_tag       ),
  .tag_table_cam_data              ( coop_tload_tag_table_cam_data      ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals                  ( coop_tload_dbg_sm_signals          )
);

// HV version of neigh reset_w
logic reset_w_neigh_ff_hv;

vclevel_shft_l2h #(.WIDTH(1)) ls_l2h_reset_w_neigh (.inp_lv(reset_w_neigh_ff), .out_hv(reset_w_neigh_ff_hv));

neigh_coop_tload_ports coop_tload_ports_pd_hv (
  // System signals
  .clock                              ( clock_lv                           ),
  .reset                              ( reset_w_neigh_ff_hv                ),

  // Cooperative TLoad ports on HV part
  .coop_tload_slv_rdy_out_data        ( coop_tload_slv_rdy_out_data        ),
  .coop_tload_slv_rdy_out_valid       ( coop_tload_slv_rdy_out_valid       ),

  .coop_tload_slv_rdy_in_data         ( coop_tload_slv_rdy_in_data         ),
  .coop_tload_slv_rdy_in_valid        ( coop_tload_slv_rdy_in_valid        ),

  .coop_tload_mst_done_out_coop_id    ( coop_tload_mst_done_out_coop_id    ),
  .coop_tload_mst_done_out_valid      ( coop_tload_mst_done_out_valid      ),

  .coop_tload_mst_done_in_coop_id     ( coop_tload_mst_done_in_coop_id     ),
  .coop_tload_mst_done_in_valid       ( coop_tload_mst_done_in_valid       ),

  // Cooperative TLoad ports on LV part
  .coop_tload_slv_rdy_out_data_lv     ( coop_tload_slv_rdy_out_data_lv     ),
  .coop_tload_slv_rdy_out_valid_lv    ( coop_tload_slv_rdy_out_valid_lv    ),

  .coop_tload_slv_rdy_in_data_lv      ( coop_tload_slv_rdy_in_data_lv      ),
  .coop_tload_slv_rdy_in_valid_lv     ( coop_tload_slv_rdy_in_valid_lv     ),

  .coop_tload_mst_done_out_coop_id_lv ( coop_tload_mst_done_out_coop_id_lv ),
  .coop_tload_mst_done_out_valid_lv   ( coop_tload_mst_done_out_valid_lv   ),

  .coop_tload_mst_done_in_coop_id_lv  ( coop_tload_mst_done_in_coop_id_lv  ),
  .coop_tload_mst_done_in_valid_lv    ( coop_tload_mst_done_in_valid_lv    )
);

////////////////////////////////////////////////////////////////////////////
// Tensor Store cooperative
////////////////////////////////////////////////////////////////////////////

neigh_tensor_store_buffer tensor_store_buffer (
  // System signals
  .clock                  ( clock_lv                        ),
  .reset                  ( reset_w_neigh_ff                ),
  // Evict request from minion
  .evict_req              ( coop_evict_req                  ),
  .evict_valid            ( coop_evict_valid                ),
  .evict_ready            ( coop_evict_ready                ),
  // Request going out
  .evict_out_req          ( tstore_evict_req                ),
  .evict_out_valid        ( tstore_evict_valid              ),
  .evict_out_ready        ( tstore_miss_req_ready           ),
  .evict_out_disable_next ( min_evict_second_access_pending ),
  // Gets Acks going back to minions
  .tstore_min_ack         ( l2_dcache_resp_tstore_ack       ),
  // Sends Acks to slave minions
  .tstore_min_resp_ready  ( tstore_min_resp_ready           ),
  .tstore_min_resp_valid  ( tstore_min_resp_valid           ),
  .tstore_min_resp        ( tstore_min_resp                 ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals         ( tstore_dbg_sm_signals           )
);

////////////////////////////////////////////////////////////////////////////////
// PMU Unit
////////////////////////////////////////////////////////////////////////////////

//          CLK       RST               EN                           DOUT              DIN                    DEF 
`RST_EN_FF (clock_lv, reset_w_neigh_ff, ~esr_pmu_ctrl.disable_clock, pmu_neigh_events, pmu_neigh_events_next, '0)

assign pmu_neigh_events_next = {tbox_min_rsp_valid & tbox_min_rsp_ready,                     // The TBOX sends a message to any minion
                                min_tbox_req_valid & min_tbox_req_ready,                     // Any minion sends a message to the TBOX
                                fill_fifo_push,                                              // An ET Link response is received from the SC/UC input
                                |out_req_push,                                               // An ET Link request is pushed into any of the BANK/UC FIFOs
                                int_req_push,                                                // An ET Link request is pushed into the intermediate FIFO
                                sc_tbox_rsp_valid & sc_tbox_rsp_ready,                       // The TBOX receives an ET Link response from SC/UC
                                tbox_miss_req_valid & tbox_miss_req_ready,                   // The TBOX sends an ET Link request to SC/UC
                                |(ptw_l2_miss_resp_valid & ptw_l2_miss_resp_ready),          // Any PTW receives an ET Link response
                                |(ptw_l2_miss_req_valid & ptw_l2_miss_req_ready),            // Any PTW sends an ET Link request
                                icache_f0_sram_resp_valid_lo & icache_f0_sram_resp_ready_lo, // The Icache receives a response from the L1 data SRAM
                                icache_f2_sram_req_valid_lo & icache_f2_sram_req_ready_lo,   // The Icache sends a request to the L1 data SRAM
                                icache_l2_miss_resp_valid & icache_l2_miss_resp_ready,       // The Icache receives an ET Link response
                                icache_l2_miss_req_valid & icache_l2_miss_req_ready,         // The Icache sends an ET Link request
                                |(local_message_valid_in & local_message_ready_in),          // A message is sent between minions through the FLN
                                |ptw_dc_resp_valid,                                          // Any minion receives a response from the PTW
                                |(dc_ptw_req_valid & dc_ptw_req_ready),                      // Any minion sends a request to the PTW
                                |icache_resp_valid,                                          // Any minion receives a response from the Icache
                                |(icache_req_valid & icache_req_ready),                      // Any minion sends a request to the Icache
                                fill_fifo_push &                                             // A cooperative store response is received
                                    (fill_fifo_in_rsp_info.opcode == ET_LINK_RSP_Ack) &
                                    (et_link_core_miss_id_t'(fill_fifo_in_rsp_info.id) == ET_LINK_Core_Miss_Id_Tensor_Store_Coop),
                                tstore_evict_valid & tstore_miss_req_ready,                  // A cooperative store request is sent
                                fill_fifo_push &                                             // A cooperative load response is received
                                    (fill_fifo_in_rsp_info.opcode == ET_LINK_RSP_AckData) &
                                    (et_link_neigh_source_t'(fill_fifo_in_rsp_info.dest) == ET_LINK_NEIGH_SRC_Coop),
                                coop_tload_miss_req_valid & coop_tload_miss_req_ready &      // An inter-neighborhood cooperative load request is sent
                                    (coop_tload_miss_req_info.opcode == ET_LINK_REQ_ReadCoop),
                                coop_tload_miss_req_valid & coop_tload_miss_req_ready,       // A cooperative load request is sent
                                |(fill_fifo_min_valid & fill_fifo_min_ready),                // Any minion receives an ET Link response
                                |(min_l2_req_bid & min_l2_req_ready)                         // Any minion sends an ET Link request
                               };

// PMU module
neigh_pmu pmu (
  // System signals
  .clock               ( clock_lv               ),
  .reset               ( reset_w_neigh_pmu      ),
  // Minion interface
  .pmu_count_up        ( pmu_count_up           ),
  .pmu_read_data       ( pmu_read_data          ),
  .pmu_read_sel        ( pmu_read_sel           ),
  .pmu_write_en        ( pmu_write_en           ),
  .pmu_write_data      ( pmu_write_data         ),
  .pmu_neigh_event_sel ( pmu_neigh_event_sel    ),
  // Neigh interface
  .pmu_neigh_events    ( pmu_neigh_events       ),
  // Module control
  .pmu_ctrl            ( esr_pmu_ctrl           )
);

////////////////////////////////////////////////////////////////////////////
// ESR file
////////////////////////////////////////////////////////////////////////////
esr_and_or_tree_L0_t esr_and_or_tree_L0_2_esr;
esr_and_or_tree_L0_t esr_and_or_tree_L0;
esr_and_or_tree_L0_t esr_and_or_tree_L0_lo;

always_comb begin
  esr_and_or_tree_L0_2_esr = 'h0;
  esr_and_or_tree_L0_2_esr.anyselected    = esr_and_or_tree_L0.anyselected   ;
  esr_and_or_tree_L0_2_esr.anyunavailable = esr_and_or_tree_L0.anyunavailable;
  esr_and_or_tree_L0_2_esr.allhavereset   = esr_and_or_tree_L0.allhavereset  ;
  esr_and_or_tree_L0_2_esr.anyhavereset   = esr_and_or_tree_L0.anyhavereset  ;
  esr_and_or_tree_L0_2_esr.allresumeack   = esr_and_or_tree_L0.allresumeack  ;
  esr_and_or_tree_L0_2_esr.anyresumeack   = esr_and_or_tree_L0.anyresumeack  ;
  esr_and_or_tree_L0_2_esr.allrunning     = esr_and_or_tree_L0.allrunning    ;
  esr_and_or_tree_L0_2_esr.anyrunning     = esr_and_or_tree_L0.anyrunning    ;
  esr_and_or_tree_L0_2_esr.allhalted      = esr_and_or_tree_L0.allhalted     ;
  esr_and_or_tree_L0_2_esr.anyhalted      = esr_and_or_tree_L0.anyhalted     ;
end

  //   CLK     DOUT                   DIN
`FF (clock_lv, esr_and_or_tree_L0_lo, esr_and_or_tree_L0)

esr_neigh #(
  .AD_WIDTH                   ( `ESR_NEIGH_APB_AD_PP_WIDTH  )
) esrs (
  .clock                          ( clock_lv                       ),
  .reset_c                        ( reset_c_neigh_ff               ),
  .reset_w                        ( reset_w_neigh_ff               ),
  .reset_d                        ( reset_d_neigh_ff               ), 
  .test_en                        ( 1'b0                           ), // DFT_SCAN_INSERT
  .dft__reset_byp                 ( 1'b0                           ), // unused
  .dft__reset                     ( 1'b0                           ), // unused
  .esr_minion_boot_op             ( minion_boot_addr               ),
  .esr_mprot_op                   ( mprot                          ),
  .esr_ipi_redirect_pc_op         ( esr_ipi_redirect_pc            ),
  .esr_hactrl_op                  ( esr_hactrl                     ),
  .esr_hastatus0_ip               ( esr_hastatus0                  ),
  .esr_hastatus1_min              ( esr_hastatus1_min              ),
  .esr_dmctrl                     ( dmctrl_lo                      ),
  .esr_icache_err_log_ctl_op      ( esr_icache_err_log_ctl         ),
  .esr_icache_err_log_sbe_ip      ( esr_icache_err_log_sbe         ),
  .esr_icache_err_log_dbe_ip      ( esr_icache_err_log_dbe         ),
  .esr_icache_err_log_info_ip     ( esr_icache_err_log_info        ),
  .esr_icache_err_detected_op     ( esr_icache_err_detected_lo     ),
  .esr_icache_err_logged_op       ( esr_icache_err_logged_lo       ),
  .esr_icache_ecc_count_ov_op     ( esr_icache_ecc_count_ov        ), 
  .apb_paddr                      ( APB_req_struct_ESR.apb_paddr   ),
  .apb_pwrite                     ( APB_req_struct_ESR.apb_pwrite  ), 
  .apb_psel                       ( APB_req_struct_ESR.apb_psel    ),
  .apb_penable                    ( APB_req_struct_ESR.apb_penable ),
  .apb_pwdata                     ( APB_req_struct_ESR.apb_pwdata  ),
  .apb_pready                     ( APB_rsp_struct_ESR.apb_pready  ),
  .apb_prdata                     ( APB_rsp_struct_ESR.apb_prdata  ),
  .apb_pslverr                    ( APB_rsp_struct_ESR.apb_pslverr ),   
  .esr_and_or_treeL0_ip           ( esr_and_or_tree_L0_2_esr       ),
  .esr_pmu_ctrl_op                ( esr_pmu_ctrl                   ),
  .esr_neigh_chicken_op           ( esr_neigh_chicken              )
);
// legacy ESRs OUTPUT put port assigned to 0
assign esr_texture_control_op = 0;
assign esr_texture_image_table_ptr_op = 0;
// Bypass Dcache bits
assign esr_bypass_dcache = esr_neigh_chicken.bypass_dcache;

assign vmspagesize = tlb_entry_type_4K;
   
// Flop ESRs from/to TBOX
//      CLK       RST               DOUT                               DIN                             DEF
`RST_FF(clock_lv, reset_w_neigh_ff, esr_texture_control_op_ff,         esr_texture_control_op,         '0)
`RST_FF(clock_lv, reset_w_neigh_ff, esr_texture_image_table_ptr_op_ff, esr_texture_image_table_ptr_op, '0)

assign tbox_control_esr         = esr_texture_control_op_ff;
assign tbox_image_table_ptr_esr = esr_texture_image_table_ptr_op_ff;

////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////
// Debug signals from/to Voltage crossings
logic [`neigh_sm_gpio_width-1:0]      neigh_sm_gpio_lo; 
neigh_sm_dbg_monitor_t                neigh_sm_signals_lo;
// Debug signals from neigh_channel to Status Monitor 0
logic                                 neigh_ch_dbg_signals_en;
logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0] neigh_ch_dbg_signals_mux;
neigh_sm_dbg_monitor_t                neigh_ch_dbg_signals_next;
neigh_sm_dbg_monitor_t                neigh_ch_dbg_signals;
// Debug signals from/to minions
minion_debug_out_t [`MIN_PER_N-1:0]   debug_minion_out_ff;

//      CLK       RST               DOUT                 DIN               DEF
`RST_FF(clock_lv, reset_d_neigh_ff, debug_minion_out_ff, debug_minion_out, '0)

logic  [`MIN_PER_N-1:0] reset_w_per_minion_pre;

for (genvar min_i= 0; min_i<`MIN_PER_N; min_i++) begin : SYNC_RESETS_PER_MINION
  rst_repeat rst_repeat_reset_w_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_w_per_minion_pre[min_i]) , .reset_out(reset_w_per_minion[min_i]));
  rst_repeat rst_repeat_reset_c_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_c_neigh)                 , .reset_out(reset_c_per_minion[min_i]));
  rst_repeat rst_repeat_reset_d_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_d_neigh)                 , .reset_out(reset_d_per_minion[min_i]));
end

assign tbox_bpam_run_control = '0; // ANT: Removed logic related to events and TBOX
   
neigh_ch_dbg neigh_ch_dbg (
  .clock                      ( clock_lv                      ),
  // Input resets
  .reset_w                    ( reset_w_dbg                   ),  
  .reset_c                    ( reset_c_neigh_ff              ),  
  .reset_d                    ( reset_d_neigh_ff              ),  
  // Output resets
  .reset_w_per_minion         ( reset_w_per_minion_pre        ),
  .reset_w_neigh              ( reset_w_neigh                 ),

   // Run-control for minions and tbox
   .debug_minion_req           ( debug_minion_in              ),
   .debug_minion_status        ( debug_minion_out_ff          ),
   .dmctrl                     ( dmctrl_lo                    ),
   .esr_hastatus0              ( esr_hastatus0                ),
   .esr_hastatus1_min          ( esr_hastatus1_min            ),
   .esr_hactrl                 ( esr_hactrl                   ),
   .esr_and_or_tree_L0         ( esr_and_or_tree_L0           ),
    // Status Monitor
  .status_monitor_enable      ( hw_dbg_sm_monitor_enabled_lo ),
  .neigh_sm_gpio              ( neigh_sm_gpio_lo             ),
  .minion_dbg_signals_en      ( minion_dbg_signals_en        ),
  .minion_dbg_signals_mux     ( minion_dbg_signals_mux       ),
  .minion_dbg_signals         ( minion_dbg_signals           ),
  .tbox_dbg_signals_en        ( tbox_dbg_signals_en          ),
  .tbox_dbg_signals_mux       ( tbox_dbg_signals_mux         ),
  .tbox_dbg_signals           ( tbox_dbg_signals             ),
  .neigh_ch_dbg_signals_en    ( neigh_ch_dbg_signals_en      ),
  .neigh_ch_dbg_signals_mux   ( neigh_ch_dbg_signals_mux     ),
  .neigh_ch_dbg_signals       ( neigh_ch_dbg_signals         ),
  .neigh_sm_signals           ( neigh_sm_signals_lo          )
);

// Connect debug signals from neigh_channel
logic [`NEIGH_DEBUG_SM_MUX_MOD_WIDTH-1:0]    neigh_ch_dbg_mod_idx;
logic [`NEIGH_DEBUG_SM_MUX_DATA_WIDTH-1:0]   neigh_ch_dbg_data_idx;
logic [(`MIN_PER_N_L-1)-1:0]                 neigh_ch_dbg_min_idx;
logic [`MIN_PER_N_R]                         neigh_ch_dbg_north_min_idx;
logic [`MIN_PER_N_R]                         neigh_ch_dbg_south_min_idx;
logic [`ICACHE_NR_REQS_L-1:0]                neigh_ch_dbg_icache_req_idx;
logic [`PTW_PER_N_R]                         neigh_ch_dbg_ptw_idx;
logic                                        neigh_ch_dbg_tload_data_idx;
et_link_rsp_opcode_t [`MIN_PER_N-1:0]        tstore_min_resp_opcode;
logic [`MIN_PER_N-1:0][`ET_LINK_ID_SIZE-1:0] local_message_info_out_id;
logic [`MIN_PER_N-1:0][8-1:0]                local_message_info_out_data;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         min_north_dbg_match_signals_next;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        min_north_dbg_filter_signals_next;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          min_north_dbg_data_signals_next;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         min_north_dbg_match_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        min_north_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          min_north_dbg_data_signals;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         min_south_dbg_match_signals_next;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        min_south_dbg_filter_signals_next;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          min_south_dbg_data_signals_next;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         min_south_dbg_match_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        min_south_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          min_south_dbg_data_signals;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         icache_ptw_dbg_match_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        icache_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          icache_dbg_data_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        ptw_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          ptw_dbg_data_signals;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         tload_tstore_dbg_match_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        tload_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          tload_dbg_data_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        tstore_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          tstore_dbg_data_signals;
logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]         fln_tbox_dpath_flb_dbg_match_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        fln_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          fln_dbg_data_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        tbox_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          tbox_dbg_data_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        dpath_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          dpath_dbg_data_signals;
logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]        flb_dbg_filter_signals;
logic [`NEIGH_DEBUG_DATA_WIDTH-1:0]          flb_dbg_data_signals;

//          CLK       RST               EN                       DOUT                  DIN                        DEF
`RST_EN_FF (clock_lv, reset_d_neigh_ff, neigh_ch_dbg_signals_en, neigh_ch_dbg_signals, neigh_ch_dbg_signals_next, '0)

// Extract debug multiplexing indexes
assign neigh_ch_dbg_mod_idx  = neigh_ch_dbg_signals_mux[`NEIGH_DEBUG_SM_MUX_DATA_WIDTH+:`NEIGH_DEBUG_SM_MUX_MOD_WIDTH];
assign neigh_ch_dbg_data_idx = neigh_ch_dbg_signals_mux[0+:`NEIGH_DEBUG_SM_MUX_DATA_WIDTH];

assign neigh_ch_dbg_min_idx         = neigh_ch_dbg_mod_idx[0+:`MIN_PER_N_L-1];
assign neigh_ch_dbg_north_min_idx   = {1'b0,neigh_ch_dbg_min_idx};
assign neigh_ch_dbg_south_min_idx   = {1'b1,neigh_ch_dbg_min_idx};
assign neigh_ch_dbg_icache_req_idx  = neigh_ch_dbg_data_idx[0+:`ICACHE_NR_REQS_L];
assign neigh_ch_dbg_ptw_idx         = neigh_ch_dbg_data_idx[0+:`PTW_PER_N_L];
assign neigh_ch_dbg_tload_data_idx  = neigh_ch_dbg_data_idx[0];

// Extract opcodes from responses from TStore to minions
always_comb begin
  for (integer unsigned i = 0; i < `MIN_PER_N; i++)
    tstore_min_resp_opcode[i] = tstore_min_resp[i].opcode;
end

// Extract ids data from requests from evict FFs to FLN
always_comb begin
  for (integer unsigned i = 0; i < `MIN_PER_N; i++) begin
    local_message_info_out_id[i]   = local_message_info_out[i].id;
    local_message_info_out_data[i] = local_message_info_out[i].data[7:0];
  end
end

// Connect debug signals from the different neighborhood channel modules before being multiplexed

// Minion debug signals (flopped)
// --------------------
logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] l2_dcache_evict_req_ready_dbg_reg;
logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] l2_dcache_evict_req_valid_dbg_reg;
et_link_minion_evict_req_info_t [`MIN_PER_N-1:0]       l2_dcache_evict_req_dbg_reg;
logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]  l2_dcache_miss_req_ready_dbg_reg;
logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]  l2_dcache_miss_req_valid_dbg_reg;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0]        l2_dcache_miss_req_dbg_reg;
logic [`MIN_PER_N-1:0]                                 l2_dcache_resp_ready_dbg_reg;
logic [`MIN_PER_N-1:0]                                 l2_dcache_resp_valid_dbg_reg;
et_link_minion_rsp_info_t [`MIN_PER_N-1:0]             l2_dcache_resp_dbg_reg;
logic [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]  miss_ff_dbg_sm_signals_reg;
logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] evict_ff_dbg_sm_signals_reg;
fill_ff_dbg_sm_t [`MIN_PER_N-1:0]                      fill_ff_dbg_sm_signals_reg;

//      CLK       EN                       DOUT                               DIN
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_evict_req_ready_dbg_reg, l2_dcache_evict_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_evict_req_valid_dbg_reg, l2_dcache_evict_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_evict_req_dbg_reg,       l2_dcache_evict_req)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_miss_req_ready_dbg_reg,  l2_dcache_miss_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_miss_req_valid_dbg_reg,  l2_dcache_miss_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_miss_req_dbg_reg,        l2_dcache_miss_req)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_resp_ready_dbg_reg,      l2_dcache_resp_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_resp_valid_dbg_reg,      l2_dcache_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, l2_dcache_resp_dbg_reg,            l2_dcache_resp)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, miss_ff_dbg_sm_signals_reg,        miss_ff_dbg_sm_signals)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, evict_ff_dbg_sm_signals_reg,       evict_ff_dbg_sm_signals)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, fill_ff_dbg_sm_signals_reg,        fill_ff_dbg_sm_signals)

// Combine match signals from all minions
// (first, north and south minions are multiplexed independently and flopped)
assign min_north_dbg_match_signals_next = (`NEIGH_DEBUG_MATCH_WIDTH)'({
                                          l2_dcache_resp_valid_dbg_reg[0+:`MIN_PER_N/2] & l2_dcache_resp_ready_dbg_reg[0+:`MIN_PER_N/2],
                                          l2_dcache_evict_req_valid_dbg_reg[0+:`MIN_PER_N/2] & l2_dcache_evict_req_ready_dbg_reg[0+:`MIN_PER_N/2],
                                          l2_dcache_miss_req_valid_dbg_reg[0+:`MIN_PER_N/2] & l2_dcache_miss_req_ready_dbg_reg[0+:`MIN_PER_N/2]});
assign min_south_dbg_match_signals_next = (`NEIGH_DEBUG_MATCH_WIDTH)'({
                                          l2_dcache_resp_valid_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2] & l2_dcache_resp_ready_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2],
                                          l2_dcache_evict_req_valid_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2] & l2_dcache_evict_req_ready_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2],
                                          l2_dcache_miss_req_valid_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2] & l2_dcache_miss_req_ready_dbg_reg[`MIN_PER_N/2+:`MIN_PER_N/2]});

assign min_north_dbg_filter_signals_next = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_resp_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_resp_ready_dbg_reg[neigh_ch_dbg_north_min_idx],
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                           l2_dcache_evict_req_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_evict_req_ready_dbg_reg[neigh_ch_dbg_north_min_idx],
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                           l2_dcache_miss_req_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_miss_req_ready_dbg_reg[neigh_ch_dbg_north_min_idx]});
assign min_south_dbg_filter_signals_next = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_resp_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_resp_ready_dbg_reg[neigh_ch_dbg_south_min_idx],
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                           l2_dcache_evict_req_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_evict_req_ready_dbg_reg[neigh_ch_dbg_south_min_idx],
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                           l2_dcache_miss_req_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_miss_req_ready_dbg_reg[neigh_ch_dbg_south_min_idx]});

assign min_north_dbg_data_signals_next = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                         fill_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         evict_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                         miss_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].address});
assign min_south_dbg_data_signals_next = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                         fill_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         evict_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                         miss_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].address});

//      CLK       EN                       DOUT                          DIN
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_north_dbg_match_signals,  min_north_dbg_match_signals_next)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_north_dbg_filter_signals, min_north_dbg_filter_signals_next)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_north_dbg_data_signals,   min_north_dbg_data_signals_next)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_south_dbg_match_signals,  min_south_dbg_match_signals_next)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_south_dbg_filter_signals, min_south_dbg_filter_signals_next)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_south_dbg_data_signals,   min_south_dbg_data_signals_next)

// Icache and PTW debug signals (flopped)
// ----------------------------
logic [`MIN_PER_N-1:0]                     icache_req_valid_dbg_reg;
logic [`MIN_PER_N-1:0]                     icache_req_ready_dbg_reg;
icache_frontend_resp [`ICACHE_NR_REQS-1:0] icache_resp_dbg_reg;
logic [`ICACHE_NR_REQS-1:0]                icache_resp_miss_dbg_reg;
logic [`ICACHE_NR_REQS-1:0]                icache_resp_valid_dbg_reg;
logic [`ICACHE_NR_REQS-1:0]                icache_fill_done_dbg_reg;
logic                                      icache_f2_sram_req_write_dbg_reg;
logic [`ICACHE_SRAM_ADD_WIDTH-1:0]         icache_f2_sram_req_addr_dbg_reg;
logic                                      icache_f2_sram_req_valid_dbg_reg;
logic                                      icache_f2_sram_req_ready_dbg_reg;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0]        icache_f0_sram_resp_dout_dbg_reg;
logic                                      icache_f0_sram_resp_valid_dbg_reg;
logic                                      icache_f0_sram_resp_ready_dbg_reg;
et_link_nodata_req_info_t                  icache_l2_miss_req_info_dbg_reg;
logic                                      icache_l2_miss_req_valid_dbg_reg;
logic                                      icache_l2_miss_req_ready_dbg_reg;
et_link_rsp_info_t                         icache_l2_miss_resp_info_dbg_reg;
logic                                      icache_l2_miss_resp_valid_dbg_reg;
logic                                      icache_l2_miss_resp_ready_dbg_reg;
icache_dbg_sm_t                            icache_dbg_sm_signals_reg;
logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]   dc_ptw_req_valid_dbg_reg;
logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]   dc_ptw_req_ready_dbg_reg;
logic [`PTW_PER_N-1:0]                     ic_ptw_req_valid_dbg_reg;
logic [`PTW_PER_N-1:0]                     ic_ptw_req_ready_dbg_reg;
minion_ptw_pte [`PTW_PER_N-1:0]            ptw_resp_data_dbg_reg;
logic [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]   ptw_dc_resp_valid_dbg_reg;
logic [`PTW_PER_N-1:0]                     ptw_ic_resp_valid_dbg_reg;
et_link_neigh_req_info_t [`PTW_PER_N-1:0]  ptw_l2_miss_req_info_dbg_reg;
logic [`PTW_PER_N-1:0]                     ptw_l2_miss_req_valid_dbg_reg;
logic [`PTW_PER_N-1:0]                     ptw_l2_miss_req_ready_dbg_reg;
et_link_rsp_info_t [`PTW_PER_N-1:0]        ptw_l2_miss_resp_info_dbg_reg;
logic [`PTW_PER_N-1:0]                     ptw_l2_miss_resp_valid_dbg_reg;
logic [`PTW_PER_N-1:0]                     ptw_l2_miss_resp_ready_dbg_reg;
ptw_dbg_sm_t [`PTW_PER_N-1:0]              ptw_dbg_sm_signals_reg;

//      CLK       EN                       DOUT                               DIN
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_req_valid_dbg_reg,          icache_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_req_ready_dbg_reg,          icache_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_resp_dbg_reg,               icache_resp)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_resp_miss_dbg_reg,          icache_resp_miss)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_resp_valid_dbg_reg,         icache_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_fill_done_dbg_reg,          icache_fill_done)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f2_sram_req_write_dbg_reg,  icache_f2_sram_req_write_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f2_sram_req_addr_dbg_reg,   icache_f2_sram_req_addr_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f2_sram_req_valid_dbg_reg,  icache_f2_sram_req_valid_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f2_sram_req_ready_dbg_reg,  icache_f2_sram_req_ready_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f0_sram_resp_dout_dbg_reg,  icache_f0_sram_resp_dout_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f0_sram_resp_valid_dbg_reg, icache_f0_sram_resp_valid_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_f0_sram_resp_ready_dbg_reg, icache_f0_sram_resp_ready_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_req_info_dbg_reg,   icache_l2_miss_req_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_req_valid_dbg_reg,  icache_l2_miss_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_req_ready_dbg_reg,  icache_l2_miss_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_resp_info_dbg_reg,  icache_l2_miss_resp_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_resp_valid_dbg_reg, icache_l2_miss_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_l2_miss_resp_ready_dbg_reg, icache_l2_miss_resp_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, icache_dbg_sm_signals_reg,         icache_dbg_sm_signals)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, dc_ptw_req_valid_dbg_reg,          dc_ptw_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, dc_ptw_req_ready_dbg_reg,          dc_ptw_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ic_ptw_req_valid_dbg_reg,          ic_ptw_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ic_ptw_req_ready_dbg_reg,          ic_ptw_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_resp_data_dbg_reg,             ptw_resp_data)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_dc_resp_valid_dbg_reg,         ptw_dc_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_ic_resp_valid_dbg_reg,         ptw_ic_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_req_info_dbg_reg,      ptw_l2_miss_req_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_req_valid_dbg_reg,     ptw_l2_miss_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_req_ready_dbg_reg,     ptw_l2_miss_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_resp_info_dbg_reg,     ptw_l2_miss_resp_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_resp_valid_dbg_reg,    ptw_l2_miss_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_l2_miss_resp_ready_dbg_reg,    ptw_l2_miss_resp_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, ptw_dbg_sm_signals_reg,            ptw_dbg_sm_signals)

// Combine match signals from Icache and PTW
assign icache_ptw_dbg_match_signals = (`NEIGH_DEBUG_MATCH_WIDTH)'({
                                         // PTW signals
                                         ptw_l2_miss_resp_valid_dbg_reg & ptw_l2_miss_resp_ready_dbg_reg,
                                         ptw_l2_miss_req_valid_dbg_reg & ptw_l2_miss_req_ready_dbg_reg,
                                         ptw_ic_resp_valid_dbg_reg,
                                         ic_ptw_req_valid_dbg_reg & ic_ptw_req_ready_dbg_reg,
                                         ptw_dc_resp_valid_dbg_reg,
                                         dc_ptw_req_valid_dbg_reg & dc_ptw_req_ready_dbg_reg,
                                         // Icache signals
                                         icache_dbg_sm_signals_reg.ucache_f2_valid,
                                         icache_l2_miss_resp_valid_dbg_reg & icache_l2_miss_resp_ready_dbg_reg,
                                         icache_l2_miss_req_valid_dbg_reg & icache_l2_miss_req_ready_dbg_reg,
                                         icache_f0_sram_resp_valid_dbg_reg & icache_f0_sram_resp_ready_dbg_reg,
                                         icache_f2_sram_req_valid_dbg_reg & icache_f2_sram_req_ready_dbg_reg,
                                         icache_dbg_sm_signals_reg.f0_l1_miss_req_valid,
                                         icache_fill_done_dbg_reg,
                                         icache_resp_valid_dbg_reg,
                                         icache_req_valid_dbg_reg & icache_req_ready_dbg_reg});

// Icache debug signals
assign icache_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                     // Signals always connected
                                     icache_l2_miss_resp_valid_dbg_reg & icache_l2_miss_resp_ready_dbg_reg,
                                     icache_l2_miss_resp_info_dbg_reg.opcode,
                                     icache_l2_miss_req_valid_dbg_reg & icache_l2_miss_req_ready_dbg_reg,
                                     icache_l2_miss_req_info_dbg_reg.address[`PA_RANGE_CA],
                                     icache_f0_sram_resp_valid_dbg_reg & icache_f0_sram_resp_ready_dbg_reg,
                                     icache_f2_sram_req_valid_dbg_reg & icache_f2_sram_req_ready_dbg_reg,
                                     icache_f2_sram_req_addr_dbg_reg,
                                     icache_f2_sram_req_write_dbg_reg,
                                     icache_dbg_sm_signals_reg.f0_l1_miss_req_valid,
                                     icache_dbg_sm_signals_reg.f0_l1_miss_req_addr,
                                     // Signals selected per ucache
                                     icache_fill_done_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_valid_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_miss_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].page_fault,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].access_fault,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].cacheable,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].bus_err,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].ecc_err,
                                     icache_dbg_sm_signals_reg.ucache_f2_valid[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.ucache_f2_paddr[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.f1_req_valid[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.f1_req_addr[neigh_ch_dbg_icache_req_idx]});

assign icache_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                   // Signals always connected
                                   icache_l2_miss_resp_info_dbg_reg.opcode,
                                   icache_l2_miss_resp_info_dbg_reg.data[7:0],
                                   icache_f0_sram_resp_dout_dbg_reg[7:0],
                                   icache_f2_sram_req_write_dbg_reg,
                                   icache_dbg_sm_signals_reg.f1_tag_hit,
                                   icache_dbg_sm_signals_reg.f0_miss_state,
                                   icache_dbg_sm_signals_reg.ucache_f0_prefetch_state,
                                   // Signals selected per ucache
                                   icache_dbg_sm_signals_reg.ucache_f0_miss_state[neigh_ch_dbg_icache_req_idx],
                                   icache_resp_miss_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].page_fault,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].access_fault,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].cacheable,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].data[7:0],
                                   icache_dbg_sm_signals_reg.ucache_f2_hit_array[neigh_ch_dbg_icache_req_idx],
                                   icache_dbg_sm_signals_reg.ucache_f2_paddr[neigh_ch_dbg_icache_req_idx],
                                   icache_dbg_sm_signals_reg.f1_req_addr[neigh_ch_dbg_icache_req_idx][42:6]});

// PTW debug signals
assign ptw_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                  // Signals selected per PTW
                                  ptw_l2_miss_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ptw_l2_miss_resp_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].opcode,
                                  ptw_l2_miss_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ptw_l2_miss_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_l2_miss_req_info_dbg_reg[neigh_ch_dbg_ptw_idx].address[`PA_RANGE_CA],
                                  ptw_ic_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_dc_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].canceled_req,
                                  ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].access_fault,
                                  ic_ptw_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ic_ptw_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  dc_ptw_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & dc_ptw_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_satp_mode,
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_prv,
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_addr});

assign ptw_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                // Signals selected per PTW
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].state,
                                ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].opcode,
                                ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].data[7:0],
                                ptw_l2_miss_req_info_dbg_reg[neigh_ch_dbg_ptw_idx].address[`PA_RANGE_CA],
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].canceled_req,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].access_fault,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].d,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].a,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].g,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].u,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].x,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].w,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].r,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].v,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_satp_mode,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_prv,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_addr});

// Cooperative TLoad and TStore debug signals (not flopped)
// ------------------------------------------
// Combine match signals from Cooperative TLoad and TStore
assign tload_tstore_dbg_match_signals = (`NEIGH_DEBUG_MATCH_WIDTH)'({
                                          // TStore signals
                                          tstore_min_resp_valid & tstore_min_resp_ready,
                                          tstore_evict_valid & tstore_miss_req_ready,
                                          coop_evict_valid & coop_evict_ready,
                                          // Cooperative TLoad signals
                                          coop_tload_tag_table_cam_pop,
                                          coop_tload_mst_done_in_valid_lv,
                                          coop_tload_mst_done_out_valid_lv,
                                          coop_tload_slv_rdy_in_valid_lv,
                                          coop_tload_slv_rdy_out_valid_lv,
                                          coop_tload_miss_req_valid & coop_tload_miss_req_ready,
                                          min_coop_miss_req_valid & min_coop_miss_req_ready});

// Cooperative TLoad debug signals
assign tload_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                    coop_tload_tag_table_cam_pop,
                                    coop_tload_tag_table_cam_tag,
                                    coop_tload_mst_done_in_valid_lv,
                                    coop_tload_mst_done_in_coop_id_lv,
                                    coop_tload_mst_done_out_valid_lv,
                                    coop_tload_mst_done_out_coop_id_lv,
                                    coop_tload_slv_rdy_in_valid_lv,
                                    coop_tload_slv_rdy_in_data_lv,
                                    coop_tload_slv_rdy_out_valid_lv,
                                    coop_tload_slv_rdy_out_data_lv,
                                    coop_tload_miss_req_valid & coop_tload_miss_req_ready,
                                    coop_tload_miss_req_info.id,
                                    coop_tload_miss_req_info.address,
                                    coop_tload_miss_req_info.data[19:0]});

assign tload_dbg_data_signals = neigh_ch_dbg_tload_data_idx == `NEIGH_CH_DEBUG_SM_TLOAD_DATA_EXT ?
                                // Cooperative TLoad ports
                                (`NEIGH_DEBUG_DATA_WIDTH)'({
                                 coop_tload_tag_table_cam_tag,
                                 coop_tload_mst_done_in_coop_id_lv,
                                 coop_tload_mst_done_out_coop_id_lv,
                                 coop_tload_slv_rdy_in_data_lv,
                                 coop_tload_slv_rdy_out_data_lv,
                                 coop_tload_miss_req_info.id,
                                 coop_tload_miss_req_info.address,
                                 coop_tload_miss_req_info.data[19:0]})
                                // Cooperative TLoad internal status
                              : (`NEIGH_DEBUG_DATA_WIDTH)'(
                                 coop_tload_dbg_sm_signals);

// TStore debug signals
assign tstore_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                     tstore_min_resp_valid & tstore_min_resp_ready,
                                     tstore_min_resp_opcode,
                                     tstore_evict_valid & tstore_miss_req_ready,
                                     tstore_evict_req.address});

assign tstore_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                   tstore_dbg_sm_signals,            
                                   tstore_min_resp_opcode,
                                   tstore_evict_req.source,
                                   tstore_evict_req.subopcode,
                                   tstore_evict_req.address,
                                   tstore_evict_req.data[7:0],
                                   tstore_evict_req.size,
                                   tstore_evict_req.qwen});

// Debug signals from FLN, TBOX, datapath and FLB (flopped)
// ----------------------------------------------
fln_dbg_sm_t                                  fln_dbg_sm_signals_reg;
logic [`MIN_PER_N-1:0][`ET_LINK_ID_SIZE-1:0]  local_message_info_out_id_dbg_reg;
logic [`MIN_PER_N-1:0][8-1:0]                 local_message_info_out_data_dbg_reg;
logic [`MIN_PER_N-1:0]                        local_message_ready_in_dbg_reg;
logic [`MIN_PER_N-1:0]                        local_message_valid_in_dbg_reg;
et_link_neigh_req_info_t                      min_tbox_req_info_dbg_reg;
logic                                         min_tbox_req_valid_dbg_reg;
logic                                         min_tbox_req_ready_dbg_reg;
et_link_neigh_rsp_info_t                      tbox_min_rsp_info_dbg_reg;
logic                                         tbox_min_rsp_valid_dbg_reg;
logic                                         tbox_min_rsp_ready_dbg_reg;
et_link_nodata_req_info_t                     tbox_miss_req_info_dbg_reg;
logic                                         tbox_miss_req_valid_dbg_reg;
logic                                         tbox_miss_req_ready_dbg_reg;
et_link_rsp_info_t                            sc_tbox_rsp_info_dbg_reg;
logic                                         sc_tbox_rsp_valid_dbg_reg;
logic                                         sc_tbox_rsp_ready_dbg_reg;
et_link_nodata_req_info_t                     miss_req_info_ff_proc_dbg_reg;
logic [`NEIGH_DATA_RANGE]                     miss_req_data_ff_dbg_reg;
logic                                         miss_req_valid_ff_dbg_reg;
logic                                         miss_req_ready_dbg_reg;
logic                                         int_req_push_dbg_reg;
logic                                         int_req_info_pop_dbg_reg;
et_link_nodata_req_info_t                     out_req_info_in_dbg_reg;
logic [`NEIGH_DATA_RANGE]                     out_req_data_in_dbg_reg;
logic [OUT_FIFOS-1:0]                         out_req_push_dbg_reg;
et_link_rsp_info_t                            neigh_sc_rsp_info_lo_dbg_reg;
logic                                         neigh_sc_rsp_valid_lo_dbg_reg;
logic                                         neigh_sc_rsp_ready_lo_dbg_reg;
logic [`MIN_PER_N-1:0]                        flb_core_neigh_req_valid_dbg_reg;
logic [`MIN_PER_N-1:0][`CSR_FL_BARRIER_RANGE] flb_core_neigh_req_data_dbg_reg;
logic [`MIN_PER_N-1:0]                        flb_neigh_core_resp_valid_dbg_reg;
logic                                         flb_neigh_core_resp_data_dbg_reg;

//      CLK       EN                       DOUT                                 DIN
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, fln_dbg_sm_signals_reg,              fln_dbg_sm_signals)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, local_message_info_out_id_dbg_reg,   local_message_info_out_id)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, local_message_info_out_data_dbg_reg, local_message_info_out_data)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, local_message_ready_in_dbg_reg,      local_message_ready_in)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, local_message_valid_in_dbg_reg,      local_message_valid_in)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_tbox_req_info_dbg_reg,           min_tbox_req_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_tbox_req_valid_dbg_reg,          min_tbox_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, min_tbox_req_ready_dbg_reg,          min_tbox_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_min_rsp_info_dbg_reg,           tbox_min_rsp_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_min_rsp_valid_dbg_reg,          tbox_min_rsp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_min_rsp_ready_dbg_reg,          tbox_min_rsp_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_miss_req_info_dbg_reg,          tbox_miss_req_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_miss_req_valid_dbg_reg,         tbox_miss_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, tbox_miss_req_ready_dbg_reg,         tbox_miss_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, sc_tbox_rsp_info_dbg_reg,            sc_tbox_rsp_info)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, sc_tbox_rsp_valid_dbg_reg,           sc_tbox_rsp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, sc_tbox_rsp_ready_dbg_reg,           sc_tbox_rsp_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, miss_req_info_ff_proc_dbg_reg,       miss_req_info_ff_proc)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, miss_req_data_ff_dbg_reg,            miss_req_data_ff_proc)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, miss_req_valid_ff_dbg_reg,           miss_req_valid_ff_proc)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, miss_req_ready_dbg_reg,              miss_req_ready)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, int_req_push_dbg_reg,                int_req_push)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, int_req_info_pop_dbg_reg,            int_req_info_pop)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, out_req_info_in_dbg_reg,             out_req_info_in)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, out_req_data_in_dbg_reg,             out_req_data_in)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, out_req_push_dbg_reg,                out_req_push)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, neigh_sc_rsp_info_lo_dbg_reg,        neigh_sc_rsp_info_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, neigh_sc_rsp_valid_lo_dbg_reg,       neigh_sc_rsp_valid_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, neigh_sc_rsp_ready_lo_dbg_reg,       neigh_sc_rsp_ready_lo)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, flb_core_neigh_req_valid_dbg_reg,    flb_core_neigh_req_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, flb_core_neigh_req_data_dbg_reg,     flb_core_neigh_req_data)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, flb_neigh_core_resp_valid_dbg_reg,   flb_neigh_core_resp_valid)
`EN_FF (clock_lv, neigh_ch_dbg_signals_en, flb_neigh_core_resp_data_dbg_reg,    flb_neigh_core_resp_data)

// Combine match signals from FLN, TBOX, datapath and FLB
assign fln_tbox_dpath_flb_dbg_match_signals = (`NEIGH_DEBUG_MATCH_WIDTH)'({
                                                // FLB signals
                                                flb_neigh_core_resp_valid_dbg_reg,
                                                flb_core_neigh_req_valid_dbg_reg,
                                                // Datapath signals
                                                neigh_sc_rsp_valid_lo_dbg_reg & neigh_sc_rsp_ready_lo_dbg_reg,
                                                out_req_push_dbg_reg,
                                                int_req_info_pop_dbg_reg,
                                                int_req_push_dbg_reg,
                                                miss_req_valid_ff_dbg_reg & miss_req_ready_dbg_reg,
                                                // TBOX signals
                                                sc_tbox_rsp_valid_dbg_reg & sc_tbox_rsp_ready_dbg_reg,
                                                tbox_miss_req_valid_dbg_reg & tbox_miss_req_ready_dbg_reg,
                                                tbox_min_rsp_valid_dbg_reg & tbox_min_rsp_ready_dbg_reg,
                                                min_tbox_req_valid_dbg_reg & min_tbox_req_ready_dbg_reg,
                                                // FLN signals
                                                local_message_valid_in_dbg_reg & local_message_ready_in_dbg_reg,
                                                fln_dbg_sm_signals_reg.local_message_valid_out});

// FLN debug signals
assign fln_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                  fln_dbg_sm_signals_reg.local_message_valid_out,
                                  local_message_info_out_id_dbg_reg});

assign fln_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                local_message_info_out_id_dbg_reg,
                                local_message_info_out_data_dbg_reg});

// TBOX debug signals
assign tbox_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                   sc_tbox_rsp_valid_dbg_reg & sc_tbox_rsp_ready_dbg_reg,
                                   sc_tbox_rsp_info_dbg_reg.opcode,
                                   tbox_miss_req_valid_dbg_reg & tbox_miss_req_ready_dbg_reg,
                                   tbox_miss_req_info_dbg_reg.address[`PA_RANGE_CA],
                                   tbox_min_rsp_valid_dbg_reg & tbox_min_rsp_ready_dbg_reg,
                                   tbox_min_rsp_info_dbg_reg.id,
                                   tbox_min_rsp_info_dbg_reg.dest,
                                   min_tbox_req_valid_dbg_reg & min_tbox_req_ready_dbg_reg,
                                   min_tbox_req_info_dbg_reg.address[`AD_ESR_MESSAGE_ID_RANGE]});

assign tbox_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                 sc_tbox_rsp_info_dbg_reg.opcode,
                                 sc_tbox_rsp_info_dbg_reg.data[7:0],
                                 tbox_miss_req_info_dbg_reg.address[`PA_RANGE_CA],
                                 tbox_min_rsp_info_dbg_reg.id,
                                 tbox_min_rsp_info_dbg_reg.dest,
                                 tbox_min_rsp_info_dbg_reg.data[7:0],
                                 tbox_min_rsp_info_dbg_reg.size,
                                 min_tbox_req_info_dbg_reg.address[`AD_ESR_MESSAGE_ID_RANGE],
                                 min_tbox_req_info_dbg_reg.data[7:0],
                                 min_tbox_req_info_dbg_reg.size});

// Datapath debug signals
assign dpath_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                    neigh_sc_rsp_valid_lo_dbg_reg & neigh_sc_rsp_ready_lo_dbg_reg,
                                    neigh_sc_rsp_info_lo_dbg_reg.id,
                                    neigh_sc_rsp_info_lo_dbg_reg.dest,
                                    neigh_sc_rsp_info_lo_dbg_reg.opcode,
                                    out_req_push_dbg_reg,
                                    out_req_info_in_dbg_reg.id,
                                    out_req_info_in_dbg_reg.source,
                                    out_req_info_in_dbg_reg.opcode,
                                    out_req_info_in_dbg_reg.address,
                                    miss_req_valid_ff_dbg_reg & miss_req_ready_dbg_reg,
                                    miss_req_info_ff_proc_dbg_reg.id,
                                    miss_req_info_ff_proc_dbg_reg.source,
                                    miss_req_info_ff_proc_dbg_reg.opcode,
                                    miss_req_info_ff_proc_dbg_reg.address});

assign dpath_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                  neigh_sc_rsp_info_lo_dbg_reg.id,
                                  neigh_sc_rsp_info_lo_dbg_reg.dest,
                                  neigh_sc_rsp_info_lo_dbg_reg.opcode,
                                  neigh_sc_rsp_info_lo_dbg_reg.data[7:0],
                                  out_req_info_in_dbg_reg.id,
                                  out_req_info_in_dbg_reg.source,
                                  out_req_info_in_dbg_reg.opcode,
                                  out_req_info_in_dbg_reg.address[15:0],
                                  out_req_data_in_dbg_reg[7:0],
                                  miss_req_info_ff_proc_dbg_reg.id,
                                  miss_req_info_ff_proc_dbg_reg.source,
                                  miss_req_info_ff_proc_dbg_reg.opcode,
                                  miss_req_info_ff_proc_dbg_reg.address,
                                  miss_req_data_ff_dbg_reg[7:0]});

// FLB debug signals
assign flb_dbg_filter_signals = (`NEIGH_DEBUG_FILTER_WIDTH)'({
                                  flb_neigh_core_resp_valid_dbg_reg,
                                  flb_neigh_core_resp_data_dbg_reg,
                                  flb_core_neigh_req_valid_dbg_reg,
                                  flb_core_neigh_req_data_dbg_reg});

assign flb_dbg_data_signals = (`NEIGH_DEBUG_DATA_WIDTH)'({
                                flb_neigh_core_resp_data_dbg_reg,
                                flb_core_neigh_req_data_dbg_reg});

// Multiplex debug signals from every neighborhood channel module
always_comb begin
  // North minion debug signals
  if (neigh_ch_dbg_mod_idx < `NEIGH_CH_DEBUG_SM_MOD_MIN4) begin
    neigh_ch_dbg_signals_next.match_signals  = min_north_dbg_match_signals;
    neigh_ch_dbg_signals_next.match_valid    = 1'b1;
    neigh_ch_dbg_signals_next.filter_signals = min_north_dbg_filter_signals;
    neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
    neigh_ch_dbg_signals_next.data_signals   = min_north_dbg_data_signals;
    neigh_ch_dbg_signals_next.data_valid     = 1'b1;
  end else if (neigh_ch_dbg_mod_idx < `NEIGH_CH_DEBUG_SM_MOD_ICACHE) begin
    // South minion debug signals
     neigh_ch_dbg_signals_next.match_signals  = min_south_dbg_match_signals;
     neigh_ch_dbg_signals_next.match_valid    = 1'b1;
     neigh_ch_dbg_signals_next.filter_signals = min_south_dbg_filter_signals;
     neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
     neigh_ch_dbg_signals_next.data_signals   = min_south_dbg_data_signals;
     neigh_ch_dbg_signals_next.data_valid     = 1'b1;
  end else if (neigh_ch_dbg_mod_idx < `NEIGH_CH_DEBUG_SM_MOD_TLOAD)  begin
      // Match signals combine signals from Icache and PTW
      neigh_ch_dbg_signals_next.match_signals = icache_ptw_dbg_match_signals;
      neigh_ch_dbg_signals_next.match_valid   = 1'b1;

    // Icache debug signals
    if (neigh_ch_dbg_mod_idx == `NEIGH_CH_DEBUG_SM_MOD_ICACHE) begin
      neigh_ch_dbg_signals_next.filter_signals = icache_dbg_filter_signals;
      neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
      neigh_ch_dbg_signals_next.data_signals   = icache_dbg_data_signals;
      neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end else begin
        // PTW debug signals

        neigh_ch_dbg_signals_next.filter_signals = ptw_dbg_filter_signals;
        neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
        neigh_ch_dbg_signals_next.data_signals   = ptw_dbg_data_signals;
        neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end
  end else if (neigh_ch_dbg_mod_idx < `NEIGH_CH_DEBUG_SM_MOD_FLN) begin
    // Match signals combine signals from Cooperative TLoad and TStore
    neigh_ch_dbg_signals_next.match_signals = tload_tstore_dbg_match_signals;
    neigh_ch_dbg_signals_next.match_valid   = 1'b1;

    // Cooperative TLoad debug signals
    if (neigh_ch_dbg_mod_idx == `NEIGH_CH_DEBUG_SM_MOD_TLOAD) begin
      neigh_ch_dbg_signals_next.filter_signals = tload_dbg_filter_signals;
      neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
      neigh_ch_dbg_signals_next.data_signals   = tload_dbg_data_signals;
      neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end else begin
        // TStore debug signals
      neigh_ch_dbg_signals_next.filter_signals = tstore_dbg_filter_signals;
      neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
      neigh_ch_dbg_signals_next.data_signals   = tstore_dbg_data_signals;
      neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end
  end else begin
        // Match signals combine signals from FLN, TBOX, datapath and FLB
      neigh_ch_dbg_signals_next.match_signals = fln_tbox_dpath_flb_dbg_match_signals;
      neigh_ch_dbg_signals_next.match_valid   = 1'b1;

        // FLN debug signals
      if (neigh_ch_dbg_mod_idx == `NEIGH_CH_DEBUG_SM_MOD_FLN) begin
          neigh_ch_dbg_signals_next.filter_signals = fln_dbg_filter_signals;
          neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
          neigh_ch_dbg_signals_next.data_signals   = fln_dbg_data_signals;
          neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // TBOX debug signals
        else if (neigh_ch_dbg_mod_idx == `NEIGH_CH_DEBUG_SM_MOD_TBOX) begin
          neigh_ch_dbg_signals_next.filter_signals = tbox_dbg_filter_signals;
          neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
          neigh_ch_dbg_signals_next.data_signals   = tbox_dbg_data_signals;
          neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end else if (neigh_ch_dbg_mod_idx == `NEIGH_CH_DEBUG_SM_MOD_DPATH) begin
        // Datapath debug signals
          neigh_ch_dbg_signals_next.filter_signals = dpath_dbg_filter_signals;
          neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
          neigh_ch_dbg_signals_next.data_signals   = dpath_dbg_data_signals;
          neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end else begin
        // FLB debug signals
          neigh_ch_dbg_signals_next.filter_signals = flb_dbg_filter_signals;
          neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
          neigh_ch_dbg_signals_next.data_signals   = flb_dbg_data_signals;
          neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
    end
end

////////////////////////////////////////////////////////////////////////////////
// APB Shire Bus Master/Shire BPAM
////////////////////////////////////////////////////////////////////////////////
apb_ff
#(
   .ADDR_WIDTH ( `NEIGH_DM_APB_ADDR_WIDTH ),
   .DATA_WIDTH ( `ESR_APB_D_WIDTH         )
)
apb_ff
(
   // System signals
   .clock           ( clock_lv                          ),
   .reset           ( reset_c_neigh_ff                  ),
   // Input slave side
   .apb_in_psel     ( APB_req_from_shire.apb_psel       ),
   .apb_in_penable  ( APB_req_from_shire.apb_penable    ),
   .apb_in_paddr    ( APB_req_from_shire.apb_paddr      ),
   .apb_in_pwrite   ( APB_req_from_shire.apb_pwrite     ),
   .apb_in_pwdata   ( APB_req_from_shire.apb_pwdata     ),
   .apb_in_pready   ( APB_rsp_to_shire.apb_pready       ),
   .apb_in_prdata   ( APB_rsp_to_shire.apb_prdata       ),
   .apb_in_pslverr  ( APB_rsp_to_shire.apb_pslverr      ),
   // Output master side
   .apb_out_psel    ( APB_req_from_shire_lo.apb_psel    ),
   .apb_out_penable ( APB_req_from_shire_lo.apb_penable ),
   .apb_out_paddr   ( APB_req_from_shire_lo.apb_paddr   ),
   .apb_out_pwrite  ( APB_req_from_shire_lo.apb_pwrite  ),
   .apb_out_pwdata  ( APB_req_from_shire_lo.apb_pwdata  ),
   .apb_out_pready  ( APB_rsp_to_shire_lo.apb_pready    ),
   .apb_out_prdata  ( APB_rsp_to_shire_lo.apb_prdata    ),
   .apb_out_pslverr ( APB_rsp_to_shire_lo.apb_pslverr   )
);

////////////////////////////////////////////////////////////////////////////////
// HI VOLTAGE CROSSING
////////////////////////////////////////////////////////////////////////////////
bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi_next;

neigh_hv_logic_bpam_rc_tbox_ack neigh_hv_logic_bpam_rc_tbox_ack_pd_hv (
  .clock                       ( clock_lv                 ),
  .reset                       ( reset_d_neigh_ff         ),
  .bpam_rc_tbox_ack_hi_reg_in  ( bpam_rc_tbox_ack_hi_next ),
  .bpam_rc_tbox_ack_hi_reg_out ( bpam_rc_tbox_ack_hi      )
);

logic reset_c_neigh_hvc ;
rst_repeat rst_repeat_reset_c_hvc (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_c_neigh), .reset_out(reset_c_neigh_hvc));

neigh_hi_voltage_cross #(
  .NUM_ELEMS (`SHIRE_HI_VOLTAGE_FIFO_SIZE)
) hi_voltage_cross (
  // System signals
  .reset_c                           ( reset_c_neigh_hvc               ),
  .reset_w                           ( reset_w_neigh_ff                ),
  .clock                             ( clock_lv                        ),

  .neigh_sm_gpio                     ( neigh_sm_gpio                   ),
  .neigh_sm_gpio_sync                ( neigh_sm_gpio_lo                ),
  .shire_id                          ( shire_id                        ),
  .shire_id_sync                     ( shire_id_lo                     ),
  .neigh_id                          ( neigh_id                        ),
  .neigh_id_sync                     ( neigh_id_lo                     ),
  .tbox_id                           ( shire_tbox_id                   ),
  .tbox_id_sync                      ( tbox_id_lo                      ),
  .tbox_en                           ( shire_tbox_en                   ),
  .tbox_en_sync                      ( tbox_en_lo                      ),
  .neigh_t0_en                       ( esr_thread0_enable              ),
  .neigh_t0_en_sync                  ( esr_thread0_enable_lo           ),
  .neigh_t1_en                       ( esr_thread1_enable              ),
  .neigh_t1_en_sync                  ( esr_thread1_enable_lo           ),
  .esr_minion_features               ( esr_minion_features             ),
  .esr_minion_features_sync          ( esr_minion_features_lo          ),
  .chicken_bits                      ( chicken_bits_vc                 ),
  .chicken_bits_sync                 ( chicken_bits_vc_lo              ),

  .hw_dbg_sm_monitor_enabled         ( hw_dbg_sm_monitor_enabled       ),
  .hw_dbg_sm_monitor_enabled_sync    ( hw_dbg_sm_monitor_enabled_lo    ), 

  .esr_icache_prefetch_conf          ( esr_icache_prefetch_conf        ),
  .esr_icache_prefetch_conf_sync     ( esr_icache_prefetch_conf_lo     ),
  .esr_icache_prefetch_start         ( esr_icache_prefetch_start       ),
  .esr_icache_prefetch_start_sync    ( esr_icache_prefetch_start_lo    ),
  .dmctrl                            ( dmctrl                          ),
  .dmctrl_sync                       ( dmctrl_lo                       ),

  .esr_shire_coop_mode               ( esr_shire_coop_mode             ),
  .esr_shire_coop_mode_sync          ( esr_shire_coop_mode_lo          ),
   
  .neigh_sc_rsp_valid                ( neigh_sc_rsp_valid              ),
  .neigh_sc_rsp_valid_sync           ( neigh_sc_rsp_valid_lo           ),
  .neigh_sc_rsp_info                 ( neigh_sc_rsp_info               ),
  .neigh_sc_rsp_info_sync            ( neigh_sc_rsp_info_lo            ),
  .neigh_sc_rsp_ready                ( neigh_sc_rsp_ready_lo           ),
  .neigh_sc_rsp_ready_sync           ( neigh_sc_rsp_ready              ),

  .int_mtip                          ( int_mtip                        ),
  .plic_meip                         ( int_meip                        ),
  .plic_seip                         ( int_seip                        ),
  .int_mtip_sync                     ( int_mtip_lo                     ),
  .plic_meip_sync                    ( int_meip_lo                     ),
  .plic_seip_sync                    ( int_seip_lo                     ),

  .esr_to_neigh_ipi_msip             ( ipi_msip                        ),
  .esr_to_neigh_ipi_msip_sync        ( ipi_msip_lo                     ),
  .esr_to_neigh_ipi_trigger          ( ipi_redirect_trigger            ),
  .esr_to_neigh_ipi_trigger_sync     ( ipi_redirect_trigger_lo         ),
  .uc_to_neigh_fcc                   ( uc_to_neigh_fcc                 ),
  .uc_to_neigh_fcc_sync              ( uc_to_neigh_fcc_lo              ),
  .uc_to_neigh_fcc_target            ( uc_to_neigh_fcc_target          ),
  .uc_to_neigh_fcc_target_sync       ( uc_to_neigh_fcc_target_lo       ),

  .flb_l2_neigh_resp_valid           ( flb_l2_neigh_resp_valid         ),
  .flb_l2_neigh_resp_valid_sync      ( flb_l2_neigh_resp_valid_lo      ),
  .flb_l2_neigh_resp_data            ( flb_l2_neigh_resp_data          ),
  .flb_l2_neigh_resp_data_sync       ( flb_l2_neigh_resp_data_lo       ),

  .icache_f0_sram_resp_dout          ( icache_f0_sram_resp_dout        ),
  .icache_f0_sram_resp_dout_sync     ( icache_f0_sram_resp_dout_lo     ),
  .icache_f0_sram_resp_valid         ( icache_f0_sram_resp_valid       ),
  .icache_f0_sram_resp_valid_sync    ( icache_f0_sram_resp_valid_lo    ),
  .icache_f0_sram_resp_ready         ( icache_f0_sram_resp_ready_lo    ),   
  .icache_f0_sram_resp_ready_sync    ( icache_f0_sram_resp_ready       )   
); //neigh_hi_voltage_cross 

neigh_lo_voltage_cross #(
  .NUM_ELEMS (`SHIRE_LO_VOLTAGE_FIFO_SIZE)
) lo_voltage_cross (
  .reset_w                           ( reset_w_neigh_ff                ),
  .reset_d                           ( reset_d_neigh_ff                ),
  .clock                             ( clock_lv                        ),
  
  .esr_icache_prefetch_done          ( esr_icache_prefetch_done_lo     ),
  .esr_icache_prefetch_done_sync     ( esr_icache_prefetch_done        ),

  .esr_icache_err_detected           ( esr_icache_err_detected_lo      ),
  .esr_icache_err_detected_sync      ( esr_icache_err_detected_op      ),
  .esr_icache_err_logged             ( esr_icache_err_logged_lo        ),
  .esr_icache_err_logged_sync        ( esr_icache_err_logged_op        ),

  .bpam_rc_tbox_ack                  ( tbox_bpam_run_control_ack       ),
  .bpam_rc_tbox_ack_sync             ( bpam_rc_tbox_ack_hi_next        ),

  .esr_and_or_tree_L0                ( esr_and_or_tree_L0_lo           ),
  .esr_and_or_tree_L0_sync           ( esr_and_or_tree_L0_op           ),

  .neigh_sm_signals                  ( neigh_sm_signals_lo             ),
  .neigh_sm_signals_sync             ( neigh_sm_signals                ), 

  .flb_neigh_l2_req_valid            ( flb_neigh_l2_req_valid_lo       ),      
  .flb_neigh_l2_req_valid_sync       ( flb_neigh_l2_req_valid          ),      
  .flb_neigh_l2_req_data             ( flb_neigh_l2_req_data_lo        ),      
  .flb_neigh_l2_req_data_sync        ( flb_neigh_l2_req_data           ),      

  .icache_f2_sram_req_write          ( icache_f2_sram_req_write_lo     ),
  .icache_f2_sram_req_write_sync     ( icache_f2_sram_req_write        ),
  .icache_f2_sram_req_addr           ( icache_f2_sram_req_addr_lo      ),
  .icache_f2_sram_req_addr_sync      ( icache_f2_sram_req_addr         ),
  .icache_f2_sram_req_valid          ( icache_f2_sram_req_valid_lo     ),
  .icache_f2_sram_req_valid_sync     ( icache_f2_sram_req_valid        ),
  .icache_f2_sram_req_ready          ( icache_f2_sram_req_ready        ),
  .icache_f2_sram_req_ready_sync     ( icache_f2_sram_req_ready_lo     )
); //shire_lo_voltage_cross

////////////////////////////////////////////////////////////////////////////////
// warm reset for shire icache fifo wr
////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_icache_pd_hv(
  .dft__reset_byp( dft__reset_byp_hv ),
  .dft__reset    ( dft__reset_hv     ),
  .clock         ( clock             ),
  .reset_in      ( reset_w_neigh_ff  ),
  .reset_out     ( reset_w_icache    )
);


////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////

function automatic logic isGlobalAtomic;
  input et_link_nodata_req_info_t req;
  begin
      isGlobalAtomic = (req.opcode == ET_LINK_REQ_Atomic) &&
                       (req.subopcode[`ET_ATOMIC_DEST_START] == `ET_ATOMIC_DEST_GLOBAL);
  end
endfunction

function automatic logic isMsgToTbox;
  input et_link_nodata_req_info_t req;
  begin
      isMsgToTbox = ((req.opcode == ET_LINK_REQ_MsgSendData)
                  && (req.address[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION)
                  && (req.address[`AD_ESR_AGENT_TYPE_RANGE] == `AD_ESR_AGENT_TYPE_TBOX));
  end
endfunction
   
function automatic logic isMsgToSameShire;
  input et_link_nodata_req_info_t req;
  input logic [`AD_ESR_SHIRE_ID_SIZE-1:0] shire_id;
  begin
      isMsgToSameShire = (req.opcode == ET_LINK_REQ_MsgSendData)
                       && (req.address[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION)
                       && ((req.address[`AD_ESR_SHIRE_ID_RANGE] == shire_id) ||
                           (req.address[`AD_ESR_SHIRE_ID_RANGE] == `LOCAL_SHIRE_ID));
  end
endfunction
   
function automatic logic isMsgToDifferentShire;
  input et_link_nodata_req_info_t req;
  input logic [`AD_ESR_SHIRE_ID_SIZE-1:0]  shire_id;
  begin
      isMsgToDifferentShire = (req.opcode == ET_LINK_REQ_MsgSendData)
                            && (req.address[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION)
                            && (req.address[`AD_ESR_SHIRE_ID_RANGE] != shire_id)
                            && (req.address[`AD_ESR_SHIRE_ID_RANGE] != `LOCAL_SHIRE_ID);
  end
endfunction

function automatic logic isUncacheableReadWrite;
    input et_link_nodata_req_info_t req;
    begin
        isUncacheableReadWrite = ((req.opcode == ET_LINK_REQ_Read)  ||
                                  (req.opcode == ET_LINK_REQ_Write) ||
                                  (req.opcode == ET_LINK_REQ_ReadCoop))
                               && !(req.address[`AD_DDR_REGION_RANGE] == `AD_DDR_REGION) // It is not in DDR/MRAM region
                               && !(req.address[`AD_BOOTROM_REGION_RANGE] == `AD_BOOTROM_REGION) // It is not in BootROM region
                               && !(req.address[`AD_SRAM_REGION_RANGE] == `AD_SRAM_REGION); // It is not in SRAM region
    end
endfunction

function automatic logic isWriteAroundtoLocalSCP;
    input et_link_nodata_req_info_t req;
    input logic [`NUM_SHIRE_IDS_R]  shire_id;
    begin
        isWriteAroundtoLocalSCP = (req.opcode == ET_LINK_REQ_WriteAround)
                                && (req.address[`AD_SCP_REGION_RANGE] == `AD_SCP_REGION)
                                && ((req.address[`AD_SCP_FL2_SHIRE_ID_RANGE] == shire_id) ||
                                    (req.address[`AD_SCP_FL2_SHIRE_ID_RANGE] == `LOCAL_SHIRE_ID));
    end
endfunction

function automatic logic isAgentForced;
  input et_link_neigh_source_t source;
  input esr_neigh_chicken_t    chicken;
  begin
      isAgentForced = ((chicken.agent_forced == `NEIGH_FORCE_AGENT_DCACHE) &&
                        ((source == ET_LINK_NEIGH_SRC_Minion0) ||
                         (source == ET_LINK_NEIGH_SRC_Minion1) ||
                         (source == ET_LINK_NEIGH_SRC_Minion2) ||
                         (source == ET_LINK_NEIGH_SRC_Minion3) ||
                         (source == ET_LINK_NEIGH_SRC_Minion4) ||
                         (source == ET_LINK_NEIGH_SRC_Minion5) ||
                         (source == ET_LINK_NEIGH_SRC_Minion6) ||
                         (source == ET_LINK_NEIGH_SRC_Minion7) ||
                         (source == ET_LINK_NEIGH_SRC_Coop))
                      ) ||
                      ((chicken.agent_forced == `NEIGH_FORCE_AGENT_ICACHE) &&
                        (source == ET_LINK_NEIGH_SRC_Icache)
                      ) ||
                      ((chicken.agent_forced == `NEIGH_FORCE_AGENT_TBOX) &&
                        (source == ET_LINK_NEIGH_SRC_Tbox)
                      ) ||
                      ((chicken.agent_forced == `NEIGH_FORCE_AGENT_PTW) &&
                        ((source == ET_LINK_NEIGH_SRC_Ptw0) ||
                         (source == ET_LINK_NEIGH_SRC_Ptw1))
                      );
  end
endfunction

function automatic [DEST_FIFOS_L-1:0] destFifoForced;
  input et_link_nodata_req_info_t req;
  input esr_neigh_chicken_t       chicken;

  logic [`SC_BANK_ID_SIZE-1:0] bank_id;
  logic [`SC_BANK_ID_SIZE-1:0] neigh_to_bank_id;
  begin
        bank_id          = req.address[`SC_BANK_SEL_ADDR_RANGE];
        neigh_to_bank_id = (`SC_BANK_ID_SIZE)'(req.address[`AD_ESR_NEIGH_ID_START+:`NUM_NEIGH_L]);
 
        destFifoForced = chicken.dest_fifo == `NEIGH_FORCE_DEST_FIFO_BANKS ?
                         // If forced to bank FIFOs, choose bank according to address
                         ( req.opcode == ET_LINK_REQ_MsgSendData           ? DEST_FIFOS_L'(neigh_to_bank_id) // Message goes to bank according to neigh ID LSBs
                         :                                                   DEST_FIFOS_L'(bank_id)          // Otherwise, go to corresponding bank FIFO
                         )
                       : chicken.dest_fifo == `NEIGH_FORCE_DEST_FIFO_UC    ? DEST_FIFOS_L'(UC_FIFO)          // Go to UC FIFO
                       :                                                     DEST_FIFOS_L'(TBOX_FIFO);       // Go to TBOX FIFO
  end
endfunction

function automatic [DEST_FIFOS_L-1:0] destFifo;
  input et_link_nodata_req_info_t req;
  input logic [`AD_ESR_SHIRE_ID_SIZE-1:0]  sid;
  input esr_neigh_chicken_t       chicken;

  logic [`SC_BANK_ID_SIZE-1:0] bank_id;
  logic [`SC_BANK_ID_SIZE-1:0] neigh_to_bank_id;
  begin
        bank_id          = req.address[`SC_BANK_SEL_ADDR_RANGE];
        neigh_to_bank_id = (`SC_BANK_ID_SIZE)'(req.address[`AD_ESR_NEIGH_ID_START+:`NUM_NEIGH_L]);
 
        destFifo = chicken.force_dest_fifo & (chicken.force_all_agents | isAgentForced(et_link_neigh_source_t'(req.source),chicken)) ? destFifoForced(req,chicken) // If forced, send to forced destination FIFO
                 // Global atomics
                 : isGlobalAtomic(req)            ? DEST_FIFOS_L'(UC_FIFO)          // Global atomics go directly to L3, so they are handled by the UC
                 // Messages
                 : isMsgToTbox(req)               ? DEST_FIFOS_L'(TBOX_FIFO)        // Message to TBOX goes to TBOX FIFO
                 : isMsgToSameShire(req,sid)      ? DEST_FIFOS_L'(neigh_to_bank_id) // Message to same shire goes to bank FIFOs according to neigh ID LSBs
                 : isMsgToDifferentShire(req,sid) ? DEST_FIFOS_L'(UC_FIFO)          // Message to different shire goes to UC FIFO
                 // Uncacheable Reads/Writes
                 : isUncacheableReadWrite(req)    ? DEST_FIFOS_L'(UC_FIFO)          // Regular uncacheable read/write accesses go to UC FIFO (neither in SCP nor in DDR regions)
                 // Other: local Atomics, WriteArounds, CacheOps and
                 //        cacheable Reads, ReadCoops and Writes
                 :                                  DEST_FIFOS_L'(bank_id);         // Always go to Shire Cache, so go to corresponding bank FIFO
  end
endfunction

endmodule

