// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_hv_logic_bpam_rc_tbox_ack (
  input  logic clock,
  input  logic reset,
  input  bpam_rc_tbox_ack_t  bpam_rc_tbox_ack_hi_reg_in,
  output bpam_rc_tbox_ack_t  bpam_rc_tbox_ack_hi_reg_out
);

//       CLK    RST    DOUT                         DIN                        DEF
`RST_FF (clock, reset, bpam_rc_tbox_ack_hi_reg_out, bpam_rc_tbox_ack_hi_reg_in,'0)
endmodule
 
