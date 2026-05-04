// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_1r1w_par_preview #(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 8,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned AddrW = (Depth > 1) ? $clog2(Depth) : 1
) (
  input  logic                    preview_clk_i,
  input  logic                    rf_clk_i,
  input  logic                    wr_data_en_1p_next_i,
  output logic [Width*Depth-1:0]  rd_data_o,
  input  logic [Width-1:0]        wr_data_i,
  input  logic [AddrW-1:0]        wr_addr_i,
  input  logic                    wr_en_i
);

  (* ram_style = "distributed" *) logic [Width-1:0] rf_q [Depth];
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
    if (wr_en_i) begin
      rf_q[wr_addr_i] <= wr_data_del_q;
    end
  end

  always_comb begin
    rd_data_o = '0;
    for (int i = 0; i < Depth; i++) begin
      rd_data_o[i*Width +: Width] = rf_q[i];
    end
  end

endmodule : prim_rf_1r1w_par_preview
