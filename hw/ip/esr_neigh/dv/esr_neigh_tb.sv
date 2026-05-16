// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_neigh_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic rst_c_ni,
  input  logic rst_w_ni,
  input  logic rst_d_ni,
  input  logic test_en_i,
  input  logic dft_reset_byp_i,
  input  logic dft_reset_i,

  input  logic [esr_pkg::EsrNeighApbPpAddrWidth-1:0] apb_paddr_i,
  input  logic                                       apb_pwrite_i,
  input  logic                                       apb_psel_i,
  input  logic                                       apb_penable_i,
  input  logic [63:0]                                apb_pwdata_i,
  output logic                                       apb_pready_o,
  output logic [63:0]                                apb_prdata_o,
  output logic                                       apb_pslverr_o,

  input  logic [63:0] hastatus0_i,
  input  logic [9:0]  and_or_tree_l0_i,
  input  logic [47:0] hastatus1_min_i,
  input  logic [9:0]  dmctrl_i,
  input  logic        icache_sbe_i,
  input  logic        icache_dbe_i,
  input  logic [$bits(icache_pkg::icache_err_log_info_t)-1:0] icache_err_info_i,
  input  logic [15:0] texture_status_i,

  output logic [63:0] minion_boot_o,
  output logic [63:0] mprot_o,
  output logic [63:0] vmspagesize_o,
  output logic [63:0] ipi_redirect_pc_o,
  output logic [63:0] hactrl_o,
  output logic [63:0] pmu_ctrl_o,
  output logic [63:0] neigh_chicken_o,
  output logic [63:0] icache_err_log_ctl_o,
  output logic [63:0] texture_control_o,
  output logic [63:0] texture_image_table_ptr_o,
  output logic        icache_err_detected_o,
  output logic        icache_err_logged_o,
  output logic        icache_ecc_count_ov_o
);
  dft_pkg::dft_t dft;
  esr_pkg::esr_hastatus0_t hastatus0;
  esr_pkg::esr_and_or_tree_l0_t and_or_tree_l0;
  esr_pkg::esr_hastatus1_t hastatus1_min;
  esr_pkg::esr_ms_dmctrl_t dmctrl;
  icache_pkg::icache_err_log_info_t icache_err_info;
  esr_pkg::esr_tex_status_t texture_status;

  logic [minion_pkg::VaSize-1:0] minion_boot;
  esr_pkg::esr_mprot_t mprot;
  minion_pkg::tlb_entry_type vmspagesize;
  esr_pkg::esr_ipi_pc_t ipi_redirect_pc;
  esr_pkg::esr_hactrl_t hactrl;
  esr_pkg::esr_pmu_ctrl_t pmu_ctrl;
  esr_pkg::esr_neigh_chicken_t neigh_chicken;
  icache_pkg::esr_icache_err_log_ctl_t icache_err_log_ctl;
  esr_pkg::esr_tex_ctl_t texture_control;
  logic [47:0] texture_image_table_ptr;

  assign dft.scanmode = dft_reset_byp_i;
  assign dft.scan_reset_n = ~dft_reset_i;
  assign dft.sram_clk_override = 1'b0;
  assign dft.ram_rei = 1'b0;
  assign dft.ram_wei = 1'b0;

  assign hastatus0 = hastatus0_i[$bits(esr_pkg::esr_hastatus0_t)-1:0];
  assign and_or_tree_l0 = and_or_tree_l0_i;
  assign hastatus1_min = hastatus1_min_i;
  assign dmctrl = dmctrl_i;
  assign icache_err_info = icache_err_info_i;
  assign texture_status = texture_status_i;

  esr_neigh u_dut (
    .clk_i                         (clk_i),
    .rst_c_ni                      (rst_ni & rst_c_ni),
    .rst_w_ni                      (rst_ni & rst_w_ni),
    .rst_d_ni                      (rst_ni & rst_d_ni),
    .test_en_i                     (test_en_i),
    .dft_i                         (dft),
    .esr_minion_boot_o             (minion_boot),
    .esr_mprot_o                   (mprot),
    .esr_vmspagesize_o             (vmspagesize),
    .esr_ipi_redirect_pc_o         (ipi_redirect_pc),
    .esr_hactrl_o                  (hactrl),
    .esr_hastatus0_i               (hastatus0),
    .esr_and_or_tree_l0_i          (and_or_tree_l0),
    .esr_pmu_ctrl_o                (pmu_ctrl),
    .esr_neigh_chicken_o           (neigh_chicken),
    .esr_icache_err_log_ctl_o      (icache_err_log_ctl),
    .esr_texture_control_o         (texture_control),
    .esr_texture_status_i          (texture_status),
    .esr_texture_image_table_ptr_o (texture_image_table_ptr),
    .esr_icache_err_log_sbe_i      (icache_sbe_i),
    .esr_icache_err_log_dbe_i      (icache_dbe_i),
    .esr_icache_err_log_info_i     (icache_err_info),
    .esr_icache_err_detected_o     (icache_err_detected_o),
    .esr_icache_err_logged_o       (icache_err_logged_o),
    .esr_icache_ecc_count_ov_o     (icache_ecc_count_ov_o),
    .esr_hastatus1_min_i           (hastatus1_min),
    .esr_dmctrl_i                  (dmctrl),
    .apb_paddr_i                   (apb_paddr_i),
    .apb_pwrite_i                  (apb_pwrite_i),
    .apb_psel_i                    (apb_psel_i),
    .apb_penable_i                 (apb_penable_i),
    .apb_pwdata_i                  (apb_pwdata_i),
    .apb_pready_o                  (apb_pready_o),
    .apb_prdata_o                  (apb_prdata_o),
    .apb_pslverr_o                 (apb_pslverr_o)
  );

  assign minion_boot_o = {{(64-minion_pkg::VaSize){1'b0}}, minion_boot};
  assign mprot_o = {{(64-$bits(esr_pkg::esr_mprot_t)){1'b0}}, mprot};
  assign vmspagesize_o = {{(64-$bits(minion_pkg::tlb_entry_type)){1'b0}}, vmspagesize};
  assign ipi_redirect_pc_o = {{(64-$bits(esr_pkg::esr_ipi_pc_t)){1'b0}}, ipi_redirect_pc};
  assign hactrl_o = {{(64-$bits(esr_pkg::esr_hactrl_t)){1'b0}}, hactrl};
  assign pmu_ctrl_o = {{(64-$bits(esr_pkg::esr_pmu_ctrl_t)){1'b0}}, pmu_ctrl};
  assign neigh_chicken_o = {{(64-$bits(esr_pkg::esr_neigh_chicken_t)){1'b0}}, neigh_chicken};
  assign icache_err_log_ctl_o = {{(64-$bits(icache_pkg::esr_icache_err_log_ctl_t)){1'b0}}, icache_err_log_ctl};
  assign texture_control_o = {{(64-$bits(esr_pkg::esr_tex_ctl_t)){1'b0}}, texture_control};
  assign texture_image_table_ptr_o = {16'h0, texture_image_table_ptr};
endmodule
