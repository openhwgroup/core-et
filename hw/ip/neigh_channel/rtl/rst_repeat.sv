// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module rst_repeat
  import dft_pkg::*;
(
  output logic reset_out,
  input  logic reset_in,
  input  logic clock,
  input  logic dft__reset_byp,
  input  logic dft__reset
);

  dft_t dft;
  logic rst_no;

  assign dft = '{scanmode: dft__reset_byp,
                 scan_reset_n: !dft__reset,
                 sram_clk_override: 1'b0,
                 ram_rei: 1'b0,
                 ram_wei: 1'b0};

  prim_rst_sync u_rst_sync (
    .clk_i  (clock),
    .rst_ni (!reset_in),
    .dft_i  (dft),
    .rst_no (rst_no)
  );

  assign reset_out = !rst_no;

endmodule : rst_repeat
