// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// gf_clkmux_wrap.v
// wrap module to allow rst's to be synchronized

module gf_clkmux_wrap (
   input  clk0,
   input  clk1,
   input  rst0,
   input  rst1,
   input  sel,
   input  clk_off,
   input  scan_clk_stop,
   input  testmode,           // Test mode selects clk0 
   input  dft__reset_byp,
   input  dft__reset,
   output clkout
);

   parameter DEFAULT_SEL = 0 ; // 0:sel clk0, 1: sel clk1, other:OFF


   logic rst0_sync;
   logic rst1_sync;
   sync_rst rst0_sync_inst (
      .reset_out      ( rst0_sync      ), 
      .reset_in       ( rst0           ),
      .clock          ( clk0           ),
      .dft__reset_byp ( dft__reset_byp ),
      .dft__reset     ( dft__reset     )
   );

   sync_rst rst1_sync_inst (
      .reset_out      ( rst1_sync      ), 
      .reset_in       ( rst1           ),
      .clock          ( clk1           ),
      .dft__reset_byp ( dft__reset_byp ),
      .dft__reset     ( dft__reset     )
   );

   gf_clkmux #(.DEFAULT_SEL(DEFAULT_SEL)) mux (
      .clk0           ( clk0          ),
      .clk1           ( clk1          ),
      .rst0           ( rst0_sync     ),
      .rst1           ( rst1_sync     ),
      .sel            ( sel           ),
      .clk_off        ( clk_off       ),
      .scan_clk_stop  ( scan_clk_stop ),
      .testmode       ( testmode      ),
      .clkout         ( clkout        )
   );


endmodule
