// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */
// The original Wallace stage drops pp17_i[32] and the top carry bit of c21/c22/c31
// when forming the shifted reduction inputs for the next compressor level.
module txfma_wallace2
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0]   sxt_i,
  input  logic [VpuDataSSz-1:0]   cin_i,
  input  logic [VpuDataSSz:0]     pp17_i,
  input  logic [2*VpuDataSSz-1:0] c21_i,
  input  logic [2*VpuDataSSz-1:0] c22_i,
  input  logic [2*VpuDataSSz-1:0] s21_i,
  input  logic [2*VpuDataSSz-1:0] s22_i,
  output logic [2*VpuDataSSz-1:0] c41_o,
  output logic [2*VpuDataSSz-1:0] s41_o
);

  localparam int unsigned TxfmaAWidth = 2 * VpuDataSSz;

  logic [TxfmaAWidth-1:0] sxt_cin;
  logic [TxfmaAWidth-1:0] pp17_ext;
  logic [TxfmaAWidth-1:0] c31;
  logic [TxfmaAWidth-1:0] s31;

  assign pp17_ext = {pp17_i[VpuDataSSz-1:0], {VpuDataSSz{1'b0}}};
  assign sxt_cin  = {sxt_i, cin_i};

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_31 (
    .x0_i ({c21_i[TxfmaAWidth-2:0], 1'b0}),
    .x1_i (s21_i),
    .x2_i ({c22_i[TxfmaAWidth-2:0], 1'b0}),
    .x3_i (s22_i),
    .c_o  (c31),
    .s_o  (s31)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa41_41 (
    .x0_i ({c31[TxfmaAWidth-2:0], 1'b0}),
    .x1_i (s31),
    .x2_i (pp17_ext),
    .x3_i (sxt_cin),
    .c_o  (c41_o),
    .s_o  (s41_o)
  );

endmodule : txfma_wallace2
/* verilator lint_on UNUSEDSIGNAL */
