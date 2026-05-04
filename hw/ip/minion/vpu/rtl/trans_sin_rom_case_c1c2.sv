// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_sin_rom_case_c1c2
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_sin_rom_case_c1c2.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h1fc528c0a55c000;
      6'h1: data_packed = 58'h000000000000000;
      6'h2: data_packed = 58'h0fca49e14a79000;
      6'h3: data_packed = 58'h0fca3e61ef1b000;
      6'h4: data_packed = 58'h000000000000000;
      6'h5: data_packed = 58'h000000000000000;
      6'h6: data_packed = 58'h0fc000000000000;
      6'h7: data_packed = 58'h0fc000000000000;
      6'h8: data_packed = 58'h03c14670292f200;
      6'h9: data_packed = 58'h03c145502e48e00;
      6'ha: data_packed = 58'h03c14430335e500;
      6'hb: data_packed = 58'h03c142e0386f000;
      6'hc: data_packed = 58'h03c141703d7a800;
      6'hd: data_packed = 58'h03c13ff04280500;
      6'he: data_packed = 58'h03c13e404780000;
      6'hf: data_packed = 58'h03c13c804c79100;
      6'h10: data_packed = 58'h01c27540516b100;
      6'h11: data_packed = 58'h01c271405655a00;
      6'h12: data_packed = 58'h01c26d005b38200;
      6'h13: data_packed = 58'h01c268a06012300;
      6'h14: data_packed = 58'h01c2640064e3900;
      6'h15: data_packed = 58'h01c25f2069ab900;
      6'h16: data_packed = 58'h01c25a006e69d00;
      6'h17: data_packed = 58'h01c254c0731df00;
      6'h18: data_packed = 58'h01c24f4077c7700;
      6'h19: data_packed = 58'h01c249807c65f00;
      6'h1a: data_packed = 58'h01c243a080f9100;
      6'h1b: data_packed = 58'h01c23d808580400;
      6'h1c: data_packed = 58'h01c2372089fb500;
      6'h1d: data_packed = 58'h01c230a08e69a00;
      6'h1e: data_packed = 58'h01c229e092caf00;
      6'h1f: data_packed = 58'h01c22300971ed00;
      6'h20: data_packed = 58'h00c437c09b64d00;
      6'h21: data_packed = 58'h00c429409f9ca00;
      6'h22: data_packed = 58'h00c41a40a3c5f00;
      6'h23: data_packed = 58'h00c40ac0a7e0400;
      6'h24: data_packed = 58'h00c3fb40abeb400;
      6'h25: data_packed = 58'h00c3eb40afe6900;
      6'h26: data_packed = 58'h00c3db00b3d1f00;
      6'h27: data_packed = 58'h00c3ca40b7acd00;
      6'h28: data_packed = 58'h00c3b940bb77400;
      6'h29: data_packed = 58'h00c3a800bf30600;
      6'h2a: data_packed = 58'h00c39600c2d8800;
      6'h2b: data_packed = 58'h00c38400c66ed00;
      6'h2c: data_packed = 58'h00c37200c9f3200;
      6'h2d: data_packed = 58'h00c35f80cd65300;
      6'h2e: data_packed = 58'h00c34cc0d0c4b00;
      6'h2f: data_packed = 58'h00c33980d411900;
      6'h30: data_packed = 58'h00c32640d74b300;
      6'h31: data_packed = 58'h00c312c0da71600;
      6'h32: data_packed = 58'h00c2fec0dd84300;
      6'h33: data_packed = 58'h00c2ea80e083100;
      6'h34: data_packed = 58'h00c2d640e36dc00;
      6'h35: data_packed = 58'h00c2c1c0e644200;
      6'h36: data_packed = 58'h00c2ad00e905e00;
      6'h37: data_packed = 58'h00c29800ebb2f00;
      6'h38: data_packed = 58'h00c282c0ee4b000;
      6'h39: data_packed = 58'h00c26d80f0cdc00;
      6'h3a: data_packed = 58'h00c257c0f33b700;
      6'h3b: data_packed = 58'h00c24240f593400;
      6'h3c: data_packed = 58'h00c22c40f7d5800;
      6'h3d: data_packed = 58'h00c21640fa01e00;
      6'h3e: data_packed = 58'h00c20040fc18100;
      6'h3f: data_packed = 58'h00c1ea00fe18300;
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
