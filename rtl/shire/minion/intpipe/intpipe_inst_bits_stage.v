// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// bits with MASK at 1 are flopped always
// bits with WR_MASK are flopped if insn is a branch/jal

// if trace encoder on, CSR (which can have illegal insn in wb
// and might need the bits to copy them in mtval) or if if there
//  is an xcpt, all bits are flopped regardless

`include "soc.vh"
module intpipe_inst_bits_stage #(
  parameter MASK       = 32'hFF,
  parameter BR_MASK    = 32'hFF
) (
  input                      clock,
  input                      enable,
  input                      is_csr,
  input                      is_br_jal,
  input                      te_enable,
  input                      xcpt,
  input [`INST_RANGE]        in,

  output logic [`INST_RANGE] out
);


  for (genvar i = 0; i < `INST_SIZE; i++) begin: FF_BITS
    logic en;

    assign en = enable &&
                (  MASK[i] ||                  // bits that are saved in all cases
                   BR_MASK[i] && is_br_jal ||  // bits that are saved for brances or jumps
                       te_enable || is_csr);       // if trace encoder on, or insn is CSR

    // and the FF
    `EN_FF(clock, en || xcpt, out[i], in[i])
  end
 

endmodule

   
