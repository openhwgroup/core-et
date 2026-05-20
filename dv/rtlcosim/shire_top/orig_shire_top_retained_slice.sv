// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Retained original-source top-wiring adapter for the project-native
// shire_top cosim.
//
// The full CORE-ET public shire_top at ORIG_ROOT/rtl/shire/shiretop/shire_top.v
// depends on generated mesh stops, third-party debug fabrics, sensors, hard
// PLL/DLL, TDR/OCC, and scan-hub surfaces that the native compute-shire
// contract intentionally omits.  This adapter keeps the retained Ainekko-owned
// top-shell behavior by instantiating the original-source channel/wrapper
// reference used by the shire_channel_wrap cosim, then layering only the
// top-level neighborhood-stub feedthroughs that are needed for this native
// top smoke comparison.  Retained observations are driven from original-source
// logic or from explicit original stub-neighborhood constants, never from the
// new shire_top DUT.

/* verilator lint_off UNUSEDSIGNAL */  // Adapter keeps observation-only fanout signals for cosim comparison.
/* verilator lint_off PINMISSING */    // The reused channel-wrap reference exposes many outputs this top cosim does not observe.
module orig_shire_top_retained_slice
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import neigh_ch_apb_mux_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_cold_ext_ni,
  input  logic rst_warm_ext_ni,
  input  logic rst_system_ext_ni,
  input  logic rst_system_debug_ext_ni,
  input  logic rst_noc_ext_ni,

  input  logic dft_scanmode_i,
  input  logic dft_scan_reset_ni,
  input  logic dft_sram_clk_i,
  input  logic dft_sram_clk_override_i,
  input  logic dft_ram_rei_i,
  input  logic dft_ram_wei_i,
  input  logic dft_mbist_en_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_phy_id_i,
  input  logic plic_mtip_i,
  input  logic plic_meip_i,
  input  logic plic_seip_i,
  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_err_int_srcs_i,
  input  logic [shirecache_pkg::BankIdSize-1:0] status_monitor_bank_sel_stim_i,
  input  logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0]
      debug_and_or_tree_l2_stim_i,

  input  logic sys_axi_ar_valid_stim_i,
  input  logic sys_axi_aw_valid_stim_i,
  input  logic sys_axi_w_valid_stim_i,
  input  logic sys_axi_b_ready_stim_i,
  input  logic sys_axi_r_ready_stim_i,
  input  logic [1:0] sys_axi_aw_vcvalid_stim_i,
  input  logic [1:0] sys_axi_w_vcvalid_stim_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] sys_id_stim_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] sys_addr_stim_i,
  input  logic [axi_pkg::SysSlaveDataSize-1:0] sys_wdata_stim_i,
  input  logic [7:0] sys_len_stim_i,
  input  logic [2:0] sys_size_stim_i,

  output logic [shire_esr_pkg::NumNeigh-1:0] rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_warm_to_neigh_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_w_icache_no_o,
  output logic rst_system_lv_no_o,
  output logic rst_system_debug_lv_no_o,
  output logic rst_noc_lv_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] clk_neigh_obs_o,
  output logic clk_shire_obs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0] tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] tbox_en_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_meip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_seip_to_neigh_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] int_mtip_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_psel_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] apb_neigh0_paddr_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_neigh0_pwdata_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_psel_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_pwrite_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shirecache_pkg::Banks:0] neigh_sc_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_rsp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_data_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0]
      uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]
      pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]
      pwr_ctrl_neigh_iso_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] ram_cfg_flat_o,
  output logic [$bits(dft_pkg::dft_t)-1:0] dft_hv_flat_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0]
      debug_and_or_tree_l2_flat_o,
  output logic ioshire_combined_err_int_o,
  output logic ioshire_noc_err_int_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_all_err_int_srcs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_done_valid_o,
  output logic sys_axi_ar_ready_obs_o,
  output logic sys_axi_aw_ready_obs_o,
  output logic sys_axi_w_ready_obs_o,
  output logic sys_axi_b_valid_obs_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] sys_axi_b_id_obs_o,
  output logic [1:0] sys_axi_b_resp_obs_o,
  output logic sys_axi_r_valid_obs_o,
  output logic [63:0] sys_axi_r_data_lo_obs_o,
  output logic [1:0] sys_axi_aw_credit_obs_o
);

  localparam int unsigned NumNeigh = shire_esr_pkg::NumNeigh;
  localparam int unsigned NumBanks = shirecache_pkg::Banks;
  localparam int unsigned NumRbox = shirecache_pkg::RboxPerShire;
  localparam int unsigned ApbShireIdx = NumBanks;
  localparam int unsigned ApbRboxIdxStart = ApbShireIdx + 1;
  localparam int unsigned ApbIcacheIdx = ApbRboxIdxStart + NumRbox;

  dft_pkg::dft_t dft_hv;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_mtime_target;
  logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_sys_axi_b_flat;
  logic [$bits(axi_pkg::sys_slave_r_t)-1:0] orig_sys_axi_r_flat;
  axi_pkg::sys_slave_b_t orig_sys_axi_b;
  axi_pkg::sys_slave_r_t orig_sys_axi_r;

  always_comb begin
    dft_hv.scanmode = dft_scanmode_i;
    dft_hv.scan_reset_n = dft_scan_reset_ni;
    dft_hv.sram_clk_override = dft_sram_clk_override_i;
    dft_hv.ram_rei = dft_ram_rei_i;
    dft_hv.ram_wei = dft_ram_wei_i;
  end

  assign dft_hv_flat_o = dft_hv;
  assign clk_neigh_obs_o = {NumNeigh{clk_i}};
  assign clk_shire_obs_o = clk_i;
  assign rst_w_icache_no_o = '0;
  assign plic_meip_to_neigh_o = {NumNeigh{plic_meip_i}};
  assign plic_seip_to_neigh_o = {NumNeigh{plic_seip_i}};
  assign int_mtip_flat_o = {shire_esr_pkg::ThreadsPerShire{plic_mtip_i}} & orig_mtime_target;
  assign orig_sys_axi_b = orig_sys_axi_b_flat;
  assign orig_sys_axi_r = orig_sys_axi_r_flat;
  assign sys_axi_b_id_obs_o = orig_sys_axi_b.id;
  assign sys_axi_b_resp_obs_o = orig_sys_axi_b.resp;
  assign sys_axi_r_data_lo_obs_o = orig_sys_axi_r.data[63:0];

  cosim_shire_channel_wrap_tb u_orig_ref (
    .clk_i,
    .rst_ni,
    .rst_c_ext_ni                       (rst_cold_ext_ni),
    .rst_w_ext_ni                       (rst_warm_ext_ni),
    .rst_d_ext_ni                       (rst_system_debug_ext_ni),
    .rst_system_ext_ni                  (rst_system_ext_ni),
    .rst_noc_ext_ni                     (rst_noc_ext_ni),
    .rst_w_icache_ext_ni                ('1),
    .dft_scanmode_i,
    .dft_scan_reset_ni,
    .dft_sram_clk_i,
    .dft_sram_clk_override_i,
    .dft_ram_rei_i,
    .dft_ram_wei_i,
    .dft_mbist_en_i,
    .shire_id_reset_val_i,
    .shire_phy_id_i,
    .apb_sel_i                          ('0),
    .apb_paddr_i                        ('0),
    .apb_pwrite_i                       (1'b0),
    .apb_psel_i                         (1'b0),
    .apb_penable_i                      (1'b0),
    .apb_pwdata_i                       ('0),
    .noc_err_int_srcs_i,
    .neigh_sc_err_detected_i            ('0),
    .neigh_sc_err_logged_i              ('0),
    .flb_neigh_l2_req_valid_i           ('0),
    .flb_neigh_l2_req_data_i            ('0),
    .neigh0_req_valid_i                 (1'b0),
    .neigh0_req_dest_i                  ('0),
    .neigh0_req_addr_i                  ('0),
    .neigh0_req_opcode_i                ('0),
    .neigh0_req_size_i                  ('0),
    .neigh0_req_qwen_i                  ('0),
    .icache_req_valid_i                 ('0),
    .icache_req_write_i                 ('0),
    .icache_req_addr_i                  ('0),
    .icache_resp_ready_i                ('0),
    .coop_slv_valid_i                   ('0),
    .coop_done_id_i                     ('0),
    .coop_done_valid_i                  ('0),
    .esr_icache_prefetch_done_stim_i    ('0),
    .esr_and_or_tree_l0_flat_stim_i     ('0),
    .debug_and_or_tree_l2_stim_i,
    .bpam_rc_tbox_ack_flat_stim_i       ('0),
    .esr_pwr_ctrl_glb_nsleepout_stim_i  ('0),
    .esr_pwr_ctrl_neigh_nsleepout_stim_i('0),
    .esr_pll_busy_stim_i                (1'b0),
    .esr_pll_rdata_stim_i               ('0),
    .esr_pll_rrdy_stim_i                (1'b0),
    .esr_pll_lock_stim_i                (1'b1),
    .esr_dll_dly_est_sts_flat_stim_i    ('0),
    .esr_dll_busy_stim_i                (1'b0),
    .esr_dll_rdata_stim_i               ('0),
    .esr_dll_rrdy_stim_i                (1'b0),
    .esr_dll_lock_stim_i                (1'b0),
    .status_monitor_bank_sel_i          (status_monitor_bank_sel_stim_i),
    .neigh_sc_rsp_ready_stim_i          ('0),
    .to_l3_axi_ar_ready_stim_i          ('1),
    .to_l3_axi_aw_ready_stim_i          ('1),
    .to_l3_axi_w_ready_stim_i           ('1),
    .to_l3_axi_b_valid_stim_i           ('0),
    .to_l3_axi_r_valid_stim_i           ('0),
    .to_sys_axi_ar_ready_stim_i         ('1),
    .to_sys_axi_aw_ready_stim_i         ('1),
    .to_sys_axi_w_ready_stim_i          ('1),
    .to_sys_axi_b_valid_stim_i          ('0),
    .to_sys_axi_r_valid_stim_i          ('0),
    .l3_axi_ar_valid_stim_i             ('0),
    .l3_axi_aw_valid_stim_i             ('0),
    .l3_axi_w_valid_stim_i              ('0),
    .l3_axi_b_ready_stim_i              ('1),
    .l3_axi_r_ready_stim_i              ('1),
    .uc_to_l3_axi_ar_ready_stim_i       (1'b1),
    .uc_to_l3_axi_aw_ready_stim_i       (1'b1),
    .uc_to_l3_axi_w_ready_stim_i        (1'b1),
    .uc_to_l3_axi_b_valid_stim_i        (1'b0),
    .uc_to_l3_axi_r_valid_stim_i        (1'b0),
    .uc_to_sys_axi_ar_ready_stim_i      (1'b1),
    .uc_to_sys_axi_aw_ready_stim_i      (1'b1),
    .uc_to_sys_axi_w_ready_stim_i       (1'b1),
    .uc_to_sys_axi_b_valid_stim_i       (1'b0),
    .uc_to_sys_axi_r_valid_stim_i       (1'b0),
    .sys_axi_ar_valid_stim_i,
    .sys_axi_aw_valid_stim_i,
    .sys_axi_w_valid_stim_i,
    .sys_axi_b_ready_stim_i,
    .sys_axi_r_ready_stim_i,
    .sys_axi_aw_vcvalid_stim_i,
    .sys_axi_w_vcvalid_stim_i,
    .sbm_write_credit_return_stim_i     (1'b0),
    .sbm_sys_axi_ar_ready_stim_i        (1'b1),
    .sbm_sys_axi_aw_ready_stim_i        (1'b1),
    .sbm_sys_axi_w_ready_stim_i         (1'b1),
    .sbm_sys_axi_b_valid_stim_i         (1'b0),
    .sbm_sys_axi_r_valid_stim_i         (1'b0),
    .axi_stim_i                         ({24'h0, noc_err_int_srcs_i}),
    .sys_id_stim_i,
    .sys_addr_stim_i,
    .sys_wdata_stim_i,
    .sys_len_stim_i,
    .sys_size_stim_i,
    .orig_rst_c_shire_no_o              (rst_c_shire_no_o),
    .orig_rst_w_shire_no_o              (rst_w_shire_no_o),
    .orig_rst_d_shire_no_o              (rst_d_shire_no_o),
    .orig_rst_system_lv_no_o            (rst_system_lv_no_o),
    .orig_rst_system_debug_lv_no_o      (rst_system_debug_lv_no_o),
    .orig_rst_noc_lv_no_o               (rst_noc_lv_no_o),
    .orig_rst_warm_to_neigh_no_o        (rst_warm_to_neigh_no_o),
    .orig_apb_neigh_psel_o              (apb_neigh_psel_o),
    .orig_apb_neigh_pwrite_o            (apb_neigh_pwrite_o),
    .orig_apb_neigh0_paddr_o            (apb_neigh0_paddr_o),
    .orig_apb_neigh0_pwdata_o           (apb_neigh0_pwdata_o),
    .orig_coop_slv_valid_o              (coop_slv_valid_o),
    .orig_coop_done_valid_o             (coop_done_valid_o),
    .orig_shire_id_o                    (shire_id_o),
    .orig_tbox_id_o                     (tbox_id_o),
    .orig_tbox_en_o                     (tbox_en_o),
    .orig_esr_mtime_local_target_o      (orig_mtime_target),
    .orig_debug_and_or_tree_l2_o        (debug_and_or_tree_l2_flat_o),
    .orig_ioshire_log_err_int_o         (ioshire_combined_err_int_o),
    .orig_uc_to_neigh_fcc_o             (uc_to_neigh_fcc_o),
    .orig_uc_to_neigh_fcc_target_o      (uc_to_neigh_fcc_target_o),
    .orig_flb_l2_neigh_resp_valid_o     (flb_l2_neigh_resp_valid_o),
    .orig_flb_l2_neigh_resp_data_o      (flb_l2_neigh_resp_data_o),
    .orig_neigh_sc_req_ready_o          (neigh_sc_req_ready_o),
    .orig_neigh_sc_rsp_valid_o          (neigh_sc_rsp_valid_o),
    .orig_icache_req_ready_o            (icache_req_ready_o),
    .orig_icache_resp_valid_o           (icache_resp_valid_o),
    .orig_noc_all_err_int_srcs_o        (noc_all_err_int_srcs_o),
    .orig_ioshire_noc_err_int_o         (ioshire_noc_err_int_o),
    .orig_pwr_ctrl_glb_nsleepin_o       (pwr_ctrl_glb_nsleepin_o),
    .orig_pwr_ctrl_glb_iso_o            (pwr_ctrl_glb_iso_o),
    .orig_pwr_ctrl_neigh_nsleepin_o     (pwr_ctrl_neigh_nsleepin_o),
    .orig_pwr_ctrl_neigh_iso_o          (pwr_ctrl_neigh_iso_o),
    .orig_ram_cfg_flat_o                (ram_cfg_flat_o),
    .orig_sys_axi_ar_ready_o            (sys_axi_ar_ready_obs_o),
    .orig_sys_axi_aw_ready_o            (sys_axi_aw_ready_obs_o),
    .orig_sys_axi_w_ready_o             (sys_axi_w_ready_obs_o),
    .orig_sys_axi_b_flat_o              (orig_sys_axi_b_flat),
    .orig_sys_axi_b_valid_o             (sys_axi_b_valid_obs_o),
    .orig_sys_axi_r_flat_o              (orig_sys_axi_r_flat),
    .orig_sys_axi_r_valid_o             (sys_axi_r_valid_obs_o),
    .orig_sys_axi_aw_credit_o           (sys_axi_aw_credit_obs_o)
  );

  for (genvar bank = 0; bank < NumBanks; bank++) begin : gen_channel_bank_apb_obs
    assign apb_channel_psel_o[bank] =
        u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].apb_psel;
    assign apb_channel_pwrite_o[bank] =
        u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].apb_pwrite;
  end

  assign apb_channel_psel_o[ApbShireIdx] =
      u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::ShireRangeLo].apb_psel;
  assign apb_channel_pwrite_o[ApbShireIdx] =
      u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::ShireRangeLo].apb_pwrite;

  for (genvar rbox = 0; rbox < NumRbox; rbox++) begin : gen_channel_rbox_apb_obs
    assign apb_channel_psel_o[ApbRboxIdxStart + rbox] =
        u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].apb_psel;
    assign apb_channel_pwrite_o[ApbRboxIdxStart + rbox] =
        u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].apb_pwrite;
  end

  assign apb_channel_psel_o[ApbIcacheIdx] =
      u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].apb_psel;
  assign apb_channel_pwrite_o[ApbIcacheIdx] =
      u_orig_ref.orig_sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].apb_pwrite;

endmodule : orig_shire_top_retained_slice
/* verilator lint_on PINMISSING */
/* verilator lint_on UNUSEDSIGNAL */
