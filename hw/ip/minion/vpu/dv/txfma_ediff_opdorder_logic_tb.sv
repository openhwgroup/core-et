// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_ediff_opdorder_logic_tb
  import vpu_defs_pkg::*;
(
  input  logic [VPU_FCMD_SZ-1:0]                  cmd_i,
  input  logic [VPU_DTYPE_SZ-1:0]                 op_dtype_i,
  input  logic [TXFMA_SIGS_SZ-1:0]                sigs_i,
  input  logic                                     sign_c_ph_i,
  input  logic                                     sign_c_pl_i,
  input  logic                                     sign_ph_pl_i,
  input  logic                                     exp_fma2_exc_i,
  input  logic [6:0]                               exp_res_2f3_i,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        align1_mux_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        align2_mux_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        opd3_mux_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        ediff1_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        ediff2_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        res_exp_hi_sel_o,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]        res_exp_lo_sel_o
);

  txfma_ediff_opdorder_logic u_dut (
    .cmd              (cmd_i),
    .op_dtype         (op_dtype_i),
    .sigs             (sigs_i),
    .sign_c_ph        (sign_c_ph_i),
    .sign_c_pl        (sign_c_pl_i),
    .sign_ph_pl       (sign_ph_pl_i),
    .exp_fma2_exc     (exp_fma2_exc_i),
    .exp_res_2f3_f2a_h(exp_res_2f3_i),
    .align1_mux_sel   (align1_mux_sel_o),
    .align2_mux_sel   (align2_mux_sel_o),
    .opd3_mux_sel     (opd3_mux_sel_o),
    .ediff1_sel       (ediff1_sel_o),
    .ediff2_sel       (ediff2_sel_o),
    .res_exp_hi_sel   (res_exp_hi_sel_o),
    .res_exp_lo_sel   (res_exp_lo_sel_o)
  );

endmodule : txfma_ediff_opdorder_logic_tb
