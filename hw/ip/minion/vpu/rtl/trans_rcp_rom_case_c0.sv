// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_rcp_rom_case_c0
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_rcp_rom_case_c0.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[6:0])
      7'h0: data_packed = 58'h000000003fffffe;
      7'h1: data_packed = 58'h000000003f80fe0;
      7'h2: data_packed = 58'h000000003f03f04;
      7'h3: data_packed = 58'h000000003e88cb3;
      7'h4: data_packed = 58'h000000003e0f83d;
      7'h5: data_packed = 58'h000000003d980f5;
      7'h6: data_packed = 58'h000000003d22636;
      7'h7: data_packed = 58'h000000003cae758;
      7'h8: data_packed = 58'h000000003c3c3c4;
      7'h9: data_packed = 58'h000000003bcbadc;
      7'ha: data_packed = 58'h000000003b5cc0d;
      7'hb: data_packed = 58'h000000003aef6c9;
      7'hc: data_packed = 58'h000000003a83a83;
      7'hd: data_packed = 58'h000000003a196b1;
      7'he: data_packed = 58'h0000000039b0ad0;
      7'hf: data_packed = 58'h000000003949661;
      7'h10: data_packed = 58'h0000000038e38e4;
      7'h11: data_packed = 58'h00000000387f1df;
      7'h12: data_packed = 58'h00000000381c0df;
      7'h13: data_packed = 58'h0000000037ba570;
      7'h14: data_packed = 58'h000000003759f24;
      7'h15: data_packed = 58'h0000000036fad88;
      7'h16: data_packed = 58'h00000000369d035;
      7'h17: data_packed = 58'h0000000036406c7;
      7'h18: data_packed = 58'h0000000035e50d9;
      7'h19: data_packed = 58'h00000000358ae03;
      7'h1a: data_packed = 58'h000000003531dec;
      7'h1b: data_packed = 58'h0000000034da034;
      7'h1c: data_packed = 58'h000000003483482;
      7'h1d: data_packed = 58'h00000000342da7e;
      7'h1e: data_packed = 58'h0000000033d91d4;
      7'h1f: data_packed = 58'h000000003385a28;
      7'h20: data_packed = 58'h000000003333333;
      7'h21: data_packed = 58'h0000000032e1c9d;
      7'h22: data_packed = 58'h00000000329161e;
      7'h23: data_packed = 58'h000000003241f6b;
      7'h24: data_packed = 58'h0000000031f3832;
      7'h25: data_packed = 58'h0000000031a6033;
      7'h26: data_packed = 58'h000000003159721;
      7'h27: data_packed = 58'h00000000310dcbf;
      7'h28: data_packed = 58'h0000000030c30c4;
      7'h29: data_packed = 58'h0000000030792ee;
      7'h2a: data_packed = 58'h000000003030304;
      7'h2b: data_packed = 58'h000000002fe80bf;
      7'h2c: data_packed = 58'h000000002fa0be7;
      7'h2d: data_packed = 58'h000000002f5a443;
      7'h2e: data_packed = 58'h000000002f14991;
      7'h2f: data_packed = 58'h000000002ecfb9c;
      7'h30: data_packed = 58'h000000002e8ba2d;
      7'h31: data_packed = 58'h000000002e4850e;
      7'h32: data_packed = 58'h000000002e05c0a;
      7'h33: data_packed = 58'h000000002dc3eed;
      7'h34: data_packed = 58'h000000002d82d82;
      7'h35: data_packed = 58'h000000002d42798;
      7'h36: data_packed = 58'h000000002d02d03;
      7'h37: data_packed = 58'h000000002cc3d8c;
      7'h38: data_packed = 58'h000000002c8590d;
      7'h39: data_packed = 58'h000000002c47f4e;
      7'h3a: data_packed = 58'h000000002c0b02b;
      7'h3b: data_packed = 58'h000000002bceb77;
      7'h3c: data_packed = 58'h000000002b93107;
      7'h3d: data_packed = 58'h000000002b580af;
      7'h3e: data_packed = 58'h000000002b1da45;
      7'h3f: data_packed = 58'h000000002ae3da9;
      7'h40: data_packed = 58'h000000002aaaaac;
      7'h41: data_packed = 58'h000000002a72129;
      7'h42: data_packed = 58'h000000002a3a0fc;
      7'h43: data_packed = 58'h000000002a02a04;
      7'h44: data_packed = 58'h0000000029cbc14;
      7'h45: data_packed = 58'h000000002995711;
      7'h46: data_packed = 58'h00000000295fad3;
      7'h47: data_packed = 58'h00000000292a73e;
      7'h48: data_packed = 58'h0000000028f5c29;
      7'h49: data_packed = 58'h0000000028c1978;
      7'h4a: data_packed = 58'h00000000288df0e;
      7'h4b: data_packed = 58'h00000000285acc6;
      7'h4c: data_packed = 58'h000000002828284;
      7'h4d: data_packed = 58'h0000000027f6027;
      7'h4e: data_packed = 58'h0000000027c4596;
      7'h4f: data_packed = 58'h0000000027932b4;
      7'h50: data_packed = 58'h000000002762761;
      7'h51: data_packed = 58'h000000002732386;
      7'h52: data_packed = 58'h000000002702702;
      7'h53: data_packed = 58'h0000000026d31c0;
      7'h54: data_packed = 58'h0000000026a43a0;
      7'h55: data_packed = 58'h000000002675c8d;
      7'h56: data_packed = 58'h000000002647c68;
      7'h57: data_packed = 58'h00000000261a31f;
      7'h58: data_packed = 58'h0000000025ed098;
      7'h59: data_packed = 58'h0000000025c04b7;
      7'h5a: data_packed = 58'h000000002593f6b;
      7'h5b: data_packed = 58'h000000002568097;
      7'h5c: data_packed = 58'h00000000253c826;
      7'h5d: data_packed = 58'h000000002511601;
      7'h5e: data_packed = 58'h0000000024e6a19;
      7'h5f: data_packed = 58'h0000000024bc44d;
      7'h60: data_packed = 58'h000000002492494;
      7'h61: data_packed = 58'h000000002468ad0;
      7'h62: data_packed = 58'h00000000243f6f1;
      7'h63: data_packed = 58'h0000000024168e0;
      7'h64: data_packed = 58'h0000000023ee08f;
      7'h65: data_packed = 58'h0000000023c5de9;
      7'h66: data_packed = 58'h00000000239e0d4;
      7'h67: data_packed = 58'h000000002376949;
      7'h68: data_packed = 58'h00000000234f72b;
      7'h69: data_packed = 58'h000000002328a6f;
      7'h6a: data_packed = 58'h000000002302301;
      7'h6b: data_packed = 58'h0000000022dc0d0;
      7'h6c: data_packed = 58'h0000000022b63cd;
      7'h6d: data_packed = 58'h000000002290be3;
      7'h6e: data_packed = 58'h00000000226b901;
      7'h6f: data_packed = 58'h000000002246b1e;
      7'h70: data_packed = 58'h000000002222221;
      7'h71: data_packed = 58'h0000000021fde01;
      7'h72: data_packed = 58'h0000000021d9eac;
      7'h73: data_packed = 58'h0000000021b6417;
      7'h74: data_packed = 58'h000000002192e2b;
      7'h75: data_packed = 58'h00000000216fcdf;
      7'h76: data_packed = 58'h00000000214d022;
      7'h77: data_packed = 58'h00000000212a7e6;
      7'h78: data_packed = 58'h000000002108420;
      7'h79: data_packed = 58'h0000000020e64c1;
      7'h7a: data_packed = 58'h0000000020c49ba;
      7'h7b: data_packed = 58'h0000000020a3300;
      7'h7c: data_packed = 58'h000000002082081;
      7'h7d: data_packed = 58'h000000002061238;
      7'h7e: data_packed = 58'h000000002040810;
      7'h7f: data_packed = 58'h000000002020203;
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
