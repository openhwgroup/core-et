// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ULX3S board wrapper for test_mul_div.

module test_mul_div_ulx3s (
  input  logic       clk_25mhz,
  input  logic       btn_pwr_n,
  output logic [7:0] led,
  output logic       ftdi_rxd,
  input  logic       ftdi_txd
);

  logic clk_pll;
  logic pll_locked;
  logic [7:0] por_cnt_q = '0;
  logic rst_ni;
  logic unused_ftdi_txd;

  test_mul_div_ulx3s_pll u_pll (
    .clkin_i  (clk_25mhz),
    .clkout_o (clk_pll),
    .locked_o (pll_locked)
  );

  always_ff @(posedge clk_pll or negedge btn_pwr_n) begin
    if (!btn_pwr_n) begin
      por_cnt_q <= '0;
    end else if (!por_cnt_q[7]) begin
      por_cnt_q <= por_cnt_q + 8'd1;
    end
  end

  assign rst_ni = por_cnt_q[7] & btn_pwr_n & pll_locked;
  assign unused_ftdi_txd = ftdi_txd;

  test_mul_div #(
    .ClkHz(10_000_000),
    .BaudRate(115200),
    .BootDelayCycles(66_666_667)
  ) u_demo (
    .clk_i    (clk_pll),
    .rst_ni   (rst_ni),
    .uart_tx_o(ftdi_rxd),
    .led_o    (led)
  );

endmodule
