// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ULX3S board wrapper for minion_top_exec.

module minion_top_exec_ulx3s (
  input  logic       clk_25mhz,
  input  logic       btn_pwr_n,
  output logic [7:0] led,
  output logic       ftdi_rxd,
  input  logic       ftdi_txd
);

  localparam int unsigned StartupHoldCycles = 10_000_000;  // 1 s @ 10 MHz
  localparam int unsigned StartupHoldW = $clog2(StartupHoldCycles + 1);

  logic                     clk_pll;
  logic [StartupHoldW-1:0] startup_cnt_q;
  logic                     pll_locked;
  logic                     rst_ni;
  logic                     unused_ftdi_txd;

  minion_top_exec_ulx3s_pll u_pll (
    .clkin_i  (clk_25mhz),
    .clkout_o (clk_pll),
    .locked_o (pll_locked)
  );

  always_ff @(posedge clk_pll or negedge btn_pwr_n) begin
    if (!btn_pwr_n) begin
      startup_cnt_q <= '0;
    end else if (!pll_locked) begin
      startup_cnt_q <= '0;
    end else if (startup_cnt_q != StartupHoldCycles[StartupHoldW-1:0]) begin
      startup_cnt_q <= startup_cnt_q + {{(StartupHoldW-1){1'b0}}, 1'b1};
    end
  end

  assign rst_ni = btn_pwr_n & pll_locked & (startup_cnt_q == StartupHoldCycles[StartupHoldW-1:0]);
  assign unused_ftdi_txd = ftdi_txd;

  minion_top_exec #(
    .ClkHz(10_000_000),
    .BaudRate(115200),
    .TimeoutCycles(10_000_000),
    // Allow FTDI re-enumeration after programming before the one-shot UART
    // summary is emitted.
    .BootDelayCycles(100_000_000)
  ) u_demo (
    .clk_i    (clk_pll),
    .rst_ni,
    .uart_tx_o(ftdi_rxd),
    .led_o    (led)
  );

endmodule
