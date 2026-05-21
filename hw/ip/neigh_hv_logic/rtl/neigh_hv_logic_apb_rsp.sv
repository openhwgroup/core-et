// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_apb_rsp (
  input  logic                                 apb_esr_rsp_vcfifo_valid_i,
  input  neigh_hv_logic_pkg::apb_from_neigh_t  apb_esr_rsp_pop_i,
  output neigh_hv_logic_pkg::apb_from_neigh_t  apb_esr_rsp_sync_o
);

  always_comb begin
    apb_esr_rsp_sync_o = '0;

    if (apb_esr_rsp_vcfifo_valid_i) begin
      apb_esr_rsp_sync_o = apb_esr_rsp_pop_i;
    end
  end

endmodule : neigh_hv_logic_apb_rsp
