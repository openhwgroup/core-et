// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// SECDED ECC encoder. Pure combinational.
//
// Generates Hamming SECDED ECC bits for the input data.
// Three configurations based on DataWidth:
//   12–26 bits → EccWidth = 6
//   27–57 bits → EccWidth = 7
//   58–64 bits → EccWidth = 8
//
// Output is {ecc, data} concatenated.
//
// Replaces: ecc_gen (etcore-soc)
//
// Differences from original:
//   - lowRISC naming (_i/_o)
//   - Parameter naming (DataWidth instead of DATA_SIZE)
//   - No `include "macros.vh"` — uses standard SV casts

module prim_ecc_enc #(
  parameter int unsigned DataWidth = 64,
  // Derived — do not override
  localparam int unsigned EccWidth =
    ((DataWidth >= 12) && (DataWidth <= 26)) ? 6 :
    ((DataWidth >= 27) && (DataWidth <= 57)) ? 7 :
    ((DataWidth >= 58) && (DataWidth <= 64)) ? 8 : 0,
  localparam int unsigned DataAndEccWidth = DataWidth + EccWidth
) (
  input  logic [DataWidth-1:0]       data_i,
  output logic [DataAndEccWidth-1:0] data_and_ecc_o  // {ecc, data}
);

  localparam int unsigned MinDataWidth = (EccWidth == 6) ? 12 : (EccWidth == 7) ? 27 : 58;
  localparam int unsigned MaxDataWidth = (EccWidth == 6) ? 26 : (EccWidth == 7) ? 57 : 64;

  // Zero-extend data to max width for the parity matrix
  logic [MaxDataWidth-1:0] d;
  if (DataWidth == MaxDataWidth) begin : gen_max
    assign d = data_i;
  end else if ((DataWidth >= MinDataWidth) && (DataWidth < MaxDataWidth)) begin : gen_ext
    assign d = MaxDataWidth'(data_i);
  end else begin : gen_bad
    assign d = 'x;
  end

  logic [EccWidth-1:0] ecc;
  assign data_and_ecc_o = {ecc, data_i};

  if (EccWidth == 6) begin : gen_ecc6

    assign ecc[0] = ^{d[01],d[03],d[05],d[06],d[09],d[11],d[12],d[15],
                       d[16],d[18],d[21],d[22],d[23],d[24],d[25]};
    assign ecc[1] = ^{d[01],d[03],d[04],d[07],d[09],d[10],d[13],d[14],
                       d[17],d[18],d[20],d[22],d[23],d[24],d[25]};
    assign ecc[2] = ^{d[01],d[02],d[05],d[07],d[08],d[11],d[13],d[14],
                       d[16],d[19],d[20],d[21],d[23],d[24],d[25]};
    assign ecc[3] = ^{d[00],d[03],d[05],d[07],d[08],d[10],d[12],d[15],
                       d[17],d[19],d[20],d[21],d[22],d[24],d[25]};
    assign ecc[4] = ^{d[00],d[02],d[04],d[06],d[09],d[11],d[13],d[15],
                       d[17],d[19],d[20],d[21],d[22],d[23],d[25]};
    assign ecc[5] = ^{d[00],d[02],d[04],d[06],d[08],d[10],d[12],d[14],
                       d[16],d[18],d[20],d[21],d[22],d[23],d[24]};

  end else if (EccWidth == 7) begin : gen_ecc7

    assign ecc[0] = ^{d[02],d[04],d[06],d[09],d[11],d[14],d[15],d[18],
                       d[20],d[23],d[25],d[28],d[30],d[31],d[34],d[36],
                       d[37],d[39],d[40],d[41],d[43],d[44],d[46],d[47],
                       d[48],d[50],d[51],d[52],d[54],d[55],d[56]};
    assign ecc[1] = ^{d[01],d[04],d[06],d[08],d[11],d[14],d[16],d[17],
                       d[20],d[22],d[24],d[26],d[29],d[32],d[33],d[36],
                       d[37],d[38],d[40],d[41],d[42],d[44],d[45],d[47],
                       d[48],d[49],d[50],d[53],d[54],d[55],d[56]};
    assign ecc[2] = ^{d[01],d[03],d[06],d[08],d[10],d[13],d[15],d[18],
                       d[19],d[21],d[25],d[27],d[30],d[32],d[33],d[35],
                       d[37],d[38],d[40],d[41],d[43],d[44],d[45],d[46],
                       d[48],d[49],d[51],d[52],d[53],d[54],d[56]};
    assign ecc[3] = ^{d[01],d[03],d[05],d[07],d[10],d[12],d[15],d[17],
                       d[20],d[22],d[23],d[27],d[29],d[31],d[34],d[35],
                       d[37],d[38],d[39],d[41],d[42],d[44],d[45],d[46],
                       d[47],d[50],d[51],d[52],d[53],d[55],d[56]};
    assign ecc[4] = ^{d[00],d[03],d[05],d[08],d[09],d[12],d[14],d[17],
                       d[19],d[21],d[24],d[26],d[28],d[30],d[34],d[35],
                       d[36],d[38],d[39],d[41],d[42],d[43],d[45],d[47],
                       d[48],d[49],d[51],d[52],d[54],d[55],d[56]};
    assign ecc[5] = ^{d[00],d[02],d[05],d[07],d[10],d[11],d[13],d[16],
                       d[19],d[22],d[24],d[25],d[28],d[31],d[33],d[35],
                       d[36],d[38],d[39],d[40],d[42],d[43],d[44],d[46],
                       d[48],d[49],d[50],d[52],d[53],d[55],d[56]};
    assign ecc[6] = ^{d[00],d[02],d[04],d[07],d[09],d[12],d[13],d[16],
                       d[18],d[21],d[23],d[26],d[27],d[29],d[32],d[35],
                       d[36],d[37],d[39],d[40],d[42],d[43],d[45],d[46],
                       d[47],d[49],d[50],d[51],d[53],d[54],d[56]};

  end else begin : gen_ecc8

    assign ecc[0] = ^{d[02],d[05],d[07],d[10],d[12],d[15],d[18],d[21],
                       d[24],d[26],d[28],d[33],d[35],d[38],d[41],d[44],
                       d[46],d[48],d[50],d[53],d[55],d[57],d[59],d[60],
                       d[62],d[63]};
    assign ecc[1] = ^{d[02],d[04],d[07],d[10],d[13],d[15],d[17],d[20],
                       d[24],d[25],d[29],d[32],d[36],d[37],d[40],d[44],
                       d[47],d[49],d[51],d[52],d[54],d[57],d[58],d[60],
                       d[62],d[63]};
    assign ecc[2] = ^{d[01],d[04],d[07],d[09],d[12],d[14],d[18],d[20],
                       d[23],d[26],d[28],d[30],d[31],d[34],d[41],d[42],
                       d[43],d[45],d[49],d[52],d[54],d[57],d[58],d[60],
                       d[61],d[63]};
    assign ecc[3] = ^{d[01],d[04],d[06],d[09],d[12],d[15],d[16],d[19],
                       d[22],d[25],d[27],d[33],d[35],d[38],d[39],d[40],
                       d[43],d[45],d[47],d[51],d[55],d[56],d[58],d[60],
                       d[61],d[63]};
    assign ecc[4] = ^{d[01],d[03],d[06],d[08],d[11],d[14],d[17],d[20],
                       d[22],d[25],d[28],d[30],d[32],d[35],d[36],d[39],
                       d[42],d[44],d[48],d[50],d[53],d[56],d[58],d[59],
                       d[61],d[63]};
    assign ecc[5] = ^{d[00],d[03],d[06],d[09],d[11],d[14],d[17],d[19],
                       d[21],d[24],d[27],d[29],d[31],d[34],d[37],d[40],
                       d[41],d[46],d[49],d[53],d[55],d[56],d[58],d[59],
                       d[61],d[62]};
    assign ecc[6] = ^{d[00],d[03],d[05],d[08],d[10],d[13],d[16],d[19],
                       d[22],d[23],d[26],d[30],d[32],d[34],d[37],d[38],
                       d[43],d[46],d[48],d[51],d[54],d[56],d[57],d[59],
                       d[61],d[62]};
    assign ecc[7] = ^{d[00],d[02],d[05],d[08],d[11],d[13],d[16],d[18],
                       d[21],d[23],d[27],d[29],d[31],d[33],d[36],d[39],
                       d[42],d[45],d[47],d[50],d[52],d[56],d[57],d[59],
                       d[60],d[62]};

  end

endmodule : prim_ecc_enc
