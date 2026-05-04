// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c4
(
  input  [`VPU_DTYPE_SZ-1:0]                 op_dtype_f4a_h, 
  input  [`VPU_FCMD_SZ-1:0]                  cmd_f4a_h, 
  input  [`TXFMA_SIGS_SZ-1:0]                sigs_f4a_h, 
  input  [`TXFMA_LXD_PS_SZ-1:0]              elxd_f4a_h,
  input  [2:0]                               sin_res_shf_idx_f4a_h,
  input                                      ph_comb_stiky_f4a_h,
  input                                      eff_sub1_f4a_h,
  input                                      eff_sub2_f4a_h,
  input                                      s_res_f4a_h,
  input                                      sc_f4a_h,
  output                                     neg_norm_a3_f4a_h,
  output                                     neg_norm_na3_f4a_h,
  output                                     clearphps_en_f4a_h,
  output                                     clear16frnd_nshin_f4a_l,
  output                                     ph_ps_clear_f4a_l,
  output [`TXFMA_NORM_SHF_AMNT_SZ-1:0]       enc_nsamt_f4a_h,
  output                                     elxd_add_in2_mux_sel_f4a_h,
  output                                     use_elxd_f4a_h
);

//================================================================================================================
// Locally used siganls 
//================================================================================================================

logic [`TXFMA_NORM_SHF_AMNT_SZ-1:0]     norm_shftamt_f4a_h;
logic [`TXFMA_NORM_SHF_AMNT_SZ-1:0]     tmp_norm_shftamt_f4a_h;
logic                                   sel_other_normshift_f4a_h;
logic                                   sel_elxd_normshift_f4a_h;
logic                                   norm_eff_sub_f4a_h;

//================================================================================================================
// Signal sent to txfma_e4
//================================================================================================================

  //use elxd hi
assign use_elxd_f4a_h = ((op_dtype_f4a_h == `VPU_DTYPE_F32) & ~(sigs_f4a_h[`TXFMA_CVT] | sigs_f4a_h[`TXFMA_NORM_F32] | (cmd_f4a_h==`VPU_TRANS_EXP_FRAC)))
                      | (cmd_f4a_h==`VPU_FCMD_CVT_F32INT | cmd_f4a_h==`VPU_FCMD_CVT_F32UINT | cmd_f4a_h==`VPU_FCMD_CVT_F32RAST) 
                      | (op_dtype_f4a_h == `VPU_DTYPE_F16_F32);



//================================================================================================================
// Normalization Shifter - Calculate shift amount 
//================================================================================================================

always_comb begin
    
  norm_shftamt_f4a_h        = `TXFMA_NORM_SHF_AMNT_SZ'b0;
  tmp_norm_shftamt_f4a_h    = `TXFMA_NORM_SHF_AMNT_SZ'b0;
  sel_other_normshift_f4a_h = 1'b0;
  sel_elxd_normshift_f4a_h  = 1'b0;

  if ((op_dtype_f4a_h != `VPU_DTYPE_F32) & sigs_f4a_h[`TXFMA_GCVT]) begin
    sel_elxd_normshift_f4a_h  = 1'b1;
    sel_other_normshift_f4a_h = 1'b0;
  end
  
  case (op_dtype_f4a_h)
    `VPU_DTYPE_INT: begin
      if (sigs_f4a_h[`TXFMA_CVT]) begin
        sel_elxd_normshift_f4a_h  = 1'b1;

        if (cmd_f4a_h==`VPU_FCMD_CVT_F32RAST)
          sel_elxd_normshift_f4a_h  = 1'b1;
      end

      if (cmd_f4a_h==`VPU_TRANS_RCP_FMA2) begin 
        tmp_norm_shftamt_f4a_h    = {4'b0,$unsigned(`TXFMA_A0-`TXFMA_B5)};
        sel_other_normshift_f4a_h = 1'b1;
      end

      if (cmd_f4a_h==`VPU_TRANS_RSQRT_FMA2) begin 
        tmp_norm_shftamt_f4a_h    = {4'b0,$unsigned(`TXFMA_A0-`TXFMA_B4)};
        sel_other_normshift_f4a_h = 1'b1;
      end

      if (cmd_f4a_h==`VPU_TRANS_LOG_FMA2) begin
        tmp_norm_shftamt_f4a_h = `TXFMA_NORM_SHF_AMNT_SZ'h7e;
        sel_other_normshift_f4a_h = 1'b1;
      end

      if (cmd_f4a_h==`VPU_TRANS_EXP_FMA2) begin
        tmp_norm_shftamt_f4a_h = {5'b0,$unsigned(`TXFMA_A0-`TXFMA_B1)};   
        sel_other_normshift_f4a_h = 1'b1;
      end

      if (cmd_f4a_h==`VPU_TRANS_SIN_P1 || cmd_f4a_h==`VPU_TRANS_SIN_P2) begin
        case (sin_res_shf_idx_f4a_h)
          3'd0: tmp_norm_shftamt_f4a_h = `TXFMA_NORM_SHF_AMNT_SZ'h7e;
          3'd1: tmp_norm_shftamt_f4a_h =  0;
          3'd2: tmp_norm_shftamt_f4a_h =  1;
          3'd3: tmp_norm_shftamt_f4a_h =  3;
          3'd4: tmp_norm_shftamt_f4a_h =  4;
          3'd5: tmp_norm_shftamt_f4a_h =  5;          
        endcase
        sel_other_normshift_f4a_h = 1'b1;
      end
    end
  
  `VPU_DTYPE_F32,`VPU_DTYPE_F16_F32: begin
      if (~((sigs_f4a_h[`TXFMA_CVT] & (cmd_f4a_h != `VPU_FCMD_FROUND)) | sigs_f4a_h[`TXFMA_NORM_F32] | (cmd_f4a_h==`VPU_TRANS_LOG_RR) | (cmd_f4a_h==`VPU_TRANS_EXP_FRAC))) begin
        sel_other_normshift_f4a_h = 1'b0;
        sel_elxd_normshift_f4a_h  = 1'b1;  
      end
  end
  endcase

  if (sel_other_normshift_f4a_h) 
    norm_shftamt_f4a_h = tmp_norm_shftamt_f4a_h;
  else if (sel_elxd_normshift_f4a_h)
    norm_shftamt_f4a_h = elxd_f4a_h;

end

///////////////////////////////////////////////////////////////////////////////
// Calculate Left/Right Normalization Shift amount 
// NOTE: These signals are driven directly to txfma_f5 where they are flopped
// locally
///////////////////////////////////////////////////////////////////////////////

assign enc_nsamt_f4a_h    = norm_shftamt_f4a_h;

  //exp hi elxd in2 mux sel
assign elxd_add_in2_mux_sel_f4a_h = (cmd_f4a_h!=`VPU_FCMD_CVT_F32INT & cmd_f4a_h!=`VPU_FCMD_CVT_F32UINT & !sigs_f4a_h[`TXFMA_MUL] & cmd_f4a_h!=`VPU_FCMD_CVT_F32RAST) & 
                                    (cmd_f4a_h==`VPU_FCMD_CVT_INTF32 | cmd_f4a_h==`VPU_FCMD_CVT_UINTF32 | cmd_f4a_h==`VPU_FCMD_CVT_RASTF32);

//================================================================================================================
// Normalization Shifter - Calculate shift amount 
//================================================================================================================

assign norm_eff_sub_f4a_h = eff_sub1_f4a_h | eff_sub2_f4a_h;

// Negate prod adder in effective sub: (ediff>=-2 AND sign bit) OR (ediff<-2 AND carry)

// This signal needs to be anded with A3 out of the Combining Adder, which is a very late signal.  
// Setup the conditions and forward to txfma_f4
assign neg_norm_a3_f4a_h = (norm_eff_sub_f4a_h | (cmd_f4a_h==`VPU_TRANS_LOG_MUL));

assign neg_norm_na3_f4a_h = ((s_res_f4a_h & sigs_f4a_h[`TXFMA_NORM_F32]) | ((cmd_f4a_h==`VPU_TRANS_SIN_P1) & (op_dtype_f4a_h==`VPU_DTYPE_INT)) |
                             (sc_f4a_h & (cmd_f4a_h==`VPU_FCMD_CVT_INTF32 | cmd_f4a_h==`VPU_FCMD_CVT_UINTF32 | cmd_f4a_h==`VPU_FCMD_CVT_RASTF32 | cmd_f4a_h==`VPU_TRANS_EXP_FRAC)));


// Input to txfma_f4 for Normalization Shifter Control
assign clear16frnd_nshin_f4a_l = (cmd_f4a_h != `VPU_FCMD_FROUND);
assign clearphps_en_f4a_h      = (op_dtype_f4a_h == `VPU_DTYPE_F16_F32) && ph_comb_stiky_f4a_h;
assign ph_ps_clear_f4a_l       = (op_dtype_f4a_h != `VPU_DTYPE_F16_F32);


endmodule 
