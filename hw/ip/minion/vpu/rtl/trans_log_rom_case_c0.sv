// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_log_rom_case_c0
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_log_rom_case_c0.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h0040000038aa3ac;
      6'h1: data_packed = 58'h00400000373cb3c;
      6'h2: data_packed = 58'h0040000035d69ba;
      6'h3: data_packed = 58'h004000003477b1a;
      6'h4: data_packed = 58'h00400000331fb74;
      6'h5: data_packed = 58'h0040000031ce746;
      6'h6: data_packed = 58'h004000003083b10;
      6'h7: data_packed = 58'h004000002f3f352;
      6'h8: data_packed = 58'h004000002e00d18;
      6'h9: data_packed = 58'h004000002cc853c;
      6'ha: data_packed = 58'h004000002b958d8;
      6'hb: data_packed = 58'h004000002a68534;
      6'hc: data_packed = 58'h00400000294078e;
      6'hd: data_packed = 58'h00400000281dd78;
      6'he: data_packed = 58'h00400000270046c;
      6'hf: data_packed = 58'h0040000025e79fe;
      6'h10: data_packed = 58'h0040000024d3c1c;
      6'h11: data_packed = 58'h0040000023c488c;
      6'h12: data_packed = 58'h0040000022b9d3c;
      6'h13: data_packed = 58'h0040000021b382e;
      6'h14: data_packed = 58'h0040000020b176e;
      6'h15: data_packed = 58'h004000001fb392c;
      6'h16: data_packed = 58'h004000001eb9ba0;
      6'h17: data_packed = 58'h004000001dc3d16;
      6'h18: data_packed = 58'h004000001cd1bf8;
      6'h19: data_packed = 58'h004000001be3698;
      6'h1a: data_packed = 58'h004000001af8b7e;
      6'h1b: data_packed = 58'h004000001a1192c;
      6'h1c: data_packed = 58'h00400000192de46;
      6'h1d: data_packed = 58'h00400000184d966;
      6'h1e: data_packed = 58'h00400000177092a;
      6'h1f: data_packed = 58'h004000001696c42;
      6'h20: data_packed = 58'h00400000547fcb8;
      6'h21: data_packed = 58'h004000005373d84;
      6'h22: data_packed = 58'h00400000526b780;
      6'h23: data_packed = 58'h00400000516695c;
      6'h24: data_packed = 58'h0040000050651f2;
      6'h25: data_packed = 58'h004000004f66ffe;
      6'h26: data_packed = 58'h004000004e6c240;
      6'h27: data_packed = 58'h004000004d747b6;
      6'h28: data_packed = 58'h004000004c7ff4a;
      6'h29: data_packed = 58'h004000004b8e7cc;
      6'h2a: data_packed = 58'h004000004aa0034;
      6'h2b: data_packed = 58'h0040000049b47a4;
      6'h2c: data_packed = 58'h0040000048cbd12;
      6'h2d: data_packed = 58'h0040000047e5f82;
      6'h2e: data_packed = 58'h004000004702e0e;
      6'h2f: data_packed = 58'h0040000046227e2;
      6'h30: data_packed = 58'h004000004544c0a;
      6'h31: data_packed = 58'h0040000044699b8;
      6'h32: data_packed = 58'h00400000439101e;
      6'h33: data_packed = 58'h0040000042bae6c;
      6'h34: data_packed = 58'h0040000041e73d0;
      6'h35: data_packed = 58'h004000004115fa8;
      6'h36: data_packed = 58'h00400000404712c;
      6'h37: data_packed = 58'h004000003f7a796;
      6'h38: data_packed = 58'h004000003eb0246;
      6'h39: data_packed = 58'h004000003de8098;
      6'h3a: data_packed = 58'h004000003d221c0;
      6'h3b: data_packed = 58'h004000003c5e544;
      6'h3c: data_packed = 58'h004000003b9ca60;
      6'h3d: data_packed = 58'h004000003add098;
      6'h3e: data_packed = 58'h004000003a1f746;
      6'h3f: data_packed = 58'h004000003963dd0;
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
