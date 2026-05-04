// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_exp_rom_case_c0 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [25:0] rom_data;

  always_comb begin
    data      = '0;
    data.c0   = {3'b0, rom_data, 3'b0};
    data.mask = 'b11;
  end


  always @*
    case (`TRANS_ROM_GET_INDEX(`TRANS_EXP_ADDR_SIZE, index))
      6'h0:    rom_data = 26'h0000002;
      6'h1:    rom_data = 26'h00b268e;
      6'h2:    rom_data = 26'h0166c34;
      6'h3:    rom_data = 26'h021d148;
      6'h4:    rom_data = 26'h02d561c;
      6'h5:    rom_data = 26'h038fb0c;
      6'h6:    rom_data = 26'h044c074;
      6'h7:    rom_data = 26'h050a6ac;
      6'h8:    rom_data = 26'h05cae10;
      6'h9:    rom_data = 26'h068d6fc;
      6'ha:    rom_data = 26'h07521d0;
      6'hb:    rom_data = 26'h0818ee4;
      6'hc:    rom_data = 26'h08e1e9c;
      6'hd:    rom_data = 26'h09ad158;
      6'he:    rom_data = 26'h0a7a77c;
      6'hf:    rom_data = 26'h0b4a168;
      6'h10:   rom_data = 26'h0c1bf84;
      6'h11:   rom_data = 26'h0cf022c;
      6'h12:   rom_data = 26'h0dc69d0;
      6'h13:   rom_data = 26'h0e9f6d0;
      6'h14:   rom_data = 26'h0f7a994;
      6'h15:   rom_data = 26'h1058288;
      6'h16:   rom_data = 26'h1138218;
      6'h17:   rom_data = 26'h121a8b0;
      6'h18:   rom_data = 26'h12ff6b8;
      6'h19:   rom_data = 26'h13e6ca0;
      6'h1a:   rom_data = 26'h14d0ad8;
      6'h1b:   rom_data = 26'h15bd1d0;
      6'h1c:   rom_data = 26'h16ac1f8;
      6'h1d:   rom_data = 26'h179dbc4;
      6'h1e:   rom_data = 26'h1891fac;
      6'h1f:   rom_data = 26'h1988e20;
      6'h20:   rom_data = 26'h1a8279c;
      6'h21:   rom_data = 26'h1b7ec90;
      6'h22:   rom_data = 26'h1c7dd7c;
      6'h23:   rom_data = 26'h1d7fad8;
      6'h24:   rom_data = 26'h1e8451c;
      6'h25:   rom_data = 26'h1f8bcd0;
      6'h26:   rom_data = 26'h2096268;
      6'h27:   rom_data = 26'h21a3668;
      6'h28:   rom_data = 26'h22b3950;
      6'h29:   rom_data = 26'h23c6ba4;
      6'h2a:   rom_data = 26'h24dcdec;
      6'h2b:   rom_data = 26'h25f60a8;
      6'h2c:   rom_data = 26'h2712460;
      6'h2d:   rom_data = 26'h28319a0;
      6'h2e:   rom_data = 26'h29540ec;
      6'h2f:   rom_data = 26'h2a79ad8;
      6'h30:   rom_data = 26'h2ba27e8;
      6'h31:   rom_data = 26'h2cce8b0;
      6'h32:   rom_data = 26'h2dfddbc;
      6'h33:   rom_data = 26'h2f307a4;
      6'h34:   rom_data = 26'h30666f8;
      6'h35:   rom_data = 26'h319fc4c;
      6'h36:   rom_data = 26'h32dc838;
      6'h37:   rom_data = 26'h341cb54;
      6'h38:   rom_data = 26'h3560638;
      6'h39:   rom_data = 26'h36a7980;
      6'h3a:   rom_data = 26'h37f25cc;
      6'h3b:   rom_data = 26'h3940bbc;
      6'h3c:   rom_data = 26'h3a92be8;
      6'h3d:   rom_data = 26'h3be86fc;
      6'h3e:   rom_data = 26'h3d41d98;
      default: rom_data = 26'h3e9f060;
    endcase

endmodule
