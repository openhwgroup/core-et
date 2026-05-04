// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_sin_rom_case_c0
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_sin_rom_case_c0.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h1fc000091f65f60;
      6'h1: data_packed = 58'h000000000000000;
      6'h2: data_packed = 58'h0fc000048bd35f0;
      6'h3: data_packed = 58'h0fc000048560540;
      6'h4: data_packed = 58'h000000000000000;
      6'h5: data_packed = 58'h000000000000000;
      6'h6: data_packed = 58'h0fc000000000000;
      6'h7: data_packed = 58'h0fc000000000000;
      6'h8: data_packed = 58'h03c000011f17084;
      6'h9: data_packed = 58'h03c000011db9270;
      6'ha: data_packed = 58'h03c000011c328dc;
      6'hb: data_packed = 58'h03c000011a835a8;
      6'hc: data_packed = 58'h03c0000118abb44;
      6'hd: data_packed = 58'h03c0000116abca0;
      6'he: data_packed = 58'h03c000011483c9c;
      6'hf: data_packed = 58'h03c000011233e60;
      6'h10: data_packed = 58'h01c0000087de2b6;
      6'h11: data_packed = 58'h01c00000868eaa6;
      6'h12: data_packed = 58'h01c00000852b8fa;
      6'h13: data_packed = 58'h01c0000083b4fa2;
      6'h14: data_packed = 58'h01c00000822b104;
      6'h15: data_packed = 58'h01c00000808df2c;
      6'h16: data_packed = 58'h01c000007eddc7c;
      6'h17: data_packed = 58'h01c000007d1ab90;
      6'h18: data_packed = 58'h01c000007b44ef2;
      6'h19: data_packed = 58'h01c00000795c960;
      6'h1a: data_packed = 58'h01c000007761d94;
      6'h1b: data_packed = 58'h01c000007554e7c;
      6'h1c: data_packed = 58'h01c000007335f08;
      6'h1d: data_packed = 58'h01c000007105288;
      6'h1e: data_packed = 58'h01c000006ec2c04;
      6'h1f: data_packed = 58'h01c000006c6eed2;
      6'h20: data_packed = 58'h00c000003504f38;
      6'h21: data_packed = 58'h00c0000033c9f22;
      6'h22: data_packed = 58'h00c000003286908;
      6'h23: data_packed = 58'h00c00000313aea5;
      6'h24: data_packed = 58'h00c000002fe71f7;
      6'h25: data_packed = 58'h00c000002e8b4e1;
      6'h26: data_packed = 58'h00c000002d27965;
      6'h27: data_packed = 58'h00c000002bbc17c;
      6'h28: data_packed = 58'h00c000002a48f47;
      6'h29: data_packed = 58'h00c0000028ce4d1;
      6'h2a: data_packed = 58'h00c00000274c44c;
      6'h2b: data_packed = 58'h00c0000025c2fe2;
      6'h2c: data_packed = 58'h00c0000024329d1;
      6'h2d: data_packed = 58'h00c00000229b452;
      6'h2e: data_packed = 58'h00c0000020fd1ba;
      6'h2f: data_packed = 58'h00c000001f58461;
      6'h30: data_packed = 58'h00c000001dace9e;
      6'h31: data_packed = 58'h00c000001bfb2da;
      6'h32: data_packed = 58'h00c000001a4338a;
      6'h33: data_packed = 58'h00c000001885320;
      6'h34: data_packed = 58'h00c0000016c141f;
      6'h35: data_packed = 58'h00c0000014f790c;
      6'h36: data_packed = 58'h00c000001328475;
      6'h37: data_packed = 58'h00c0000011538f1;
      6'h38: data_packed = 58'h00c000000f7991b;
      6'h39: data_packed = 58'h00c000000d9a795;
      6'h3a: data_packed = 58'h00c000000bb670d;
      6'h3b: data_packed = 58'h00c0000009cda2b;
      6'h3c: data_packed = 58'h00c0000007e03a7;
      6'h3d: data_packed = 58'h00c0000005ee63a;
      6'h3e: data_packed = 58'h00c0000003f84a1;
      6'h3f: data_packed = 58'h00c0000001fe1a2;
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
