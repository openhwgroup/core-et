// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_log_rom_case_c0 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [25:0] rom_data;

  always_comb begin
    data      = '0;
    data.c0   = {5'b0, rom_data, 1'b0};
    data.mask = 'b1;
  end

    always @*
      case(`TRANS_ROM_GET_INDEX(`TRANS_LOG_ADDR_SIZE, index))
        6'h20: rom_data = 26'h2a3fe5c;
        6'h21: rom_data = 26'h29b9ec2;
        6'h22: rom_data = 26'h2935bc0;
        6'h23: rom_data = 26'h28b34ae;
        6'h24: rom_data = 26'h28328f9;
        6'h25: rom_data = 26'h27b37ff;
        6'h26: rom_data = 26'h2736120;
        6'h27: rom_data = 26'h26ba3db;
        6'h28: rom_data = 26'h263ffa5;
        6'h29: rom_data = 26'h25c73e6;
        6'h2a: rom_data = 26'h255001a;
        6'h2b: rom_data = 26'h24da3d2;
        6'h2c: rom_data = 26'h2465e89;
        6'h2d: rom_data = 26'h23f2fc1;
        6'h2e: rom_data = 26'h2381707;
        6'h2f: rom_data = 26'h23113f1;
        6'h30: rom_data = 26'h22a2605;
        6'h31: rom_data = 26'h2234cdc;
        6'h32: rom_data = 26'h21c880f;
        6'h33: rom_data = 26'h215d736;
        6'h34: rom_data = 26'h20f39e8;
        6'h35: rom_data = 26'h208afd4;
        6'h36: rom_data = 26'h2023896;
        6'h37: rom_data = 26'h1fbd3cb;
        6'h38: rom_data = 26'h1f58123;
        6'h39: rom_data = 26'h1ef404c;
        6'h3a: rom_data = 26'h1e910e0;
        6'h3b: rom_data = 26'h1e2f2a2;
        6'h3c: rom_data = 26'h1dce530;
        6'h3d: rom_data = 26'h1d6e84c;
        6'h3e: rom_data = 26'h1d0fba3;
        6'h3f: rom_data = 26'h1cb1ee8;
        6'h00: rom_data = 26'h1c551d6;
        6'h01: rom_data = 26'h1b9e59e;
        6'h02: rom_data = 26'h1aeb4dd;
        6'h03: rom_data = 26'h1a3bd8d;
        6'h04: rom_data = 26'h198fdba;
        6'h05: rom_data = 26'h18e73a3;
        6'h06: rom_data = 26'h1841d88;
        6'h07: rom_data = 26'h179f9a9;
        6'h08: rom_data = 26'h170068c;
        6'h09: rom_data = 26'h166429e;
        6'h0a: rom_data = 26'h15cac6c;
        6'h0b: rom_data = 26'h153429a;
        6'h0c: rom_data = 26'h14a03c7;
        6'h0d: rom_data = 26'h140eebc;
        6'h0e: rom_data = 26'h1380236;
        6'h0f: rom_data = 26'h12f3cff;
        6'h10: rom_data = 26'h1269e0e;
        6'h11: rom_data = 26'h11e2446;
        6'h12: rom_data = 26'h115ce9e;
        6'h13: rom_data = 26'h10d9c17;
        6'h14: rom_data = 26'h1058bb7;
        6'h15: rom_data = 26'h0fd9c96;
        6'h16: rom_data = 26'h0f5cdd0;
        6'h17: rom_data = 26'h0ee1e8b;
        6'h18: rom_data = 26'h0e68dfc;
        6'h19: rom_data = 26'h0df1b4c;
        6'h1a: rom_data = 26'h0d7c5bf;
        6'h1b: rom_data = 26'h0d08c96;
        6'h1c: rom_data = 26'h0c96f23;
        6'h1d: rom_data = 26'h0c26cb3;
        6'h1e: rom_data = 26'h0bb8495;
        default: rom_data = 26'h0b4b621;
            endcase

endmodule

