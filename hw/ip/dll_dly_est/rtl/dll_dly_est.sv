// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module dll_dly_est
  import esr_pkg::*;
#(
  parameter int unsigned Width = 8
) (
  input  logic                 clk_shire_i,
  input  logic                 rst_shire_ni,
  input  logic                 clk_neigh_i,
  input  logic                 rst_neigh_ni,

  input  esr_dll_dly_est_ctl_t dll_dly_est_ctl_i,
  output esr_dll_dly_est_sts_t dll_dly_est_sts_o
);

  logic [7:0]       tx_dly_est_code;
  logic             tx_dly_est_write;
  logic [7:0]       rx_dly_est_code;
  logic [Width-1:0] tx_wr_data;
  logic [Width-1:0] rx_rd_data;

  // Preserve original wrapper semantics: do not override core parameters.
  // Wrapper Width only sizes the semisynchronous register payload.
  dll_dly_est_core u_dll_dly_est_core_pd_hv (
    .clk_i              (clk_shire_i),
    .rst_ni             (rst_shire_ni),
    .dll_dly_est_ctl_i  (dll_dly_est_ctl_i),
    .dll_dly_est_sts_o  (dll_dly_est_sts_o),
    .tx_dly_est_write_o (tx_dly_est_write),
    .tx_dly_est_code_o  (tx_dly_est_code),
    .rx_dly_est_code_i  (rx_dly_est_code)
  );

  generate
    if (Width == 8) begin : gen_width_exact
      assign tx_wr_data = tx_dly_est_code;
      assign rx_dly_est_code = rx_rd_data;
    end else if (Width > 8) begin : gen_width_wide
      assign tx_wr_data = {{(Width - 8){1'b0}}, tx_dly_est_code};
      assign rx_dly_est_code = rx_rd_data[7:0];
    end else begin : gen_width_narrow
      assign tx_wr_data = tx_dly_est_code[Width-1:0];
      assign rx_dly_est_code = {{(8 - Width){1'b0}}, rx_rd_data};
    end
  endgenerate

  // semisync_reg_wr_hiv behavior, inlined to keep this single-use register
  // crossing local to the DLL wrapper. The original level shifters are logical
  // pass-throughs in RTL simulation; ASIC insertion remains at the voltage-
  // crossing boundary that owns the clocks.
  logic             tx_wr_en_d1_hv_q;
  logic [Width-1:0] tx_wr_data_d1_hv_q;
  logic             data_valid_loopback_q;
  logic [Width-1:0] data_loopback_q;

  always_ff @(posedge clk_shire_i or negedge rst_shire_ni) begin
    if (!rst_shire_ni) begin
      tx_wr_en_d1_hv_q <= 1'b0;
    end else if (tx_wr_en_d1_hv_q || tx_dly_est_write) begin
      tx_wr_en_d1_hv_q <= tx_dly_est_write;
    end
  end

  always_ff @(posedge clk_shire_i) begin
    if (tx_dly_est_write) begin
      tx_wr_data_d1_hv_q <= tx_wr_data;
    end
  end

  always_ff @(posedge clk_neigh_i or negedge rst_neigh_ni) begin
    if (!rst_neigh_ni) begin
      data_valid_loopback_q <= 1'b0;
    end else if (data_valid_loopback_q || tx_wr_en_d1_hv_q) begin
      data_valid_loopback_q <= tx_wr_en_d1_hv_q;
    end
  end

  always_ff @(posedge clk_neigh_i) begin
    if (tx_wr_en_d1_hv_q) begin
      data_loopback_q <= tx_wr_data_d1_hv_q;
    end
  end

  // semisync_reg_wr_lov behavior for the loopback return. The read-side
  // rd_valid signal is intentionally omitted because the original wrapper left
  // it unconnected and the DLL core generates its own expected valid delay.
  logic             rx_wr_en_d1_lv_q;
  logic [Width-1:0] rx_wr_data_d1_lv_q;

  always_ff @(posedge clk_neigh_i or negedge rst_neigh_ni) begin
    if (!rst_neigh_ni) begin
      rx_wr_en_d1_lv_q <= 1'b0;
    end else if (rx_wr_en_d1_lv_q || data_valid_loopback_q) begin
      rx_wr_en_d1_lv_q <= data_valid_loopback_q;
    end
  end

  always_ff @(posedge clk_neigh_i) begin
    if (data_valid_loopback_q) begin
      rx_wr_data_d1_lv_q <= data_loopback_q;
    end
  end

  always_ff @(posedge clk_shire_i) begin
    if (rx_wr_en_d1_lv_q) begin
      rx_rd_data <= rx_wr_data_d1_lv_q;
    end
  end

endmodule
