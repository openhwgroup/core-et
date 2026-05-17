// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module pll_debug (
  input  logic       dft__pll_dbg_resetn,
  input  logic       dft__reset_byp,
  input  logic       dft__reset,
  input  logic       dft__tdr_tck,
  input  logic       dft__tdr_capturedr,
  input  logic       dft__tdr_updatedr,
  input  logic       dft__tdr_shiftdr,
  input  logic       dft__tdr_trstn,
  input  logic       dft__tdr_ena,
  input  logic       dft__tdr_tdi,
  output logic       dft__tdr_tdo,
  input  logic       rc_start_count,
  input  logic [14:0] clk_misc_obs_in,
  input  logic       yin_in,
  input  logic       yang_in,
  input  logic       xll_lock_1,
  input  logic       xll_lock_0,
  output logic       yin_out,
  output logic       yang_out
);
  assign dft__tdr_tdo = dft__tdr_ena & dft__tdr_shiftdr & dft__tdr_tdi & dft__tdr_trstn;
  assign yin_out = yin_in;
  assign yang_out = yang_in;

  logic unused_inputs;
  assign unused_inputs = ^{dft__pll_dbg_resetn, dft__reset_byp, dft__reset, dft__tdr_tck,
                           dft__tdr_capturedr, dft__tdr_updatedr, rc_start_count,
                           clk_misc_obs_in, xll_lock_1, xll_lock_0};
endmodule
