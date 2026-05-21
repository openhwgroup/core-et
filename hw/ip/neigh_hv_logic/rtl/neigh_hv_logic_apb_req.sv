// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_apb_req (
  input  logic rst_push_ni,
  input  logic clk_push_i,
  input  logic apb_pready_neigh_i,
  input  logic apb_esr_req_apb_psel_i,
  input  logic apb_esr_req_apb_penable_i,
  output logic apb_esr_req_vcfifo_push_o
);

  logic apb_ready_wait_q;
  logic apb_ready_wait_d;

  assign apb_esr_req_vcfifo_push_o =
      (apb_esr_req_apb_psel_i | apb_esr_req_apb_penable_i) & ~apb_ready_wait_q;

  always_comb begin
    apb_ready_wait_d = apb_ready_wait_q;

    if (apb_esr_req_apb_penable_i && !apb_ready_wait_q) begin
      apb_ready_wait_d = 1'b1;
    end
    if (apb_pready_neigh_i) begin
      apb_ready_wait_d = 1'b0;
    end
  end

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      apb_ready_wait_q <= 1'b0;
    end else begin
      apb_ready_wait_q <= apb_ready_wait_d;
    end
  end

endmodule : neigh_hv_logic_apb_req
