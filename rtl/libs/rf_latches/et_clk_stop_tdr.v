// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_stop_tdr.v
//
// This module provides the ability to cleanly stop clocks that are not sourced from a gf_clk_mux

module et_clk_stop_tdr
#(
    parameter SYNC_STAGES = 2
) (
  // Output stoppable clock
  output              clk_out,
  // Input clock to stop
  input               clk_in,
  // Optional functional clk gate signal, tie to 0 if not used
  // (for clocks that already have a functional clock gate)
  input               clk_en,
  // Clock gate cell te signals
  input               clk_gate_te,
  // Scan dump mode enable
  input               sd_mode,
  // jtag_tdr_reg:
  // Inputs
  input  wire tdr_tck,
  input  wire tdr_resetl,
  input  wire tdr_reg_en,
  input  wire tdr_shiftdr_en, 
  input  wire tdr_updatedr_en,  
  input  wire tdr_capturedr_en, 
  input  wire tdr_si,
  // Outputs
  output wire tdr_so
);

   localparam WIDTH=6;
   wire [WIDTH-1:0] tdr_in = {WIDTH{1'b0}};
   wire [WIDTH-1:0] tdr_out;



   wire 	     tdr_ovr_sd_cs_st_en_clk;            // Override Control select
   wire 	     tdr_val_sd_cs_st_en_clk;            // Override value applied when Override Control select =1
   wire 	     dft__sd_cs_st_en_clk;               // Output of the bypass mux

   wire 	     tdr_ovr_sd_cs_st_clk;               // Override Control select
   wire 	     tdr_val_sd_cs_st_clk;               // Override value applied when Override Control select =1
   wire 	     dft__sd_cs_st_clk;                  // Output of the bypass mux

   wire 	     tdr_ovr_sd_cs_st_as_clk;               // Override Control select
   wire 	     tdr_val_sd_cs_st_as_clk;               // Override value applied when Override Control select =1
   wire 	     dft__sd_cs_st_as_clk;                  // Output of the bypass mux

   jtag_tdr_reg #( .WIDTH(WIDTH), .READ_TDR(1) ) jtag_tdr_reg 
   (
    // Outputs
    .so                                 (tdr_so),
    .tdr_out                            (tdr_out[WIDTH-1:0]),
    // Inputs
    .tck                                (tdr_tck),
    .resetl                             (tdr_resetl),
    .reg_en                             (tdr_reg_en),
    .shiftdr_en                         (tdr_shiftdr_en),
    .updatedr_en                        (tdr_updatedr_en),
    .capturedr_en                       (tdr_capturedr_en),
    .si                                 (tdr_si),
    .tdr_in                             (tdr_in[WIDTH-1:0])
    );

   // Map tdr bits to control wires
   assign 	     tdr_ovr_sd_cs_st_en_clk  = tdr_out[0];   
   assign 	     tdr_val_sd_cs_st_en_clk  = tdr_out[1];       

   assign 	     tdr_ovr_sd_cs_st_clk     = tdr_out[2];               
   assign 	     tdr_val_sd_cs_st_clk     = tdr_out[3];              

   assign 	     tdr_ovr_sd_cs_st_as_clk     = tdr_out[4];             
   assign 	     tdr_val_sd_cs_st_as_clk     = tdr_out[5];

   // Instantiate the bypass muxes (use explict mux cells here, provides more flexibilty for ECOs)
   et_combo_mux2  tdr_ovr_sd_cs_st_en_clk_mux   (
                  .x    (dft__sd_cs_st_en_clk),          
                  .d0   (1'b0),          
                  .d1   (tdr_val_sd_cs_st_en_clk),          
                  .s    (tdr_ovr_sd_cs_st_en_clk) 
                  );

   et_combo_mux2  tdr_ovr_sd_cs_st_clk_mux   (
                  .x    (dft__sd_cs_st_clk),          
                  .d0   (1'b0),          
                  .d1   (tdr_val_sd_cs_st_clk),          
                  .s    (tdr_ovr_sd_cs_st_clk) 
                  );

   et_combo_mux2  tdr_ovr_sd_cs_st_as_clk_mux   (
                  .x    (dft__sd_cs_st_as_clk),          
                  .d0   (1'b0),          
                  .d1   (tdr_val_sd_cs_st_as_clk),          
                  .s    (tdr_ovr_sd_cs_st_as_clk) 
                  );


  // Instantiate the clk_stop module
et_clk_stop
#(
    .SYNC_STAGES (SYNC_STAGES)
)
u_et_clk_stop
(
    
    .clk_out            (clk_out),                // Output stoppable clock
    .clk_in             (clk_in),                 // Input clock to stop
    .clk_en             (clk_en),                 // Optional functional clk gate signal, tie to 0 if not used
    .clk_gate_te        (clk_gate_te),            // Clock gate cell te signals
    .sd_mode            (sd_mode),                // Scan dump mode enable
        
    .sd_cs_st_en_clk    (dft__sd_cs_st_en_clk),   // Clock stop arm/enable signal
    .sd_cs_st_clk       (dft__sd_cs_st_clk),      // Clock stop signal
    .sd_cs_st_as_clk    (dft__sd_cs_st_as_clk)    // Stop clock asynchronously, in case clocks are dead (bypasses synchronizer flops)
);


endmodule
