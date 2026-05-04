// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_rnd_ctl_gen_hi
(
  input                                    fround_sticky_until_l,
  input                                    round_bit_ps,
  input                                    round_bit_ph,  
  input                                    round_bit_int, 
  input                                    l_bit_ps, 
  input                                    l_bit_ph, 
  input                                    l_bit_int, 
  input                                    neg_norm,
  input                                    res_sign_hi,
  input        [`FP_RM_BITS-1:0]           rm,
  input                                    rm_clr,
  input                                    rm_rne,
  input                                    rm_rmm,
  input                                    sticky_until_r,
  input        [`VPU_DTYPE_SZ-1:0]         op_dtype,
  input        [`VPU_FCMD_SZ-1:0]          cmd,  
  input        [`TXFMA_SIGS_SZ-1:0]        sigs,
  output logic [`TXFMA_FTZ_HI_SZ-1:0]      ftz_hi,
  output logic [`TXFMA_RND_CTL_HI_SZ-1:0]  rnd_ctl_hi,
  output logic                             sticky_until_l
);

logic round_bit;
logic l_bit;
logic rne;
logic rup;
logic rdn;
logic rmm;
logic rtz;
logic ps;
logic ph;
logic int32;
logic neg_inc;
logic roundup_r;
logic roundup_r_cvt;
logic roundup_l;
logic roundup_l_cvt;
logic cvt_eff_rne;
logic cvt_eff_rup;
logic cvt_eff_rdn;
logic cvt_eff_rmm;
logic [`FP_RM_BITS-1:0] rm2;
logic trans1_int_roundup_l;
logic trans2_ps_roundup_r;
logic cvt_rastf32_roundup_l;
logic nr2_frcpfxp_roundup_l;


always_comb
begin
  round_bit = 0;
  l_bit = 0;
  if (ps)
  begin
    round_bit = round_bit_ps;
    l_bit     = l_bit_ps;
  end
  if (ph)
  begin
    round_bit = round_bit_ph;
    l_bit     = l_bit_ph;
  end
  if (int32)
  begin
    round_bit = round_bit_int; 
    l_bit     = l_bit_int; 
  end
end

assign sticky_until_l = (cmd ==`VPU_FCMD_FROUND) ? fround_sticky_until_l : sticky_until_r | (round_bit ^ neg_norm);

assign neg_inc = ((ps | ph) & (neg_norm & ~sticky_until_r)) | (int32 & neg_norm);

assign rm2 = rm_clr ?                         RTZ 
           : rm_rne ?                         RNE 
           : rm_rmm ?                         RMM
           : rm;

assign rne   = (rm2 == RNE);
assign rup   = (rm2 == RUP);
assign rdn   = (rm2 == RDN);
assign rmm   = (rm2 == RMM);
assign rtz   = (rm2 == RTZ);

assign ps    = ((op_dtype == `VPU_DTYPE_F32 || op_dtype == `VPU_DTYPE_F16_F32) & ~(sigs[`TXFMA_FX_F32] | sigs[`TXFMA_CVT] | sigs[`TXFMA_NORM_F32] | (cmd==`VPU_TRANS_EXP_FRAC))) | 
                ((op_dtype == `VPU_DTYPE_INT) & sigs[`TXFMA_CVT]) | ((op_dtype != `VPU_DTYPE_F32) & sigs[`TXFMA_F32_NORM]);
assign ph    = ((op_dtype == `VPU_DTYPE_F16) | (cmd == `VPU_FCMD_CVT_F16F32)) & ~sigs[`TXFMA_F32_FX];
assign int32 = (op_dtype == `VPU_DTYPE_F32) & (sigs[`TXFMA_CVT] | sigs[`TXFMA_NORM_F32] | (cmd==`VPU_TRANS_EXP_FRAC));

assign roundup_r = ((rtz | rdn | rup) & neg_inc) | 
                   (rne & ((l_bit & round_bit) | (~l_bit & round_bit & sticky_until_r) | (~l_bit & round_bit & ~neg_inc) | (~l_bit & ~round_bit & ~sticky_until_r & neg_inc))) |
                    (rmm & round_bit & ~neg_inc);
assign roundup_l = (rdn & sticky_until_l & res_sign_hi) | 
                   (rup & sticky_until_l & ~res_sign_hi) | 
                   (rne & ((l_bit ^ round_bit) & ~sticky_until_r & neg_inc)) | 
                   (rmm & neg_inc);

assign cvt_eff_rne = rne & (round_bit ^ neg_norm) & ~((round_bit ^ neg_norm) & ~sticky_until_r & (~l_bit ^ neg_norm));
assign cvt_eff_rup = rup & sticky_until_l;
assign cvt_eff_rdn = rdn & sticky_until_l;
assign cvt_eff_rmm = rmm & (round_bit ^ neg_norm);

assign roundup_r_cvt = 0;
assign roundup_l_cvt = ~res_sign_hi & (cvt_eff_rup | cvt_eff_rne |cvt_eff_rmm) |  neg_inc & ~(cvt_eff_rdn | cvt_eff_rmm | cvt_eff_rne);

assign trans1_int_roundup_l  = sigs[`TXFMA_TRANS1] & (cmd!=`VPU_TRANS_LOG_FMA2);
assign trans2_ps_roundup_r   = sigs[`TXFMA_TRANS2];
assign cvt_rastf32_roundup_l = (cmd == `VPU_FCMD_CVT_RASTF32) & sticky_until_l;
assign nr2_frcpfxp_roundup_l = (cmd==`VPU_FCMD_NR2_FRCPFXP) & neg_norm & ~sticky_until_r;

assign rnd_ctl_hi[0] = roundup_r_cvt & int32 | (cmd==`VPU_TRANS_LOG_FMA2);
assign rnd_ctl_hi[1] = roundup_l_cvt & int32 | trans1_int_roundup_l | cvt_rastf32_roundup_l | nr2_frcpfxp_roundup_l;
assign rnd_ctl_hi[2] = roundup_r & ps        | trans2_ps_roundup_r;
assign rnd_ctl_hi[3] = roundup_l & ps;
assign rnd_ctl_hi[4] = roundup_r & ph;
assign rnd_ctl_hi[5] = roundup_l & ph;

assign ftz_hi[2] = (round_bit ^ neg_norm) & ~sticky_until_r & rne & ph;
assign ftz_hi[1] = (round_bit ^ neg_norm) & ~sticky_until_r & rne & ps;
assign ftz_hi[0] = 0;//(round_bit ^ neg_norm) & ~sticky_until_r & rne & int32;

endmodule
