// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_exp_special_detect_tb
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
  output logic                      ea_hi_zero_o,
  output logic                      eb_hi_zero_o,
  output logic                      ec_zero_o,
  output logic                      ea_lo_zero_o,
  output logic                      eb_lo_zero_o,
  output logic                      ea_hi_all_one_o,
  output logic                      eb_hi_all_one_o,
  output logic                      ec_all_one_o,
  output logic                      ea_lo_all_one_o,
  output logic                      eb_lo_all_one_o
);

  txfma_exp_special_detect u_dut (
    .opa                (opa_i),
    .opb                (opb_i),
    .opc                (opc_i),
    .op_dtype           (op_dtype_i),
    .sigs               (sigs_i),
    .cmd                (cmd_i),
    .trans_exp_fma2_exc (trans_exp_fma2_exc_i),
    .trans_taylor       (trans_taylor_i),
    .ea_hi_zero         (ea_hi_zero_o),
    .eb_hi_zero         (eb_hi_zero_o),
    .ec_zero            (ec_zero_o),
    .ea_lo_zero         (ea_lo_zero_o),
    .eb_lo_zero         (eb_lo_zero_o),
    .ea_hi_all_one      (ea_hi_all_one_o),
    .eb_hi_all_one      (eb_hi_all_one_o),
    .ec_all_one         (ec_all_one_o),
    .ea_lo_all_one      (ea_lo_all_one_o),
    .eb_lo_all_one      (eb_lo_all_one_o)
  );

endmodule : txfma_exp_special_detect_tb
