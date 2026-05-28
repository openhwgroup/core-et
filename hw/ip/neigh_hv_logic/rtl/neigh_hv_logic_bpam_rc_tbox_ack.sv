// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_bpam_rc_tbox_ack (
  input  logic                                      clk_hv_i,
  input  logic                                      rst_hv_ni,
  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t     bpam_rc_tbox_ack_hi_reg_i,
  output neigh_hv_logic_pkg::bpam_rc_tbox_ack_t     bpam_rc_tbox_ack_hi_reg_o
);

  always_ff @(posedge clk_hv_i or negedge rst_hv_ni) begin
    if (!rst_hv_ni) begin
      bpam_rc_tbox_ack_hi_reg_o <= '0;
    end else begin
      bpam_rc_tbox_ack_hi_reg_o <= bpam_rc_tbox_ack_hi_reg_i;
    end
  end

endmodule : neigh_hv_logic_bpam_rc_tbox_ack
