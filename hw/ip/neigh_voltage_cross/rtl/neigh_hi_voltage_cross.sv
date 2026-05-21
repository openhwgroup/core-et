// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hi_voltage_cross
  import dft_pkg::*;
  import neigh_voltage_cross_pkg::*;
#(
  parameter int unsigned FifoDepth = DefaultFifoDepth
) (
  input  logic                                rst_c_push_ni,
  input  logic                                rst_c_pop_ni,
  input  logic                                rst_push_ni,
  input  logic                                rst_pop_ni,
  input  logic                                clk_push_i,
  input  logic                                clk_pop_i,
  input  dft_t                                dft_hv_i,
  input  dft_t                                dft_lv_i,

  input  logic [NvcNumShireIdsBits-1:0]       shire_id_i,
  output logic [NvcNumShireIdsBits-1:0]       shire_id_sync_o,
  input  logic [NumNeighBits-1:0]             neigh_id_i,
  output logic [NumNeighBits-1:0]             neigh_id_sync_o,
  input  logic [TboxPerShireBits-1:0]         tbox_id_i,
  output logic [TboxPerShireBits-1:0]         tbox_id_sync_o,
  input  minion_pkg::esr_minion_features_t    esr_minion_features_i,
  output minion_pkg::esr_minion_features_t    esr_minion_features_sync_o,
  input  minion_chicken_bits_vc_t             chicken_bits_i,
  output minion_chicken_bits_vc_t             chicken_bits_sync_o,
  input  logic                                esr_shire_coop_mode_i,
  output logic                                esr_shire_coop_mode_sync_o,

  input  logic                                tbox_en_i,
  output logic                                tbox_en_sync_o,
  input  logic [MinPerN-1:0]                  neigh_t0_en_i,
  output logic [MinPerN-1:0]                  neigh_t0_en_sync_o,
  input  logic [MinPerN-1:0]                  neigh_t1_en_i,
  output logic [MinPerN-1:0]                  neigh_t1_en_sync_o,

  input  esr_pkg::esr_ms_dmctrl_t             dmctrl_i,
  output esr_pkg::esr_ms_dmctrl_t             dmctrl_sync_o,
  input  bpam_run_control_neigh_t             bpam_run_control_i,
  output bpam_run_control_neigh_t             bpam_run_control_sync_o,

  input  icache_pkg::icache_prefetch_conf_t   esr_icache_prefetch_conf_i,
  output icache_pkg::icache_prefetch_conf_t   esr_icache_prefetch_conf_sync_o,
  input  logic                                esr_icache_prefetch_start_i,
  output logic                                esr_icache_prefetch_start_sync_o,

  input  logic                                neigh_sc_rsp_valid_i,
  output logic                                neigh_sc_rsp_valid_sync_o,
  input  etlink_pkg::rsp_t                    neigh_sc_rsp_info_i,
  output etlink_pkg::rsp_t                    neigh_sc_rsp_info_sync_o,
  input  logic                                neigh_sc_rsp_ready_i,
  output logic                                neigh_sc_rsp_ready_sync_o,

  input  logic [NeighSmGpioWidth-1:0]         neigh_sm_gpio_i,
  output logic [NeighSmGpioWidth-1:0]         neigh_sm_gpio_sync_o,
  input  logic                                hw_dbg_sm_monitor_enabled_i,
  output logic                                hw_dbg_sm_monitor_enabled_sync_o,

  input  logic                                apb_pready_neigh_i,
  input  apb_to_neigh_t                       apb_esr_req_i,
  output apb_to_neigh_t                       apb_esr_req_sync_o,

  input  logic [UcFccTargetSize-1:0]          uc_to_neigh_fcc_target_i,
  output logic [UcFccTargetSize-1:0]          uc_to_neigh_fcc_target_sync_o,
  input  logic [MinPerN-1:0]                  uc_to_neigh_fcc_i,
  output logic [MinPerN-1:0]                  uc_to_neigh_fcc_sync_o,

  input  logic [MinPerN-1:0]                  int_mtip_i,
  input  logic                                plic_meip_i,
  input  logic                                plic_seip_i,
  output logic [MinPerN-1:0]                  int_mtip_sync_o,
  output logic                                plic_meip_sync_o,
  output logic                                plic_seip_sync_o,

  input  logic [ThreadsPerN-1:0]              esr_to_neigh_ipi_msip_i,
  output logic [ThreadsPerN-1:0]              esr_to_neigh_ipi_msip_sync_o,
  input  logic [ThreadsPerN-1:0]              esr_to_neigh_ipi_trigger_i,
  output logic [ThreadsPerN-1:0]              esr_to_neigh_ipi_trigger_sync_o,

  input  logic                                flb_l2_neigh_resp_valid_i,
  output logic                                flb_l2_neigh_resp_valid_sync_o,
  input  logic                                flb_l2_neigh_resp_data_i,
  output logic                                flb_l2_neigh_resp_data_sync_o,

  input  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout_i,
  output logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout_sync_o,
  input  logic                                icache_f0_sram_resp_valid_i,
  output logic                                icache_f0_sram_resp_valid_sync_o,
  input  logic                                icache_f0_sram_resp_ready_i,
  output logic                                icache_f0_sram_resp_ready_sync_o
);

  localparam int unsigned DmctrlWidth = $bits(esr_pkg::esr_ms_dmctrl_t);
  localparam int unsigned BpamRcWidth = $bits(bpam_run_control_neigh_t);
  localparam int unsigned StaticWidth = $bits({shire_id_i,
                                               neigh_id_i,
                                               tbox_id_i,
                                               esr_minion_features_i,
                                               chicken_bits_i,
                                               esr_shire_coop_mode_i});
  localparam int unsigned ConfWidth = $bits({tbox_en_i, neigh_t0_en_i, neigh_t1_en_i});
  localparam int unsigned DbgSmWidth = $bits({neigh_sm_gpio_i, hw_dbg_sm_monitor_enabled_i});
  localparam int unsigned InterruptWidth = $bits({int_mtip_i, plic_meip_i, plic_seip_i});

  logic ct_one_hv;

  neigh_hv_logic_constants u_neigh_hv_logic_constants (
    .ct_one_o(ct_one_hv)
  );

  // I-cache SRAM response: payload FIFO, HV push -> LV pop.
  logic icache_sram_resp_vcfifo_valid;

  assign icache_f0_sram_resp_valid_sync_o = icache_sram_resp_vcfifo_valid;

  prim_fifo_semisync_hiv #(
    .Width($bits(icache_f0_sram_resp_dout_i)),
    .Depth(FifoDepth)
  ) u_icache_sram_resp_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (icache_f0_sram_resp_valid_i),
    .wdata_i  (icache_f0_sram_resp_dout_i),
    .ready_o  (icache_f0_sram_resp_ready_sync_o),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (icache_f0_sram_resp_ready_i),
    .rdata_o  (icache_f0_sram_resp_dout_sync_o),
    .valid_o  (icache_sram_resp_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  // APB ESR request: push pulse from the original helper, LV side holds the
  // last request with psel/penable cleared after each accepted beat.
  logic          apb_esr_req_vcfifo_valid;
  apb_to_neigh_t apb_esr_req_pop;
  apb_to_neigh_t apb_esr_req_last_q;
  apb_to_neigh_t apb_esr_req_last_d;
  logic          apb_esr_req_vcfifo_push;
  logic          apb_esr_req_ready_unused;

  neigh_hv_logic_apb_req u_neigh_hv_logic_apb_req (
    .rst_push_ni                (rst_push_ni),
    .clk_push_i                 (clk_push_i),
    .apb_pready_neigh_i         (apb_pready_neigh_i),
    .apb_esr_req_apb_psel_i     (apb_esr_req_i.apb_psel),
    .apb_esr_req_apb_penable_i  (apb_esr_req_i.apb_penable),
    .apb_esr_req_vcfifo_push_o  (apb_esr_req_vcfifo_push)
  );

  prim_fifo_semisync_hiv #(
    .Width($bits(apb_esr_req_i)),
    .Depth(FifoDepth)
  ) u_apb_esr_req_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_c_push_ni),
    .push_i   (apb_esr_req_vcfifo_push),
    .wdata_i  (apb_esr_req_i),
    .ready_o  (apb_esr_req_ready_unused),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_c_pop_ni),
    .pop_i    (apb_esr_req_vcfifo_valid),
    .rdata_o  (apb_esr_req_pop),
    .valid_o  (apb_esr_req_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  always_comb begin
    apb_esr_req_last_d = apb_esr_req_last_q;
    if (apb_esr_req_vcfifo_valid) begin
      apb_esr_req_sync_o = apb_esr_req_pop;
      apb_esr_req_last_d = apb_esr_req_pop;
      apb_esr_req_last_d.apb_penable = 1'b0;
      apb_esr_req_last_d.apb_psel = 1'b0;
    end else begin
      apb_esr_req_sync_o = apb_esr_req_last_q;
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      apb_esr_req_last_q <= '0;
    end else begin
      apb_esr_req_last_q <= apb_esr_req_last_d;
    end
  end

  // Shire-cache response FIFO.
  logic neigh_sc_rsp_vcfifo_valid;

  assign neigh_sc_rsp_valid_sync_o = neigh_sc_rsp_vcfifo_valid;

  prim_fifo_semisync_hiv #(
    .Width($bits(neigh_sc_rsp_info_i)),
    .Depth(FifoDepth)
  ) u_neigh_sc_rsp_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (neigh_sc_rsp_valid_i),
    .wdata_i  (neigh_sc_rsp_info_i),
    .ready_o  (neigh_sc_rsp_ready_sync_o),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (neigh_sc_rsp_ready_i),
    .rdata_o  (neigh_sc_rsp_info_sync_o),
    .valid_o  (neigh_sc_rsp_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  // Fast-local-barrier response pulse FIFO.
  logic flb_l2_neigh_resp_vcfifo_valid;
  logic flb_l2_neigh_resp_ready_unused;

  assign flb_l2_neigh_resp_valid_sync_o = flb_l2_neigh_resp_vcfifo_valid;

  prim_fifo_semisync_hiv #(
    .Width($bits(flb_l2_neigh_resp_data_i)),
    .Depth(FifoDepth)
  ) u_flb_l2_neigh_resp_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (flb_l2_neigh_resp_valid_i),
    .wdata_i  (flb_l2_neigh_resp_data_i),
    .ready_o  (flb_l2_neigh_resp_ready_unused),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (flb_l2_neigh_resp_vcfifo_valid),
    .rdata_o  (flb_l2_neigh_resp_data_sync_o),
    .valid_o  (flb_l2_neigh_resp_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  // Level-style interrupt synchronizers: original semisync_reg_wr_hiv with
  // wr_en tied high.
  logic [InterruptWidth-1:0] interrupt_wr_data_d1_q;
  logic [InterruptWidth-1:0] interrupt_rd_data_q;
  logic                      interrupt_wr_en_d1_q;
  logic                      interrupt_rd_valid_q;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      interrupt_wr_en_d1_q <= 1'b0;
    end else if (interrupt_wr_en_d1_q | ct_one_hv) begin
      interrupt_wr_en_d1_q <= ct_one_hv;
    end
  end

  always_ff @(posedge clk_push_i) begin
    if (ct_one_hv) begin
      interrupt_wr_data_d1_q <= {int_mtip_i, plic_meip_i, plic_seip_i};
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      interrupt_rd_valid_q <= 1'b0;
    end else if (interrupt_rd_valid_q | interrupt_wr_en_d1_q) begin
      interrupt_rd_valid_q <= interrupt_wr_en_d1_q;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    if (interrupt_wr_en_d1_q) begin
      interrupt_rd_data_q <= interrupt_wr_data_d1_q;
    end
  end

  assign {int_mtip_sync_o, plic_meip_sync_o, plic_seip_sync_o} = interrupt_rd_data_q;

  // MSIP level interrupt synchronizer.
  logic [ThreadsPerN-1:0] ipi_msip_wr_data_d1_q;
  logic [ThreadsPerN-1:0] ipi_msip_rd_data_q;
  logic                   ipi_msip_wr_en_d1_q;
  logic                   ipi_msip_rd_valid_q;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      ipi_msip_wr_en_d1_q <= 1'b0;
    end else if (ipi_msip_wr_en_d1_q | ct_one_hv) begin
      ipi_msip_wr_en_d1_q <= ct_one_hv;
    end
  end

  always_ff @(posedge clk_push_i) begin
    if (ct_one_hv) begin
      ipi_msip_wr_data_d1_q <= esr_to_neigh_ipi_msip_i;
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      ipi_msip_rd_valid_q <= 1'b0;
    end else if (ipi_msip_rd_valid_q | ipi_msip_wr_en_d1_q) begin
      ipi_msip_rd_valid_q <= ipi_msip_wr_en_d1_q;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    if (ipi_msip_wr_en_d1_q) begin
      ipi_msip_rd_data_q <= ipi_msip_wr_data_d1_q;
    end
  end

  assign esr_to_neigh_ipi_msip_sync_o = ipi_msip_rd_data_q;

  // Pulse-style IPI trigger FIFO.
  logic                  ipi_trigger_vcfifo_valid;
  logic                  ipi_trigger_vcfifo_ready;
  logic [ThreadsPerN-1:0] ipi_trigger_pop_data;
  logic                  ipi_trigger_push;

  neigh_hv_logic_ipi #(
    .ThreadsPerN(ThreadsPerN)
  ) u_neigh_hv_logic_ipi (
    .esr_to_neigh_ipi_trigger_i      (esr_to_neigh_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_push_o (ipi_trigger_push)
  );

  prim_fifo_semisync_hiv #(
    .Width($bits(esr_to_neigh_ipi_trigger_i)),
    .Depth(FifoDepth)
  ) u_ipi_trigger_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (ipi_trigger_push),
    .wdata_i  (esr_to_neigh_ipi_trigger_i),
    .ready_o  (ipi_trigger_vcfifo_ready),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (ipi_trigger_vcfifo_valid),
    .rdata_o  (ipi_trigger_pop_data),
    .valid_o  (ipi_trigger_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  always_comb begin
    esr_to_neigh_ipi_trigger_sync_o = '0;
    if (ipi_trigger_vcfifo_valid) begin
      esr_to_neigh_ipi_trigger_sync_o = ipi_trigger_pop_data;
    end
  end

  // UC FCC pulse FIFO.
  logic                         neigh_uc_fcc_vcfifo_valid;
  logic                         neigh_uc_fcc_vcfifo_push;
  logic [UcFccTargetSize-1:0]   uc_to_neigh_fcc_target_pop;
  logic [MinPerN-1:0]           uc_to_neigh_fcc_pop;
  logic                         neigh_uc_fcc_ready_unused;

  neigh_hv_logic_uc_fcc #(
    .MinPerN(MinPerN)
  ) u_neigh_hv_logic_uc_fcc (
    .uc_to_neigh_fcc_i          (uc_to_neigh_fcc_i),
    .neigh_uc_fcc_vcfifo_push_o (neigh_uc_fcc_vcfifo_push)
  );

  prim_fifo_semisync_hiv #(
    .Width($bits({uc_to_neigh_fcc_i, uc_to_neigh_fcc_target_i})),
    .Depth(FifoDepth)
  ) u_neigh_us_fcc_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (neigh_uc_fcc_vcfifo_push),
    .wdata_i  ({uc_to_neigh_fcc_i, uc_to_neigh_fcc_target_i}),
    .ready_o  (neigh_uc_fcc_ready_unused),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (neigh_uc_fcc_vcfifo_valid),
    .rdata_o  ({uc_to_neigh_fcc_pop, uc_to_neigh_fcc_target_pop}),
    .valid_o  (neigh_uc_fcc_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  always_comb begin
    uc_to_neigh_fcc_sync_o = '0;
    uc_to_neigh_fcc_target_sync_o = '0;
    if (neigh_uc_fcc_vcfifo_valid) begin
      uc_to_neigh_fcc_sync_o = uc_to_neigh_fcc_pop;
      uc_to_neigh_fcc_target_sync_o = uc_to_neigh_fcc_target_pop;
    end
  end

  // Static configuration ESR synchronizer.
  logic [StaticWidth-1:0] static_wr_data_d1_q;
  logic [StaticWidth-1:0] static_rd_data_q;
  logic                   static_wr_en_d1_q;
  logic                   static_rd_valid_q;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      static_wr_en_d1_q <= 1'b0;
    end else if (static_wr_en_d1_q | ct_one_hv) begin
      static_wr_en_d1_q <= ct_one_hv;
    end
  end

  always_ff @(posedge clk_push_i) begin
    if (ct_one_hv) begin
      static_wr_data_d1_q <= {shire_id_i,
                              neigh_id_i,
                              tbox_id_i,
                              esr_minion_features_i,
                              chicken_bits_i,
                              esr_shire_coop_mode_i};
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      static_rd_valid_q <= 1'b0;
    end else if (static_rd_valid_q | static_wr_en_d1_q) begin
      static_rd_valid_q <= static_wr_en_d1_q;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    if (static_wr_en_d1_q) begin
      static_rd_data_q <= static_wr_data_d1_q;
    end
  end

  assign {shire_id_sync_o,
          neigh_id_sync_o,
          tbox_id_sync_o,
          esr_minion_features_sync_o,
          chicken_bits_sync_o,
          esr_shire_coop_mode_sync_o} = static_rd_data_q;

  // Configuration ESR synchronizer.
  logic [ConfWidth-1:0] conf_wr_data_d1_q;
  logic [ConfWidth-1:0] conf_rd_data_q;
  logic                 conf_wr_en_d1_q;
  logic                 conf_rd_valid_q;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      conf_wr_en_d1_q <= 1'b0;
    end else if (conf_wr_en_d1_q | ct_one_hv) begin
      conf_wr_en_d1_q <= ct_one_hv;
    end
  end

  always_ff @(posedge clk_push_i) begin
    if (ct_one_hv) begin
      conf_wr_data_d1_q <= {tbox_en_i, neigh_t0_en_i, neigh_t1_en_i};
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      conf_rd_valid_q <= 1'b0;
    end else if (conf_rd_valid_q | conf_wr_en_d1_q) begin
      conf_rd_valid_q <= conf_wr_en_d1_q;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    if (conf_wr_en_d1_q) begin
      conf_rd_data_q <= conf_wr_data_d1_q;
    end
  end

  assign {tbox_en_sync_o, neigh_t0_en_sync_o, neigh_t1_en_sync_o} = conf_rd_data_q;

  // Status monitor debug static synchronizer.
  logic [DbgSmWidth-1:0] dbg_sm_wr_data_d1_q;
  logic [DbgSmWidth-1:0] dbg_sm_rd_data_q;
  logic                  dbg_sm_wr_en_d1_q;
  logic                  dbg_sm_rd_valid_q;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      dbg_sm_wr_en_d1_q <= 1'b0;
    end else if (dbg_sm_wr_en_d1_q | ct_one_hv) begin
      dbg_sm_wr_en_d1_q <= ct_one_hv;
    end
  end

  always_ff @(posedge clk_push_i) begin
    if (ct_one_hv) begin
      dbg_sm_wr_data_d1_q <= {neigh_sm_gpio_i, hw_dbg_sm_monitor_enabled_i};
    end
  end

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      dbg_sm_rd_valid_q <= 1'b0;
    end else if (dbg_sm_rd_valid_q | dbg_sm_wr_en_d1_q) begin
      dbg_sm_rd_valid_q <= dbg_sm_wr_en_d1_q;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    if (dbg_sm_wr_en_d1_q) begin
      dbg_sm_rd_data_q <= dbg_sm_wr_data_d1_q;
    end
  end

  assign {neigh_sm_gpio_sync_o, hw_dbg_sm_monitor_enabled_sync_o} = dbg_sm_rd_data_q;

  // DMCTRL + BPAM run-control pulse FIFO and LV-side registered commit.
  logic [BpamRcWidth-1:0] bpam_run_control_pop;
  logic [DmctrlWidth-1:0] dmctrl_pop;
  logic [DmctrlWidth-1:0] dmctrl_pop_q;
  logic                   bpam_run_control_vcfifo_valid;
  logic                   dmctrl_we_q;
  logic                   bpam_run_control_vcfifo_ready;
  logic                   bpam_run_control_vcfifo_push;

  neigh_hv_logic_bpam #(
    .BpamRcSize(BpamRcWidth)
  ) u_neigh_hv_logic_bpam (
    .rst_push_ni                      (rst_c_push_ni),
    .clk_push_i                       (clk_push_i),
    .bpam_run_control_i               (bpam_run_control_i),
    .dmctrl_write_enable_i            (dmctrl_i.write_enable),
    .bpam_run_control_vcfifo_push_o   (bpam_run_control_vcfifo_push)
  );

  prim_fifo_semisync_hiv #(
    .Width(BpamRcWidth + DmctrlWidth),
    .Depth(FifoDepth)
  ) u_bpam_run_control_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_c_push_ni),
    .push_i   (bpam_run_control_vcfifo_push),
    .wdata_i  ({bpam_run_control_i, dmctrl_i}),
    .ready_o  (bpam_run_control_vcfifo_ready),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_c_pop_ni),
    .pop_i    (bpam_run_control_vcfifo_valid),
    .rdata_o  ({bpam_run_control_pop, dmctrl_pop}),
    .valid_o  (bpam_run_control_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  always_ff @(posedge clk_pop_i or negedge rst_c_pop_ni) begin
    if (!rst_c_pop_ni) begin
      dmctrl_pop_q <= '0;
      bpam_run_control_sync_o <= '0;
    end else if (bpam_run_control_vcfifo_valid) begin
      dmctrl_pop_q <= dmctrl_pop;
      bpam_run_control_sync_o <= bpam_run_control_pop;
    end
  end

  always_ff @(posedge clk_pop_i) begin
    dmctrl_we_q <= bpam_run_control_vcfifo_valid;
  end

  always_comb begin
    dmctrl_sync_o = dmctrl_pop_q;
    dmctrl_sync_o.write_enable = dmctrl_we_q;
  end

  // I-cache prefetch pulse FIFO.
  logic icache_prefetch_vcfifo_valid;
  logic icache_prefetch_ready_unused;

  assign esr_icache_prefetch_start_sync_o = icache_prefetch_vcfifo_valid;

  prim_fifo_semisync_hiv #(
    .Width($bits(esr_icache_prefetch_conf_i)),
    .Depth(FifoDepth)
  ) u_icache_prefetch_vcfifo_wr_hiv (
    .clk_wr_i (clk_push_i),
    .rst_wr_ni(rst_push_ni),
    .push_i   (esr_icache_prefetch_start_i),
    .wdata_i  (esr_icache_prefetch_conf_i),
    .ready_o  (icache_prefetch_ready_unused),
    .clk_rd_i (clk_pop_i),
    .rst_rd_ni(rst_pop_ni),
    .pop_i    (icache_prefetch_vcfifo_valid),
    .rdata_o  (esr_icache_prefetch_conf_sync_o),
    .valid_o  (icache_prefetch_vcfifo_valid),
    .dft_hv_i (dft_hv_i),
    .dft_lv_i (dft_lv_i)
  );

  logic unused_ready;
  assign unused_ready = &{1'b0,
                          apb_esr_req_ready_unused,
                          flb_l2_neigh_resp_ready_unused,
                          ipi_trigger_vcfifo_ready,
                          neigh_uc_fcc_ready_unused,
                          bpam_run_control_vcfifo_ready,
                          icache_prefetch_ready_unused};

  logic unused_valids;
  assign unused_valids = &{1'b0,
                           interrupt_rd_valid_q,
                           ipi_msip_rd_valid_q,
                           static_rd_valid_q,
                           conf_rd_valid_q,
                           dbg_sm_rd_valid_q};

endmodule : neigh_hi_voltage_cross
