// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module tima_top (
  // System signals
  input  logic        clock,
  // F1 inputs
  input	 logic        f1_mul_en,
  input  logic [31:0] f1_in1,
  input  logic [31:0] f1_in2,
  input  logic        f1_in1_signed,
  input  logic        f1_in2_signed,   
  // F2 inputs
  input  logic [31:0] f3_in3,
  input  logic        f2_zap_opd3,
  input  logic        f2_tima_en,   
  // F3 output
  output logic [31:0] f3_out
);


    //internal signals
  logic [`TIMA_INT8_MUL_NUM-1:0]              f1_in1_sign;
  logic [`TIMA_INT8_MUL_NUM-1:0]              f1_in2_sign;
  logic [31:0]                                f2_in1;
  logic [31:0]                                f2_in2;
  logic signed [`TIMA_INT8_MUL_NUM-1:0][17:0] f2_prod;
  logic [18:0]                                f2_prod_add;
  logic [`TIMA_INT8_MUL_NUM-1:0]              f2_in1_sign;
  logic [`TIMA_INT8_MUL_NUM-1:0]              f2_in2_sign;
  logic [`TIMA_INT8_MUL_NUM-1:0][8:0]         f2_in1_part;
  logic [`TIMA_INT8_MUL_NUM-1:0][8:0]         f2_in2_part;
  logic [18:0]                                f3_prod_add;
  logic [31:0]                                f3_comb_add;
  logic                                       f3_zap_opd3;
  logic [31:0]                                f3_in3_zapped;
  logic [31:0]                                f3_prod_add_sx;
  

  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

    //op sign configuration
 
  for (genvar i = 0; i < `TIMA_INT8_MUL_NUM; i++) begin : F1_SIGN 
    //in1/in2 sign bit
    assign f1_in1_sign[i] = !f1_in1_signed ? 1'b0
                          :                  f1_in1[i*8+7];
    assign f1_in2_sign[i] = !f1_in2_signed ? 1'b0
                          :                  f1_in2[i*8+7];
  end


  for (genvar i = 0; i < `TIMA_INT8_MUL_NUM; i++) begin : F1_IN
    //      CLK    EN            DOUT            DIN   
    `EN_FF (clock, f1_mul_en,    f2_in1[i*8+:8], f1_in1[i*8+:8])
    `EN_FF (clock, f1_mul_en,    f2_in2[i*8+:8], f1_in2[i*8+:8])
    `EN_FF (clock, f1_mul_en,    f2_in1_sign[i], f1_in1_sign[i])
    `EN_FF (clock, f1_mul_en,    f2_in2_sign[i], f1_in2_sign[i])    
  end


  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

    //int8 products:
      //   in1[7:0] * in2[7:0]
      //  in1[15:8] * in2[15:8]
      // in1[23:16] * in2[23:16]
      // in1[31:24] * in2[31:24]
  for (genvar i = 0; i < `TIMA_INT8_MUL_NUM; i++) begin : F2_PROD
      //int9 operands
    assign f2_in1_part[i] = {f2_in1_sign[i],f2_in1[i*8+:8]};
    assign f2_in2_part[i] = {f2_in2_sign[i],f2_in2[i*8+:8]};

      //product
    assign f2_prod[i] = $signed(f2_in1_part[i])*$signed(f2_in2_part[i]);
  end

  //partial product addition = (in1[7:0] * in2[7:0]) + (in1[7:0] * in2[7:0]) +
  //                           (in1[7:0] + in2[7:0]) + (in1[7:0] * in2[7:0])
  assign f2_prod_add = `SX(19,f2_prod[0]) + `SX(19,f2_prod[1]) + `SX(19,f2_prod[2]) + `SX(19,f2_prod[3]);


  
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  // F3 Stage
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  //     CLK    EN          DOUT         DIN
  `EN_FF(clock, f2_tima_en, f3_prod_add, f2_prod_add)
  `FF   (clock,             f3_zap_opd3, f2_zap_opd3)

    //in3 zap
  assign f3_in3_zapped = {($bits(f3_in3)){f3_zap_opd3}} & f3_in3;

    //partial product sign extension
  assign f3_prod_add_sx = `SX(32,f3_prod_add);

    //final adder
  tima_adder comb_adder
  (
    .a    ( f3_prod_add_sx ),
    .b    ( f3_in3_zapped  ),
    .sum  ( f3_comb_add    )
  );

    //tima output
  assign f3_out = f3_comb_add;

endmodule


