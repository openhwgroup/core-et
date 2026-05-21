// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncacheable_fcc
  import shire_uncached_pkg::*;
#(
  parameter int unsigned NumNeighP = NumNeigh
) (
  output logic [NumNeighP-1:0][MinPerNeigh-1:0] fcc_o,
  output logic [UcFccTargetSize-1:0]             fcc_target_o,
  input  uc_esr_fcc_enable_sigs_t                esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i
);

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_wdata;
  assign unused_wdata = ^esr_wdata_i[shire_esr_pkg::EsrApbDataWidth-1:(MinPerNeigh * NumNeighP)];
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    fcc_o        = '0;
    fcc_target_o = '0;
    if (|esr_enables_i) begin
      fcc_o = esr_wdata_i[(MinPerNeigh * NumNeighP)-1:0];
    end
    for (int unsigned i = 0; i < UcFccRegisters; i++) begin
      fcc_target_o |= {UcFccTargetSize{esr_enables_i[i]}} & i[UcFccTargetSize-1:0];
    end
  end

endmodule : uncacheable_fcc
