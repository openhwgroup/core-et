// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_esr_neigh_tb (
  input  logic clk_i,
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

  input  logic [63:0] hastatus0_i,
  input  logic [9:0]  and_or_tree_l0_i,
  input  logic [47:0] hastatus1_min_i,
  input  logic [9:0]  dmctrl_i,
  input  logic        icache_sbe_i,
  input  logic        icache_dbe_i,
  input  logic [$bits(icache_pkg::icache_err_log_info_t)-1:0] icache_err_info_i,
  input  logic [15:0] texture_status_i,

  output logic [63:0] orig_minion_boot_o,
  output logic [63:0] new_minion_boot_o,
  output logic [63:0] orig_mprot_o,
  output logic [63:0] new_mprot_o,
  output logic [63:0] orig_vmspagesize_o,
  output logic [63:0] new_vmspagesize_o,
  output logic [63:0] orig_ipi_redirect_pc_o,
  output logic [63:0] new_ipi_redirect_pc_o,
  output logic [63:0] orig_hactrl_o,
  output logic [63:0] new_hactrl_o,
  output logic [63:0] orig_pmu_ctrl_o,
  output logic [63:0] new_pmu_ctrl_o,
  output logic [63:0] orig_neigh_chicken_o,
  output logic [63:0] new_neigh_chicken_o,
  output logic [63:0] orig_icache_err_log_ctl_o,
  output logic [63:0] new_icache_err_log_ctl_o,
  output logic [63:0] orig_texture_control_o,
  output logic [63:0] new_texture_control_o,
  output logic [63:0] orig_texture_image_table_ptr_o,
  output logic [63:0] new_texture_image_table_ptr_o,
  output logic        orig_icache_err_detected_o,
  output logic        new_icache_err_detected_o,
  output logic        orig_icache_err_logged_o,
  output logic        new_icache_err_logged_o,
  output logic        orig_icache_ecc_count_ov_o,
  output logic        new_icache_ecc_count_ov_o,
  output logic        orig_apb_pready_o,
  output logic        new_apb_pready_o,
  output logic [63:0] orig_apb_prdata_o,
  output logic [63:0] new_apb_prdata_o,
  output logic        orig_apb_pslverr_o,
  output logic        new_apb_pslverr_o
);
  dft_pkg::dft_t dft;
  esr_pkg::esr_hastatus0_t new_hastatus0;
  esr_pkg::esr_and_or_tree_l0_t new_and_or_tree_l0;
  esr_pkg::esr_hastatus1_t new_hastatus1_min;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl;
  icache_pkg::icache_err_log_info_t new_icache_err_info;
  esr_pkg::esr_tex_status_t new_texture_status;

  logic [minion_pkg::VaSize-1:0] new_minion_boot;
  esr_pkg::esr_mprot_t new_mprot;
  minion_pkg::tlb_entry_type new_vmspagesize;
  esr_pkg::esr_ipi_pc_t new_ipi_redirect_pc;
  esr_pkg::esr_hactrl_t new_hactrl;
  esr_pkg::esr_pmu_ctrl_t new_pmu_ctrl;
  esr_pkg::esr_neigh_chicken_t new_neigh_chicken;
  icache_pkg::esr_icache_err_log_ctl_t new_icache_err_log_ctl;
  esr_pkg::esr_tex_ctl_t new_texture_control;
  logic [47:0] new_texture_image_table_ptr;

  logic [minion_pkg::VaSize-1:0] orig_minion_boot;
  logic [$bits(esr_pkg::esr_mprot_t)-1:0] orig_mprot;
  logic [$bits(minion_pkg::tlb_entry_type)-1:0] orig_vmspagesize;
  logic [$bits(esr_pkg::esr_ipi_pc_t)-1:0] orig_ipi_redirect_pc;
  logic [$bits(esr_pkg::esr_hactrl_t)-1:0] orig_hactrl;
  logic [$bits(esr_pkg::esr_pmu_ctrl_t)-1:0] orig_pmu_ctrl;
  logic [$bits(esr_pkg::esr_neigh_chicken_t)-1:0] orig_neigh_chicken;
  logic [$bits(icache_pkg::esr_icache_err_log_ctl_t)-1:0] orig_icache_err_log_ctl;
  logic [$bits(esr_pkg::esr_tex_ctl_t)-1:0] orig_texture_control;
  logic [47:0] orig_texture_image_table_ptr;

  assign dft.scanmode = dft_reset_byp_i;
  assign dft.scan_reset_n = ~dft_reset_i;
  assign dft.sram_clk_override = 1'b0;
  assign dft.ram_rei = 1'b0;
  assign dft.ram_wei = 1'b0;

  assign new_hastatus0 = hastatus0_i[$bits(esr_pkg::esr_hastatus0_t)-1:0];
  assign new_and_or_tree_l0 = and_or_tree_l0_i;
  assign new_hastatus1_min = hastatus1_min_i;
  assign new_dmctrl = dmctrl_i;
  assign new_icache_err_info = icache_err_info_i;
  assign new_texture_status = texture_status_i;

  esr_neigh #(
    .AdWidth (esr_pkg::EsrNeighApbPpAddrWidth)
  ) u_new (
    .clk_i                         (clk_i),
    .rst_c_ni                      (rst_c_ni),
    .rst_w_ni                      (rst_w_ni),
    .rst_d_ni                      (rst_d_ni),
    .test_en_i                     (test_en_i),
    .dft_i                         (dft),
    .esr_minion_boot_o             (new_minion_boot),
    .esr_mprot_o                   (new_mprot),
    .esr_vmspagesize_o             (new_vmspagesize),
    .esr_ipi_redirect_pc_o         (new_ipi_redirect_pc),
    .esr_hactrl_o                  (new_hactrl),
    .esr_hastatus0_i               (new_hastatus0),
    .esr_and_or_tree_l0_i          (new_and_or_tree_l0),
    .esr_pmu_ctrl_o                (new_pmu_ctrl),
    .esr_neigh_chicken_o           (new_neigh_chicken),
    .esr_icache_err_log_ctl_o      (new_icache_err_log_ctl),
    .esr_texture_control_o         (new_texture_control),
    .esr_texture_status_i          (new_texture_status),
    .esr_texture_image_table_ptr_o (new_texture_image_table_ptr),
    .esr_icache_err_log_sbe_i      (icache_sbe_i),
    .esr_icache_err_log_dbe_i      (icache_dbe_i),
    .esr_icache_err_log_info_i     (new_icache_err_info),
    .esr_icache_err_detected_o     (new_icache_err_detected_o),
    .esr_icache_err_logged_o       (new_icache_err_logged_o),
    .esr_icache_ecc_count_ov_o     (new_icache_ecc_count_ov_o),
    .esr_hastatus1_min_i           (new_hastatus1_min),
    .esr_dmctrl_i                  (new_dmctrl),
    .apb_paddr_i                   (apb_paddr_i),
    .apb_pwrite_i                  (apb_pwrite_i),
    .apb_psel_i                    (apb_psel_i),
    .apb_penable_i                 (apb_penable_i),
    .apb_pwdata_i                  (apb_pwdata_i),
    .apb_pready_o                  (new_apb_pready_o),
    .apb_prdata_o                  (new_apb_prdata_o),
    .apb_pslverr_o                 (new_apb_pslverr_o)
  );

  esr_neigh_orig #(
    .AD_WIDTH (esr_pkg::EsrNeighApbPpAddrWidth)
  ) u_orig (
    .clock                          (clk_i),
    .reset_c                        (!rst_c_ni),
    .reset_w                        (!rst_w_ni),
    .reset_d                        (!rst_d_ni),
    .test_en                        (test_en_i),
    .esr_minion_boot_op             (orig_minion_boot),
    .esr_mprot_op                   (orig_mprot),
    .esr_vmspagesize_op             (orig_vmspagesize),
    .esr_ipi_redirect_pc_op         (orig_ipi_redirect_pc),
    .esr_hactrl_op                  (orig_hactrl),
    .esr_hastatus0_ip               (hastatus0_i),
    .esr_and_or_treeL0_ip           (and_or_tree_l0_i),
    .esr_pmu_ctrl_op                (orig_pmu_ctrl),
    .esr_neigh_chicken_op           (orig_neigh_chicken),
    .esr_icache_err_log_ctl_op      (orig_icache_err_log_ctl),
    .esr_texture_control_op         (orig_texture_control),
    .esr_texture_status_ip          (texture_status_i),
    .esr_texture_image_table_ptr_op (orig_texture_image_table_ptr),
    .esr_icache_err_log_sbe_ip      (icache_sbe_i),
    .esr_icache_err_log_dbe_ip      (icache_dbe_i),
    .esr_icache_err_log_info_ip     (icache_err_info_i),
    .esr_icache_err_detected_op     (orig_icache_err_detected_o),
    .esr_icache_err_logged_op       (orig_icache_err_logged_o),
    .esr_icache_ecc_count_ov_op     (orig_icache_ecc_count_ov_o),
    .esr_hastatus1_min              (hastatus1_min_i),
    .esr_dmctrl                     (dmctrl_i),
    .dft__reset_byp                 (dft_reset_byp_i),
    .dft__reset                     (dft_reset_i),
    .apb_paddr                      (apb_paddr_i),
    .apb_pwrite                     (apb_pwrite_i),
    .apb_psel                       (apb_psel_i),
    .apb_penable                    (apb_penable_i),
    .apb_pwdata                     (apb_pwdata_i),
    .apb_pready                     (orig_apb_pready_o),
    .apb_prdata                     (orig_apb_prdata_o),
    .apb_pslverr                    (orig_apb_pslverr_o)
  );

  assign new_minion_boot_o = {{(64-minion_pkg::VaSize){1'b0}}, new_minion_boot};
  assign new_mprot_o = {{(64-$bits(esr_pkg::esr_mprot_t)){1'b0}}, new_mprot};
  assign new_vmspagesize_o = {{(64-$bits(minion_pkg::tlb_entry_type)){1'b0}}, new_vmspagesize};
  assign new_ipi_redirect_pc_o = {{(64-$bits(esr_pkg::esr_ipi_pc_t)){1'b0}}, new_ipi_redirect_pc};
  assign new_hactrl_o = {{(64-$bits(esr_pkg::esr_hactrl_t)){1'b0}}, new_hactrl};
  assign new_pmu_ctrl_o = {{(64-$bits(esr_pkg::esr_pmu_ctrl_t)){1'b0}}, new_pmu_ctrl};
  assign new_neigh_chicken_o = {{(64-$bits(esr_pkg::esr_neigh_chicken_t)){1'b0}}, new_neigh_chicken};
  assign new_icache_err_log_ctl_o = {{(64-$bits(icache_pkg::esr_icache_err_log_ctl_t)){1'b0}}, new_icache_err_log_ctl};
  assign new_texture_control_o = {{(64-$bits(esr_pkg::esr_tex_ctl_t)){1'b0}}, new_texture_control};
  assign new_texture_image_table_ptr_o = {16'h0, new_texture_image_table_ptr};

  assign orig_minion_boot_o = {{(64-minion_pkg::VaSize){1'b0}}, orig_minion_boot};
  assign orig_mprot_o = {{(64-$bits(esr_pkg::esr_mprot_t)){1'b0}}, orig_mprot};
  assign orig_vmspagesize_o = {{(64-$bits(minion_pkg::tlb_entry_type)){1'b0}}, orig_vmspagesize};
  assign orig_ipi_redirect_pc_o = {{(64-$bits(esr_pkg::esr_ipi_pc_t)){1'b0}}, orig_ipi_redirect_pc};
  assign orig_hactrl_o = {{(64-$bits(esr_pkg::esr_hactrl_t)){1'b0}}, orig_hactrl};
  assign orig_pmu_ctrl_o = {{(64-$bits(esr_pkg::esr_pmu_ctrl_t)){1'b0}}, orig_pmu_ctrl};
  assign orig_neigh_chicken_o = {{(64-$bits(esr_pkg::esr_neigh_chicken_t)){1'b0}}, orig_neigh_chicken};
  assign orig_icache_err_log_ctl_o = {{(64-$bits(icache_pkg::esr_icache_err_log_ctl_t)){1'b0}}, orig_icache_err_log_ctl};
  assign orig_texture_control_o = {{(64-$bits(esr_pkg::esr_tex_ctl_t)){1'b0}}, orig_texture_control};
  assign orig_texture_image_table_ptr_o = {16'h0, orig_texture_image_table_ptr};
endmodule
