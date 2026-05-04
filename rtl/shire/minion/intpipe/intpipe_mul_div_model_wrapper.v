// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


`include "soc.vh"

module intpipe_mul_div_model_wrapper (/*AUTOARG*/
  // Outputs
  req_ready, resp_valid, resp_valid_early, resp_data, resp_dest,
  // Inputs
  clock_aon, reset, req_valid, req_valid_early, req_fn, req_fn_early,
  req_dw, req_in1, req_in2, req_dest, kill, chicken_bit_mul_div, resp_ready
);


  // System signals
  input  logic               clock_aon;
  input  logic               reset;
  // Request port
  output logic               req_ready;
  input  logic               req_valid;
  input  logic               req_valid_early;
  input  core_alu_func       req_fn;
  input  core_alu_func       req_fn_early;
  input  logic               req_dw;
  input  logic [`XREG_RANGE] req_in1;
  input  logic [`XREG_RANGE] req_in2;
  //input [`XREG_ADDR_RANGE]   req_dest_addr;           // destination register
  //input                      req_dest_thread_id;      // destination register
  input  minion_reg_dest     req_dest;
  input  logic               kill;
  // Chicken bits
  input  logic               chicken_bit_mul_div;
  // Response port
  input  logic               resp_ready;
  output logic               resp_valid;
  output logic               resp_valid_early;
  output logic [`XREG_RANGE] resp_data;
  //output [`XREG_ADDR_RANGE]  resp_dest_addr;          // response destination address
  //output                     resp_dest_thread_id;     // response destination thread id
  output minion_reg_dest     resp_dest;



   intpipe_mul_div
   mul_div_model
     ( .clock ( clock_aon ), .* );
   

   endmodule
