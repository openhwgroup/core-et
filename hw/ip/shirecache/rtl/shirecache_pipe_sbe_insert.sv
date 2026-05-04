// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Single-bit error injection for manufacturing test (DFT).
//
// Watches write requests and injects a single-bit error on every
// InsertRatio'th write, cycling through all bit positions.
// When InsertEnable=0 (default), outputs all zeros (no injection).
//
// Replaces: shire_cache_pipe_sbe_insert (etcore-soc)
//
// Differences from original:
//   - Active-low async reset
//   - InsertEnable/InsertRatio parameters instead of `ifdef/`define
//   - lowRISC naming conventions

// When InsertEnable=0 (default), clk/rst/wr_en are unused — output is tied to zero.
/* verilator lint_off UNUSEDSIGNAL */  // clk_i/rst_ni/wr_en_i unused when InsertEnable=0 (output tied to zero)
module shirecache_pipe_sbe_insert #(
  parameter int unsigned DataWidth       = 64,
  parameter int unsigned EccWidth        = 8,
  parameter bit          InsertEnable    = 1'b0,
  parameter int unsigned InsertRatio     = 32,
  localparam int unsigned DataAndEccWidth = DataWidth + EccWidth
) (
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic                       wr_en_i,
  output logic [DataAndEccWidth-1:0] data_ecc_bit_flip_o
);

  if (!InsertEnable) begin : gen_disabled

    assign data_ecc_bit_flip_o = '0;

  end else begin : gen_enabled

    localparam int unsigned InsertCntSize       = $clog2(InsertRatio + 1);
    localparam int unsigned DataAndEccSizeLog2  = $clog2(DataAndEccWidth);

    logic [InsertCntSize-1:0]       insert_count_q;
    logic [DataAndEccSizeLog2-1:0]  bit_count_q;

    logic                           insert_en;
    logic [InsertCntSize-1:0]       insert_count_d;
    logic [DataAndEccSizeLog2-1:0]  bit_count_d;

    always_comb begin
      insert_count_d = (insert_count_q == InsertCntSize'(InsertRatio - 1)) ?
                       '0 : insert_count_q + InsertCntSize'(1);
      insert_en = wr_en_i && (insert_count_q == InsertCntSize'(InsertRatio - 1));
      bit_count_d = (bit_count_q == DataAndEccSizeLog2'(DataAndEccWidth - 1)) ?
                    '0 : bit_count_q + DataAndEccSizeLog2'(1);
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni)         insert_count_q <= '0;
      else if (wr_en_i)   insert_count_q <= insert_count_d;
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni)         bit_count_q <= '0;
      else if (insert_en) bit_count_q <= bit_count_d;
    end

    always_comb begin
      data_ecc_bit_flip_o = '0;
      for (int unsigned ii = 0; ii < DataAndEccWidth; ii++) begin
        data_ecc_bit_flip_o[ii] = insert_en && (ii == int'(bit_count_q));
      end
    end

  end

endmodule : shirecache_pipe_sbe_insert
