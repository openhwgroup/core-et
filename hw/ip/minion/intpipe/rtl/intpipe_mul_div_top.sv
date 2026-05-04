// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_mul_div_top -- Integer Multiplier / Divider Top contract wrapper.
//
// The externally visible intpipe-facing contract stays here so existing
// integration points and tests continue to instantiate `intpipe_mul_div_top`.
// The technology-selected implementation now lives behind `prim_mul_div`.

module intpipe_mul_div_top
  import minion_pkg::*;
  import dft_pkg::*;
(
  // System signals
  input  logic                  clk_i,
  input  logic                  rst_ni,
  // DFT
  input  dft_t                  dft_i,
  // Request port
  output logic                  req_ready,
  input  logic                  req_valid,
  input  logic                  req_valid_early,
  input  alu_func_e             req_fn,
  input  alu_func_e             req_fn_early,
  input  logic                  req_dw,
  input  logic [XregSize-1:0]   req_in1,
  input  logic [XregSize-1:0]   req_in2,
  input  minion_reg_dest_t      req_dest,
  input  logic                  kill,
  // Chicken bits
  input  logic                  chicken_bit_mul_div,
  // Response port
  input  logic                  resp_ready,
  output logic                  resp_valid,
  output logic                  resp_valid_early,
  output logic [XregSize-1:0]   resp_data,
  output minion_reg_dest_t      resp_dest
);

  prim_mul_div u_prim_mul_div (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .dft_i               (dft_i),
    .req_ready           (req_ready),
    .req_valid           (req_valid),
    .req_valid_early     (req_valid_early),
    .req_fn              (req_fn),
    .req_fn_early        (req_fn_early),
    .req_dw              (req_dw),
    .req_in1             (req_in1),
    .req_in2             (req_in2),
    .req_dest            (req_dest),
    .kill                (kill),
    .chicken_bit_mul_div (chicken_bit_mul_div),
    .resp_ready          (resp_ready),
    .resp_valid          (resp_valid),
    .resp_valid_early    (resp_valid_early),
    .resp_data           (resp_data),
    .resp_dest           (resp_dest)
  );

endmodule // intpipe_mul_div_top
