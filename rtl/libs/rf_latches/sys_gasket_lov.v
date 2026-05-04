// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

module sys_gasket_lov (
  input  logic                  clock                    , // high-voltage
  input  logic                  clock_fb_ref             , // high-voltage
  output logic                  clock_lv                 , // low-voltage
  input  logic                  reset_warm_in            , // high-voltage
  input  logic                  gpio_ndmreset_in         , // high-voltage
  input  logic                  ndmreset_in              , // high-voltage
  input  logic                  dmactive_in              , // high-voltage
  input  logic                  reset_cold_in            , // high-voltage
  input  logic                  dft__reset_byp           , // high-voltage 
  input  logic                  dft__reset               , // high-voltage 
  input  logic                  use_shire_tdr_reset_cntl , // high-voltage
  input  shire_tdr_reset_cntl_t shire_tdr_reset_cntl     , // high-voltage
  output logic                  reset_w                  , // low-voltage
  output logic                  reset_c                  , // low-voltage
  output logic                  reset_d                  , // low-voltage
  output logic                  dft__reset_byp_lv        , // low-voltage
  output logic                  dft__reset_lv              // low-voltage
);
  localparam SYS_GASKET_RST_RESYNC = 1'b0; // Submodule sys_gasket_reset contains reset repeaters

  logic ls_h2l_clock_pd_hv_out;

  // Change voltage of Neigh clock
  vclevel_shft_h2l #(.WIDTH(1)) ls_h2l_clock_pd_hv (.inp_hv(clock),.out_lv(ls_h2l_clock_pd_hv_out));

  // This buffer at LV (dont touch type) forces that real level shifter to be added right here
  et_combo_buf ls_pd_lv (.x(clock_lv), .a(ls_h2l_clock_pd_hv_out));

  wire reset_warm_int   ;
  wire gpio_ndmreset_int;
  wire ndmreset_int     ;
  wire dmactive_int     ;
  wire reset_cold_int   ;

  sys_gasket_lov_tdr_cntl
  sys_gasket_lov_tdr_cntl_pd_hv( 
    .use_shire_tdr     ( use_shire_tdr_reset_cntl              ), 
    .reset_warm_in     ( reset_warm_in                         ),
    .gpio_ndmreset_in  ( gpio_ndmreset_in                      ),
    .ndmreset_in       ( ndmreset_in                           ),
    .dmactive_in       ( dmactive_in                           ),
    .reset_cold_in     ( reset_cold_in                         ),
    .reset_warm_tdr    ( shire_tdr_reset_cntl.reset_warm_in    ),
    .gpio_ndmreset_tdr ( shire_tdr_reset_cntl.gpio_ndmreset_in ),
    .ndmreset_tdr      ( shire_tdr_reset_cntl.ndmreset_in      ),
    .dmactive_tdr      ( shire_tdr_reset_cntl.dmactive_in      ),
    .reset_cold_tdr    ( shire_tdr_reset_cntl.reset_cold_in    ),
    .reset_warm_out    ( reset_warm_int                        ),
    .gpio_ndmreset_out ( gpio_ndmreset_int                     ),
    .ndmreset_out      ( ndmreset_int                          ),
    .dmactive_out      ( dmactive_int                          ),
    .reset_cold_out    ( reset_cold_int                        )
  );

  wire reset_warm_ls_in   ;
  wire gpio_ndmreset_ls_in;
  wire ndmreset_ls_in     ;
  wire dmactive_ls_in     ;
  wire reset_cold_ls_in   ;

  wire clock_inv             ;
  wire reset_warm_in_sync    ;
  wire gpio_ndmreset_in_sync ; 
  wire ndmreset_in_sync      ; 
  wire dmactive_in_sync      ;
  wire reset_cold_in_sync    ;

  et_combo_inv clock_inv_pd_hv (.x(clock_inv), .a(clock_fb_ref));

  sync_rst 
  sync_rst_reset_warm_pd_hv( 
    .dft__reset_byp( dft__reset_byp     ),
    .dft__reset    ( dft__reset         ),
    .clock         ( clock_inv          ),
    .reset_in      ( reset_warm_int     ),
    .reset_out     ( reset_warm_in_sync )
  );

  sync_rst 
  sync_rst_ndmreset_pd_hv( 
    .dft__reset_byp( dft__reset_byp   ),
    .dft__reset    ( dft__reset       ),
    .clock         ( clock_inv        ),
    .reset_in      ( ndmreset_int     ),
    .reset_out     ( ndmreset_in_sync )
  );

  sync_rst 
  sync_rst_gpio_ndmreset_pd_hv( 
    .dft__reset_byp( dft__reset_byp        ),
    .dft__reset    ( dft__reset            ),
    .clock         ( clock_inv             ),
    .reset_in      ( gpio_ndmreset_int     ),
    .reset_out     ( gpio_ndmreset_in_sync )
  );

  sync_rst 
  sync_rst_dmactive_pd_hv( 
    .dft__reset_byp ( dft__reset_byp   ),
    .dft__reset     ( dft__reset       ),
    .clock          ( clock_inv        ),
    .reset_in       ( dmactive_int     ),
    .reset_out      ( dmactive_in_sync )
  );
  sync_rst 
  sync_rst_reset_cold_pd_hv( 
    .dft__reset_byp ( dft__reset_byp     ),
    .dft__reset     ( dft__reset         ),
    .clock          ( clock_inv          ),
    .reset_in       ( reset_cold_int     ),
    .reset_out      ( reset_cold_in_sync )
  );

  //----------------------------------------------------------------------------------
  // (High to Low)
  //----------------------------------------------------------------------------------
  // Input level shifters (from 650mV to 400mV) 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_reset_warm_in     ( .out_lv ( reset_warm_ls_in    ) , .inp_hv ( reset_warm_in_sync    )) ; 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_gpio_ndmreset_in  ( .out_lv ( gpio_ndmreset_ls_in ) , .inp_hv ( gpio_ndmreset_in_sync )) ; 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_ndmreset_in       ( .out_lv ( ndmreset_ls_in      ) , .inp_hv ( ndmreset_in_sync      )) ; 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_dmactive_in       ( .out_lv ( dmactive_ls_in      ) , .inp_hv ( dmactive_in_sync      )) ; 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_reset_cold_in     ( .out_lv ( reset_cold_ls_in    ) , .inp_hv ( reset_cold_in_sync    )) ; 

  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_dft__reset_byp_pd_hv ( .out_lv ( dft__reset_byp_lv ) , .inp_hv ( dft__reset_byp )) ;
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_dft__reset_pd_hv     ( .out_lv ( dft__reset_lv     ) , .inp_hv ( dft__reset     )) ;
  
  //----------------------------------------------------------------------------------
  // (Low Voltage Domain)
  //----------------------------------------------------------------------------------
  sys_gasket_reset   #( 
    .RST_RESYNC       ( SYS_GASKET_RST_RESYNC )) 
  sys_gasket_pd_lv (
    .clock            ( clock_lv            ),
    .reset_warm_in    ( reset_warm_ls_in    ),
    .gpio_ndmreset_in ( gpio_ndmreset_ls_in ),
    .ndmreset_in      ( ndmreset_ls_in      ),
    .dmactive_in      ( dmactive_ls_in      ),
    .reset_cold_in    ( reset_cold_ls_in    ),
    .reset_w          ( reset_w             ),  
    .reset_c          ( reset_c             ),  
    .reset_d          ( reset_d             ),  
    .dft__reset_byp   ( dft__reset_byp_lv   ),
    .dft__reset       ( dft__reset_lv       )
  );

  wire unused_ok = &{
     `ifndef ET_ASCENT_LINT
     1'b0,
     `endif
     shire_tdr_reset_cntl, // not all fields are used in this module
     shire_tdr_reset_cntl  // not all fields are used in this module
  };


endmodule
