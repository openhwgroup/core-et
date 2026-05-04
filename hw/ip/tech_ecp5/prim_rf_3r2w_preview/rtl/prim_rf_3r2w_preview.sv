// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_3r2w_preview #(
  parameter int unsigned Width        = 32,
  parameter int unsigned Entries      = 32,
  parameter int unsigned Level2CkGate = 0,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned AddrW       = $clog2(Entries)
) (
  input  logic              preview_clk_i,
  input  logic              rf_clk_i,
  input  logic [AddrW-1:0]  rd_addr_a_i,
  output logic [Width-1:0]  rd_data_a_o,
  input  logic [AddrW-1:0]  rd_addr_b_i,
  output logic [Width-1:0]  rd_data_b_o,
  input  logic [AddrW-1:0]  rd_addr_c_i,
  output logic [Width-1:0]  rd_data_c_o,
  input  logic              wr_en_a_i,
  input  logic [AddrW-1:0]  wr_addr_a_i,
  input  logic              wr_data_a_en_1p_next_i,
  input  logic [Width-1:0]  wr_data_a_i,
  input  logic              wr_en_b_i,
  input  logic [AddrW-1:0]  wr_addr_b_i,
  input  logic              wr_data_b_en_1p_next_i,
  input  logic [Width-1:0]  wr_data_b_i
);

  (* ram_style = "distributed" *) logic [Width-1:0] rf_a_rd_a_q [Entries];
  (* ram_style = "distributed" *) logic [Width-1:0] rf_a_rd_b_q [Entries];
  (* ram_style = "distributed" *) logic [Width-1:0] rf_a_rd_c_q [Entries];
  (* ram_style = "distributed" *) logic [Width-1:0] rf_b_rd_a_q [Entries];
  (* ram_style = "distributed" *) logic [Width-1:0] rf_b_rd_b_q [Entries];
  (* ram_style = "distributed" *) logic [Width-1:0] rf_b_rd_c_q [Entries];
  logic [Entries-1:0]       lvt_q;
  logic [Width-1:0]         wr_data_a_del_q;
  logic [Width-1:0]         wr_data_b_del_q;
  /* verilator lint_off UNUSEDPARAM */  // Retained for interface compatibility with the generic seam.
  logic [0:0] unused_level2_ck_gate;
  /* verilator lint_on UNUSEDPARAM */

  assign unused_level2_ck_gate = Level2CkGate[0:0];

  if (PreviewClkSameAsRf) begin : gen_same_preview_clk
    always_ff @(negedge rf_clk_i) begin
      if (wr_data_a_en_1p_next_i) begin
        wr_data_a_del_q <= wr_data_a_i;
      end
      if (wr_data_b_en_1p_next_i) begin
        wr_data_b_del_q <= wr_data_b_i;
      end
    end
  end else begin : gen_split_preview_clk
    always_ff @(negedge preview_clk_i) begin
      if (wr_data_a_en_1p_next_i) begin
        wr_data_a_del_q <= wr_data_a_i;
      end
      if (wr_data_b_en_1p_next_i) begin
        wr_data_b_del_q <= wr_data_b_i;
      end
    end
  end

  always_ff @(posedge rf_clk_i) begin
    if (wr_en_b_i) begin
      rf_b_rd_a_q[wr_addr_b_i] <= wr_data_b_del_q;
      rf_b_rd_b_q[wr_addr_b_i] <= wr_data_b_del_q;
      rf_b_rd_c_q[wr_addr_b_i] <= wr_data_b_del_q;
      lvt_q[wr_addr_b_i] <= 1'b1;
    end
    if (wr_en_a_i) begin
      rf_a_rd_a_q[wr_addr_a_i] <= wr_data_a_del_q;
      rf_a_rd_b_q[wr_addr_a_i] <= wr_data_a_del_q;
      rf_a_rd_c_q[wr_addr_a_i] <= wr_data_a_del_q;
      lvt_q[wr_addr_a_i] <= 1'b0;
    end
  end

  assign rd_data_a_o = lvt_q[rd_addr_a_i] ? rf_b_rd_a_q[rd_addr_a_i] : rf_a_rd_a_q[rd_addr_a_i];
  assign rd_data_b_o = lvt_q[rd_addr_b_i] ? rf_b_rd_b_q[rd_addr_b_i] : rf_a_rd_b_q[rd_addr_b_i];
  assign rd_data_c_o = lvt_q[rd_addr_c_i] ? rf_b_rd_c_q[rd_addr_c_i] : rf_a_rd_c_q[rd_addr_c_i];

endmodule : prim_rf_3r2w_preview
