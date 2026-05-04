// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_rcp_rom_case_c0 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [24:0] rom_data;

  always_comb begin
    data      = '0;
    data.c0   = {6'b0, 1'b1, rom_data};
    data.mask = 'b0;
  end

    always @*
            case(`TRANS_ROM_GET_INDEX(`TRANS_RCP_ADDR_SIZE, index))
                7'h0:  rom_data = 25'h1fffffe;
                7'h1:  rom_data = 25'h1f80fe0;
                7'h2:  rom_data = 25'h1f03f04;
                7'h3:  rom_data = 25'h1e88cb3;
                7'h4:  rom_data = 25'h1e0f83d;
                7'h5:  rom_data = 25'h1d980f5;
                7'h6:  rom_data = 25'h1d22636;
                7'h7:  rom_data = 25'h1cae758;
                7'h8:  rom_data = 25'h1c3c3c4;
                7'h9:  rom_data = 25'h1bcbadc;
                7'ha:  rom_data = 25'h1b5cc0d;
                7'hb:  rom_data = 25'h1aef6c9;
                7'hc:  rom_data = 25'h1a83a83;
                7'hd:  rom_data = 25'h1a196b1;
                7'he:  rom_data = 25'h19b0ad0;
                7'hf:  rom_data = 25'h1949661;
                7'h10: rom_data = 25'h18e38e4;
                7'h11: rom_data = 25'h187f1df;
                7'h12: rom_data = 25'h181c0df;
                7'h13: rom_data = 25'h17ba570;
                7'h14: rom_data = 25'h1759f24;
                7'h15: rom_data = 25'h16fad88;
                7'h16: rom_data = 25'h169d035;
                7'h17: rom_data = 25'h16406c7;
                7'h18: rom_data = 25'h15e50d9;
                7'h19: rom_data = 25'h158ae03;
                7'h1a: rom_data = 25'h1531dec;
                7'h1b: rom_data = 25'h14da034;
                7'h1c: rom_data = 25'h1483482;
                7'h1d: rom_data = 25'h142da7e;
                7'h1e: rom_data = 25'h13d91d4;
                7'h1f: rom_data = 25'h1385a28;
                7'h20: rom_data = 25'h1333333;
                7'h21: rom_data = 25'h12e1c9d;
                7'h22: rom_data = 25'h129161e;
                7'h23: rom_data = 25'h1241f6b;
                7'h24: rom_data = 25'h11f3832;
                7'h25: rom_data = 25'h11a6033;
                7'h26: rom_data = 25'h1159721;
                7'h27: rom_data = 25'h110dcbf;
                7'h28: rom_data = 25'h10c30c4;
                7'h29: rom_data = 25'h10792ee;
                7'h2a: rom_data = 25'h1030304;
                7'h2b: rom_data = 25'h0fe80bf;
                7'h2c: rom_data = 25'h0fa0be7;
                7'h2d: rom_data = 25'h0f5a443;
                7'h2e: rom_data = 25'h0f14991;
                7'h2f: rom_data = 25'h0ecfb9c;
                7'h30: rom_data = 25'h0e8ba2d;
                7'h31: rom_data = 25'h0e4850e;
                7'h32: rom_data = 25'h0e05c0a;
                7'h33: rom_data = 25'h0dc3eed;
                7'h34: rom_data = 25'h0d82d82;
                7'h35: rom_data = 25'h0d42798;
                7'h36: rom_data = 25'h0d02d03;
                7'h37: rom_data = 25'h0cc3d8c;
                7'h38: rom_data = 25'h0c8590d;
                7'h39: rom_data = 25'h0c47f4e;
                7'h3a: rom_data = 25'h0c0b02b;
                7'h3b: rom_data = 25'h0bceb77;
                7'h3c: rom_data = 25'h0b93107;
                7'h3d: rom_data = 25'h0b580af;
                7'h3e: rom_data = 25'h0b1da45;
                7'h3f: rom_data = 25'h0ae3da9;
                7'h40: rom_data = 25'h0aaaaac;
                7'h41: rom_data = 25'h0a72129;
                7'h42: rom_data = 25'h0a3a0fc;
                7'h43: rom_data = 25'h0a02a04;
                7'h44: rom_data = 25'h09cbc14;
                7'h45: rom_data = 25'h0995711;
                7'h46: rom_data = 25'h095fad3;
                7'h47: rom_data = 25'h092a73e;
                7'h48: rom_data = 25'h08f5c29;
                7'h49: rom_data = 25'h08c1978;
                7'h4a: rom_data = 25'h088df0e;
                7'h4b: rom_data = 25'h085acc6;
                7'h4c: rom_data = 25'h0828284;
                7'h4d: rom_data = 25'h07f6027;
                7'h4e: rom_data = 25'h07c4596;
                7'h4f: rom_data = 25'h07932b4;
                7'h50: rom_data = 25'h0762761;
                7'h51: rom_data = 25'h0732386;
                7'h52: rom_data = 25'h0702702;
                7'h53: rom_data = 25'h06d31c0;
                7'h54: rom_data = 25'h06a43a0;
                7'h55: rom_data = 25'h0675c8d;
                7'h56: rom_data = 25'h0647c68;
                7'h57: rom_data = 25'h061a31f;
                7'h58: rom_data = 25'h05ed098;
                7'h59: rom_data = 25'h05c04b7;
                7'h5a: rom_data = 25'h0593f6b;
                7'h5b: rom_data = 25'h0568097;
                7'h5c: rom_data = 25'h053c826;
                7'h5d: rom_data = 25'h0511601;
                7'h5e: rom_data = 25'h04e6a19;
                7'h5f: rom_data = 25'h04bc44d;
                7'h60: rom_data = 25'h0492494;
                7'h61: rom_data = 25'h0468ad0;
                7'h62: rom_data = 25'h043f6f1;
                7'h63: rom_data = 25'h04168e0;
                7'h64: rom_data = 25'h03ee08f;
                7'h65: rom_data = 25'h03c5de9;
                7'h66: rom_data = 25'h039e0d4;
                7'h67: rom_data = 25'h0376949;
                7'h68: rom_data = 25'h034f72b;
                7'h69: rom_data = 25'h0328a6f;
                7'h6a: rom_data = 25'h0302301;
                7'h6b: rom_data = 25'h02dc0d0;
                7'h6c: rom_data = 25'h02b63cd;
                7'h6d: rom_data = 25'h0290be3;
                7'h6e: rom_data = 25'h026b901;
                7'h6f: rom_data = 25'h0246b1e;
                7'h70: rom_data = 25'h0222221;
                7'h71: rom_data = 25'h01fde01;
                7'h72: rom_data = 25'h01d9eac;
                7'h73: rom_data = 25'h01b6417;
                7'h74: rom_data = 25'h0192e2b;
                7'h75: rom_data = 25'h016fcdf;
                7'h76: rom_data = 25'h014d022;
                7'h77: rom_data = 25'h012a7e6;
                7'h78: rom_data = 25'h0108420;
                7'h79: rom_data = 25'h00e64c1;
                7'h7a: rom_data = 25'h00c49ba;
                7'h7b: rom_data = 25'h00a3300;
                7'h7c: rom_data = 25'h0082081;
                7'h7d: rom_data = 25'h0061238;
                7'h7e: rom_data = 25'h0040810;
                default: rom_data = 25'h0020203;
            endcase

endmodule
