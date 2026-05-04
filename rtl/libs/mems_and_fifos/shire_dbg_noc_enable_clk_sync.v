// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// shire_dbg_noc_enable_clk_sync.v
//
// synchronization of enable_clock_debug signal at hiv 
//                                                            .
//                                                   (HiV)    .  (LoV)  
//                                                            .             nsip_gate_sh0_dn_evt_pd_lv 
//                     shire_dbg_noc_enable_clk_sync_pd_hv    .             nsip_gate_sh0_dn_msg_pd_lv 
//                     +-------------------------------+      .             +--------------------+
//                     | shire_dbg_noc_enable_clk_sync |      .             | minshire_nsip_gate |x2
//                     |                               |      .             |                    |
//                     |                               | levelshifter       |                    |
//                     |            et_cdc_sync        |  H2L .             |                    |
//  enable_clock_debug |              +---+            |   |\ .             |     +---+          |
//                =====|=============>|   |============|==>| >.=============|====>|   |=>....    |
//                     |     inv  +---|>  |            |   |/ .           +-|-----|>  |          |
//                     |    |\    |   +---+            |      .           | |     +---+          |
//                  +--|----| >O--+                    |      .           | |      `RST_FF       |
//                  |  |    |/                         |      .           | |                    |
//                  |  +-------------------------------+      .           | +--------------------+
//                  |                                      /| .           |
//                  +-------------------------------------< |-.-----------+------                  
//                                                         \| . clk__noc_lv                      
//                                                        L2H .          
//                                                levelshifter.        
//
`include "soc.vh"
                                   
module shire_dbg_noc_enable_clk_sync #(    
  parameter SYNC_STAGES_SHIRE_TO_NOC = `SYNC_STAGES_400mV_500Mhz 
)                                    
(                             
   input  clk__noc                , //HiV
   input  reset_noc               , //HiV
   input  enable_clock_debug      , //HiV
   output enable_clock_debug_sync   //HiV
);

  // synchronization is made in hiv domain
  et_cdc_sync #(
       .WIDTH       (1),
       .SYNC_STAGES (SYNC_STAGES_SHIRE_TO_NOC)
  ) en_clk_dbg_sync (
      .clock        (~clk__noc                 ), //Negedge clock
      .reset        (reset_noc                 ),
      .din          (enable_clock_debug        ),
      .dout         (enable_clock_debug_sync   )
  );


endmodule

