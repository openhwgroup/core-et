// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Simple 8N1 UART receiver with a two-flop input synchronizer.

module ulx3s_uart_rx #(
  parameter int unsigned ClkHz    = 25_000_000,
  parameter int unsigned BaudRate = 115200,
  localparam int unsigned ClksPerBit = (ClkHz + (BaudRate / 2)) / BaudRate,
  localparam int unsigned HalfClksPerBit = ClksPerBit / 2,
  localparam int unsigned CtrW = (ClksPerBit > 1) ? $clog2(ClksPerBit) : 1,
  localparam logic [31:0] BitCtrReload = ClksPerBit - 1,
  localparam logic [31:0] HalfBitCtrReload = HalfClksPerBit
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       rx_i,
  output logic [7:0] data_o,
  output logic       valid_o
);

  typedef enum logic [1:0] {
    StIdle,
    StStart,
    StData,
    StStop
  } state_e;

  state_e          state_d, state_q;
  logic [CtrW-1:0] baud_ctr_d, baud_ctr_q;
  logic [2:0]      bit_idx_d, bit_idx_q;
  logic [7:0]      data_d, data_q;
  logic            valid_d, valid_q;
  logic            rx_meta_q, rx_sync_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rx_meta_q <= 1'b1;
      rx_sync_q <= 1'b1;
    end else begin
      rx_meta_q <= rx_i;
      rx_sync_q <= rx_meta_q;
    end
  end

  always_comb begin
    state_d    = state_q;
    baud_ctr_d = baud_ctr_q;
    bit_idx_d  = bit_idx_q;
    data_d     = data_q;
    valid_d    = 1'b0;

    unique case (state_q)
      StIdle: begin
        if (!rx_sync_q) begin
          state_d    = StStart;
          baud_ctr_d = HalfBitCtrReload[CtrW-1:0];
          bit_idx_d  = '0;
        end
      end

      StStart: begin
        if (baud_ctr_q == '0) begin
          if (!rx_sync_q) begin
            state_d    = StData;
            baud_ctr_d = BitCtrReload[CtrW-1:0];
          end else begin
            state_d = StIdle;
          end
        end else begin
          baud_ctr_d = baud_ctr_q - 1'b1;
        end
      end

      StData: begin
        if (baud_ctr_q == '0) begin
          data_d[bit_idx_q] = rx_sync_q;
          baud_ctr_d        = BitCtrReload[CtrW-1:0];
          if (bit_idx_q == 3'd7) begin
            state_d = StStop;
          end else begin
            bit_idx_d = bit_idx_q + 3'd1;
          end
        end else begin
          baud_ctr_d = baud_ctr_q - 1'b1;
        end
      end

      StStop: begin
        if (baud_ctr_q == '0) begin
          state_d = StIdle;
          valid_d = rx_sync_q;
        end else begin
          baud_ctr_d = baud_ctr_q - 1'b1;
        end
      end

      default: begin
        state_d = StIdle;
      end
    endcase
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q    <= StIdle;
      baud_ctr_q <= '0;
      bit_idx_q  <= '0;
      data_q     <= '0;
      valid_q    <= 1'b0;
    end else begin
      state_q    <= state_d;
      baud_ctr_q <= baud_ctr_d;
      bit_idx_q  <= bit_idx_d;
      data_q     <= data_d;
      valid_q    <= valid_d;
    end
  end

  assign data_o  = data_q;
  assign valid_o = valid_q;

endmodule
