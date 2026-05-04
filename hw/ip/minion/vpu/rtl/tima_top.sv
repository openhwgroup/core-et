// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module tima_top
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                  clk_i,
  input  logic                  f1_mul_en_i,
  input  logic [VpuDataSSz-1:0] f1_in1_i,
  input  logic [VpuDataSSz-1:0] f1_in2_i,
  input  logic                  f1_in1_signed_i,
  input  logic                  f1_in2_signed_i,
  input  logic [VpuDataSSz-1:0] f2_in3_i,
  input  logic                  f2_zap_opd3_i,
  input  logic                  f2_tima_en_i,
  input  logic                  f2_in3_en_i,
  output logic [VpuDataSSz-1:0] f3_out_o
);

  localparam int unsigned ByteWidth    = 8;
  localparam int unsigned Int9Width    = 9;
  localparam int unsigned ProdWidth    = 18;
  localparam int unsigned ProdAddWidth = 19;

  logic [TimaInt8MulNum-1:0]                        f1_in1_sign;
  logic [TimaInt8MulNum-1:0]                        f1_in2_sign;
  logic [VpuDataSSz-1:0]                            f2_in1_q;
  logic [VpuDataSSz-1:0]                            f2_in2_q;
  logic [TimaInt8MulNum-1:0]                        f2_in1_sign_q;
  logic [TimaInt8MulNum-1:0]                        f2_in2_sign_q;
  logic [TimaInt8MulNum-1:0][Int9Width-1:0]         f2_in1_part;
  logic [TimaInt8MulNum-1:0][Int9Width-1:0]         f2_in2_part;
  logic signed [TimaInt8MulNum-1:0][ProdWidth-1:0]  f2_prod;
  logic signed [ProdAddWidth-1:0]                   f2_prod_add;
  logic signed [ProdAddWidth-1:0]                   f3_prod_add_q;
  logic [VpuDataSSz-1:0]                            f3_in3_q;
  logic                                             f3_zap_opd3_q;
  logic [VpuDataSSz-1:0]                            f3_in3_zapped;
  logic [VpuDataSSz-1:0]                            f3_prod_add_sx;
  logic [VpuDataSSz-1:0]                            f3_comb_add;

  always_comb begin
    for (int unsigned lane = 0; lane < TimaInt8MulNum; ++lane) begin
      f1_in1_sign[lane] = f1_in1_signed_i && f1_in1_i[lane*ByteWidth+ByteWidth-1];
      f1_in2_sign[lane] = f1_in2_signed_i && f1_in2_i[lane*ByteWidth+ByteWidth-1];
      f2_in1_part[lane] = {f2_in1_sign_q[lane], f2_in1_q[lane*ByteWidth +: ByteWidth]};
      f2_in2_part[lane] = {f2_in2_sign_q[lane], f2_in2_q[lane*ByteWidth +: ByteWidth]};
      f2_prod[lane] = $signed(f2_in1_part[lane]) * $signed(f2_in2_part[lane]);
    end
  end

  always_comb begin
    f2_prod_add = '0;
    for (int unsigned lane = 0; lane < TimaInt8MulNum; ++lane) begin
      f2_prod_add += {{(ProdAddWidth-ProdWidth){f2_prod[lane][ProdWidth-1]}}, f2_prod[lane]};
    end
  end

  always_ff @(posedge clk_i) begin
    if (f1_mul_en_i) begin
      for (int unsigned lane = 0; lane < TimaInt8MulNum; ++lane) begin
        f2_in1_q[lane*ByteWidth +: ByteWidth] <= f1_in1_i[lane*ByteWidth +: ByteWidth];
        f2_in2_q[lane*ByteWidth +: ByteWidth] <= f1_in2_i[lane*ByteWidth +: ByteWidth];
        f2_in1_sign_q[lane]                   <= f1_in1_sign[lane];
        f2_in2_sign_q[lane]                   <= f1_in2_sign[lane];
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f2_tima_en_i) begin
      f3_prod_add_q <= f2_prod_add;
    end
    if (f2_in3_en_i) begin
      f3_in3_q <= f2_in3_i;
    end
    f3_zap_opd3_q <= f2_zap_opd3_i;
  end

  assign f3_in3_zapped  = {VpuDataSSz{f3_zap_opd3_q}} & f3_in3_q;
  assign f3_prod_add_sx = {{(VpuDataSSz-ProdAddWidth){f3_prod_add_q[ProdAddWidth-1]}}, f3_prod_add_q};

  tima_adder #(
    .Width (VpuDataSSz)
  ) u_comb_adder (
    .a_i   (f3_prod_add_sx),
    .b_i   (f3_in3_zapped),
    .sum_o (f3_comb_add)
  );

  assign f3_out_o = f3_comb_add;

endmodule : tima_top
