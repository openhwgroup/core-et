// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_norm_shf
  import vpu_pkg::*;
#(
  parameter int left_bit_in   = TxfmaA2,
  parameter int left_bit_out  = TxfmaA0,
  parameter int right_bit_in  = TxfmaB45,
  parameter int right_bit_out = TxfmaB31,
  parameter int shift_size    = TxfmaExpSextPsSz
) (
  input  logic [left_bit_in:right_bit_in]   op_i,
  input  logic [shift_size-1:0]             shfamnt_i,
  output logic [left_bit_out:right_bit_out] op_norm_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // The original forms the full shifted intermediate and then intentionally slices only the normalized output window.
  logic [left_bit_in:right_bit_in] op_norm_ext;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [shift_size-1:0]           shfamnt_abs;

  assign shfamnt_abs = shfamnt_i[shift_size-1] ? -shfamnt_i : shfamnt_i;
  assign op_norm_ext = shfamnt_i[shift_size-1] ? (op_i >> shfamnt_abs) : (op_i << shfamnt_abs);
  assign op_norm_o   = op_norm_ext[left_bit_out:right_bit_out];

endmodule : txfma_norm_shf
