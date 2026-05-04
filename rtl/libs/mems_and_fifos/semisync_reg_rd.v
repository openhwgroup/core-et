// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// semisync_reg_rd

module semisync_reg_rd #(parameter
   WIDTH = 1
) (
   input  logic              clock_rd,
   input  logic              reset_rd,

   input  logic              wr_en_d1,
   input  logic [WIDTH-1:0]  wr_data_d1,

   output logic              rd_valid,
   output logic [WIDTH-1:0]  rd_data
);
   
   `RST_EN_FF (clock_rd, reset_rd, rd_valid || wr_en_d1, rd_valid,  wr_en_d1, '0)
   `EN_FF (    clock_rd,           wr_en_d1,             rd_data,   wr_data_d1)

endmodule
