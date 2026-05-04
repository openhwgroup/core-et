// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// semisync_reg_wr

module semisync_reg_wr #(parameter
   WIDTH = 1
) (
   input  logic              clock_wr,
   input  logic              reset_wr,

   input  logic              wr_en,
   input  logic [WIDTH-1:0]  wr_data,

   output logic              wr_en_d1,
   output logic [WIDTH-1:0]  wr_data_d1
);
   
   `RST_EN_FF (clock_wr, reset_wr, wr_en_d1 || wr_en, wr_en_d1,   wr_en, '0)
   `EN_FF (    clock_wr,           wr_en,             wr_data_d1, wr_data)

endmodule
