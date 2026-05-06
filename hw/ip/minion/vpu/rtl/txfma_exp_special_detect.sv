// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module txfma_exp_special_detect
(
  input  [VPU_DATA_S_SZ-1:0] opa,
  input  [VPU_DATA_S_SZ-1:0] opb,
  input  [VPU_DATA_S_SZ-1:0] opc,
  input  [VPU_DTYPE_SZ-1:0]  op_dtype,
  input  [TXFMA_SIGS_SZ-1:0] sigs,
  input  [VPU_FCMD_SZ-1:0]   cmd,
  input                       trans_exp_fma2_exc,
  input                       trans_taylor,
  output logic                ea_hi_zero,
  output logic                eb_hi_zero,
  output logic                ec_zero,
  output logic                ea_lo_zero,
  output logic                eb_lo_zero,
  output logic                ea_hi_all_one,
  output logic                eb_hi_all_one,
  output logic                ec_all_one,
  output logic                ea_lo_all_one,
  output logic                eb_lo_all_one
);


logic ea_hi_zero30_27;
logic eb_hi_zero30_27;
logic ec_zero30_27;
logic ea_hi_zero26_23;
logic eb_hi_zero26_23;
logic ec_zero26_23;
logic ea_hi_zero14_10;
logic eb_hi_zero14_10;
logic ec_zero14_10;
logic ea_one30_27;
logic eb_one30_27;
logic ec_one30_27;
logic ea_one26_23;
logic eb_one26_23;
logic ec_one26_23;
logic ea_one14_10;
logic eb_one14_10;
logic ec_one14_10;
logic sin_rom_op;


assign ea_hi_zero30_27 = !(|opa[30:27]);
assign eb_hi_zero30_27 = !(|opb[30:27]);
assign ec_zero30_27 = !(|opc[30:27]);

assign ea_hi_zero26_23 = !(|opa[26:23]);
assign eb_hi_zero26_23 = !(|opb[26:23]);
assign ec_zero26_23 = !(|opc[26:23]);

assign ea_hi_zero14_10 = !(|opa[14:10]);
assign eb_hi_zero14_10 = !(|opb[14:10]);
assign ec_zero14_10 = !(|opc[14:10]);

assign ea_one30_27 = &opa[30:27];
assign eb_one30_27 = &opb[30:27];
assign ec_one30_27 = &opc[30:27];

assign ea_one26_23 = &opa[26:23];
assign eb_one26_23 = &opb[26:23];
assign ec_one26_23 = &opc[26:23];

assign ea_one14_10 = &opa[14:10];
assign eb_one14_10 = &opb[14:10];
assign ec_one14_10 = &opc[14:10];

assign sin_rom_op = ((cmd==VPU_TRANS_SIN_P1) & !trans_taylor) | ((cmd==VPU_TRANS_SIN_P2) & !trans_taylor);


///////////////////////////////////////////////////////////////////////////////
// Output format mux
///////////////////////////////////////////////////////////////////////////////

always_comb begin
  ea_hi_zero    = 1'b0;
  eb_hi_zero    = 1'b0;
  ec_zero    = 1'b0;
  ea_lo_zero    = 1'b0;
  eb_lo_zero    = 1'b0;
  ea_hi_all_one = 1'b0;
  eb_hi_all_one = 1'b0;
  ec_all_one = 1'b0;
  ea_lo_all_one = 1'b0;
  eb_lo_all_one = 1'b0;

  if (((op_dtype==VPU_DTYPE_F32) && !sin_rom_op) || (sigs[TXFMA_TRANS2] && !((cmd==VPU_TRANS_EXP_FMA2) && !trans_exp_fma2_exc))) begin
    ea_hi_zero    = ea_hi_zero30_27 & ea_hi_zero26_23 & (cmd!=VPU_TRANS_SIN_P2 & !(cmd==VPU_TRANS_SIN_P3 & !trans_taylor));
    eb_hi_zero    = eb_hi_zero30_27 & eb_hi_zero26_23;
    ec_zero    = ec_zero30_27 & ec_zero26_23 & (cmd!=VPU_TRANS_SIN_P2);
    ea_hi_all_one = ea_one30_27 & ea_one26_23;
    eb_hi_all_one = eb_one30_27 & eb_one26_23;
    ec_all_one = ec_one30_27 & ec_one26_23;
  end

  case (op_dtype)
    VPU_DTYPE_F16 : begin
      ec_zero  = ~(|opc[14:10]);
      ec_all_one  = &opc[14:10];
    end
    VPU_DTYPE_F16_F32 : begin
      ea_hi_zero    = ea_hi_zero30_27 & !opa[26];
      eb_hi_zero    = eb_hi_zero30_27 & !opb[26];
      ea_lo_zero    = ea_hi_zero14_10;
      eb_lo_zero    = eb_hi_zero14_10;
      ea_hi_all_one = ea_one30_27 & opa[26];
      eb_hi_all_one = eb_one30_27 & opb[26];
      ea_lo_all_one = ea_one14_10;
      eb_lo_all_one = eb_one14_10;
      ec_zero    = ec_zero30_27 & ec_zero26_23;
      ec_all_one = ec_one30_27 & ec_one26_23;
    end
    VPU_DTYPE_F11 : begin
      ec_zero    = ~(|opc[10:6]);
      ec_all_one = &opc[10:6];
    end
    VPU_DTYPE_F10 : begin
      ec_zero    = ~(|opc[9:5]);
      ec_all_one = &opc[9:5];
    end
  endcase

    //clear bits for trans log mul: a/b are int, c is f32
  if (cmd==VPU_TRANS_LOG_MUL) begin
    ea_hi_zero = 1'b0;
    eb_hi_zero = 1'b0;
  end

    //clear bits for trans2, except sin
  if (sigs[TXFMA_TRANS2]) begin
    ea_hi_zero = 1'b0;
    ec_zero = 1'b0;
  end

    //inf detection for rcp
  if (cmd==VPU_TRANS_RCP_FMA2) begin
    eb_hi_all_one = eb_one30_27 & eb_one26_23;
  end
end

endmodule
