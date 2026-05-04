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

/* verilator lint_off UNUSEDSIGNAL */  // Imported original TXFMA fraction top retains scan/test pins and helper taps that are unused in this build.
module txfmafrac_top
(
  input                                     txfma_clk,
  input                                     txfmac0_se,
  input                                     txfma_se,
  input                                     test_enable,
  input                                     si_txfmac0,
  input                                     si_txfma_byp_abc_opd,
// clock en
  input                                     opa_clken_frfa_h,
  input                                     opb_clken_frfa_h,
  input                                     opc_clken_frfa_h,
  input                                     transop_clken_frfa_h,
  input                                     txfma_clken_frfa_h,
  input                                     txfma_clken_f0a_h,
  input                                     multype_clken_f0a_h,
  input                                     txfma_clken_f1a_h,
  input                                     multype_clken_f1a_h,
  input                                     txfma_clken_f2a_h,
  input                                     txfma_clken_f3a_h,
  input                                     txfma_clken_f4a_h,
  input                                     txfma_clken_f5a_h,

// F0 Inputs
  input [VPU_DATA_S_SZ-1:0]                opa_byp_frfa_h,
  input [VPU_DATA_S_SZ-1:0]                opb_byp_frfa_h,
  input [VPU_DATA_S_SZ-1:0]                opc_byp_frfa_h,
  input [TRANS_C0_MAX_SIZE-1:0]            xrom_c1c0k_ra_h,
  input [TRANS_C2_MAX_SIZE-1:0]            xrom_c2_ra_h,
  input [TRANS_MASK_SIZE-1:0]              trans_mask_ra_h,
  input [5:0]                               eb_hi_k_exa_h,
  input [1:0]                               sel_ebin_hi_exa_h,
  input [1:0]                               sel_eainmux_exa_h,
  input [1:0]                               sel_ec5b_exa_h,
  input [1:0]                               sel_ecin_exa_h,
  input [1:0]                               sel_sc_exa_h,
  input [1:0]                               sel_opcgfx_exa_h,
  input [1:0]                               sel_xopc_exa_h,
  input [1:0]                               sel_opcfmt_exa_h,
  input [1:0]                               sel_transfa_exa_h,
  input [1:0]                               sel_transfb_exa_h,
  input [1:0]                               sel_opafmt_exa_h,
  input [1:0]                               sel_opbfmt_exa_h,
  input                                     sel_eblo_ph_exa_h,
  input                                     sel_ec5bdet_exa_h,
  input                                     elo_eb_en_exa_h,
  input                                     ehi_eb_en_exa_h,
  input                                     elo_ea_en_exa_h,
  input                                     fa_en_exa_h,
  input                                     fb_en_exa_h,
  input                                     fc_en_exa_h,
  input                                     sel_ealo_ph_exa_h,
  input                                     sa_hi_en_exa_h,
  input                                     sa_lo_en_exa_h,
  input                                     sb_hi_en_exa_h,
  input                                     sb_lo_en_exa_h,
  input                                     sc_en_exa_h,
  input                                     ehi_ea_en_exa_h,
  input                                     ec_en_exa_h,
  input                                     neg_fmaop_exa_h,
  input                                     trans_op2_exa_l,
  input                                     sa1_nr2_frcpfxp_exa_h,
  input                                     sel_sign_sinp2_exa_h,
  input                                     sa_zap_exa_l,
  input                                     sb_zap_exa_l,
  input                                     sel_f10_exa_h,
  input                                     sel_f11_exa_h,
  input                                     fc_dtps_exa_h,
  input                                     sel_snorm_exa_h,
  input                                     sel_g1sk2_exa_h,
  input                                     sel_g1sk8_exa_h,
  input                                     sel_g1sk16_exa_h,
  input                                     sel_g1sk24_exa_h,
  input                                     sel_xexpfrac_exa_h,
  input                                     mul_sign_f0a_h,
  input                                     mul_hp_mode_f0a_h,
  input                                     dtps_f0a_h,
// F1 Inputs
  input                                     sel_famask_f1a_h,
  input                                     paddin1_sels_f1a_h,
  input                                     paddin2_selc_f1a_h,
  input                                     paddin2_selfcneg_f1a_h,
  input                                     padd_cin_f1a_h,
// F2 Inputs
  input [6:0]                               exp_res_f2a_h,
  input [1:0]                               op3p3spc_sel_f2a_h,
  input [2:0]                               op3p2c_sel_f2a_h,
  input [1:0]                               ugfxrplc_sel_f2a_h,
  input [1:0]                               sgfxrplc_sel_f2a_h,
  input [1:0]                               icgfxc_sel_f2a_h,
  input [1:0]                               al1znp2c_sel_f2a_h,
  input [1:0]                               al2c_sel_f2a_h,
  input [5:0]                               ediff_abs_diff_hi_f2a_h,
  input [4:0]                               align2_rshamt_c3_f2a_h,
  input [1:0]                               al1p2c_sel_f2a_h,
  input                                     al1p1c_sel_f2a_h,
  input                                     op3p1c_sel_p_f2a_h,
  input                                     op3p1c_sel_r_f2a_h,
  input                                     rm_rne_f2a_h,
  input                                     rm_rmm_f2a_h,
  input                                     frnd_rupnsc_f2a_h,
  input                                     frnd_rdnsc_f2a_h,
  input                                     frnd_t1_f2a_h,
  input                                     frnd_t2_f2a_h,
  input                                     al1c_zap_f2a_h,
  input                                     op1c_log_mul_clr_f2a_h,
  input                                     op2_sxtto_g_f2a_h,
  input                                     negop1c_int_logmul_clr_f2a_h,
  input                                     psc_op1_clr_f2a_h,
  input                                     align2_zap_f2a_h,
  input                                     align2c_neg_f2a_h,
// F3 Inputs
  input                                     op3g_sel_f2a_h,
  input                                     al2c3_neg_cin_ps_f3a_h,
  input                                     al1c3_neg_cin_f3a_h,
  input                                     fma_subopc3_f3a_h,
  input [1:0]                               imxc_sel_f3a_h,
// F4 Inputs
  input                                     clear16frnd_nshin_f4a_l,
  input                                     clearphps_en_f4a_h,
  input                                     ph_ps_clear_f4a_l,
  input                                     neg_norm_a3_f4a_h,
  input                                     neg_norm_na3_f4a_h,
  input [TXFMA_NORM_SHF_AMNT_SZ-1:0]       enc_nsamt_f4a_h,
// F5 Inputs
  input                                     en_logrrsel1b_f5a_l,
  input                                     mmx_sel_immres_f5a_h,
  input                                     nshcen_sel1b_f5a_h,
  input                                     rc_ps_f5a_h,
  input                                     rc_ph_f5a_h,
  input                                     rc6_ps_f5a_h,
  input                                     rc6_ph_f5a_h,
  input                                     rc_int32_f5a_h,
  input                                     int32_stky2r_f5a_h,
  input                                     stky2g_f5a_h,
  input                                     rc_3rmxr_f5a_h,
  input                                     rc_rl01_f5a_h,
  input                                     rc_rmm_f5a_h,
  input                                     rc_rup_f5a_h,
  input                                     rc_rne_f5a_h,
  input                                     rc_rdn_f5a_h,
  input                                     rc_res_sign_f5a_l,
  input                                     rc_transrnd_f5a_h,
  input                                     rc_transrnd_log_fma2_f5a_h,
// F6 Inputs
  input [FP_CLASS_BITS-1:0]                classify_f6a_h,
  input [1:0]                               rrmux32_sel_f6a_h,
  input [1:0]                               sel_excreshi_f6a_h,
  input [1:0]                               sel_excreslo_f6a_h,
  input [1:0]                               sel_fmares_hi_f6a_h,
  input [1:0]                               sel_fmares_lo_f6a_h,
  input [TXFMA_EXP_PS_SZ-1:0]              exp_res_f6a_h,
  input                                     sel_classify_out_f6a_h,
  input                                     sel_nr1_frcpres_f6a_h,
  input                                     trans2_ps_roundup_r_f5a_h,
  input                                     normmask1_f6a_h,
  input                                     normmask2_f6a_h,
  input                                     trans_sinp2m_f6a_h,
  input                                     s_res_f6a_h,
  input                                     rrmux16l_sel_f6a_l,
  input                                     ps_rndovf_f6a_h,
  input                                     ph_rndovf_f6a_h,
  input                                     sel_set_bit0_one_f6a_h,
  input                                     effsub1_set_sign1_f6a_h,
  input                                     f10f11mux_sel_f6a_h,
  input                                     sel_psexc_sr_f6a_h,
  input                                     sel_bit31_one_f6a_h,
  input                                     sel_b30to0_ones_f6a_h,
  input                                     sel_ps_unf_f6a_h,
  input                                     sel_zerores_f6a_h,
  input                                     sel_ps_normone_f6a_h,
  input                                     sel_ps_inf_f6a_h,
  input                                     sel_ps_cqn_f6a_h,
  input                                     sel_ps_large_f6a_h,
  input                                     sel_phsp_f6a_h,
  input                                     sel_phhi_sr_f6a_h,
  input                                     sel_phhi_large_f6a_h,
  input                                     sel_phhi_unf_f6a_h,
  input                                     sel_phhi_infres_f6a_h,
  input                                     sel_f11inf_f6a_h,
  input                                     sel_f10cqnan_f6a_h,
  input                                     sel_f11sp_f6a_h,
  input                                     sel_f10sp_f6a_h,
  input                                     sel_f11large_f6a_h,
  input                                     sel_f10large_f6a_h,
  input                                     sel_gmaxu24_f6a_h,
  input                                     sel_gmaxu16_f6a_h,
  input                                     sel_gmaxs16_f6a_h,
  input                                     sel_gmins16_f6a_h,
  input                                     sel_gmaxs8_f6a_h,
  input                                     sel_gmaxu2_f6a_h,
  input                                     sel_gmins8_f6a_h,
  input                                     sel_gmaxu8_f6a_h,
  input                                     sel_gfxmax_f6a_h,
// F0 Outputs
  output logic [TXFMA_EXP_PS_SZ-1:0]  	    ea_hi_f0a_h,
  output logic [TXFMA_EXP_PH_SZ-1:0]  	    ea_lo_f0a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]  	    eb_hi_f0a_h,
  output logic [TXFMA_EXP_PH_SZ-1:0]  	    eb_lo_f0a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]  	    ec_f0a_h,
  output logic                              sa_hi_f0a_h,
  output logic                              sa_lo_f0a_h,
  output logic                         	    sb_hi_f0a_h,
  output logic                         	    sb_lo_f0a_h,
  output logic                         	    sc_f0a_h,
  output logic				                ea_hi_zero_f0a_l,
  output logic                         	    ea_hi_all1_f0a_h,
  output logic				                ea_lo_zero_f0a_l,
  output logic                         	    ea_lo_all1_f0a_h,
  output logic				                eb_hi_zero_f0a_l,
  output logic                         	    eb_hi_all1_f0a_h,
  output logic				                eb_lo_zero_f0a_l,
  output logic                         	    eb_lo_all1_f0a_h,
  output logic                              eb_eql_127_f0a_h,
  output logic                              eb_eql_126_f0a_h,
  output logic                              eb_eql_125_f0a_h,
  output logic				                ec_zero_f0a_l,
  output logic                         	    ec_all1_f0a_h,
  output logic                              fcf10_zero_f0a_l,
  output logic                              fcf11_zero_f0a_l,
  output logic                              fcps_zero_f0a_h,
  output logic                              fcph_zero_f0a_l,
  output logic                              fcnorm_zero_29_8_f0a_h,
  output logic                              fcnorm16_eq_1_f0a_h,
  output logic                              fcnorm8_eq_1_f0a_h,
  output logic                              fcnorm_30_f0a_h,
  output logic                              faph_hi_b0_f0a_h,
  output logic                              fbph_hi_b0_f0a_h,
  output logic                              fcph_b0_f0a_h,
  output logic                              fcf11_b0_f0a_h,
  output logic                              fcf10_b0_f0a_h,
  output logic                              fcn8_one_30_24_f0a_h,
  output logic                              fcn10_one_30_22_f0a_h,
  output logic                              fcn16_one_30_16_f0a_h,
  output logic                              fcn24_one_30_8_f0a_h,
  output logic                              faps_zero_f0a_l,
  output logic                              fbps_zero_f0a_l,
  output logic                              faph_hi_zero_f0a_l,
  output logic                              fbph_hi_zero_f0a_l,
  output logic                              faph_lo_zero_f0a_l,
  output logic                              fbph_lo_zero_f0a_l,
  output logic                              faph_lo_b0_f0a_h,
  output logic                              fbph_lo_b0_f0a_h,
  output logic                              faps_b0_f0a_h,
  output logic                              fbps_b0_f0a_h,
  output logic                              fc_31_f0a_h,
  output logic                              fcps_b0_f0a_h,
// F1 Outputs
  output logic [3:0]                        etrz_fa_hi_ph_f1a_h,
  output logic [4:0]                        etrz_fa_lo_ps_f1a_h,
  output logic [3:0]                        etrz_fb_hi_ph_f1a_h,
  output logic [4:0]                        etrz_fb_lo_ps_f1a_h,
  output logic [4:0]                        etrz_fc_f1a_h,
// F2 Outputs
  output logic                              fc_b0_f2a_h,
// F3 Outputs
  output logic                              prodsum_a2_f3a_h,
  output logic                              fround_rbit_f3a_h,
// F4 Outputs
  output logic [TXFMA_LXD_PS_SZ-1:0]       elxd_f4a_h,
  output logic [TXFMA_WSPS_ETRZ_SZ-1:0]    etrz50_f4a_h,
// F5 Outputs
  output logic                              ph_allone2r_f5a_h,
  output logic                              ph_allone2l_f5a_h,
  output logic                              ps_allone2r_f5a_h,
  output logic                              ps_allone2l_f5a_h,
  output logic                              nshc_f5a_h,
  output logic                              ph_stky2l_f5a_h,
  output logic                              ps_stky2l_f5a_h,
  output logic                              int32_stky2l_f5a_h,
  output logic                              comb_add_a3_f5a_h,
// F6 Outputs
  output [VPU_DATA_S_SZ-1:0]               fma_res_f6a_h,
  output logic                              frac_zero_f6a_h,
  output logic                              rnd_l_ph_f6a_h,
  output logic                              rnd_r_ph_f6a_h,
  output logic                              rnd_l_ps_f6a_h,
  output logic                              rnd_r_ps_f6a_h,
  output logic                              int_res_l_bit_f6a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////

logic [VPU_DATA_S_SZ-1:0]                   fa_f0a_h;
logic [VPU_DATA_S_SZ-1:0]                   fb_f0a_h;
logic [VPU_DATA_S_SZ-1:0]                   fc_f0a_h;
logic [TXFMA_A3:0]                          wallace_fC21_f2a_h;
logic [TXFMA_A3:0]                          wallace_fC22_f2a_h;
logic [TXFMA_A3:0]                          wallace_fS21_f2a_h;
logic [TXFMA_A3:0]                          wallace_fS22_f2a_h;
logic [VPU_DATA_S_SZ-1:0]                   booth_fCin_f2a_h;
logic [VPU_DATA_S_SZ-1:0]                   booth_fSxt_f2a_h;
logic [32:0]                                 booth_fpp17_f2a_h;
logic [TXFMA_A3:TXFMA_B27]                 fa_f2a_h;
logic [TXFMA_A3:TXFMA_B27]                 fc_f2a_h;
logic [TXFMA_A3:TXFMA_B27]                 fa_f3a_h;
logic [TXFMA_A3:TXFMA_B27]                 fc_f3a_h;
logic [TXFMA_A3:TXFMA_B59]                 prodsum_f3a_h;
logic [TXFMA_A3:TXFMA_B47]                 comb_s_f4a_h;
logic [TXFMA_A3:TXFMA_B47]                 comb_c_f4a_h;
logic [TXFMA_A3:TXFMA_B27]                 int_minmax_res_f4a_h;
logic [TXFMA_A3:TXFMA_B27]                 int_minmax_res_f5a_h;
logic [TXFMA_A3:TXFMA_B47]                 comb_add_f5a_h;
logic [TXFMA_A0:TXFMA_B31]                 mergd_frac_res_hi_f6a_h;
logic [TXFMA_FTZ_SZ-1:0]                    ftz_f6a_l;
dft_t                                        test_enable_dft;

always_comb begin
  test_enable_dft = '0;
  test_enable_dft.scanmode = test_enable;
end
logic                                        neg_norm_f5a_h;
logic                                        op2_sxtto_g_f3a_h;
logic                                        rnd_r_int_f6a_h;
logic                                        opa_frf_clk;
logic                                        opb_frf_clk;
logic                                        opc_frf_clk;
logic                                        multype_f0_clk;
logic                                        trans_frf_clk;
logic                                        ctrl_frf_clk;
logic                                        ctrl_f0_clk;
logic                                        ctrl_f1_clk;
logic                                        multype_f1_clk;
logic                                        ctrl_f2_clk;
logic                                        ctrl_f3_clk;
logic                                        ctrl_f4_clk;
logic                                        ctrl_f5_clk;
logic                                        rnd_l_int_f6a_h;


///////////////////////////////////////////////////////////////////////////////
// CLK gates
///////////////////////////////////////////////////////////////////////////////

prim_clk_gate cgate_txfma0_opa_frf (
  .clk_i ( txfma_clk        ),
  .en_i  ( opa_clken_frfa_h ),
  .dft_i ( test_enable_dft  ),
  .clk_o ( opa_frf_clk      )
);

prim_clk_gate cgate_txfma0_opb_frf (
  .clk_i ( txfma_clk        ),
  .en_i  ( opb_clken_frfa_h ),
  .dft_i ( test_enable_dft  ),
  .clk_o ( opb_frf_clk      )
);

prim_clk_gate cgate_txfma0_opc_frf (
  .clk_i ( txfma_clk        ),
  .en_i  ( opc_clken_frfa_h ),
  .dft_i ( test_enable_dft  ),
  .clk_o ( opc_frf_clk      )
);

prim_clk_gate cgate_txfma0_trans_frf (
  .clk_i ( txfma_clk            ),
  .en_i  ( transop_clken_frfa_h ),
  .dft_i ( '0                   ),
  .clk_o ( trans_frf_clk        )
);

prim_clk_gate cgate_txfma0_ctrl_frf (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_frfa_h ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_frf_clk       )
);

prim_clk_gate cgate_txfma0_ctrl_f0 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f0a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f0_clk        )
);

prim_clk_gate cgate_txfma0_multype_f0 (
  .clk_i ( txfma_clk           ),
  .en_i  ( multype_clken_f0a_h ),
  .dft_i ( '0                  ),
  .clk_o ( multype_f0_clk      )
);

prim_clk_gate cgate_txfma0_multype_f1 (
  .clk_i ( txfma_clk           ),
  .en_i  ( multype_clken_f1a_h ),
  .dft_i ( '0                  ),
  .clk_o ( multype_f1_clk      )
);

prim_clk_gate cgate_txfma0_ctrl_f1 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f1a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f1_clk        )
);

prim_clk_gate cgate_txfma0_ctrl_f2 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f2a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f2_clk        )
);

prim_clk_gate cgate_txfma0_ctrl_f3 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f3a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f3_clk        )
);

prim_clk_gate cgate_txfma0_ctrl_f4 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f4a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f4_clk        )
);

prim_clk_gate cgate_txfma0_ctrl_f5 (
  .clk_i ( txfma_clk          ),
  .en_i  ( txfma_clken_f5a_h  ),
  .dft_i ( '0                 ),
  .clk_o ( ctrl_f5_clk        )
);


///////////////////////////////////////////////////////////////////////////////
// F0 component: INMUX for sign, exponent and mantissa
///////////////////////////////////////////////////////////////////////////////

txfma_f0 txfma0_f0
(
  .opa_frf_clk                    ( opa_frf_clk             ),
  .opb_frf_clk                    ( opb_frf_clk             ),
  .opc_frf_clk                    ( opc_frf_clk             ),
  .trans_frf_clk                  ( trans_frf_clk           ),
  .ctrl_frf_clk                   ( ctrl_frf_clk            ),
  .opa_byp_frfa_h                 ( opa_byp_frfa_h          ),
  .opb_byp_frfa_h                 ( opb_byp_frfa_h          ),
  .opc_byp_frfa_h                 ( opc_byp_frfa_h          ),
  .xrom_c1c0k_ra_h                ( xrom_c1c0k_ra_h         ),
  .xrom_c2_ra_h                   ( xrom_c2_ra_h            ),
  .trans_mask_ra_h                ( trans_mask_ra_h         ),
  .eb_hi_k_exa_h                  ( eb_hi_k_exa_h           ),
  .sel_eblo_ph_exa_h              ( sel_eblo_ph_exa_h       ),
  .sel_ec5bdet_exa_h              ( sel_ec5bdet_exa_h       ),
  .sel_ebin_hi_exa_h              ( sel_ebin_hi_exa_h       ),
  .elo_eb_en_exa_h                ( elo_eb_en_exa_h         ),
  .ehi_eb_en_exa_h                ( ehi_eb_en_exa_h         ),
  .elo_ea_en_exa_h                ( elo_ea_en_exa_h         ),
  .fa_en_exa_h                    ( fa_en_exa_h             ),
  .fb_en_exa_h                    ( fb_en_exa_h             ),
  .fc_en_exa_h                    ( fc_en_exa_h             ),
  .sel_ealo_ph_exa_h              ( sel_ealo_ph_exa_h       ),
  .sel_eainmux_exa_h              ( sel_eainmux_exa_h       ),
  .sel_ec5b_exa_h                 ( sel_ec5b_exa_h          ),
  .sel_ecin_exa_h                 ( sel_ecin_exa_h          ),
  .sa_hi_en_exa_h                 ( sa_hi_en_exa_h          ),
  .sa_lo_en_exa_h                 ( sa_lo_en_exa_h          ),
  .sb_hi_en_exa_h                 ( sb_hi_en_exa_h          ),
  .sb_lo_en_exa_h                 ( sb_lo_en_exa_h          ),
  .sc_en_exa_h                    ( sc_en_exa_h             ),
  .ehi_ea_en_exa_h                ( ehi_ea_en_exa_h         ),
  .ec_en_exa_h                    ( ec_en_exa_h             ),
  .neg_fmaop_exa_h                ( neg_fmaop_exa_h         ),
  .trans_op2_exa_l                ( trans_op2_exa_l         ),
  .sa1_nr2_frcpfxp_exa_h          ( sa1_nr2_frcpfxp_exa_h   ),
  .sel_sc_exa_h                   ( sel_sc_exa_h            ),
  .sel_sign_sinp2_exa_h           ( sel_sign_sinp2_exa_h    ),
  .sa_zap_exa_l                   ( sa_zap_exa_l            ),
  .sb_zap_exa_l                   ( sb_zap_exa_l            ),
  .sel_f10_exa_h                  ( sel_f10_exa_h           ),
  .sel_f11_exa_h                  ( sel_f11_exa_h           ),
  .sel_opcgfx_exa_h               ( sel_opcgfx_exa_h        ),
  .fc_dtps_exa_h                  ( fc_dtps_exa_h           ),
  .sel_xopc_exa_h                 ( sel_xopc_exa_h          ),
  .sel_opcfmt_exa_h               ( sel_opcfmt_exa_h        ),
  .sel_snorm_exa_h                ( sel_snorm_exa_h         ),
  .sel_g1sk2_exa_h                ( sel_g1sk2_exa_h         ),
  .sel_g1sk8_exa_h                ( sel_g1sk8_exa_h         ),
  .sel_g1sk16_exa_h               ( sel_g1sk16_exa_h        ),
  .sel_g1sk24_exa_h               ( sel_g1sk24_exa_h        ),
  .sel_xexpfrac_exa_h             ( sel_xexpfrac_exa_h      ),
  .sel_transfa_exa_h              ( sel_transfa_exa_h       ),
  .sel_transfb_exa_h              ( sel_transfb_exa_h       ),
  .sel_opafmt_exa_h               ( sel_opafmt_exa_h        ),
  .sel_opbfmt_exa_h               ( sel_opbfmt_exa_h        ),
  .sa_hi_f0a_h                    ( sa_hi_f0a_h             ),
  .sa_lo_f0a_h                    ( sa_lo_f0a_h             ),
  .sb_hi_f0a_h                    ( sb_hi_f0a_h             ),
  .sb_lo_f0a_h                    ( sb_lo_f0a_h             ),
  .sc_f0a_h                       ( sc_f0a_h                ),
  .ea_hi_f0a_h                    ( ea_hi_f0a_h             ),
  .ea_lo_f0a_h                    ( ea_lo_f0a_h             ),
  .eb_hi_f0a_h                    ( eb_hi_f0a_h             ),
  .eb_lo_f0a_h                    ( eb_lo_f0a_h             ),
  .ec_f0a_h                       ( ec_f0a_h                ),
  .fa_f0a_h                       ( fa_f0a_h                ),
  .fb_f0a_h                       ( fb_f0a_h                ),
  .fc_f0a_h                       ( fc_f0a_h                ),
  .ea_hi_zero_f0a_l               ( ea_hi_zero_f0a_l        ),
  .ea_hi_all1_f0a_h               ( ea_hi_all1_f0a_h        ),
  .ea_lo_zero_f0a_l               ( ea_lo_zero_f0a_l        ),
  .ea_lo_all1_f0a_h               ( ea_lo_all1_f0a_h        ),
  .eb_hi_zero_f0a_l               ( eb_hi_zero_f0a_l        ),
  .eb_hi_all1_f0a_h               ( eb_hi_all1_f0a_h        ),
  .eb_lo_zero_f0a_l               ( eb_lo_zero_f0a_l        ),
  .eb_lo_all1_f0a_h               ( eb_lo_all1_f0a_h        ),
  .ec_zero_f0a_l                  ( ec_zero_f0a_l           ),
  .ec_all1_f0a_h                  ( ec_all1_f0a_h           ),
  .eb_eql_126_f0a_h               ( eb_eql_126_f0a_h        ),
  .eb_eql_127_f0a_h               ( eb_eql_127_f0a_h        ),
  .eb_eql_125_f0a_h               ( eb_eql_125_f0a_h        ),
  .fcf10_zero_f0a_l               ( fcf10_zero_f0a_l        ),
  .fcf11_zero_f0a_l               ( fcf11_zero_f0a_l        ),
  .fcps_zero_f0a_h                ( fcps_zero_f0a_h         ),
  .fcph_zero_f0a_l                ( fcph_zero_f0a_l         ),
  .fcnorm_zero_29_8_f0a_h         ( fcnorm_zero_29_8_f0a_h  ),
  .fcnorm16_eq_1_f0a_h            ( fcnorm16_eq_1_f0a_h     ),
  .fcnorm8_eq_1_f0a_h             ( fcnorm8_eq_1_f0a_h      ),
  .fcnorm_30_f0a_h                ( fcnorm_30_f0a_h         ),
  .faph_hi_b0_f0a_h               ( faph_hi_b0_f0a_h        ),
  .fbph_hi_b0_f0a_h               ( fbph_hi_b0_f0a_h        ),
  .fcph_b0_f0a_h                  ( fcph_b0_f0a_h           ),
  .fcf11_b0_f0a_h                 ( fcf11_b0_f0a_h          ),
  .fcf10_b0_f0a_h                 ( fcf10_b0_f0a_h          ),
  .fcn8_one_30_24_f0a_h           ( fcn8_one_30_24_f0a_h    ),
  .fcn10_one_30_22_f0a_h          ( fcn10_one_30_22_f0a_h   ),
  .fcn16_one_30_16_f0a_h          ( fcn16_one_30_16_f0a_h   ),
  .fcn24_one_30_8_f0a_h           ( fcn24_one_30_8_f0a_h    ),
  .faps_zero_f0a_l                ( faps_zero_f0a_l         ),
  .fbps_zero_f0a_l                ( fbps_zero_f0a_l         ),
  .faph_hi_zero_f0a_l             ( faph_hi_zero_f0a_l      ),
  .fbph_hi_zero_f0a_l             ( fbph_hi_zero_f0a_l      ),
  .faph_lo_zero_f0a_l             ( faph_lo_zero_f0a_l      ),
  .fbph_lo_zero_f0a_l             ( fbph_lo_zero_f0a_l      ),
  .faph_lo_b0_f0a_h               ( faph_lo_b0_f0a_h        ),
  .fbph_lo_b0_f0a_h               ( fbph_lo_b0_f0a_h        ),
  .faps_b0_f0a_h                  ( faps_b0_f0a_h           ),
  .fbps_b0_f0a_h                  ( fbps_b0_f0a_h           ),
  .fc_31_f0a_h                    ( fc_31_f0a_h             ),
  .fcps_b0_f0a_h                  ( fcps_b0_f0a_h           )
);

///////////////////////////////////////////////////////////////////////////////
// F1 component
///////////////////////////////////////////////////////////////////////////////

txfma_f1 txfma0_f1
(
  .ctrl_f0_clk              ( ctrl_f0_clk         ),
  .multype_f0_clk            ( multype_f0_clk       ),
  .ctrl_f1_clk              ( ctrl_f1_clk         ),
  .multype_f1_clk            ( multype_f1_clk       ),
  .fa_f0a_h                 ( fa_f0a_h            ),
  .fb_f0a_h                 ( fb_f0a_h            ),
  .fc_f0a_h                 ( fc_f0a_h            ),
  .dtps_f0a_h               ( dtps_f0a_h          ),
  .fa_f2a_h                 ( fa_f2a_h            ),
  .fc_f2a_h                 ( fc_f2a_h            ),
  .fc_b0_f2a_h              ( fc_b0_f2a_h         ),
  .mul_sign_f0a_h           ( mul_sign_f0a_h      ),
  .mul_hp_mode_f0a_h        ( mul_hp_mode_f0a_h   ),
  .wallace_fC21_f2a_h       ( wallace_fC21_f2a_h  ),
  .wallace_fC22_f2a_h       ( wallace_fC22_f2a_h  ),
  .wallace_fS21_f2a_h       ( wallace_fS21_f2a_h  ),
  .wallace_fS22_f2a_h       ( wallace_fS22_f2a_h  ),
  .booth_fCin_f2a_h         ( booth_fCin_f2a_h    ),
  .booth_fSxt_f2a_h         ( booth_fSxt_f2a_h    ),
  .booth_fpp17_f2a_h        ( booth_fpp17_f2a_h   ),
  .etrz_fa_hi_ph_f1a_h      ( etrz_fa_hi_ph_f1a_h ),
  .etrz_fa_lo_ps_f1a_h      ( etrz_fa_lo_ps_f1a_h ),
  .etrz_fb_hi_ph_f1a_h      ( etrz_fb_hi_ph_f1a_h ),
  .etrz_fb_lo_ps_f1a_h      ( etrz_fb_lo_ps_f1a_h ),
  .etrz_fc_f1a_h            ( etrz_fc_f1a_h       )
);

///////////////////////////////////////////////////////////////////////////////
// F2 component
///////////////////////////////////////////////////////////////////////////////

txfma_f2 txfma0_f2
(
  .ctrl_f1_clk               ( ctrl_f1_clk            ),
  .ctrl_f2_clk               ( ctrl_f2_clk            ),
  .fa_f2a_h                  ( fa_f2a_h               ),
  .fc_f2a_h                  ( fc_f2a_h               ),
  .paddin1_sels_f1a_h        ( paddin1_sels_f1a_h     ),
  .paddin2_selc_f1a_h        ( paddin2_selc_f1a_h     ),
  .paddin2_selfcneg_f1a_h    ( paddin2_selfcneg_f1a_h ),
  .padd_cin_f1a_h            ( padd_cin_f1a_h         ),
  .sel_famask_f1a_h          ( sel_famask_f1a_h       ),
  .booth_fCin_f2a_h          ( booth_fCin_f2a_h       ),
  .booth_fSxt_f2a_h          ( booth_fSxt_f2a_h       ),
  .booth_fpp17_f2a_h         ( booth_fpp17_f2a_h      ),
  .wallace_fC21_f2a_h        ( wallace_fC21_f2a_h     ),
  .wallace_fC22_f2a_h        ( wallace_fC22_f2a_h     ),
  .wallace_fS21_f2a_h        ( wallace_fS21_f2a_h     ),
  .wallace_fS22_f2a_h        ( wallace_fS22_f2a_h     ),
  .prodsum_f3a_h             ( prodsum_f3a_h          ),
  .fa_f3a_h                  ( fa_f3a_h               ),
  .fc_f3a_h                  ( fc_f3a_h               )
);

///////////////////////////////////////////////////////////////////////////////
// F3 component
///////////////////////////////////////////////////////////////////////////////

txfma_f3 txfma0_f3
(
  .ctrl_f2_clk                  ( ctrl_f2_clk                  ),
  .ctrl_f3_clk                  ( ctrl_f3_clk                  ),
  .fa_f3a_h                     ( fa_f3a_h                     ),
  .fc_f3a_h                     ( fc_f3a_h                     ),
  .prodsum_f3a_h                ( prodsum_f3a_h                ),
  .exp_res_f2a_h                ( exp_res_f2a_h                ),
  .ediff_abs_diff_hi_f2a_h      ( ediff_abs_diff_hi_f2a_h      ),
  .align2_rshamt_c3_f2a_h       ( align2_rshamt_c3_f2a_h       ),
  .op3p3spc_sel_f2a_h           ( op3p3spc_sel_f2a_h           ),
  .op3p1c_sel_p_f2a_h           ( op3p1c_sel_p_f2a_h           ),
  .op3p2c_sel_f2a_h             ( op3p2c_sel_f2a_h             ),
  .op3p1c_sel_r_f2a_h           ( op3p1c_sel_r_f2a_h           ),
  .op3g_sel_f2a_h               ( op3g_sel_f2a_h               ),
  .op2_sxtto_g_f2a_h            ( op2_sxtto_g_f2a_h            ),
  .imxc_sel_f3a_h               ( imxc_sel_f3a_h               ),
  .fround_rbit_f3a_h            ( fround_rbit_f3a_h            ),
  .ugfxrplc_sel_f2a_h           ( ugfxrplc_sel_f2a_h           ),
  .sgfxrplc_sel_f2a_h           ( sgfxrplc_sel_f2a_h           ),
  .icgfxc_sel_f2a_h             ( icgfxc_sel_f2a_h             ),
  .al1p1c_sel_f2a_h             ( al1p1c_sel_f2a_h             ),
  .al1p2c_sel_f2a_h             ( al1p2c_sel_f2a_h             ),
  .rm_rne_f2a_h                 ( rm_rne_f2a_h                 ),
  .rm_rmm_f2a_h                 ( rm_rmm_f2a_h                 ),
  .frnd_rupnsc_f2a_h            ( frnd_rupnsc_f2a_h            ),
  .frnd_rdnsc_f2a_h             ( frnd_rdnsc_f2a_h             ),
  .frnd_t1_f2a_h                ( frnd_t1_f2a_h                ),
  .frnd_t2_f2a_h                ( frnd_t2_f2a_h                ),
  .al2c3_neg_cin_ps_f3a_h       ( al2c3_neg_cin_ps_f3a_h       ),
  .al1c_zap_f2a_h               ( al1c_zap_f2a_h               ),
  .op1c_log_mul_clr_f2a_h       ( op1c_log_mul_clr_f2a_h       ),
  .negop1c_int_logmul_clr_f2a_h ( negop1c_int_logmul_clr_f2a_h ),
  .psc_op1_clr_f2a_h            ( psc_op1_clr_f2a_h            ),
  .op2_sxtto_g_f3a_h            ( op2_sxtto_g_f3a_h            ),
  .al1znp2c_sel_f2a_h           ( al1znp2c_sel_f2a_h           ),
  .al2c_sel_f2a_h               ( al2c_sel_f2a_h               ),
  .align2_zap_f2a_h             ( align2_zap_f2a_h             ),
  .align2c_neg_f2a_h            ( align2c_neg_f2a_h            ),
  .comb_s_f4a_h                 ( comb_s_f4a_h                 ),
  .comb_c_f4a_h                 ( comb_c_f4a_h                 ),
  .int_minmax_res_f4a_h         ( int_minmax_res_f4a_h         ),
  .prodsum_a2_f3a_h             ( prodsum_a2_f3a_h             )
);

///////////////////////////////////////////////////////////////////////////////
// F4 component
///////////////////////////////////////////////////////////////////////////////

txfma_f4 txfma0_f4
(
  .ctrl_f3_clk                      ( ctrl_f3_clk             ),
  .ctrl_f4_clk                      ( ctrl_f4_clk             ),
  .comb_s_f4a_h                     ( comb_s_f4a_h            ),
  .comb_c_f4a_h                     ( comb_c_f4a_h            ),
  .op2_sxtto_g_f3a_h                ( op2_sxtto_g_f3a_h       ),
  .int_minmax_res_f4a_h             ( int_minmax_res_f4a_h    ),
  .neg_norm_a3_f4a_h                ( neg_norm_a3_f4a_h       ),
  .neg_norm_na3_f4a_h               ( neg_norm_na3_f4a_h      ),
  .clearphps_en_f4a_h               ( clearphps_en_f4a_h      ),
  .clear16frnd_nshin_f4a_l          ( clear16frnd_nshin_f4a_l ),
  .ph_ps_clear_f4a_l                ( ph_ps_clear_f4a_l       ),
  .al1c3_neg_cin_f3a_h              ( al1c3_neg_cin_f3a_h     ),
  .fma_subopc3_f3a_h                ( fma_subopc3_f3a_h       ),
  .neg_norm_f5a_h                   ( neg_norm_f5a_h          ),
  .int_minmax_res_f5a_h             ( int_minmax_res_f5a_h    ),
  .comb_add_f5a_h                   ( comb_add_f5a_h          ),
  .elxd_f4a_h                       ( elxd_f4a_h              ),
  .etrz50_f4a_h                     ( etrz50_f4a_h            )
);

///////////////////////////////////////////////////////////////////////////////
// F5 component
///////////////////////////////////////////////////////////////////////////////

txfma_f5 txfma0_f5
(
  .ctrl_f4_clk                ( ctrl_f4_clk                ),
  .ctrl_f5_clk                ( ctrl_f5_clk                ),
  .comb_add_f5a_h             ( comb_add_f5a_h             ),
  .int_minmax_res_f5a_h       ( int_minmax_res_f5a_h       ),
  .enc_nsamt_f4a_h            ( enc_nsamt_f4a_h            ),
  .neg_norm_f5a_h             ( neg_norm_f5a_h             ),
  .en_logrrsel1b_f5a_l        ( en_logrrsel1b_f5a_l        ),
  .mmx_sel_immres_f5a_h       ( mmx_sel_immres_f5a_h       ),
  .nshcen_sel1b_f5a_h         ( nshcen_sel1b_f5a_h         ),
  .rc_ph_f5a_h                ( rc_ph_f5a_h                ),
  .rc_ps_f5a_h                ( rc_ps_f5a_h                ),
  .rc_int32_f5a_h             ( rc_int32_f5a_h             ),
  .int32_stky2r_f5a_h         ( int32_stky2r_f5a_h         ),
  .stky2g_f5a_h               ( stky2g_f5a_h               ),
  .rc_3rmxr_f5a_h             ( rc_3rmxr_f5a_h             ),
  .rc_rl01_f5a_h              ( rc_rl01_f5a_h              ),
  .rc_rmm_f5a_h               ( rc_rmm_f5a_h               ),
  .rc_rup_f5a_h               ( rc_rup_f5a_h               ),
  .rc_rne_f5a_h               ( rc_rne_f5a_h               ),
  .rc_rdn_f5a_h               ( rc_rdn_f5a_h               ),
  .rc_res_sign_f5a_l          ( rc_res_sign_f5a_l          ),
  .rc_transrnd_f5a_h          ( rc_transrnd_f5a_h          ),
  .rc_transrnd_log_fma2_f5a_h ( rc_transrnd_log_fma2_f5a_h ),
  .ph_stky2l_f5a_h            ( ph_stky2l_f5a_h            ),
  .ps_stky2l_f5a_h            ( ps_stky2l_f5a_h            ),
  .int32_stky2l_f5a_h         ( int32_stky2l_f5a_h         ),
  .nshc_f5a_h                 ( nshc_f5a_h                 ),
  .frac_zero_f6a_h            ( frac_zero_f6a_h            ),
  .rnd_l_ph_f6a_h             ( rnd_l_ph_f6a_h             ),
  .rnd_r_ph_f6a_h             ( rnd_r_ph_f6a_h             ),
  .rnd_l_ps_f6a_h             ( rnd_l_ps_f6a_h             ),
  .rnd_r_ps_f6a_h             ( rnd_r_ps_f6a_h             ),
  .rnd_l_int_f6a_h            ( rnd_l_int_f6a_h            ),
  .rnd_r_int_f6a_h            ( rnd_r_int_f6a_h            ),
  .ph_allone2r_f5a_h          ( ph_allone2r_f5a_h          ),
  .ph_allone2l_f5a_h          ( ph_allone2l_f5a_h          ),
  .ps_allone2r_f5a_h          ( ps_allone2r_f5a_h          ),
  .ps_allone2l_f5a_h          ( ps_allone2l_f5a_h          ),
  .ftz_f6a_l                  ( ftz_f6a_l                  ),
  .comb_add_a3_f5a_h          ( comb_add_a3_f5a_h          ),
  .mergd_frac_res_hi_f6a_h    ( mergd_frac_res_hi_f6a_h    )
);

///////////////////////////////////////////////////////////////////////////////
// F6 component
///////////////////////////////////////////////////////////////////////////////

txfma_f6 txfma0_f6
(
  .ctrl_f5_clk                  ( ctrl_f5_clk               ),
  .mergd_frac_res_hi_f6a_h      ( mergd_frac_res_hi_f6a_h   ),
  .rnd_l_ph_f6a_h               ( rnd_l_ph_f6a_h            ),
  .rnd_r_ph_f6a_h               ( rnd_r_ph_f6a_h            ),
  .rnd_l_ps_f6a_h               ( rnd_l_ps_f6a_h            ),
  .rnd_r_ps_f6a_h               ( rnd_r_ps_f6a_h            ),
  .rnd_r_int_f6a_h              ( rnd_r_int_f6a_h           ),
  .rnd_l_int_f6a_h              ( rnd_l_int_f6a_h           ),
  .ftz_f6a_l                    ( ftz_f6a_l                 ),
  .classify_f6a_h               ( classify_f6a_h            ),
  .sel_classify_out_f6a_h       ( sel_classify_out_f6a_h    ),
  .rc6_ph_f5a_h                 ( rc6_ph_f5a_h              ),
  .rc6_ps_f5a_h                 ( rc6_ps_f5a_h              ),
  .rc_int32_f5a_h               ( rc_int32_f5a_h            ),
  .sel_nr1_frcpres_f6a_h        ( sel_nr1_frcpres_f6a_h     ),
  .trans2_ps_roundup_r_f5a_h    ( trans2_ps_roundup_r_f5a_h ),
  .normmask1_f6a_h              ( normmask1_f6a_h           ),
  .normmask2_f6a_h              ( normmask2_f6a_h           ),
  .trans_sinp2m_f6a_h           ( trans_sinp2m_f6a_h        ),
  .s_res_f6a_h                  ( s_res_f6a_h               ),
  .exp_res_f6a_h                ( exp_res_f6a_h             ),
  .rrmux32_sel_f6a_h            ( rrmux32_sel_f6a_h         ),
  .rrmux16l_sel_f6a_l           ( rrmux16l_sel_f6a_l        ),
  .ps_rndovf_f6a_h              ( ps_rndovf_f6a_h           ),
  .ph_rndovf_f6a_h              ( ph_rndovf_f6a_h           ),
  .sel_set_bit0_one_f6a_h       ( sel_set_bit0_one_f6a_h    ),
  .effsub1_set_sign1_f6a_h      ( effsub1_set_sign1_f6a_h   ),
  .f10f11mux_sel_f6a_h          ( f10f11mux_sel_f6a_h       ),
  .sel_psexc_sr_f6a_h           ( sel_psexc_sr_f6a_h        ),
  .sel_bit31_one_f6a_h          ( sel_bit31_one_f6a_h       ),
  .sel_b30to0_ones_f6a_h        ( sel_b30to0_ones_f6a_h     ),
  .sel_ps_unf_f6a_h             ( sel_ps_unf_f6a_h          ),
  .sel_zerores_f6a_h            ( sel_zerores_f6a_h         ),
  .sel_ps_normone_f6a_h         ( sel_ps_normone_f6a_h      ),
  .sel_ps_inf_f6a_h             ( sel_ps_inf_f6a_h          ),
  .sel_ps_cqn_f6a_h             ( sel_ps_cqn_f6a_h          ),
  .sel_ps_large_f6a_h           ( sel_ps_large_f6a_h        ),
  .sel_phsp_f6a_h               ( sel_phsp_f6a_h            ),
  .sel_phhi_sr_f6a_h            ( sel_phhi_sr_f6a_h         ),
  .sel_phhi_large_f6a_h         ( sel_phhi_large_f6a_h      ),
  .sel_phhi_unf_f6a_h           ( sel_phhi_unf_f6a_h        ),
  .sel_phhi_infres_f6a_h        ( sel_phhi_infres_f6a_h     ),
  .sel_f11inf_f6a_h             ( sel_f11inf_f6a_h          ),
  .sel_f10cqnan_f6a_h           ( sel_f10cqnan_f6a_h        ),
  .sel_f11sp_f6a_h              ( sel_f11sp_f6a_h           ),
  .sel_f10sp_f6a_h              ( sel_f10sp_f6a_h           ),
  .sel_f11large_f6a_h           ( sel_f11large_f6a_h        ),
  .sel_f10large_f6a_h           ( sel_f10large_f6a_h        ),
  .sel_gmaxu24_f6a_h            ( sel_gmaxu24_f6a_h         ),
  .sel_gmaxu16_f6a_h            ( sel_gmaxu16_f6a_h         ),
  .sel_gmaxs16_f6a_h            ( sel_gmaxs16_f6a_h         ),
  .sel_gmins16_f6a_h            ( sel_gmins16_f6a_h         ),
  .sel_gmaxs8_f6a_h             ( sel_gmaxs8_f6a_h          ),
  .sel_gmaxu2_f6a_h             ( sel_gmaxu2_f6a_h          ),
  .sel_gmins8_f6a_h             ( sel_gmins8_f6a_h          ),
  .sel_gmaxu8_f6a_h             ( sel_gmaxu8_f6a_h          ),
  .sel_gfxmax_f6a_h             ( sel_gfxmax_f6a_h          ),
  .sel_excreshi_f6a_h           ( sel_excreshi_f6a_h        ),
  .sel_excreslo_f6a_h           ( sel_excreslo_f6a_h        ),
  .sel_fmares_hi_f6a_h          ( sel_fmares_hi_f6a_h       ),
  .sel_fmares_lo_f6a_h          ( sel_fmares_lo_f6a_h       ),
  .int_res_l_bit_f6a_h          ( int_res_l_bit_f6a_h       ),
  .fma_res_f6a_h                ( fma_res_f6a_h             )
);

endmodule
/* verilator lint_on UNUSEDSIGNAL */
