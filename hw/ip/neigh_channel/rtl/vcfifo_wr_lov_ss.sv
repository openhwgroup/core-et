// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vcfifo_wr_lov_ss
  import dft_pkg::*;
#(
  parameter int unsigned ELEM_SIZE = 32,
  parameter int unsigned NUM_ELEMS = 4
) (
  input  logic                  clock_push,
  input  logic                  reset_push,
  input  logic                  clock_pop,
  input  logic                  reset_pop,
  input  logic                  push,
  input  logic [ELEM_SIZE-1:0]  push_data,
  input  logic                  pop,
  output logic [ELEM_SIZE-1:0]  pop_data,
  output logic                  ready,
  output logic                  valid,
  input  logic                  dft__reset_byp_push,
  input  logic                  dft__reset_push,
  input  logic                  dft__reset_byp_pop,
  input  logic                  dft__reset_pop
);

  dft_t dft_lv;
  dft_t dft_hv;

  assign dft_lv = '{scanmode: dft__reset_byp_push,
                    scan_reset_n: !dft__reset_push,
                    sram_clk_override: 1'b0,
                    ram_rei: 1'b0,
                    ram_wei: 1'b0};
  assign dft_hv = '{scanmode: dft__reset_byp_pop,
                    scan_reset_n: !dft__reset_pop,
                    sram_clk_override: 1'b0,
                    ram_rei: 1'b0,
                    ram_wei: 1'b0};

  prim_fifo_semisync_lov #(
    .Width(ELEM_SIZE),
    .Depth(NUM_ELEMS)
  ) u_fifo (
    .clk_wr_i(clock_push),
    .rst_wr_ni(!reset_push),
    .push_i(push),
    .wdata_i(push_data),
    .ready_o(ready),
    .clk_rd_i(clock_pop),
    .rst_rd_ni(!reset_pop),
    .pop_i(pop),
    .rdata_o(pop_data),
    .valid_o(valid),
    .dft_hv_i(dft_hv),
    .dft_lv_i(dft_lv)
  );

endmodule : vcfifo_wr_lov_ss
