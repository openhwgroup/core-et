// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_gfx_replicator
(
  input        [`VPU_FCMD_SZ-1:0]       cmd,
  input        [`TXFMA_A1:`TXFMA_B45]   in,
  output logic [`TXFMA_A3:`TXFMA_B45]   out
);

localparam num_rep_24 = (`TXFMA_A1-`TXFMA_B45+1)/24;
localparam num_rep_16 = (`TXFMA_A1-`TXFMA_B45+1)/16;
localparam num_rep_10 = (`TXFMA_A1-`TXFMA_B45+1)/10;
localparam num_rep_8  = (`TXFMA_A1-`TXFMA_B45+1)/8;
localparam num_rep_2  = (`TXFMA_A1-`TXFMA_B45+1)/2;

always_comb begin
  case (cmd)
    `VPU_FCMD_CVT_F32UN24 : out = {2'b0, {num_rep_24{in[`TXFMA_A0:`TXFMA_A0-23]}}};
    `VPU_FCMD_CVT_F32UN16 : out = {2'b0, {num_rep_16{in[`TXFMA_A0:`TXFMA_A0-15]}}};
    `VPU_FCMD_CVT_F32UN10 : out = {2'b0, {num_rep_10{in[`TXFMA_A0:`TXFMA_A0-9]}}, ((`TXFMA_A1-`TXFMA_B45+1)-num_rep_10*10)'(0)};
    `VPU_FCMD_CVT_F32UN8  : out = {2'b0, {num_rep_8{in[`TXFMA_A0:`TXFMA_A0-7]}}};
    `VPU_FCMD_CVT_F32UN2  : out = {2'b0, {num_rep_2{in[`TXFMA_A0:`TXFMA_A0-1]}}};
    `VPU_FCMD_CVT_F32SN16 : out = {2'b0, {num_rep_16{in[`TXFMA_B0:`TXFMA_B0-14]}}, (num_rep_16)'(0)} ;
    `VPU_FCMD_CVT_F32SN8  : out = {2'b0, {num_rep_8{in[`TXFMA_B0:`TXFMA_B0-6]}}, (num_rep_8)'(0)};
    default: out='0;
  endcase
end

endmodule
