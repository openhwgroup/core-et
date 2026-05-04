// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_exp_rom_case_c0
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_exp_rom_case_c0.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h00c000000000010;
      6'h1: data_packed = 58'h00c000000593470;
      6'h2: data_packed = 58'h00c000000b361a0;
      6'h3: data_packed = 58'h00c0000010e8a40;
      6'h4: data_packed = 58'h00c0000016ab0e0;
      6'h5: data_packed = 58'h00c000001c7d860;
      6'h6: data_packed = 58'h00c0000022603a0;
      6'h7: data_packed = 58'h00c000002853560;
      6'h8: data_packed = 58'h00c000002e57080;
      6'h9: data_packed = 58'h00c00000346b7e0;
      6'ha: data_packed = 58'h00c000003a90e80;
      6'hb: data_packed = 58'h00c0000040c7720;
      6'hc: data_packed = 58'h00c00000470f4e0;
      6'hd: data_packed = 58'h00c000004d68ac0;
      6'he: data_packed = 58'h00c0000053d3be0;
      6'hf: data_packed = 58'h00c000005a50b40;
      6'h10: data_packed = 58'h00c0000060dfc20;
      6'h11: data_packed = 58'h00c000006781160;
      6'h12: data_packed = 58'h00c000006e34e80;
      6'h13: data_packed = 58'h00c0000074fb680;
      6'h14: data_packed = 58'h00c000007bd4ca0;
      6'h15: data_packed = 58'h00c0000082c1440;
      6'h16: data_packed = 58'h00c0000089c10c0;
      6'h17: data_packed = 58'h00c0000090d4580;
      6'h18: data_packed = 58'h00c0000097fb5c0;
      6'h19: data_packed = 58'h00c000009f36500;
      6'h1a: data_packed = 58'h00c00000a6856c0;
      6'h1b: data_packed = 58'h00c00000ade8e80;
      6'h1c: data_packed = 58'h00c00000b560fc0;
      6'h1d: data_packed = 58'h00c00000bcede20;
      6'h1e: data_packed = 58'h00c00000c48fd60;
      6'h1f: data_packed = 58'h00c00000cc47100;
      6'h20: data_packed = 58'h00c00000d413ce0;
      6'h21: data_packed = 58'h00c00000dbf6480;
      6'h22: data_packed = 58'h00c00000e3eebe0;
      6'h23: data_packed = 58'h00c00000ebfd6c0;
      6'h24: data_packed = 58'h00c00000f4228e0;
      6'h25: data_packed = 58'h00c00000fc5e680;
      6'h26: data_packed = 58'h00c0000104b1340;
      6'h27: data_packed = 58'h00c000010d1b340;
      6'h28: data_packed = 58'h00c00001159ca80;
      6'h29: data_packed = 58'h00c000011e35d20;
      6'h2a: data_packed = 58'h00c0000126e6f60;
      6'h2b: data_packed = 58'h00c000012fb0540;
      6'h2c: data_packed = 58'h00c000013892300;
      6'h2d: data_packed = 58'h00c00001418cd00;
      6'h2e: data_packed = 58'h00c000014aa0760;
      6'h2f: data_packed = 58'h00c0000153cd6c0;
      6'h30: data_packed = 58'h00c000015d13f40;
      6'h31: data_packed = 58'h00c000016674580;
      6'h32: data_packed = 58'h00c000016feede0;
      6'h33: data_packed = 58'h00c000017983d20;
      6'h34: data_packed = 58'h00c0000183337c0;
      6'h35: data_packed = 58'h00c000018cfe260;
      6'h36: data_packed = 58'h00c0000196e41c0;
      6'h37: data_packed = 58'h00c00001a0e5aa0;
      6'h38: data_packed = 58'h00c00001ab031c0;
      6'h39: data_packed = 58'h00c00001b53cc00;
      6'h3a: data_packed = 58'h00c00001bf92e60;
      6'h3b: data_packed = 58'h00c00001ca05de0;
      6'h3c: data_packed = 58'h00c00001d495f40;
      6'h3d: data_packed = 58'h00c00001df437e0;
      6'h3e: data_packed = 58'h00c00001ea0ecc0;
      6'h3f: data_packed = 58'h00c00001f4f8300;
      default: data_packed = 58'h0;
    endcase

    data_o = '0;
    data_o.exc    = data_packed[57];
    data_o.mask   = data_packed[56:50];
    data_o.taylor = data_packed[49];
    data_o.c2     = data_packed[48:32];
    data_o.c0     = data_packed[31:0];
  end

endmodule
