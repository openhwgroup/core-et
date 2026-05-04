// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ULX3S UART bring-up top.
//
// - Sends a boot banner once after reset.
// - Echoes received UART bytes after the banner has been queued.
// - Uses prim_fifo_sync from the repo as the TX queue.

module ulx3s_uart_bringup #(
  parameter int unsigned ClkHz       = 25_000_000,
  parameter int unsigned BaudRate    = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TxFifoDepth = 64,
  localparam int unsigned TxDepthW   = $clog2(TxFifoDepth + 1),
  localparam int unsigned BootDelayW = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned BootMsgLen = 32,
  localparam int unsigned BootIdxW   = $clog2(BootMsgLen + 1)
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       uart_rx_i,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  logic [7:0] tx_wdata;
  logic       tx_wvalid;
  logic       tx_wready;
  logic [7:0] tx_rdata;
  logic       tx_rvalid;
  logic       tx_rready;
  logic       tx_pop_ready;
  logic [TxDepthW-1:0] tx_depth;
  logic       tx_full;
  logic       tx_empty;

  logic [7:0] rx_data;
  logic       rx_valid;
  logic       tx_busy;
  logic [BootDelayW-1:0] boot_delay_q;
  logic [BootIdxW-1:0] boot_idx_q;
  logic       rx_activity_q;
  logic       drop_activity_q;
  logic       tx_depth_xor;
  localparam logic [31:0] BootMsgLenVal = BootMsgLen;
  localparam logic [31:0] BootDelayInitVal = BootDelayCycles;

  function automatic logic [7:0] boot_msg_byte(input logic [BootIdxW-1:0] idx);
    unique case (idx)
      0:  boot_msg_byte = 8'h41;  // A
      1:  boot_msg_byte = 8'h69;  // i
      2:  boot_msg_byte = 8'h6e;  // n
      3:  boot_msg_byte = 8'h65;  // e
      4:  boot_msg_byte = 8'h6b;  // k
      5:  boot_msg_byte = 8'h6b;  // k
      6:  boot_msg_byte = 8'h6f;  // o
      7:  boot_msg_byte = 8'h20;  // space
      8:  boot_msg_byte = 8'h55;  // U
      9:  boot_msg_byte = 8'h4c;  // L
      10: boot_msg_byte = 8'h58;  // X
      11: boot_msg_byte = 8'h33;  // 3
      12: boot_msg_byte = 8'h53;  // S
      13: boot_msg_byte = 8'h20;  // space
      14: boot_msg_byte = 8'h55;  // U
      15: boot_msg_byte = 8'h41;  // A
      16: boot_msg_byte = 8'h52;  // R
      17: boot_msg_byte = 8'h54;  // T
      18: boot_msg_byte = 8'h20;  // space
      19: boot_msg_byte = 8'h54;  // T
      20: boot_msg_byte = 8'h45;  // E
      21: boot_msg_byte = 8'h53;  // S
      22: boot_msg_byte = 8'h54;  // T
      23: boot_msg_byte = 8'h20;  // space
      24: boot_msg_byte = 8'h50;  // P
      25: boot_msg_byte = 8'h41;  // A
      26: boot_msg_byte = 8'h53;  // S
      27: boot_msg_byte = 8'h53;  // S
      28: boot_msg_byte = 8'h45;  // E
      29: boot_msg_byte = 8'h44;  // D
      30: boot_msg_byte = 8'h0d;  // \r
      31: boot_msg_byte = 8'h0a;  // \n
      default: boot_msg_byte = 8'h00;
    endcase
  endfunction

  wire boot_delay_done = (boot_delay_q == '0);
  wire boot_done = (boot_idx_q == BootMsgLenVal[BootIdxW-1:0]);
  wire boot_push = boot_delay_done && !boot_done && tx_wvalid && tx_wready;
  wire echo_drop = boot_done && rx_valid && !tx_wready;

  always_comb begin
    tx_wvalid = 1'b0;
    tx_wdata  = 8'h00;

    if (boot_delay_done && !boot_done) begin
      tx_wvalid = 1'b1;
      tx_wdata  = boot_msg_byte(boot_idx_q);
    end else if (rx_valid) begin
      tx_wvalid = 1'b1;
      tx_wdata  = rx_data;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      boot_delay_q    <= BootDelayInitVal[BootDelayW-1:0];
      boot_idx_q      <= '0;
      rx_activity_q   <= 1'b0;
      drop_activity_q <= 1'b0;
    end else begin
      if (!boot_delay_done) begin
        boot_delay_q <= boot_delay_q - 1'b1;
      end

      if (boot_push) begin
        boot_idx_q <= boot_idx_q + 1'b1;
      end

      if (rx_valid) begin
        rx_activity_q  <= ~rx_activity_q;
      end

      if (echo_drop) begin
        drop_activity_q <= ~drop_activity_q;
      end
    end
  end

  prim_fifo_sync #(
    .Width(8),
    .Depth(TxFifoDepth)
  ) u_tx_fifo (
    .clk_i,
    .rst_ni,
    .clr_i    (1'b0),
    .wvalid_i (tx_wvalid),
    .wready_o (tx_wready),
    .wdata_i  (tx_wdata),
    .rvalid_o (tx_rvalid),
    .rready_i (tx_pop_ready),
    .rdata_o  (tx_rdata),
    .depth_o  (tx_depth),
    .full_o   (tx_full),
    .empty_o  (tx_empty)
  );

  ulx3s_uart_rx #(
    .ClkHz(ClkHz),
    .BaudRate(BaudRate)
  ) u_rx (
    .clk_i,
    .rst_ni,
    .rx_i    (uart_rx_i),
    .data_o  (rx_data),
    .valid_o (rx_valid)
  );

  ulx3s_uart_tx #(
    .ClkHz(ClkHz),
    .BaudRate(BaudRate)
  ) u_tx (
    .clk_i,
    .rst_ni,
    .valid_i (tx_rvalid),
    .data_i  (tx_rdata),
    .ready_o (tx_rready),
    .tx_o    (uart_tx_o),
    .busy_o  (tx_busy)
  );

  assign tx_pop_ready = tx_rvalid & tx_rready;
  assign tx_depth_xor = ^tx_depth;

  assign led_o = {
    !boot_done,
    tx_busy,
    tx_full,
    !tx_empty,
    rx_activity_q,
    drop_activity_q,
    tx_depth_xor,
    tx_depth[0]
  };

endmodule
