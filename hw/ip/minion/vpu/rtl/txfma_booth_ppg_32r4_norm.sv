// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_booth_ppg_32r4_norm
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0] mulcand_i,
  input  logic [2:0]            r4input_i,
  input  logic                  sign_i,
  output logic [VpuDataSSz:0]   pp_o
);

  logic [4:0] booth;
  logic [VpuDataSSz:0] signed_mulcand;

  assign signed_mulcand = sign_i ? {mulcand_i[VpuDataSSz-1], mulcand_i} : {1'b0, mulcand_i};

  always_comb begin
    unique case (r4input_i)
      3'b000, 3'b111: booth = 5'b00001;
      3'b001, 3'b010: booth = 5'b00010;
      3'b101, 3'b110: booth = 5'b00100;
      3'b011:         booth = 5'b01000;
      3'b100:         booth = 5'b10000;
      default:        booth = 5'b00001;
    endcase
  end

  always_comb begin
    unique case (booth)
      5'b00001: pp_o = '0;
      5'b00010: pp_o = signed_mulcand;
      5'b00100: pp_o = ~signed_mulcand;
      5'b01000: pp_o = {mulcand_i, 1'b0};
      5'b10000: pp_o = ~{mulcand_i, 1'b0};
      default:  pp_o = '0;
    endcase
  end

endmodule : txfma_booth_ppg_32r4_norm
