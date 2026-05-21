// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_icache_err (
  input  logic rst_pop_ni,
  input  logic clk_pop_i,
  input  logic ff_datain_i,
  output logic ff_dataout_o
);

  logic update_en;

  assign update_en = ff_datain_i ^ ff_dataout_o;

  always_ff @(posedge clk_pop_i or negedge rst_pop_ni) begin
    if (!rst_pop_ni) begin
      ff_dataout_o <= 1'b0;
    end else if (update_en) begin
      ff_dataout_o <= ff_datain_i;
    end
  end

endmodule : neigh_hv_logic_icache_err
