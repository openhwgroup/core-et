// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


`include "soc.vh"

module pll_lock_lost_capture
#(
    parameter USE_OUTPUT = 0
)
(
    input    logic        clk_ref,
    input    logic        pll_clk_sel,
    input    logic        pll_lock_detect,

    output   logic        pll_lock_lost_ref
);

  logic pll_lock_lost_pre;
  //// logic pll_lock_lost_sync;
  logic pll_clk_sel_sync;

   // synchronize the mux select to the reference clock
      et_cdc_sync #(
           .WIDTH       (1),
           .SYNC_STAGES (2)
      ) clk_sel_sync (
          .clock        (clk_ref),
          .reset        (1'b0),
          .din          (pll_clk_sel),
          .dout         (pll_clk_sel_sync)
      );

  // capture loss of pll lock when clock mux is selecting the pll. Once set, the capture flop
  // stays asserted until the clock mux selects the reference clock
  `FF(clk_ref,  pll_lock_lost_pre, pll_clk_sel_sync & (~pll_lock_detect | pll_lock_lost_pre))

  generate 
    if (USE_OUTPUT) begin : use_output
       assign pll_lock_lost_ref = pll_lock_lost_pre;
    end else begin : unused_output
       logic unused_pll_lock_lost;
      // if not using the output, then instaniate a buffer to ensure the cpature register is not eaten.
       et_combo_buf buf_lock (.a(pll_lock_lost_pre), .x(unused_pll_lock_lost));

       assign pll_lock_lost_ref = pll_lock_lost_pre;
       wire unused_ok = &{ 1'b0,
                           unused_pll_lock_lost
                          };
    end
  endgenerate

endmodule


 


