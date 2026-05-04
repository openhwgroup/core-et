// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_sin_rom_case_c1c2 (
  input  trans_rom_index         index,
  output trans_fma_coefficients  data
);

  logic [34:0] rom_data;
  logic [`TRANS_SIN_ADDR_SIZE-1:0] addr;

  assign addr = `TRANS_ROM_GET_INDEX(`TRANS_SIN_ADDR_SIZE, index);

  always_comb begin

    data = '0;


    if (addr[5] == 1'b1) begin
      data.c2   = {rom_data[`TRANS_SIN_C2_RANGE], 2'b0};
      data.c0   = {4'b0, rom_data[`TRANS_SIN_C1_RANGE], `TRANS_C1_FILLER};
      data.mask = 'b11;
    end else if (addr[4] == 1'b1) begin
      data.c2   = {1'b0, rom_data[`TRANS_SIN_C2_RANGE], 1'b0};
      data.c0   = {4'b0, rom_data[`TRANS_SIN_C1_RANGE], `TRANS_C1_FILLER};
      data.mask = 'b111;
    end else if (addr[3] == 1'b1) begin
      data.c2   = {2'b0, rom_data[`TRANS_SIN_C2_RANGE]};
      data.c0   = {4'b0, rom_data[`TRANS_SIN_C1_RANGE], `TRANS_C1_FILLER};
      data.mask = 'b1111;
    end else if (addr[1] == 1'b1) begin
      data.c2   = {1'b0, rom_data[`TRANS_SIN_C2_RANGE], 1'b0};
      data.c0   = {1'b0, rom_data[`TRANS_SIN_C1_RANGE], 3'b0, `TRANS_C1_FILLER};
      data.mask = 'b111111;
    end else if (addr == 0) begin
      data.c2   = {2'b0, rom_data[`TRANS_SIN_C2_RANGE]};
      data.c0   = {1'b0, rom_data[`TRANS_SIN_C1_RANGE], 3'b0, `TRANS_C1_FILLER};
      data.mask = 'b1111111;
    end

  end


  always @* 
    case (addr)
      6'h0:    rom_data = 35'h528c14ab8;
      6'h2:    rom_data = 35'h524f294f2;
      6'h3:    rom_data = 35'h51f33de36;
      6'h8:    rom_data = 35'h1467292f2;
      6'h9:    rom_data = 35'h14552e48e;
      6'ha:    rom_data = 35'h1443335e5;
      6'hb:    rom_data = 35'h142e386f0;
      6'hc:    rom_data = 35'h14173d7a8;
      6'hd:    rom_data = 35'h13ff42805;
      6'he:    rom_data = 35'h13e447800;
      6'hf:    rom_data = 35'h13c84c791;
      6'h10:   rom_data = 35'h13aa516b1;
      6'h11:   rom_data = 35'h138a5655a;
      6'h12:   rom_data = 35'h13685b382;
      6'h13:   rom_data = 35'h134560123;
      6'h14:   rom_data = 35'h132064e39;
      6'h15:   rom_data = 35'h12f969ab9;
      6'h16:   rom_data = 35'h12d06e69d;
      6'h17:   rom_data = 35'h12a6731df;
      6'h18:   rom_data = 35'h127a77c77;
      6'h19:   rom_data = 35'h124c7c65f;
      6'h1a:   rom_data = 35'h121d80f91;
      6'h1b:   rom_data = 35'h11ec85804;
      6'h1c:   rom_data = 35'h11b989fb5;
      6'h1d:   rom_data = 35'h11858e69a;
      6'h1e:   rom_data = 35'h114f92caf;
      6'h1f:   rom_data = 35'h1118971ed;
      6'h20:   rom_data = 35'h10df9b64d;
      6'h21:   rom_data = 35'h10a59f9ca;
      6'h22:   rom_data = 35'h1069a3c5f;
      6'h23:   rom_data = 35'h102ba7e04;
      6'h24:   rom_data = 35'h0fedabeb4;
      6'h25:   rom_data = 35'h0fadafe69;
      6'h26:   rom_data = 35'h0f6cb3d1f;
      6'h27:   rom_data = 35'h0f29b7acd;
      6'h28:   rom_data = 35'h0ee5bb774;
      6'h29:   rom_data = 35'h0ea0bf306;
      6'h2a:   rom_data = 35'h0e58c2d88;
      6'h2b:   rom_data = 35'h0e10c66ed;
      6'h2c:   rom_data = 35'h0dc8c9f32;
      6'h2d:   rom_data = 35'h0d7ecd653;
      6'h2e:   rom_data = 35'h0d33d0c4b;
      6'h2f:   rom_data = 35'h0ce6d4119;
      6'h30:   rom_data = 35'h0c99d74b3;
      6'h31:   rom_data = 35'h0c4bda716;
      6'h32:   rom_data = 35'h0bfbdd843;
      6'h33:   rom_data = 35'h0baae0831;
      6'h34:   rom_data = 35'h0b59e36dc;
      6'h35:   rom_data = 35'h0b07e6442;
      6'h36:   rom_data = 35'h0ab4e905e;
      6'h37:   rom_data = 35'h0a60ebb2f;
      6'h38:   rom_data = 35'h0a0bee4b0;
      6'h39:   rom_data = 35'h09b6f0cdc;
      6'h3a:   rom_data = 35'h095ff33b7;
      6'h3b:   rom_data = 35'h0909f5934;
      6'h3c:   rom_data = 35'h08b1f7d58;
      6'h3d:   rom_data = 35'h0859fa01e;
      6'h3e:   rom_data = 35'h0801fc181;
      6'h3f:   rom_data = 35'h07a8fe183;
      default: rom_data = 35'h0;
    endcase
endmodule
