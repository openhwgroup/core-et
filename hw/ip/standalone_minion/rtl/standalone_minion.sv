// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// standalone_minion -- single-Minion standalone shell.
//
// Mechanical translation of rtl/shire/standalone_minion/standalone_minion.v.
// The wrapper keeps the original APB mux/SPIO block, Minion, I-cache,
// neighborhood miss/evict/fill FIFOs, shared PTW, ET-Link request/response
// glue, ECO spare ports, reset-domain split, and SRAM DFT clock override.

/* verilator lint_off SYNCASYNCNET */  // The shell preserves three independently asserted original reset domains and fans their active-low translations into child async-reset ports.
module standalone_minion
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import etlink_pkg::*;
  import esr_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import neigh_pkg::*;
  import shirecache_pkg::*;
#(
  parameter int unsigned ShireId = 254,
  parameter int unsigned DftScanWidth = 12,
  parameter int unsigned StandaloneApbAddrWidth = 16,
  parameter int unsigned NumMissAgents = 4,
  parameter int unsigned MissFifoDepth = 3
) (
  input  logic                                             clk_i,
  input  logic                                             rst_ni,
  input  logic                                             rst_debug_ni,
  input  logic                                             rst_non_debug_ni,

  input  dft_t                                             dft_i,
  input  logic                                             dft_sram_clk_i,
  output logic [DftScanWidth-1:0]                          dft_scanout_o,
  output logic                                             dft_occ_scanout_o,

  input  logic [EtEcoMinionInputWidth-1:0]                 eco_i,
  output logic [EtEcoMinionOutputWidth-1:0]                eco_o,

  input  logic                                             ioshire_i,
  input  logic                                             int_mieco_i,
  input  logic                                             int_mtip_i,
  input  logic                                             int_meip_i,
  input  logic                                             int_seip_i,
  input  logic [NrThreads-1:0]                             int_msip_i,
  input  logic [NumShireIdsBits-1:0]                       shire_id_i,
  input  logic [MinPerSBits-1:0]                           shire_min_id_i,
  input  logic [NrThreads-1:0]                             thread_enable_i,
  input  logic [VaSize-1:0]                                minion_boot_addr_i,
  input  esr_mprot_t                                       mprot_i,
  input  tlb_entry_type                                    vmspagesize_i,
  input  esr_minion_features_t                             minion_features_i,
  input  logic                                             shire_coop_mode_i,
  input  logic [ShireMinionMemOverrideBits-1:0]            minion_mem_override_i,
  input  minion_debug_in_t                                 debug_in_i,
  input  logic                                             spio_rc_resumeack_i,
  input  logic                                             minion_dbg_sig_enable_i,
  output neigh_sm_dbg_monitor_t                            minion_dbg_signals_o,
  input  logic [NeighDebugSmMuxWidth-1:0]                  minion_dbg_signals_mux_i,
  input  logic                                             esr_bypass_icache_i,
  input  logic                                             esr_bypass_dcache_i,
  input  ram_cfg_t                                         ram_cfg_i,
  input  esr_icache_err_log_ctl_t                          esr_icache_err_log_ctl_i,
  output logic                                             esr_icache_err_log_sbe_o,
  output logic                                             esr_icache_err_log_dbe_o,
  output icache_err_log_info_t                             esr_icache_err_log_info_o,
  input  icache_bist_req_t                                 icache_bist_req_info_i,
  output icache_bist_rsp_t                                 icache_bist_rsp_info_o,
  input  logic                                             etl_req_ready_i,
  output logic                                             etl_req_valid_o,
  output req_t                                             etl_req_info_o,
  output logic                                             etl_rsp_ready_o,
  input  logic                                             etl_rsp_valid_i,
  input  rsp_t                                             etl_rsp_info_i,
  input  logic                                             te_enable_i,
  output trace_encoder_signals_t                           trace_encoder_o,
  output esr_spdmctrl_t                                    esr_spdmctrl_o,
  output esr_sphastatus_t                                  esr_sphastatus_o,
  input  logic [StandaloneApbAddrWidth-1:0]                apb_paddr_i,
  input  logic                                             apb_penable_i,
  output logic [BpamShireApbDataWidth-1:0]                 apb_prdata_o,
  output logic                                             apb_pready_o,
  input  logic                                             apb_psel_i,
  output logic                                             apb_pslverr_o,
  input  logic [BpamShireApbDataWidth-1:0]                 apb_pwdata_i,
  input  logic                                             apb_pwrite_i
);
  localparam int unsigned IcacheRequestors = 1;
  localparam int unsigned IcacheMinions = 1;
  localparam int unsigned PtwReqs = 2;
  localparam int unsigned DcacheMissAgent = 0;
  localparam int unsigned DcacheEvictAgent = 1;
  localparam int unsigned IcacheMissAgent = 2;
  localparam int unsigned PtwMissAgent = 3;
  localparam int unsigned MinIdToNeighIdLsb = 3;
  localparam int unsigned MinIdToNeighIdWidth = 2;

  logic rst_warm_ni;
  logic rst_minion_debug_ni;
  dft_t minion_dft;

  assign rst_warm_ni = rst_ni & rst_non_debug_ni;
  assign rst_minion_debug_ni = rst_ni & rst_debug_ni;

  always_comb begin
    // The original standalone shell forwards Minion scan/reset controls but
    // ties Minion-local DFT control (`dft__cntl`) low. Keep top-level RAM
    // inhibit/clock-override controls live for the standalone I-cache data RAM
    // wrapper only; do not introduce a new DFT RAM-inhibit path into Minion.
    minion_dft = dft_i;
    minion_dft.sram_clk_override = 1'b0;
    minion_dft.ram_rei = 1'b0;
    minion_dft.ram_wei = 1'b0;
  end

  // The current project DFT abstraction carries reset/clock/RAM controls, not
  // scan-chain plumbing. Match the original non-DFT-insertion behavior by tying
  // scan outputs idle.
  assign dft_scanout_o = '0;
  assign dft_occ_scanout_o = 1'b0;

  // Minion miss FF.
  neigh_req_t min_miss_req;
  logic       min_miss_req_valid;
  logic       min_miss_req_bid;
  logic       min_miss_req_ready;

  // Minion evict FF.
  neigh_req_t min_evict_req;
  logic       min_evict_req_valid;
  logic       min_evict_req_ready;

  logic [DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_ready;
  logic [DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid;
  et_link_minion_evict_req_info_t   l2_dcache_evict_req;

  logic [DcacheL2MissReqPorts-1:0]  l2_dcache_miss_req_ready;
  logic [DcacheL2MissReqPorts-1:0]  l2_dcache_miss_req_valid;
  et_link_minion_miss_req_info_t    l2_dcache_miss_req;

  logic                             l2_dcache_resp_ready;
  logic                             l2_dcache_resp_valid;
  et_link_minion_rsp_info_t         l2_dcache_resp;

  logic                             icache_req_ready;
  logic                             icache_req_valid;
  fe_icache_req_t                   icache_req;

  logic                             icache_resp_valid;
  logic                             icache_resp_miss;
  icache_fe_resp_t                  icache_resp;
  logic                             icache_fill_done;
  logic                             icache_flush_data;

  minion_satp_info                  satp_info;
  minion_satp_info                  matp_info;
  logic                             tlb_invalidate;

  minion_ptw_req                    dc_ptw_req_data;
  logic                             dc_ptw_req_valid;
  logic                             dc_ptw_req_ready;
  minion_ptw_pte                    ptw_dc_resp_data;
  logic                             ptw_dc_resp_valid;

  minion_ptw_req                    ic_ptw_req_data;
  logic                             ic_ptw_req_valid;
  logic                             ic_ptw_req_ready;
  logic                             ic_ptw_invalidate;
  logic                             ptw_ic_resp_valid;
  minion_ptw_pte                    ptw_ic_resp_data;

  minion_interrupts                 interrupts;
  logic                             evict_second_access_q;
  logic                             evict_second_access_d;

  logic                             miss_req_push;
  logic                             miss_req_fifo_full;
  logic                             miss_req_pop;
  neigh_req_t                       miss_req_pop_data;
  logic                             miss_req_pop_valid;
  logic                             miss_req_first_half_grant;

  rsp_t [NeighNumRespAgents-1:0]    fill_fifo_out_rsp_info;
  logic [NeighNumRespAgents-1:0]    fill_fifo_out_rsp_valid;
  logic [NeighNumRespAgents-1:0]    fill_ff_available;

  logic [EsrSpioApbPpAddrWidth-1:0] esr_apb_paddr;
  logic                             esr_apb_penable;
  logic [BpamShireApbDataWidth-1:0] esr_apb_prdata;
  logic                             esr_apb_pready;
  logic                             esr_apb_psel;
  logic                             esr_apb_pslverr;
  logic [BpamShireApbDataWidth-1:0] esr_apb_pwdata;
  logic                             esr_apb_pwrite;

  logic [IcacheNeighApbAddrWidth-1:0] icache_apb_paddr;
  logic                               icache_apb_pwrite;
  logic                               icache_apb_psel;
  logic                               icache_apb_penable;
  logic [BpamShireApbDataWidth-1:0]   icache_apb_pwdata;
  logic                               icache_apb_pready;
  logic [BpamShireApbDataWidth-1:0]   icache_apb_prdata;
  logic                               icache_apb_pslverr;

  logic [MinDmApbAddrWidth-1:0]     minion_apb_paddr;
  logic                             minion_apb_penable;
  logic [BpamShireApbDataWidth-1:0] minion_apb_prdata;
  logic                             minion_apb_pready;
  logic                             minion_apb_psel;
  logic                             minion_apb_pslverr;
  logic [BpamShireApbDataWidth-1:0] minion_apb_pwdata;
  logic                             minion_apb_pwrite;

  logic [IcacheDataApbAddrWidth-1:0] ic_data_apb_paddr;
  logic                              ic_data_apb_pwrite;
  logic                              ic_data_apb_psel;
  logic                              ic_data_apb_penable;
  logic [BpamShireApbDataWidth-1:0]  ic_data_apb_pwdata;
  logic                              ic_data_apb_pready;
  logic [BpamShireApbDataWidth-1:0]  ic_data_apb_prdata;
  logic                              ic_data_apb_pslverr;

  minion_debug_out_t debug_out;

  always_comb begin
    interrupts = '0;
    interrupts.mieco       = {{(NrThreads-1){1'b0}}, int_mieco_i};
    interrupts.mtip        = {NrThreads{int_mtip_i}};
    interrupts.msip        = int_msip_i;
    interrupts.meip        = {NrThreads{int_meip_i}};
    interrupts.seip        = {NrThreads{int_seip_i}};
  end

  sp_apb_mux #(
    .master_apb_addr  (StandaloneApbAddrWidth),
    .master_apb_data  (BpamShireApbDataWidth),
    .esr_apb_addr     (EsrSpioApbPpAddrWidth),
    .esr_apb_data     (BpamShireApbDataWidth),
    .icache_apb_addr  (IcacheNeighApbAddrWidth),
    .icache_apb_data  (BpamShireApbDataWidth),
    .minion_apb_addr  (MinDmApbAddrWidth),
    .minion_apb_data  (BpamShireApbDataWidth),
    .icache_d_apb_addr(IcacheDataApbAddrWidth),
    .icache_d_apb_data(BpamShireApbDataWidth)
  ) u_sp_apb_mux (
    .apb_paddr          (apb_paddr_i),
    .apb_penable        (apb_penable_i),
    .apb_prdata         (apb_prdata_o),
    .apb_pready         (apb_pready_o),
    .apb_psel           (apb_psel_i),
    .apb_pslverr        (apb_pslverr_o),
    .apb_pwdata         (apb_pwdata_i),
    .apb_pwrite         (apb_pwrite_i),
    .esr_apb_paddr      (esr_apb_paddr),
    .esr_apb_penable    (esr_apb_penable),
    .esr_apb_prdata     (esr_apb_prdata),
    .esr_apb_pready     (esr_apb_pready),
    .esr_apb_psel       (esr_apb_psel),
    .esr_apb_pslverr    (esr_apb_pslverr),
    .esr_apb_pwdata     (esr_apb_pwdata),
    .esr_apb_pwrite     (esr_apb_pwrite),
    .icache_apb_paddr   (icache_apb_paddr),
    .icache_apb_pwrite  (icache_apb_pwrite),
    .icache_apb_psel    (icache_apb_psel),
    .icache_apb_penable (icache_apb_penable),
    .icache_apb_pwdata  (icache_apb_pwdata),
    .icache_apb_pready  (icache_apb_pready),
    .icache_apb_prdata  (icache_apb_prdata),
    .icache_apb_pslverr (icache_apb_pslverr),
    .minion_apb_paddr   (minion_apb_paddr),
    .minion_apb_penable (minion_apb_penable),
    .minion_apb_prdata  (minion_apb_prdata),
    .minion_apb_pready  (minion_apb_pready),
    .minion_apb_psel    (minion_apb_psel),
    .minion_apb_pslverr (minion_apb_pslverr),
    .minion_apb_pwdata  (minion_apb_pwdata),
    .minion_apb_pwrite  (minion_apb_pwrite),
    .ic_data_apb_paddr  (ic_data_apb_paddr),
    .ic_data_apb_pwrite (ic_data_apb_pwrite),
    .ic_data_apb_psel   (ic_data_apb_psel),
    .ic_data_apb_penable(ic_data_apb_penable),
    .ic_data_apb_pwdata (ic_data_apb_pwdata),
    .ic_data_apb_pready (ic_data_apb_pready),
    .ic_data_apb_prdata (ic_data_apb_prdata),
    .ic_data_apb_pslverr(ic_data_apb_pslverr)
  );

  esr_spio #(
    .AdWidth(EsrSpioApbPpAddrWidth)
  ) u_debug_esrs (
    .clk_i                 (clk_i),
    .rst_c_ni              (rst_debug_ni),
    .rst_w_ni              (rst_warm_ni),
    .rst_d_ni              (rst_debug_ni),
    .test_en_i             (1'b0),
    .esr_spdmctrl_o        (esr_spdmctrl_o),
    .svcproc_dbg_status_i  (debug_out),
    .esr_sphastatus_o      (esr_sphastatus_o),
    .spio_rc_resumeack_i   (spio_rc_resumeack_i),
    .apb_paddr_i           (esr_apb_paddr),
    .apb_pwrite_i          (esr_apb_pwrite),
    .apb_psel_i            (esr_apb_psel),
    .apb_penable_i         (esr_apb_penable),
    .apb_pwdata_i          (esr_apb_pwdata),
    .apb_pready_o          (esr_apb_pready),
    .apb_prdata_o          (esr_apb_prdata),
    .apb_pslverr_o         (esr_apb_pslverr)
  );

  /* verilator lint_off PINCONNECTEMPTY */  // The standalone shell intentionally leaves original-unused minion local and PMU sideband outputs open.
  minion_top #(
    .UseMmi(0)
  ) u_minion (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_ni),
    .rst_d_ni                   (rst_minion_debug_ni),
    .rst_w_ni                   (rst_warm_ni),
    .dft_i                      (minion_dft),
    .eco_i                      ('0),
    .eco_o                      (),
    .ioshire_i                  (ioshire_i),
    .nsleepin_i                 (1'b0),
    .iso_enable_i               (1'b0),
    .nsleepout_o                (),
    .chicken_bits_i             ('1),
    .l2_dcache_evict_req_ready_i(l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid_o(l2_dcache_evict_req_valid),
    .l2_dcache_evict_req_o      (l2_dcache_evict_req),
    .l2_dcache_miss_req_ready_i (l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid_o (l2_dcache_miss_req_valid),
    .l2_dcache_miss_req_o       (l2_dcache_miss_req),
    .l2_dcache_resp_ready_o     (l2_dcache_resp_ready),
    .l2_dcache_resp_valid_i     (l2_dcache_resp_valid),
    .l2_dcache_resp_i           (l2_dcache_resp),
    .icache_req_ready_i         (icache_req_ready),
    .icache_req_valid_o         (icache_req_valid),
    .icache_req_o               (icache_req),
    .icache_resp_valid_i        (icache_resp_valid),
    .icache_resp_miss_i         (icache_resp_miss),
    .icache_resp_i              (icache_resp),
    .icache_fill_done_i         (icache_fill_done),
    .icache_flush_data_o        (icache_flush_data),
    .satp_info_o                (satp_info),
    .matp_info_o                (matp_info),
    .tlb_invalidate_o           (tlb_invalidate),
    .dc_ptw_req_data_o          (dc_ptw_req_data),
    .dc_ptw_req_valid_o         (dc_ptw_req_valid),
    .dc_ptw_req_ready_i         (dc_ptw_req_ready),
    .ptw_dc_resp_data_i         (ptw_dc_resp_data),
    .ptw_dc_resp_valid_i        (ptw_dc_resp_valid),
    .interrupts_i               (interrupts),
    .shire_id_i                 (shire_id_i),
    .shire_min_id_i             (shire_min_id_i),
    .enabled_i                  (thread_enable_i),
    .reset_vector_i             (minion_boot_addr_i),
    .mprot_i                    (mprot_i),
    .vmspagesize_i              (vmspagesize_i),
    .flb_neigh_req_valid_o      (),
    .flb_neigh_req_data_o       (),
    .flb_neigh_resp_valid_i     (1'b0),
    .flb_neigh_resp_data_i      (1'b0),
    .te_thread_sel_i            (1'b0),
    .trace_encoder_o            (trace_encoder_o),
    .te_enable_i                (te_enable_i),
    .apb_paddr_i                (minion_apb_paddr),
    .apb_penable_i              (minion_apb_penable),
    .apb_prdata_o               (minion_apb_prdata),
    .apb_pready_o               (minion_apb_pready),
    .apb_psel_i                 (minion_apb_psel),
    .apb_pslverr_o              (minion_apb_pslverr),
    .apb_pwdata_i               (minion_apb_pwdata),
    .apb_pwrite_i               (minion_apb_pwrite),
    .debug_in_i                 (debug_in_i),
    .debug_out_o                (debug_out),
    .minion_dbg_signals_o       (minion_dbg_signals_o),
    .minion_dbg_signals_mux_i   (minion_dbg_signals_mux_i),
    .minion_dbg_sig_enable_i    (minion_dbg_sig_enable_i),
    .esr_features_i             (minion_features_i),
    .esr_bypass_dcache_i        (esr_bypass_dcache_i),
    .esr_shire_coop_mode_i      (shire_coop_mode_i),
    .esr_minion_mem_override_i  (minion_mem_override_i),
    .pmu_count_up_o             (),
    .pmu_read_data_i            ('0),
    .pmu_read_sel_o             (),
    .pmu_write_en_o             (),
    .pmu_write_data_o           (),
    .pmu_neigh_event_sel_o      ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  assign min_miss_req_bid = min_miss_req_valid & ~min_evict_req_valid & ~evict_second_access_q;

  /* verilator lint_off PINCONNECTEMPTY */  // Standalone omits coop, local-message, prefetch-done, and debug sidebands that are unused in the original shell.
  neigh_miss_ff u_miss_ff (
    .clk_i                 (clk_i),
    .rst_ni                (rst_warm_ni),
    .l2_dcache_req_i       (l2_dcache_miss_req),
    .l2_dcache_req_valid_i (l2_dcache_miss_req_valid),
    .l2_dcache_req_ready_o (l2_dcache_miss_req_ready),
    .miss_req_o            (min_miss_req),
    .miss_valid_o          (min_miss_req_valid),
    .miss_ready_i          (min_miss_req_ready),
    .coop_miss_req_o       (),
    .coop_miss_valid_o     (),
    .coop_miss_ready_i     ('0),
    .dbg_sm_signals_o      ()
  );

  neigh_evict_ff u_evict_ff (
    .clk_i                 (clk_i),
    .rst_ni                (rst_warm_ni),
    .shire_id_i            (shire_id_i),
    .neigh_id_i            (shire_min_id_i[MinIdToNeighIdLsb +: MinIdToNeighIdWidth]),
    .l2_dcache_req_i       (l2_dcache_evict_req),
    .l2_dcache_req_valid_i (l2_dcache_evict_req_valid),
    .l2_dcache_req_ready_o (l2_dcache_evict_req_ready),
    .local_channel_mask_i  ('0),
    .local_dest_id_i       ('0),
    .local_message_ready_i (1'b0),
    .local_message_valid_o (),
    .local_message_info_o  (),
    .evict_req_o           (min_evict_req),
    .evict_valid_o         (min_evict_req_valid),
    .evict_ready_i         (min_evict_req_ready),
    .coop_evict_req_o      (),
    .coop_evict_valid_o    (),
    .coop_evict_ready_i    (1'b0),
    .dbg_sm_signals_o      ()
  );

  rsp_t fill_fifo_min_resp;
  logic fill_fifo_min_valid;
  logic fill_fifo_min_ready;

  assign fill_fifo_min_resp  = fill_fifo_out_rsp_info[NeighAgentMinion];
  assign fill_fifo_min_valid = fill_fifo_out_rsp_valid[NeighAgentMinion];

  neigh_fill_ff u_fill_ff (
    .clk_i                 (clk_i),
    .rst_ni                (rst_warm_ni),
    .fill_fifo_resp_i      (fill_fifo_min_resp),
    .fill_fifo_valid_i     (fill_fifo_min_valid),
    .fill_fifo_ready_o     (fill_fifo_min_ready),
    .tstore_min_resp_i     ('0),
    .tstore_min_resp_valid_i(1'b0),
    .tstore_min_resp_ready_o(),
    .local_message_info_i  ('0),
    .local_message_valid_i (1'b0),
    .local_message_ready_o (),
    .tbox_min_rsp_info_i   ('0),
    .tbox_min_rsp_valid_i  (1'b0),
    .tbox_min_rsp_ready_o  (),
    .l2_resp_o             (l2_dcache_resp),
    .l2_resp_valid_o       (l2_dcache_resp_valid),
    .l2_resp_ready_i       (l2_dcache_resp_ready),
    .tstore_min_ack_o      (),
    .dbg_sm_signals_o      ()
  );

  req_nodata_t                  icache_miss_req_info;
  logic                         icache_miss_req_valid;
  logic                         icache_miss_req_ready;
  rsp_t                         icache_miss_resp_info;
  logic                         icache_miss_resp_valid;
  logic                         icache_miss_resp_ready;
  logic                         icache_f2_sram_req_write;
  logic [IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr;
  logic                         icache_f2_sram_req_valid;
  logic                         icache_f2_sram_req_ready;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] icache_f0_sram_resp_dout;
  logic [IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout_flat;
  logic                         icache_f0_sram_resp_valid;
  logic                         icache_f0_sram_resp_ready;

  logic [IcacheRequestors-1:0]                       icache_req_ready_vec;
  logic [IcacheRequestors-1:0]                       icache_req_valid_vec;
  frontend_icache_req_t                              icache_req_vec [IcacheRequestors-1:0];
  logic [IcacheRequestors-1:0][0:0]                  icache_req_min_id_vec;
  logic [IcacheRequestors-1:0]                       icache_resp_valid_vec;
  logic [IcacheRequestors-1:0]                       icache_resp_miss_vec;
  icache_frontend_resp_t                             icache_resp_vec [IcacheRequestors-1:0];
  logic [IcacheRequestors-1:0]                       icache_fill_done_vec;
  minion_satp_info                                   icache_satp_info [IcacheMinions-1:0];
  minion_satp_info                                   icache_matp_info [IcacheMinions-1:0];
  logic [IcacheMinions-1:0]                          icache_tlb_invalidate;
  minion_ptw_req                                     icache_ptw_req_data [IcacheRequestors-1:0];
  logic [IcacheRequestors-1:0]                       icache_ptw_req_valid;
  logic [IcacheRequestors-1:0]                       icache_ptw_req_ready;
  logic [IcacheRequestors-1:0]                       icache_ptw_invalidate;
  logic [IcacheRequestors-1:0]                       icache_ptw_resp_valid;
  minion_ptw_pte                                     icache_ptw_resp_data [IcacheRequestors-1:0];

  always_comb begin
    icache_req_ready = icache_req_ready_vec[0];
    icache_req_valid_vec[0] = icache_req_valid;
    icache_req_vec[0] = icache_req;
    icache_req_min_id_vec[0] = '0;
    icache_resp_valid = icache_resp_valid_vec[0];
    icache_resp_miss = icache_resp_miss_vec[0];
    icache_resp = icache_resp_vec[0];
    icache_fill_done = icache_fill_done_vec[0];
    icache_satp_info[0] = satp_info;
    icache_matp_info[0] = matp_info;
    icache_tlb_invalidate[0] = tlb_invalidate;
    ic_ptw_req_data = icache_ptw_req_data[0];
    ic_ptw_req_valid = icache_ptw_req_valid[0];
    icache_ptw_req_ready[0] = ic_ptw_req_ready;
    ic_ptw_invalidate = icache_ptw_invalidate[0];
    icache_ptw_resp_valid[0] = ptw_ic_resp_valid;
    icache_ptw_resp_data[0] = ptw_ic_resp_data;
    icache_f0_sram_resp_dout = icache_f0_sram_resp_dout_flat;
  end

  icache_top #(
    .NrMinions    (IcacheMinions),
    .NrRequestors (IcacheRequestors)
  ) u_icache (
    .clk_i                       (clk_i),
    .rst_ni                      (rst_warm_ni),
    .ioshire_i                   (ioshire_i),
    .esr_prefetch_conf_i         ('0),
    .esr_prefetch_start_i        (1'b0),
    .esr_prefetch_done_o         (),
    .esr_err_log_ctl_i           (esr_icache_err_log_ctl_i),
    .esr_err_log_sbe_o           (esr_icache_err_log_sbe_o),
    .esr_err_log_dbe_o           (esr_icache_err_log_dbe_o),
    .esr_err_log_info_o          (esr_icache_err_log_info_o),
    .esr_mprot_i                 (mprot_i),
    .esr_vmspagesize_i           (vmspagesize_i),
    .esr_bypass_icache_i         (esr_bypass_icache_i),
    .esr_shire_coop_mode_i       (shire_coop_mode_i),
    .f0_req_ready_o              (icache_req_ready_vec),
    .f0_req_valid_i              (icache_req_valid_vec),
    .f0_req_i                    (icache_req_vec),
    .f0_req_min_id_i             (icache_req_min_id_vec),
    .f4_resp_valid_o             (icache_resp_valid_vec),
    .f4_resp_miss_o              (icache_resp_miss_vec),
    .f4_resp_o                   (icache_resp_vec),
    .f5_resp_fill_done_o         (icache_fill_done_vec),
    .f0_flush_data_i             (icache_flush_data),
    .f0_l2_miss_req_disable_next_i(evict_second_access_d),
    .f0_l2_miss_req_ready_i      (icache_miss_req_ready),
    .f0_l2_miss_req_valid_o      (icache_miss_req_valid),
    .f0_l2_miss_req_o            (icache_miss_req_info),
    .f0_l2_miss_resp_ready_o     (icache_miss_resp_ready),
    .f0_l2_miss_resp_i           (icache_miss_resp_info),
    .f0_l2_miss_resp_valid_i     (icache_miss_resp_valid),
    .satp_info_i                 (icache_satp_info),
    .matp_info_i                 (icache_matp_info),
    .tlb_invalidate_i            (icache_tlb_invalidate),
    .ptw_req_data_o              (icache_ptw_req_data),
    .ptw_req_valid_o             (icache_ptw_req_valid),
    .ptw_req_ready_i             (icache_ptw_req_ready),
    .ptw_invalidate_o            (icache_ptw_invalidate),
    .ptw_resp_valid_i            (icache_ptw_resp_valid),
    .ptw_resp_data_i             (icache_ptw_resp_data),
    .f2_sram_req_write_o         (icache_f2_sram_req_write),
    .f2_sram_req_addr_o          (icache_f2_sram_req_addr),
    .f2_sram_req_valid_o         (icache_f2_sram_req_valid),
    .f2_sram_req_ready_i         (icache_f2_sram_req_ready),
    .f0_sram_resp_dout_i         (icache_f0_sram_resp_dout),
    .f0_sram_resp_valid_i        (icache_f0_sram_resp_valid),
    .f0_sram_resp_ready_o        (icache_f0_sram_resp_ready),
    .apb_paddr_i                 (icache_apb_paddr),
    .apb_pwrite_i                (icache_apb_pwrite),
    .apb_psel_i                  (icache_apb_psel),
    .apb_penable_i               (icache_apb_penable),
    .apb_pwdata_i                (icache_apb_pwdata),
    .apb_pready_o                (icache_apb_pready),
    .apb_prdata_o                (icache_apb_prdata),
    .apb_pslverr_o               (icache_apb_pslverr),
    .dbg_sm_signals_o            ()
  );

  icache_data_ram_wrap #(
    .Id               (0),
    .StandaloneMinion (1'b1)
  ) u_icache_data_ram (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_warm_ni),
    .dft_sram_clk_i         (dft_sram_clk_i),
    .dft_i                  (dft_i),
    .icache_req_write_i     (icache_f2_sram_req_write),
    .icache_req_addr_i      (icache_f2_sram_req_addr),
    .icache_req_valid_i     (icache_f2_sram_req_valid),
    .icache_req_ready_o     (icache_f2_sram_req_ready),
    .icache_resp_dout_o     (icache_f0_sram_resp_dout_flat),
    .icache_resp_valid_o    (icache_f0_sram_resp_valid),
    .icache_resp_ready_i    (icache_f0_sram_resp_ready),
    .neigh_sc_rsp_info_i    (icache_miss_resp_info),
    .neigh_sc_rsp_valid_i   (icache_miss_resp_valid),
    .neigh_sc_rsp_ready_i   (icache_miss_resp_ready),
    .ram_cfg_i              (ram_cfg_i),
    .bist_req_info_i        (icache_bist_req_info_i),
    .bist_rsp_info_o        (icache_bist_rsp_info_o),
    .apb_paddr_i            (ic_data_apb_paddr),
    .apb_pwrite_i           (ic_data_apb_pwrite),
    .apb_psel_i             (ic_data_apb_psel),
    .apb_penable_i          (ic_data_apb_penable),
    .apb_pwdata_i           (ic_data_apb_pwdata),
    .apb_pready_o           (ic_data_apb_pready),
    .apb_prdata_o           (ic_data_apb_prdata),
    .apb_pslverr_o          (ic_data_apb_pslverr)
  );

  assign icache_miss_resp_info = fill_fifo_out_rsp_info[NeighAgentIcache];
  assign icache_miss_resp_valid = fill_fifo_out_rsp_valid[NeighAgentIcache];

  minion_ptw_req ptw_req_data [PtwReqs-1:0];
  logic [PtwReqs-1:0] ptw_req_valid;
  logic [PtwReqs-1:0] ptw_req_ready;
  logic [PtwReqs-1:0] ptw_invalidate;
  minion_ptw_pte ptw_resp_data;
  logic [PtwReqs-1:0] ptw_resp_valid;
  neigh_req_t ptw_miss_req_info;
  logic       ptw_miss_req_valid;
  logic       ptw_miss_req_ready;
  rsp_t       ptw_miss_resp_info;
  logic       ptw_miss_resp_valid;
  logic       ptw_miss_resp_ready;

  always_comb begin
    ptw_req_data[0] = dc_ptw_req_data;
    ptw_req_data[1] = ic_ptw_req_data;
    ptw_req_valid = {ic_ptw_req_valid, dc_ptw_req_valid};
    dc_ptw_req_ready = ptw_req_ready[0];
    ic_ptw_req_ready = ptw_req_ready[1];
    ptw_invalidate = {ic_ptw_invalidate, 1'b0};
    ptw_dc_resp_data = ptw_resp_data;
    ptw_ic_resp_data = ptw_resp_data;
    ptw_dc_resp_valid = ptw_resp_valid[0];
    ptw_ic_resp_valid = ptw_resp_valid[1];
  end

  neigh_shared_ptw #(
    .NReqs(PtwReqs)
  ) u_shared_ptw (
    .clk_i                      (clk_i),
    .rst_ni                     (rst_warm_ni),
    .ioshire_i                  (ioshire_i),
    .req_data_i                 (ptw_req_data),
    .req_valid_i                (ptw_req_valid),
    .req_ready_o                (ptw_req_ready),
    .invalidate_i               (ptw_invalidate),
    .mprot_i                    (mprot_i),
    .vmspagesize_i              (vmspagesize_i),
    .resp_data_o                (ptw_resp_data),
    .resp_valid_o               (ptw_resp_valid),
    .mem_miss_req_disable_next_i(evict_second_access_d),
    .mem_miss_req_info_o        (ptw_miss_req_info),
    .mem_miss_req_valid_o       (ptw_miss_req_valid),
    .mem_miss_req_ready_i       (ptw_miss_req_ready),
    .mem_fill_req_info_i        (ptw_miss_resp_info),
    .mem_fill_req_valid_i       (ptw_miss_resp_valid),
    .mem_fill_req_ready_o       (ptw_miss_resp_ready),
    .dbg_sm_signals_o           ()
  );

  assign ptw_miss_resp_info = fill_fifo_out_rsp_info[NeighAgentPtw];
  assign ptw_miss_resp_valid = fill_fifo_out_rsp_valid[NeighAgentPtw];

  always_comb begin
    evict_second_access_d = evict_second_access_q;
    if (min_evict_req_ready && (min_evict_req.size == SizeLine) && !evict_second_access_q) begin
      evict_second_access_d = 1'b1;
    end else if (min_evict_req_ready && evict_second_access_q) begin
      evict_second_access_d = 1'b0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_warm_ni) begin
    if (!rst_warm_ni) begin
      evict_second_access_q <= 1'b0;
    end else begin
      evict_second_access_q <= evict_second_access_d;
    end
  end

  logic [NumMissAgents-1:0] miss_req_arb_bid;
  logic [NumMissAgents-1:0] miss_req_arb_grant;
  logic                     min_miss_req_arb_grant;
  logic                     min_evict_req_arb_grant;
  logic                     icache_miss_req_arb_grant;
  logic                     ptw_miss_req_arb_grant;
  neigh_req_t               miss_req_info;
  logic                     miss_req_valid;
  logic                     miss_req_ready;
  logic [NumMissAgents-1:0][0:0] miss_req_arb_dummy_data;
  logic                     miss_req_arb_dummy_out;

  always_comb begin
    miss_req_arb_bid = '0;
    miss_req_arb_bid[DcacheMissAgent] = min_miss_req_bid;
    miss_req_arb_bid[DcacheEvictAgent] = min_evict_req_valid;
    miss_req_arb_bid[IcacheMissAgent] = icache_miss_req_valid;
    miss_req_arb_bid[PtwMissAgent] = ptw_miss_req_valid;
    miss_req_arb_dummy_data = '0;
  end

  prim_arb_rr #(
    .NumReqs (NumMissAgents),
    .Width   (1)
  ) u_miss_req_arbiter (
    .clk_i    (clk_i),
    .rst_ni   (rst_warm_ni),
    .reqs_i   (miss_req_arb_bid),
    .pop_i    (miss_req_ready),
    .grants_o (miss_req_arb_grant),
    .data_i   (miss_req_arb_dummy_data),
    .data_o   (miss_req_arb_dummy_out)
  );

  assign min_miss_req_arb_grant    = miss_req_arb_grant[DcacheMissAgent];
  assign min_evict_req_arb_grant   = miss_req_arb_grant[DcacheEvictAgent];
  assign icache_miss_req_arb_grant = miss_req_arb_grant[IcacheMissAgent];
  assign ptw_miss_req_arb_grant    = miss_req_arb_grant[PtwMissAgent];

  assign min_miss_req_ready    = miss_req_ready & min_miss_req_arb_grant;
  assign min_evict_req_ready   = miss_req_ready & min_evict_req_arb_grant;
  assign icache_miss_req_ready = miss_req_ready & icache_miss_req_arb_grant;
  assign ptw_miss_req_ready    = miss_req_ready & ptw_miss_req_arb_grant;

  always_comb begin
    miss_req_info = '0;

    if (min_miss_req_arb_grant) begin
      miss_req_info = min_miss_req;
      miss_req_info.source = SrcMinion0;
    end

    if (icache_miss_req_arb_grant) begin
      miss_req_info.id        = icache_miss_req_info.id;
      miss_req_info.source    = SrcIcache;
      miss_req_info.wdata     = icache_miss_req_info.wdata;
      miss_req_info.opcode    = icache_miss_req_info.opcode;
      miss_req_info.subopcode = icache_miss_req_info.subopcode;
      miss_req_info.address   = icache_miss_req_info.address;
      miss_req_info.data      = min_evict_req.data;
      miss_req_info.size      = icache_miss_req_info.size;
      miss_req_info.qwen      = icache_miss_req_info.qwen;
    end

    if (ptw_miss_req_arb_grant) begin
      miss_req_info = ptw_miss_req_info;
      miss_req_info.source = SrcPtw0;
    end

    if (min_evict_req_arb_grant) begin
      miss_req_info = min_evict_req;
      miss_req_info.source = SrcMinion0;
    end
  end

  assign miss_req_valid = |miss_req_arb_bid;
  assign miss_req_ready = ~miss_req_fifo_full;
  assign miss_req_push = miss_req_valid & ~miss_req_fifo_full;

  prim_fifo #(
    .Width      ($bits(neigh_req_t)),
    .Depth      (MissFifoDepth),
    .UseLatches (1'b0)
  ) u_miss_fifo (
    .clk_i       (clk_i),
    .rst_ni      (rst_warm_ni),
    .push_early_i(1'b0),
    .push_i      (miss_req_push),
    .push_data_i (miss_req_info),
    .full_o      (miss_req_fifo_full),
    .full_next_o (),
    .pop_i       (miss_req_pop),
    .pop_data_o  (miss_req_pop_data),
    .valid_o     (miss_req_pop_valid),
    .valid_next_o()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  assign miss_req_pop = miss_req_pop_valid && (miss_req_first_half_grant || etl_req_ready_i);

  logic                     miss_req_is_second_half_q;
  logic                     miss_req_is_second_half_d;
  logic [NeighDataSize-1:0] miss_req_first_half_q;

  always_comb begin
    miss_req_is_second_half_d = miss_req_is_second_half_q;
    miss_req_first_half_grant = 1'b0;

    if (miss_req_pop_valid && miss_req_pop_data.wdata &&
        (miss_req_pop_data.size == SizeLine) && !miss_req_is_second_half_q) begin
      miss_req_is_second_half_d = 1'b1;
      miss_req_first_half_grant = 1'b1;
    end else if (miss_req_pop && miss_req_is_second_half_q) begin
      miss_req_is_second_half_d = 1'b0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_warm_ni) begin
    if (!rst_warm_ni) begin
      miss_req_is_second_half_q <= 1'b0;
    end else begin
      miss_req_is_second_half_q <= miss_req_is_second_half_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (miss_req_first_half_grant) begin
      miss_req_first_half_q <= miss_req_pop_data.data;
    end
  end

  always_comb begin
    etl_req_info_o = '0;
    etl_req_info_o.id        = miss_req_pop_data.id;
    etl_req_info_o.source    = miss_req_pop_data.source;
    etl_req_info_o.wdata     = miss_req_pop_data.wdata;
    etl_req_info_o.opcode    = miss_req_pop_data.opcode;
    etl_req_info_o.subopcode = miss_req_pop_data.subopcode;
    etl_req_info_o.address   = miss_req_pop_data.address;
    if (miss_req_is_second_half_q) begin
      etl_req_info_o.data = {miss_req_pop_data.data, miss_req_first_half_q};
    end else begin
      etl_req_info_o.data = {miss_req_pop_data.data, miss_req_pop_data.data};
    end
    etl_req_info_o.size      = miss_req_pop_data.size;
    etl_req_info_o.qwen      = miss_req_pop_data.qwen;
  end

  assign etl_req_valid_o = miss_req_pop_valid && !miss_req_first_half_grant;

  logic fill_fifo_push;
  logic fill_fifo_full;

  assign fill_fifo_push = etl_rsp_valid_i & ~fill_fifo_full;
  assign etl_rsp_ready_o = ~fill_fifo_full;

  neigh_fill_fifo u_fill_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_warm_ni),
    .push_i        (fill_fifo_push),
    .push_data_i   (etl_rsp_info_i),
    .coop_data_i   ('0),
    .full_o        (fill_fifo_full),
    .out_rsp_ready_i(fill_ff_available),
    .out_rsp_valid_o(fill_fifo_out_rsp_valid),
    .out_rsp_info_o (fill_fifo_out_rsp_info)
  );

  always_comb begin
    fill_ff_available = '1;
    fill_ff_available[NeighAgentMinion] = fill_fifo_min_ready;
    fill_ff_available[NeighAgentIcache] = icache_miss_resp_ready;
    fill_ff_available[NeighAgentPtw] = ptw_miss_resp_ready;
  end

  prim_eco_ports #(
    .InputWidth (EtEcoMinionInputWidth),
    .OutputWidth(EtEcoMinionOutputWidth)
  ) u_eco_ports (
    .eco_i(eco_i),
    .eco_o(eco_o)
  );

  logic unused_ok;
  assign unused_ok = &{1'b0, ShireId[7:0], miss_req_arb_dummy_out, icache_miss_req_info[63:60]};

endmodule : standalone_minion
/* verilator lint_on SYNCASYNCNET */
