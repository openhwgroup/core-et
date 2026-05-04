// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// gf_clkmux_tdr.v
// Author:   Ed Toy (ed.toy@esperantotech.com)
// DOC:      Sun Jun  9 15:53:11 PDT 2019
// DESC:     This module wraps gf_clkmux() module with a JTAG TDR register,
//           where the JTAG TDR register provides the required positive control
//           of the gf_clkmux() inputs for DFT test purposes.  The JTAG TDR
//           interface can also be used to implement the clock-stop function
//           required for ScanDump/RamDump.
//
//           The structure of the overrides from the TDR is as follows:
//               -for each bit to be overridden, an override select bit and an 
//                override value is provided from the TDR space.
//
//           The initial version of this module was generated using the
//           vp/emacs verilog mode flow.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


`include "soc.vh"

module gf_clkmux_tdr #(parameter
   DEFAULT_SEL = 0,
   WIDTH    = 12,
   READ_TDR = 1
) (
  // gf_clkmux:
  // Inputs
  input clk0,
  input clk1,
  input rst0,           // tdr override provided
  input rst1,           // tdr override provided
  input sel,            // tdr override provided
  input clk_off,        // tdr override provided
  input scan_clk_stop,  // tdr override provided
  input testmode,       // tdr override provided
  // Outputs
  output clkout,

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


   wire [WIDTH-1:0] tdr_in = {WIDTH{1'b0}};
   wire [WIDTH-1:0] tdr_out;

   // define tdr control wires
   wire 	     tdr_ovr_rst0;              // Override Control select
   wire 	     tdr_val_rst0;              // Override value applied when Override Control select =1
   wire 	     dft__rst0;                 // Output of the bypass mux
   
   wire 	     tdr_ovr_rst1;              // Override Control select
   wire 	     tdr_val_rst1;              // Override value applied when Override Control select =1
   wire 	     dft__rst1;                 // Output of the bypass mux

   wire 	     tdr_ovr_sel;               // Override Control select
   wire 	     tdr_val_sel;               // Override value applied when Override Control select =1
   wire 	     dft__sel;                  // Output of the bypass mux
   
   wire 	     tdr_ovr_clk_off;           // Override Control select
   wire 	     tdr_val_clk_off;           // Override value applied when Override Control select =1
   wire 	     dft__clk_off;              // Output of the bypass mux

   wire 	     tdr_ovr_scan_clk_stop;     // Override Control select
   wire 	     tdr_val_scan_clk_stop;     // Override value applied when Override Control select =1
   wire 	     dft__scan_clk_stop;        // Output of the bypass mux
   
   wire 	     tdr_ovr_testmode;          // Override Control select
   wire 	     tdr_val_testmode;          // Override value applied when Override Control select =1
   wire 	     dft__testmode;             // Output of the bypass mux
   

   jtag_tdr_reg #( .WIDTH(WIDTH), .READ_TDR(READ_TDR) ) jtag_tdr_reg 
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
   assign  tdr_ovr_rst0            =  tdr_out[0];
   assign  tdr_val_rst0            =  tdr_out[1];
   
   assign  tdr_ovr_rst1            =  tdr_out[2];
   assign  tdr_val_rst1            =  tdr_out[3];

   assign  tdr_ovr_sel             =  tdr_out[4];
   assign  tdr_val_sel             =  tdr_out[5];
   
   assign  tdr_ovr_clk_off         =  tdr_out[6];
   assign  tdr_val_clk_off         =  tdr_out[7];

   assign  tdr_ovr_scan_clk_stop   =  tdr_out[8];
   assign  tdr_val_scan_clk_stop   =  tdr_out[9];
   
   assign  tdr_ovr_testmode        =  tdr_out[10];
   assign  tdr_val_testmode        =  tdr_out[11];


   // Instantiate the bypass muxes (use explict mux cells here, provides more flexibilty for ECOs)

   et_combo_mux2  tdr_ovr_rst0_mux          (.x(dft__rst0),          .d0(rst0),          .d1(tdr_val_rst0),          .s(tdr_ovr_rst0) );
   et_combo_mux2  tdr_ovr_rst1_mux          (.x(dft__rst1),          .d0(rst1),          .d1(tdr_val_rst1),          .s(tdr_ovr_rst1) );
   et_combo_mux2  tdr_ovr_sel_mux           (.x(dft__sel),           .d0(sel),           .d1(tdr_val_sel),           .s(tdr_ovr_sel) );
   et_combo_mux2  tdr_ovr_clk_off_mux       (.x(dft__clk_off),       .d0(clk_off),       .d1(tdr_val_clk_off),       .s(tdr_ovr_clk_off) );
   et_combo_mux2  tdr_ovr_scan_clk_stop_mux (.x(dft__scan_clk_stop), .d0(scan_clk_stop), .d1(tdr_val_scan_clk_stop), .s(tdr_ovr_scan_clk_stop) );
   et_combo_mux2  tdr_ovr_testmode_mux      (.x(dft__testmode),      .d0(testmode),      .d1(tdr_val_testmode),      .s(tdr_ovr_testmode) );
   
   
   gf_clkmux  #( 
		 .DEFAULT_SEL    (DEFAULT_SEL))
   gf_clkmux
   (
    // Outputs
    .clkout                             (clkout),
    // Inputs
    .clk0                               (clk0),
    .clk1                               (clk1),
    .rst0                               (dft__rst0),
    .rst1                               (dft__rst1),
    .sel                                (dft__sel),
    .clk_off                            (dft__clk_off),
    .scan_clk_stop                      (dft__scan_clk_stop),
    .testmode                           (dft__testmode)
    );


endmodule

