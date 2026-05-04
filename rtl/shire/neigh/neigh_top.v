// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"


// This is a neighborhood of N minions in a shire
module neigh_top #(
`ifdef ET_SIMULATION
  parameter int              SHIRE_ID        = 0,
  parameter int              NEIGH_ID        = 0,
`endif // ifdef ET_SIMULATION
  parameter [`MIN_PER_N-1:0] DISABLE_MINIONS = {`MIN_PER_N{1'b0}},
  parameter [`MIN_PER_N-1:0] STUB_MINIONS    = {`MIN_PER_N{1'b0}}
) (
    // North HiV interface
    // -------------------
    // System signals
  input logic                                                       clock,                   //HiV neigh clock 
  input logic                                                       reset_cold,
  input logic                                                       reset_warm, 
  output logic                                                      reset_w_icache,           //To the Shire Icache HiV reset

  // DFT signals from shire_channel_wrap
  input  logic [`SHIRE_DFT_SCANIN_NEIGH_WIDTH-1:0]                  dft__scanin_hv,           //Scan In
  output logic [`SHIRE_DFT_SCANOUT_NEIGH_WIDTH-1:0]                 dft__scanout_hv,          //Scan Out
  input  logic                                                      dft__scan_mode_hv,        //Scan Mode
  input  logic                                                      dft__reset_byp_hv,        //Reset byp
  input  logic                                                      dft__scan_enable_hv,      //Scan Enable
  input  logic [`SHIRE_DFT_MODE_WIDTH-1:0]                          dft__test_mode_hv,        //Compression selection
  input  logic                                                      dft__reset_hv,            //DFT mode reset
  input  logic                                                      dft__clock_gate_en_hv,    //For ICG TE connection
  input  logic [`SHIRE_DFT_CNTL_WIDTH-1:0]                          dft__cntl_hv,             //spare bus
  input  logic                                                      dft__occ_scanin_hv,       //OCC scan input
  output logic                                                      dft__occ_scanout_hv,      //OCC scan output
  input  logic                                                      dft__occ_reset_hv,        //OCC Reset
  input  logic                                                      dft__occ_testmode_hv,     //OCC testmode
  input  logic                                                      dft__scan_ate_clk_hv,     //ATE shift clock
  input  logic                                                      dft__occ_bypass_hv,       //OCC bypass

  input  logic                                                      dft__use_reset_cntl_hv,   // DFT reset control over sys_gasket_lov
  input  `toVector(shire_tdr_reset_cntl_t)                          dft__reset_cntl_hv,       // DFT reset control over sys_gasket_lov

  // DFT signals - coming in from shire_channel_wrap at _hv and need to be level shifted within neigh
  `ifdef GFX_ENABLED
  input  logic [`SHIRE_DFT_SCANIN_TBOX_WIDTH-1:0]                   dft__scanin_tbox_hv,      //Scan In
  output logic [`SHIRE_DFT_SCANOUT_TBOX_WIDTH-1:0]                  dft__scanout_tbox_hv,     //Scan Out
  input  logic                                                      dft__occ_scanin_tbox_hv,  //OCC scan input
  output logic                                                      dft__occ_scanout_tbox_hv, //OCC scan output
  `endif //GFX_ENABLED

  // ECO ports
  input  logic [`ET_ECO_NEIGH_INPUT_WIDTH-1:0]                      eco_i,
  output logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0]                     eco_o,

  // power (udf ctrl ports)
  input  logic                                                      pwr_ctrl_glb_nsleepin,
  output logic                                                      pwr_ctrl_glb_nsleepout,
  input  logic                                                      pwr_ctrl_glb_isolate,
  input  logic [`MIN_PER_N-1:0]                                     pwr_ctrl_min_nsleepin,
  output logic [`MIN_PER_N-1:0]                                     pwr_ctrl_min_nsleepout,
  input  logic [`MIN_PER_N-1:0]                                     pwr_ctrl_min_isolate,

  // Clock gate Ctrl override
  input  `toVector(minion_chicken_bits_vc_t)                        esr_clk_gate_ctrl, 
    
  // Memory control override
  input logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]                 esr_minion_mem_override,

  // Debug
  output `toVector(bpam_rc_tbox_ack_t)                              bpam_rc_tbox_ack_hi,     //from neigh channel to shire

  // ESRs
  input  logic [`NUM_SHIRE_IDS_R]                                   shire_id,
  input  logic [`NUM_NEIGH_R]                                       neigh_id,
  input  logic [`TBOX_PER_SHIRE_R]                                  shire_tbox_id,
  input  logic                                                      shire_tbox_en,
  input  logic [`MIN_PER_N-1:0]                                     esr_thread0_enable,
  input  logic [`MIN_PER_N-1:0]                                     esr_thread1_enable,
  input  logic [`ESR_MINION_FEATURE_SZ-1:0]                         esr_minion_features,
  input  `toVector(icache_prefetch_conf_t)                          esr_icache_prefetch_conf,
  input  logic                                                      esr_icache_prefetch_start,
  output logic                                                      esr_icache_prefetch_done,
  output logic                                                      esr_icache_err_detected,
  output logic                                                      esr_icache_err_logged,
  input  `toVector(esr_ms_dmctrl_t)                                 dmctrl,
  output `toVector(esr_and_or_tree_L0_t)                            esr_and_or_tree_L0,
  input  logic                                                      esr_shire_coop_mode,
    
  // L2 Request bus
  input  logic [(`SC_BANKS+1)-1:0]                                  neigh_sc_req_ready,
  output logic [(`SC_BANKS+1)-1:0]                                  neigh_sc_req_valid,
  output `toVector(et_link_req_info_t)                              neigh_sc_req_info,
  // L2 Response bus
  output logic                                                      neigh_sc_rsp_ready,
  input  logic                                                      neigh_sc_rsp_valid,
  input  `toVector(et_link_rsp_info_t)                              neigh_sc_rsp_info,
    
  // UltraSoc Status monitor signals
  input  logic                                                      hw_dbg_sm_monitor_enabled,
  input  logic   [`neigh_sm_gpio_width - 1:0]                       neigh_sm_gpio, 
  output `toVector(neigh_sm_dbg_monitor_t)                          neigh_sm_signals,
  // APB from Shire Bus Master or Shire BPAM to read/write ESR
  input  `toVector(APB_to_neigh_t)                                  APB_ESR_req,
  output `toVector(APB_from_neigh_t)                                APB_ESR_rsp,

  // Interrupts
  input  logic [`THREADS_PER_N-1:0]                                 int_mtip,
  input  logic [`THREADS_PER_N-1:0]                                 int_meip,
  input  logic [`THREADS_PER_N-1:0]                                 int_seip,

  // IPI
  input logic [`THREADS_PER_N-1:0]                                  ipi_msip,
  input logic [`THREADS_PER_N-1:0]                                  ipi_redirect_trigger,
  input logic [`MIN_PER_N-1:0]                                      uc_to_neigh_fcc,
  input logic [`UC_FCC_TARGET_SIZE-1:0]                             uc_to_neigh_fcc_target,
  // Fast Local Barrier request interface between neigh and L2
  output logic                                                      flb_neigh_l2_req_valid,
  output logic [`CSR_FL_BARRIER_RANGE]                              flb_neigh_l2_req_data,
  // Fast Local Barrier response interface between neigh and L2
  input logic                                                       flb_l2_neigh_resp_valid,
  input logic                                                       flb_l2_neigh_resp_data,
  // Request to Icache L1 SRAM blocks
  output logic                                                      icache_f2_sram_req_write,
  output logic [`ICACHE_SRAM_ADD_WIDTH-1:0]                         icache_f2_sram_req_addr,
  output logic                                                      icache_f2_sram_req_valid,
  input  logic                                                      icache_f2_sram_req_ready,
  // Response from Icache L1 SRAM blocks
  input  logic [`ICACHE_SRAM_DATA_WIDTH-1:0]                        icache_f0_sram_resp_dout,
  input  logic                                                      icache_f0_sram_resp_valid,
  output logic                                                      icache_f0_sram_resp_ready,
  // Voltage monitor
  output wire                                                       voltage_monitor_vdd,
  output wire                                                       voltage_monitor_vss,

`ifdef GFX_ENABLED
  // South TBOX LoV interface
  // ------------------------
  // System signals
  output logic                                                      tbox_clock,              //LoV output clock to TBox
  output logic                                                      tbox_reset_c,            //To the TBox LoV reset
  output logic                                                      tbox_reset_d,            //To the TBox LoV reset
  output logic                                                      tbox_reset_w,            //To the TBox LoV reset 

  // DFT signals from neigh_top to tbox
  output logic [`SHIRE_DFT_SCANIN_TBOX_WIDTH-1:0]                   dft__scanin_to_tbox_lv,
  input  logic [`SHIRE_DFT_SCANOUT_TBOX_WIDTH-1:0]                  dft__scanout_to_tbox_lv,
  output logic                                                      dft__scan_mode_to_tbox_lv,
  output logic                                                      dft__reset_byp_to_tbox_lv,
  output logic                                                      dft__scan_enable_to_tbox_lv,
  output logic [`SHIRE_DFT_MODE_WIDTH-1:0]                          dft__test_mode_to_tbox_lv,
  output logic                                                      dft__reset_to_tbox_lv,
  output logic                                                      dft__clock_gate_en_to_tbox_lv,
  output logic [`SHIRE_DFT_CNTL_WIDTH-1:0]                          dft__cntl_to_tbox_lv,
  output logic                                                      dft__occ_scanin_to_tbox_lv,
  input  logic                                                      dft__occ_scanout_to_tbox_lv,
  output logic                                                      dft__occ_reset_to_tbox_lv,
  output logic                                                      dft__occ_testmode_to_tbox_lv,
  output logic                                                      dft__scan_ate_clk_to_tbox_lv,
  output logic                                                      dft__occ_bypass_to_tbox_lv,

  // ESRs
  output logic [`TBOX_PER_SHIRE_R]                                  tbox_id_lo,
  output logic [15:0]                                               tbox_control_esr,
  output logic [`VA_SIZE-1:0]                                       tbox_image_table_ptr_esr,
  // TBOX neighborhood request bus
  output logic                                                      tbox_neigh_req_ready_lo,
  input  logic                                                      tbox_neigh_req_valid_lo,
  input  `toVector(et_link_neigh_req_info_t)                        tbox_neigh_req_info_lo,
  // TBOX neighborhood response bus
  input  logic                                                      tbox_neigh_rsp_ready_lo,
  output logic                                                      tbox_neigh_rsp_valid_lo,
  output `toVector(et_link_rsp_info_t)                              tbox_neigh_rsp_info_lo,
  // TBOX monitor signals
  output  logic                                                     tbox_dbg_signals_en,
  input  `toVector(neigh_sm_dbg_monitor_t)                          tbox_dbg_signals,
  output  logic   [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]                   tbox_dbg_signals_mux,
  // APB from Shire Bus Master or BPAM to read/write TBOX           
  output  `toVector(APB_to_neigh_tbox_t)                            tbox_APB_req,
  input   `toVector(APB_from_neigh_t)                               tbox_APB_rsp,
  // Debug
  output  `toVector(tbox_bpam_run_control_t)                        tbox_bpam_run_control,
  input   `toVector(bpam_rc_tbox_ack_t)                             tbox_bpam_run_control_ack, //from TBOX to neigh channel 
`endif
    
  // North LoV interface
  // -------------------
  // Cooperative TLoad slave ready output bus
  output `toVector(coop_tload_slv_rdy_req_t)                        coop_tload_slv_rdy_out_data,
  output logic                                                      coop_tload_slv_rdy_out_valid,
  // Cooperative TLoad slave ready input buses
  input  `toVectorExt(coop_tload_slv_rdy_req_t,[(`NUM_NEIGH-1)-1:0])coop_tload_slv_rdy_in_data,
  input  logic [(`NUM_NEIGH-1)-1:0]                                 coop_tload_slv_rdy_in_valid,
  // Cooperative TLoad master done output bus
  output logic [`SHIRE_COOP_ID_RANGE]                               coop_tload_mst_done_out_coop_id,
  output logic [(`NUM_NEIGH-1)-1:0]                                 coop_tload_mst_done_out_valid,
  // Cooperative TLoad master done input buses
  input  logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]           coop_tload_mst_done_in_coop_id,
  input  logic [(`NUM_NEIGH-1)-1:0]                                 coop_tload_mst_done_in_valid

);

`ifndef GFX_ENABLED
  logic                                            tbox_clock;         //LoV output clock to TBox
  logic                                            tbox_reset_c;       //To the TBox LoV reset
  logic                                            tbox_reset_d;       //To the TBox LoV reset
  logic                                            tbox_reset_w;       //To the TBox LoV reset 
  // ESRs
  logic [`TBOX_PER_SHIRE_R]                        tbox_id_lo;
  logic [15:0]                                     tbox_control_esr;
  logic [`VA_SIZE-1:0]                             tbox_image_table_ptr_esr;
  // TBOX neighborhood request bus
  logic                                            tbox_neigh_req_ready_lo;
  logic                                            tbox_neigh_req_valid_lo;
  `toVector(et_link_neigh_req_info_t)              tbox_neigh_req_info_lo;
  // TBOX neighborhood response bus
  logic                                            tbox_neigh_rsp_ready_lo;
  logic                                            tbox_neigh_rsp_valid_lo;
  `toVector(et_link_rsp_info_t)                    tbox_neigh_rsp_info_lo;
  // TBOX monitor signals
  logic                                            tbox_dbg_signals_en;
  `toVector(neigh_sm_dbg_monitor_t)                tbox_dbg_signals;
  logic   [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]          tbox_dbg_signals_mux;
  // APB from Shire Bus Master or BPAM to read/write TBOX 
  `toVector(APB_to_neigh_tbox_t)                   tbox_APB_req;
  `toVector(APB_from_neigh_t)                      tbox_APB_rsp;
  // Debug
  `toVector(tbox_bpam_run_control_t)               tbox_bpam_run_control;
  `toVector(bpam_rc_tbox_ack_t)                    tbox_bpam_run_control_ack; //from TBOX to neigh channel

  // Set all input ports from TBOX not instantiated because of GFX disabled
  assign tbox_neigh_req_valid_lo      = '0;
  assign tbox_neigh_req_info_lo       = '0;
  assign tbox_neigh_rsp_ready_lo      = '0;
  assign tbox_dbg_signals             = '0;
  assign tbox_APB_rsp                 = '1;
  assign tbox_bpam_run_control_ack    = '0;
`endif // GFX_ENABLED

logic [`MIN_PER_N-1:0]                      reset_c_minion;
logic [`MIN_PER_N-1:0]                      reset_d_minion;
logic [`MIN_PER_N-1:0]                      reset_w_minion;
logic [`NUM_SHIRE_IDS_R]                    shire_id_lo;
logic [`ESR_MINION_FEATURE_SZ-1:0]          esr_minion_features_lo;
logic [`MIN_PER_N-1:0]                      thread0_enable;
logic [`MIN_PER_N-1:0]                      thread1_enable;
logic                                       esr_shire_coop_mode_lo;
logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0] esr_minion_mem_override_lo;

//Connection to Ultrasoc modules - hwdebug
logic   [`MIN_PER_N-1:0]    minion_dbg_signals_en;

// APB signals to minions
logic [`MIN_PER_N-1:0][`ESR_APB_D_WIDTH - 1:0]          minion_apb_prdata; 
logic [`MIN_PER_N-1:0]                                  minion_apb_pready;   
logic [`MIN_PER_N-1:0]                                  minion_apb_pslverr;
logic                                                   minion_apb_penable; 
logic [`MIN_PER_N-1:0]                                  minion_apb_psel; 
logic                                                   minion_apb_pwrite; 
logic [`MIN_DM_APB_ADDR_WIDTH-1:0]                      minion_apb_paddr; 
logic [`ESR_APB_D_WIDTH  - 1:0]                         minion_apb_pwdata; 

////////////////////////////////////////////////////////////////////////////////
// Center channel with the Icaches, PTWs, trans ROM, arbiters, FF to access L2
////////////////////////////////////////////////////////////////////////////////

wire [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0]          l2_dcache_evict_req_ready;
wire [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0]          l2_dcache_evict_req_valid;
wire [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]           l2_dcache_miss_req_ready;
wire [`MIN_PER_N-1:0][`DCACHE_L2_MISS_REQ_PORTS-1:0]           l2_dcache_miss_req_valid;
`EVL_WIRE et_link_minion_evict_req_info_t [`MIN_PER_N-1:0]     l2_dcache_evict_req;
`EVL_WIRE et_link_minion_miss_req_info_t [`MIN_PER_N-1:0]      l2_dcache_miss_req;
wire [`MIN_PER_N-1:0]                                          l2_dcache_resp_ready;
wire [`MIN_PER_N-1:0]                                          l2_dcache_resp_valid;
`EVL_WIRE et_link_minion_rsp_info_t [`MIN_PER_N-1:0]           l2_dcache_resp;
frontend_icache_req [`MIN_PER_N-1:0]                           icache_req;
wire [`MIN_PER_N-1:0]                                          icache_req_valid;
wire [`MIN_PER_N-1:0]                                          icache_req_ready;
wire [`MIN_PER_N-1:0]                                          icache_flush_data;
wire [`ICACHE_NR_REQS-1:0]                                     icache_resp_valid;
wire [`ICACHE_NR_REQS-1:0]                                     icache_resp_miss;
`EVL_WIRE icache_frontend_resp [`ICACHE_NR_REQS-1:0]           icache_resp;
wire [`ICACHE_NR_REQS-1:0]                                     icache_fill_done;
`EVL_WIRE minion_satp_info [`MIN_PER_N-1:0]                    satp_info;
`EVL_WIRE minion_satp_info [`MIN_PER_N-1:0]                    matp_info;
logic [`MIN_PER_N-1:0]                                         tlb_invalidate;
`EVL_WIRE minion_ptw_req [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]    dc_ptw_req_data;
wire [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                        dc_ptw_req_valid;
wire [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                        dc_ptw_req_ready;
`EVL_WIRE minion_ptw_pte [`PTW_PER_N-1:0]                      ptw_dc_resp_data;
wire [`PTW_PER_N-1:0][`MIN_PER_PTW-1:0]                        ptw_dc_resp_valid;
logic [`MIN_PER_N-1:0][`MIN_PER_S_R]                           shire_min_id;
minion_debug_in_t [`MIN_PER_N-1:0]                             debug_minion_in;
minion_debug_out_t [`MIN_PER_N-1:0]                            debug_minion_out;
neigh_sm_dbg_monitor_t [`MIN_PER_N-1:0]                        minion_dbg_signals;
logic [`MIN_PER_N-1:0][`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]          minion_dbg_signals_mux;
minion_interrupts [`MIN_PER_N-1:0]                             interrupts_per_minion;
logic [`MIN_PER_N-1:0]                                         flb_neigh_req_valid;
logic [`MIN_PER_N-1:0][`CSR_FL_BARRIER_RANGE]                  flb_neigh_req_data;
logic [`MIN_PER_N-1:0]                                         flb_neigh_resp_valid;
logic                                                          flb_neigh_resp_data;
logic [`VA_SIZE-1:0]                                           minion_boot_addr;
esr_mprot_t                                                    mprot;
tlb_entry_type                                                 vmspagesize;
logic                                                          esr_bypass_dcache;

// Logical stubbing of the neigh_top interfaces 
////////////////////////////////////////////////////////////////////////////
logic                                   coop_tload_slv_rdy_out_valid_pwrstub  ;
logic [(`NUM_NEIGH-1)-1:0]              coop_tload_mst_done_out_valid_pwrstub ;
APB_to_neigh_t                          APB_ESR_req_pwrstub                   ;
APB_from_neigh_t                        APB_ESR_rsp_pwrstub                   ;
logic [(`SC_BANKS+1)-1:0]               neigh_sc_req_ready_pwrstub            ; 
logic [(`SC_BANKS+1)-1:0]               neigh_sc_req_valid_pwrstub            ; 
logic                                   neigh_sc_rsp_ready_pwrstub            ; 
logic                                   neigh_sc_rsp_valid_pwrstub            ; 
logic                                   esr_icache_prefetch_done_pwrstub      ;
logic                                   esr_icache_err_detected_pwrstub       ;
logic                                   esr_icache_err_logged_pwrstub         ;
logic                                   icache_f0_sram_resp_valid_pwrstub     ; 
logic                                   icache_f0_sram_resp_ready_pwrstub     ; 
esr_and_or_tree_L0_t                    esr_and_or_tree_L0_pwrstub            ;
logic                                   flb_neigh_l2_req_valid_pwrstub        ; 
logic                                   flb_l2_neigh_resp_valid_pwrstub       ; 
logic                                   icache_f2_sram_req_write_pwrstub      ; 
logic                                   icache_f2_sram_req_valid_pwrstub      ; 
logic                                   icache_f2_sram_req_ready_pwrstub      ; 
bpam_rc_tbox_ack_t                      bpam_rc_tbox_ack_hi_pwrstub           ;
//logic                                   dll_feedback_shire_pwrstub            ; 
logic                                   reset_w_icache_pwrstub                ;
logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0]  eco_o_pwrstub                         ;

// PMU
logic  [`MIN_PER_N-1:0][`PMU_MINION_COUNTERS_RANGE]                         pmu_count_up;
logic  [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`XREG_RANGE]                  pmu_read_data;
logic  [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE]   pmu_read_sel;                     
logic  [`MIN_PER_N-1:0][`PMU_TOTAL_COUNTERS_RANGE]                          pmu_write_en;
logic  [`MIN_PER_N-1:0][`XREG_RANGE]                                        pmu_write_data;
logic  [`MIN_PER_N-1:0][`PMU_NEIGH_EVENT_CNT_SEL_RANGE]                     pmu_neigh_event_sel;

// DFT low voltage
logic                                            dft__reset_byp_lv; // from neigh_channel
logic                                            dft__reset_lv;     // from neigh_channel

minion_chicken_bits_vc_t chicken_bits_vc_lo; 
minion_chicken_bits_t    min_chicken_bits;

assign min_chicken_bits.rsv_1                           = 1'b0; //reserved chicken bits
assign min_chicken_bits.rsv_0                           = 1'b0; //reserved chicken bits
assign min_chicken_bits.min_frontend_clock_gate_disable = chicken_bits_vc_lo.min_frontend_clock_gate_disable;
assign min_chicken_bits.min_dcache_clock_gate_disable   = chicken_bits_vc_lo.min_dcache_clock_gate_disable;
assign min_chicken_bits.min_vputrans_clock_gate_disable = chicken_bits_vc_lo.min_vputrans_clock_gate_disable;
assign min_chicken_bits.min_vputima_clock_gate_disable  = chicken_bits_vc_lo.min_vputima_clock_gate_disable;
assign min_chicken_bits.min_vpulane_clock_gate_disable  = chicken_bits_vc_lo.min_vpulane_clock_gate_disable;
assign min_chicken_bits.min_intpipe_clock_gate_disable  = chicken_bits_vc_lo.min_intpipe_clock_gate_disable;


wire clock_minion;
neigh_channel channel (
  // North HiV interface
  // -------------------
  // System signals
  .clock                           ( clock                            ), //HiV input 
  .reset_cold                      ( reset_cold                       ), //HiV input reset_c 
  .reset_warm                      ( reset_warm                       ), //HiV input
  .reset_w_icache                  ( reset_w_icache_pwrstub           ), //HiV output reset_w to shire Icache

  // DFT signals
  .dft__reset_byp_hv               ( dft__reset_byp_hv                ),
  .dft__reset_hv                   ( dft__reset_hv                    ),
  .dft__use_reset_cntl_hv          ( dft__use_reset_cntl_hv           ),
  .dft__reset_cntl_hv              ( dft__reset_cntl_hv               ),

  // Clock gate Ctrl override
  .chicken_bits_vc                 ( esr_clk_gate_ctrl                ),

  // Debug
  .bpam_rc_tbox_ack_hi             ( bpam_rc_tbox_ack_hi_pwrstub      ), 
    
  // ESRs
  .shire_id                        ( shire_id                         ),
  .neigh_id                        ( neigh_id                         ),
  .shire_tbox_id                   ( shire_tbox_id                    ),
  .shire_tbox_en                   ( shire_tbox_en                    ),
  .esr_thread0_enable              ( esr_thread0_enable               ),
  .esr_thread1_enable              ( esr_thread1_enable               ),
  .esr_minion_features             ( esr_minion_features              ),
  .esr_icache_prefetch_conf        ( esr_icache_prefetch_conf         ),
  .esr_icache_prefetch_start       ( esr_icache_prefetch_start        ),
  .esr_icache_prefetch_done        ( esr_icache_prefetch_done_pwrstub ),
  .esr_icache_err_detected_op      ( esr_icache_err_detected_pwrstub  ),
  .esr_icache_err_logged_op        ( esr_icache_err_logged_pwrstub    ),
  .dmctrl                          ( dmctrl                           ),
  .esr_and_or_tree_L0_op           ( esr_and_or_tree_L0_pwrstub       ),
  .esr_shire_coop_mode             ( esr_shire_coop_mode              ),
  .esr_minion_mem_override         ( esr_minion_mem_override          ),

  // L2 Request bus
  .neigh_sc_req_ready              ( neigh_sc_req_ready_pwrstub       ),
  .neigh_sc_req_valid              ( neigh_sc_req_valid_pwrstub       ),
  .neigh_sc_req_info               ( neigh_sc_req_info                ),
  // L2 Response bus
  .neigh_sc_rsp_valid              ( neigh_sc_rsp_valid_pwrstub       ),
  .neigh_sc_rsp_info               ( neigh_sc_rsp_info                ),
  .neigh_sc_rsp_ready              ( neigh_sc_rsp_ready_pwrstub       ),

  // UltraSoc Status monitor signals
  .hw_dbg_sm_monitor_enabled       ( hw_dbg_sm_monitor_enabled        ),
  .neigh_sm_gpio                   ( neigh_sm_gpio                    ),
  .neigh_sm_signals                ( neigh_sm_signals                 ),
  // APB from Shire Bus Master or Shire BPAM to read/write ESR or I$
  .APB_req_from_shire              ( APB_ESR_req_pwrstub              ),
  .APB_rsp_to_shire                ( APB_ESR_rsp_pwrstub              ),

  // Interrupts
  .int_mtip                        ( int_mtip                         ),
  .int_meip                        ( int_meip                         ),
  .int_seip                        ( int_seip                         ),
  // IPI
  .ipi_msip                        ( ipi_msip                         ),
  .ipi_redirect_trigger            ( ipi_redirect_trigger             ),
  .uc_to_neigh_fcc                 ( uc_to_neigh_fcc                  ),
  .uc_to_neigh_fcc_target          ( uc_to_neigh_fcc_target           ),
  // Fast Local Barrier request interface between neigh and L2
  .flb_neigh_l2_req_valid          ( flb_neigh_l2_req_valid_pwrstub   ),
  .flb_neigh_l2_req_data           ( flb_neigh_l2_req_data            ),
  // Fast Local Barrier response interface between neigh and L2
  .flb_l2_neigh_resp_valid         ( flb_l2_neigh_resp_valid_pwrstub  ),
  .flb_l2_neigh_resp_data          ( flb_l2_neigh_resp_data           ),
  // Request to L1 SRAM blocks
  .icache_f2_sram_req_write        ( icache_f2_sram_req_write_pwrstub ),
  .icache_f2_sram_req_addr         ( icache_f2_sram_req_addr          ),
  .icache_f2_sram_req_valid        ( icache_f2_sram_req_valid_pwrstub ),
  .icache_f2_sram_req_ready        ( icache_f2_sram_req_ready_pwrstub ),
  // Response from L1 SRAM blocks
  .icache_f0_sram_resp_dout        ( icache_f0_sram_resp_dout         ),
  .icache_f0_sram_resp_valid       ( icache_f0_sram_resp_valid_pwrstub),
  .icache_f0_sram_resp_ready       ( icache_f0_sram_resp_ready_pwrstub),

  // North LoV interface
  // -------------------
  // Cooperative TLoad slave ready output bus
  .coop_tload_slv_rdy_out_data     ( coop_tload_slv_rdy_out_data      ),
  .coop_tload_slv_rdy_out_valid    ( coop_tload_slv_rdy_out_valid_pwrstub ),
  // Cooperative TLoad slave ready input buses
  .coop_tload_slv_rdy_in_data      ( coop_tload_slv_rdy_in_data       ),
  .coop_tload_slv_rdy_in_valid     ( coop_tload_slv_rdy_in_valid      ),
  // Cooperative TLoad master done output bus
  .coop_tload_mst_done_out_coop_id ( coop_tload_mst_done_out_coop_id  ),
  .coop_tload_mst_done_out_valid   ( coop_tload_mst_done_out_valid_pwrstub ),
  // Cooperative TLoad master done input buses                        
  .coop_tload_mst_done_in_coop_id  ( coop_tload_mst_done_in_coop_id   ),
  .coop_tload_mst_done_in_valid    ( coop_tload_mst_done_in_valid     ),
  // Minion LoV interface
  // --------------------
  // Sytem signals
  .clock_minion                    ( clock_minion                     ), //LoV output clock to Minions  
  .reset_c_per_minion              ( reset_c_minion                   ), //LoV output reset_c to Minions
  .reset_d_per_minion              ( reset_d_minion                   ), //LoV output reset_d to Minions
  .reset_w_per_minion              ( reset_w_minion                   ), //LoV output reset_w to Minions
  // DFT signals
  .dft__reset_byp_lv               ( dft__reset_byp_lv                ),
  .dft__reset_lv                   ( dft__reset_lv                    ),
  // Clock gate Ctrl override
  .chicken_bits_vc_lo              ( chicken_bits_vc_lo               ),
  // ESRs
  .shire_id_lo                     ( shire_id_lo                      ),
  .shire_min_id                    ( shire_min_id                     ),
  .thread0_enable                  ( thread0_enable                   ),
  .thread1_enable                  ( thread1_enable                   ),
  .esr_minion_features_lo          ( esr_minion_features_lo           ),
  .esr_shire_coop_mode_lo          ( esr_shire_coop_mode_lo           ),
  .esr_minion_mem_override_lo      ( esr_minion_mem_override_lo       ),
    
  // Neighborhood ESRs broadcast to the Minions
  .esr_bypass_dcache               ( esr_bypass_dcache                ),
  .minion_boot_addr                ( minion_boot_addr                 ),
  .mprot                           ( mprot                            ),
  .vmspagesize                     ( vmspagesize                      ),
  // Interrupts
  .interrupts_per_minion           ( interrupts_per_minion            ),
  // Fast Local Barrier request interface with the neigh
  .flb_core_neigh_req_valid        ( flb_neigh_req_valid              ),
  .flb_core_neigh_req_data         ( flb_neigh_req_data               ),
  // Fast Local Barrier response interface with the neigh
  .flb_neigh_core_resp_valid       ( flb_neigh_resp_valid             ),
  .flb_neigh_core_resp_data        ( flb_neigh_resp_data              ),
  // Minion Dcache L2 request to evict
  .l2_dcache_evict_req_ready       ( l2_dcache_evict_req_ready        ),
  .l2_dcache_evict_req_valid       ( l2_dcache_evict_req_valid        ),
  .l2_dcache_evict_req             ( l2_dcache_evict_req              ),
  // Minion Dcache L2 request to miss
  .l2_dcache_miss_req_ready        ( l2_dcache_miss_req_ready         ),
  .l2_dcache_miss_req_valid        ( l2_dcache_miss_req_valid         ),
  .l2_dcache_miss_req              ( l2_dcache_miss_req               ),
  // Minion Dcache L2 response
  .l2_dcache_resp_ready            ( l2_dcache_resp_ready             ),
  .l2_dcache_resp_valid            ( l2_dcache_resp_valid             ),
  .l2_dcache_resp                  ( l2_dcache_resp                   ),
  // Minion Icache request
  .icache_req_ready                ( icache_req_ready                 ),
  .icache_req_valid                ( icache_req_valid                 ),
  .icache_req                      ( icache_req                       ),
  // Minion Icache response
  .icache_resp_valid               ( icache_resp_valid                ),
  .icache_resp_miss                ( icache_resp_miss                 ),
  .icache_resp                     ( icache_resp                      ),
  .icache_fill_done                ( icache_fill_done                 ),
  // ICache control
  .icache_flush_data               ( icache_flush_data                ),
  // TLB/PTW control
  .satp_info                       ( satp_info                        ),
  .matp_info                       ( matp_info                        ),
  .tlb_invalidate                  ( tlb_invalidate                   ),
  // PTW request
  .dc_ptw_req_data                 ( dc_ptw_req_data                  ),
  .dc_ptw_req_valid                ( dc_ptw_req_valid                 ),
  .dc_ptw_req_ready                ( dc_ptw_req_ready                 ),
  // PTW response
  .ptw_dc_resp_data                ( ptw_dc_resp_data                 ),
  .ptw_dc_resp_valid               ( ptw_dc_resp_valid                ),
  // APB connection with the minions
  .minion_apb_prdata               ( minion_apb_prdata                ), 
  .minion_apb_pready               ( minion_apb_pready                ),
  .minion_apb_pslverr              ( minion_apb_pslverr               ),
  .minion_apb_penable              ( minion_apb_penable               ),
  .minion_apb_psel                 ( minion_apb_psel                  ),
  .minion_apb_pwrite               ( minion_apb_pwrite                ),
  .minion_apb_paddr                ( minion_apb_paddr                 ),
  .minion_apb_pwdata               ( minion_apb_pwdata                ),
  // Minion run control signals
  .debug_minion_in                 ( debug_minion_in                  ),
  .debug_minion_out                ( debug_minion_out                 ),
  // Status Monitor signals   
  .minion_dbg_signals_en           ( minion_dbg_signals_en            ),
  .minion_dbg_signals              ( minion_dbg_signals               ),
  .minion_dbg_signals_mux          ( minion_dbg_signals_mux           ),
  // PMU interface
  .pmu_count_up                    ( pmu_count_up                     ),
  .pmu_read_data                   ( pmu_read_data                    ),
  .pmu_read_sel                    ( pmu_read_sel                     ),
  .pmu_write_en                    ( pmu_write_en                     ),
  .pmu_write_data                  ( pmu_write_data                   ),
  .pmu_neigh_event_sel             ( pmu_neigh_event_sel              ),

  // South TBOX LoV interface
  // ------------------------
  // System signals
  .clock_tbox                      ( tbox_clock                       ), //LoV output clock to Tbox
  .tbox_reset_c                    ( tbox_reset_c                     ), //LoV output reset_c to TBox
  .tbox_reset_w                    ( tbox_reset_w                     ), //LoV output reset_w to TBox
  .tbox_reset_d                    ( tbox_reset_d                     ), //LoV output reset_d to TBox
  // TBOX ESRs
  .tbox_id_lo                      ( tbox_id_lo                       ),
  .tbox_control_esr                ( tbox_control_esr                 ),
  .tbox_image_table_ptr_esr        ( tbox_image_table_ptr_esr         ),
  // TBOX neighborhood request bus
  .tbox_neigh_req_ready            ( tbox_neigh_req_ready_lo          ),
  .tbox_neigh_req_valid            ( tbox_neigh_req_valid_lo          ),
  .tbox_neigh_req_info             ( tbox_neigh_req_info_lo           ),
  // TBOX neighborhood response bus                      
  .tbox_neigh_rsp_ready            ( tbox_neigh_rsp_ready_lo          ),
  .tbox_neigh_rsp_valid            ( tbox_neigh_rsp_valid_lo          ),
  .tbox_neigh_rsp_info             ( tbox_neigh_rsp_info_lo           ),
  // TBOX monitor signals
  .tbox_dbg_signals_en             ( tbox_dbg_signals_en              ),
  .tbox_dbg_signals                ( tbox_dbg_signals                 ),
  .tbox_dbg_signals_mux            ( tbox_dbg_signals_mux             ),
  // APB from Shire Bus Master or BPAM to read/write TBOX 
  .tbox_APB_req                    ( tbox_APB_req                     ),
  .tbox_APB_rsp                    ( tbox_APB_rsp                     ),
  // Debug
  .tbox_bpam_run_control           ( tbox_bpam_run_control            ),
  .tbox_bpam_run_control_ack       ( tbox_bpam_run_control_ack        )
);


////////////////////////////////////////////////////////////////////////////////
// Generates the minions
////////////////////////////////////////////////////////////////////////////////

// et_eco_ports for neigh_top
et_eco_ports #(
   .INPUT_WIDTH(`ET_ECO_NEIGH_INPUT_WIDTH), 
   .OUTPUT_WIDTH(`ET_ECO_NEIGH_OUTPUT_WIDTH)
) eco_ports_pd_hv (
   .eco_o (eco_o_pwrstub),
   .eco_i (eco_i)
);

// Global isolate signal controlled by test
logic  pwr_ctrl_glb_isolate_int;
//assign pwr_ctrl_glb_isolate_int = dft__use_reset_cntl_hv ? 1'b0 : pwr_ctrl_glb_isolate;

neigh_hv_logic_pwr_ctrl_tdr_isolate pwr_ctrl_tdr_isolate_pd_hv (
  .dft_tdr_ctrl_in          ( dft__use_reset_cntl_hv   ),
  .pwr_ctrl_glb_isolate_in  ( pwr_ctrl_glb_isolate     ),
  .pwr_ctrl_glb_isolate_out ( pwr_ctrl_glb_isolate_int )
);

// Convert power control signals from HV to LV
logic                  pwr_ctrl_glb_nsleepin_lv;
logic                  pwr_ctrl_glb_nsleepout_lv;
logic                  pwr_ctrl_glb_isolate_lv;
logic [`MIN_PER_N-1:0] pwr_ctrl_min_nsleepin_lv;
logic [`MIN_PER_N-1:0] pwr_ctrl_min_nsleepout_lv;
logic [`MIN_PER_N-1:0] pwr_ctrl_min_isolate_lv;

vclevel_shft_h2l #(
   .WIDTH ( 2 + 2*`MIN_PER_N                                   )
) ls_h2l_pwr_ctrl_pd_hv (
  .inp_hv ( {pwr_ctrl_glb_nsleepin,pwr_ctrl_glb_isolate_int,
             pwr_ctrl_min_nsleepin,pwr_ctrl_min_isolate}       ),
  .out_lv ( {pwr_ctrl_glb_nsleepin_lv,pwr_ctrl_glb_isolate_lv,
             pwr_ctrl_min_nsleepin_lv,pwr_ctrl_min_isolate_lv} )
);
//assign pwr_ctrl_glb_nsleepin_lv =  pwr_ctrl_glb_nsleepin ;
//assign pwr_ctrl_glb_isolate_lv  =  pwr_ctrl_glb_isolate_int  ;
//assign pwr_ctrl_min_nsleepin_lv =  pwr_ctrl_min_nsleepin ;
//assign pwr_ctrl_min_isolate_lv  =  pwr_ctrl_min_isolate  ;


//vclevel_shft_l2h
//#(
//   .WIDTH ( 1 + `MIN_PER_N                                        )
//)
//ls_l2h_pwr_ctrl_pd_hv
//(
//  .inp_lv ( {pwr_ctrl_glb_nsleepout_lv,pwr_ctrl_min_nsleepout_lv} ),
//  .out_hv ( {pwr_ctrl_glb_nsleepout,pwr_ctrl_min_nsleepout}       )
//);

// Per minion power control
logic [`MIN_PER_N-1:0] minion_nsleepin;
logic [`MIN_PER_N-1:0] minion_nsleepout;
logic [`MIN_PER_N-1:0] minion_iso_enable;

logic  pwr_ctrl_glb_nsleepout_int;
assign pwr_ctrl_glb_nsleepout_int = pwr_ctrl_glb_nsleepin_lv;
assign pwr_ctrl_glb_nsleepout_lv  = pwr_ctrl_glb_nsleepout_int & ( &pwr_ctrl_min_nsleepout_lv);



neigh_top_pwrstub neigh_top_pwrstub_pd_hv (
  .pwr_ctrl_glb_isolate                       ( pwr_ctrl_glb_isolate_int             ), // input logic
    
  .coop_tload_slv_rdy_out_valid__pwrstub_in   ( coop_tload_slv_rdy_out_valid_pwrstub ), // input  logic                                        
  .coop_tload_slv_rdy_out_valid__pwrstub_out  ( coop_tload_slv_rdy_out_valid         ), // output logic                                        
  .coop_tload_mst_done_out_valid__pwrstub_in  ( coop_tload_mst_done_out_valid_pwrstub), // input  logic [(`NUM_NEIGH-1)-1:0]                   
  .coop_tload_mst_done_out_valid__pwrstub_out ( coop_tload_mst_done_out_valid        ), // output logic [(`NUM_NEIGH-1)-1:0]                   
                                                                                                                                               
  .APB_ESR_req__pwrstub_in                    ( APB_ESR_req                          ), // input  APB_to_neigh_t                               
  .APB_ESR_req__pwrstub_out                   ( APB_ESR_req_pwrstub                  ), // output APB_to_neigh_t                               
  .APB_ESR_rsp__pwrstub_in                    ( APB_ESR_rsp_pwrstub                  ), // input  APB_from_neigh_t                             
  .APB_ESR_rsp__pwrstub_out                   ( APB_ESR_rsp                          ), // output APB_from_neigh_t                             
                                                                                                          
  .neigh_sc_req_ready__pwrstub_in             ( neigh_sc_req_ready                   ), // input  logic [(`SC_BANKS+1)-1:0]                    
  .neigh_sc_req_ready__pwrstub_out            ( neigh_sc_req_ready_pwrstub           ), // output logic [(`SC_BANKS+1)-1:0]                    
  .neigh_sc_req_valid__pwrstub_out            ( neigh_sc_req_valid                   ), // output logic [(`SC_BANKS+1)-1:0]                    
  .neigh_sc_req_valid__pwrstub_in             ( neigh_sc_req_valid_pwrstub           ), // input  logic [(`SC_BANKS+1)-1:0]                    
                                                                                                                                               
  .neigh_sc_rsp_ready__pwrstub_in             ( neigh_sc_rsp_ready_pwrstub           ), // input  logic                                        
  .neigh_sc_rsp_valid__pwrstub_in             ( neigh_sc_rsp_valid                   ), // input  logic                                        
  .neigh_sc_rsp_ready__pwrstub_out            ( neigh_sc_rsp_ready                   ), // output  logic                                        
  .neigh_sc_rsp_valid__pwrstub_out            ( neigh_sc_rsp_valid_pwrstub           ), // output  logic                                        
                                                                                                                                               
  .esr_icache_prefetch_done__pwrstub_in       ( esr_icache_prefetch_done_pwrstub     ), // input  logic                                        
  .esr_icache_err_detected__pwrstub_in        ( esr_icache_err_detected_pwrstub      ), // input  logic                                        
  .esr_icache_err_logged__pwrstub_in          ( esr_icache_err_logged_pwrstub        ), // input  logic                                        
  .esr_icache_prefetch_done__pwrstub_out      ( esr_icache_prefetch_done             ), // output logic                                  
  .esr_icache_err_detected__pwrstub_out       ( esr_icache_err_detected              ), // output logic                                  
  .esr_icache_err_logged__pwrstub_out         ( esr_icache_err_logged                ), // output logic                                  
    
  .icache_f0_sram_resp_valid__pwrstub_in      ( icache_f0_sram_resp_valid            ), // input  logic                                  
  .icache_f0_sram_resp_ready__pwrstub_in      ( icache_f0_sram_resp_ready_pwrstub    ), // input  logic                                  
  .icache_f0_sram_resp_valid__pwrstub_out     ( icache_f0_sram_resp_valid_pwrstub    ), // output logic                                  
  .icache_f0_sram_resp_ready__pwrstub_out     ( icache_f0_sram_resp_ready            ), // output logic                                  
    
  .esr_and_or_tree_L0__pwrstub_in             ( esr_and_or_tree_L0_pwrstub           ), // input  esr_and_or_tree_L0_t                   
  .esr_and_or_tree_L0__pwrstub_out            ( esr_and_or_tree_L0                   ), // output esr_and_or_tree_L0_t                   
    
  .flb_neigh_l2_req_valid__pwrstub_in         ( flb_neigh_l2_req_valid_pwrstub       ), // input  logic                                              
  .flb_l2_neigh_resp_valid__pwrstub_in        ( flb_l2_neigh_resp_valid              ), // input  logic                                              
  .flb_neigh_l2_req_valid__pwrstub_out        ( flb_neigh_l2_req_valid               ), // output logic                                              
  .flb_l2_neigh_resp_valid__pwrstub_out       ( flb_l2_neigh_resp_valid_pwrstub      ), // output logic                                              
                                                                                                                
  .icache_f2_sram_req_write__pwrstub_in       ( icache_f2_sram_req_write_pwrstub     ), // input  logic                                              
  .icache_f2_sram_req_valid__pwrstub_in       ( icache_f2_sram_req_valid_pwrstub     ), // input  logic                                              
  .icache_f2_sram_req_ready__pwrstub_in       ( icache_f2_sram_req_ready             ), // input  logic                                              
  .icache_f2_sram_req_write__pwrstub_out      ( icache_f2_sram_req_write             ), // output logic                                              
  .icache_f2_sram_req_valid__pwrstub_out      ( icache_f2_sram_req_valid             ), // output logic                                  
  .icache_f2_sram_req_ready__pwrstub_out      ( icache_f2_sram_req_ready_pwrstub     ), // output logic                                  
    
  .bpam_rc_tbox_ack_hi__pwrstub_in            ( bpam_rc_tbox_ack_hi_pwrstub          ), // input  bpam_rc_tbox_ack_t                     
  .bpam_rc_tbox_ack_hi__pwrstub_out           ( bpam_rc_tbox_ack_hi                  ), // output bpam_rc_tbox_ack_t                     
    
  .reset_w_icache__pwrstub_in                 ( reset_w_icache_pwrstub               ), // input  logic                                  
  .reset_w_icache__pwrstub_out                ( reset_w_icache                       ), // output logic                                  
    
  .eco_o__pwrstub_in                          ( eco_o_pwrstub                        ), // input  logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0] 
  .eco_o__pwrstub_out                         ( eco_o                                ), // output logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0] 

  .pwr_ctrl_glb_nsleepout_in                  (pwr_ctrl_glb_nsleepout_lv             ),
  .pwr_ctrl_min_nsleepout_in                  (pwr_ctrl_min_nsleepout_lv             ), 

  .pwr_ctrl_glb_nsleepout_out                 (pwr_ctrl_glb_nsleepout                ) ,
  .pwr_ctrl_min_nsleepout_out                 (pwr_ctrl_min_nsleepout                ) 

);


for(genvar gen_it = 0; gen_it < `MIN_PER_N; gen_it++) begin : gen_minions
  localparam icacheReqIdx    = gen_it / `ICACHE_MIN_PER_REQ;
  localparam minPerPtwIdx    = gen_it % `MIN_PER_PTW;
  localparam ptwIdx          = gen_it / `MIN_PER_PTW;
    
  if((STUB_MINIONS[gen_it] == 1'b0) && (DISABLE_MINIONS[gen_it] == 1'b0)) begin : min_inst
    // thread enables
    logic [`CORE_NR_THREADS-1:0]      thread_enables;
    // set thread enables
    assign thread_enables = { thread1_enable[gen_it], thread0_enable[gen_it]};
    // pwr control
    assign minion_nsleepin[gen_it]           =  pwr_ctrl_glb_nsleepin_lv & pwr_ctrl_min_nsleepin_lv[gen_it]; // nsleepin is active '0'
    assign minion_iso_enable[gen_it]         =  pwr_ctrl_glb_isolate_lv  | pwr_ctrl_min_isolate_lv[gen_it];  // iso is active '1'
    assign pwr_ctrl_min_nsleepout_lv[gen_it] =  minion_nsleepout[gen_it];

    logic [`SHIRE_DFT_SCANOUT_MINION_WIDTH-1:0]    minion_dft__scanout_unused_ok;
    logic                                          minion_dft__occ_scanout_unused_ok;
    logic [`ET_ECO_MINION_OUTPUT_WIDTH-1:0]        minion_eco_o_unused_ok;
    TraceEncoder_signals                           minion_traceEncoder_unused_ok;

    minion_top
`ifdef ET_SIMULATION #(
         .SHIRE_ID(SHIRE_ID),
         .NEIGH_ID(NEIGH_ID),
         .MINION_ID(gen_it))
`endif
        minion (
          // System inputs
          .clock                               ( clock_minion                            ),
          .reset                               ( reset_c_minion[gen_it]                  ), 
          .reset_debug                         ( reset_d_minion[gen_it]                  ),
          .reset_non_debug                     ( reset_w_minion[gen_it]                  ),

          // DFT signals
          .dft__scanin                         ( '0                                      ), // DFT_SCAN_INSERT
          .dft__scanout                        ( minion_dft__scanout_unused_ok           ), // DFT_SCAN_INSERT
          .dft__scan_mode                      ( '0                                      ), // DFT_SCAN_INSERT
          .dft__reset_byp                      ( dft__reset_byp_lv                       ),
          .dft__scan_enable                    ( '0                                      ), // DFT_SCAN_INSERT
          .dft__test_mode                      ( '0                                      ), // DFT_SCAN_INSERT
          .dft__reset                          ( dft__reset_lv                           ),
          .dft__clock_gate_en                  ( '0                                      ), // DFT_SCAN_INSERT
          .dft__cntl                           ( '0                                      ), // DFT_SCAN_INSERT
          .dft__occ_scanin                     ( '0                                      ), // DFT_SCAN_INSERT
          .dft__occ_scanout                    ( minion_dft__occ_scanout_unused_ok       ), // DFT_SCAN_INSERT
          .dft__occ_reset                      ( '0                                      ), // DFT_SCAN_INSERT
          .dft__occ_testmode                   ( '0                                      ), // DFT_SCAN_INSERT
          .dft__scan_ate_clk                   ( '0                                      ), // DFT_SCAN_INSERT
          .dft__occ_bypass                     ( '0                                      ), // DFT_SCAN_INSERT
          .eco_i                               ( '0                                      ),
          .eco_o                               ( minion_eco_o_unused_ok                  ),
          // Is this in the IOShire
          .ioshire                             ( 1'b0                                    ),
          // Pwr Ctrl
          .nsleepin                            ( minion_nsleepin[gen_it]                 ),
          .iso_enable                          ( minion_iso_enable[gen_it]               ),
          .nsleepout                           ( minion_nsleepout[gen_it]                ),
          // Chicken bits
          .chicken_bits                        ( min_chicken_bits                        ), 
          // DCache request to L2 evict        
          .l2_dcache_evict_req_ready           ( l2_dcache_evict_req_ready[gen_it]       ),
          .l2_dcache_evict_req_valid           ( l2_dcache_evict_req_valid[gen_it]       ),
          .l2_dcache_evict_req                 ( l2_dcache_evict_req[gen_it]             ),
          // DCache request to L2 miss
          .l2_dcache_miss_req_ready            ( l2_dcache_miss_req_ready[gen_it]        ),
          .l2_dcache_miss_req_valid            ( l2_dcache_miss_req_valid[gen_it]        ),
          .l2_dcache_miss_req                  ( l2_dcache_miss_req[gen_it]              ),
          // L2 response to DCache
          .l2_dcache_resp_ready                ( l2_dcache_resp_ready[gen_it]            ),
          .l2_dcache_resp_valid                ( l2_dcache_resp_valid[gen_it]            ),
          .l2_dcache_resp                      ( l2_dcache_resp[gen_it]                  ),
          // ICache requests
          .icache_req_ready                    ( icache_req_ready[gen_it]                ),
          .icache_req_valid                    ( icache_req_valid[gen_it]                ),
          .icache_req                          ( icache_req[gen_it]                      ),
          // ICache response
          .icache_resp_valid                   ( icache_resp_valid[icacheReqIdx]         ),
          .icache_resp_miss                    ( icache_resp_miss[icacheReqIdx]          ),
          .icache_resp                         ( icache_resp[icacheReqIdx]               ),
          .icache_fill_done                    ( icache_fill_done[icacheReqIdx]          ),
          // ICache control
          .icache_flush_data                   ( icache_flush_data[gen_it]               ),
          // TLB/PTW control
          .satp_info                           ( satp_info[gen_it]                       ),
          .matp_info                           ( matp_info[gen_it]                       ),
          .tlb_invalidate                      ( tlb_invalidate[gen_it]                  ),
          // PTW request
          .dc_ptw_req_data                     ( dc_ptw_req_data[ptwIdx][minPerPtwIdx]   ),
          .dc_ptw_req_valid                    ( dc_ptw_req_valid[ptwIdx][minPerPtwIdx]  ),
          .dc_ptw_req_ready                    ( dc_ptw_req_ready[ptwIdx][minPerPtwIdx]  ),
          // PTW response
          .ptw_dc_resp_data                    ( ptw_dc_resp_data[ptwIdx]                ),
          .ptw_dc_resp_valid                   ( ptw_dc_resp_valid[ptwIdx][minPerPtwIdx] ),
          // Interrupts
          .interrupts                          ( interrupts_per_minion[gen_it]           ),
          // Thread ID
          .shire_id                            ( shire_id_lo                             ),
          .shire_min_id                        ( shire_min_id[gen_it]                    ),
          // Reset PC
          .enabled                             ( thread_enables                          ),
          .reset_vector                        ( minion_boot_addr                        ),
          // PMA control
          .mprot                               ( mprot                                   ),
          // Minimum page size ESR
          .vmspagesize                         ( vmspagesize                             ),
          // ESR configuration
          .esr_features                        ( esr_minion_features_lo                  ),
          .esr_bypass_dcache                   ( esr_bypass_dcache                       ),
          .esr_shire_coop_mode                 ( esr_shire_coop_mode_lo                  ),
          .esr_minion_mem_override             ( esr_minion_mem_override_lo              ),
          // Fast Local Barrier response interface with the neigh
          .flb_neigh_req_valid                 ( flb_neigh_req_valid[gen_it]             ),
          .flb_neigh_req_data                  ( flb_neigh_req_data[gen_it]              ),
          // Fast Local Barrier response interface with the neigh
          .flb_neigh_resp_valid                ( flb_neigh_resp_valid[gen_it]            ),
          .flb_neigh_resp_data                 ( flb_neigh_resp_data                     ),
          // Trace Encoder signals
          .te_thread_sel                       ( '0                                      ),
          .traceEncoder                        ( minion_traceEncoder_unused_ok           ),
          .te_enable                           ( 1'b0                                    ),
           // APB 
          .apb_paddr                           ( minion_apb_paddr                        ),        
          .apb_penable                         ( minion_apb_penable                      ),      
          .apb_prdata                          ( minion_apb_prdata[gen_it]               ),       
          .apb_pready                          ( minion_apb_pready[gen_it]               ),       
          .apb_psel                            ( minion_apb_psel[gen_it]                 ),         
          .apb_pslverr                         ( minion_apb_pslverr[gen_it]              ),      
          .apb_pwdata                          ( minion_apb_pwdata                       ),      
          .apb_pwrite                          ( minion_apb_pwrite                       ),
          // debug control
          .debug_in                            ( debug_minion_in[gen_it]                 ),
          .debug_out                           ( debug_minion_out[gen_it]                ),
          .minion_dbg_signals                  ( minion_dbg_signals[gen_it]              ),
          .minion_dbg_signals_mux              ( minion_dbg_signals_mux[gen_it]          ),
          .minion_dbg_sig_enable               ( minion_dbg_signals_en[gen_it]           ),
          // PMU signals
          .pmu_count_up                        ( pmu_count_up[gen_it]                    ),
          .pmu_read_data                       ( pmu_read_data[gen_it]                   ),
          .pmu_read_sel                        ( pmu_read_sel[gen_it]                    ),
          .pmu_write_en                        ( pmu_write_en[gen_it]                    ),
          .pmu_write_data                      ( pmu_write_data[gen_it]                  ),
          .pmu_neigh_event_sel                 ( pmu_neigh_event_sel[gen_it]             )
        );
        // synopsys translate_off
        ////////////////////////////////////////////////////////////////////////////
        // Log generation for RTL vs netlist comparison
        ////////////////////////////////////////////////////////////////////////////
`ifdef ENABLE_REPLAY_STIMULI
`ifndef VERILATOR    
        // Monitor signals for netlist debug
        logic                                      mon_core_wb_valid;
        logic                                      mon_core_wb_thread_id;
        logic[`PC_RANGE]                           mon_core_wb_reg_pc;

        assign mon_core_wb_valid                 = minion.core.wb_valid          ;
        assign mon_core_wb_thread_id             = minion.core.wb_thread_id      ;
        assign mon_core_wb_reg_pc                = minion.core.intpipe.wb_reg_pc[`PC_RANGE] ;
        integer                                    minion_replay_desc;
        integer                                    minion_debug_file;

        bit                                   dump_log_compare; // Dump the log compare
        string                                s;

        initial begin
          $sformat(s, "%m");
          // Detecting and only doing this for SHIRE[0], neigh[0] and minion[0]
          if ((s == "top.soc_top.SHIRE[0].shire_top.shire_top.gen_neigh[0].neigh.neigh_inst.gen_minions[0].min_inst")) begin
            if (`et_value_plusargs("dump_minion_vectors=%d", dump_log_compare)) begin
              if (dump_log_compare) begin
                $display("Opening log file for minion replay\n");
                minion_debug_file = $fopen("compare_log_0_0.txt", "w");
                minion_replay_desc = $save_stimuli_v2("compare_log_0_0_repl_minion_top.txt", minion, "traceEncoder");
              end
            end
          end
        end
       
        always_ff @ (negedge clock_minion)
        begin
          if (dump_log_compare)
          begin
            // Monitor signals for netlist debug
            $fwrite(minion_debug_file, "%x, %x, %x %x %x\n", mon_core_wb_valid, mon_core_wb_thread_id, mon_core_wb_reg_pc, l2_dcache_evict_req_valid[gen_it], l2_dcache_evict_req[gen_it]);
          end
        end

`endif
`endif
        // synopsys translate_on
    end else begin
        `ifndef EVL_SIMULATION
            assign l2_dcache_evict_req_valid[gen_it]       = 'b0;
            assign l2_dcache_miss_req_valid[gen_it]        = '0;
            assign l2_dcache_evict_req[gen_it]             = '0;
            assign l2_dcache_miss_req[gen_it]              = '0;
            assign l2_dcache_resp_ready[gen_it]            = 1'b0;
            assign icache_req_valid[gen_it]                = '0;
            assign icache_req[gen_it]                      = '0;
            assign icache_flush_data[gen_it]               = 1'b0;
            assign tlb_invalidate[gen_it]                  = 1'b0;
            assign dc_ptw_req_data[ptwIdx][minPerPtwIdx]   = '0;
            assign dc_ptw_req_valid[ptwIdx][minPerPtwIdx]  = '0;
            assign pmu_count_up[gen_it]                    = '0;
            assign pmu_read_sel[gen_it]                    = '0;
            assign pmu_write_en[gen_it]                    = 1'b0;
            assign pmu_write_data[gen_it]                  = '0;
            assign pmu_neigh_event_sel[gen_it]             = '0;
        `endif
            assign satp_info[gen_it]                       = '0;
            assign matp_info[gen_it]                       = '0;
    end // else: !if(STUB_MINIONS[gen_it] == 1'b0)

end // block: gen_minions



`ifdef EVL_SIMULATION
  evl_neigh_interface #(
    .NEIGH_ID        (NEIGH_ID),
    .DISABLE_MINIONS (DISABLE_MINIONS),
    .STUB_MINIONS    (STUB_MINIONS),
    `ifdef GFX_ENABLED
    .GFX_ENABLED     (1)
    `else
    .GFX_ENABLED     (0)
    `endif
) neigh_ifc (
    .reset_neigh (reset_w_minion[0]),
    .isolate     (pwr_ctrl_glb_isolate_int),
    .clock       (clock_minion),
    .APB_ESR_req (APB_ESR_req),
    .APB_ESR_rsp (APB_ESR_rsp)
);
`endif // EVL_SIMULATION


   // NOTE: PD indicates that the below dft _h2l and _l2h signals will get level shifters inserted by the flow so do not need hand instantiated level shifters
   // NOTE: scanout* are reversed
   `ifdef GFX_ENABLED
   assign dft__scanin_to_tbox_lv         = dft__scanin_tbox_hv;
   assign dft__scanout_tbox_hv           = dft__scanout_to_tbox_lv;
   assign dft__scan_mode_to_tbox_lv      = dft__scan_mode_hv;
   assign dft__reset_byp_to_tbox_lv      = dft__reset_byp_lv;
   assign dft__scan_enable_to_tbox_lv    = dft__scan_enable_hv;
   assign dft__test_mode_to_tbox_lv      = dft__test_mode_hv;
   assign dft__reset_to_tbox_lv          = dft__reset_lv;
   assign dft__clock_gate_en_to_tbox_lv  = dft__clock_gate_en_hv;
   assign dft__cntl_to_tbox_lv           = dft__cntl_hv;
   assign dft__occ_scanin_to_tbox_lv     = dft__occ_scanin_tbox_hv;
   assign dft__occ_scanout_tbox_hv       = dft__occ_scanout_to_tbox_lv;
   assign dft__occ_reset_to_tbox_lv      = dft__occ_reset_hv;
   assign dft__occ_testmode_to_tbox_lv   = dft__occ_testmode_hv;
   assign dft__scan_ate_clk_to_tbox_lv   = dft__scan_ate_clk_hv;
   assign dft__occ_bypass_to_tbox_lv     = dft__occ_bypass_hv;
   `endif // GFX_ENABLED


   // dft lint 
   generate
      if (`ET_DFT_INSERTION == 0) begin : no_dft_insertion
         assign dft__scanout_hv = '0;
         assign dft__occ_scanout_hv = 1'b0;
      end
   endgenerate
   wire dft_unused_ok = &{
   `ifndef ET_ASCENT_LINT
      1'b0,
   `endif
   `ifndef GFX_ENABLED
      dft__scan_mode_hv,
      dft__scan_enable_hv,
      dft__test_mode_hv,
      dft__clock_gate_en_hv,
      dft__cntl_hv,
      dft__occ_reset_hv,
      dft__occ_testmode_hv,
      dft__scan_ate_clk_hv,
      dft__occ_bypass_hv,
   `endif
      dft__scanin_hv,
      dft__occ_scanin_hv
   };


    // Voltage Monitor sense point - must follow instance naming convention vm_tie_<RAIL_NAME>,
    // where <RAIL_NAME> comes from the official VDD list, or VSS constants must be correct value 
    // for LEC, 1'b1 for all VDD, 1'b0 for all VSS
    et_combo_vm_vin_hi vm_tie_VDD_MNN ( .x(voltage_monitor_vdd) );
    et_combo_vm_vin_lo vm_tie_VSS     ( .x(voltage_monitor_vss) );

   //
   // Include the Neighborhood Event FIFO only if enabled.
   //
   `ifdef ET_USE_EVENT_FIFOS
      neigh_event_fifo #(.SHIRE_ID(SHIRE_ID), .NEIGH_ID(NEIGH_ID), .DISABLE_MINIONS(DISABLE_MINIONS), .STUB_MINIONS(STUB_MINIONS)) neigh_event_fifo(.*);
   `endif // ifdef ET_USE_EVENT_FIFO

endmodule
