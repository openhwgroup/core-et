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

/* verilator lint_off CASEINCOMPLETE */  // Imported original TXFMA control decode intentionally leaves impossible mask/opcode combinations uncovered.
/* verilator lint_off UNUSEDSIGNAL */  // Packed classify/control buses keep their original shape even when not every bit is consumed here.
module txfma_c0
(
  input                                 clock,
  input                                 reset,
  input                                 in_valid_exa_h,
  input minion_pkg::vpu_ctrl_sigs_t     sigs_in_exa_h,
  input [TXFMA_RM_SZ-1:0]              rm_in_exa_h,
  input [TRANS_MASK_SIZE-1:0]          trans_mask_exa_h,
  input                                 trans_taylor_exa_h,
  input                                 trans_exp_fma2_exc_exa_h,
  input                                 use_prev_sigs_exa_h,
  input                                 sa_hi_f0a_h,
  input                                 sa_lo_f0a_h,
  input                                 sb_hi_f0a_h,
  input                                 sb_lo_f0a_h,
  input                                 sc_f0a_h,
  input                                 ea_hi_zero_f0a_l,
  input                                 ea_hi_all1_f0a_h,
  input                                 ea_lo_zero_f0a_l,
  input                                 ea_lo_all1_f0a_h,
  input                                 eb_hi_zero_f0a_l,
  input                                 eb_hi_all1_f0a_h,
  input                                 eb_lo_zero_f0a_l,
  input                                 eb_lo_all1_f0a_h,
  input                                 ec_zero_f0a_l,
  input                                 ec_all1_f0a_h,
  input                                 eb_eql_127_f0a_h,
  input                                 eb_eql_126_f0a_h,
  input                                 eb_eql_125_f0a_h,
  input                                 fcf10_zero_f0a_l,
  input                                 fcf11_zero_f0a_l,
  input                                 fcps_zero_f0a_h,
  input                                 fcph_zero_f0a_l,
  input                                 fcnorm_zero_29_8_f0a_h,
  input                                 fcnorm16_eq_1_f0a_h,
  input                                 fcnorm8_eq_1_f0a_h,
  input                                 fcn8_one_30_24_f0a_h,
  input                                 fcn10_one_30_22_f0a_h,
  input                                 fcn16_one_30_16_f0a_h,
  input                                 fcn24_one_30_8_f0a_h,
  input                                 faps_zero_f0a_l,
  input                                 fbps_zero_f0a_l,
  input                                 faph_hi_zero_f0a_l,
  input                                 fbph_hi_zero_f0a_l,
  input                                 faph_lo_zero_f0a_l,
  input                                 fbph_lo_zero_f0a_l,
  input                                 faph_lo_b0_f0a_h,
  input                                 fbph_lo_b0_f0a_h,
  input                                 faps_b0_f0a_h,
  input                                 fbps_b0_f0a_h,
  input                                 fc_31_f0a_h,
  input                                 fcnorm_30_f0a_h,
  input                                 faph_hi_b0_f0a_h,
  input                                 fbph_hi_b0_f0a_h,
  input                                 fcph_b0_f0a_h,
  input                                 fcf11_b0_f0a_h,
  input                                 fcf10_b0_f0a_h,
  input                                 fcps_b0_f0a_h,
  output logic                          trans_exp_fma2_exc_f0a_h,
  output logic                          opa_valid_rfa_h,
  output logic                          opb_valid_rfa_h,
  output logic                          opc_valid_rfa_h,
  output logic                          ctrl_valid_f0a_h,
  output logic                          is_trans_rfa_h,
  output logic [5:0]                    eb_hi_k_exa_h,
  output logic                          fa_en_exa_h,
  output logic                          fb_en_exa_h,
  output logic                          fc_en_exa_h,
  output logic                          sa_hi_en_exa_h,
  output logic                          sa_lo_en_exa_h,
  output logic                          sb_hi_en_exa_h,
  output logic                          sb_lo_en_exa_h,
  output logic                          ea_hi_zero_clr_f0a_h,
  output logic                          eb_hi_zero_clr_f0a_h,
  output logic                          ea_lo_zero_clr_f0a_h,
  output logic                          eb_lo_zero_clr_f0a_h,
// Exponent Selects/Enabled to TXFMA F0
  output [1:0]                          sel_eainmux_exa_h,
  output logic                          sel_ealo_ph_exa_h,
  output logic                          ehi_ea_en_exa_h,
  output logic                          elo_ea_en_exa_h,
  output logic [1:0]                    sel_ebin_hi_exa_h,
  output logic                          sel_eblo_ph_exa_h,
  output logic                          ehi_eb_en_exa_h,
  output logic                          elo_eb_en_exa_h,
  output logic [1:0]                    sel_ec5b_exa_h,
  output logic [1:0]                    sel_ecin_exa_h,
  output logic                          ec_en_exa_h,
  output logic				            sel_ec5bdet_exa_h,
  output logic                          sc_en_exa_h,
  output logic                          sa1_nr2_frcpfxp_exa_h,
  output logic [1:0]                    sel_sc_exa_h,
  output logic                          sel_sign_sinp2_exa_h,
  output logic                          sa_zap_exa_l,
  output logic                          sb_zap_exa_l,
  output logic                          sel_f10_exa_h,
  output logic                          sel_f11_exa_h,
  output logic [1:0]                    sel_opcgfx_exa_h,
  output logic                          fc_dtps_exa_h,
  output logic                          neg_fmaop_exa_h,
  output logic                          trans_op2_exa_l,
  output logic [1:0]                    sel_xopc_exa_h,
  output logic [1:0]                    sel_opcfmt_exa_h,
  output logic                          sel_snorm_exa_h,
  output logic                          sel_g1sk2_exa_h,
  output logic                          sel_g1sk8_exa_h,
  output logic                          sel_g1sk16_exa_h,
  output logic                          sel_g1sk24_exa_h,
  output logic                          sel_xexpfrac_exa_h,
  output logic [1:0]                    sel_transfa_exa_h,
  output logic [1:0]                    sel_transfb_exa_h,
  output logic [1:0]                    sel_opafmt_exa_h,
  output logic [1:0]                    sel_opbfmt_exa_h,
  output logic                          mul_sign_f0a_h,
  output logic                          mul_hp_mode_f0a_h,
  output logic                         	res_inf_f0a_h,
  output logic                         	res_snan_f0a_h,
  output logic                         	res_qnan_f0a_h,
  output logic [FP_CLASS_BITS-1:0]    	classify_f0a_h,
  output logic                         	prod_inf_hi_f0a_h,
  output logic                         	c_inf_f0a_h,
  output logic                         	a_inf_f0a_h,
  output logic [VPU_FLAGS_SZ-1:0]     	flags_f0a_h,
  output logic                         	a_is_qnan_hi_f0a_h,
  output logic                         	c_is_qnan_f0a_h,
  output logic                         	a_is_snan_hi_f0a_h,
  output logic                         	c_is_snan_f0a_h,
  output logic                         	res_pos_int_nan_f0a_h,
  output logic                         	res_neg_int_nan_f0a_h,
  output logic                         	fc_zero30_8_f0a_h,
  output logic                         	res_uint_nan_f0a_h,
  output logic                         	res_zero_f0a_h,
  output logic                         	res_one_f0a_h,
  output logic                         	prod_inf_lo_f0a_h,
  output logic                         	fa_hi_zero_f0a_h,
  output logic                         	fb_hi_zero_f0a_h,
  output logic                         	res_unorm_one_f0a_h,
  output logic                         	res_snorm_one_f0a_h,
  output logic                          flags_en_f0a_h,
  output logic                          dtps_f0a_h,
  output logic                          spec_res_en_f0a_h,
  output logic                          use_prev_sigs_f0a_h,
  output logic [TXFMA_RM_SZ-1:0]       rm_f0a_h,
  output logic [VPU_FCMD_SZ-1:0]       cmd_f0a_h,
  output logic [TXFMA_SIGS_SZ-1:0]     sigs_f0a_h,
  output logic [VPU_DTYPE_SZ-1:0]      op_dtype_f0a_h,
  output logic [2:0]                    sin_res_shf_idx_f0a_h,
  output logic                          flog_mul_prod_zero_f0a_h,
  output logic                          ec_zero_clr_f0a_h,
  output logic                          use_mul_f0a_h
);

///////////////////////////////////////////////////////////////////////////////
// Local signals
///////////////////////////////////////////////////////////////////////////////

logic                           opa_trans_valid_rfa_h;
logic                           opb_trans_valid_rfa_h;
logic                           opc_trans_valid_rfa_h;
logic                           cvt_norm_trans_valid_rfa_h;
minion_pkg::vpu_ctrl_sigs_t     sigs_qual_exa_h;
logic [TXFMA_SIGS_SZ-1:0]      sigs_exa_h;
logic [TXFMA_RM_SZ-1:0]        rm_exa_h;
logic [VPU_FCMD_SZ-1:0]        cmd_exa_h;
logic [VPU_DTYPE_SZ-1:0]       op_dtype_exa_h;
logic [2:0]                     sin_res_shf_idx_exa_h;
logic                           zap_sc_exa_h;
logic                           trans_mul_sign_exa_h;
logic                           mul_sign_exa_h;
logic                           mul_hp_mode_exa_h;
logic                           cvt_ps2gfx_exa_h;
logic                           res_pot_inf_f0a_h;
logic                           res_pot_qnan_f0a_h;
logic                           a_zero_hi_f0a_h;
logic                           b_zero_hi_f0a_h;
logic                           c_zero_hi_f0a_h;
logic [FP_CLASS_BITS-1:0]      classify_a_hi_f0a_h;
logic [FP_CLASS_BITS-1:0]      classify_a_lo_f0a_h;
logic [FP_CLASS_BITS-1:0]      classify_b_hi_f0a_h;
logic [FP_CLASS_BITS-1:0]      classify_b_lo_f0a_h;
logic [FP_CLASS_BITS-1:0]    	classify_c_f0a_h;
logic                           inf_nan1_hi_f0a_h;
logic                           inf_nan2_hi_f0a_h;
logic                           inf_nan1_lo_f0a_h;
logic                           res_inf_trans_f0a_h;
logic                           fa_zero30_21_f0a_h;
logic                           fa_zero30_8_f0a_h;
logic                           fb_zero28_6_f0a_h;
logic                           fb_zero28_19_f0a_h;
logic                           fc_zero30_21_f0a_h;
logic                           fcnorm_zero_30_8_h;
logic                           sign_prod_hi_f0a_h;
logic                           sign_prod_lo_f0a_h;
logic                           s_prod_hi_prod_lo_f0a_h;
logic                           s_prod_hi_c_hi_f0a_h;
logic                           s_prod_lo_c_hi_f0a_h;
logic                           trans_sin_taylor_p2_op_exa_h;
logic                           trans_sin_rom_p3_op_exa_h;
logic                           trans_op1_exa_h;
logic                           flags_denorm_f0a_h;
logic                           ctrl_valid_exa_h;
logic                           rsqrt_res_nan_inv_f0a_h;
logic                           sin_res_nan_inv_f0a_h;
logic                           log_res_nan_inv_f0a_h;
logic                           c0_ctrl_en_exa_h;
logic                           inv_ops_f0a_h;
logic                           use_mul_rfa_h;
logic                           spec_res_en_nan_f0a_h;
logic                           spec_res_en_norm_f0a_h;
logic                           use_mul_op_f0a_h;


///////////////////////////////////////////////////////////////////////////////
// Local Flops - Needed to guarantee timing
///////////////////////////////////////////////////////////////////////////////

assign c0_ctrl_en_exa_h = in_valid_exa_h & !use_prev_sigs_exa_h;

always_ff @(posedge clock) begin
  if (reset) begin
    ctrl_valid_f0a_h <= 1'b0;
  end else begin
    ctrl_valid_f0a_h <= ctrl_valid_exa_h;
  end

  if (in_valid_exa_h) begin
    use_prev_sigs_f0a_h <= use_prev_sigs_exa_h;
  end

  if (c0_ctrl_en_exa_h) begin
    use_mul_op_f0a_h <= use_mul_rfa_h;
    sin_res_shf_idx_f0a_h <= sin_res_shf_idx_exa_h;
    mul_sign_f0a_h <= mul_sign_exa_h;
    mul_hp_mode_f0a_h <= mul_hp_mode_exa_h;
    rm_f0a_h <= rm_exa_h;
    op_dtype_f0a_h <= op_dtype_exa_h;
    cmd_f0a_h <= cmd_exa_h;
    sigs_f0a_h <= sigs_exa_h;
    trans_exp_fma2_exc_f0a_h <= trans_exp_fma2_exc_exa_h;
  end
end


//======================================================================================
// Control logic for txfma_f0; Instruction information comes in during the
// ID/F0 pipestage and is flopped locally
//======================================================================================

  // TXFMA control signals
  // operands trans valid enable
assign opa_trans_valid_rfa_h = sigs_in_exa_h.trans | (sigs_in_exa_h.cmd==VPU_TRANS_EXP_FRAC) | (sigs_in_exa_h.cmd==VPU_TRANS_SIN_P2) |
                               (sigs_in_exa_h.cmd==VPU_FCMD_NR2_FRCPFXP);
assign opb_trans_valid_rfa_h = (sigs_in_exa_h.trans & (sigs_in_exa_h.cmd!=VPU_TRANS_SIN_TRANSFORM));
assign opc_trans_valid_rfa_h = sigs_in_exa_h.trans | sigs_in_exa_h.rom | (sigs_in_exa_h.cmd==VPU_TRANS_SIN_P2) | (sigs_in_exa_h.cmd==VPU_TRANS_SIN_TRANSFORM) |
                               (sigs_in_exa_h.cmd==VPU_FCMD_NR2_FRCPFXP);

assign cvt_norm_trans_valid_rfa_h = (sigs_in_exa_h.cmd == VPU_FCMD_CVT_UN24F32) |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_UN16F32) |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_UN10F32) |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_UN8F32)  |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_UN2F32)  |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_SN16F32) |
                                    (sigs_in_exa_h.cmd == VPU_FCMD_CVT_SN8F32);

assign ctrl_valid_exa_h = in_valid_exa_h;

assign use_mul_rfa_h = in_valid_exa_h & (sigs_exa_h[TXFMA_FMA] | sigs_exa_h[TXFMA_MUL] | sigs_exa_h[TXFMA_FMS] | cvt_norm_trans_valid_rfa_h);


  // operands FF enable
assign opa_valid_rfa_h = in_valid_exa_h & (sigs_in_exa_h.ren1 | opa_trans_valid_rfa_h);
assign opb_valid_rfa_h = in_valid_exa_h & (sigs_in_exa_h.ren2 | opb_trans_valid_rfa_h | cvt_norm_trans_valid_rfa_h);
assign opc_valid_rfa_h = in_valid_exa_h & (sigs_in_exa_h.ren3 | opc_trans_valid_rfa_h);

  // trans enable
assign is_trans_rfa_h = (in_valid_exa_h & sigs_in_exa_h.trans);

assign sigs_qual_exa_h =  sigs_in_exa_h & {$bits(sigs_in_exa_h){in_valid_exa_h}};
assign rm_exa_h        =  rm_in_exa_h & {$bits(rm_in_exa_h){in_valid_exa_h}};
assign cmd_exa_h       =  sigs_in_exa_h.cmd & {$bits(sigs_in_exa_h.cmd){in_valid_exa_h}};

assign trans_sin_taylor_p2_op_exa_h = ((cmd_exa_h==VPU_TRANS_SIN_P2) & trans_taylor_exa_h);
assign trans_sin_rom_p3_op_exa_h    = ((cmd_exa_h==VPU_TRANS_SIN_P3) & !trans_taylor_exa_h);
assign trans_op1_exa_h = sigs_exa_h[TXFMA_TRANS1];
assign trans_op2_exa_l = ~sigs_exa_h[TXFMA_TRANS2];

  //data type from decoder + trans
always_comb begin
  op_dtype_exa_h = sigs_qual_exa_h.dtype;

  if (((cmd_exa_h==VPU_TRANS_SIN_P1) & !trans_taylor_exa_h) | ((cmd_exa_h==VPU_TRANS_SIN_P2) & !trans_taylor_exa_h))
    op_dtype_exa_h = (VPU_DTYPE_INT) & {$bits(sigs_qual_exa_h.dtype){in_valid_exa_h}};
end

assign sigs_exa_h[TXFMA_REN1]  = sigs_qual_exa_h.ren1 | sigs_qual_exa_h.trans | (cmd_exa_h==VPU_TRANS_EXP_FRAC) | (cmd_exa_h==VPU_TRANS_SIN_P2) | (cmd_exa_h==VPU_FCMD_NR2_FRCPFXP);
assign sigs_exa_h[TXFMA_REN2]  = sigs_qual_exa_h.ren2 | sigs_exa_h[TXFMA_NORM_F32] | (sigs_qual_exa_h.trans & (cmd_exa_h!=VPU_TRANS_SIN_TRANSFORM));
assign sigs_exa_h[TXFMA_REN3]  = sigs_qual_exa_h.ren3 | sigs_qual_exa_h.trans | sigs_qual_exa_h.rom | (cmd_exa_h==VPU_TRANS_SIN_P2) | (cmd_exa_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_exa_h==VPU_FCMD_NR2_FRCPFXP);
assign sigs_exa_h[TXFMA_FMA]   = sigs_qual_exa_h.fma;
assign sigs_exa_h[TXFMA_FMS]   = sigs_qual_exa_h.fms;
assign sigs_exa_h[TXFMA_ADD]   = sigs_qual_exa_h.add;
assign sigs_exa_h[TXFMA_SUB]   = sigs_qual_exa_h.sub;
assign sigs_exa_h[TXFMA_MUL]   = sigs_qual_exa_h.mul;
assign sigs_exa_h[TXFMA_CVT]   = sigs_qual_exa_h.cvt;
assign sigs_exa_h[TXFMA_GCVT]  = sigs_qual_exa_h.gcvt;

assign sigs_exa_h[TXFMA_UGCVT] = (cmd_exa_h == VPU_FCMD_CVT_F32UN24) |
                                  (cmd_exa_h == VPU_FCMD_CVT_F32UN16) |
                                  (cmd_exa_h == VPU_FCMD_CVT_F32UN10) |
                                  (cmd_exa_h == VPU_FCMD_CVT_F32UN8) |
                                  (cmd_exa_h == VPU_FCMD_CVT_F32UN2) |
                                  (cmd_exa_h == VPU_FCMD_CVT_UN24F32) |
                                  (cmd_exa_h == VPU_FCMD_CVT_UN16F32) |
                                  (cmd_exa_h == VPU_FCMD_CVT_UN10F32) |
                                  (cmd_exa_h == VPU_FCMD_CVT_UN8F32) |
                                  (cmd_exa_h == VPU_FCMD_CVT_UN2F32)

                                  ;

assign sigs_exa_h[TXFMA_SGCVT] = sigs_exa_h[TXFMA_GCVT] & ~sigs_exa_h[TXFMA_UGCVT];

assign sigs_exa_h[TXFMA_F32_FX] = (cmd_exa_h == VPU_FCMD_CVT_F32F16) |
                                   (cmd_exa_h == VPU_FCMD_CVT_F32F11) |
                                   (cmd_exa_h == VPU_FCMD_CVT_F32F10);
assign sigs_exa_h[TXFMA_FX_F32] = (cmd_exa_h == VPU_FCMD_CVT_F16F32) |
                                   (cmd_exa_h == VPU_FCMD_CVT_F11F32) |
                                   (cmd_exa_h == VPU_FCMD_CVT_F10F32);

assign sigs_exa_h[TXFMA_F32_NORM] = (cmd_exa_h == VPU_FCMD_CVT_F32UN24) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32UN16) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32UN10) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32UN8) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32UN2) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32SN16) |
                                     (cmd_exa_h == VPU_FCMD_CVT_F32SN8);
assign sigs_exa_h[TXFMA_NORM_F32] = (cmd_exa_h == VPU_FCMD_CVT_UN24F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_UN16F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_UN10F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_UN8F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_UN2F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_SN16F32) |
                                     (cmd_exa_h == VPU_FCMD_CVT_SN8F32);

assign sigs_exa_h[TXFMA_TRANS1] = (cmd_exa_h==VPU_TRANS_RCP_FMA1) | (cmd_exa_h==VPU_TRANS_RSQRT_FMA1) |
                                   (cmd_exa_h==VPU_TRANS_LOG_FMA1) | (cmd_exa_h==VPU_TRANS_EXP_FMA1) |
                                   (cmd_exa_h==VPU_TRANS_LOG_FMA2) | ((cmd_exa_h==VPU_TRANS_SIN_P1) & !trans_taylor_exa_h);

assign sigs_exa_h[TXFMA_TRANS2] = (cmd_exa_h==VPU_TRANS_RCP_FMA2) | (cmd_exa_h==VPU_TRANS_RSQRT_FMA2) |
                                   (cmd_exa_h==VPU_TRANS_EXP_FMA2) | ((cmd_exa_h==VPU_TRANS_SIN_P2) & !trans_taylor_exa_h);


// FP32 add_sub
assign sigs_exa_h[TXFMA_FP32_ADD_SUB] = (op_dtype_exa_h == VPU_DTYPE_F32) & (sigs_qual_exa_h.fma | sigs_qual_exa_h.fms | sigs_qual_exa_h.add | sigs_qual_exa_h.sub);

  //sin shift amount
always_comb begin
  sin_res_shf_idx_exa_h = 3'd2;

  if ((cmd_exa_h==VPU_TRANS_SIN_P1) & !trans_taylor_exa_h && trans_mask_exa_h[5])
    sin_res_shf_idx_exa_h = 3'd0;

  if ((cmd_exa_h==VPU_TRANS_SIN_P2) & !trans_taylor_exa_h) begin
    case (trans_mask_exa_h[6:2])
      5'b00000: sin_res_shf_idx_exa_h = 3'd5;
      5'b00001: sin_res_shf_idx_exa_h = 3'd4;
      5'b00011: sin_res_shf_idx_exa_h = 3'd3;
      5'b01111: sin_res_shf_idx_exa_h = 3'd2;
      5'b11111: sin_res_shf_idx_exa_h = 3'd1;
    endcase
  end
end

  // TXFMA F0 Pipestage control signals

assign zap_sc_exa_h     = ((op_dtype_exa_h==VPU_DTYPE_F11) | (op_dtype_exa_h==VPU_DTYPE_F10)) | ((cmd_exa_h==VPU_TRANS_SIN_P2) & trans_taylor_exa_h);
assign cvt_ps2gfx_exa_h = (cmd_exa_h==VPU_FCMD_CVT_UN24F32) | (cmd_exa_h==VPU_FCMD_CVT_UN16F32) | (cmd_exa_h==VPU_FCMD_CVT_SN16F32) | (cmd_exa_h==VPU_FCMD_CVT_UN10F32) |
                          (cmd_exa_h==VPU_FCMD_CVT_UN8F32) | (cmd_exa_h==VPU_FCMD_CVT_SN8F32) | (cmd_exa_h==VPU_FCMD_CVT_UN2F32);

always_comb
begin
  eb_hi_k_exa_h = 6'b0;

  if (cmd_exa_h==VPU_FCMD_CVT_UN24F32)
    eb_hi_k_exa_h = 6'd55;       //37h
  if ((cmd_exa_h==VPU_FCMD_CVT_UN16F32) | (cmd_exa_h==VPU_FCMD_CVT_SN16F32))
    eb_hi_k_exa_h = 6'd47;       //2f
  if (cmd_exa_h==VPU_FCMD_CVT_UN10F32)
    eb_hi_k_exa_h = 6'd41;       //29
  if ((cmd_exa_h==VPU_FCMD_CVT_UN8F32) | (cmd_exa_h==VPU_FCMD_CVT_SN8F32))
    eb_hi_k_exa_h = 6'd39;        //27
  if (cmd_exa_h==VPU_FCMD_CVT_UN2F32)
    eb_hi_k_exa_h = 6'd33;        //21
  if (cmd_exa_h==VPU_TRANS_SIN_P2)
    eb_hi_k_exa_h = 6'd36;        //24
end

// OperandA Exponent Selects
assign sel_eainmux_exa_h[1]  = (cmd_exa_h==VPU_TRANS_EXP_FRAC) | (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sel_eainmux_exa_h[0]  = ((op_dtype_exa_h==VPU_DTYPE_F32) & ~((cmd_exa_h==VPU_TRANS_EXP_FRAC) | (cmd_exa_h==VPU_TRANS_SIN_P3))) | (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sel_ealo_ph_exa_h     = (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign ehi_ea_en_exa_h       = sigs_exa_h[TXFMA_REN1];
assign elo_ea_en_exa_h       = sigs_exa_h[TXFMA_REN1] & (op_dtype_exa_h==VPU_DTYPE_F16_F32);

// OperandB Exponent Selects
assign sel_ebin_hi_exa_h[1]  = ((op_dtype_exa_h==VPU_DTYPE_F16_F32) | ((cmd_exa_h==VPU_TRANS_EXP_FMA2) & ~trans_exp_fma2_exc_exa_h));
assign sel_ebin_hi_exa_h[0]  = sigs_exa_h[TXFMA_TRANS2] | (op_dtype_exa_h==VPU_DTYPE_F32 & (~cvt_ps2gfx_exa_h & ~(cmd_exa_h==VPU_TRANS_SIN_P2)));
assign sel_eblo_ph_exa_h     = (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign ehi_eb_en_exa_h       = sigs_exa_h[TXFMA_REN2];
assign elo_eb_en_exa_h       = sigs_exa_h[TXFMA_REN2] & (op_dtype_exa_h==VPU_DTYPE_F16_F32);

// OperandC Exponent Selects
assign sel_ec5b_exa_h[1]     = (cmd_exa_h == VPU_FCMD_CVT_F32F10);
assign sel_ec5b_exa_h[0]     = (cmd_exa_h == VPU_FCMD_CVT_F32F11);
assign sel_ecin_exa_h[1]     = (cmd_exa_h == VPU_FCMD_CVT_F32F10) | (cmd_exa_h == VPU_FCMD_CVT_F32F11) | (cmd_exa_h== VPU_FCMD_CVT_F32F16) | (cmd_exa_h==VPU_TRANS_SIN_P2);
assign sel_ecin_exa_h[0]     = (cmd_exa_h == VPU_FCMD_CVT_F32F10) | (cmd_exa_h == VPU_FCMD_CVT_F32F11) | (cmd_exa_h== VPU_FCMD_CVT_F32F16) | ((op_dtype_exa_h==VPU_DTYPE_F32) & !trans_sin_taylor_p2_op_exa_h) | (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sel_ec5bdet_exa_h     = (cmd_exa_h == VPU_FCMD_CVT_F32F16) | (cmd_exa_h == VPU_FCMD_CVT_F32F11) | (cmd_exa_h == VPU_FCMD_CVT_F32F10);
assign ec_en_exa_h           = sigs_exa_h[TXFMA_REN3];

assign fa_en_exa_h           = sigs_exa_h[TXFMA_REN1];
assign fb_en_exa_h           = sigs_exa_h[TXFMA_REN2];
assign fc_en_exa_h           = sigs_exa_h[TXFMA_REN3];

assign sa_hi_en_exa_h        = sigs_exa_h[TXFMA_REN1];
assign sa_lo_en_exa_h        = sigs_exa_h[TXFMA_REN1] & (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sb_hi_en_exa_h        = sigs_exa_h[TXFMA_REN2];
assign sb_lo_en_exa_h        = sigs_exa_h[TXFMA_REN2] & (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sc_en_exa_h           = sigs_exa_h[TXFMA_REN3];

assign neg_fmaop_exa_h       = (cmd_exa_h==VPU_FCMD_NMADD) | (cmd_exa_h==VPU_FCMD_NMSUB);
assign sa1_nr2_frcpfxp_exa_h = (cmd_exa_h==VPU_FCMD_NR2_FRCPFXP);

assign sel_sc_exa_h[1]       = (op_dtype_exa_h==VPU_DTYPE_SN16) | (cmd_exa_h==VPU_FCMD_CVT_F32F16) |  zap_sc_exa_h;
assign sel_sc_exa_h[0]       = (op_dtype_exa_h==VPU_DTYPE_SN8) | zap_sc_exa_h;

assign sel_sign_sinp2_exa_h  = ((cmd_exa_h==VPU_TRANS_SIN_P2) & trans_taylor_exa_h);

assign sa_zap_exa_l          = ~((op_dtype_exa_h==VPU_DTYPE_SN16) | (op_dtype_exa_h==VPU_DTYPE_SN8) | (cmd_exa_h==VPU_FCMD_CVT_F32F16) | (cmd_exa_h==VPU_FCMD_NR2_FRCPFXP) |
                                 (op_dtype_exa_h==VPU_DTYPE_F10) | (op_dtype_exa_h==VPU_DTYPE_F11));

assign sb_zap_exa_l          = ~((sigs_exa_h[TXFMA_NORM_F32]) | (op_dtype_exa_h==VPU_DTYPE_SN16) | (op_dtype_exa_h==VPU_DTYPE_SN8) | (cmd_exa_h==VPU_FCMD_CVT_F32F16) |
                                 (op_dtype_exa_h==VPU_DTYPE_F10) | (op_dtype_exa_h==VPU_DTYPE_F11));

assign sel_f10_exa_h         = (cmd_exa_h == VPU_FCMD_CVT_F32F10);
assign sel_f11_exa_h         = (cmd_exa_h == VPU_FCMD_CVT_F32F11);

assign sel_opcgfx_exa_h[1]   = (cmd_exa_h==VPU_FCMD_CVT_F32UN8) | (cmd_exa_h==VPU_FCMD_CVT_F32SN8) | (cmd_exa_h==VPU_FCMD_CVT_F32UN2);
assign sel_opcgfx_exa_h[0]   = (cmd_exa_h==VPU_FCMD_CVT_F32UN2) | (cmd_exa_h==VPU_FCMD_CVT_F32UN10);

assign fc_dtps_exa_h         = (op_dtype_exa_h==VPU_DTYPE_F32) | (op_dtype_exa_h==VPU_DTYPE_F16_F32);

assign sel_xopc_exa_h[1]     = (cmd_exa_h==VPU_FCMD_NR2_FRCPFXP) | ((op_dtype_exa_h==VPU_DTYPE_F32) & !trans_sin_taylor_p2_op_exa_h) | (op_dtype_exa_h==VPU_DTYPE_F16_F32) | (op_dtype_exa_h==VPU_DTYPE_UN24) ;
assign sel_xopc_exa_h[0]     = (op_dtype_exa_h==VPU_DTYPE_F10) | (op_dtype_exa_h==VPU_DTYPE_F11) | ((op_dtype_exa_h==VPU_DTYPE_F32) & !trans_sin_taylor_p2_op_exa_h) |
                               (op_dtype_exa_h==VPU_DTYPE_F16_F32) | (op_dtype_exa_h==VPU_DTYPE_UN24);

assign sel_opcfmt_exa_h[1]   = (|sel_xopc_exa_h) | (cmd_exa_h==VPU_FCMD_CVT_F32F16) | trans_sin_taylor_p2_op_exa_h | trans_op1_exa_h | ~trans_op2_exa_l ;
assign sel_opcfmt_exa_h[0]   = (|sel_opcgfx_exa_h) | (cmd_exa_h==VPU_FCMD_CVT_F32SN16) | (cmd_exa_h==VPU_FCMD_CVT_F32UN16) | (cmd_exa_h==VPU_FCMD_CVT_F32F16);


assign sel_snorm_exa_h       = (cmd_exa_h==VPU_FCMD_CVT_SN16F32) | (cmd_exa_h==VPU_FCMD_CVT_SN8F32);
assign sel_g1sk2_exa_h       = (cmd_exa_h==VPU_FCMD_CVT_UN2F32);
assign sel_g1sk8_exa_h       = (cmd_exa_h==VPU_FCMD_CVT_UN8F32) | (cmd_exa_h==VPU_FCMD_CVT_SN8F32);
assign sel_g1sk16_exa_h      = (cmd_exa_h==VPU_FCMD_CVT_UN16F32) | (cmd_exa_h==VPU_FCMD_CVT_SN16F32);
assign sel_g1sk24_exa_h      = (cmd_exa_h==VPU_FCMD_CVT_UN24F32);
assign sel_xexpfrac_exa_h    = (cmd_exa_h==VPU_TRANS_EXP_FRAC);

assign sel_transfa_exa_h[1]  = (cmd_exa_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_exa_h==VPU_TRANS_EXP_FRAC) | trans_sin_rom_p3_op_exa_h;
assign sel_transfa_exa_h[0]  = (trans_sin_rom_p3_op_exa_h | (cmd_exa_h==VPU_TRANS_LOG_MUL));


assign sel_transfb_exa_h[1]  = (cmd_exa_h==VPU_TRANS_LOG_MUL) | trans_sin_taylor_p2_op_exa_h;
assign sel_transfb_exa_h[0]  = trans_sin_taylor_p2_op_exa_h | cvt_ps2gfx_exa_h;

assign sel_opafmt_exa_h[1]   = (op_dtype_exa_h==VPU_DTYPE_F16_F32) | (cmd_exa_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_exa_h==VPU_TRANS_EXP_FRAC) | trans_sin_rom_p3_op_exa_h |
                               (cmd_exa_h==VPU_TRANS_LOG_MUL) | (sigs_exa_h[TXFMA_TRANS1] & (cmd_exa_h!=VPU_TRANS_LOG_FMA2));
assign sel_opafmt_exa_h[0]   = (op_dtype_exa_h==VPU_DTYPE_F32) | (cmd_exa_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_exa_h==VPU_TRANS_EXP_FRAC) | trans_sin_rom_p3_op_exa_h |
                               (cmd_exa_h==VPU_TRANS_LOG_MUL) | (sigs_exa_h[TXFMA_TRANS1] & (cmd_exa_h!=VPU_TRANS_LOG_FMA2));

assign sel_opbfmt_exa_h[1]   = (|sel_transfb_exa_h) | trans_op1_exa_h | ~trans_op2_exa_l | (op_dtype_exa_h==VPU_DTYPE_F16_F32);
assign sel_opbfmt_exa_h[0]   = ((op_dtype_exa_h==VPU_DTYPE_F32) & ~sigs_exa_h[TXFMA_GCVT]) | (|sel_transfb_exa_h) | sigs_exa_h[TXFMA_TRANS1] | sigs_exa_h[TXFMA_TRANS2];



/////////////////////////////////////////////////////////////////////////////////////////
// Control signals for the Multiplier
/////////////////////////////////////////////////////////////////////////////////////////

//1 for signed Integer MUL only
assign trans_mul_sign_exa_h = sigs_exa_h[TXFMA_TRANS2] & (cmd_exa_h!=VPU_TRANS_EXP_FMA2) | (cmd_exa_h==VPU_TRANS_LOG_FMA2) | ((cmd_exa_h==VPU_TRANS_SIN_P2) & !trans_taylor_exa_h);
assign mul_sign_exa_h    = ((cmd_exa_h==VPU_FCMD_MULL || cmd_exa_h==VPU_FCMD_MULH || trans_mul_sign_exa_h || cmd_exa_h==VPU_FCMD_NR2_FRCPFXP) && (op_dtype_exa_h==VPU_DTYPE_INT));
assign mul_hp_mode_exa_h = (op_dtype_exa_h == VPU_DTYPE_F16_F32);


//===================================================================================================================
// Mantissa Special Detect
// NOTE: the logic within txfma_special_detect module has been split between the datapath (txfma_f0) and
// control (txfma_c0).
//===================================================================================================================

  // Qualify the Exponent Zero signals with REN1/REN2/REN3 to block
  // interference farther along the datapath
assign ea_hi_zero_clr_f0a_h = ~ea_hi_zero_f0a_l & sigs_f0a_h[TXFMA_REN1];
assign ea_lo_zero_clr_f0a_h = ~ea_lo_zero_f0a_l & sigs_f0a_h[TXFMA_REN1] & (op_dtype_f0a_h==VPU_DTYPE_F16_F32);
assign eb_hi_zero_clr_f0a_h = ~eb_hi_zero_f0a_l & sigs_f0a_h[TXFMA_REN2] & ~sigs_f0a_h[TXFMA_NORM_F32] & (cmd_f0a_h!=VPU_TRANS_LOG_MUL);
assign eb_lo_zero_clr_f0a_h = ~eb_lo_zero_f0a_l & sigs_f0a_h[TXFMA_REN2] & (op_dtype_f0a_h==VPU_DTYPE_F16_F32);
assign ec_zero_clr_f0a_h    = ~ec_zero_f0a_l & sigs_f0a_h[TXFMA_REN3];

assign sign_prod_hi_f0a_h = sa_hi_f0a_h ^ sb_hi_f0a_h;
assign sign_prod_lo_f0a_h = sa_lo_f0a_h ^ sb_lo_f0a_h;
assign s_prod_hi_prod_lo_f0a_h = sign_prod_hi_f0a_h ^ sign_prod_lo_f0a_h;
assign s_prod_hi_c_hi_f0a_h = sign_prod_hi_f0a_h ^ sc_f0a_h;
assign s_prod_lo_c_hi_f0a_h = sign_prod_lo_f0a_h ^ sc_f0a_h;

 // OpB is shifted down by 2 to compensate for multiplication into the upper (A3A2) bits
assign fa_zero30_21_f0a_h = !faph_hi_b0_f0a_h & !faph_hi_zero_f0a_l;
assign fa_zero30_8_f0a_h = !faps_b0_f0a_h & !faps_zero_f0a_l;

assign fb_zero28_19_f0a_h = !fbph_hi_b0_f0a_h & !fbph_hi_zero_f0a_l;
assign fb_zero28_6_f0a_h = !fbps_b0_f0a_h & !fbps_zero_f0a_l;

assign fc_zero30_21_f0a_h = !fcph_b0_f0a_h & !fcph_zero_f0a_l;   // SS use this only for ph to ps
assign fcnorm_zero_30_8_h = !fcnorm_30_f0a_h & fcnorm_zero_29_8_f0a_h;
assign fc_zero30_8_f0a_h  = !fcps_b0_f0a_h & fcps_zero_f0a_h;
assign fa_hi_zero_f0a_h   = fa_zero30_8_f0a_h;
assign fb_hi_zero_f0a_h   = fb_zero28_6_f0a_h;

assign flog_mul_prod_zero_f0a_h = fa_zero30_8_f0a_h & !faph_hi_zero_f0a_l | fb_zero28_6_f0a_h;

assign use_mul_f0a_h = ctrl_valid_f0a_h & use_mul_op_f0a_h;


always_comb
begin
  res_uint_nan_f0a_h       = 0;
  res_zero_f0a_h           = 0;
  res_one_f0a_h            = 0;
  res_snan_f0a_h           = 0;
  inv_ops_f0a_h            = 0;
  res_pot_inf_f0a_h        = 0;
  res_pot_qnan_f0a_h       = 0;
  prod_inf_hi_f0a_h        = 0;
  c_inf_f0a_h              = 0;
  a_inf_f0a_h              = 0;
  a_zero_hi_f0a_h          = 0;
  b_zero_hi_f0a_h          = 0;
  c_zero_hi_f0a_h          = 0;
  classify_a_hi_f0a_h      = 0;
  classify_a_lo_f0a_h      = 0;
  classify_b_hi_f0a_h      = 0;
  classify_b_lo_f0a_h      = 0;
  classify_c_f0a_h         = 0;
  inf_nan1_hi_f0a_h        = 0;
  inf_nan2_hi_f0a_h        = 0;
  inf_nan1_lo_f0a_h        = 0;
  prod_inf_lo_f0a_h        = 0;
  res_inf_trans_f0a_h      = 0;
  res_unorm_one_f0a_h      = 0;
  res_snorm_one_f0a_h      = 0;
  res_pos_int_nan_f0a_h    = 0;
  res_neg_int_nan_f0a_h    = 0;
  rsqrt_res_nan_inv_f0a_h  = 0;
  sin_res_nan_inv_f0a_h    = 0;
  log_res_nan_inv_f0a_h    = 0;


      //special cases for trans2 frcp/frsqrt/exp
  if ((op_dtype_f0a_h==VPU_DTYPE_INT) && sigs_f0a_h[TXFMA_TRANS2] && ~((cmd_f0a_h==VPU_TRANS_EXP_FMA2) && ~trans_exp_fma2_exc_f0a_h)) begin
    b_zero_hi_f0a_h = (eb_hi_zero_clr_f0a_h & fb_zero28_6_f0a_h);

    classify_b_hi_f0a_h[FP_CLASS_NEG_ZERO]     = sb_hi_f0a_h & b_zero_hi_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_ZERO]     = !sb_hi_f0a_h & b_zero_hi_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_DENORMAL] = !sb_hi_f0a_h & eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL] = sb_hi_f0a_h & eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h;

    classify_b_hi_f0a_h[FP_CLASS_POS_INF]      = !sb_hi_f0a_h & eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l;
    classify_b_hi_f0a_h[FP_CLASS_NEG_INF]      = sb_hi_f0a_h & eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l;
    classify_b_hi_f0a_h[FP_CLASS_QNAN]         = eb_hi_all1_f0a_h & fbps_b0_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_SNAN]         = eb_hi_all1_f0a_h & !fbps_b0_f0a_h & fbps_zero_f0a_l;

      //inf when zero or denormal
    res_inf_trans_f0a_h = classify_b_hi_f0a_h[FP_CLASS_POS_ZERO] | classify_b_hi_f0a_h[FP_CLASS_NEG_ZERO] |
                          classify_b_hi_f0a_h[FP_CLASS_POS_DENORMAL] | classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL];

      //zero when inf and not rsqrt
    res_zero_f0a_h = classify_b_hi_f0a_h[FP_CLASS_POS_INF] | (classify_b_hi_f0a_h[FP_CLASS_NEG_INF] & (cmd_f0a_h == VPU_TRANS_RCP_FMA2));

      //frcp is zero on a value greater than 2 ^126
    res_zero_f0a_h |= (eb_eql_127_f0a_h | (eb_eql_126_f0a_h & !fb_zero28_6_f0a_h)) & (cmd_f0a_h==VPU_TRANS_RCP_FMA2);

      //trans are NaN
    res_pot_qnan_f0a_h  = classify_b_hi_f0a_h[FP_CLASS_QNAN];
    res_snan_f0a_h      = classify_b_hi_f0a_h[FP_CLASS_SNAN];

      //rsqrt is nan when opb is negative and not zero/denormal
    rsqrt_res_nan_inv_f0a_h = (sb_hi_f0a_h & !classify_b_hi_f0a_h[FP_CLASS_NEG_ZERO] & !classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL] & !classify_b_hi_f0a_h[FP_CLASS_QNAN] & (cmd_f0a_h == VPU_TRANS_RSQRT_FMA2));
    res_pot_qnan_f0a_h |= rsqrt_res_nan_inv_f0a_h;

      //clear special detection for exp fma2, as they were treated at trans exp frac operation
    if (cmd_f0a_h==VPU_TRANS_EXP_FMA2) begin
      res_inf_trans_f0a_h = classify_b_hi_f0a_h[FP_CLASS_POS_INF];
      res_one_f0a_h       = classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL] | classify_b_hi_f0a_h[FP_CLASS_POS_DENORMAL] | classify_b_hi_f0a_h[FP_CLASS_POS_ZERO] | classify_b_hi_f0a_h[FP_CLASS_NEG_ZERO];
      res_zero_f0a_h      = classify_b_hi_f0a_h[FP_CLASS_NEG_INF];
      res_pot_qnan_f0a_h  = classify_b_hi_f0a_h[FP_CLASS_QNAN] | res_snan_f0a_h;
    end
  end


  if (op_dtype_f0a_h==VPU_DTYPE_F32) begin
    // if opa[31] is 0 that means all exp bits are 0.This is done in InMux
    a_zero_hi_f0a_h = ea_hi_zero_clr_f0a_h & fa_zero30_8_f0a_h;
    b_zero_hi_f0a_h = eb_hi_zero_clr_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l;
    c_zero_hi_f0a_h = ec_zero_clr_f0a_h & fc_zero30_8_f0a_h;

    classify_a_hi_f0a_h[FP_CLASS_NEG_INF] = sa_hi_f0a_h & ea_hi_all1_f0a_h & fa_zero30_8_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_NEG_NORMAL] = sa_hi_f0a_h & !ea_hi_zero_clr_f0a_h & !ea_hi_all1_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_NEG_DENORMAL] = sa_hi_f0a_h & ea_hi_zero_clr_f0a_h & !fa_zero30_8_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_NEG_ZERO] = sa_hi_f0a_h & a_zero_hi_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_POS_ZERO] = !sa_hi_f0a_h & a_zero_hi_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_POS_DENORMAL] = !sa_hi_f0a_h & ea_hi_zero_clr_f0a_h & !fa_zero30_8_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_POS_NORMAL] = !sa_hi_f0a_h & !ea_hi_zero_clr_f0a_h & !ea_hi_all1_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_POS_INF] = !sa_hi_f0a_h & ea_hi_all1_f0a_h & fa_zero30_8_f0a_h;
    classify_a_hi_f0a_h[FP_CLASS_SNAN] = ea_hi_all1_f0a_h & !faps_b0_f0a_h & faps_zero_f0a_l;
    classify_a_hi_f0a_h[FP_CLASS_QNAN] = ea_hi_all1_f0a_h & faps_b0_f0a_h;

    classify_b_hi_f0a_h[FP_CLASS_NEG_INF] = sb_hi_f0a_h & eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l;
    classify_b_hi_f0a_h[FP_CLASS_NEG_NORMAL] = sb_hi_f0a_h & !eb_hi_zero_clr_f0a_h & !eb_hi_all1_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL] = sb_hi_f0a_h & eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_NEG_ZERO] = sb_hi_f0a_h & b_zero_hi_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_ZERO] = !sb_hi_f0a_h & b_zero_hi_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_DENORMAL] = !sb_hi_f0a_h & eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_NORMAL] = !sb_hi_f0a_h & !eb_hi_zero_clr_f0a_h & !eb_hi_all1_f0a_h;
    classify_b_hi_f0a_h[FP_CLASS_POS_INF] = !sb_hi_f0a_h & eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l;
    classify_b_hi_f0a_h[FP_CLASS_SNAN] = eb_hi_all1_f0a_h & !fbps_b0_f0a_h & fbps_zero_f0a_l;
    classify_b_hi_f0a_h[FP_CLASS_QNAN] = eb_hi_all1_f0a_h & fbps_b0_f0a_h;

    classify_c_f0a_h[FP_CLASS_NEG_INF] = sc_f0a_h & ec_all1_f0a_h & fc_zero30_8_f0a_h;
    classify_c_f0a_h[FP_CLASS_NEG_NORMAL] = sc_f0a_h & !ec_zero_clr_f0a_h & !ec_all1_f0a_h;
    classify_c_f0a_h[FP_CLASS_NEG_DENORMAL] = sc_f0a_h & ec_zero_clr_f0a_h & !fc_zero30_8_f0a_h;
    classify_c_f0a_h[FP_CLASS_NEG_ZERO] = sc_f0a_h & c_zero_hi_f0a_h;
    classify_c_f0a_h[FP_CLASS_POS_ZERO] = !sc_f0a_h & c_zero_hi_f0a_h;
    classify_c_f0a_h[FP_CLASS_POS_DENORMAL] = !sc_f0a_h & ec_zero_clr_f0a_h & !fc_zero30_8_f0a_h;
    classify_c_f0a_h[FP_CLASS_POS_NORMAL] = !sc_f0a_h & !ec_zero_clr_f0a_h & !ec_all1_f0a_h;
    classify_c_f0a_h[FP_CLASS_POS_INF] = !sc_f0a_h & ec_all1_f0a_h & fc_zero30_8_f0a_h;
    classify_c_f0a_h[FP_CLASS_SNAN] = ec_all1_f0a_h & !fcps_b0_f0a_h & !fcps_zero_f0a_h;
    classify_c_f0a_h[FP_CLASS_QNAN] = ec_all1_f0a_h & fcps_b0_f0a_h;

    c_inf_f0a_h = ec_all1_f0a_h & fc_zero30_8_f0a_h;

      //sin is enabling ren3 to perform internal operation, but opc is not used, so clear special detect for operand c
    if ((cmd_f0a_h==VPU_TRANS_SIN_P1) || (cmd_f0a_h==VPU_TRANS_SIN_P2) || (cmd_f0a_h==VPU_TRANS_SIN_P3)) begin
      classify_c_f0a_h &= '0;
      c_inf_f0a_h       = 0;
    end

    // potential infinity (in case NaN doesn't occur)
    res_pot_inf_f0a_h = ea_hi_all1_f0a_h & fa_zero30_8_f0a_h | eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l | c_inf_f0a_h;
    a_inf_f0a_h = ea_hi_all1_f0a_h & fa_zero30_8_f0a_h;
    prod_inf_hi_f0a_h = ea_hi_all1_f0a_h & fa_zero30_8_f0a_h | eb_hi_all1_f0a_h & !fbps_b0_f0a_h  & !fbps_zero_f0a_l;

    // potential qnan (in case the result isn't snan)
    res_pot_qnan_f0a_h = classify_a_hi_f0a_h[FP_CLASS_QNAN] | classify_b_hi_f0a_h[FP_CLASS_QNAN] | classify_c_f0a_h[FP_CLASS_QNAN];

    // 0*inf
    inf_nan1_hi_f0a_h = (ea_hi_all1_f0a_h & fa_zero30_8_f0a_h & eb_hi_zero_clr_f0a_h | eb_hi_all1_f0a_h & !fbps_b0_f0a_h & !fbps_zero_f0a_l & ea_hi_zero_clr_f0a_h)
                      & (sigs_f0a_h[TXFMA_FMA] | sigs_f0a_h[TXFMA_FMS] | sigs_f0a_h[TXFMA_MUL]);
    // inf-inf
    inf_nan2_hi_f0a_h = ((prod_inf_hi_f0a_h & c_inf_f0a_h & (sign_prod_hi_f0a_h ^ sc_f0a_h ^ (sigs_f0a_h[TXFMA_SUB] | sigs_f0a_h[TXFMA_FMS]))) & ~res_pot_qnan_f0a_h)
                      & (sigs_f0a_h[TXFMA_SUB] | sigs_f0a_h[TXFMA_ADD] | sigs_f0a_h[TXFMA_FMA] | sigs_f0a_h[TXFMA_FMS] | sigs_f0a_h[TXFMA_MUL]);

    inv_ops_f0a_h = inf_nan1_hi_f0a_h | inf_nan2_hi_f0a_h;

    res_snan_f0a_h    = classify_a_hi_f0a_h[FP_CLASS_SNAN] | classify_b_hi_f0a_h[FP_CLASS_SNAN] | classify_c_f0a_h[FP_CLASS_SNAN];

    // F32toINT
    res_pos_int_nan_f0a_h   = (classify_c_f0a_h[FP_CLASS_POS_INF] | classify_c_f0a_h[FP_CLASS_SNAN] | classify_c_f0a_h[FP_CLASS_QNAN]) &
                               ((cmd_f0a_h == VPU_FCMD_CVT_INTF32) | (cmd_f0a_h == VPU_FCMD_CVT_RASTF32));
    res_neg_int_nan_f0a_h   = classify_c_f0a_h[FP_CLASS_NEG_INF] & ((cmd_f0a_h == VPU_FCMD_CVT_INTF32) | (cmd_f0a_h == VPU_FCMD_CVT_RASTF32));


    res_uint_nan_f0a_h      = (classify_c_f0a_h[FP_CLASS_POS_INF] | classify_c_f0a_h[FP_CLASS_SNAN] | classify_c_f0a_h[FP_CLASS_QNAN]) & (cmd_f0a_h == VPU_FCMD_CVT_UINTF32);
    res_zero_f0a_h          = sc_f0a_h & ~(sc_f0a_h & (classify_c_f0a_h[FP_CLASS_SNAN] | classify_c_f0a_h[FP_CLASS_QNAN])) & (cmd_f0a_h == VPU_FCMD_CVT_UINTF32);

      //LOG
    if (cmd_f0a_h==VPU_TRANS_LOG_MUL) begin
        //log result is nan when opc is negative or NaN
        //inf when opc is inf or zero
      res_inf_trans_f0a_h   = classify_c_f0a_h[FP_CLASS_POS_INF] | classify_c_f0a_h[FP_CLASS_POS_ZERO] | classify_c_f0a_h[FP_CLASS_NEG_ZERO] |
                              classify_c_f0a_h[FP_CLASS_POS_DENORMAL] | classify_c_f0a_h[FP_CLASS_NEG_DENORMAL];
      log_res_nan_inv_f0a_h = (sc_f0a_h & ~(res_inf_trans_f0a_h | classify_c_f0a_h[FP_CLASS_QNAN]));
      res_pot_qnan_f0a_h    = log_res_nan_inv_f0a_h | classify_c_f0a_h[FP_CLASS_QNAN];
    end

      //EXP
    if (cmd_f0a_h==VPU_TRANS_EXP_FRAC) begin
      res_zero_f0a_h      = classify_c_f0a_h[FP_CLASS_NEG_ZERO] | classify_c_f0a_h[FP_CLASS_POS_ZERO];
      res_inf_trans_f0a_h = classify_c_f0a_h[FP_CLASS_POS_INF] | classify_c_f0a_h[FP_CLASS_NEG_INF];
      res_pot_qnan_f0a_h  = classify_c_f0a_h[FP_CLASS_QNAN];
      res_pot_inf_f0a_h   = 1'b0;
      res_snan_f0a_h      = classify_c_f0a_h[FP_CLASS_SNAN];
    end

      //SIN
    if ((cmd_f0a_h==VPU_TRANS_SIN_TRANSFORM) || (cmd_f0a_h==VPU_FCMD_FFRC_SIN)) begin
      res_pot_qnan_f0a_h  = classify_c_f0a_h[FP_CLASS_SNAN] | classify_c_f0a_h[FP_CLASS_QNAN];
      res_inf_trans_f0a_h = classify_c_f0a_h[FP_CLASS_POS_INF] | classify_c_f0a_h[FP_CLASS_NEG_INF];
    end
    if (cmd_f0a_h==VPU_TRANS_SIN_P3) begin
      // res_pot_qnan_f0a_h = classify_b_hi_f0a_h[FP_CLASS_SNAN] | classify_b_hi_f0a_h[FP_CLASS_QNAN] | classify_b_hi_f0a_h[FP_CLASS_POS_INF] | classify_b_hi_f0a_h[FP_CLASS_NEG_INF];
      sin_res_nan_inv_f0a_h = classify_b_hi_f0a_h[FP_CLASS_POS_INF] | classify_b_hi_f0a_h[FP_CLASS_NEG_INF];
      res_pot_qnan_f0a_h    = sin_res_nan_inv_f0a_h | classify_b_hi_f0a_h[FP_CLASS_SNAN] | classify_b_hi_f0a_h[FP_CLASS_QNAN];
      //0.25
      res_one_f0a_h = eb_eql_125_f0a_h & fb_zero28_6_f0a_h;
    end
  end

    //other data types
  case (op_dtype_f0a_h)
     VPU_DTYPE_F16_F32 : begin

        // For VPU_DTYPE_F16_F32 OperandA & OperandB hold 2 16-bit half-precision FP numbers;
        // OperandC holds 1 32-bit single-precision FP number.
        //
        classify_a_lo_f0a_h[FP_CLASS_SNAN] = ea_lo_all1_f0a_h & !faph_lo_b0_f0a_h & faph_lo_zero_f0a_l;
        classify_a_lo_f0a_h[FP_CLASS_QNAN] = ea_lo_all1_f0a_h & faph_lo_b0_f0a_h;
        classify_b_lo_f0a_h[FP_CLASS_SNAN] = eb_lo_all1_f0a_h & !fbph_lo_b0_f0a_h & fbph_lo_zero_f0a_l;
        classify_b_lo_f0a_h[FP_CLASS_QNAN] = eb_lo_all1_f0a_h & fbph_lo_b0_f0a_h;
        classify_a_hi_f0a_h[FP_CLASS_SNAN] = ea_hi_all1_f0a_h & !faph_hi_b0_f0a_h & faph_hi_zero_f0a_l;
        classify_a_hi_f0a_h[FP_CLASS_QNAN] = ea_hi_all1_f0a_h & faph_hi_b0_f0a_h;
        classify_b_hi_f0a_h[FP_CLASS_SNAN] = eb_hi_all1_f0a_h & !fbph_hi_b0_f0a_h & fbph_hi_zero_f0a_l;
        classify_b_hi_f0a_h[FP_CLASS_QNAN] = eb_hi_all1_f0a_h & fbph_hi_b0_f0a_h;
        classify_c_f0a_h[FP_CLASS_NEG_INF] = sc_f0a_h & ec_all1_f0a_h & fc_zero30_8_f0a_h;
        classify_c_f0a_h[FP_CLASS_POS_INF] = !sc_f0a_h & ec_all1_f0a_h & fc_zero30_8_f0a_h;
        classify_c_f0a_h[FP_CLASS_SNAN]    = ec_all1_f0a_h & !fcps_b0_f0a_h & !fcps_zero_f0a_h;
        classify_c_f0a_h[FP_CLASS_QNAN]    = ec_all1_f0a_h & fcps_b0_f0a_h;


        // potential infinity (in case NaN doesn't occur)
        res_pot_inf_f0a_h = ea_lo_all1_f0a_h & !faph_lo_b0_f0a_h & !faph_lo_zero_f0a_l | eb_lo_all1_f0a_h & !fbph_lo_b0_f0a_h & !fbph_lo_zero_f0a_l;
        prod_inf_lo_f0a_h = res_pot_inf_f0a_h
                          & ~(classify_a_lo_f0a_h[FP_CLASS_QNAN] | classify_b_lo_f0a_h[FP_CLASS_QNAN]);
        prod_inf_hi_f0a_h = ((ea_hi_all1_f0a_h & !faph_hi_b0_f0a_h & !faph_hi_zero_f0a_l) | (eb_hi_all1_f0a_h & !fbph_hi_b0_f0a_h & !fbph_hi_zero_f0a_l))
                          & ~(classify_a_hi_f0a_h[FP_CLASS_QNAN] | classify_b_hi_f0a_h[FP_CLASS_QNAN]);


        // potential infinity (in case NaN doesn't occur)
        c_inf_f0a_h = ec_all1_f0a_h &  fc_zero30_8_f0a_h;
        res_pot_inf_f0a_h =  prod_inf_hi_f0a_h | prod_inf_lo_f0a_h | c_inf_f0a_h;

        // potential qnan (in case the result isn't snan)
        res_pot_qnan_f0a_h = classify_a_hi_f0a_h[FP_CLASS_QNAN] | classify_b_hi_f0a_h[FP_CLASS_QNAN] | classify_a_lo_f0a_h[FP_CLASS_QNAN] |
                             classify_b_lo_f0a_h[FP_CLASS_QNAN] | classify_c_f0a_h[FP_CLASS_QNAN];

        // 0*inf
        inf_nan1_lo_f0a_h = ea_lo_all1_f0a_h & !faph_lo_b0_f0a_h & !faph_lo_zero_f0a_l & eb_lo_zero_clr_f0a_h | eb_lo_all1_f0a_h & !fbph_lo_b0_f0a_h & !fbph_lo_zero_f0a_l & ea_lo_zero_clr_f0a_h;
        inf_nan1_hi_f0a_h = ((ea_hi_all1_f0a_h & !faph_hi_b0_f0a_h & !faph_hi_zero_f0a_l & eb_hi_zero_clr_f0a_h) | (eb_hi_all1_f0a_h & !fbph_hi_b0_f0a_h & !fbph_hi_zero_f0a_l & ea_hi_zero_clr_f0a_h) | inf_nan1_lo_f0a_h)
                          & (sigs_f0a_h[TXFMA_FMA] | sigs_f0a_h[TXFMA_MUL]);

        // inf-inf
        inf_nan2_hi_f0a_h = ((prod_inf_hi_f0a_h & prod_inf_lo_f0a_h & s_prod_hi_prod_lo_f0a_h) |
                             (prod_inf_hi_f0a_h & c_inf_f0a_h & s_prod_hi_c_hi_f0a_h) |
                             (prod_inf_lo_f0a_h & c_inf_f0a_h & s_prod_lo_c_hi_f0a_h))
                          & (sigs_f0a_h[TXFMA_FMA] | sigs_f0a_h[TXFMA_MUL]);

        res_snan_f0a_h    = classify_a_hi_f0a_h[FP_CLASS_SNAN] | classify_b_hi_f0a_h[FP_CLASS_SNAN] | classify_a_lo_f0a_h[FP_CLASS_SNAN] | classify_b_lo_f0a_h[FP_CLASS_SNAN] |  classify_c_f0a_h[FP_CLASS_SNAN];
        inv_ops_f0a_h     = inf_nan1_hi_f0a_h | inf_nan2_hi_f0a_h;
    end

   VPU_DTYPE_F16 : begin
        classify_c_f0a_h[FP_CLASS_SNAN]    = ec_all1_f0a_h & ~fcph_b0_f0a_h & fcph_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_QNAN]    = ec_all1_f0a_h & fcph_b0_f0a_h;
        classify_c_f0a_h[FP_CLASS_NEG_INF] = sc_f0a_h & ec_all1_f0a_h & !fcph_b0_f0a_h & !fcph_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_POS_INF] = !sc_f0a_h & ec_all1_f0a_h & !fcph_b0_f0a_h & !fcph_zero_f0a_l;

        res_pot_inf_f0a_h = classify_c_f0a_h[FP_CLASS_NEG_INF] | classify_c_f0a_h[FP_CLASS_POS_INF];

        // potential qnan (in case the result isn't snan)
        res_pot_qnan_f0a_h = classify_c_f0a_h[FP_CLASS_QNAN];
        res_snan_f0a_h     = classify_c_f0a_h[FP_CLASS_SNAN];
    end

    VPU_DTYPE_F11 : begin
        classify_c_f0a_h[FP_CLASS_SNAN]    = ec_all1_f0a_h & ~fcf11_b0_f0a_h & fcf11_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_QNAN]    = ec_all1_f0a_h & fcf11_b0_f0a_h;
        classify_c_f0a_h[FP_CLASS_NEG_INF] = sc_f0a_h & ec_all1_f0a_h & ~fcf11_b0_f0a_h & ~fcf11_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_POS_INF] = ~sc_f0a_h & ec_all1_f0a_h & ~fcf11_b0_f0a_h & ~fcf11_zero_f0a_l;

        res_pot_inf_f0a_h = classify_c_f0a_h[FP_CLASS_NEG_INF] | classify_c_f0a_h[FP_CLASS_POS_INF];

        // potential qnan (in case the result isn't snan)
        res_pot_qnan_f0a_h = classify_c_f0a_h[FP_CLASS_QNAN];
        res_snan_f0a_h     = classify_c_f0a_h[FP_CLASS_SNAN];
    end
    VPU_DTYPE_F10 : begin
        classify_c_f0a_h[FP_CLASS_SNAN]    = ec_all1_f0a_h & ~fcf10_b0_f0a_h & fcf10_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_QNAN]    = ec_all1_f0a_h & fcf10_b0_f0a_h;
        classify_c_f0a_h[FP_CLASS_NEG_INF] = sc_f0a_h & ec_all1_f0a_h & ~fcf10_b0_f0a_h & ~fcf10_zero_f0a_l;
        classify_c_f0a_h[FP_CLASS_POS_INF] = ~sc_f0a_h & ec_all1_f0a_h & ~fcf10_b0_f0a_h & ~fcf10_zero_f0a_l;

        res_pot_inf_f0a_h = classify_c_f0a_h[FP_CLASS_NEG_INF] | classify_c_f0a_h[FP_CLASS_POS_INF];

        // potential qnan (in case the result isn't snan)
        res_pot_qnan_f0a_h = classify_c_f0a_h[FP_CLASS_QNAN];
        res_snan_f0a_h     = classify_c_f0a_h[FP_CLASS_SNAN];
    end
    VPU_DTYPE_UN24 : begin
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_unorm_one_f0a_h = fc_31_f0a_h & fcn24_one_30_8_f0a_h;
    end
    VPU_DTYPE_UN16 : begin
      // Even though the LSB is 16, the bottom bits are guaranteed to be zero so we
      // use the signal fcps_zero_f0a_h
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_unorm_one_f0a_h = fc_31_f0a_h & fcn16_one_30_16_f0a_h;
    end
    VPU_DTYPE_UN10 : begin
      // Even though the LSB is 22, the bottom bits are guaranteed to be zero so we
      // use the signal fcph_zero_f0a_l
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_unorm_one_f0a_h = fc_31_f0a_h & fcn10_one_30_22_f0a_h;
    end
    VPU_DTYPE_UN8 : begin
      // Even though the LSB is 24, the bottom bits are guaranteed to be zero so we
      // use the signal fcph_zero_f0a_l
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_unorm_one_f0a_h = fc_31_f0a_h & fcn8_one_30_24_f0a_h;
    end
    VPU_DTYPE_UN2 : begin
      c_zero_hi_f0a_h = !(fc_31_f0a_h | fcnorm_30_f0a_h);
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_unorm_one_f0a_h = fc_31_f0a_h & fcnorm_30_f0a_h;
    end
    VPU_DTYPE_SN16 : begin
      // We only need to test from bits [30:16]; however we use the available signal fcnorm_zero_30_8_h
      // since the bits [15:8] are guaranteed to be zero
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_snorm_one_f0a_h = (~fc_31_f0a_h & fcn16_one_30_16_f0a_h) | (fc_31_f0a_h & fcnorm_zero_30_8_h) | (fc_31_f0a_h & fcnorm16_eq_1_f0a_h);
    end
    VPU_DTYPE_SN8 : begin
      c_zero_hi_f0a_h = !fc_31_f0a_h & fcnorm_zero_30_8_h;
      classify_c_f0a_h[FP_CLASS_POS_ZERO] = c_zero_hi_f0a_h;
      res_snorm_one_f0a_h = (~fc_31_f0a_h & fcn8_one_30_24_f0a_h) | (fc_31_f0a_h & fcnorm_zero_30_8_h) | (fc_31_f0a_h & fcnorm8_eq_1_f0a_h);
    end
  endcase
end

assign res_inf_f0a_h = !(res_qnan_f0a_h | res_snan_f0a_h) & res_pot_inf_f0a_h | res_inf_trans_f0a_h;
assign res_qnan_f0a_h = !res_snan_f0a_h & (res_pot_qnan_f0a_h | inv_ops_f0a_h);


  //flags denorm mux
always_comb begin

  case (op_dtype_f0a_h)
    VPU_DTYPE_F32 : begin
      flags_denorm_f0a_h = (cmd_f0a_h!=VPU_TRANS_LOG_MUL) & ((ea_hi_zero_clr_f0a_h & !fa_zero30_8_f0a_h) | (eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h)) | (ec_zero_clr_f0a_h & !fc_zero30_8_f0a_h);
      if (sigs_f0a_h[TXFMA_NORM_F32])
        flags_denorm_f0a_h = ea_hi_zero_clr_f0a_h & !fa_zero30_8_f0a_h;
    end
    VPU_DTYPE_INT : begin
      flags_denorm_f0a_h = 1'b0;

      if (sigs_f0a_h[TXFMA_TRANS2])
        flags_denorm_f0a_h = (eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h) & ~((cmd_f0a_h==VPU_TRANS_EXP_FMA2) & ~trans_exp_fma2_exc_f0a_h);
    end

    VPU_DTYPE_F16_F32 : begin
      flags_denorm_f0a_h = (ea_hi_zero_clr_f0a_h & !fa_zero30_21_f0a_h) | (eb_hi_zero_clr_f0a_h & !fb_zero28_19_f0a_h) | (ec_zero_clr_f0a_h & !fc_zero30_8_f0a_h);
      flags_denorm_f0a_h |= (ea_lo_zero_clr_f0a_h & (faph_lo_b0_f0a_h | faph_lo_zero_f0a_l))  | (eb_lo_zero_clr_f0a_h & (fbph_lo_b0_f0a_h | fbph_lo_zero_f0a_l));
    end
    VPU_DTYPE_F16 : begin
      // We only need to test from bits [30:16]; however we use the available signal fc_zero30_8_f0a_h
      // since the bits [15:8] are guaranteed to be zero
      flags_denorm_f0a_h = ec_zero_clr_f0a_h & !fc_zero30_21_f0a_h;
    end
    VPU_DTYPE_F11 : begin
      // NOTE: opc[5:0] is moved into fc[30:25]
      // We only need to test from bits [30:25]; however we use the available signal fc_zero30_21_f0a_h
      // since the bits [24:21] are guaranteed to be zero
      //flags_denorm_f0a_h = ec_zero_clr_f0a_h & ~(|opc[5:0]);
      flags_denorm_f0a_h = ec_zero_clr_f0a_h & ~fc_zero30_21_f0a_h;
    end
    VPU_DTYPE_F10 : begin
      // NOTE: opc[4:0] is moved into fc[30:26]
      // We only need to test from bits [30:26]; however we use the available signal fc_zero30_21_f0a_h
      // since the bits [25:21] are guaranteed to be zero
      //flags_denorm_f0a_h = ec_zero_clr_f0a_h & ~(|opc[4:0]);
      flags_denorm_f0a_h = ec_zero_clr_f0a_h & ~fc_zero30_21_f0a_h;
    end
    default : begin
      flags_denorm_f0a_h = 1'b0;
    end
  endcase

    //sin res is denormal for intermediate sin u-inst so final sin_p3 can process it andflash to zero
  if ((cmd_f0a_h==VPU_TRANS_SIN_TRANSFORM) || (cmd_f0a_h==VPU_FCMD_FFRC_SIN)) begin
    flags_denorm_f0a_h = classify_c_f0a_h[FP_CLASS_POS_DENORMAL] | classify_c_f0a_h[FP_CLASS_NEG_DENORMAL];
  end
  if ((cmd_f0a_h==VPU_TRANS_SIN_P1) || (cmd_f0a_h==VPU_TRANS_SIN_P2)) begin
    flags_denorm_f0a_h = classify_a_hi_f0a_h[FP_CLASS_POS_DENORMAL] | classify_a_hi_f0a_h[FP_CLASS_NEG_DENORMAL] | classify_b_hi_f0a_h[FP_CLASS_POS_DENORMAL] | classify_b_hi_f0a_h[FP_CLASS_NEG_DENORMAL];
  end
  if ((cmd_f0a_h==VPU_TRANS_SIN_P3)) begin
    flags_denorm_f0a_h = (eb_hi_zero_clr_f0a_h & !fb_zero28_6_f0a_h);
  end
end


  //flags
always_comb begin
     //ovf/unf flags set at the end of pipeline
  flags_f0a_h[VPU_FLAG_OVF]      = 1'b0;
  flags_f0a_h[VPU_FLAG_UNF]      = 1'b0;
  flags_f0a_h[VPU_FLAG_DENORMAL] = flags_denorm_f0a_h;
  flags_f0a_h[VPU_FLAG_INVALID]  = res_snan_f0a_h | inf_nan1_hi_f0a_h | inf_nan2_hi_f0a_h | rsqrt_res_nan_inv_f0a_h | sin_res_nan_inv_f0a_h | log_res_nan_inv_f0a_h;
  flags_f0a_h[VPU_FLAG_DIV0]     = (res_inf_trans_f0a_h && ((cmd_f0a_h==VPU_TRANS_RCP_FMA2) | (cmd_f0a_h==VPU_TRANS_RSQRT_FMA2)));
    //set inexact flag from frcp.ps on a value greater than 2^126
  flags_f0a_h[VPU_FLAG_INEXACT]  = (res_zero_f0a_h && (cmd_f0a_h==VPU_TRANS_RCP_FMA2));
end

assign a_is_qnan_hi_f0a_h = classify_a_hi_f0a_h[FP_CLASS_QNAN];
assign c_is_qnan_f0a_h = classify_c_f0a_h[FP_CLASS_QNAN];
assign a_is_snan_hi_f0a_h = classify_a_hi_f0a_h[FP_CLASS_SNAN];
assign c_is_snan_f0a_h = classify_c_f0a_h[FP_CLASS_SNAN];

  //flags enable
assign flags_en_f0a_h = (|flags_f0a_h);

assign dtps_f0a_h = (op_dtype_f0a_h == VPU_DTYPE_F32) ;

  //special result enable
assign spec_res_en_nan_f0a_h  = res_snan_f0a_h | res_qnan_f0a_h | res_pos_int_nan_f0a_h | res_neg_int_nan_f0a_h | res_uint_nan_f0a_h;
assign spec_res_en_norm_f0a_h = res_unorm_one_f0a_h | res_snorm_one_f0a_h;
assign spec_res_en_f0a_h      = spec_res_en_nan_f0a_h | spec_res_en_norm_f0a_h | res_inf_f0a_h | res_zero_f0a_h | (|classify_c_f0a_h) | res_one_f0a_h;

assign classify_f0a_h = classify_c_f0a_h;

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on CASEINCOMPLETE */
