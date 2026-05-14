// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_exp_special_detect_tb
  import vpu_defs_pkg::*;
(
  input  logic [VPU_DATA_S_SZ-1:0] opa_i,
  input  logic [VPU_DATA_S_SZ-1:0] opb_i,
  input  logic [VPU_DATA_S_SZ-1:0] opc_i,
  input  logic [VPU_DTYPE_SZ-1:0]  op_dtype_i,
  input  logic [TXFMA_SIGS_SZ-1:0] sigs_i,
  input  logic [VPU_FCMD_SZ-1:0]   cmd_i,
  input  logic                      trans_exp_fma2_exc_i,
  input  logic                      trans_taylor_i,
  output logic                      orig_ea_hi_zero_o,
  output logic                      orig_eb_hi_zero_o,
  output logic                      orig_ec_zero_o,
  output logic                      orig_ea_lo_zero_o,
  output logic                      orig_eb_lo_zero_o,
  output logic                      orig_ea_hi_all_one_o,
  output logic                      orig_eb_hi_all_one_o,
  output logic                      orig_ec_all_one_o,
  output logic                      orig_ea_lo_all_one_o,
  output logic                      orig_eb_lo_all_one_o,
  output logic                      new_ea_hi_zero_o,
  output logic                      new_eb_hi_zero_o,
  output logic                      new_ec_zero_o,
  output logic                      new_ea_lo_zero_o,
  output logic                      new_eb_lo_zero_o,
  output logic                      new_ea_hi_all_one_o,
  output logic                      new_eb_hi_all_one_o,
  output logic                      new_ec_all_one_o,
  output logic                      new_ea_lo_all_one_o,
  output logic                      new_eb_lo_all_one_o
);

  txfma_exp_special_detect_orig u_orig (
    .opa               (opa_i),
    .opb               (opb_i),
    .opc               (opc_i),
    .op_dtype          (op_dtype_i),
    .sigs              (sigs_i),
    .cmd               (cmd_i),
    .trans_exp_fma2_exc(trans_exp_fma2_exc_i),
    .trans_taylor      (trans_taylor_i),
    .ea_hi_zero        (orig_ea_hi_zero_o),
    .eb_hi_zero        (orig_eb_hi_zero_o),
    .ec_zero           (orig_ec_zero_o),
    .ea_lo_zero        (orig_ea_lo_zero_o),
    .eb_lo_zero        (orig_eb_lo_zero_o),
    .ea_hi_all_one     (orig_ea_hi_all_one_o),
    .eb_hi_all_one     (orig_eb_hi_all_one_o),
    .ec_all_one        (orig_ec_all_one_o),
    .ea_lo_all_one     (orig_ea_lo_all_one_o),
    .eb_lo_all_one     (orig_eb_lo_all_one_o)
  );

  txfma_exp_special_detect u_new (
    .opa               (opa_i),
    .opb               (opb_i),
    .opc               (opc_i),
    .op_dtype          (op_dtype_i),
    .sigs              (sigs_i),
    .cmd               (cmd_i),
    .trans_exp_fma2_exc(trans_exp_fma2_exc_i),
    .trans_taylor      (trans_taylor_i),
    .ea_hi_zero        (new_ea_hi_zero_o),
    .eb_hi_zero        (new_eb_hi_zero_o),
    .ec_zero           (new_ec_zero_o),
    .ea_lo_zero        (new_ea_lo_zero_o),
    .eb_lo_zero        (new_eb_lo_zero_o),
    .ea_hi_all_one     (new_ea_hi_all_one_o),
    .eb_hi_all_one     (new_eb_hi_all_one_o),
    .ec_all_one        (new_ec_all_one_o),
    .ea_lo_all_one     (new_ea_lo_all_one_o),
    .eb_lo_all_one     (new_eb_lo_all_one_o)
  );

endmodule : cosim_txfma_exp_special_detect_tb
