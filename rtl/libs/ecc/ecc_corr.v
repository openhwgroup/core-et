// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements ecc correction for 6, 7 and 8 ecc bits
//
//  This is a purely combinatorial block
//  
//  If 12 <= DATA_SIZE  <= 26, ECC_SIZE=6
//  If 27 <= DATA_SIZE  <= 57, ECC_SIZE=7
//  If 58 <= DATA_SIZE  <= 64, ECC_SIZE=8
//  
//     (NOTE: although ecc8 can go up to 120, this module only supports
//      ecc8 up to a DATA_SIZE=64 since no application uses more than 64)

module ecc_corr #(parameter 
   DATA_SIZE = 64,
   // below parameters are considered localparams and should not be modified by the instantiation
   ECC_SIZE  = (((DATA_SIZE>=12) && (DATA_SIZE<=26)) ? 6 :
                ((DATA_SIZE>=27) && (DATA_SIZE<=57)) ? 7 :
                ((DATA_SIZE>=58) && (DATA_SIZE<=64)) ? 8 : 32'hx),
   DATA_AND_ECC_SIZE = (DATA_SIZE + ECC_SIZE)
) (
   input  logic [DATA_AND_ECC_SIZE-1:0]  data_and_ecc,   // NOTE: concatenated input = {ecc, data}
   output logic [DATA_SIZE-1:0]          data_corr,      // corrected data
   output logic                          sbe,            // single bit error indication
   output logic                          dbe             // double bit error indication
);


   localparam MIN_DATA_SIZE = ((ECC_SIZE==6) ? 12 : (ECC_SIZE==7) ? 27 : 58); // ecc6,7,8
   localparam MAX_DATA_SIZE = ((ECC_SIZE==6) ? 26 : (ECC_SIZE==7) ? 57 : 64); // ecc6,7,8 (NOTE: ecc8 can go up to 120, but this module only supports up to 64 since no application uses more than 64)

   logic [DATA_SIZE-1:0] data;
   logic [ECC_SIZE-1:0]  ecc;
   assign {ecc, data} = data_and_ecc;

   logic [DATA_SIZE-1:0] data_unused;
   logic [ECC_SIZE-1:0]  expected_ecc;
   ecc_gen #(.DATA_SIZE(DATA_SIZE), .ECC_SIZE (ECC_SIZE)) expected_ecc_gen (
      .data        (  data                       ),
      .data_and_ecc( {expected_ecc, data_unused} )
   );

   wire [DATA_SIZE-1:0] bit_flip; // assigned below
   wire [ECC_SIZE-1:0]  ecc_check = ecc ^ expected_ecc;
   wire                 any_err   = |ecc_check;

   // assign outputs
   assign data_corr = data ^ bit_flip;
   assign sbe = ^ecc_check;
   assign dbe = any_err && !sbe;


   // look up table for bit_flip
   logic [MAX_DATA_SIZE-1:0] int_bit_flip;
   generate
      if (ECC_SIZE==6) begin : ecc6
         always_comb begin
            case (ecc_check)
               6'h38  :  int_bit_flip = 26'h0000001;  // 00
               6'h07  :  int_bit_flip = 26'h0000002;  // 01
               6'h34  :  int_bit_flip = 26'h0000004;  // 02
               6'h0b  :  int_bit_flip = 26'h0000008;  // 03
               6'h32  :  int_bit_flip = 26'h0000010;  // 04
               6'h0d  :  int_bit_flip = 26'h0000020;  // 05
               6'h31  :  int_bit_flip = 26'h0000040;  // 06
               6'h0e  :  int_bit_flip = 26'h0000080;  // 07
               6'h2c  :  int_bit_flip = 26'h0000100;  // 08
               6'h13  :  int_bit_flip = 26'h0000200;  // 09
               6'h2a  :  int_bit_flip = 26'h0000400;  // 10
               6'h15  :  int_bit_flip = 26'h0000800;  // 11
               6'h29  :  int_bit_flip = 26'h0001000;  // 12
               6'h16  :  int_bit_flip = 26'h0002000;  // 13
               6'h26  :  int_bit_flip = 26'h0004000;  // 14
               6'h19  :  int_bit_flip = 26'h0008000;  // 15
               6'h25  :  int_bit_flip = 26'h0010000;  // 16
               6'h1a  :  int_bit_flip = 26'h0020000;  // 17
               6'h23  :  int_bit_flip = 26'h0040000;  // 18
               6'h1c  :  int_bit_flip = 26'h0080000;  // 19
               6'h3e  :  int_bit_flip = 26'h0100000;  // 20
               6'h3d  :  int_bit_flip = 26'h0200000;  // 21
               6'h3b  :  int_bit_flip = 26'h0400000;  // 22
               6'h37  :  int_bit_flip = 26'h0800000;  // 23
               6'h2f  :  int_bit_flip = 26'h1000000;  // 24
               6'h1f  :  int_bit_flip = 26'h2000000;  // 25
               default:  int_bit_flip = 26'h0000000;
            endcase
         end // always_comb
      end    // ecc6
      else if (ECC_SIZE==7) begin : ecc7
         always_comb begin
            case (ecc_check)
               7'h70  :  int_bit_flip = 57'h000000000000001;  // 00
               7'h0e  :  int_bit_flip = 57'h000000000000002;  // 01
               7'h61  :  int_bit_flip = 57'h000000000000004;  // 02
               7'h1c  :  int_bit_flip = 57'h000000000000008;  // 03
               7'h43  :  int_bit_flip = 57'h000000000000010;  // 04
               7'h38  :  int_bit_flip = 57'h000000000000020;  // 05
               7'h07  :  int_bit_flip = 57'h000000000000040;  // 06
               7'h68  :  int_bit_flip = 57'h000000000000080;  // 07
               7'h16  :  int_bit_flip = 57'h000000000000100;  // 08
               7'h51  :  int_bit_flip = 57'h000000000000200;  // 09
               7'h2c  :  int_bit_flip = 57'h000000000000400;  // 10
               7'h23  :  int_bit_flip = 57'h000000000000800;  // 11
               7'h58  :  int_bit_flip = 57'h000000000001000;  // 12
               7'h64  :  int_bit_flip = 57'h000000000002000;  // 13
               7'h13  :  int_bit_flip = 57'h000000000004000;  // 14
               7'h0d  :  int_bit_flip = 57'h000000000008000;  // 15
               7'h62  :  int_bit_flip = 57'h000000000010000;  // 16
               7'h1a  :  int_bit_flip = 57'h000000000020000;  // 17
               7'h45  :  int_bit_flip = 57'h000000000040000;  // 18
               7'h34  :  int_bit_flip = 57'h000000000080000;  // 19
               7'h0b  :  int_bit_flip = 57'h000000000100000;  // 20
               7'h54  :  int_bit_flip = 57'h000000000200000;  // 21
               7'h2a  :  int_bit_flip = 57'h000000000400000;  // 22
               7'h49  :  int_bit_flip = 57'h000000000800000;  // 23
               7'h32  :  int_bit_flip = 57'h000000001000000;  // 24
               7'h25  :  int_bit_flip = 57'h000000002000000;  // 25
               7'h52  :  int_bit_flip = 57'h000000004000000;  // 26
               7'h4c  :  int_bit_flip = 57'h000000008000000;  // 27
               7'h31  :  int_bit_flip = 57'h000000010000000;  // 28
               7'h4a  :  int_bit_flip = 57'h000000020000000;  // 29
               7'h15  :  int_bit_flip = 57'h000000040000000;  // 30
               7'h29  :  int_bit_flip = 57'h000000080000000;  // 31
               7'h46  :  int_bit_flip = 57'h000000100000000;  // 32
               7'h26  :  int_bit_flip = 57'h000000200000000;  // 33
               7'h19  :  int_bit_flip = 57'h000000400000000;  // 34
               7'h7c  :  int_bit_flip = 57'h000000800000000;  // 35
               7'h73  :  int_bit_flip = 57'h000001000000000;  // 36
               7'h4f  :  int_bit_flip = 57'h000002000000000;  // 37
               7'h3e  :  int_bit_flip = 57'h000004000000000;  // 38
               7'h79  :  int_bit_flip = 57'h000008000000000;  // 39
               7'h67  :  int_bit_flip = 57'h000010000000000;  // 40
               7'h1f  :  int_bit_flip = 57'h000020000000000;  // 41
               7'h7a  :  int_bit_flip = 57'h000040000000000;  // 42
               7'h75  :  int_bit_flip = 57'h000080000000000;  // 43
               7'h2f  :  int_bit_flip = 57'h000100000000000;  // 44
               7'h5e  :  int_bit_flip = 57'h000200000000000;  // 45
               7'h6d  :  int_bit_flip = 57'h000400000000000;  // 46
               7'h5b  :  int_bit_flip = 57'h000800000000000;  // 47
               7'h37  :  int_bit_flip = 57'h001000000000000;  // 48
               7'h76  :  int_bit_flip = 57'h002000000000000;  // 49
               7'h6b  :  int_bit_flip = 57'h004000000000000;  // 50
               7'h5d  :  int_bit_flip = 57'h008000000000000;  // 51
               7'h3d  :  int_bit_flip = 57'h010000000000000;  // 52
               7'h6e  :  int_bit_flip = 57'h020000000000000;  // 53
               7'h57  :  int_bit_flip = 57'h040000000000000;  // 54
               7'h3b  :  int_bit_flip = 57'h080000000000000;  // 55
               7'h7f  :  int_bit_flip = 57'h100000000000000;  // 56
               default:  int_bit_flip = 57'h000000000000000;
            endcase
         end // always_comb
      end    // ecc7
      else begin : ecc8
         always_comb begin
            case (ecc_check)
               8'he0  :  int_bit_flip = 64'h0000000000000001;  // 00
               8'h1c  :  int_bit_flip = 64'h0000000000000002;  // 01
               8'h83  :  int_bit_flip = 64'h0000000000000004;  // 02
               8'h70  :  int_bit_flip = 64'h0000000000000008;  // 03
               8'h0e  :  int_bit_flip = 64'h0000000000000010;  // 04
               8'hc1  :  int_bit_flip = 64'h0000000000000020;  // 05
               8'h38  :  int_bit_flip = 64'h0000000000000040;  // 06
               8'h07  :  int_bit_flip = 64'h0000000000000080;  // 07
               8'hd0  :  int_bit_flip = 64'h0000000000000100;  // 08
               8'h2c  :  int_bit_flip = 64'h0000000000000200;  // 09
               8'h43  :  int_bit_flip = 64'h0000000000000400;  // 10
               8'hb0  :  int_bit_flip = 64'h0000000000000800;  // 11
               8'h0d  :  int_bit_flip = 64'h0000000000001000;  // 12
               8'hc2  :  int_bit_flip = 64'h0000000000002000;  // 13
               8'h34  :  int_bit_flip = 64'h0000000000004000;  // 14
               8'h0b  :  int_bit_flip = 64'h0000000000008000;  // 15
               8'hc8  :  int_bit_flip = 64'h0000000000010000;  // 16
               8'h32  :  int_bit_flip = 64'h0000000000020000;  // 17
               8'h85  :  int_bit_flip = 64'h0000000000040000;  // 18
               8'h68  :  int_bit_flip = 64'h0000000000080000;  // 19
               8'h16  :  int_bit_flip = 64'h0000000000100000;  // 20
               8'ha1  :  int_bit_flip = 64'h0000000000200000;  // 21
               8'h58  :  int_bit_flip = 64'h0000000000400000;  // 22
               8'hc4  :  int_bit_flip = 64'h0000000000800000;  // 23
               8'h23  :  int_bit_flip = 64'h0000000001000000;  // 24
               8'h1a  :  int_bit_flip = 64'h0000000002000000;  // 25
               8'h45  :  int_bit_flip = 64'h0000000004000000;  // 26
               8'ha8  :  int_bit_flip = 64'h0000000008000000;  // 27
               8'h15  :  int_bit_flip = 64'h0000000010000000;  // 28
               8'ha2  :  int_bit_flip = 64'h0000000020000000;  // 29
               8'h54  :  int_bit_flip = 64'h0000000040000000;  // 30
               8'ha4  :  int_bit_flip = 64'h0000000080000000;  // 31
               8'h52  :  int_bit_flip = 64'h0000000100000000;  // 32
               8'h89  :  int_bit_flip = 64'h0000000200000000;  // 33
               8'h64  :  int_bit_flip = 64'h0000000400000000;  // 34
               8'h19  :  int_bit_flip = 64'h0000000800000000;  // 35
               8'h92  :  int_bit_flip = 64'h0000001000000000;  // 36
               8'h62  :  int_bit_flip = 64'h0000002000000000;  // 37
               8'h49  :  int_bit_flip = 64'h0000004000000000;  // 38
               8'h98  :  int_bit_flip = 64'h0000008000000000;  // 39
               8'h2a  :  int_bit_flip = 64'h0000010000000000;  // 40
               8'h25  :  int_bit_flip = 64'h0000020000000000;  // 41
               8'h94  :  int_bit_flip = 64'h0000040000000000;  // 42
               8'h4c  :  int_bit_flip = 64'h0000080000000000;  // 43
               8'h13  :  int_bit_flip = 64'h0000100000000000;  // 44
               8'h8c  :  int_bit_flip = 64'h0000200000000000;  // 45
               8'h61  :  int_bit_flip = 64'h0000400000000000;  // 46
               8'h8a  :  int_bit_flip = 64'h0000800000000000;  // 47
               8'h51  :  int_bit_flip = 64'h0001000000000000;  // 48
               8'h26  :  int_bit_flip = 64'h0002000000000000;  // 49
               8'h91  :  int_bit_flip = 64'h0004000000000000;  // 50
               8'h4a  :  int_bit_flip = 64'h0008000000000000;  // 51
               8'h86  :  int_bit_flip = 64'h0010000000000000;  // 52
               8'h31  :  int_bit_flip = 64'h0020000000000000;  // 53
               8'h46  :  int_bit_flip = 64'h0040000000000000;  // 54
               8'h29  :  int_bit_flip = 64'h0080000000000000;  // 55
               8'hf8  :  int_bit_flip = 64'h0100000000000000;  // 56
               8'hc7  :  int_bit_flip = 64'h0200000000000000;  // 57
               8'h3e  :  int_bit_flip = 64'h0400000000000000;  // 58
               8'hf1  :  int_bit_flip = 64'h0800000000000000;  // 59
               8'h8f  :  int_bit_flip = 64'h1000000000000000;  // 60
               8'h7c  :  int_bit_flip = 64'h2000000000000000;  // 61
               8'he3  :  int_bit_flip = 64'h4000000000000000;  // 62
               8'h1f  :  int_bit_flip = 64'h8000000000000000;  // 63
               default:  int_bit_flip = 64'h0000000000000000;
            endcase
         end // always_comb
      end    // ecc8
   endgenerate

   generate
      if (DATA_SIZE==MAX_DATA_SIZE) begin : max_size
         assign bit_flip = int_bit_flip;
      end
      else if ((DATA_SIZE>=MIN_DATA_SIZE) && (DATA_SIZE<=MAX_DATA_SIZE)) begin : legal_size_not_max
         assign bit_flip = int_bit_flip[0 +: DATA_SIZE];
      end
      else begin : illegal_size
         assign bit_flip = 'x;
      end
   endgenerate

   wire unused_ok = &{1'b0, int_bit_flip, data_unused};


endmodule
