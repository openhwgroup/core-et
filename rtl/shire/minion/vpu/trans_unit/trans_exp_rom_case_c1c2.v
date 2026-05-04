// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_exp_rom_case_c1c2 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [28:0] rom_data;

  always_comb begin
    data      = '0;
    data.c2   = {rom_data[`TRANS_EXP_C2_RANGE], 6'b0};
    data.c0   = {2'b0, rom_data[`TRANS_EXP_C1_RANGE], 6'b0, `TRANS_C1_FILLER};
    data.mask = 'b11;
  end

  always @* begin
    case (`TRANS_ROM_GET_INDEX(`TRANS_EXP_ADDR_SIZE, index))
      6'h0:    rom_data = 29'h0f7562e3;
      6'h1:    rom_data = 29'h0fa166c1;
      6'h2:    rom_data = 29'h0fd16aa8;
      6'h3:    rom_data = 29'h0ffd6e9b;
      6'h4:    rom_data = 29'h10297299;
      6'h5:    rom_data = 29'h105176a3;
      6'h6:    rom_data = 29'h10817ab7;
      6'h7:    rom_data = 29'h10ad7ed7;
      6'h8:    rom_data = 29'h10dd8302;
      6'h9:    rom_data = 29'h11098739;
      6'ha:    rom_data = 29'h113d8b7b;
      6'hb:    rom_data = 29'h11698fca;
      6'hc:    rom_data = 29'h119d9424;
      6'hd:    rom_data = 29'h11cd988b;
      6'he:    rom_data = 29'h11fd9cfe;
      6'hf:    rom_data = 29'h1231a17d;
      6'h10:   rom_data = 29'h1261a609;
      6'h11:   rom_data = 29'h1299aaa1;
      6'h12:   rom_data = 29'h12c9af47;
      6'h13:   rom_data = 29'h12fdb3f9;
      6'h14:   rom_data = 29'h1335b8b8;
      6'h15:   rom_data = 29'h1369bd85;
      6'h16:   rom_data = 29'h139dc25f;
      6'h17:   rom_data = 29'h13d9c746;
      6'h18:   rom_data = 29'h1411cc3b;
      6'h19:   rom_data = 29'h1449d13e;
      6'h1a:   rom_data = 29'h1481d64f;
      6'h1b:   rom_data = 29'h14b9db6e;
      6'h1c:   rom_data = 29'h14ede09c;
      6'h1d:   rom_data = 29'h1529e5d8;
      6'h1e:   rom_data = 29'h1565eb22;
      6'h1f:   rom_data = 29'h15a1f07b;
      6'h20:   rom_data = 29'h15ddf5e3;
      6'h21:   rom_data = 29'h1615fb5b;
      6'h22:   rom_data = 29'h165e00e0;
      6'h23:   rom_data = 29'h16960676;
      6'h24:   rom_data = 29'h16da0c1b;
      6'h25:   rom_data = 29'h171a11d0;
      6'h26:   rom_data = 29'h175a1795;
      6'h27:   rom_data = 29'h179a1d6a;
      6'h28:   rom_data = 29'h17d62350;
      6'h29:   rom_data = 29'h181a2946;
      6'h2a:   rom_data = 29'h18662f4b;
      6'h2b:   rom_data = 29'h18a23563;
      6'h2c:   rom_data = 29'h18e63b8b;
      6'h2d:   rom_data = 29'h192e41c4;
      6'h2e:   rom_data = 29'h1972480f;
      6'h2f:   rom_data = 29'h19ba4e6b;
      6'h30:   rom_data = 29'h1a0254d9;
      6'h31:   rom_data = 29'h1a465b59;
      6'h32:   rom_data = 29'h1a8a61ec;
      6'h33:   rom_data = 29'h1ae6688e;
      6'h34:   rom_data = 29'h1b266f46;
      6'h35:   rom_data = 29'h1b76760f;
      6'h36:   rom_data = 29'h1bc67ceb;
      6'h37:   rom_data = 29'h1c0e83db;
      6'h38:   rom_data = 29'h1c5a8ade;
      6'h39:   rom_data = 29'h1cae91f4;
      6'h3a:   rom_data = 29'h1cf6991f;
      6'h3b:   rom_data = 29'h1d4ea05c;
      6'h3c:   rom_data = 29'h1d9ea7af;
      6'h3d:   rom_data = 29'h1df2af15;
      6'h3e:   rom_data = 29'h1e42b691;
      default: rom_data = 29'h1e9abe21;
    endcase
  end

endmodule
