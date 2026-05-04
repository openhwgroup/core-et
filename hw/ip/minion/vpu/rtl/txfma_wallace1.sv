// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */
// The original Wallace tree shifts the stage-1 carry vectors left by one bit
// and intentionally drops the top carry bit from c11/c12/c13/c14.
module txfma_wallace1
  import minion_pkg::*;
(
  input  logic [VpuDataSSz:0]     pp1_i,
  input  logic [VpuDataSSz:0]     pp2_i,
  input  logic [VpuDataSSz:0]     pp3_i,
  input  logic [VpuDataSSz:0]     pp4_i,
  input  logic [VpuDataSSz:0]     pp5_i,
  input  logic [VpuDataSSz:0]     pp6_i,
  input  logic [VpuDataSSz:0]     pp7_i,
  input  logic [VpuDataSSz:0]     pp8_i,
  input  logic [VpuDataSSz:0]     pp9_i,
  input  logic [VpuDataSSz:0]     pp10_i,
  input  logic [VpuDataSSz:0]     pp11_i,
  input  logic [VpuDataSSz:0]     pp12_i,
  input  logic [VpuDataSSz:0]     pp13_i,
  input  logic [VpuDataSSz:0]     pp14_i,
  input  logic [VpuDataSSz:0]     pp15_i,
  input  logic [VpuDataSSz:0]     pp16_i,
  input  logic                    mode_i,
  output logic [2*VpuDataSSz-1:0] c21_o,
  output logic [2*VpuDataSSz-1:0] c22_o,
  output logic [2*VpuDataSSz-1:0] s21_o,
  output logic [2*VpuDataSSz-1:0] s22_o
);

  localparam int unsigned PpWidth     = VpuDataSSz + 1;
  localparam int unsigned TxfmaAWidth = 2 * VpuDataSSz;

  logic [TxfmaAWidth-1:0] pp1_ext;
  logic [TxfmaAWidth-1:0] pp2_ext;
  logic [TxfmaAWidth-1:0] pp3_ext;
  logic [TxfmaAWidth-1:0] pp4_ext;
  logic [TxfmaAWidth-1:0] pp5_ext;
  logic [TxfmaAWidth-1:0] pp6_ext;
  logic [TxfmaAWidth-1:0] pp7_ext;
  logic [TxfmaAWidth-1:0] pp8_ext;
  logic [TxfmaAWidth-1:0] pp9_ext;
  logic [TxfmaAWidth-1:0] pp10_ext;
  logic [TxfmaAWidth-1:0] pp11_ext;
  logic [TxfmaAWidth-1:0] pp12_ext;
  logic [TxfmaAWidth-1:0] pp13_ext;
  logic [TxfmaAWidth-1:0] pp14_ext;
  logic [TxfmaAWidth-1:0] pp15_ext;
  logic [TxfmaAWidth-1:0] pp16_ext;
  logic [TxfmaAWidth-1:0] c11;
  logic [TxfmaAWidth-1:0] c12;
  logic [TxfmaAWidth-1:0] c13;
  logic [TxfmaAWidth-1:0] c14;
  logic [TxfmaAWidth-1:0] s11;
  logic [TxfmaAWidth-1:0] s12;
  logic [TxfmaAWidth-1:0] s13;
  logic [TxfmaAWidth-1:0] s14;
  logic                   ph;
  logic                   ps_int32;

  // NOTE: since ps/int32 are treated the same, the original combines them and
  // uses ~mode for that path.
  assign ph       = mode_i;
  assign ps_int32 = ~mode_i;

  assign pp1_ext  = {{(TxfmaAWidth-PpWidth){1'b0}}, pp1_i}
                  | {{(TxfmaAWidth-34){1'b0}}, ps_int32, 33'b0}
                  | {{(TxfmaAWidth-14){1'b0}}, ph, 13'b0};
  assign pp2_ext  = {{(TxfmaAWidth-PpWidth-2){1'b0}}, pp2_i, 2'b0};
  assign pp3_ext  = {{(TxfmaAWidth-PpWidth-4){1'b0}}, pp3_i, 4'b0};
  assign pp4_ext  = {{(TxfmaAWidth-PpWidth-6){1'b0}}, pp4_i, 6'b0};
  assign pp5_ext  = {{(TxfmaAWidth-PpWidth-8){1'b0}}, pp5_i, 8'b0};
  assign pp6_ext  = {{(TxfmaAWidth-PpWidth-10){1'b0}}, pp6_i, 10'b0};
  assign pp7_ext  = {{(TxfmaAWidth-PpWidth-12){1'b0}}, pp7_i, 12'b0};
  assign pp8_ext  = {{(TxfmaAWidth-PpWidth-14){1'b0}}, pp8_i, 14'b0};
  assign pp9_ext  = {{(TxfmaAWidth-PpWidth-16){1'b0}}, pp9_i, 16'b0};
  assign pp10_ext = {{(TxfmaAWidth-PpWidth-18){1'b0}}, pp10_i, 18'b0};
  assign pp11_ext = {{(TxfmaAWidth-PpWidth-20){1'b0}}, pp11_i, 20'b0}
                  | {{(TxfmaAWidth-54){1'b0}}, ph, 53'b0};
  assign pp12_ext = {{(TxfmaAWidth-PpWidth-22){1'b0}}, pp12_i, 22'b0};
  assign pp13_ext = {{(TxfmaAWidth-PpWidth-24){1'b0}}, pp13_i, 24'b0};
  assign pp14_ext = {{(TxfmaAWidth-PpWidth-26){1'b0}}, pp14_i, 26'b0};
  assign pp15_ext = {{(TxfmaAWidth-PpWidth-28){1'b0}}, pp15_i, 28'b0};
  assign pp16_ext = {{(TxfmaAWidth-PpWidth-30){1'b0}}, pp16_i, 30'b0};

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_11 (
    .x0_i (pp1_ext),
    .x1_i (pp2_ext),
    .x2_i (pp3_ext),
    .x3_i (pp4_ext),
    .c_o  (c11),
    .s_o  (s11)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_12 (
    .x0_i (pp5_ext),
    .x1_i (pp6_ext),
    .x2_i (pp7_ext),
    .x3_i (pp8_ext),
    .c_o  (c12),
    .s_o  (s12)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_13 (
    .x0_i (pp9_ext),
    .x1_i (pp10_ext),
    .x2_i (pp11_ext),
    .x3_i (pp12_ext),
    .c_o  (c13),
    .s_o  (s13)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_14 (
    .x0_i (pp13_ext),
    .x1_i (pp14_ext),
    .x2_i (pp15_ext),
    .x3_i (pp16_ext),
    .c_o  (c14),
    .s_o  (s14)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_21 (
    .x0_i ({c11[TxfmaAWidth-2:0], 1'b0}),
    .x1_i (s11),
    .x2_i ({c12[TxfmaAWidth-2:0], 1'b0}),
    .x3_i (s12),
    .c_o  (c21_o),
    .s_o  (s21_o)
  );

  txfma_4_2_compressor_array #(
    .Width (TxfmaAWidth)
  ) u_csa42_22 (
    .x0_i ({c13[TxfmaAWidth-2:0], 1'b0}),
    .x1_i (s13),
    .x2_i ({c14[TxfmaAWidth-2:0], 1'b0}),
    .x3_i (s14),
    .c_o  (c22_o),
    .s_o  (s22_o)
  );

endmodule : txfma_wallace1
/* verilator lint_on UNUSEDSIGNAL */
