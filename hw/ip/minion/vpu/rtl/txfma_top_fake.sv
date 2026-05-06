// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import minion_pkg::*;
import vpu_pkg::*;
/* verilator lint_on IMPORTSTAR */

module txfma_top_fake (
  input  logic                  clock,
  input  logic                  reset,
  input  logic                  in_valid,
  input  vpu_pkg::vpu_input_t            in_data,
  input  vpu_pkg::trans_fma_coefficients trans_coefficients,
  output vpu_pkg::vpu_output_t           out_data_res,
  output logic                  out_comp_res
);

  import "DPI-C" pure function int dpi_TXFMA_EMU(
    input bit [31:0] src1,
    input bit [31:0] src2,
    input bit [31:0] src3,
    input bit [6:0]  cmd,
    input bit [3:0]  dtype,
    input bit [2:0]  rm,
    output bit [5:0] out_flags
  );

  /* verilator lint_off UNUSEDSIGNAL */  // The fake path only consumes the arithmetic command, dtype, rm, and input operands from the packed VPU request.
  vpu_pkg::vpu_input_t f2_bits_q;
  vpu_pkg::vpu_input_t f3_bits_q;
  vpu_pkg::vpu_input_t f4_bits_q;
  vpu_pkg::vpu_input_t f5_bits_q;
  vpu_pkg::vpu_input_t f6_bits_q;
  vpu_pkg::vpu_input_t f7_bits_q;
  /* verilator lint_on UNUSEDSIGNAL */

  logic f2_valid_q;
  logic f3_valid_q;
  logic f4_valid_q;
  logic f5_valid_q;
  logic f6_valid_q;
  logic f7_valid_q;

  bit [5:0] out_flags;
  /* verilator lint_off UNUSEDSIGNAL */  // The fake model keeps the same parent interface as the real TXFMA top, but does not consume ROM coefficients.
  vpu_pkg::trans_fma_coefficients trans_coefficients_unused;
  /* verilator lint_on UNUSEDSIGNAL */

  assign trans_coefficients_unused = trans_coefficients;
  assign out_comp_res = 1'b0;

  always_ff @(posedge clock) begin
    if (reset) begin
      f2_valid_q <= 1'b0;
      f3_valid_q <= 1'b0;
      f4_valid_q <= 1'b0;
      f5_valid_q <= 1'b0;
      f6_valid_q <= 1'b0;
      f7_valid_q <= 1'b0;
    end else begin
      f2_valid_q <= in_valid;
      f3_valid_q <= f2_valid_q;
      f4_valid_q <= f3_valid_q;
      f5_valid_q <= f4_valid_q;
      f6_valid_q <= f5_valid_q;
      f7_valid_q <= f6_valid_q;
    end

    f2_bits_q <= in_data;
    f3_bits_q <= f2_bits_q;
    f4_bits_q <= f3_bits_q;
    f5_bits_q <= f4_bits_q;
    f6_bits_q <= f5_bits_q;
    f7_bits_q <= f6_bits_q;
  end

  always_ff @(posedge clock) begin
    if (f7_valid_q) begin
      out_data_res.data[31:0] <= dpi_TXFMA_EMU(
        f7_bits_q.in1[31:0],
        f7_bits_q.in2[31:0],
        f7_bits_q.in3[31:0],
        f7_bits_q.sigs.cmd,
        f7_bits_q.sigs.dtype,
        f7_bits_q.rm,
        out_flags
      );
      out_data_res.exc <= out_flags;
    end
  end

endmodule : txfma_top_fake
