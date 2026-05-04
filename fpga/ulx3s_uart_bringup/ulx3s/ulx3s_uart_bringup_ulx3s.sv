// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ULX3S board wrapper for the UART bring-up project.

module ulx3s_uart_bringup_ulx3s (
  input  logic       clk_25mhz,
  input  logic       btn_pwr_n,
  output logic [7:0] led,
  output logic       ftdi_rxd,
  input  logic       ftdi_txd
);

  // FPGA-only power-on reset stretcher. ECP5 initializes this counter to zero
  // at configuration time; pressing BTN_PWR reasserts reset.
  logic [7:0] por_cnt_q = '0;
  logic       rst_ni;

  always_ff @(posedge clk_25mhz or negedge btn_pwr_n) begin
    if (!btn_pwr_n) begin
      por_cnt_q <= '0;
    end else if (!por_cnt_q[7]) begin
      por_cnt_q <= por_cnt_q + 8'd1;
    end
  end

  assign rst_ni = por_cnt_q[7] & btn_pwr_n;

  ulx3s_uart_bringup #(
    .ClkHz(25_000_000),
    .BaudRate(115200),
    .BootDelayCycles(2_500_000)
  ) u_demo (
    .clk_i    (clk_25mhz),
    .rst_ni,
    .uart_rx_i(ftdi_txd),
    .uart_tx_o(ftdi_rxd),
    .led_o    (led)
  );

endmodule
