// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

// This module is instantiated in pshire_top, which is in the 750 mv domain.
// The input signals are all in the 750 mv domain. If there is power domain
// crossing needed for these signals, the UPF flow will take care of it. There
// is no level-shifter hand-instantiated in this module, or its parent
// modules.
//
// All of the output signals are in the 750 mv power domain. No output signal
// is going to other power domains.
module sys_gasket_pshire_top (
  input  logic clock                ,
  input  logic reset_warm_in        ,
  input  logic reset_cold_in        ,
  input  logic dft__reset_byp       ,
  input  logic dft__reset           ,

  output logic reset_w              ,
  output logic reset_c               
);
  
  wire reset_warm_in_sync    ;
  wire reset_cold_in_sync    ;

    sync_rst
    sync_rst_reset_warm_in(
      .dft__reset_byp( dft__reset_byp     ),
      .dft__reset    ( dft__reset         ),
      .clock         ( clock              ),
      .reset_in      ( reset_warm_in      ),
      .reset_out     ( reset_warm_in_sync )
    );

    sync_rst
    sync_rst_reset_cold_in(
      .dft__reset_byp ( dft__reset_byp     ),
      .dft__reset     ( dft__reset         ),
      .clock          ( clock              ),
      .reset_in       ( reset_cold_in      ),
      .reset_out      ( reset_cold_in_sync )
    );

  // Outputs
  assign  reset_w = reset_warm_in_sync | reset_cold_in_sync;
  assign  reset_c =                      reset_cold_in_sync ;

endmodule
