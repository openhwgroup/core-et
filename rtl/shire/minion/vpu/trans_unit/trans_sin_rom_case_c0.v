// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_sin_rom_case_c0 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [26:0] rom_data;
  logic [`TRANS_SIN_ADDR_SIZE-1:0] addr;

  assign addr = `TRANS_ROM_GET_INDEX(`TRANS_SIN_ADDR_SIZE, index);

  always_comb begin

    data = '0;

    if (addr[5] == 1'b1) begin
      data.c0   = {5'b0, rom_data};
      data.mask = 'b11;
    end else if (addr[4] == 1'b1) begin
      data.c0   = {4'b0, rom_data, 1'b0};
      data.mask = 'b111;
    end else if (addr[3] == 1'b1) begin
      data.c0   = {3'b0, rom_data, 2'b0};
      data.mask = 'b1111;
    end else if (addr[1] == 1'b1) begin
      data.c0   = {1'b0, rom_data, 4'b0};
      data.mask = 'b111111;
    end else if (addr == 0) begin
      data.c0   = {rom_data, 5'b0};
      data.mask = 'b1111111;
    end

  end


  always @* begin
    case (addr)
      6'h0:    rom_data = 27'h48fb2fb;
      6'h2:    rom_data = 27'h48bd35f;
      6'h3:    rom_data = 27'h4856054;
      6'h8:    rom_data = 27'h47c5c21;
      6'h9:    rom_data = 27'h476e49c;
      6'ha:    rom_data = 27'h470ca37;
      6'hb:    rom_data = 27'h46a0d6a;
      6'hc:    rom_data = 27'h462aed1;
      6'hd:    rom_data = 27'h45aaf28;
      6'he:    rom_data = 27'h4520f27;
      6'hf:    rom_data = 27'h448cf98;
      6'h10:   rom_data = 27'h43ef15b;
      6'h11:   rom_data = 27'h4347553;
      6'h12:   rom_data = 27'h4295c7d;
      6'h13:   rom_data = 27'h41da7d1;
      6'h14:   rom_data = 27'h4115882;
      6'h15:   rom_data = 27'h4046f96;
      6'h16:   rom_data = 27'h3f6ee3e;
      6'h17:   rom_data = 27'h3e8d5c8;
      6'h18:   rom_data = 27'h3da2779;
      6'h19:   rom_data = 27'h3cae4b0;
      6'h1a:   rom_data = 27'h3bb0eca;
      6'h1b:   rom_data = 27'h3aaa73e;
      6'h1c:   rom_data = 27'h399af84;
      6'h1d:   rom_data = 27'h3882944;
      6'h1e:   rom_data = 27'h3761602;
      6'h1f:   rom_data = 27'h3637769;
      6'h20:   rom_data = 27'h3504f38;
      6'h21:   rom_data = 27'h33c9f22;
      6'h22:   rom_data = 27'h3286908;
      6'h23:   rom_data = 27'h313aea5;
      6'h24:   rom_data = 27'h2fe71f7;
      6'h25:   rom_data = 27'h2e8b4e1;
      6'h26:   rom_data = 27'h2d27965;
      6'h27:   rom_data = 27'h2bbc17c;
      6'h28:   rom_data = 27'h2a48f47;
      6'h29:   rom_data = 27'h28ce4d1;
      6'h2a:   rom_data = 27'h274c44c;
      6'h2b:   rom_data = 27'h25c2fe2;
      6'h2c:   rom_data = 27'h24329d1;
      6'h2d:   rom_data = 27'h229b452;
      6'h2e:   rom_data = 27'h20fd1ba;
      6'h2f:   rom_data = 27'h1f58461;
      6'h30:   rom_data = 27'h1dace9e;
      6'h31:   rom_data = 27'h1bfb2da;
      6'h32:   rom_data = 27'h1a4338a;
      6'h33:   rom_data = 27'h1885320;
      6'h34:   rom_data = 27'h16c141f;
      6'h35:   rom_data = 27'h14f790c;
      6'h36:   rom_data = 27'h1328475;
      6'h37:   rom_data = 27'h11538f1;
      6'h38:   rom_data = 27'h0f7991b;
      6'h39:   rom_data = 27'h0d9a795;
      6'h3a:   rom_data = 27'h0bb670d;
      6'h3b:   rom_data = 27'h09cda2b;
      6'h3c:   rom_data = 27'h07e03a7;
      6'h3d:   rom_data = 27'h05ee63a;
      6'h3e:   rom_data = 27'h03f84a1;
      6'h3f:   rom_data = 27'h01fe1a2;
      default: rom_data = 27'h0;
    endcase
  end

endmodule
