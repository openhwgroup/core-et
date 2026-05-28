// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module txfma_f6_tb
(
  input logic ctrl_f5_clk,
  input logic [TXFMA_A0:TXFMA_B31] mergd_frac_res_hi_f6a_h,
  input logic [FP_CLASS_BITS-1:0] classify_f6a_h,
  input logic sel_classify_out_f6a_h,
  input logic sel_nr1_frcpres_f6a_h,
  input logic rnd_l_ph_f6a_h,
  input logic rnd_r_ph_f6a_h,
  input logic rnd_l_ps_f6a_h,
  input logic rnd_r_ps_f6a_h,
  input logic rnd_l_int_f6a_h,
  input logic rnd_r_int_f6a_h,
  input logic [TXFMA_FTZ_SZ-1:0] ftz_f6a_l,
  input logic rc6_ph_f5a_h,
  input logic rc6_ps_f5a_h,
  input logic rc_int32_f5a_h,
  input logic normmask1_f6a_h,
  input logic normmask2_f6a_h,
  input logic trans_sinp2m_f6a_h,
  input logic s_res_f6a_h,
  input logic [TXFMA_EXP_PS_SZ-1:0] exp_res_f6a_h,
  input logic trans2_ps_roundup_r_f5a_h,
  input logic ps_rndovf_f6a_h,
  input logic ph_rndovf_f6a_h,
  input logic sel_set_bit0_one_f6a_h,
  input logic effsub1_set_sign1_f6a_h,
  input logic [1:0] rrmux32_sel_f6a_h,
  input logic rrmux16l_sel_f6a_l,
  input logic f10f11mux_sel_f6a_h,
  input logic sel_psexc_sr_f6a_h,
  input logic sel_bit31_one_f6a_h,
  input logic sel_b30to0_ones_f6a_h,
  input logic sel_zerores_f6a_h,
  input logic sel_ps_unf_f6a_h,
  input logic sel_ps_normone_f6a_h,
  input logic sel_ps_inf_f6a_h,
  input logic sel_ps_cqn_f6a_h,
  input logic sel_ps_large_f6a_h,
  input logic sel_phsp_f6a_h,
  input logic sel_phhi_sr_f6a_h,
  input logic sel_phhi_large_f6a_h,
  input logic sel_phhi_unf_f6a_h,
  input logic sel_phhi_infres_f6a_h,
  input logic sel_f11inf_f6a_h,
  input logic sel_f10cqnan_f6a_h,
  input logic sel_f11sp_f6a_h,
  input logic sel_f10sp_f6a_h,
  input logic sel_f11large_f6a_h,
  input logic sel_f10large_f6a_h,
  input logic sel_gmaxu24_f6a_h,
  input logic sel_gmaxu16_f6a_h,
  input logic sel_gmaxs16_f6a_h,
  input logic sel_gmins16_f6a_h,
  input logic sel_gmaxu2_f6a_h,
  input logic sel_gmaxu8_f6a_h,
  input logic sel_gmaxs8_f6a_h,
  input logic sel_gmins8_f6a_h,
  input logic sel_gfxmax_f6a_h,
  input logic [1:0] sel_excreshi_f6a_h,
  input logic [1:0] sel_excreslo_f6a_h,
  input logic [1:0] sel_fmares_hi_f6a_h,
  input logic [1:0] sel_fmares_lo_f6a_h,
  output logic int_res_l_bit_f6a_h,
  output logic [VPU_DATA_S_SZ-1:0] fma_res_f6a_h
);

  txfma_f6 u_dut (.*);

endmodule : txfma_f6_tb
