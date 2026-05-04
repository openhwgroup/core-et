// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_exp_rom_case_c1c2
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   hw/ip/minion/vpu/data/trans_rom/trans_exp_rom_case_c1c2.packed.hex
  logic [57:0]      data_packed;

  always_comb begin
    unique case (index_i[5:0])
      6'h0: data_packed = 58'h00cf74058b8c000;
      6'h1: data_packed = 58'h00cfa0059b04000;
      6'h2: data_packed = 58'h00cfd005aaa0000;
      6'h3: data_packed = 58'h00cffc05ba6c000;
      6'h4: data_packed = 58'h00d02805ca64000;
      6'h5: data_packed = 58'h00d05005da8c000;
      6'h6: data_packed = 58'h00d08005eadc000;
      6'h7: data_packed = 58'h00d0ac05fb5c000;
      6'h8: data_packed = 58'h00d0dc060c08000;
      6'h9: data_packed = 58'h00d108061ce4000;
      6'ha: data_packed = 58'h00d13c062dec000;
      6'hb: data_packed = 58'h00d168063f28000;
      6'hc: data_packed = 58'h00d19c065090000;
      6'hd: data_packed = 58'h00d1cc06622c000;
      6'he: data_packed = 58'h00d1fc0673f8000;
      6'hf: data_packed = 58'h00d2300685f4000;
      6'h10: data_packed = 58'h00d260069824000;
      6'h11: data_packed = 58'h00d29806aa84000;
      6'h12: data_packed = 58'h00d2c806bd1c000;
      6'h13: data_packed = 58'h00d2fc06cfe4000;
      6'h14: data_packed = 58'h00d33406e2e0000;
      6'h15: data_packed = 58'h00d36806f614000;
      6'h16: data_packed = 58'h00d39c07097c000;
      6'h17: data_packed = 58'h00d3d8071d18000;
      6'h18: data_packed = 58'h00d4100730ec000;
      6'h19: data_packed = 58'h00d4480744f8000;
      6'h1a: data_packed = 58'h00d48007593c000;
      6'h1b: data_packed = 58'h00d4b8076db8000;
      6'h1c: data_packed = 58'h00d4ec078270000;
      6'h1d: data_packed = 58'h00d528079760000;
      6'h1e: data_packed = 58'h00d56407ac88000;
      6'h1f: data_packed = 58'h00d5a007c1ec000;
      6'h20: data_packed = 58'h00d5dc07d78c000;
      6'h21: data_packed = 58'h00d61407ed6c000;
      6'h22: data_packed = 58'h00d65c080380000;
      6'h23: data_packed = 58'h00d6940819d8000;
      6'h24: data_packed = 58'h00d6d808306c000;
      6'h25: data_packed = 58'h00d718084740000;
      6'h26: data_packed = 58'h00d758085e54000;
      6'h27: data_packed = 58'h00d7980875a8000;
      6'h28: data_packed = 58'h00d7d4088d40000;
      6'h29: data_packed = 58'h00d81808a518000;
      6'h2a: data_packed = 58'h00d86408bd2c000;
      6'h2b: data_packed = 58'h00d8a008d58c000;
      6'h2c: data_packed = 58'h00d8e408ee2c000;
      6'h2d: data_packed = 58'h00d92c090710000;
      6'h2e: data_packed = 58'h00d97009203c000;
      6'h2f: data_packed = 58'h00d9b80939ac000;
      6'h30: data_packed = 58'h00da00095364000;
      6'h31: data_packed = 58'h00da44096d64000;
      6'h32: data_packed = 58'h00da880987b0000;
      6'h33: data_packed = 58'h00dae409a238000;
      6'h34: data_packed = 58'h00db2409bd18000;
      6'h35: data_packed = 58'h00db7409d83c000;
      6'h36: data_packed = 58'h00dbc409f3ac000;
      6'h37: data_packed = 58'h00dc0c0a0f6c000;
      6'h38: data_packed = 58'h00dc580a2b78000;
      6'h39: data_packed = 58'h00dcac0a47d0000;
      6'h3a: data_packed = 58'h00dcf40a647c000;
      6'h3b: data_packed = 58'h00dd4c0a8170000;
      6'h3c: data_packed = 58'h00dd9c0a9ebc000;
      6'h3d: data_packed = 58'h00ddf00abc54000;
      6'h3e: data_packed = 58'h00de400ada44000;
      6'h3f: data_packed = 58'h00de980af884000;
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
