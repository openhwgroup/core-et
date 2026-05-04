// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// jtag_tdr_reg
//   
module jtag_tdr_reg #(parameter
   WIDTH    = 150,
   READ_TDR = 1
) (
   input  logic             tck,
   input  logic             resetl,
   input  logic             reg_en,
   input  logic             shiftdr_en, 
   input  logic             updatedr_en,  
   input  logic             capturedr_en, 
   input  logic             si,
   output logic             so,
   input  logic [WIDTH-1:0] tdr_in,
   output logic [WIDTH-1:0] tdr_out
);

   et_tdr_reg #(.WIDTH(WIDTH), .READ_TDR(READ_TDR)) tdr_reg (
      .tdr_tck       ( tck                 ),
      .tdr_trstn     ( resetl              ),
      .tdr_ena       ( reg_en              ),
      .tdr_shiftdr   ( shiftdr_en          ),
      .tdr_updatedr  ( updatedr_en         ),
      .tdr_capturedr ( capturedr_en        ),
      .tdr_tdi       ( si                  ),
      .tdr_tdo       ( so                  ),
      .tdr_in        ( tdr_in              ),
      .tdr_out       ( tdr_out             )
   );


endmodule
