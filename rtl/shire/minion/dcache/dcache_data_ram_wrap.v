// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// wrapper for separating the dcache data ram into two 32-bit wide rams

module dcache_data_ram_wrap(
  clk,
  rd_addr,
  wr_addr,
  rd_enable,
  rd_enablelob,
  rd_enablehib,
  wr_enablehi,
  wr_enablelo,
  rd_data,
  wr_data
);

  input [6:0]  rd_addr, wr_addr;
  input [63:0] wr_data;
  input        rd_enable, rd_enablelob, rd_enablehib;
  input        wr_enablehi, wr_enablelo;
  input        clk;
  output [63:0] rd_data; // the register values read

  dcache_128x32_1r1w_lram dcache_128x32_1r1w_lram_low (
    // System signals
    .clk           ( clk                       ),
    // Read port
    .rd_addr       ( rd_addr                   ),
    .rd_data       ( rd_data[31:0]             ),
    .rd_enable     ( rd_enable & !rd_enablelob ),
    // Write port
    .wr_addr       ( wr_addr                   ),
    .wr_data       ( wr_data[31:0]             ),
    .wr_enable     ( wr_enablelo               )

  );

  dcache_128x32_1r1w_lram dcache_128x32_1r1w_lram_high (
    // System signals
    .clk           ( clk                       ),
    // Read port
    .rd_addr       ( rd_addr                   ),
    .rd_data       ( rd_data[63:32]            ),
    .rd_enable     ( rd_enable & !rd_enablehib ),
    // Write port
    .wr_addr       ( wr_addr                   ),
    .wr_data       ( wr_data[63:32]            ),
    .wr_enable     ( wr_enablehi               )

  );

endmodule
