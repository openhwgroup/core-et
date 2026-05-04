// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_lru_sel_tb #(
  parameter int unsigned NUM_CLIENTS = 8
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,
  input  logic [NUM_CLIENTS-1:0]  access_entry_i,
  output logic [NUM_CLIENTS-1:0]  new_victim_o,
  output logic [NUM_CLIENTS-1:0]  orig_victim_o
);

  logic reset_orig;
  assign reset_orig = ~rst_ni;

  lru_sel #(
    .NumClients(NUM_CLIENTS)
  ) u_new (
    .clk_i,
    .rst_ni,
    .access_entry_i,
    .victim_o(new_victim_o)
  );

  lru_sel_orig #(
    .NUM_CLIENTS(NUM_CLIENTS)
  ) u_orig (
    .clock(clk_i),
    .reset(reset_orig),
    .access_entry(access_entry_i),
    .victim(orig_victim_o)
  );

endmodule : cosim_lru_sel_tb
