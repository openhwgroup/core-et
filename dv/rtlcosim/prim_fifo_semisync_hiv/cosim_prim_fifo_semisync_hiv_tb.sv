// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_prim_fifo_semisync_hiv_tb
  import dft_pkg::*;
#(
  parameter int unsigned Width = 8,
  parameter int unsigned Depth = 3,
  parameter int unsigned PushRelay = 0,
  parameter int unsigned PopRelay = 0
) (
  input  logic             clk_wr_i,
  input  logic             rst_wr_ni,
  input  logic             push_i,
  input  logic [Width-1:0] wdata_i,
  input  logic             clk_rd_i,
  input  logic             rst_rd_ni,
  input  logic             pop_i,
  input  logic             dft_hv_scanmode_i,
  input  logic             dft_hv_scan_reset_ni,
  input  logic             dft_lv_scanmode_i,
  input  logic             dft_lv_scan_reset_ni,
  output logic             orig_ready_o,
  output logic             new_ready_o,
  output logic             orig_valid_o,
  output logic             new_valid_o,
  output logic [Width-1:0] orig_rdata_o,
  output logic [Width-1:0] new_rdata_o
);

  dft_t dft_hv;
  dft_t dft_lv;

  assign dft_hv = '{
    scanmode: dft_hv_scanmode_i,
    scan_reset_n: dft_hv_scan_reset_ni,
    sram_clk_override: 1'b0,
    ram_rei: 1'b0,
    ram_wei: 1'b0
  };

  assign dft_lv = '{
    scanmode: dft_lv_scanmode_i,
    scan_reset_n: dft_lv_scan_reset_ni,
    sram_clk_override: 1'b0,
    ram_rei: 1'b0,
    ram_wei: 1'b0
  };

  vcfifo_wr_hiv_ss #(
    .ELEM_SIZE(Width),
    .NUM_ELEMS(Depth),
    .PUSH_RELAY(PushRelay),
    .POP_RELAY(PopRelay)
  ) u_orig (
    .clock_push(clk_wr_i),
    .reset_push(~rst_wr_ni),
    .clock_pop(clk_rd_i),
    .reset_pop(~rst_rd_ni),
    .ready(orig_ready_o),
    .push(push_i),
    .push_data(wdata_i),
    .valid(orig_valid_o),
    .pop(pop_i),
    .pop_data(orig_rdata_o),
    .dft__reset_byp_push(dft_hv_scanmode_i),
    .dft__reset_push(~dft_hv_scan_reset_ni),
    .dft__reset_byp_pop(dft_lv_scanmode_i),
    .dft__reset_pop(~dft_lv_scan_reset_ni)
  );

  prim_fifo_semisync_hiv #(
    .Width(Width),
    .Depth(Depth),
    .PushRelay(PushRelay),
    .PopRelay(PopRelay)
  ) u_new (
    .clk_wr_i(clk_wr_i),
    .rst_wr_ni(rst_wr_ni),
    .push_i(push_i),
    .wdata_i(wdata_i),
    .ready_o(new_ready_o),
    .clk_rd_i(clk_rd_i),
    .rst_rd_ni(rst_rd_ni),
    .pop_i(pop_i),
    .rdata_o(new_rdata_o),
    .valid_o(new_valid_o),
    .dft_hv_i(dft_hv),
    .dft_lv_i(dft_lv)
  );

endmodule
