// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off SYNCASYNCNET */  // Active-high wrapper intentionally mirrors original sync_rst async assertion.
module sys_gasket_lov_sync_rst #(
  parameter int unsigned Stages = 2
) (
  input  logic clock,
  input  logic reset_in,
  input  logic dft__reset_byp,
  input  logic dft__reset,
  output logic reset_out
);

  logic [Stages-1:0] sync_rst_meta_flops_q;
  logic int_reset_in_n;

  assign int_reset_in_n = dft__reset_byp ? !dft__reset : !reset_in;

  always_ff @(posedge clock or negedge int_reset_in_n) begin
    if (!int_reset_in_n) begin
      sync_rst_meta_flops_q <= {Stages{1'b1}};
    end else begin
      sync_rst_meta_flops_q <= {sync_rst_meta_flops_q[Stages-2:0], 1'b0};
    end
  end

  assign reset_out = dft__reset_byp ? dft__reset : sync_rst_meta_flops_q[Stages-1];

endmodule : sys_gasket_lov_sync_rst
/* verilator lint_on SYNCASYNCNET */
