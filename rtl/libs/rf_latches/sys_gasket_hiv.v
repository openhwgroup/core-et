// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

module sys_gasket_hiv (
  input  logic                  clock                    ,
  input  logic                  noc_clock                ,
  input  logic                  reset_warm_in            ,
  input  logic                  gpio_ndmreset_in         ,
  input  logic                  ndmreset_in              ,
  input  logic                  dmactive_in              ,
  input  logic                  reset_cold_in            ,
  input  logic                  reset_n_system           ,
  input  logic                  reset_n_system_debug     ,
  input  logic                  dft__reset_byp           ,
  input  logic                  dft__reset               ,
  input  logic                  use_shire_tdr_reset_cntl ,
  input  shire_tdr_reset_cntl_t shire_tdr_reset_cntl     ,
  output logic                  reset_w                  ,
  output logic                  reset_c                  ,
  output logic                  reset_d                  ,
  output logic                  reset_noc_lv             ,
  output logic                  reset_noc_debug_lv       ,
  output logic                  noc_clock_lv             ,
  output logic                  reset_n_system_lv        ,
  output logic                  reset_n_system_debug_lv  ,
  output logic                  dft__reset_byp_hv        ,
  output logic                  dft__reset_byp_lv        ,
  output logic                  dft__reset_hv            ,
  output logic                  dft__reset_lv

);
  localparam SYS_GASKET_RST_RESYNC = 1'b1; // Submodule sys_gasket_reset contains reset synchronizers

  wire reset_warm_int    = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.reset_warm_in    : reset_warm_in    ;
  wire gpio_ndmreset_int = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.gpio_ndmreset_in : gpio_ndmreset_in ;
  wire ndmreset_int      = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.ndmreset_in      : ndmreset_in      ;
  wire dmactive_int      = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.dmactive_in      : dmactive_in      ;
  wire reset_cold_int    = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.reset_cold_in    : reset_cold_in    ;

  wire reset_warm_ls_in   ;
  wire gpio_ndmreset_ls_in;
  wire ndmreset_ls_in     ;
  wire dmactive_ls_in     ;
  wire reset_cold_ls_in   ;

  //----------------------------------------------------------------------------------
  // (Low to High)
  //----------------------------------------------------------------------------------
  // Input level shifters (from 650mV to 650mV/750mV/850mV) 
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_reset_warm_in     ( .out_hv ( reset_warm_ls_in    ) , .inp_lv ( reset_warm_int    )) ;
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_gpio_ndmreset_in  ( .out_hv ( gpio_ndmreset_ls_in ) , .inp_lv ( gpio_ndmreset_int )) ;
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_ndmreset_in       ( .out_hv ( ndmreset_ls_in      ) , .inp_lv ( ndmreset_int      )) ;
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_dmactive_in       ( .out_hv ( dmactive_ls_in      ) , .inp_lv ( dmactive_int      )) ;
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_reset_cold_in     ( .out_hv ( reset_cold_ls_in    ) , .inp_lv ( reset_cold_int    )) ;

  // Scan_mode 650mV to 650/750/850mV
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_dft__reset_byp_hv ( .out_hv ( dft__reset_byp_hv ) , .inp_lv ( dft__reset_byp )) ;
  vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_dft__reset_hv     ( .out_hv ( dft__reset_hv     ) , .inp_lv ( dft__reset     )) ;

  //----------------------------------------------------------------------------------
  // (High to Low)
  //----------------------------------------------------------------------------------
  // Scan_mode 650mV to 400mV
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_dft__reset_byp_lv ( .out_lv ( dft__reset_byp_lv ) , .inp_hv ( dft__reset_byp )) ;
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_dft__reset_lv     ( .out_lv ( dft__reset_lv     ) , .inp_hv ( dft__reset     )) ;
  // NOC clk+resets 650mV to 400mV

  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_noc_clock_lv      ( .out_lv ( noc_clock_lv      ) , .inp_hv ( noc_clock )) ;


  wire reset_n_system_int       = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.reset_n_system       : reset_n_system       ;
  wire reset_n_system_debug_int = use_shire_tdr_reset_cntl ?  shire_tdr_reset_cntl.reset_n_system_debug : reset_n_system_debug ;

  wire reset_n_system_sync      ;
  wire reset_n_system_debug_sync; 

  // NOTE: for 3-voltage level partitions, this module goes into a pd_gv domain
  sys_gasket_noc_reset_sync
  sys_gasket_noc_reset_sync_pd_hv (
    .noc_clock                 ( noc_clock                 ),
    .reset_n_system            ( reset_n_system_int        ),
    .reset_n_system_debug      ( reset_n_system_debug_int  ),
    .dft__reset_byp            ( dft__reset_byp            ),
    .dft__reset                ( dft__reset                ),
    .reset_n_system_sync       ( reset_n_system_sync       ),
    .reset_n_system_debug_sync ( reset_n_system_debug_sync )             
  );

  wire reset_n_system_sync_lv      ;
  wire reset_n_system_debug_sync_lv; 
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_reset_n_system_lv       ( .out_lv ( reset_n_system_sync_lv       ) , .inp_hv ( reset_n_system_sync       )) ;
  vclevel_shft_h2l # (.WIDTH (1)) ls_h2l_reset_n_system_debug_lv ( .out_lv ( reset_n_system_debug_sync_lv ) , .inp_hv ( reset_n_system_debug_sync )) ;

  //----------------------------------------------------------------------------------
  // (Low Voltage Domain)
  //----------------------------------------------------------------------------------
  sys_gasket_noc_reset
  sys_gasket_noc_reset_pd_lv (
    .clock                ( noc_clock_lv                 ),
    .reset_n_system       ( reset_n_system_sync_lv       ),
    .reset_n_system_debug ( reset_n_system_debug_sync_lv ),
    .dft__reset_byp       ( dft__reset_byp_lv            ),
    .dft__reset           ( dft__reset_lv                ),
    .reset_n_noc          ( reset_n_system_lv            ),
    .reset_n_noc_debug    ( reset_n_system_debug_lv      ),
    .reset_noc            ( reset_noc_lv                 ),
    .reset_noc_debug      ( reset_noc_debug_lv           )
  );
  
  //----------------------------------------------------------------------------------
  // (High Voltage Domain)
  //----------------------------------------------------------------------------------
  sys_gasket_reset   #( 
    .RST_RESYNC       ( SYS_GASKET_RST_RESYNC )) 
  sys_gasket_pd_hv (
    .clock            ( clock                 ),
    .reset_warm_in    ( reset_warm_ls_in      ),
    .gpio_ndmreset_in ( gpio_ndmreset_ls_in   ),
    .ndmreset_in      ( ndmreset_ls_in        ),
    .dmactive_in      ( dmactive_ls_in        ),
    .reset_cold_in    ( reset_cold_ls_in      ),
    .reset_w          ( reset_w               ),  
    .reset_c          ( reset_c               ),  
    .reset_d          ( reset_d               ),  
    .dft__reset_byp   ( dft__reset_byp_hv     ),
    .dft__reset       ( dft__reset_hv         )
  );

endmodule
