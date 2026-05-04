// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: registered-read 1R1W RF with an internal write-preview
// seam. The ECP5 implementation keeps the original parent-visible timing with
// edge-triggered staging around the RF write edge.

module prim_rf_1r1w_reg_preview #(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 32,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned AddrW = (Depth > 1) ? $clog2(Depth) : 1
) (
  input  logic              preview_clk_i,
  input  logic              rf_clk_i,
  input  logic              wr_data_en_1p_next_i,
  input  logic [Width-1:0]  wr_data_i,
  input  logic [AddrW-1:0]  wr_addr_i,
  input  logic              wr_en_i,
  input  logic [AddrW-1:0]  rd_addr_i,
  input  logic              rd_en_i,
  output logic [Width-1:0]  rd_data_o
);

  (* ram_style = "block" *) logic [Width-1:0] rf_q [Depth];
  logic [Width-1:0] wr_data_del_q;
  logic [AddrW-1:0] rd_addr_q;

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
    if (rd_en_i) begin
      rd_addr_q <= rd_addr_i;
    end
    if (wr_en_i) begin
      rf_q[wr_addr_i] <= wr_data_del_q;
    end
  end

  assign rd_data_o = rf_q[rd_addr_q];

endmodule : prim_rf_1r1w_reg_preview
