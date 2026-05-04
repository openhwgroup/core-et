// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_dly_mux
//    combinatorial clock delay MUX, built from et_clk_dly_buf_a, et_clk_dly_buf_b, for PD dont touch
//
//    Delay modules:                                     
//    ----------------
//            2x HDBULT08_DEL_R2V5_1(2x40ps)            
//    tap_b = 1x HDBULT08_INV_CK_2  (   5ps) --> ~85ps  
//                                                      
//    tap_a = 3x HDBULT08_INV_CK_2   (3x5ps) -->  15ps  
//
//    Delay structure:
//    ----------------                                                                               '0'|\sel_a_3                               
//                                '0'|\sel_b_15                                +----------+          ---| |                                 
//         +-----------+          ---| |                                       |          |       CK    | |o--+ /CK                           
//         |           |       /CK   | |o-+ CK                             +---|-|>o|>o|>o|-------------| |   |                              
//     +---|-|>-|>-|>o-|-------------| |  |                                |   |          |             |/    |                              
//     |   |           |             |/   |                                |   +----------+tap_a_3  muxn_a_3  |                              
//     |   +-----------+tap_b_15 muxn_b_15|                                +-----------------+                |                  
//     +----  .......                     ...                                                |                | |\sel_a_2                   
//                        |                     | |\sel_b_2                    +----------+  |                +-| |                    
//         +-----------+  |                     +-| |                          |          |  |    /CK           | |o-+ CK             
//         |           |  |     CK                | |o-+ /CK               +---|-|>o|>o|>o|--+------------------| |  |                 
//     +---|-|>-|>-|>o-|--+-----------------------| |  |                   |   |          |                     |/   |                           
//     |   |           |                          |/   |                   |   +----------+tap_a_2          muxn_a_2 |                 
//     |   +-----------+tap_b_2               muxn_b_2 |                   +-----------------+                       |                 
//     +------------------+                            |                                     |                       | |\sel_a_1            
//                        |                            | |\sel_b_1             +----------+  |                       +-| |             
//         +-----------+  |                            +-| |                   |          |  |     CK                  | |o-+ /CK       
//         |           |  |     /CK                      | |o-+ CK         +---|-|>o|>o|>o|--+-------------------------| |  |          
//     +---|-|>-|>-|>o-|--+------------------------------| |  |            |   |          |                            |/   |                                   
//     |   |           |                                 |/   |            |   +----------+tap_a_1                 muxn_a_1 | |\sel_a_0     
//     |   +-----------+tap_b_1                      muxn_b_1 | |\sel_b_0  |                                                +-| |   CK    
//     |                                                      +-| |   /CK  |                                                  | |o---  
//     |                                                        | |o-------+--------------------------------------------------| |   clk_out
// ----+--------------------------------------------------------| |                                                           |/       
//  CK                                                          |/                                                        muxn_a_0     
//  clk_in                                                   muxn_b_0     
//     \_____________________________ _______________________________/     \__________________________ _____________________________/
//                                   V                                                                V
//       Muxn Delay = 15ps                                                 Muxn Delay = 15ps      
//       Tap B Delay        = 2*Del+Inv = (2*40+15) = 85 ps                Tap A Delay       = 3*Inv = (3*5) = 15 ps                         
//       Tap B Delay Branch = Tap B Delay + muxn = (85+15) = 100 ps        Tap A Delay Brach = Tap A Delay + muxn = (15+15) = 30 ps                         
//       Selected delay N is    15ps + N*(100ps)                           Select A delay Tap N --> 15ps + N*(30ps)                            
//       Default delay          15ps                                       Default delay          15ps                                        
//       Delay list: (15, 115, 215, 315, ..., 1515) ps                     Delays: (15, 45, 75, 105) ps 
//
module et_clk_dly_mux (
   output logic       clk_out,
   input  logic       clk_in,
   input  logic [5:0] s
);
  //Translated binary selection to cascaded mux sels
  logic [15:0] sel_tap_b;
  logic [3:0]  sel_tap_a;
  always_comb begin
    sel_tap_b= 16'd0;
    sel_tap_a= 2'd0;
    case (s[5:2])
      4'b0000: sel_tap_b = 16'b0000000000000000;
      4'b0001: sel_tap_b = 16'b0000000000000001;
      4'b0010: sel_tap_b = 16'b0000000000000011;
      4'b0011: sel_tap_b = 16'b0000000000000111;
      4'b0100: sel_tap_b = 16'b0000000000001111; 
      4'b0101: sel_tap_b = 16'b0000000000011111; 
      4'b0110: sel_tap_b = 16'b0000000000111111; 
      4'b0111: sel_tap_b = 16'b0000000001111111; 
      4'b1000: sel_tap_b = 16'b0000000011111111; 
      4'b1001: sel_tap_b = 16'b0000000111111111; 
      4'b1010: sel_tap_b = 16'b0000001111111111; 
      4'b1011: sel_tap_b = 16'b0000011111111111; 
      4'b1100: sel_tap_b = 16'b0000111111111111; 
      4'b1101: sel_tap_b = 16'b0001111111111111; 
      4'b1110: sel_tap_b = 16'b0011111111111111; 
      4'b1111: sel_tap_b = 16'b0111111111111111; 
    endcase
    case (s[1:0])
      2'b00: sel_tap_a = 4'b0000;
      2'b01: sel_tap_a = 4'b0001;
      2'b10: sel_tap_a = 4'b0011;
      2'b11: sel_tap_a = 4'b0111;
    endcase
  end

`ifdef ET_LIBS_USE_STD_CELLS 
  et_clk_dly_mux_grp_cells ET_GROUP_CELLS_et_clk_dly_mux  (
     .clk_out   ( clk_out   ),
     .clk_in    ( clk_in    ),
     .sel_tap_a ( sel_tap_a ),
     .sel_tap_b ( sel_tap_b )
  );
`else 
  //Delay cells Tap B
  generate
    logic [14:0] tap_b;
    logic [14:0] tap_b_muxn;
    logic        tap_b_out;

    if (1) begin : et_clk_dly_mux_tap_b
      //Tap b chain
      et_clk_dly_buf_b    tap_b_1  ( .x(tap_b[0]      ), .a(clk_in   ) );
      et_clk_dly_buf_b    tap_b_2  ( .x(tap_b[1]      ), .a(tap_b[0] ) );
      et_clk_dly_buf_b    tap_b_3  ( .x(tap_b[2]      ), .a(tap_b[1] ) );
      et_clk_dly_buf_b    tap_b_4  ( .x(tap_b[3]      ), .a(tap_b[2] ) );
      et_clk_dly_buf_b    tap_b_5  ( .x(tap_b[4]      ), .a(tap_b[3] ) );
      et_clk_dly_buf_b    tap_b_6  ( .x(tap_b[5]      ), .a(tap_b[4] ) );
      et_clk_dly_buf_b    tap_b_7  ( .x(tap_b[6]      ), .a(tap_b[5] ) );
      et_clk_dly_buf_b    tap_b_8  ( .x(tap_b[7]      ), .a(tap_b[6] ) );
      et_clk_dly_buf_b    tap_b_9  ( .x(tap_b[8]      ), .a(tap_b[7] ) );
      et_clk_dly_buf_b    tap_b_10 ( .x(tap_b[9]      ), .a(tap_b[8] ) );
      et_clk_dly_buf_b    tap_b_11 ( .x(tap_b[10]     ), .a(tap_b[9] ) );
      et_clk_dly_buf_b    tap_b_12 ( .x(tap_b[11]     ), .a(tap_b[10]) );
      et_clk_dly_buf_b    tap_b_13 ( .x(tap_b[12]     ), .a(tap_b[11]) );
      et_clk_dly_buf_b    tap_b_14 ( .x(tap_b[13]     ), .a(tap_b[12]) );
      et_clk_dly_buf_b    tap_b_15 ( .x(tap_b[14]     ), .a(tap_b[13]) );
      //Tap b output mux
      et_clk_muxn    tap_b_muxn_0  ( .x(tap_b_out     ), .s(sel_tap_b[0] ), .d0(clk_in   ), .d1(tap_b_muxn[0] ));
      //Tap b daisy chained muxn gates
      et_clk_muxn    tap_b_muxn_1  ( .x(tap_b_muxn[0] ), .s(sel_tap_b[1] ), .d0(tap_b[0] ), .d1(tap_b_muxn[1] ));
      et_clk_muxn    tap_b_muxn_2  ( .x(tap_b_muxn[1] ), .s(sel_tap_b[2] ), .d0(tap_b[1] ), .d1(tap_b_muxn[2] ));
      et_clk_muxn    tap_b_muxn_3  ( .x(tap_b_muxn[2] ), .s(sel_tap_b[3] ), .d0(tap_b[2] ), .d1(tap_b_muxn[3] ));
      et_clk_muxn    tap_b_muxn_4  ( .x(tap_b_muxn[3] ), .s(sel_tap_b[4] ), .d0(tap_b[3] ), .d1(tap_b_muxn[4] ));
      et_clk_muxn    tap_b_muxn_5  ( .x(tap_b_muxn[4] ), .s(sel_tap_b[5] ), .d0(tap_b[4] ), .d1(tap_b_muxn[5] ));
      et_clk_muxn    tap_b_muxn_6  ( .x(tap_b_muxn[5] ), .s(sel_tap_b[6] ), .d0(tap_b[5] ), .d1(tap_b_muxn[6] ));
      et_clk_muxn    tap_b_muxn_7  ( .x(tap_b_muxn[6] ), .s(sel_tap_b[7] ), .d0(tap_b[6] ), .d1(tap_b_muxn[7] ));
      et_clk_muxn    tap_b_muxn_8  ( .x(tap_b_muxn[7] ), .s(sel_tap_b[8] ), .d0(tap_b[7] ), .d1(tap_b_muxn[8] ));
      et_clk_muxn    tap_b_muxn_9  ( .x(tap_b_muxn[8] ), .s(sel_tap_b[9] ), .d0(tap_b[8] ), .d1(tap_b_muxn[9] ));
      et_clk_muxn    tap_b_muxn_10 ( .x(tap_b_muxn[9] ), .s(sel_tap_b[10]), .d0(tap_b[9] ), .d1(tap_b_muxn[10]));
      et_clk_muxn    tap_b_muxn_11 ( .x(tap_b_muxn[10]), .s(sel_tap_b[11]), .d0(tap_b[10]), .d1(tap_b_muxn[11]));
      et_clk_muxn    tap_b_muxn_12 ( .x(tap_b_muxn[11]), .s(sel_tap_b[12]), .d0(tap_b[11]), .d1(tap_b_muxn[12]));
      et_clk_muxn    tap_b_muxn_13 ( .x(tap_b_muxn[12]), .s(sel_tap_b[13]), .d0(tap_b[12]), .d1(tap_b_muxn[13]));
      et_clk_muxn    tap_b_muxn_14 ( .x(tap_b_muxn[13]), .s(sel_tap_b[14]), .d0(tap_b[13]), .d1(tap_b_muxn[14]));
      et_clk_muxn    tap_b_muxn_15 ( .x(tap_b_muxn[14]), .s(sel_tap_b[15]), .d0(tap_b[14]), .d1(1'b0          ));
    end
  endgenerate

  //Delay cells Tap A
  generate
    logic [2:0] tap_a;
    logic [2:0] tap_a_muxn;
    if (1) begin : et_clk_dly_mux_tap_a
      //Tap a chain
      et_clk_dly_buf_a tap_a_1  ( .x(tap_a[0] ), .a(tap_b_out) );
      et_clk_dly_buf_a tap_a_2  ( .x(tap_a[1] ), .a(tap_a[0] ) );
      et_clk_dly_buf_a tap_a_3  ( .x(tap_a[2] ), .a(tap_a[1] ) );
      //Tap a output mux
      et_clk_muxn    tap_a_muxn_0  ( .x(clk_out       ), .s( sel_tap_a[0] ), .d0( tap_b_out), .d1( tap_a_muxn[0] ));
      //Tap a daisy chained muxn gates
      et_clk_muxn    tap_a_muxn_1  ( .x(tap_a_muxn[0] ), .s( sel_tap_a[1] ), .d0( tap_a[0] ), .d1( tap_a_muxn[1] ));
      et_clk_muxn    tap_a_muxn_2  ( .x(tap_a_muxn[1] ), .s( sel_tap_a[2] ), .d0( tap_a[1] ), .d1( tap_a_muxn[2] ));
      et_clk_muxn    tap_a_muxn_3  ( .x(tap_a_muxn[2] ), .s( sel_tap_a[3] ), .d0( tap_a[2] ), .d1( 1'b0          )); 
    end
  endgenerate

`endif

endmodule

`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_clk_dly_mux_grp_cells (
   output logic         clk_out   ,
   input  logic         clk_in    ,
   input  logic [3:0]   sel_tap_a ,
   input  logic [15:0]  sel_tap_b 
); 
  //Delay cells Tap B
  generate
    logic [14:0] tap_b;
    logic [14:0] tap_b_muxn;
    logic        tap_b_out;

    if (1) begin : et_clk_dly_mux_tap_b
      //Tap b chain
      et_clk_dly_buf_b    tap_b_1  ( .x(tap_b[0]      ), .a(clk_in   ) );
      et_clk_dly_buf_b    tap_b_2  ( .x(tap_b[1]      ), .a(tap_b[0] ) );
      et_clk_dly_buf_b    tap_b_3  ( .x(tap_b[2]      ), .a(tap_b[1] ) );
      et_clk_dly_buf_b    tap_b_4  ( .x(tap_b[3]      ), .a(tap_b[2] ) );
      et_clk_dly_buf_b    tap_b_5  ( .x(tap_b[4]      ), .a(tap_b[3] ) );
      et_clk_dly_buf_b    tap_b_6  ( .x(tap_b[5]      ), .a(tap_b[4] ) );
      et_clk_dly_buf_b    tap_b_7  ( .x(tap_b[6]      ), .a(tap_b[5] ) );
      et_clk_dly_buf_b    tap_b_8  ( .x(tap_b[7]      ), .a(tap_b[6] ) );
      et_clk_dly_buf_b    tap_b_9  ( .x(tap_b[8]      ), .a(tap_b[7] ) );
      et_clk_dly_buf_b    tap_b_10 ( .x(tap_b[9]      ), .a(tap_b[8] ) );
      et_clk_dly_buf_b    tap_b_11 ( .x(tap_b[10]     ), .a(tap_b[9] ) );
      et_clk_dly_buf_b    tap_b_12 ( .x(tap_b[11]     ), .a(tap_b[10]) );
      et_clk_dly_buf_b    tap_b_13 ( .x(tap_b[12]     ), .a(tap_b[11]) );
      et_clk_dly_buf_b    tap_b_14 ( .x(tap_b[13]     ), .a(tap_b[12]) );
      et_clk_dly_buf_b    tap_b_15 ( .x(tap_b[14]     ), .a(tap_b[13]) );
      //Tap b output mux
      et_clk_muxn    tap_b_muxn_0  ( .x(tap_b_out     ), .s(sel_tap_b[0] ), .d0(clk_in   ), .d1(tap_b_muxn[0] ));
      //Tap b daisy chained muxn gates
      et_clk_muxn    tap_b_muxn_1  ( .x(tap_b_muxn[0] ), .s(sel_tap_b[1] ), .d0(tap_b[0] ), .d1(tap_b_muxn[1] ));
      et_clk_muxn    tap_b_muxn_2  ( .x(tap_b_muxn[1] ), .s(sel_tap_b[2] ), .d0(tap_b[1] ), .d1(tap_b_muxn[2] ));
      et_clk_muxn    tap_b_muxn_3  ( .x(tap_b_muxn[2] ), .s(sel_tap_b[3] ), .d0(tap_b[2] ), .d1(tap_b_muxn[3] ));
      et_clk_muxn    tap_b_muxn_4  ( .x(tap_b_muxn[3] ), .s(sel_tap_b[4] ), .d0(tap_b[3] ), .d1(tap_b_muxn[4] ));
      et_clk_muxn    tap_b_muxn_5  ( .x(tap_b_muxn[4] ), .s(sel_tap_b[5] ), .d0(tap_b[4] ), .d1(tap_b_muxn[5] ));
      et_clk_muxn    tap_b_muxn_6  ( .x(tap_b_muxn[5] ), .s(sel_tap_b[6] ), .d0(tap_b[5] ), .d1(tap_b_muxn[6] ));
      et_clk_muxn    tap_b_muxn_7  ( .x(tap_b_muxn[6] ), .s(sel_tap_b[7] ), .d0(tap_b[6] ), .d1(tap_b_muxn[7] ));
      et_clk_muxn    tap_b_muxn_8  ( .x(tap_b_muxn[7] ), .s(sel_tap_b[8] ), .d0(tap_b[7] ), .d1(tap_b_muxn[8] ));
      et_clk_muxn    tap_b_muxn_9  ( .x(tap_b_muxn[8] ), .s(sel_tap_b[9] ), .d0(tap_b[8] ), .d1(tap_b_muxn[9] ));
      et_clk_muxn    tap_b_muxn_10 ( .x(tap_b_muxn[9] ), .s(sel_tap_b[10]), .d0(tap_b[9] ), .d1(tap_b_muxn[10]));
      et_clk_muxn    tap_b_muxn_11 ( .x(tap_b_muxn[10]), .s(sel_tap_b[11]), .d0(tap_b[10]), .d1(tap_b_muxn[11]));
      et_clk_muxn    tap_b_muxn_12 ( .x(tap_b_muxn[11]), .s(sel_tap_b[12]), .d0(tap_b[11]), .d1(tap_b_muxn[12]));
      et_clk_muxn    tap_b_muxn_13 ( .x(tap_b_muxn[12]), .s(sel_tap_b[13]), .d0(tap_b[12]), .d1(tap_b_muxn[13]));
      et_clk_muxn    tap_b_muxn_14 ( .x(tap_b_muxn[13]), .s(sel_tap_b[14]), .d0(tap_b[13]), .d1(tap_b_muxn[14]));
      et_clk_muxn    tap_b_muxn_15 ( .x(tap_b_muxn[14]), .s(sel_tap_b[15]), .d0(tap_b[14]), .d1(1'b0          ));
    end
  endgenerate

  //Delay cells Tap A
  generate
    logic [2:0] tap_a;
    logic [2:0] tap_a_muxn;
    if (1) begin : et_clk_dly_mux_tap_a
      //Tap a chain
      et_clk_dly_buf_a tap_a_1  ( .x(tap_a[0] ), .a(tap_b_out) );
      et_clk_dly_buf_a tap_a_2  ( .x(tap_a[1] ), .a(tap_a[0] ) );
      et_clk_dly_buf_a tap_a_3  ( .x(tap_a[2] ), .a(tap_a[1] ) );
      //Tap a output mux
      et_clk_muxn    tap_a_muxn_0  ( .x(clk_out       ), .s( sel_tap_a[0] ), .d0( tap_b_out), .d1( tap_a_muxn[0] ));
      //Tap a daisy chained muxn gates
      et_clk_muxn    tap_a_muxn_1  ( .x(tap_a_muxn[0] ), .s( sel_tap_a[1] ), .d0( tap_a[0] ), .d1( tap_a_muxn[1] ));
      et_clk_muxn    tap_a_muxn_2  ( .x(tap_a_muxn[1] ), .s( sel_tap_a[2] ), .d0( tap_a[1] ), .d1( tap_a_muxn[2] ));
      et_clk_muxn    tap_a_muxn_3  ( .x(tap_a_muxn[2] ), .s( sel_tap_a[3] ), .d0( tap_a[2] ), .d1( 1'b0          )); 
    end
  endgenerate
  
endmodule
`endif
    
