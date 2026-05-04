// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_1r1w_diff_preview #(
  parameter int unsigned RWidth     = 32,
  parameter int unsigned RAlignment = 32,
  parameter int unsigned WWidth     = 32,
  parameter int unsigned Entries    = 32,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned R2WRatio  = WWidth / RWidth,
  localparam int unsigned REntries  = (Entries * WWidth) / RWidth * (RWidth / RAlignment),
  localparam int unsigned RAddrW    = $clog2(REntries),
  localparam int unsigned WAddrW    = $clog2(Entries)
) (
  input  logic                    preview_clk_i,
  input  logic                    rf_clk_i,
  input  logic [R2WRatio-1:0]     wr_data_en_1p_next_i,
  output logic [RWidth-1:0]       rd_data_o,
  input  logic [RAddrW-1:0]       rd_addr_i,
  input  logic [WWidth-1:0]       wr_data_i,
  input  logic [WAddrW-1:0]       wr_addr_i,
  input  logic [R2WRatio-1:0]     wr_en_i
);

  localparam bit Ecp5SupportedSize =
      (RWidth == 32 && RAlignment == 32 && WWidth == 32 && Entries == 32)
   || (RWidth == 8 && RAlignment == 8 && WWidth == 8  && Entries == 2)
   || (RWidth == 8 && RAlignment == 8 && WWidth == 16 && Entries == 2)
   || (RWidth == 32 && RAlignment == 16 && WWidth == 256 && Entries == 2);

  if (!Ecp5SupportedSize) begin : gen_unsupported_size
    initial begin
      $error("Unsupported ECP5 prim_rf_1r1w_diff_preview size: RWidth=%0d RAlignment=%0d WWidth=%0d Entries=%0d",
             RWidth, RAlignment, WWidth, Entries);
      $finish;
    end
    prim_rf_1r1w_diff_preview_ecp5_unsupported_size u_compile_time_error();
  end

  (* ram_style = "distributed" *) logic [WWidth-1:0] rf_q [Entries];
  logic [WWidth-1:0] wr_data_del_q;
  logic [WWidth*Entries-1:0]            rf_visible;
  logic [WWidth*Entries-1:0]            rf_full;
  logic [WWidth*Entries+RAlignment-1:0] rf_full_ext;
  logic [RWidth-1:0]                    rf_read [REntries];

  if (PreviewClkSameAsRf) begin : gen_same_preview_clk
    always_ff @(negedge rf_clk_i) begin
      for (int j = 0; j < R2WRatio; j++) begin
        if (wr_data_en_1p_next_i[j]) begin
          wr_data_del_q[j*RWidth +: RWidth] <= wr_data_i[j*RWidth +: RWidth];
        end
      end
    end
  end else begin : gen_split_preview_clk
    always_ff @(negedge preview_clk_i) begin
      for (int j = 0; j < R2WRatio; j++) begin
        if (wr_data_en_1p_next_i[j]) begin
          wr_data_del_q[j*RWidth +: RWidth] <= wr_data_i[j*RWidth +: RWidth];
        end
      end
    end
  end

  always_comb begin
    for (int j = 0; j < Entries; j++) begin
      rf_full[j*WWidth +: WWidth] = rf_q[j];
    end

    rf_visible = rf_full;
    for (int entry = 0; entry < Entries; entry++) begin
      if (wr_addr_i == entry[WAddrW-1:0]) begin
        for (int lane = 0; lane < R2WRatio; lane++) begin
          if (wr_en_i[lane]) begin
            rf_visible[entry*WWidth + lane*RWidth +: RWidth] =
                wr_data_del_q[lane*RWidth +: RWidth];
          end
        end
      end
    end

    rf_full_ext = {rf_visible[RAlignment-1:0], rf_visible};

    for (int j = 0; j < REntries; j++) begin
      rf_read[j] = rf_full_ext[j*RAlignment +: RWidth];
    end
  end

  always_ff @(posedge rf_clk_i) begin
    for (int j = 0; j < R2WRatio; j++) begin
      if (wr_en_i[j]) begin
        rf_q[wr_addr_i][j*RWidth +: RWidth] <= wr_data_del_q[j*RWidth +: RWidth];
      end
    end
  end

  assign rd_data_o = rf_read[rd_addr_i];

endmodule : prim_rf_1r1w_diff_preview
