// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_wallace1
(
  input  [32:0]               pp1,
  input  [32:0]               pp2,
  input  [32:0]               pp3, 
  input  [32:0]               pp4,
  input  [32:0]               pp5,
  input  [32:0]               pp6,
  input  [32:0]               pp7,
  input  [32:0]               pp8,
  input  [32:0]               pp9, 
  input  [32:0]               pp10,
  input  [32:0]               pp11,
  input  [32:0]               pp12,
  input  [32:0]               pp13,
  input  [32:0]               pp14,
  input  [32:0]               pp15,
  input  [32:0]               pp16,  
  input  [`VPU_DTYPE_SZ-1:0]  op_dtype,
  output logic [`TXFMA_A3:0]  C21,
  output logic [`TXFMA_A3:0]  C22,  
  output logic [`TXFMA_A3:0]  S21,
  output logic [`TXFMA_A3:0]  S22
);

logic [`TXFMA_A3:0] pp1_ext;
logic [`TXFMA_A3:0] pp2_ext;
logic [`TXFMA_A3:0] pp3_ext;
logic [`TXFMA_A3:0] pp4_ext;
logic [`TXFMA_A3:0] pp5_ext;
logic [`TXFMA_A3:0] pp6_ext;
logic [`TXFMA_A3:0] pp7_ext;
logic [`TXFMA_A3:0] pp8_ext;
logic [`TXFMA_A3:0] pp9_ext;
logic [`TXFMA_A3:0] pp10_ext;
logic [`TXFMA_A3:0] pp11_ext;
logic [`TXFMA_A3:0] pp12_ext;
logic [`TXFMA_A3:0] pp13_ext;
logic [`TXFMA_A3:0] pp14_ext;
logic [`TXFMA_A3:0] pp15_ext;
logic [`TXFMA_A3:0] pp16_ext;
logic [`TXFMA_A3:0] c11;
logic [`TXFMA_A3:0] c12;
logic [`TXFMA_A3:0] c13;
logic [`TXFMA_A3:0] c14;
logic [`TXFMA_A3:0] s11;
logic [`TXFMA_A3:0] s12;
logic [`TXFMA_A3:0] s13;
logic [`TXFMA_A3:0] s14;
logic ph;
logic ps;
logic int32;

assign ph = (op_dtype == `VPU_DTYPE_F16_F32);
assign ps  = op_dtype == `VPU_DTYPE_F32;
assign int32 = op_dtype == `VPU_DTYPE_INT;

assign pp1_ext={31'b0, pp1} | {{(`TXFMA_A3-34){1'b0}}, (ps | int32), 33'b0} | {{(`TXFMA_A3-14){1'b0}}, ph, 13'b0}; // magic '1'
assign pp2_ext={29'b0, pp2, 2'b0};
assign pp3_ext={27'b0, pp3, 4'b0};
assign pp4_ext={25'b0, pp4, 6'b0};
assign pp5_ext={23'b0, pp5, 8'b0};
assign pp6_ext={21'b0, pp6, 10'b0};
assign pp7_ext={19'b0, pp7, 12'b0};
assign pp8_ext={17'b0, pp8, 14'b0};
assign pp9_ext={15'b0, pp9, 16'b0};
assign pp10_ext={13'b0, pp10, 18'b0};
assign pp11_ext={11'b0, pp11, 20'b0} | {{(`TXFMA_A3-54){1'b0}}, ph, 53'b0}; // magic '1'
assign pp12_ext={9'b0, pp12, 22'b0};
assign pp13_ext={7'b0, pp13, 24'b0};
assign pp14_ext={5'b0, pp14, 26'b0};
assign pp15_ext={3'b0, pp15, 28'b0};
assign pp16_ext={1'b0, pp16, 30'b0};


// 1. Level 
txfma_4_2_compressor_array #64 csa42_11(pp1_ext, pp2_ext, pp3_ext, pp4_ext, c11, s11);
txfma_4_2_compressor_array #64 csa42_12(pp5_ext, pp6_ext, pp7_ext, pp8_ext, c12, s12);
txfma_4_2_compressor_array #64 csa42_13(pp9_ext, pp10_ext, pp11_ext, pp12_ext, c13, s13);
txfma_4_2_compressor_array #64 csa42_14(pp13_ext, pp14_ext, pp15_ext, pp16_ext, c14, s14);

// 2. Level
txfma_4_2_compressor_array #64 csa42_21({c11[62:0], 1'b0}, s11, {c12[62:0], 1'b0}, s12, C21, S21);
txfma_4_2_compressor_array #64 csa42_22({c13[62:0], 1'b0}, s13, {c14[62:0], 1'b0}, s14, C22, S22);

endmodule
