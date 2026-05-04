// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Simple 8N1 UART transmitter.

module ulx3s_uart_tx #(
  parameter int unsigned ClkHz    = 25_000_000,
  parameter int unsigned BaudRate = 115200,
  localparam int unsigned ClksPerBit = (ClkHz + (BaudRate / 2)) / BaudRate,
  localparam int unsigned CtrW = (ClksPerBit > 1) ? $clog2(ClksPerBit) : 1,
  localparam logic [31:0] BitCtrReload = ClksPerBit - 1
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       valid_i,
  input  logic [7:0] data_i,
  output logic       ready_o,
  output logic       tx_o,
  output logic       busy_o
);

  typedef enum logic [1:0] {
    StIdle,
    StStart,
    StData,
    StStop
  } state_e;

  state_e            state_d, state_q;
  logic [CtrW-1:0]   baud_ctr_d, baud_ctr_q;
  logic [2:0]        bit_idx_d, bit_idx_q;
  logic [7:0]        shreg_d, shreg_q;

  always_comb begin
    state_d    = state_q;
    baud_ctr_d = baud_ctr_q;
    bit_idx_d  = bit_idx_q;
    shreg_d    = shreg_q;

    unique case (state_q)
      StIdle: begin
        if (valid_i) begin
          state_d    = StStart;
          baud_ctr_d = BitCtrReload[CtrW-1:0];
          bit_idx_d  = '0;
          shreg_d    = data_i;
        end
      end

      StStart: begin
        if (baud_ctr_q == '0) begin
          state_d    = StData;
          baud_ctr_d = BitCtrReload[CtrW-1:0];
        end else begin
          baud_ctr_d = baud_ctr_q - 1'b1;
        end
      end

      StData: begin
        if (baud_ctr_q == '0) begin
          baud_ctr_d = BitCtrReload[CtrW-1:0];
          if (bit_idx_q == 3'd7) begin
            state_d = StStop;
          end else begin
            bit_idx_d = bit_idx_q + 3'd1;
            shreg_d   = {1'b0, shreg_q[7:1]};
          end
        end else begin
          baud_ctr_d = baud_ctr_q - 1'b1;
        end
      end

      StStop: begin
        if (baud_ctr_q == '0) begin
          state_d = StIdle;
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
      shreg_q    <= '0;
    end else begin
      state_q    <= state_d;
      baud_ctr_q <= baud_ctr_d;
      bit_idx_q  <= bit_idx_d;
      shreg_q    <= shreg_d;
    end
  end

  always_comb begin
    ready_o = 1'b0;
    tx_o    = 1'b1;
    busy_o  = (state_q != StIdle);

    unique case (state_q)
      StIdle: begin
        ready_o = 1'b1;
        tx_o    = 1'b1;
      end
      StStart: tx_o = 1'b0;
      StData:  tx_o = shreg_q[0];
      StStop:  tx_o = 1'b1;
      default: tx_o = 1'b1;
    endcase
  end

endmodule
