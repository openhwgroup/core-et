// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c0 
(
  input  [`VPU_DTYPE_SZ-1:0]       op_dtype_f0a_h, 
  input                            ctrl_valid_f0a_h,
  input                            vpu_ctrl_ren1_f0a_h,
  input                            vpu_ctrl_ren2_f0a_h,
  input                            vpu_ctrl_ren3_f0a_h,
  input                            vpu_ctrl_fma_f0a_h,
  input                            vpu_ctrl_fms_f0a_h,
  input                            vpu_ctrl_add_f0a_h,
  input                            vpu_ctrl_sub_f0a_h,
  input                            vpu_ctrl_mul_f0a_h,
  input                            vpu_ctrl_cvt_f0a_h,
  input                            vpu_ctrl_gcvt_f0a_h,
  input                            vpu_ctrl_trans_f0a_h,
  input                            vpu_ctrl_rom_f0a_h,
  input [`VPU_FCMD_SZ-1:0]         vpu_ctrl_cmd_f0a_h,
  input                            trans_taylor,
  input                            trans_fma1_mask,
  input  [4:0]                     trans_fma2_mask,
  output [`TXFMA_SIGS_SZ-1:0]      txfma_sigs_f0a_h,
  output logic [`VPU_DTYPE_SZ-1:0] op_dtype_out_f0a_h,
  output logic [2:0]               sin_res_shf_idx_f0a_h
);

  // TXFMA control signals
assign txfma_sigs_f0a_h[`TXFMA_REN1]  = vpu_ctrl_ren1_f0a_h | vpu_ctrl_trans_f0a_h | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_EXP_FRAC) | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P2) | (vpu_ctrl_cmd_f0a_h==`VPU_FCMD_NR2_FRCPFXP);
assign txfma_sigs_f0a_h[`TXFMA_REN2]  = vpu_ctrl_ren2_f0a_h | txfma_sigs_f0a_h[`TXFMA_NORM_F32] | (vpu_ctrl_trans_f0a_h & (vpu_ctrl_cmd_f0a_h!=`VPU_TRANS_SIN_TRANSFORM));
assign txfma_sigs_f0a_h[`TXFMA_REN3]  = vpu_ctrl_ren3_f0a_h | (vpu_ctrl_trans_f0a_h | vpu_ctrl_rom_f0a_h) | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_TRANSFORM) | (vpu_ctrl_cmd_f0a_h==`VPU_FCMD_NR2_FRCPFXP);

assign txfma_sigs_f0a_h[`TXFMA_FMA]   = vpu_ctrl_fma_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_FMS]   = vpu_ctrl_fms_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_ADD]   = vpu_ctrl_add_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_SUB]   = vpu_ctrl_sub_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_MUL]   = vpu_ctrl_mul_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_CVT]   = vpu_ctrl_cvt_f0a_h;
assign txfma_sigs_f0a_h[`TXFMA_GCVT]  = vpu_ctrl_gcvt_f0a_h;

assign txfma_sigs_f0a_h[`TXFMA_UGCVT] = (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN24) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN16) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN8) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN24F32) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN16F32) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN10F32) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN8F32) | 
                                        (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN2F32); 
assign txfma_sigs_f0a_h[`TXFMA_SGCVT] = txfma_sigs_f0a_h[`TXFMA_GCVT] & ~txfma_sigs_f0a_h[`TXFMA_UGCVT];
assign txfma_sigs_f0a_h[`TXFMA_F32_FX] = (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32F16) |
                                         (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32F11) |
                                         (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32F10);
assign txfma_sigs_f0a_h[`TXFMA_FX_F32] = (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F16F32) |
                                         (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F11F32) |
                                         (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F10F32);
assign txfma_sigs_f0a_h[`TXFMA_F32_NORM] = (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN24) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN16) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN10) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN8) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32UN2) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32SN16) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_F32SN8);
assign txfma_sigs_f0a_h[`TXFMA_NORM_F32] = (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN24F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN16F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN10F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN8F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_UN2F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_SN16F32) |
                                           (vpu_ctrl_cmd_f0a_h == `VPU_FCMD_CVT_SN8F32);

assign txfma_sigs_f0a_h[`TXFMA_TRANS1] = (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_RCP_FMA1) | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_RSQRT_FMA1) 
                                       | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_LOG_FMA1) | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_EXP_FMA1)
                                       | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_LOG_FMA2) | ((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P1) & !trans_taylor);

assign txfma_sigs_f0a_h[`TXFMA_TRANS2] = (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_RCP_FMA2) | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_RSQRT_FMA2)
                                       | (vpu_ctrl_cmd_f0a_h==`VPU_TRANS_EXP_FMA2) | ((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P2) & !trans_taylor);

  //data type from decoder + trans
always_comb begin
  op_dtype_out_f0a_h = op_dtype_f0a_h;

  if (((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P1) & !trans_taylor) | ((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P2) & !trans_taylor))
    op_dtype_out_f0a_h = `VPU_DTYPE_INT;
end

// FP32 add_sub
assign txfma_sigs_f0a_h[`TXFMA_FP32_ADD_SUB] = (op_dtype_f0a_h == `VPU_DTYPE_F32) & (vpu_ctrl_fma_f0a_h | vpu_ctrl_fms_f0a_h | vpu_ctrl_add_f0a_h | vpu_ctrl_sub_f0a_h);


  //sin shift amount
always_comb begin
  sin_res_shf_idx_f0a_h = 3'd2;

  if ((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P1) & !trans_taylor && trans_fma1_mask)
    sin_res_shf_idx_f0a_h = 3'd0;

  if ((vpu_ctrl_cmd_f0a_h==`VPU_TRANS_SIN_P2) & !trans_taylor) begin 
    case (trans_fma2_mask)
      5'b00000: sin_res_shf_idx_f0a_h = 3'd5;
      5'b00001: sin_res_shf_idx_f0a_h = 3'd4;
      5'b00011: sin_res_shf_idx_f0a_h = 3'd3;
      5'b01111: sin_res_shf_idx_f0a_h = 3'd2;
      5'b11111: sin_res_shf_idx_f0a_h = 3'd1;
    endcase
  end
end

 
endmodule 
