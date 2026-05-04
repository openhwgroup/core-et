// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_rcp_rom_case_c1c2 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [25:0] rom_data;

  always_comb begin
    data      = '0;
    data.c2   = {rom_data[`TRANS_RCP_C2_RANGE], 7'b0};
    data.c0   = {3'h7, rom_data[`TRANS_RCP_C1_RANGE], 5'b0, `TRANS_C1_FILLER};
    data.mask = 'b0;
  end

    always @*
      case(`TRANS_ROM_GET_INDEX(`TRANS_RCP_ADDR_SIZE, index))
        7'h0:  rom_data = 26'h3f50002;
        7'h1:  rom_data = 26'h3dd03f6;
        7'h2:  rom_data = 26'h3c607d3;
        7'h3:  rom_data = 26'h3b10b99;
        7'h4:  rom_data = 26'h39a0f4a;
        7'h5:  rom_data = 26'h38612e5;
        7'h6:  rom_data = 26'h373166b;
        7'h7:  rom_data = 26'h35f19de;
        7'h8:  rom_data = 26'h34c1d3d;
        7'h9:  rom_data = 26'h33b2089;
        7'ha:  rom_data = 26'h32723c4;
        7'hb:  rom_data = 26'h31726ec;
        7'hc:  rom_data = 26'h3062a03;
        7'hd:  rom_data = 26'h2f62d09;
        7'he:  rom_data = 26'h2e62fff;
        7'hf:  rom_data = 26'h2d732e5;
        7'h10: rom_data = 26'h2c735bc;
        7'h11: rom_data = 26'h2b83884;
        7'h12: rom_data = 26'h2ab3b3d;
        7'h13: rom_data = 26'h29d3de8;
        7'h14: rom_data = 26'h2914084;
        7'h15: rom_data = 26'h2844314;
        7'h16: rom_data = 26'h2774597;
        7'h17: rom_data = 26'h26a480d;
        7'h18: rom_data = 26'h25f4a76;
        7'h19: rom_data = 26'h2524cd4;
        7'h1a: rom_data = 26'h2464f26;
        7'h1b: rom_data = 26'h23c516c;
        7'h1c: rom_data = 26'h22f53a8;
        7'h1d: rom_data = 26'h22555d8;
        7'h1e: rom_data = 26'h21b57fd;
        7'h1f: rom_data = 26'h2115a19;
        7'h20: rom_data = 26'h2075c2a;
        7'h21: rom_data = 26'h1ff5e31;
        7'h22: rom_data = 26'h1f5602f;
        7'h23: rom_data = 26'h1ec6223;
        7'h24: rom_data = 26'h1e2640f;
        7'h25: rom_data = 26'h1da65f1;
        7'h26: rom_data = 26'h1d167cb;
        7'h27: rom_data = 26'h1c9699c;
        7'h28: rom_data = 26'h1c16b65;
        7'h29: rom_data = 26'h1b96d26;
        7'h2a: rom_data = 26'h1b16edf;
        7'h2b: rom_data = 26'h1aa7090;
        7'h2c: rom_data = 26'h1a3723a;
        7'h2d: rom_data = 26'h19b73dc;
        7'h2e: rom_data = 26'h1957577;
        7'h2f: rom_data = 26'h18d770c;
        7'h30: rom_data = 26'h1877899;
        7'h31: rom_data = 26'h17f7a20;
        7'h32: rom_data = 26'h17b7b9f;
        7'h33: rom_data = 26'h1737d19;
        7'h34: rom_data = 26'h16e7e8c;
        7'h35: rom_data = 26'h1677ffa;
        7'h36: rom_data = 26'h1618161;
        7'h37: rom_data = 26'h15c82c2;
        7'h38: rom_data = 26'h156841d;
        7'h39: rom_data = 26'h1508574;
        7'h3a: rom_data = 26'h14b86c4;
        7'h3b: rom_data = 26'h146880f;
        7'h3c: rom_data = 26'h1428954;
        7'h3d: rom_data = 26'h13c8a95;
        7'h3e: rom_data = 26'h1378bd1;
        7'h3f: rom_data = 26'h1328d07;
        7'h40: rom_data = 26'h12d8e39;
        7'h41: rom_data = 26'h1298f66;
        7'h42: rom_data = 26'h124908f;
        7'h43: rom_data = 26'h12091b2;
        7'h44: rom_data = 26'h11b92d2;
        7'h45: rom_data = 26'h11793ed;
        7'h46: rom_data = 26'h1129504;
        7'h47: rom_data = 26'h10f9616;
        7'h48: rom_data = 26'h10a9725;
        7'h49: rom_data = 26'h107982f;
        7'h4a: rom_data = 26'h1049935;
        7'h4b: rom_data = 26'h0ff9a38;
        7'h4c: rom_data = 26'h0fb9b37;
        7'h4d: rom_data = 26'h0f89c32;
        7'h4e: rom_data = 26'h0f49d2a;
        7'h4f: rom_data = 26'h0f09e1e;
        7'h50: rom_data = 26'h0ed9f0e;
        7'h51: rom_data = 26'h0e99ffb;
        7'h52: rom_data = 26'h0e7a0e4;
        7'h53: rom_data = 26'h0e4a1ca;
        7'h54: rom_data = 26'h0e1a2ad;
        7'h55: rom_data = 26'h0dda38d;
        7'h56: rom_data = 26'h0daa46a;
        7'h57: rom_data = 26'h0d6a544;
        7'h58: rom_data = 26'h0d4a61a;
        7'h59: rom_data = 26'h0d1a6ee;
        7'h5a: rom_data = 26'h0cfa7be;
        7'h5b: rom_data = 26'h0cca88c;
        7'h5c: rom_data = 26'h0c7a958;
        7'h5d: rom_data = 26'h0c6aa20;
        7'h5e: rom_data = 26'h0c3aae5;
        7'h5f: rom_data = 26'h0c1aba8;
        7'h60: rom_data = 26'h0bfac68;
        7'h61: rom_data = 26'h0baad27;
        7'h62: rom_data = 26'h0b8ade2;
        7'h63: rom_data = 26'h0b6ae9b;
        7'h64: rom_data = 26'h0b4af51;
        7'h65: rom_data = 26'h0b3b004;
        7'h66: rom_data = 26'h0afb0b7;
        7'h67: rom_data = 26'h0adb166;
        7'h68: rom_data = 26'h0abb213;
        7'h69: rom_data = 26'h0a9b2be;
        7'h6a: rom_data = 26'h0a6b367;
        7'h6b: rom_data = 26'h0a5b40d;
        7'h6c: rom_data = 26'h0a1b4b2;
        7'h6d: rom_data = 26'h0a0b554;
        7'h6e: rom_data = 26'h09fb5f4;
        7'h6f: rom_data = 26'h09db692;
        7'h70: rom_data = 26'h09ab72f;
        7'h71: rom_data = 26'h099b7c9;
        7'h72: rom_data = 26'h096b862;
        7'h73: rom_data = 26'h095b8f8;
        7'h74: rom_data = 26'h093b98d;
        7'h75: rom_data = 26'h091ba20;
        7'h76: rom_data = 26'h08fbab1;
        7'h77: rom_data = 26'h08fbb40;
        7'h78: rom_data = 26'h08cbbce;
        7'h79: rom_data = 26'h08abc5a;
        7'h7a: rom_data = 26'h089bce4;
        7'h7b: rom_data = 26'h087bd6d;
        7'h7c: rom_data = 26'h085bdf4;
        7'h7d: rom_data = 26'h084be79;
        7'h7e: rom_data = 26'h082befd;
        default: rom_data = 26'h081bf7f;
            endcase

endmodule

