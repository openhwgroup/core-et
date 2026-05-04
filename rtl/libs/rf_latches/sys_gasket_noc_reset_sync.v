// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

module sys_gasket_noc_reset_sync (
  input  noc_clock                ,
  input  reset_n_system           ,
  input  reset_n_system_debug     ,
  input  dft__reset_byp           ,
  input  dft__reset               ,
  output reset_n_system_sync      ,
  output reset_n_system_debug_sync             
);

  sync_rstn
  sync_rst_reset_n_system_pd_hv ( 
    .dft__reset_byp( dft__reset_byp      ),
    .dft__reset    ( dft__reset          ),
    .clock         ( ~noc_clock          ),
    .reset_in_n    ( reset_n_system      ),
    .reset_out_n   ( reset_n_system_sync )
  );

  sync_rstn
  sync_rst_reset_n_system_debug_pd_hv ( 
    .dft__reset_byp( dft__reset_byp            ),
    .dft__reset    ( dft__reset                ),
    .clock         ( ~noc_clock                ),
    .reset_in_n    ( reset_n_system_debug      ),
    .reset_out_n   ( reset_n_system_debug_sync )
  );

endmodule
