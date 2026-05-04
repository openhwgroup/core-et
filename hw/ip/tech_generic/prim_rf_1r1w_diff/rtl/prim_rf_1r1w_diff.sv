// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed mixed-width 1R1W register file.
// Generic/default implementation.

module prim_rf_1r1w_diff #(
  parameter int unsigned RWidth     = 32,
  parameter int unsigned RAlignment = 32,
  parameter int unsigned WWidth     = 32,
  parameter int unsigned Entries    = 32,
  localparam int unsigned R2WRatio  = WWidth / RWidth,
  localparam int unsigned REntries  = (Entries * WWidth) / RWidth * (RWidth / RAlignment),
  localparam int unsigned RAddrW    = $clog2(REntries),
  localparam int unsigned WAddrW    = $clog2(Entries)
) (
  input  logic                    clk_i,
  output logic [RWidth-1:0]       rd_data_o,
  input  logic [RAddrW-1:0]       rd_addr_i,
  input  logic [WWidth-1:0]       wr_data_i,
  input  logic [R2WRatio-1:0]     wr_data_en_1p_i,
  input  logic [WAddrW-1:0]       wr_addr_i,
  input  logic [R2WRatio-1:0]     wr_en_i
);

  logic [WWidth-1:0] rf_q [Entries];
  logic [WWidth-1:0] wr_data_del_q;

  logic [WWidth*Entries-1:0]            rf_full;
  logic [WWidth*Entries+RAlignment-1:0] rf_full_ext;
  logic [RWidth-1:0]                    rf_read [REntries];

  always_ff @(negedge clk_i) begin
    for (int j = 0; j < R2WRatio; j++) begin
      if (wr_data_en_1p_i[j]) begin
        wr_data_del_q[j*RWidth +: RWidth] <= wr_data_i[j*RWidth +: RWidth];
      end
    end
  end

  always_ff @(posedge clk_i) begin
    for (int j = 0; j < R2WRatio; j++) begin
      if (wr_en_i[j]) begin
        rf_q[wr_addr_i][j*RWidth +: RWidth] <= wr_data_del_q[j*RWidth +: RWidth];
      end
    end
  end

  always_comb begin
    for (int j = 0; j < Entries; j++) begin
      rf_full[j*WWidth +: WWidth] = rf_q[j];
    end

    rf_full_ext = {rf_full[RAlignment-1:0], rf_full};

    for (int j = 0; j < REntries; j++) begin
      rf_read[j] = rf_full_ext[j*RAlignment +: RWidth];
    end
  end

  assign rd_data_o = rf_read[rd_addr_i];

endmodule : prim_rf_1r1w_diff
