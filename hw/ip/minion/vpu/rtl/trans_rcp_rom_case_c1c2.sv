// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_rcp_rom_case_c1c2
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_rcp_rom_case_c1c2.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[6:0])
      7'h0: data_packed = 58'h001fa80e0004000;
      7'h1: data_packed = 58'h001ee80e07ec000;
      7'h2: data_packed = 58'h001e300e0fa6000;
      7'h3: data_packed = 58'h001d880e1732000;
      7'h4: data_packed = 58'h001cd00e1e94000;
      7'h5: data_packed = 58'h001c300e25ca000;
      7'h6: data_packed = 58'h001b980e2cd6000;
      7'h7: data_packed = 58'h001af80e33bc000;
      7'h8: data_packed = 58'h001a600e3a7a000;
      7'h9: data_packed = 58'h0019d80e4112000;
      7'ha: data_packed = 58'h0019380e4788000;
      7'hb: data_packed = 58'h0018b80e4dd8000;
      7'hc: data_packed = 58'h0018300e5406000;
      7'hd: data_packed = 58'h0017b00e5a12000;
      7'he: data_packed = 58'h0017300e5ffe000;
      7'hf: data_packed = 58'h0016b80e65ca000;
      7'h10: data_packed = 58'h0016380e6b78000;
      7'h11: data_packed = 58'h0015c00e7108000;
      7'h12: data_packed = 58'h0015580e767a000;
      7'h13: data_packed = 58'h0014e80e7bd0000;
      7'h14: data_packed = 58'h0014880e8108000;
      7'h15: data_packed = 58'h0014200e8628000;
      7'h16: data_packed = 58'h0013b80e8b2e000;
      7'h17: data_packed = 58'h0013500e901a000;
      7'h18: data_packed = 58'h0012f80e94ec000;
      7'h19: data_packed = 58'h0012900e99a8000;
      7'h1a: data_packed = 58'h0012300e9e4c000;
      7'h1b: data_packed = 58'h0011e00ea2d8000;
      7'h1c: data_packed = 58'h0011780ea750000;
      7'h1d: data_packed = 58'h0011280eabb0000;
      7'h1e: data_packed = 58'h0010d80eaffa000;
      7'h1f: data_packed = 58'h0010880eb432000;
      7'h20: data_packed = 58'h0010380eb854000;
      7'h21: data_packed = 58'h000ff80ebc62000;
      7'h22: data_packed = 58'h000fa80ec05e000;
      7'h23: data_packed = 58'h000f600ec446000;
      7'h24: data_packed = 58'h000f100ec81e000;
      7'h25: data_packed = 58'h000ed00ecbe2000;
      7'h26: data_packed = 58'h000e880ecf96000;
      7'h27: data_packed = 58'h000e480ed338000;
      7'h28: data_packed = 58'h000e080ed6ca000;
      7'h29: data_packed = 58'h000dc80eda4c000;
      7'h2a: data_packed = 58'h000d880eddbe000;
      7'h2b: data_packed = 58'h000d500ee120000;
      7'h2c: data_packed = 58'h000d180ee474000;
      7'h2d: data_packed = 58'h000cd80ee7b8000;
      7'h2e: data_packed = 58'h000ca80eeaee000;
      7'h2f: data_packed = 58'h000c680eee18000;
      7'h30: data_packed = 58'h000c380ef132000;
      7'h31: data_packed = 58'h000bf80ef440000;
      7'h32: data_packed = 58'h000bd80ef73e000;
      7'h33: data_packed = 58'h000b980efa32000;
      7'h34: data_packed = 58'h000b700efd18000;
      7'h35: data_packed = 58'h000b380efff4000;
      7'h36: data_packed = 58'h000b080f02c2000;
      7'h37: data_packed = 58'h000ae00f0584000;
      7'h38: data_packed = 58'h000ab00f083a000;
      7'h39: data_packed = 58'h000a800f0ae8000;
      7'h3a: data_packed = 58'h000a580f0d88000;
      7'h3b: data_packed = 58'h000a300f101e000;
      7'h3c: data_packed = 58'h000a100f12a8000;
      7'h3d: data_packed = 58'h0009e00f152a000;
      7'h3e: data_packed = 58'h0009b80f17a2000;
      7'h3f: data_packed = 58'h0009900f1a0e000;
      7'h40: data_packed = 58'h0009680f1c72000;
      7'h41: data_packed = 58'h0009480f1ecc000;
      7'h42: data_packed = 58'h0009200f211e000;
      7'h43: data_packed = 58'h0009000f2364000;
      7'h44: data_packed = 58'h0008d80f25a4000;
      7'h45: data_packed = 58'h0008b80f27da000;
      7'h46: data_packed = 58'h0008900f2a08000;
      7'h47: data_packed = 58'h0008780f2c2c000;
      7'h48: data_packed = 58'h0008500f2e4a000;
      7'h49: data_packed = 58'h0008380f305e000;
      7'h4a: data_packed = 58'h0008200f326a000;
      7'h4b: data_packed = 58'h0007f80f3470000;
      7'h4c: data_packed = 58'h0007d80f366e000;
      7'h4d: data_packed = 58'h0007c00f3864000;
      7'h4e: data_packed = 58'h0007a00f3a54000;
      7'h4f: data_packed = 58'h0007800f3c3c000;
      7'h50: data_packed = 58'h0007680f3e1c000;
      7'h51: data_packed = 58'h0007480f3ff6000;
      7'h52: data_packed = 58'h0007380f41c8000;
      7'h53: data_packed = 58'h0007200f4394000;
      7'h54: data_packed = 58'h0007080f455a000;
      7'h55: data_packed = 58'h0006e80f471a000;
      7'h56: data_packed = 58'h0006d00f48d4000;
      7'h57: data_packed = 58'h0006b00f4a88000;
      7'h58: data_packed = 58'h0006a00f4c34000;
      7'h59: data_packed = 58'h0006880f4ddc000;
      7'h5a: data_packed = 58'h0006780f4f7c000;
      7'h5b: data_packed = 58'h0006600f5118000;
      7'h5c: data_packed = 58'h0006380f52b0000;
      7'h5d: data_packed = 58'h0006300f5440000;
      7'h5e: data_packed = 58'h0006180f55ca000;
      7'h5f: data_packed = 58'h0006080f5750000;
      7'h60: data_packed = 58'h0005f80f58d0000;
      7'h61: data_packed = 58'h0005d00f5a4e000;
      7'h62: data_packed = 58'h0005c00f5bc4000;
      7'h63: data_packed = 58'h0005b00f5d36000;
      7'h64: data_packed = 58'h0005a00f5ea2000;
      7'h65: data_packed = 58'h0005980f6008000;
      7'h66: data_packed = 58'h0005780f616e000;
      7'h67: data_packed = 58'h0005680f62cc000;
      7'h68: data_packed = 58'h0005580f6426000;
      7'h69: data_packed = 58'h0005480f657c000;
      7'h6a: data_packed = 58'h0005300f66ce000;
      7'h6b: data_packed = 58'h0005280f681a000;
      7'h6c: data_packed = 58'h0005080f6964000;
      7'h6d: data_packed = 58'h0005000f6aa8000;
      7'h6e: data_packed = 58'h0004f80f6be8000;
      7'h6f: data_packed = 58'h0004e80f6d24000;
      7'h70: data_packed = 58'h0004d00f6e5e000;
      7'h71: data_packed = 58'h0004c80f6f92000;
      7'h72: data_packed = 58'h0004b00f70c4000;
      7'h73: data_packed = 58'h0004a80f71f0000;
      7'h74: data_packed = 58'h0004980f731a000;
      7'h75: data_packed = 58'h0004880f7440000;
      7'h76: data_packed = 58'h0004780f7562000;
      7'h77: data_packed = 58'h0004780f7680000;
      7'h78: data_packed = 58'h0004600f779c000;
      7'h79: data_packed = 58'h0004500f78b4000;
      7'h7a: data_packed = 58'h0004480f79c8000;
      7'h7b: data_packed = 58'h0004380f7ada000;
      7'h7c: data_packed = 58'h0004280f7be8000;
      7'h7d: data_packed = 58'h0004200f7cf2000;
      7'h7e: data_packed = 58'h0004100f7dfa000;
      7'h7f: data_packed = 58'h0004080f7efe000;
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
