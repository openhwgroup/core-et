// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c5
(
  input     [`VPU_DTYPE_SZ-1:0]                 op_dtype_f5a_h,
  input     [`VPU_FCMD_SZ-1:0]                  cmd_f5a_h,
  input     [`TXFMA_A0:`TXFMA_B31]              mergd_frac_res_f5a_h,
  input                                         s_f5a_h,
  input     [`TXFMA_RND_CTL_SZ-1:0]             rnd_ctl_f5a_h,
  input                                         ec_zero_f5a_h,
  output                                        sel_gmaxu24_f5a_h,
  output                                        sel_gmaxu16_f5a_h,
  output                                        sel_gmaxu10_f5a_h,
  output                                        sel_gmaxu8_f5a_h,
  output                                        sel_gmaxu2_f5a_h,
  output                                        sel_gminany_f5a_h,
  output                                        sel_gmaxs16_f5a_h,
  output                                        sel_gmins16_f5a_h,
  output                                        sel_gmaxs8_f5a_h,
  output                                        sel_gmins8_f5a_h,
  output                                        s_res_f5a_h,
  output                                        rnd_ovf_f5a_h
);

logic mergd_all_one_l_hi_ph;
logic mergd_all_one_r_hi_ph;
logic mergd_all_one_l_hi_ps;
logic mergd_all_one_r_hi_ps;
logic mergd_all_one_int;

// Result reduction for the purpose of inspecting round overflow. 3 gate delay
assign mergd_all_one_l_hi_ph = &mergd_frac_res_f5a_h[`TXFMA_A0:`TXFMA_B9];
assign mergd_all_one_r_hi_ph = mergd_all_one_l_hi_ph & mergd_frac_res_f5a_h[`TXFMA_B10];
assign mergd_all_one_l_hi_ps = mergd_all_one_r_hi_ph & (&mergd_frac_res_f5a_h[`TXFMA_B11:`TXFMA_B22]);
assign mergd_all_one_r_hi_ps = mergd_all_one_l_hi_ps & mergd_frac_res_f5a_h[`TXFMA_B23];
assign mergd_all_one_int = mergd_all_one_r_hi_ps & (&mergd_frac_res_f5a_h[`TXFMA_B24:`TXFMA_B31]);


  // We select incremented exponent in case of rounding overflow
assign rnd_ovf_f5a_h = (
                           (rnd_ctl_f5a_h[2] & 
                           (((op_dtype_f5a_h == `VPU_DTYPE_F32 | op_dtype_f5a_h == `VPU_DTYPE_F16_F32)) | cmd_f5a_h ==`VPU_FCMD_CVT_F32INT | 
                            cmd_f5a_h == `VPU_FCMD_CVT_F32UINT | cmd_f5a_h==`VPU_FCMD_CVT_F32RAST) & mergd_all_one_r_hi_ps) |

                           (rnd_ctl_f5a_h[3] & 
                           (((op_dtype_f5a_h == `VPU_DTYPE_F32 | op_dtype_f5a_h == `VPU_DTYPE_F16_F32)) | cmd_f5a_h ==`VPU_FCMD_CVT_F32INT | 
                            cmd_f5a_h == `VPU_FCMD_CVT_F32UINT | cmd_f5a_h==`VPU_FCMD_CVT_F32RAST) & mergd_all_one_l_hi_ps) |

                           (rnd_ctl_f5a_h[4] & 
                           (op_dtype_f5a_h == `VPU_DTYPE_F16 | cmd_f5a_h ==`VPU_FCMD_CVT_F16F32) & mergd_all_one_r_hi_ph) |

                           (rnd_ctl_f5a_h[5] & 
                           (op_dtype_f5a_h == `VPU_DTYPE_F16 | cmd_f5a_h ==`VPU_FCMD_CVT_F16F32) & mergd_all_one_l_hi_ph)
                           );


  // invert sign for NMADD and NMSUB operations 
assign s_res_f5a_h = s_f5a_h;


// generating min/max MUX select for norms
//
assign sel_gmaxu24_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_UN24F32);
assign sel_gmaxu16_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_UN16F32);
assign sel_gmaxu10_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_UN10F32);
assign sel_gmaxu8_f5a_h  = (cmd_f5a_h == `VPU_FCMD_CVT_UN8F32);
assign sel_gmaxu2_f5a_h  = (cmd_f5a_h == `VPU_FCMD_CVT_UN2F32);

assign sel_gminany_f5a_h = ((cmd_f5a_h == `VPU_FCMD_CVT_UN24F32) |
                            (cmd_f5a_h == `VPU_FCMD_CVT_UN16F32) |
                            (cmd_f5a_h == `VPU_FCMD_CVT_UN10F32) |
                            (cmd_f5a_h == `VPU_FCMD_CVT_UN8F32)  |
                            (cmd_f5a_h == `VPU_FCMD_CVT_UN2F32));

assign sel_gmaxs16_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_SN16F32);
assign sel_gmins16_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_SN16F32);

assign sel_gmaxs8_f5a_h  = (cmd_f5a_h == `VPU_FCMD_CVT_SN8F32);
assign sel_gmins8_f5a_h  = (cmd_f5a_h == `VPU_FCMD_CVT_SN8F32);

endmodule 
