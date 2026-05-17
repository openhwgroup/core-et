// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_channel_leaves_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [9:0] dmctrl_bits_i,
  output logic [9:0] dmctrl_bits_o,

  input  logic [14:0] bpam_bits_i,
  input  logic [3:0]  tbox_single_step_i,
  input  logic        rbox_single_step_i,
  output logic [43:0] bpam_neigh_bits_o,
  output logic [5:0]  bpam_rbox_bits_o,

  input  logic [7:0]  shire_id_i,
  input  logic [10:0] and_or_l2_bits_i,
  input  logic [39:0] and_or_l0_bits_i,
  output logic [10:0] and_or_l1_bits_o,
  output logic [10:0] and_or_l2_bits_o,

  input  logic [27:0] coop_slv_rdy_data_i,
  input  logic [3:0]  coop_slv_rdy_valid_i,
  input  logic [19:0] coop_mst_done_id_i,
  input  logic [11:0] coop_mst_done_valid_i,
  output logic [83:0] coop_slv_rdy_data_o,
  output logic [11:0] coop_slv_rdy_valid_o,
  output logic [59:0] coop_mst_done_id_o,
  output logic [11:0] coop_mst_done_valid_o,

  input  logic [18:0] noc_err_int_srcs_i,
  output logic [18:0] noc_all_err_int_srcs_o,
  output logic        ioshire_noc_err_int_o,

  input  logic        xll_enable_i,
  input  logic        xll_reset_i,
  input  logic        xll_run_i,
  input  logic        xll_write_i,
  input  logic [5:0]  xll_reg_first_i,
  input  logic [3:0]  xll_reg_num_i,
  input  logic [1:0]  xll_pclk_sel_i,
  input  logic [255:0] xll_wdata_i,
  input  logic [15:0] xll_prdata_i,
  input  logic        xll_pready_i,
  output logic        xll_busy_o,
  output logic        xll_rrdy_o,
  output logic [15:0] xll_rdata_o,
  output logic        xll_pclk_o,
  output logic        xll_preset_no,
  output logic [5:0]  xll_paddr_o,
  output logic        xll_psel_o,
  output logic        xll_penable_o,
  output logic        xll_pwrite_o,
  output logic [15:0] xll_pwdata_o,
  output logic [1:0]  xll_pstrb_o,

  input  logic [4:0]  dft_bits_i,
  input  logic        pll_lock_i,
  input  logic        dll_lock_i,
  input  logic [3:0]  pll_gate_disable_i,
  input  logic        pll_debug_gate_disable_i,
  input  logic [16:0] pll_auto_cfg_bits_i,
  input  logic [13:0] dll_auto_cfg_bits_i,
  input  logic [3:0]  pll_clk_sel_bits_i,
  output logic [3:0]  pll_clk_neigh_o,
  output logic        pll_clk_shire_o,
  output logic        pll_clk_debug_o,
  output logic        pll_lock_detect_o,
  output logic        dll_lock_detect_o,
  output logic        pll_busy_o,
  output logic        pll_rrdy_o,
  output logic [15:0] pll_rdata_o,
  output logic        dll_busy_o,
  output logic        dll_rrdy_o,
  output logic [15:0] dll_rdata_o,

  output logic        icache_apb_pready_o,
  output logic [63:0] icache_apb_prdata_o,
  output logic        icache_apb_pslverr_o
);

  localparam int unsigned NumNeigh = 4;
  localparam int unsigned BpamNeighWidth = $bits(neigh_voltage_cross_pkg::bpam_run_control_neigh_t);
  localparam int unsigned CoopRdyWidth = $bits(neigh_pkg::coop_tload_slv_rdy_req_t);
  localparam int unsigned CoopIdWidth = neigh_pkg::ShireCoopIdSize;
  localparam int unsigned L0Width = $bits(esr_pkg::esr_and_or_tree_l0_t);

  esr_pkg::esr_ms_dmctrl_t dmctrl_i;
  esr_pkg::esr_ms_dmctrl_t dmctrl_o;
  shire_channel_leaves_pkg::bpam_run_control_t bpam_i;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_neigh [NumNeigh-1:0];
  rbox_pkg::rc_t bpam_rbox;
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t and_or_l2_i;
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t and_or_l2_o;
  esr_pkg::esr_and_or_tree_l0_t and_or_l0 [NumNeigh-1:0];
  shire_esr_pkg::esr_and_or_tree_l1_t and_or_l1;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_slv_rdy_out_data [NumNeigh-1:0];
  neigh_pkg::coop_tload_slv_rdy_req_t coop_slv_rdy_in_data [NumNeigh-1:0][NumNeigh-2:0];
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_slv_rdy_in_valid;
  logic [NumNeigh-1:0][CoopIdWidth-1:0] coop_mst_done_out_id;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_mst_done_out_valid;
  logic [NumNeigh-1:0][NumNeigh-2:0][CoopIdWidth-1:0] coop_mst_done_in_id;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_mst_done_in_valid;
  dft_pkg::dft_t dft;
  shire_esr_pkg::esr_pll_auto_config_t pll_auto_cfg;
  shire_esr_pkg::esr_dll_auto_config_t dll_auto_cfg;
  shire_esr_pkg::esr_clkmux_ctl_t pll_clk_sel;

  logic [NumNeigh-1:0] icache_req_write;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr;
  logic [NumNeigh-1:0] icache_req_valid;
  logic [NumNeigh-1:0] icache_req_ready;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout;
  logic [NumNeigh-1:0] icache_resp_valid;
  logic [NumNeigh-1:0] icache_resp_ready;
  etlink_pkg::rsp_t neigh_sc_rsp_info [NumNeigh-1:0];
  logic [NumNeigh-1:0] neigh_sc_rsp_valid;
  logic [NumNeigh-1:0] neigh_sc_rsp_ready;
  shirecache_pkg::icache_bist_rsp_t icache_bist_rsp;
  /* verilator lint_off UNUSEDSIGNAL */  // The grouped unit wrapper exposes only APB idle checks from icache_mems.
  logic unused_icache_outputs;
  /* verilator lint_on UNUSEDSIGNAL */

  assign dmctrl_i = dmctrl_bits_i;
  assign dmctrl_bits_o = dmctrl_o;
  assign bpam_i = bpam_bits_i;
  assign and_or_l2_i = and_or_l2_bits_i;
  assign and_or_l2_bits_o = and_or_l2_o;
  assign and_or_l1_bits_o = and_or_l1;
  assign dft = dft_bits_i;
  assign pll_auto_cfg = pll_auto_cfg_bits_i;
  assign dll_auto_cfg = dll_auto_cfg_bits_i;
  assign pll_clk_sel = pll_clk_sel_bits_i;

  for (genvar n = 0; n < NumNeigh; n++) begin : gen_pack
    assign bpam_neigh_bits_o[(n*BpamNeighWidth) +: BpamNeighWidth] = bpam_neigh[n];
    assign and_or_l0[n] = and_or_l0_bits_i[(n*L0Width) +: L0Width];
    assign coop_slv_rdy_out_data[n] = coop_slv_rdy_data_i[(n*CoopRdyWidth) +: CoopRdyWidth];
    assign coop_mst_done_out_id[n] = coop_mst_done_id_i[(n*CoopIdWidth) +: CoopIdWidth];
    assign coop_mst_done_out_valid[n] = coop_mst_done_valid_i[(n*(NumNeigh-1)) +: (NumNeigh-1)];
    for (genvar m = 0; m < NumNeigh-1; m++) begin : gen_pack_other
      assign coop_slv_rdy_data_o[((n*(NumNeigh-1)+m)*CoopRdyWidth) +: CoopRdyWidth] =
          coop_slv_rdy_in_data[n][m];
      assign coop_mst_done_id_o[((n*(NumNeigh-1)+m)*CoopIdWidth) +: CoopIdWidth] =
          coop_mst_done_in_id[n][m];
    end
  end
  assign coop_slv_rdy_valid_o = coop_tload_slv_rdy_in_valid;
  assign coop_mst_done_valid_o = coop_mst_done_in_valid;
  assign bpam_rbox_bits_o = bpam_rbox;

  shire_dmctrl u_dmctrl (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .dmctrl_i  (dmctrl_i),
    .dmctrl_o  (dmctrl_o)
  );

  shire_bpam_run_control u_bpam (
    .bpam_run_control_i       (bpam_i),
    .tbox_single_step_i       (tbox_single_step_i),
    .rbox_single_step_i       (rbox_single_step_i),
    .bpam_run_control_neigh_o (bpam_neigh),
    .bpam_run_control_rbox_o  (bpam_rbox)
  );

  shire_and_or_tree_daisychain u_and_or (
    .shire_id_i                 (shire_id_i),
    .debug_and_or_tree_l2_i     (and_or_l2_i),
    .debug_and_or_tree_l2_o     (and_or_l2_o),
    .esr_and_or_tree_l0_i       (and_or_l0),
    .esr_and_or_tree_l1_local_o (and_or_l1)
  );

  shire_coop_tload_bus u_coop (
    .coop_tload_slv_rdy_out_data_i      (coop_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_i     (coop_slv_rdy_valid_i),
    .coop_tload_slv_rdy_in_data_o       (coop_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_o      (coop_tload_slv_rdy_in_valid),
    .coop_tload_mst_done_out_coop_id_i  (coop_mst_done_out_id),
    .coop_tload_mst_done_out_valid_i    (coop_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id_o   (coop_mst_done_in_id),
    .coop_tload_mst_done_in_valid_o     (coop_mst_done_in_valid)
  );

  shire_ioshire_noc_ints u_ios_noc (
    .clk_noc_i               (clk_i),
    .rst_noc_ni              (rst_ni),
    .noc_err_int_srcs_i      (noc_err_int_srcs_i),
    .noc_all_err_int_srcs_o  (noc_all_err_int_srcs_o),
    .ioshire_noc_err_int_o   (ioshire_noc_err_int_o)
  );

  shire_xll_control u_xll (
    .clk_i           (clk_i),
    .rst_ni          (rst_ni),
    .esr_enable_i    (xll_enable_i),
    .esr_reset_i     (xll_reset_i),
    .esr_run_i       (xll_run_i),
    .esr_write_i     (xll_write_i),
    .esr_reg_first_i (xll_reg_first_i),
    .esr_reg_num_i   (xll_reg_num_i),
    .esr_pclk_sel_i  (xll_pclk_sel_i),
    .esr_wdata_i     (xll_wdata_i),
    .esr_busy_o      (xll_busy_o),
    .esr_rrdy_o      (xll_rrdy_o),
    .esr_rdata_o     (xll_rdata_o),
    .pclk_o          (xll_pclk_o),
    .preset_no       (xll_preset_no),
    .paddr_o         (xll_paddr_o),
    .psel_o          (xll_psel_o),
    .penable_o       (xll_penable_o),
    .pwrite_o        (xll_pwrite_o),
    .pwdata_o        (xll_pwdata_o),
    .pstrb_o         (xll_pstrb_o),
    .prdata_i        (xll_prdata_i),
    .pready_i        (xll_pready_i)
  );

  shire_pll_wrapper u_pll (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .dft_i                    (dft),
    .clk_ref_i                (clk_i),
    .clk_step_i               (clk_i),
    .clk_pll_i                ({4{clk_i}}),
    .clk_dll_i                (clk_i),
    .clk_neigh_o              (pll_clk_neigh_o),
    .clk_shire_o              (pll_clk_shire_o),
    .clk_shire_debug_o        (pll_clk_debug_o),
    .pll_lock_i               (pll_lock_i),
    .dll_lock_i               (dll_lock_i),
    .pll_lock_detect_sys_o    (pll_lock_detect_o),
    .dll_lock_detect_sys_o    (dll_lock_detect_o),
    .clk_gate_neigh_disable_i (pll_gate_disable_i),
    .clk_gate_debug_disable_i (pll_debug_gate_disable_i),
    .pll_auto_cfg_i           (pll_auto_cfg),
    .pll_auto_busy_o          (pll_busy_o),
    .pll_auto_wdata_i         ('0),
    .pll_auto_rdata_o         (pll_rdata_o),
    .pll_auto_rrdy_o          (pll_rrdy_o),
    .dll_auto_cfg_i           (dll_auto_cfg),
    .dll_auto_busy_o          (dll_busy_o),
    .dll_auto_wdata_i         ('0),
    .dll_auto_rdata_o         (dll_rdata_o),
    .dll_auto_rrdy_o          (dll_rrdy_o),
    .clk_sel_i                (pll_clk_sel)
  );

  assign icache_req_write = '0;
  assign icache_req_addr = '0;
  assign icache_req_valid = '0;
  assign icache_resp_ready = '1;
  assign neigh_sc_rsp_info = '{default: '0};
  assign neigh_sc_rsp_valid = '0;
  assign neigh_sc_rsp_ready = '0;
  assign unused_icache_outputs = ^{icache_req_ready, icache_resp_dout, icache_resp_valid,
                                    icache_bist_rsp};

  icache_mems u_icache_mems (
    .clk_i                (clk_i),
    .rst_ni               ({NumNeigh{rst_ni}}),
    .dft_sram_clk_i       (clk_i),
    .dft_i                (dft),
    .icache_req_write_i   (icache_req_write),
    .icache_req_addr_i    (icache_req_addr),
    .icache_req_valid_i   (icache_req_valid),
    .icache_req_ready_o   (icache_req_ready),
    .icache_resp_dout_o   (icache_resp_dout),
    .icache_resp_valid_o  (icache_resp_valid),
    .icache_resp_ready_i  (icache_resp_ready),
    .neigh_sc_rsp_info_i  (neigh_sc_rsp_info),
    .neigh_sc_rsp_valid_i (neigh_sc_rsp_valid),
    .neigh_sc_rsp_ready_i (neigh_sc_rsp_ready),
    .ram_cfg_i            ('0),
    .bist_req_info_i      ('0),
    .bist_rsp_info_o      (icache_bist_rsp),
    .apb_paddr_i          ('0),
    .apb_pwrite_i         (1'b0),
    .apb_psel_i           (1'b0),
    .apb_penable_i        (1'b0),
    .apb_pwdata_i         ('0),
    .apb_pready_o         (icache_apb_pready_o),
    .apb_prdata_o         (icache_apb_prdata_o),
    .apb_pslverr_o        (icache_apb_pslverr_o)
  );

endmodule
