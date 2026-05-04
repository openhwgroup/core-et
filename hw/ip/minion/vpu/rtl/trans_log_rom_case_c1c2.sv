// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_log_rom_case_c1c2
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_log_rom_case_c1c2.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h005e400d1d64000;
      6'h1: data_packed = 58'h005d300d2c84000;
      6'h2: data_packed = 58'h005c400d3b1c000;
      6'h3: data_packed = 58'h005b500d4938000;
      6'h4: data_packed = 58'h005a700d56e0000;
      6'h5: data_packed = 58'h0059980d6414000;
      6'h6: data_packed = 58'h0058c80d70d8000;
      6'h7: data_packed = 58'h0057f80d7d3c000;
      6'h8: data_packed = 58'h0057380d8938000;
      6'h9: data_packed = 58'h0056800d94d4000;
      6'ha: data_packed = 58'h0055d00da018000;
      6'hb: data_packed = 58'h0055300dab00000;
      6'hc: data_packed = 58'h0054980db598000;
      6'hd: data_packed = 58'h0053f80dbfe0000;
      6'he: data_packed = 58'h0053700dc9d8000;
      6'hf: data_packed = 58'h0052d80dd390000;
      6'h10: data_packed = 58'h0052500ddcfc000;
      6'h11: data_packed = 58'h0051c80de624000;
      6'h12: data_packed = 58'h0051580def08000;
      6'h13: data_packed = 58'h0050e00df7b0000;
      6'h14: data_packed = 58'h0050700e001c000;
      6'h15: data_packed = 58'h004ff80e0850000;
      6'h16: data_packed = 58'h004f900e104c000;
      6'h17: data_packed = 58'h004f200e1814000;
      6'h18: data_packed = 58'h004ec00e1fa4000;
      6'h19: data_packed = 58'h004e680e2704000;
      6'h1a: data_packed = 58'h004e000e2e38000;
      6'h1b: data_packed = 58'h004db00e3538000;
      6'h1c: data_packed = 58'h004d580e3c0c000;
      6'h1d: data_packed = 58'h004d000e42b8000;
      6'h1e: data_packed = 58'h004cb00e4938000;
      6'h1f: data_packed = 58'h004c600e4f94000;
      6'h20: data_packed = 58'h004e780de47c000;
      6'h21: data_packed = 58'h004e300debb4000;
      6'h22: data_packed = 58'h004dd00df2c8000;
      6'h23: data_packed = 58'h004d780df9b4000;
      6'h24: data_packed = 58'h004d300e0074000;
      6'h25: data_packed = 58'h004cf80e070c000;
      6'h26: data_packed = 58'h004cb00e0d84000;
      6'h27: data_packed = 58'h004c680e13d8000;
      6'h28: data_packed = 58'h004c200e1a08000;
      6'h29: data_packed = 58'h004bd80e2018000;
      6'h2a: data_packed = 58'h004b980e2608000;
      6'h2b: data_packed = 58'h004b680e2bd4000;
      6'h2c: data_packed = 58'h004b280e3184000;
      6'h2d: data_packed = 58'h004ae80e3718000;
      6'h2e: data_packed = 58'h004aa80e3c90000;
      6'h2f: data_packed = 58'h004a800e41e4000;
      6'h30: data_packed = 58'h004a480e4724000;
      6'h31: data_packed = 58'h004a100e4c48000;
      6'h32: data_packed = 58'h0049e00e5150000;
      6'h33: data_packed = 58'h0049b80e5640000;
      6'h34: data_packed = 58'h0049800e5b1c000;
      6'h35: data_packed = 58'h0049500e5fdc000;
      6'h36: data_packed = 58'h0049280e6484000;
      6'h37: data_packed = 58'h0048f80e6918000;
      6'h38: data_packed = 58'h0048d80e6d94000;
      6'h39: data_packed = 58'h0048a80e71fc000;
      6'h3a: data_packed = 58'h0048800e7650000;
      6'h3b: data_packed = 58'h0048600e7a8c000;
      6'h3c: data_packed = 58'h0048300e7ebc000;
      6'h3d: data_packed = 58'h0048080e82d4000;
      6'h3e: data_packed = 58'h0047e80e86d8000;
      6'h3f: data_packed = 58'h0047c00e8acc000;
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
