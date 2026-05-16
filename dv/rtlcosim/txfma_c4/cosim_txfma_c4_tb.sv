// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_c4_tb import vpu_defs_pkg::*;
(
  input logic [VPU_DTYPE_SZ-1:0] op_dtype_f4a_h,
  input logic [VPU_FCMD_SZ-1:0] cmd_f4a_h,
  input logic [TXFMA_SIGS_SZ-1:0] sigs_f4a_h,
  input logic [TXFMA_LXD_PS_SZ-1:0] elxd_f4a_h,
  input logic [2:0] sin_res_shf_idx_f4a_h,
  input logic ph_comb_stiky_f4a_h,
  input logic eff_sub1_f4a_h,
  input logic eff_sub2_f4a_h,
  input logic s_res_f4a_h,
  input logic sc_f4a_h,
  output logic orig_neg_norm_a3_f4a_h,
  output logic new_neg_norm_a3_f4a_h,
  output logic orig_neg_norm_na3_f4a_h,
  output logic new_neg_norm_na3_f4a_h,
  output logic orig_clearphps_en_f4a_h,
  output logic new_clearphps_en_f4a_h,
  output logic orig_clear16frnd_nshin_f4a_l,
  output logic new_clear16frnd_nshin_f4a_l,
  output logic orig_ph_ps_clear_f4a_l,
  output logic new_ph_ps_clear_f4a_l,
  output logic [TXFMA_NORM_SHF_AMNT_SZ-1:0] orig_enc_nsamt_f4a_h,
  output logic [TXFMA_NORM_SHF_AMNT_SZ-1:0] new_enc_nsamt_f4a_h,
  output logic orig_elxd_add_in2_mux_sel_f4a_h,
  output logic new_elxd_add_in2_mux_sel_f4a_h,
  output logic orig_use_elxd_f4a_h,
  output logic new_use_elxd_f4a_h
);

  txfma_c4_orig u_orig (
    .op_dtype_f4a_h (op_dtype_f4a_h),
    .cmd_f4a_h (cmd_f4a_h),
    .sigs_f4a_h (sigs_f4a_h),
    .elxd_f4a_h (elxd_f4a_h),
    .sin_res_shf_idx_f4a_h (sin_res_shf_idx_f4a_h),
    .ph_comb_stiky_f4a_h (ph_comb_stiky_f4a_h),
    .eff_sub1_f4a_h (eff_sub1_f4a_h),
    .eff_sub2_f4a_h (eff_sub2_f4a_h),
    .s_res_f4a_h (s_res_f4a_h),
    .sc_f4a_h (sc_f4a_h),
    .neg_norm_a3_f4a_h (orig_neg_norm_a3_f4a_h),
    .neg_norm_na3_f4a_h (orig_neg_norm_na3_f4a_h),
    .clearphps_en_f4a_h (orig_clearphps_en_f4a_h),
    .clear16frnd_nshin_f4a_l (orig_clear16frnd_nshin_f4a_l),
    .ph_ps_clear_f4a_l (orig_ph_ps_clear_f4a_l),
    .enc_nsamt_f4a_h (orig_enc_nsamt_f4a_h),
    .elxd_add_in2_mux_sel_f4a_h (orig_elxd_add_in2_mux_sel_f4a_h),
    .use_elxd_f4a_h (orig_use_elxd_f4a_h)
  );

  txfma_c4 u_new (
    .op_dtype_f4a_h (op_dtype_f4a_h),
    .cmd_f4a_h (cmd_f4a_h),
    .sigs_f4a_h (sigs_f4a_h),
    .elxd_f4a_h (elxd_f4a_h),
    .sin_res_shf_idx_f4a_h (sin_res_shf_idx_f4a_h),
    .ph_comb_stiky_f4a_h (ph_comb_stiky_f4a_h),
    .eff_sub1_f4a_h (eff_sub1_f4a_h),
    .eff_sub2_f4a_h (eff_sub2_f4a_h),
    .s_res_f4a_h (s_res_f4a_h),
    .sc_f4a_h (sc_f4a_h),
    .neg_norm_a3_f4a_h (new_neg_norm_a3_f4a_h),
    .neg_norm_na3_f4a_h (new_neg_norm_na3_f4a_h),
    .clearphps_en_f4a_h (new_clearphps_en_f4a_h),
    .clear16frnd_nshin_f4a_l (new_clear16frnd_nshin_f4a_l),
    .ph_ps_clear_f4a_l (new_ph_ps_clear_f4a_l),
    .enc_nsamt_f4a_h (new_enc_nsamt_f4a_h),
    .elxd_add_in2_mux_sel_f4a_h (new_elxd_add_in2_mux_sel_f4a_h),
    .use_elxd_f4a_h (new_use_elxd_f4a_h)
  );

endmodule : cosim_txfma_c4_tb
