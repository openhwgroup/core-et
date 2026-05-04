// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Glitch Free Clock mux
// Project       : Shire_top
//-----------------------------------------------------------------------------
// File          : gf_clk_mux.v
// Author        :   <juanjo.galindo@esperantotech.com>
// Created       : 27.11.2018
// Last modified : 27.11.2018 
//-----------------------------------------------------------------------------

module gf_clkmux (
   input  clk0,
   input  clk1,
   input  rst0,
   input  rst1,
   input  sel,
   input  clk_off,
   input  scan_clk_stop,
   input  testmode,           // Test mode selects clk0 
   output clkout
);

   parameter DEFAULT_SEL = 0 ; // 0:sel clk0, 1: sel clk1, other:OFF


`ifdef ET_LIBS_USE_STD_CELLS 
   gf_clkmux_grp_cells #(.DEFAULT_SEL(DEFAULT_SEL)) ET_GROUP_CELLS_gf_clk_mux_grp_cells (
      .clk0          ( clk0          ),
      .clk1          ( clk1          ),
      .rst0          ( rst0          ),
      .rst1          ( rst1          ),
      .sel           ( sel           ),
      .clk_off       ( clk_off       ),
      .scan_clk_stop ( scan_clk_stop ),
      .testmode      ( testmode      ),
      .clkout        ( clkout        )
   );
`else 

   localparam DEF0 = (DEFAULT_SEL==0);
   localparam DEF1 = (DEFAULT_SEL==1);
   localparam SYNC_STAGES = `SYNC_STAGES_SYNC_RST;
   
   wire sel0;
   wire sel1;
   wire sel_clk0;
   wire sel_clk1;
   wire clkoff_sel_clk0;
   wire clkoff_sel_clk1;
   wire gated_clk0;
   wire gated_clk1;
   wire gate_en0;
   wire gate_en1;
   wire cfg_sel_clk0;
   wire cfg_sel_clk1;
   wire cfg_clkoff_clk0;
   wire cfg_clkoff_clk1;
   logic sel0_ff_clk1;
   logic sel1_ff_clk0;
   
   //Synchronization of sel, clk_off inputs to clk1 clock (To be used in branch0, to gate clock clk0)
   //synchronized signals will be OR'ed with other branch sel signal (clk1 clock domain)
   et_cdc_sync #(
     .WIDTH       ( 1                 ),
     .SYNC_STAGES ( SYNC_STAGES       ),
     .RESET_VAL   ( 1'b0              ))
   et_cdc_sync_cfg_sel_clk1_branch
   (
     .clock       ( clk1              ),
     .reset       ( rst1              ),
     .din         ( sel               ),
     .dout        ( cfg_sel_clk1      )
   );
   et_cdc_sync #(
     .WIDTH       ( 1                 ),
     .SYNC_STAGES ( SYNC_STAGES       ),
     .RESET_VAL   ( 1'b0              ))
   et_cdc_sync_cfg_clkoff_clk1_branch
   (
     .clock       ( clk1                  ),
     .reset       ( rst1                  ),
     .din         ( clk_off|scan_clk_stop ),
     .dout        ( cfg_clkoff_clk1       )
   ); 
   
   //Synchronization of sel, clk_off inputs to clk0 clock (To be used in branch1, to gate clock clk1)
   //synchronized signals will be OR'ed with other branch sel signal (clk0 clock domain)
   et_cdc_sync #(
     .WIDTH       ( 1                 ),
     .SYNC_STAGES ( SYNC_STAGES       ),
     .RESET_VAL   ( 1'b0              ))
   et_cdc_sync_cfg_sel_clk0_branch
   (
     .clock       ( clk0              ),
     .reset       ( rst0              ),
     .din         ( sel               ),
     .dout        ( cfg_sel_clk0      )
   );
   et_cdc_sync #(
     .WIDTH       ( 1                 ),
     .SYNC_STAGES ( SYNC_STAGES       ),
     .RESET_VAL   ( 1'b0              ))
   et_cdc_sync_cfg_clkoff_clk0_branch
   (
     .clock       ( clk0                  ),
     .reset       ( rst0                  ),
     .din         ( clk_off|scan_clk_stop ),
     .dout        ( cfg_clkoff_clk0       )
   ); 
   
   //Feedback Msfilter selects to each clock branch  
   assign sel0   =  ~(  cfg_sel_clk1 | sel_clk1) ; //cross-coupling from the other branch
   assign sel1   =  ~( ~cfg_sel_clk0 | sel_clk0) ; //cross-coupling from the other branch 
   
   //Output of combinational gate is latched 
   always @(posedge clk1 or posedge rst1) begin : sel0_ff_clk1_flop 
      if (rst1) sel0_ff_clk1 <= DEF0; 
      else      sel0_ff_clk1 <= sel0; 
   end
   always @(posedge clk0 or posedge rst0) begin : sel1_ff_clk0_flop 
      if (rst0) sel1_ff_clk0 <= DEF1; 
      else      sel1_ff_clk0 <= sel1; 
   end
   
   //Latched sel value goes to current branch clock domain through a meta filter
   //From clk1 to clk0
   et_cdc_sync #(
     .WIDTH       ( 1            ),
     .SYNC_STAGES ( SYNC_STAGES  ),
     .RESET_VAL   ( DEF0         ))
   et_cdc_sync_clk0_sel_branch 
   (
     .clock       ( clk0         ),
     .reset       ( rst0         ),
     .din         ( sel0_ff_clk1 ),
     .dout        ( sel_clk0     )
   ); 
   
   //From clk0 to clk1
   et_cdc_sync #(
     .WIDTH       ( 1            ),
     .SYNC_STAGES ( SYNC_STAGES  ),
     .RESET_VAL   ( DEF1         ))
   et_cdc_sync_clk1_sel_branch 
   (
     .clock       ( clk1         ),
     .reset       ( rst1         ),
     .din         ( sel1_ff_clk0 ),
     .dout        ( sel_clk1     )
   ); 
   
   //clk_off stage
   assign clkoff_sel_clk0 = sel_clk0 & ~cfg_clkoff_clk0;                          
   assign clkoff_sel_clk1 = sel_clk1 & ~cfg_clkoff_clk1;                          
   
   //test_mode stage
   assign gate_en0 =  clkoff_sel_clk0 |  testmode;                          
   assign gate_en1 =  clkoff_sel_clk1 & ~testmode;                          
   
   // clock gates
   et_clk_gate gated_clk0_inst (.enclk (gated_clk0), .en (gate_en0), .clk (clk0), .te (1'b0));
   et_clk_gate gated_clk1_inst (.enclk (gated_clk1), .en (gate_en1), .clk (clk1), .te (1'b0));
   
   // OR-ed output branches
   assign clkout = gated_clk0 | gated_clk1 ;

`endif

endmodule


`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module gf_clkmux_grp_cells (
  input clk0,
  input clk1,
  input rst0,
  input rst1,
  input sel,
  input clk_off,
  input scan_clk_stop,
  input testmode,           // Test mode selects clk0 
  output clkout
);

   parameter DEFAULT_SEL = 0 ; // 0:sel clk0, 1: sel clk1, other:OFF
   
   localparam DEF0 = (DEFAULT_SEL==0);
   localparam DEF1 = (DEFAULT_SEL==1);
   localparam SYNC_STAGES = `SYNC_STAGES_SYNC_RST;

   wire sel0;
   wire sel1;
   wire sel_clk0;
   wire sel_clk1;
   wire clkoff_sel_clk0;
   wire clkoff_sel_clk1;
   wire gated_clk0;
   wire gated_clk1;
   wire gate_en0;
   wire gate_en1;
   wire cfg_sel_clk0;
   wire cfg_sel_clk1;
   wire cfg_clkoff_clk0;
   wire cfg_clkoff_clk1;
   logic sel0_ff_clk1;
   logic sel1_ff_clk0;

   generate
     if (1) begin : ET_DONT_TOUCH_gf_clkmux


       //ORing disconnection port (test & functional)
       wire ored_clk_off;
       et_combo_or or_clk_off (.x(ored_clk_off), .a1(clk_off), .a2(scan_clk_stop));

       //Synchronization of sel, clk_off inputs to clk1 clock 
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL (1'b0)) et_cdc_sync_cfg_sel_clk1_branch    (.clock(clk1), .reset(rst1), .din(sel         ), .dout(cfg_sel_clk1   ));
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL (1'b0)) et_cdc_sync_cfg_clkoff_clk1_branch (.clock(clk1), .reset(rst1), .din(ored_clk_off), .dout(cfg_clkoff_clk1));
       //Synchronization of sel, clk_off inputs to clk0 clock
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL (1'b0)) et_cdc_sync_cfg_sel_clk0_branch    (.clock(clk0), .reset(rst0), .din(sel         ), .dout(cfg_sel_clk0   ));
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL (1'b0)) et_cdc_sync_cfg_clkoff_clk0_branch (.clock(clk0), .reset(rst0), .din(ored_clk_off), .dout(cfg_clkoff_clk0));

       //Feedback Msfilter selects to each clock branch, cross-coupling from the other branch   
       et_combo_nor   nor_AB_sel0  (.x(sel0), .a1(cfg_sel_clk1), .a2(sel_clk1));
       et_combo_nor2b nor_nAB_sel1 (.x(sel1), .a (cfg_sel_clk0), .b (sel_clk0)); //.a is inverted input

       wire rst0_n;
       wire rst1_n;
       et_combo_inv rst0_inv (.x(rst0_n), .a(rst0));
       et_combo_inv rst1_inv (.x(rst1_n), .a(rst1));

       //Output of combinational gate is latched 
       if (DEF0) begin : DEF_0
         et_ff_async_set   sel0_ff_clk1_flop (.clock(clk1), .set  (rst1_n), .din(sel0), .dout(sel0_ff_clk1));
         et_ff_async_reset sel1_ff_clk0_flop (.clock(clk0), .reset(rst0_n), .din(sel1), .dout(sel1_ff_clk0));
       end else if (DEF1) begin : DEF_1   
         et_ff_async_reset sel0_ff_clk1_flop (.clock(clk1), .reset(rst1_n), .din(sel0), .dout(sel0_ff_clk1));
         et_ff_async_set   sel1_ff_clk0_flop (.clock(clk0), .set  (rst0_n), .din(sel1), .dout(sel1_ff_clk0));
       end else begin : DEF_X
         et_ff_async_reset sel0_ff_clk1_flop (.clock(clk1), .reset(rst1_n), .din(sel0), .dout(sel0_ff_clk1));
         et_ff_async_reset sel1_ff_clk0_flop (.clock(clk0), .reset(rst0_n), .din(sel1), .dout(sel1_ff_clk0));
       end

       //From clk1 to clk0
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL(DEF0)) et_cdc_sync_clk0_sel_branch (.clock(clk0), .reset(rst0), .din(sel0_ff_clk1), .dout(sel_clk0)); 
       //From clk0 to clk1
       et_cdc_sync #(.WIDTH(1),.SYNC_STAGES(SYNC_STAGES),.RESET_VAL(DEF1)) et_cdc_sync_clk1_sel_branch (.clock(clk1), .reset(rst1), .din(sel1_ff_clk0), .dout(sel_clk1)); 

       //clk_off stage
       et_combo_and2b and_nAB_b0    (.x(clkoff_sel_clk0), .a(cfg_clkoff_clk0), .b(sel_clk0)); //.a is inverted input
       et_combo_and2b and_nAB_b1    (.x(clkoff_sel_clk1), .a(cfg_clkoff_clk1), .b(sel_clk1)); //.a is inverted input

       //test_mode stage
       et_combo_or    or_A1A2       (.x(gate_en0), .a1(clkoff_sel_clk0), .a2(testmode));
       et_combo_and2b and_nAB       (.x(gate_en1), .a(testmode), .b(clkoff_sel_clk1)); //.a is inverted input

       // clock gates
       et_clk_gate gated_clk0_inst (.enclk (gated_clk0), .en (gate_en0), .clk (clk0), .te (1'b0));
       et_clk_gate gated_clk1_inst (.enclk (gated_clk1), .en (gate_en1), .clk (clk1), .te (1'b0));

       // output = OR-ed clock gate outputs
       et_combo_or clk_output_or (.x(clkout), .a1(gated_clk0), .a2(gated_clk1));

     end
   endgenerate


endmodule
`endif  // ET_LIBS_USE_STD_CELLS
