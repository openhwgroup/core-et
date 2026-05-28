// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_lo_voltage_cross
  import dft_pkg::*;
  import neigh_voltage_cross_pkg::*;
#(
  parameter int unsigned FifoDepth = DefaultFifoDepth,
  parameter bit          CrossClockVoltage = 1'b1,
  parameter logic [MinPerN-1:0] StubMinions = '0
) (
  input  logic                                      rst_c_push_ni,
  input  logic                                      rst_c_pop_ni,
  input  logic                                      rst_push_ni,
  input  logic                                      rst_pop_ni,
  input  logic                                      clk_push_i,
  input  logic                                      clk_pop_i,
  input  dft_t                                      dft_hv_i,
  input  dft_t                                      dft_lv_i,

  input  logic                                      rst_w_icache_ni,
  output logic                                      rst_w_icache_sync_no,

  input  logic                                      debug_rst_push_ni,
  input  logic                                      debug_rst_pop_ni,

  input  logic                                      esr_icache_prefetch_done_i,
  output logic                                      esr_icache_prefetch_done_sync_o,
  input  logic                                      esr_icache_err_detected_i,
  output logic                                      esr_icache_err_detected_sync_o,
  input  logic                                      esr_icache_err_logged_i,
  output logic                                      esr_icache_err_logged_sync_o,

  /* verilator lint_off UNUSEDSIGNAL */  // In direct same-clock voltage-crossing mode the BPAM ack path is tied off by the original channel shell.
  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t     bpam_rc_tbox_ack_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output neigh_hv_logic_pkg::bpam_rc_tbox_ack_t     bpam_rc_tbox_ack_sync_o,

  input  minion_pkg::neigh_sm_dbg_monitor_t         neigh_sm_signals_i,
  output minion_pkg::neigh_sm_dbg_monitor_t         neigh_sm_signals_sync_o,

  input  neigh_hv_logic_pkg::apb_from_neigh_t       apb_esr_rsp_i,
  output neigh_hv_logic_pkg::apb_from_neigh_t       apb_esr_rsp_sync_o,

  input  logic                                      flb_neigh_l2_req_valid_i,
  output logic                                      flb_neigh_l2_req_valid_sync_o,
  input  logic [NvcCsrFlBarrierSize-1:0]            flb_neigh_l2_req_data_i,
  output logic [NvcCsrFlBarrierSize-1:0]            flb_neigh_l2_req_data_sync_o,

  input  logic                                      icache_f2_sram_req_write_i,
  output logic                                      icache_f2_sram_req_write_sync_o,
  input  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr_i,
  output logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr_sync_o,
  input  logic                                      icache_f2_sram_req_valid_i,
  output logic                                      icache_f2_sram_req_valid_sync_o,
  input  logic                                      icache_f2_sram_req_ready_i,
  output logic                                      icache_f2_sram_req_ready_sync_o,

  input  esr_pkg::esr_and_or_tree_l0_t              esr_and_or_tree_l0_i,
  output esr_pkg::esr_and_or_tree_l0_t              esr_and_or_tree_l0_sync_o
);

  localparam int unsigned EsrAndOrTreeL0Width = $bits(esr_pkg::esr_and_or_tree_l0_t);

  // Original path level-shifted the LV warm reset then used rst_repeat in the
  // HV/pop domain. The translated reset stays active-low through prim_rst_sync.
  prim_rst_sync u_rst_repeat_reset_w_icache_pd_hv (
    .clk_i  (clk_pop_i),
    .rst_ni (rst_w_icache_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_w_icache_sync_no)
  );

  generate
    if (CrossClockVoltage) begin : gen_cross_clock_voltage
      // I-cache SRAM request FIFO, LV push -> HV pop.
      logic icache_sram_req_vcfifo_valid;

      assign icache_f2_sram_req_valid_sync_o = icache_sram_req_vcfifo_valid;

      prim_fifo_semisync_lov #(
        .Width($bits({icache_f2_sram_req_write_i, icache_f2_sram_req_addr_i})),
        .Depth(FifoDepth)
      ) u_icache_sram_req_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(rst_push_ni),
        .push_i   (icache_f2_sram_req_valid_i),
        .wdata_i  ({icache_f2_sram_req_write_i, icache_f2_sram_req_addr_i}),
        .ready_o  (icache_f2_sram_req_ready_sync_o),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(rst_pop_ni),
        .pop_i    (icache_f2_sram_req_ready_i),
        .rdata_o  ({icache_f2_sram_req_write_sync_o, icache_f2_sram_req_addr_sync_o}),
        .valid_o  (icache_sram_req_vcfifo_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      // APB ESR response FIFO.
      logic                                apb_esr_rsp_vcfifo_valid;
      neigh_hv_logic_pkg::apb_from_neigh_t apb_esr_rsp_pop;
      logic                                apb_esr_rsp_ready_unused;
      logic                                apb_esr_rsp_vcfifo_push;

      assign apb_esr_rsp_vcfifo_push = apb_esr_rsp_i.apb_pready;

      prim_fifo_semisync_lov #(
        .Width($bits(apb_esr_rsp_i)),
        .Depth(FifoDepth)
      ) u_apb_esr_rsp_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(rst_c_push_ni),
        .push_i   (apb_esr_rsp_vcfifo_push),
        .wdata_i  (apb_esr_rsp_i),
        .ready_o  (apb_esr_rsp_ready_unused),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(rst_c_pop_ni),
        .pop_i    (apb_esr_rsp_vcfifo_valid),
        .rdata_o  (apb_esr_rsp_pop),
        .valid_o  (apb_esr_rsp_vcfifo_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      neigh_hv_logic_apb_rsp u_neigh_hv_logic_apb_rsp (
        .apb_esr_rsp_vcfifo_valid_i (apb_esr_rsp_vcfifo_valid),
        .apb_esr_rsp_pop_i          (apb_esr_rsp_pop),
        .apb_esr_rsp_sync_o         (apb_esr_rsp_sync_o)
      );

      // TBOX run-control acknowledge FIFO. STUB_MINIONS preserves the original
      // generate-time zeroing path used by simulation-only stubs.
      if (&StubMinions[MinPerN-1:0] == 1'b0) begin : gen_bpam_rc_tbox_ack_vxsing
        neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_s1_q;
        neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_ff_q;
        logic                                  bpam_rc_tbox_ack_push;
        logic                                  bpam_rc_tbox_ack_ready;
        logic                                  bpam_rc_tbox_vcfifo_valid;

        always_ff @(posedge clk_push_i) begin
          bpam_rc_tbox_ack_s1_q <= bpam_rc_tbox_ack_i;
        end

        assign bpam_rc_tbox_ack_push = debug_rst_push_ni ?
                                       (bpam_rc_tbox_ack_ff_q != bpam_rc_tbox_ack_s1_q) :
                                       1'b0;

        always_ff @(posedge clk_push_i) begin
          bpam_rc_tbox_ack_ff_q <= bpam_rc_tbox_ack_s1_q;
        end

        prim_fifo_semisync_lov #(
          .Width($bits(bpam_rc_tbox_ack_i)),
          .Depth(FifoDepth)
        ) u_bpam_rc_tbox_vcfifo_wr_lov (
          .clk_wr_i (clk_push_i),
          .rst_wr_ni(debug_rst_push_ni),
          .push_i   (bpam_rc_tbox_ack_push),
          .wdata_i  (bpam_rc_tbox_ack_s1_q),
          .ready_o  (bpam_rc_tbox_ack_ready),
          .clk_rd_i (clk_pop_i),
          .rst_rd_ni(debug_rst_pop_ni),
          .pop_i    (bpam_rc_tbox_vcfifo_valid),
          .rdata_o  (bpam_rc_tbox_ack_sync_o),
          .valid_o  (bpam_rc_tbox_vcfifo_valid),
          .dft_lv_i (dft_lv_i),
          .dft_hv_i (dft_hv_i)
        );

        logic unused_bpam_ready;
        assign unused_bpam_ready = &{1'b0, bpam_rc_tbox_ack_ready};
      end else begin : gen_bpam_rc_tbox_ack_stubbed
        assign bpam_rc_tbox_ack_sync_o = '0;
      end

      // Fast-local-barrier request FIFO.
      logic flb_neigh_l2_req_vcfifo_valid;
      logic flb_neigh_l2_req_ready_unused;

      assign flb_neigh_l2_req_valid_sync_o = flb_neigh_l2_req_vcfifo_valid;

      prim_fifo_semisync_lov #(
        .Width($bits(flb_neigh_l2_req_data_i)),
        .Depth(FifoDepth)
      ) u_flb_neigh_l2_req_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(rst_push_ni),
        .push_i   (flb_neigh_l2_req_valid_i),
        .wdata_i  (flb_neigh_l2_req_data_i),
        .ready_o  (flb_neigh_l2_req_ready_unused),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(rst_pop_ni),
        .pop_i    (flb_neigh_l2_req_vcfifo_valid),
        .rdata_o  (flb_neigh_l2_req_data_sync_o),
        .valid_o  (flb_neigh_l2_req_vcfifo_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      // Status monitor FIFO pushes whenever the write-side FIFO is ready,
      // matching the original ready-to-push self-connection.
      logic status_mon_vcfifo_valid;
      logic status_mon_push;

      prim_fifo_semisync_lov #(
        .Width($bits(neigh_sm_signals_i)),
        .Depth(FifoDepth)
      ) u_status_mon_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(debug_rst_push_ni),
        .push_i   (status_mon_push),
        .wdata_i  (neigh_sm_signals_i),
        .ready_o  (status_mon_push),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(debug_rst_pop_ni),
        .pop_i    (status_mon_vcfifo_valid),
        .rdata_o  (neigh_sm_signals_sync_o),
        .valid_o  (status_mon_vcfifo_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      // Debug status AND/OR tree FIFO.
      logic                             debug_tree_vcfifo_valid;
      logic                             debug_tree_vcfifo_push;
      logic                             debug_tree_vcfifo_ready;
      logic [EsrAndOrTreeL0Width-1:0]   esr_and_or_tree_l0_q;

      assign debug_tree_vcfifo_push = (esr_and_or_tree_l0_i != esr_and_or_tree_l0_q) ?
                                      (rst_push_ni & debug_rst_push_ni) :
                                      1'b0;

      always_ff @(posedge clk_push_i or negedge rst_push_ni or negedge debug_rst_push_ni) begin
        if (!rst_push_ni || !debug_rst_push_ni) begin
          esr_and_or_tree_l0_q <= '1;
        end else if (debug_tree_vcfifo_ready) begin
          esr_and_or_tree_l0_q <= esr_and_or_tree_l0_i;
        end
      end

      prim_fifo_semisync_lov #(
        .Width(EsrAndOrTreeL0Width),
        .Depth(FifoDepth)
      ) u_debug_tree_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(debug_rst_push_ni),
        .push_i   (debug_tree_vcfifo_push),
        .wdata_i  (esr_and_or_tree_l0_i),
        .ready_o  (debug_tree_vcfifo_ready),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(debug_rst_pop_ni),
        .pop_i    (debug_tree_vcfifo_valid),
        .rdata_o  (esr_and_or_tree_l0_sync_o),
        .valid_o  (debug_tree_vcfifo_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      // I-cache prefetch-done edge FIFO.
      logic esr_icache_prefetch_done_s1_q;
      logic esr_icache_prefetch_done_ff_q;
      logic esr_icache_prefetch_done_push;
      logic esr_icache_prefetch_done_ready;
      logic esr_icache_prefetch_done_valid;

      always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
        if (!rst_push_ni) begin
          esr_icache_prefetch_done_s1_q <= 1'b1;
          esr_icache_prefetch_done_ff_q <= 1'b0;
        end else if (esr_icache_prefetch_done_ready) begin
          esr_icache_prefetch_done_s1_q <= esr_icache_prefetch_done_i;
          esr_icache_prefetch_done_ff_q <= esr_icache_prefetch_done_s1_q;
        end
      end

      assign esr_icache_prefetch_done_push = rst_push_ni ?
                                             (esr_icache_prefetch_done_ff_q != esr_icache_prefetch_done_s1_q) :
                                             1'b0;

      prim_fifo_semisync_lov #(
        .Width($bits(esr_icache_prefetch_done_i)),
        .Depth(FifoDepth)
      ) u_icache_prefetch_done_vcfifo_wr_lov (
        .clk_wr_i (clk_push_i),
        .rst_wr_ni(rst_push_ni),
        .push_i   (esr_icache_prefetch_done_push),
        .wdata_i  (esr_icache_prefetch_done_s1_q),
        .ready_o  (esr_icache_prefetch_done_ready),
        .clk_rd_i (clk_pop_i),
        .rst_rd_ni(rst_pop_ni),
        .pop_i    (esr_icache_prefetch_done_valid),
        .rdata_o  (esr_icache_prefetch_done_sync_o),
        .valid_o  (esr_icache_prefetch_done_valid),
        .dft_lv_i (dft_lv_i),
        .dft_hv_i (dft_hv_i)
      );

      // I-cache error detected/logged pulse semisync registers followed by the
      // original HV-side edge-holding helper.
      logic esr_icache_err_detected_wr_en_d1_q;
      logic esr_icache_err_detected_sync_rd;
      logic esr_icache_err_logged_wr_en_d1_q;
      logic esr_icache_err_logged_sync_rd;

      always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
        if (!rst_push_ni) begin
          esr_icache_err_detected_wr_en_d1_q <= 1'b0;
          esr_icache_err_logged_wr_en_d1_q <= 1'b0;
        end else begin
          if (esr_icache_err_detected_wr_en_d1_q | esr_icache_err_detected_i) begin
            esr_icache_err_detected_wr_en_d1_q <= esr_icache_err_detected_i;
          end
          if (esr_icache_err_logged_wr_en_d1_q | esr_icache_err_logged_i) begin
            esr_icache_err_logged_wr_en_d1_q <= esr_icache_err_logged_i;
          end
        end
      end

      always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
        if (!rst_pop_ni) begin
          esr_icache_err_detected_sync_rd <= 1'b0;
          esr_icache_err_logged_sync_rd <= 1'b0;
        end else begin
          if (esr_icache_err_detected_sync_rd | esr_icache_err_detected_wr_en_d1_q) begin
            esr_icache_err_detected_sync_rd <= esr_icache_err_detected_wr_en_d1_q;
          end
          if (esr_icache_err_logged_sync_rd | esr_icache_err_logged_wr_en_d1_q) begin
            esr_icache_err_logged_sync_rd <= esr_icache_err_logged_wr_en_d1_q;
          end
        end
      end

      neigh_hv_logic_icache_err u_neigh_hv_logic_icache_err_detected (
        .rst_pop_ni  (rst_pop_ni),
        .clk_pop_i   (clk_pop_i),
        .ff_datain_i (esr_icache_err_detected_sync_rd),
        .ff_dataout_o(esr_icache_err_detected_sync_o)
      );

      neigh_hv_logic_icache_err u_neigh_hv_logic_icache_err_logged (
        .rst_pop_ni  (rst_pop_ni),
        .clk_pop_i   (clk_pop_i),
        .ff_datain_i (esr_icache_err_logged_sync_rd),
        .ff_dataout_o(esr_icache_err_logged_sync_o)
      );

      logic unused_ready;
      assign unused_ready = &{1'b0,
                              apb_esr_rsp_ready_unused,
                              flb_neigh_l2_req_ready_unused};

    end else begin : gen_no_cross_clock_voltage
      assign icache_f2_sram_req_write_sync_o = icache_f2_sram_req_write_i;
      assign icache_f2_sram_req_addr_sync_o  = icache_f2_sram_req_addr_i;
      assign icache_f2_sram_req_valid_sync_o = icache_f2_sram_req_valid_i;
      assign icache_f2_sram_req_ready_sync_o = icache_f2_sram_req_ready_i;
      assign apb_esr_rsp_sync_o = apb_esr_rsp_i;
      assign bpam_rc_tbox_ack_sync_o = bpam_rc_tbox_ack_i;
      assign flb_neigh_l2_req_valid_sync_o = flb_neigh_l2_req_valid_i;
      assign flb_neigh_l2_req_data_sync_o = flb_neigh_l2_req_data_i;
      assign neigh_sm_signals_sync_o = neigh_sm_signals_i;
      assign esr_and_or_tree_l0_sync_o = esr_and_or_tree_l0_i;
      assign esr_icache_prefetch_done_sync_o = esr_icache_prefetch_done_i;
      assign esr_icache_err_detected_sync_o = esr_icache_err_detected_i;
      assign esr_icache_err_logged_sync_o = esr_icache_err_logged_i;
    end
  endgenerate

endmodule : neigh_lo_voltage_cross
