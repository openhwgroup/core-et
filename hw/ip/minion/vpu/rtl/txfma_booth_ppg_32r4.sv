// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_booth_ppg_32r4
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0] mpcnd_i,
  input  logic [VpuDataSSz-1:0] mplr_i,
  input  logic                  mode_i,
  input  logic                  sign_i,
  output logic [VpuDataSSz-1:0] sxt_o,
  output logic [VpuDataSSz-1:0] cin_o,
  output logic [VpuDataSSz:0]   pp1_o,
  output logic [VpuDataSSz:0]   pp2_o,
  output logic [VpuDataSSz:0]   pp3_o,
  output logic [VpuDataSSz:0]   pp4_o,
  output logic [VpuDataSSz:0]   pp5_o,
  output logic [VpuDataSSz:0]   pp6_o,
  output logic [VpuDataSSz:0]   pp7_o,
  output logic [VpuDataSSz:0]   pp8_o,
  output logic [VpuDataSSz:0]   pp9_o,
  output logic [VpuDataSSz:0]   pp10_o,
  output logic [VpuDataSSz:0]   pp11_o,
  output logic [VpuDataSSz:0]   pp12_o,
  output logic [VpuDataSSz:0]   pp13_o,
  output logic [VpuDataSSz:0]   pp14_o,
  output logic [VpuDataSSz:0]   pp15_o,
  output logic [VpuDataSSz:0]   pp16_o,
  output logic [VpuDataSSz:0]   pp17_o
);

  localparam int unsigned NumNormPp = 16;
  localparam int unsigned NumPp     = 17;

  logic [34:0] ext_mplr;
  /* verilator lint_off UNUSEDSIGNAL */
  // The original helper builds 34-bit sideband vectors even though only the
  // subset consumed by `sxt_o`/`cin_o` is observed outside the block.
  logic [33:0] ext_sxt;
  logic [33:0] ext_cin;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [VpuDataSSz:0] pp_raw [NumPp-1:0];
  logic                ph;

  assign ph = mode_i;
  assign ext_mplr = {2'b0, mplr_i, 1'b0};

  for (genvar i = 0; i < NumPp; ++i) begin : gen_sxt_cin
    assign ext_cin[2*i] = ext_mplr[2*i+2] & !(ext_mplr[2*i+2] & ext_mplr[2*i+1] & ext_mplr[2*i]);
    assign ext_cin[2*i+1] = 1'b0;
    assign ext_sxt[2*i] =
        ~((ext_mplr[2*i+2] ^ (mpcnd_i[VpuDataSSz-1] & sign_i)) &
          ~((ext_mplr[2*i +: 3] == 3'b111) | (ext_mplr[2*i +: 3] == 3'b000)));
    assign ext_sxt[2*i+1] = 1'b1;
  end

  always_comb begin
    sxt_o = {ext_sxt[30:0], ext_cin[32]};
    cin_o = ext_cin[VpuDataSSz-1:0];

    if (ph) begin
      sxt_o = {ext_sxt[30:20], 2'b0, ext_cin[32:20], 6'h0};
      cin_o = {7'h0, ext_sxt[11:0], ext_cin[12:0]};
    end
  end

  txfma_booth_ppg_32r4_norm u_ppg1 (
    .mulcand_i (mpcnd_i),
    .r4input_i ({mplr_i[1:0], 1'b0}),
    .sign_i    (sign_i),
    .pp_o      (pp_raw[0])
  );

  for (genvar i = 1; i < NumNormPp; ++i) begin : gen_ppg_norm
    txfma_booth_ppg_32r4_norm u_ppg (
      .mulcand_i (mpcnd_i),
      .r4input_i (mplr_i[(2*i+1) -: 3]),
      .sign_i    (sign_i),
      .pp_o      (pp_raw[i])
    );
  end

  txfma_booth_ppg_32r4_msb u_ppg17 (
    .mulcand_i (mpcnd_i),
    .msb_i     (mplr_i[VpuDataSSz-1]),
    .sign_i    (sign_i),
    .pp_o      (pp_raw[16])
  );

  assign pp1_o  = pp_raw[0]  & {{20{~ph}}, 13'h1fff};
  assign pp2_o  = pp_raw[1]  & {{20{~ph}}, 13'h1fff};
  assign pp3_o  = pp_raw[2]  & {{20{~ph}}, 13'h1fff};
  assign pp4_o  = pp_raw[3]  & {{20{~ph}}, 13'h1fff};
  assign pp5_o  = pp_raw[4]  & {{20{~ph}}, 13'h1fff};
  assign pp6_o  = pp_raw[5]  & {{20{~ph}}, 13'h1fff};
  assign pp7_o  = pp_raw[6]  & {{20{~ph}}, 13'h1fff};
  assign pp8_o  = pp_raw[7];
  assign pp9_o  = pp_raw[8];
  assign pp10_o = pp_raw[9];
  assign pp11_o = pp_raw[10] & {15'h7fff, {18{~ph}}};
  assign pp12_o = pp_raw[11] & {15'h7fff, {18{~ph}}};
  assign pp13_o = pp_raw[12] & {15'h7fff, {18{~ph}}};
  assign pp14_o = pp_raw[13] & {15'h7fff, {18{~ph}}};
  assign pp15_o = pp_raw[14] & {15'h7fff, {18{~ph}}};
  assign pp16_o = pp_raw[15] & {15'h7fff, {18{~ph}}};
  assign pp17_o = pp_raw[16] & {15'h7fff, {18{~ph}}};

endmodule : txfma_booth_ppg_32r4
