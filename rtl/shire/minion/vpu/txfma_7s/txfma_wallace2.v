// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_wallace2
(
  input  [`VPU_DATA_S_SZ-1:0]   Sxt,
  input  [`VPU_DATA_S_SZ-1:0]   Cin,
  input  [32:0]                 pp17,    
  input  [`TXFMA_A3:0]          C21,
  input  [`TXFMA_A3:0]          C22,
  input  [`TXFMA_A3:0]          S21,
  input  [`TXFMA_A3:0]          S22,  
  output [`TXFMA_A3:0]          C41,
  output [`TXFMA_A3:0]          S41  
);

logic [`TXFMA_A3:0] SxtCin;
logic [`TXFMA_A3:0] pp17_ext;
logic [`TXFMA_A3:0] C31;
logic [`TXFMA_A3:0] S31;

assign pp17_ext={pp17[`VPU_DATA_S_SZ-1:0], 32'b0};

assign SxtCin={Sxt, Cin};

// 3. Level 
txfma_4_2_compressor_array #64 csa42_31({C21[62:0], 1'b0}, S21, {C22[62:0], 1'b0}, S22, C31, S31);

// 4. Level
txfma_4_2_compressor_array #64 csa41_41({C31[62:0], 1'b0}, S31, pp17_ext, SxtCin, C41, S41);

endmodule
