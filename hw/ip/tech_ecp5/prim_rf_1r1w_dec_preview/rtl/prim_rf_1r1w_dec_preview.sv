// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_1r1w_dec_preview #(
  parameter int unsigned Width = 32,
  parameter int unsigned Entries = 8,
  parameter bit          PreviewClkSameAsRf = 1'b1
) (
  input  logic                 preview_clk_i,
  input  logic                 rf_clk_i,
  input  logic                 wr_data_en_1p_next_i,
  input  logic [Entries-1:0]   rd_addr_i,
  output logic [Width-1:0]     rd_data_o,
  input  logic [Width-1:0]     wr_data_i,
  input  logic [Entries-1:0]   wr_addr_i,
  input  logic                 wr_en_i
);

  logic [Width-1:0] rf_q [Entries];
  logic [Width-1:0] wr_data_del_q;

  if (PreviewClkSameAsRf) begin : gen_same_preview_clk
    always_ff @(negedge rf_clk_i) begin
      if (wr_data_en_1p_next_i) begin
        wr_data_del_q <= wr_data_i;
      end
    end
  end else begin : gen_split_preview_clk
    always_ff @(negedge preview_clk_i) begin
      if (wr_data_en_1p_next_i) begin
        wr_data_del_q <= wr_data_i;
      end
    end
  end

  always_ff @(posedge rf_clk_i) begin
    for (int i = 0; i < Entries; i++) begin
      if (wr_en_i && wr_addr_i[i]) begin
        rf_q[i] <= wr_data_del_q;
      end
    end
  end

  always_comb begin
    rd_data_o = '0;
    for (int i = 0; i < Entries; i++) begin
      if (rd_addr_i[i]) begin
        rd_data_o |= rf_q[i];
      end
    end
  end

endmodule : prim_rf_1r1w_dec_preview
