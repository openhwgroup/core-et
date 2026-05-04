// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

module sys_gasket_noc_reset (
  input  clock                ,
  input  reset_n_system       ,
  input  reset_n_system_debug ,
  input  dft__reset_byp       ,
  input  dft__reset           ,
  output reset_n_noc          ,
  output reset_n_noc_debug    ,
  output reset_noc            ,
  output reset_noc_debug            
);

  wire reset_system_sync      ;
  wire reset_system_debug_sync; 

  rst_repeat 
  rst_repeat_reset_n_system( 
    .dft__reset_byp( dft__reset_byp      ),
    .dft__reset    ( dft__reset          ),
    .clock         ( clock               ),
    .reset_in      ( ~reset_n_system     ),
    .reset_out     ( reset_system_sync   )
  );

  rst_repeat 
  rst_repeat_reset_n_system_debug( 
    .dft__reset_byp( dft__reset_byp            ),
    .dft__reset    ( dft__reset                ),
    .clock         ( clock                     ),
    .reset_in      ( ~reset_n_system_debug     ),
    .reset_out     ( reset_system_debug_sync )
  );

  assign  reset_noc        = reset_system_sync       ;
  assign  reset_noc_debug  = reset_system_debug_sync ;

  assign  reset_n_noc        = !reset_system_sync       ;
  assign  reset_n_noc_debug  = !reset_system_debug_sync ;

endmodule
