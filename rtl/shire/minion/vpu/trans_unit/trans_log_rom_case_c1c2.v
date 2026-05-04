// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module trans_log_rom_case_c1c2
(
    input trans_rom_index           index,
    output trans_fma_coefficients   data
    );

    logic [25:0] rom_data;

    always_comb
    begin
        data = '0;
        data.c2 = { rom_data[`TRANS_LOG_C2_RANGE] , 7'b0};
        data.c0 = { 2'h3, rom_data[`TRANS_LOG_C1_RANGE], 6'b0, `TRANS_C1_FILLER };
        data.mask = 'b1;
    end

    always @*
            case(`TRANS_ROM_GET_INDEX(`TRANS_LOG_ADDR_SIZE, index))
                6'h20:  rom_data = 26'h1cf791f;
                6'h21:  rom_data = 26'h1c67aed;
                6'h22:  rom_data = 26'h1ba7cb2;
                6'h23:  rom_data = 26'h1af7e6d;
                6'h24:  rom_data = 26'h1a6801d;
                6'h25:  rom_data = 26'h19f81c3;
                6'h26:  rom_data = 26'h1968361;
                6'h27:  rom_data = 26'h18d84f6;
                6'h28:  rom_data = 26'h1848682;
                6'h29:  rom_data = 26'h17b8806;
                6'h2a:  rom_data = 26'h1738982;
                6'h2b:  rom_data = 26'h16d8af5;
                6'h2c:  rom_data = 26'h1658c61;
                6'h2d:  rom_data = 26'h15d8dc6;
                6'h2e:  rom_data = 26'h1558f24;
                6'h2f:  rom_data = 26'h1509079;
                6'h30: rom_data = 26'h14991c9;
                6'h31: rom_data = 26'h1429312;
                6'h32: rom_data = 26'h13c9454;
                6'h33: rom_data = 26'h1379590;
                6'h34: rom_data = 26'h13096c7;
                6'h35: rom_data = 26'h12a97f7;
                6'h36: rom_data = 26'h1259921;
                6'h37: rom_data = 26'h11f9a46;
                6'h38: rom_data = 26'h11b9b65;
                6'h39: rom_data = 26'h1159c7f;
                6'h3a: rom_data = 26'h1109d94;
                6'h3b: rom_data = 26'h10c9ea3;
                6'h3c: rom_data = 26'h1069faf;
                6'h3d: rom_data = 26'h101a0b5;
                6'h3e: rom_data = 26'h0fda1b6;
                6'h3f: rom_data = 26'h0f8a2b3;
                6'h00: rom_data = 26'h3c84759;
                6'h01: rom_data = 26'h3a64b21;
                6'h02: rom_data = 26'h3884ec7;
                6'h03: rom_data = 26'h36a524e;
                6'h04: rom_data = 26'h34e55b8;
                6'h05: rom_data = 26'h3335905;
                6'h06: rom_data = 26'h3195c36;
                6'h07: rom_data = 26'h2ff5f4f;
                6'h08: rom_data = 26'h2e7624e;
                6'h09: rom_data = 26'h2d06535;
                6'h0a: rom_data = 26'h2ba6806;
                6'h0b: rom_data = 26'h2a66ac0;
                6'h0c: rom_data = 26'h2936d66;
                6'h0d: rom_data = 26'h27f6ff8;
                6'h0e: rom_data = 26'h26e7276;
                6'h0f: rom_data = 26'h25b74e4;
                6'h10: rom_data = 26'h24a773f;
                6'h11: rom_data = 26'h2397989;
                6'h12: rom_data = 26'h22b7bc2;
                6'h13: rom_data = 26'h21c7dec;
                6'h14: rom_data = 26'h20e8007;
                6'h15: rom_data = 26'h1ff8214;
                6'h16: rom_data = 26'h1f28413;
                6'h17: rom_data = 26'h1e48605;
                6'h18: rom_data = 26'h1d887e9;
                6'h19: rom_data = 26'h1cd89c1;
                6'h1a: rom_data = 26'h1c08b8e;
                6'h1b: rom_data = 26'h1b68d4e;
                6'h1c: rom_data = 26'h1ab8f03;
                6'h1d: rom_data = 26'h1a090ae;
                6'h1e: rom_data = 26'h196924e;
                default: rom_data = 26'h18c93e5;
            endcase

endmodule

