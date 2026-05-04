// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"


module shire_dbg_noc_voltage_cross
#(  parameter SYNC_STAGES_NOC_TO_SHIRE = `SYNC_STAGES_650mV_1000Mhz,
    parameter SYNC_STAGES_SHIRE_TO_NOC = `SYNC_STAGES_400mV_500Mhz,
    parameter UPSTREAM_VC_FIFOS        = 1, 
    parameter NO_SYNC_CLK_DISABLE      = 0
)
(
  // System signals
  input  logic                               clock                            ,
  input  logic                               reset                            ,
  input  logic                               clk__noc_lv                      ,
  input  logic                               reset_noc_lv                     ,

  input  logic                               clk_gate_debug_disable           ,

  // NSIP TX message interface
  input  logic                                     ust_out_nsip_msg_beat_valid      ,  // Valid
  input  logic [`DEBUG_NSIP_HOST_PORT_WIDTH - 1:0] ust_out_nsip_msg_dest_hp         ,  // Host ID
  input  logic [1:0]                               ust_out_nsip_msg_dest_int        ,  // Interface (wired to 0)
  input  logic [3:0]                               ust_out_nsip_msg_xfer_qos        ,  // QoS (wired to 0)
  input  logic                                     ust_out_nsip_msg_beat_sop        ,  // Start of packet
  input  logic                                     ust_out_nsip_msg_beat_eop        ,  // End of packet
  input  logic [`DEBUG_NSIP_MSG_DATA_WIDTH - 1:0]  ust_out_nsip_msg_beat_data       ,  // Message data
  output logic                                     ust_out_nsip_msg_credit_inc      ,  // Credit increment
  output logic                                     host_beat_valid_defer_sh0_dn_msg ,  // Valid
  output logic [`DEBUG_NSIP_HOST_PORT_WIDTH- 1:0]  host_dest_hp_defer_sh0_dn_msg    ,  // Host ID
  output logic [1:0]                               host_dest_int_defer_sh0_dn_msg   ,  // Interface (wired to 0)
  output logic [3:0]                               host_xfer_qos_defer_sh0_dn_msg   ,  // QoS (wired to 0)
  output logic                                     host_beat_sop_defer_sh0_dn_msg   ,  // Start of packet
  output logic                                     host_beat_eop_defer_sh0_dn_msg   ,  // End of packet
  output logic [`DEBUG_NSIP_MSG_DATA_WIDTH - 1:0]  host_beat_data_defer_sh0_dn_msg  ,  // Message data
  input  logic                                     noc_credit_inc_defer_sh0_dn_msg  ,  // Credit increment
    // NSIP RX message interface
  output logic                                     ust_in_nsip_msg_beat_valid       , // Valid
  output logic                                     ust_in_nsip_msg_beat_sop         , // Start of packet
  output logic                                     ust_in_nsip_msg_beat_eop         , // End of packet
  output logic [`DEBUG_NSIP_MSG_DATA_WIDTH - 1:0]  ust_in_nsip_msg_beat_data        , // Message data
  input  logic                                     ust_in_nsip_msg_credit_inc       , // Credit increment
  input  logic                                     noc_beat_valid_defer_sh0_dn_msg  , // Valid
  input  logic                                     noc_beat_sop_defer_sh0_dn_msg    , // Start of packet
  input  logic                                     noc_beat_eop_defer_sh0_dn_msg    , // End of packet
  input  logic [`DEBUG_NSIP_MSG_DATA_WIDTH - 1:0]  noc_beat_data_defer_sh0_dn_msg   , // Message data
  output logic                                     host_credit_inc_defer_sh0_dn_msg , // Credit increment
    // NSIP TX event interface
  input  logic                                     ust_out_nsip_evt_beat_valid      , // Valid
  input  logic [`DEBUG_NSIP_HOST_PORT_WIDTH- 1:0]  ust_out_nsip_evt_dest_hp         , // Host ID
  input  logic [1:0]                               ust_out_nsip_evt_dest_int        , // Interface (wired to 0)
  input  logic [3:0]                               ust_out_nsip_evt_xfer_qos        , // QoS (wired to 0)
  input  logic                                     ust_out_nsip_evt_beat_sop        , // Start of packet
  input  logic                                     ust_out_nsip_evt_beat_eop        , // End of packet
  input  logic [`DEBUG_NSIP_EVT_DATA_WIDTH- 1:0]   ust_out_nsip_evt_beat_data       , // Message data
  output logic                                     ust_out_nsip_evt_credit_inc      , // Credit increment
  output logic                                     host_beat_valid_defer_sh0_dn_evt , // Valid
  output logic [`DEBUG_NSIP_HOST_PORT_WIDTH- 1:0]  host_dest_hp_defer_sh0_dn_evt    , // Host ID
  output logic [1:0]                               host_dest_int_defer_sh0_dn_evt   , // Interface (wired to 0)
  output logic [3:0]                               host_xfer_qos_defer_sh0_dn_evt   , // QoS (wired to 0)
  output logic                                     host_beat_sop_defer_sh0_dn_evt   , // Start of packet
  output logic                                     host_beat_eop_defer_sh0_dn_evt   , // End of packet
  output logic [`DEBUG_NSIP_EVT_DATA_WIDTH- 1:0]   host_beat_data_defer_sh0_dn_evt  , // Message data
  input  logic                                     noc_credit_inc_defer_sh0_dn_evt  , // Credit increment
    // NSIP RX event interface
  output logic                                     ust_in_nsip_evt_beat_valid       , // Valid
  output logic                                     ust_in_nsip_evt_beat_sop         , // Start of packet
  output logic                                     ust_in_nsip_evt_beat_eop         , // End of packet
  output logic [`DEBUG_NSIP_EVT_DATA_WIDTH- 1:0]   ust_in_nsip_evt_beat_data        , // Message data
  input  logic                                     ust_in_nsip_evt_credit_inc       , // Credit increment
  input  logic                                     noc_beat_valid_defer_sh0_dn_evt  , // Valid
  input  logic                                     noc_beat_sop_defer_sh0_dn_evt    , // Start of packet
  input  logic                                     noc_beat_eop_defer_sh0_dn_evt    , // End of packet
  input  logic [`DEBUG_NSIP_EVT_DATA_WIDTH- 1:0]   noc_beat_data_defer_sh0_dn_evt   , // Message data
  output logic                                     host_credit_inc_defer_sh0_dn_evt , // Credit increment
  input  logic                              dft__reset_byp_hv                ,
  input  logic                              dft__reset_hv                    ,
  input  logic                              dft__reset_byp_lv                ,
  input  logic                              dft__reset_lv      
  );                                                                    


  wire enable_clock_debug_noc_lv  ;

  generate
      if ( NO_SYNC_CLK_DISABLE == 1 )
      begin :no_sync_clk_disable_gen
        // clk_gate_debug_disable is already lv and synchronous to clk__noc
        assign enable_clock_debug_noc_lv = ~clk_gate_debug_disable;
      end 
      else 
      begin: sync_clk_disable_gen 
        // clk_gate_debug_disable is hv and not synchronous to clk__noc
        ////////////////////////////////////////////////////////////////////////////////////////////////////////\\
        // Synchronization of enable_clock_debug signal                                                         ||
        // Metastability filters (et_cdc_sync) MUST be instantiated only in HiV                                 ||
        //======================================================================================================||
        //                                                   (HiV)    .  (LoV)                                  ||
        //                                                            .             nsip_gate_sh0_dn_evt_pd_lv  ||
        //                     shire_dbg_noc_enable_clk_sync_pd_hv    .             nsip_gate_sh0_dn_msg_pd_lv  ||
        //                     +-------------------------------+      .             +--------------------+      ||
        //                     | shire_dbg_noc_enable_clk_sync |      .             | shire_nsip_gate    |x2    ||
        //                     |                               |      .             |                    |      ||
        //                     |                               | levelshifter       |                    |      ||
        //                     |            et_cdc_sync        |  H2L .             |                    |      ||
        //  enable_clock_debug |              +---+            |   |\ .             |     +---+          |      ||
        //                =====|=============>|   |============|==>| >.=============|====>|   |=>....    |      ||
        //                     |     inv  +---|>  |            |   |/ .           +-|-----|>  |          |      ||
        //                     |    |\    |   +---+            |      .           | |     +---+          |      ||
        //       clk__noc   +--|----| >O--+                    |      .           | |      `RST_FF       |      ||
        //                  |  |    |/                         |      .           | |                    |      ||
        //                  |  +-------------------------------+      .           | +--------------------+      ||
        //                  |                                      /| .           |                             ||
        //                  +-------------------------------------< |-.-----------+------                       ||
        //                                                         \| .            clk__noc_lv                  ||
        //                                                        L2H .            (input port)                 ||
        //                                                levelshifter.                                         ||
          wire clk__noc                   ; //HiV
          wire reset_noc                  ; //HiV 
          wire enable_clock_debug_sync    ;
          
          // Getting NoC reset HiV
          vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_reset_noc        ( .out_hv ( reset_noc ) , .inp_lv ( reset_noc_lv )) ;
          // Getting NoC clock HiV
          vclevel_shft_l2h # (.WIDTH (1)) ls_l2h_reset_noc_clock  ( .out_hv ( clk__noc  ) , .inp_lv ( clk__noc_lv  )) ;

          shire_dbg_noc_enable_clk_sync #(  
              .SYNC_STAGES_SHIRE_TO_NOC ( SYNC_STAGES_SHIRE_TO_NOC )) 
          shire_dbg_noc_enable_clk_sync_pd_hv (
              .clk__noc                 ( clk__noc                 ),
              .reset_noc                ( reset_noc                ),
              .enable_clock_debug       ( ~clk_gate_debug_disable  ),
              .enable_clock_debug_sync  ( enable_clock_debug_sync  )
          );
          
          // Sync'ed  clk_gate_debug_disable to LoV
          vclevel_shft_h2l #(.WIDTH(1)) ls_h2l_en_clk_dbg     (.out_lv(enable_clock_debug_noc_lv), .inp_hv(enable_clock_debug_sync));  // 650/750/850mV

    end
endgenerate  

//======================================================================================================//

  generate
      if ( UPSTREAM_VC_FIFOS == 1 ) 
      begin :upstream_msg_fifos_instance

        // NSIP TX message interface

        vcfifo_wr_hiv_credit_gcd #(
           .ELEM_SIZE           (  $bits(ust_out_nsip_msg_dest_hp)  +
                                   $bits(ust_out_nsip_msg_dest_int) +
                                   $bits(ust_out_nsip_msg_xfer_qos) +
                                   $bits(ust_out_nsip_msg_beat_sop) +
                                   $bits(ust_out_nsip_msg_beat_eop) +
                                   $bits(ust_out_nsip_msg_beat_data)     )
           ,.NUM_ELEMS          ( `MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH     )
           ,.DOWNSTREAM_CREDITS ( `MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH )
           ,.WR_PTR_SYNC_STAGES ( SYNC_STAGES_SHIRE_TO_NOC               )
           ,.RD_PTR_SYNC_STAGES ( SYNC_STAGES_NOC_TO_SHIRE               )
        ) ust_out_nsip_msg_fifo (
           .clock_pop           ( clk__noc_lv                       ),
           .reset_pop           ( reset_noc_lv                      ),
           .clock_push          ( clock                             ),
           .reset_push          ( reset                             ),
           .push                ( ust_out_nsip_msg_beat_valid       ), // credit based interface, valid only asserted when want to push
           .push_credit         ( ust_out_nsip_msg_credit_inc       ), // (output)
           .push_data           ( { ust_out_nsip_msg_dest_hp   ,
                                    ust_out_nsip_msg_dest_int  ,
                                    ust_out_nsip_msg_xfer_qos  ,
                                    ust_out_nsip_msg_beat_sop  ,
                                    ust_out_nsip_msg_beat_eop  ,
                                    ust_out_nsip_msg_beat_data }    ),
           .ready               (                                   ),
           .valid               ( host_beat_valid_defer_sh0_dn_msg  ),
           .empty               (                                   ),
           .pop_data            ( {host_dest_hp_defer_sh0_dn_msg  ,    // Host ID
                                   host_dest_int_defer_sh0_dn_msg ,    // Interface       ( wired to 0)
                                   host_xfer_qos_defer_sh0_dn_msg ,    // QoS             ( wired to 0)
                                   host_beat_sop_defer_sh0_dn_msg ,    // Start of packet
                                   host_beat_eop_defer_sh0_dn_msg ,    // End of packet
                                   host_beat_data_defer_sh0_dn_msg} ), // Message data
           .pop_credit          ( noc_credit_inc_defer_sh0_dn_msg   ), // (input)
           .dft__reset_byp_push ( dft__reset_byp_hv                 ),
           .dft__reset_push     ( dft__reset_hv                     ),
           .dft__reset_byp_pop  ( dft__reset_byp_lv                 ),
           .dft__reset_pop      ( dft__reset_lv                     ) 
        );
    end
    else
    begin: upstream_msg_fifos_no_instance
        always_comb
        begin
           ust_out_nsip_msg_credit_inc      = '0;
           host_beat_valid_defer_sh0_dn_msg = '0;
           host_dest_hp_defer_sh0_dn_msg    = '0;   
           host_dest_int_defer_sh0_dn_msg   = '0; 
           host_xfer_qos_defer_sh0_dn_msg   = '0; 
           host_beat_sop_defer_sh0_dn_msg   = '0; 
           host_beat_eop_defer_sh0_dn_msg   = '0; 
           host_beat_data_defer_sh0_dn_msg  = '0; 
        end

        logic   ust_dbg_upstream_msg_fifo_unused_ok;
        assign  ust_dbg_upstream_msg_fifo_unused_ok = &{ust_out_nsip_msg_beat_valid,
                                                        ust_out_nsip_msg_dest_hp,
                                                        ust_out_nsip_msg_dest_int,
                                                        ust_out_nsip_msg_xfer_qos,
                                                        ust_out_nsip_msg_beat_sop,
                                                        ust_out_nsip_msg_beat_eop,
                                                        ust_out_nsip_msg_beat_data,
                                                        noc_credit_inc_defer_sh0_dn_msg
                                                       };
    end
  endgenerate

  // NSIP RX message interface
  logic ust_in_nsip_msg_fifo_empty;
  logic noc_beat_valid_defer_sh0_dn_msg_gated;
  logic host_credit_inc_defer_sh0_dn_msg_pre ;

  shire_nsip_gate  nsip_gate_sh0_dn_msg_pd_lv (
     .clk__noc_lv                   (clk__noc_lv                           ),
     .reset_noc_lv                  (reset_noc_lv                          ),
     .noc_beat_valid_defer_mc_gated (noc_beat_valid_defer_sh0_dn_msg_gated ),
     .noc_beat_valid_defer_mc       (noc_beat_valid_defer_sh0_dn_msg       ),
     .host_credit_inc_defer_mc      (host_credit_inc_defer_sh0_dn_msg      ),
     .host_credit_inc_defer_mc_pre  (host_credit_inc_defer_sh0_dn_msg_pre  ),
     .enable_clock_debug_noc_lv     (enable_clock_debug_noc_lv             )
  ); 

  vcfifo_wr_lov_credit_gcd #(
     .ELEM_SIZE           (  $bits(ust_in_nsip_msg_beat_sop)+
                             $bits(ust_in_nsip_msg_beat_eop)+
                             $bits(ust_in_nsip_msg_beat_data)      )
     ,.NUM_ELEMS          ( `MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH )
     ,.DOWNSTREAM_CREDITS ( `MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH     )
     ,.WR_PTR_SYNC_STAGES ( SYNC_STAGES_NOC_TO_SHIRE               )
     ,.RD_PTR_SYNC_STAGES ( SYNC_STAGES_SHIRE_TO_NOC               )
  ) ust_in_nsip_msg_fifo (
     .clock_push          ( clk__noc_lv                            ),
     .reset_push          ( reset_noc_lv                           ),
     .clock_pop           ( clock                                  ),
     .reset_pop           ( reset                                  ),
     .push                ( noc_beat_valid_defer_sh0_dn_msg_gated  ),
     .push_data           ( { noc_beat_sop_defer_sh0_dn_msg ,         // Start of packet
                              noc_beat_eop_defer_sh0_dn_msg ,         // End of packet
                              noc_beat_data_defer_sh0_dn_msg}      ), // Message data 
     .push_credit         ( host_credit_inc_defer_sh0_dn_msg_pre   ), // (output)
     .ready               (                                        ),
     .empty               ( ust_in_nsip_msg_fifo_empty             ),
     .valid               ( ust_in_nsip_msg_beat_valid             ), // credit based interface, valid only asserted when want to push
     .pop_credit          ( ust_in_nsip_msg_credit_inc             ), // (input)
     .pop_data            ( { ust_in_nsip_msg_beat_sop  ,
                              ust_in_nsip_msg_beat_eop  ,
                              ust_in_nsip_msg_beat_data }          ),
     .dft__reset_byp_push ( dft__reset_byp_lv                      ),
     .dft__reset_push     ( dft__reset_lv                          ),
     .dft__reset_byp_pop  ( dft__reset_byp_hv                      ),
     .dft__reset_pop      ( dft__reset_hv                          ) 
  );

  // NSIP TX event interface
  logic ust_out_nsip_evt_fifo_empty;

  vcfifo_wr_hiv_credit_gcd #(
     .ELEM_SIZE            ( $bits(ust_out_nsip_evt_dest_hp)+
                             $bits(ust_out_nsip_evt_dest_int)+
                             $bits(ust_out_nsip_evt_xfer_qos)+
                             $bits(ust_out_nsip_evt_beat_sop)+
                             $bits(ust_out_nsip_evt_beat_eop)+
                             $bits(ust_out_nsip_evt_beat_data            ))
     ,.NUM_ELEMS           ( `MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH_EVENT     )
     ,.DOWNSTREAM_CREDITS  ( `MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH_EVENT )
     ,.WR_PTR_SYNC_STAGES  ( SYNC_STAGES_SHIRE_TO_NOC                     )
     ,.RD_PTR_SYNC_STAGES  ( SYNC_STAGES_NOC_TO_SHIRE                     )
  ) ust_out_nsip_evt_fifo (
     .clock_pop           ( clk__noc_lv                      ),
     .reset_pop           ( reset_noc_lv                     ),
     .clock_push          ( clock                            ),
     .reset_push          ( reset                            ),
     .push                ( ust_out_nsip_evt_beat_valid      ), // credit based interface, valid only asserted when want to push
     .push_credit         ( ust_out_nsip_evt_credit_inc      ),
     .push_data           ({ust_out_nsip_evt_dest_hp   ,
                            ust_out_nsip_evt_dest_int  ,
                            ust_out_nsip_evt_xfer_qos  ,
                            ust_out_nsip_evt_beat_sop  ,
                            ust_out_nsip_evt_beat_eop  ,
                            ust_out_nsip_evt_beat_data }     ),
     .ready               (                                  ),
     .valid               ( host_beat_valid_defer_sh0_dn_evt ),
     .empty               ( ust_out_nsip_evt_fifo_empty      ),
     .pop_data            ({host_dest_hp_defer_sh0_dn_evt,                                  // Host ID
                            host_dest_int_defer_sh0_dn_evt,                                 // Interface (wired to 0)
                            host_xfer_qos_defer_sh0_dn_evt,                                 // QoS (wired to 0)
                            host_beat_sop_defer_sh0_dn_evt,                                 // Start of packet
                            host_beat_eop_defer_sh0_dn_evt,                                 // End of packet
                            host_beat_data_defer_sh0_dn_evt[`DEBUG_NSIP_EVT_DATA_WIDTH-1:0]}// Message data
                                                             ),
     .pop_credit          ( noc_credit_inc_defer_sh0_dn_evt  ),
     .dft__reset_byp_push ( dft__reset_byp_hv                ),
     .dft__reset_push     ( dft__reset_hv                    ),
     .dft__reset_byp_pop  ( dft__reset_byp_lv                ),
     .dft__reset_pop      ( dft__reset_lv                    ) 
  );
  
  // Leaf NSIP RX event interface
  logic ust_in_nsip_evt_fifo_empty;

  logic noc_beat_valid_defer_sh0_dn_evt_gated ;
  logic host_credit_inc_defer_sh0_dn_evt_pre  ;

  shire_nsip_gate  nsip_gate_sh0_dn_evt_pd_lv (
     .clk__noc_lv                   (clk__noc_lv                           ),
     .reset_noc_lv                  (reset_noc_lv                          ),
     .noc_beat_valid_defer_mc_gated (noc_beat_valid_defer_sh0_dn_evt_gated ),
     .noc_beat_valid_defer_mc       (noc_beat_valid_defer_sh0_dn_evt       ),
     .host_credit_inc_defer_mc      (host_credit_inc_defer_sh0_dn_evt      ),
     .host_credit_inc_defer_mc_pre  (host_credit_inc_defer_sh0_dn_evt_pre  ),
     .enable_clock_debug_noc_lv     (enable_clock_debug_noc_lv             )
  ); 

  vcfifo_wr_lov_credit_gcd #(
     .ELEM_SIZE           ( $bits(ust_in_nsip_evt_beat_sop)+
                            $bits(ust_in_nsip_evt_beat_eop)+
                            $bits(ust_in_nsip_evt_beat_data)             )
     ,.NUM_ELEMS          ( `MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH_EVENT )
     ,.DOWNSTREAM_CREDITS ( `MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH_EVENT     )
     ,.WR_PTR_SYNC_STAGES ( SYNC_STAGES_NOC_TO_SHIRE                     )
     ,.RD_PTR_SYNC_STAGES ( SYNC_STAGES_SHIRE_TO_NOC                     )
  ) ust_in_nsip_evt_fifo (
     .clock_push          ( clk__noc_lv                                  ),
     .reset_push          ( reset_noc_lv                                 ),
     .clock_pop           ( clock                                        ),
     .reset_pop           ( reset                                        ),
     .push                ( noc_beat_valid_defer_sh0_dn_evt_gated        ),
     .push_data           ( { noc_beat_sop_defer_sh0_dn_evt ,                                 // Start of packet
                              noc_beat_eop_defer_sh0_dn_evt ,                                 // End of packet
                              noc_beat_data_defer_sh0_dn_evt[`DEBUG_NSIP_EVT_DATA_WIDTH-1:0]} // Message data
                            ),
     .push_credit         ( host_credit_inc_defer_sh0_dn_evt_pre         ),
     .ready               (                                              ),
     .empty               ( ust_in_nsip_evt_fifo_empty                   ),
     .valid               ( ust_in_nsip_evt_beat_valid                   ), // credit based interface, valid only asserted when want to push
     .pop_credit          ( ust_in_nsip_evt_credit_inc                   ),
     .pop_data            ( {ust_in_nsip_evt_beat_sop ,
                             ust_in_nsip_evt_beat_eop ,
                             ust_in_nsip_evt_beat_data}                  ),
     .dft__reset_byp_push ( dft__reset_byp_lv                            ),
     .dft__reset_push     ( dft__reset_lv                                ),
     .dft__reset_byp_pop  ( dft__reset_byp_hv                            ),
     .dft__reset_pop      ( dft__reset_hv                                ) 
  );

endmodule : shire_dbg_noc_voltage_cross

