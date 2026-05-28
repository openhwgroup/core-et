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

module cosim_txfma_f1_tb
(
  input logic ctrl_f0_clk,
  input logic multype_f0_clk,
  input logic ctrl_f1_clk,
  input logic multype_f1_clk,
  input logic [VPU_DATA_S_SZ-1:0] fa_f0a_h,
  input logic [VPU_DATA_S_SZ-1:0] fb_f0a_h,
  input logic [VPU_DATA_S_SZ-1:0] fc_f0a_h,
  input logic dtps_f0a_h,
  input logic mul_sign_f0a_h,
  input logic mul_hp_mode_f0a_h,
  output logic orig_fc_b0_f2a_h,
  output logic new_fc_b0_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] orig_fc_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] new_fc_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] orig_fa_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] new_fa_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] orig_wallace_fC21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] new_wallace_fC21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] orig_wallace_fC22_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] new_wallace_fC22_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] orig_wallace_fS21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] new_wallace_fS21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] orig_wallace_fS22_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] new_wallace_fS22_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] orig_booth_fCin_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] new_booth_fCin_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] orig_booth_fSxt_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] new_booth_fSxt_f2a_h,
  output logic [32:0] orig_booth_fpp17_f2a_h,
  output logic [32:0] new_booth_fpp17_f2a_h,
  output logic [3:0] orig_etrz_fa_hi_ph_f1a_h,
  output logic [3:0] new_etrz_fa_hi_ph_f1a_h,
  output logic [4:0] orig_etrz_fa_lo_ps_f1a_h,
  output logic [4:0] new_etrz_fa_lo_ps_f1a_h,
  output logic [3:0] orig_etrz_fb_hi_ph_f1a_h,
  output logic [3:0] new_etrz_fb_hi_ph_f1a_h,
  output logic [4:0] orig_etrz_fb_lo_ps_f1a_h,
  output logic [4:0] new_etrz_fb_lo_ps_f1a_h,
  output logic [4:0] orig_etrz_fc_f1a_h,
  output logic [4:0] new_etrz_fc_f1a_h
);

  txfma_f1_orig u_orig (
    .ctrl_f0_clk (ctrl_f0_clk),
    .multype_f0_clk (multype_f0_clk),
    .ctrl_f1_clk (ctrl_f1_clk),
    .multype_f1_clk (multype_f1_clk),
    .fa_f0a_h (fa_f0a_h),
    .fb_f0a_h (fb_f0a_h),
    .fc_f0a_h (fc_f0a_h),
    .dtps_f0a_h (dtps_f0a_h),
    .mul_sign_f0a_h (mul_sign_f0a_h),
    .mul_hp_mode_f0a_h (mul_hp_mode_f0a_h),
    .fc_b0_f2a_h (orig_fc_b0_f2a_h),
    .fc_f2a_h (orig_fc_f2a_h),
    .fa_f2a_h (orig_fa_f2a_h),
    .wallace_fC21_f2a_h (orig_wallace_fC21_f2a_h),
    .wallace_fC22_f2a_h (orig_wallace_fC22_f2a_h),
    .wallace_fS21_f2a_h (orig_wallace_fS21_f2a_h),
    .wallace_fS22_f2a_h (orig_wallace_fS22_f2a_h),
    .booth_fCin_f2a_h (orig_booth_fCin_f2a_h),
    .booth_fSxt_f2a_h (orig_booth_fSxt_f2a_h),
    .booth_fpp17_f2a_h (orig_booth_fpp17_f2a_h),
    .etrz_fa_hi_ph_f1a_h (orig_etrz_fa_hi_ph_f1a_h),
    .etrz_fa_lo_ps_f1a_h (orig_etrz_fa_lo_ps_f1a_h),
    .etrz_fb_hi_ph_f1a_h (orig_etrz_fb_hi_ph_f1a_h),
    .etrz_fb_lo_ps_f1a_h (orig_etrz_fb_lo_ps_f1a_h),
    .etrz_fc_f1a_h (orig_etrz_fc_f1a_h)
  );

  txfma_f1 u_new (
    .ctrl_f0_clk (ctrl_f0_clk),
    .multype_f0_clk (multype_f0_clk),
    .ctrl_f1_clk (ctrl_f1_clk),
    .multype_f1_clk (multype_f1_clk),
    .fa_f0a_h (fa_f0a_h),
    .fb_f0a_h (fb_f0a_h),
    .fc_f0a_h (fc_f0a_h),
    .dtps_f0a_h (dtps_f0a_h),
    .mul_sign_f0a_h (mul_sign_f0a_h),
    .mul_hp_mode_f0a_h (mul_hp_mode_f0a_h),
    .fc_b0_f2a_h (new_fc_b0_f2a_h),
    .fc_f2a_h (new_fc_f2a_h),
    .fa_f2a_h (new_fa_f2a_h),
    .wallace_fC21_f2a_h (new_wallace_fC21_f2a_h),
    .wallace_fC22_f2a_h (new_wallace_fC22_f2a_h),
    .wallace_fS21_f2a_h (new_wallace_fS21_f2a_h),
    .wallace_fS22_f2a_h (new_wallace_fS22_f2a_h),
    .booth_fCin_f2a_h (new_booth_fCin_f2a_h),
    .booth_fSxt_f2a_h (new_booth_fSxt_f2a_h),
    .booth_fpp17_f2a_h (new_booth_fpp17_f2a_h),
    .etrz_fa_hi_ph_f1a_h (new_etrz_fa_hi_ph_f1a_h),
    .etrz_fa_lo_ps_f1a_h (new_etrz_fa_lo_ps_f1a_h),
    .etrz_fb_hi_ph_f1a_h (new_etrz_fb_hi_ph_f1a_h),
    .etrz_fb_lo_ps_f1a_h (new_etrz_fb_lo_ps_f1a_h),
    .etrz_fc_f1a_h (new_etrz_fc_f1a_h)
  );

endmodule : cosim_txfma_f1_tb
