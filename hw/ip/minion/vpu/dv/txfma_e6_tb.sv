// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_e6_tb
  import vpu_defs_pkg::*;
(
  input  logic [TXFMA_EXP_SEXT_PS_SZ-1:0] exp_res_in_i,
  input  logic                             ps_rndovf_i,
  input  logic                             ph_rndovf_i,
  input  logic [VPU_FCMD_SZ-1:0]           cmd_i,
  input  logic [TXFMA_SIGS_SZ-1:0]         sigs_i,
  input  logic [VPU_DTYPE_SZ-1:0]          op_dtype_i,
  output logic                             def_ovf_ps_o,
  output logic                             pot_ovf_ps_o,
  output logic                             def_unf_ps_o,
  output logic                             pot_unf_ps_o,
  output logic                             def_ovf_ph_o,
  output logic                             pot_ovf_ph_o,
  output logic                             def_unf_ph_o,
  output logic                             pot_unf_ph_o,
  output logic [TXFMA_EXP_PS_SZ-1:0]       exp_res_o,
  output logic                             exp_res_zero_o
);

  txfma_e6 u_dut (
    .exp_res_in_f6a_h  (exp_res_in_i),
    .ps_rndovf_f6a_h   (ps_rndovf_i),
    .ph_rndovf_f6a_h   (ph_rndovf_i),
    .cmd_f6a_h         (cmd_i),
    .sigs_f6a_h        (sigs_i),
    .op_dtype_f6a_h    (op_dtype_i),
    .def_ovf_ps_f6a_h  (def_ovf_ps_o),
    .pot_ovf_ps_f6a_h  (pot_ovf_ps_o),
    .def_unf_ps_f6a_h  (def_unf_ps_o),
    .pot_unf_ps_f6a_h  (pot_unf_ps_o),
    .def_ovf_ph_f6a_h  (def_ovf_ph_o),
    .pot_ovf_ph_f6a_h  (pot_ovf_ph_o),
    .def_unf_ph_f6a_h  (def_unf_ph_o),
    .pot_unf_ph_f6a_h  (pot_unf_ph_o),
    .exp_res_f6a_h     (exp_res_o),
    .exp_res_zero_f6a_h(exp_res_zero_o)
  );

endmodule : txfma_e6_tb
