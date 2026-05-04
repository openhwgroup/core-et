// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// minshire_nsip_gate.v
//
// Implement gating logic for nsip bridges to ensure it is 
// placed in the low voltage domain
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
//       clk__noc --+--|----| >O--+                    |      .           | |      `RST_FF       |
//                  |  |    |/                         |      .           | |                    |
//                  |  +-------------------------------+      .           | +--------------------+
//                  |                                      |\ .           |
//                  +--------------------------------------| >.-----------+                  
//                                                         |/ . clk__noc_lv                      
//                                                        H2L .          
//                                                  levelshifter         
`include "soc.vh"

module shire_nsip_gate
(
   input  clk__noc_lv,
   input  reset_noc_lv,
   output noc_beat_valid_defer_mc_gated,
   input  noc_beat_valid_defer_mc,

   output host_credit_inc_defer_mc,
   input  host_credit_inc_defer_mc_pre, 

   input  enable_clock_debug_noc_lv
);
  logic enable_clock_debug_noc_sync;

  `RST_FF (clk__noc_lv, reset_noc_lv, enable_clock_debug_noc_sync , enable_clock_debug_noc_lv , 1'b0)

   // when clock is gated off, inhibit pushes into the vcfifo
   assign noc_beat_valid_defer_mc_gated = noc_beat_valid_defer_mc   & enable_clock_debug_noc_sync ;

   // when clock is gated off, give an immediate credit return on any valid transfer
   assign host_credit_inc_defer_mc      = enable_clock_debug_noc_sync ? host_credit_inc_defer_mc_pre 
                                                                      : noc_beat_valid_defer_mc;

endmodule

