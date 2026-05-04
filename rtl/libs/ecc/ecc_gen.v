// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements ecc generation for 6, 7 and 8 ecc bits
// 
//   This is a purely combinatorial block
//   
//  If 12 <= DATA_SIZE  <= 26, ECC_SIZE=6
//  If 27 <= DATA_SIZE  <= 57, ECC_SIZE=7
//  If 58 <= DATA_SIZE  <= 64, ECC_SIZE=8
//  
//     (NOTE: although ecc8 can go up to 120, this module only supports
//      ecc8 up to a DATA_SIZE=64 since no application uses more than 64)

module ecc_gen #(parameter 
   DATA_SIZE = 64,
   // below parameters are considered localparams and should not be modified by the instantiation
   ECC_SIZE  = (((DATA_SIZE>=12) && (DATA_SIZE<=26)) ? 6 :
                ((DATA_SIZE>=27) && (DATA_SIZE<=57)) ? 7 :
                ((DATA_SIZE>=58) && (DATA_SIZE<=64)) ? 8 : 32'hx),
   DATA_AND_ECC_SIZE = (DATA_SIZE + ECC_SIZE)
) (
   input  logic [DATA_SIZE-1:0]         data,        // data to be ecc protected
   output logic [DATA_AND_ECC_SIZE-1:0] data_and_ecc // NOTE: concatenated output = {ecc, data}
);

   localparam MIN_DATA_SIZE = ((ECC_SIZE==6) ? 12 : (ECC_SIZE==7) ? 27 : 58); // ecc6,7,8
   localparam MAX_DATA_SIZE = ((ECC_SIZE==6) ? 26 : (ECC_SIZE==7) ? 57 : 64); // ecc6,7,8 

   logic [MAX_DATA_SIZE-1:0] int_data;
   generate
      if (DATA_SIZE==MAX_DATA_SIZE) begin : max_size
         assign int_data = data;
      end
      else if ((DATA_SIZE>=MIN_DATA_SIZE) && (DATA_SIZE<MAX_DATA_SIZE)) begin : legal_size_not_max
         assign int_data = `ZX(MAX_DATA_SIZE, data);
      end
      else begin : illegal_size
         assign int_data = 'x;
      end
   endgenerate

   logic [ECC_SIZE-1:0] ecc;
   assign data_and_ecc = {ecc, data};

   // ecc generation rom
   generate
      if (ECC_SIZE == 6) begin : ecc6

         assign ecc[0] = ^ {
            int_data[01], int_data[03], int_data[05], int_data[06], int_data[09], int_data[11], int_data[12], int_data[15],
            int_data[16], int_data[18], int_data[21], int_data[22], int_data[23], int_data[24], int_data[25]};

         assign ecc[1] = ^ {
            int_data[01], int_data[03], int_data[04], int_data[07], int_data[09], int_data[10], int_data[13], int_data[14],
            int_data[17], int_data[18], int_data[20], int_data[22], int_data[23], int_data[24], int_data[25]};

         assign ecc[2] = ^ {
            int_data[01], int_data[02], int_data[05], int_data[07], int_data[08], int_data[11], int_data[13], int_data[14],
            int_data[16], int_data[19], int_data[20], int_data[21], int_data[23], int_data[24], int_data[25]};

         assign ecc[3] = ^ {
            int_data[00], int_data[03], int_data[05], int_data[07], int_data[08], int_data[10], int_data[12], int_data[15],
            int_data[17], int_data[19], int_data[20], int_data[21], int_data[22], int_data[24], int_data[25]};

         assign ecc[4] = ^ {
            int_data[00], int_data[02], int_data[04], int_data[06], int_data[09], int_data[11], int_data[13], int_data[15],
            int_data[17], int_data[19], int_data[20], int_data[21], int_data[22], int_data[23], int_data[25]};

         assign ecc[5] = ^ {
            int_data[00], int_data[02], int_data[04], int_data[06], int_data[08], int_data[10], int_data[12], int_data[14],
            int_data[16], int_data[18], int_data[20], int_data[21], int_data[22], int_data[23], int_data[24]};

      end
      else if (ECC_SIZE == 7) begin : ecc7

         assign ecc[0] = ^ {
            int_data[02], int_data[04], int_data[06], int_data[09], int_data[11], int_data[14], int_data[15], int_data[18],
            int_data[20], int_data[23], int_data[25], int_data[28], int_data[30], int_data[31], int_data[34], int_data[36],
            int_data[37], int_data[39], int_data[40], int_data[41], int_data[43], int_data[44], int_data[46], int_data[47],
            int_data[48], int_data[50], int_data[51], int_data[52], int_data[54], int_data[55], int_data[56]};

         assign ecc[1] = ^ {
            int_data[01], int_data[04], int_data[06], int_data[08], int_data[11], int_data[14], int_data[16], int_data[17],
            int_data[20], int_data[22], int_data[24], int_data[26], int_data[29], int_data[32], int_data[33], int_data[36],
            int_data[37], int_data[38], int_data[40], int_data[41], int_data[42], int_data[44], int_data[45], int_data[47],
            int_data[48], int_data[49], int_data[50], int_data[53], int_data[54], int_data[55], int_data[56]};

         assign ecc[2] = ^ {
            int_data[01], int_data[03], int_data[06], int_data[08], int_data[10], int_data[13], int_data[15], int_data[18],
            int_data[19], int_data[21], int_data[25], int_data[27], int_data[30], int_data[32], int_data[33], int_data[35],
            int_data[37], int_data[38], int_data[40], int_data[41], int_data[43], int_data[44], int_data[45], int_data[46],
            int_data[48], int_data[49], int_data[51], int_data[52], int_data[53], int_data[54], int_data[56]};

         assign ecc[3] = ^ {
            int_data[01], int_data[03], int_data[05], int_data[07], int_data[10], int_data[12], int_data[15], int_data[17],
            int_data[20], int_data[22], int_data[23], int_data[27], int_data[29], int_data[31], int_data[34], int_data[35],
            int_data[37], int_data[38], int_data[39], int_data[41], int_data[42], int_data[44], int_data[45], int_data[46],
            int_data[47], int_data[50], int_data[51], int_data[52], int_data[53], int_data[55], int_data[56]};

         assign ecc[4] = ^ {
            int_data[00], int_data[03], int_data[05], int_data[08], int_data[09], int_data[12], int_data[14], int_data[17],
            int_data[19], int_data[21], int_data[24], int_data[26], int_data[28], int_data[30], int_data[34], int_data[35],
            int_data[36], int_data[38], int_data[39], int_data[41], int_data[42], int_data[43], int_data[45], int_data[47],
            int_data[48], int_data[49], int_data[51], int_data[52], int_data[54], int_data[55], int_data[56]};

         assign ecc[5] = ^ {
            int_data[00], int_data[02], int_data[05], int_data[07], int_data[10], int_data[11], int_data[13], int_data[16],
            int_data[19], int_data[22], int_data[24], int_data[25], int_data[28], int_data[31], int_data[33], int_data[35],
            int_data[36], int_data[38], int_data[39], int_data[40], int_data[42], int_data[43], int_data[44], int_data[46],
            int_data[48], int_data[49], int_data[50], int_data[52], int_data[53], int_data[55], int_data[56]};

         assign ecc[6] = ^ {
            int_data[00], int_data[02], int_data[04], int_data[07], int_data[09], int_data[12], int_data[13], int_data[16],
            int_data[18], int_data[21], int_data[23], int_data[26], int_data[27], int_data[29], int_data[32], int_data[35],
            int_data[36], int_data[37], int_data[39], int_data[40], int_data[42], int_data[43], int_data[45], int_data[46],
            int_data[47], int_data[49], int_data[50], int_data[51], int_data[53], int_data[54], int_data[56]};

      end
      else begin : ecc8

         assign ecc[0] = ^ {
            int_data[02], int_data[05], int_data[07], int_data[10], int_data[12], int_data[15], int_data[18], int_data[21],
            int_data[24], int_data[26], int_data[28], int_data[33], int_data[35], int_data[38], int_data[41], int_data[44],
            int_data[46], int_data[48], int_data[50], int_data[53], int_data[55], int_data[57], int_data[59], int_data[60],
            int_data[62], int_data[63]};

         assign ecc[1] = ^ {
            int_data[02], int_data[04], int_data[07], int_data[10], int_data[13], int_data[15], int_data[17], int_data[20],
            int_data[24], int_data[25], int_data[29], int_data[32], int_data[36], int_data[37], int_data[40], int_data[44],
            int_data[47], int_data[49], int_data[51], int_data[52], int_data[54], int_data[57], int_data[58], int_data[60],
            int_data[62], int_data[63]};

         assign ecc[2] = ^ {
            int_data[01], int_data[04], int_data[07], int_data[09], int_data[12], int_data[14], int_data[18], int_data[20],
            int_data[23], int_data[26], int_data[28], int_data[30], int_data[31], int_data[34], int_data[41], int_data[42],
            int_data[43], int_data[45], int_data[49], int_data[52], int_data[54], int_data[57], int_data[58], int_data[60],
            int_data[61], int_data[63]};

         assign ecc[3] = ^ {
            int_data[01], int_data[04], int_data[06], int_data[09], int_data[12], int_data[15], int_data[16], int_data[19],
            int_data[22], int_data[25], int_data[27], int_data[33], int_data[35], int_data[38], int_data[39], int_data[40],
            int_data[43], int_data[45], int_data[47], int_data[51], int_data[55], int_data[56], int_data[58], int_data[60],
            int_data[61], int_data[63]};

         assign ecc[4] = ^ {
            int_data[01], int_data[03], int_data[06], int_data[08], int_data[11], int_data[14], int_data[17], int_data[20],
            int_data[22], int_data[25], int_data[28], int_data[30], int_data[32], int_data[35], int_data[36], int_data[39],
            int_data[42], int_data[44], int_data[48], int_data[50], int_data[53], int_data[56], int_data[58], int_data[59],
            int_data[61], int_data[63]};

         assign ecc[5] = ^ {
            int_data[00], int_data[03], int_data[06], int_data[09], int_data[11], int_data[14], int_data[17], int_data[19],
            int_data[21], int_data[24], int_data[27], int_data[29], int_data[31], int_data[34], int_data[37], int_data[40],
            int_data[41], int_data[46], int_data[49], int_data[53], int_data[55], int_data[56], int_data[58], int_data[59],
            int_data[61], int_data[62]};

         assign ecc[6] = ^ {
            int_data[00], int_data[03], int_data[05], int_data[08], int_data[10], int_data[13], int_data[16], int_data[19],
            int_data[22], int_data[23], int_data[26], int_data[30], int_data[32], int_data[34], int_data[37], int_data[38],
            int_data[43], int_data[46], int_data[48], int_data[51], int_data[54], int_data[56], int_data[57], int_data[59],
            int_data[61], int_data[62]};

         assign ecc[7] = ^ {
            int_data[00], int_data[02], int_data[05], int_data[08], int_data[11], int_data[13], int_data[16], int_data[18],
            int_data[21], int_data[23], int_data[27], int_data[29], int_data[31], int_data[33], int_data[36], int_data[39],
            int_data[42], int_data[45], int_data[47], int_data[50], int_data[52], int_data[56], int_data[57], int_data[59],
            int_data[60], int_data[62]};

      end
   endgenerate
                


endmodule
